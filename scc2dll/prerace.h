#ifndef __PRERACE_H
#define __PRERACE_H

#include "main.h"
#include "basicmap.h"
#include "player.h"
#include "ga.h"
#include "location.h"
#include "building.h"
#include "start.h"
#include "blist.h"

using namespace std;

class EXPORT PRERACE
{
protected:
	static START* start;

//------------------ optimizations:-----------------
	UNIT* global; // non-static pointer to player's global total force (location 0.total)
	UNIT* location; // non-static pointer to players total/availible units
	const PLAYER* startPlayer; //pointer to player in start

	const static BASIC_MAP* pMap; // MAP is all the same for all players using 'start'
//------------- end -------------------------------
	
 	//multimap<int, Building> buildingList; // key is time
	BuildingList buildingList;
	
	static int noise[MAX_TIME];
	static int markerCounter;
	static UNIT unit[MAX_PLAYER][MAX_LOCATIONS];
	LAST last[MAX_LENGTH]; // last* is to save the last position, for movements
	int lastcounter;
	int lastunit;
	static int startInitialized;
	bool ready;
	const UNIT_STATISTICS* pStats;
	int neededMinerals,neededGas;
	
	void createSpecial();
	void resetSpecial();
	const int harvestMinerals() const;
	const int harvestGas() const; 

	const bool calculateReady() const;
	void adjustAvailibility(const int loc, const int fac, const UNIT_STATISTICS* stat);
	const int calculatePrimaryFitness(const bool ready) const;
	void replaceCode(const int IP, const int num);

	int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...
private:
//	static int doTransform; // -> a new goal was added so the genetic information is probably wrong... bad thing to do but better than keeping track which goals are already changed
//	static int transformList[UNIT_TYPE_COUNT]; // old geno type -> new geno type
	
	int playerNum;
	int minerals,gas,timer;
	int IP;
	int mineralHarvestPerSecond[MAX_LOCATIONS][45];
	int gasHarvestPerSecond[MAX_LOCATIONS][5];
	int harvestedGas,harvestedMinerals;
	int supply;		// free supply
	int maxSupply; // total supply
	int ftime[MAX_GOALS]; //when the goal is reached / when the last item is produced (ALL locations...*/
	int length,timeout;
	int calculated;

public:
	static void setStartConditions(START* pStart);
	static void initNoise();
	static void copyMap(); //copies the startforce from map to static 'units'
	
	const static int getMapLocationAvailible(const int player, const int loc, const int unittype);
	const static int getMapLocationTotal(const int player, const int loc, const int unittype);
																				
	static void setMapLocationAvailible(const int player, const int loc, const int unittype, const int num);
	static void setMapLocationTotal(const int player, const int loc, const int unittype, const int num);
																				
	static void addMapLocationAvailible(const int player, const int loc, const int unittype, const int num);
	static void addMapLocationTotal(const int player, const int loc, int unittype, const int num);
																				
	const int getLocationAvailible(const int loc, const int unittype) const;
	const int getLocationTotal(const int loc, const int unittype) const;
																				
	void setLocationAvailible(const int loc, int unittype, int num);
	void setLocationTotal(int loc, const int unittype, int num);
																				
	void addLocationAvailible(const int loc, const int unittype, const int num);
	void addLocationTotal(const int loc, const int unittype, const int num);

	int Code[MAX_LENGTH];
	int Marker[MAX_LENGTH];

	void setpStats(const UNIT_STATISTICS* pStats);
	const UNIT_STATISTICS* getpStats() const;

	const static BASIC_MAP** getMap();
	static GA* ga;

	const int getPlayerNum() const;
	void setPlayer(const int num); // assigns player data from start (start minerals, supply etc.) and sets the appropriate optimized pointers (global, location, pMap etc.) CALL IT AFTER EACH MAP CHANGE AND PLAYER CHANGE!!

	const PLAYER* getPlayer() const;

	const int getCalculated() const; 
	void setCalculated(const int num);

	void initializePlayer();
	void adjustMineralHarvest(const int location);
	void adjustGasHarvest(const int location);
	void adjustHarvest();

	void setNeedSupply(const int needSupply);
	void setHaveSupply(const int haveSupply);
	void setMinerals(const int minerals);
	void setGas(const int gas);

	const int getNeedSupply() const;
	const int getHaveSupply() const;
	const int getMinerals() const;
	const int getGas() const;


	void setTimer(const int time);
	const int getTimer() const;

	void setTimeOut(const int time);
	const int getTimeOut() const;

	static void resetGeneMarker();
	//int resetSupply();
//	static int setMap(MAP* map);
//	static void resetMapInitialized();

	const int getIP() const;
	void setIP(const int IP);

	void setMineralHarvestPerSecond(const int tloc, const int worker, const int minerals);
	const int getMineralHarvestPerSecond(const int tloc, const int worker) const;

	void setGasHarvestPerSecond(const int tloc, const int worker, const int gas);
	const int getGasHarvestPerSecond(const int tloc, const int worker) const;

	const int getHarvestedMinerals() const;
	const int getHarvestedGas() const;

	void setHarvestedMinerals(const int minerals);
	void setHarvestedGas(const int gas);

	void setFinalTime(const int goal, const int time);
	const int getFinalTime(const int goal) const;

	const int getLength() const;
	void setLength(const int length);
		
	PRERACE();
	~PRERACE();
};

#endif // __PRERACE_H

