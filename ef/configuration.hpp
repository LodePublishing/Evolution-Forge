#ifndef _EF_CONFIGURATION_HPP
#define _EF_CONFIGURATION_HPP

#include "../stl/misc.hpp"

extern const unsigned int MAX_GENERATIONS;
extern const unsigned int MIN_GENERATIONS;

class EF_Configuration
{
	public:
		EF_Configuration();
		~EF_Configuration();

		void initDefaults();
				
		const bool isAutoRuns() const;
		const bool isWaitAfterChange() const;
		const bool isCompactDisplayMode() const;
		const bool isFacilityMode() const;
		const bool isSoftwareMouse() const;
		const bool isRaceSpecificTheme() const;
		const bool isDnaSpiral() const;
		const bool isToolTips() const;
		const bool isShowDebug() const;
		const unsigned int getMaxGenerations() const;
		const unsigned int getGameSpeed() const;


		const bool setAutoRuns(const bool auto_runs);
		const bool setWaitAfterChange(const bool wait_after_change);
		const bool setCompactDisplayMode(const bool compact_display_mode);
		const bool setFacilityMode(const bool facility_mode);
		const bool setSoftwareMouse(const bool software_mouse);
		const bool setRaceSpecificTheme(const bool race_specific_theme);
		const bool setDnaSpiral(const bool dna_spiral);
		const bool setToolTips(const bool tool_tips);
		const bool setShowDebug(const bool show_debug);
		const bool setMaxGenerations(const unsigned int max_generations);
		const bool setGameSpeed(const unsigned int game_speed);
		
		void setConfigurationFile(const std::string& configuration_file);
		void loadConfigurationFile();
		void saveToFile() const;

	private:
		bool autoRuns;
		bool waitAfterChange;
		bool compactDisplayMode;
		bool facilityMode;
		bool softwareMouse;
		bool raceSpecificTheme;
		bool dnaSpiral;
		bool toolTips;
		bool showDebug;
		unsigned int maxGenerations;		// how many unchanged generations shall occur until the run is terminated
		unsigned int gameSpeed;
		std::string configurationFile;	
};

extern EF_Configuration efConfiguration;

inline const unsigned int EF_Configuration::getGameSpeed() const {
#ifdef _SCC_DEBUG
	if(gameSpeed>6) {
                toErrorLog("WARNING (CoreConfiguration::getGameSpeed()): Value out of range.");
		return(3);
        }
#endif
        return(gameSpeed);
}


inline const unsigned int EF_Configuration::getMaxGenerations() const 
{
#ifdef _SCC_DEBUG
	if((maxGenerations<MIN_GENERATIONS)||(maxGenerations>MAX_GENERATIONS)) {
		toErrorLog("WARNING: (EF_Configuration::getMaxGenerations): Value out of range.");return(MIN_GENERATIONS);
	}
#endif
	return(maxGenerations);
}


inline const bool EF_Configuration::isAutoRuns() const {
	return(autoRuns);
}

inline const bool EF_Configuration::isWaitAfterChange() const {
	return(waitAfterChange);
}

inline const bool EF_Configuration::isCompactDisplayMode() const {
	return(compactDisplayMode);
}

inline const bool EF_Configuration::isFacilityMode() const {
	return(facilityMode);
}

inline const bool EF_Configuration::isSoftwareMouse() const {
	return(softwareMouse);
}



inline const bool EF_Configuration::isRaceSpecificTheme() const {
	return(raceSpecificTheme);
}

inline const bool EF_Configuration::isDnaSpiral() const {
	return(dnaSpiral);
}

inline const bool EF_Configuration::isToolTips() const {
	return(toolTips);
}

inline const bool EF_Configuration::isShowDebug() const {
	return(showDebug);
}

#endif // _EF_CONFIGURATION_HPP



