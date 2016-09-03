#include "unit.hpp"

UNIT::UNIT()
{
	resetData();
}

UNIT::~UNIT()
{
}

void UNIT::resetData()
{
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		setAvailible(i,0);
		setTotal(i,0);
	}		
}

void UNIT::copy(const UNIT* unit)
{
#ifdef _SCC_DEBUG
	if(!unit) {
		toLog("DEBUG: (UNIT::copy): Variable unit not initialized.");return;
	}
#endif
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		setAvailible(i,unit->getAvailible(i));
		setTotal(i,unit->getTotal(i));
	}
}

void UNIT::adjustSupply(const eRace race, unsigned int& needSupply, unsigned int& haveSupply)
{
	// TODO wenn mehr Einheiten als Versorgung da is, wirds negativ... :/
	for(int j=UNIT_TYPE_COUNT;j--;)
		if(total[j])
		{
			needSupply+=stats[race][j].needSupply*total[j];
			haveSupply+=stats[race][j].haveSupply*total[j];
/*				if(needSupply+haveSupply+stats[race][j].haveSupply*total[j]>MAX_SUPPLY)
				{
					if(haveSupply<MAX_SUPPLY)
					{
						needSupply+=MAX_SUPPLY-haveSupply;
						haveSupply=MAX_SUPPLY;
					}
				}
				{
					needSupply-=stats[race][j].needSupply*total[j];
					haveSupply-=stats[race][j].needSupply*total[j];
				}
			} else
				needSupply-=stats[race][j].needSupply*total[j];*/
		}
#ifdef _SCC_DEBUG
	if(needSupply>haveSupply)
		toLog("DEBUG: (UNIT::adjustSupply): Too many units :/");
#endif
}

void UNIT::adjustResearches(const eRace race) // only use this on location ZERO
{
	int firstResearch=0;
	int basicResearch=0;
	int lastResearch=0;
	int firstUpgrade=0;
	int lastUpgrade=0;
	switch(race)
	{
		case TERRA:
			{
				firstResearch=R_STIM_PACKS;
				lastResearch=R_CHARON_BOOSTER;
				basicResearch=STIM_PACKS-firstResearch;
				firstUpgrade=R_INFANTRY_ARMOR;
				lastUpgrade=R_SHIP_WEAPONS;
			}break;
		case PROTOSS:
			{
				firstResearch=R_PSIONIC_STORM;
				lastResearch=R_ARGUS_TALISMAN;
				basicResearch=PSIONIC_STORM-firstResearch;
				firstUpgrade=R_ARMOR;
				lastUpgrade=R_PLASMA_SHIELDS;
			}break;
		case ZERG:
			{
				firstResearch=R_VENTRAL_SACKS;
				lastResearch=R_LURKER_ASPECT;
				basicResearch=VENTRAL_SACKS-firstResearch;
				firstUpgrade=R_CARAPACE;
				lastUpgrade=R_FLYER_ATTACKS;
			}break;
	}

	for(int j=firstResearch;j<=lastResearch;j++)
	{
		total[j]=1-total[j+basicResearch];
		availible[j]=1-total[j+basicResearch];
	}
	for(int j=firstUpgrade;j<=lastUpgrade;j++)
	{
		total[j]=3-total[j+basicResearch];
		availible[j]=(3-total[j+basicResearch]>0);
		//temporary researches and upgrades
	}
}
//TODO debug

const unsigned int UNIT::getAvailible(const unsigned int unitType) const
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

const unsigned int UNIT::getTotal(const unsigned int unittype) const
{
#ifdef _SCC_DEBUG
	if(unittype>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::getTotal): Value unittype out of range.");return(0);
	}
	if(total[unittype]>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::getTotal): Variable total not initialized.");return(0);
	}
#endif
	return(total[unittype]);
}

void UNIT::setAvailible(const unsigned int unittype, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(unittype>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::setAvailible): Value unittype out of range.");return;
	}
	if(availible>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::setAvailible): Value availible out of range.");return;
	}
#endif
	this->availible[unittype]=availible;
}

void UNIT::setTotal(const unsigned int unittype, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(unittype>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::setTotal): Value unittype out of range.");return;
	}
	if(total>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::setTotal): Value total out of range.");return;
	}
#endif
	this->total[unittype]=total;
}

void UNIT::addAvailible(const unsigned int unittype, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(unittype>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::addAvailible): Value unittype out of range.");return;
	}
	if(this->availible[unittype]+availible>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::addAvailible): Value num out of range.");return;
	}

#endif
	this->availible[unittype]+=availible;
}

void UNIT::addTotal(const unsigned int unittype, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(unittype>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (UNIT::addTotal): Value unittype out of range.");return;
	}
	if(this->total[unittype]+total>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (UNIT::addTotal): Value num out of range.");return;
	}
#endif
	this->total[unittype]+=total;
}

void UNIT::addOneAvailible(const unsigned int unittype)
{
#ifdef _SCC_DEBUG
    if(unittype>=UNIT_TYPE_COUNT) {
        toLog("DEBUG: (UNIT::addOneAvailible): Value unittype out of range.");return;
    }
    if(availible[unittype]+1>MAX_TOTAL_UNITS) {
        toLog("DEBUG: (UNIT::addOneAvailible): Value num out of range.");return;
    }
#endif
    this->availible[unittype]++;
}
                                                                                                                                                            
void UNIT::addOneTotal(const unsigned int unittype)
{
#ifdef _SCC_DEBUG
    if(unittype>=UNIT_TYPE_COUNT) {
        toLog("DEBUG: (UNIT::addOneTotal): Value unittype out of range.");return;
    }
    if(total[unittype]+1>MAX_TOTAL_UNITS) {
        toLog("DEBUG: (UNIT::addOneTotal): Value num out of range.");return;
    }
#endif
    this->total[unittype]++;
}

void UNIT::removeOneAvailible(const unsigned int unittype)
{
#ifdef _SCC_DEBUG
    if(unittype>=UNIT_TYPE_COUNT) {
        toLog("DEBUG: (UNIT::removeOneAvailible): Value unittype out of range.");return;
    }
    if((availible[unittype]<1)||(availible[unittype]>MAX_TOTAL_UNITS)) {
        toLog("DEBUG: (UNIT::removeOneAvailible): Value num out of range.");return;
    }
#endif
    this->availible[unittype]--;
}
                                                                                                                                                            
void UNIT::removeOneTotal(const unsigned int unittype)
{
#ifdef _SCC_DEBUG
    if(unittype>=UNIT_TYPE_COUNT) {
        toLog("DEBUG: (UNIT::removeOneTotal): Value unittype out of range.");return;
    }
    if((total[unittype]<1)||(total[unittype]>MAX_TOTAL_UNITS)) {
        toLog("DEBUG: (UNIT::removeOneTotal): Value num out of range.");return;
    }
#endif
    this->total[unittype]--;
}

