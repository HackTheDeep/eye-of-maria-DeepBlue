#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/Timeline.h"

namespace amnh {

typedef std::shared_ptr<class TimelineManager> TimelineManagerRef;

class TimelineManager {

public:
	static TimelineManagerRef getInstance() {
		static TimelineManagerRef instance = nullptr;
		if (!instance) {
			instance = std::make_shared<TimelineManager>();
		}
		return instance;
	}

	TimelineManager();
	~TimelineManager();

	void setup();

	void update(double deltaTime);

	void setNormProgress(float progress) { mProgress = progress; }
	float getNormProgress() const { return mProgress; }
	float getAbsProgress() const { return glm::mix(mStartTime, mEndTime, mProgress); }

	void setStartTime(const float value) { mStartTime = value; }
	float getStartTime() const { return mStartTime; }

	void setEndTime(const float value) { mEndTime = value; }
	float getEndTime() const { return mEndTime; }

	void animateSpeedTo(float speed);

protected:

	ci::Anim<float> mSpeedMult = 1.0f;
	float mSecondsPerPlaythrough = 30.0f;
	float mProgress = 0.0f;

	float mStartTime = 0;
	float mEndTime = 0;

};
}
