#include "MainController.h"

#include "cinder/Log.h"

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
	addChild(mUiController);

	mUiController->setup();
}

}

