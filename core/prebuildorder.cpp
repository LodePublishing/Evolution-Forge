#include "prebuildorder.hpp"
#include <string.h>
#include <math.h>

PREBUILDORDER::PREBUILDORDER(unsigned int player_number, START* start, UNIT (&units)[MAX_PLAYER][MAX_LOCATIONS]):
	buildingQueue(),

	successType(OK),
	successUnit(0),

	anaraceBuildFacility(0),
	anaraceBuildLocation(0),
	anaraceBuildTotalTime(0),
	anaraceBuildUnit(0),
	anaraceBuildFinishedTime(0),
	
	ready(false),

	Code(),
	IP(Code.begin()),
	unit(units),
	
	pStart(start),
	conditionsChanged(true),
	alwaysBuildWorkers(false),
	onlySwapOrders(false),

	playerNum(player_number),
	timer(0), //? max_time-1?

	resource(RACE::MAX_RESOURCE_TYPES),
	neededResource(RACE::MAX_RESOURCE_TYPES),
	harvestedResource(RACE::MAX_RESOURCE_TYPES),
	
	needSupply(0),
	haveSupply(0),
	timeout(0)
{
	for(unsigned int i = MAX_LOCATIONS; i--;)
	{
//		resourceHarvestPerFrame[i].resize(RACE::MAX_RESOURCE_TYPES);
		accepterDistance[i].resize(RACE::MAX_RESOURCE_TYPES);
	}
	
	resetGeneCode();
//	resetSpecial();
}

PREBUILDORDER::~PREBUILDORDER()
{
	while(!buildingQueue.empty())
		buildingQueue.pop();
}

void PREBUILDORDER::postProcessing()
{}

void PREBUILDORDER::resetData()
{
	conditionsChanged = true;
	while(!buildingQueue.empty())
		buildingQueue.pop();

	// ? mmmh...  TODO
//	resources.resize(GAME::
//	for(unsigned int i = MAX_LOCATIONS; i--;)
//		resourcesHarvestPerFrame.resize(RACE::MAX_RESOURCE_TYPES);
//	neededResources.
//	harvestedResources
	ready = false;
	timer = 0;
	resetGeneCode();
	resetInstructionPointer();
	needSupply = 0;
	haveSupply = 0;
	timeout = 0;
//	resetSpecial();
}

void PREBUILDORDER::eraseRemainingCode() 
{
	if(IP != Code.end())
		Code.erase(IP, Code.end());
}

/*const bool PREBUILDORDER::buildGene(const unsigned int build_unit)
{
	const UNIT_STATISTICS* stat = &((*getpStats())[build_unit]);
	bool ok = false;
	successType = OK;
	successUnit = 0;

	if(build_unit <= UNIT_TYPE_COUNT)
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
			ok = putInBuildingQueue(build_unit);
		} // if resources prere etc ok
	} // end build_unit <= REFINERY+1
	return(ok);
}*/

const bool PREBUILDORDER::buildGene(const unsigned int build_unit)
{
#ifdef _SCC_DEBUG
	if(build_unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (PREBUILDORDER::buildGene()): build_unit out of range.");return(false);
	}
#endif
	const UNIT_STATISTICS& stat = (*getpStats())[build_unit];

	
	//TODO: Array und testen wo der comp am meisten haengenbleibt und abbricht... moeglichst dann nach oben bringen!
	if(
		((stat.prerequisite[0] == 0)||(getLocationTotal(GLOBAL, stat.prerequisite[0]) > 0))&&
		((stat.prerequisite[1] == 0)||(getLocationTotal(GLOBAL, stat.prerequisite[1]) > 0))&&
		((stat.prerequisite[2] == 0)||(getLocationTotal(GLOBAL, stat.prerequisite[2]) > 0))
		
		&&
		( ((stat.facility[0] > 0)&&(getLocationAvailible(GLOBAL, stat.facility[0]) > 0))||
		  ((stat.facility[1] > 0)&&(getLocationAvailible(GLOBAL, stat.facility[1]) > 0))||
		  ((stat.facility[2] > 0)&&(getLocationAvailible(GLOBAL, stat.facility[2]) > 0))||
		  ((stat.facility[0]==0)&&(stat.facility[1]==0)&&(stat.facility[2] == 0))
		)
		&&
		(((getHaveSupply() >= stat.needSupply + getNeedSupply() - stat.haveSupply) && (stat.needSupply+getNeedSupply() <= GAME::MAX_SUPPLY)) || (stat.needSupply == 0)) 
		// TODO: IS_LOST einfuegen! ?
		&&
		((stat.facility2 == 0) || (getLocationAvailible(GLOBAL, stat.facility2) > 0)))
	{
		bool ok = true;
		for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
			if(getResource(i) < stat.resource[i])
			{
				if(neededResource[i] > stat.resource[i] - getResource(i))
					neededResource[i] = stat.resource[i] - getResource(i);
				ok = false;
				// kein break, damit wir alle neededResource schreiben koennen!
			}
		if(ok)
			return(putInBuildingQueue(build_unit));
	} //end prere/fac else
	return(false);
}



