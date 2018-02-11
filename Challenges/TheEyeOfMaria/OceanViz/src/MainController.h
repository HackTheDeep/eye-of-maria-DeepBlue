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
#include "globe/POV.h"

namespace amnh {

typedef std::shared_ptr<class MainController> MainControllerRef;

class MainController {

public:
	MainController();
	~MainController();

	void setup(bluecadet::views::BaseViewRef rootView);
	void update();
	void draw() ;

protected:
	inline void invertMouseCoords(ci::app::MouseEvent & event);

	void handleMouseDown(ci::app::MouseEvent event);
	void handleMouseDrag(ci::app::MouseEvent event);
	void handleMouseWheel(ci::app::MouseEvent event);

	ci::Arcball			mArcball;
	ci::CameraPersp		mCamera;
	ci::Sphere			mArcballSphere;

	Earth				mEarth;
	//POVRef              mPov = nullptr;
	UiControllerRef		mUiController = nullptr;

	//camera and mouse manipulation
	//ci::vec2			mLastMouse;
	//ci::vec2			mCurrentMouse;

};
}
