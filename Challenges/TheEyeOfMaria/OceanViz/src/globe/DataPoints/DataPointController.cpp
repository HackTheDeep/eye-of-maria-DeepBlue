#include "DataPointController.h"

#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;


namespace amnh {

DataPointController::DataPointController() {
}

DataPointController::~DataPointController() {
}

void DataPointController::setup() {

	mNumUsedPoints = 0;


	int numPoints = 101000;
	for (int i = 0; i < numPoints; i++) {
		//setup all the placeholder points
		DataPoint p;
		vec3 pos = vec3(0.0f);
		p.setup(pos);
		p.mColor = ColorA::gray(0.0f, 0.0f);
		mPointsList.push_back(p);

	}


	// Create mPoints buffer on GPU and copy over data.
	// Mark as streaming, since we will copy new data every frame.
	mPointsVbo = gl::Vbo::create(GL_ARRAY_BUFFER, mPointsList, GL_STREAM_DRAW);

	// Set how the data will be laid out in the buffer.
	geom::BufferLayout particleLayout;
	particleLayout.append(geom::Attrib::POSITION, 3, sizeof(DataPoint), offsetof(DataPoint, mPos));
	particleLayout.append(geom::Attrib::COLOR, 4, sizeof(DataPoint), offsetof(DataPoint, mColor));
	particleLayout.append(geom::Attrib::CUSTOM_0, 1, sizeof(DataPoint), offsetof(DataPoint, mRadius));
	particleLayout.append(geom::Attrib::CUSTOM_1, 1, sizeof(DataPoint), offsetof(DataPoint, mTimeStamp));
	particleLayout.append(geom::Attrib::CUSTOM_2, 1, sizeof(DataPoint), offsetof(DataPoint, bShow));

	// Create mesh by pairing our particle layout with our particle Vbo.
	// A VboMesh is an array of layout + vbo pairs
	auto mesh = gl::VboMesh::create(mPointsList.size(), GL_POINTS, { { particleLayout, mPointsVbo } });


	// load particle shader
	loadShader();

	//create the batch
	mPointsBatch = gl::Batch::create(mesh, mPointsShader, { { geom::CUSTOM_0, "iPointRadius" },
															{ geom::CUSTOM_1, "iTimeStamp" },
															{ geom::CUSTOM_2, "iShowPoint" } });

}

void DataPointController::addDrifterData() {

	//get drifter data
	auto drifterMap = DataManager::getInstance()->getAllDrifters();

	CI_LOG_I("Adding Drifter Data...");

	mNumDrifterPts = 0;
	driftersStartIndex = mNumUsedPoints;

	minDrifterQuality = 999999999999;
	maxDrifterQuality = -1;

	//iterate through each drifter
	map<string, DrifterModel>::iterator it;
	for (it = drifterMap.begin(); it != drifterMap.end(); it++){
	
		//go through each drifter model and get all the data points
		vector<DrifterModel::SampleEvent> events = it->second.getAllSampleEvents();

		for (int i = 0; i < events.size(); i++) {

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong( events[i].latitude, events[i].longitude ));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::DRIFTER);
			mPointsList[mNumUsedPoints].mTimeStamp = (float)events[i].time;
			mPointsList[mNumUsedPoints].mQualityIndex = events[i].qualityIndex;

			if (events[i].qualityIndex > maxDrifterQuality) maxDrifterQuality = events[i].qualityIndex;
			if (events[i].qualityIndex < minDrifterQuality) minDrifterQuality = events[i].qualityIndex;

			mNumUsedPoints++;
			mNumDrifterPts++;
		}
	}

	CI_LOG_I(mNumDrifterPts << " Drifter Points Added");

}

void DataPointController::addHurricaneData() {

	//get drifter data
	auto hurricanes = DataManager::getInstance()->getAllHurricaneModels();

	CI_LOG_I("Adding Hurricane Data...");

	mNumHurricanePts = 0;
	hurricaneStartIndex = mNumUsedPoints;

	minHurPressure = 99999999999;
	maxHurPressure = -1;
	minHurWindSpeed = 99999999999;
	maxHurWindSpeed = -1;

	//go through hurricanes
	for (int i = 0; i < hurricanes.size(); i++) {

		auto events = hurricanes[i].getAllSampleEvents();
		//go through sample events of hurricane
		for(int j = 0; j < events.size(); j++){

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong(events[j].latitude, events[j].longitude));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::HURRICANE);
			mPointsList[mNumUsedPoints].mTimeStamp = (float)events[j].timestamp;
			mPointsList[mNumUsedPoints].mWind = events[j].wind;
			mPointsList[mNumUsedPoints].mPressure = events[j].pressure;
			mPointsList[mNumUsedPoints].mStormType = events[j].stormType;
			mPointsList[mNumUsedPoints].mCategory = events[j].category;

			if (events[j].wind > maxHurWindSpeed) maxHurWindSpeed = events[j].wind;
			if (events[j].wind < minHurWindSpeed) minHurWindSpeed = events[j].wind;
			if (events[j].pressure > maxHurPressure) maxHurPressure = events[j].pressure;
			if (events[j].pressure < minHurPressure) minHurPressure = events[j].pressure;
			
			CI_LOG_I("Hurricane wind: " << mPointsList[mNumUsedPoints].mWind);

			mNumUsedPoints++;
			mNumHurricanePts++;
		}
	}

	CI_LOG_I(mNumHurricanePts << " Hurricane Points Added");

}

