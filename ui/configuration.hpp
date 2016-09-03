#ifndef _UI_CONFIGURATION_HPP
#define _UI_CONFIGURATION_HPP

#include <list>
#include <map>
#include "../stl/misc.hpp"
#include <string>
#include "../sdl/dc.hpp"


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
// This is still a public class though access _SHOULD_ be made through the corresponding functions of settings
private:
	eLanguage language;
	eResolution resolution;
	eBitDepth bitdepth;
	eTheme theme;
	
	bool glowingButtons;
	bool transparency;
	bool smoothMovements;
	bool unloadGraphics;
	std::string configurationFile;
public:
	UI_Configuration();
	~UI_Configuration();

	void initDefaults();

	const eLanguage getLanguage() const;
	const eResolution getResolution() const;
	const eBitDepth getBitDepth() const;
	const eTheme getTheme() const;
	const bool isGlowingButtons() const;
	const bool isTransparency() const;
	const bool isSmoothMovements() const;
	const bool isUnloadGraphics() const;
	
	const bool setResolution(const eResolution current_resolution);
	const bool setBitDepth(const eBitDepth current_bitdepth);
	const bool setLanguage(const eLanguage current_language);
	const bool setTheme(const eTheme current_theme);
	
	const bool setGlowingButtons(const bool glowing_buttons);
	const bool setTransparency(const bool trans_parency);
	const bool setSmoothMovements(const bool smooth_movements);
	const bool setUnloadGraphics(const bool unload_graphics);

	void setConfigurationFile(const std::string& configuration_file);
	void loadConfigurationFile();
	void saveToFile() const;
};

extern UI_Configuration uiConfiguration;

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



