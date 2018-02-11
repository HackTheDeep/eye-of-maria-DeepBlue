#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/core/BaseApp.h"
#include "bluecadet/views/TouchView.h"

#include "MainController.h"


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
	void keyDown(KeyEvent event) override;

protected:
	MainControllerRef	mMainController = nullptr;

};

void OceanVizApp::prepareSettings(ci::app::App::Settings* settings) {
	SettingsManager::setInstance(OceanSettings::getInstance());

	settings->setHighDensityDisplayEnabled(true);

	SettingsManager::getInstance()->setup(settings, ci::app::getAssetPath("settings.json"), [=](SettingsManager * manager) {
	});
}

void OceanVizApp::setup() {
	BaseApp::setup();
	// Configure params
	auto params = OceanSettings::getInstance()->getParams();
	params->setSize(ivec2(400, 500));

	// Main Controller not in BC Views hierarchy
	mMainController = make_shared<MainController>();
	mMainController->setup(getRootView());
}

void OceanVizApp::update() {
	BaseApp::update();
	mMainController->update();
}

void OceanVizApp::draw() {
	gl::clear();
	mMainController->draw();
	BaseApp::draw(false);
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