void DataPointController::addFloaterData() {

	//get drifter data
	auto floaterMap = DataManager::getInstance()->getAllFloats();

	CI_LOG_I("Adding Floater Data...");

	mNumFloatPts = 0;
	floaterStartIndex = mNumUsedPoints;

	minFloatSalinity = 9999999999;
	maxFloatSalinity -1;
	minFloatTemp = 9999999999;
	maxFloatTemp = -1;
	minFloatPressure = 999999999;
	maxFloatPressure = -1;

	//iterate through each drifter
	map<string, FloatModel>::iterator it;
	for (it = floaterMap.begin(); it != floaterMap.end(); it++) {

		//go through each drifter model and get all the data points
		vector<FloatModel::SampleEvent> events = it->second.getAllSampleEvents();

		for (int i = 0; i < events.size(); i++) {

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong(events[i].latitude, events[i].longitude));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::FLOAT);
			mPointsList[mNumUsedPoints].mTimeStamp = (float)events[i].timestamp;
			mPointsList[mNumUsedPoints].mTemp = events[i].temp;
			mPointsList[mNumUsedPoints].mPSalinity = events[i].psal;
			mPointsList[mNumUsedPoints].mPressure = events[i].pressure;

			if (events[i].temp > maxFloatTemp) maxFloatTemp = events[i].temp;
			if (events[i].temp < minFloatTemp) minFloatTemp = events[i].temp;
			
			if (events[i].psal > maxFloatSalinity) maxFloatSalinity = events[i].psal;
			if (events[i].psal < minFloatSalinity) minFloatSalinity = events[i].psal;

			if (events[i].pressure > maxFloatPressure) maxFloatPressure = events[i].pressure;
			if (events[i].pressure < minFloatPressure) minFloatPressure = events[i].pressure;

			mNumUsedPoints++;
			mNumFloatPts++;
		}
	}

	CI_LOG_I(mNumFloatPts << " Floater Points Added");

}

vec3 DataPointController::getPolarFromLatLong(float lat, float lon) {
	float theta = glm::radians(90 - lat);
	float phi = glm::radians(180 + lon);

	//set the radius a little bitter than the earth radius
	float radius = 851.0f;

	return vec3(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi)) * vec3(radius);
}

void DataPointController::loadShader() {
	try {
		mPointsShader = gl::GlslProg::create(loadAsset("points/point.vert"), loadAsset("points/point.frag"), loadAsset("points/point.geom"));
	}
	catch (const std::exception &e) {
		console() << e.what() << endl;
	}
	CI_LOG_I("Loading Point Shader");
}

void DataPointController::replaceBatchShader() {
	mPointsBatch->replaceGlslProg(mPointsShader);
	CI_LOG_I("Replacing Batch Shader");
}


void DataPointController::update() {

	// Copy particle data onto the GPU.
	// Map the GPU memory and write over it.
	void *gpuMem = mPointsVbo->mapReplace();
	std::memcpy(gpuMem, mPointsList.data(), mPointsList.size() * sizeof(DataPoint));
	mPointsVbo->unmap();

}

void DataPointController::reMapHurricaneColors(HurricaneColor colorType) {

	ColorA startCol;
	ColorA endCol;

	float startVal;
	float endVal;

	float val;

	switch (colorType) {
	case HurricaneColor::WIND:
		startCol = ColorA(1.0f, 1.0f, 0.0f, 1.0f);	//yellow
		endCol = ColorA(1.0f, 0.0f, 1.0f, 1.0f);	//magenta
		startVal = minHurWindSpeed;
		endVal = maxHurWindSpeed;
		CI_LOG_I("Mapping Hurricane colors to Wind");
		break;
	case HurricaneColor::PRESSURE:
		startCol = ColorA(0.0f, 0.0f, 1.0f, 1.0f);	//blue
		endCol = ColorA(0.0f, 1.0f, 1.0f, 1.0f);	//cyan
		startVal = minHurPressure;
		endVal = maxHurPressure;
		CI_LOG_I("Mapping Hurricane colors to Pressure");
		break;
	case HurricaneColor::CATEGORY:
		startCol = ColorA(1.0f, 1.0f, 1.0f, 1.0f);	//white
		endCol = ColorA(1.0f, 0.0f, 0.0f, 1.0f);	//red
		startVal = 0.0f;
		endVal = 5.0f;
		CI_LOG_I("Mapping Hurricane colors to Pressure");
		break;
	default:
		break;
	}

	for (int i = hurricaneStartIndex; i < hurricaneStartIndex + mNumHurricanePts; i++) {

		switch (colorType){
		case HurricaneColor::PRESSURE:
			val = mPointsList[i].mPressure;
			break;
		case HurricaneColor::WIND:
			val = mPointsList[i].mWind;
			break;
		case HurricaneColor::CATEGORY:
			val = (float)mPointsList[i].mCategory;
			break;
		default:
			break;
		}

		float pct = clamp(lmap(val, startVal, endVal, 0.0f, 1.0f), 0.0f, 1.0f);
		mPointsList[i].mColor = startCol.lerp(pct, endCol);
	}


}


