#ifndef _CORE_STATISTICS_HPP
#define _CORE_STATISTICS_HPP

#include "defs.hpp"

class STATISTICS
{
	public:
		STATISTICS();
		~STATISTICS();
		STATISTICS(const STATISTICS& object);
		STATISTICS& operator=(const STATISTICS& object);
				
		void resetData();

		void setNeedSupply(const unsigned int need_supply);
		void setHaveSupply(const unsigned int have_supply);
		void setFitness(const unsigned int fitness);
		void setHaveMinerals(const unsigned int have_minerals);
		void setHaveGas(const unsigned int have_gas);

		const unsigned int getNeedSupply() const;	// supply that is used up by all units
		const unsigned int getHaveSupply() const;	// total supply of supply buildings (overlord, supply depot, command center, ...)
		const unsigned int getFitness() const;
		const unsigned int getHaveMinerals() const; // minerals at that time
		const unsigned int getHaveGas() const;		// gas at that time
		
	private:
		unsigned int needSupply;		// supply that is used up by all units
		unsigned int haveSupply;		// total supply of supply buildings (overlord, supply depot, command center, ...)
		unsigned int minerals;			// minerals at that time
		unsigned int gas;			// gas at that time
		unsigned int fitness;	//fitness at that time
};

// ------ STATISTICS SET FUNCTIONS ------
inline void STATISTICS::setNeedSupply(const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
	if(need_supply > MAX_SUPPLY) {
		toLog("DEBUG: (STATISTICS::setNeedSupply): Value need_supply out of range.");return;
	}
#endif
	needSupply = need_supply;
}

inline void STATISTICS::setHaveSupply(const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
	if(have_supply > MAX_SUPPLY) {
		toLog("DEBUG: (STATISTICS::setHaveSupply): Value have_supply out of range.");return;
	}
#endif
	haveSupply = have_supply;
}

inline void STATISTICS::setHaveMinerals(const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
	if(have_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (STATISTICS::setHaveMinerals): Value have_minerals out of range.");return;
	}
#endif
	minerals = have_minerals;
}
																				
inline void STATISTICS::setHaveGas(const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
	if(have_gas >= MAX_GAS) {
		toLog("DEBUG: (STATISTICS::setHaveGas): Value have_gas out of range.");return;
	}
#endif
	gas = have_gas;
}

inline void STATISTICS::setFitness(const unsigned int stat_fitness)
{
#ifdef _SCC_DEBUG
	if(stat_fitness > MAX_PFITNESS) {
		toLog("DEBUG: (STATISTICS::setFitness): Value fitness out of range.");return;
	}
#endif
	fitness = stat_fitness;
}

// ------ STATISTICS GET FUNCTIONS ------
inline const unsigned int STATISTICS::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply > MAX_SUPPLY) {
		toLog("DEBUG: (STATISTICS::getNeedSupply): Variable needSupply not initialized.");	return(0);
	}
#endif
	return(needSupply);
}


inline const unsigned int STATISTICS::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if(haveSupply > MAX_SUPPLY) {
		toLog("DEBUG: (STATISTICS::getHaveSupply): Variable haveSupply not initialized.");return(0);
	}
#endif
	return(haveSupply);
}

inline const unsigned int STATISTICS::getHaveMinerals() const
{
#ifdef _SCC_DEBUG
	if(minerals > MAX_MINERALS) {
		toLog("DEBUG: (STATISTICS::getHaveMinerals): Variable minerals not initialized.");return(0);
	}
#endif
	return(minerals);
}

inline const unsigned int STATISTICS::getHaveGas() const
{
#ifdef _SCC_DEBUG
	if(gas > MAX_GAS) {
		toLog("DEBUG: (STATISTICS::getHaveGas): Variable gas not initialized.");return(0);
	}
#endif
	return(gas);
}

inline const unsigned int STATISTICS::getFitness() const
{
#ifdef _SCC_DEBUG
	// TODO max pfitness 
	if(fitness>MAX_PFITNESS) {
		toLog("DEBUG: (STATISTICS::getFitness): Variable fitness not initialized.");return(0);
	}
#endif
	return(fitness);
}


#endif

