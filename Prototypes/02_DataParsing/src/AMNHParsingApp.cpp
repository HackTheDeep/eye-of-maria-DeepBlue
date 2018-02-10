#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "bluecadet/core/BaseApp.h"
#include "Parser.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace bluecadet::core;
using namespace amnh;

class AMNHParsingApp : public BaseApp {
  public:
		static void prepareSettings(ci::app::App::Settings* settings);
		void setup() override;
		void mouseDown( MouseEvent event ) override;
		void update() override;
		void draw() override;
};

void AMNHParsingApp::prepareSettings(ci::app::App::Settings* settings) {
	SettingsManager::getInstance()->setup(settings, ci::app::getAssetPath("settings.json"), [](SettingsManager * manager) {});
}

void AMNHParsingApp::setup()
{
	BaseApp::setup();
	BaseApp::addTouchSimulatorParams();
}

void AMNHParsingApp::mouseDown( MouseEvent event )
{
	Parser::getInstance()->parseHurricaneData();
	Parser::getInstance()->parseDrifterData();

}

void AMNHParsingApp::update()
{
}

void AMNHParsingApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP(AMNHParsingApp, RendererGl(RendererGl::Options().msaa(4).stencil(true)), AMNHParsingApp::prepareSettings);
