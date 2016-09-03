#include "prerace.hpp"

/* PRERACE.CPP - last changed: 6/23/04								*
 * Author: Clemens Lode												*
 * Copyright: Clemens Lode											*
 *																	*
 *  PURPPOSE OF THIS FILE:										    *
 *		Supply various functions for the classes RACE and ANARACE,  *
 * 		mainly concerning harvesting, supply and units				*/


PRERACE::PRERACE():
	location(0),
	pStartCondition(0),
	buildingQueue(), // TODO
//	lastcounter(0),
//	lastunit(0),
	pStats(0),
	neededMinerals(0),
	neededGas(0),
	ready(false),
	pGoal(0),
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
	for(int i=MAX_LOCATIONS;i--;)
	{
		for(int j=45;j--;)
			setMineralHarvestPerSecond(i,j,0);
		for(int j=5;j--;)
			setGasHarvestPerSecond(i,j,0);
	}
/*	for(int i=MAX_LENGTH;i--;)
	{
		Code[i]=0;
		last[i].unit=0;
		last[i].location=0;
		last[i].count=0;
	}*/
	resetSpecial();
}

PRERACE::PRERACE(const PRERACE& object) :
	location(object.location),
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
	for(int i=MAX_LENGTH;i--;)
	{
	//	last[i] = object.last[i];
		Code[i] = object.Code[i];
		Marker[i] = object.Marker[i];
	}
		
	for(int i=MAX_LOCATIONS;i--;)
	{
		larvaInProduction[i] = object.larvaInProduction[i];
		for(int j = 0; j<45; j++)
			mineralHarvestPerSecond[i][j] = object.mineralHarvestPerSecond[i][j];
		for(int j = 0; j<5; j++)
			gasHarvestPerSecond[i][j] = object.gasHarvestPerSecond[i][j];
	}	
	
}

PRERACE& PRERACE::operator=(const PRERACE& object)
{
    location = object.location;
    pStartCondition = object.pStartCondition;
    buildingQueue = object.buildingQueue;
//    lastcounter = object.lastcounter;
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
    for(int i=MAX_LENGTH;i--;)
    {
//        last[i] = object.last[i];
        Code[i] = object.Code[i];
        Marker[i] = object.Marker[i];
    }
                                                                                                                                                            
    for(int i=MAX_LOCATIONS;i--;)
    {
        larvaInProduction[i] = object.larvaInProduction[i];
        for(int j = 0; j<45; j++)
            mineralHarvestPerSecond[i][j] = object.mineralHarvestPerSecond[i][j];
        for(int j = 0; j<5; j++)
            gasHarvestPerSecond[i][j] = object.gasHarvestPerSecond[i][j];
    }
	return(*this);
}


void PRERACE::prepareForNewGeneration()
{
    setHarvestedGas(0);setHarvestedMinerals(0);
    setWastedGas(0);setWastedMinerals(0);

    setMinerals((*pStartCondition)->getMinerals());
    setGas((*pStartCondition)->getGas());
    setTimer(configuration.getMaxTime()-(*pStartCondition)->getStartTime());
    setNeedSupply((*pStartCondition)->getNeedSupply());
    setHaveSupply((*pStartCondition)->getHaveSupply());
    
	const UNIT* start_units = (*pStartCondition)->getUnit(0);
//	int j = 0;
	for(int i=UNIT_TYPE_COUNT;i--;)
		if(start_units->getTotal(i))
		{
			(*pGoal)->calculateFinalTimes(0, i, start_units->getTotal(i), 0);
/*			last[j].location = 1;
			last[j].unit = i;
			last[j].count = 1;
			last[j].
			TODO liste oder Vektor*/
		}
		
	
/*	for(int i=4;i--;) //TODO
    {
        last[i].location=1;
        last[i].unit=SCV;
        last[i].count=1;
    }
    for(int i=4;i<MAX_LENGTH;i++)
    {
        last[i].location=0;
        last[i].unit=0;
        last[i].count=0;
    }
    lastcounter=4;*/
    setTimeOut(configuration.getMaxTimeOut());
    setIP(configuration.getMaxLength()-1);
    ready=false;

    resetSpecial();
}                                                                                                                                                            

PRERACE::~PRERACE()
{ }

const eRace PRERACE::getRace() const
{
	return(pStart->getPlayerRace(playerNum));
}

// -----------------------------------
// ------ CONTROLS FROM OUTSIDE ------
// -----------------------------------

void PRERACE::replaceCode(const unsigned int ip, const unsigned int code)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::replaceCode): Value ip out of range.");return;
	}
	if(code >= (*pGoal)->getMaxBuildTypes()) {
		toLog("DEBUG: (PRERACE::replaceCode): Value code out of range.");return;
	}
#endif
	Code[ip] = code;
	markerCounter++;
	Marker[ip] = markerCounter;
}

// ------------------------------------------
// ------ END OF CONTROLS FROM OUTSIDE ------
// ------------------------------------------



// --------------------------------------------------------------
// ------ FUNCTIONS THAT ARE PRIMARILY USED DURING THE RUN ------
// --------------------------------------------------------------

const unsigned int PRERACE::calculateIdleTime() const
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
			toLog("ERROR: PRERACE::calculateIdleTime(): FinishedTime > getTimer...");
		}
#endif
        unsigned int buildingRemaining = getTimer() - buildingQueue.top().getBuildFinishedTime();
		// TODO
        if(idle_time > buildingRemaining) 
			idle_time = buildingRemaining;
    }

    if( harvest_gas )
