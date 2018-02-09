#include "Parser.h"

#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace amnh {
	Parser::Parser() {}

	Parser::~Parser() {}

	void Parser::parseJSON(ci::JsonTree & tree)
	{
//		setFieldFromJsonIfExists(&mVideosPreload, "videos.preload");
//		setFieldFromJsonIfExists(&mVideosAssetDir, "videos.assetDir");
	}
	

	void Parser::parseDrifterData() {
		std::string fileName = "../assets/data/drifer_data.dat";
		std::ifstream file;
		file.open(fileName);
		bool didReadFirstLine = false;
		while (file.good()) {
			std::vector<string> results = getNextLineAndSplitIntoTokens(file, ' ');
			if (didReadFirstLine && results.size() > 6) {
				DrifterModelRef drifter = make_shared<DrifterModel>();
				drifter->setId(results[mDrifter_IdIndex]);
				drifter->setMonth(results[mDrifter_MonthIndex]);
				drifter->setDay(results[mDrifter_DayIndex]);
				drifter->setYear(results[mDrifter_YearIndex]);
				drifter->setLat(results[mDrifter_LatIndex]);
				drifter->setLong(results[mDrifter_LongIndex]);
				drifter->setQualityIndex(results[mDrifter_QualIndexIndex]);
				mDrifterModels.push_back(drifter);
			}
			didReadFirstLine = true;
		}

		CI_LOG_I("Done parsing DRIFTERS:");
		CI_LOG_I(mDrifterModels.size());
		CI_LOG_I("");

	}
	
	void Parser::parseHurricanData()
	{
		std::string fileName = "../assets/data/storm_track_statistics.csv";
		std::ifstream file;
		file.open(fileName);
		bool didReadFirstLine = false;
		while (file.good()) {
			std::vector<string> results = getNextLineAndSplitIntoTokens(file, '\t');
			//CI_LOG_I(results);

			if (didReadFirstLine && results.size() > 7) {
				HurricaneModelRef hurricane = make_shared<HurricaneModel>();
				hurricane->setDate(results[mHurricane_DateIndex]);
				hurricane->setTime(results[mHurricane_TimeIndex]);
				hurricane->setLat(results[mHurricane_LatIndex]);
				hurricane->setLong(results[mHurricane_LongIndex]);
				hurricane->setWind(results[mHurricane_WindIndex]);
				hurricane->setPressure(results[mHurricane_PressureIndex]);
				hurricane->setStormType(results[mHurricane_StormTypeIndex]);
				hurricane->setCategory(results[mHurricane_CategoryIndex]);
				mHurricaneModels.push_back(hurricane);
			}
			didReadFirstLine = true;
		}
		CI_LOG_I("Done parsing HURRICANES:");
		CI_LOG_I(mHurricaneModels.size());
		CI_LOG_I("");

	}

	std::vector<std::string> Parser::getNextLineAndSplitIntoTokens(std::istream& str, const char delimeter)
	{

		std::vector<std::string>   result;
		std::string                line;
		std::getline(str, line);

		std::stringstream          lineStream(line);
		std::string                cell;

		while (std::getline(lineStream, cell, delimeter))
		{
			if (cell != "") {
				result.push_back(cell);
			}
		}
		// This checks for a trailing comma with no data after it.
		if (!lineStream && cell.empty())
		{
			// If there was a trailing comma then add an empty element.
			result.push_back("");
		}
		return result;
	}

}