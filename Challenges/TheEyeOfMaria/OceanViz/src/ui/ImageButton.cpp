#include "ImageButton.h"

#include "cinder/Log.h"

#include "bluecadet/core/ScreenLayout.h"

#include "data/OceanSettings.h"
#include "data/TimelineManager.h"
#include "data/DataManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;

namespace amnh {

ImageButton::ImageButton() {
}

ImageButton::~ImageButton() {
}

void ImageButton::setTexture(const ci::gl::TextureRef texture) {
	mTexture = texture;

	if (!mTexture) {
		setSize(vec2(0));
		return;
	}

	setSize(mTexture->getSize());
}

void ImageButton::draw() {
	if (mTexture) {
		gl::ScopedColor scopedColor(getNumTouches() > 0 ? mMouseDownColor : mMouseUpColor);
		gl::draw(mTexture);
	}
}


}

