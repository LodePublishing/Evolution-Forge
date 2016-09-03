#ifndef _UI_CONFIGURATION_HPP
#define _UI_CONFIGURATION_HPP

//#include "defs.hpp"
#include <list>
#include <map>
#include "../stl/misc.hpp"
#include <string>

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
	bool glowingButtons;
	bool transparency;
	bool smoothMovements;
	std::string configurationFile;
public:
	UI_Configuration();
	~UI_Configuration();

	void initDefaults();

	const eLanguage getLanguage() const;
	const bool isGlowingButtons() const;
	const bool isTransparency() const;
	const bool isSmoothMovements() const;
	
	void setLanguage(const eLanguage current_language);
	void setGlowingButtons(const bool glowing_buttons);
	void setTransparency(const bool trans_parency);
	void setSmoothMovements(const bool smooth_movements);

	void setConfigurationFile(const std::string& configuration_file);
	void loadConfigurationFile();
	void saveToFile() const;
};

extern UI_Configuration uiConfiguration;

inline const eLanguage UI_Configuration::getLanguage() const {
	return(language);
}

inline const bool UI_Configuration::isGlowingButtons() const {
	return(glowingButtons);
}

inline const bool UI_Configuration::isTransparency() const {
	return(transparency);
}

inline const bool UI_Configuration::isSmoothMovements() const {
	return(smoothMovements);
}

inline void UI_Configuration::setLanguage(const eLanguage current_language) {
	language = current_language;
}

inline void UI_Configuration::setGlowingButtons(const bool glowing_buttons) {
	glowingButtons = glowing_buttons;
}

inline void UI_Configuration::setTransparency(const bool trans_parency) {
	transparency = trans_parency;
}

inline void UI_Configuration::setSmoothMovements(const bool smooth_movements) {
	smoothMovements = smooth_movements;
}

#endif // _UI_CONFIGURATION_HPP



