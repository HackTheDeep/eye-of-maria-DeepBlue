#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/core/BaseApp.h"
#include "bluecadet/views/TouchView.h"

#include "data/OceanSettings.h"
#include "MainController.h"
#include "data/DataManager.h"

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
	void lateSetup() override;

protected:
	MainControllerRef mMainController = nullptr;
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

	// do lightweight setup here
}

void OceanVizApp::lateSetup() {
	BaseApp::lateSetup();

	// do heavy lifting setup here

	// Front load data
	bool parseData = true;
	if (parseData) {
		DataManager::getInstance()->parseDrifterDirectoryData();
		DataManager::getInstance()->parseDrifterData();
		DataManager::getInstance()->parseHurricanData();
	}

	getRootView()->setBackgroundColor(Color::gray(0));
	mMainController = make_shared<MainController>();
	getRootView()->addChild(mMainController);
	mMainController->setup();

}

void OceanVizApp::update() {
	BaseApp::update();
}

void OceanVizApp::draw() {
	BaseApp::draw();
}

// Make sure to pass a reference to prepareSettings to configure the app correctly. MSAA and other render options are optional.
CINDER_APP(OceanVizApp, RendererGl(RendererGl::Options().msaa(1)), OceanVizApp::prepareSettings);