void PREBUILDORDER::prepareForNewGeneration()
{
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
	{
		setHarvestedResource(i, 0);
		setResource(i, (*(getStartCondition()))->getResource(i));
		adjustAccepterDistanceAllLocations(i);
//		adjustHarvestAllLocations(i);
	}
	
	setTimer( coreConfiguration.getMaxTime() - (*(getStartCondition()))->getStartTime() );
	setNeedSupply((*(getStartCondition()))->getNeedSupply());
	setHaveSupply((*(getStartCondition()))->getHaveSupply());
	
	const UNIT& start_units = (*(getStartCondition()))->getUnit(0);
	for(unsigned int k=RACE::UNIT_TYPE_COUNT; k--;) // TODO
		if(start_units.getTotal(k))
			getGoal()->calculateFinalTimesAtBeginning(0, k, start_units.getTotal(k), 0);
	setTimeOut(coreConfiguration.getMaxTimeOut());

	resetInstructionPointer();
	
		
	
	ready = false;

//	resetSpecial();
}																																							
// --------------------------------------------------------------
// ------ FUNCTIONS THAT ARE PRIMARILY USED DURING THE RUN ------
// --------------------------------------------------------------

const unsigned int PREBUILDORDER::calculateIdleTime() const
{
	unsigned int idle_time = getTimer();
	std::vector<unsigned int> harvest_resource(RACE::MAX_RESOURCE_TYPES);
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
		harvest_resource[i] = harvestResource(i);

	if((!buildingQueue.empty())&&(buildingQueue.top().canBeCompleted())) {
		// buildingRemaining = time until the building will be completed
#ifdef _SCC_DEBUG
		if(buildingQueue.top().getBuildFinishedTime() > getTimer())
		{
	// => Gebaeude sollte garnicht mehr in der buildingqueue sein...
			toErrorLog("ERROR: PREBUILDORDER::calculateIdleTime(): FinishedTime > getTimer...");
		}
#endif
		unsigned int time_until_next_building = getTimer() - buildingQueue.top().getBuildFinishedTime();
		// TODO
		if(idle_time > time_until_next_building) 
			idle_time = time_until_next_building;
	}

	unsigned int idle_time_resources = MAX_TIME;
	
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
		if(harvest_resource[i] > 0)
		{
	// Zeit berechnen bei der bei momentaner Sammelgeschwindigkeit das Ziel erfuellt sein wird
			unsigned int time_until_enough_resources = ( neededResource[i] + ( harvest_resource[i] - neededResource[i] % harvest_resource[i])) / harvest_resource[i];
			if(time_until_enough_resources > idle_time_resources)
				idle_time_resources = time_until_enough_resources;
		}
	
  //		(neededGas + harvest_gas - neededGas % harvest_gas)/harvestGas() < (neededMinerals+(harvest_minerals - neededMinerals % harvest_minerals))/harvest_minerals))  ?
	if(idle_time > idle_time_resources)
		idle_time = idle_time_resources;
	
	if(idle_time > getTimeOut())
		idle_time = getTimeOut();
	
	return(idle_time);
}

// called within the calculateStep function
void PREBUILDORDER::adjustLocationUnitsAfterCompletion(const unsigned int location_number, const eFacilityType facility_type, const unsigned int facility, const unsigned int facility2, const unsigned int count)
	// TODO count?
{
	switch(facility_type)
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
			if(facility2)
				addOneLocationAvailible(location_number, facility2); //?
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
				removeOneLocationTotal(GLOBAL, facility2); // mmmh... fehlerquelle TODO
//r_researches need location 0
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_EVERYWHERE:
			if(facility)
				addOneLocationAvailible(location_number, facility);
			if(facility2)
				removeOneTotalEverywhere(facility2);
			break;
		case NEEDED_ALWAYS:break;
		
		case IS_LOST_GLOBALLY:
			if(facility)
				removeOneLocationTotal(location_number, facility);
		case NEEDED_ONCE_IS_LOST_GLOBALLY:
			if(facility2)
				removeOneLocationTotalAllPlayers(location_number, facility2);
			break;
		
		case NEEDED_UNTIL_COMPLETE_IS_LOST_GLOBALLY:
			if(facility)
				addOneLocationAvailible(location_number, facility);
			if(facility2)
				removeOneLocationTotalAllPlayers(location_number, facility2);
			break;
		case NO_FACILITY_BEHAVIOUR_DEFINED:
		case FACILITY_TYPES:				    
		default:
#ifdef _SCC_DEBUG
			toErrorLog("ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
#endif
		break;
	} // end switch facility_type
}

