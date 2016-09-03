#ifndef _UI_CONFIGURATION_HPP
#define _UI_CONFIGURATION_HPP

#include "../stl/misc.hpp"
#include "../sdl/dc.hpp"
#include <set>

enum eResolution
{
	ZERO_RESOLUTION,
	RESOLUTION_640x480,
	RESOLUTION_800x600,
	RESOLUTION_1024x768,
	RESOLUTION_1280x1024,
	MAX_RESOLUTIONS
};


// ------ GENERAL THEMES ------
enum eTheme
{
	ZERO_THEME,
	DARK_BLUE_THEME,
	YELLOW_THEME,
	DARK_RED_THEME,
	GREEN_THEME,
	GREY_THEME,

	MAX_COLOR_THEMES
};
// ------ END GENERAL THEMES ------




class UI_Configuration
{
	public:
		UI_Configuration();
		~UI_Configuration();

		void initDefaults();
		void loadConfigurationFile();
		void saveToFile() const;
		void visitHelpChapter(const unsigned int chapter);

// get/set commands:
		const eLanguage getLanguage() const;
		const eResolution getResolution() const;
		const eBitDepth getBitDepth() const;
		const eTheme getTheme() const;
		const bool isBackgroundBitmap() const;

		const unsigned int getMusicVolume() const;
		const unsigned int getSoundVolume() const;

		const unsigned int getChannels() const;

		const bool isMusic() const;
		const bool isSound() const;
		
		const unsigned int getDesiredFramerate() const;
		const unsigned int getDesiredCPU() const;
		const unsigned int getCurrentFramerate() const;
		const unsigned int getCurrentFramesPerGeneration() const;
		
		const bool isFullScreen() const;
		const bool isGlowingButtons() const;
		const bool isTransparency() const;
		const bool isSmoothMovements() const;
		const bool isUnloadGraphics() const;
		
		const bool setBackgroundBitmap(const bool background_bitmap);
		const bool isFirstStart() const;
		
		const bool setResolution(const eResolution current_resolution);
		const bool setBitDepth(const eBitDepth current_bitdepth);
		const bool setLanguage(const eLanguage current_language);
		const bool setTheme(const eTheme current_theme);

		const bool setDesiredFramerate(const unsigned int desired_frame_rate);
		const bool setDesiredCPU(const unsigned int cpu_usage);
		const bool setCurrentFramerate(const unsigned int frame_rate);
		const bool setCurrentFramesPerGeneration(const unsigned int frames_per_generation);

		
		const bool setFullScreen(const bool full_screen);

		const bool setMusicVolume(const unsigned int music_volume);
		const bool setSoundVolume(const unsigned int sound_volume);
		const bool setChannels(const unsigned int channel_num);
		
		const bool setMusic(const bool use_music);
		const bool setSound(const bool use_sound);
		
		const bool setGlowingButtons(const bool glowing_buttons);
		const bool setTransparency(const bool trans_parency);
		const bool setSmoothMovements(const bool smooth_movements);
		const bool setUnloadGraphics(const bool unload_graphics);
		
		const bool setFirstStart(const bool first_start);
		void setConfigurationFile(const std::string& configuration_file);
		
		const bool isVisitedHelpChapter(const unsigned int chapter) const;
		void parseParameters(std::list<std::string>& arguments);
	
		static const unsigned int MIN_CPU_USAGE = 1;
		static const unsigned int MAX_CPU_USAGE = 99;
		static const unsigned int MIN_DESIRED_FRAMERATE = 1;
		static const unsigned int MAX_DESIRED_FRAMERATE = 1000;
	private:
		eLanguage language;
		eResolution resolution;
		eBitDepth bitdepth;
		eTheme theme;
		bool backgroundBitmap;

		unsigned int musicVolume;
		unsigned int soundVolume;

		unsigned int channels;
	
		bool useMusic;
		bool useSound;
		
		unsigned int desiredFramerate; // X Frames per generation
		unsigned int desiredCPU; // hold X FPS
		unsigned int currentFramerate;
		unsigned int currentFramesPerGeneration;

		bool fullScreen;
		bool glowingButtons;
		bool transparency;
		bool smoothMovements;
		bool unloadGraphics;

		bool firstStart;
		
		std::string configurationFile;

		std::set<unsigned int> visitedHelpChapters;
};

extern UI_Configuration uiConfiguration;

inline const bool UI_Configuration::isBackgroundBitmap() const {
	return(backgroundBitmap);
}

inline void UI_Configuration::setConfigurationFile(const std::string& configuration_file) {
	configurationFile = configuration_file;
}

inline const bool UI_Configuration::isFirstStart() const {
	return(firstStart);
}

inline const eResolution UI_Configuration::getResolution() const {
	return(resolution);
}

inline const eBitDepth UI_Configuration::getBitDepth() const {
	return(bitdepth);
}

inline const eLanguage UI_Configuration::getLanguage() const {
	return(language);
}

inline const eTheme UI_Configuration::getTheme() const {
	return(theme);
}

inline const unsigned int UI_Configuration::getChannels() const {
	return(channels);
}

inline const unsigned int UI_Configuration::getMusicVolume() const {
	return(musicVolume);
}

inline const unsigned int UI_Configuration::getSoundVolume() const {
	return(soundVolume);
}

inline const bool UI_Configuration::isMusic() const {
	return(useMusic);
}

inline const bool UI_Configuration::isSound() const {
	return(useSound);
}

inline const unsigned int UI_Configuration::getDesiredFramerate() const
{
#ifdef _SCC_DEBUG
	if((desiredFramerate<MIN_DESIRED_FRAMERATE)||(desiredFramerate>MAX_DESIRED_FRAMERATE)) {
		toErrorLog("WARNING: (UI_Configuration::getDesiredFramerate): Value out of range.");return(MIN_DESIRED_FRAMERATE);
	}
#endif
	return(desiredFramerate);
}

inline const unsigned int UI_Configuration::getDesiredCPU() const
{
#ifdef _SCC_DEBUG
	if((desiredCPU<MIN_CPU_USAGE)||(desiredCPU>MAX_CPU_USAGE)) {
		toErrorLog("WARNING: (UI_Configuration::getDesiredCPU): Value out of range.");
		if(desiredCPU<MIN_CPU_USAGE) return(MIN_CPU_USAGE);
		else return(MAX_CPU_USAGE);
	}
#endif
	return(desiredCPU);
}

inline const unsigned int UI_Configuration::getCurrentFramerate() const {
	return(currentFramerate);
}


inline const unsigned int UI_Configuration::getCurrentFramesPerGeneration() const {
	return(currentFramesPerGeneration);
}
inline const bool UI_Configuration::isFullScreen() const {
	return(fullScreen);
}

inline const bool UI_Configuration::isGlowingButtons() const {
	return(glowingButtons);
}

inline const bool UI_Configuration::isTransparency() const {
	return(transparency);
}

inline const bool UI_Configuration::isUnloadGraphics() const {
	return(unloadGraphics);
}

inline const bool UI_Configuration::isSmoothMovements() const {
	return(smoothMovements);
}


#endif // _UI_CONFIGURATION_HPP



