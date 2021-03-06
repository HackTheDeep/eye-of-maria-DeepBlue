#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/core/SettingsManager.h"

namespace amnh {

typedef std::shared_ptr<class OceanSettings> OceanSettingsRef;

class OceanSettings : public bluecadet::core::SettingsManager {

public:
	static OceanSettingsRef getInstance() {
		static OceanSettingsRef instance = nullptr;
		if (!instance) {
			instance = std::make_shared<OceanSettings>();
		}
		return instance;
	}

	OceanSettings();
	~OceanSettings();

	void initParams();

	float mTrailDuration = 0.8f; // 0...1
	float mTrailFadePower = 10.0f; // trail alpha = progress ^ mTrailFadePower
};
}
