#include "anabuildorder.hpp"

/* ANABUILDORDER.CPP - last changed: 6/23/04							  *
 * Author: Clemens Lode											 *
 * Copyright: Clemens Lode										  *
 *																  *
 *  PURPPOSE OF THIS FILE:										  *
 *	  This is the core of the core, where the main calculations   *
 *	  are made. Also this class is used as an interface to the	*
 *	  core from outside the library. In addition some data is	 *
		preformatted/precalculated -> statistics.					*/


void ANABUILDORDER::resetStaticData()
{
	successType = 0;
	successUnit = 0;
}

// TODO: reimplement/recheck the speed of the units
ANABUILDORDER::ANABUILDORDER():
	PREBUILDORDER(),
	fitnessAverage(0),
	fitnessVariance(0),
	positiveCrossover(0),
	wayneCrossover(0),
	negativeCrossover(0),
	unitsTotal(4),
	unitsTotalMax(4),
	nonGoalsUnitsTotalMax(4),
	optimizing(false),
	active(false),
	maxpFitness(0),
	maxsFitness(0),
	maxtFitness(MAX_TFITNESS),
	timePercentage(0),
	goalPercentage(0),
	currentpFitness(0),
	averageLength(0)
{
	for(unsigned int i = MAX_TIME;i--;)
		timeStatistics[i].resetData();
}

void ANABUILDORDER::resetData()
{
	PREBUILDORDER::resetData();
	unitsTotal = 4;
	unitsTotalMax = 4;
	nonGoalsUnitsTotalMax = 4;
	optimizing = false;
	active = false;
	maxpFitness = 0;
	maxsFitness = 0;
	maxtFitness = MAX_TFITNESS;
	timePercentage = 0;
	goalPercentage = 0;
	currentpFitness = 0;
	averageLength = 0;
	fitnessAverage = 0;
	fitnessVariance = 0;
	positiveCrossover = 0;
	wayneCrossover = 0;
	negativeCrossover = 0;
	for(unsigned int i = MAX_TIME;i--;)
		timeStatistics[i].resetData();
	programList.clear();	
}

ANABUILDORDER::~ANABUILDORDER()
{ }

ANABUILDORDER::ANABUILDORDER(const ANABUILDORDER& object) :
	PREBUILDORDER((PREBUILDORDER)object),
	fitnessAverage( object.fitnessAverage ),
	fitnessVariance( object.fitnessVariance ),
	positiveCrossover( object.positiveCrossover ),
	wayneCrossover( object.wayneCrossover ), 
	negativeCrossover( object.negativeCrossover ),
	unitsTotal( object.unitsTotal ),
	unitsTotalMax( object.unitsTotalMax ),
	nonGoalsUnitsTotalMax( object.nonGoalsUnitsTotalMax ),
	optimizing( object.optimizing ),
	active( object.active ), 
	maxpFitness( object.maxpFitness ),
	maxsFitness( object.maxsFitness ),
	maxtFitness( object.maxtFitness ),
	timePercentage( object.timePercentage ),
	goalPercentage( object.goalPercentage ),
	currentpFitness( object.currentpFitness ),
	averageLength( object.averageLength )
{
	memcpy(timeStatistics, object.timeStatistics, MAX_TIME * sizeof(STATISTICS));
}

ANABUILDORDER& ANABUILDORDER::operator=(const ANABUILDORDER& object)
{
	(PREBUILDORDER)(*this) = (PREBUILDORDER)object;
	fitnessAverage = object.fitnessAverage;
	fitnessVariance = object.fitnessVariance;
	positiveCrossover = object.positiveCrossover;
	wayneCrossover = object.wayneCrossover;
	negativeCrossover = object.negativeCrossover;
	unitsTotal = object.unitsTotal;
	unitsTotalMax = object.unitsTotalMax;
	nonGoalsUnitsTotalMax = object.nonGoalsUnitsTotalMax;
	optimizing = object.optimizing;
	active = object.active;
	maxpFitness = object.maxpFitness;
	maxsFitness = object.maxsFitness;
	maxtFitness = object.maxtFitness;
	timePercentage = object.timePercentage;
	goalPercentage = object.goalPercentage;
	currentpFitness = object.currentpFitness;
	averageLength = object.averageLength;
        memcpy(timeStatistics, object.timeStatistics, MAX_TIME * sizeof(STATISTICS));

	return(*this);
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
	averageLength = 0;
	setTimer(0); //?
}


