#include "anarace.hpp"

/* ANARACE.CPP - last changed: 6/23/04                              *
 * Author: Clemens Lode                                             *
 * Copyright: Clemens Lode                                          *
 *                                                                  *
 *  PURPPOSE OF THIS FILE:                                          *
 *      This is the core of the core, where the main calculations   *
 *      are made. Also this class is used as an interface to the    *
 *      core from outside the library. In addition some data is     *
        preformatted/precalculated -> statistics.				    */


STATISTICS::STATISTICS():
	needSupply(0),
	haveSupply(0),
	minerals(0),
	gas(0),
	fitness(0)
{ }

STATISTICS::~STATISTICS() {};

void STATISTICS::resetData()
{
	needSupply=0;
	haveSupply=0;
	minerals=0;
	gas=0;
	fitness=0;
}

PROGRAM::PROGRAM():
	built(false),
	time(0),
	location(0),
	successType(0),
	successUnit(0),
	facility(0),
	BT(0)
{
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		forceCount[i]=0;
		availibleCount[i]=0;
	}
}

PROGRAM::~PROGRAM() {};

void PROGRAM::resetData()
{
	built=false;
	time=0;
	location=0;
	successType=0;
	successUnit=0;
	facility=0;
	BT=0;
        for(int i=UNIT_TYPE_COUNT;i--;)
        {
                forceCount[i]=0;
                availibleCount[i]=0;
        }
}

void ANARACE::resetStaticData()
{
	PRERACE::resetStaticPrerace();
	successType=0;
	successUnit=0;
}

// TODO: reimplement/recheck the speed of the units
ANARACE::ANARACE():
	PRERACE(),
	unitsTotal(4),
	unitsTotalMax(4),
	nonGoalsUnitsTotalMax(4),
	unchangedGenerations(0),
	currentRun(0),
	optimizing(false),
	active(true),
	totalGeneration(0),
	maxpFitness(0),
	maxsFitness(0),
	maxtFitness(MAX_TFITNESS),
	timePercentage(0),
	goalPercentage(0),
	currentpFitness(0),
	averageLength(0),
	fitnessAverage(0),
	fitnessVariance(0),
	positiveCrossover(0),
	wayneCrossover(0),
	negativeCrossover(0)
{
	
	for(int i = MAX_LENGTH;i--;)
	{
		phaenoCode[i]=0;            // the final build order: an array of unit numbers (as defined in main.h)
		ipStatistics[i].resetData();
		ipStatistics[i+1].resetData();
		program[i].resetData();
	}
	for(int i = MAX_TIME;i--;)
		timeStatistics[i].resetData();
}

void ANARACE::resetData()
{
	PRERACE::resetPrerace();
        unitsTotal=4;
        unitsTotalMax=4;
        nonGoalsUnitsTotalMax=4;
        unchangedGenerations=0;
        currentRun=0;
        optimizing=false;
        active=true;
        totalGeneration=0;
        maxpFitness=0;
        maxsFitness=0;
        maxtFitness=MAX_TFITNESS;
        timePercentage=0;
        goalPercentage=0;
        currentpFitness=0;
        averageLength=0;
        fitnessAverage=0;
        fitnessVariance=0;
        positiveCrossover=0;
        wayneCrossover=0;
        negativeCrossover=0;
        for(int i = MAX_LENGTH;i--;)
        {
                phaenoCode[i]=0;            // the final build order: an array of unit numbers (as defined in main.h)
                ipStatistics[i].resetData();
                ipStatistics[i+1].resetData();
                program[i].resetData();
        }
        for(int i = MAX_TIME;i--;)
                timeStatistics[i].resetData();
}

ANARACE::~ANARACE()
{ }

ANARACE::ANARACE(const ANARACE& object) :
	PRERACE((PRERACE)object),
	unitsTotal( object.unitsTotal ),
	unitsTotalMax( object.unitsTotalMax ),
	nonGoalsUnitsTotalMax( object.nonGoalsUnitsTotalMax ),
	unchangedGenerations( object.unchangedGenerations ),
	currentRun( object.currentRun ),
	optimizing( object.optimizing ),
	active( object.active ),
	totalGeneration( object.totalGeneration ),
	maxpFitness( object.maxpFitness ),
	maxsFitness( object.maxsFitness ),
	maxtFitness( object.maxtFitness ),
	timePercentage( object.timePercentage ),
	goalPercentage( object.goalPercentage ),
	currentpFitness( object.currentpFitness ),
	averageLength( object.averageLength ),
	fitnessAverage( object.fitnessAverage ),
	fitnessVariance( object.fitnessVariance ),
	positiveCrossover( object.positiveCrossover ),
	wayneCrossover( object.wayneCrossover ), 
	negativeCrossover( object.negativeCrossover )
{
	for(int i = MAX_LENGTH;i--;)
	{
		phaenoCode[i] = object.phaenoCode[i];
		program[i] = object.program[i];
		ipStatistics[i] = object.ipStatistics[i];
	}	
	for(int i = MAX_TIME;i--;)
		timeStatistics[i] = object.timeStatistics[i];
}

ANARACE& ANARACE::operator=(const ANARACE& object)
{
	(PRERACE)(*this) = (PRERACE)object;
    unitsTotal = object.unitsTotal;
    unitsTotalMax = object.unitsTotalMax;
    nonGoalsUnitsTotalMax = object.nonGoalsUnitsTotalMax;
    unchangedGenerations = object.unchangedGenerations;
    currentRun = object.currentRun;
    optimizing = object.optimizing;
    active = object.active;
    totalGeneration = object.totalGeneration;
    maxpFitness = object.maxpFitness;
    maxsFitness = object.maxsFitness;
    maxtFitness = object.maxtFitness;
    timePercentage = object.timePercentage;
    goalPercentage = object.goalPercentage;
    currentpFitness = object.currentpFitness;
    averageLength = object.averageLength;
    fitnessAverage = object.fitnessAverage;
    fitnessVariance = object.fitnessVariance;
    positiveCrossover = object.positiveCrossover;
    wayneCrossover = object.wayneCrossover;
    negativeCrossover = object.negativeCrossover;
    for(int i = MAX_LENGTH;i--;)
    {
        phaenoCode[i] = object.phaenoCode[i];
        program[i] = object.program[i];
        ipStatistics[i] = object.ipStatistics[i];
    }
    for(int i = MAX_TIME;i--;)
        timeStatistics[i] = object.timeStatistics[i];
		
	return(*this);
}



