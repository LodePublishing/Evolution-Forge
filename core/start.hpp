#ifndef _CORE_START_HPP
#define _CORE_START_HPP

#include "basicmap.hpp"
#include "goalentry.hpp"
#include "startcondition.hpp"
#include "harvest.hpp"

#include <list>

// this is newly created/changed everytime the startconditions (map, force, goals etc.) change

/*Settingsfiles

Hardcopy version of parsed files

insert Start Player conditions:
	-> START_CONDITION -> map // units
	-> START_CONDITION -> player // startmins, startgas, time, supply etc.
	-> START_CONDITION -> group // units
	-> BASIC_MAP -> map // neutral player, distances etc.
	-> GOAL -> player // goals and/or goal modes
	=> ALL TO 'START' i.e. the temporary conditions.

A RESET will reload the data from the loaded default pointers*/
	
class PLAYER_GROUP
{
	unsigned int player;
	unsigned int count;
	unsigned int type;
	unsigned int location;
};

class START
{
	private:
// ----- Pointers to current basic configuration ------
		const BASIC_MAP* tmpmap;
		const HARVEST_SPEED* harvest[MAX_RACES]; // pointer to harvest data in settings
		const START_CONDITION* startcondition[MAX_PLAYER]; // units are here!
		const GOAL_ENTRY* tmpgoal[MAX_PLAYER]; // basic 
	
// ----- DATA WHICH IS COPIED EACH GENERATION ON THE PLAYERS -------
//		list<GROUP> group[MAX_LOCATIONS]; // ~~~
		
		UNIT totalForce[MAX_PLAYER]; // !!! 0 is neutral player !!! TODO bischen Speicherverschwendung...
		UNIT startForce[MAX_PLAYER][MAX_LOCATIONS]; // !!! 0 is neutral player !!!

// ----- GOALS ARE NOT COPIED AS THEY DO NOT CHANGE DURNING A RUN (yet)
		GOAL_ENTRY currentGoal[MAX_PLAYER]; // initialized things
		unsigned int startPosition[MAX_PLAYER];
		eRace playerRace[MAX_PLAYER];
// TODO Funktion zum verteilen von startcondition auf units abhaengig von tmpmap (startposition) und startPositions

		GOAL_ENTRY* pCurrentGoal[MAX_PLAYER];
		const UNIT_STATISTICS* pStats[MAX_PLAYER];

	        bool startConditionsInitialized;
        	bool mapInitialized;
// Benutzer waehlt: Karte, Spielmodus, Harvestspeed, und fuegt Spieler mit Rasse, Startforce (default), StartPosition (absolut / zufaellig), Siegbedingungen (goals), 
	public:
		const unsigned int getBasicMineralHarvestSpeedPerSecond(const unsigned int playerNum, const unsigned int worker) const; // 'player' noch rausoptimieren!
		const unsigned int getBasicGasHarvestSpeedPerSecond(const unsigned int playerNum, const unsigned int worker) const;
		void assignMap(const BASIC_MAP* map); 
		void setHarvestSpeed(const eRace race, const HARVEST_SPEED* harvest_speed); // copy data (pointers) from settings 
		// assign Mode ?
		void setStartPosition(const unsigned int playerNum, const unsigned int startPosition);
		void setPlayerRace(const unsigned int playerNum, const eRace race); // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
		void assignStartcondition(const unsigned int playerNum, const START_CONDITION* start_condition);
		
		void fillGroups();
		
		void assignGoal(const unsigned int playerNum, const GOAL_ENTRY* goal);

		const UNIT_STATISTICS* const* getpStats(const unsigned int playerNum) const;
		
		const START_CONDITION* const* getStartcondition(const unsigned int playerNum) const; 
		void copyStartForce(void* target) const;
		const BASIC_MAP* const* getMap() const;
		GOAL_ENTRY** getCurrentGoal(const unsigned int playerNum);

		const eRace getPlayerRace(const unsigned int playerNum) const;
	
		START& operator=(const START& start);
		START(const START& start);
		START();
		~START();
};
#endif // _CORE_START_HPP