// Reset all ongoing data (between two runs)
void ANABUILDORDER::prepareForNewGeneration() // resets all data to standard starting values
{
	PREBUILDORDER::prepareForNewGeneration();
	setCurrentpFitness(0);

	memset(timeStatistics, 0, MAX_TIME * sizeof(STATISTICS));

	std::list<PROGRAM>::iterator i = programList.begin();
	while(i!=programList.end())
		i = programList.erase(i);	
}

// ----------------------------------------
// ------ CORE OF THE CORE FUNCTIONS ------
// ----------------------------------------

void ANABUILDORDER::postProcessing()
{
	countUnitsTotal();
	unsigned int maxPoints=getGoal()->countGoals();
	if(maxPoints>0)
		goalPercentage = 100 * currentpFitness / maxPoints;
	else goalPercentage = 0;
}

const bool ANABUILDORDER::calculateStep()
{
//ZERG: CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
	//TODO: Fehler hier, getHaveSupply - getNeedSupply kann -1 werden!
	// needSupply war 11, maxneedSupply war 10 :/

	if((!getTimer()) || (ready = calculateReady()) || (!getIP()))
	{
		setLength(coreConfiguration.getMaxLength()-getIP());
		if(!ready) 
			setTimer(0);
//		if(getGoal()->getMode()==0)
			setCurrentpFitness(calculatePrimaryFitness(ready));
		while(!buildingQueue.empty())
			buildingQueue.pop();

		if(ready)
		{
			std::list<PROGRAM>::iterator i = programList.begin(); 
			while(i!=programList.end())
			{
				if((getTimer() < i->getBT()) || (i->getTime() < getTimer() - i->getBT()))
				{
					i = programList.erase(i);
					toErrorLog("deleted something");
				}
				else ++i;
			}
		}
		postProcessing();
	
// ------ ANABUILDORDER SPECIFIC ------


//		setTimeStatisticsNeedSupply(getTimer(), getNeedSupply());
//		setTimeStatisticsHaveSupply(getTimer(), getHaveSupply());
//		setTimeStatisticsHaveMinerals(getTimer(), getMinerals());
//		setTimeStatisticsHaveGas(getTimer(), getGas());
//		setTimeStatisticsFitness(getTimer(), calculatePrimaryFitness(ready)); // ~~

// ------ END ANABUILDORDER SPECIFIC ------

		return(true);
	}

	bool ok=true;
	while((ok)&&(getIP()))
	{
		unsigned int code = getGoal()->toPhaeno(getCurrentCode());
		if((code >= BUILD_PARALLEL_2) && (code <= BUILD_PARALLEL_16))
                {
                        setIP(getIP()-1);
                        while((getGoal()->toPhaeno(getCurrentCode()) >= LAST_UNIT)&&(getIP()))
			{
#ifdef _SCC_DEBUG
				toErrorLog("WARNING (BUILDORDER::calculateStep()): Current code out of range => ignoring order.");
#endif
			        setIP(getIP()-1);
			}
                        PARALLEL_COMMAND* pcommand = new PARALLEL_COMMAND;
                        switch(code)
                        {
                                case BUILD_PARALLEL_2:pcommand->count = 2;break;
                                case BUILD_PARALLEL_4:pcommand->count = 4;break;
                                case BUILD_PARALLEL_8:pcommand->count = 8;break;
                                case BUILD_PARALLEL_16:pcommand->count = 16;break;
                                default:break; // ~~
                        }
                        pcommand->unit = getGoal()->toPhaeno(getCurrentCode());
			
                        if(getIP()) 
				setIP(getIP()-1);
                        parallelCommandQueues.push_back(pcommand);
                        ok = true;
                } else
		{
// set needed_ to maximum to determine the minimum of minerals/gas our jobs need (needed_ is set in buildGene)
			neededMinerals = MAX_MINERALS;
			neededGas = MAX_GAS;
			
			ok = buildGene(getGoal()->toPhaeno(getCurrentCode()));

//TODO PROBLEM: wenn Einheit gebaut wurde (also in die build list incl. IP eingefuegt wurde) aber gleichzeitig ein timeout war, wird die Einheit als TIMEOUT markiert aber trotzdem gebaut
// Problemloesung: Ueberpruefen, dass utnen auch wirklich das Mininimum gewaehlt fuer t gewaehlt wird! 

			if((ok) || (!getTimeOut())) 
			{
				setTimeOut(coreConfiguration.getMaxTimeOut());
				setIP(getIP()-1);
			}
// Try parallel commands
			
       	                std::list<PARALLEL_COMMAND*>::iterator command = parallelCommandQueues.begin(); 
               	        while(command != parallelCommandQueues.end())
                       	{
				unsigned int build_unit = (*command)->unit;
                               	ok = buildGene((*command)->unit);
                                if(ok)  
      	                        {
               	                        --(*command)->count;
                       	                if(!(*command)->count)
					{
						delete *command;
                               	                command = parallelCommandQueues.erase(command);
					}
               	                } else 
				{
					do
						++command;
					while((command!=parallelCommandQueues.end())&&((*command)->unit == build_unit));
				}
                	}
		}
	}

//  ------ LEAP FORWARD IN TIME ------
	unsigned int t = calculateIdleTime();
//	int oldMinerals = getMinerals();
  //  int oldGas = getGas();
  	
//	if(t>0)
//	{
//		setProgramSuccessType(getIP(), TIMEOUT_ERROR);
//		setProgramSuccessUnit(getIP(), 0); TODO
//		setProgramSuccessLocation(0);
//	}

	for(unsigned int i = t; i--;)
	{
 		timeStatistics[getTimer()-i].setNeedSupply(getNeedSupply());
		timeStatistics[getTimer()-i].setHaveSupply(getHaveSupply());
		timeStatistics[getTimer()-i].setHaveMinerals(getMinerals()+harvestMinerals()*i);
		timeStatistics[getTimer()-i].setHaveGas(getGas()+harvestGas()*i);
		timeStatistics[getTimer()-i].setFitness(calculatePrimaryFitness(ready)); // ~~
	}

	setMinerals(getMinerals()+harvestMinerals()*t);
	setGas(getGas()+harvestGas()*t);
//#ifdef _SCC_DEBUG // not really needed because buildorder itself records that TODO
	setHarvestedGas(getHarvestedGas()+harvestGas()*t);
	setHarvestedMinerals(getHarvestedMinerals()+harvestMinerals()*t);
//#endif
//      setWastedGas(getWastedGas() + oldGas*t + (getGas() - oldGas) * t / 2);
//	setWastedMinerals(getWastedMinerals() + oldMinerals*t + (getMinerals() - oldMinerals) * t / 2);

	
	setTimeOut(getTimeOut()-t);
	setTimer(getTimer()-t);
//  ------ END LEAP FORWARD IN TIME ------
	
	bool foundAnother = true;
	while((!buildingQueue.empty()) && (foundAnother == true))
	{
		if((buildingQueue.top().canBeCompleted()) && (buildingQueue.top().getBuildFinishedTime() == getTimer()))
		{
			foundAnother = true;
			const Building& build(buildingQueue.top());
			if(buildingQueue.top().getType() == INTRON)
			{
				buildingQueue.pop();
				continue;
			}

			const UNIT_STATISTICS* stat = &((*pStats)[build.getType()]);

// ------ ACTUAL BUILDING ------
			adjustLocationUnitsAfterCompletion(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2, build.getUnitCount());
			setHaveSupply(getHaveSupply()+stat->haveSupply);
			addLocationTotal(build.getLocation(), build.getType(), build.getUnitCount());
			addLocationAvailible(build.getLocation(), build.getType(), build.getUnitCount());
// ------ END OF ACTUAL BUILDING ------

			
// ------ SPECIAL RULES ------
			if(build.getType() == REFINERY) {
				removeOneMapLocationTotal(GLOBAL, build.getLocation(), VESPENE_GEYSIR);
				adjustGasHarvest(build.getLocation());
			} else
			if((build.getType() == COMMAND_CENTER)&&(!getLocationTotal(build.getLocation(), COMMAND_CENTER))) {
				adjustMineralHarvest(build.getLocation());
				adjustGasHarvest(build.getLocation());
			} else 
// BUILDORDER SPECIFIC!
			if((getGoal()->getRace() == ZERG)&&(build.getType() == LARVA))
			{
				removeLarvaFromQueue(build.getLocation());
				if(checkForLarva(build.getLocation()))
					buildIt(LARVA);
			}
// ------ END SPECIAL RULES ------

			
// ------ CHECK WHETHER WE ARE READY ------
			getGoal()->calculateFinalTimes(build.getLocation(), build.getType(), getLocationTotal(build.getLocation(), build.getType()), getRealTimer());
			ready = calculateReady();
// ------ END CHECK ------

			
// ------ ENQUEUE THE LAST ITEM SO IT CAN BE ACCESSED BY THE MOVING ROUTINES ------
//			last[lastcounter].unit=build.getType();
//			last[lastcounter].count=build.getUnitCount();
//			last[lastcounter].location=build.getLocation(); TODO

			if((stat->create)&&(stat->create!=build.getType())&&(!build.getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addOneLocationTotal(build.getLocation(), stat->create);
				addOneLocationAvailible(build.getLocation(), stat->create);
//				if( last[lastcounter].unit == stat->create ) 
//					++last[lastcounter].count; //TODO ??? 
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}
					//evtl noch location==0 als 'egal wo' einfuehren
//			++lastcounter;
// ------ END OF LAST ITEM ------
			
// ------ RECORD THE BUILDING ------
//			setProgramFacility(build.getIP(), build.getFacility());
//			setProgramIsBuilt(build.getIP(), true);
//			setProgramLocation(build.getIP(), build.getLocation());
//			setProgramBT(build.getIP(), build.getTotalBuildTime()); //~~~
// ------ END OF RECORD -------
			buildingQueue.pop();
			
		} // end while(getremainingbuildorder) ~|~
		else foundAnother = false;
	} //end while
//TODO: Alles rausschmeissen, was schon von race berechnet wurde!
	
	return(false);

	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
}

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)

