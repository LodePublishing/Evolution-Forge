#ifndef _CORE_START_HPP
#define _CORE_START_HPP

#include "basicmap.hpp"
#include "goalentry.hpp"
#include "startcondition.hpp"
#include "harvest.hpp"

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
	public:
		START(UNIT (&start_force)[MAX_LOCATIONS]);
		~START();

		const bool isCompletelyInitialized() const;
		const bool assignMap(const BASIC_MAP* map); 
		const bool setHarvestSpeed(const unsigned int race, const HARVEST_SPEED* harvest_speed); // copy data (pointers) from settings 
		const bool setStartPosition(const unsigned int startPosition);
		const bool setPlayerRace(const unsigned int race); // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
		const bool assignStartCondition(const START_CONDITION* start_condition);
		const bool assignGoal(const GOAL_ENTRY* goal);
		void readjustGoals();

		void fillMapWithStartUnits();

		const std::vector<UNIT_STATISTICS>* getpStats() const;
		
		const START_CONDITION* const* getStartCondition() const; 
		const BASIC_MAP* const* getMap() const;
		GOAL_ENTRY* getCurrentGoal();
		const unsigned int getPlayerRace() const;
//		const unsigned int getBasicMineralHarvestSpeedPerSecond(const unsigned int worker) const; // 'player' noch rausoptimieren!
//		const unsigned int getBasicGasHarvestSpeedPerSecond(const unsigned int worker) const;
	private:

		const unsigned int getStartPosition() const;
// ----- Pointers to current basic configuration ------
		const BASIC_MAP* tmpmap;
		std::vector<const HARVEST_SPEED*> harvest; // pointer to harvest data in settings
		const START_CONDITION* startCondition; // units are here!
		const GOAL_ENTRY* tmpgoal; // basic 
	
// ----- DATA WHICH IS COPIED EACH GENERATION ON THE PLAYERS -------
		
		UNIT (&startForce)[MAX_LOCATIONS];

// ----- GOALS ARE NOT COPIED AS THEY DO NOT CHANGE DURNING A RUN (yet)
		GOAL_ENTRY* currentGoal; // initialized things
		unsigned int startPosition;
		unsigned int playerRace;
		const std::vector<UNIT_STATISTICS>* pStats;
// TODO Funktion zum verteilen von startcondition auf units abhaengig von tmpmap (startposition) und startPositions


		bool mapInitialized;
		std::vector<bool> harvestSpeedInitialized;
		bool startConditionInitialized;
		bool startForceInitialized;
		bool tmpgoalInitialized;
		bool currentGoalInitialized;
		bool startPositionInitialized;
		bool playerRaceInitialized;
		bool pStatsInitialized;
		
// Benutzer waehlt: Karte, Spielmodus, Harvestspeed, und fuegt Spieler mit Rasse, Startforce (default), StartPosition (absolut / zufaellig), Siegbedingungen (goals), 
//		START& operator=(const START& start);
//		START(const START& start);
};

inline const bool START::setHarvestSpeed(const unsigned int race, const HARVEST_SPEED* harvest_speed)
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (START::setHarvestSpeed()): Value race out of range.");return(false);
	}
	if(harvest_speed == NULL) {
		toErrorLog("DEBUG (START::setHarvestSpeed()): Value harvest_speed not initialized.");return(false);
	}
#endif
	if(harvest_speed == harvest[race])
		return(false);
	harvest[race] = harvest_speed;

	harvestSpeedInitialized[race] = true;
	return(true);
}

inline const bool START::setStartPosition(const unsigned int start_position)
{
#ifdef _SCC_DEBUG
	if(mapInitialized == false) {
		toErrorLog("DEBUG (START::setStartPosition()): Map not initialzed.");return(false);
	}
	if((start_position < 1) || (start_position >= tmpmap->getMaxLocations())) {
		toErrorLog("DEBUG (START::setStartPosition()): Value start_position out of range.");return(false);
	}
#endif
	if(startPosition == start_position)
		return(false);
	startPosition = start_position;
	startPositionInitialized = true;
	return(true);
}

inline const unsigned int START::getStartPosition() const {
#ifdef _SCC_DEBUG
	if(startPositionInitialized == false) {
		toErrorLog("DEBUG (START::getStartPosition()): Variable startPosition not initialzed.");return(0);
	}
#endif
	return(startPosition);
}

/*inline const unsigned int START::getBasicMineralHarvestSpeedPerSecond(const unsigned int worker) const // 'player' noch rausoptimieren! 
{
#ifdef _SCC_DEBUG
	if(harvestSpeedInitialized[getPlayerRace()] == false) {
		toErrorLog("DEBUG (START::getBasicMineralHarvestSpeedPerSecond()): Variable harvest not initialized.");return(0);
	}
#endif
	return(harvest[getPlayerRace()]->getHarvestMineralSpeed(worker));
}

inline const unsigned int START::getBasicGasHarvestSpeedPerSecond(const unsigned int worker) const {
#ifdef _SCC_DEBUG
	if(harvestSpeedInitialized[getPlayerRace()] == false) {
		toErrorLog("DEBUG (START::getBasicGasHarvestSpeedPerSecond()): Variable harvest not initialized.");return(0);
	}
#endif
	return(harvest[getPlayerRace()]->getHarvestGasSpeed(worker));
}*/
				
inline const BASIC_MAP* const* START::getMap() const
{
#ifdef _SCC_DEBUG
	if(mapInitialized == false) {
		toErrorLog("DEBUG (START::getMap()): Variable tmpmap not initialized.");
	}
#endif
	return(&tmpmap);
}

inline const START_CONDITION* const* START::getStartCondition() const 
{
#ifdef _SCC_DEBUG
	if(startConditionInitialized == false) {
		toErrorLog("DEBUG (START::getStartCondition()): Variable startCondition not initialized.");
	}
#endif
	return(&(startCondition));
}


inline const unsigned int START::getPlayerRace() const 
{
#ifdef _SCC_DEBUG
	if((playerRaceInitialized == false) || (playerRace >= GAME::MAX_RACES)){
		toErrorLog("DEBUG (START::getPlayerRace()): Variable playerRace not initialized.");return(0);
	}
#endif
	return(playerRace);
}

inline GOAL_ENTRY* START::getCurrentGoal() 
{
#ifdef _SCC_DEBUG
	if(currentGoalInitialized == false) {
		toErrorLog("DEBUG (START::getCurrentGoal()): Variable currentGoal not initialized.");
	}
#endif
	return(currentGoal);
}

inline const std::vector<UNIT_STATISTICS>* START::getpStats() const 
{
#ifdef _SCC_DEBUG
	if(pStatsInitialized == false) {
		toErrorLog("DEBUG (START::getpStats()): Variable pStats not initialized.");
	}
#endif
	return(pStats);
}


#endif // _CORE_START_HPP

