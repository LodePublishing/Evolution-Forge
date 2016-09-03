#include "race.hpp"

/* RACE.CPP - last changed: 9/24/04                              *
 * Author: Clemens Lode                                             *
 * Copyright: Clemens Lode                                          *
 *                                                                  *
 *  PURPPOSE OF THIS FILE:                                          *
 *      This is the core of the core, where the main calculations   *
 *      are made. */

RACE::RACE():
	PRERACE(),
	mutationRate(20),
	pFitness(0),
	sFitness(0),
	tFitness(99999)
{ }

RACE::~RACE()
{ }

RACE::RACE(const RACE& object) :
	PRERACE((PRERACE)(object)),
	mutationRate( object.mutationRate ),
	pFitness( object.pFitness ),
	sFitness( object.sFitness ),
	tFitness( object.tFitness )
{ }

void RACE::resetData()
{
	PRERACE::resetPrerace();
	mutationRate=20;
	pFitness=0;
	tFitness=99999;
}

void RACE::resetStaticData()
{
	PRERACE::resetStaticPrerace();
}

RACE& RACE::operator=(const RACE& object)
{
	(PRERACE)(*this) = (PRERACE)(object);
	mutationRate = object.mutationRate;
	pFitness = object.pFitness;
	sFitness = object.sFitness;
	tFitness = object.tFitness;
	return(*this);
}

const unsigned int RACE::calculateSecondaryFitness() const
{
	// total gathered resources minus minerals that were not used
	int tsF=getHarvestedMinerals() + getHarvestedGas();// - (getWastedMinerals() + getWastedGas()) / getRealTimer();
	//TODO: evtl gas und minerals (wie urspruenglich eigentlich) in Verhaeltnis setyen wieviel es jeweils Geysire/Mineralien gibt...	
	for(int i=GAS_SCV+1;i--;)
		if((getpGoal()->getAllGoal(i)>0)&&(getpGoal()->getAllGoal(i)+(*pStartCondition)->getLocationTotal(GLOBAL, i)<getLocationTotal(GLOBAL, i)))
			tsF-=((*pStartCondition)->getLocationTotal(GLOBAL, i)-(*pStartCondition)->getLocationTotal(GLOBAL,i)-getpGoal()->getAllGoal(i))*(stats[getpGoal()->getRace()][i].gas+stats[getpGoal()->getRace()][i].minerals);
	return(tsF);
}

// TODO: reimplement/recheck the speed of the units
const bool RACE::calculateStep()
{
//ZERG:  CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
// TODO evtl bestTime wieder rein, Probleme gibts aber mit allen statistischen Sachen (sFitness, tFitness, ...)
	
	if((!getTimer())||(ready=calculateReady())||(!getIP())) 
	{
		setLength(configuration.getMaxLength()-getIP());
		if(!ready) 
			setTimer(0);
//		if(getpGoal()->getMode()==0)
			setpFitness(calculatePrimaryFitness(ready));
		while(!buildingQueue.empty()) 
			buildingQueue.pop();

// ----- RACE SPECIFIC -----
		settFitness(gettFitness()-getLength());
		setsFitness(calculateSecondaryFitness());
// ----- END RACE SPECIFIC ------

	
		return(true);
	}
/*		if(getpGoal()->getRace()==ZERG)
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
	
	bool ok=true;
	bool first=true;

	while((ok)&&(getIP()))
	{
//TODO: Variable einfuegen, damit Benutzer z.B. max Befehle pro Zeiteinheit (oder pro Location!?) einstellen kann

// set needed_ to maximum to determine the minimum of minerals/gas our jobs need (needed_ is set in buildGene)
		neededMinerals=MAX_MINERALS;
		neededGas=MAX_GAS;
		ok=buildGene(getpGoal()->toPhaeno(getCurrentCode()));
		
		if((ok)||(!getTimeOut()))
		{
			if(!first)
				settFitness(gettFitness()-1);
			first=false;
			if(!getTimeOut())
				settFitness(gettFitness()-5);
			
			setTimeOut(configuration.getMaxTimeOut());


			setIP(getIP()-1);
		}
	}


//  ------ LEAP FORWARD IN TIME ------
	int t=calculateIdleTime();
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
			adjustLocationUnitsAfterCompletion(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2);			
// increase haveSupply AFTER the building is completed (needSupply is increased BEFORE it's started!)
			setHaveSupply(getHaveSupply()+stat->haveSupply);
			addLocationTotal(build.getLocation(),build.getType(), build.getUnitCount());
			addLocationAvailible(build.getLocation(),build.getType(), build.getUnitCount());
// ------- END OF ACTUAL BUILDING ------

			
// ------ SPECIAL RULES ------
			if(build.getType()==REFINERY) {
				removeOneMapLocationTotal(GLOBAL, build.getLocation(), VESPENE_GEYSIR);
				adjustGasHarvest(build.getLocation());
			} else
			if((build.getType()==COMMAND_CENTER)&&(!getLocationTotal(build.getLocation(),COMMAND_CENTER))) {
				adjustMineralHarvest(build.getLocation());
				adjustGasHarvest(build.getLocation());
			} else 
// if the larva was built reduce the number of larvas in the queue
			if((build.getType()==LARVA)&&(getRace()==ZERG)) {
				removeLarvaFromQueue(build.getLocation());
			}
// ------ END SPECIAL RULES ------

			
// ------ CHECK WHETHER WE ARE READY ------
			getpGoal()->calculateFinalTimes(build.getLocation(), build.getType(), getLocationTotal(build.getLocation(), build.getType()), getRealTimer());
			ready=calculateReady();
// ------ END CHECK -------
			

// ------ ENQUEUE THE LAST ITEM SO IT CAN BE ACCESSED BY THE MOVING ROUTINES ------			
//			last[lastcounter].unit=build.getType(); TODO
//			last[lastcounter].count=build.getUnitCount();
//			last[lastcounter].location=build.getLocation();

			if((stat->create)&&(stat->create!=build.getType())&&(!build.getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addOneLocationTotal(build.getLocation(), stat->create);
				addOneLocationAvailible(build.getLocation(), stat->create);
//				if(last[lastcounter].unit==stat->create) last[lastcounter].count++; //TODO ???
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}
//			lastcounter++;
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

const bool RACE::buildGene(const unsigned int build_unit)
{
	const UNIT_STATISTICS* stat=&(*pStats)[build_unit];
	bool ok=false;
	if(build_unit<=REFINERY+1)
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
			{
				if(buildIt(build_unit)==true)
				{
					ok=true;
// ------ RACE SPECIFIC, tFITNESS ------
					if(getMinerals()*3<4*stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)) settFitness(gettFitness()-2);
					if(getGas()*3<4*stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)) settFitness(gettFitness()-2);
//					if((stat->needSupply>0)&&(getNeedSupply()*4<5*stat->needSupply)) settFitness(gettFitness()-2);  TODO
					if((getMinerals()*5/4<stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit))||
					   (getGas()*5/4<stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)))
						settFitness(gettFitness()-1);
				}
// ------ END RACE SPECIFIC, tFITNESS ------
			} //end minerals/gas else
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
				lastcounter--;
				int nr=0;
				while((nr<MAX_BUILDINGS-1)&&(build->getRemainingBuildTime(nr)))
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
	return(ok);
}

// Reset all ongoing data (between two generations)
void RACE::prepareForNewGeneration() // resets all data to standard starting values
{
	PRERACE::prepareForNewGeneration();

	setpFitness(0);
	setsFitness(0);
	settFitness(MAX_TFITNESS);
}



