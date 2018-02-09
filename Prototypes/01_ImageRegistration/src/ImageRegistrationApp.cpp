#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/Arcball.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "CinderOpenCV.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageRegistrationApp : public App {
  public:
	void setup() override;
	
	void setupCam();
	void setupShaders();
	void loadImages();
	
	cv::Mat getWarpMatrix(const cv::Mat & imageToAlign, const cv::Mat & referenceImage, int warpMode);
	void warpImage(const cv::Mat & imageToWarp, const cv::Mat & targetImage, const cv::Mat & referenceImage, int warpMode);
	std::shared_ptr<std::thread> loadAndRegister(const vector<fs::path> & paths, int startIndex, int dir); // dir = -1/+1
	
	void resize() override;
	void mouseDown( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void mouseWheel( MouseEvent event ) override;
	
	void update() override;
	void draw() override;
	
protected:
	Arcball mArcball;
	CameraPersp mCamera;
	Sphere mArcballSphere;
	
	vector<gl::TextureRef> mInputTextures;
	vector<gl::TextureRef> mOutputTextures;
	
	gl::GlslProgRef mSliceShader = nullptr;
	
	float mThresholdMin = 0.25f;
	float mThresholdMax = 0.75f;
	vec3 mScale = vec3(0.001f, 0.001f, 1.0f);
	bool mDrawInput = true;
	bool mDrawOutput = true;
	int mNumIterations = 100;
	vector<int> mWarpModeValues = {cv::MOTION_TRANSLATION, cv::MOTION_EUCLIDEAN, cv::MOTION_AFFINE, cv::MOTION_HOMOGRAPHY};
	vector<string> mWarpModeLabels = {"Translation", "Euclidian", "Affine", "Homography"};
	int mWarpModeIndex = 0;
	params::InterfaceGlRef mParams;
};

void ImageRegistrationApp::setup()
{
	setupCam();
	setupShaders();
	
	mParams = params::InterfaceGl::create("Settings", vec2(200, 250));
	
	mParams->addParam("Iterations", &mNumIterations).group("Processing");
	mParams->addParam("Warp Mode", mWarpModeLabels, &mWarpModeIndex).group("Processing");
	mParams->addButton("Register Images", [=] { loadImages(); }, "group=Processing");
	
	mParams->addParam("Scale", &mScale).group("Rendering");
	mParams->addParam("Threshold Min", &mThresholdMin).step(1.0f / 255.0f).group("Rendering");
	mParams->addParam("Threshold Max", &mThresholdMax).step(1.0f / 255.0f).group("Rendering");
	mParams->addButton("Draw Input", [=] { mDrawInput = true; mDrawOutput = false; }, "group=Rendering");
	mParams->addButton("Draw Output", [=] { mDrawInput = false; mDrawOutput = true; }, "group=Rendering");
	mParams->addButton("Draw Both", [=] { mDrawInput = true; mDrawOutput = true; }, "group=Rendering");
}

void ImageRegistrationApp::setupCam() {
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	mCamera.setPerspective(45.0f, getWindowAspectRatio(), 0.1f, 1000.0f);
	mCamera.lookAt(vec3(0, 0, 2), vec3(0));
	
	mArcballSphere = Sphere(vec3(0), 1.0f);
	
	mArcball = Arcball(&mCamera, mArcballSphere);
}

cv::Mat ImageRegistrationApp::getWarpMatrix(const cv::Mat & imageToAlign, const cv::Mat & referenceImage, int warpMode) {
	// Registration code based on https://www.learnopencv.com/image-alignment-ecc-in-opencv-c-python/
	
	// Set a 2x3 or 3x3 warp matrix depending on the motion model.
	cv::Mat warpMatrix;
	
	// Initialize the matrix to identity
	if (warpMode == cv::MOTION_HOMOGRAPHY) {
		warpMatrix = cv::Mat::eye(3, 3, CV_32F);
	} else {
		warpMatrix = cv::Mat::eye(2, 3, CV_32F);
	}
	
	// Specify the threshold of the increment
	// in the correlation coefficient between two iterations
	double terminationEps = 1e-10;
	
	// Define termination criteria
	cv::TermCriteria criteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, mNumIterations, terminationEps);
	
	// Run the ECC algorithm. The results are stored in warp_matrix.
	try {
		cv::findTransformECC(
							 referenceImage,
							 imageToAlign,
							 warpMatrix,
							 warpMode,
							 criteria
							 );
	} catch (const exception & e) {
		CI_LOG_E("Could not find full transform image (" << e.what() << ")");
	}
	
	return warpMatrix;
}

