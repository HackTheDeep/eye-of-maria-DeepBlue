#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/Arcball.h"
#include "cinder/Camera.h"
#include "cinder/Sphere.h"

#include "bluecadet/core/ScreenLayout.h"
#include "bluecadet/views/BaseView.h"
#include "ui/UiController.h"

#include "data/OceanSettings.h"
#include "globe/Earth.h"

namespace amnh {

typedef std::shared_ptr<class MainController> MainControllerRef;

class MainController {

public:
	MainController();
	~MainController();

	void setup(bluecadet::views::BaseViewRef rootView);
	void update();
	void draw() ;

	Earth & getEarth() { return mEarth; }

protected:
	void finishSetup(); // called once data is loaded

	inline void invertMouseCoords(ci::app::MouseEvent & event);

	void handleMouseDown(ci::app::MouseEvent event);
	void handleMouseDrag(ci::app::MouseEvent event);
	void handleMouseWheel(ci::app::MouseEvent event);

	bluecadet::views::BaseViewRef mRootView;
	ci::Arcball			mArcball;
	ci::CameraPersp		mCamera;

	Earth				mEarth;
	UiControllerRef		mUiController = nullptr;

	bool				bEnableDepthTest = true;
	void				toggleDepthTest() { bEnableDepthTest = !bEnableDepthTest; };

	//camera and mouse manipulation
	//ci::vec2			mLastMouse;
	//ci::vec2			mCurrentMouse;

};
}
