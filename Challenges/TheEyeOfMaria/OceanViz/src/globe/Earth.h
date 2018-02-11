#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

class Earth {
public:
	Earth();

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
	
	std::vector<vec3>	mPoints;

};