#include "race.hpp"

/* RACE.CPP - last changed: 6/23/04                              *
 * Author: Clemens Lode                                             *
 * Copyright: Clemens Lode                                          *
 *                                                                  *
 *  PURPPOSE OF THIS FILE:                                          *
 *      This is the core of the core, where the main calculations   *
 *      are made. */

RACE::RACE()
{
	setpFitness(0);
	setsFitness(0);
	settFitness(99999);
}

RACE::~RACE()
{
}


const int RACE::calculateSecondaryFitness() const
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
		setLength(ga->maxLength-getIP());
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
			
			setTimeOut(ga->maxTimeOut);
			setIP(getIP()-1);
		}
	}


//  ------ LEAP FORWARD IN TIME ------
	int t=calculateIdleTime();
	int oldMinerals = getMinerals();
	int oldGas = getGas();

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
		if(buildingQueue.top().getBuildFinishedTime()==getTimer())
		{
			foundAnother=true;
		    const Building build = buildingQueue.top();
			const UNIT_STATISTICS* stat=&(*pStats)[build.getType()];

// ------ ACTUAL BUILDING ------
			adjustLocationUnitsAfterCompletion(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2);			
// increase haveSupply AFTER the building is completed (needSupply is increased BEFORE it's started!)
			setHaveSupply(getHaveSupply()+stat->haveSupply);
			addLocationTotal(build.getLocation(),build.getType(),build.getUnitCount());
			addLocationAvailible(build.getLocation(),build.getType(),build.getUnitCount());
// ------- END OF ACTUAL BUILDING ------

			
// ------ SPECIAL RULES ------
			if(build.getType()==REFINERY) {
				addMapLocationTotal(GLOBAL, build.getLocation(),VESPENE_GEYSIR,-1);
				adjustGasHarvest(build.getLocation());
			} else
			if((build.getType()==COMMAND_CENTER)&&(!getLocationTotal(build.getLocation(),COMMAND_CENTER))) {
				adjustMineralHarvest(build.getLocation());
				adjustGasHarvest(build.getLocation());
			} else 
// if the larva was built reduce the number of larvas in the queue
			if((build.getType()==LARVA)&&(getRace()==ZERG)&&(!buildGene(LARVA))) {
					removeLarvaFromQueue(build.getLocation());
			}
// ------ END SPECIAL RULES ------

			
// ------ CHECK WHETHER WE ARE READY ------
			getpGoal()->calculateFinalTimes(build.getLocation(), build.getType(), getRealTimer());
			ready=calculateReady();
// ------ END CHECK -------
			

// ------ ENQUEUE THE LAST ITEM SO IT CAN BE ACCESSED BY THE MOVING ROUTINES ------			
			last[lastcounter].unit=build.getType();
			last[lastcounter].count=build.getUnitCount();
			last[lastcounter].location=build.getLocation();

			if((stat->create)&&(!build.getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addOneLocationTotal(build.getLocation(),stat->create);
				addOneLocationAvailible(build.getLocation(),stat->create);
				if(last[lastcounter].unit==stat->create) last[lastcounter].count++; //TODO ???
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}
			lastcounter++;
// ------ END OF LAST ITEM
			
// berechne 'FinalTime', die angibt, wann alle Einheiten (bzw. die letzte Einheit) unsereres Zieles fertiggestellt wurden
// Quasi wie "ready" nur ohne Zeit
//TODO CHECKEN!!
// Did we reach the right number at the right time?
//			  i=MAX_GOALS;  TODO? koennen wir mehrere goals gleichzeitig erfuell0rn?
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


const bool RACE::buildGene(const int unit)
{
	const UNIT_STATISTICS* stat=&(*pStats)[unit];
	bool ok=false;
	if(unit<=REFINERY+1)
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
			((stat->facility2==0)||(getLocationAvailible(GLOBAL, stat->facility2)>0)))
		{
		
			if
			(getMinerals()<stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, unit))
			{
				if(neededMinerals>stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)-getMinerals())
					neededMinerals=stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)-getMinerals();
			}
			else
			if(getGas()<stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, unit))
			{
				if(neededGas>stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)-getGas())
					neededGas=stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)-getGas();
			}
			else
			{
				//Zuerst: availible pruefen ob am Ort gebaut werden kann
				//Wenn nicht => +/- absteigen bis alle locations durch sind
				int fac=0;
				int tloc=1;
				int ttloc=0;
				int j=0;

				if(lastcounter>0)
				{	
					lastcounter--;
					tloc=last[lastcounter].location;
				}

//				if((stat->facility2==0)||(getLocationAvailible(/*tloc*/0,stat->facility2)>0))
				for(fac=3;fac--;)
					if( ((stat->facility[fac]>0)&&
				 		(getLocationAvailible(tloc,stat->facility[fac])>((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)))) ||  //sonderregel wegen doppel morphunits von protoss
						((stat->facility[fac]==0)&&(fac==0))) 
					{
						ok=true;
						break;
					}

				j=1;
				if(!ok)
					while(j<MAX_LOCATIONS)
					{
						ttloc=(*pMap)->getLocation(tloc)->getNearest(j);
//						if((stat->facility2==0)||(getLocationAvailible(ttloc,stat->facility2)>0)) TODO
//						{
						for(fac=3;fac--;)
							if( ((stat->facility[fac]>0)&&(getLocationAvailible(ttloc,stat->facility[fac])>((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)))) || ((stat->facility[fac]==0)&&(fac==0)))
							{
								tloc=ttloc;
								ok=true;
								break;
							}
						break;
//					}	
						j++;
					}

				if((ok)&&(unit==REFINERY)) {
					if(getMapLocationAvailible(GLOBAL, tloc, VESPENE_GEYSIR) <=0)
						ok=false;
					else 
						addMapLocationAvailible(GLOBAL, tloc, VESPENE_GEYSIR, -1);
				}
//TODO: Wenn verschiedene facilities moeglich sind, dann das letzte nehmen						
//				bewegliche Sachen ueberdenken...
//					evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//					auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//		Phagen ueber Phagen...			
				if(ok)
				{
					if(getpGoal()->getRace()==ZERG)
					{
						if((*pStats)[unit].facility[0]==LARVA)
						{
		// Larva wird benoetigt zum Bau? Fein, dann bauen wir eine neue Larva falls nicht schon alle hatcheries etc. belegt sidn
							if(
									// Gesamtzahl der Larven < 3 * HATCHERY?
									(((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE))*3>
									  (larvaInProduction[tloc]+getLocationTotal(tloc, LARVA)))&&
									 // max 1 larva pro Gebaeude produzieren
									 ((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE)>larvaInProduction[tloc])))) // => zuwenig Larven da!
							{
								if(buildGene(LARVA))
									addLarvaToQueue(tloc);
							}
					
						}
					}

