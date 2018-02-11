#include "MainController.h"

#include "cinder/Log.h"

#include "bluecadet/text/StyleManager.h"
#include "bluecadet/touch/TouchManager.h"
#include "bluecadet/utils/ImageManager.h"

#include "data/OceanSettings.h"
#include "data/DataManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::text;
using namespace bluecadet::views;
using namespace bluecadet::utils;

namespace amnh {

MainController::MainController() {
}

MainController::~MainController() {
}

void MainController::setup(bluecadet::views::BaseViewRef rootView) {
	StyleManager::getInstance()->setup(getAssetPath("fonts/styles.json"));
	ImageManager::getInstance()->loadAllFromDir(getAssetPath("ui"));

	// Front load data
	bool parseData = true;
	if (parseData) {
		DataManager::getInstance()->setup();
	}

	mUiController = make_shared<UiController>();
	mUiController->setup();
	rootView->addChild(mUiController);

	// Create the camera controller.
	//mPov = make_shared<POV>(ci::vec3(0.0f, 0.0f, 1000.0f), ci::vec3(0.0f, 0.0f, 0.0f));

	mCamera.setPerspective(60.0f, getWindowAspectRatio(), 0.1f, 20000.0f);
	mCamera.lookAt(vec3(0, 0, 800.0f), vec3(0));

	mArcballSphere = Sphere(vec3(0), 800.0f);

	mArcball = Arcball(&mCamera, mArcballSphere);
	mArcball.setQuat(glm::angleAxis(1.89068f, vec3(0.277f, -0.950f, -0.145f)));

	//create earth model
	mEarth.setup();

	//setup the points placeholders and VBO
	DataPointController::getInstance()->setup();

	//add the data points
	DataPointController::getInstance()->addDrifterData();
	DataPointController::getInstance()->addHurricaneData();

	
	//wire up signals
	getWindow()->getSignalMouseDown().connect(100, bind(&MainController::handleMouseDown, this, std::placeholders::_1));
	getWindow()->getSignalMouseDrag().connect(100, bind(&MainController::handleMouseDrag, this, std::placeholders::_1));
	getWindow()->getSignalMouseWheel().connect(bind(&MainController::handleMouseWheel, this, std::placeholders::_1));

	//OceanSettings::getInstance()->getParams()->addParam<quat>("Cam Rot", [&] (quat q) { mArcball.setQuat(q); }, [&] { return mArcball.getQuat(); });
}

void MainController::update() {
	//mPov->update();
	mCamera.setAspectRatio(getWindowAspectRatio());
	mEarth.update();
	DataPointController::getInstance()->update();
}

void MainController::draw() {
	gl::ScopedMatrices scopedMatrices;
	gl::ScopedDepth	depth(true);

	//mPov->applyMatrix();

	gl::setMatrices(mCamera);
	gl::rotate(mArcball.getQuat());

	mEarth.draw();

	DataPointController::getInstance()->draw();
}

inline void MainController::invertMouseCoords(ci::app::MouseEvent & event) {
	event.setPos(event.getWindow()->getSize() - event.getPos());
}

void MainController::handleMouseWheel(ci::app::MouseEvent event) {
	//mPov->adjustDist(event.getWheelIncrement() * -5.0f);
	mCamera.setEyePoint(mCamera.getEyePoint() + vec3(0, 0, event.getWheelIncrement() * -5.0f));
}

void MainController::handleMouseDown(ci::app::MouseEvent event) {
	if (bluecadet::touch::TouchManager::getInstance()->getNumTouchedViews() > 0) {
		return;
	}
	invertMouseCoords(event);
	mArcball.mouseDown(event);
}

void MainController::handleMouseDrag(ci::app::MouseEvent event) {
	if (bluecadet::touch::TouchManager::getInstance()->getNumTouchedViews() > 0) {
		return;
	}
	invertMouseCoords(event);
	mArcball.mouseDrag(event);
	/*static bool firstMouseMove = true;

	if (!firstMouseMove) {
		mLastMouse = mCurrentMouse;
	} else {
		mLastMouse = event.getPos();
		firstMouseMove = false;
	}

	mCurrentMouse = event.getPos();

	mPov->adjustAngle((mLastMouse.x - mCurrentMouse.x) * 0.01f, mCurrentMouse.y - (getWindowHeight() * 0.5f));*/
}

}

 