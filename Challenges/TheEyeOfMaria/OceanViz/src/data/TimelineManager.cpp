#include "TimelineManager.h"

#include "cinder/Log.h"
#include "data/OceanSettings.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace amnh {

TimelineManager::TimelineManager()
{
}

TimelineManager::~TimelineManager() {
}

void TimelineManager::setup() {
	//OceanSettings::getInstance()->getParams()->addParam("Speed", &mSpeed).group("Timeline");
}

void TimelineManager::update(double deltaTime) {
	if (mIsPaused) {
		return;
	}
	float deltaProgress = mSpeedMult * deltaTime / mSecondsPerPlaythrough;
	mProgress = fmodf(mProgress + deltaProgress, 1.0f);
}

void TimelineManager::animateSpeedTo(float speed, float duration) {
	mTargetSpeedMult = speed;
	timeline().apply(&mSpeedMult, speed, duration, easeInOutQuad);
}

}

