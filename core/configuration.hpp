#ifndef _CORE_CONFIGURATION_HPP
#define _CORE_CONFIGURATION_HPP

#include "defs.hpp"
#include <list>
#include <map>
#include "../stl/misc.hpp"

class CoreConfiguration
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
	
	bool preprocessBuildOrder; // should the program create a basic valid solution? TODO: IMPLEMENT IT!
	bool allowGoalAdaption;
	bool alwaysBuildWorker;

	bool autoSaveRuns;  // raus
	bool restrictSC;
	bool facilityMode;

	std::string configurationFile;
public:
	CoreConfiguration();
	~CoreConfiguration();

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

	const bool isAutoSaveRuns() const;
	const bool isRestrictSC() const;
	const bool isFacilityMode() const;	
	
	const bool isPreprocessBuildOrder() const;
	const bool isAllowGoalAdaption() const;
	const bool isAlwaysBuildWorker() const;
	
	void setCrossingOver(const unsigned int crossing_over);
	void setBreedFactor(const unsigned int breed_factor);
	void setMutationFactor(const unsigned int mutation_factor);
	void setMaxTime(const unsigned int max_time);
	void setMaxTimeOut(const unsigned int time_out);
	void setMaxLength(const unsigned int max_length);
	void setMaxRuns(const unsigned int max_runs);
	void setMaxGenerations(const unsigned int max_generations);
	void setNoise(const unsigned int desired_noise);

	void setAutoSaveRuns(const bool auto_save_runs);
	void setRestrictSC(const bool restrict_sc);
	void setFacilityMode(const bool facility_mode);
	void setPreprocessBuildOrder(const bool preprocess_build_order);
	void setAllowGoalAdaption(const bool allow_goal_adaption);
	void setAlwaysBuildWorker(const bool always_build_scv);

	void setConfigurationFile(const std::string& configuration_file);
	void loadConfigurationFile();
	void saveToFile() const;
};

extern CoreConfiguration coreConfiguration;

inline const unsigned int CoreConfiguration::getCrossingOver() const {
#ifdef _SCC_DEBUG
	if((crossingOver<MIN_CROSSING_OVER)||(crossingOver>MAX_CROSSING_OVER)) {
		toLog("WARNING: (CoreConfiguration::getCrossingOver): Value out of range.");return(MIN_CROSSING_OVER);
	}
#endif
	return(crossingOver);
}

inline const unsigned int CoreConfiguration::getBreedFactor() const {
#ifdef _SCC_DEBUG
	if((breedFactor<MIN_BREED_FACTOR)||(breedFactor>MAX_BREED_FACTOR)) {
		toLog("WARNING: (CoreConfiguration::getBreedFactor): Value out of range.");return(MIN_BREED_FACTOR);
	}
#endif
	return(breedFactor);
}

inline const unsigned int CoreConfiguration::getMutationFactor() const {
#ifdef _SCC_DEBUG
	if((mutationFactor<MIN_MUTATION_FACTOR)||(mutationFactor>MAX_MUTATION_FACTOR)) {
		toLog("WARNING: (CoreConfiguration::getMutationFactor): Value out of range.");return(MIN_MUTATION_FACTOR);
	}
#endif
	return(mutationFactor);
}

inline const unsigned int CoreConfiguration::getMaxTime() const {
#ifdef _SCC_DEBUG
	if((maxTime<MIN_TIME)||(maxTime>MAX_TIME)) {
		toLog("WARNING: (CoreConfiguration::getMaxTime): Value out of range.");return(MIN_TIME);
	}
#endif
	return(maxTime);
}

inline const unsigned int CoreConfiguration::getMaxTimeOut() const {
#ifdef _SCC_DEBUG
	if((maxTimeOut<MIN_TIMEOUT)||(maxTimeOut>MAX_TIMEOUT)) {
		toLog("WARNING: (CoreConfiguration::getMaxTimeOut): Value out of range.");return(MIN_TIMEOUT);
	}
#endif
	return(maxTimeOut);
}

inline const unsigned int CoreConfiguration::getMaxLength() const {
#ifdef _SCC_DEBUG
	if((maxLength<MIN_LENGTH)||(maxLength>MAX_LENGTH)) {
		toLog("WARNING: (CoreConfiguration::getMaxLength): Value out of range.");return(MIN_LENGTH);
	}
#endif
	return(maxLength);
}
inline const unsigned int CoreConfiguration::getMaxRuns() const 
{
#ifdef _SCC_DEBUG
	if((maxRuns<MIN_RUNS)||(maxRuns>MAX_RUNS)) {
		toLog("WARNING: (CoreConfiguration::getMaxRuns): Value out of range.");return(MIN_RUNS);
	}
#endif
	return(maxRuns);
}

