/*
 *  Earth.cpp
 *  Earthquake
 *
 *  Created by Robert Hodgin on 6/30/09.
 *  Copyright 2009 The Barbarian Group. All rights reserved.
 *
 */

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Earth.h"

using namespace ci;
using namespace ci::app;
using namespace amnh;

Earth::Earth(){
}

void Earth::setup(){

	mLoc = vec3(0);
	mRadius = 850.0f;
	mLightDir = glm::normalize(vec3(0.025f, 0.25f, 1.0f));

	// Load the textures for the Earth.
	auto fmt = gl::Texture2d::Format().wrap(GL_REPEAT).mipmap().minFilter(GL_LINEAR_MIPMAP_LINEAR);
	mTexDiffuse = gl::Texture2d::create(loadImage(getAssetPath("earth/earthDiffuse.png")), fmt);
	mTexNormal = gl::Texture2d::create(loadImage(getAssetPath("earth/earthNormal.png")), fmt);
	mTexMask = gl::Texture2d::create(loadImage(getAssetPath("earth/earthMask.png")), fmt);

	// Create the Earth mesh with a custom shader.

	mEarthSphere = Sphere(vec3(), mRadius);
	mEarthSphereGeom = geom::Sphere(mEarthSphere).subdivisions(120);

	auto earthShader = gl::GlslProg::create(loadAsset("earth/earth.vert"), loadAsset("earth/earth.frag"));
	earthShader->uniform("texDiffuse", 0);
	earthShader->uniform("texNormal", 1);
	earthShader->uniform("texMask", 2);
	mEarth = gl::Batch::create(mEarthSphereGeom, earthShader);

}


void Earth::update()
{
	mEarth->getGlslProg()->uniform( "lightDir", mLightDir );
}

void Earth::draw()
{
	gl::ScopedFaceCulling	cull( true, GL_BACK );
	gl::ScopedTextureBind	tex0( mTexDiffuse, 0 );
	gl::ScopedTextureBind	tex1( mTexNormal, 1 );
	gl::ScopedTextureBind	tex2( mTexMask, 2 );

	mEarth->draw();

}

void Earth::loadShader() {
	auto earthShader = gl::GlslProg::create(loadAsset("earth/earth.vert"), loadAsset("earth/earth.frag"));
	earthShader->uniform("texDiffuse", 0);
	earthShader->uniform("texNormal", 1);
	earthShader->uniform("texMask", 2);
	if (mEarth) {
		mEarth->replaceGlslProg(earthShader);
	} else {
		mEarth = gl::Batch::create(geom::Sphere().radius(mRadius).subdivisions(120), earthShader);
	}
}

