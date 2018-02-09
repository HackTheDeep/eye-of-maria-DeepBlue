#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"

#include "bluecadet/views/BaseView.h"
#include "bluecadet/views/ImageView.h"
#include "bluecadet/views/TouchView.h"

namespace amnh {

	typedef std::shared_ptr<class HurricaneModel> HurricaneModelRef;

	class HurricaneModel {

	public:
		HurricaneModel();
		~HurricaneModel();

		// Functions
		void							setDate(std::string date) { mDate = date; };
		void							setTime(std::string time) { mTime = time; };
		void							setLat(std::string lat) { mLat = lat; };
		void							setLong(std::string longitude) { mLong = longitude; };
		void							setWind(std::string wind) { mWind = wind; };
		void							setPressure(std::string pressure) { mPressure = pressure; };
		void							setStormType(std::string type) { mStormType = type; };
		void							setCategory(std::string category) { mCategory = category; };

		std::string						getDate() { return mDate; };
		std::string						getTime() { return mDate; };
		std::string						getLat() { return mLat; };
		std::string						getLong() { return mLong; };
		std::string						getWind() { return mWind; };
		std::string						getPressure() { return mPressure; };
		std::string						getStormType() { return mStormType; };
		std::string						getCategory() { return mCategory; };

	protected:
		// Functions

		// Properties
		std::string						mDate;
		std::string						mTime;
		std::string						mLat;
		std::string						mLong;
		std::string						mWind;
		std::string						mPressure;
		std::string						mStormType;
		std::string						mCategory;



	};
}