void ANARACE::restartData()
{
	resetSpecial();
	setTotalGeneration(0);
	setMaxpFitness(0);
	setMaxsFitness(0);
	setMaxtFitness(MAX_TFITNESS);
	setUnchangedGenerations(0);
	unitsTotal=1; // ~4 ?
	unitsTotalMax=1;
	nonGoalsUnitsTotalMax=1;
	timePercentage=0;
	goalPercentage=0;
	averageLength=0;
	setTimer(0);
}


// Reset all ongoing data (between two runs)
void ANARACE::prepareForNewGeneration() // resets all data to standard starting values
{
	PRERACE::prepareForNewGeneration();
	setCurrentpFitness(0);

	for(int i=2*MAX_LENGTH;i--;)
	{
		setIPStatisticsHaveSupply(i, 0);
		setIPStatisticsHaveMinerals(i, 0);
		setIPStatisticsHaveGas(i, 0);
		setIPStatisticsNeedSupply(i, 0);
		setIPStatisticsFitness(i, 0);
	}
	
	for(int i=MAX_TIME;i--;)
	{
		setTimeStatisticsHaveSupply(i, 0);
		setTimeStatisticsHaveMinerals(i, 0);
		setTimeStatisticsHaveGas(i, 0);
		setTimeStatisticsNeedSupply(i, 0);
		setTimeStatisticsFitness(i, 0);
	}
	
	for(int i=MAX_LENGTH;i--;)//ANA~
	{
		setProgramSuccessType(i, 0);
		setProgramSuccessUnit(i, 0);
		setProgramIsBuilt(i, false);
		setProgramTime(i, 0);
		for(int j=UNIT_TYPE_COUNT;j--;)
		{
			setProgramAvailibleCount(i, j, 0);
			setProgramTotalCount(i, j, 0);
		}
		//program[i].temp=0;
		setProgramLocation(i,0);
//		setProgramIsGoal(i,false);
		setProgramFacility(i,0);
		setProgramBT(i,0);
		phaenoCode[i]=0;
	}
}

// ----------------------------------------
// ------ CORE OF THE CORE FUNCTIONS ------
// ----------------------------------------

