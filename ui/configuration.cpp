#include "configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

UI_Configuration::UI_Configuration(): 
	language(ZERO_LANGUAGE),
	glowingButtons(true),
	transparency(false),
	smoothMovements(true),
	configurationFile("settings/ui.cfg")
{}

UI_Configuration::~UI_Configuration()
{ }

void UI_Configuration::initDefaults()
{
	setLanguage(ENGLISH_LANGUAGE);
	setGlowingButtons(true);
	setTransparency(false);
	setSmoothMovements(true);
	configurationFile = "settings/ui.cfg";
}

void UI_Configuration::setConfigurationFile(const std::string& configuration_file)
{
	configurationFile = configuration_file;
}

void UI_Configuration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!pFile.is_open())
	{
		toLog("ERROR: (UI_Configuration::saveToFile): File could not be opened.");
		return;
	}
	pFile << "@SETTINGS" << std::endl;
	pFile << "# 1 = english, 2 = german" << std::endl;
	pFile << "    \"Language\" = \"" << (int)getLanguage() << "\"" << std::endl;
	pFile << "# glowing effects" << std::endl;
	pFile << "    \"Glowing buttons\" = \"" << (int)isGlowingButtons() << "\"" << std::endl;
	pFile << "# moving rectangles, 2 = all objects move smoothly, 1 = some objects move smoothly, 0 = all objects jump directly to their destination" << std::endl;
	pFile << "    \"Smooth movements\" = \"" << (int)isSmoothMovements() << "\"" << std::endl;
	pFile << "# Transparency, let's burn some cpu power (NOT YET IMPLEMENTED)" << std::endl;
	pFile << "    \"Transparency\" = \"" << (int)isTransparency() << "\"" << std::endl;
	pFile << "@END" << std::endl;
}

void UI_Configuration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	if(!pFile.is_open())
	{
		toLog("WARNING: (UI_Configuration::loadConfigurationFile): File not found.");
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

			if((i=block.find("Language"))!=block.end()){
				i->second.pop_front();
			   	setLanguage((eLanguage)(atoi(i->second.front().c_str())));
			}
			if((i=block.find("Glowing buttons"))!=block.end()){
				i->second.pop_front();
			   	setGlowingButtons(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Transparency"))!=block.end()){
				i->second.pop_front();
			   	setTransparency(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Smooth movements"))!=block.end()){
				i->second.pop_front();
			   	setSmoothMovements(atoi(i->second.front().c_str()));
			}

		}
	}// END while
} // schoen :)

	
UI_Configuration uiConfiguration;

