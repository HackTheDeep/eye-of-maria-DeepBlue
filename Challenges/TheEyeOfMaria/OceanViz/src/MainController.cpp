#include "MainController.h"

#include "cinder/Log.h"

#include "bluecadet/text/StyleManager.h"

#include "data/OceanSettings.h"
#include "data/DataManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::text;
using namespace bluecadet::views;

namespace amnh {

MainController::MainController() {
}

MainController::~MainController() {
}

void MainController::setup(bluecadet::views::BaseViewRef rootView) {
	StyleManager::getInstance()->setup(getAssetPath("fonts/styles.json"));

	// Front load data
	bool parseData = true;
	if (parseData) {
		DataManager::getInstance()->parseDrifterDirectoryData();
		DataManager::getInstance()->parseDrifterData();
	}

	mUiController = make_shared<UiController>();
	mUiController->setup();
	rootView->addChild(mUiController);

	// Create the camera controller.
	mPov = make_shared<POV>(ci::vec3(0.0f, 0.0f, 1000.0f), ci::vec3(0.0f, 0.0f, 0.0f));

	//create earth model
	mEarth.setup();

	//setup the data points
	DataPointController::getInstance()->setup();
	
	//wire up signals
	getWindow()->getSignalMouseMove().connect(bind(&MainController::handleMouseMove, this, std::placeholders::_1));
	getWindow()->getSignalMouseWheel().connect(bind(&MainController::handleMouseWheel, this, std::placeholders::_1));
}

void MainController::update() {
	mPov->update();
	mEarth.update();
	DataPointController::getInstance()->update();
}

void MainController::draw() {
	gl::ScopedMatrices scopedMatrices;
	gl::ScopedDepth	depth(true);

	mPov->applyMatrix();

	mEarth.draw();

	DataPointController::getInstance()->draw();
}

void MainController::handleMouseWheel(const ci::app::MouseEvent & event) {
	mPov->adjustDist(event.getWheelIncrement() * -5.0f);
}

void MainController::handleMouseMove(const ci::app::MouseEvent & event) {
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

}