const bool ANARACE::calculateStep()
{
//ZERG: CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
	//TODO: Fehler hier, getHaveSupply - getNeedSupply kann -1 werden!
	// needSupply war 11, maxneedSupply war 10 :/

	if((!getTimer())||(ready=calculateReady())||(!getIP()))
	{
		setLength(configuration.getMaxLength()-getIP());
		if(!ready) 
			setTimer(0);
		while(!buildingQueue.empty())
            buildingQueue.pop();

		for(int i=MAX_LENGTH;i--;)
			phaenoCode[i]=getpGoal()->toPhaeno(getCode(i));
	//	if(getpGoal()->getMode()==0)
			setCurrentpFitness(calculatePrimaryFitness(ready));
		
// ------ ANARACE SPECIFIC ------
		countUnitsTotal();
		unsigned int maxPoints=getpGoal()->countGoals();
		if(maxPoints>0)
			goalPercentage = 100 * currentpFitness / maxPoints;
		else goalPercentage = 0;

        setTimeStatisticsNeedSupply(getTimer(), getNeedSupply());
        setTimeStatisticsHaveSupply(getTimer(), getHaveSupply());
        setTimeStatisticsHaveMinerals(getTimer(), getMinerals());
        setTimeStatisticsHaveGas(getTimer(), getGas());
        setTimeStatisticsFitness(getTimer(), calculatePrimaryFitness(ready)); // ~~


// ------ END ANARACE SPECIFIC ------


		return(true);
	}

	bool ok=true;
	while((ok)&&(getIP()))
	{
// set needed_ to maximum to determine the minimum of minerals/gas our jobs need (needed_ is set in buildGene)
		neededMinerals=MAX_MINERALS;
		neededGas=MAX_GAS;
		setIPStatisticsNeedSupply(getIP()*2+1, getNeedSupply());
		setIPStatisticsHaveSupply(getIP()*2+1, getHaveSupply());
		setIPStatisticsHaveMinerals(getIP()*2+1, getMinerals());
		setIPStatisticsHaveGas(getIP()*2+1, getGas());
		setIPStatisticsFitness(getIP()*2+1, calculatePrimaryFitness(ready));

		ok=buildGene(getpGoal()->toPhaeno(getCurrentCode()));
	
		setIPStatisticsNeedSupply(getIP()*2, getNeedSupply());
		setIPStatisticsHaveSupply(getIP()*2, getHaveSupply());
		setIPStatisticsHaveMinerals(getIP()*2, getMinerals());
		setIPStatisticsHaveGas(getIP()*2, getGas());
		setIPStatisticsFitness(getIP()*2, calculatePrimaryFitness(ready));
	
		if(successType>0)
		{
			setProgramSuccessType(getIP(), successType);
			setProgramSuccessUnit(getIP(), successUnit);
		}
		
//TODO PROBLEM: wenn Einheit gebaut wurde (also in die build list incl. IP eingefuegt wurde) aber gleichzeitig ein timeout war, wird die Einheit als TIMEOUT markiert aber trotzdem gebaut
// Problemloesung: Ueberpruefen, dass utnen auch wirklich das Mininimum gewaehlt fuer t gewaehlt wird! 

		if((ok)||(!getTimeOut())) {
			if(ok) {
				setProgramTime(getIP(), getTimer());
			} else {
				setProgramTime(getIP(),configuration.getMaxTime());
				setProgramSuccessType(getIP(), TIMEOUT_ERROR);
				setProgramSuccessUnit(getIP(), 0);
//				setProgramSuccessLocation(0);
			}
			for(int i=UNIT_TYPE_COUNT;i--;)	{
				setProgramTotalCount(getIP(), i, getLocationTotal(GLOBAL, i));
				setProgramAvailibleCount(getIP(),i,getLocationAvailible(GLOBAL, i));
			}
	
			setTimeOut(configuration.getMaxTimeOut());
			setIP(getIP()-1);
		}
	}

//  ------ LEAP FORWARD IN TIME ------
	int t=calculateIdleTime();
//    int oldMinerals = getMinerals();
  //  int oldGas = getGas();

	for(int i=0;i<t;i++)
	{
 		setTimeStatisticsNeedSupply(getTimer()-i, getNeedSupply());
		setTimeStatisticsHaveSupply(getTimer()-i, getHaveSupply());
		setTimeStatisticsHaveMinerals(getTimer()-i, getMinerals()+harvestMinerals()*i);
		setTimeStatisticsHaveGas(getTimer()-i, getGas()+harvestGas()*i);
		setTimeStatisticsFitness(getTimer()-i, calculatePrimaryFitness(ready)); // ~~
	} 

	setMinerals(getMinerals()+harvestMinerals()*t);
	setHarvestedMinerals(getHarvestedMinerals()+harvestMinerals()*t);
//    setWastedMinerals(getWastedMinerals() + oldMinerals*t + (getMinerals() - oldMinerals) * t / 2);
	
	setGas(getGas()+harvestGas()*t);
	setHarvestedGas(getHarvestedGas()+harvestGas()*t);
  //  setWastedGas(getWastedGas() + oldGas*t + (getGas() - oldGas) * t / 2);

	
	setTimeOut(getTimeOut()-t);
	setTimer(getTimer()-t);
//  ------ END LEAP FORWARD IN TIME ------
	
	bool foundAnother=true;
    while((!buildingQueue.empty())&&(foundAnother==true))
    {
        if((buildingQueue.top().canBeCompleted()) && (buildingQueue.top().getBuildFinishedTime() == getTimer()))
        {
			foundAnother=true;
	       	const Building build = buildingQueue.top();
			const UNIT_STATISTICS* stat=&((*pStats)[build.getType()]);

// ------ ACTUAL BUILDING ------
			adjustLocationUnitsAfterCompletion(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2);			
			setHaveSupply(getHaveSupply()+stat->haveSupply);
			addLocationTotal(build.getLocation(),build.getType(), build.getUnitCount());
			addLocationAvailible(build.getLocation(),build.getType(),build.getUnitCount());
// ------ END OF ACTUAL BUILDING ------

			
// ------ SPECIAL RULES ------
			if(build.getType()==REFINERY) {
				removeOneMapLocationTotal(GLOBAL, build.getLocation(), VESPENE_GEYSIR);
				adjustGasHarvest(build.getLocation());
			} else
			if((build.getType()==COMMAND_CENTER)&&(!getLocationTotal(build.getLocation(),COMMAND_CENTER))) {
				adjustMineralHarvest(build.getLocation());
				adjustGasHarvest(build.getLocation());
			} else 
// RACE SPECIFIC!
			if((build.getType()==LARVA)&&(getpGoal()->getRace()==ZERG)) {
				removeLarvaFromQueue(build.getLocation());
			}
// ------ END SPECIAL RULES ------

			
// ------ CHECK WHETHER WE ARE READY ------
			getpGoal()->calculateFinalTimes(build.getLocation(), build.getType(), getLocationTotal(build.getLocation(), build.getType()), getRealTimer());
			ready=calculateReady();
// ------ END CHECK ------

			
// ------ ENQUEUE THE LAST ITEM SO IT CAN BE ACCESSED BY THE MOVING ROUTINES ------
//			last[lastcounter].unit=build.getType();
//			last[lastcounter].count=build.getUnitCount();
//			last[lastcounter].location=build.getLocation(); TODO

			if((stat->create)&&(!build.getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addOneLocationTotal(build.getLocation(), stat->create);
				addOneLocationAvailible(build.getLocation(), stat->create);
//				if(stat->create==LARVA)
//					while(true);	
//				if( last[lastcounter].unit == stat->create ) 
//					last[lastcounter].count++; //TODO ??? 
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}
					//evtl noch location==0 als 'egal wo' einfuehren
//			lastcounter++;
// ------ END OF LAST ITEM ------
			
// ------ RECORD THE BUILDING ------
			setProgramFacility(build.getIP(), build.getFacility());
			setProgramIsBuilt(build.getIP(),true);
			setProgramLocation(build.getIP(),build.getLocation());
			setProgramBT(build.getIP(),build.getTotalBuildTime()); //~~~
// ------ END OF RECORD -------
			
			buildingQueue.pop();
		} // end while(getremainingbuildorder) ~|~
		else foundAnother=false;
	} //end while
//TODO: Alles rausschmeissen, was schon von race berechnet wurde!
	
	return(false);

	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
}

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)