void ImageRegistrationApp::warpImage(const cv::Mat & imageToWarp, const cv::Mat & targetImage, const cv::Mat & referenceImage, int warpMode) {
	cv::Mat warpMatrix = getWarpMatrix(imageToWarp, referenceImage, warpMode);
	
	if (warpMode != cv::MOTION_HOMOGRAPHY) {
		cv::warpAffine(imageToWarp, targetImage, warpMatrix, referenceImage.size(), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP);
	} else {
		cv::warpPerspective(imageToWarp, targetImage, warpMatrix, referenceImage.size(), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP);
	}
}

void ImageRegistrationApp::loadImages() {
	static const set<string> supportedExtensions = {".tif", ".png", ".jpg"};
	const ci::fs::path dir = getAssetPath("Ancyromonas-cA-TEM-320");
	
	vector<fs::path> paths;
	
	for (fs::directory_iterator it(dir); it != fs::directory_iterator(); ++it) {
		const fs::path filePath = it->path();
		const string extension = filePath.extension().string();
		if (supportedExtensions.count(extension) != 0) {
			paths.push_back(filePath);
		}
	}
	
	// sort files
	std::sort(paths.begin(), paths.end(), [](fs::path const & a, fs::path const & b) {
		return a.filename().string() < b.filename().string();
	});
	
	mInputTextures.clear();
	mOutputTextures.clear();
	
	mInputTextures.resize(paths.size());
	mOutputTextures.resize(paths.size());
	
	auto threadA = loadAndRegister(paths, paths.size() / 2, -1);
	auto threadB = loadAndRegister(paths, paths.size() / 2 + 1, 1);
	
	threadA->join();
	threadB->join();
}

std::shared_ptr<std::thread> ImageRegistrationApp::loadAndRegister(const vector<fs::path> & paths, int startIndex, int dir) {
	auto context = gl::Context::create(gl::context());
	return make_shared<thread>([=] {
		cv::Mat input;
		cv::Mat output;
		cv::Mat prevOutput;
		
		ci::ThreadSetup threadSetup;
		context->makeCurrent();
		
		gl::Texture::Format texFormat;
		texFormat
			.mipmap(true)
			.minFilter(GL_LINEAR_MIPMAP_LINEAR)
			.magFilter(GL_LINEAR);
		
		for (int i = startIndex; i >= 0 && i < paths.size(); i += dir) {
			const auto & filePath = paths[i];
			
			try {
				CI_LOG_D("Loading image " + filePath.filename().string());
				
				Surface surface(loadImage(filePath));
				input = toOcv(surface, CV_8U);
				
				if (i != startIndex) {
					warpImage(input, output, prevOutput, mWarpModeValues[mWarpModeIndex]);
					
				} else {
					output = cv::Mat(input);
				}
				
				mInputTextures[i] = gl::Texture::create(fromOcv(input), texFormat);
				mOutputTextures[i] = gl::Texture::create(fromOcv(output), texFormat);
				
				// create fence after all gpu commands
				auto fence = gl::Sync::create();
				fence->clientWaitSync();
				
				prevOutput = cv::Mat(output);
				
			} catch (const Exception & e) {
				CI_LOG_EXCEPTION("Can't load image at " + filePath.string(), e);
			} catch (const exception & e) {
				CI_LOG_E("Can't load image at " + filePath.string() + " (" + e.what() + ")");
			}
		}
	});
}

void ImageRegistrationApp::resize() {
	mCamera.setAspectRatio(getWindowAspectRatio());
}

