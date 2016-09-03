#ifndef _CORE_CONFIGURATION_HPP
#define _CORE_CONFIGURATION_HPP

#include "defs.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <deque>

using namespace std;

class Configuration
{
// This is still a public class though access _SHOULD_ be made through the corresponding functions of settings
private:
	unsigned int crossingOver;			// percentage of population that should exchange their genetic informations with each other
	unsigned int breedFactor;		// percentage of population that should copy 1:1 their genetic informations 
	unsigned int mutationFactor;
	unsigned int maxTime;			// border of global searchspace in seconds (the lower the faster but needs longer to get a valid solution at the first place)
	unsigned int maxTimeOut;			// border of local searchspace in seconds (how long the program should be allowed to wait until prerequisites of an item are fulfilled)
	unsigned int maxLength;			// max length of individual (as maxTime)
	unsigned int maxRuns;			// how many different solutions should be searched
	unsigned int maxGenerations;		// how many unchanged generations shall occur until the run is terminated
	unsigned int noise; //in %
	unsigned int dynamicFramerate; // X Frames per generation
	unsigned int staticFramerate; // hold X FPS

	unsigned int currentFramerate;
	unsigned int currentFramesPerGeneration;

	eLanguage language;

	bool autoSaveRuns;
	bool restrictSC;
	bool facilityMode;
	bool preprocessBuildOrder; // should the program create a basic valid solution? TODO: IMPLEMENT IT!
	bool allowGoalAdaption;
	bool softwareMouse;
	bool fullScreen;
	bool backgroundBitmap;
	bool allowStaticFramerate;
	bool glowingButtons;
	bool dnaSpiral;
	bool tooltips;
	bool transparency;
	bool smoothMovements;
	

	string configurationFile;
	
public:
	Configuration();
	~Configuration();

	void initDefaults();

	const unsigned int getCrossingOver() const;
	const unsigned int getBreedFactor() const;
	const unsigned int getMutationFactor() const;
	const unsigned int getMaxTime() const;
	const unsigned int getMaxTimeOut() const;
	const unsigned int getMaxLength() const;
	const unsigned int getMaxRuns() const;
	const unsigned int getMaxGenerations() const;
	const unsigned int getNoise() const;
	const unsigned int getStaticFramerate() const;
	const unsigned int getDynamicFramerate() const;

	const unsigned int getCurrentFramerate() const;
	const unsigned int getCurrentFramesPerGeneration() const;

	const eLanguage getLanguage() const;


	const bool isAutoSaveRuns() const;
	const bool isRestrictSC() const;
	const bool isFacilityMode() const;	
	const bool isPreprocessBuildOrder() const;
	const bool isAllowGoalAdaption() const;
	const bool isSoftwareMouse() const;
	const bool isFullScreen() const;
	const bool isBackgroundBitmap() const;
	const bool isAllowStaticFramerate() const;
	const bool isGlowingButtons() const;
	const bool isDnaSpiral() const;
	const bool isTooltips() const;
	const bool isTransparency() const;
	const bool isSmoothMovements() const;
	
	void setCrossingOver(const unsigned int crossing_over);
	void setBreedFactor(const unsigned int breed_factor);
	void setMutationFactor(const unsigned int mutation_factor);
	void setMaxTime(const unsigned int max_time);
	void setMaxTimeOut(const unsigned int time_out);
	void setMaxLength(const unsigned int max_length);
	void setMaxRuns(const unsigned int max_runs);
	void setMaxGenerations(const unsigned int max_generations);
	void setNoise(const unsigned int desired_noise);
	void setDynamicFramerate(const unsigned int dynamic_framerate);
	void setStaticFramerate(const unsigned int static_frame_rate);

	void setLanguage(const eLanguage current_language);

	void setAutoSaveRuns(const bool auto_save_runs);
	void setRestrictSC(const bool restrict_sc);
	void setFacilityMode(const bool facility_mode);
	void setPreprocessBuildOrder(const bool preprocess_build_order);
	void setAllowGoalAdaption(const bool allow_goal_adaption);
	void setSoftwareMouse(const bool software_mouse);
	void setFullScreen(const bool full_screen);
	void setBackgroundBitmap(const bool background_bitmap);
	void setAllowStaticFramerate(const bool allow_static_framerate);
	void setGlowingButtons(const bool glowing_buttons);
	void setDnaSpiral(const bool dna_spiral);
	void setTooltips(const bool tool_tips);
	void setTransparency(const bool trans_parency);
	void setSmoothMovements(const bool smooth_movements);

