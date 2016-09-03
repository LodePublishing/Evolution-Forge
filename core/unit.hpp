#ifndef _CORE_UNIT_HPP
#define _CORE_UNIT_HPP

#include "defs.hpp"

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
		//TODO evtl noch nen paar get/set Funktionen rein... 
		void copy(const UNIT* unit);
		const unsigned int getAvailible(const unsigned int unittype) const;
		const unsigned int getTotal(const unsigned int unittype) const;
		void setAvailible(const unsigned int unittype, const unsigned int availible);
		void setTotal(const unsigned int unittype, const unsigned int total);

		void addAvailible(const unsigned int unittype, const unsigned int availible);
		void addTotal(const unsigned int unittype, const unsigned int total);

		void addOneAvailible(const unsigned int unittype);
		void addOneTotal(const unsigned int unittype);
		void removeOneAvailible(const unsigned int unittype);
		void removeOneTotal(const unsigned int unittype);
		
		void adjustResearches(const eRace race);
		void adjustSupply(const eRace race, unsigned int& needSupply, unsigned int& haveSupply);
		void resetData();
		UNIT();
		~UNIT();
};

#endif // _CORE_UNIT_HPP

