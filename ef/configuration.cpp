#include "configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

const unsigned int MAX_GENERATIONS = 10000;
const unsigned int MIN_GENERATIONS = 100;

EF_Configuration::EF_Configuration():
	autoRuns(false),
	waitAfterChange(false),
	compactDisplayMode(false),
	facilityMode(true),
	softwareMouse(false),
	raceSpecificTheme(true),
	dnaSpiral(true),
	toolTips(true),
	showDebug(false),
	maxGenerations(MAX_GENERATIONS-1),
	gameSpeed(3),
	configurationFile("settings/main.cfg")
{ }

EF_Configuration::~EF_Configuration()
{ }

void EF_Configuration::initDefaults()
{
	setAutoRuns(false);
	setWaitAfterChange(false);
	setCompactDisplayMode(false);
	setFacilityMode(true);
	setRaceSpecificTheme(true);
	setDnaSpiral(true);
	setToolTips(true);
	setShowDebug(false);
	setMaxGenerations(MAX_GENERATIONS-1);
	setGameSpeed(3);
	configurationFile = "settings/main.cfg";
}

void EF_Configuration::setConfigurationFile(const std::string& configuration_file)
{
	configurationFile = configuration_file;
}

void EF_Configuration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!checkStreamIsOpen(pFile, "EF_Configuration::saveToFile", configurationFile))
		return;
	pFile << "@SETTINGS" << std::endl;
	pFile << "# Do autosave at the end of a run or ask for it?" << std::endl;
	pFile << "    \"Auto runs\" = \"" << (int)isAutoRuns() << "\"" << std::endl;
	pFile << "    \"Max unchanged Generations\" = \"" << getMaxGenerations() << "\"" << std::endl;
	pFile << "# Desired framerate: If the computer is fast the calculation speed is improved, if the computer is slow the calculation speed is decreased" << std::endl;
	pFile << "# Wait briefly after each change in the build order or just progress as fast as possible? (1 / 0)" << std::endl;
	pFile << "    \"Wait after change\" = \"" << (int)isWaitAfterChange() << "\"" << std::endl;
	pFile << "# Display entries in the build order window compact (i.e. '6x Zergling' instead of Zergling, Zergling, Zergling, ...)" << std::endl;
	pFile << "    \"Compact display mode\" = \"" << (int)isCompactDisplayMode() << "\"" << std::endl;
	pFile << "# Order entries in the unitmenu by area or by facility?" << std::endl;
	pFile << "    \"Facility mode\" = \"" << (int)isFacilityMode() << "\"" << std::endl;
	pFile << "# Show nice DNA spiral?" << std::endl;
	pFile << "    \"DNA Spiral\" = \"" << (int)isDnaSpiral() << "\"" << std::endl;
	pFile << "# use global theme (0) or race specific themes + the global theme (1)?" << std::endl;
	pFile << "    \"Race specific theme\" = \"" << (int)isRaceSpecificTheme() << "\"" << std::endl;
	pFile << "    \"Tooltips\" = \"" << (int)isToolTips() << "\"" << std::endl;
	pFile << "# show which part of the program needs how much CPU resources" << std::endl;
	pFile << "    \"Show debug\" = \"" << (int)isShowDebug() << "\"" << std::endl;
        pFile << "# Game speed (0 = slowest, 6 = fastest)" << std::endl;
        pFile << "\"Game speed\" = \"" << getGameSpeed() << "\"" << std::endl;
	pFile << "@END" << std::endl;
}
	
