#ifndef _EF_CONFIGURATION_HPP
#define _EF_CONFIGURATION_HPP

#include <list>
#include <map>
#include <string>
#include "../stl/misc.hpp"

class EF_Configuration
{
	private:
		unsigned int desiredFramerate; // X Frames per generation
		unsigned int desiredCPU; // hold X FPS
		unsigned int currentFramerate;
		unsigned int currentFramesPerGeneration;

		bool autoSaveRuns;
		bool restrictSC;
		bool facilityMode;
		bool fullScreen;
		bool softwareMouse;
		bool backgroundBitmap;
		bool dnaSpiral;
		bool toolTips;
		bool showDebug;
	

		std::string configurationFile;
	public:
		EF_Configuration();
		~EF_Configuration();

		static const unsigned int MIN_CPU_USAGE = 1;
		static const unsigned int MAX_CPU_USAGE = 99;
		static const unsigned int MIN_DESIRED_FRAMERATE = 1;
		static const unsigned int MAX_DESIRED_FRAMERATE = 200;


		void initDefaults();

		const unsigned int getDesiredFramerate() const;
		const unsigned int getDesiredCPU() const;
		const unsigned int getCurrentFramerate() const;
		const unsigned int getCurrentFramesPerGeneration() const;
		const bool isAutoSaveRuns() const;
		const bool isRestrictSC() const;
		const bool isFacilityMode() const;	
		const bool isSoftwareMouse() const;
		const bool isFullScreen() const;
		const bool isBackgroundBitmap() const;
		const bool isDnaSpiral() const;
		const bool isToolTips() const;
		const bool isShowDebug() const;
	
		const bool setDesiredFramerate(const unsigned int desired_frame_rate);
		const bool setDesiredCPU(const unsigned int cpu_usage);
		const bool setCurrentFramerate(const unsigned int frame_rate);
		const bool setCurrentFramesPerGeneration(const unsigned int frames_per_generation);
		const bool setAutoSaveRuns(const bool auto_save_runs);
		const bool setRestrictSC(const bool restrict_sc);
		const bool setFacilityMode(const bool facility_mode);
		const bool setSoftwareMouse(const bool software_mouse);
		const bool setFullScreen(const bool full_screen);
		const bool setBackgroundBitmap(const bool background_bitmap);
		const bool setDnaSpiral(const bool dna_spiral);
		const bool setToolTips(const bool tool_tips);
		const bool setShowDebug(const bool show_debug);

		void setConfigurationFile(const std::string& configuration_file);
		void loadConfigurationFile();
		void saveToFile() const;
};

extern EF_Configuration efConfiguration;


inline const unsigned int EF_Configuration::getDesiredFramerate() const
{
#ifdef _SCC_DEBUG
	if((desiredFramerate<MIN_DESIRED_FRAMERATE)||(desiredFramerate>MAX_DESIRED_FRAMERATE)) {
		toLog("WARNING: (EF_Configuration::getDesiredFramerate): Value out of range.");return(MIN_DESIRED_FRAMERATE);
	}
#endif
	return(desiredFramerate);
}

inline const unsigned int EF_Configuration::getDesiredCPU() const
{
#ifdef _SCC_DEBUG
	if((desiredCPU<MIN_CPU_USAGE)||(desiredCPU>MAX_CPU_USAGE)) {
		toLog("WARNING: (EF_Configuration::getDesiredCPU): Value out of range.");
		if(desiredCPU<MIN_CPU_USAGE) return(MIN_CPU_USAGE);
		else return(MAX_CPU_USAGE);
	}
#endif
	return(desiredCPU);
}

inline const unsigned int EF_Configuration::getCurrentFramerate() const {
	return(currentFramerate);
}


inline const unsigned int EF_Configuration::getCurrentFramesPerGeneration() const {
	return(currentFramesPerGeneration);
}

inline const bool EF_Configuration::isAutoSaveRuns() const {
	return(autoSaveRuns);
}

inline const bool EF_Configuration::isRestrictSC() const {
	return(restrictSC);
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



