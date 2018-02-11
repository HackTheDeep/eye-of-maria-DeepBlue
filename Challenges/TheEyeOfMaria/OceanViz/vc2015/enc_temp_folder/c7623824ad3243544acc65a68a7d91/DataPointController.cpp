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


	int numPoints = 100000;
	mNumUsedPoints = 0;

	for (int i = 0; i < numPoints; i++) {
		//setup all the placeholder points
		DataPoint p;
		vec3 pos = vec3(0.0f);
		p.setup(pos);
		p.mColor = ColorA::gray(0.0f, 0.0f);
		mPointsList.push_back(p);

		//if (i < 2880) {
		//	float lat = lmap((float)i, 0.0f, (float)2880, 90.0f, -90.0f);
		//	float lon = i;

		//	float theta = glm::radians(90 - lat);
		//	float phi = glm::radians(180 + lon);

		//	DataPoint p;
		//	vec3 pos = vec3(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));
		//	pos *= vec3(255.0f);
		//	p.setup( i, pos );
		//	mPointsList.push_back(p);
		//} else {

		//}

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

}

void DataPointController::addDrifterData() {

	//get drifter data
	auto drifterMap = DataManager::getInstance()->getAllDrifters();

	CI_LOG_I("Adding Drifter Data...");

	//iterate through each drifter
	map<string, DrifterModel>::iterator it;
	for (it = drifterMap.begin(); it != drifterMap.end(); it++){
	
		//go through each drifter model and get all the data points
		vector<DrifterModel::SampleEvent> events = it->second.getAllSampleEvents();

		for (int i = 0; i < events.size(); i++) {

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong( events[i].latitude, events[i].longitude ));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::DRIFTER);
			mPointsList[mNumUsedPoints].mTimeStamp = (float)events[i].time;
			mPointsList[mNumUsedPoints].mQualityIndex = events[i].qaulityIndex;

			mNumUsedPoints++;
		}
	}

	CI_LOG_I(mNumUsedPoints << " Drifter Points Added");

}

void DataPointController::addHurricaneData() {

	//get drifter data
	auto hurricanes = DataManager::getInstance()->getAllHurricaneModels();

	CI_LOG_I("Adding Hurricane Data...");

	int numHurricanePoints;

	//go through hurricanes
	for (int i = 0; i < hurricanes.size(); i++) {

		auto events = hurricanes[i].getAllSampleEvents();
		//go through sample events of hurricane
		for(int j = 0; j < events.size(); j++){

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong(events[j].latitude, events[j].longitude));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::HURRICANE);
			mPointsList[mNumUsedPoints].mTimeStamp = (float)events[i].timestamp;
			mPointsList[mNumUsedPoints].mWind = events[i].wind;
			mPointsList[mNumUsedPoints].mPressure = events[i].pressure;
			mPointsList[mNumUsedPoints].mStormType = events[i].stormType;
			mPointsList[mNumUsedPoints].mCategory = events[i].category;

			mNumUsedPoints++;
			numHurricanePoints++;
		}
	}

	CI_LOG_I(numHurricanePoints << " Hurricane Points Added");

}

void DataPointController::addFloaterData() {

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

void DataPointController::draw() {

	mPointsShader->uniform("uViewScale", 1.0f);
	float t = TimelineManager::getInstance()->getAbsProgress();
	mPointsShader->uniform("uPlayhead", t);
	mPointsBatch->draw();
}

}