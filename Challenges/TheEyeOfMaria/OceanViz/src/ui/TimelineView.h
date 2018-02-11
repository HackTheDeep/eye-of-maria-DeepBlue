#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/views/BaseView.h"
#include "bluecadet/views/StrokedRectView.h"
#include "bluecadet/views/TextView.h"
#include "bluecadet/views/TouchView.h"

namespace amnh {

typedef std::shared_ptr<class TimelineView> TimelineViewRef;

class TimelineView : public bluecadet::views::BaseView {

public:
	TimelineView();
	~TimelineView();

	void setup();
	void update(const double deltaTime) override;

	void play();
	void pause();
	void togglePause();
	void faster();
	void slower();

protected:
	bluecadet::views::TouchViewRef mTrack= nullptr;
	bluecadet::views::BaseViewRef mProgress = nullptr;

	bluecadet::views::TextViewRef mCurrentTimeLabel = nullptr;
	bluecadet::views::TextViewRef mCurrentTime = nullptr;
	bluecadet::views::TextViewRef mSpeed = nullptr;

	bluecadet::views::TextViewRef mStartTime = nullptr;
	bluecadet::views::TextViewRef mEndTime = nullptr;


};
}
