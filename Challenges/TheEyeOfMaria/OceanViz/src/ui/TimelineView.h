#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/views/BaseView.h"
#include "bluecadet/views/StrokedRectView.h"
#include "bluecadet/views/TouchView.h"

namespace amnh {

typedef std::shared_ptr<class TimelineView> TimelineViewRef;

class TimelineView : public bluecadet::views::TouchView {

public:
	TimelineView();
	~TimelineView();

	void setup();

protected:
	bluecadet::views::StrokedRectViewRef mTrack = nullptr;
	bluecadet::views::BaseViewRef mProgress = nullptr;
};
}
