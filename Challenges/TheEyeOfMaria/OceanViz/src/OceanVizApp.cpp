#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/core/BaseApp.h"
#include "bluecadet/views/TouchView.h"

#include "globe/POV.h"

#include "MainController.h"
#include "globe/DataPoints/DataPointController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;
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

public:

	//camera and mouse manipulation
	POVRef              mPov;
	vec2              mLastMouse;
	vec2              mCurrentMouse;

	void lateSetup() override;

protected:

	MainControllerRef mMainController;
	UiControllerRef mUiController = nullptr;

};

void OceanVizApp::prepareSettings(ci::app::App::Settings* settings) {
	SettingsManager::setInstance(OceanSettings::getInstance());

	settings->setHighDensityDisplayEnabled(true);

	SettingsManager::getInstance()->setup(settings, ci::app::getAssetPath("settings.json"), [=](SettingsManager * manager) {
		// Optional: Override json defaults at runtime
		manager->mFullscreen = false;
		manager->mWindowSize = ivec2(1280, 720);
	});


}

void OceanVizApp::setup() {
	BaseApp::setup();

	// Optional: configure your root view
	getRootView()->setBackgroundColor(Color::gray(0.0f));

	// Sample content
	//auto button = make_shared<TouchView>();
	//button->setPosition(vec2(400, 300));
	//button->setSize(vec2(200, 100));
	//button->setBackgroundColor(Color(1, 0, 0));
	//button->getSignalTapped().connect([=](bluecadet::touch::TouchEvent e) { CI_LOG_I("Button tapped"); });
	//getRootView()->addChild(button);

	getRootView()->setBackgroundColor(Color::gray(0));

	mUiController = make_shared<UiController>();
	mUiController->setup();
	getRootView()->addChild(mUiController);

	// Create the camera controller.

	mPov = make_shared<POV>(this, ci::vec3(0.0f, 0.0f, 1000.0f), ci::vec3(0.0f, 0.0f, 0.0f));


	mMainController = make_shared<MainController>();
	mMainController->setup();
	
	// do lightweight setup here
}

void OceanVizApp::lateSetup() {
	BaseApp::lateSetup();

	// do heavy lifting setup here




}

void OceanVizApp::update() {
	BaseApp::update();

	if(mPov != nullptr) mPov->update();
	
	mMainController->update();

}

void OceanVizApp::draw() {
	BaseApp::draw();

	mMainController->draw();

}


void OceanVizApp::mouseWheel(MouseEvent event)
{
	mPov->adjustDist(event.getWheelIncrement() * -5.0f);
}

void OceanVizApp::mouseMove(MouseEvent event)
{
	static bool firstMouseMove = true;

	if (!firstMouseMove) {
		mLastMouse = mCurrentMouse;
	}
	else {
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
CINDER_APP(OceanVizApp, RendererGl(RendererGl::Options().msaa(1)), OceanVizApp::prepareSettings);