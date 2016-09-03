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
	pMap(NULL),
//	location(0),
//	pStartCondition(NULL),
	buildingQueue(), // TODO
	parallelCommandQueues(),
//	lastcounter(0),
//	lastunit(0),
	unit(NULL),
	pStats(NULL),
	neededMinerals(0),
	neededGas(0),
	pStart(NULL),
	ready(false),
	pGoal(NULL),
	alwaysBuildWorkers(false),
	onlySwapOrders(false),
	conditionsChanged(true),

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
	conditionsChanged = true;
//	pStartCondition = NULL;
	while(!buildingQueue.empty())
		buildingQueue.pop();
	unit = NULL;
	pStats = NULL;
	neededMinerals = 0;
	neededGas = 0;
	pStart = NULL;
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
	// pMap == NULL?
	memset(mineralHarvestPerSecond, 0, 45 * MAX_LOCATIONS*sizeof(int));
	memset(gasHarvestPerSecond, 0, 5 * MAX_LOCATIONS*sizeof(int));
	memset(Code, 0, MAX_LENGTH*sizeof(int));
//	memset(Marker, 0, MAX_LENGTH*sizeof(int));
	resetSpecial();
}

PREBUILDORDER::PREBUILDORDER(const PREBUILDORDER& object) :
//	location(object.location),
//	pStartCondition(object.pStartCondition),
	buildingQueue(object.buildingQueue),
//	lastcounter(object.lastcounter),
//	lastunit(object.lastunit),
	unit(object.unit),
	pStats(object.pStats),
	neededMinerals(object.neededMinerals),
	neededGas(object.neededGas),
	pStart(object.pStart),
	ready(object.ready),
	pGoal(object.pGoal),
	alwaysBuildWorkers(object.alwaysBuildWorkers),
	onlySwapOrders(object.onlySwapOrders),
	conditionsChanged(true),
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
	memcpy(mineralHarvestPerSecond, object.mineralHarvestPerSecond, MAX_LOCATIONS * 45 * sizeof(int));
	memcpy(gasHarvestPerSecond, object.gasHarvestPerSecond, MAX_LOCATIONS * 5 * sizeof(int));
}

PREBUILDORDER& PREBUILDORDER::operator=(const PREBUILDORDER& object)
{
//	location = object.location;
//	pStartCondition = object.pStartCondition;
	buildingQueue = object.buildingQueue;
//	lastcounter = object.lastcounter;
  //  lastunit = object.lastunit;
	unit = object.unit;
	pStats = object.pStats;
	neededMinerals = object.neededMinerals;
	neededGas = object.neededGas;
	pStart = object.pStart;
	ready = object.ready;
	pGoal = object.pGoal;
	
	alwaysBuildWorkers = object.alwaysBuildWorkers;
	onlySwapOrders = object.onlySwapOrders;
	conditionsChanged = true;

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

const bool PREBUILDORDER::setAlwaysBuildWorkers(const bool always_build_workers)
{
	if(alwaysBuildWorkers == always_build_workers)
		return(false);
	conditionsChanged = true;
	alwaysBuildWorkers = always_build_workers;
	return(true);
}

const bool PREBUILDORDER::setOnlySwapOrders(const bool only_swap_orders)
{
	if(onlySwapOrders == only_swap_orders)
		return(false);
	conditionsChanged = true;
	onlySwapOrders = only_swap_orders;
	return(true);
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

	if(isAlwaysBuildWorkers())
	{
		PARALLEL_COMMAND* gimme = new PARALLEL_COMMAND();
		gimme->unit = SCV;
		gimme->count = 64;
		parallelCommandQueues.push_back(gimme);
	}
								
	setHarvestedGas(0);setHarvestedMinerals(0);
	setWastedGas(0);setWastedMinerals(0);

	setMinerals((*(getStartCondition()))->getMinerals());
	setGas((*(getStartCondition()))->getGas());
	setTimer(coreConfiguration.getMaxTime()-(*(getStartCondition()))->getStartTime());
	setNeedSupply((*(getStartCondition()))->getNeedSupply());
	setHaveSupply((*(getStartCondition()))->getHaveSupply());
	
	const UNIT* start_units = (*(getStartCondition()))->getUnit(0);
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

	if(getRace() == ZERG)
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

const bool PREBUILDORDER::replaceCode(const unsigned int ip, const unsigned int code)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::replaceCode): Value ip out of range.");return(false);
	}
	if(code >= getGoal()->getMaxBuildTypes()) {
		toLog("DEBUG: (PREBUILDORDER::replaceCode): Value code out of range.");return(false);
	}