const bool ANARACE::buildGene(const unsigned int build_unit)
{
	const UNIT_STATISTICS* stat = &((*pStats)[build_unit]);
	bool ok = false;
	successType = OK;
	successUnit = 0;

	if(build_unit <= REFINERY+1)
	{
		if((stat->prerequisite[0] > 0)&&(getLocationTotal(GLOBAL, stat->prerequisite[0]) == 0)) {
			successUnit = stat->prerequisite[0];
			successType = PREREQUISITE_WAS_FULFILLED;
		}
		else		
		if((stat->prerequisite[1] > 0)&&(getLocationTotal(GLOBAL, stat->prerequisite[1]) == 0)) {
			successUnit = stat->prerequisite[1];
			successType = PREREQUISITE_WAS_FULFILLED;
		}
		else 
		if((stat->prerequisite[2] > 0)&&(getLocationTotal(GLOBAL, stat->prerequisite[2]) == 0)) {
			successUnit = stat->prerequisite[2];
			successType = PREREQUISITE_WAS_FULFILLED;
		}
		else
		if //ANA~
//TODO, < ((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)) einfuegen....
			( ((stat->facility[0] == 0)||(getLocationAvailible(GLOBAL, stat->facility[0]) == 0))&&
			  ((stat->facility[1] == 0)||(getLocationAvailible(GLOBAL, stat->facility[1]) == 0))&&
			  ((stat->facility[2] == 0)||(getLocationAvailible(GLOBAL, stat->facility[2]) == 0))&&
			  ((stat->facility[0] > 0)||(stat->facility[1] > 0)||(stat->facility[2] > 0))
			)
 //TODO: availible/force[0] auf 100 setzen... naj ama guggn
		{
			if(stat->facility[0] > 0)
				successUnit = stat->facility[0];
			else if(stat->facility[1] > 0)
				successUnit = stat->facility[1];
			else if(stat->facility[2] > 0)
				successUnit = stat->facility[2];
			successType = FACILITY_BECAME_AVAILIBLE;
		}
		else
		if(((stat->facility2 > 0) && (getLocationAvailible(GLOBAL, stat->facility2) == 0))||
		   ((stat->facility2 == build_unit) && (getLocationAvailible(GLOBAL, stat->facility2) < 2)))
		{
			successUnit = stat->facility2;
			successType = FACILITY_BECAME_AVAILIBLE;
		}
//TODO: evtl success 2 Schritte zurueckverfolgen...
		else
		if(((getHaveSupply() < getNeedSupply() + stat->needSupply - stat->haveSupply) ||
		    (stat->needSupply + getNeedSupply() > MAX_SUPPLY)) &&
			(stat->needSupply != 0)) {
			successUnit = 0;
			successType = SUPPLY_WAS_SATISFIED;
		}
		else
		if(getMinerals() < stat->minerals + stat->upgrade_cost * getLocationTotal(GLOBAL, build_unit))
		{
			successUnit = 0;
			successType = ENOUGH_MINERALS_WERE_GATHERED;
			if(neededMinerals > stat->minerals + stat->upgrade_cost * getLocationTotal(GLOBAL, build_unit) - getMinerals())
				neededMinerals = stat->minerals + stat->upgrade_cost * getLocationTotal(GLOBAL, build_unit) - getMinerals();
//Fehler is wahrscheinlich hier irgendwoe...
		}
		else
		if(getGas() < stat->gas + stat->upgrade_cost * getLocationTotal(GLOBAL, build_unit))
		{
			successUnit = 0;
			successType = ENOUGH_GAS_WAS_GATHERED;
			if(neededGas > stat->gas + stat->upgrade_cost * getLocationTotal(GLOBAL, build_unit) - getGas())
				neededGas = stat->gas + stat->upgrade_cost * getLocationTotal(GLOBAL, build_unit) - getGas();
		}
		else
		if((stat->upgrade[0] > 0) && (getLocationTotal(GLOBAL, build_unit) >= 2) && (!getLocationTotal(GLOBAL, stat->upgrade[0])))
		{
            successUnit = stat->upgrade[0];
			successType = PREREQUISITE_WAS_FULFILLED;
		}
		else
		if((stat->upgrade[1] > 0) && (getLocationTotal(GLOBAL, build_unit) >= 3) && (!getLocationTotal(GLOBAL, stat->upgrade[1])))
		{
            successUnit = stat->upgrade[1];
            successType = PREREQUISITE_WAS_FULFILLED;
		}
		else	
		{
			ok = buildIt(build_unit);
		} // if resources prere etc ok
	} // end build_unit <= REFINERY+1
//TODO: error wenn facilities net gefunden...
	else // build_unit > REFINERY+1
	{
	/*	int count=0;
		switch(build_unit)
		{
			   case MOVE_ONE_1_FORWARD:count=1;break;
			   case MOVE_ONE_3_FORWARD:count=3;break;
			   case MOVE_ONE_1_BACKWARD:count=-1;break;
			   default:count=0;break;
		}
		   if(count>0)
		{
			if((lastcounter>0)&&(getLocationAvailible(last[lastcounter-1].location,last[lastcounter-1].unit)>0)&&((*pStats)[last[lastcounter-1].unit].speed>0))
				{
					lastcounter--;
					int nr=0;
					while((nr<MAX_BUILDINGS-1)&&(getRemainingBuildTime(nr)))
							nr++;
					//TODO: Fehler wenn nicht genug buildings
					if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>last[lastcounter].count)
						setUnitCount(nr,last[lastcounter].count);
					else setUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
					setFacility(nr,0);
					if(last[lastcounter].location+count<1)
						count=MAX_LOCATIONS+count;
					else if(last[lastcounter].location+count>=MAX_LOCATIONS)
						count=1+last[lastcounter].location+count-MAX_LOCATIONS;
					else count=last[lastcounter].location+count;

					setLocation(nr,count);
					setType(nr,last[lastcounter].unit);
					setRemainingBuildTime(nr,pMap->getDistance(last[lastcounter].location,count)*100/(*pStats)[last[lastcounter].unit].speed);
					setOnTheRun(nr,1);
					setIP(nr,getIP()); // ~ANA
						// 2x Unit => send 8/All instead of just one unit there
			
					if((getIP()>1)&&((Code[0][getIP()-1]==build_unit)||(Code[1][getIP()-1]==build_unit)))
					{
										if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>=6)
									setUnitCount(nr,6);
								else setUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
								setProgramIsBuilt(getIP(),1);
	
								setIP(getIP()-1);
								//TODO dominance... und 8 checken... evtl weniger
					   }*/
/*					setProgramIsBuilt(getIP(),1);

					addLocationAvailible(last[lastcounter].location,getType(nr),-getUnitCount(nr));
					addLocationTotal(last[lastcounter].location,getType(nr),-getUnitCount(nr));
						ok=1;
				}
		}*/
	}
					
/*	  else
	  if((gRace==ZERG)&&(build_unit==BREAK_UP_BUILDING)&&(BuildingRunning>0)) // lieber eine variable mit last_gebaeude oder so machen und da die Daten speichern, anstatt Programm oder buildings durchzulaufen...
			{
				int min=5000;
				int n=0;
				for(i=0;i<MAX_BUILDINGS;i++)
					if(building[i].RB>0)
					{
						if((stats[2][building[i].type].type==2) && (stats[2][building[i].type].BT-building[i].RB<min))
		// type == 2 because this makes only sense with buildings
		// Sure... but unit about Lurker, Guardien, Sunken, ...  ?
		// mmmh... on the other hand this really makes no sense :}
						{
							min=stats[2][building[i].type].BT-building[i].RB;
							n=i;
						}
					}

					if(min<5000)
					{
						peonminerals++;
						minerals+=stats[2][building[n].type].minerals*0.75;
						gas+=stats[2][building[n].type].gas*0.75;
						Supply--;
						force[DRONE]++;
						if(building[n].type==REFINERY)
						{
							Vespene_Extractors--;
							Vespene_Av++;
						}
						building[n].type=255;
						building[n].RB=0;
//						program[IP].built=1;
//						program[building[n].IP].built=1;
					}*/
//TODO: Verwandtschaftsgrad fuer crossing over feststellen!	
//	if(ok) setProgramIsBuilt(getIP(),1); //??? TODO ???
//	else setProgramIsBuilt(getIP(),0);


	return(ok);
}



