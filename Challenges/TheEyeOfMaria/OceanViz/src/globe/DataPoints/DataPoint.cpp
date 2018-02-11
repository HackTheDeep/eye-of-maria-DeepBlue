#pragma once


#include "DataPoint.h"


using namespace ci;
using namespace ci::app;
using namespace std;



namespace amnh {

DataPoint::DataPoint() {
}

void DataPoint::setup(int id, ci::vec3 pos) {

	mId = id;
	mPos = pos;

	mColor = ColorA(1.0f, 0.0f, 0.0f, 1.0f);
	mRadius = 5.0f;
	bShow = 1.0f;


}

}