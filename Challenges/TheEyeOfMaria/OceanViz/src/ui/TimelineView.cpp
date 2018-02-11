#include "TimelineView.h"

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

TimelineView::TimelineView() {
}

TimelineView::~TimelineView() {
}

void TimelineView::setup() {
	setSize(vec2(350, 115));

	auto timelineManager = TimelineManager::getInstance();
	auto data = DataManager::getInstance();

	timelineManager->setup();
	timelineManager->setStartTime(data->getMinTimestamp());
	timelineManager->setEndTime(data->getMaxTimestamp());

	auto bg = make_shared<StrokedRectView>();
	bg->setStrokeColor(Color::white());
	bg->setStrokeWidth(1);
	bg->setBackgroundColor(ColorA::gray(0, 0.5f));
	bg->setSize(getSize());
	addChild(bg);

	mTrack = make_shared<TouchView>();
	mTrack->setSize(vec2(310, 15));
	mTrack->setPosition(vec2(20, 40));
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

	// current time
	const float timeMargin = 10.0f;
	mCurrentTimeLabel = make_shared<TextView>();
	mCurrentTimeLabel->setText("TIME", "label.large");
	mCurrentTimeLabel->setPosition(mTrack->getPosition() + vec2(0, -mCurrentTimeLabel->getHeight() - timeMargin));
	addChild(mCurrentTimeLabel);

	mCurrentTime = make_shared<TextView>();
	mCurrentTime->setText("<time>", "label.large.highlighted");
	mCurrentTime->setTextAlign(bluecadet::text::TextAlign::Right);
	mCurrentTime->setPosition(mTrack->getPosition() + vec2(mTrack->getWidth() - mCurrentTime->getWidth(), -mCurrentTime->getHeight() - timeMargin));
	addChild(mCurrentTime);

	// start/end times
	mStartTime = make_shared<TextView>();
	mStartTime->setText(data->getDateStringFromTimestamp(data->getMinTimestamp()), "label.small");
	mStartTime->setPosition(mTrack->getPosition() + vec2(0, mTrack->getHeight() + timeMargin));
	addChild(mStartTime);

	mEndTime = make_shared<TextView>();
	mEndTime->setTextAlign(bluecadet::text::TextAlign::Right);
	mEndTime->setText(data->getDateStringFromTimestamp(DataManager::getInstance()->getMaxTimestamp()), "label.small");
	mEndTime->setPosition(mTrack->getPosition() + mTrack->getSize() + vec2(-mEndTime->getWidth(), timeMargin));
	addChild(mEndTime);

	// speed
	mSpeed = make_shared<TextView>();
	mSpeed->setText("<speed>", "label.small.highlighted");
	mSpeed->setTextAlign(bluecadet::text::TextAlign::Right);
	mSpeed->setPosition(getBounds().getLowerRight() - mSpeed->getSize() - vec2(30));
	addChild(mSpeed);

	mTrack->getSignalTouchMoved().connect([=] (const bluecadet::touch::TouchEvent & event) {
		vec2 posInTrack = mTrack->convertGlobalToLocal(event.globalPosition);
		float dragDist = clamp(posInTrack.x, 0.0f, mTrack->getWidth());
		float progress = dragDist / mTrack->getWidth();

		mProgress->setScale(vec2(progress, 1.0f));

		TimelineManager::getInstance()->setNormProgress(progress);
	});

	// params
	auto params = OceanSettings::getInstance()->getParams();
	params->addButton("Play", [=] { play(); });
	params->addButton("Pause", [=] { pause(); });
	params->addButton("Faster", [=] { faster(); });
	params->addButton("Slower", [=] { slower(); });
}

void TimelineView::update(double deltaTime) {
	auto timelineManager = TimelineManager::getInstance();
	auto data = DataManager::getInstance();

	if (mTrack->getNumTouches() == 0) {
		// update progress if not dragging
		timelineManager->update(deltaTime);
		float progress = timelineManager->getNormProgress();
		mProgress->setScale(vec2(progress, 1.0f));
	}

	// update time
	mCurrentTime->setText(data->getDateStringFromTimestamp(timelineManager->getAbsProgress()));
	mCurrentTime->setPosition(vec2(mTrack->getBounds().getUpperRight().x - mCurrentTime->getWidth(), mCurrentTime->getPosition()().y));

	// update speed
	float speed = timelineManager->getSpeed();
	int precision = 1;
	if (speed < 0.25f) precision = 3;
	else if (speed < 0.5f) precision = 2;
	mSpeed->setText("x" + bluecadet::text::toPrecision(speed, precision));
	mSpeed->setPosition(getSize() - mSpeed->getSize() - vec2(20, 15));
}

void TimelineView::play() {
	TimelineManager::getInstance()->setPaused(false);
}

void TimelineView::pause() {
	TimelineManager::getInstance()->setPaused(true);
}

void TimelineView::togglePause() {
	TimelineManager::getInstance()->setPaused(!TimelineManager::getInstance()->isPaused());
}

void TimelineView::faster() {
	auto timelineManager = TimelineManager::getInstance();
	if (timelineManager->getSpeed() < 8.0f) {
		timelineManager->animateSpeedTo(timelineManager->getSpeed() * 2.0f);
	}
}

void TimelineView::slower() {
	auto timelineManager = TimelineManager::getInstance();
	if (timelineManager->getSpeed() > 1.0f / 8.0f) {
		timelineManager->animateSpeedTo(timelineManager->getSpeed() / 2.0f);
	}
}


}

