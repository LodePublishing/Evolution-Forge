#include "configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

CoreConfiguration::CoreConfiguration() : 
	crossingOver(MIN_CROSSING_OVER),
	breedFactor(20),
	mutationFactor(200),
	maxTime(MAX_TIME),
	maxTimeOut(MAX_TIMEOUT),
	maxLength(MAX_LENGTH),
	noise(0),
	preprocessBuildOrder(false),
	allowGoalAdaption(true),
	fastCalculation(false),
	allowWaitOrders(true),
	waitAccuracy(100),
	configurationFile("settings/core.cfg"),
	gameType(0)
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
	setMaxLength(MAX_LENGTH);
	setNoise(0);
	setPreprocessBuildOrder(false);
	setAllowGoalAdaption(true);
	setAllowWaitOrders(true);
	setWaitAccuracy(100);
	setFastCalculation(false);
	configurationFile = "settings/core.cfg";
}

void CoreConfiguration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!checkStreamIsOpen(pFile, "CoreConfiguration::saveToFile", configurationFile))
		return;
	pFile << "@SETTINGS" << std::endl;
	pFile << "# fast calculation?" << std::endl;
	pFile << "\"Fast calculation\" = \"" << (int)isFastCalculation() << "\"" << std::endl;
	pFile << "# use broodwar units?" << std::endl;
	pFile << "# allow orders to let the build order wait a certain time instead of building units and buildings as soon as possible?" << std::endl;
	pFile << "\"Allow wait orders\" = \"" << (int)isAllowWaitOrders() << "\"" << std::endl;
	pFile << "# time in seconds to wait (less = the more accurate but needs also much more time to calculate)" << std::endl;
	pFile << "\"Wait accuracy\" = \"" << getWaitAccuracy() << "\"" << std::endl;	

//	pFile << "# max time in seconds" << std::endl;
//	pFile << "    \"Max Time\" = \"" << getMaxTime() << "\"" << std::endl;
//	pFile << "# Preprocessing not implemented yet" << std::endl;
//	pFile << "    \"Preprocess Buildorder\" = \"" << (int)isPreprocessBuildOrder() << "\"" << std::endl;
//	pFile << "    \"Max Length\" = \"" << getMaxLength() << "\"" << std::endl;
//	pFile << "# max timeout for each order in seconds" << std::endl;
//	pFile << "    \"Max Timeout\" = \"" << getMaxTimeOut() << "\"" << std::endl;
//	pFile << "# Allow to ignore unnecessary goals (mainly for using for example only \"lair\" instead of both \"lair\" and \"hatchery\")" << std::endl;
//	pFile << "    \"Allow goal adaption\" = \"" << (int)isAllowGoalAdaption() << "\"" << std::endl;
//	pFile << "# Breed Factor:" << std::endl;
//	pFile << "# This parameter determines how many % of the players will multiply by 1:1 copy" << std::endl;
//	pFile << "    \"Breed Factor\" = \"" << getBreedFactor() << "\"" << std::endl;
//	pFile << "# Crossing Over:" << std::endl;
//	pFile << "# This parameter determines how many % of the players will multiply by crossover# CURRENTLY NOT IMPLEMENTED!" << std::endl;
//	pFile << "    \"Crossing Over\" = \"" << getCrossingOver() << "\"" << std::endl;
//	pFile << "# 10 means that programs will be sorted into 10-program groups, the best 2 are" << std::endl;
//	pFile << "# taken for reproduction and the worst two are replaced by their children" << std::endl;
	pFile << "@END" << std::endl;
}

void CoreConfiguration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	if(!checkStreamIsOpen(pFile, "CoreConfiguration::loadConfigurationFile()", configurationFile))
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
		if(!checkStreamForFailure(pFile, "CoreConfiguration::loadConfigurationFile()", configurationFile))
			return;
		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start == std::string::npos) || (text[0] == '#') || (text[0] == '\0'))
			continue; // ignore line
		size_t stop = text.find_first_of("\t ", start);
		if(stop == std::string::npos) 
			stop = text.size();
		std::string index = text.substr(start, stop);
		if(index=="@SETTINGS")
		{
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			if(!parse_block_map(pFile, block))
			{
#ifdef _SCC_DEBUG
				toErrorLog("WARNING (CoreConfiguration::loadConfigurationFile) No concluding @END was found!");
#endif
			}
			std::map<std::string, std::list<std::string> >::iterator i;
			if((i=block.find("Fast calculation"))!=block.end()){
				i->second.pop_front();
			   	setFastCalculation(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Allow wait orders"))!=block.end()){
				i->second.pop_front();
			   	setAllowWaitOrders(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Wait accuracy"))!=block.end()){
				i->second.pop_front();
			   	setWaitAccuracy(atoi(i->second.front().c_str()));
			}

/*			if((i=block.find("Allow goal adaption"))!=block.end()){
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
			}*/
		}
		old_pos = pFile.tellg();
	}// END while
} // schoen :)

const bool CoreConfiguration::setGameType(const unsigned int game_type_number)
{
	if(gameType == game_type_number)
		return(false);
#ifdef _SCC_DEBUG
	if((game_type_number==0) || (game_type_number>GAME::MAX_GAME_TYPES)) {
		toErrorLog("DEBUG (CoreConfiguration::setGameType()): Value game_type_number out of range.");return(false);
	}
#endif
	gameType = game_type_number;
	toErrorLog(gameType);
	return(true);
}

