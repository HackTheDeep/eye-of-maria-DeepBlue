#include "TimelineView.h"

#include "cinder/Log.h"

#include "bluecadet/core/ScreenLayout.h"

#include "data/OceanSettings.h"
#include "data/TimelineManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;

namespace amnh {

TimelineView::TimelineView() {
}

TimelineView::~TimelineView() {
}

void TimelineView::setup() {
	setSize(vec2(350, 115));

	TimelineManager::getInstance()->setup();

	auto bg = make_shared<StrokedRectView>();
	bg->setStrokeColor(Color::white());
	bg->setStrokeWidth(1);
	bg->setBackgroundColor(ColorA::gray(0, 0.5f));
	bg->setSize(getSize());
	addChild(bg);

	mTrack = make_shared<TouchView>();
	mTrack->setSize(vec2(310, 15));
	mTrack->setPosition(vec2(20, 30));
	addChild(mTrack);

	mProgress = make_shared<BaseView>();
	mProgress->setBackgroundColor(ColorA(0.239, 0.349, 0.6, 0.75));
	mProgress->setSize(mTrack->getSize());
	mTrack->addChild(mProgress);

	auto trackBg = make_shared<StrokedRectView>();
	trackBg->setStrokeColor(Color::white());
	trackBg->setStrokeWidth(1);
	trackBg->setSize(mTrack->getSize());
	mTrack->addChild(trackBg);

	mTrack->getSignalTouchMoved().connect([=] (const bluecadet::touch::TouchEvent & event) {
		vec2 posInTrack = mTrack->convertGlobalToLocal(event.globalPosition);
		float dragDist = clamp(posInTrack.x, 0.0f, mTrack->getWidth());
		float progress = dragDist / mTrack->getWidth();

		mProgress->setScale(vec2(progress, 1.0f));

		TimelineManager::getInstance()->setNormProgress(progress);
	});

	// params
	auto params = OceanSettings::getInstance()->getParams();
	auto timelineManager = TimelineManager::getInstance();
	params->addParam<bool>("Play", [=](bool v) { timelineManager->setPaused(!v); }, [=] { return !timelineManager->isPaused(); }).group("Timeline");
	params->addButton("Faster", [=] { if (timelineManager->getSpeed() < 8.0f) timelineManager->animateSpeedTo(timelineManager->getSpeed() * 2.0f); });
	params->addButton("Slower", [=] { if (timelineManager->getSpeed() > 0.125f) timelineManager->animateSpeedTo(timelineManager->getSpeed() * 0.5f); });
}

void TimelineView::update(double deltaTime) {
	if (mTrack->getNumTouches() == 0) {
		// update progress if not dragging
		TimelineManager::getInstance()->update(deltaTime);
		float progress = TimelineManager::getInstance()->getNormProgress();
		mProgress->setScale(vec2(progress, 1.0f));
	}
}


}

