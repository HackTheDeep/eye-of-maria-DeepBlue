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
	float deltaProgress = mSpeedMult * deltaTime / mSecondsPerPlaythrough;
	mProgress = fmodf(mProgress + deltaProgress, 1.0f);
}

void TimelineManager::animateSpeedTo(float speed) {
	timeline().apply(&mSpeedMult, speed, 0.3, easeInOutQuad);
}

}

