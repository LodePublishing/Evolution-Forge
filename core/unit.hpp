#ifndef _CORE_UNIT_HPP
#define _CORE_UNIT_HPP

#include "game.hpp"
#include "../stl/misc.hpp"

/*1. researches + erforschungen werden nur fuer location 0 gezaehlt, wie bisher
2. globales array, also locations x unit x spieler
3. 'map' fuer jede location, fuer kaempfe, nur gebaeude/unit

eine startforce (auch player 0) zusammenstellen, vorgegebene startforces jeweils relativ zum Startpunkt*/

class UNIT
{
	public:
		UNIT();
		~UNIT();

		UNIT(const UNIT& object);
		UNIT& operator=(const UNIT& object);
	
		//TODO evtl noch nen paar get/set Funktionen rein... 
		const unsigned int getAvailible(const unsigned int unit_type) const;
		const unsigned int getTotal(const unsigned int unit_type) const;
		void setAvailible(const unsigned int unit_type, const unsigned int availible);
		void setTotal(const unsigned int unit_type, const unsigned int total);

		void modifyAvailible(const unsigned int unit_type, const signed int availible);
		void modifyTotal(const unsigned int unit_type, const signed int total);

		void addOneAvailible(const unsigned int unit_type);
		void addOneTotal(const unsigned int unit_type);
		void removeOneAvailible(const unsigned int unit_type);
		void removeOneTotal(const unsigned int unit_type);
		
		void adjustResearches(const unsigned int race);
		void adjustSupply(const unsigned int race, unsigned int& needSupply, unsigned int& haveSupply);
		void resetData();
	private:
		std::vector<unsigned int> availible;
		std::vector<unsigned int> total;
};

inline const unsigned int UNIT::getAvailible(const unsigned int unitType) const
{
#ifdef _SCC_DEBUG
	if(unitType >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::getAvailible): Value unitType out of range.");return(0);
	}
	if(availible[unitType] > GAME::MAX_TOTAL_UNITS) {
		toErrorLog("DEBUG: (UNIT::getAvailible): Variable availible not initialized.");return(0);
	}
#endif
	return(availible[unitType]);
}

inline const unsigned int UNIT::getTotal(const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::getTotal): Value unit_type out of range.");return(0);
	}
	if(total[unit_type] > GAME::MAX_TOTAL_UNITS) {
		toErrorLog("DEBUG: (UNIT::getTotal): Variable total not initialized.");return(0);
	}
#endif
	return(total[unit_type]);
}

inline void UNIT::setAvailible(const unsigned int unit_type, const unsigned int unit_availible)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::setAvailible): Value unit_type out of range.");return;
	}
	if(unit_availible > GAME::MAX_TOTAL_UNITS) {
		toErrorLog("DEBUG: (UNIT::setAvailible): Value unit_availible out of range.");return;
	}
#endif
	availible[unit_type] = unit_availible;
}

inline void UNIT::setTotal(const unsigned int unit_type, const unsigned int unit_total)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::setTotal): Value unit_type out of range.");return;
	}
	if(unit_total > GAME::MAX_TOTAL_UNITS) {
		toErrorLog("DEBUG: (UNIT::setTotal): Value unit_total out of range.");return;
	}
#endif
	total[unit_type] = unit_total;
}

inline void UNIT::modifyAvailible(const unsigned int unit_type, const signed int unit_availible)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::modifyAvailible): Value unit_type out of range.");return;
	}
	if((availible[unit_type]+unit_availible > GAME::MAX_TOTAL_UNITS)) {
		toErrorLog("DEBUG: (UNIT::modifyAvailible): Value unit_availible out of range.");return;
	}

#endif
	availible[unit_type] += unit_availible;
}

inline void UNIT::modifyTotal(const unsigned int unit_type, const signed int unit_total)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::modifyTotal): Value unit_type out of range.");return;
	}
	if((total[unit_type]+unit_total > GAME::MAX_TOTAL_UNITS)) {
		toErrorLog("DEBUG: (UNIT::modifyTotal): Value unit_total out of range.");return;
	}
#endif
	total[unit_type] += unit_total;
}

inline void UNIT::addOneAvailible(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::addOneAvailible): Value unit_type out of range.");return;
	}
	if(availible[unit_type]+1 > GAME::MAX_TOTAL_UNITS) {
		toErrorLog("DEBUG: (UNIT::addOneAvailible): Value num out of range.");return;
	}
#endif
	++availible[unit_type];
}

inline void UNIT::addOneTotal(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::addOneTotal): Value unit_type out of range.");return;
	}
	if(total[unit_type] + 1 > GAME::MAX_TOTAL_UNITS) {
		toErrorLog("DEBUG: (UNIT::addOneTotal): Value num out of range.");return;
	}
#endif
	++total[unit_type];
}

inline void UNIT::removeOneAvailible(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::removeOneAvailible): Value unit_type out of range.");return;
	}
	if((availible[unit_type] == 0) || (availible[unit_type] > GAME::MAX_TOTAL_UNITS)) {
		toErrorLog("DEBUG: (UNIT::removeOneAvailible): Value num out of range.");return;
	}
#endif
	--availible[unit_type];
}

inline void UNIT::removeOneTotal(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (UNIT::removeOneTotal): Value unit_type out of range.");return;
	}
	if((total[unit_type] == 0) || (total[unit_type] > GAME::MAX_TOTAL_UNITS)) {
		toErrorLog("DEBUG: (UNIT::removeOneTotal): Value num out of range.");return;
	}
#endif
	--total[unit_type];
}


#endif // _CORE_UNIT_HPP

