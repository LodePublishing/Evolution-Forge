#include "configuration.hpp"
#include <sstream>
#include <fstream>

UI_Configuration::UI_Configuration(): 
	language(ZERO_LANGUAGE),
	resolution(RESOLUTION_640x480),
	bitdepth(DEPTH_32BIT),
	theme(DARK_BLUE_THEME),
	backgroundBitmap(false),
	musicVolume(75),
	soundVolume(75),
	channels(16),
	useMusic(true),
	useSound(true),
	desiredFramerate(25),
	desiredCPU(75),
	currentFramerate(1),
	currentFramesPerGeneration(1),
	fullScreen(false),
	glowingButtons(true),
	transparency(false),
	smoothMovements(true),
	unloadGraphics(false),
	firstStart(false),
	configurationFile("settings/ui.cfg"),
	visitedHelpChapters()
{}

UI_Configuration::~UI_Configuration()
{ }

void UI_Configuration::initDefaults()
{
	setLanguage(ENGLISH_LANGUAGE);
	setResolution(RESOLUTION_640x480);
	setBitDepth(DEPTH_32BIT);
	setTheme(DARK_BLUE_THEME);
	setBackgroundBitmap(false);
	setMusicVolume(75);
	setSoundVolume(75);
	setChannels(16);
	setMusic(true);
	setSound(true);
	setDesiredFramerate(25);
	setDesiredCPU(75);
	setCurrentFramerate(1);
	setCurrentFramesPerGeneration(1),
	setFullScreen(false);
	setGlowingButtons(true);
	setTransparency(false);
	setSmoothMovements(true);
	setUnloadGraphics(false);
	setFirstStart(false);
	configurationFile = "settings/ui.cfg";
}

void UI_Configuration::parseParameters(std::list<std::string>& arguments)
{
	for(std::list<std::string>::iterator i = arguments.begin(); i != arguments.end(); )
	{
		bool not_found = false;
		if((*i) == "-nosound")
		{
			setSound(false);
			setMusic(false);
		} 
		else if((*i) == "-640")
		{
			setResolution(RESOLUTION_640x480);
		} else if((*i) == "-800")
		{
			setResolution(RESOLUTION_800x600);
		} else if((*i) == "-1024")
		{
			setResolution(RESOLUTION_1024x768);
		} else if((*i) == "-1280")
		{
			setResolution(RESOLUTION_1280x1024);
		} else if((*i) == "-8bit")
		{
			setBitDepth(DEPTH_8BIT);
		} else if((*i) == "-16bit")
		{
			setBitDepth(DEPTH_16BIT);
		} else if((*i) == "-24bit")
		{
			setBitDepth(DEPTH_24BIT);
		} else if((*i) == "-32bit")
		{
			setBitDepth(DEPTH_32BIT);
		} else if((*i) == "-fs")
		{
			setFullScreen(true);
		} else if((*i) == "-window")
		{
			setFullScreen(false);
		} else 
			not_found = true;
		if(not_found)
			i++;
		else i = arguments.erase(i);
	}

}

void UI_Configuration::visitHelpChapter(const unsigned int chapter)
{
	visitedHelpChapters.insert(chapter);
}