// ------ RACE SPECIFIC, tFITNESS ------
					if(getMinerals()*3<4*stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)) settFitness(gettFitness()-2);
					if(getGas()*3<4*stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)) settFitness(gettFitness()-2);
//					if((stat->needSupply>0)&&(getNeedSupply()*4<5*stat->needSupply)) settFitness(gettFitness()-2);  TODO
					if((getMinerals()*5/4<stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, unit))||
					   (getGas()*5/4<stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)))
						settFitness(gettFitness()-1);
// ------ END RACE SPECIFIC, tFITNESS ------
					
					if(lastunit==0) lastunit=unit;
					if(unit!=lastunit)//~~
					{
						settFitness(gettFitness()-1);
						lastunit=unit;
					}
                                             
					Building build;					
					build.setOnTheRun(false);
                    build.setFacility(stat->facility[fac]);
                    build.setLocation(tloc);
                    build.setUnitCount(1);
                    build.setBuildFinishedTime(getTimer()-stat->BT/*+3200*(stat->facility2==unit)*/); //~~ hack :/ TODO SINN???????
                    build.setTotalBuildTime(stat->BT);
                    build.setType(unit);
//					build.setIP(getIP()); needed only for Anarace!
					
// upgrade_cost is 0 if it's no upgrade
					setMinerals(getMinerals()-(stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL,unit)));
					setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL,unit)));
	
					setNeedSupply(getNeedSupply()+stat->needSupply);
//					if((stat->needSupply>0)||(((*pStats)[stat->facility[0]].needSupply<0)&&(stat->facilityType==IS_LOST)))  TODO!!!!
//						setNeedSupply(getNeedSupply()-stat->needSupply); //? Beschreibung!
					adjustAvailibility(tloc, fac, stat);
					buildingQueue.push(build);
				} //end if(ok)
			} //end minerals/gas else
		} //end prere/fac else
	} //end unit < REFINERY
	else // unit > REFINERY+1
	{

//hier checken!

// TODO ueberlegen was mit scv passiert, das gerade ein Gebaeude fertiggestellt hat... steht das in irgendeinem "last" drin?
/*		int count=0;
		switch(unit)
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
				if((getIP()>1)&&((Code[0][getIP()-1]==unit)||(Code[1][getIP()-1]==unit)))
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
	  if((gRace==ZERG)&&(unit==BREAK_UP_BUILDING)&&(BuildingRunning>0)) // lieber eine variable mit last_gebaeude oder so machen und da die Daten speichern, anstatt Programm oder buildings durchzulaufen...
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

// ----------------------------------------------------
// ------ PRETTY UNINTERESTING SET/GET FUNCTIONS ------
// ----------------------------------------------------

void RACE::setpFitness(const int pFitness) 
{
#ifdef _SCC_DEBUG
	if(pFitness<0) {
		toLog("DEBUG: (RACE::setpFitness): Value pFitness out of range.");return;
	}
#endif
	this->pFitness=pFitness;
}

void RACE::setsFitness(const int sFitness)
{
#ifdef _SCC_DEBUG
	if((sFitness<0)||(sFitness>MAX_MINERALS+MAX_GAS)) {
		toLog("DEBUG: (RACE::setsFitness): Value sFitness out of range.");return;
	}
#endif
	this->sFitness=sFitness;
}

void RACE::settFitness(const int tFitness)
{
#ifdef _SCC_DEBUG
	if(tFitness<0) {
		toLog("DEBUG: (RACE::settFitness): Value tFitness out of range.");return;
	}
#endif
	this->tFitness=tFitness;
}

const int RACE::getpFitness() const
{
#ifdef _SCC_DEBUG
	if((pFitness<0)||(pFitness>30000)) { // TODO
		toLog("DEBUG: (RACE::getpFitness): Variable pFitness not initialized.");return(0);
	}
#endif
	return(pFitness);
}

const int RACE::getsFitness() const
{
#ifdef _SCC_DEBUG
	if((sFitness<0)||(sFitness>MAX_MINERALS+MAX_GAS)) {
		toLog("DEBUG: (RACE::getsFitness): Variable sFitness not initialized.");return(0);
	}
#endif
	return(sFitness);
}

const int RACE::gettFitness() const
{
#ifdef _SCC_DEBUG
	if((tFitness<0)||(tFitness>MAX_TFITNESS)) {
		toLog("DEBUG: (RACE::gettFitness): Variable tFitness not initialized.");return(0);
	}
#endif
	return(tFitness);
}

