#include "DisplayPallet.h"

#include "cinder/Log.h"

#include "bluecadet/core/ScreenLayout.h"

#include "data/OceanSettings.h"
#include "data/DataManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace bluecadet::core;
using namespace bluecadet::views;

namespace amnh {

	DisplayPallet::DisplayPallet() {
	}

	DisplayPallet::~DisplayPallet() {
	}

	void DisplayPallet::setup() {
		setSize(vec2(350, 115));

		auto bg = make_shared<StrokedRectView>();
		bg->setStrokeColor(Color::white());
		bg->setStrokeWidth(1);
		bg->setBackgroundColor(ColorA::gray(0, 0.5f));
		bg->setSize(getSize());
		addChild(bg);


		// Setup Views
		mHeaderButton = make_shared<TouchView>();
		mHeaderButton->setup(vec2(getWidth(), 40));
		TextViewRef	mHeaderBtnText = make_shared<TextView>();
		mHeaderBtnText->setup("Display Toggles");
		mHeaderButton->addChild(mHeaderBtnText);

		mDrifterButton = make_shared<TouchView>();
		mDrifterButton->setup(vec2(350/3, 40));
		mDrifterButton->setPosition(vec2(0, mHeaderButton->getHeight()));
		mDrifterButton->setBackgroundColor(Color(1, 0, 0));
		TextViewRef	mDriftBtnText = make_shared<TextView>();
		mDriftBtnText->setup("Drifters");
		mDrifterButton->addChild(mDriftBtnText);

		mFloatsButton = make_shared<TouchView>();
		mFloatsButton->setup(vec2(350 / 3, 40));
		mFloatsButton->setPosition(vec2(mDrifterButton->getWidth(), mHeaderButton->getHeight()));
		mFloatsButton->setBackgroundColor(Color(1, 0, 1));
		TextViewRef	mFloatBtnText = make_shared<TextView>();
		mFloatBtnText->setup("Floats");
		mFloatsButton->addChild(mFloatBtnText);

		mHurricaneButton = make_shared<TouchView>();
		mHurricaneButton->setup(vec2(350 / 3, 40));
		mHurricaneButton->setPosition(vec2(mDrifterButton->getWidth() * 2, mHeaderButton->getHeight()));
		mHurricaneButton->setBackgroundColor(Color(0, 1, 1));
		TextViewRef	mHurricaneBtnText = make_shared<TextView>();
		mHurricaneBtnText->setup("Hurricane");
		mHurricaneButton->addChild(mHurricaneBtnText);

		addChild(mHeaderButton);
		addChild(mDrifterButton);
		addChild(mFloatsButton);
		addChild(mHurricaneButton);

		// params
		auto params = OceanSettings::getInstance()->getParams();
/*		params->addButton("Play", [=] { play(); });
		params->addButton("Pause", [=] { pause(); });
		params->addButton("Faster", [=] { faster(); });
		params->addButton("Slower", [=] { slower(); });*/
	}



}