#endif
	bool changed_bo = false;
	if(getCode(ip)!=code)
		changed_bo=true;
	setCode(ip, code);
	return(changed_bo);
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
	// TODO count?
{
	switch(facilityType)
	{
		case NO_FACILITY:break;
		case IS_LOST:
		case IS_MORPHING:
/*			if(stats[SPECIAL_UNIT_TYPE) // TODO
			{
			
				if(facility)
					setLocationTotal(location_number, facility, getLocationTotal(location_number, facility) - count);
//				if(facility2)
//					removeOneLocationTotal(location_number, facility2);
			} else*/ // TODO!!!
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
	if(!is_ready)
	{
		unsigned int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; 
		memset(bonus, 0, UNIT_TYPE_COUNT * MAX_LOCATIONS * sizeof(int));

		tpF = getGoal()->calculateFitness((*unit)[playerNum], bonus);
		
		std::priority_queue<Building, std::vector<Building> >  save = buildingQueue;
		while(!buildingQueue.empty())
		{
			const Building& build = buildingQueue.top();
			if(bonus[build.getLocation()][build.getType()] > 0)
			{
				tpF+=(build.getTotalBuildTime()+build.getBuildFinishedTime())*100/bonus[build.getLocation()][build.getType()];
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
//	pStartCondition = getStartCondition();
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
	setMinerals((*(getStartCondition()))->getMinerals());
	setGas((*(getStartCondition()))->getGas());
	setTimer(coreConfiguration.getMaxTime()-(*(getStartCondition()))->getStartTime());
	setNeedSupply((*(getStartCondition()))->getNeedSupply());
	setHaveSupply((*(getStartCondition()))->getHaveSupply());
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

/*void PREBUILDORDER::assignStartCondition(const START_CONDITION* start_condition, const bool neutral_player) 
{
	pStart->assignStartCondition(start_condition);
	if(neutral_player)
		pStart->fillAsNeutralPlayer();
	else
		pStart->fillAsActivePlayer();
}*/

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

// TODO OPTIMIZE
void PREBUILDORDER::mutateGeneCode()
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
			if((*(getStartCondition()))->getLocationTotal(GLOBAL,i))
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
#ifdef _SCC_DEBUG
	if(tMaxBuildTypes <= 1)
	{
		toLog("ERROR: PREBUILDORDER::mutateGeneCode(): No units can be build with current configuration!");return;
	}
#endif
		
	for(unsigned int x=MAX_LENGTH-1;x>MAX_LENGTH-getLength(); --x) //length
	{
// IS_LOST ETC!!!
		if(rand() % (MAX_LENGTH*100/coreConfiguration.getMutationFactor())==0)
		{
			int new_item;
			if(isAlwaysBuildWorkers())
				new_item = tGeno[(rand()%(tMaxBuildTypes-1))+1];
			else 
				new_item = tGeno[rand()%tMaxBuildTypes];
			int random=3;
			if(!isOnlySwapOrders())
				random = rand()%4;
			switch(random)
			{
				case 0://remove
				{
					for(unsigned int i=x;i<MAX_LENGTH-1; ++i)
						Code[i] = Code[i+1];
					Code[MAX_LENGTH-1] = new_item;
				}break;
				case 1://add
				{
					memmove(Code+x+1, Code+x, (MAX_LENGTH-x-1) * sizeof(int));
					Code[x] = new_item;
				}break;
				case 2://change one entry
				{
					Code[x] = new_item;
				}break;
				case 3://exchange two entries
					if(x < MAX_LENGTH)
					{
						unsigned int y=rand()%(MAX_LENGTH-x) + x;
						if(buildable[getGoal()->toPhaeno(Code[y])])
						{
							unsigned int l;
							l=Code[x];Code[x]=Code[y];Code[y]=l;
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
							for(unsigned int i = 0; i < block_length; ++i)
								tmpCode[i] = Code[x + i];
							for(unsigned int i = target_position; i < x; ++i)
								Code[i+block_length] = Code[i];
							for(unsigned int i = 0; i<block_length; ++i)
								Code[target_position + i] = tmpCode[i];
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
}

#include <sstream>
const bool PREBUILDORDER::checkForLarva(const unsigned int current_location_window)
{	
		// Larva wird benoetigt zum Bau? Fein, dann bauen wir eine neue Larva falls nicht schon alle hatcheries etc. belegt sidn
				// Gesamtzahl der Larven < 3 * HATCHERY?
// TODO: Ueberpruefen: Eine Art Defizit einfuehren: Wenn eine Brutstaette voll ausgelastet ist und anschliessend eine neue fertig wird, wird dann ploetzlich schneller produziert?

// Falls mehr Produktionsstaetten existieren als Larven in Produktion und Larven auf dem Feld sind...
	
	if(((   getLocationTotal(current_location_window, HATCHERY)+
		getLocationTotal(current_location_window, LAIR)+
		getLocationTotal(current_location_window, HIVE)) *3 > 
		(larvaInProduction[current_location_window]+getLocationTotal(current_location_window, LARVA)))  &&

// ... und mehr Produktionsstaetten als Larven in Produktion existieren			
 		((getLocationTotal(current_location_window, HATCHERY)+
		 getLocationTotal(current_location_window, LAIR)+
		 getLocationTotal(current_location_window, HIVE) > 
		  larvaInProduction[current_location_window]))) // => zuwenig Larven da!
// ... dann baue eine Larve
	{
		return true;
	} else return false;
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

void PREBUILDORDER::setConditionsChanged(const bool conditions_changed) 
{
	conditionsChanged = conditions_changed;
}


// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

//START* PREBUILDORDER::pStart;
//const BASIC_MAP* const* PREBUILDORDER::pMap;

//signed int PREBUILDORDER::noise[MAX_TIME];
//unsigned int PREBUILDORDER::markerCounter;

//map<CODE, SITUATION*> PREBUILDORDER::situationHashMap;


