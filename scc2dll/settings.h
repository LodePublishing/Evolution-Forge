#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "soup.h"
#include "main.h"
#include "goal.h"	
#include "harvest.h"
#include "basicmap.h"
#include "ga.h"

#define MELEE 0
#define USE_MAP_SETTINGS 1 

// generally all function return 0 if there was an error and 1 if there was no error

class EXPORT SETTINGS
{
private:
	GOAL_ENTRY loadedGoal[MAX_GOAL_ENTRIES];
	START_CONDITION loadedStartcondition[MAX_START_CONDITIONS];
	BASIC_MAP loadedMap[MAX_MAPS]; //modes: 0: ignore map settings and make up a default force, 1: use map settings
	HARVEST_SPEED harvestSpeed[MAX_RACES];
	GA ga;
	SOUP soup;
//	DEFAULT defaults; //?
	START start;
	
	void setMapCount(const int mapCount);
	void setStartconditionCount(const int startconditionCount);
	void setGoalCount(const int goalCount);
	int currentMap;
public:
	void calculateAnaplayer();
//	void initializeMap();
	const BASIC_MAP* getMap(const int mapNumber) const;

	void setMap(const int mapNumber);
	// 0 = 1vs1, 1 = ... ma guggn...

//	void adjustGoals(const int player);

	void checkForChange() const;
//	void setStartRace(const int player, const eRace race);
//all those nasty range checking stuff :-)
	const int getMAXBreedFactor() const;
	const int getMAXMode() const;
	const int getMAXCrossOver() const;
	const int getMAXMaxTime() const;
	const int getMAXMaxTimeOut() const;
	const int getMAXMaxLength() const;
	const int getMAXMaxRuns() const;
	const int getMAXMaxGenerations() const;
	const int getMAXCurrentMap() const;

	const int getMINBreedFactor() const;
	const int getMINMode() const;
	const int getMINCrossOver() const;
	const int getMINMaxTime() const;
	const int getMINMaxTimeOut() const;
	const int getMINMaxLength() const;
	const int getMINMaxRuns() const;
	const int getMINMaxGenerations() const;
	const int getMINCurrentMap() const;

	void setMaxTime(const int maxTime); //sets max Time in minutes
	void setMaxTimeOut(const int maxTimeOut); //
	void setAllowGoalAdaption(const bool allowGoalAdaption=true);
	void setMaxLength(const int maxLength);
	void setMaxRuns(const int maxRuns);
	void setMaxGenerations(const int maxGenerations);
	void setPreprocessBuildOrder(const bool preprocess);
//	void setCurrentMap(int num);
	void setGoal(const int goal, const int player);
	void setBreedFactor(const int breedFactor);
	void setMode(const int mode);
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
//	int getCurrentMap();
	const int getCurrentGoal() const;
	const int getHarvestMineralsSpeed(const eRace race, const int workers) const; // get basic mineral harvest speed of <race> with <workers> workers
	const int getHarvestGasSpeed(const eRace race, const int workers) const; // get basic mineral harvest speed of <race> with <workers> workers
//	int getDistance(int l1,int l2); // get distance between location 1 and 2
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

