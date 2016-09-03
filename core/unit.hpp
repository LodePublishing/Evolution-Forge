#ifndef _CORE_UNIT_HPP
#define _CORE_UNIT_HPP

#include "main.hpp"

/*1. researches + erforschungen werden nur fuer location 0 gezaehlt, wie bisher
2. globales array, also locations x unit x spieler
3. 'map' fuer jede location, fuer kaempfe, nur gebaeude/unit

eine startforce (auch player 0) zusammenstellen, vorgegebene startforces jeweils relativ zum Startpunkt*/

class EXPORT UNIT
{
	private:
		int availible[UNIT_TYPE_COUNT];
		int total[UNIT_TYPE_COUNT];
	public:
		//TODO evtl noch nen paar get/set Funktionen rein... 
		void copy(const UNIT* unit);
		const int getAvailible(const int unit) const;
		const int getTotal(const int unit) const;
		void setAvailible(const int unit, const int availible);
		void setTotal(const int unit, const int total);
		void addAvailible(const int unit, const int availible);
		void addTotal(const int unit, const int total);
		void adjustResearches(const eRace race);
		void adjustSupply(const eRace race, int& needSupply, int& haveSupply);
		void resetData();
		UNIT();
		~UNIT();
};

#endif // _CORE_UNIT_HPP