//	  ( harvest_minerals || 
  //    	(neededGas + harvest_gas - neededGas % harvest_gas)/harvestGas() < (neededMinerals+(harvest_minerals - neededMinerals % harvest_minerals))/harvest_minerals)) 
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
void PRERACE::adjustLocationUnitsAfterCompletion(const unsigned int location_number, const eFacilityType facilityType, const unsigned int facility, const unsigned int facility2)
{
#ifdef _SCC_DEBUG
    if((location_number >= MAX_LOCATIONS))  {
        toLog("DEBUG: (PRERACE::adjustLocationUnitsAfterCompletion): Value location_number out of range.");return;
    }
// TODO andere facilities!!	
#endif

				
	switch(facilityType)
	{
		case NO_FACILITY:break;
		case IS_LOST:
			if(facility)
				removeOneLocationTotal(location_number, facility);
				//availible was already taken account when starting the building
			if(facility2)
				removeOneLocationTotal(location_number, facility2);
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

void PRERACE::adjustAvailibility(const unsigned int loc, const unsigned int fac, const UNIT_STATISTICS* stat)
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
const bool PRERACE::calculateReady() const
{
	return((*pGoal)->calculateReady(location));
}

// nicht const da buildingqueue verzehrt wird :/
// TODO OPTIMIZE
const unsigned int PRERACE::calculatePrimaryFitness(const bool is_ready)
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
		for(int i=MAX_LOCATIONS;i--;)
			for(int j=UNIT_TYPE_COUNT;j--;)
				bonus[i][j]=0;
// ------ END BONUSSYSTEM INIT ------

//				bool first=true;
		for(std::list<GOAL>::const_iterator i = (*pGoal)->goal.begin(); i!= (*pGoal)->goal.end(); i++)
//		while((*pGoal)->getNextGoal(i, first))
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
						while((j<MAX_LOCATIONS) && (bon>getLocationTotal(loc, i->getUnit())))
						{
							sumup+=getLocationTotal(loc, i->getUnit())*(100-(*pMap)->getLocation(loc)->getDistance(i->getLocation())); //was pMap->location[j]...
							bon-=getLocationTotal(loc, i->getUnit());
							j++;
						}
						// Falls j<MAX_LOCATIONS => unser "Bon" wurde schon vorher aufgebraucht => An dieser Stelle j den Rest draufgeben... 
						if(j<MAX_LOCATIONS)
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
//						  setpFitness(getpFitness()+(i->getTime()*sumup)/(i->getCount()*configuration.maxTime));
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
    	std::priority_queue<Building, std::deque<Building> >  save = buildingQueue;
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

/*				if(((*pGoal)->goal[build.getType(i)].time>0)&&(getLocationTotal(build.getLocation(i),build.getType(i))==0))
					pFitness+=(build.getRemainingBuildTime(i)*100*(*pGoal)->goal[build.getType(i)].time*getLocationTotal(0,i))/((*pGoal)->goal[build.getType(i)].count*(*pStats)[build.getType(i)].BT*configuration.maxTime);//hier auch ProgramBT
				else
					pFitness+=((build.getRemainingBuildTime(i)*100)/((*pGoal)->goal[build.getType(i)].count*(*pStats)[build.getType(i)].BT));*/

				bonus[build.getLocation()][build.getType()]--;
			}
			buildingQueue.pop();
		}
		buildingQueue = save;
	} // end of is_ready=false
	else   // all goals fulfilled, fitness <- timer
	{
		tpF+=getTimer();
		tpF+=(*pGoal)->countGoals()*100;
//		for(list<GOAL>::const_iterator i = (*pGoal)->goal.begin();i!=(*pGoal)->goal.end();++i)
//				{
//						if(i->getCount()>0)
//								tpF+=100;
/*					  if((i->getUnit()!=GAS_SCV)&&(i->getUnit()!=SCV)) //do not punish 'too much' workers!
								if(i->getCount()<getLocationTotal(i->getLocation(),i->getUnit()))
										setsFitness(getsFitness()+(-getLocationTotal(i->getLocation(),i->getUnit())+i->getCount())*(stats[(*pGoal)->getRace()][i->getUnit()].minerals+stats[(*pGoal)->getRace()][i->getUnit()].gas));*/
//				}
	}
	return(tpF);
}
// end of calculatePrimaryFitness


const bool PRERACE::buildIt(const unsigned int build_unit)
{
    const UNIT_STATISTICS* stat = &((*pStats)[build_unit]);
	//Zuerst: availible pruefen ob am Ort gebaut werden kann
	//Wenn nicht => +/- absteigen bis alle locations durch sind
	unsigned int picked_facility = 0;
	unsigned int current_location_window = 1; // TODO
//	unsigned int ttloc=0;
//	unsigned int j=0;

/*	if(lastcounter>0)
	{	
		lastcounter--;
		tloc=last[lastcounter].location;
	}*/
	bool ok = false;
	if(stat->facility[0]==0)
		ok=true;
	else
	// special rule for morphing units of protoss
	if((stat->facility2>0) && (stat->facilityType == IS_LOST) && (stat->facility[0] == stat->facility2))
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
			for(picked_facility = 0; picked_facility<3; picked_facility++)
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
//            	        for(fac=3;fac--;)
						for(fac=0;fac<3; fac++)
                        if(
                        // special rules for morphing units of protoss
                        ((stat->facilityType != IS_LOST) || (stat->facility[fac] != stat->facility2) || (getLocationAvailible(ttloc, stat->facility[fac]) >= 2)) &&
						((stat->facility[fac] > 0) && (getLocationAvailible(ttloc, stat->facility[fac])))
                        || ((stat->facility[fac]==0)&&(fac==0))) //~~
                                                                                                                       
//                      for(fac=3;fac--;)
//                          if( ((stat->facility[fac]>0)&&(getLocationAvailible(ttloc,stat->facility[fac])>((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)))) || ((stat->facility[fac]==0)&&(fac==0)))
                            {
                                tloc=ttloc;
                                ok=true;
                                break;
                            }
//                          break;
//                      }
                        j++;
                    }*/
                                                                                                                       
	if((ok)&&(build_unit==REFINERY)) {
		if(getMapLocationAvailible(GLOBAL, current_location_window, VESPENE_GEYSIR) <=0)
			ok=false;
		else
			removeOneMapLocationAvailible(GLOBAL, current_location_window, VESPENE_GEYSIR);
	}
