#include "unit.hpp"

UNIT::UNIT()
{
	resetData();
};

UNIT::~UNIT()
{
};

void UNIT::resetData()
{
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		setAvailible(i,0);
		setTotal(i,0);
	}		
};

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
};

void UNIT::adjustSupply(const eRace race, int& needSupply, int& haveSupply)
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
		};
#ifdef _SCC_DEBUG
	if(needSupply>haveSupply)
		toLog("DEBUG: (UNIT::adjustSupply): Too many units :/");
#endif
};

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

const int UNIT::getAvailible(const int unit) const
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (UNIT::getAvailible): Value unit out of range.");return(0);
	}
	if((availible[unit]<0)||(availible[unit]>MAX_SUPPLY)) {
		toLog("DEBUG: (UNIT::getAvailible): Variable availible not initialized.");return(0);
	}
#endif
	return(availible[unit]);
}

const int UNIT::getTotal(const int unit) const
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (UNIT::getTotal): Value unit out of range.");return(0);
	}
	if((total[unit]<0)||(total[unit]>MAX_SUPPLY)) {
		toLog("DEBUG: (UNIT::getTotal): Variable total not initialized.");return(0);
	}
#endif
	return(total[unit]);
};

void UNIT::setAvailible(const int unit, const int availible)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (UNIT::setAvailible): Value unit out of range.");return;
	}
	if((availible<0)||(availible>MAX_SUPPLY)) {
		toLog("DEBUG: (UNIT::setAvailible): Value availible out of range.");return;
	}
#endif
	this->availible[unit]=availible;
};

void UNIT::setTotal(const int unit, const int total)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (UNIT::setTotal): Value unit out of range.");return;
	}
	if((total<0)||(total>MAX_SUPPLY)) {
		toLog("DEBUG: (UNIT::setTotal): Value total out of range.");return;
	}
#endif
	this->total[unit]=total;
};

void UNIT::addAvailible(const int unit, const int availible)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (UNIT::addAvailible): Value unit out of range.");return;
	}
	if((this->availible[unit]+availible<0)||(this->availible[unit]+availible>MAX_SUPPLY)) {
		toLog("DEBUG: (UNIT::addAvailible): Value num out of range.");return;
	}

#endif
	this->availible[unit]+=availible;
};

void UNIT::addTotal(const int unit, const int total)
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (UNIT::addTotal): Value unit out of range.");return;
	}
	if((this->total[unit]+total<0)||(this->total[unit]+total>MAX_SUPPLY)) {
		toLog("DEBUG: (UNIT::addTotal): Value num out of range.");return;
	}
#endif
	this->total[unit]+=total;
};



