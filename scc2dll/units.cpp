#include "units.h"
#include "debug.h"

UNITS::UNITS()
{
	resetData();
};

UNITS::~UNITS()
{
};

void UNITS::resetData()
{
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		setAvailible(i,0);
		setTotal(i,0);
	}		
};

void UNITS::copy(UNITS* units)
{
#ifdef _SCC_DEBUG
	if(!units)
	{
		debug.toLog(0,"DEBUG: (UNITS::copy): Variable units not initialized [%i].",units);
		return;
	}
#endif
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		setAvailible(i,units->getAvailible(i));
		setTotal(i,units->getTotal(i));
	}
};

void UNITS::adjustSupply(int race, int& supply, int& maxSupply)
{
#ifdef _SCC_DEBUG
	if((race<0)||(race>=MAX_RACES))
	{
		debug.toLog(0,"DEBUG: (UNITS::adjustSupply): Value race [%i] out of range.",race);
		return;
	}
#endif
	for(int j=UNIT_TYPE_COUNT;j--;)
		if(stats[race][j].supply<0)
		{
			if(maxSupply-stats[race][j].supply*total[j]>MAX_SUPPLY)
			{
				if(maxSupply<MAX_SUPPLY)
				{
					supply+=MAX_SUPPLY-maxSupply;
					maxSupply=MAX_SUPPLY;
				}
			}
			else
			{
				supply-=stats[race][j].supply*total[j];
				maxSupply-=stats[race][j].supply*total[j];
			}
		} else
			supply-=stats[race][j].supply*total[j];
};

void UNITS::adjustResearches(int race) // only use this on location ZERO
{
#ifdef _SCC_DEBUG
	if((race<0)||(race>=MAX_RACES))
	{
		debug.toLog(0,"DEBUG: (UNITS::adjustResearches): Value race [%i] out of range.",race);
		return;
	}
#endif
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
			};break;
		case PROTOSS:
			{
				firstResearch=R_PSIONIC_STORM;
				lastResearch=R_ARGUS_TALISMAN;
				basicResearch=PSIONIC_STORM-firstResearch;
				firstUpgrade=R_ARMOR;
				lastUpgrade=R_PLASMA_SHIELDS;
			};break;
		case ZERG:
			{
				firstResearch=R_VENTRAL_SACKS;
				lastResearch=R_LURKER_ASPECT;
				basicResearch=VENTRAL_SACKS-firstResearch;
				firstUpgrade=R_CARAPACE;
				lastUpgrade=R_FLYER_ATTACKS;
			};break;
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
	};
};
//TODO debug
int UNITS::getAvailible(int unit)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (UNITS::getAvailible): Value unit [%i] out of range.",unit);
		return(0);
	}
    if((availible[unit]<0)||(availible[unit]>MAX_SUPPLY))
    {
        debug.toLog(0,"DEBUG: (UNITS::getAvailible): Variable availible[%i] not initialized [%i].", unit, availible[unit]);
        return(0);
    }
#endif
	return(availible[unit]);
}
int UNITS::getTotal(int unit)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (UNITS::getTotal): Value unit [%i] out of range.",unit);
		return(0);
	}
	if((total[unit]<0)||(total[unit]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (UNITS::getTotal): Variable total[%i] not initialized [%i].", unit, total[unit]);
		return(0);
	}
#endif
	return(total[unit]);
};

void UNITS::setAvailible(int unit, int availible)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (UNITS::setAvailible): Value unit [%i] out of range.",unit);
		return;
	}
	if((availible<0)||(availible>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (UNITS::setAvailible): Value availible [%i] out of range.", availible);
		return;
	}
#endif
	this->availible[unit]=availible;
};

void UNITS::setTotal(int unit, int total)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (UNITS::setTotal): Value unit [%i] out of range.", unit);
		return;
	}
	if((total<0)||(total>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (UNITS::setTotal): Value total [%i] out of range.", total);
		return;
	}
#endif
	this->total[unit]=total;
};

void UNITS::addAvailible(int unit, int num)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (UNITS::addAvailible): Value unit [%i] out of range.",unit);
		return;
	}
	if((availible[unit]+num<0)||(availible[unit]+num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (UNITS::addAvailible): Value num [%i] out of range.",num);
		return;
	}

#endif
	availible[unit]+=num;
};

void UNITS::addTotal(int unit, int num)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (UNITS::addTotal): Value unit [%i] out of range.",unit);
		return;
	}
	if((total[unit]+num<0)||(total[unit]+num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (UNITS::addTotal): Value num [%i] out of range.",num);
		return;
	}
#endif
	total[unit]+=num;
};



