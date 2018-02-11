#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "bluecadet/core/ScreenLayout.h"
#include "bluecadet/views/BaseView.h"
#include "ui/UiController.h"

namespace amnh {

typedef std::shared_ptr<class MainController> MainControllerRef;

class MainController : public bluecadet::views::BaseView {

public:
	MainController();
	~MainController();

	void setup();

protected:
	UiControllerRef mUiController = nullptr;
};
}
