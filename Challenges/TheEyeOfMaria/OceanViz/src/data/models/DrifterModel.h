#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"

namespace amnh {

	struct DrifterModel {
	public:
		struct SampleEvent {
			std::time_t		time;
			float			latitude;
			float			longitude;
			float			qaulityIndex;
		};

		DrifterModel();
		~DrifterModel();

		// Functions
		void							setId(std::string id) { mId = id; };
		void							addSampleEvent(SampleEvent sample) { mSampleEvents.push_back(sample); }
		/*		void							setMonth(std::string month) { mMonth = month; };
		void							setDay(std::string day) { mDay = day; };
		void							setYear(std::string year) { mYear = year; };
		void							setLat(std::string lat) { mLat = lat; };
		void							setLong(std::string longitude) { mLong = longitude; };
		void							setQualityIndex(std::string qualIndex) { mQualityIndex = qualIndex; };*/

		std::string						getId() { return mId; };
		/*		std::string						getMonth() { return mMonth; };
		std::string						getDay() { return mDay; };
		std::string						getYear() { return mYear; };
		std::string						getLat() { return mLat; };
		std::string						getLong() { return mLong; };
		std::string						getQualityIndex() { return mQualityIndex; };*/

	protected:
		// Functions

		// Properties
		std::string						mId;
		std::vector<SampleEvent>		mSampleEvents;
	};
}
