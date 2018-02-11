#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/core/BaseApp.h"
#include "bluecadet/views/TouchView.h"
#include "bluecadet/text/StyleManager.h"

#include "globe/POV.h"

#include "MainController.h"
#include "globe/DataPoints/DataPointController.h"
#include "data/DataManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;
using namespace bluecadet::text;
using namespace bluecadet::touch;

using namespace amnh;



class OceanVizApp : public BaseApp {
public:
	static void prepareSettings(ci::app::App::Settings* settings);
	void setup() override;
	void update() override;
	void draw() override;

	void mouseMove(MouseEvent event);
	void mouseWheel(MouseEvent event);
	void keyDown(KeyEvent event) override;

protected:

	MainControllerRef	mMainController = nullptr;
	UiControllerRef		mUiController = nullptr;
	//camera and mouse manipulation
	POVRef              mPov;
	vec2				mLastMouse;
	vec2				mCurrentMouse;

};

void OceanVizApp::prepareSettings(ci::app::App::Settings* settings) {
	SettingsManager::setInstance(OceanSettings::getInstance());

	settings->setHighDensityDisplayEnabled(true);

	SettingsManager::getInstance()->setup(settings, ci::app::getAssetPath("settings.json"), [=](SettingsManager * manager) {
	});
}

void OceanVizApp::setup() {
	BaseApp::setup();

	StyleManager::getInstance()->setup(getAssetPath("fonts/styles.json"));

	// Front load data
	bool parseData = true;
	if (parseData) {
		DataManager::getInstance()->parseDrifterDirectoryData();
		DataManager::getInstance()->parseDrifterData();
		DataManager::getInstance()->parseHurricanData();
	}

	mUiController = make_shared<UiController>();
	mUiController->setup();
	getRootView()->addChild(mUiController);

	auto params = OceanSettings::getInstance()->getParams();
	params->setSize(ivec2(400, 500));

	// Create the camera controller.
	mPov = make_shared<POV>(this, ci::vec3(0.0f, 0.0f, 1000.0f), ci::vec3(0.0f, 0.0f, 0.0f));

	//Main Controller not in BC Views hierarchy
	mMainController = make_shared<MainController>();
	mMainController->setup();
}

void OceanVizApp::update() {
	BaseApp::update();

	mPov->update();

	mMainController->update();
}

void OceanVizApp::draw() {
	gl::clear();
	{
		gl::ScopedMatrices scopedMatrices;
		mPov->applyMatrix();
		mMainController->draw();
	}
	
	{
		BaseApp::draw(false);
	}
}


void OceanVizApp::mouseWheel(MouseEvent event) {
	mPov->adjustDist(event.getWheelIncrement() * -5.0f);
}

void OceanVizApp::mouseMove(MouseEvent event) {
	static bool firstMouseMove = true;

	if (!firstMouseMove) {
		mLastMouse = mCurrentMouse;
	} else {
		mLastMouse = event.getPos();
		firstMouseMove = false;
	}

	mCurrentMouse = event.getPos();

	mPov->adjustAngle((mLastMouse.x - mCurrentMouse.x) * 0.01f, mCurrentMouse.y - (getWindowHeight() * 0.5f));
}

void OceanVizApp::keyDown(KeyEvent event) {

	//just play through slides manually for now
	switch (event.getCode()) {

		case KeyEvent::KEY_RSHIFT:

			DataPointController::getInstance()->loadShader();
			DataPointController::getInstance()->replaceBatchShader();

			break;

		default:
			BaseApp::keyDown(event);
	}

}

// Make sure to pass a reference to prepareSettings to configure the app correctly. MSAA and other render options are optional.
CINDER_APP(OceanVizApp, RendererGl(RendererGl::Options().msaa(2)), OceanVizApp::prepareSettings);