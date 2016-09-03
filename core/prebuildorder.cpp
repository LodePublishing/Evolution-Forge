#include "prebuildorder.hpp"
#include <string.h>
/* PREBUILDORDER.CPP - last changed: 6/23/04								*
 * Author: Clemens Lode												*
 * Copyright: Clemens Lode											*
 *																	*
 *  PURPPOSE OF THIS FILE:											*
 *		Supply various functions for the classes RACE and ANARACE,  *
 * 		mainly concerning harvesting, supply and units				*/


PREBUILDORDER::PREBUILDORDER():
//	location(0),
	pStartCondition(NULL),
	buildingQueue(), // TODO
//	lastcounter(0),
//	lastunit(0),
	pStats(NULL),
	neededMinerals(0),
	neededGas(0),
	ready(false),
	pGoal(NULL),
	playerNum(0),
	minerals(0),
	gas(0),
	timer(0), //? max_time-1?
	IP(0),
	harvestedMinerals(0),
	harvestedGas(0),
	wastedMinerals(0),
	wastedGas(0),
	needSupply(0),
	haveSupply(0),
	length(0),
	timeout(0)
{
	memset(mineralHarvestPerSecond, 0, 45 * MAX_LOCATIONS*sizeof(int));
	memset(gasHarvestPerSecond, 0, 5 * MAX_LOCATIONS*sizeof(int));
	memset(Code, 0, MAX_LENGTH*sizeof(int));
//	memset(Marker, 0, MAX_LENGTH*sizeof(int));
	resetSpecial();
}

void PREBUILDORDER::resetPrerace()
{
//	location = NULL;
	pStartCondition = NULL;
	while(!buildingQueue.empty())
		buildingQueue.pop();
	pStats = NULL;
	neededMinerals = 0;
	neededGas = 0;
	ready = false;
	pGoal = NULL;
	playerNum = 0;
	minerals = 0;
	gas = 0;
	timer = 0; //? max_time-1?
	IP = 0;
	harvestedMinerals = 0;
	harvestedGas = 0;
	wastedMinerals = 0;
	wastedGas = 0;
	needSupply = 0;
	haveSupply = 0;
	length = 0;
	timeout = 0;
	memset(mineralHarvestPerSecond, 0, 45 * MAX_LOCATIONS*sizeof(int));
	memset(gasHarvestPerSecond, 0, 5 * MAX_LOCATIONS*sizeof(int));
	memset(Code, 0, MAX_LENGTH*sizeof(int));
//	memset(Marker, 0, MAX_LENGTH*sizeof(int));
	resetSpecial();
}

PREBUILDORDER::PREBUILDORDER(const PREBUILDORDER& object) :
//	location(object.location),
	pStartCondition(object.pStartCondition),
	buildingQueue(object.buildingQueue),
//	lastcounter(object.lastcounter),
//	lastunit(object.lastunit),
	pStats(object.pStats),
	neededMinerals(object.neededMinerals),
	neededGas(object.neededGas),
	ready(object.ready),
	pGoal(object.pGoal),
	playerNum(object.playerNum),
	minerals(object.minerals),
	gas(object.gas),
	timer(object.timer),
	IP(object.IP),
	harvestedMinerals(object.harvestedMinerals),
	harvestedGas(object.harvestedGas),
	wastedMinerals(object.wastedMinerals),
	wastedGas(object.wastedGas),
	needSupply(object.needSupply),
	haveSupply(object.haveSupply),
	length(object.length),
	timeout(object.timeout)
{
	memcpy(Code, object.Code, MAX_LENGTH * sizeof(int));
//	memcpy(Marker, object.Marker, MAX_LENGTH * sizeof(int));
	//memcpy(last, object.last, MAX_LENGTH * sizeof(int));
	
	memcpy(larvaInProduction, object.larvaInProduction, MAX_LOCATIONS * sizeof(int));
	memcpy(mineralHarvestPerSecond, mineralHarvestPerSecond, MAX_LOCATIONS * 45 * sizeof(int));
	memcpy(gasHarvestPerSecond, object.gasHarvestPerSecond, MAX_LOCATIONS * 5 * sizeof(int));
}

PREBUILDORDER& PREBUILDORDER::operator=(const PREBUILDORDER& object)
{
//	location = object.location;
	pStartCondition = object.pStartCondition;
	buildingQueue = object.buildingQueue;
//	lastcounter = object.lastcounter;
  //  lastunit = object.lastunit;
	pStats = object.pStats;
	neededMinerals = object.neededMinerals;
	neededGas = object.neededGas;
	ready = object.ready;
	pGoal = object.pGoal;
	playerNum = object.playerNum;
	minerals = object.minerals;
	gas = object.gas;
	timer = object.timer;
	IP = object.IP;
	harvestedMinerals = object.harvestedMinerals;
	harvestedGas = object.harvestedGas;
	wastedMinerals = object.wastedMinerals;
	wastedGas = object.wastedGas;
	needSupply = object.needSupply;
	haveSupply = object.haveSupply;
	length = object.length;
	timeout = object.timeout;
	memcpy(Code, object.Code, MAX_LENGTH * sizeof(int));
//	memcpy(Marker, object.Marker, MAX_LENGTH * sizeof(int));
	//memcpy(last, object.last, MAX_LENGTH * sizeof(int));

	memcpy(larvaInProduction, object.larvaInProduction, MAX_LOCATIONS * sizeof(int));	
	memcpy(mineralHarvestPerSecond, object.mineralHarvestPerSecond, MAX_LOCATIONS * 45 * sizeof(int));
	memcpy(mineralHarvestPerSecond, object.mineralHarvestPerSecond, MAX_LOCATIONS * 5 * sizeof(int));

	return(*this);
}


void PREBUILDORDER::prepareForNewGeneration()
{
	assignStart(pStart);
	std::list<PARALLEL_COMMAND*>::iterator i = parallelCommandQueues.begin();
	while(i!=parallelCommandQueues.end())
	{
		delete *i;
		i = parallelCommandQueues.erase(i);
	}

	if(coreConfiguration.isAlwaysBuildWorker())
	{
		PARALLEL_COMMAND* gimme = new PARALLEL_COMMAND();
		gimme->unit = SCV;
		gimme->count = 64;
		parallelCommandQueues.push_back(gimme);
	}
								
	setHarvestedGas(0);setHarvestedMinerals(0);
	setWastedGas(0);setWastedMinerals(0);

	setMinerals((*pStartCondition)->getMinerals());
	setGas((*pStartCondition)->getGas());
	setTimer(coreConfiguration.getMaxTime()-(*pStartCondition)->getStartTime());
	setNeedSupply((*pStartCondition)->getNeedSupply());
	setHaveSupply((*pStartCondition)->getHaveSupply());
	
	const UNIT* start_units = (*pStartCondition)->getUnit(0);
//	int j = 0;
	for(unsigned int k=UNIT_TYPE_COUNT;k--;)
		if(start_units->getTotal(k))
		{
			getGoal()->calculateFinalTimes(0, k, start_units->getTotal(k), 0);
/*			last[j].location = 1;
			last[j].unit = k;
			last[j].count = 1;
			last[j].
			TODO liste oder Vektor*/
		}
		
	
/*	for(int k=4;k--;) //TODO
	{
		last[k].location=1;
		last[k].unit=SCV;
		last[k].count=1;
	}
	for(unsigned int int k=4;i<MAX_LENGTH; ++k)
	{
		last[k].location=0;
		last[k].unit=0;
		last[k].count=0;
	}
	lastcounter=4;*/
	setTimeOut(coreConfiguration.getMaxTimeOut());
	setIP(coreConfiguration.getMaxLength()-1);
	ready=false;

	resetSpecial();
}																																							

PREBUILDORDER::~PREBUILDORDER()
{
	std::list<PARALLEL_COMMAND*>::iterator i = parallelCommandQueues.begin();
	while(i!=parallelCommandQueues.end())
	{
		delete *i;
		i = parallelCommandQueues.erase(i);
	}
}

const eRace PREBUILDORDER::getRace() const
{
	return(pStart->getPlayerRace());
}

// -----------------------------------
// ------ CONTROLS FROM OUTSIDE ------
// -----------------------------------

void PREBUILDORDER::replaceCode(const unsigned int ip, const unsigned int code)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::replaceCode): Value ip out of range.");return;
	}
	if(code >= getGoal()->getMaxBuildTypes()) {
		toLog("DEBUG: (PREBUILDORDER::replaceCode): Value code out of range.");return;
	}
#endif
	setCode(ip, code);
//	++markerCounter;
//	setMarker(ip, markerCounter);
}

// ------------------------------------------
// ------ END OF CONTROLS FROM OUTSIDE ------
// ------------------------------------------



// --------------------------------------------------------------
// ------ FUNCTIONS THAT ARE PRIMARILY USED DURING THE RUN ------
// --------------------------------------------------------------

