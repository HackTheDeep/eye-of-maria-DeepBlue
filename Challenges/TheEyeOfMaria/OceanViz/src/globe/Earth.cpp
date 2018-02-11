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

Earth::Earth()
	: mLoc( 0 ), mRadius( 250 )
{
	mLightDir = glm::normalize( vec3( 0.025f, 0.25f, 1.0f ) );

	// Load the textures for the Earth.
	auto fmt = gl::Texture2d::Format().wrap( GL_REPEAT ).mipmap().minFilter( GL_LINEAR_MIPMAP_LINEAR );
	mTexDiffuse = gl::Texture2d::create( loadImage( getAssetPath("earth/earthDiffuse.png" ) ), fmt );
	mTexNormal = gl::Texture2d::create( loadImage(getAssetPath("earth/earthNormal.png" ) ), fmt );
	mTexMask = gl::Texture2d::create( loadImage(getAssetPath("earth/earthMask.png" ) ), fmt );

	//mTexDiffuse = gl::Texture2d::create(loadImage(loadResource(RES_EARTHDIFFUSE)), fmt);
	//mTexNormal = gl::Texture2d::create(loadImage(loadResource(RES_EARTHNORMAL)), fmt);
	//mTexMask = gl::Texture2d::create(loadImage(loadResource(RES_EARTHMASK)), fmt);

	// Create the Earth mesh with a custom shader.
	auto earthShader = gl::GlslProg::create(loadAsset("earth/earth.vert"), loadAsset("earth/earth.frag"));
	earthShader->uniform( "texDiffuse", 0 );
	earthShader->uniform( "texNormal", 1 );
	earthShader->uniform( "texMask", 2 );
	mEarth = gl::Batch::create( geom::Sphere().radius( mRadius ).subdivisions( 120 ), earthShader );


	for (int i = 0; i < 10; i++) {

		float lat;
		float lon;

		float theta = glm::radians(90 - lat);
		float phi = glm::radians(180 + lon);

		vec3 p = vec3(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));

		mPoints.push_back(p);
	}


}

void Earth::update()
{
	mEarth->getGlslProg()->uniform( "lightDir", mLightDir );
	
}

void Earth::draw()
{
	gl::ScopedFaceCulling cull( true, GL_BACK );
	gl::ScopedTextureBind tex0( mTexDiffuse, 0 );
	gl::ScopedTextureBind tex1( mTexNormal, 1 );
	gl::ScopedTextureBind tex2( mTexMask, 2 );

	mEarth->draw();
}