//TODO: Wenn verschiedene facilities moeglich sind, dann das letzte nehmen
//              bewegliche Sachen ueberdenken...
//                  evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//                  auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//      Phagen ueber Phagen...
	if(ok)
	{ 
 		if((getpGoal()->getRace()==ZERG) &&
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
		build.setUnitCount(1);
		build.setBuildFinishedTime(getTimer()-stat->BT);
		build.setTotalBuildTime(stat->BT);
		build.setType(build_unit);
		build.setIP(getIP()); //needed only for Anarace!
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


// ------------------------------
// ------ HARVEST ROUTINES ------
// ------------------------------

void PRERACE::adjustMineralHarvest(const unsigned int location_number)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::adjustMineralHarvest): Value location_number out of range.");return;
	}
#endif
	//TODO Zerg hatchery,lair etc.
	
// WAYNE Location,	  kein Command Center 							und keine Mineralien
	if((location_number == 0) || ((!getLocationTotal(location_number, COMMAND_CENTER)) && (!getMapLocationTotal(0, location_number, MINERAL_PATCH))))
	{
		for(int i=45;i--;)
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
	else if(pStart->getBasicMineralHarvestSpeedPerSecond(getPlayerNumber(), 1)) // SONST: Falls wir ne minimalSammelgeschwindigkeit haben eintragen
	{
//		int k;
		for(int i=45;i--;)
		{
//			k=0;
//			for(j=0;j<45;j++)
//				if(i*8<=j*pMap->location[num].getTotal(0][MINERAL_PATCH]) 
//				{ k=j;j=45;}
			setMineralHarvestPerSecond(location_number, i, pStart->getBasicMineralHarvestSpeedPerSecond(getPlayerNumber(), i/*k*/));//*pMap->location[num].getTotal(0][MINERAL_PATCH])/8;
		}	//ab hier funzt alles... nur scheint startPlayer->getBasic... nicht richtig initialisiert zu sein...
	
	}
}

void PRERACE::adjustGasHarvest(const unsigned int location_number)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::adjustGasHarvest): Value location_number out of range.");return;
	}
#endif
	if((location_number == 0)||((!getLocationTotal(location_number, COMMAND_CENTER)) && (!getLocationTotal(location_number, REFINERY))))
	{
		for(int i=5;i--;)
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
	else if(pStart->getBasicMineralHarvestSpeedPerSecond(getPlayerNumber(), 1))
	{
//		int k;
		for(int i=5;i--;)
		{
//			k=0;
//			for(j=0;j<5;j++)
//				if(i<=j*pMap->location[num].getTotal(playerNum][REFINERY]) { k=j;j=5;}
					setGasHarvestPerSecond(location_number, i, pStart->getBasicGasHarvestSpeedPerSecond(getPlayerNumber(), i/*k*/)*getLocationTotal(location_number, REFINERY));

//					hier liegts problem wegen gas
//						gasharvestps ist zu gross, evtl wegen zu vieler refineries!
		}
	}
}

void PRERACE::adjustHarvestAllLocations()
{
	for(int i=MAX_LOCATIONS;i--;) {
		adjustMineralHarvest(i);
		adjustGasHarvest(i);
	}
}