// called when the building starts to be build
void PREBUILDORDER::adjustAvailibility(const unsigned int location, const eFacilityType facility_type, const unsigned int facility, const unsigned int facility2, const signed int count)
{
	switch(facility_type)
	{
		case NO_FACILITY:break;
		case IS_LOST:
			if(facility)
			{
				modifyLocationAvailible(location, facility, -count);
				setNeedSupply(getNeedSupply() - count * (*getpStats())[facility].needSupply);
				setHaveSupply(getHaveSupply() - count * (*getpStats())[facility].haveSupply);
			}
			if(facility2)
			{
				modifyLocationAvailible(location, facility2, -count);
				setNeedSupply(getNeedSupply() - count * (*getpStats())[facility2].needSupply);
				setHaveSupply(getHaveSupply() - count * (*getpStats())[facility2].haveSupply);
			}
			break;
		case IS_MORPHING:
			if(facility)
				modifyLocationAvailible(location, facility, -count);
			if(facility2)
				modifyLocationAvailible(location, facility2, -count);
			break;
		case NEEDED_ONCE:
			break;
		case NEEDED_UNTIL_COMPLETE:
			if(facility)
				modifyLocationAvailible(location, facility, -count);
			if(facility2)
				modifyLocationAvailible(location, facility2, -count);
				//kommt glaub nicht vor...
			break;
		case NEEDED_ONCE_IS_LOST:
			if(facility2)
			{
				modifyLocationAvailible(location, facility2, -count);
				setNeedSupply(getNeedSupply() - count * (*getpStats())[facility2].needSupply);
				setHaveSupply(getHaveSupply() - count * (*getpStats())[facility2].haveSupply);	
				// <- nicht noetig :/ eigentlich schon... bei gas scv z.B. :/
			}
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST:
			if(facility)
				modifyLocationAvailible(location, facility, -count);
			if(facility2)
			{
				modifyLocationAvailible(GLOBAL/*location*/ ,facility2, -count); // primarily for temporary R_researches, have to be location 0
// TODO				 setNeedSupply(getNeedSupply()+(*getpStats())[facility2].needSupply); // <- nicht noetig :/
			}
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_EVERYWHERE:
			if(facility)
				modifyLocationAvailible(location, facility, -count);
			if(facility2)
			{
				removeOneAvailibleEverywhere(facility2); // count?
//				modifyLocationAvailible(GLOBAL/*location*/ ,facility2, -count); // primarily for temporary R_researches, have to be location 0
// TODO				 setNeedSupply(getNeedSupply()+(*getpStats())[facility2].needSupply); // <- nicht noetig :/
			}
			break;
		case NEEDED_ALWAYS:
			if(facility)
				modifyLocationAvailible(location, facility, -count);
				// needSupply?
			if(facility2)
				modifyLocationAvailible(location, facility2, -count);
			break;

		case IS_LOST_GLOBALLY:
			if(facility)
			{
				modifyLocationAvailible(location, facility, -count);
				setNeedSupply(getNeedSupply() - count * (*getpStats())[facility].needSupply);
				setHaveSupply(getHaveSupply() - count * (*getpStats())[facility].haveSupply);
			}
		case NEEDED_ONCE_IS_LOST_GLOBALLY:
			if(facility2)
				removeOneLocationAvailibleAllPlayers(location, facility2);
			// supply koennen wir uns sparen... bis jetzt...
			break;
		
		case NEEDED_UNTIL_COMPLETE_IS_LOST_GLOBALLY:
			if(facility)
				modifyLocationAvailible(location, facility, -count);
			if(facility2)
				removeOneLocationAvailibleAllPlayers(location, facility2);
			break;
		case NO_FACILITY_BEHAVIOUR_DEFINED:
		case FACILITY_TYPES:
		default:
#ifdef _SCC_DEBUG
			  toErrorLog("ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
#endif
			break;
	}
}

// nur fuer Zerg
void PREBUILDORDER::cancelBuilding(const Building& build)
{
	const UNIT_STATISTICS* stat = &((*getpStats())[build.getType()]);
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
		setResource(i, getResource(i) + GAME::CANCELING_BUILDING_RETURN * stat->resource[i] / 100);
	setNeedSupply(getNeedSupply()-stat->needSupply);

//	if(build.getFacility()!=LARVA)
		adjustAvailibility(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2, -1);
//	else if(build.getFacility() == LARVA)
//		removeOneLocationTotal(build.getLocation(), LARVA); TODO !!
}

void PREBUILDORDER::processBuildingQueue()
{
	bool found_another = true;
	while((!buildingQueue.empty()) && (found_another == true))
	{
		if((buildingQueue.top().canBeCompleted()) && (buildingQueue.top().getBuildFinishedTime() == getTimer()))
		{
			found_another = true;
			const Building& build(buildingQueue.top());
/*			if(build.getType() == INTRON)
			{
				buildingQueue.pop();
				continue;
			}*/ // mmmh... das hier ist ne Optimierung, wir *wissen*, dass Intron nichts verursacht... naja... lieber raus :o

			const UNIT_STATISTICS* stat = &((*getpStats())[build.getType()]);

// ------ ACTUAL BUILDING ------
			adjustLocationUnitsAfterCompletion(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2, build.getUnitCount());
			setHaveSupply(getHaveSupply()+stat->haveSupply);
			modifyLocationTotal(build.getLocation(), build.getType(), build.getUnitCount());
			modifyLocationAvailible(build.getLocation(), build.getType(), build.getUnitCount());


// ------ END OF ACTUAL BUILDING ------

// ------ CHECK HARVESTING ------
			if(stat->affectsResource > 0)
			{
				for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
				{
					int j = (1 << i); // 1, 2, 4, 8, ...
					if(stat->affectsResource & j)
					{
						if(stat->affectsResource & 128)
							adjustAccepterDistanceAllLocations(i);
//							adjustHarvestAllLocations(i);
//						else
//							adjustResourceDistance(i, build.getLocation()); // TODO... eigentlich nie... wenn neuer accepter gebaut wird betrifft es alle locations... 
					}
				}
			}
// ------ END CHECK ------
			
// ------ SPECIAL RULES ------
/*			else if((getRace() == ZERG))
			{
				if(build.getType() == LARVA)
				{
					removeLarvaFromQueue(build.getLocation());
					if(checkForLarva(build.getLocation()))
						putInBuildingQueue(LARVA);
				} else if(build.getType() == BREAK_UP_BUILDING)
				{
					// LARVEN WERDEN GETOETET!
					unsigned int command_time = (getTimer() + build.getTotalBuildTime());
// suche Einheit mit kleinstem groesserem Startzeitpunkt:
					std::list<Building> b_list;
					while(!buildingQueue.empty())
					{
						const Building& my_build = buildingQueue.top();
						unsigned int start_time = my_build.getBuildFinishedTime() + my_build.getTotalBuildTime();
						// PROBLEM: das holt nur das oberste von der building queue!
						if((start_time > command_time) && (build.getType() != LARVA) && (build.getFacility() != LARVA))
						{
							cancelBuilding(my_build);
							buildingQueue.pop();
							break;
						}
						b_list.push_back(buildingQueue.top());
						buildingQueue.pop();
					}
					std::list<Building>::iterator i = b_list.begin();
					while(i!=b_list.end())
					{
						buildingQueue.push(*i);
						++i;
					}
				}
			}*/ // TODO!
// ------ END SPECIAL RULES ------

			
// ------ CHECK WHETHER WE ARE READY ------
			getGoal()->calculateFinalTimes(build.getLocation(), build.getType(), getLocationTotal(build.getLocation(), build.getType()), getRealTimer());
// ------ END CHECK ------

			if((stat->create) && (stat->create != build.getType()))// && (!build.getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
                        { //here no unitCount! ~~~
	                        addOneLocationTotal(build.getLocation(), stat->create);
	                        addOneLocationAvailible(build.getLocation(), stat->create);
																			//                              if(last[lastcounter].unit==stat->create) ++last[lastcounter].count; //TODO ???
               // ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein... ?
	       		}

	       
	       // ------ ENQUEUE THE LAST ITEM SO IT CAN BE ACCESSED BY THE MOVING ROUTINES ------


// ------ END OF LAST ITEM ------
			
			buildingQueue.pop();
			
		} // end while(getremainingbuildorder) ~|~
		else found_another = false;
	} //end while
	ready = calculateReady();
}

const std::list<unsigned int>& PREBUILDORDER::getCode() const {
	return(Code);
}

void PREBUILDORDER::leapForwardInTime()
{
	unsigned int time_span = calculateIdleTime();
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
	{
		unsigned int delta = time_span * harvestResource(i);
		setResource(i, getResource(i) + delta);
		setHarvestedResource(i, getHarvestedResource(i) + delta);
	}
	
	setTimeOut(getTimeOut() - time_span);
	setTimer(getTimer() - time_span);
}

void PREBUILDORDER::resetNeededResources()
{
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
		neededResource[i] = GAME::MAX_RESOURCES;
}

const bool PREBUILDORDER::executeAlwaysBuildInstructions()
{
	bool ok = false;
	for(std::list<GOAL>::const_iterator i = getGoal()->goalList.begin(); i != getGoal()->goalList.end(); ++i)
		if(i->getIsAlwaysBuild() && buildGene(i->getUnit()))
			ok = true;
	return(ok);
}

const bool PREBUILDORDER::executeNextInstruction()
{
	unsigned int command = getGoal()->toPhaeno(getCurrentCode());

	bool ok = buildGene(command);
	if((ok) || (isTimeOut()))
	{
		resetTimeOut();
		moveToNextInstruction();
	}
	return(ok);
}

const bool PREBUILDORDER::putInBuildingQueue(const unsigned int build_unit)
{
	//Zuerst: availible pruefen ob am Ort gebaut werden kann
	//Wenn nicht => +/- absteigen bis alle locations durch sind
	
	const UNIT_STATISTICS* stat = &((*getpStats())[build_unit]);
	bool ok = false;
	unsigned int picked_facility = 0;
	unsigned int current_location_window = 1; // TODO

//	if((!buildingQueue.empty())&&(buildingQueue.top().getType() == INTRON))
//		return(false); TODO evtl in stats reinschreiben
		
	// = = = > INTRON? Ignoriere alle folgenden Befehle
	// Larva? Larva ermoeglichen, aber gleichzeitig noch ein INTRON einfuegen TODO
	
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
		// pick one availible facility: 
			for(picked_facility = 0; picked_facility<3; ++picked_facility)
				if((stat->facility[picked_facility]>0)&&(getLocationAvailible(current_location_window, stat->facility[picked_facility])>0))
				{
					ok=true;
					break;
				}
	}
				
        if(ok)
        {
		// tja, theoretisch muesste jeder Player ein Geysir pro Standort zugewiesen bekommen und dann muesste der neutrale Player pruefen ob 
		// eine Aenderung aufgetreten ist TODO
/*                if(build_unit==REFINERY)
                {
                        if(getUnit(NEUTRAL_PLAYER, current_location_window).getAvailible(VESPENE_GEYSIR) <=0) // TODO!
                                ok=false;
                        else
				getUnit(NEUTRAL_PLAYER, current_location_window).removeOneAvailible(VESPENE_GEYSIR);
                } */
//		else
//              if(build_unit==GAS_SCV)
//              {
//                      if(getLocationTotal(current_location_window, REFINERY)*4 <= getLocationTotal(current_location_window, GAS_SCV))
//                              ok=false;
//
 //             } // Optimierung?! TODO
        }
//TODO: Wenn verschiedene facilities moeglich sind, dann das letzte nehmen
//			  bewegliche Sachen ueberdenken...
//				  evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//				  auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//	  Phagen ueber Phagen...
	if(ok)
	{ 
		Building build;
		build.setFacility(stat->facility[picked_facility]);
		build.setLocation(current_location_window);
//		if(build_unit == FROM_GAS_TO_MINERALS)
//			build.setUnitCount(getLocationAvailible(current_location_window, GAS_SCV)); // TODO falls wir ALLE gas scvs rueberschicken wollen
//		else
			build.setUnitCount(1+(stat->create == build_unit));
/*		if(build_unit == INTRON) // TODO
		{
			build.setBuildFinishedTime(getTimer() - coreConfiguration.getWaitAccuracy());
			build.setTotalBuildTime(coreConfiguration.getWaitAccuracy());
		} else*/
		{	
			build.setBuildFinishedTime(getTimer()-stat->BT);
			build.setTotalBuildTime(stat->BT);
		}
		// if it's a larva we don't want it displayed  PLUS  it may overwrite the data we had already! (see **) !! TODO
//		if((getRace() != ZERG) || (build_unit != LARVA)) TODO
		{
			anaraceBuildTotalTime = build.getTotalBuildTime();
			anaraceBuildLocation = current_location_window;
			anaraceBuildFacility = stat->facility[picked_facility];
			anaraceBuildUnit = build_unit;
			anaraceBuildFinishedTime = build.getBuildFinishedTime();
		}
		build.setType(build_unit);
		for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
			setResource(i, getResource(i) - stat->resource[i]);
		
		setNeedSupply(getNeedSupply()+stat->needSupply);
//		if((stat->needSupply>0)||(((*getpStats())[stat->facility[0]].needSupply<0)&&(stat->facilityType==IS_LOST)))  TODO!!!!
//		setNeedSupply(getNeedSupply()-stat->needSupply); //? Beschreibung!
		adjustAvailibility(current_location_window, stat->facilityType, stat->facility[picked_facility], stat->facility2, 1);
		buildingQueue.push(build);

// ---- SPECIAL RULES -----		
/*		if(getGoal()->getRace()==ZERG)
		{
			if(build_unit==LARVA)
				addLarvaToQueue(current_location_window);
			else
			if(((*getpStats())[build_unit].facility[0]==LARVA)&&(checkForLarva(current_location_window)))
				putInBuildingQueue(LARVA); // ( ** ) this call will destroy anaraceBuild* ... :o
		}*/ // TODO!
// ----- END SPECIAL RULES -----

		
	/*	if(build.getType() == INTRON)
		{
			std::list<Building> b_list;
			// fuege ein 'erinnerungsintron' vor jeden Befehl der frueher fertig wird (und somit dem INTRON die Spitzenposition streitig machen wuerde) TODO
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
		}*/ // TODO
	} //end if(ok)
	
	return(ok);
}

