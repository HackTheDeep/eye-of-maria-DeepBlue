#include "DataManager.h"

#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace amnh {
	DataManager::DataManager() {}

	DataManager::~DataManager() {}

//		setFieldFromJsonIfExists(&mVideosPreload, "videos.preload");
//		setFieldFromJsonIfExists(&mVideosAssetDir, "videos.assetDir");


	void DataManager::parseDrifterData() {
		std::string fileName = "../assets/data/drifer_data.dat";
		std::ifstream file;
		file.open(fileName);
		bool didReadFirstLine = false;
		while (file.good()) {
			std::vector<string> results = getNextLineAndSplitIntoTokens(file, ' ');
			if (didReadFirstLine && results.size() > 6) {
				string id = results[mDrifter_IdIndex];

				std::map<string, DrifterModel>::iterator it = mDrifterMap.find(id);
				if (it != mDrifterMap.end()) {
					DrifterModel::SampleEvent drifterEvent;
					drifterEvent.latitude = results[mDrifter_LatIndex];
					drifterEvent.longitude = results[mDrifter_LongIndex];
					drifterEvent.qaulityIndex = results[mDrifter_QualIndexIndex];
					it->second.addSampleEvent(drifterEvent);
				}
			}
			didReadFirstLine = true;
		}

		CI_LOG_I("Done parsing DRIFTERS DATA");
	}

	void DataManager::parseDrifterDirectoryData() {
		std::string fileName = "../assets/data/directory.dat";
		std::ifstream file;
		file.open(fileName);
		bool didReadFirstLine = false;
		while (file.good()) {
			std::vector<string> results = getNextLineAndSplitIntoTokens(file, ' ');
			if (didReadFirstLine && results.size() > 16) {
				DrifterModel drifter = DrifterModel();
				drifter.setId(results[mDirectory_IdIndex]);
				mDrifterMap[drifter.getId()] = drifter;

				/*				DrifterDirectoryModelRef model = make_shared<DrifterDirectoryModel>();
				model->setId(results[mDirectory_IdIndex]);
				model->setWmo(results[mDirectory_WmoIndex]);
				model->setExp(results[mDirectory_ExpIndex]);
				model->setFirstDateDay(results[mDirectory_FirstDateDayIndex]);
				model->setFirstDateMonth(results[mDirectory_FirstDateMonthIndex]);
				model->setFirstDateYear(results[mDirectory_FirstDateYearIndex]);
				model->setFirstLat(results[mDirectory_FirstLatIndex]);
				model->setFirstLong(results[mDirectory_FirstLongIndex]);
				model->setEndDateDay(results[mDirectory_EndDateDayIndex]);
				model->setEndDateMonth(results[mDirectory_EndDateMonthIndex]);
				model->setEndDateYear(results[mDirectory_EndDateYearIndex]);
				model->setEndLat(results[mDirectory_EndLatIndex]);
				model->setEndLong(results[mDirectory_EndLongIndex]);
				model->setDropOffDateDay(results[mDirectory_DropOffDateDayIndex]);
				model->setDropOffDateMonth(results[mDirectory_DropOffDateMonthIndex]);
				model->setDropOffDateYear(results[mDirectory_DropOffDateMonthIndex]);
				model->setDeathCode(results[mDirectory_DeathCodeIndex]);

				mDrifterDirectoryModels[model->getId()] = model;*/
			}
			didReadFirstLine = true;
		}


		CI_LOG_I("Done parsing DRIFTER DIRECTORY:");
		CI_LOG_I(mDrifterMap.size());
		CI_LOG_I("");


	}


	// Utility Functions
	std::vector<std::string> DataManager::getNextLineAndSplitIntoTokens(std::istream& str, const char delimeter)
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