const unsigned int PREBUILDORDER::calculateIdleTime() const
{
	unsigned int idle_time=getTimer();
	unsigned int harvest_minerals = harvestMinerals();
	unsigned int harvest_gas = harvestGas();

	if((!buildingQueue.empty())&&(buildingQueue.top().canBeCompleted())) {
		// buildingRemaining = time until the building will be completed
#ifdef _SCC_DEBUG
		if(buildingQueue.top().getBuildFinishedTime() > getTimer())
		{
	// => Gebaeude sollte garnicht mehr in der buildingqueue sein...
			toLog("ERROR: PREBUILDORDER::calculateIdleTime(): FinishedTime > getTimer...");
		}
#endif
		unsigned int buildingRemaining = getTimer() - buildingQueue.top().getBuildFinishedTime();
		// TODO
		if(idle_time > buildingRemaining) 
			idle_time = buildingRemaining;
	}

	if( harvest_gas )
//	  ( harvest_minerals || 
  //		(neededGas + harvest_gas - neededGas % harvest_gas)/harvestGas() < (neededMinerals+(harvest_minerals - neededMinerals % harvest_minerals))/harvest_minerals)) 
	{
		unsigned int gasRemaining = ( neededGas + ( harvest_gas - neededGas % harvest_gas)) / harvest_gas;
		if(idle_time > gasRemaining) 
			idle_time = gasRemaining;
	}
	else if(harvest_minerals) {
		unsigned int mineralsRemaining=(neededMinerals + ( harvest_minerals - neededMinerals % harvest_minerals)) / harvest_minerals;
		if(idle_time>mineralsRemaining) 
			idle_time=mineralsRemaining;
	}
	if(idle_time>getTimeOut()) {
		idle_time=getTimeOut();
	}
	return(idle_time);
}

// called within the calculateStep function
void PREBUILDORDER::adjustLocationUnitsAfterCompletion(const unsigned int location_number, const eFacilityType facilityType, const unsigned int facility, const unsigned int facility2, const unsigned int count)
{
	switch(facilityType)
	{
		case NO_FACILITY:break;
		case IS_LOST:
		case IS_MORPHING:
			if(SPECIAL_UNIT_TYPE) // TODO
			{
				if(facility)
					setLocationTotal(location_number, facility, getLocationTotal(location_number, facility) - count);
//				if(facility2)
//					removeOneLocationTotal(location_number, facility2);
			} else
			{
				if(facility)
					removeOneLocationTotal(location_number, facility);
					//availible was already taken account when starting the building
				if(facility2)
					removeOneLocationTotal(location_number, facility2);
			}
			break;
		case NEEDED_ONCE:break;
		case NEEDED_UNTIL_COMPLETE:
			if(facility)
				addOneLocationAvailible(location_number, facility);
			break; // fuer spaeter mal: Wenn in 2 Fabriken produziert wuerde wirds probmelatisch, da
//in Buiding nur eine facility gespeichert wird...
		case NEEDED_ONCE_IS_LOST:
			if(facility2)
				removeOneLocationTotal(location_number, facility2);
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST:
			// this is a research, no supply needed, applied to GLOBAL
			if(facility)
				addOneLocationAvailible(location_number, facility);
			if(facility2)
				removeOneLocationTotal(GLOBAL, facility2);
//r_researches need location 0
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
			// this is an update, no supply, applies to GLOBAL
			if(facility)
				addOneLocationAvailible(location_number, facility);
			if(facility2) // special rule for upgrades!
			{
				removeOneLocationTotal(GLOBAL, facility2);
				if(getLocationTotal(GLOBAL, facility2) > 0)
					addOneLocationAvailible(GLOBAL, facility2);
			}
			break;
		case NEEDED_ALWAYS:break;
		case NO_FACILITY_BEHAVIOUR_DEFINED:
		case FACILITY_TYPES:
		default:
#ifdef _SCC_DEBUG
			toLog("ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
#endif
		break;
	} // end switch facilityType
}

void PREBUILDORDER::adjustAvailibility(const unsigned int loc, const unsigned int fac, const UNIT_STATISTICS* stat)
{
	switch(stat->facilityType)
	{
		case NO_FACILITY:break;
		case IS_LOST:
			if(stat->facility[fac]>0)
			{
				removeOneLocationAvailible(loc, stat->facility[fac]);
				setNeedSupply(getNeedSupply()-(*pStats)[stat->facility[fac]].needSupply);
				setHaveSupply(getHaveSupply()-(*pStats)[stat->facility[fac]].haveSupply);
			}
			if(stat->facility2>0)
			{
				removeOneLocationAvailible(loc, stat->facility2);
				setNeedSupply(getNeedSupply()-(*pStats)[stat->facility2].needSupply);
				setHaveSupply(getHaveSupply()-(*pStats)[stat->facility2].haveSupply);
			}
			break;
		case IS_MORPHING:
			if(stat->facility[fac]>0)
				removeOneLocationAvailible(loc, stat->facility[fac]);
			if(stat->facility2>0)
				removeOneLocationAvailible(loc, stat->facility2);
			break;
		case NEEDED_ONCE:
			break;
		case NEEDED_UNTIL_COMPLETE:
			if(stat->facility[fac]>0)
				removeOneLocationAvailible(loc, stat->facility[fac]);
			if(stat->facility2>0)
				removeOneLocationAvailible(loc, stat->facility2);
				//kommt glaub nicht vor...
			break;
		case NEEDED_ONCE_IS_LOST:
			if(stat->facility2>0)
			{
				removeOneLocationAvailible(loc, stat->facility2);
				setNeedSupply(getNeedSupply()-(*pStats)[stat->facility2].needSupply);
				setHaveSupply(getHaveSupply()-(*pStats)[stat->facility2].haveSupply);	
				// <- nicht noetig :/ eigentlich schon... bei gas scv z.B. :/
			}
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST:
			if(stat->facility2>0)
			{
				removeOneLocationAvailible(GLOBAL/*location*/ ,stat->facility2); // primarily for temporary R_researches, have to be location 0
// TODO				 setNeedSupply(getNeedSupply()+(*pStats)[stat->facility2].needSupply); // <- nicht noetig :/
			}
			if(stat->facility[fac]>0)
				removeOneLocationAvailible(loc, stat->facility[fac]);
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
			if(stat->facility2>0)
				removeOneLocationAvailible(GLOBAL/*loc*/,stat->facility2);
			// no needSupply gain as the item is recovered... well or not? mmmh... ~~~~
			if(stat->facility[fac]>0)
				removeOneLocationAvailible(loc, stat->facility[fac]);
			break;
		case NEEDED_ALWAYS:
			if(stat->facility[fac]>0)
				removeOneLocationAvailible(loc, stat->facility[fac]);
				// needSupply?
			if(stat->facility2>0)
				removeOneLocationAvailible(loc, stat->facility2);
			break;
		case NO_FACILITY_BEHAVIOUR_DEFINED:
		case FACILITY_TYPES:
		default:
#ifdef _SCC_DEBUG
			  toLog("ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
#endif
			break;
	}
}

/* compares the current force on the map and the finnishing times of the unit types with our predefined goals */
const bool PREBUILDORDER::calculateReady() const
{
	return(getGoal()->calculateReady((*unit)[playerNum]));
}

// nicht const da buildingqueue verzehrt wird :/
// TODO OPTIMIZE !!

// STARK VON LOCATIONS ABHAENGIG!
const unsigned int PREBUILDORDER::calculatePrimaryFitness(const bool is_ready)
{
	unsigned int tpF=0;
//TODO evtl noch uebrige availible miteinbeziehen
//TODO: Nicht alle Einheiten am Ort? => Ort egal sein lassen aber zur Zeit hinzuzaehlen
	// Nicht alle Einheiten ueberhaupt gebaut UND nicht alle am Ort => nur viertel Bonus fuer Einheiten die nicht am Ort sind
	if(!is_ready)
	{
// ------ INIT THE BONUS SYSTEM FOR REMAINING BUILD ORDERS IN THE BUILDING LIST ------
// temporary data to check whether a bonus is already given (only applies if total > goal)
		unsigned int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; 
		memset(bonus, 0, UNIT_TYPE_COUNT * MAX_LOCATIONS * sizeof(int));
// ------ END BONUSSYSTEM INIT ------

//				bool first=true;
		for(std::list<GOAL>::const_iterator i = getGoal()->goal.begin(); i!= getGoal()->goal.end(); ++i)
//		while(getGoal()->getNextGoal(i, first))
		{
//			first=false;
// ------ NOT ENOUGH UNITS BUILD OF THIS TYPE? -------
				if(i->getCount() > getLocationTotal(i->getLocation(), i->getUnit()))
				{
// ------ ADD TO MAX BONUS -----
					bonus[i->getLocation()][i->getUnit()]+=i->getCount()-getLocationTotal(i->getLocation(), i->getUnit());
					//total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
					//TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen
					//evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!
					unsigned int sumup=0;
// location 0? Dann ist es trivial
					if(i->getLocation()==0)
						sumup=getLocationTotal(0, i->getUnit())*100;
					else
					{
				// location != 0 ? Dann schaun wir mal, ob wir auf der Karte noch wo Units verstreut haben
				// mehr als goalcount koennen wir keinen Bonus vergeben
						unsigned int j=1;
						unsigned int bon = i->getCount();
						unsigned int loc=(*pMap)->getLocation(i->getLocation())->getNearest(j);
						while((j<(*getMap())->getMaxLocations()) && (bon>getLocationTotal(loc, i->getUnit())))
						{
							sumup+=getLocationTotal(loc, i->getUnit())*(100-(*pMap)->getLocation(loc)->getDistance(i->getLocation())); //was pMap->location[j]...
							bon-=getLocationTotal(loc, i->getUnit());
							++j;
							loc = (*pMap)->getLocation(i->getLocation())->getNearest(j);
						}
						// Falls j<MAX_LOCATIONS => unser "Bon" wurde schon vorher aufgebraucht => An dieser Stelle j den Rest draufgeben... 
						if(j<(*getMap())->getMaxLocations())
							sumup+=bon*(100-(*pMap)->getLocation(loc)->getDistance(i->getLocation()));
					}

//jetzt steht in sumup die gesammelten totals gewichtet mit den Entfernungen zum Ziel
										//TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
										// evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
										// bissl komplex da mans ja den einzelnen goals verteilen muss...
					if((i->getTime()>0)&&(i->getFinalTime()>i->getTime()))
//					{
//						if(getFinalTime(i)>0) //??? TODO
//						if(getFinalTime(i)>i->getTime())
						tpF+=(i->getTime()*sumup)/(i->getCount()*i->getFinalTime());
//						else setpFitness(getpFitness()+sumup/i->getCount());
//TODO...~~~
//						else while(true); // <- kann eigentlich nicht auftreten!
//						  setpFitness(getpFitness()+(i->getTime()*sumup)/(i->getCount()*coreConfiguration.maxTime));
//					  }
					else
						tpF+=sumup/i->getCount();
				} // END total < goal
				else
//if( /*((i->getLocation()==0)&&(i->getCount()<=getLocationTotal(0,i->getUnit()))) || ( (i->getLocation()>0)&&*/(i->getCount()<=getLocationTotal(i->getLocation(),i->getUnit())))
								//total >= goal ?
				{
// Checken wann wir das Ziel erreicht haetten
					if((i->getTime()>0)&&(i->getFinalTime()>i->getTime()))
// aha, wir haben die Zeit ueberschritten => trotzdem anteilig Bonus geben
						tpF+=(i->getTime()*100/i->getFinalTime());
// keine Zeitbeschraenkung + wir haben genuegend Einheiten am Zielort => gg
					else tpF+=100;
// does not work yet, if this is uncommented, sFitness occasionally jumps to -1222000 or something like that... :/
// include the final location maybe...
				}
		} //end of goal checking
// TODO: Check for very small 'goal.time' values, probably in scc.cpp!!

// ------ EXTRACT SOME INFORMATIONS OF THE REMAINING BUILD ORDER LIST ------
// bonus is filled with all goals sorted by location and time
		std::priority_queue<Building, std::vector<Building> >  save = buildingQueue;
		while(!buildingQueue.empty())
		{
			const Building& build = buildingQueue.top();
// ------ IS THIS BUILDING PART OF THE GOAL LIST? ------
			if(bonus[build.getLocation()][build.getType()] > 0)
			{
// as 'ready' is false we hit the time limit. Therefore buildFinishedTime is 
// negative and adding totalBuildTime results in the time the item was already
// in the queue 
				tpF+=(build.getTotalBuildTime()+build.getBuildFinishedTime())*100/bonus[build.getLocation()][build.getType()];

		// TODO time

/*				if((getGoal()->goal[build.getType(i)].time>0)&&(getLocationTotal(build.getLocation(i),build.getType(i))==0))
					pFitness+=(build.getRemainingBuildTime(i)*100*getGoal()->goal[build.getType(i)].time*getLocationTotal(0,i))/(getGoal()->goal[build.getType(i)].count*(*pStats)[build.getType(i)].BT*coreConfiguration.maxTime);//hier auch ProgramBT
				else
					pFitness+=((build.getRemainingBuildTime(i)*100)/((getGoal()->goal[build.getType(i)].count*(*pStats)[build.getType(i)].BT));*/

				--bonus[build.getLocation()][build.getType()];
			}
			buildingQueue.pop();
		}
		buildingQueue = save;
	} // end of is_ready=false
	else   // all goals fulfilled, fitness <- timer
	{
		tpF += getTimer();
		tpF += getGoal()->countGoals()*100;
//		for(std::list<GOAL>::const_iterator i = getGoal()->goal.begin();i!=getGoal()->goal.end();++i)
//				{
//						if(i->getCount()>0)
//								tpF+=100;
/*					  if((i->getUnit()!=GAS_SCV)&&(i->getUnit()!=SCV)) //do not punish 'too much' workers!
								if(i->getCount()<getLocationTotal(i->getLocation(),i->getUnit()))
										setsFitness(getsFitness()+(-getLocationTotal(i->getLocation(),i->getUnit())+i->getCount())*(stats[getGoal()->getRace()][i->getUnit()].minerals+stats[getGoal()->getRace()][i->getUnit()].gas));*/
//				}
	}
	return(tpF);
}
// end of calculatePrimaryFitness

#if 0
const bool PREBUILDORDER::buildIt(const unsigned int build_unit)
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
//						for(fac=3;fac--;)
						for(fac=0;fac<3; ++fac)
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
																													   
	if((ok)&&(build_unit==REFINERY)) {
		if(getMapLocationAvailible(GLOBAL, current_location_window, VESPENE_GEYSIR) <=0)
			ok=false;
		else
			removeOneMapLocationAvailible(GLOBAL, current_location_window, VESPENE_GEYSIR);
	}
//TODO: Wenn verschiedene facilities moeglich sind, dann das letzte nehmen
//			  bewegliche Sachen ueberdenken...
//				  evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//				  auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//	  Phagen ueber Phagen...
	if(ok)
	{ 
 		if((getGoal()->getRace()==ZERG) &&
//		  ((*pStats)[build_unit].facility[0]==LARVA)&&
			(build_unit!=LARVA) &&
		// Larva wird benoetigt zum Bau? Fein, dann bauen wir eine neue Larva falls nicht schon alle hatcheries etc. belegt sidn
				// Gesamtzahl der Larven < 3 * HATCHERY?
		   ((getLocationTotal(current_location_window, HATCHERY)+
			 getLocationTotal(current_location_window, LAIR)+
			 getLocationTotal(current_location_window, HIVE)) *3 > 
			 (larvaInProduction[current_location_window]+getLocationTotal(current_location_window, LARVA)))  &&
// max 1 larva pro Gebaeude produzieren
 		   ((getLocationTotal(current_location_window, HATCHERY)+
			 getLocationTotal(current_location_window, LAIR)+
			 getLocationTotal(current_location_window, HIVE) > 
			  larvaInProduction[current_location_window]))) // => zuwenig Larven da!
			{
				addLarvaToQueue(current_location_window);
				if(!buildIt(LARVA));
//					removeLarvaFromQueue(current_location_window);
			}
																													  
		Building build;
		build.setOnTheRun(false);
		build.setFacility(stat->facility[picked_facility]);
		build.setLocation(current_location_window);
		build.setUnitCount(1+(stat->create == build_unit));
		build.setBuildFinishedTime(getTimer()-stat->BT);
		build.setTotalBuildTime(stat->BT);
		build.setType(build_unit);
		build.setIP(getIP()); //needed only for Anarace!

		TODO program!
// upgrade_cost is 0 if it's no upgrade
		setMinerals(getMinerals()-(stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)));
		setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)));
		setNeedSupply(getNeedSupply()+stat->needSupply);
