#pragma once
#include "cinder/Vector.h"
#include "cinder/Camera.h"
#include "cinder/app/App.h"

typedef std::shared_ptr<class POV> POVRef;

class POV {
public:
	POV() {}
	POV( ci::vec3 aEye, ci::vec3 aCenter );

	void update();

	void adjustAngle( float aAngleDelta, float aPitch );
	void adjustDist( float aDist );
	void setEye( ci::vec3 v );
	void setCenter( ci::vec3 v );

	void applyMatrix();

public:
	ci::CameraPersp  mCam;
	ci::vec3         mEye;
	ci::vec3         mCenter;
	ci::vec3         mEyeNormal;

	float            mAngle, mAngleDest;
	float            mDist, mDistDest;

private:
	float            mAspectRatio;
	float            mFOV;


};
