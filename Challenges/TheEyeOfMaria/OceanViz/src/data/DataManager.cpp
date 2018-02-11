#include "DataManager.h"

#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace amnh {
	DataManager::DataManager() {
		mMinTimeStamp = time(0);
		mMaxTimeStamp = -1;

	}

	DataManager::~DataManager() {}

	void DataManager::setup() {
		parseDrifterDirectoryData();
		parseDrifterData();
		parseHurricanData();
	};

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
					// Parse some data
					string dateString = getDrifterDateString(results[mDrifter_YearIndex], results[mDrifter_MonthIndex], results[mDrifter_DayIndex]);
					auto timeStamp = dateStringToTimestamp(dateString);
					if (timeStamp > mMaxTimeStamp) { mMaxTimeStamp = timeStamp; }
					if (timeStamp < mMinTimeStamp) { mMinTimeStamp = timeStamp; }

					// Create new model struct and populate properties
					DrifterModel::SampleEvent drifterEvent;
					drifterEvent.latitude = std::stof(results[mDrifter_LatIndex]);
					drifterEvent.longitude = std::stof(results[mDrifter_LongIndex]);
					drifterEvent.qaulityIndex = std::stof(results[mDrifter_QualIndexIndex]);
					drifterEvent.time = timeStamp;
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
			}
			didReadFirstLine = true;
		}


		CI_LOG_I("Done parsing DRIFTER DIRECTORY:");
		CI_LOG_I(mDrifterMap.size());
		CI_LOG_I("");
	}

	void DataManager::parseHurricanData()
	{
		std::string fileName = "../assets/data/storm_track_statistics.csv";
		std::ifstream file;
		file.open(fileName);
		bool didReadFirstLine = false;
		HurricaneModel hurricane = HurricaneModel(); // Hard coded because we're only handling one hurricane
		hurricane.setId("Maria");

		while (file.good()) {
			std::vector<string> results = getNextLineAndSplitIntoTokens(file, '\t');
			//CI_LOG_I(results);

			if (didReadFirstLine && results.size() > 7) {
				string dateString = getHurricaneDateString(results[mHurricane_DateIndex], results[mHurricane_TimeIndex]);
				auto timestamp = dateStringToTimestamp(dateString);
				std::string  latString = results[mHurricane_LatIndex].substr(0, results[mHurricane_LatIndex].length() - 2); //Trim special Chars
				std::string  longString = results[mHurricane_LongIndex].substr(0, results[mHurricane_LongIndex].length() - 2); //Trim sepcial Chars
				std::string  windString = results[mHurricane_WindIndex].substr(0, results[mHurricane_WindIndex].find(" ")); //Trim mph
				std::string  pressureString = results[mHurricane_PressureIndex].substr(0, results[mHurricane_PressureIndex].find(" ")); //Trim mb

				HurricaneModel::SampleEvent hurricaneEvent = HurricaneModel::SampleEvent();
				hurricaneEvent.timestamp = timestamp;
				hurricaneEvent.latitude = stof(latString);
				hurricaneEvent.longitude = std::stof(longString);
				hurricaneEvent.category = results[mHurricane_CategoryIndex];
				hurricaneEvent.wind = stof(windString);
				hurricaneEvent.stormType = results[mHurricane_StormTypeIndex];
				hurricane.addSampleEvent(hurricaneEvent);
				
			}
			didReadFirstLine = true;
		}
		mHurricaneModels.push_back(hurricane);
		CI_LOG_I("Done parsing HURRICANES:");
		CI_LOG_I(mHurricaneModels.size());
		CI_LOG_I("");

	}


	/////////////////////////////////////
	//	Utility Functions
	//
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

	
	unsigned long DataManager::dateStringToTimestamp(std::string datetime) {
		if (datetime.length()<19) { 
			std::cout << "invalid string - cant convert to timestamp"; 
			return -1;
		}

		struct tm tm;
		tm.tm_year = atoi(datetime.substr(0, 4).c_str()) - 1900;
		tm.tm_mon = atoi(datetime.substr(5, 2).c_str()) - 1;
		tm.tm_mday = atoi(datetime.substr(8, 2).c_str());
		tm.tm_hour = atoi(datetime.substr(11, 2).c_str());
		tm.tm_min = atoi(datetime.substr(14, 2).c_str());
		tm.tm_sec = atoi(datetime.substr(17, 2).c_str());

		char buff[80];
		strftime(buff, 80, "%Y.%m.%d %H:%M:%S", &tm);
		return mktime(&tm);
	}

	string DataManager::getDrifterDateString(std::string year, std::string month, std::string day) {
		float monthFloat = std::stof(month);
		string timeString_month = (monthFloat >= 10) ? to_string(int(monthFloat)) : "0" + to_string(int(monthFloat));
		float dayFloat = std::stof(day);
		string timeString_day = to_string(int(dayFloat));
		timeString_day = (dayFloat > 10) ? timeString_day : "0" + timeString_day;
		float dayDecimal = dayFloat - int(dayFloat);
		float hourFloat = dayDecimal * 24;
		string timeString_hour = to_string(int(hourFloat));
		timeString_hour = (hourFloat >= 10) ? timeString_hour : "0" + timeString_hour;
		float minutesDecimal = hourFloat - int(hourFloat);
		float minutesFloat = minutesDecimal * 60;
		string timeString_minutes = to_string(int(minutesFloat));
		timeString_minutes = (minutesFloat >= 10) ? timeString_minutes : "0" + timeString_minutes;
		float secondsDecimal = minutesFloat - int(minutesFloat);
		float secondsFloat = secondsDecimal * 60;
		string timeString_seconds = to_string(int(secondsFloat));
		timeString_seconds = (secondsFloat >= 10) ? timeString_seconds : "0" + timeString_seconds;

		string timeString = year + "." + timeString_month + "." + timeString_day + " " + timeString_hour + ":" + timeString_minutes + ":" + timeString_seconds;
		return timeString;
	}

	std::string DataManager::getHurricaneDateString(std::string date, std::string time) {
		std::string timeString_month = "09"; // Hardcoded because Maria only took place in september 
		std::string timeString_seconds = "00"; // Hardcoded because Maria Data only reports hours and minutes
		std::string timeString_year = "2017"; // Hardcoded because Maria Data only reports hours and minutes

		int day = stoi(date.substr(0, date.find("-")));
		std::string timeString_day = (day >= 10) ? to_string(day) : "0" + to_string(day);
		std::string hurricaneTime = time.substr(0, time.find(" "));
		std::string timeString_hour = time.substr(0, 2);
		std::string timeString_minutes = time.substr(3, 2);
		string timeString = timeString_year + "." + timeString_month + "." + timeString_day + " " + timeString_hour + ":" + timeString_minutes + ":" + timeString_seconds;
		return timeString;
	}


	std::string DataManager::getDateStringFromTimestamp(time_t timestamp) {
		std::tm * ptm = std::localtime(&timestamp);
		string year = to_string(1900 + ptm->tm_year);
		string month = (ptm->tm_mon >= 10) ? to_string(ptm->tm_mon + 1) : "0" + to_string(ptm->tm_mon + 1);
		string day = (ptm->tm_mday >= 10) ? to_string(ptm->tm_mday) : "0" + to_string(ptm->tm_mday);
		string hour = (ptm->tm_hour >= 10) ? to_string(ptm->tm_hour) : "0" + to_string(ptm->tm_hour);
		string minutes = (ptm->tm_min >= 10) ? to_string(ptm->tm_min) : "0" + to_string(ptm->tm_min);
		string seconds = (ptm->tm_sec >= 10) ? to_string(ptm->tm_sec) : "0" + to_string(ptm->tm_sec);
		string dateString = year + "-" + month + "-" + day + " " + hour + ":" + minutes + ":" + seconds;
		return dateString;
	}



}