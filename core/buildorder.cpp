#include "buildorder.hpp"

/* BUILDORDER.CPP - last changed: 9/24/04			      *
 * Author: Clemens Lode					     *
 * Copyright: Clemens Lode					  *
 *								  *
 *  PURPPOSE OF THIS FILE:					  *
 *      This is the core of the core, where the main calculations   *
 *      are made. */

BUILDORDER::BUILDORDER():
	PREBUILDORDER(),
	mutationRate(20),
	pFitness(0),
	sFitness(0),
	tFitness(99999)
{ }

BUILDORDER::~BUILDORDER()
{ }

BUILDORDER::BUILDORDER(const BUILDORDER& object) :
	PREBUILDORDER((PREBUILDORDER)(object)),
	mutationRate( object.mutationRate ),
	pFitness( object.pFitness ),
	sFitness( object.sFitness ),
	tFitness( object.tFitness )
{ }

void BUILDORDER::resetData()
{
	PREBUILDORDER::resetPrerace();
	mutationRate = 20;
	pFitness = 0;
	tFitness = 99999;
}

BUILDORDER& BUILDORDER::operator=(const BUILDORDER& object)
{
	(PREBUILDORDER)(*this) = (PREBUILDORDER)(object);
	mutationRate = object.mutationRate;
	pFitness = object.pFitness;
	sFitness = object.sFitness;
	tFitness = object.tFitness;
	return(*this);
}
#include <sstream>
const unsigned int BUILDORDER::calculateSecondaryFitness() const
{
	//TODO: evtl gas und minerals (wie urspruenglich eigentlich) in Verhaeltnis setyen wieviel es jeweils Geysire/Mineralien gibt...	
	unsigned int penalty = 0;
	for(unsigned int i=GAS_SCV+1; i--;)
	{
		unsigned int total = getGoal()->getAllGoal(i);
		if((total>0)&&((*pStartCondition)->getLocationTotal(GLOBAL, i) > total))
			total = (*pStartCondition)->getLocationTotal(GLOBAL, i);
		if(getLocationTotal(GLOBAL, i) > total)
			penalty += (getLocationTotal(GLOBAL,i) - total) * (stats[getGoal()->getRace()][i].gas+stats[getGoal()->getRace()][i].minerals);
	}
	if(getHarvestedMinerals() + getHarvestedGas() < penalty)
	{
		std::ostringstream os; os.str("");
		for(unsigned int i=GAS_SCV+1; i--;)
		{
			unsigned int total = getGoal()->getAllGoal(i);
			if((*pStartCondition)->getLocationTotal(GLOBAL, i) > total)
				total = (*pStartCondition)->getLocationTotal(GLOBAL, i);					
			if(total < getLocationTotal(GLOBAL, i))
			{
				os.str("");
				os << "Unit: " << i << ", " << (getLocationTotal(GLOBAL,i) - total) * (stats[getGoal()->getRace()][i].gas+stats[getGoal()->getRace()][i].minerals);
				toLog(os.str());
			}
		}
		os.str("");
		os << "Penalty/Ressources: " << penalty << " / " << getHarvestedMinerals() + getHarvestedGas() << " workers: " << getLocationTotal(GLOBAL, 1);
		toLog(os.str());
	}
	return(getHarvestedMinerals() + getHarvestedGas() - penalty);
}

