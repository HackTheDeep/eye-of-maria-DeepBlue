#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/CinderGlm.h"
#include "cinder/CinderMath.h"
#include "cinder/Easing.h"

#include "DataPoint.h"

namespace amnh {

	typedef std::shared_ptr<class DataPointController> DataPointControllerRef;

	class DataPointController {

	public:
		DataPointController();
		~DataPointController();

		static DataPointControllerRef getInstance() {
			static DataPointControllerRef instance = nullptr;
			if (!instance) {
				instance = std::make_shared<DataPointController>();
			}
			return instance;
		}

		void setup();
		void update();
		void draw();

		void loadShader();
		void replaceBatchShader();

	protected:

		std::vector<DataPoint>	mPointsList;

		ci::gl::VboRef		mPointsVbo;
		ci::gl::BatchRef	mPointsBatch;
		ci::gl::GlslProgRef	mPointsShader;

	};
}