const bool ANABUILDORDER::buildGene(const unsigned int build_unit)
{
	const UNIT_STATISTICS* stat = &((*pStats)[build_unit]);
	bool ok = false;
	successType = OK;
	successUnit = 0;

	if(build_unit <= LAST_UNIT)
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
							++nr;
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
				for(i=0;i<MAX_BUILDINGS;++i)
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
						++peonminerals;
						minerals+=stats[2][building[n].type].minerals*0.75;
						gas+=stats[2][building[n].type].gas*0.75;
						--Supply;
						++force[DRONE];
						if(building[n].type==REFINERY)
						{
							--Vespene_Extractors;
							++Vespene_Av;
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

const bool ANABUILDORDER::buildIt(const unsigned int build_unit)
{
	//Zuerst: availible pruefen ob am Ort gebaut werden kann
	//Wenn nicht => +/- absteigen bis alle locations durch sind

	const UNIT_STATISTICS* stat = &((*pStats)[build_unit]);
	bool ok = false;
	unsigned int picked_facility = 0;
	unsigned int current_location_window = 1; // TODO
//	unsigned int ttloc=0;
//	unsigned int j=0;

/*	if(lastcounter>0)
	{	
		--lastcounter;
		tloc=last[lastcounter].location;
	}*/

	if((!buildingQueue.empty())&&(buildingQueue.top().getType() == INTRON))
		return(false);

	if(stat->facility[0]==0)
		ok=true;
	else
	// special rule for morphing units of protoss
	if((stat->facility2>0) && ((stat->facilityType == IS_LOST) || (stat->facilityType == IS_MORPHING)) && (stat->facility[0] == stat->facility2))
	{
		if(getLocationAvailible(current_location_window, stat->facility2) >=2)
		{
			ok = true;
			picked_facility = 0;
		}
	} else
	{
		// research/upgrade:
		if((stat->facility2==0) || (getLocationAvailible(current_location_window, stat->facility2)>=1))
		{
		// pick one availible facility: 
			for(picked_facility = 0; picked_facility<3; ++picked_facility)
				if((stat->facility[picked_facility]>0)&&(getLocationAvailible(current_location_window, stat->facility[picked_facility])>0))
				{
					ok=true;
					break;
				}
		}						
	}
				
//				j=1;
				// none found? search other parts of the map... TODO
/*				if(!ok)
					while(j<MAX_LOCATIONS)
					{
						ttloc=(*pMap)->getLocation(tloc)->getNearest(j);
//						if((stat->facility2==0)||(getLocationAvailible(ttloc,stat->facility2)>0)) TODO
//						{
//						for(fac=3; fac--;)
						for(fac=0; fac<3; ++fac)
						if(
						// special rules for morphing units of protoss
						((stat->facilityType != IS_LOST) || (stat->facility[fac] != stat->facility2) || (getLocationAvailible(ttloc, stat->facility[fac]) >= 2)) &&
						((stat->facility[fac] > 0) && (getLocationAvailible(ttloc, stat->facility[fac])))
						|| ((stat->facility[fac]==0)&&(fac==0))) //~~
																													   
//					  for(fac=3;fac--;)
//						  if( ((stat->facility[fac]>0)&&(getLocationAvailible(ttloc,stat->facility[fac])>((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)))) || ((stat->facility[fac]==0)&&(fac==0)))
							{
								tloc=ttloc;
								ok=true;
								break;
							}
//						  break;
//					  }
						++j;
					}*/
																													   
	if(ok)
	{
		if(build_unit==REFINERY)
		{
			if(getMapLocationAvailible(0, current_location_window, VESPENE_GEYSIR) <=0) // TODO!
				ok=false;
			else
				removeOneMapLocationAvailible(0, current_location_window, VESPENE_GEYSIR);
		} else 
		if(build_unit==GAS_SCV)
		{
			if(getLocationTotal(current_location_window, REFINERY)*4 <= getLocationTotal(current_location_window, GAS_SCV))
				ok=false;
			
		}
	}
//TODO: Wenn verschiedene facilities moeglich sind, dann das letzte nehmen
//			  bewegliche Sachen ueberdenken...
//				  evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//				  auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//	  Phagen ueber Phagen...
	if(ok)
	{ 
		Building build;
		build.setOnTheRun(false);
		build.setFacility(stat->facility[picked_facility]);
		build.setLocation(current_location_window);
		if(build_unit == FROM_GAS_TO_MINERALS)
			build.setUnitCount(getLocationAvailible(current_location_window, GAS_SCV));
		else
			build.setUnitCount(1+(stat->create == build_unit));
		build.setBuildFinishedTime(getTimer()-stat->BT);
		build.setTotalBuildTime(stat->BT);
		build.setType(build_unit);
		buildingQueue.push(build);

		bool is_larva = (getGoal()->getRace()==ZERG) && (build_unit==LARVA);									

		PROGRAM program;
		if(!is_larva)
		{
//		for(unsigned int i = UNIT_TYPE_COUNT; i--;)
//		{
//			program.setTotalCount(i, getLocationTotal(GLOBAL, i));
//			program.setAvailibleCount(i, getLocationAvailible(GLOBAL, i));
//		}
		program.setUnit(build_unit);
		program.setTime(getTimer());
		program.setFacility(build.getFacility());
		program.setLocation(build.getLocation());
		program.setBT(build.getTotalBuildTime());
	/*	if(successType>0)
		{
			program.setSuccessType(successType);
			program.setSuccessUnit(successUnit);
		}*/ //?
	
		program.before.setNeedSupply(getNeedSupply());
		program.before.setHaveSupply(getHaveSupply());
		program.before.setHaveMinerals(getMinerals());
		program.before.setHaveGas(getGas());
		}
		
// upgrade_cost is 0 if it's no upgrade
		setMinerals(getMinerals()-(stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)));
		setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)));
		setNeedSupply(getNeedSupply()+stat->needSupply);
