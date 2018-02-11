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

	mDisplayPallet = make_shared<DisplayPallet>();
	mDisplayPallet->setPosition(vec2(50, mTimelineView->getPosition().value().y + mTimelineView->getHeight() + 20));
	mDisplayPallet->setup();

	addChild(mTimelineView);
	addChild(mDisplayPallet);

}

}

