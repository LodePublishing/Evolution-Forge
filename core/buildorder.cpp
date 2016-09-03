#include "buildorder.hpp"

BUILDORDER::BUILDORDER(const unsigned int player_number, START* start, UNIT (&units)[MAX_PLAYER][MAX_LOCATIONS]):
	PREBUILDORDER(player_number, start, units),
	mutationRate(20),
	pFitness(0),
	sFitness(0)
{ }

BUILDORDER::~BUILDORDER()
{ }

void BUILDORDER::resetData()
{
	PREBUILDORDER::resetData();
	mutationRate = 20;
	pFitness = 0;
	sFitness = 0;
}

// Reset all ongoing data (between two generations)
void BUILDORDER::prepareForNewGeneration() // resets all data to standard starting values
{
	PREBUILDORDER::prepareForNewGeneration();
	setpFitness(0);
	setsFitness(0);
}

const unsigned int BUILDORDER::calculateSecondaryFitness() const
{
	std::vector<unsigned int> bonus[MAX_LOCATIONS];
	for(unsigned int i = MAX_LOCATIONS; i--;)
	{
		bonus[i].resize(RACE::UNIT_TYPE_COUNT);
		bonus[i].assign(RACE::UNIT_TYPE_COUNT, 0);
	}
	
	//TODO: evtl gas und minerals (wie urspruenglich eigentlich) in Verhaeltnis setyen wieviel es jeweils Geysire/Mineralien gibt...	
	unsigned int penalty = 0;
	getGoal()->calculateBonus(bonus);

	for(unsigned int j = MAX_LOCATIONS; j--;)
		for(unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
			if(bonus[j][i]>0)
			{
				unsigned int total = bonus[j][i];
				if((*(getStartCondition()))->getLocationTotal(j, i) > total)
					total = (*(getStartCondition()))->getLocationTotal(j, i);
				if(getLocationTotal(j, i) > total)
				{
					unsigned int res = 0;
					for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
						res += GAME::race[getRace()].stats[i].resource[k];
					penalty += (getLocationTotal(GLOBAL,i) - total) * res;
				}
			}
	unsigned int res = 0;
	for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
		res += getHarvestedResource(k);
#ifdef _SCC_DEBUG
	if(res < penalty)
	{
		toErrorLog("WARNING (BUILDORDER::calculateSecondaryFitness()): Penalty out of range.");
		return(0);
	}
#endif
	return(res - penalty);
}

void BUILDORDER::postProcessing()
{
	setpFitness(calculatePrimaryFitness(ready));
	setsFitness(calculateSecondaryFitness());
	while(!buildingQueue.empty()) 
		buildingQueue.pop();
// ----- BUILDORDER SPECIFIC -----
// ----- END BUILDORDER SPECIFIC ------
	PREBUILDORDER::postProcessing();
}

const bool BUILDORDER::calculateStep()
{
	if(calculationDone())
		return(true);
	bool ok = true;
	while((ok)&&(!codeFinished()))
		ok = executeNextInstruction();
	ready = calculateReady();
	
	leapForwardInTime();
	processBuildingQueue();
	
	return(false);
}