//		if((stat->needSupply>0)||(((*pStats)[stat->facility[0]].needSupply<0)&&(stat->facilityType==IS_LOST)))  TODO!!!!
//		setNeedSupply(getNeedSupply()-stat->needSupply); //? Beschreibung!
		adjustAvailibility(current_location_window, picked_facility, stat);
		buildingQueue.push(build);
	} //end if(ok)
	return(ok);
}
#endif
const bool PREBUILDORDER::isDifferent(const unsigned int* code) const //, const unsigned int* marker) const
{
	for(unsigned int i = MAX_LENGTH;i--;)
		if((getCode(i)!=code[i]))//||(getMarker(i)!=marker[i]))
			return(true);
	return(false);	
}

// ------------------------------
// ------ HARVEST ROUTINES ------
// ------------------------------

void PREBUILDORDER::adjustMineralHarvest(const unsigned int location_number)
{
	//TODO Zerg hatchery,lair etc.
	
// WAYNE Location,	  kein Command Center 							und keine Mineralien
	if((location_number == 0) || ((!getLocationTotal(location_number, COMMAND_CENTER)) && (!getMapLocationTotal(0, location_number, MINERAL_PATCH))))
	{
		for(unsigned int i=45;i--;)
			setMineralHarvestPerSecond(location_number, i, 0);
	}
//	else if((!pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(pMap->location[num].getTotal(0][MINERAL_PATCH]))
//	{
	//nach naehestem command_center suchen
//	}
//	else if((pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(!pMap->location[num].getTotal(0][MINERAL_PATCH]))
//	{
	//nach naehesten Mineralien suchen
//	} TODO

	//TODO: Wenn 2 SPieler an einem sammeln, beide einberechnen!
	else if(pStart->getBasicMineralHarvestSpeedPerSecond(1)) // SONST: Falls wir ne minimalSammelgeschwindigkeit haben eintragen
	{
//		int k;
		for(unsigned int i=45;i--;)
		{
//			k=0;
//			for(j=0;j<45;++j)
//				if(i*8<=j*pMap->location[num].getTotal(0][MINERAL_PATCH]) 
//				{ k=j;j=45;}
			setMineralHarvestPerSecond(location_number, i, pStart->getBasicMineralHarvestSpeedPerSecond(i/*k*/));//*pMap->location[num].getTotal(0][MINERAL_PATCH])/8;
		}	//ab hier funzt alles... nur scheint startPlayer->getBasic... nicht richtig initialisiert zu sein...
	
	}
}

