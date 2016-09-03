#ifndef __PRERACE_H
#define __PRERACE_H

#include "main.h"
#include "map.h"
#include "player.h"
#include "ga.h"
#include "location.h"
#include "building.h"
#include "blist.h"
#include "start.h"

class EXPORT PRERACE
{
protected:
	static START* start;
	static MAP* pMap; // MAP is all the same for all players using 'start'

//------------------ optimizations:-----------------
	UNITS* global; // non-static pointer to player's global total force (location 0.total)
	UNITS* location; // non-static pointer to players total/availible units
	PLAYER* startPlayer; //pointer to player in start
//------------- end -------------------------------
	
 	BuildingList buildingList;
	static int noise[MAX_TIME];
	static int markerCounter;
	static UNITS units[MAX_PLAYER][MAX_LOCATIONS];
	LAST last[MAX_LENGTH]; // last* is to save the last position, for movements
	int lastcounter;
	int lastunit;
	static int startInitialized;
	int ready;
	const UNIT_STATISTICS* pStats;
	int neededMinerals,neededGas;
	
	void createSpecial();
	void resetSpecial();
	int harvestMinerals();
	int harvestGas();

	int calculateReady();
	void adjustAvailibility(int loc,int fac,const UNIT_STATISTICS* stat);
	int calculatePrimaryFitness(int ready);
	void replaceCode(int IP, int num);

	int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...
private:
//	static int doTransform; // -> a new goal was added so the genetic information is probably wrong... bad thing to do but better than keeping track which goals are already changed
//	static int transformList[UNIT_TYPE_COUNT]; // old geno type -> new geno type
	
	int playerNum;
	int mins,gas,timer;
	int IP;
	int mineralHarvestPerSecond[MAX_LOCATIONS][45];
	int gasHarvestPerSecond[MAX_LOCATIONS][5];
	int harvestedGas,harvestedMins;
	int supply;		// free supply
	int maxSupply; // total supply
	int ftime[MAX_GOALS]; //when the goal is reached / when the last item is produced (ALL locations...*/
	int length,timeout;
	int calculated;

public:
	static void setStartConditions(START* pStart);
	static void initNoise();
	static void initializeMap(); //copies the startforce from map to static 'units'
	
	static int getMapLocationAvailible(int player, int loc, int type);
	static int getMapLocationForce(int player, int loc, int type);
																				
	static int setMapLocationAvailible(int player, int loc, int type, int num);
	static int setMapLocationForce(int player, int loc, int type, int num);
																				
	static int addMapLocationAvailible(int player, int loc, int type, int num);
	static int addMapLocationForce(int player, int loc, int type, int num);
																				
	int getLocationAvailible(int loc, int type);
	int getLocationForce(int loc, int type);
																				
	int setLocationAvailible(int loc, int type, int num);
	int setLocationForce(int loc, int type, int num);
																				
	int addLocationAvailible(int loc, int type, int num);
	int addLocationForce(int loc, int type, int num);


	int Code[MAX_LENGTH];
	int Marker[MAX_LENGTH];

	int setpStats(const UNIT_STATISTICS* pStats);
	const UNIT_STATISTICS* getpStats();

	static MAP* getMap();
	static GA* ga;

	int getPlayerNum();
	void setPlayer(int num); // assigns player data from start (start mins, supply etc.) and sets the appropriate optimized pointers (global, location, pMap etc.) CALL IT AFTER EACH MAP CHANGE AND PLAYER CHANGE!!

	PLAYER* getPlayer();

	int getCalculated();
	int setCalculated(int num);

	void initializePlayer();
	int adjustMineralHarvest(int location);
	int adjustGasHarvest(int location);
	int adjustHarvest();

	int setSupply(int supply);
	int setMaxSupply(int supply);
	int setMins(int mins);
	int setGas(int gas);
	int getSupply();
	int getMaxSupply();
	int getMins();
	int getGas();


	int setTimer(int time);
	int getTimer();

	int setTimeOut(int time);
	int getTimeOut();

	static void resetGeneMarker();
	//int resetSupply();
//	static int setMap(MAP* map);
//	static void resetMapInitialized();

	int getIP();
	int setIP(int IP);

	int setMineralHarvestPerSecond(int tloc,int worker,int mins);
	int getMineralHarvestPerSecond(int tloc,int worker);

	int setGasHarvestPerSecond(int tloc,int worker,int gas);
	int getGasHarvestPerSecond(int tloc,int worker);

	int getHarvestedMins();
	int getHarvestedGas();

	int setHarvestedMins(int mins);
	int setHarvestedGas(int gas);

	int setFinalTime(int goal, int time);
	int getFinalTime(int goal);

	int getLength();
	int setLength(int length);
		
	PRERACE();
	~PRERACE();
};

#endif // __PRERACE_H

