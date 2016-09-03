#ifndef __PRERACE_H
#define __PRERACE_H

#include "main.h"
#include "map.h"
#include "player.h"
#include "ga.h"
#include "location.h"
#include "building.h"
#include "wx/list.h"

WX_DECLARE_LIST(Building, BuildingList);

struct LARVACOUNTER
{
	int location;
	int counter; //produce every 20 seconds a new larva
	int larvacount; //do not produce more than 3 larva per larvacounter
};

class EXPORT PRERACE
{
protected:
	static MAP* pMap;
	LAST last[MAX_LENGTH]; // last* is to save the last position, for movements
	int lastcounter;
	int lastunit;
	static int mapInitialized;
	int harvestMinerals();
	int harvestGas();
	const UNIT_STATISTICS* pStats;
	int ready;

	int neededMinerals,neededGas;

	void createSpecial();
	void resetSpecial();

	int larvacounternumber;
	LARVACOUNTER larva[30]; //koennen larvacounter (hatcheries etc.) auch wegfallen? mmmh... naja, wenn sie zerstoert werden... TODO

	int calculateReady();
	void adjustAvailibility(int loc,int fac,const UNIT_STATISTICS* stat);

private:
	static MAP_LOCATION loc[MAX_PLAYER][MAX_LOCATIONS];
	MAP_LOCATION* location;
	PLAYER* player;
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
        BuildingList buildingList;
	//wxList buildingList;

        static int noise[MAX_TIME];

        static int markerCounter;
	static int bestTime; // cancel calculation if this calculation is worse than 25%
																		    
	int getMapLocationAvailible(int player, int loc, int type);
	int getMapLocationForce(int player, int loc, int type);
																			    
	int setMapLocationAvailible(int player, int loc, int type, int num);
	int setMapLocationForce(int player, int loc, int type, int num);
																			    
	int addMapLocationAvailible(int player, int loc, int type, int num);
	int addMapLocationForce(int player, int loc, int type, int num);
																			    

	int getLocationAvailible(int loc, int type);
	int getLocationForce(int loc, int type);
																			    
	int setLocationAvailible(int loc, int type, int num);
	int setLocationForce(int loc, int type, int num);
																			    
	int addLocationAvailible(int loc, int type, int num);
	int addLocationForce(int loc, int type, int num);

	int Code[2][MAX_LENGTH];
	int Marker[2][MAX_LENGTH];

	int setpStats(const UNIT_STATISTICS* pStats);
	const UNIT_STATISTICS* getpStats();

	static MAP* getMap();
	static GA* ga;

	int getPlayerNum();
	int setPlayerNum(int num);

	int setPlayer(PLAYER* player);
	PLAYER* getPlayer();

	int getCalculated();
	int setCalculated(int num);

	int loadPlayer(int num);
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

	int resetSupply();
	static int setMap(MAP* map);
	static void resetMapInitialized();

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

