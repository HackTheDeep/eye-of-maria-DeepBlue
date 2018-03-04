#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"

namespace amnh {

	struct DrifterModel {
	public:
		struct SampleEvent {
			std::time_t		timestamp = 0;
			float			normalizedTime = 0;
			float			latitude = 0;
			float			longitude = 0;
			float			qualityIndex = 0;
			float			depth = 0;
		};

		DrifterModel(std::string id = "", ci::ColorA color = ci::ColorA::zero()) : mId(id), mColor(color){};
		~DrifterModel();

		// Functions
		inline void							addSampleEvent(const SampleEvent & sample) { mSampleEvents.push_back(sample); }
		inline const std::vector<SampleEvent> &	getAllSampleEventsConst() const { return mSampleEvents; };
		inline std::vector<SampleEvent> &	getAllSampleEvents() { return mSampleEvents; };

		void							setId(std::string id) { mId = id; };
		inline const std::string &		getId() const { return mId; };

		inline const ci::ColorA &		getColor() const { return mColor; }
		inline void						setColor(ci::ColorA c) { mColor = c; }

	protected:

		// Properties
		std::string						mId;
		ci::ColorA						mColor = ci::ColorA::zero(); // zero = use default color
		std::vector<SampleEvent>		mSampleEvents;
	};
}
