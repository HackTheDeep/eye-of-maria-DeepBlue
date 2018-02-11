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
	//particleLayout.append(geom::Attrib::CUSTOM_1, 1, sizeof(DataPoint), offsetof(DataPoint, bShow));
	particleLayout.append(geom::Attrib::CUSTOM_1, 1, sizeof(DataPoint), offsetof(DataPoint, mTimeStamp));

	// Create mesh by pairing our particle layout with our particle Vbo.
	// A VboMesh is an array of layout + vbo pairs
	auto mesh = gl::VboMesh::create(mPointsList.size(), GL_POINTS, { { particleLayout, mPointsVbo } });


	// load particle shader
	loadShader();

	//create the batch
	mPointsBatch = gl::Batch::create(mesh, mPointsShader, { { geom::CUSTOM_0, "iPointRadius" },
															{ geom::CUSTOM_1, "iTimeStamp" } });


	//CI_LOG_I("Min TimeStamp: " << DataManager::getInstance()->getMinTimestamp() << "Max TimeStamp: " << DataManager::getInstance()->getMaxTimestamp());
	
}

void DataPointController::addDrifterData() {

	//get drifter data
	auto drifterMap = DataManager::getInstance()->getAllDrifters();

	CI_LOG_I("Adding Drifter Data...");

	mNumDrifterPts = 0;
	driftersStartIndex = mNumUsedPoints;

	//iterate through each drifter
	for (auto & drifter : drifterMap){
	
		//go through each drifter model and get all the data points
		vector<DrifterModel::SampleEvent> events = drifter.second.getAllSampleEvents();

		for (const auto & event : events) {

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong( event.latitude, event.longitude ));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::DRIFTER);
			mPointsList[mNumUsedPoints].mTimeStamp = event.normalizedTime;
			mPointsList[mNumUsedPoints].mQualityIndex = event.qaulityIndex;

			mNumUsedPoints++;
			mNumDrifterPts++;
		}
	}

	CI_LOG_I(mNumDrifterPts << " Drifter Points Added");

}

void DataPointController::addHurricaneData() {

	//get drifter data
	auto hurricanes = DataManager::getInstance()->getAllHurricaneModels();

	float minTimestamp = DataManager::getInstance()->getMinTimestamp();
	float totalDuration = DataManager::getInstance()->getMaxTimestamp() - minTimestamp;

	CI_LOG_I("Adding Hurricane Data...");

	mNumHurricanePts = 0;
	hurricaneStartIndex = mNumUsedPoints;

	minHurPressure = 99999999999;
	maxHurPressure = -1;
	minHurWindSpeed = 99999999999;
	maxHurWindSpeed = -1;

	//go through hurricanes
	for (auto & hurricane : hurricanes) {

		//go through sample events of hurricane
		for(const auto & event : hurricane.getAllSampleEvents()){

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong(event.latitude, event.longitude));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::HURRICANE);
			mPointsList[mNumUsedPoints].mTimeStamp = event.normalizedTime;
			mPointsList[mNumUsedPoints].mWind = event.wind;
			mPointsList[mNumUsedPoints].mPressure = event.pressure;
			mPointsList[mNumUsedPoints].mStormType = event.stormType;
			mPointsList[mNumUsedPoints].mCategory = event.category;

			if (event.wind > maxHurWindSpeed) maxHurWindSpeed = event.wind;
			if (event.wind < minHurWindSpeed) minHurWindSpeed = event.wind;
			if (event.pressure > maxHurPressure) maxHurPressure = event.pressure;
			if (event.pressure < minHurPressure) minHurPressure = event.pressure;
			
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

	float minTimestamp = DataManager::getInstance()->getMinTimestamp();
	float totalDuration = DataManager::getInstance()->getMaxTimestamp() - minTimestamp;

	CI_LOG_I("Adding Floater Data...");

	mNumFloatPts = 0;
	floaterStartIndex = mNumUsedPoints;

	//iterate through each drifter
	for (auto & floater : floaterMap) {

		//go through each drifter model and get all the data points
		vector<FloatModel::SampleEvent> events = floater.second.getAllSampleEvents();

		for (const auto & event : events) {

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong(event.latitude, event.longitude));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::FLOAT);
			mPointsList[mNumUsedPoints].mTimeStamp = event.normalizedTime;
			mPointsList[mNumUsedPoints].mTemp = event.temp;
			mPointsList[mNumUsedPoints].mPSalinity = event.psal;
			mPointsList[mNumUsedPoints].mPressure = event.pressure;

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
	float radius = 330.0f;

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
		startCol = ColorA(1.0f, 1.0f, 0.0f, 1.0f);
		endCol = ColorA(1.0f, 0.0f, 0.0f, 1.0f);
		startVal = minHurWindSpeed;
		endVal = maxHurWindSpeed;
		CI_LOG_I("Mapping Hurricane colors to Wind");
		break;
	case HurricaneColor::PRESSURE:
		startCol = ColorA(0.0f, 0.0f, 1.0f, 1.0f);
		endCol = ColorA(1.0f, 1.0f, 1.0f, 1.0f);
		startVal = minHurPressure;
		endVal = maxHurPressure;
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

		default:
			break;
		}

		float pct = clamp(lmap(val, startVal, endVal, 0.0f, 1.0f), 0.0f, 1.0f);
		mPointsList[i].mColor = startCol.lerp(pct, endCol);
	}


}

void DataPointController::draw() {

	mPointsShader->uniform("uViewScale", 1.0f);
	float t = TimelineManager::getInstance()->getNormProgress();
	gl::ScopedBlendAdditive scopedBlend;
	mPointsShader->uniform("uPlayhead", t);
	mPointsBatch->draw();
}

}