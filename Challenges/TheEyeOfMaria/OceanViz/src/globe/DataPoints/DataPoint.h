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

	void setup(int id, ci::vec3 pos );

	enum class DataType {
		FLOAT,
		DRIFTER,
		HURRICANE
	};

	DataType	mDataType;

	int			mId;
	ci::vec3	mPos;
	ci::ColorA	mColor;
	float		mRadius;
	float		bShow;

};

}