/* compares the current force on the map and the finnishing times of the unit types with our predefined goals */
const bool PREBUILDORDER::calculateReady() const {
	return(getGoal()->calculateReady(unit[playerNum]));
}

// nicht const da buildingqueue verzehrt wird :/
// TODO OPTIMIZE !!

// STARK VON LOCATIONS ABHAENGIG!
const unsigned int PREBUILDORDER::calculatePrimaryFitness(const bool is_ready)
{
	unsigned int tpF=0;
	if(!is_ready)
	{
		std::vector<unsigned int> bonus[MAX_LOCATIONS]; 
		for(unsigned int i = MAX_LOCATIONS; i--;)
			bonus[i].assign(RACE::UNIT_TYPE_COUNT, 0);

		tpF = getGoal()->calculateFitness(unit[playerNum], bonus);
		
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
		tpF += getGoal()->countGoals()*1000;
	}
	return(tpF);
}
// end of calculatePrimaryFitness
// ------------------------------
// ------ HARVEST ROUTINES ------
// ------------------------------

// TODO evtl spaeter: Arbeiter automatisch zur naechsten Ressourcenstelle mit Gather > 0 schicken falls zu viele Arbeiter schon vor Ort

// evtl nur eine temporaere 'distance' (je location) list fuehren, die auf Basis der vorhandenen Accepterstellen die Sammelmenge bestimmen
// race, resource_type, 'distance', mineral patches, worker
// RACE::resourceHarvestPerFrame


