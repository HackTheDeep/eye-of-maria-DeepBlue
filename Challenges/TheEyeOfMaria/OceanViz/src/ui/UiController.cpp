#include "UiController.h"

#include "cinder/Log.h"

#include "data/OceanSettings.h"
#include "bluecadet/core/ScreenLayout.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;

namespace amnh {

UiController::UiController() {
}

UiController::~UiController() {
}

void amnh::UiController::setup() {
	mTimelineView = make_shared<TimelineView>();
	mTimelineView->setPosition(vec2(50, 40));
	mTimelineView->setup();
	addChild(mTimelineView);


}

}