void UI_Configuration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!pFile.is_open())
	{
		toErrorLog("ERROR: (UI_Configuration::saveToFile): File could not be opened.");
		return;
	}
	pFile << "@SETTINGS" << std::endl;
	pFile << "# Is this the first start? Then display a language menu at the beginning... 1 = yes, 0 = no" << std::endl;
	pFile << "    \"First start\" = \"" << (int)isFirstStart() << "\"" << std::endl;
	pFile << "# 1 = english, 2 = german" << std::endl;
	pFile << "    \"Language\" = \"" << (int)getLanguage() << "\"" << std::endl;
	pFile << "# 1 = 640x480, 2 = 800x600, 3 = 1024x768, 4 = 1280x1024" << std::endl;
	pFile << "    \"Resolution\" = \"" << (int)getResolution() << "\"" << std::endl;
	pFile << "# 0 = 256 colors, 1 = 64k colors, 2 = 16mio colors, 3 = 4mrd colors" << std::endl;
	pFile << "    \"Bit depth\" = \"" << (int)getBitDepth() << "\"" << std::endl;
	pFile << "# 1 = dark red theme, 2 = dark blue theme, 4 = yellow theme" << std::endl;
	pFile << "    \"Theme\" = \"" << (int)getTheme() << "\"" << std::endl;
	pFile << "# use background bitmap, saves some cpu power if deactivated" << std::endl;
	pFile << "    \"Background bitmap\" = \"" << (int)isBackgroundBitmap() << "\"" << std::endl;
	
	pFile << "# use music (1: on, 0: off)" << std::endl;
	pFile << "    \"Music\" = \"" << (int)isMusic() << "\"" << std::endl;
	pFile << "# use sound (1: on, 0: off)" << std::endl;
	pFile << "    \"Sound effects\" = \"" << (int)isSound() << "\"" << std::endl;

	pFile << "# music volume (0% - 100%)" << std::endl;
	pFile << "    \"Music volume\" = \"" << (int)getMusicVolume() << "\"" << std::endl;
	pFile << "# sound volume (0% - 100%)" << std::endl;
	pFile << "    \"Sound volume\" = \"" << (int)getSoundVolume() << "\"" << std::endl;
	pFile << "# max number of simultaneously played sounds" << std::endl;
	pFile << "    \"Channels\" = \"" << (int)getChannels() << "\"" << std::endl;
	
	pFile << "    \"Desired framerate\" = \"" << getDesiredFramerate() << "\"" << std::endl;
	pFile << "# Desired CPU usage" << std::endl;
	pFile << "    \"Desired CPU usage\" = \"" << getDesiredCPU() << "\"" << std::endl;
	pFile << "" << std::endl;

	pFile << "    \"Fullscreen\" = \"" << (int)isFullScreen() << "\"" << std::endl;
	pFile << "# glowing effects" << std::endl;
	pFile << "    \"Glowing buttons\" = \"" << (int)isGlowingButtons() << "\"" << std::endl;
	pFile << "# moving rectangles, 2 = all objects move smoothly, 1 = some objects move smoothly, 0 = all objects jump directly to their destination" << std::endl;
	pFile << "    \"Smooth movements\" = \"" << (int)isSmoothMovements() << "\"" << std::endl;
	pFile << "# Transparency, let's burn some cpu power (NOT YET IMPLEMENTED)" << std::endl;
	pFile << "    \"Transparency\" = \"" << (int)isTransparency() << "\"" << std::endl;
	pFile << "# Unload graphics if they are not needed, for low-memory systems" << std::endl;
	pFile << "    \"Unload graphics\" = \"" << (int)isUnloadGraphics() << "\"" << std::endl;
	pFile << "# internal" << std::endl;
	pFile << "    \"Visited help chapters\" =";
	for(std::set<unsigned int>::const_iterator i = visitedHelpChapters.begin(); i != visitedHelpChapters.end(); ++i)
		pFile << " " << *i;
	pFile << std::endl;
	pFile << "@END" << std::endl;
}

