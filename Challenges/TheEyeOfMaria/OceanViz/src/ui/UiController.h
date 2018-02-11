#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/views/BaseView.h"
#include "TimelineView.h"

namespace amnh {

typedef std::shared_ptr<class UiController> UiControllerRef;

class UiController : public bluecadet::views::BaseView {

public:
	UiController();
	~UiController();

	void setup();

protected:
	TimelineViewRef mTimelineView = nullptr;
};
}
