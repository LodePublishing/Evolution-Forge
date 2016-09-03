#include "configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

EF_Configuration::EF_Configuration():
	desiredFramerate(25),
	desiredCPU(75),
	currentFramerate(1),
	currentFramesPerGeneration(1),
	autoSaveRuns(false),
	restrictSC(true),
	facilityMode(true),
	fullScreen(false),
	softwareMouse(false),
	backgroundBitmap(false),
	dnaSpiral(true),
	toolTips(true),
	showDebug(false),
	configurationFile("settings/main.cfg")
{ }

EF_Configuration::~EF_Configuration()
{ }

void EF_Configuration::initDefaults()
{
	setDesiredFramerate(25);
	setDesiredCPU(75);
	setCurrentFramerate(1);
	setCurrentFramesPerGeneration(1),
	setAutoSaveRuns(false);
	setRestrictSC(false);
	setFacilityMode(true);
	setFullScreen(false);
	setSoftwareMouse(false);
	setBackgroundBitmap(false);
	setDnaSpiral(true);
	setToolTips(true);
	setShowDebug(false);
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
	pFile << "# Desired framerate: If the computer is fast the calculation speed is improved, if the computer is slow the calculation speed is decreased" << std::endl;
	pFile << "    \"Desired framerate\" = \"" << getDesiredFramerate() << "\"" << std::endl;
	pFile << "# Desired CPU usage" << std::endl;
	pFile << "    \"Desired CPU usage\" = \"" << getDesiredCPU() << "\"" << std::endl;
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
	pFile << "# show which part of the program needs how much CPU resources" << std::endl;
	pFile << "    \"Show debug\" = \"" << (int)isShowDebug() << "\"" << std::endl;
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

	toLog("* Loading " + configurationFile);
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
		{
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
#ifdef _SCC_DEBUG
			toLog("WARNING: (EF_Configuration::loadConfigurationFile) Long line!");
#endif
		}
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
				toLog("WARNING: (EF_Configuration::loadConfigurationFile) No concluding @END was found!");
#endif
			}			
				

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
			if((i=block.find("Tooltips"))!=block.end()){
				i->second.pop_front();
				setToolTips(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Show debug"))!=block.end()){
				i->second.pop_front();
				setShowDebug(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Background bitmap"))!=block.end()){
				i->second.pop_front();
			   	setBackgroundBitmap(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Desired framerate"))!=block.end()){
				i->second.pop_front();
			   	setDesiredFramerate(atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Desired CPU usage"))!=block.end()){
				i->second.pop_front();
			   	setDesiredCPU(atoi(i->second.front().c_str()));
			}		
		
			if((i=block.find("DNA Spiral"))!=block.end()){
				i->second.pop_front();
			   	setDnaSpiral(atoi(i->second.front().c_str()));
			}
		}
		old_pos = pFile.tellg();
	}// END while
} // schoen :)

const bool EF_Configuration::setAutoSaveRuns(const bool auto_save_runs) 
{
	if(autoSaveRuns == auto_save_runs)
		return(false);		
	autoSaveRuns = auto_save_runs;
	return(true);
}

const bool EF_Configuration::setRestrictSC(const bool restrict_sc) 
{
	if(restrictSC == restrict_sc)
		return(false);
	restrictSC = restrict_sc;
	return(true);
}

const bool EF_Configuration::setFacilityMode(const bool facility_mode) 
{
	if(facilityMode == facility_mode)
		return(false);
	facilityMode = facility_mode;
	return(true);
}

const bool EF_Configuration::setSoftwareMouse(const bool software_mouse) 
{
	if(softwareMouse == software_mouse)
		return(false);
	softwareMouse = software_mouse;
	return(true);
}

const bool EF_Configuration::setFullScreen(const bool full_screen) 
{
	if(fullScreen == full_screen)
		return(false);
	fullScreen = full_screen;
	return(true);
}

const bool EF_Configuration::setBackgroundBitmap(const bool background_bitmap) 
{
	if(backgroundBitmap == background_bitmap)
		return(false);
	backgroundBitmap = background_bitmap;
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

const bool EF_Configuration::setCurrentFramerate(const unsigned int frame_rate) 
{
	if(currentFramerate == frame_rate)
		return(false);
	currentFramerate = frame_rate;
	return(true);
}

const bool EF_Configuration::setCurrentFramesPerGeneration(const unsigned int frames_per_generation) 
{
	if(currentFramesPerGeneration == frames_per_generation)
		return(false);
	currentFramesPerGeneration = frames_per_generation;
	return(true);
}

const bool EF_Configuration::setDesiredCPU(const unsigned int desired_cpu_usage)
{
	if(desiredCPU == desired_cpu_usage)
		return(false);
#ifdef _SCC_DEBUG
	if((desired_cpu_usage<1)||(desired_cpu_usage>99)) {
		toLog("WARNING: (EF_Configuration::setDesiredCPU): Value out of range.");return(false);
	}
#endif
	desiredCPU = desired_cpu_usage;
	return(true);
}

const bool EF_Configuration::setDesiredFramerate(const unsigned int desired_frame_rate)
{
	if(desiredFramerate == desired_frame_rate)
		return(false);
#ifdef _SCC_DEBUG
	if((desired_frame_rate<MIN_DESIRED_FRAMERATE)||(desired_frame_rate>MAX_DESIRED_FRAMERATE)) {
		toLog("WARNING: (EF_Configuration::setDesiredFramerate): Value out of range.");return(false);
	}
#endif
	desiredFramerate = desired_frame_rate;
	return(true);
}
EF_Configuration efConfiguration;
//const unsigned int EF_Configuration::MIN_DYNAMIC_FRAMERATE = 0;
//const unsigned int EF_Configuration::MAX_DYNAMIC_FRAMERATE = 100;
//const unsigned int EF_Configuration::MIN_STATIC_FRAMERATE = 1;
//const unsigned int EF_ConfigurationMAX_STATIC_FRAMERATE = 100;