void UI_Configuration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	if(!checkStreamIsOpen(pFile, "UI_Configuration::loadConfigurationFile()", configurationFile))
        {
//                toErrorLog(lookUpString(START_CREATE_NEW_DEFAULT_FILE_STRING));
//                problem: kein Zugriff auf theme!
                initDefaults();
                saveToFile();
                return;
        }

	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
                if(!checkStreamForFailure(pFile, "UI_Configuration::loadConfigurationFile()", configurationFile))
                        return;
		
		std::string text = line;
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
			pFile.seekg(old_pos);
			parse_block_map(pFile, block);

			if((i=block.find("First start"))!=block.end()){
				i->second.pop_front();
			   	setFirstStart(atoi(i->second.front().c_str()));
			}

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
			if((i=block.find("Background bitmap"))!=block.end()){
				i->second.pop_front();
			   	setBackgroundBitmap(atoi(i->second.front().c_str()));
			}
		
			if((i=block.find("Music"))!=block.end()){
				i->second.pop_front();
			   	setMusic(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Sound effects"))!=block.end()){
				i->second.pop_front();
			   	setSound(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Music volume"))!=block.end()){
				i->second.pop_front();
			   	setMusicVolume(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Sound volume"))!=block.end()){
				i->second.pop_front();
			   	setSoundVolume(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Channels"))!=block.end()){
				i->second.pop_front();
			   	setChannels(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Desired framerate"))!=block.end()){
				i->second.pop_front();
			   	setDesiredFramerate(atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Desired CPU usage"))!=block.end()){
				i->second.pop_front();
			   	setDesiredCPU(atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Fullscreen"))!=block.end()){
				i->second.pop_front();
			   	setFullScreen(atoi(i->second.front().c_str()));
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
		
			if((i=block.find("Visited help chapters"))!=block.end()){
				i->second.pop_front();
				visitedHelpChapters.clear();
				for(std::list<std::string>::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
					visitedHelpChapters.insert(atoi(j->c_str()));
			}				
		}
		old_pos = pFile.tellg();
	}// END while
} // schoen :)

const bool UI_Configuration::isVisitedHelpChapter(const unsigned int chapter) const
{
	if(visitedHelpChapters.find(chapter) == visitedHelpChapters.end())
		return(false);
	else return(true);
}

const bool UI_Configuration::setFirstStart(const bool first_start) 
{
	if(firstStart == first_start)
		return(false);
	firstStart = first_start;
	if(!firstStart)
		saveToFile();
	return(true);
}

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

const bool UI_Configuration::setBackgroundBitmap(const bool background_bitmap) 
{
	if(backgroundBitmap == background_bitmap)
		return(false);
	backgroundBitmap = background_bitmap;
	return(true);
}

const bool UI_Configuration::setMusic(const bool use_music) 
{
	if(useMusic == use_music)
		return(false);
	useMusic = use_music;
	return(true);
}

const bool UI_Configuration::setSound(const bool use_sound) 
{
	if(useSound == use_sound)
		return(false);
	useSound = use_sound;
	return(true);
}

const bool UI_Configuration::setMusicVolume(const unsigned int music_volume)
{
	if(musicVolume == music_volume)
		return(false);
	musicVolume = music_volume;
	return(true);
}

const bool UI_Configuration::setSoundVolume(const unsigned int sound_volume) 
{
	if(soundVolume == sound_volume)
		return(false);
	soundVolume = sound_volume;
	return(true);
}

const bool UI_Configuration::setChannels(const unsigned int channel_num)
{
	if(channels == channel_num)
		return(false);
	channels = channel_num;
	return(true);
}

const bool UI_Configuration::setCurrentFramerate(const unsigned int frame_rate) 
{
	if(currentFramerate == frame_rate)
		return(false);
	currentFramerate = frame_rate;
	return(true);
}

const bool UI_Configuration::setCurrentFramesPerGeneration(const unsigned int frames_per_generation) 
{
	if(currentFramesPerGeneration == frames_per_generation)
		return(false);
	currentFramesPerGeneration = frames_per_generation;
	return(true);
}

const bool UI_Configuration::setDesiredCPU(const unsigned int desired_cpu_usage)
{
	if(desiredCPU == desired_cpu_usage)
		return(false);
#ifdef _SCC_DEBUG
	if((desired_cpu_usage<1)||(desired_cpu_usage>100)) {
		toErrorLog("WARNING: (UI_Configuration::setDesiredCPU): Value out of range.");return(false);
	}
#endif
	desiredCPU = desired_cpu_usage;
	return(true);
}

const bool UI_Configuration::setDesiredFramerate(const unsigned int desired_frame_rate)
{
	if(desiredFramerate == desired_frame_rate)
		return(false);
#ifdef _SCC_DEBUG
	if((desired_frame_rate<MIN_DESIRED_FRAMERATE)||(desired_frame_rate>MAX_DESIRED_FRAMERATE)) {
		toErrorLog("WARNING: (UI_Configuration::setDesiredFramerate): Value out of range.");return(false);
	}
#endif
	desiredFramerate = desired_frame_rate;
	return(true);
}

const bool UI_Configuration::setFullScreen(const bool full_screen) 
{
	if(fullScreen == full_screen)
		return(false);
	fullScreen = full_screen;
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

