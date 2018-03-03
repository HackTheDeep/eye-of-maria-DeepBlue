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
			float			normalizedTime;
			float			latitude;
			float			longitude;
			float			wind;
			float			pressure;
			std::string		stormType;
			int				category;
		};

		HurricaneModel();
		~HurricaneModel();

		// Functions
		inline void							addSampleEvent(SampleEvent sample) { mSampleEvents.push_back(sample); }
		inline std::vector<SampleEvent> &	getAllSampleEvents() { return mSampleEvents; };

		void							setId(std::string id) { mId = id; };
		const std::string &				getId() const { return mId; };


	protected:
		// Functions

		// Properties
		std::string					mId;
		std::vector<SampleEvent>	mSampleEvents;


	};
}
