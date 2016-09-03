#ifndef _CORE_SETTINGS_HPP
#define _CORE_SETTINGS_HPP

#include "soup.hpp"
#include "main.hpp"
#include "goal.hpp"	
#include "harvest.hpp"
#include "basicmap.hpp"
#include "ga.hpp"

#include <time.h>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <deque>

// generally all function return 0 if there was an error and 1 if there was no error

class EXPORT SETTINGS
{
private:
	GOAL_ENTRY loadedGoal[MAX_GOAL_ENTRIES];
	START_CONDITION loadedStartcondition[MAX_START_CONDITIONS];
	BASIC_MAP loadedMap[MAX_MAPS]; //modes: 0: ignore map settings and make up a default force, 1: use map settings
	HARVEST_SPEED loadedHarvestSpeed[MAX_HARVEST_SPEEDS];
	GA ga;
	SOUP soup;
	START start;
	
	void setMapCount(const int mapCount);
	void setStartconditionCount(const int startconditionCount);
	void setGoalCount(const int goalCount);
	int currentMap;
public:
	void assignMap(const int mapNumber);
	void setMode(const int mode);
	void setHarvestSpeed(const eRace race, const int harvest); // copy data (pointers) from settings
	void setStartRace(const int player, const eRace race);
    void setStartcondition(const int player, const int startcondition);
	void fillGroups();
	void setStartPosition(const int player, const int startposition);
	void setGoal(const int player, const int goal);

	void calculateAnaplayer();
	const BASIC_MAP* getMap(const int mapNumber) const;
	void checkForChange() const;


	void setMaxTime(const int maxTime); //sets max Time in minutes
	void setMaxTimeOut(const int maxTimeOut); //
	void setAllowGoalAdaption(const bool allowGoalAdaption=true);
	void setMaxLength(const int maxLength);
	void setMaxRuns(const int maxRuns);
	void setMaxGenerations(const int maxGenerations);
	void setPreprocessBuildOrder(const bool preprocess);
//	void setCurrentMap(int num);
	void setBreedFactor(const int breedFactor);
	void setCrossOver(const int crossOver);

	const int getBreedFactor() const;
	const int getMode() const;
	const int getCrossOver() const;
	const int getMaxTime() const;
	const int getMaxTimeOut() const;
	const int getMaxLength() const;
	const int getMaxRuns() const;
	const int getMaxGenerations() const;
	const bool getPreprocessBuildOrder() const;
	const int getCurrentGoal() const;
	const GA* getGa() const;
	const int getGoalCount() const;
	const int getMapCount() const;
	const int getForceCount() const;
	const int getStartconditionCount() const;
	
	const GOAL_ENTRY* getGoal(const int goalNumber) const;

	void initDefaults(); 

	void loadStartconditionFile(const string& startconditionFile);
	void loadGoalFile(const string& goalFile);
	void loadSettingsFile(const string& settingsFile);
	void loadMapFile(const string& mapFile);
	void loadHarvestFile(const string& harvestFile);
// todo evtl error codes...
	
	ANARACE** newGeneration(ANARACE* oldAnarace[MAX_PLAYER]);
	void initSoup(); //?

	SETTINGS();
	~SETTINGS();
};

extern SETTINGS settings;

#endif

