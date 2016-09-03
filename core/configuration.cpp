#include "configuration.hpp"
#include "defs.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

CoreConfiguration::CoreConfiguration(): 
	crossingOver(MIN_CROSSING_OVER),
	breedFactor(20),
	mutationFactor(200),
	maxTime(MAX_TIME),
	maxTimeOut(MAX_TIMEOUT),
	maxLength(MAX_LENGTH),
	maxRuns(MAX_RUNS),
	maxGenerations(200),
	noise(0),
	preprocessBuildOrder(false),
	allowGoalAdaption(true),
	alwaysBuildWorker(false),
	configurationFile("settings/core.cfg")
{ }

CoreConfiguration::~CoreConfiguration()
{ }

void CoreConfiguration::initDefaults()
{
	setCrossingOver(MIN_CROSSING_OVER);
	setBreedFactor(20);
	setMutationFactor(200);
	setMaxTime(MAX_TIME-1);
	setMaxTimeOut(MAX_TIMEOUT-1);
	setMaxLength(MAX_LENGTH-1);
	setMaxRuns(MAX_RUNS-1);
	setMaxGenerations(MAX_GENERATIONS-1);
	setNoise(0);
	setPreprocessBuildOrder(false);
	setAllowGoalAdaption(true);
	setAlwaysBuildWorker(true);
	configurationFile = "settings/core.cfg";
}

void CoreConfiguration::setConfigurationFile(const std::string& configuration_file)
{
	configurationFile = configuration_file;
}

void CoreConfiguration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!pFile.is_open())
	{
		toLog("ERROR: (CoreConfiguration::saveToFile): File could not be opened.");
		return;
	}
	pFile << "@SETTINGS" << std::endl;
	pFile << "# max time in seconds" << std::endl;
	pFile << "    \"Max Time\" = \"" << getMaxTime() << "\"" << std::endl;
	pFile << "# Preprocessing not implemented yet" << std::endl;
	pFile << "    \"Preprocess Buildorder\" = \"" << (int)isPreprocessBuildOrder() << "\"" << std::endl;
	pFile << "    \"Max unchanged Generations\" = \"" << getMaxGenerations() << "\"" << std::endl;
	pFile << "    \"Max Length\" = \"" << getMaxLength() << "\"" << std::endl;
	pFile << "    \"Max Runs\" = \"" << getMaxRuns() << "\"" << std::endl;
	pFile << "# max timeout for each order in seconds" << std::endl;
	pFile << "    \"Max Timeout\" = \"" << getMaxTimeOut() << "\"" << std::endl;
	pFile << "# Allow to ignore unnecessary goals (mainly for using for example only \"lair\" instead of both \"lair\" and \"hatchery\")" << std::endl;
	pFile << "    \"Allow goal adaption\" = \"" << (int)isAllowGoalAdaption() << "\"" << std::endl;
	pFile << "# Breed Factor:" << std::endl;
	pFile << "# This parameter determines how many % of the players will multiply by 1:1 copy" << std::endl;
	pFile << "    \"Breed Factor\" = \"" << getBreedFactor() << "\"" << std::endl;
	pFile << "# Crossing Over:" << std::endl;
	pFile << "# This parameter determines how many % of the players will multiply by crossover# CURRENTLY NOT IMPLEMENTED!" << std::endl;
	pFile << "    \"Crossing Over\" = \"" << getCrossingOver() << "\"" << std::endl;
	pFile << "# 10 means that programs will be sorted into 10-program groups, the best 2 are" << std::endl;
	pFile << "# taken for reproduction and the worst two are replaced by their children" << std::endl;
	pFile << "" << std::endl;                                                       
	pFile << "    \"Always Build Worker\" = \"" << (int)isAlwaysBuildWorker() << "\"" << std::endl;
	pFile << "@END" << std::endl;
}

void CoreConfiguration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	if(!pFile.is_open())
	{
		toLog("WARNING: (CoreConfiguration::loadConfigurationFile): File not found.");
		toLog("-> Creating new file with default values...");
		initDefaults();
		saveToFile();		
		return;
	}
	char line[1024];
	std::string text;
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		text=line;
		size_t start=text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		size_t stop=text.find_first_of("\t ", start);
		if(stop==std::string::npos) stop=text.size();
		std::string index=text.substr(start, stop);
		std::string value;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@SETTINGS")
		{
			std::map<std::string, std::list<std::string> > block;
			parse_block(pFile, block);

			if((i=block.find("Allow goal adaption"))!=block.end()){
				i->second.pop_front();
			   	setAllowGoalAdaption(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Time"))!=block.end()){
				i->second.pop_front();
			   	setMaxTime(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Timeout"))!=block.end()){
				i->second.pop_front();
			   	setMaxTimeOut(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Length"))!=block.end()){
				i->second.pop_front();
			   	setMaxLength(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Runs"))!=block.end()){
				i->second.pop_front();
			   	setMaxRuns(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Preprocess Buildorder"))!=block.end()){
				i->second.pop_front();
			   	setPreprocessBuildOrder(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Breed Factor"))!=block.end()){
				i->second.pop_front();
			   	setBreedFactor(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Crossing Over"))!=block.end()){ 
				i->second.pop_front();
				setCrossingOver(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max unchanged Generations"))!=block.end()){
				i->second.pop_front();
			   	setMaxGenerations(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Always Build Worker"))!=block.end()){
				i->second.pop_front();
			   	setAlwaysBuildWorker(atoi(i->second.front().c_str()));
			}

		}
	}// END while
} // schoen :)

	
CoreConfiguration coreConfiguration;