void PREBUILDORDER::adjustAccepterDistance(const unsigned int location_number, const unsigned int resource_type) 
{
#ifdef _SCC_DEBUG
	if(location_number >= (*getMap())->getMaxLocations()) {
		toErrorLog("DEBUG (PREBUILDORDER::adjustAccepterDistance()): location_number out of range.");return;
	}
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::adjustAccepterDistance()): resource_type out of range.");return;
	}
#endif
	std::list<unsigned int>& t = GAME::race[getRace()].resource[resource_type].accepter;
	bool found = false;
	for(unsigned int i = 0; (!found)&&(i < MAX_LOCATIONS); ++i)
	{
		unsigned int nearest = (*getMap())->getLocation(location_number)->getNearest(i);
		for(std::list<unsigned int>::const_iterator j = t.begin(); (!found)&&(j != t.end()); ++j)
			if(getLocationTotal(nearest, *j) > 0)
			{
				accepterDistance[location_number][resource_type] = (*getMap())->getLocation(location_number)->getDistance(nearest) + (*getMap())->getLocation(location_number)->getResourceDistance(resource_type);
				found = true;
			}
	}
	if(!found)
		accepterDistance[location_number][resource_type] = RESOURCE::MAX_RESOURCE_DISTANCE; // ~
}

const unsigned int PREBUILDORDER::getResourceHarvestPerFrame( const unsigned int resource_type, const unsigned int location_number, const unsigned int worker ) const {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::getResourceHarvestPerFrame()): Value resource_type out of range.");return(0);
	}
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toErrorLog("DEBUG (PREBUILDORDER::getResourceHarvestPerFrame()): Value location_number out of range.");return(0);
	}
	if(worker>=999) {
		toErrorLog("DEBUG (PREBUILDORDER::getResourceHarvestPerFrame()): Value worker out of range.");return(0);
	}
