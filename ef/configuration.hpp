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
				
		const unsigned int getDesiredFramerate() const;
		const unsigned int getDesiredCPU() const;
		const unsigned int getCurrentFramerate() const;
		const unsigned int getCurrentFramesPerGeneration() const;
		const bool isAutoRuns() const;
		const bool isWaitAfterChange() const;
		const bool isCompactDisplayMode() const;
		const bool isFacilityMode() const;
		const bool isSoftwareMouse() const;
		const bool isFullScreen() const;
		const bool isBackgroundBitmap() const;
		const bool isRaceSpecificTheme() const;
		const bool isDnaSpiral() const;
		const bool isToolTips() const;
		const bool isShowDebug() const;
		const unsigned int getMaxGenerations() const;
		const unsigned int getGameSpeed() const;

		const bool setDesiredFramerate(const unsigned int desired_frame_rate);
		const bool setDesiredCPU(const unsigned int cpu_usage);
		const bool setCurrentFramerate(const unsigned int frame_rate);
		const bool setCurrentFramesPerGeneration(const unsigned int frames_per_generation);
		const bool setAutoRuns(const bool auto_runs);
		const bool setWaitAfterChange(const bool wait_after_change);
		const bool setCompactDisplayMode(const bool compact_display_mode);
		const bool setFacilityMode(const bool facility_mode);
		const bool setSoftwareMouse(const bool software_mouse);
		const bool setFullScreen(const bool full_screen);
		const bool setBackgroundBitmap(const bool background_bitmap);
		const bool setRaceSpecificTheme(const bool race_specific_theme);
		const bool setDnaSpiral(const bool dna_spiral);
		const bool setToolTips(const bool tool_tips);
		const bool setShowDebug(const bool show_debug);
		const bool setMaxGenerations(const unsigned int max_generations);
		const bool setGameSpeed(const unsigned int game_speed);
		
		void setConfigurationFile(const std::string& configuration_file);
		void loadConfigurationFile();
		void saveToFile() const;

		static const unsigned int MIN_CPU_USAGE = 1;
		static const unsigned int MAX_CPU_USAGE = 99;
		static const unsigned int MIN_DESIRED_FRAMERATE = 1;
		static const unsigned int MAX_DESIRED_FRAMERATE = 200;
	private:
		unsigned int desiredFramerate; // X Frames per generation
		unsigned int desiredCPU; // hold X FPS
		unsigned int currentFramerate;
		unsigned int currentFramesPerGeneration;

		bool autoRuns;
		bool waitAfterChange;
		bool compactDisplayMode;
		bool facilityMode;
		bool fullScreen;
		bool softwareMouse;
		bool backgroundBitmap;
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

inline const unsigned int EF_Configuration::getDesiredFramerate() const
{
#ifdef _SCC_DEBUG
	if((desiredFramerate<MIN_DESIRED_FRAMERATE)||(desiredFramerate>MAX_DESIRED_FRAMERATE)) {
		toErrorLog("WARNING: (EF_Configuration::getDesiredFramerate): Value out of range.");return(MIN_DESIRED_FRAMERATE);
	}
#endif
	return(desiredFramerate);
}

inline const unsigned int EF_Configuration::getDesiredCPU() const
{
#ifdef _SCC_DEBUG
	if((desiredCPU<MIN_CPU_USAGE)||(desiredCPU>MAX_CPU_USAGE)) {
		toErrorLog("WARNING: (EF_Configuration::getDesiredCPU): Value out of range.");
		if(desiredCPU<MIN_CPU_USAGE) return(MIN_CPU_USAGE);
		else return(MAX_CPU_USAGE);
	}
#endif
	return(desiredCPU);
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

inline const unsigned int EF_Configuration::getCurrentFramerate() const {
	return(currentFramerate);
}


inline const unsigned int EF_Configuration::getCurrentFramesPerGeneration() const {
	return(currentFramesPerGeneration);
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

inline const bool EF_Configuration::isFullScreen() const {
	return(fullScreen);
}

inline const bool EF_Configuration::isBackgroundBitmap() const {
	return(backgroundBitmap);
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



