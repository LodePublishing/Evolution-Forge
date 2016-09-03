#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "soup.h"
#include "main.h"
#include "goal.h"	
#include "harvest.h"
#include "map.h"
#include "ga.h"
#include "default.h"

#define MELEE 0
#define USE_MAP_SETTINGS 1 

// generally all function return 0 if there was an error and 1 if there was no error

class EXPORT SETTINGS
{
private:
	GOAL_ENTRY loadedGoal[MAX_GOAL_ENTRIES];
	MAP loadedMap[MAX_MAPS]; //modes: 0: ignore map settings and make up a default force, 1: use map settings
	HARVEST_SPEED harvestSpeed[3];
	GA ga;
	SOUP soup;
	DEFAULT defaults;
	START start;
	int setMapCount(int num);
	int setGoalCount(int num);
	int currentMap;
	int UMS;
public:
	void calculateAnaplayer();
	void initializeMap();
	MAP* getMap(int num);

	void setMap(int UMS, int num=-1/*, int playerMode=0*/); // evtl noch player dazu
	// 0 = 1vs1, 1 = ... ma guggn...

	int getUMS();
	void adjustGoals(int player);

	void checkForChange();
	void setStartRace(int player, int race);
//all those nasty range checking stuff :-)
	int getMAXBreedFactor();
	int getMAXMode();
	int getMAXCrossOver();
	int getMAXMaxTime();
	int getMAXMaxTimeOut();
	int getMAXMaxLength();
	int getMAXMaxRuns();
	int getMAXMaxGenerations();
	int getMAXPreprocessBuildOrder();
	int getMAXCurrentMap();

	int getMINBreedFactor();
	int getMINMode();
	int getMINCrossOver();
	int getMINMaxTime();
	int getMINMaxTimeOut();
	int getMINMaxLength();
	int getMINMaxRuns();
	int getMINMaxGenerations();
	int getMINPreprocessBuildOrder();
	int getMINCurrentMap();

	int setMaxTime(int num); //sets max Time in minutes
	int setMaxTimeOut(int num); //
	int setAllowGoalAdaption(int num);
	int setMaxLength(int num);
	int setMaxRuns(int num);
	int setMaxGenerations(int num);
	int setPreprocessBuildOrder(int num);
//	int setCurrentMap(int num);
	void setGoal(int goal, int player);
	int setBreedFactor(int num);
	int setMode(int num);
	int setCrossOver(int num);

	int getBreedFactor();
	int getMode();
	int getCrossOver();
	int getMaxTime();
	int getMaxTimeOut();
	int getMaxLength();
	int getMaxRuns();
	int getMaxGenerations();
	int getPreprocessBuildOrder();
//	int getCurrentMap();
	int getCurrentGoal();
	int getHarvestMineralsSpeed(int race, int workers); // get basic mineral harvest speed of <race> with <workers> workers
	int getHarvestGasSpeed(int race, int workers); // get basic mineral harvest speed of <race> with <workers> workers
//	int getDistance(int l1,int l2); // get distance between location 1 and 2
	GA* getGa();
	int getGoalCount();
	int getMapCount();
	GOAL_ENTRY* getGoal(int num);

	void initDefaults();

	void loadDefaultsFile(const char* defaultFile);
	int loadGoalFile(const char* goalFile);
	int loadSettingsFile(const char* settingsFile);
	int loadMapFile(const char* mapFile);
	int loadHarvestFile(const char* harvestFile);
	ANARACE** newGeneration(ANARACE* oldAnarace[MAX_PLAYER]);

	int initSoup();

	SETTINGS();
	~SETTINGS();
};

extern SETTINGS settings;

#endif

