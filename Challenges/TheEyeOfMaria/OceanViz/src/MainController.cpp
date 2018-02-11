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

	mEarth.setup();
	
}

void MainController::update() {

	mEarth.update();

}

void MainController::draw() {


	mEarth.draw();

}

}