// TODO: reimplement/recheck the speed of the units
const bool BUILDORDER::calculateStep()
{
//ZERG:  CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
// TODO evtl bestTime wieder rein, Probleme gibts aber mit allen statistischen Sachen (sFitness, tFitness, ...)
	
	if((!getTimer()) || (ready = calculateReady()) || (!getIP())) 
	{
		setLength(coreConfiguration.getMaxLength()-getIP());
		if(!ready) 
			setTimer(0);
//		if(getGoal()->getMode()==0)
			setpFitness(calculatePrimaryFitness(ready));
		while(!buildingQueue.empty()) 
			buildingQueue.pop();

// ----- BUILDORDER SPECIFIC -----
		settFitness(gettFitness()-getLength());
		setsFitness(calculateSecondaryFitness());
// ----- END BUILDORDER SPECIFIC ------

	
		return(true);
	}
/*		if(getGoal()->getRace()==ZERG)
		{
//		  ((*pStats)[build_unit].facility[0]==LARVA)&&
		// Larva wird benoetigt zum Bau? Fein, dann bauen wir eine neue Larva falls nicht schon alle hatcheries etc. belegt sidn
				// Gesamtzahl der Larven < 3 * HATCHERY?
		   if(((getLocationTotal(1, HATCHERY)+
		     getLocationTotal(1, LAIR)+
			 getLocationTotal(1, HIVE)) *3 > 
			 (larvaInProduction[1]+getLocationTotal(1, LARVA))))
			 {
// max 1 larva pro Gebaeude produzieren
 		   if(((getLocationTotal(1, HATCHERY)+
		     getLocationTotal(1, LAIR)+
			 getLocationTotal(1, HIVE) > larvaInProduction[1]))) // => zuwenig Larven da!
			 {
				if(buildIt(LARVA)) // TODO!
					addLarvaToQueue(1);
			}
			}
		}*/
	
	bool ok = true;
	bool first = true;

	while((ok)&&(getIP()))
	{
//TODO: Variable einfuegen, damit Benutzer z.B. max Befehle pro Zeiteinheit (oder pro Location!?) einstellen kann

// set needed_ to maximum to determine the minimum of minerals/gas our jobs need (needed_ is set in buildGene)
		neededMinerals = MAX_MINERALS;
		neededGas = MAX_GAS;
		unsigned int code = getGoal()->toPhaeno(getCurrentCode());
		if((code >= BUILD_PARALLEL_2) && (code <= BUILD_PARALLEL_16))
		{
			setIP(getIP()-1);
			while((getGoal()->toPhaeno(getCurrentCode()) > GAS_SCV)&&(getIP()))
				setIP(getIP()-1);
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
			ok = buildGene(getGoal()->toPhaeno(getCurrentCode()));
		// ~~
			if((ok)||(!getTimeOut()))
			{
				if(!first)
					settFitness(gettFitness()-1);
				first = false;
				if(!getTimeOut())
					settFitness(gettFitness()-5);
				setTimeOut(coreConfiguration.getMaxTimeOut());
				setIP(getIP()-1);
			}
// Try parallel commands
			std::list<PARALLEL_COMMAND*>::iterator command = parallelCommandQueues.begin(); 
			while(command != parallelCommandQueues.end())
			{
				unsigned int build_unit = (*command)->unit;
				ok = buildGene(build_unit);
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
	int t = calculateIdleTime();
//	int oldMinerals = getMinerals();
//	int oldGas = getGas();

	setMinerals(getMinerals()+harvestMinerals()*t);
	setHarvestedMinerals(getHarvestedMinerals()+harvestMinerals()*t);
//	setWastedMinerals(getWastedMinerals() + oldMinerals*t + (getMinerals() - oldMinerals) * t / 2);

	setGas(getGas()+harvestGas()*t);
	setHarvestedGas(getHarvestedGas()+harvestGas()*t);
//	setWastedGas(getWastedGas() + oldGas*t + (getGas() - oldGas) * t / 2);
			
	setTimeOut(getTimeOut()-t);
	setTimer(getTimer()-t);
//  ------ END LEAP FORWARD IN TIME ------
	
	bool foundAnother=true;
	while((!buildingQueue.empty())&&(foundAnother==true))
	{
		if((buildingQueue.top().canBeCompleted())&&(buildingQueue.top().getBuildFinishedTime()==getTimer()))
		{
			foundAnother=true;
			const Building& build(buildingQueue.top());
			const UNIT_STATISTICS* stat=&(*pStats)[build.getType()];

// ------ ACTUAL BUILDING ------
			adjustLocationUnitsAfterCompletion(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2, build.getUnitCount());
// increase haveSupply AFTER the building is completed (needSupply is increased BEFORE it's started!)
			setHaveSupply(getHaveSupply()+stat->haveSupply);
			addLocationTotal(build.getLocation(),build.getType(), build.getUnitCount());
			addLocationAvailible(build.getLocation(),build.getType(), build.getUnitCount());
// ------- END OF ACTUAL BUILDING ------

			
// ------ SPECIAL RULES ------
			if(build.getType() == REFINERY) {
				removeOneMapLocationTotal(GLOBAL, build.getLocation(), VESPENE_GEYSIR);
				adjustGasHarvest(build.getLocation());
			} else
			if((build.getType() == COMMAND_CENTER)&&(!getLocationTotal(build.getLocation(),COMMAND_CENTER))) {
				adjustMineralHarvest(build.getLocation());
				adjustGasHarvest(build.getLocation());
			} else 
// if the larva was built reduce the number of larvas in the queue
			if((build.getType()==LARVA)&&(getRace()==ZERG)) {
				removeLarvaFromQueue(build.getLocation());

                                if(// Gesamtzahl der Larven < 3 * HATCHERY?
                   ((getLocationTotal(build.getLocation(), HATCHERY)+
                         getLocationTotal(build.getLocation(), LAIR)+
                         getLocationTotal(build.getLocation(), HIVE)) *3 >
                         (larvaInProduction[build.getLocation()]+getLocationTotal(build.getLocation(), LARVA)))  &&
// max 1 larva pro Gebaeude produzieren
                   ((getLocationTotal(build.getLocation(), HATCHERY)+
                         getLocationTotal(build.getLocation(), LAIR)+
                         getLocationTotal(build.getLocation(), HIVE) >
                          larvaInProduction[build.getLocation()]))) // => zuwenig Larven da!
                        {
                                addLarvaToQueue(build.getLocation());
                                if(!buildIt(LARVA));
//                                      removeLarvaFromQueue(build.getLocation());
                        }

				
			}
// ------ END SPECIAL RULES ------

			
// ------ CHECK WHETHER WE ARE READY ------
			getGoal()->calculateFinalTimes(build.getLocation(), build.getType(), getLocationTotal(build.getLocation(), build.getType()), getRealTimer());
			ready = calculateReady();
// ------ END CHECK -------
			

// ------ ENQUEUE THE LAST ITEM SO IT CAN BE ACCESSED BY THE MOVING ROUTINES ------			
//			last[lastcounter].unit=build.getType(); TODO
//			last[lastcounter].count=build.getUnitCount();
//			last[lastcounter].location=build.getLocation();

			if((stat->create) && (stat->create != build.getType()) && (!build.getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addOneLocationTotal(build.getLocation(), stat->create);
				addOneLocationAvailible(build.getLocation(), stat->create);
//				if(last[lastcounter].unit==stat->create) ++last[lastcounter].count; //TODO ???
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}
//			++lastcounter;
// ------ END OF LAST ITEM
			buildingQueue.pop();
// oder: irgendeine location... TODO: Problem: die Einheiten koennen irgendwo sein, also nicht gesammelt an einem Fleck...
		} // END of if(build.getBuildFinishedTime()==getTimer())
		else foundAnother=false;
	} // END of while
	return(false);
	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
}

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)


//FEHLER MUSS HIER SEIN!! TODO

//TODO: pFitness wird total falsch berechnet fuer races die nicht alle Ziele erfuellt haben!1 [location problem halt...]

const bool BUILDORDER::buildGene(const unsigned int build_unit)
{
	const UNIT_STATISTICS* stat = &(*pStats)[build_unit];
	bool ok = false;
	if(build_unit <= LAST_UNIT)
	{
		//TODO: Array und testen wo der comp am meisten haengenbleibt und abbricht... moeglichst dann nach oben bringen!
		if(
			((stat->prerequisite[0]==0)||(getLocationTotal(GLOBAL, stat->prerequisite[0])>0))&&
			((stat->prerequisite[1]==0)||(getLocationTotal(GLOBAL, stat->prerequisite[1])>0))&&
			((stat->prerequisite[2]==0)||(getLocationTotal(GLOBAL, stat->prerequisite[2])>0))
			
			&&
			( ((stat->facility[0]>0)&&(getLocationAvailible(GLOBAL, stat->facility[0])>0))||
			  ((stat->facility[1]>0)&&(getLocationAvailible(GLOBAL, stat->facility[1])>0))||
			  ((stat->facility[2]>0)&&(getLocationAvailible(GLOBAL, stat->facility[2])>0))||
			  ((stat->facility[0]==0)&&(stat->facility[1]==0)&&(stat->facility[2]==0))
			)
			&&
			(((getHaveSupply()>=stat->needSupply+getNeedSupply()-stat->haveSupply)&&(stat->needSupply+getNeedSupply()<=MAX_SUPPLY)) ||(stat->needSupply==0)) // TODO: IS_LOST einfuegen!
			&&
			((stat->facility2==0)||(getLocationAvailible(GLOBAL, stat->facility2)>0))
			&&
			((stat->upgrade[0]==0)||(getLocationTotal(GLOBAL, build_unit)<2) || (getLocationTotal(GLOBAL, stat->upgrade[0]))) 
			&&
			((stat->upgrade[1]==0)||(getLocationTotal(GLOBAL, build_unit)<3) || (getLocationTotal(GLOBAL, stat->upgrade[1])))) 
		{
		
			if
			(getMinerals()<stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit))
			{
				if(neededMinerals>stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getMinerals())
					neededMinerals=stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getMinerals();
			}
			else
			if(getGas()<stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit))
			{
				if(neededGas>stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getGas())
					neededGas=stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getGas();
			}
			else
				if(buildIt(build_unit)==true)
				{
					ok=true;
// ------ BUILDORDER SPECIFIC, tFITNESS ------
					if(getMinerals()*3<4*stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)) settFitness(gettFitness()-2);
					if(getGas()*3<4*stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)) settFitness(gettFitness()-2);
//				      if((stat->needSupply>0)&&(getNeedSupply()*4<5*stat->needSupply)) settFitness(gettFitness()-2);  TODO
					if((getMinerals()*5/4<stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit))||(getGas()*5/4<stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)))
						settFitness(gettFitness()-1);
				} else ok = false;
		} //end prere/fac else
	} //end build_unit < REFINERY
	else // build_unit > REFINERY+1
	{

//hier checken!

// TODO ueberlegen was mit scv passiert, das gerade ein Gebaeude fertiggestellt hat... steht das in irgendeinem "last" drin?
/*		int count=0;
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
				--lastcounter;
				int nr=0;
				while((nr<MAX_BUILDINGS-1)&&(build->getRemainingBuildTime(nr)))
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
				setRemainingBuildTime(nr,(*pMap)->location[last[lastcounter].location].getDistance(count)*100/(*pStats)[last[lastcounter].unit].speed);
				setOnTheRun(nr,1);
//						building[nr].IP=IP; TODO?
						// 2x Unit => send 8/All instead of just one unit there
				if((getIP()>1)&&((Code[0][getIP()-1]==build_unit)||(Code[1][getIP()-1]==build_unit)))
				{
					if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>=6)
						setUnitCount(nr,6);
					else setUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
//					setProgramIsBuilt(getIP(),1);
					setIP(getIP()-1);
				}*/
//				addLocationAvailible(last[lastcounter].location,build->getType(nr),-build->getUnitCount(nr));
//				addLocationTotal(last[lastcounter].location,build->getType(nr),-build->getUnitCount(nr));
//				setProgramIsBuilt(getIP(),1);
//				ok=true;
//			}
//		}
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
	return(ok);
}

const bool BUILDORDER::buildIt(const unsigned int build_unit)
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
		if(build_unit == FROM_GAS_TO_MINERALS)
			build.setUnitCount(getLocationAvailible(current_location_window, GAS_SCV));
		else
			build.setUnitCount(1+(stat->create == build_unit));
		build.setBuildFinishedTime(getTimer()-stat->BT);
		build.setTotalBuildTime(stat->BT);
		build.setType(build_unit);
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
// Reset all ongoing data (between two generations)
void BUILDORDER::prepareForNewGeneration() // resets all data to standard starting values
{
	PREBUILDORDER::prepareForNewGeneration();
	setpFitness(0);
	setsFitness(0);
	settFitness(MAX_TFITNESS);
}