void PREBUILDORDER::adjustGasHarvest(const unsigned int location_number)
{
	if((location_number == 0)||((!getLocationTotal(location_number, COMMAND_CENTER)) && (!getLocationTotal(location_number, REFINERY))))
	{
		for(unsigned int i=5;i--;)
			setGasHarvestPerSecond(location_number, i, 0);
	}
/*	else if((!pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(pMap->location[num].getTotal(playerNum][REFINERY]))
	{
	//nach naehestem command_center suchen
	}
	else if((pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(!pMap->location[num].getTotal(playerNum][REFINERY]))
	{
	//nach naehesten Mineralien suchen
	}*/
	else if(pStart->getBasicMineralHarvestSpeedPerSecond(1))
	{
//		int k;
		for(unsigned int i=5;i--;)
		{
//			k=0;
//			for(j=0;j<5;++j)
//				if(i<=j*pMap->location[num].getTotal(playerNum][REFINERY]) { k=j;j=5;}
					setGasHarvestPerSecond(location_number, i, pStart->getBasicGasHarvestSpeedPerSecond(i/*k*/)*getLocationTotal(location_number, REFINERY));

//					hier liegts problem wegen gas
//						gasharvestps ist zu gross, evtl wegen zu vieler refineries!
		}
	}
}

void PREBUILDORDER::adjustHarvestAllLocations()
{
	for(unsigned int i = (*getMap())->getMaxLocations();i--;)
	{
		adjustMineralHarvest(i);
		adjustGasHarvest(i);
	}
}

const unsigned int PREBUILDORDER::harvestMinerals() const
{
	unsigned int sum=0;
//	  int t=(rand()%10)-5;
	for(unsigned int i=1;i<(*getMap())->getMaxLocations();++i)//~~
	{
		unsigned int s=getLocationAvailible(i, SCV);
		if(s)
		{
			//availible is 0, harvestSpeed ist ok!
			if(s<44) //~~ war 56
				sum+=getMineralHarvestPerSecond(i,s);
			else
				sum+=getMineralHarvestPerSecond(i,44);
/*		{
			setMinerals(getMinerals()+getMineralHarvestPerSecond(i,s));
			//~~~ neu berechnen / Tabelle anlegen! sind ja nicht bei jeder Exe gleichviel Mineralien... <- das sollte sich mit adjustHarvest erledigt haben
			harvestedMinerals+=getMineralHarvestPerSecond(i,s); //~~~
		}
		else
		{
			setMinerals(getMinerals()+getMineralHarvestPerSecond(i,44)); //war 56
			harvestedMinerals+=getMineralHarvestPerSecond(i,44);
		}*/
		}
	}
	return(sum);
}


const unsigned int PREBUILDORDER::harvestGas() const
{
	unsigned int sum=0;
//	int t=(rand()%10)-5;
	for(unsigned int i=1;i<(*getMap())->getMaxLocations();++i)//~~
	{
		unsigned int s = getLocationAvailible(i, GAS_SCV);
		if(s)
		{
			if(s<4)
				sum+=getGasHarvestPerSecond(i, s);
			else
				sum+=getGasHarvestPerSecond(i, 4); //TODO
	/*
				harvestedGas+=getGasHarvestPerSecond(i,s);
			}
			else
			{
				setGas(getGas()+getGasHarvestPerSecond(i,4));
				harvestedGas+=getGasHarvestPerSecond(i,4);
			}*/
		}
	}
	return(sum);
}

// --------------------------------------
// ------ END OF HARVEST FUNCTIONS ------
// --------------------------------------


// ------------------------------------
// ----- INITIALIZATION ROUTINES ------ 
// ------------------------------------

/* assigns 'start' and sets a pointer to the corresponding pointer * 
 * in start. This means, whenever the map changes in start, it is also	  *
 * changed for all players. This must be set whenever startCondition, Goal, pStats, start itself or map is changed!  */ 
void PREBUILDORDER::assignStart(START* start)
{
#ifdef _SCC_DEBUG
	if(!start) {
		toLog("DEBUG: (PREBUILDORDER::assignStart): Variable pStart not initialized.");return;
	}
#endif
	pStart = start;

//	Optimierungen...
	
	pMap = pStart->getMap();
	pStartCondition = pStart->getStartCondition();
	setGoal(pStart->getCurrentGoal());
	setpStats(pStart->getpStats());

//	memset(noise, 0, MAX_TIME * sizeof(int));
//	markerCounter = 1;
//	setPlayerNumber(playerNum);
}

/*void PREBUILDORDER::initNoise()
{
//	if(coreCnfiguration.noise>0)
//		for(int j=0;j<MAX_TIME;++j)
//			noise[j]=rand()%coreConfiguration.noise-rand()%coreConfiguration.noise;
//	else TODO
	memset(noise, 0, MAX_TIME * sizeof(int));
}*/

void PREBUILDORDER::resetSpecial()
{
	memset(larvaInProduction, 0, MAX_LOCATIONS * sizeof(int));
}

void PREBUILDORDER::setPlayerNumber(const unsigned int player_number)
{
#ifdef _SCC_DEBUG
	if((player_number < 1) || (player_number > MAX_PLAYER)) {
		toLog("DEBUG: (PREBUILDORDER::setPlayerNumber): Value out of range.");return;
	}
#endif
	playerNum = player_number;
//	location = &((*unit)[playerNum]);
}


void PREBUILDORDER::assignUnits(UNIT (*units)[MAX_INTERNAL_PLAYER][MAX_LOCATIONS])
{
	unit = units;
}

void PREBUILDORDER::initializePlayer()
{
	setMinerals((*pStartCondition)->getMinerals());
	setGas((*pStartCondition)->getGas());
	setTimer(coreConfiguration.getMaxTime()-(*pStartCondition)->getStartTime());
	setNeedSupply((*pStartCondition)->getNeedSupply());
	setHaveSupply((*pStartCondition)->getHaveSupply());
}

void PREBUILDORDER::eraseIllegalCode()
{
	for(unsigned int i=MAX_LENGTH;i--;)
		if(getCode(i) >= getGoal()->getMaxBuildTypes())
			setCode(i, 0);
/*		{
			for(int k=i;k--;)
				Code[k+1]=Code[k];
			Code[0]=0;
		}*/
}

void PREBUILDORDER::eraseUselessCode()
{
	unsigned int allUnits[UNIT_TYPE_COUNT];
	for(int i=UNIT_TYPE_COUNT;i--;)
		allUnits[i]=getLocationTotal(GLOBAL,i);
	for(unsigned int i=MAX_LENGTH;i--;)
	{
		bool ok=true;
		for(unsigned int k=3;k--;)
			ok&=((stats[getGoal()->getRace()][getGoal()->toPhaeno(getCode(i))].prerequisite[k]==0)||
				 (allUnits[stats[getGoal()->getRace()][getGoal()->toPhaeno(getCode(i))].prerequisite[k]]));
//WTF? allUnits mit prerequisite vergleichen!?

//TODO so ganz sauber is des net
		if(!ok)
		{
			for(unsigned int k=i;k--;)
				setCode(k+1, getCode(k));
			setCode(0, 0);
		}
		else
			++allUnits[getGoal()->toPhaeno(getCode(i))];
	}
}