	void setCurrentFramerate(const unsigned int frame_rate);
	void setCurrentFramesPerGeneration(const unsigned int frames_per_generation);
	
	void setConfigurationFile(const string& configuration_file);
	void loadConfigurationFile();
	void saveToFile() const;
};

extern Configuration configuration;

extern void parse_2nd_block(ifstream& stream, map<string, map<string, list<string> > >& block);
extern void parse_block(ifstream& stream, map<string, list<string> >& block);
extern void parse_line(string& text, list<string>& words);


inline const unsigned int Configuration::getCrossingOver() const {
#ifdef _SCC_DEBUG
	if((crossingOver<MIN_CROSSING_OVER)||(crossingOver>MAX_CROSSING_OVER)) {
		toLog("WARNING: (Configuration::getCrossingOver): Value out of range.");return(MIN_CROSSING_OVER);
	}
#endif
	return(crossingOver);
}

inline const unsigned int Configuration::getBreedFactor() const {
#ifdef _SCC_DEBUG
	if((breedFactor<MIN_BREED_FACTOR)||(breedFactor>MAX_BREED_FACTOR)) {
		toLog("WARNING: (Configuration::getBreedFactor): Value out of range.");return(MIN_BREED_FACTOR);
	}
#endif
	return(breedFactor);
}

inline const unsigned int Configuration::getMutationFactor() const {
#ifdef _SCC_DEBUG
	if((mutationFactor<MIN_MUTATION_FACTOR)||(mutationFactor>MAX_MUTATION_FACTOR)) {
		toLog("WARNING: (Configuration::getMutationFactor): Value out of range.");return(MIN_MUTATION_FACTOR);
	}
#endif
	return(mutationFactor);
}

inline const unsigned int Configuration::getMaxTime() const {
#ifdef _SCC_DEBUG
	if((maxTime<MIN_TIME)||(maxTime>MAX_TIME)) {
		toLog("WARNING: (Configuration::getMaxTime): Value out of range.");return(MIN_TIME);
	}
#endif
	return(maxTime);
}

inline const unsigned int Configuration::getMaxTimeOut() const {
#ifdef _SCC_DEBUG
	if((maxTimeOut<MIN_TIMEOUT)||(maxTimeOut>MAX_TIMEOUT)) {
		toLog("WARNING: (Configuration::getMaxTimeOut): Value out of range.");return(MIN_TIMEOUT);
	}
#endif
	return(maxTimeOut);
}

inline const unsigned int Configuration::getMaxLength() const {
#ifdef _SCC_DEBUG
	if((maxLength<MIN_LENGTH)||(maxLength>MAX_LENGTH)) {
		toLog("WARNING: (Configuration::getMaxLength): Value out of range.");return(MIN_LENGTH);
	}
#endif
	return(maxLength);
}
inline const unsigned int Configuration::getMaxRuns() const 
{
#ifdef _SCC_DEBUG
	if((maxRuns<MIN_RUNS)||(maxRuns>MAX_RUNS)) {
		toLog("WARNING: (Configuration::getMaxRuns): Value out of range.");return(MIN_RUNS);
	}
#endif
	return(maxRuns);
}

inline const unsigned int Configuration::getMaxGenerations() const 
{
#ifdef _SCC_DEBUG
	if((maxGenerations<MIN_GENERATIONS)||(maxGenerations>MAX_GENERATIONS)) {
		toLog("WARNING: (Configuration::getMaxGenerations): Value out of range.");return(MIN_NOISE);
	}
#endif
	return(maxGenerations);
}

inline const unsigned int Configuration::getNoise() const {
#ifdef _SCC_DEBUG
	if((noise < MIN_NOISE) || (noise > MAX_NOISE)) {
		toLog("WARNING: (Configuration::getNoise): Value out of range.");return(MIN_NOISE);
	}
#endif
	return(noise);
}

