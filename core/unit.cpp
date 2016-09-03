#include "unit.hpp"

UNIT::UNIT()
{
	resetData();
}

UNIT::~UNIT()
{ }

UNIT::UNIT(const UNIT& object)
{
	availible = object.availible;
	total = object.total;
}

UNIT& UNIT::operator=(const UNIT& object)
{
	availible = object.availible;
	total = object.total;
	return(*this);
}

void UNIT::resetData()
{
	availible.assign(RACE::UNIT_TYPE_COUNT, 0);
	total.assign(RACE::UNIT_TYPE_COUNT, 0);
}

void UNIT::adjustSupply(const unsigned int race, unsigned int& needSupply, unsigned int& haveSupply)
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (UNIT::adjustSupply()): Value race out of range.");return;
	}	
	if(needSupply >= 2*GAME::MAX_SUPPLY) { // TODO
		toErrorLog("DEBUG (UNIT::adjustSupply()): Value needSupply out of range.");return;
	}
	if(haveSupply >= 2*GAME::MAX_SUPPLY) { // TODO
		toErrorLog("DEBUG (UNIT::adjustSupply()): Value haveSupply out of range.");return;
	}
#endif
	// TODO wenn mehr Einheiten als Versorgung da is, wirds negativ... :/
	for(unsigned int j=RACE::UNIT_TYPE_COUNT;j--;)
		if(total[j])
		{
			needSupply += GAME::race[race].stats[j].needSupply * total[j];
			haveSupply += GAME::race[race].stats[j].haveSupply * total[j];
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

void UNIT::adjustResearches(const unsigned int race) 
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (UNIT::adjustResearches()): Value race out of range.");return;
	}	
#endif
	
// Setze verfuegbare Research/Upgrademoeglichkeiten auf 1 bzw. 3 minus bereits vorhandene (z.B. ueber Starteinheiten) Researches/Upgrades
	for(unsigned int j = GAME::race[race].research.first; j <= GAME::race[race].research.last; ++j)
	{
		unsigned int count = total[j - GAME::race[race].research.first + GAME::race[race].research.basic];
#ifdef _SCC_DEBUG
		if(count > 1)
		{
			toErrorLog("DEBUG (UNIT::adjustResearches()): More researches than availible were set.");
			count = 1;
		}
#endif
		setTotal(j, 1 - count);
		setAvailible(j, 1 - count);
	}
}