#endif
	if(accepterDistance[location_number][resource_type] == RESOURCE::MAX_RESOURCE_DISTANCE)
		return(0);
	unsigned int workers = worker;
	if(worker > RESOURCE::MAX_RESOURCE_WORKERS)
		workers = RESOURCE::MAX_RESOURCE_WORKERS;
	unsigned int resource_provider = getLocationTotal(location_number, GAME::race[getRace()].resource[resource_type].provider);
	if((resource_provider == 0) || (workers == 0))
		return(0);
	return(GAME::race[getRace()].resource[resource_type].resourceHarvestPerFrame[accepterDistance[location_number][resource_type]][getLocationTotal(location_number, GAME::race[getRace()].resource[resource_type].provider)][workers]);
}


#if 0
void PREBUILDORDER::adjustHarvest(const unsigned int location_number, const unsigned int resource_type)
{
#ifdef _SCC_DEBUG
	if(location_number >= (*getMap())->getMaxLocations()) {
		toErrorLog("DEBUG (PREBUILDORDER::adjustHarvest()): location_number out of range.");return;
	}
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::adjustHarvest()): resource_type out of range.");return;
	}
#endif

	// 1. kein provider => 0
	// 2. naehesten acceptor suchen
	// 3. 
	// naehesten provider sucehn
	// 
	// 
	
	unsigned int resource_provider = getLocationTotal(location_number, GAME::race[getRace()].resource[resource_type].provider);
	if(resource_provider == 0)	
	{
		resourceHarvestPerFrame[location_number][resource_type].resize(0);
		return;
	}
	
	bool accepter_availible = false;
	for(std::list<unsigned int>::const_iterator i = GAME::race[getRace()].resource[resource_type].accepter.begin(); i != GAME::race[getRace()].resource[resource_type].accepter.end(); ++i)
		if(getLocationTotal(location_number, *i)>0)
		{
			accepter_availible = true;
			break;
		}
	if(!accepter_availible)
	{
		resourceHarvestPerFrame[location_number][resource_type].resize(0);
		return;
	}
	unsigned int gather_amount = GAME::race[getRace()].resource[resource_type].gatherAmount;
	if(gather_amount == 0)
	{
		resourceHarvestPerFrame[location_number][resource_type].resize(0);
		return;
	}

	
// TODO worker speed mit rein in distance
	unsigned int distance_time = (*getMap())->getLocation(location_number)->getResourceDistance(resource_type);
	unsigned int harvest_time = GAME::race[getRace()].resource[resource_type].harvestTime;

