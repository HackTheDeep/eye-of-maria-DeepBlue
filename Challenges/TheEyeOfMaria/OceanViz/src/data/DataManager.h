#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"

#include "bluecadet/views/BaseView.h"
#include "bluecadet/views/ImageView.h"
#include "bluecadet/views/TouchView.h"
//#include "models/HurricaneModel.h"
#include "models/DrifterModel.h"
//#include "models/BuoyModel.h"

namespace amnh {

	typedef std::shared_ptr<class DataManager> DataManagerRef;

	class DataManager {

	public:
		static DataManagerRef getInstance() {
			static DataManagerRef instance = nullptr;
			if (!instance) {
				instance = DataManagerRef(new DataManager());
			}
			return instance;
		}

		DataManager();
		~DataManager();

		// Functions
		void							parseDrifterData();
		void							parseDrifterDirectoryData();
//		void							parseHurricanData();
//		void							parseBuoyData();

	protected:
		// Functions
		std::vector<std::string>		getNextLineAndSplitIntoTokens(std::istream& str, const char delimeter);

		// Model Vectors
//		std::vector<HurricaneModelRef> mHurricaneModels;
//		std::vector<DrifterModel> mDrifterModels;
//		std::vector<BuoyModelRef> mBuoyModels;
//		std::map<std::string, DrifterDirectoryModelRef> mDrifterDirectoryModels;
		std::map<std::string, DrifterModel> mDrifterMap;


		// Hurricane Properties (hardcoded)
		int mHurricane_DateIndex = 0;
		int mHurricane_TimeIndex = 1;
		int mHurricane_LatIndex = 2;
		int mHurricane_LongIndex = 3;
		int mHurricane_WindIndex = 4;
		int mHurricane_PressureIndex = 5;
		int mHurricane_StormTypeIndex = 6;
		int mHurricane_CategoryIndex = 7;

		// Drifter Properties (hardcoded)
		int mDrifter_IdIndex = 0;
		int mDrifter_MonthIndex = 1;
		int mDrifter_DayIndex = 2;
		int mDrifter_YearIndex = 3;
		int mDrifter_LatIndex = 4;
		int mDrifter_LongIndex = 5;
		int mDrifter_QualIndexIndex = 6;

		// Drifter Directory Properties (hardcoded)
		int mDirectory_IdIndex = 0;
		int mDirectory_WmoIndex = 1;
		int mDirectory_ExpIndex = 2;
		int mDirectory_FirstDateDayIndex = 3;
		int mDirectory_FirstDateMonthIndex = 4;
		int mDirectory_FirstDateYearIndex = 5;
		int mDirectory_FirstLatIndex = 6;
		int mDirectory_FirstLongIndex = 7;
		int mDirectory_EndDateDayIndex = 8;
		int mDirectory_EndDateMonthIndex = 9;
		int mDirectory_EndDateYearIndex = 10;
		int mDirectory_EndLatIndex = 11;
		int mDirectory_EndLongIndex = 12;
		int mDirectory_DropOffDateDayIndex = 13;
		int mDirectory_DropOffDateMonthIndex = 14;
		int mDirectory_DropOffDateYearIndex = 15;
		int mDirectory_DeathCodeIndex = 16;

		// Buoy Properites
		int mBuoy_IdIndex = 0;
		int mBuoy_MonthIndex = 1;
		int mBuoy_DayIndex = 2;
		int mBuoy_HourIndex = 3;
		int mBuoy_MinuteIndex = 4;
		int mBuoy_SecondIndex = 5;
		int mBuoy_LatIndex = 6;
		int mBuoy_LongIndex = 7;

	};
}
