#ifndef _CORE_CONFIGURATION_HPP
#define _CORE_CONFIGURATION_HPP

#include "game.hpp"
#include <list>
#include <map>
#include "../stl/misc.hpp"

extern const unsigned int MIN_MUTATION_FACTOR;
extern const unsigned int MAX_MUTATION_FACTOR;

extern const unsigned int MAX_BREED_FACTOR;
extern const unsigned int MIN_BREED_FACTOR;

extern const unsigned int MIN_NOISE;
extern const unsigned int MAX_NOISE;

extern const unsigned int MAX_CROSSING_OVER;
extern const unsigned int MIN_CROSSING_OVER;

class CoreConfiguration
{
// This is still a public class though access _SHOULD_ be made through the corresponding functions of settings
	public:
		CoreConfiguration();
		~CoreConfiguration();

		void initDefaults();

		const bool isFastCalculation() const;
		const bool isAllowWaitOrders() const;
		const unsigned int getWaitAccuracy() const;
		const unsigned int getCrossingOver() const;
		const unsigned int getBreedFactor() const;
		const unsigned int getMutationFactor() const;
		const unsigned int getMaxTime() const;
		const unsigned int getMaxTimeOut() const;
		const unsigned int getMaxLength() const;
		const unsigned int getNoise() const;

		const unsigned int getGameType() const;

		const bool isPreprocessBuildOrder() const;
		const bool isAllowGoalAdaption() const;
		
		const bool setFastCalculation(const bool fast_calculation);
		const bool setAllowWaitOrders(const bool allow_wait_orders);
		const bool setWaitAccuracy(const unsigned int wait_accuracy);

		const bool setCrossingOver(const unsigned int crossing_over);
		const bool setBreedFactor(const unsigned int breed_factor);
		const bool setMutationFactor(const unsigned int mutation_factor);
		const bool setMaxTime(const unsigned int max_time);
		const bool setMaxTimeOut(const unsigned int time_out);
		const bool setMaxLength(const unsigned int max_length);
		const bool setNoise(const unsigned int desired_noise);

		const bool setPreprocessBuildOrder(const bool preprocess_build_order);
		const bool setAllowGoalAdaption(const bool allow_goal_adaption);

		const bool setGameType(const unsigned int game_type_number);

		void setConfigurationFile(const std::string& configuration_file);
		void loadConfigurationFile();
		void saveToFile() const;
	private:
// internal:
		unsigned int crossingOver;			// percentage of population that should exchange their genetic informations with each other
		unsigned int breedFactor;		// percentage of population that should copy 1:1 their genetic informations 
		unsigned int mutationFactor;
		unsigned int maxTime;			// border of global searchspace in seconds (the lower the faster but needs longer to get a valid solution at the first place)
		unsigned int maxTimeOut;			// border of local searchspace in seconds (how long the program should be allowed to wait until prerequisites of an item are fulfilled)
		unsigned int maxLength;			// max length of individual (as maxTime)
		unsigned int noise; //in %
		
		bool preprocessBuildOrder; // should the program create a basic valid solution? TODO: IMPLEMENT IT!
		bool allowGoalAdaption;

	// external:
		bool fastCalculation;

		bool allowWaitOrders;
		unsigned int waitAccuracy;

		unsigned int gameType;

		std::string configurationFile;
	
};

extern CoreConfiguration coreConfiguration;

inline const unsigned int CoreConfiguration::getGameType() const {
#ifdef _SCC_DEBUG
	if(gameType > GAME::MAX_GAME_TYPES) {
		toErrorLog("DEBUG (CoreConfiguration::getGameType()): Variable gameType not initialized.");return(0);
	}
#endif
	return(gameType);
}

inline const bool CoreConfiguration::isFastCalculation() const {
	return(fastCalculation);
}

inline const bool CoreConfiguration::isAllowWaitOrders() const {
	return(allowWaitOrders);
}

inline const unsigned int CoreConfiguration::getWaitAccuracy() const {
#ifdef _SCC_DEBUG
	if((waitAccuracy<1)||(waitAccuracy>1200)) {
		toErrorLog("WARNING (CoreConfiguration::getWaitAccuracy()()): Value out of range.");return(100);
	}
#endif
	return(waitAccuracy);
}

inline const unsigned int CoreConfiguration::getCrossingOver() const {
#ifdef _SCC_DEBUG
	if((crossingOver<MIN_CROSSING_OVER)||(crossingOver>MAX_CROSSING_OVER)) {
		toErrorLog("WARNING (CoreConfiguration::getCrossingOver()): Value out of range.");return(MIN_CROSSING_OVER);
	}
#endif
	return(crossingOver);
}

inline const unsigned int CoreConfiguration::getBreedFactor() const {
#ifdef _SCC_DEBUG
	if((breedFactor<MIN_BREED_FACTOR)||(breedFactor>MAX_BREED_FACTOR)) {
		toErrorLog("WARNING (CoreConfiguration::getBreedFactor()): Value out of range.");return(MIN_BREED_FACTOR);
	}
#endif
	return(breedFactor);
}

inline const unsigned int CoreConfiguration::getMutationFactor() const {
#ifdef _SCC_DEBUG
	if((mutationFactor<MIN_MUTATION_FACTOR)||(mutationFactor>MAX_MUTATION_FACTOR)) {
		toErrorLog("WARNING (CoreConfiguration::getMutationFactor()): Value out of range.");return(MIN_MUTATION_FACTOR);
	}
#endif
	return(mutationFactor);
}

inline const unsigned int CoreConfiguration::getMaxTime() const {
#ifdef _SCC_DEBUG
	if((maxTime<MIN_TIME)||(maxTime>MAX_TIME)) {
		toErrorLog("WARNING (CoreConfiguration::getMaxTime()): Value out of range.");return(MIN_TIME);
	}
#endif
	return(maxTime);
}

inline const unsigned int CoreConfiguration::getMaxTimeOut() const {
#ifdef _SCC_DEBUG
	if((maxTimeOut<MIN_TIMEOUT)||(maxTimeOut>MAX_TIMEOUT)) {
		toErrorLog("WARNING (CoreConfiguration::getMaxTimeOut()): Value out of range.");return(MIN_TIMEOUT);
	}
#endif
	return(maxTimeOut);
}

inline const unsigned int CoreConfiguration::getMaxLength() const {
#ifdef _SCC_DEBUG
	if((maxLength<MIN_LENGTH)||(maxLength>MAX_LENGTH)) {
		toErrorLog("WARNING (CoreConfiguration::getMaxLength()): Value out of range.");return(MIN_LENGTH);
	}
#endif
	return(maxLength);
}

inline const unsigned int CoreConfiguration::getNoise() const {
#ifdef _SCC_DEBUG
	if((noise < MIN_NOISE) || (noise > MAX_NOISE)) {
		toErrorLog("WARNING (CoreConfiguration::getNoise()): Value out of range.");return(MIN_NOISE);
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

inline void CoreConfiguration::setConfigurationFile(const std::string& configuration_file) {
	configurationFile = configuration_file;
}

#endif // _CORE_CONFIGURATION_HPP