// nach naehestem command_center suchen TODO
// nach naehesten Mineralien suchen TODO
//TODO: Wenn 2 SPieler an einem sammeln, beide einberechnen!

	bool change = true;
	unsigned int worker = 0;
	unsigned int value = 0;
	unsigned int old_value = 9999;
	while((value != old_value)&&(worker < 50))
	{
	//	Wahrscheinlichkeit besetzt:
	//		(worker-1) *
		
                if(worker <= 8)
       	                value = worker * gather_amount / (harvest_time + 2.0 * distance_time);
                else
       	                value = worker * gather_amount / ( harvest_time + 2.0 * distance_time + (0.5 * (sqrt(2.0 * distance_time + harvest_time + 4 * (worker-1) * harvest_time ) - 2.0 * distance_time - harvest_time)));
		old_value = value;
		resourceHarvestPerFrame[location_number][resource_type].push_back(value);
		worker++;
	}
}
#endif

void PREBUILDORDER::adjustAccepterDistanceAllLocations(const unsigned int resource_type)
{
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::adjustAccepterDistanceAllLocations()): resource_type out of range.");return;
	}
#endif
	for(unsigned int i = (*getMap())->getMaxLocations();i--;)
		adjustAccepterDistance(i, resource_type);
}


const unsigned int PREBUILDORDER::harvestResource(const unsigned int resource_type) const
{
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::harvestResource()): resource_type out of range.");return(0);
	}
#endif
	unsigned int sum = 0;
	for(unsigned int i = 1; i < (*getMap())->getMaxLocations(); ++i)
		sum += (getResourceHarvestPerFrame(resource_type, i, getLocationAvailible(i, GAME::race[getRace()].resource[resource_type].gatherer)));
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

void PREBUILDORDER::resetSpecial()
{
	memset(larvaInProduction, 0, MAX_LOCATIONS * sizeof(int));
}

void PREBUILDORDER::eraseIllegalCode()
{
	unsigned int removed = getGoal()->getWasRemoved();
	if(removed != 999)
	{
		for(std::list<unsigned int>::iterator i = Code.begin(); i != Code.end();)
			if(*i == removed)
				i = Code.erase(i);
			else 
			{
				if(*i > removed)
					--(*i);
				++i;
			}
	}
	
	for(std::list<unsigned int>::iterator i = Code.begin(); i != Code.end(); ++i)
		if(*i >= getGoal()->getMaxBuildTypes())
			*i = 0;
	// TODO => eher checken und woanders resetten als hier herummachen...
//	remove_if(Code.begin(), Code.end(), greater_equal<unsigned int>(getGoal()->getMaxBuildTypes()));
}




