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

void amnh::TimelineView::setup() {
	setSize(vec2(350, 115));

	auto bg = make_shared<StrokedRectView>();
	bg->setStrokeColor(Color::white());
	bg->setStrokeWidth(1);
	bg->setBackgroundColor(ColorA::gray(0, 0.5f));
	bg->setSize(getSize());

	mTrack = make_shared<StrokedRectView>();
	mTrack->setStrokeColor(Color::white());
	mTrack->setStrokeWidth(1);
	mTrack->setBackgroundColor(ColorA::gray(0, 0.5f));

}

}

