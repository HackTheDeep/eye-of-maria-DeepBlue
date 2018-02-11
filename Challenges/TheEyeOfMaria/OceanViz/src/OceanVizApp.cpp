#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/core/BaseApp.h"
#include "bluecadet/views/TouchView.h"

#include "data/OceanSettings.h"

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
	getRootView()->setBackgroundColor(Color::gray(0.5f));

	// Sample content
	auto button = make_shared<TouchView>();
	button->setPosition(vec2(400, 300));
	button->setSize(vec2(200, 100));
	button->setBackgroundColor(Color(1, 0, 0));
	button->getSignalTapped().connect([=](bluecadet::touch::TouchEvent e) { CI_LOG_I("Button tapped"); });
	getRootView()->addChild(button);
}

void OceanVizApp::update() {
	BaseApp::update();
}

void OceanVizApp::draw() {
	BaseApp::draw();
}

// Make sure to pass a reference to prepareSettings to configure the app correctly. MSAA and other render options are optional.
CINDER_APP(OceanVizApp, RendererGl(RendererGl::Options().msaa(1)), OceanVizApp::prepareSettings);