/*CODE::CODE(const unsigned int* code, const unsigned int code_length)
{
	for(int i = MAX_LENGTH-code_length;i<MAX_LENGTH;++i)
		Code[i]=code[i];
	length = code_length;
	
}

const bool CODE::operator==(const CODE& object) const
{
	if(length!=object.length)
		return(false);
	for(int i = MAX_LENGTH-length;i<MAX_LENGTH;++i)
		if(Code[i] != object.Code[i])
			return(false);
	return(true);
}

const bool CODE::operator<(const CODE& object) const
{
	if(length<object.length)
			return(true);
	else if(length>object.length)
			return(false);
	for(int i = MAX_LENGTH-length; i<MAX_LENGTH;++i)
		if(Code[i]>object.Code[i])
			return(true);
	return(false);
}

CODE& CODE::operator=(const unsigned int* code)
{
	for(int i = MAX_LENGTH;i--;)
		Code[i] = code[i];
	return(*this);
}

CODE::~CODE()
{ }

SITUATION::SITUATION()
{
	for(unsigned int i=UNIT_TYPE_COUNT;i--;)
	{
		ttGeno[i]=999;
		tGeno[i]=0;
		buildable[i]=false;
		force[i]=0;
	}
	tMaxBuildTypes = 0;			
}

SITUATION::SITUATION(const SITUATION& object)
{ 
	setTMaxBuildTypes( object.getTMaxBuildTypes() );
	for(int i = UNIT_TYPE_COUNT;i--;)
	{
		setForce(i, object.getForce(i));
		setBuildable(i, object.getBuildable(i));
		setTGeno(i, object.getTGeno(i));
		setTTGeno(i, object.getTTGeno(i));
	}
}

SITUATION::~SITUATION()
{ }

unsigned int SITUATION::getTMaxBuildTypes() const
{
#ifdef _SCC_DEBUG
	if(tMaxBuildTypes >= UNIT_TYPE_COUNT)
	{	
		toLog("DEBUG: (SITUATION::getTMaxBuildTypes): Variable tMaxBuildTypes not initialized.");return(0);
	}
#endif
	return(tMaxBuildTypes);
}

unsigned int SITUATION::getForce(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{	
		toLog("DEBUG: (SITUATION::getForce): Value unit out of range.");return(0);
	}
	if(force[unit] > MAX_SUPPLY)
	{	
		toLog("DEBUG: (SITUATION::getForce): Variable force not initialized.");return(0);
	}
#endif
	return(force[unit]);
}

unsigned int SITUATION::getBuildable(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{	
		toLog("DEBUG: (SITUATION::getBuildable): Value unit out of range.");return(0);
	}
	if(buildable[unit] > MAX_SUPPLY)
	{	
		toLog("DEBUG: (SITUATION::getBuildable): Variable buildable not initialized.");return(0);
	}
#endif
	return(buildable[unit]);
}

unsigned int SITUATION::getTGeno(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{	
		toLog("DEBUG: (SITUATION::getTGeno): Value unit out of range.");return(0);
	}
	if(tGeno[unit] > MAX_SUPPLY)
	{	
		toLog("DEBUG: (SITUATION::getTGeno): Variable tGeno not initialized.");return(0);
	}
#endif
	return(tGeno[unit]);
}

unsigned int SITUATION::getTTGeno(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{	
		toLog("DEBUG: (SITUATION::getTTGeno): Value unit out of range.");return(0);
	}
	if(ttGeno[unit] > 1000)
	{	
		toLog("DEBUG: (SITUATION::getTTGeno): Variable ttGeno not initialized.");return(0);
	}
#endif
	return(ttGeno[unit]);
}

void SITUATION::setTMaxBuildTypes(const unsigned int count)
{
#ifdef _SCC_DEBUG
	if(count >= UNIT_TYPE_COUNT)
	{
		toLog("DEBUG: (SITUATION::setTMaxBuildTypes): Value count out of range.");return;
	}
#endif
	tMaxBuildTypes = count;
}

void SITUATION::addTMaxBuildTypes()
{
#ifdef _SCC_DEBUG
	if(tMaxBuildTypes >= UNIT_TYPE_COUNT-1)
	{
		toLog("DEBUG: (SITUATION::addTMaxBuildTypes): Value out of range.");return;
	}
#endif
	++tMaxBuildTypes;
}

void SITUATION::subTMaxBuildTypes()
{
#ifdef _SCC_DEBUG
	if(tMaxBuildTypes == 0)
	{
		toLog("DEBUG: (SITUATION::subTMaxBuildTypes): Value out of range.");return;
	}
#endif
	--tMaxBuildTypes;
}

void SITUATION::setForce(const unsigned int unit, const unsigned int count)
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{
		toLog("DEBUG: (SITUATION::setForce): Value unit out of range.");return;
	}
	if(count > MAX_SUPPLY)
	{
		toLog("DEBUG: (SITUATION::setForce): Value count out of range.");return;
	}
#endif
	force[unit] = count;
}

void SITUATION::setBuildable(const unsigned int unit, const unsigned int count)
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{
		toLog("DEBUG: (SITUATION::setBuildable): Value unit out of range.");return;
	}
	if(count > 1)
	{
		toLog("DEBUG: (SITUATION::setBuildable): Value count out of range.");return;
	}
#endif
	buildable[unit] = count;
}

void SITUATION::setTGeno(const unsigned int unit, const unsigned int count)
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{
		toLog("DEBUG: (SITUATION::setTGeno): Value unit out of range.");return;
	}
	if(count >= UNIT_TYPE_COUNT)
	{
		toLog("DEBUG: (SITUATION::setTGeno): Value count out of range.");return;
	}
#endif
	tGeno[unit] = count;
}

void SITUATION::setTTGeno(const unsigned int unit, const unsigned int count)
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT)
	{
		toLog("DEBUG: (SITUATION::setTTGeno): Value unit out of range.");return;
	}
	if(count >= 1000)
	{
		toLog("DEBUG: (SITUATION::setTTGeno): Value count out of range.");return;
	}
#endif
	ttGeno[unit] = count;
}*/

