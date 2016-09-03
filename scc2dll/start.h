#ifndef __START_H
#define __START_H

#include "basicmap.h"
#include "goal.h"
#include "startcondition.h"
#include <list>
#include "group.h"
#include "harvest.h"

using namespace std;
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
	
class START
{
	private:
// ----- Pointers to current basic configuration ------
		const START_CONDITION* startcondition[MAX_PLAYER]; // units are here!
		const GOAL_ENTRY* tmpgoal[MAX_PLAYER];
		const BASIC_MAP* tmpmap;
		const HARVEST_SPEED* harvest[MAX_RACES];
// ----- DATA WHICH IS COPIED EACH GENERATION ON THE PLAYERS -------
		list<GROUP> group[MAX_LOCATIONS]; // ~~~
    	UNIT startForce[MAX_PLAYER][MAX_LOCATIONS];

// ----- GOALS ARE NOT COPIED AS THEY DO NOT CHANGE DURNING A RUN (yet)
		GOAL_ENTRY currentGoal[MAX_PLAYER];

		int startPosition[MAX_PLAYER];
// TODO Funktion zum verteilen von startcondition auf units abhaengig von tmpmap (startposition) und startPositions


// Benutzer waehlt: Karte, Spielmodus und fuegt Spieler mit Rasse, Startforce (default), StartPosition (absolut / zufaellig), Siegbedingungen (goals), 

		
		
		PLAYER startPlayer[MAX_PLAYER]; // goals, startconditions, some initializations etc., STARTFORCE is already in unit and group!
				
	public:
		void assignStartcondition(const int player, const START_CONDITION* startcondition);
		void assignGoal(const GOAL_ENTRY* goal, const int player);
//		void adjustGoals(const int player); ?
		void assignMap(const BASIC_MAP* map);
		void setPlayerRace(const int player, const eRace race);
		const PLAYER* getStartPlayer(const int player) const;
//		const START_CONDITION* getStartcondition(const int player) const; NEIN, NUR INTERN!
		void copyStartForce(void* target);
		const BASIC_MAP** getMap() const;
		const HARVEST_SPEED** getHarvest(const int player) const;
		START();
		~START();
};
#endif