const bool CoreConfiguration::setFastCalculation(const bool fast_calculation) 
{
	if(fastCalculation == fast_calculation)
		return(false);
	fastCalculation = fast_calculation;
	return(true);
}

const bool CoreConfiguration::setAllowWaitOrders(const bool allow_wait_orders) 
{
	if(allowWaitOrders == allow_wait_orders)
		return(false);
	allowWaitOrders = allow_wait_orders;
	return(true);
}

const bool CoreConfiguration::setWaitAccuracy(const unsigned int wait_accuracy) 
{
	if(waitAccuracy == wait_accuracy)
		return(false);
#ifdef _SCC_DEBUG
	if((wait_accuracy<1) || (wait_accuracy>1200)) {
		toErrorLog("WARNING (CoreConfiguration::setWaitAccuracy()()): Value out of range.");return(false);
	}
#endif
	waitAccuracy = wait_accuracy;
	return(true);
}

const bool CoreConfiguration::setCrossingOver(const unsigned int crossing_over) 
{
	if(crossingOver == crossing_over)
		return(false);
#ifdef _SCC_DEBUG
	if((crossing_over<MIN_CROSSING_OVER)||(crossing_over>MAX_CROSSING_OVER)) {
		toErrorLog("WARNING (CoreConfiguration::setCrossingOver()): Value out of range.");return(false);
	}
#endif
	crossingOver = crossing_over;
	return(true);
}

const bool CoreConfiguration::setBreedFactor(const unsigned int breed_factor) 
{
	if(breedFactor == breed_factor)
		return(false);
#ifdef _SCC_DEBUG
	if((breed_factor<MIN_BREED_FACTOR)||(breed_factor>MAX_BREED_FACTOR)) {
		toErrorLog("WARNING (CoreConfiguration::setBreedFactor()): Value out of range.");return(false);
	}
#endif
	breedFactor = breed_factor;
	return(true);
}
const bool CoreConfiguration::setMutationFactor(const unsigned int mutation_factor) 
{
	if(mutationFactor == mutation_factor)
		return(false);
#ifdef _SCC_DEBUG
	if((mutation_factor<MIN_MUTATION_FACTOR)||(mutation_factor>MAX_MUTATION_FACTOR)) {
		toErrorLog("WARNING (CoreConfiguration::setMutationFactor()): Value out of range.");return(false);
	}
#endif
	mutationFactor = mutation_factor;
	return(true);
}

const bool CoreConfiguration::setMaxTime(const unsigned int max_time) 
{
	if(maxTime == max_time)
		return(false);
#ifdef _SCC_DEBUG
	if((max_time<MIN_TIME)||(max_time>MAX_TIME)) {
		toErrorLog("WARNING (CoreConfiguration::setMaxTime()): Value out of range.");return(false);
	}
#endif
	maxTime = max_time;
	return(true);
}

const bool CoreConfiguration::setMaxTimeOut(const unsigned int time_out) 
{
	if(maxTimeOut == time_out)
		return(false);
#ifdef _SCC_DEBUG
	if((time_out<MIN_TIMEOUT)||(time_out>MAX_TIMEOUT)) {
		toErrorLog("WARNING (CoreConfiguration::setMaxTimeOut()): Value out of range.");return(false);
	}
#endif
	maxTimeOut = time_out;
	return(true);
}

const bool CoreConfiguration::setMaxLength(const unsigned int max_length) 
{
	if(maxLength == max_length)
		return(false);
#ifdef _SCC_DEBUG
	if((max_length<MIN_LENGTH)||(max_length>MAX_LENGTH)) {
		toErrorLog("WARNING (CoreConfiguration::setMaxLength()): Value out of range.");return(false);
	}
#endif
	maxLength = max_length;
	return(true);
}

const bool CoreConfiguration::setNoise(const unsigned int desired_noise)
{
	if(noise == desired_noise)
		return(false);
#ifdef _SCC_DEBUG
	if((desired_noise < MIN_NOISE)||(desired_noise > MAX_NOISE)) {
		toErrorLog("WARNING (CoreConfiguration::setNoise()): Value out of range.");return(false);
	}
#endif
	noise = desired_noise;
	return(true);
}

const bool CoreConfiguration::setPreprocessBuildOrder(const bool preprocess_build_order) 
{
	if(preprocessBuildOrder == preprocess_build_order)
		return(false);
	preprocessBuildOrder = preprocess_build_order;
	return(true);
}

const bool CoreConfiguration::setAllowGoalAdaption(const bool allow_goal_adaption) 
{
	if(allowGoalAdaption == allow_goal_adaption)
		return(false);
	allowGoalAdaption = allow_goal_adaption;
	return(true);
}

CoreConfiguration coreConfiguration;

const unsigned int MAX_MUTATION_FACTOR = 400;
const unsigned int MIN_MUTATION_FACTOR = 1;

const unsigned int MAX_BREED_FACTOR = 40;
const unsigned int MIN_BREED_FACTOR = 0;

const unsigned int MIN_NOISE = 0;
const unsigned int MAX_NOISE = 100;

const unsigned int MAX_CROSSING_OVER = 40;
const unsigned int MIN_CROSSING_OVER = 0;