#if 0 
// TODO OPTIMIZE
void PREBUILDORDER::mutateGeneCode(const bool* fixed_list)
{
//      return;
// TODO logger machen, welche Mutationsart besonders erfolgreich ist
	if(getLength()==0)
		setLength(MAX_LENGTH);//coreCnfiguration.getMaxLength()); // TODO
	bool useOld=false;
	SITUATION* situation;
	CODE* thisCode = new CODE(Code, getLength());
	map<CODE, SITUATION*>::iterator foundSolution = situationHashMap.find(*thisCode);
	if(foundSolution == situationHashMap.end())
	{
		situation = new SITUATION();

		for(unsigned int i=GAS_SCV+1;i--;)
			if((getGoal()->getIsBuildable(i))&&((*pStartCondition)->getLocationTotal(GLOBAL,i)>0)) //set start force
			{
				situation->setForce(i, (*pStartCondition)->getLocationTotal(GLOBAL,i));
				situation->setBuildable(i, 1);
			}

		for(unsigned int i = GAS_SCV+1; i--;)
		{
			if(situation->getForce(i))
			{
				bool ok=true;
				for(int j=GAS_SCV+1;j--;)
					if((situation->getForce(j))&&((*pStats)[j].create>0)&&((*pStats)[j].create == i)) {
						ok=false;break;
					}
				if(ok)
				{
					situation->setTGeno(situation->getTMaxBuildTypes(), getGoal()->toGeno(i));
					situation->setTTGeno(i, situation->getTMaxBuildTypes());
					situation->addTMaxBuildTypes();
					situation->setBuildable(i, 1);
				}
			} else

			if(  ( (getGoal()->getIsBuildable(i) ) &&
			   (((*pStats)[i].prerequisite[0]==0)||(situation->getForce((*pStats)[i].prerequisite[0])))&&
			   (((*pStats)[i].prerequisite[1]==0)||(situation->getForce((*pStats)[i].prerequisite[1])))&&
			   (((*pStats)[i].prerequisite[2]==0)||(situation->getForce((*pStats)[i].prerequisite[2])))&&
			   ((((*pStats)[i].facility[0]>0)&&(situation->getForce((*pStats)[i].facility[0])))||
			   (((*pStats)[i].facility[1]>0)&&(situation->getForce((*pStats)[i].facility[1])))||
			   (((*pStats)[i].facility[2]>0)&&(situation->getForce((*pStats)[i].facility[2])))||
			   (((*pStats)[i].facility[0]==(*pStats)[i].facility[1]==(*pStats)[i].facility[2]==0)))))
			{
				situation->setTGeno(situation->getTMaxBuildTypes(), getGoal()->toGeno(i));
				situation->setTTGeno(i, situation->getTMaxBuildTypes());
				situation->addTMaxBuildTypes();
				situation->setBuildable(i, 1);
			}
			
			if((getGoal()->getIsStatic(i))&&getGoal()->getIsBuildable(i)&&getGoal()->getAllGoal(i)&&(situation->getForce(i) >= getGoal()->getAllGoal(i))&&(situation->getTTGeno(i) != 999))
			{
//				for(int j=tMaxBuildTypes;j--;)
//					if(tGeno[j] == getGoal()->toGeno(i))
					{
						for(int k = situation->getTTGeno(i); k < situation->getTMaxBuildTypes()-1; ++k)
							situation->setTGeno(k, situation->getTGeno(k+1));
						situation->setTTGeno(i, 999);
						situation->subTMaxBuildTypes();
//						break;
					}
			}
	
		}
		situationHashMap.insert(pair<CODE, SITUATION*>(*thisCode, situation));
		situation->getTMaxBuildTypes();
	}
	else
	{
		situation = foundSolution->second;
		foundSolution->second->getTMaxBuildTypes();
	}
	situation->getTMaxBuildTypes();
	delete thisCode;
	situation->getTMaxBuildTypes();

	for(unsigned int x=MAX_LENGTH;x--;) //length
	{

//alle ueberpruefen, ob die ueberhaupt baubar sind... aber von hinten her!


		if(coreConfiguration.getMutationFactor()==0)
			return;
		if(rand() % (MAX_LENGTH*100/coreConfiguration.getMutationFactor())==0)
		{
			switch(rand() % 4)
			{
				//TODO: wenn generateBuildOrder==1 dann bleibts stehen!
				case 0://delete one variabel entry and move - Mehrere Schmieden/Kasernen etc. zulassen!
				{
//				if(((getGoal()->isVariable[Code[0][x]]==1)&&(getGoal()->isVariable[Code[1][x]]==1))||(!coreConfiguration.preprocessBuildOrder)) TODO
						//TODO: ueberlegen, ob Code evtl struct sein sollte... mmmh

/*					if(x < MAX_LENGTH-1)
					{
						memmove(Code+x+1, Code+x, 4*(MAX_LENGTH-x-2));
						memmove(Marker+x+1, Marker+x, 4*(MAX_LENGTH-x-2));
					}*/
					for(unsigned int i=x;i<MAX_LENGTH-1;++i)
					{
						Code[i] = Code[i+1];
						Marker[i] = Marker[i+1];
					}
					// TODO hier auch das buildable und tMaxBuildTypes rein... irgendwie den Code als "mutier mich" markieren und spaetereinfuegen
//					++markerCounter;Marker[MAX_LENGTH-1] = markerCounter;
					unsigned int y;
//				if(coreConfiguration.preprocessBuildOrder) // TODO
//				while(getGoal()->isVariable[y]==0) y=rand()%getGoal()->getMaxBuildTypes();
//				else
					y = situation->getTGeno(rand()%situation->getTMaxBuildTypes());//getGoal()->getMaxBuildTypes();
					Code[MAX_LENGTH-1]=y;
//				if(coreConfiguration.preprocessBuildOrder) // TODO
//				while(getGoal()->isVariable[y]==0) y=rand()%getGoal()->getMaxBuildTypes();
//				else
//				y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
//				Code[1][MAX_LENGTH-1]=y;
				}break;
				case 1://add one variable entry
				{
					memmove(Code+x+1, Code+x, MAX_LENGTH-x-1);
					memmove(Marker+x+1, Marker+x, MAX_LENGTH-x-1);
					
/*					for(unsigned int i=MAX_LENGTH-1;i>x; --i)
					{
						Code[i]=Code[i-1];
						Marker[i]=Marker[i-1];
					}*/
//					++markerCounter;Marker[x]=markerCounter;
					unsigned int y;
//				if(coreConfiguration.preprocessBuildOrder) TODO
//					while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
//				else
					y=situation->getTGeno(rand()%situation->getTMaxBuildTypes());//getGoal()->getMaxBuildTypes();
					Code[x]=y;
//				if(coreConfiguration.preprocessBuildOrder)
//					while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getGoal()->getMaxBuildTypes();
//				else
				}break;
				case 2://change one entry
				{
//				if(getGoal()->isVariable[Code[k][x]]==1) TODO
					{
						int y;
						//int y=rand()%getGoal()->getMaxBuildTypes();//Optimieren
//					if(coreConfiguration.preprocessBuildOrder) TODO
//						while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getGoal()->getMaxBuildTypes();
//					else
						y=situation->getTGeno(rand()%situation->getTMaxBuildTypes());//getGoal()->getMaxBuildTypes();
						Code[x]=y;
						++markerCounter;Marker[x]=markerCounter;
					}
				}break;
				case 3://exchange two entries
					if(x < MAX_LENGTH)
					{
						//hier erst recht
						unsigned int y=rand()%(MAX_LENGTH-x) + x;
//					y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//					if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH;
						if(situation->getBuildable(getGoal()->toPhaeno(Code[y])))
						{
							unsigned int l;
							l=Code[x];Code[x]=Code[y];Code[y]=l;
							l=Marker[x];Marker[x]=Marker[y];Marker[y]=l;
						}
					}break;
					case 4://move a block of orders  [a..b..ta..tb..c..d] -> [a..ta..tb..b..c..d]
					//~~~TODO bug, marker und code wird nicht richtig verschoben
					if((getLength()>2)&&(x>0))
					{
						unsigned int block_length = rand()%(MAX_LENGTH-x);
						unsigned int target_position = rand()%x;
						if(block_length > 0)
						{       
							unsigned int tmpCode[MAX_LENGTH];
							unsigned int tmpMarker[MAX_LENGTH];
							
							for(unsigned int i = 0; i < block_length; ++i)
							{
								tmpCode[i] = Code[x + i];
								tmpMarker[i] = Marker[x + i];
							}
								
							for(unsigned int i = target_position; i < x; ++i)
							{
								Code[i+block_length] = Code[i];
								Marker[i+block_length] = Marker[i];
							}

							for(unsigned int i = 0; i<block_length; ++i)
							{
								Code[target_position + i] = tmpCode[i];
								Marker[target_position + i] = tmpMarker[i];
							}
						}
					}break; 
				default:
#ifdef _SCC_DEBUG		       
				toLog("ERROR in PREBUILDORDER::mutateGeneCode: rand out of Range");
#endif
				break; // TODO ERROR
			}
		}		       
	
		CODE* theCode = new CODE(Code, getLength());
		map<CODE, SITUATION*>::iterator foundSolution = situationHashMap.find(*theCode);
		if(foundSolution == situationHashMap.end())
		{
			SITUATION* temp = situation;
			situation = new SITUATION(*temp);
			delete temp;

			situation->setForce(getGoal()->toPhaeno(Code[x]), situation->getForce(getGoal()->toPhaeno(Code[x])) + 1);
			if((*pStats)[getGoal()->toPhaeno(Code[x])].create>0)
				situation->setForce((*pStats)[getGoal()->toPhaeno(Code[x])].create, (*pStats)[getGoal()->toPhaeno(Code[x])].create + 1);

		situation->setTMaxBuildTypes(0);
		for(unsigned int i = GAS_SCV+1; i--;)
		{
			if(situation->getForce(i))
			{
				bool ok=true;
				for(int j=GAS_SCV+1; j--;)
					if((situation->getForce(j))&&((*pStats)[j].create>0)&&((*pStats)[j].create == i)) {
						ok=false;break;
					}
				if(ok)
				{
					situation->setTGeno(situation->getTMaxBuildTypes(), getGoal()->toGeno(i));
					situation->setTTGeno(i, situation->getTMaxBuildTypes());
					situation->addTMaxBuildTypes();
					situation->setBuildable(i, 1);
				}
			} else

			if(  ( (getGoal()->getIsBuildable(i) ) &&
			   (((*pStats)[i].prerequisite[0]==0)||(situation->getForce((*pStats)[i].prerequisite[0])))&&
			   (((*pStats)[i].prerequisite[1]==0)||(situation->getForce((*pStats)[i].prerequisite[1])))&&
			   (((*pStats)[i].prerequisite[2]==0)||(situation->getForce((*pStats)[i].prerequisite[2])))&&
			   ((((*pStats)[i].facility[0]>0)&&(situation->getForce((*pStats)[i].facility[0])))||
			   (((*pStats)[i].facility[1]>0)&&(situation->getForce((*pStats)[i].facility[1])))||
			   (((*pStats)[i].facility[2]>0)&&(situation->getForce((*pStats)[i].facility[2])))||
			   (((*pStats)[i].facility[0]==(*pStats)[i].facility[1]==(*pStats)[i].facility[2]==0)))))
			{
				situation->setTGeno(situation->getTMaxBuildTypes(), getGoal()->toGeno(i));
				situation->setTTGeno(i, situation->getTMaxBuildTypes());
				situation->addTMaxBuildTypes();
				situation->setBuildable(i, 1);
			}
			
			if((getGoal()->getIsStatic(i))&&getGoal()->getIsBuildable(i)&&getGoal()->getAllGoal(i)&&(situation->getForce(i)>=getGoal()->getAllGoal(i))&&(situation->getTTGeno(i)!=999))
			{
//				for(int j=tMaxBuildTypes; j--;)
//					if(tGeno[j] == getGoal()->toGeno(i))
					{
						for(int k = situation->getTTGeno(i); k < situation->getTMaxBuildTypes()-1; ++k)
							situation->setTGeno(k, situation->getTGeno(k+1));
						situation->setTTGeno(i, 999);
						situation->subTMaxBuildTypes();
//						break;
					}
			}
	
		}
		
//		situationHashMap.insert(pair<CODE, SITUATION*>(*theCode, situation));
	} 
	else
	{
		situation = foundSolution->second;
	}
	delete theCode;
	
	}			       
}
#endif