void EF_Configuration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	if(!checkStreamIsOpen(pFile, "EF_Configuration::loadConfigurationFile()", configurationFile))
	{
		toErrorLog("-> Creating new file with default values...");
		initDefaults();
		saveToFile();
		return;
	}

	toInitLog("* Loading " + configurationFile);
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "EF_Configuration::loadConfigurationFile()", configurationFile))
			return;
		std::string text = line;
		size_t start=text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		size_t stop=text.find_first_of("\t ", start);
		if(stop==std::string::npos) stop = text.size();
		std::string index=text.substr(start, stop);
		std::string value;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@SETTINGS")
		{
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			if(!parse_block_map(pFile, block))
			{
#ifdef _SCC_DEBUG
				toErrorLog("WARNING: (EF_Configuration::loadConfigurationFile) No concluding @END was found!");
#endif
			}			
				

			if((i=block.find("Auto runs"))!=block.end()) {
				i->second.pop_front();
			   	setAutoRuns(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max generations"))!=block.end()) {
				i->second.pop_front();
			   	setMaxGenerations(atoi(i->second.front().c_str()));
			}

			if((i=block.find("Wait after change"))!=block.end()){
				i->second.pop_front();
			   	setWaitAfterChange(atoi(i->second.front().c_str()));
			}
	
			if((i=block.find("Compact display mode"))!=block.end()){
				i->second.pop_front();
			   	setCompactDisplayMode(atoi(i->second.front().c_str()));
			}
		
			if((i=block.find("Facility mode"))!=block.end()){
				i->second.pop_front();
			   	setFacilityMode(atoi(i->second.front().c_str()));
			}
	
			if((i=block.find("Tooltips"))!=block.end()){
				i->second.pop_front();
				setToolTips(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Show debug"))!=block.end()){
				i->second.pop_front();
				setShowDebug(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Race specific theme"))!=block.end()){
				i->second.pop_front();
			   	setRaceSpecificTheme(atoi(i->second.front().c_str()));
			}
			if((i=block.find("DNA Spiral"))!=block.end()){
				i->second.pop_front();
			   	setDnaSpiral(atoi(i->second.front().c_str()));
			}

                        if((i=block.find("Game speed"))!=block.end()){
				i->second.pop_front();
				setGameSpeed(atoi(i->second.front().c_str()));
			}
		}
		old_pos = pFile.tellg();
	}// END while
} // schoen :)

const bool EF_Configuration::setAutoRuns(const bool auto_runs) 
{
	if(autoRuns == auto_runs)
		return(false);		
	autoRuns = auto_runs;
	return(true);
}

const bool EF_Configuration::setWaitAfterChange(const bool wait_after_change)
{
	if(waitAfterChange == wait_after_change)
		return(false);
	waitAfterChange = wait_after_change;
	return(true);
}

const bool EF_Configuration::setCompactDisplayMode(const bool compact_display_mode) 
{
	if(compactDisplayMode == compact_display_mode)
		return(false);
	compactDisplayMode = compact_display_mode;
	return(true);
}

const bool EF_Configuration::setFacilityMode(const bool facility_mode) 
{
	if(facilityMode == facility_mode)
		return(false);
	facilityMode = facility_mode;
	return(true);
}

const bool EF_Configuration::setRaceSpecificTheme(const bool race_specific_theme) 
{
	if(raceSpecificTheme == race_specific_theme)
		return(false);
	raceSpecificTheme = race_specific_theme;
	return(true);
}
const bool EF_Configuration::setDnaSpiral(const bool dna_spiral) 
{
	if(dnaSpiral == dna_spiral)
		return(false);
	dnaSpiral = dna_spiral;
	return(true);
}

const bool EF_Configuration::setToolTips(const bool tool_tips) 
{
	if(toolTips == tool_tips)
		return(false);
	toolTips = tool_tips;
	return(true);
}

const bool EF_Configuration::setShowDebug(const bool show_debug)
{
	if(showDebug == show_debug)
		return(false);
	showDebug = show_debug;
	return(true);
}

const bool EF_Configuration::setMaxGenerations(const unsigned int max_generations)
{
	if(maxGenerations == max_generations)
		return(false);
	maxGenerations = max_generations;
	return(true);
}

const bool EF_Configuration::setGameSpeed(const unsigned int game_speed)
{
	if(gameSpeed == game_speed)
		return(false);
#ifdef _SCC_DEBUG
	if(game_speed>6) {
		toErrorLog("WARNING (CoreConfiguration::setGameSpeed()): Value out of range.");
		return(false);
	}
#endif
	gameSpeed = game_speed;
	return(true);
}

EF_Configuration efConfiguration;