// -------------------------------------------
// ------ END CORE OF THE CORE FUNCTIONS -----
// -------------------------------------------


// -------------------------------
// ------ CONTROL FUNCTIONS ------
// -------------------------------

/*
void ANARACE::removeOrder(const unsigned int ip)
{
		for(int j=IP;j--;)
		{
				Code[j+1]=Code[j];
				Marker[j+1]=Marker[j];
				program[j+1].built=program[j].built;
		}
	for(int j=0;j<MAX_LENGTH;j++)
		phaenoCode[j]=getpGoal()->toPhaeno(Code[j]);
	(*pStartCondition)->wasChanged(); // to allow update of time etc. of anarace
};


void ANARACE::insertOrder(int unit, int position)
{
	int l=0;
	int i;

	for(i=MAX_LENGTH;(l<=position)&&(i--);)
		if(getProgramIsBuilt(i))
			l++;
	i++;
	if(i==0)
		i=MAX_LENGTH-1;
	for(int j=0;j<i;j++)
	{
		Code[j]=Code[j+1];
		Marker[j]=Marker[j+1];
		program[j].built=program[j+1].built;
	}
	
	if(getpGoal()->allGoal[unit]==0)
	{
		getpGoal()->addGoal(unit,1,0,0);
		(*pStartCondition)->changeAccepted();
	}
	replaceCode(i,getpGoal()->toGeno(unit));
	
	program[i].built=1;

	for(int j=0;j<MAX_LENGTH;j++)
		phaenoCode[j]=getpGoal()->toPhaeno(Code[j]); 
	(*pStartCondition)->wasChanged(); // to allow update of time etc. of anarace
}
*/
//void ANARACE::backupMap()
//{
/*	for(int i=0;i<pMap->getMaxPlayer()-1;i++)
		for(int j=0;j<MAX_LOCATIONS;j++)
			for(int k=0;k<UNIT_TYPE_COUNT;k++)
			{
				backupLoc[i][j].availible[k]=getMapLocationAvailible(i,j,k);
				backupLoc[i][j].force[k]=getMapLocationTotal(i,j,k);
			}*/
//	return(1);
//}

//void ANARACE::restoreMap()
//{
/*	for(int i=0;i<pMap->getMaxPlayer()-1;i++)
		for(int j=0;j<MAX_LOCATIONS;j++)
			for(int k=0;k<UNIT_TYPE_COUNT;k++)
			{
				setMapLocationAvailible(i,j,k,backupLoc[i][j].availible[k]);
				setMapLocationTotal(i,j,k,backupLoc[i][j].force[k]);
			}*/
//};

// -----------------------------------
// ------ END CONTROL FUNCTIONS ------
// -----------------------------------

// ----------------------------
// ------ MISC FUNCTIONS ------
// ----------------------------

void ANARACE::countUnitsTotal()
{
	unitsTotal = 0;
	unitsTotalMax = 1;
	nonGoalsUnitsTotalMax = 1;
	for (int i = GAS_SCV+1; i--;)
	{
		if (getLocationTotal(GLOBAL, i) > unitsTotalMax)
			unitsTotalMax = getLocationTotal(GLOBAL, i);
		if (getpGoal()->getAllGoal(i) > unitsTotalMax)
			unitsTotalMax = getpGoal()->getAllGoal(i);
		unitsTotal += getLocationTotal(GLOBAL, i);
		if ((getpGoal()->getAllGoal(i) == 0) && (getLocationTotal(GLOBAL, i) > nonGoalsUnitsTotalMax))
			nonGoalsUnitsTotalMax = getLocationTotal(GLOBAL, i);
	}
}

// -----------------------------------
// ------ END OF MISC FUNCTIONS ------
// -----------------------------------

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const unsigned int ANARACE::getAverageLength() const
{
	return(averageLength);
}

const unsigned int ANARACE::getUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(unitsTotalMax>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotalMax);
}

const unsigned int ANARACE::getNonGoalsUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(nonGoalsUnitsTotalMax>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getNonGoalsUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(nonGoalsUnitsTotalMax);
}
const unsigned int ANARACE::getUnitsTotal() const
{
#ifdef _SCC_DEBUG
	if(unitsTotal>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotal): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotal);
}

