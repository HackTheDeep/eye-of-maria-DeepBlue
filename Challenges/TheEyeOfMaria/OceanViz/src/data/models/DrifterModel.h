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
		};

		DrifterModel();
		~DrifterModel();

		// Functions
		inline void							addSampleEvent(SampleEvent sample) { mSampleEvents.push_back(sample); }
		inline std::vector<SampleEvent> &	getAllSampleEvents() { return mSampleEvents; };

		void							setId(std::string id) { mId = id; };
		const std::string &				getId() const { return mId; };

	protected:
		// Functions

		// Properties
		std::string						mId;
		std::vector<SampleEvent>		mSampleEvents;
	};
}
