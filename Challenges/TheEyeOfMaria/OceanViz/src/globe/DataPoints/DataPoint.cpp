#pragma once


#include "DataPoint.h"


using namespace ci;
using namespace ci::app;
using namespace std;



namespace amnh {

DataPoint::DataPoint() {
}

void DataPoint::setup(ci::vec3 pos) {

	mPos = pos;

	mColor = ColorA(1.0f, 0.0f, 0.0f, 1.0f);
	mRadius = 1.0f;
	bShow = 1.0f;

}

void DataPoint::setType(DataType type) {

	mDataType = type;
	switch (type)
	{
	case amnh::DataPoint::DataType::FLOAT:
		mColor = ColorA(1.0f, 0.8f, 0.0f, 1.0f);
		mRadius = 2.0f;
		break;
	case amnh::DataPoint::DataType::DRIFTER:
		mColor = ColorA(0.0f, 1.0f, 0.0f, 1.0f);
		mRadius = 2.0f;
		break;
	case amnh::DataPoint::DataType::HURRICANE:
		mColor = ColorA(1.0f, 0.0f, 0.0f, 1.0f);
		mRadius = 8.0f;
		break;
	default:
		break;
	}

}

}