const bool ANARACE::isActive() const
{
	return(active);
}

void ANARACE::setActive(const bool set_active)
{
	active=set_active;
}

const bool ANARACE::isOptimizing() const
{
	return(optimizing);
}

void ANARACE::setOptimizing(const bool set_optimizing)
{
	optimizing=set_optimizing;
}

const unsigned int ANARACE::getProgramFacility(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Value ip out of range.");return(0);
	}
	if(program[ip].facility >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Variable not initialized.");return(0);
	}
#endif
//	if((getRace()==PROTOSS)&&(program[ip].facility==PROBE))
//		while(true);

	return(program[ip].facility);
}

const unsigned int ANARACE::getProgramBT(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramBT): Value ip out of range.");return(0);
	}
	if(program[ip].BT >= MAX_TIME) {
		toLog("DEBUG: (ANARACE::getProgramBT): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].BT);
}																	  

const unsigned int ANARACE::getProgramSuccessType(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Value ip out of range.");return(0);
	}
	if(program[ip].successType >= ERROR_MESSAGES) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].successType);
}

const unsigned int ANARACE::getProgramSuccessUnit(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Value ip out of range.");return(0);
	}
	if(program[ip].successUnit >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].successUnit);
}

/*void ANARACE::setProgramIsConstant(const unsigned int ip, const bool isConstant)
{
		if((ip >= MAX_LENGTH)) {
				toLog("DEBUG: (ANARACE::setProgramIsConstant): Value ip out of range.");return(0);
		}
		program[ip].isConstant = isConstant;
};*/

void ANARACE::setProgramAvailibleCount(const unsigned int ip, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value ip out of range.");return;
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value unit_type out of range.");return;
	}
	if(unit_count >= MAX_TOTAL_UNITS) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value unit_count out of range.");return;
	}
#endif
	program[ip].availibleCount[unit_type] = unit_count;
}

void ANARACE::setProgramTotalCount(const unsigned int ip, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value ip out of range.");return;
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value unit_type out of range.");return;
	}
	if(unit_count >= MAX_TOTAL_UNITS) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value unit_count out of range.");return;
	}
#endif
	program[ip].forceCount[unit_type] = unit_count;
}

// first time is 0!
void ANARACE::setProgramTime(const unsigned int ip, const unsigned int program_time)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value ip out of range.");return;
	}
	if(program_time > configuration.getMaxTime()) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value program_time out of range.");return;
	}
#endif
	program[ip].time = program_time;
}

void ANARACE::setProgramBT(const unsigned int ip, const unsigned int program_bt)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value ip out of range.");return;
	}
	if(program_bt >= MAX_TIME) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value program_bt out of range.");return;
	}
#endif
	program[ip].BT = program_bt;
}

void ANARACE::setProgramIsBuilt(const unsigned int ip, const bool program_is_built)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramIsBuilt): Value ip out of range.");return;
	}
#endif
	program[ip].built = program_is_built;
}

void ANARACE::setProgramLocation(const unsigned int ip, const unsigned int program_location)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value ip out of range.");return;
	}
	if(program_location >= MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value program_location out of range.");return;
	}
#endif
	program[ip].location = program_location;
}


// ------ STATISTICS SET FUNCTIONS ------
void ANARACE::setIPStatisticsNeedSupply(const unsigned int ip, const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsNeedSupply): Value ip out of range.");return;
    }
	if(need_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setIPStatisticsNeedSupply): Value need_supply out of range.");return;
	}
#endif
	ipStatistics[ip].needSupply = need_supply;
}

void ANARACE::setIPStatisticsHaveSupply(const unsigned int ip, const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsHaveSupply): Value ip out of range.");return;
    }
	if(have_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setIPStatisticsHaveSupply): Value have_supply out of range.");return;
	}
#endif
	ipStatistics[ip].haveSupply = have_supply;
}

void ANARACE::setIPStatisticsHaveMinerals(const unsigned int ip, const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsHaveMinerals): Value ip out of range.");return;
    }
	if(have_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::setIPStatisticsHaveMinerals): Value have_minerals out of range.");return;
	}
#endif
	ipStatistics[ip].minerals = have_minerals;
}
																				
void ANARACE::setIPStatisticsHaveGas(const unsigned int ip, const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsHaveGas): Value ip out of range.");return;
    }
	if(have_gas >= MAX_GAS) {
		toLog("DEBUG: (ANARACE::setIPStatisticsHaveGas): Value have_gas out of range.");return;
	}
#endif
	ipStatistics[ip].gas = have_gas;
}

void ANARACE::setIPStatisticsFitness(const unsigned int ip, const unsigned int fitness)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsFitness): Value ip out of range.");return;
    }
	if(fitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setIPStatisticsFitness): Value fitness out of range.");return;
	}
#endif
	ipStatistics[ip].fitness = fitness;
}


void ANARACE::setTimeStatisticsNeedSupply(const unsigned int time, const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsNeedSupply): Value time out of range.");return;
    }
	if(need_supply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsNeedSupply): Value need_supply out of range.");return;
	}
#endif
	timeStatistics[time].needSupply = need_supply;
}
void ANARACE::setTimeStatisticsHaveSupply(const unsigned int time, const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsHaveSupply): Value time out of range.");return;
    }
	if(have_supply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsHaveSupply): Value have_supply out of range.");return;
	}
#endif
	timeStatistics[time].haveSupply = have_supply;
}

void ANARACE::setTimeStatisticsHaveMinerals(const unsigned int time, const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
    if((time >= MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsHaveMinerals): Value time out of range.");return;
    }
	if(have_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsHaveMinerals): Value have_minerals out of range.");return;
	}
#endif
	timeStatistics[time].minerals = have_minerals;
}
																				
void ANARACE::setTimeStatisticsHaveGas(const unsigned int time, const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
    if((time >= MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsHaveGas): Value time out of range.");return;
    }
	if(have_gas >= MAX_GAS) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsHaveGas): Value have_gas out of range.");return;
	}