void DataPointController::reMapDrifterColors(DrifterColor colorType) {

	ColorA startCol;
	ColorA endCol;

	float startVal;
	float endVal;

	float val;

	switch (colorType) {
	case DrifterColor::QUALITY:
		startCol = ColorA(1.0f, 0.0f, 1.0f, 1.0f);	//magenta
		endCol = ColorA(1.0f, 1.0f, 0.0f, 1.0f);	//yellow
		startVal = minDrifterQuality;
		endVal = maxDrifterQuality;
		CI_LOG_I("Mapping Drifter colors to Quality Index");
		break;

	default:
		break;
	}

	for (int i = driftersStartIndex; i < driftersStartIndex + mNumDrifterPts; i++) {

		switch (colorType) {
		case DrifterColor::QUALITY:
			val = (float)mPointsList[i].mQualityIndex;
			break;

		default:
			break;
		}

		float pct = clamp(lmap(val, startVal, endVal, 0.0f, 1.0f), 0.0f, 1.0f);
		mPointsList[i].mColor = startCol.lerp(pct, endCol);
	}


}

void DataPointController::reMapFloaterColors(FloaterColor colorType) {

	ColorA startCol;
	ColorA endCol;

	float startVal;
	float endVal;

	float val;

	switch (colorType) {
	case FloaterColor::PRESSURE:
		startCol = ColorA::gray(0.4f, 1.0f);	//gray
		endCol = ColorA::gray(1.0f, 1.0f);		//white
		startVal = minFloatPressure;
		endVal = maxFloatPressure;
		CI_LOG_I("Mapping Floater colors to Pressure");
		break;
	case FloaterColor::TEMP:
		startCol = ColorA(1.0f, 1.0f, 0.0f, 1.0f);	//yellow
		endCol = ColorA(1.0f, 0.0f, 0.0f, 1.0f);	//red
		startVal = minFloatTemp;
		endVal = maxFloatTemp;
		CI_LOG_I("Mapping Floater colors to Temperature");
		break;
	case FloaterColor::SALINITY:
		startCol = ColorA(1.0f, 1.0f, 1.0f, 1.0f);	//white
		endCol = ColorA(0.0f, 1.0f, 1.0f, 1.0f);	//cyan
		startVal = minFloatSalinity;
		endVal = maxFloatSalinity;
		CI_LOG_I("Mapping Floater colors to Salinity");
		break;
	default:
		break;
	}

	for (int i = floaterStartIndex; i < floaterStartIndex + mNumFloatPts; i++) {

		switch (colorType) {
		case FloaterColor::PRESSURE:
			val = mPointsList[i].mPressure;
			break;
		case FloaterColor::TEMP:
			val = mPointsList[i].mTemp;
			break;
		case FloaterColor::SALINITY:
			val = (float)mPointsList[i].mPSalinity;
			break;
		default:
			break;
		}

		float pct = clamp(lmap(val, startVal, endVal, 0.0f, 1.0f), 0.0f, 1.0f);
		mPointsList[i].mColor = startCol.lerp(pct, endCol);
	}


}

void DataPointController::toggleDrifters() {
	bShowDrifterPts = !bShowDrifterPts;
	for (int i = driftersStartIndex; i < driftersStartIndex + mNumDrifterPts; i++) {
		mPointsList[i].bShow = bShowDrifterPts;
	}

}

void DataPointController::toggleFloats() {
	bShowFloatPts = !bShowFloatPts;
	for (int i = floaterStartIndex; i < floaterStartIndex + mNumFloatPts; i++) {
		mPointsList[i].bShow = bShowFloatPts;
	}

}

void DataPointController::toggleHurricane() {
	bShowHurricanePts = !bShowHurricanePts;
	for (int i = hurricaneStartIndex; i < hurricaneStartIndex + mNumHurricanePts; i++) {
		mPointsList[i].bShow = bShowHurricanePts;
	}
}

void DataPointController::draw() {

	mPointsShader->uniform("uViewScale", 1.0f);
	float t = TimelineManager::getInstance()->getAbsProgress();
	mPointsShader->uniform("uPlayhead", t);
	mPointsBatch->draw();
}

}