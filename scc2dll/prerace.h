#ifndef __PRERACE_H
#define __PRERACE_H

#include "main.h"
#include "map.h"
#include "player.h"
#include "ga.h"
#include "location.h"

class EXPORT PRERACE
{
	protected:
		static MAP* pMap;
		LAST last[MAX_LENGTH];
		int lastcounter;
		int lastunit;
		static int mapInitialized;
		void harvestResources();
		const UNIT_STATISTICS* pStats;
		int time,ready;
		int timeout;
	private:
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
		int length;
	public:
		static MAP_LOCATION loc[MAX_PLAYER][MAX_LOCATIONS];
		MAP_LOCATION* location;

		int calculated;

	        int Code[2][MAX_LENGTH];
		int setpStats(const UNIT_STATISTICS* pStats);

		static MAP* getMap();
		static GA* ga;

		int getPlayerNum();
		int setPlayerNum(int num);

		int setPlayer(PLAYER* player);
		PLAYER* getPlayer();

		int getCalculated();
		int setCalculated(int num);
		int loadPlayer(int num);
	        int adjustMineralHarvest(int tloc);
        	int adjustGasHarvest(int tloc);
		int adjustHarvest();
		int setSupply(int num);
		int setMaxSupply(int num);
		int setMins(int num);
		int setGas(int num);
		int setTimer(int num);
		int setTimeOut(int num);
		int resetSupply();
		static int setMap(MAP* map);
		static void resetMapInitialized();

		int getIP();
		int setIP(int num);

		int setMineralHarvestPerSecond(int tloc,int worker,int num);
		int getMineralHarvestPerSecond(int tloc,int worker);

		int setGasHarvestPerSecond(int tloc,int worker,int num);
		int getGasHarvestPerSecond(int tloc,int worker);

		int getTimeOut();
		
		int getHarvestedMins();
		int getHarvestedGas();

		int setHarvestedMins(int num);
		int setHarvestedGas(int num);

		int setFinalTime(int goal, int num);
		int getFinalTime(int goal);

		int getLength();
		int setLength(int num);
		
		int getSupply();
		int getMaxSupply();
		int getMins();
		int getGas();
		int getTimer();
		PRERACE();
		~PRERACE();
};

#endif // __PRERACE_H

