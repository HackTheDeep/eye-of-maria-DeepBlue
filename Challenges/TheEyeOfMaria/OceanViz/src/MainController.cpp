#include "MainController.h"

#include "cinder/Log.h"
#include "data/OceanSettings.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;

namespace amnh {

MainController::MainController() {
}

MainController::~MainController() {
}

void MainController::setup() {
	setSize(ScreenLayout::getInstance()->getAppSize());
	mUiController = make_shared<UiController>();
	mUiController->setup();
	addChild(mUiController);

	auto params = OceanSettings::getInstance()->getParams();
	params->setSize(ivec2(400, 500));
}

}

