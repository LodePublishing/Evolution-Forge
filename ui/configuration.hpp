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
	DARK_RED_THEME,
	DARK_BLUE_THEME,
	GREEN_THEME,
	YELLOW_THEME,
	GREY_THEME,

	MAX_COLOR_THEMES
};
// ------ END GENERAL THEMES ------



// ------ LANGUAGES ------
enum eLanguage
{
	ZERO_LANGUAGE,
	ENGLISH_LANGUAGE,
	GERMAN_LANGUAGE,
	ITALIAN_LANGUAGE,
	PORTUGESE_LANGUAGE,
	DUTCH_LANGUAGE,
	FINNISH_LANGUAGE,
	GREEK_LANGUAGE,
	FRENCH_LANGUAGE,
	SPANISH_LANGUAGE,
	POLSKI_LANGUAGE,
	KOREAN_LANGUAGE,
	CHINESE_LANGUAGE,
	RUSSIAN_LANGUAGE,

	MAX_LANGUAGES
};
// ------ END LANGUAGES ------


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

#ifndef _NO_FMOD_SOUND
		const unsigned int getMusicVolume() const;
		const unsigned int getSoundVolume() const;

		const unsigned int getChannels() const;

		const bool isMusic() const;
		const bool isSound() const;
#endif
		
		const bool isGlowingButtons() const;
		const bool isTransparency() const;
		const bool isSmoothMovements() const;
		const bool isUnloadGraphics() const;
		
		const bool isFirstStart() const;
		
		const bool setResolution(const eResolution current_resolution);
		const bool setBitDepth(const eBitDepth current_bitdepth);
		const bool setLanguage(const eLanguage current_language);
		const bool setTheme(const eTheme current_theme);

#ifndef _NO_FMOD_SOUND
		const bool setMusicVolume(const unsigned int music_volume);
		const bool setSoundVolume(const unsigned int sound_volume);
		const bool setChannels(const unsigned int channel_num);
		
		const bool setMusic(const bool use_music);
		const bool setSound(const bool use_sound);
#endif
		const bool setGlowingButtons(const bool glowing_buttons);
		const bool setTransparency(const bool trans_parency);
		const bool setSmoothMovements(const bool smooth_movements);
		const bool setUnloadGraphics(const bool unload_graphics);
		
		const bool setFirstStart(const bool first_start);
		void setConfigurationFile(const std::string& configuration_file);
		
		const bool isVisitedHelpChapter(const unsigned int chapter) const;
		
	private:
		eLanguage language;
		eResolution resolution;
		eBitDepth bitdepth;
		eTheme theme;

#ifndef _NO_FMOD_SOUND
		unsigned int musicVolume;
		unsigned int soundVolume;

		unsigned int channels;
	
		bool useMusic;
		bool useSound;
#endif
		bool glowingButtons;
		bool transparency;
		bool smoothMovements;
		bool unloadGraphics;

		bool firstStart;
		
		std::string configurationFile;

		std::set<unsigned int> visitedHelpChapters;
};

extern UI_Configuration uiConfiguration;

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

#ifndef _NO_FMOD_SOUND
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
#endif

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



