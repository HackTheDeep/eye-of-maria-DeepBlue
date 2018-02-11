#include "FloatModel.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace amnh {
	FloatModel::FloatModel() {}
	FloatModel::~FloatModel() {}
	void FloatModel::sortEvents() {
		std::sort(
			mSampleEvents.begin(),
			mSampleEvents.end(),
			[](const SampleEvent& lhs, const SampleEvent& rhs){ return lhs.timestamp < rhs.timestamp; }
		);
	}



}