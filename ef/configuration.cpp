#include "configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

EF_Configuration::EF_Configuration(): 
	dynamicFramerate(25),
	staticFramerate(25),
	currentFramerate(1),
	currentFramesPerGeneration(1),
	autoSaveRuns(false),
	restrictSC(true),
	facilityMode(true),
	fullScreen(false),
	softwareMouse(false),
	backgroundBitmap(false),
	allowStaticFramerate(false),
	dnaSpiral(true),
	toolTips(true),
	configurationFile("settings/main.cfg")
{ }

EF_Configuration::~EF_Configuration()
{ }

void EF_Configuration::initDefaults()
{
	setDynamicFramerate(25);
	setStaticFramerate(25);
	setCurrentFramerate(1);
	setCurrentFramesPerGeneration(1),
	setAutoSaveRuns(false);
	setRestrictSC(false);
	setFacilityMode(true);
	setFullScreen(false);
	setSoftwareMouse(false);
	setBackgroundBitmap(false);
	setAllowStaticFramerate(false);
	setDnaSpiral(true);
	setToolTips(true);
	configurationFile = "settings/main.cfg";
}

void EF_Configuration::setConfigurationFile(const std::string& configuration_file)
{
	configurationFile = configuration_file;
}

void EF_Configuration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!pFile.is_open())
	{
		toLog("ERROR: (EF_Configuration::saveToFile): File could not be opened.");
		return;
	}
	pFile << "@SETTINGS" << std::endl;
	pFile << "# Do autosave at the end of a run or ask for it?" << std::endl;
	pFile << "    \"Autosave runs\" = \"" << (int)isAutoSaveRuns() << "\"" << std::endl;
	pFile << "# set this to 1 to set all details to zero, fastest output" << std::endl;
	pFile << "# Real minimalists should take a look at the command line options where SDL can be deactivated completely! 8-DD" << std::endl;
	pFile << "# CURRENTLY NOT IMPLEMENTED" << std::endl;
	pFile << "    \"Minimalist\" = \"0\"" << std::endl; // TODO
	pFile << "" << std::endl;
	pFile << "# how a frame update is weighted compared to a core update (e.g. 4 means that a new generation is calculated every 4 frames)" << std::endl;
	pFile << "    \"Allow static framerate\" = \"" << (int)isAllowStaticFramerate() << "\"" << std::endl;
	pFile << "# allow the program to dynamicly reduce the speed of the core to reach this frame rate. 0 = offline." << std::endl;
	pFile << "    \"Static framerate\" = \"" << getStaticFramerate() << "\"" << std::endl;
	pFile << "# Draw X frames per new generation" << std::endl;
	pFile << "    \"Dynamic framerate\" = \"" << getDynamicFramerate() << "\"" << std::endl;
	pFile << "" << std::endl;                                                                                
	pFile << "# Restrict unit menus to StarCraft (TM) units?" << std::endl;
	pFile << "    \"Restrict units\" = \"" << (int)isRestrictSC() << "\"" << std::endl;
	pFile << "# Order entries in the unitmenu by area or by facility?" << std::endl;
	pFile << "    \"Facility mode\" = \"" << (int)isFacilityMode() << "\"" << std::endl;
	pFile << "# Show nice DNA spiral?" << std::endl;
	pFile << "    \"DNA Spiral\" = \"" << (int)isDnaSpiral() << "\"" << std::endl;
	pFile << "# use background bitmap, saves some cpu power if deactivated" << std::endl;
	pFile << "    \"Background bitmap\" = \"" << (int)isBackgroundBitmap() << "\"" << std::endl;
	pFile << "    \"Fullscreen\" = \"" << (int)isFullScreen() << "\"" << std::endl;
	pFile << "    \"Software mouse\" = \"" << (int)isSoftwareMouse() << "\"" << std::endl;
	pFile << "    \"Tooltips\" = \"" << (int)isToolTips() << "\"" << std::endl;
	pFile << "@END" << std::endl;
}

void EF_Configuration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	if(!pFile.is_open())
	{
		toLog("WARNING: (EF_Configuration::loadConfigurationFile): File not found.");
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
		if(stop==std::string::npos) stop = text.size();
		std::string index=text.substr(start, stop);
		std::string value;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@SETTINGS")
		{
			std::map<std::string, std::list<std::string> > block;
			parse_block(pFile, block);

			if((i=block.find("Autosave runs"))!=block.end()) {
				i->second.pop_front();
			   	setAutoSaveRuns(atoi(i->second.front().c_str()));
			}
		
			if((i=block.find("Restrict units"))!=block.end()){
				i->second.pop_front();
				setRestrictSC(atoi(i->second.front().c_str()));
			}
			
			if((i=block.find("Facility mode"))!=block.end()){
				i->second.pop_front();
			   	setFacilityMode(atoi(i->second.front().c_str()));
			}
	
			if((i=block.find("Fullscreen"))!=block.end()){
				i->second.pop_front();
			   	setFullScreen(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Software mouse"))!=block.end()){
				i->second.pop_front();
			   	setSoftwareMouse(atoi(i->second.front().c_str()));
			}
		
			if((i=block.find("Background bitmap"))!=block.end()){
				i->second.pop_front();
			   	setBackgroundBitmap(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Allow static framerate"))!=block.end()){
				i->second.pop_front();
			   	setAllowStaticFramerate(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Static framerate"))!=block.end()){
				i->second.pop_front();
			   	setStaticFramerate(atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Dynamic framerate"))!=block.end()){
				i->second.pop_front();
			   	setDynamicFramerate(atoi(i->second.front().c_str()));
			}		
		
			if((i=block.find("DNA Spiral"))!=block.end()){
				i->second.pop_front();
			   	setDnaSpiral(atoi(i->second.front().c_str()));
			}
		}
	}// END while
} // schoen :)

	
EF_Configuration efConfiguration;
//const unsigned int EF_Configuration::MIN_DYNAMIC_FRAMERATE = 0;
//const unsigned int EF_Configuration::MAX_DYNAMIC_FRAMERATE = 100;
//const unsigned int EF_Configuration::MIN_STATIC_FRAMERATE = 1;
//const unsigned int EF_ConfigurationMAX_STATIC_FRAMERATE = 100;

