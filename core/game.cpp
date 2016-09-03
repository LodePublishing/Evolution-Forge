#include "game.hpp"


GAME::GAME()
{}

GAME::~GAME()
{}

const std::string CORE_VERSION="Beta 1.80";

const bool GAME::setLanguage(const eLanguage language)
{
	if((languageInitializedForGameStrings.size()<=language) || (!languageInitializedForGameStrings[language]))
		return(false);
	for(unsigned int i = MAX_RACES; i--;)
		if((race.size() < MAX_RACES) || (race[i].languageInitializedForUnitsStrings.size() < language) || (!race[i].languageInitializedForUnitsStrings[language]))
			return(false);
	currentLanguage = language;
	return(true);
}

void GAME::init()
{}

void GAME::initGameStrings()
{
// ------ GAME STRINGS ------
	{
		languageInitializedForGameStrings.assign(MAX_LANGUAGES, false);
		gameStringList.resize(MAX_LANGUAGES);
		std::list<std::string> string_files = findFiles(gameDirectory, "strings", "game", "");
		for(std::list<std::string>::iterator j = string_files.begin(); j != string_files.end(); ++j)
			loadStringFile(*j, gameStringIdentifier, gameStringList, languageInitializedForGameStrings, MAX_GAME_STRINGS);
	}
}

void GAME::initRaceStrings()
{
// ------ END GAME STRINGS ------
// ------ UNIT STRINGS ------

	for(unsigned int i = MAX_RACES; i--;)
	{
		race[i].languageInitializedForUnitsStrings.resize(MAX_LANGUAGES);
		race[i].languageInitializedForUnitsStrings.assign(MAX_LANGUAGES, false);
		race[i].unitsStringList.resize(MAX_LANGUAGES);
		std::list<std::string> string_files = findFiles(gameDirectory, "strings", "units", race[i].raceString);
		for(std::list<std::string>::iterator j = string_files.begin(); j != string_files.end(); ++j)
			loadStringFile(*j, race[i].unitsStringIdentifier, race[i].unitsStringList, race[i].languageInitializedForUnitsStrings, RACE::UNIT_TYPE_COUNT);
	}

}

const std::string& GAME::lookUpGameString(const unsigned int game_string_id)
{
#ifdef _SCC_DEBUG
	if(game_string_id >= MAX_GAME_STRINGS) {
		toErrorLog("ERROR (GAME::lookUpGameString()): Value game_string_id out of range.");return(gameStringList[currentLanguage][0]);
	}
#endif
	return(gameStringList[currentLanguage][game_string_id]);
}

void GAME::initResources()
{
	for(unsigned int i = MAX_RACES; i--;)
		race[i].initResources();
}


void GAME::initStringIdentifier()
{}

unsigned int GAME::MAX_GAME_TYPES = 0;
unsigned int GAME::MAX_RACES = 0;
unsigned int GAME::MAX_SUPPLY = 0;
unsigned int GAME::MAX_TOTAL_UNITS = 0;
unsigned int GAME::MAX_RESOURCES = 0;
unsigned int GAME::CANCELING_BUILDING_RETURN = 0;

eLanguage GAME::currentLanguage = ZERO_LANGUAGE;
std::string GAME::gameDirectory = "";

std::vector<RACE> GAME::race;
std::vector<unsigned int> GAME::gameSpeedFrames;
std::vector<std::string> GAME::gameStringIdentifier;
std::vector< std::vector<std::string> > GAME::gameStringList; // languages
std::vector<bool> GAME::languageInitializedForGameStrings;

