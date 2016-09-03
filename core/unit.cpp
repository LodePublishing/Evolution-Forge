#include "unit.hpp"

UNIT::UNIT()
{
	resetData();
}

UNIT::~UNIT()
{ }

UNIT::UNIT(const UNIT& object)
{
    for(int i=UNIT_TYPE_COUNT;i--;)
	{
		setAvailible(i, object.getAvailible(i));
		setTotal(i, object.getTotal(i));
	}
}

UNIT& UNIT::operator=(const UNIT& object)
{
    for(int i=UNIT_TYPE_COUNT;i--;)
    {
		setAvailible(i, object.getAvailible(i));
		setTotal(i, object.getTotal(i));
	}
	return(*this);
}

void UNIT::resetData()
{
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		setAvailible(i, 0);
		setTotal(i, 0);
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
	unsigned int firstResearch=0;
	unsigned int basicResearch=0;
	unsigned int lastResearch=0;
	unsigned int firstUpgrade=0;
	unsigned int lastUpgrade=0;
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
		default:break;
	}

	for(unsigned int j=firstResearch; j<=lastResearch; j++)
	{
		setTotal(j, 1-total[j+basicResearch]);
		setAvailible(j, 1-total[j+basicResearch]);
	}
	for(unsigned int j=firstUpgrade; j<=lastUpgrade; j++)
	{
		setTotal(j, 3-total[j+basicResearch]);
		setAvailible(j, 3-total[j+basicResearch]); // >0 !? wtf? TODO
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

const unsigned int UNIT::getTotal(const unsigned int unit_type) const
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

void UNIT::setAvailible(const unsigned int unit_type, const unsigned int unit_availible)
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

void UNIT::setTotal(const unsigned int unit_type, const unsigned int unit_total)
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

void UNIT::addAvailible(const unsigned int unit_type, const unsigned int unit_availible)
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

void UNIT::addTotal(const unsigned int unit_type, const unsigned int unit_total)
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

void UNIT::addOneAvailible(const unsigned int unit_type)
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
                                                                                                                                                            
void UNIT::addOneTotal(const unsigned int unit_type)
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

void UNIT::removeOneAvailible(const unsigned int unit_type)
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
                                                                                                                                                            
void UNIT::removeOneTotal(const unsigned int unit_type)
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

