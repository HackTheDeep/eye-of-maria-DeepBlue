#include "TimelineManager.h"

#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace amnh {

TimelineManager::TimelineManager()
{
}

TimelineManager::~TimelineManager() {
}

void TimelineManager::update() {
	mProgress = fmodf(mProgress + mSpeedMult * mSpeed, 1.0f);
}

void TimelineManager::animateSpeedTo(float speed) {
	timeline().apply(&mSpeedMult, speed, 0.3, easeInOutQuad);
}

}