//		if((stat->needSupply>0)||(((*pStats)[stat->facility[0]].needSupply<0)&&(stat->facilityType==IS_LOST)))  TODO!!!!
//		setNeedSupply(getNeedSupply()-stat->needSupply); //? Beschreibung!
		adjustAvailibility(current_location_window, picked_facility, stat);

// ---- SPECIAL RULES -----		
	 	
		if(getGoal()->getRace()==ZERG)
		{
			if(build_unit==LARVA)
				addLarvaToQueue(current_location_window);
			else
			if(((*pStats)[build_unit].facility[0]==LARVA)&&(checkForLarva(current_location_window)))
				buildIt(LARVA);
		}
// ---- END SPECIAL RULES -----
	
		if(!is_larva) //?
		{
			program.after.setNeedSupply(getNeedSupply());
			program.after.setHaveSupply(getHaveSupply());
			program.after.setHaveMinerals(getMinerals());
			program.after.setHaveGas(getGas());

			program.setUsedFacilityCount(getLocationTotal(GLOBAL, stat->facility[picked_facility]) - getLocationAvailible(GLOBAL, stat->facility[picked_facility])); // TODO evtl nach Ort
			programList.push_back(program);
		}

		if(build.getType() == INTRON)
		{
			std::list<Building> b_list;
			while((!buildingQueue.empty())&&(buildingQueue.top().getBuildFinishedTime() > build.getBuildFinishedTime()))
			{
				b_list.push_back(buildingQueue.top());
				buildingQueue.pop();
				
			}
			std::list<Building>::iterator i = b_list.begin();
			while(i!=b_list.end())
			{
				build.setBuildFinishedTime(i->getBuildFinishedTime());
				build.setTotalBuildTime(getTimer() - i->getBuildFinishedTime());
				buildingQueue.push(build);
				buildingQueue.push(*i);
				++i;
			}		
		}

	} //end if(ok)
	return(ok);
}

