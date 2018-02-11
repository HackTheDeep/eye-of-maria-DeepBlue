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

	//create earth model
	mEarth.setup();

	//setup the data points
	DataPointController::getInstance()->setup();
	
}

void MainController::update() {

	mEarth.update();
	DataPointController::getInstance()->update();
}

void MainController::draw() {


	gl::ScopedDepth	depth(true);
	mEarth.draw();
	DataPointController::getInstance()->draw();

}

}