inline const unsigned int Configuration::getStaticFramerate() const
{
#ifdef _SCC_DEBUG
	if((staticFramerate<MIN_STATIC_FRAMERATE)||(staticFramerate>MAX_STATIC_FRAMERATE)) {
		toLog("WARNING: (Configuration::getStaticFramerate): Value out of range.");return(MIN_STATIC_FRAMERATE);
	}
#endif
	return(staticFramerate);
}

inline const unsigned int Configuration::getDynamicFramerate() const
{
#ifdef _SCC_DEBUG
	if((dynamicFramerate<MIN_DYNAMIC_FRAMERATE)||(dynamicFramerate>MAX_DYNAMIC_FRAMERATE)) {
		toLog("WARNING: (Configuration::getDynamicFramerate): Value out of range.");return(MIN_DYNAMIC_FRAMERATE);
	}
#endif
	return(dynamicFramerate);
}

inline const eLanguage Configuration::getLanguage() const {
	return(language);
}

inline const unsigned int Configuration::getCurrentFramerate() const {
	return(currentFramerate);
}

inline void Configuration::setCurrentFramerate(const unsigned int frame_rate) {
	currentFramerate = frame_rate;
}

inline void Configuration::setCurrentFramesPerGeneration(const unsigned int frames_per_generation) {
	currentFramesPerGeneration = frames_per_generation;
}

inline const unsigned int Configuration::getCurrentFramesPerGeneration() const {
	return(currentFramesPerGeneration);
}

inline const bool Configuration::isAutoSaveRuns() const {
	return(autoSaveRuns);
}

inline const bool Configuration::isRestrictSC() const {
	return(restrictSC);
}

inline const bool Configuration::isFacilityMode() const {
	return(facilityMode);
}

inline const bool Configuration::isPreprocessBuildOrder() const {
	return(preprocessBuildOrder);
}

inline const bool Configuration::isAllowGoalAdaption() const {
	return(allowGoalAdaption);
}

inline const bool Configuration::isSoftwareMouse() const {
	return(softwareMouse);
}

inline const bool Configuration::isFullScreen() const {
	return(fullScreen);
}

inline const bool Configuration::isBackgroundBitmap() const {
	return(backgroundBitmap);
}

inline const bool Configuration::isAllowStaticFramerate() const {
	return(allowStaticFramerate);
}

inline const bool Configuration::isGlowingButtons() const {
	return(glowingButtons);
}

inline const bool Configuration::isDnaSpiral() const {
	return(dnaSpiral);
}

inline const bool Configuration::isTooltips() const {
	return(tooltips);
}

inline const bool Configuration::isTransparency() const {
	return(transparency);
}

inline const bool Configuration::isSmoothMovements() const {
	return(smoothMovements);
}

inline void Configuration::setCrossingOver(const unsigned int crossing_over) 
{
#ifdef _SCC_DEBUG
	if((crossing_over<MIN_CROSSING_OVER)||(crossing_over>MAX_CROSSING_OVER)) {
		toLog("WARNING: (Configuration::setCrossingOver): Value out of range.");return;
	}
#endif
	crossingOver = crossing_over;
}

inline void Configuration::setBreedFactor(const unsigned int breed_factor) 
{
#ifdef _SCC_DEBUG
	if((breed_factor<MIN_BREED_FACTOR)||(breed_factor>MAX_BREED_FACTOR)) {
		toLog("WARNING: (Configuration::setBreedFactor): Value out of range.");return;
	}
#endif
	breedFactor = breed_factor;
}
inline void Configuration::setMutationFactor(const unsigned int mutation_factor) 
{
#ifdef _SCC_DEBUG
	if((mutation_factor<MIN_MUTATION_FACTOR)||(mutation_factor>MAX_MUTATION_FACTOR)) {
		toLog("WARNING: (Configuration::setMutationFactor): Value out of range.");return;
	}
#endif
	mutationFactor = mutation_factor;
}

inline void Configuration::setMaxTime(const unsigned int max_time) 
{
#ifdef _SCC_DEBUG
	if((max_time<MIN_TIME)||(max_time>MAX_TIME)) {
		toLog("WARNING: (Configuration::setMaxTime): Value out of range.");return;
	}
#endif
	maxTime = max_time;
}

inline void Configuration::setMaxTimeOut(const unsigned int time_out) 
{
#ifdef _SCC_DEBUG
	if((time_out<MIN_TIMEOUT)||(time_out>MAX_TIMEOUT)) {
		toLog("WARNING: (Configuration::setMaxTimeOut): Value out of range.");return;
	}
#endif
	maxTimeOut = time_out;
}

