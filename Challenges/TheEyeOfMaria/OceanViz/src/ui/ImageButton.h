#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/views/BaseView.h"
#include "bluecadet/views/TouchView.h"

namespace amnh {

typedef std::shared_ptr<class ImageButton> ImageButtonRef;

class ImageButton : public bluecadet::views::TouchView {

public:
	ImageButton();
	~ImageButton();

	void  setTexture(const ci::gl::TextureRef texture);
	ci::gl::TextureRef  getTexture() const { return mTexture; }

protected:
	void draw() override;

	ci::gl::TextureRef mTexture = nullptr;
	ci::ColorA mMouseDownColor = ci::ColorA::gray(0.8f, 1.0f);
	ci::ColorA mMouseUpColor = ci::ColorA::gray(1.0, 1.0f);


};
}
