#include "anabuildorder.hpp"

ANABUILDORDER::ANABUILDORDER(const unsigned int player_number, START* start, UNIT (&units)[MAX_PLAYER][MAX_LOCATIONS]):
	PREBUILDORDER(player_number, start, units),
	unitsTotal(4),
	unitsTotalMax(4),
	nonGoalsUnitsTotalMax(4),
	optimizing(false),
	maxpFitness(0),
	maxsFitness(0),
	timePercentage(0),
	goalPercentage(0),
	currentpFitness(0),
	timeStatisticsList()
{}

ANABUILDORDER::~ANABUILDORDER()
{ }

void ANABUILDORDER::resetData()
{
	PREBUILDORDER::resetData();
	unitsTotal = 4;
	unitsTotalMax = 4;
	nonGoalsUnitsTotalMax = 4;
	optimizing = false;
	maxpFitness = 0;
	maxsFitness = 0;
	timePercentage = 0;
	goalPercentage = 0;
	currentpFitness = 0;
	timeStatisticsList.clear();
	programList.clear();	
}

void ANABUILDORDER::restartData()
{
	resetSpecial();
	setMaxpFitness(0);
	setMaxsFitness(0);
	unitsTotal = 1; // ~4 ?
	unitsTotalMax = 1;
	nonGoalsUnitsTotalMax = 1;
	timePercentage = 0;
	goalPercentage = 0;
	setTimer(0); //?
}

// Reset all ongoing data (between two runs)
void ANABUILDORDER::prepareForNewGeneration() // resets all data to standard starting values
{
	PREBUILDORDER::prepareForNewGeneration();
	setCurrentpFitness(0);

	timeStatisticsList.clear();
	programList.clear();
}

// ----------------------------------------
// ------ CORE OF THE CORE FUNCTIONS ------
// ----------------------------------------

void ANABUILDORDER::postProcessing()
{
	countUnitsTotal();
	setCurrentpFitness(calculatePrimaryFitness(ready));
	
	unsigned int max_fitness = getGoal()->calculateMaxFitness();
	if(max_fitness > 0)
		goalPercentage = 100 * currentpFitness / max_fitness;
	else goalPercentage = 100;
	
//	toErrorLog("------------");
//	toErrorLog(goalPercentage);
//	toErrorLog(currentpFitness);
//	toErrorLog(max_fitness);
//	toErrorLog("------------");
	
	while(!buildingQueue.empty())
		buildingQueue.pop();

	if(ready)
	{
		std::list<PROGRAM>::iterator i = programList.begin(); 
		while(i!=programList.end())
		{
			if((getTimer() < i->getBT()) || (i->getTime() < getTimer() - i->getBT()))
				i = programList.erase(i);
			else ++i;
		}
	}
	PREBUILDORDER::postProcessing();
}

const STATISTICS ANABUILDORDER::generateStatistics() const
{
	STATISTICS my_stat;
	my_stat.setTime(getTimer());
	my_stat.setNeedSupply(getNeedSupply());
	my_stat.setHaveSupply(getHaveSupply());

	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
		my_stat.setHaveResource(i, getResource(i));
	return(my_stat);
}

const bool ANABUILDORDER::calculateStep()
{
	if(calculationDone())
		return(true);
	bool ok = true;
	while((ok)&&(!codeFinished()))
	{
		resetNeededResources();
		PROGRAM program;
		program.buildingQueue = buildingQueue;
		program.before = generateStatistics();
		program.successUnit = successUnit;
		program.successType = successType;

		if(!(ok = executeAlwaysBuildInstructions()))
			ok = executeNextInstruction();
		
		// TODO timeout irgendwie anzeigen...
		
		if(ok) // && ((getRace()!=ZERG) || (anaraceBuildUnit != LARVA))) TODO... evtl nur von der Oberflaeche entscheiden was denn angezeigt werden soll
		{
		// HACK
			program.setFacility(anaraceBuildFacility);
			program.setLocation(anaraceBuildLocation);
			program.setBT(anaraceBuildTotalTime);
			program.setUsedFacilityCount(getLocationTotal(GLOBAL, anaraceBuildFacility) - getLocationAvailible(GLOBAL, anaraceBuildFacility));
		// END HACK
			program.setUnit(anaraceBuildUnit);
			program.after = generateStatistics();
			
			programList.push_back(program);
		}
	}
	ready = calculateReady();
	timeStatisticsList.push_back(generateStatistics());
	leapForwardInTime();
	processBuildingQueue();
	timeStatisticsList.push_back(generateStatistics());
	return(false);
}

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)

const bool ANABUILDORDER::writeProgramBackToCode(const std::list<PROGRAM>& program_list)
{
	std::list<unsigned int> program_code;
	for(std::list<PROGRAM>::const_iterator i = program_list.begin(); i != program_list.end(); ++i)
		program_code.push_back(getGoal()->toGeno(i->getUnit()));
	bool changed_bo = isDifferent(program_code);
	if(changed_bo)
		assignCode(program_code);
	return(changed_bo);
}


// -------------------------------------------
// ------ END CORE OF THE CORE FUNCTIONS -----
// -------------------------------------------


// -------------------------------
// ------ CONTROL FUNCTIONS ------
// -------------------------------

void ANABUILDORDER::copyProgramList(const std::list<PROGRAM>& program_list)
{
	programList.clear();
	for(std::list<PROGRAM>::const_iterator i = program_list.begin();i!=program_list.end();++i)
	{
		PROGRAM p;
		p.setUnit(i->getUnit());
		programList.push_back(p);
	}
}


// -----------------------------------
// ------ END CONTROL FUNCTIONS ------
// -----------------------------------

// ----------------------------
// ------ MISC FUNCTIONS ------
// ----------------------------

void ANABUILDORDER::countUnitsTotal()
{
	unitsTotal = 0;
	nonGoalsUnitsTotalMax = 1;
	for (unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
	{
		unitsTotal += getLocationTotal(GLOBAL, i);
		if ((!getGoal()->getIsGoal(i)) && (getLocationTotal(GLOBAL, i) > nonGoalsUnitsTotalMax))
			nonGoalsUnitsTotalMax = getLocationTotal(GLOBAL, i);
	}
}

// -----------------------------------
// ------ END OF MISC FUNCTIONS ------
// -----------------------------------

const unsigned int ANABUILDORDER::getGoalPercentage() const
{
	if(getTimer()==0)
		return(goalPercentage);
	else 
	{
		unsigned int optimalTime = getGoal()->calculateFastestBO((*(getStartCondition()))->getUnit(GLOBAL));
		if(getRealTimer()==0)
			return(100);
		else
			return(100*optimalTime/getRealTimer());
	}
}

