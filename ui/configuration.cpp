#include "configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

UI_Configuration::UI_Configuration(): 
	language(ZERO_LANGUAGE),
	resolution(RESOLUTION_640x480),
	bitdepth(DEPTH_32BIT),
	theme(DARK_BLUE_THEME),
	glowingButtons(true),
	transparency(false),
	smoothMovements(true),
	unloadGraphics(false),
	configurationFile("settings/ui.cfg")
{}

UI_Configuration::~UI_Configuration()
{ }

void UI_Configuration::initDefaults()
{
	setLanguage(ENGLISH_LANGUAGE);
	setResolution(RESOLUTION_640x480);
	setBitDepth(DEPTH_32BIT);
	setTheme(DARK_BLUE_THEME);
	setGlowingButtons(true);
	setTransparency(false);
	setSmoothMovements(true);
	setUnloadGraphics(false);
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
	pFile << "# 1 = 640x480, 2 = 800x600, 3 = 1024x768, 4 = 1280x1024" << std::endl;
	pFile << "    \"Resolution\" = \"" << (int)getResolution() << "\"" << std::endl;
	pFile << "# 0 = 256 colors, 1 = 64k colors, 2 = 16mio colors, 3 = 4mrd colors" << std::endl;
	pFile << "    \"Bit depth\" = \"" << (int)getBitDepth() << "\"" << std::endl;
	pFile << "# 1 = dark red theme, 2 = dark blue theme, 4 = yellow theme" << std::endl;
	pFile << "    \"Theme\" = \"" << (int)getTheme() << "\"" << std::endl;
	pFile << "# glowing effects" << std::endl;
	pFile << "    \"Glowing buttons\" = \"" << (int)isGlowingButtons() << "\"" << std::endl;
	pFile << "# moving rectangles, 2 = all objects move smoothly, 1 = some objects move smoothly, 0 = all objects jump directly to their destination" << std::endl;
	pFile << "    \"Smooth movements\" = \"" << (int)isSmoothMovements() << "\"" << std::endl;
	pFile << "# Transparency, let's burn some cpu power (NOT YET IMPLEMENTED)" << std::endl;
	pFile << "    \"Transparency\" = \"" << (int)isTransparency() << "\"" << std::endl;
	pFile << "# Unload graphics if they are not needed, for low-memory systems" << std::endl;
	pFile << "    \"Unload graphics\" = \"" << (int)isUnloadGraphics() << "\"" << std::endl;

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
			if((i=block.find("Resolution"))!=block.end()){
				i->second.pop_front();
			   	setResolution((eResolution)(atoi(i->second.front().c_str())));
			}
			if((i=block.find("Bit depth"))!=block.end()){
				i->second.pop_front();
			   	setBitDepth((eBitDepth)(atoi(i->second.front().c_str())));
			}
			if((i=block.find("Theme"))!=block.end()){
				i->second.pop_front();
			   	setTheme((eTheme)(atoi(i->second.front().c_str())));
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
			if((i=block.find("Unload graphics"))!=block.end()){
				i->second.pop_front();
			   	setUnloadGraphics(atoi(i->second.front().c_str()));
			}

		}
	}// END while
} // schoen :)

const bool UI_Configuration::setResolution(const eResolution current_resolution) 
{
	if(resolution == current_resolution)
		return(false);
	resolution = current_resolution;
	return(true);
}

const bool UI_Configuration::setBitDepth(const eBitDepth current_bitdepth) 
{
	if(bitdepth == current_bitdepth)
		return(false);
	bitdepth = current_bitdepth;
	return(true);
}

const bool UI_Configuration::setLanguage(const eLanguage current_language) 
{
	if(language == current_language)
		return(false);
	language = current_language;
	return(true);
}

const bool UI_Configuration::setTheme(const eTheme current_theme) 
{
	if(theme == current_theme)
		return(false);
	theme = current_theme;
	return(true);
}

const bool UI_Configuration::setGlowingButtons(const bool glowing_buttons) 
{
	if(glowingButtons == glowing_buttons)
		return(false);
	glowingButtons = glowing_buttons;
	return(true);
}

const bool UI_Configuration::setTransparency(const bool trans_parency) 
{
	if(transparency == trans_parency)
		return(false);
	transparency = trans_parency;
	return(true);
}

const bool UI_Configuration::setSmoothMovements(const bool smooth_movements) 
{
	if(smoothMovements == smooth_movements)
		return(false);
	smoothMovements = smooth_movements;
	return(true);
}

const bool UI_Configuration::setUnloadGraphics(const bool unload_graphics) 
{
	if(unloadGraphics == unload_graphics)
		return(false);
	unloadGraphics = unload_graphics;
	return(true);
}

UI_Configuration uiConfiguration;