inline void Configuration::setMaxLength(const unsigned int max_length) 
{
#ifdef _SCC_DEBUG
	if((max_length<MIN_LENGTH)||(max_length>MAX_LENGTH)) {
		toLog("WARNING: (Configuration::setMaxLength): Value out of range.");return;
	}
#endif
	maxLength = max_length;
}

inline void Configuration::setMaxRuns(const unsigned int max_runs) 
{
#ifdef _SCC_DEBUG
	if((max_runs<MIN_RUNS)||(max_runs>MAX_RUNS)) {
		toLog("WARNING: (Configuration::setMaxRuns): Value out of range.");return;
	}
#endif
	maxRuns = max_runs;
}

inline void Configuration::setMaxGenerations(const unsigned int max_generations) 
{
#ifdef _SCC_DEBUG
	if((max_generations<MIN_GENERATIONS)||(max_generations>MAX_GENERATIONS)) {
		toLog("WARNING: (Configuration::setMaxGenerations): Value out of range.");return;
	}
#endif
	maxGenerations = max_generations;
}

inline void Configuration::setNoise(const unsigned int desired_noise)
{
#ifdef _SCC_DEBUG
	if((desired_noise < MIN_NOISE)||(desired_noise > MAX_NOISE)) {
		toLog("WARNING: (Configuration::setNoise): Value out of range.");return;
	}
#endif
	noise = desired_noise;
}


inline void Configuration::setDynamicFramerate(const unsigned int dynamic_framerate)
{
#ifdef _SCC_DEBUG
	if((dynamic_framerate<MIN_DYNAMIC_FRAMERATE)||(dynamic_framerate>MAX_DYNAMIC_FRAMERATE)) {
		toLog("WARNING: (Configuration::setDynamicFramerate): Value out of range.");return;
	}
#endif
	dynamicFramerate = dynamic_framerate;
}

inline void Configuration::setStaticFramerate(const unsigned int frame_rate)
{
#ifdef _SCC_DEBUG
	if((frame_rate<MIN_STATIC_FRAMERATE)||(frame_rate>MAX_STATIC_FRAMERATE)) {
		toLog("WARNING: (Configuration::setStaticFramerate): Value out of range.");return;
	}
#endif
	staticFramerate = frame_rate;
}

inline void Configuration::setLanguage(const eLanguage current_language) {
	language = current_language;
}

inline void Configuration::setAutoSaveRuns(const bool auto_save_runs) {
	autoSaveRuns = auto_save_runs;
}

inline void Configuration::setRestrictSC(const bool restrict_sc) {
	restrictSC = restrict_sc;
}


inline void Configuration::setFacilityMode(const bool facility_mode) {
	facilityMode = facility_mode;
}

inline void Configuration::setPreprocessBuildOrder(const bool preprocess_build_order) {
	preprocessBuildOrder = preprocess_build_order;
}

inline void Configuration::setAllowGoalAdaption(const bool allow_goal_adaption) {
	allowGoalAdaption = allow_goal_adaption;
}

inline void Configuration::setSoftwareMouse(const bool software_mouse) {
	softwareMouse = software_mouse;
}

inline void Configuration::setFullScreen(const bool full_screen) {
	fullScreen = full_screen;
}

inline void Configuration::setBackgroundBitmap(const bool background_bitmap) {
	backgroundBitmap = background_bitmap;
}

inline void Configuration::setAllowStaticFramerate(const bool allow_static_framerate) {
	allowStaticFramerate = allow_static_framerate;
}

inline void Configuration::setGlowingButtons(const bool glowing_buttons) {
	glowingButtons = glowing_buttons;
}

inline void Configuration::setDnaSpiral(const bool dna_spiral) {
	dnaSpiral = dna_spiral;
}

inline void Configuration::setTooltips(const bool tool_tips) {
	tooltips = tool_tips;
}

inline void Configuration::setTransparency(const bool trans_parency) {
	transparency = trans_parency;
}

inline void Configuration::setSmoothMovements(const bool smooth_movements) {
	smoothMovements = smooth_movements;
}



#endif // _CORE_CONFIGURATION_HPP