// TODO OPTIMIZE
void PREBUILDORDER::mutateGeneCode()
{
// TODO logger machen, welche Mutationsart besonders erfolgreich ist
	if(coreConfiguration.getMutationFactor()==0)
		return;

	std::vector<NEED> t_need = getGoal()->getNeedsAtStart(); // Auf Basis der Starteinheiten angepasste verbleibenden Voraussetzungen
	std::vector<bool> t_buildable = getGoal()->getIsBuildableAtStart();
	std::vector<bool> t_checked = getGoal()->getIsCheckedAtStart();
	std::vector<unsigned int> t_geno = getGoal()->getGenoAtStart();
	unsigned int t_max_build_types = getGoal()->getMaxBuildTypesAtStart();

#ifdef _SCC_DEBUG
	if(t_max_build_types == 0) {
		toErrorLog("ERROR: PREBUILDORDER::mutateGeneCode(): No units can be build with current configuration!");return;
	}
#endif
	if(Code.size() == 0)
	{
		unsigned int new_item;
		if(isAlwaysBuildWorkers())
			new_item = t_geno[(rand()%(t_max_build_types-1))+1];
		else
			new_item = t_geno[rand()%t_max_build_types];
		Code.insert(Code.begin(), new_item);
	}
	std::list<unsigned int>::iterator x = Code.begin();
	while(x != Code.end())
	{
// IS_LOST ETC!!! ?
	// => mehrmals durchlaufen damit auch mehrfache Mutationen an einem Punkt stattfinden koennen
//		for(unsigned int z = coreConfiguration.getMutationFactor(); z--;) TODO
		if(rand() % (1 + Code.size() * 100 / coreConfiguration.getMutationFactor())==0)
		{
			unsigned int new_item;
			if(isAlwaysBuildWorkers())
				new_item = t_geno[(rand()%(t_max_build_types-1))+1];
			else 
				new_item = t_geno[rand()%t_max_build_types];
			int random=4;
			if(isOnlySwapOrders())
				random = (rand()%3) +3;
			else random = rand()%6;
			switch(random)
			{
				case 0://remove
				{
					x = Code.erase(x);
					std::list<unsigned int>::iterator y = Code.begin();
					while(y != Code.end())
						++y;
					while(y != Code.begin())
						--y;
				}
				break;
				case 1://add
					if(Code.size() < coreConfiguration.getMaxLength()-1)
						x = Code.insert(x, new_item);
				break;
				case 2://change one entry
					*x = new_item;
				break;
				case 3://move one entry (no new_item) here
				{
					std::list<unsigned int>::iterator y = x;
					do {
						++y;
					} while((y != Code.end())&&((rand()%16)||(!t_buildable[getGoal()->toPhaeno(*y)])));
					if(y != Code.end())
						Code.splice(x, Code, y);
				}break;
				case 4://exchange two entries
				{
					std::list<unsigned int>::iterator y = x;
					do {
						++y;
					} while((y != Code.end())&&((rand()%16)||(!t_buildable[getGoal()->toPhaeno(*y)])));
					if(y != Code.end())
					{
						unsigned int swap = *x;
						*x = *y;
						*y = swap;
					}
				}break;
				case 5:// move block 
				{
	// search random valid entry:
					std::list<unsigned int>::iterator y = x;
					do {
						++y;
					} while((y != Code.end())&&((rand()%16)||(!t_buildable[getGoal()->toPhaeno(*y)])));
					if(y != Code.end())
					{
	// proceed until a random end or an invalid unit:
						std::list<unsigned int>::iterator z = y;
						do {
							++z;
						} while((z != Code.end())&&(rand()%16)&&(t_buildable[getGoal()->toPhaeno(*z)]));
						if(z != Code.end())
						{
	// move block [y..z] in front of x:
							++z; 
	 						Code.splice(x, Code, y, z);
						}
					}
					
				}break;	
				default:
#ifdef _SCC_DEBUG		       
					toErrorLog("ERROR (PREBUILDORDER::mutateGeneCode()): rand out of range");
#endif
				break;
			}
		}
		if(x == Code.end())
			break;
		int i = getGoal()->toPhaeno(*x);

//		if(!t_buildable[i]) // !
//		{
			// remove;
			// continue;
//		} TODO falls zufaellige bos...
		
		++x;
		
		std::list<unsigned int> new_buildable;
		
		if(!t_checked[i])
		{
			GAME::race[getRace()].allows[i].updateBuildableList(t_need, new_buildable, i);
			t_checked[i]=true;
			
			// if it is marked as singular item then remove entry, don't build this anymore from now on				
/*			if(getGoal()->getIsSingleOccurence(i))
				for(unsigned int j = t_max_build_types;j--;)
					if(t_geno[j] == getGoal()->toGeno(i))
					{
						--t_max_build_types;
						for(unsigned int k = j; k < t_max_build_types; ++k)
							t_geno[k] = t_geno[k+1];
					}*/
		}
		
		if((*getpStats())[i].create)
		{
			i = (*getpStats())[i].create;
			if(!t_checked[i])
			{
				GAME::race[getRace()].allows[i].updateBuildableList(t_need, new_buildable, i);
				t_checked[i]=true;
			}
		}
		for(std::list<unsigned int>::iterator j = new_buildable.begin(); j != new_buildable.end(); ++j)
			if(getGoal()->getIsBuildable(*j)) // wirklich noetig?
			{
				t_buildable[*j]=true;
				t_geno[t_max_build_types]=getGoal()->toGeno(*j);
				++t_max_build_types;
			}
		
	} //while
}

const bool PREBUILDORDER::calculationDone()
{
	if((ready) || (!getTimer()) || (codeFinished() && (buildingQueue.empty() || (!getTimeOut()))))
	{
//		eraseRemainingCode();
		if(!ready) 
		{
			ready = calculateReady();
			if(!ready)
				setTimer(0);
		}
		postProcessing();
		return(true);
	}
	return(false);
}

#include <sstream>
const bool PREBUILDORDER::checkForLarva(const unsigned int current_location_window)
{	
		// Larva wird benoetigt zum Bau? Fein, dann bauen wir eine neue Larva falls nicht schon alle hatcheries etc. belegt sidn
				// Gesamtzahl der Larven < 3 * HATCHERY?
// TODO: Ueberpruefen: Eine Art Defizit einfuehren: Wenn eine Brutstaette voll ausgelastet ist und anschliessend eine neue fertig wird, wird dann ploetzlich schneller produziert?

// Falls mehr Produktionsstaetten existieren als Larven in Produktion und Larven auf dem Feld sind...
	
/*	if(((   getLocationTotal(current_location_window, HATCHERY)+
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
	} else */return false; // TODO!
}



//Reinitialize programs with random orders
void PREBUILDORDER::resetGeneCode()
{
	Code.clear();
	resetInstructionPointer();
}


// -------------------------------------------------------------------
// ------ BELOW ALL THE GET/SET FUNCTIONS, PRETTY UNINTERESTING ------
// -------------------------------------------------------------------

void PREBUILDORDER::setConditionsChanged(const bool conditions_changed) 
{
	conditionsChanged = conditions_changed;
	if(conditionsChanged)
		resetData();
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



// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------
