#include "DataPointController.h"

#include "cinder/Log.h"

#include "data/OceanSettings.h"

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


	//int numPoints = 101000;
	const int numPoints = DataManager::getInstance()->getNumEvents(); // the max num of points we can display
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
	const auto & drifterMap = DataManager::getInstance()->getAllDrifters();

	CI_LOG_I("Adding Drifter Data...");

	mNumDrifterPts = 0;
	driftersStartIndex = mNumUsedPoints;

	minDrifterQuality = 999999999999;
	maxDrifterQuality = -1;

	//iterate through each drifter
	for (const auto & drifter : drifterMap){
	
		//go through each drifter model and get all the data points
		const auto & events = drifter.second.getAllSampleEventsConst();

		const auto & color = drifter.second.getColor();
		const bool lockedColor = color != ci::ColorA::zero();

		for (const auto & event : events) {

			if (event.normalizedTime < 0) {
				// skip invalid drifters
				continue;
			}

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong( event.latitude, event.longitude ));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::DRIFTER);
			mPointsList[mNumUsedPoints].mTimeStamp = event.normalizedTime;
			mPointsList[mNumUsedPoints].mQualityIndex = event.qualityIndex;
			mPointsList[mNumUsedPoints].mColor = color;
			mPointsList[mNumUsedPoints].mLockedColor = lockedColor;

			if (event.qualityIndex > maxDrifterQuality) maxDrifterQuality = event.qualityIndex;
			if (event.qualityIndex < minDrifterQuality) minDrifterQuality = event.qualityIndex;

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
			
			//CI_LOG_I("Hurricane wind: " << mPointsList[mNumUsedPoints].mWind);

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

	minFloatSalinity = 9999999999;
	maxFloatSalinity -1;
	minFloatTemp = 9999999999;
	maxFloatTemp = -1;
	minFloatPressure = 999999999;
	maxFloatPressure = -1;

	//iterate through each drifter
	for (auto & floater : floaterMap) {

		//go through each drifter model and get all the data points
		vector<FloatModel::SampleEvent> events = floater.second.getAllSampleEvents();

		int numLineSegments = 0;

		for (int i = 0; i < events.size(); i++) {

			mPointsList[mNumUsedPoints].setup(getPolarFromLatLong(events[i].latitude, events[i].longitude));
			mPointsList[mNumUsedPoints].setType(DataPoint::DataType::FLOAT);
			mPointsList[mNumUsedPoints].mTimeStamp = events[i].normalizedTime;
			mPointsList[mNumUsedPoints].mTemp = events[i].temp;
			mPointsList[mNumUsedPoints].mPSalinity = events[i].psal;
			mPointsList[mNumUsedPoints].mPressure = events[i].pressure;

			if (events[i].temp > maxFloatTemp) maxFloatTemp = events[i].temp;
			if (events[i].temp < minFloatTemp) minFloatTemp = events[i].temp;
			
			if (events[i].psal > maxFloatSalinity) maxFloatSalinity = events[i].psal;
			if (events[i].psal < minFloatSalinity) minFloatSalinity = events[i].psal;

			if (events[i].pressure > maxFloatPressure) maxFloatPressure = events[i].pressure;
			if (events[i].pressure < minFloatPressure) minFloatPressure = events[i].pressure;

			//setup the lines
			if (i > 0) {

				linePoints.push_back( mPointsList[mNumUsedPoints-1].mPos );
				linePoints.push_back( mPointsList[mNumUsedPoints  ].mPos );
				numLineSegments ++;


			}

			mNumUsedPoints++;
			mNumFloatPts++;

		}

		//add colors for each line from white to red
		for (int i = 0; i < numLineSegments; i++) {
			
			float pct = i/(float)numLineSegments;

			ColorA start = ColorA(1.0f, 1.0f, 1.0f, 1.0f);
			ColorA end = ColorA(1.0f, 0.0f, 0.0f, 1.0f);

			lineColors.push_back( start.lerp(pct, end) );
			lineColors.push_back( start.lerp(pct, end) );

		}
	}

	CI_LOG_I(mNumFloatPts << " Floater Points Added");

}

void DataPointController::setupLines() {

	CI_LOG_I("Number of Lines Points: " << linePoints.size());


	//prepare layout of vertex attributes
	gl::VboMesh::Layout layout;
	layout.usage(GL_STATIC_DRAW).attrib(geom::Attrib::POSITION, 3);
	layout.usage(GL_STATIC_DRAW).attrib(geom::Attrib::COLOR, 4);

	//create the mesh in gl_lines mode (i.e. every successive pair of points is a line)
	mLinesMesh = gl::VboMesh::create((uint32_t)linePoints.size(), GL_LINES, { layout });
	mLinesMesh->bufferAttrib(geom::Attrib::POSITION, linePoints);
	mLinesMesh->bufferAttrib(geom::Attrib::COLOR, lineColors);

	//get stock shader
	mLinesShader = gl::getStockShader(gl::ShaderDef().color());

	//create the batch
	mLinesBatch = gl::Batch::create(mLinesMesh, mLinesShader );

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

		auto & point = mPointsList[i];

		if (point.mLockedColor) {
			continue;
		}

		switch (colorType) {
		case DrifterColor::QUALITY:
			val = (float)point.mQualityIndex;
			break;

		default:
			break;
		}

		float pct = (startVal == endVal) ? 1.0f : clamp(lmap(val, startVal, endVal, 0.0f, 1.0f), 0.0f, 1.0f);
		point.mColor = startCol.lerp(pct, endCol);
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

	drawLines();

	mPointsShader->uniform("uTrailDuration", OceanSettings::getInstance()->mTrailDuration);
	mPointsShader->uniform("uTrailFadePower", OceanSettings::getInstance()->mTrailFadePower);
	float t = TimelineManager::getInstance()->getNormProgress();
	gl::ScopedBlendAdditive scopedBlend;
	mPointsShader->uniform("uPlayhead", t);
	mPointsBatch->draw();
}

void DataPointController::drawLines() {

	if (bShowFloatPts) {
		gl::color(ColorA::gray(1.0f, 1.0f ));
		mLinesBatch->draw();
	}

}

}