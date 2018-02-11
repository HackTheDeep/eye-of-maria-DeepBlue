#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/views/BaseView.h"
#include "bluecadet/views/StrokedRectView.h"
#include "bluecadet/views/TextView.h"
#include "bluecadet/views/TouchView.h"

namespace amnh {

	typedef std::shared_ptr<class DisplayPallet> DisplayPalletRef;

	class DisplayPallet : public bluecadet::views::BaseView {

	public:
		DisplayPallet();
		~DisplayPallet();

		void setup();

	protected:
		bluecadet::views::TouchViewRef mHeaderButton;
		bluecadet::views::TouchViewRef mDrifterButton;
		bluecadet::views::TouchViewRef mFloatsButton;
		bluecadet::views::TouchViewRef mHurricaneButton;


	};
}
