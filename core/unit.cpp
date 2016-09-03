#include "unit.hpp"

UNIT::UNIT()
{
	resetData();
}

UNIT::~UNIT()
{ }

UNIT::UNIT(const UNIT& object)
{
	memcpy(availible, object.availible, UNIT_TYPE_COUNT*sizeof(int));
	memcpy(total, object.total, UNIT_TYPE_COUNT*sizeof(int));
}

UNIT& UNIT::operator=(const UNIT& object)
{
	memcpy(availible, object.availible, UNIT_TYPE_COUNT*sizeof(int));
	memcpy(total, object.total, UNIT_TYPE_COUNT*sizeof(int));
	return(*this);
}

void UNIT::resetData()
{
	memset(availible, 0, UNIT_TYPE_COUNT * sizeof(int));
	memset(total, 0, UNIT_TYPE_COUNT * sizeof(int));
}

void UNIT::adjustSupply(const eRace race, unsigned int& needSupply, unsigned int& haveSupply)
{
	// TODO wenn mehr Einheiten als Versorgung da is, wirds negativ... :/
	for(unsigned int j=UNIT_TYPE_COUNT;j--;)
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
		toErrorLog("DEBUG ((UNIT::adjustSupply()): Too many units :/");
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

	for(unsigned int j=firstResearch; j<=lastResearch; ++j)
	{
		setTotal(j, 1-total[j+basicResearch]);
		setAvailible(j, 1-total[j+basicResearch]);
	}
	for(unsigned int j=firstUpgrade; j<=lastUpgrade; ++j)
	{
		setTotal(j, 3-total[j+basicResearch]);
		setAvailible(j, 3-total[j+basicResearch]); // >0 !? wtf? TODO
		//temporary researches and upgrades
	}
}