inline const unsigned int CoreConfiguration::getMaxGenerations() const 
{
#ifdef _SCC_DEBUG
	if((maxGenerations<MIN_GENERATIONS)||(maxGenerations>MAX_GENERATIONS)) {
		toLog("WARNING: (CoreConfiguration::getMaxGenerations): Value out of range.");return(MIN_NOISE);
	}
#endif
	return(maxGenerations);
}

inline const unsigned int CoreConfiguration::getNoise() const {
#ifdef _SCC_DEBUG
	if((noise < MIN_NOISE) || (noise > MAX_NOISE)) {
		toLog("WARNING: (CoreConfiguration::getNoise): Value out of range.");return(MIN_NOISE);
	}
#endif
	return(noise);
}

inline const bool CoreConfiguration::isPreprocessBuildOrder() const {
	return(preprocessBuildOrder);
}

inline const bool CoreConfiguration::isAllowGoalAdaption() const {
	return(allowGoalAdaption);
}

inline const bool CoreConfiguration::isAlwaysBuildWorker() const {
	return(alwaysBuildWorker);
}

inline void CoreConfiguration::setCrossingOver(const unsigned int crossing_over) 
{
#ifdef _SCC_DEBUG
	if((crossing_over<MIN_CROSSING_OVER)||(crossing_over>MAX_CROSSING_OVER)) {
		toLog("WARNING: (CoreConfiguration::setCrossingOver): Value out of range.");return;
	}
#endif
	crossingOver = crossing_over;
}

inline void CoreConfiguration::setBreedFactor(const unsigned int breed_factor) 
{
#ifdef _SCC_DEBUG
	if((breed_factor<MIN_BREED_FACTOR)||(breed_factor>MAX_BREED_FACTOR)) {
		toLog("WARNING: (CoreConfiguration::setBreedFactor): Value out of range.");return;
	}
#endif
	breedFactor = breed_factor;
}
inline void CoreConfiguration::setMutationFactor(const unsigned int mutation_factor) 
{
#ifdef _SCC_DEBUG
	if((mutation_factor<MIN_MUTATION_FACTOR)||(mutation_factor>MAX_MUTATION_FACTOR)) {
		toLog("WARNING: (CoreConfiguration::setMutationFactor): Value out of range.");return;
	}
#endif
	mutationFactor = mutation_factor;
}

inline void CoreConfiguration::setMaxTime(const unsigned int max_time) 
{
#ifdef _SCC_DEBUG
	if((max_time<MIN_TIME)||(max_time>MAX_TIME)) {
		toLog("WARNING: (CoreConfiguration::setMaxTime): Value out of range.");return;
	}
#endif
	maxTime = max_time;
}

inline void CoreConfiguration::setMaxTimeOut(const unsigned int time_out) 
{
#ifdef _SCC_DEBUG
	if((time_out<MIN_TIMEOUT)||(time_out>MAX_TIMEOUT)) {
		toLog("WARNING: (CoreConfiguration::setMaxTimeOut): Value out of range.");return;
	}
#endif
	maxTimeOut = time_out;
}

inline void CoreConfiguration::setMaxLength(const unsigned int max_length) 
{
#ifdef _SCC_DEBUG
	if((max_length<MIN_LENGTH)||(max_length>MAX_LENGTH)) {
		toLog("WARNING: (CoreConfiguration::setMaxLength): Value out of range.");return;
	}
#endif
	maxLength = max_length;
}

inline void CoreConfiguration::setMaxRuns(const unsigned int max_runs) 
{
#ifdef _SCC_DEBUG
	if((max_runs<MIN_RUNS)||(max_runs>MAX_RUNS)) {
		toLog("WARNING: (CoreConfiguration::setMaxRuns): Value out of range.");return;
	}
#endif
	maxRuns = max_runs;
}

inline void CoreConfiguration::setMaxGenerations(const unsigned int max_generations) 
{
#ifdef _SCC_DEBUG
	if((max_generations<MIN_GENERATIONS)||(max_generations>MAX_GENERATIONS)) {
		toLog("WARNING: (CoreConfiguration::setMaxGenerations): Value out of range.");return;
	}
#endif
	maxGenerations = max_generations;
}

inline void CoreConfiguration::setNoise(const unsigned int desired_noise)
{
#ifdef _SCC_DEBUG
	if((desired_noise < MIN_NOISE)||(desired_noise > MAX_NOISE)) {
		toLog("WARNING: (CoreConfiguration::setNoise): Value out of range.");return;
	}
#endif
	noise = desired_noise;
}

inline void CoreConfiguration::setPreprocessBuildOrder(const bool preprocess_build_order) {
	preprocessBuildOrder = preprocess_build_order;
}

inline void CoreConfiguration::setAllowGoalAdaption(const bool allow_goal_adaption) {
	allowGoalAdaption = allow_goal_adaption;
}

inline void CoreConfiguration::setAlwaysBuildWorker(const bool always_build_worker) {
	alwaysBuildWorker = always_build_worker;
}

#endif // _CORE_CONFIGURATION_HPP



