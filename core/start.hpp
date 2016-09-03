#ifndef _CORE_START_HPP
#define _CORE_START_HPP

#include "basicmap.hpp"
#include "goal.hpp"
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
	
class GROUP
{
	int player;
	int count;
	int type;
	int location;
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
		
		int totalForce[MAX_PLAYER][UNIT_TYPE_COUNT]; // !!! 0 is neutral player !!!
		UNIT startForce[MAX_PLAYER][MAX_LOCATIONS]; // !!! 0 is neutral player !!!

// ----- GOALS ARE NOT COPIED AS THEY DO NOT CHANGE DURNING A RUN (yet)
		GOAL_ENTRY currentGoal[MAX_PLAYER]; // initialized things
		int startPosition[MAX_PLAYER];
		eRace playerRace[MAX_PLAYER];
// TODO Funktion zum verteilen von startcondition auf units abhaengig von tmpmap (startposition) und startPositions

		GOAL_ENTRY* pCurrentGoal[MAX_PLAYER];
		const UNIT_STATISTICS* pStats[MAX_PLAYER];

// Benutzer waehlt: Karte, Spielmodus, Harvestspeed, und fuegt Spieler mit Rasse, Startforce (default), StartPosition (absolut / zufaellig), Siegbedingungen (goals), 
	public:
		const int getBasicMineralHarvestSpeedPerSecond(const int player, const int worker) const; // 'player' noch rausoptimieren!
		const int getBasicGasHarvestSpeedPerSecond(const int player, const int worker) const;
		void assignMap(const BASIC_MAP* map); 
		void setHarvestSpeed(const eRace race, const HARVEST_SPEED* harvest); // copy data (pointers) from settings 
		// assign Mode ?
		void setStartPosition(const int player, const int startPosition);
		void setPlayerRace(const int player, const eRace race); // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
		void setStartcondition(const int player, const START_CONDITION* startcondition);
		
		void fillGroups();
		
		void setGoal(const int player, const GOAL_ENTRY* goal);

		const UNIT_STATISTICS* const* getpStats(const int playerNum);
		
		const START_CONDITION* const* getStartcondition(const int player) const; 
		void copyStartForce(void* target);
		const BASIC_MAP* const* getMap() const;
		GOAL_ENTRY** getCurrentGoal(const int playerNum);

		START();
		~START();
};
#endif // _CORE_START_HPP

