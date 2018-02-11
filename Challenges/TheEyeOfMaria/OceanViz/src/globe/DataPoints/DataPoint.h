#pragma once

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/Easing.h"
#include "cinder/Timeline.h"
#include "cinder/Log.h"


namespace amnh {

typedef std::shared_ptr<class DataPoint> DataPointRef;

class DataPoint {

public:

	DataPoint();
	~DataPoint() {};

	void setup(ci::vec3 pos );


	enum class DataType {
		FLOAT,
		DRIFTER,
		HURRICANE
	};

	void setType(DataType type);

	DataType	mDataType;

	ci::vec3	mPos;
	ci::ColorA	mColor;
	float		mRadius;
	float		bShow;

	//for drifters
	float mTimeStamp = std::numeric_limits<float>::max();
	float		mQualityIndex;

	//for hurricanes
	float		mWind;
	float		mPressure;
	std::string	mStormType;
	std::string	mCategory;

	//for floats


};

}