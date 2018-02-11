#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/CinderGlm.h"
#include "cinder/CinderMath.h"
#include "cinder/Easing.h"

#include "DataPoints/DataPoint.h"
#include "DataPoints/DataPointController.h"

class Earth {
public:
	Earth();

	void setup();
	void update();
	void draw();

public:

	float mRadius;

	ci::vec3 mLoc;
	ci::vec3 mLightDir;

	ci::gl::Texture2dRef mTexDiffuse;
	ci::gl::Texture2dRef mTexNormal;
	ci::gl::Texture2dRef mTexMask;

	ci::gl::BatchRef     mEarth;
	

};