#endif
	timeStatistics[time].gas = have_gas;
}
void ANARACE::setTimeStatisticsFitness(const unsigned int time, const unsigned int fitness)
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsFitness): Value time out of range.");return;
    }
	if(fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsFitness): Value fitness out of range.");return;
	}
#endif
	timeStatistics[time].fitness=fitness;
}
// ------ END OF STATISTICS SET FUNCTIONS ------


const unsigned int ANARACE::getProgramTotalCount(const unsigned int ip, const unsigned int unit_type) const	   // how many units of the type at phaenoCode[s] do exist at that time?
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value ip out of range.");return(0);
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value unit_type out of range.");return(0);
	}
#endif
	return(program[ip].forceCount[unit_type]);
}

// how many units of the type at phaenoCode[s] do exist at that time?
const unsigned int ANARACE::getProgramAvailibleCount(const unsigned int ip, const unsigned int unit_type) const	   
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value ip out of range.");return(0);
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value unit_type out of range.");return(0);
	}
#endif
	return(program[ip].availibleCount[unit_type]);
}

void ANARACE::setProgramFacility(const unsigned int ip, const unsigned int facility)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value ip out of range.");return;
	}
	if(facility>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value facility out of range.");return;
	}
#endif
	program[ip].facility = facility;
}

void ANARACE::setProgramSuccessType(const unsigned int ip, const unsigned int type)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value ip out of range.");return;
	}
	if(type >= ERROR_MESSAGES) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value type out of range.");return;
	}
#endif
	program[ip].successType = type;
}

void ANARACE::setProgramSuccessUnit(const unsigned int ip, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value ip out of range.");return;
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value unit_type out of range.");return;
	}
#endif
	program[ip].successUnit = unit_type;
}

// ------ STATISTICS GET FUNCTIONS ------
const unsigned int ANARACE::getIPStatisticsNeedSupply(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsNeedSupply): Value ip out of range.");return(0);
    }
	if(ipStatistics[ip].needSupply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getIPStatisticsNeedSupply): Variable ipStatistics.needSupply not initialized.");	return(0);
	}
#endif
	return(ipStatistics[ip].needSupply);
}

const unsigned int ANARACE::getIPStatisticsHaveSupply(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsHaveSupply): Value ip out of range.");return(0);
    }
	if(ipStatistics[ip].haveSupply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getIPStatisticsHaveSupply): Variable ipStatistics.haveSupply not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].haveSupply);
}

const unsigned int ANARACE::getIPStatisticsHaveMinerals(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
	        toLog("DEBUG: (ANARACE::getIPStatisticsHaveMinerals): Value ip out of range.");return(0);
	}
	if(ipStatistics[ip].minerals > MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::getIPStatisticsHaveMinerals): Variable ipStatistics.minerals not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].minerals);
}

const unsigned int ANARACE::getIPStatisticsHaveGas(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsHaveGas): Value ip out of range.");return(0);
    }
	if(ipStatistics[ip].gas > MAX_GAS) {
		toLog("DEBUG: (ANARACE::getIPStatisticsHaveGas): Variable ipStatistics.gas not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].gas);
}

const unsigned int ANARACE::getIPStatisticsFitness(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsFitness): Value ip out of range.");return(0);
    }
	// TODO max pfitness 
	if(ipStatistics[ip].fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getIPStatisticsFitness): Variable ipStatistics.fitness not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].fitness);
}


const unsigned int ANARACE::getTimeStatisticsNeedSupply(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::getTimeStatisticsNeedSupply): Value time out of range.");return(0);
    }
	if(timeStatistics[time].needSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsNeedSupply): Variable timeStatistics.needSupply not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].needSupply);
}

const unsigned int ANARACE::getTimeStatisticsHaveSupply(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::getTimeStatisticsHaveSupply): Value time out of range.");return(0);
    }
	if(timeStatistics[time].haveSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsHaveSupply): Variable timeStatistics.haveSupply not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].haveSupply);
}

const unsigned int ANARACE::getTimeStatisticsHaveMinerals(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
	        toLog("DEBUG: (ANARACE::getTimeStatisticsHaveMinerals): Value time out of range.");return(0);
	}
	if(timeStatistics[time].minerals>MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsHaveMinerals): Variable timeStatistics.minerals not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].minerals);
}

const unsigned int ANARACE::getTimeStatisticsHaveGas(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::getTimeStatisticsHaveGas): Value time out of range.");return(0);
    }
	if(timeStatistics[time].gas>MAX_GAS) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsHaveGas): Variable timeStatistics.gas not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].gas);
}

const unsigned int ANARACE::getTimeStatisticsFitness(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::getTimeStatisticsFitness): Value time out of range.");return(0);
    }
	// TODO max pfitness 
	if(timeStatistics[time].fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsFitness): Variable timeStatistics.fitness not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].fitness);
}
// ------ END OF STATISTICS GET FUNCTIONS ------




const bool ANARACE::getProgramIsBuilt(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsBuilt): Value ip out of range.");return(0);
	}
#endif
	return(program[ip].built);
}


/*const bool ANARACE::getProgramIsConstant(const unsigned int ip)
{
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsConstant): Value ip [%i] out of range.",IP);return(false);
	}
	return(program[ip].isConstant);
};*/

// first program time is
const unsigned int ANARACE::getProgramTime(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTime): Value ip out of range.");return(0);
	}
	if(program[ip].time > configuration.getMaxTime()) {
		toLog("DEBUG: (ANARACE::getProgramTime): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].time);
}

const unsigned int ANARACE::getRealProgramTime(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getProgramLocation): Value ip out of range.");return(0);
    }
#endif
	return(configuration.getMaxTime() - getProgramTime(ip));
}


const unsigned int ANARACE::getProgramLocation(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Value ip out of range.");return(0);
	}
	if(program[ip].location > MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Variable program.location not initialized.");return(0);
	}