void ImageRegistrationApp::mouseDown(MouseEvent event) {
	mArcball.mouseDown(event);
}

void ImageRegistrationApp::mouseDrag(MouseEvent event) {
	mArcball.mouseDrag(event);
}

void ImageRegistrationApp::mouseWheel(MouseEvent event) {
	mCamera.setEyePoint(mCamera.getEyePoint() + vec3(0, 0, event.getWheelIncrement()));
}

void ImageRegistrationApp::update() {
}

void ImageRegistrationApp::setupShaders() {
	mSliceShader = gl::GlslProg::create(gl::GlslProg::Format()
										.vertex(CI_GLSL(150,
														uniform mat4 ciModelViewProjection;
														in vec4 ciColor;
														in vec4 ciPosition;
														in vec2 ciTexCoord0;
														out vec2 vTexCoord0;
														out vec4 vColor;
														void main(void) {
															vTexCoord0 = ciTexCoord0;
															vColor = ciColor;
															gl_Position	= ciModelViewProjection * ciPosition;
														}
														))
										.fragment(CI_GLSL(150,
														  uniform sampler2D uTex0;
														  uniform float uThresholdMin = 0.0;
														  uniform float uThresholdMax = 1.0;
														  in vec4 vColor;
														  in vec2 vTexCoord0;
														  out vec4 oColor;
														  
														  void main( void ) {
															  vec4 texColor = texture(uTex0, vTexCoord0);
															  
															  // avg rgb
															  float brightness = length(texColor.rgb);
															  
															  // normalize
															  brightness = (brightness - uThresholdMin) / (uThresholdMax - uThresholdMin);
															  
															  if (brightness < 0.0 || brightness > 1.0) {
																  discard;
															  }
															  
															  // increase contrast
															  float alpha = brightness;
															  float rgb = clamp(pow(brightness + 0.5, 10), 0.0f, 1.0f);
															  
															  oColor = vec4(vec3(rgb), alpha) * vColor;
														  }
														  )));
}

void ImageRegistrationApp::draw()
{
	gl::clear(ColorA::black());
	gl::color(ColorA::white());
	gl::setMatrices(mCamera);
	gl::rotate(mArcball.getQuat());
	
//	gl::ScopedBlendPremult scopedPremult;
	gl::ScopedBlendAlpha scopedAlphaBlend;
	gl::ScopedDepthTest scopedDepthTest(true);
	gl::ScopedGlslProg scopedShader(mSliceShader);
	
	{
		
		mSliceShader->uniform("uThresholdMin", mThresholdMin);
		mSliceShader->uniform("uThresholdMax", mThresholdMax);
		
		vec2 texSize(512, 512);
		Rectf texRect(-texSize * 0.5f, texSize * 0.5f);
		
		for (int i = 0; i < mInputTextures.size(); ++i) {
			gl::ScopedMatrices scopedMatrices;
			
			auto inputTex = mInputTextures[i];
			auto outputTex = mOutputTextures[i];
			float normIdx = (float)i / (float)mInputTextures.size();
			vec3 pos = mScale * vec3(0, 0, normIdx - 0.5f);
			
			gl::translate(pos);
			gl::scale(mScale);
			
			if (mDrawOutput) {
				gl::ScopedMatrices scopedMatrices;
				gl::ScopedColor scopedColor(ColorA(1, 1, 1, 1));
				gl::ScopedTextureBind scopedTex(outputTex);
				gl::drawSolidRect(texRect);
			}
			
			if (mDrawInput) {
				gl::ScopedMatrices scopedMatrices;
				gl::ScopedColor scopedColor(ColorA(0, 1, 1, 1));
				gl::ScopedTextureBind scopedTex(inputTex);
				gl::drawSolidRect(texRect);
			}
		}
	}
	
	mParams->draw();
}

CINDER_APP(ImageRegistrationApp, RendererGl, [] (ci::app::App::Settings * settings) {
	settings->setWindowSize(1270, 720);
	settings->setMultiTouchEnabled(false);
})
