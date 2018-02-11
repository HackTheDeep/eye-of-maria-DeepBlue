#include "OceanSettings.h"

#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace amnh {

OceanSettings::OceanSettings() {
	
}

OceanSettings::~OceanSettings() {
}

void OceanSettings::initParams() {
	auto params = getParams();
	params->addParam("Trail Duration", &mTrailDuration).group("Trail").min(0).max(1.0f).step(0.05f);
	params->addParam("Trail Fade Power", &mTrailFadePower).group("Trail").min(0).max(16.0f).step(0.25f);
}



}

