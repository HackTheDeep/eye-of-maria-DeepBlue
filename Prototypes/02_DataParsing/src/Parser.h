#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"

#include "bluecadet/views/BaseView.h"
#include "bluecadet/views/ImageView.h"
#include "bluecadet/views/TouchView.h"
#include "models/HurricaneModel.h"
#include "models/DrifterModel.h"

namespace amnh {

	typedef std::shared_ptr<class Parser> ParserRef;

	class Parser {

	public:
		static ParserRef getInstance() {
			static ParserRef instance = nullptr;
			if (!instance) {
				instance = ParserRef(new Parser());
			}
			return instance;
		}

		Parser();
		~Parser();

		// Functions
		void							parseHurricaneData();
		void				  			parseDrifterData();
		void							parseFloatsData();
	protected:
		// Functions
		void							parseJSON(ci::JsonTree & tree);
		void							loadCSVData(std::string fileName);
		std::vector<std::string>		getNextLineAndSplitIntoTokens(std::istream& str, const char delimeter);

		// Model Vectors
		std::vector<HurricaneModelRef> mHurricaneModels;
		std::vector<DrifterModelRef> mDrifterModels;
		


		// Hurricane Properties (hardcoded)
		int mHurricane_DateIndex = 0;
		int mHurricane_TimeIndex = 1;
		int mHurricane_LatIndex = 2;
		int mHurricane_LongIndex = 3;
		int mHurricane_WindIndex = 4;
		int mHurricane_PressureIndex = 5;
		int mHurricane_StormTypeIndex = 6;
		int mHurricane_CategoryIndex = 7;

		// id    mm   dd   yy      lat      lon    qual. index
		// Drifter Properties (hardcoded)
		int mDrifter_IdIndex = 0;
		int mDrifter_MonthIndex = 1;
		int mDrifter_DayIndex = 2;
		int mDrifter_YearIndex = 3;
		int mDrifter_LatIndex = 4;
		int mDrifter_LongIndex = 5;
		int mDrifter_QualIndexIndex = 6;


	};
}
