#ifndef _EF_CONFIGURATION_HPP
#define _EF_CONFIGURATION_HPP

#include <list>
#include <map>
#include <string>
#include "../stl/misc.hpp"

class EF_Configuration
{
	private:
		unsigned int dynamicFramerate; // X Frames per generation
		unsigned int staticFramerate; // hold X FPS
		unsigned int currentFramerate;
		unsigned int currentFramesPerGeneration;

		bool autoSaveRuns;
		bool restrictSC;
		bool facilityMode;
		bool fullScreen;
		bool softwareMouse;
		bool backgroundBitmap;
		bool allowStaticFramerate;
		bool dnaSpiral;
		bool toolTips;

		std::string configurationFile;
	public:
		EF_Configuration();
		~EF_Configuration();

		static const unsigned int MIN_DYNAMIC_FRAMERATE = 0;
		static const unsigned int MAX_DYNAMIC_FRAMERATE = 100;
		static const unsigned int MIN_STATIC_FRAMERATE = 1;
		static const unsigned int MAX_STATIC_FRAMERATE = 100;


		void initDefaults();

		const unsigned int getStaticFramerate() const;
		const unsigned int getDynamicFramerate() const;
		const unsigned int getCurrentFramerate() const;
		const unsigned int getCurrentFramesPerGeneration() const;
		const bool isAutoSaveRuns() const;
		const bool isRestrictSC() const;
		const bool isFacilityMode() const;	
		const bool isSoftwareMouse() const;
		const bool isFullScreen() const;
		const bool isBackgroundBitmap() const;
		const bool isAllowStaticFramerate() const;
		const bool isDnaSpiral() const;
		const bool isToolTips() const;
	
		void setDynamicFramerate(const unsigned int dynamic_framerate);
		void setStaticFramerate(const unsigned int static_frame_rate);
		void setCurrentFramerate(const unsigned int frame_rate);
		void setCurrentFramesPerGeneration(const unsigned int frames_per_generation);
		void setAutoSaveRuns(const bool auto_save_runs);
		void setRestrictSC(const bool restrict_sc);
		void setFacilityMode(const bool facility_mode);
		void setSoftwareMouse(const bool software_mouse);
		void setFullScreen(const bool full_screen);
		void setBackgroundBitmap(const bool background_bitmap);
		void setAllowStaticFramerate(const bool allow_static_framerate);
		void setDnaSpiral(const bool dna_spiral);
		void setToolTips(const bool tool_tips);

		void setConfigurationFile(const std::string& configuration_file);
		void loadConfigurationFile();
		void saveToFile() const;
};

extern EF_Configuration efConfiguration;

//extern void parse_2nd_block(std::ifstream& stream, std::map<std::string, std::map<std::string, std::list<std::string> > >& block);
//extern void parse_block(std::ifstream& stream, std::map<std::string, std::list<std::string> >& block);
//extern void parse_line(std::string& text, std::list<std::string>& words);


inline const unsigned int EF_Configuration::getStaticFramerate() const
{
#ifdef _SCC_DEBUG
	if((staticFramerate<MIN_STATIC_FRAMERATE)||(staticFramerate>MAX_STATIC_FRAMERATE)) {
		toLog("WARNING: (EF_Configuration::getStaticFramerate): Value out of range.");return(MIN_STATIC_FRAMERATE);
	}
#endif
	return(staticFramerate);
}

inline const unsigned int EF_Configuration::getDynamicFramerate() const
{
#ifdef _SCC_DEBUG
	if((dynamicFramerate<MIN_DYNAMIC_FRAMERATE)||(dynamicFramerate>MAX_DYNAMIC_FRAMERATE)) {
		toLog("WARNING: (EF_Configuration::getDynamicFramerate): Value out of range.");return(MIN_DYNAMIC_FRAMERATE);
	}
#endif
	return(dynamicFramerate);
}

inline const unsigned int EF_Configuration::getCurrentFramerate() const {
	return(currentFramerate);
}

inline void EF_Configuration::setCurrentFramerate(const unsigned int frame_rate) {
	currentFramerate = frame_rate;
}

inline void EF_Configuration::setCurrentFramesPerGeneration(const unsigned int frames_per_generation) {
	currentFramesPerGeneration = frames_per_generation;
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

inline const bool EF_Configuration::isAllowStaticFramerate() const {
	return(allowStaticFramerate);
}

inline const bool EF_Configuration::isDnaSpiral() const {
	return(dnaSpiral);
}

inline const bool EF_Configuration::isToolTips() const {
	return(toolTips);
}

inline void EF_Configuration::setDynamicFramerate(const unsigned int dynamic_framerate)
{
#ifdef _SCC_DEBUG
	if((dynamic_framerate<MIN_DYNAMIC_FRAMERATE)||(dynamic_framerate>MAX_DYNAMIC_FRAMERATE)) {
		toLog("WARNING: (EF_Configuration::setDynamicFramerate): Value out of range.");return;
	}
#endif
	dynamicFramerate = dynamic_framerate;
}

inline void EF_Configuration::setStaticFramerate(const unsigned int frame_rate)
{
#ifdef _SCC_DEBUG
	if((frame_rate<MIN_STATIC_FRAMERATE)||(frame_rate>MAX_STATIC_FRAMERATE)) {
		toLog("WARNING: (EF_Configuration::setStaticFramerate): Value out of range.");return;
	}
#endif
	staticFramerate = frame_rate;
}

inline void EF_Configuration::setAutoSaveRuns(const bool auto_save_runs) {
	autoSaveRuns = auto_save_runs;
}

inline void EF_Configuration::setRestrictSC(const bool restrict_sc) {
	restrictSC = restrict_sc;
}

inline void EF_Configuration::setFacilityMode(const bool facility_mode) {
	facilityMode = facility_mode;
}

inline void EF_Configuration::setSoftwareMouse(const bool software_mouse) {
	softwareMouse = software_mouse;
}

inline void EF_Configuration::setFullScreen(const bool full_screen) {
	fullScreen = full_screen;
}

inline void EF_Configuration::setBackgroundBitmap(const bool background_bitmap) {
	backgroundBitmap = background_bitmap;
}

inline void EF_Configuration::setAllowStaticFramerate(const bool allow_static_framerate) {
	allowStaticFramerate = allow_static_framerate;
}

inline void EF_Configuration::setDnaSpiral(const bool dna_spiral) {
	dnaSpiral = dna_spiral;
}

inline void EF_Configuration::setToolTips(const bool tool_tips) {
	toolTips = tool_tips;
}

#endif // _EF_CONFIGURATION_HPP