// TODO OPTIMIZE
void PREBUILDORDER::mutateGeneCode(/*const bool* fixed_list*/)
{
// TODO logger machen, welche Mutationsart besonders erfolgreich ist
	if(coreConfiguration.getMutationFactor()==0)
		return;
	if(getLength()==0) 
		setLength(MAX_LENGTH);//coreConfiguration.getMaxLength()); // TODO

	bool checked[UNIT_TYPE_COUNT];
	bool buildable[UNIT_TYPE_COUNT];
	unsigned int tMaxBuildTypes = 0;
	unsigned int tGeno[UNIT_TYPE_COUNT]; // !! keine anderen units drueber nehmen!

	NEED need[UNIT_TYPE_COUNT];

	for(unsigned int i = UNIT_TYPE_COUNT; i--;)
	{
		need[i] = getGoal()->need[i];
		checked[i] = false;
		buildable[i] = false;
	}
	
	memset(tGeno, 0, UNIT_TYPE_COUNT*sizeof(int));
 
	ALLOW* allow = getGoal()->allow;

	for(unsigned int i = LAST_UNIT;i--;)
		if(getGoal()->getIsBuildable(i))
		{
			buildable[i]=true;
			tGeno[tMaxBuildTypes]=getGoal()->toGeno(i);
			++tMaxBuildTypes;
			if((*pStartCondition)->getLocationTotal(GLOBAL,i))
			{
				std::list<unsigned int> newBuildable;
				for(std::list<unsigned int>::iterator j = allow[i].facility.begin();j!=allow[i].facility.end(); ++j) 
					if(need[*j].facilityIsDone())
						newBuildable.push_back(*j);
				for(std::list<unsigned int>::iterator j = allow[i].facility2.begin();j!=allow[i].facility2.end(); ++j) 
					if(need[*j].facility2IsDone())
						newBuildable.push_back(*j);
				for(std::list<unsigned int>::iterator j = allow[i].prerequisite.begin();j!=allow[i].prerequisite.end(); ++j) 
					if(need[*j].prerequisiteIsDone())
						newBuildable.push_back(*j);
				checked[i]=true;
				for(std::list<unsigned int>::iterator j = newBuildable.begin();j!=newBuildable.end(); ++j)
				{
					buildable[*j]=true;
					tGeno[tMaxBuildTypes]=getGoal()->toGeno(*j);
					tMaxBuildTypes++;
				}
			}
		}
		
	for(unsigned int x=MAX_LENGTH-1;x>MAX_LENGTH-getLength(); --x) //length
	{
// IS_LOST ETC!!!
		if(rand() % (MAX_LENGTH*100/coreConfiguration.getMutationFactor())==0)
		{
			switch(rand() % 4)
			{
				//TODO: wenn generateBuildOrder==1 dann bleibts stehen!
				case 0://delete one variabel entry and move - Mehrere Schmieden/Kasernen etc. zulassen!
				{
//				if(((getGoal()->isVariable[Code[0][x]]==1)&&(getGoal()->isVariable[Code[1][x]]==1))||(!coreConfiguration.preprocessBuildOrder)) TODO
						//TODO: ueberlegen, ob Code evtl struct sein sollte... mmmh

/*					if(x < MAX_LENGTH-1)
					{
						memmove(Code+x+1, Code+x, 4*(MAX_LENGTH-x-2));
						memmove(Marker+x+1, Marker+x, 4*(MAX_LENGTH-x-2));
					}*/
					for(unsigned int i=x;i<MAX_LENGTH-1; ++i)
					{
						Code[i] = Code[i+1];
//						Marker[i] = Marker[i+1];
					}
					// TODO hier auch das buildable und tMaxBuildTypes rein... irgendwie den Code als "mutier mich" markieren und spaetereinfuegen
//					++markerCounter;Marker[MAX_LENGTH-1] = markerCounter;
					unsigned int y;
//				if(coreConfiguration.preprocessBuildOrder) // TODO
//				while(getGoal()->isVariable[y]==0) y=rand()%getGoal()->getMaxBuildTypes();
//				else
					y = tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
					Code[MAX_LENGTH-1]=y;
//				if(coreConfiguration.preprocessBuildOrder) // TODO
//				while(getGoal()->isVariable[y]==0) y=rand()%getGoal()->getMaxBuildTypes();
//				else
//				y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
//				Code[1][MAX_LENGTH-1]=y;
				}break;
				case 1://add one variable entry
				{
					memmove(Code+x+1, Code+x, (MAX_LENGTH-x-1) * sizeof(int));
//					memmove(Marker+x+1, Marker+x, MAX_LENGTH-x-1);
					
/*					for(unsigned int i=MAX_LENGTH-1; i>x; --i)
					{
						Code[i]=Code[i-1];
						Marker[i]=Marker[i-1];
					}*/
//					++markerCounter;Marker[x]=markerCounter;
					unsigned int y;
//				if(coreConfiguration.preprocessBuildOrder) TODO
//					while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
//				else
					y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
					Code[x]=y;
//				if(coreConfiguration.preprocessBuildOrder)
//					while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getGoal()->getMaxBuildTypes();
//				else
				}break;
				case 2://change one entry
				{
//				if(getGoal()->isVariable[Code[k][x]]==1) TODO
					{
						int y;
						//int y=rand()%getGoal()->getMaxBuildTypes();//Optimieren
//					if(coreConfiguration.preprocessBuildOrder) TODO
//						while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getGoal()->getMaxBuildTypes();
//					else
						y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
						Code[x]=y;
//						++markerCounter;Marker[x]=markerCounter;
					}
				}break;
				case 3://exchange two entries
					if(x < MAX_LENGTH)
					{
						//hier erst recht
						unsigned int y=rand()%(MAX_LENGTH-x) + x;
//					y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//					if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH;
						if(buildable[getGoal()->toPhaeno(Code[y])])
						{
							unsigned int l;
							l=Code[x];Code[x]=Code[y];Code[y]=l;
//							l=Marker[x];Marker[x]=Marker[y];Marker[y]=l;
						}
					}break;
				case 4://move a block of orders  [a..b..ta..tb..c..d] -> [a..ta..tb..b..c..d]
					//~~~TODO bug, marker und code wird nicht richtig verschoben
					if((getLength()>2)&&(x>0))
					{
						unsigned int block_length = rand()%(MAX_LENGTH-x);
						unsigned int target_position = rand()%x;
						if(block_length > 0)
						{       
							unsigned int tmpCode[MAX_LENGTH];
//							unsigned int tmpMarker[MAX_LENGTH];
							
							for(unsigned int i = 0; i < block_length; ++i)
							{
								tmpCode[i] = Code[x + i];
//								tmpMarker[i] = Marker[x + i];
							}
								
							for(unsigned int i = target_position; i < x; ++i)
							{
								Code[i+block_length] = Code[i];
//								Marker[i+block_length] = Marker[i];
							}

							for(unsigned int i = 0; i<block_length; ++i)
							{
								Code[target_position + i] = tmpCode[i];
//								Marker[target_position + i] = tmpMarker[i];
							}
						}
					}break; 
				default:
#ifdef _SCC_DEBUG		       
				toLog("ERROR in PREBUILDORDER::mutateGeneCode: rand out of Range");
#endif
				break; // TODO ERROR
			}
		}
			int i = getGoal()->toPhaeno(getCode(x));
			if(!checked[i])
			{
				{
					std::list<unsigned int> newBuildable;
					for(std::list<unsigned int>::iterator j = allow[i].facility.begin();j!=allow[i].facility.end(); ++j)
					if(need[*j].facilityIsDone())
						newBuildable.push_back(*j);
					for(std::list<unsigned int>::iterator j = allow[i].facility2.begin();j!=allow[i].facility2.end(); ++j)
					if(need[*j].facility2IsDone())
						newBuildable.push_back(*j);
					for(std::list<unsigned int>::iterator j = allow[i].prerequisite.begin();j!=allow[i].prerequisite.end(); ++j)
					if(need[*j].prerequisiteIsDone())
						newBuildable.push_back(*j);
					checked[i]=true;
					if(getGoal()->getIsStatic(i))
					// remove
						for(unsigned int j = tMaxBuildTypes;j--;)
							if(tGeno[j] == getGoal()->toGeno(i))
							{
								--tMaxBuildTypes;
								for(unsigned int k = j;k<tMaxBuildTypes; ++k)
									tGeno[k] = tGeno[k+1];
							}
			
					for(std::list<unsigned int>::iterator j = newBuildable.begin();j!=newBuildable.end(); ++j) 
					{
						buildable[*j]=true;
						tGeno[tMaxBuildTypes]=getGoal()->toGeno(*j);
						++tMaxBuildTypes;
					}
				}
				if((*pStats)[i].create>0)
				{
					i = (*pStats)[i].create;
					if(!checked[i])
					{
						std::list<unsigned int> newBuildable;
			       			for(std::list<unsigned int>::iterator j = allow[i].facility.begin();j!=allow[i].facility.end(); ++j)
							if(need[*j].facilityIsDone())
								newBuildable.push_back(*j);
						for(std::list<unsigned int>::iterator j = allow[i].facility2.begin();j!=allow[i].facility2.end(); ++j)
							if(need[*j].facility2IsDone())
								newBuildable.push_back(*j);
						for(std::list<unsigned int>::iterator j = allow[i].prerequisite.begin();j!=allow[i].prerequisite.end(); ++j)
							if(need[*j].prerequisiteIsDone())
								newBuildable.push_back(*j);
				       		checked[i]=true;
						for(std::list<unsigned int>::iterator j = newBuildable.begin();j!=newBuildable.end(); ++j)
						{
							buildable[*j]=true;
							tGeno[tMaxBuildTypes]=getGoal()->toGeno(*j);
							++tMaxBuildTypes;
						}
					}
				}
			}
	
	}
#if 0
//alle ueberpruefen, ob die ueberhaupt baubar sind... aber von hinten her!
//		if(fixed_list[MAX_LENGTH-x])
//			continue;
		if(rand() % (MAX_LENGTH*100/coreConfiguration.getMutationFactor())==0)
		{
			switch(rand() % 2)
			{
				//TODO: wenn generateBuildOrder==1 dann bleibts stehen!
				case 0://delete one variabel entry and move - Mehrere Schmieden/Kasernen etc. zulassen!
				{
//				  if(((getGoal()->isVariable[Code[0][x]]==1)&&(getGoal()->isVariable[Code[1][x]]==1))||(!coreConfiguration.preprocessBuildOrder)) TODO
						//TODO: ueberlegen, ob Code evtl struct sein sollte... mmmh

					int i = x;
					while(i < MAX_LENGTH-1)
					{
						while((i<MAX_LENGTH-1) && (fixed_list[i]))
							++i;
						if(i<MAX_LENGTH-1)
						{
							int j = i+1;
							while((j<MAX_LENGTH)&&(fixed_list[j]))
								++j;
							if((j<MAX_LENGTH)&&(!fixed_list[j]))
							{
								setCode(i, getCode(j));
								setMarker(i, getMarker(j));
							}
						}
						if(!fixed_list[i])
							++i;
					}
					// TODO hier auch das buildable und tMaxBuildTypes rein... irgendwie den Code als "mutier mich" markieren und spaetereinfuegen
					++markerCounter;setMarker(MAX_LENGTH-1, markerCounter);
					unsigned int y;
//				  if(coreConfiguration.preprocessBuildOrder) // TODO
//				  while(getGoal()->isVariable[y]==0) y=rand()%getGoal()->getMaxBuildTypes();
//				  else
					y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
					setCode(MAX_LENGTH-1, y);
//				  if(coreConfiguration.preprocessBuildOrder) // TODO
//				  while(getGoal()->isVariable[y]==0) y=rand()%getGoal()->getMaxBuildTypes();
//				  else
//				  y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
//				  Code[1][MAX_LENGTH-1]=y;
				}break;
				case 1://add one variable entry
				{
					int i = MAX_LENGTH-1;
					while(i>x)
					{
						while((i>x) && (fixed_list[i]))
							--i;
						if(i>x)
						{
							int j=i-1;
							while((j) && (fixed_list[j]))
								--j;
							if(!fixed_list[j])
							{
								setCode(i, getCode(j));
								setMarker(i, getMarker(j));
							}
						}
						if(!fixed_list[i])
							--i;
					}
					++markerCounter;setMarker(x, markerCounter);
					unsigned int y;
//				  if(coreConfiguration.preprocessBuildOrder) TODO
//					  while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
//				  else
					y = tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
					setCode(x, y);
//				  if(coreConfiguration.preprocessBuildOrder)
//					  while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getGoal()->getMaxBuildTypes();
//				  else
				}break;
				case 2://change one entry
				{
//				  if(getGoal()->isVariable[Code[k][x]]==1) TODO
					{
						int y;
						//int y=rand()%getGoal()->getMaxBuildTypes();//Optimieren
//					  if(coreConfiguration.preprocessBuildOrder) TODO
//						  while(getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getGoal()->getMaxBuildTypes();
//					  else
						y=tGeno[rand()%tMaxBuildTypes];//getGoal()->getMaxBuildTypes();
						setCode(x, y);
						++markerCounter;setMarker(x, markerCounter);
					}
				}break;
				case 3://exchange two entries
					if(x < MAX_LENGTH)
					{
						//hier erst recht
						unsigned int y=rand()%(MAX_LENGTH-x) + x;
//					  y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//					  if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH;
						if((buildable[getGoal()->toPhaeno(getCode(y))])&&(!fixed_list[y]))
						{
							unsigned int l;
							l=getCode(x);setCode(x, getCode(y));setCode(y, l);
							l=getMarker(x);setMarker(x, getMarker(y));setMarker(y, l);
						}
					}break;
					case 4://move a block of orders  [a..b..ta..tb..c..d] -> [a..ta..tb..b..c..d]
					//~~~TODO bug, marker und code wird nicht richtig verschoben
					if((getLength()>2)&&(x>0))
					{
						unsigned int block_length = rand()%(MAX_LENGTH-x);
						unsigned int target_position = rand()%x;
						if(block_length > 0)
						{
							unsigned int tmpCode[MAX_LENGTH];
							unsigned int tmpMarker[MAX_LENGTH];
							
							for(unsigned int i = 0; i < block_length; ++i)
							{
								tmpCode[i] = getCode(x + i);
								tmpMarker[i] = getMarker(x + i);
							}							
								
							for(unsigned int i = target_position; i < x; ++i)
							{
								setCode(i+block_length, getCode(i));
								setMarker(i+block_length, getMarker(i));
							}

							for(unsigned int i = 0; i<block_length; ++i)
							{
								setCode(target_position + i, tmpCode[i]);
								setMarker(target_position + i, tmpMarker[i]);
							}
						}
					}break;
				default:
#ifdef _SCC_DEBUG
				toLog("ERROR in PREBUILDORDER::mutateGeneCode: rand out of Range");
#endif
				break; // TODO ERROR
			}
		}
	}
