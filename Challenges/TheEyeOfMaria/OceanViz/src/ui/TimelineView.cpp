#include "TimelineView.h"

#include "cinder/Log.h"

#include "data/OceanSettings.h"
#include "bluecadet/core/ScreenLayout.h"

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

	auto bg = make_shared<StrokedRectView>();
	bg->setStrokeColor(Color::white());
	bg->setStrokeWidth(1);
	bg->setBackgroundColor(ColorA::gray(0, 0.5f));
	bg->setSize(getSize());
	addChild(bg);

	mTrack = make_shared<StrokedRectView>();
	mTrack->setStrokeColor(Color::white());
	mTrack->setStrokeWidth(1);
	mTrack->setSize(vec2(310, 15));
	mTrack->setPosition(vec2(10, 30));
	addChild(mTrack);

	mProgress = make_shared<BaseView>();
	mProgress->setBackgroundColor(ColorA(0.239, 0.349, 0.6, 0.75));
	mProgress->setSize(mTrack->getSize());
	mTrack->addChild(mProgress);
}

void TimelineView::update(double deltaTime) {

}

void TimelineView::handleTouchMoved(const bluecadet::touch::TouchEvent & event) {
	vec2 posInTrack = mTrack->convertGlobalToLocal(event.globalPosition);
	float w = clamp(posInTrack.x, 0.0f, mTrack->getWidth());
	mProgress->setWidth(w);
}


}

