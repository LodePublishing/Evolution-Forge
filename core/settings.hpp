#ifndef _CORE_SETTINGS_HPP
#define _CORE_SETTINGS_HPP

#include "soup.hpp"
#include "goal.hpp"	
#include "harvest.hpp"
#include "basicmap.hpp"
#include "ga.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <deque>

// generally all function return 0 if there was an error and 1 if there was no error

using namespace std;

class SETTINGS
{
private:
	vector<GOAL_ENTRY> loadedGoal[MAX_RACES];
	vector<START_CONDITION> loadedStartcondition[MAX_RACES];
	vector<BASIC_MAP> loadedMap; //modes: 0: ignore map settings and make up a default force, 1: use map settings
	HARVEST_SPEED loadedHarvestSpeed[MAX_RACES];
	GA ga;
	SOUP soup;
	START start;
	
	unsigned int currentMap;
	unsigned int speed;
public:
	const unsigned int getSpeed() const;
	void setSpeed(const unsigned int speed);	

	const bool getIsNewRun();
	
	void assignMap(const unsigned int mapNumber);
	void setHarvestSpeed(const eRace race, const unsigned int harvest); // copy data (pointers) from settings
	void assignStartRace(const unsigned int player, const eRace race);
    void assignStartcondition(const unsigned int player, const unsigned int startcondition);
	void fillGroups();
	void setStartPosition(const unsigned int player, const unsigned int startposition);
	void assignGoal(const unsigned int player, const unsigned int goal);

	void calculateAnaplayer();
	const BASIC_MAP* getMap(const unsigned int mapNumber) const;
	void checkForChange() const;


	void setMaxTime(const unsigned int maxTime); //sets max Time in minutes
	void setMaxTimeOut(const unsigned int maxTimeOut); //
	void setAllowGoalAdaption(const bool allowGoalAdaption=true);
	void setMaxLength(const unsigned int maxLength);
	void setMaxRuns(const unsigned int maxRuns);
	void setMaxGenerations(const unsigned int maxGenerations);
	void setPreprocessBuildOrder(const bool preprocess);
//	void setCurrentMap(int num);
	void setBreedFactor(const unsigned int breedFactor);
	void setCrossOver(const unsigned int crossOver);

	const unsigned int getBreedFactor() const;
	const unsigned int getCrossOver() const;
	const unsigned int getMaxTime() const;
	const unsigned int getMaxTimeOut() const;
	const unsigned int getMaxLength() const;
	const unsigned int getMaxRuns() const;
	const unsigned int getMaxGenerations() const;
	const bool getPreprocessBuildOrder() const;
	GOAL_ENTRY* getCurrentGoal(const unsigned int player);
	const GA* getGa() const;
	const unsigned int getGoalCount(const unsigned int player) const;
	const unsigned int getMapCount() const;
	const unsigned int getStartconditionCount(const unsigned int player) const;
	
	const GOAL_ENTRY* getGoal(const unsigned int player, const unsigned int goalNumber) const;
	const START_CONDITION* getStartcondition(const unsigned int player, const unsigned int startconditionNumber) const;

	void initDefaults(); 

	void loadStartconditionFile(const string& startconditionFile);
	void loadGoalFile(const string& goalFile);
	void loadSettingsFile(const string& settingsFile);
	void loadMapFile(const string& mapFile);
	void loadHarvestFile(const string& harvestFile);
// todo evtl error codes...
	
	ANARACE** newGeneration(ANARACE* oldAnarace[MAX_PLAYER]);
	void assignRunParametersToSoup();

	SETTINGS();
	~SETTINGS();
};

extern SETTINGS settings;

#endif