#endif
}




//Reinitialize programs with random orders
void PREBUILDORDER::resetGeneCode()
{
//	mutationRate=500+rand()%1000;
/*	if((coreConfiguration.preprocessBuildOrder)&&(basicLength>0))
	{
		memcpy(Code,basicBuildOrder,MAX_LENGTH*4);
		for(int i=MAX_LENGTH;i--;)
		{
			++markerCounter;Marker[i]=markerCounter;
		}
	}
	else*/
//	{
//		unsigned int y=0;
/*		switch(getGoal()->getRace())
		{
			case TERRA:y=SUPPLY_DEPOT;break;
			case PROTOSS:y=PYLON;break;
			case ZERG:y=OVERLORD;break;
			default:break; // TODO ERROR
		}*/
		memset(Code, 0, MAX_LENGTH * sizeof(int));
//		for(int i=MAX_LENGTH;i--;)
//		{
//			if((i+4)%stats[getGoal()->getRace()][y].needSupply==0)
//			{
//				setCode(i, 0);//rand()%getGoal()->getMaxBuildTypes());
				
//			} else
//			{
//				Code[0][i]=/*rand()%*/getGoal()->toGeno(SCV);//getMaxBuildTypes();
//				Code[1][i]=/*rand()%*/getGoal()->toGeno(SCV);//getMaxBuildTypes();
//			}
//			++markerCounter;setMarker(i, markerCounter);
//		}
//	}
}

#if 0
void PREBUILDORDER::crossOver(PREBUILDORDER* parent2, PREBUILDORDER* child1, PREBUILDORDER* child2)
{
/*	int counter=MAX_LENGTH;
	for(int i=0;i<MAX_LENGTH; ++i)
	{
		if(rand()%counter<5)
		{
			int num=MAX_LENGTH-counter;
			memcpy(&child1->Code[0][i-num],&Code[0][i-num],num*4);
			memcpy(&child1->Marker[0][i-num],&Marker[0][i-num],num*4);

			memcpy(&child1->Code[1][i-num],&parent2->Code[1][i-num],num*4);
			memcpy(&child1->Marker[1][i-num],&parent2->Marker[1][i-num],num*4);

			memcpy(&child2->Code[1][i-num],&Code[1][i-num],num*4);
			memcpy(&child2->Marker[1][i-num],&Marker[1][i-num],num*4);

			memcpy(&child2->Code[0][i-num],&parent2->Code[0][i-num],num*4);
			memcpy(&child2->Marker[0][i-num],&parent2->Marker[0][i-num],num*4);

			counter=MAX_LENGTH; //~~ TODO
			RACE* c=child1;
			child1=child2;
			child2=c;
		}
		--counter;
	}
	int num=MAX_LENGTH-counter;
	memcpy(&child1->Code[0][counter],&Code[0][counter],num*4);
	memcpy(&child1->Marker[0][counter],&Marker[0][counter],num*4);

	memcpy(&child1->Code[1][counter],&parent2->Code[1][counter],num*4);
	memcpy(&child1->Marker[1][counter],&parent2->Marker[1][counter],num*4);

	memcpy(&child2->Code[1][counter],&Code[1][counter],num*4);
	memcpy(&child2->Marker[1][counter],&Marker[1][counter],num*4);

	memcpy(&child2->Code[0][counter],&parent2->Code[0][counter],num*4);
	memcpy(&child2->Marker[0][counter],&parent2->Marker[0][counter],num*4);

	child1->mutationRate=(2*mutationRate+parent2->mutationRate)/3;
	child2->mutationRate=(2*parent2->mutationRate+mutationRate)/3;*/
}
#endif


// -------------------------------------------------------------------
// ------ BELOW ALL THE GET/SET FUNCTIONS, PRETTY UNINTERESTING ------
// -------------------------------------------------------------------

void PREBUILDORDER::copyCode(PREBUILDORDER& player)
{
	memcpy(Code, player.Code, sizeof(int) * MAX_LENGTH);
}

void PREBUILDORDER::copyCode(unsigned int* dst) const
{
	memcpy(dst, Code, MAX_LENGTH * sizeof(int));
}


// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

//START* PREBUILDORDER::pStart;
//const BASIC_MAP* const* PREBUILDORDER::pMap;

//signed int PREBUILDORDER::noise[MAX_TIME];
//unsigned int PREBUILDORDER::markerCounter;

//map<CODE, SITUATION*> PREBUILDORDER::situationHashMap;


