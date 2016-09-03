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
	
class START
{
	private:
// ----- Pointers to current basic configuration ------
		const BASIC_MAP* tmpmap;
		const HARVEST_SPEED* harvest[MAX_RACES]; // pointer to harvest data in settings
		const START_CONDITION* startCondition; // units are here!
		const GOAL_ENTRY* tmpgoal; // basic 
	
// ----- DATA WHICH IS COPIED EACH GENERATION ON THE PLAYERS -------
//		list<GROUP> group[MAX_LOCATIONS]; // ~~~
		
		UNIT totalForce; // !!! 0 is neutral player !!! TODO bischen Speicherverschwendung...
		UNIT (*startForce)[MAX_LOCATIONS]; // !!! 0 is neutral player !!!

// ----- GOALS ARE NOT COPIED AS THEY DO NOT CHANGE DURNING A RUN (yet)
		GOAL_ENTRY currentGoal; // initialized things
		unsigned int startPosition;
		eRace playerRace;
		const UNIT_STATISTICS* pStats;
// TODO Funktion zum verteilen von startcondition auf units abhaengig von tmpmap (startposition) und startPositions


		bool startConditionsInitialized;
		bool mapInitialized;
// Benutzer waehlt: Karte, Spielmodus, Harvestspeed, und fuegt Spieler mit Rasse, Startforce (default), StartPosition (absolut / zufaellig), Siegbedingungen (goals), 

	public:
		START(UNIT (*start_force)[MAX_LOCATIONS]);
		~START();
		START& operator=(const START& start);
		//START(const START& start);

		const bool assignMap(const BASIC_MAP* map); 
		const bool setHarvestSpeed(const eRace race, const HARVEST_SPEED* harvest_speed); // copy data (pointers) from settings 
		// assign Mode ?
		const bool setStartPosition(const unsigned int startPosition);
		const bool setPlayerRace(const eRace race); // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
		const bool assignStartCondition(const START_CONDITION* start_condition);
		const bool assignGoal(const GOAL_ENTRY* goal);

		void fillAsActivePlayer();
		void fillAsNeutralPlayer();

		const UNIT_STATISTICS* const* getpStats() const;
		const START_CONDITION* const* getStartCondition() const; 
//		void copyStartForce(UNIT (*target)[MAX_LOCATIONS]) const;
		const BASIC_MAP* const* getMap() const;
		GOAL_ENTRY* getCurrentGoal();
		const eRace getPlayerRace() const;
		const unsigned int getBasicMineralHarvestSpeedPerSecond(const unsigned int worker) const; // 'player' noch rausoptimieren!
		const unsigned int getBasicGasHarvestSpeedPerSecond(const unsigned int worker) const;
};

inline const bool START::setHarvestSpeed(const eRace race, const HARVEST_SPEED* harvest_speed)
{
	if(harvest_speed == harvest[race])
		return(false);
	harvest[race]=harvest_speed;
	return(true);
}

inline const unsigned int START::getBasicMineralHarvestSpeedPerSecond(const unsigned int worker) const // 'player' noch rausoptimieren! 
{
	return(harvest[playerRace]->getHarvestMineralSpeed(worker));
}

inline const unsigned int START::getBasicGasHarvestSpeedPerSecond(const unsigned int worker) const {
	return(harvest[playerRace]->getHarvestGasSpeed(worker));
}
				
inline const BASIC_MAP* const* START::getMap() const
{
	return(&tmpmap);
}

inline const START_CONDITION* const* START::getStartCondition() const
{
	return(&(startCondition));
}

inline const bool START::setStartPosition(const unsigned int start_position)
{
	if(startPosition == start_position)
		return(false);
#ifdef _SCC_DEBUG
	if((start_position < 1) || (start_position >= tmpmap->getMaxLocations())) {
		toLog("DEBUG: (START::setStartPosition): Value start_position out of range.");return(false);
	}
#endif
	startPosition = start_position;
	return(true);
}

inline const eRace START::getPlayerRace() const {
	return(playerRace);
}

inline GOAL_ENTRY* START::getCurrentGoal() {
	return(&(currentGoal));
}

inline const UNIT_STATISTICS* const* START::getpStats() const {
	return(&(pStats));
}


#endif // _CORE_START_HPP

