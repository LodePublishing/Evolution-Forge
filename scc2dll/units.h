#ifndef __UNITS_H
#define __UNITS_H

#include "main.h"

class EXPORT UNITS
{
	private:
		int availible[UNIT_TYPE_COUNT];
        int total[UNIT_TYPE_COUNT];
    public:
		//TODO evtl noch nen paar get/set Funktionen rein... 
		void copy(UNITS* units);
		int getAvailible(int unit);
		int getTotal(int unit);
		void setAvailible(int unit, int num);
		void setTotal(int unit, int num);
		void addAvailible(int unit, int num);
		void addTotal(int unit, int num);
        void adjustResearches(int race);
        void adjustSupply(int race, int& supply, int& maxSupply);
		void resetData();
        UNITS();
        ~UNITS();
};

#endif