const unsigned int PRERACE::harvestMinerals() const
{
	unsigned int sum=0;
//	  int t=(rand()%10)-5;
	for(int i=1;i<MAX_LOCATIONS;i++)//~~
	{
		int s=location[i].getAvailible(SCV);
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


const unsigned int PRERACE::harvestGas() const
{
	int sum=0;
//	int t=(rand()%10)-5;
	for(int i=1;i<MAX_LOCATIONS;i++)//~~
	{
		int s=location[i].getAvailible(GAS_SCV);
		if(s)
		{
			if(s<4)
				sum+=getGasHarvestPerSecond(i,s);
			else
				sum+=getGasHarvestPerSecond(i,4); //TODO
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

/* staticly assigns 'start' and sets a pointer to the corresponding pointer * 
 * in start. This means, whenever the map changes in start, it is also	  *
 * changed for all players. This must be set only once per program call	 */ 
void PRERACE::assignStart(START* start)
{
#ifdef _SCC_DEBUG
	if(!start) {
		toLog("DEBUG: (PRERACE::assignStart): Variable pStart not initialized.");return;
	}
#endif
	pStart=start;
	pMap=pStart->getMap();
}
/* copies the precalculated startforce from pStart into our map (unit[][]
 * this is done 1:1, i.e. with memcpy */
void PRERACE::copyMap()
{
	pStart->copyStartForce(&(unit[0][0]));
}


void PRERACE::initNoise()
{
//	if(configuration.noise>0)
//		for(int j=0;j<MAX_TIME;j++)
//			noise[j]=rand()%configuration.noise-rand()%configuration.noise;
//	else TODO
		for(int j=MAX_TIME;j--;)
			noise[j]=0;
}

void PRERACE::resetGeneMarker()
{
	markerCounter=1;
}

void PRERACE::resetSpecial()
{
	for(int i=MAX_LOCATIONS;i--;)
		larvaInProduction[i]=0;
}

void PRERACE::removeLarvaFromQueue(const unsigned int location_number)
{
#ifdef _SCC_DEBUG
    if((location_number<1) || (location_number >= MAX_LOCATIONS)) {
	        toLog("DEBUG: (PRERACE::removeLarvaFromQueue): Value location_number out of range.");return;
    }
	if((larvaInProduction[location_number]<1)||(larvaInProduction[location_number]>=MAX_SUPPLY)) {
	        toLog("DEBUG: (PRERACE::removeLarvaFromQueue): Variable larvaInProduction not initialized or out of range.");return;
    }
#endif
	larvaInProduction[location_number]--;
}

void PRERACE::addLarvaToQueue(const unsigned int location_number)
{
#ifdef _SCC_DEBUG
    if((location_number<1) || (location_number >= MAX_LOCATIONS)) {
	        toLog("DEBUG: (PRERACE::addLarvaFromQueue): Value location_number out of range.");return;
    }
	if(larvaInProduction[location_number]>=MAX_SUPPLY) {
	        toLog("DEBUG: (PRERACE::addLarvaFromQueue): Variable larvaInProduction not initialized or out of range.");return;
    }
#endif
	larvaInProduction[location_number]++;
}

void PRERACE::setPlayerNumber(const unsigned int player_number)
{
#ifdef _SCC_DEBUG
	if((player_number < 1) || (player_number >= MAX_PLAYER)) {
		toLog("DEBUG: (PRERACE::setPlayerNumber): Value out of range.");return;
	}
#endif
	if(playerNum == player_number)
		return;
	playerNum = player_number;
	location = &(unit[player_number][0]);

	pStartCondition = pStart->getStartcondition(player_number);
	setCurrentGoal(pStart->getCurrentGoal(player_number));
	setpStats(pStart->getpStats(player_number));

//	global=&unit[playerNum][0];
}

void PRERACE::initializePlayer()
{
	setMinerals((*pStartCondition)->getMinerals());
	setGas((*pStartCondition)->getGas());
	setTimer(configuration.getMaxTime()-(*pStartCondition)->getStartTime());
	setNeedSupply((*pStartCondition)->getNeedSupply());
	setHaveSupply((*pStartCondition)->getHaveSupply());
}

void PRERACE::eraseIllegalCode()
{
	for(int i=MAX_LENGTH;i--;)
		if(Code[i]>=getpGoal()->getMaxBuildTypes())
			Code[i]=0;
/*		{
			for(int k=i;k--;)
				Code[k+1]=Code[k];
			Code[0]=0;
		}*/
}

void PRERACE::eraseUselessCode()
{
	unsigned int allUnits[UNIT_TYPE_COUNT];
	for(int i=UNIT_TYPE_COUNT;i--;)
		allUnits[i]=getLocationTotal(GLOBAL,i);
	for(int i=MAX_LENGTH;i--;)
	{
		bool ok=true;
		for(int k=3;k--;)
			ok&=((stats[getpGoal()->getRace()][getpGoal()->toPhaeno(Code[i])].prerequisite[k]==0)||
				 (allUnits[stats[getpGoal()->getRace()][getpGoal()->toPhaeno(Code[i])].prerequisite[k]]));
//WTF? allUnits mit prerequisite vergleichen!?

//TODO so ganz sauber is des net
		if(!ok)
		{
			for(int k=i;k--;)
				Code[k+1]=Code[k];
			Code[0]=0;
		}
		else
			allUnits[getpGoal()->toPhaeno(Code[i])]++;
	}
}


// TODO OPTIMIZE
void PRERACE::mutateGeneCode()
{
//	return;
// TODO logger machen, welche Mutationsart besonders erfolgreich ist
	if(getLength()==0) 
		setLength(MAX_LENGTH);//configuration.getMaxLength()); // TODO

	unsigned int force[GAS_SCV+1];
	bool buildable[GAS_SCV+1];
	unsigned int tMaxBuildTypes;
	unsigned int tGeno[GAS_SCV+1]; // !! keine anderen units drueber nehmen!


	for(unsigned int i=GAS_SCV+1;i--;)
	{
        tGeno[i]=0;
    	if((*pGoal)->getIsBuildable(i))
	    {
    	    force[i]=((*pStartCondition)->getLocationTotal(GLOBAL,i)>0); //set start force
        	if(force[i]) buildable[i]=true;
    	}
	    else
    	{
        	force[i]=0;
	        buildable[i]=false;
	    }
	}
                                                                                                                                                            
    for(unsigned int x=MAX_LENGTH;x--;) //length
    {
        if(x<MAX_LENGTH-1) { // erstes nicht einbeziehen
            force[(*pGoal)->toPhaeno(Code[x+1])]=1;
			if((*pStats)[(*pGoal)->toPhaeno(Code[x+1])].create>0)
            	force[(*pStats)[(*pGoal)->toPhaeno(Code[x+1])].create]=1;
        }
                                                                                                                                                            
        tMaxBuildTypes=0;
        for(unsigned int i=GAS_SCV+1;i--;)
        {
            if(force[i]) 
			{
				bool ok=true;
				for(int j=GAS_SCV+1;j--;)
					if((force[j])&&((*pStats)[j].create>0)&&((*pStats)[j].create == i)) {
						ok=false;break;
					}
				if(ok)
				{
					tGeno[tMaxBuildTypes]=(*pGoal)->toGeno(i);
					tMaxBuildTypes++;
					buildable[i]=true;
				}
			} else
			
            if(  ( ((*pGoal)->getIsBuildable(i) ) &&		
			   (((*pStats)[i].prerequisite[0]==0)||(force[(*pStats)[i].prerequisite[0]]))&&
               (((*pStats)[i].prerequisite[1]==0)||(force[(*pStats)[i].prerequisite[1]]))&&
               (((*pStats)[i].prerequisite[2]==0)||(force[(*pStats)[i].prerequisite[2]]))&&
               ((((*pStats)[i].facility[0]>0)&&(force[(*pStats)[i].facility[0]]))||
			   (((*pStats)[i].facility[1]>0)&&(force[(*pStats)[i].facility[1]]))||
			   (((*pStats)[i].facility[2]>0)&&(force[(*pStats)[i].facility[2]]))||
			   (((*pStats)[i].facility[0]==(*pStats)[i].facility[1]==(*pStats)[i].facility[2]==0)))))
				{
					tGeno[tMaxBuildTypes]=(*pGoal)->toGeno(i);
            	    tMaxBuildTypes++;
					buildable[i]=true;
				}
        }
                                                                                                                                                    
//alle ueberpruefen, ob die ueberhaupt baubar sind... aber von hinten her!
                                                                                                                                                            
        if(configuration.getMutationFactor()==0)
            return;
        if(rand() % (MAX_LENGTH*100/configuration.getMutationFactor())==0)
        {
            switch(rand() % 4)
            {
                //TODO: wenn generateBuildOrder==1 dann bleibts stehen!
                case 0://delete one variabel entry and move - Mehrere Schmieden/Kasernen etc. zulassen!
                {
//                  if((((*pGoal)->isVariable[Code[0][x]]==1)&&((*pGoal)->isVariable[Code[1][x]]==1))||(!configuration.preprocessBuildOrder)) TODO
                        //TODO: ueberlegen, ob Code evtl struct sein sollte... mmmh
                        for(unsigned int i=x;i<MAX_LENGTH-1;i++)
                        {
                            Code[i]=Code[i+1];
                            Marker[i]=Marker[i+1];
                        }
                    // TODO hier auch das buildable und tMaxBuildTypes rein... irgendwie den Code als "mutier mich" markieren und spaetereinfuegen
                    markerCounter++;Marker[MAX_LENGTH-1]=markerCounter;
                    unsigned int y;
//                  if(configuration.preprocessBuildOrder) // TODO
//                  while((*pGoal)->isVariable[y]==0) y=rand()%(*pGoal)->getMaxBuildTypes();
//                  else
                    y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
                    Code[MAX_LENGTH-1]=y;
//                  if(configuration.preprocessBuildOrder) // TODO
//                  while((*pGoal)->isVariable[y]==0) y=rand()%(*pGoal)->getMaxBuildTypes();
//                  else
//                  y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
//                  Code[1][MAX_LENGTH-1]=y;
                }break;
                case 1://add one variable entry
                {
                    for(unsigned int i=MAX_LENGTH-1;i>x;i--)
                    {
                        Code[i]=Code[i-1];
                        Marker[i]=Marker[i-1];
                    }
                    markerCounter++;Marker[x]=markerCounter;
                    unsigned int y;
//                  if(configuration.preprocessBuildOrder) TODO
//                      while((*pGoal)->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
//                  else
                    y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
                    Code[x]=y;
//                  if(configuration.preprocessBuildOrder)
//                      while((*pGoal)->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%(*pGoal)->getMaxBuildTypes();
//                  else
                }break;
                case 2://change one entry
                {
//                  if((*pGoal)->isVariable[Code[k][x]]==1) TODO
                    {
                        int y;
                        //int y=rand()%(*pGoal)->getMaxBuildTypes();//Optimieren
//                      if(configuration.preprocessBuildOrder) TODO
//                          while((*pGoal)->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%(*pGoal)->getMaxBuildTypes();
//                      else
                        y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
                        Code[x]=y;
                        markerCounter++;Marker[x]=markerCounter;
                    }
                }break;
                case 3://exchange two entries
					if(x < MAX_LENGTH)
                    {
                        //hier erst recht
                        unsigned int y=rand()%(MAX_LENGTH-x) + x;
//                      y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//                      if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH;
                        if(buildable[(*pGoal)->toPhaeno(Code[y])])
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
							
							for(unsigned int i = 0; i < block_length; i++)
							{
								tmpCode[i] = Code[x + i];
								tmpMarker[i] = Marker[x + i];
							}							
								
							for(unsigned int i = target_position; i < x; i++)
							{
								Code[i+block_length] = Code[i];
								Marker[i+block_length] = Marker[i];
							}

							for(unsigned int i = 0; i<block_length; i++)
							{
								Code[target_position + i] = tmpCode[i];
								Marker[target_position + i] = tmpMarker[i];
							}
                        }
                    }break;
				default:
#ifdef _SCC_DEBUG
				toLog("ERROR in PRERACE::mutateGeneCode: rand out of Range");
#endif
				break; // TODO ERROR
            }
        }
    }
}



//Reinitialize programs with random orders
void PRERACE::resetGeneCode()
{
//	mutationRate=500+rand()%1000;
/*	if((configuration.preprocessBuildOrder)&&(basicLength>0))
	{
		memcpy(Code,basicBuildOrder,MAX_LENGTH*4);
		for(int i=MAX_LENGTH;i--;)
		{
			markerCounter++;Marker[i]=markerCounter;
		}
	}
	else*/
	{
		unsigned int y=0;
		switch((*pGoal)->getRace())
		{
			case TERRA:y=SUPPLY_DEPOT;break;
			case PROTOSS:y=PYLON;break;
			case ZERG:y=OVERLORD;break;
			default:break; // TODO ERROR
		}
		for(int i=MAX_LENGTH;i--;)
		{
//			if((i+4)%stats[(*pGoal)->getRace()][y].needSupply==0)
//			{
				Code[i]=rand()%(*pGoal)->getMaxBuildTypes();
//			} else
//			{
//				Code[0][i]=/*rand()%*/(*pGoal)->toGeno(SCV);//getMaxBuildTypes();
//				Code[1][i]=/*rand()%*/(*pGoal)->toGeno(SCV);//getMaxBuildTypes();
//			}
			markerCounter++;Marker[i]=markerCounter;
		}
	}
}

#if 0
void PRERACE::crossOver(PRERACE* parent2, PRERACE* child1, PRERACE* child2)
{
/*	int counter=MAX_LENGTH;
	for(int i=0;i<MAX_LENGTH;i++)
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
		counter--;
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

const unsigned int PRERACE::getCode(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
        toLog("DEBUG: (PRERACE::getCode): Value ip out of range.");return(0);
	}	
	if(Code[ip] >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PRERACE::getCode): Variable Code not initialized.");return(0);
	}
#endif
	return(Code[ip]);
}

const unsigned int PRERACE::getCurrentCode() const
{
#ifdef _SCC_DEBUG
    if(Code[getIP()] >= UNIT_TYPE_COUNT) {
        toLog("DEBUG: (PRERACE::getCurrentCode): Variable Code not initialized.");return(0);
    }
#endif
	return(Code[getIP()]);
}

const unsigned int PRERACE::getMarker(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip > MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::getMarker): Value ip out of range.");return(0);
	}
#endif
	return(Marker[ip]); 
}
void PRERACE::copyCode(PRERACE& player)
{
	for(int i=MAX_LENGTH;i--;)
	{
		Code[i] = player.Code[i];
		Marker[i] = player.Marker[i];
	}
}

// ------ UNITS ------
const unsigned int PRERACE::getMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::getMapLocationAvailible): Value player out of range.");return(0);
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMapLocationAvailible): Value location_number out of range.");return(0);
	}
#endif
	return(unit[player][location_number].getAvailible(unit_type));
}

const unsigned int PRERACE::getMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::getMapLocationTotal): Value player out of range.");return(0);
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMapLocationTotal): Value location_number out of range.");return(0);
	}
#endif
	return(unit[player][location_number].getTotal(unit_type));
}

void PRERACE::setMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].setAvailible(unit_type, availible);
}

void PRERACE::setMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].setTotal(unit_type, total);
}

void PRERACE::addOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].addOneAvailible(unit_type);
}

void PRERACE::addOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].addOneTotal(unit_type);
}

void PRERACE::removeOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >=(*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].removeOneAvailible(unit_type);
}


void PRERACE::removeOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >=(*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].removeOneTotal(unit_type);
}

const unsigned int PRERACE::getLocationAvailible(const unsigned int location_number, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getLocationAvailible): Value location_number out of range.");return(0);
	}
#endif
	return(location[location_number].getAvailible(unit_type));
}

const unsigned int PRERACE::getLocationTotal(const unsigned int location_number, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::getLocationTotal): Value location_number out of range.");return(0);
	}
#endif
	return(location[location_number].getTotal(unit_type));
}


void PRERACE::setLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setLocationAvailible): Value location_number out of range.");return;
	}
#endif
	location[location_number].setAvailible(unit_type, availible);
}

void PRERACE::setLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS)	{
		toLog("DEBUG: (PRERACE::setLocationTotal): Value location_number out of range.");return;
	}
#endif
	location[location_number].setTotal(unit_type, total);
}

void PRERACE::addLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS) {
		toLog("DEBUG: (PRERACE::addLocationAvailible): Value location_number out of range.");return;
	}
#endif	
	location[location_number].addAvailible(unit_type, availible);
	if(location_number!=GLOBAL) //sonst waers ja doppelt...
		location[GLOBAL].addAvailible(unit_type, availible);
}

void PRERACE::addLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS)	{
		toLog("DEBUG: (PRERACE::addLocationTotal): Value location_number out of range.");return;
	}
#endif
	location[location_number].addTotal( unit_type, total );
	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
		location[GLOBAL].addTotal(unit_type, total);
}

void PRERACE::addOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
    if(location_number >= MAX_LOCATIONS) {
        toLog("DEBUG: (PRERACE::addLocationAvailible): Value location_number out of range.");return;
    }
#endif
    location[location_number].addOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
    if(location_number!=GLOBAL) 
        location[GLOBAL].addOneAvailible( unit_type );
}

void PRERACE::addOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
    if(location_number >= MAX_LOCATIONS) {
        toLog("DEBUG: (PRERACE::addLocationTotal): Value location_number out of range.");return;
    }
#endif
    location[location_number].addOneTotal( unit_type );
    if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
        location[GLOBAL].addOneTotal( unit_type );
}

void PRERACE::removeOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
    if((location_number >= MAX_LOCATIONS)) {
        toLog("DEBUG: (PRERACE::removeOneLocationAvailible): Value location_number out of range.");return;
    }
#endif
    location[location_number].removeOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
    if(location_number!=GLOBAL)
        location[GLOBAL].removeOneAvailible( unit_type );
}

void PRERACE::removeOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
    if((location_number >= MAX_LOCATIONS)) {
        toLog("DEBUG: (PRERACE::removeOneLocationTotal): Value location_number out of range.");return;
    }
#endif
    location[location_number].removeOneTotal( unit_type );
    if(location_number!=GLOBAL) 
        location[GLOBAL].removeOneTotal( unit_type );
}
// ------ END UNITS -----


// ----- SUPPLY ------
void PRERACE::setNeedSupply(const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
	if(need_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::setNeedSupply): Value need_supply out of range.");return;
	}
#endif
	needSupply = need_supply;
}

const unsigned int PRERACE::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::getNeedSupply): Variable not initialized.");return(0);
	}
#endif
	return(needSupply);
}

void PRERACE::setHaveSupply(const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
	if(have_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::setHaveSupply): Value have_supply out of range.");return;
	}
#endif
	haveSupply = have_supply;
}

const unsigned int PRERACE::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if(haveSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::getHaveSupply): Variable not initialized.");return(0);
	}
#endif
	return(haveSupply);
}
// ----- END SUPPLY ------

// ----- HARVEST ------

void PRERACE::setMinerals(const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
	if(have_minerals > MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::setMinerals): Value have_minerals out of range.");return;
	}
#endif
	minerals = have_minerals;
}

const unsigned int PRERACE::getMinerals() const
{
#ifdef _SCC_DEBUG
	if(minerals > MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::getMinerals): Variable minerals not initialized.");return(0);
	}
#endif
	return(minerals);
}

void PRERACE::setGas(const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
	if(have_gas > MAX_GAS) {
		toLog("DEBUG: (PRERACE::setGas): Value have_gas out of range.");return;
	}
#endif
	gas = have_gas;
}

const unsigned int PRERACE::getGas() const
{
#ifdef _SCC_DEBUG
	if(gas > MAX_GAS) {
		toLog("DEBUG: (PRERACE::getGas): Variable gas not initialized.");return(0);
	}
#endif
	return(gas);
}

void PRERACE::setMineralHarvestPerSecond( const unsigned int location_number, const unsigned int worker, const unsigned int mineral_harvest_per_second )
{
#ifdef _SCC_DEBUG
	if(mineral_harvest_per_second >= MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value mineral_harvest_per_second out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value location_number out of range.");return;
	}
	if(worker>=45) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value worker out of range.");return;
	}
#endif
	mineralHarvestPerSecond[location_number][worker] = mineral_harvest_per_second;
}

const unsigned int PRERACE::getMineralHarvestPerSecond( const unsigned int location_number, const unsigned int worker ) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Value location_number out of range.");return(0);
	}
	if(worker>=45) {
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Value worker out of range.");return(0);
	}
	if(mineralHarvestPerSecond[location_number][worker]>MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Variable mineralHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(mineralHarvestPerSecond[location_number][worker]);
}

void PRERACE::setGasHarvestPerSecond( const unsigned int location_number, const unsigned int worker, const unsigned int gas_harvest_per_second )
{
#ifdef _SCC_DEBUG
    if(gas_harvest_per_second >= MAX_GAS) {
        toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value gas_harvest_per_second out of range.");return;
    }
    if((location_number >= MAX_LOCATIONS)) {
        toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value location_number out of range.");return;
    }
    if(worker >= 5) {
        toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value worker out of range.");return;
    }
#endif
	gasHarvestPerSecond[location_number][worker] = gas_harvest_per_second;
}

const unsigned int PRERACE::getGasHarvestPerSecond( const unsigned int location_number, const unsigned int worker ) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Value location_number out of range.");return(0);
	}
	if(worker>=5)	{
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Value worker out of range.");return(0);
	}
	if(gasHarvestPerSecond[location_number][worker]>MAX_GAS) {
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Variable gasHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(gasHarvestPerSecond[location_number][worker]);
}

void PRERACE::setHarvestedMinerals( const unsigned int harvested_minerals )
{
#ifdef _SCC_DEBUG
	if(harvested_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::setHarvestedMinerals): Value out of range.");return;
	}
#endif
	harvestedMinerals = harvested_minerals;
}

const unsigned int PRERACE::getHarvestedMinerals() const
{
#ifdef _SCC_DEBUG
	if(harvestedMinerals>MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::getHarvestedMinerals): Variable harvestedMinerals not initialized.");return(0);
	}
#endif
	return(harvestedMinerals);
}

void PRERACE::setHarvestedGas( const unsigned int harvested_gas )
{
#ifdef _SCC_DEBUG
	if(harvested_gas >= MAX_GAS) {
		toLog("DEBUG: (PRERACE::setHarvestedGas): Value out of range.");return;
	}
#endif
	harvestedGas = harvested_gas;
}

const unsigned int PRERACE::getHarvestedGas() const
{
#ifdef _SCC_DEBUG
	if(harvestedGas > MAX_GAS) {
		toLog("DEBUG: (PRERACE::getHarvestedGas): Variable harvestedGas not initialized.");return(0);
	}
#endif
	return(harvestedGas);
}


void PRERACE::setWastedMinerals( const unsigned int wasted_minerals )
{
#ifdef _SCC_DEBUG
	if(wasted_minerals >= MAX_MINERALS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::setWastedMinerals): Value out of range.");return;
	}
#endif
	wastedMinerals = wasted_minerals;
}

const unsigned int PRERACE::getWastedMinerals() const
{
#ifdef _SCC_DEBUG
	if(wastedMinerals >= MAX_MINERALS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::getWastedMinerals): Variable wastedMinerals not initialized.");return(0);
	}
#endif
	return(wastedMinerals);
}

void PRERACE::setWastedGas(const unsigned int wasted_gas)
{
#ifdef _SCC_DEBUG
	if(wasted_gas >= MAX_GAS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::setWastedGas): Value out of range.");return;
	}
#endif
	wastedGas = wasted_gas;
}

const unsigned int PRERACE::getWastedGas() const
{
#ifdef _SCC_DEBUG
	if(wastedGas >= MAX_GAS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::getWastedGas): Variable wastedGas not initialized.");return(0);
	}
#endif
	return(wastedGas);
}
// ----- END HARVEST -----

void PRERACE::setCurrentGoal(GOAL_ENTRY** current_goal)
{
#ifdef _SCC_DEBUG
	if(!current_goal) {
		toLog("DEBUG: (PRERACE::setCurrentGoal): Variable current_goal not initialized.");return;
	}
#endif
	pGoal = current_goal;
}

const unsigned int PRERACE::getPlayerNumber() const
{
#ifdef _SCC_DEBUG
	if(playerNum >= MAX_PLAYER) {
		toLog("DEBUG: (PRERACE::getPlayerNumber): Variable not initialized.");return(0);
	}
#endif
	return(playerNum);
}

const START_CONDITION* const* PRERACE::getStartCondition()
{
	return(pStartCondition);
}


void PRERACE::setTimer(const unsigned int time)
{
#ifdef _SCC_DEBUG
	if(time > configuration.getMaxTime()) {
		toLog("DEBUG: (PRERACE::setTimer): Value time out of range.");return;
	}
#endif
	timer = time;
}

const unsigned int PRERACE::getTimer() const
{
#ifdef _SCC_DEBUG
	if(timer > configuration.getMaxTime()) {
		toLog("DEBUG: (PRERACE::getTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(timer);
}


const unsigned int PRERACE::getRealTimer() const
{
#ifdef _SCC_DEBUG
	if(timer > configuration.getMaxTime()) {
		toLog("DEBUG: (PRERACE::getRealTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(configuration.getMaxTime()-timer);
	// TODO auf > checken
}

void PRERACE::setpStats(const UNIT_STATISTICS* const* player_stats)
{
#ifdef _SCC_DEBUG
	if((*player_stats)[0].minerals!=0) { // TODO
		toLog("DEBUG: (PRERACE::setpStats): Variable not initialized.");return;
	}
#endif
	pStats = player_stats;
}

const UNIT_STATISTICS* const * PRERACE::getpStats() const
{
#ifdef _SCC_DEBUG
	if(!pStats)	{
		toLog("DEBUG: (PRERACE::getpStats): Variable not initialized.");return(0);
	}
#endif
	return(pStats);
}

void PRERACE::setIP(const unsigned int ip)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::setIP): Value out of range.");return;
	}
#endif
	IP = ip;
}

const unsigned int PRERACE::getIP() const
{
#ifdef _SCC_DEBUG
	if(IP>MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::getIP): Variable IP not initialized.");return(0);
	}
#endif
	return(IP);
}

/*const unsigned int PRERACE::getFinalTime(const unsigned int goal) const
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=MAX_GOALS))	{
		toLog("DEBUG: (PRERACE::getFinalTime): Value goal out of range.");return(0);
	}
	if((ftime[goal]<0)||(ftime[goal]>=MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getFinalTime): Variable ftime not initialized.");return(0);
	}
#endif
	return(ftime[goal]);
}*/

const unsigned int PRERACE::getLength() const
{
#ifdef _SCC_DEBUG
	if(length>MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::getLength): Variable length not initialized.");return(0);
	}
#endif
	return(length);
}

/*void PRERACE::setFinalTime(const unsigned int goal, const unsigned int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>=MAX_TIME)) {
		toLog("DEBUG: (PRERACE::setFinalTime): Value time out of range.");return;
	}
	if((goal<0)||(goal>=MAX_GOALS)) {
		toLog("DEBUG: (PRERACE::setFinalTime): Value goal out of range.");return;
	}
#endif
	ftime[goal]=time;
}*/

void PRERACE::setLength(const unsigned int bo_length)
{
#ifdef _SCC_DEBUG
	if(bo_length > MAX_LENGTH)
	{
		toLog("DEBUG: (PRERACE::setLength): Value bo_length out of range.");return;
	}
#endif
	length = bo_length;
}


void PRERACE::setTimeOut(const unsigned int time_out)
{
#ifdef _SCC_DEBUG
	if(time_out > configuration.getMaxTimeOut()) {
		toLog("DEBUG: (PRERACE::setTimeOut): Value time_out out of range.");return;
	}
#endif
	timeout = time_out;
}

const unsigned int PRERACE::getTimeOut() const
{
#ifdef _SCC_DEBUG
	if(timeout > configuration.getMaxTimeOut()) {
		toLog("DEBUG: (PRERACE::getTimeOut): Variable timeout not initialized.");return(0);
	}
#endif
	return(timeout);
}

GOAL_ENTRY** PRERACE::getCurrentGoal() const
{
	return(pGoal);
}

const BASIC_MAP* const* PRERACE::getMap()
{
	return(pMap);
}

// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

//Configuration* PRERACE::configuration;
START* PRERACE::pStart;
const BASIC_MAP* const* PRERACE::pMap;

signed int PRERACE::noise[MAX_TIME];
unsigned int PRERACE::markerCounter;
UNIT PRERACE::unit[MAX_PLAYER][MAX_LOCATIONS];


