#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"

namespace amnh {
	struct HurricaneModel {
	public:
		struct SampleEvent {
			time_t			timestamp;
			float			latitude;
			float			longitude;
			float			wind;
			float			pressure;
			std::string		stormType;
			std::string		category;

		};

		HurricaneModel();
		~HurricaneModel();

		// Functions
		void							addSampleEvent(SampleEvent sample) { mSampleEvents.push_back(sample); }
		std::vector<SampleEvent>		getAllSampleEvents() { return mSampleEvents; };

		void							setId(std::string id) { mId = id; };
		std::string						getId() { return mId; };


	protected:
		// Functions

		// Properties
		std::string					mId;
		std::vector<SampleEvent>	mSampleEvents;


	};
}
