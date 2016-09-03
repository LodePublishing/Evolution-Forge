#ifndef _CORE_UNIT_HPP
#define _CORE_UNIT_HPP

#include "defs.hpp"
#include "../stl/misc.hpp"

/*1. researches + erforschungen werden nur fuer location 0 gezaehlt, wie bisher
2. globales array, also locations x unit x spieler
3. 'map' fuer jede location, fuer kaempfe, nur gebaeude/unit

eine startforce (auch player 0) zusammenstellen, vorgegebene startforces jeweils relativ zum Startpunkt*/

class UNIT
{
	private:
		unsigned int availible[UNIT_TYPE_COUNT];
		unsigned int total[UNIT_TYPE_COUNT];
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

		void addAvailible(const unsigned int unit_type, const unsigned int availible);
		void addTotal(const unsigned int unit_type, const unsigned int total);

		void addOneAvailible(const unsigned int unit_type);
		void addOneTotal(const unsigned int unit_type);
		void removeOneAvailible(const unsigned int unit_type);
		void removeOneTotal(const unsigned int unit_type);
		
		void adjustResearches(const eRace race);
		void adjustSupply(const eRace race, unsigned int& needSupply, unsigned int& haveSupply);
		void resetData();
};

inline const unsigned int UNIT::getAvailible(const unsigned int unitType) const
{
#ifdef _SCC_DEBUG
	if(unitType>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::getAvailible): Value unitType out of range.");return(0);
	}
	if(availible[unitType]>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::getAvailible): Variable availible not initialized.");return(0);
	}
#endif
	return(availible[unitType]);
}

inline const unsigned int UNIT::getTotal(const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::getTotal): Value unit_type out of range.");return(0);
	}
	if(total[unit_type]>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::getTotal): Variable total not initialized.");return(0);
	}
#endif
	return(total[unit_type]);
}

inline void UNIT::setAvailible(const unsigned int unit_type, const unsigned int unit_availible)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::setAvailible): Value unit_type out of range.");return;
	}
	if(unit_availible > MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::setAvailible): Value unit_availible out of range.");return;
	}
#endif
	availible[unit_type] = unit_availible;
}

inline void UNIT::setTotal(const unsigned int unit_type, const unsigned int unit_total)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::setTotal): Value unit_type out of range.");return;
	}
	if(unit_total > MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::setTotal): Value unit_total out of range.");return;
	}
#endif
	total[unit_type] = unit_total;
}

inline void UNIT::addAvailible(const unsigned int unit_type, const unsigned int unit_availible)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::addAvailible): Value unit_type out of range.");return;
	}
	if(availible[unit_type]+unit_availible>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::addAvailible): Value unit_availible out of range.");return;
	}

#endif
	availible[unit_type] += unit_availible;
}

inline void UNIT::addTotal(const unsigned int unit_type, const unsigned int unit_total)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::addTotal): Value unit_type out of range.");return;
	}
	if(total[unit_type]+unit_total > MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::addTotal): Value unit_total out of range.");return;
	}
#endif
	total[unit_type] += unit_total;
}

inline void UNIT::addOneAvailible(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::addOneAvailible): Value unit_type out of range.");return;
	}
	if(availible[unit_type]+1>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::addOneAvailible): Value num out of range.");return;
	}
#endif
	availible[unit_type]++;
}

inline void UNIT::addOneTotal(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::addOneTotal): Value unit_type out of range.");return;
	}
	if(total[unit_type]+1>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::addOneTotal): Value num out of range.");return;
	}
#endif
	total[unit_type]++;
}

inline void UNIT::removeOneAvailible(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::removeOneAvailible): Value unit_type out of range.");return;
	}
	if((availible[unit_type]<1)||(availible[unit_type]>MAX_TOTAL_UNITS)) {
		toLog("DEBUG: (UNIT::removeOneAvailible): Value num out of range.");return;
	}
#endif
   availible[unit_type]--;
}

inline void UNIT::removeOneTotal(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::removeOneTotal): Value unit_type out of range.");return;
	}
	if((total[unit_type] < 1)||(total[unit_type] > MAX_TOTAL_UNITS)) {
		toLog("DEBUG: (UNIT::removeOneTotal): Value num out of range.");return;
	}
#endif
	total[unit_type]--;
}


#endif // _CORE_UNIT_HPP

