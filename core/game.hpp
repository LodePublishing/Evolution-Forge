#ifndef _CORE_GAME_HPP
#define _CORE_GAME_HPP

#include "race.hpp"
#include "../stl/misc.hpp"

extern const std::string CORE_VERSION;

#define GLOBAL 0
#define MAIN 1
#define MAX_LOCATIONS 2
#define MIN_LOCATIONS 2 

#define MAX_TIME 54000
#define MIN_TIME 4500

#define MAX_TIMEOUT 4000
#define MIN_TIMEOUT 300

#define MAX_TFITNESS 99999
#define MAX_PFITNESS 99999

#define MAX_PROGRAMS 64

#define MIN_LENGTH 32
#define MAX_LENGTH 96

// max simultaneous players
#define MAX_PLAYER 1
#define MIN_PLAYER 1

// TODO allgemeiner als Mins/Gas
enum eErrorMessages
{
	OK,
	ENOUGH_MINERALS_WERE_GATHERED,
	ENOUGH_GAS_WAS_GATHERED,
	SUPPLY_WAS_SATISFIED,
	PREREQUISITE_WAS_FULFILLED,
	FACILITY_BECAME_AVAILIBLE,
	TIMEOUT_ERROR,
	UNKNOWN_ERROR,
	ERROR_MESSAGES
};



class GAME
{
	public:
		GAME();
		virtual ~GAME();

		// ARRR P-) Never write over these variables until I found a way to make them const! maybe with protected set routines...
		static unsigned int MAX_GAME_TYPES;
		static unsigned int MAX_RACES;
		static unsigned int MAX_SUPPLY;
		static unsigned int MAX_TOTAL_UNITS;
		static unsigned int MAX_RESOURCES;
		
		enum eGameStrings
		{
			NULL_STRING = 0,
			FIRST_RACE_STRING = 1,
			FIRST_RESOURCE_STRING = 10,
			FIRST_UNIT_MENU_STRING = 20,
			FIRST_GAME_SPEED_STRING = 40
		};

		
		static unsigned int MAX_GAME_STRINGS;

		static unsigned int CANCELING_BUILDING_RETURN;

		static std::string gameDirectory;
// -----
		

		static std::vector<RACE> race;
		static std::vector<unsigned int> gameSpeedFrames;

		static eLanguage currentLanguage;
		
					
		
		static std::vector<std::string> gameStringIdentifier;
		static std::vector< std::vector<std::string> > gameStringList; // languages
		static const std::string& lookUpGameString(const unsigned int game_string_id);
		static const std::string& lookUpUnitString(const unsigned int unit_race, const unsigned int unit_string_id);
		static std::vector<bool> languageInitializedForGameStrings;

		static void initResources();

		virtual void initStringIdentifier();
		

		static const bool setLanguage(const eLanguage language);

		virtual void init();

	protected:
	private:
};


inline const std::string& GAME::lookUpUnitString(const unsigned int unit_race, const unsigned int unit_string_id)
{
#ifdef _SCC_DEBUG
	if(unit_race >= MAX_RACES) {
		toErrorLog("ERROR (GAME::lookUpUnitString()): Value unit_race out of range.");return(race[0].unitsStringList[currentLanguage][0]);
	}
	if(unit_string_id >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("ERROR (GAME::lookUpUnitString()): Value unit_string_id out of range.");return(race[unit_race].unitsStringList[currentLanguage][0]);
	}
#endif
	return(race[unit_race].unitsStringList[currentLanguage][unit_string_id]);
}

inline const std::string& GAME::lookUpGameString(const unsigned int game_string_id)
{
#ifdef _SCC_DEBUG
	if(game_string_id >= MAX_GAME_STRINGS) {
		toErrorLog("ERROR (GAME::lookUpGameString()): Value game_string_id out of range.");return(gameStringList[currentLanguage][0]);
	}
#endif
	return(gameStringList[currentLanguage][game_string_id]);
}




#endif