const bool ANABUILDORDER::writeProgramBackToCode(std::list<PROGRAM>& program_list)
{
	bool changed_bo=false;
	int ip=coreConfiguration.getMaxLength()-1;
	for(std::list<PROGRAM>::const_iterator i = program_list.begin(); i!=program_list.end();++i)
	{
		// TODO etwas problematisch... am besten intern pruefen... oder keine SCVs generell bauen? mmmh...
//		if((!isAlwaysBuildWorkers())||(i->getUnit()!=SCV))
		{
			if(replaceCode(ip, getGoal()->toGeno(i->getUnit())))
				changed_bo = true;
			--ip;
		}
	}
	setLength(program_list.size());
	return(changed_bo);
}


// -------------------------------------------
// ------ END CORE OF THE CORE FUNCTIONS -----
// -------------------------------------------


// -------------------------------
// ------ CONTROL FUNCTIONS ------
// -------------------------------

//void ANABUILDORDER::backupMap()
//{
/*	for(int i=0;i<pMap->getMaxPlayer()-1; ++i)
		for(int j=0;j<MAX_LOCATIONS; ++j)
			for(int k=0;k<UNIT_TYPE_COUNT; ++k)
			{
				backupLoc[i][j].availible[k]=getMapLocationAvailible(i,j,k);
				backupLoc[i][j].force[k]=getMapLocationTotal(i,j,k);
			}*/