#endif
	return(program[ip].location);
}


const unsigned int ANARACE::getTimePercentage() const
{
	return(timePercentage);
}

const unsigned int ANARACE::getGoalPercentage() const
{
	if(getTimer()==0)
		return(goalPercentage);
	else 
	{
		unsigned int optimalTime = getpGoal()->calculateFastestBO((*pStartCondition)->getUnit(GLOBAL));
		if(optimalTime==0)
			return(100);
		else
			return(100*optimalTime/getRealTimer());
	}
}

const GOAL_TREE ANARACE::getGoalTree(const unsigned int currentGoalUnit) const
{
	return(getpGoal()->getGoalTree((*pStartCondition)->getUnit(GLOBAL), currentGoalUnit));
}

void ANARACE::setPhaenoCode(const unsigned int ip, const unsigned int phaeno_code)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value ip out of range.");return;
	}
	if(phaeno_code >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value phaeno_code out of range.");return;
	}
#endif
	phaenoCode[ip] = phaeno_code;
}

const unsigned int ANARACE::getPhaenoCode(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getPhaenoCode): Value ip out of range.");return(0);
	}
#endif
	return(phaenoCode[ip]);
}

const unsigned int ANARACE::getCurrentpFitness() const
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
    if(currentpFitness > MAX_PFITNESS) {
        toLog("DEBUG: (ANARACE::getCurrentpFitness): Variable currentpFitness not initialized.");return(0);
    }
#endif
	return(currentpFitness);
}

void ANARACE::setCurrentpFitness(const unsigned int current_pfitness)
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
    if(current_pfitness > MAX_PFITNESS) {
        toLog("DEBUG: (ANARACE::setCurrentpFitness): Variable current_pfitness not initialized.");return;
    }
#endif
	currentpFitness = current_pfitness;
}

const unsigned int ANARACE::getUnchangedGenerations() const
{
#ifdef _SCC_DEBUG	
	if(unchangedGenerations > configuration.getMaxGenerations()) {
		toLog("DEBUG: (ANARACE::getUnchangedGenerations): Variable unchangedGenerations not initialized.");return(0);
	}
#endif
	return(unchangedGenerations);
}

const unsigned int ANARACE::getRun() const
{
#ifdef _SCC_DEBUG	
	if(currentRun > configuration.getMaxRuns()) {
		toLog("DEBUG: (ANARACE::getRun): Variable currentRun not initialized.");return(0);
	}
#endif
	return(currentRun);
}

const unsigned int ANARACE::getTotalGeneration() const
{
	return(totalGeneration);
}

const unsigned int ANARACE::getMaxpFitness() const
{
#ifdef _SCC_DEBUG	
// TODO max pfitness?
	if(maxpFitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getMaxpFitness): Variable maxpFitness not initialized.");return(0);
	}
#endif
	return(maxpFitness);
}

const unsigned int ANARACE::getMaxsFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxsFitness > MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANARACE::getMaxsFitness): Variable maxsFitness not initialize.");return(0);
	}
#endif
	return(maxsFitness);
}

const unsigned int ANARACE::getMaxtFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxtFitness > MAX_TFITNESS) {
		toLog("DEBUG: (ANARACE::getMaxtFitness): Variable maxtFitness not initialized.");return(0);
	}
#endif
	return(maxtFitness);
}

void ANARACE::setUnchangedGenerations(const unsigned int unchanged_generations)
{
#ifdef _SCC_DEBUG	
	if(unchanged_generations > configuration.getMaxGenerations()) {
		toLog("DEBUG: (ANARACE::setUnchangedGenerations): Value out of range.");return;
	}
#endif
	unchangedGenerations = unchanged_generations;
}

void ANARACE::setRun(const unsigned int current_run)
{
#ifdef _SCC_DEBUG
	if(current_run > configuration.getMaxRuns()) {
		toLog("DEBUG: (ANARACE::setRun): Value out of range.");return;
	}
#endif
	currentRun = current_run;
}

void ANARACE::setTotalGeneration(const unsigned int total_generation)
{
	totalGeneration = total_generation;
}

void ANARACE::setMaxpFitness(const unsigned int max_pfitness) 
{
#ifdef _SCC_DEBUG
	// TODO maxpitness
	if(max_pfitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setMaxpFitness): Value out of range.");return;
	}
#endif
	maxpFitness = max_pfitness;
}

void ANARACE::setMaxsFitness(const unsigned int max_sfitness)
{
#ifdef _SCC_DEBUG
	if(max_sfitness > MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANARACE::setMaxsFitness): Value out of range.");return;
	}
#endif
	maxsFitness = max_sfitness;
}

void ANARACE::setMaxtFitness(const unsigned int max_tfitness)
{
#ifdef _SCC_DEBUG
	if(max_tfitness > MAX_TFITNESS) {
		toLog("DEBUG: (ANARACE::setMaxtFitness): Value out of range.");return;
	}
#endif
	maxtFitness = max_tfitness;
}

/*void ANARACE::analyzeBuildOrder()
{
//keeps track of the '@' symbol on the left of each build order entry
//if the goal is set to 10 marines, the eleventh won't be marked as a 'fulfilled goal' with a '@'
	int tGoal[UNIT_TYPE_COUNT];
// reset the tgGoals (to sign with '@' the units which are part of the goal list)
// and subtract the units that are already on the map
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
	{
		tGoal[i]=0;
		for(int j=1;j<MAX_LOCATIONS;j++)
			tGoal[i]+=getpGoal()->globalGoal[j][i]-pStartcondition->getLocationTotal(j,i);
	}

	for(int i=0;i<MAX_LENGTH;i++)
		if(tGoal[phaenoCode[i]]>0) //~~~~~~ location=0?
		{
			tGoal[phaenoCode[i]]--;
			setProgramIsGoal(i,true);
		}
	else setProgramIsGoal(i,false);
};*/


unsigned int ANARACE::successType;
unsigned int ANARACE::successUnit;

