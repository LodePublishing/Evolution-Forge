#ifndef _CORE_STATISTICS_HPP
#define _CORE_STATISTICS_HPP

#include "game.hpp"
#include "../stl/misc.hpp"

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
		void setHaveResource(const unsigned int resource_type, const unsigned int have_resource);
		void setTime(const unsigned int this_time);

		const unsigned int getNeedSupply() const;	// supply that is used up by all units
		const unsigned int getHaveSupply() const;	// total supply of supply buildings (overlord, supply depot, command center, ...)
		const unsigned int getFitness() const;
		const unsigned int getHaveResource(const unsigned int resource_type) const;
		const unsigned int getTime() const;
		
	private:
		unsigned int needSupply;		// supply that is used up by all units
		unsigned int haveSupply;		// total supply of supply buildings (overlord, supply depot, command center, ...)
		std::vector<unsigned int> resource;
		unsigned int fitness;	//fitness at that time
		unsigned int time;
};

// ------ STATISTICS SET FUNCTIONS ------
inline void STATISTICS::setNeedSupply(const unsigned int need_supply) {
#ifdef _SCC_DEBUG
	if(need_supply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (STATISTICS::setNeedSupply()): Value need_supply out of range.");return;
	}
#endif
	needSupply = need_supply;
}

inline void STATISTICS::setHaveSupply(const unsigned int have_supply) {
#ifdef _SCC_DEBUG
	if(have_supply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (STATISTICS::setHaveSupply()): Value have_supply out of range.");return;
	}
#endif
	haveSupply = have_supply;
}

inline void STATISTICS::setHaveResource(const unsigned int resource_type, const unsigned int have_resource) {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (STATISTICS::setHaveResource()): Value resource_type out of range.");return;
	}
	if(have_resource >= GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (STATISTICS::setHaveResource()): Value have_resource out of range.");return;
	}
#endif
	resource[resource_type] = have_resource;
}
																				
inline void STATISTICS::setFitness(const unsigned int stat_fitness) {
#ifdef _SCC_DEBUG
	if(stat_fitness > MAX_PFITNESS) {
		toErrorLog("DEBUG (STATISTICS::setFitness()): Value fitness out of range.");return;
	}
#endif
	fitness = stat_fitness;
}

inline void STATISTICS::setTime(const unsigned int this_time) {
#ifdef _SCC_DEBUG
	if(this_time > MAX_TIME) {
		toErrorLog("DEBUG (STATISTICS::setFitness()): Value time out of range.");return;
	}
#endif
	time = this_time;
}

// ------ STATISTICS GET FUNCTIONS ------
inline const unsigned int STATISTICS::getNeedSupply() const {
#ifdef _SCC_DEBUG
	if(needSupply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (STATISTICS::getNeedSupply()): Variable needSupply not initialized.");	return(0);
	}
#endif
	return(needSupply);
}


inline const unsigned int STATISTICS::getHaveSupply() const {
#ifdef _SCC_DEBUG
	if(haveSupply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (STATISTICS::getHaveSupply()): Variable haveSupply not initialized.");return(0);
	}
#endif
	return(haveSupply);
}

inline const unsigned int STATISTICS::getHaveResource(const unsigned int resource_type) const {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (STATISTICS::getHaveResource()): Value resource_type out of range.");return(0);
	}
	if(resource[resource_type] > GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (STATISTICS::getHaveResource()): Variable resource not initialized.");return(0);
	}
#endif
	return(resource[resource_type]);
}

inline const unsigned int STATISTICS::getFitness() const {
#ifdef _SCC_DEBUG
	// TODO max pfitness 
	if(fitness>MAX_PFITNESS) {
		toErrorLog("DEBUG (STATISTICS::getFitness()): Variable fitness not initialized.");return(0);
	}
#endif
	return(fitness);
}

inline const unsigned int STATISTICS::getTime() const {
#ifdef _SCC_DEBUG
	if(time>MAX_TIME) {
		toErrorLog("DEBUG (STATISTICS::getTime()): Variable time not initialized.");return(0);
	}
#endif
	return(time);
}

#endif