//	return(1);
//}

//void ANABUILDORDER::restoreMap()
//{
/*	for(int i=0;i<pMap->getMaxPlayer()-1; ++i)
		for(int j=0;j<MAX_LOCATIONS; ++j)
			for(int k=0;k<UNIT_TYPE_COUNT; ++k)
			{
				setMapLocationAvailible(i,j,k,backupLoc[i][j].availible[k]);
				setMapLocationTotal(i,j,k,backupLoc[i][j].force[k]);
			}*/
//};



void ANABUILDORDER::copyProgramList(std::list<PROGRAM>& program_list)
{
	programList.clear();
	for(std::list<PROGRAM>::iterator i = program_list.begin();i!=program_list.end();++i)
	{
		PROGRAM p;
		p.setUnit(i->getUnit());
		// TODO ! mit in bo datei speichern
		p.setTime(coreConfiguration.getMaxTime());
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
//	unitsTotalMax = 1;
	nonGoalsUnitsTotalMax = 1;
	for (unsigned int i = LAST_UNIT; i--;)
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
		unsigned int optimalTime = getGoal()->calculateFastestBO((*(pStart->getStartCondition()))->getUnit(GLOBAL));
//		return(optimalTime);
		if(/*(optimalTime==0)||*/(getRealTimer()==0))
			return(100);
		else
			return(100*optimalTime/getRealTimer());
	}
}


unsigned int ANABUILDORDER::successType;
unsigned int ANABUILDORDER::successUnit;

