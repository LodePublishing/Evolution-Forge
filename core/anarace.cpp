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


// TODO: reimplement/recheck the speed of the units
ANARACE::ANARACE()
{
	setGeneration(0);
	setMaxpFitness(0);
	setMaxsFitness(0);
	setMaxtFitness(MAX_TFITNESS);
	setUnchangedGenerations(0);
	setRun(0);
	optimizing=false;
	active=true;
	unitsTotal=4;
	unitsTotalMax=4;
	timePercentage=0;
	goalPercentage=0;
};

ANARACE::~ANARACE()
{
};

// Reset all ongoing data (between two runs)
void EXPORT ANARACE::resetData() // resets all data to standard starting values
{
	setCurrentpFitness(0);
	for(int i=MAX_GOALS;i--;)
		setFinalTime(i,0);
	resetSpecial();

	for(int i=MAX_TIME;i--;)
	{
		setStatisticsHaveSupply(i,0);
		setStatisticsHaveMinerals(i,0);
		setStatisticsHaveGas(i,0);
		setStatisticsNeedSupply(i,0);
		setStatisticsFitness(i,0);
	}

	for(int i=MAX_LENGTH;i--;)//ANA~
	{
		setProgramSuccessType(i,0);
		setProgramSuccessUnit(i,0);
//		program[i].successLocation=0;
		setProgramIsBuilt(i,false);
		setProgramTime(i,MAX_TIME);
		for(int j=0;j<UNIT_TYPE_COUNT;j++)
		{
			setProgramAvailibleCount(i,j,0);
			setProgramTotalCount(i,j,0);
		}
		//program[i].temp=0;
		setProgramLocation(i,0);
//		setProgramIsGoal(i,false);
		setProgramFacility(i,0);
		setProgramBT(i,0);
		phaenoCode[i]=0;
	}
	setHarvestedGas(0);
	setHarvestedMinerals(0);
	setMinerals((*pStartCondition)->getMinerals());
	setGas((*pStartCondition)->getGas());
	setTimer(ga->maxTime-(*pStartCondition)->getStartTime());
	setNeedSupply((*pStartCondition)->getNeedSupply());
	setHaveSupply((*pStartCondition)->getHaveSupply());

	for(int i=4;i--;) //TODO
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
	lastcounter=4;
	setTimeOut(ga->maxTimeOut);
	setIP(ga->maxLength-1);
	ready=0;
}

// ----------------------------------------
// ------ CORE OF THE CORE FUNCTIONS ------
// ----------------------------------------

const bool EXPORT ANARACE::calculateStep()
{
//ZERG: CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
	//TODO: Fehler hier, getHaveSupply - getNeedSupply kann -1 werden!
	// needSupply war 11, maxneedSupply war 10 :/
	setStatisticsNeedSupply(getTimer(),getNeedSupply());
	setStatisticsHaveSupply(getTimer(),getHaveSupply());
	setStatisticsHaveMinerals(getTimer(),getMinerals());
	setStatisticsHaveGas(getTimer(),getGas());
	setStatisticsFitness(getTimer(),calculatePrimaryFitness(ready));

	if((!getTimer())||(ready=calculateReady())||(!getIP()))//||(((*pGoal)->bestTime*4<3*ga->maxTime)&&(4*getTimer()<3*(*pGoal)->bestTime))) //TODO calculateReady optimieren
	{
		setLength(ga->maxLength-getIP());
		if(!ready) setTimer(0);
		for(int i=MAX_LENGTH;i--;)
			phaenoCode[i]=(*pGoal)->toPhaeno(Code[i]);
	//	if((*pGoal)->getMode()==0)
			setCurrentpFitness(calculatePrimaryFitness(ready));
		buildingList.Clear();
		countUnitsTotal();
		return(true);
	}

	bool ok=true;
	while((ok)&&(getIP()))
	{
		neededMinerals=MAX_MINERALS;
		neededGas=MAX_GAS;
		ok=buildGene((*pGoal)->toPhaeno(Code[getIP()]));
		if(successType>0)
		{
			setProgramSuccessType(getIP(),successType);
//			setProgramSuccessLocation(successLocation);
			setProgramSuccessUnit(getIP(),successUnit);
		}
//PROBLEM: wenn Einheit gebaut wurde (also in die build list incl. IP eingefuegt wurde) aber gleichzeitig ein timeout war, wird die Einheit als TIMEOUT markiert aber trotzdem gebaut
// Problemloesung: Ueberpruefen, dass utnen auch wirklich das Mininimum gewaehlt fuer t gewaehlt wird! 
		if((ok)||(!getTimeOut()))
		{
			if(ok)
			{
				setProgramTime(getIP(),ga->maxTime-getTimer()); //ANA~
			}
			else 
			{
				setProgramTime(getIP(),ga->maxTime);
				setProgramSuccessType(getIP(),TIMEOUT_ERROR);
				setProgramSuccessUnit(getIP(),0);
//				setProgramSuccessLocation(0);
			}
//			program[getIP()].temp=location[1].availible[COMMAND_CENTER];
			for(int i=0;i<UNIT_TYPE_COUNT;i++)
			{
				setProgramTotalCount(getIP(),i,getLocationTotal(GLOBAL,i));
				setProgramAvailibleCount(getIP(),i,getLocationAvailible(0,i));
			}

//TODO Warum addiert er das hier und setzt es nicht einfach??

//TODO ueber getPlayer etc. den PhaenoCode rauszufinden erscheint mir etwas umstaendlich... evtl in "program" integrieren...
	
			setTimeOut(ga->maxTimeOut);
			setIP(getIP()-1);
		}
	}

	int t=calculateIdleTime();

	int mult=0;
	for(int i=getTimer()-t;i<getTimer();i++) {
			mult+=noise[i];
	}
	t=1;
//	mult=mult/t;
	mult=0;
	setMinerals(getMinerals()+harvestMinerals()*(100+mult)*t/100);
	setHarvestedMinerals(getHarvestedMinerals()+harvestMinerals()*(100+mult)*t/100);
	setGas(getGas()+harvestGas()*(100+mult)*t/100);
	setHarvestedGas(getHarvestedGas()+harvestGas()*(100+mult)*t/100);
	
	setTimeOut(getTimeOut()-t);
	setTimer(getTimer()-t);
	if(buildingList.isEmpty())
		return(false);
	BNODE* node = buildingList.GetHead();
	Building* build = node->GetData();
	while(node)
	{
		BNODE* oldnode=node;
		node=node->GetNext();
		build->setRemainingBuildTime(build->getRemainingBuildTime()-t);
		if(!build->getRemainingBuildTime())
		{
			const UNIT_STATISTICS* stat=&((*pStats)[build->getType()]);
			setProgramFacility(build->getIP(), build->getFacility());
			switch(stat->facilityType)
			{
				case NO_FACILITY:break;
				case IS_LOST:
					if(build->getFacility())
					{
						addLocationTotal(build->getLocation(),build->getFacility(),-1);
//						setProgramFacility(build->getIP(),build->getFacility());
						//availible was already taken account when starting the building
					}
					if(stat->facility2)
						addLocationTotal(build->getLocation(),stat->facility2,-1);
					break;
				case NEEDED_ONCE:break;//TODO ueberlegen ob man nicht jedem programdings ein facility zuweist... mmmh
				case NEEDED_UNTIL_COMPLETE:
					if(build->getFacility())
					{
						addLocationAvailible(build->getLocation(),build->getFacility(),1);
//						setProgramFacility(build->getIP(),build->getFacility());
					}
					break; // TODO: fuer spaeter mal: Wenn in 2 Fabriken produziert wuerde wirds probmelatisch, da
//in Buiding nur eine facility gespeichert wird...??
				case NEEDED_ONCE_IS_LOST:
					if(stat->facility2)
						addLocationTotal(build->getLocation(),stat->facility2,-1);
					break;
				case NEEDED_UNTIL_COMPLETE_IS_LOST:
					if(build->getFacility())
					{
						addLocationAvailible(build->getLocation(),build->getFacility(),1);
//						setProgramFacility(build->getIP(),build->getFacility());
//						setNeedSupply(getNeedSupply()-(*pStats)[build->getFacility()].needSupply); //<- ?
//						setHaveSupply(getHaveSupply()-(*pStats)[build->getFacility()].haveSupply); //<- ?
					}
					if(stat->facility2)
						addLocationTotal(GLOBAL/*build->getLocation()*/,stat->facility2,-1);
//r_researches need location 0~~ TODO
					break;
				case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
					{
						if(build->getFacility())
						{
							addLocationAvailible(build->getLocation(),build->getFacility(),1);
//							setProgramFacility(build->getIP(),build->getFacility()); //?
						}
						if(stat->facility2) // special rule for upgrades! need 0 location
						{
							addLocationTotal(GLOBAL/*build->getLocation()*/,stat->facility2,-1);
							addLocationAvailible(0/*build->getLocation()*/,stat->facility2,1);
						};
					}
					break;
				case NEEDED_ALWAYS:break;
				case NO_FACILITY_BEHAVIOUR_DEFINED:
				default:
				#ifdef _SCC_DEBUG
						toLog("ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
				#endif
						break;
			}

			setHaveSupply(getHaveSupply()+stat->haveSupply);
//			setNeedSupply(getNeedSupply()+stat->needSupply); ?

/*			if(stat->needSupply<0) 
			{
				if(getHaveSupply()-stat->needSupply>MAX_SUPPLY)
				{
					if(getHaveSupply()<MAX_SUPPLY)
					{
						setNeedSupply(getNeedSupply()+(MAX_SUPPLY-getHaveSupply()));
						setHaveSupply(MAX_SUPPLY);//problem beim abbrechen!
					}
				}
				else
				{
					setNeedSupply(getNeedSupply()-stat->needSupply);
					setHaveSupply(getHaveSupply()-stat->needSupply);
				}
			};*/
			//~~~~
			addLocationTotal(build->getLocation(),build->getType(),build->getUnitCount());
			addLocationAvailible(build->getLocation(),build->getType(),build->getUnitCount());

			if(build->getType()==REFINERY) 
			{
				addMapLocationTotal(GLOBAL, build->getLocation(),VESPENE_GEYSIR,-1);
				adjustGasHarvest(build->getLocation());
			}
			else
			if((build->getType()==COMMAND_CENTER)&&(!getLocationTotal(build->getLocation(),COMMAND_CENTER)))
			{
				adjustMineralHarvest(build->getLocation());
				adjustGasHarvest(build->getLocation());
			}
 else if((build->getType()==LARVA)&&((*pGoal)->getRace()==ZERG))
			{
				if(!buildGene(LARVA))
					larvaInProduction[build->getLocation()]--;
//					  if(
//TODO Was wenn Gebaeude zerstoert? etc.? eigentlich is das hier net noetig...
//								  (((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE))*3>
//									(larvaInProduction[tloc]+getLocationTotal(tloc, LARVA)))&&
//								   ((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE)<larvaInProduction[tloc])))) // => zuwenig Larven da!
//						  {
//							  if(buildGene(LARVA))
//								  larvaInProduction[tloc]++;
//						  }
//					  };
//				  };
			};

			last[lastcounter].unit=build->getType();
			last[lastcounter].count=build->getUnitCount();
			last[lastcounter].location=build->getLocation();

			if((stat->create)&&(!build->getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addLocationTotal(build->getLocation(),stat->create,1);
				addLocationAvailible(build->getLocation(),stat->create,1);
				if(last[lastcounter].unit==stat->create) last[lastcounter].count++; //TODO ???
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}
					//evtl noch location==0 als 'egal wo' einfuehren
			lastcounter++;

			//ANA~
			setProgramIsBuilt(build->getIP(),true);
			setProgramLocation(build->getIP(),build->getLocation());
			setProgramBT(build->getIP(),build->getTotalBuildTime()); //~~~
//			IP zeugs checken... length is immer 2 :/	 ??

				for(int i=MAX_GOALS;i--;)
// ist dieses goal belegt?
					if(((*pGoal)->goal[i].unit>0)&&
// befinden wir uns am richtigen Ort?
						(((*pGoal)->goal[i].location==0)||(build->getLocation()==(*pGoal)->goal[i].location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
						(build->getType()==(*pGoal)->goal[i].unit))
								setFinalTime(i,ga->maxTime-getTimer());
// Did we reach the right number at the right time?
//			  i=MAX_GOALS;  TODO? koennen wir mehrere goals gleichzeitig erfuell0rn?

			ready=calculateReady();
			buildingList.DeleteNode(oldnode);
/*			map<int, Building>::iterator temp=buildNode;
			temp++;
			buildingList.erase(buildNode);
			buildNode=temp;*/
		} // end while(getremainingbuildorder) ~|~
		if(node)
			build=(Building*)node->GetData();
//		else buildNode++;
	} //end while
//TODO: Alles rausschmeissen, was schon von race berechnet wurde!
	
	return(false);

	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
}

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)

const bool ANARACE::buildGene(const int unit)
{
	const UNIT_STATISTICS* stat=&((*pStats)[unit]);
	bool ok=false;

	successType=0;
	successUnit=0;
//	successLocation=0;

//	if(unit==1)
//		successType=0; //?

	if(unit<=EXTRACTOR+1)
	{
		if((stat->prerequisite[0]>0)&&(getLocationTotal(GLOBAL,stat->prerequisite[0])==0))
		{
			successUnit=stat->prerequisite[0];
//			successLocation=0;
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else		
		if((stat->prerequisite[1]>0)&&(getLocationTotal(GLOBAL,stat->prerequisite[1])==0))
		{
			successUnit=stat->prerequisite[1];
//			successLocation=0;
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else 
		if((stat->prerequisite[2]>0)&&(getLocationTotal(GLOBAL,stat->prerequisite[2])==0))
		{
			successUnit=stat->prerequisite[2];
//			successLocation=0;
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else
		if //ANA~
//TODO, < ((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)) einfuegen....
			( ((stat->facility[0]==0)||(getLocationAvailible(0,stat->facility[0])==0))&&
			  ((stat->facility[1]==0)||(getLocationAvailible(0,stat->facility[1])==0))&&
			  ((stat->facility[2]==0)||(getLocationAvailible(0,stat->facility[2])==0))&&
			  ((stat->facility[0]>0)||(stat->facility[1]>0)||(stat->facility[2]>0))
			)
 //TODO: availible/force[0] auf 100 setzen... naj ama guggn
		{
			if(stat->facility[0]>0)
				successUnit=stat->facility[0];
			else if(stat->facility[1]>0)
				successUnit=stat->facility[1];
			else if(stat->facility[2]>0)
				successUnit=stat->facility[2];
//			successLocation=0;
			successType=FACILITY_BECAME_AVAILIBLE;
		}
		else
		if((stat->facility2>0)&&(getLocationAvailible(0,stat->facility2)==0))
		{
			successUnit=stat->facility2;
//			successLocation=0;
			successType=FACILITY_BECAME_AVAILIBLE;
		}
//TODO: evtl success 2 Schritte zurueckverfolgen...
		else
		if(((getHaveSupply()<getNeedSupply()+stat->needSupply-stat->haveSupply)||(stat->needSupply+getNeedSupply()>MAX_SUPPLY))&&(stat->needSupply!=0))
		{
			successUnit=0;
//			  successLocation=0;
			successType=SUPPLY_WAS_SATISFIED;
		}
		else
		if(getMinerals()<stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL,unit))
		{
			successUnit=0;
//			successLocation=0;
			successType=ENOUGH_MINERALS_WERE_GATHERED;
			if(neededMinerals>stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL,unit)-getMinerals())
				neededMinerals=stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL,unit)-getMinerals();
//Fehler is wahrscheinlich hier irgendwoe...

		}
		else
		if(getGas()<stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL,unit))
		{
			successUnit=0;
//			successLocation=0;
			successType=ENOUGH_GAS_WAS_GATHERED;
			if(neededGas>stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL,unit)-getGas())
				neededGas=stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL,unit)-getGas();
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

//			if((stat->facility2==0)||(getLocationAvailible(tloc,stat->facility2)>0))
			for(fac=3;fac--;)
				if( ((stat->facility[fac]>0)&&(getLocationAvailible(tloc,stat->facility[fac])>((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)))) || ((stat->facility[fac]==0)&&(fac==0))) 
				{
					ok=true;
					break;
				}
		
			j=1;
			if(!ok)
				while(j<MAX_LOCATIONS)
				{
					ttloc=(*pMap)->getLocation(tloc)->getNearest(j);
//					if((stat->facility2==0)||(getLocationAvailible(ttloc,stat->facility2)>0))
//					{
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
//				bewegliche Sachen ueberdenken...
//					evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//					auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//		Phagen ueber Phagen...

			if((ok)&&(unit==REFINERY))
			{
				if(getMapLocationAvailible(0,tloc,VESPENE_GEYSIR)<=0)
					ok=false;
				else
					addMapLocationAvailible(0,tloc,VESPENE_GEYSIR,-1);
			};

			if(ok)
			{
					if((*pGoal)->getRace()==ZERG)
					{
						if((*pStats)[unit].facility[0]==LARVA)
						{
							if(
									(((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE))*3==
									  (larvaInProduction[tloc]+getLocationTotal(tloc, LARVA)))&&
									 ((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE)<larvaInProduction[tloc])))) // => zuwenig Larven da!
							{
								if(buildGene(LARVA))
									larvaInProduction[tloc]++;
							}
						};
					};
					
				/*Building build;
				build.setOnTheRun(0);
				build.setFacility(stat->facility[fac]);
				build.setLocation(tloc);
				build.setUnitCount(1);
//				build.setRemainingBuildTime(stat->BT//+3200*(stat->facility2==unit)*///); //~~ hack :/ TODO ???????????? SINN?
//				build.setTotalBuildTime(build.getRemainingBuildTime());
//				build.setType(unit);

					 BNODE *node=NULL;
					for (node = buildingList.GetTail();node&&(((Building*)node->GetData())->getRemainingBuildTime()<stat->BT/*+3200*(stat->facility2==unit)*/);node = node->GetPrev());
					Building* build=new Building();
					if(node&&(((Building*)node->GetData())->getRemainingBuildTime()>=stat->BT/*+3200*(stat->facility2==unit)*/))
						buildingList.Insert(node,build);
					else if(!node) buildingList.Append(build);

					build->setOnTheRun(false);
					build->setFacility(stat->facility[fac]);
					build->setLocation(tloc);
					build->setUnitCount(1);
					build->setRemainingBuildTime(stat->BT/*+3200*(stat->facility2==unit)*/); //~~ hack :/ TODO SINN???????
					build->setTotalBuildTime(build->getRemainingBuildTime());
					build->setType(unit);

					
				setMinerals(getMinerals()-(stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)));
				setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, unit)));
				setNeedSupply(getNeedSupply()+stat->needSupply);					 
//				setHaveSupply(getHaveSupply()-stat->needSupply);
				
//				if((stat->needSupply>0)||(((*pStats)[stat->facility[0]].needSupply<0)&&(stat->facilityType==IS_LOST))) 
//					setNeedSupply(getNeedSupply()-stat->needSupply);TODO
				adjustAvailibility(tloc,fac,stat);
				build->setIP(getIP());
				
//				buildingList.insert(pair<int, Building>(stat->BT/*+3200*(stat->facility2==unit)*/, build));
			} //kk!=1?
		} // if resources prere etc ok
	}
//TODO: error wenn facilities net gefunden...
	else // unit > EXTRACTOR+1
	{
	/*	int count=0;
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
			
					if((getIP()>1)&&((Code[0][getIP()-1]==unit)||(Code[1][getIP()-1]==unit)))
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
						if(building[n].type==EXTRACTOR)
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
void EXPORT ANARACE::removeOrder(const int IP)
{
		for(int j=IP;j--;)
		{
				Code[j+1]=Code[j];
				Marker[j+1]=Marker[j];
				program[j+1].built=program[j].built;
		}
	for(int j=0;j<MAX_LENGTH;j++)
		phaenoCode[j]=(*pGoal)->toPhaeno(Code[j]);
	(*pStartCondition)->wasChanged(); // to allow update of time etc. of anarace
};


void EXPORT ANARACE::insertOrder(int unit, int position)
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
	
	if((*pGoal)->allGoal[unit]==0)
	{
		(*pGoal)->addGoal(unit,1,0,0);
		(*pStartCondition)->changeAccepted();
	}
	replaceCode(i,(*pGoal)->toGeno(unit));
	
	program[i].built=1;

	for(int j=0;j<MAX_LENGTH;j++)
		phaenoCode[j]=(*pGoal)->toPhaeno(Code[j]); 
	(*pStartCondition)->wasChanged(); // to allow update of time etc. of anarace
}
*/
//void EXPORT ANARACE::backupMap()
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

//void EXPORT ANARACE::restoreMap()
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

const int ANARACE::maximum(const int unit) const
{
/*	int max=0;
	int t=0;
	if((stats[(*pGoal)->getRace()][unit].facility[0])&&(!getMap()->location[0].force[1][stats[(*pGoal)->getRace()][unit].facility[0]])&&(stats[(*pGoal)->getRace()][unit].facility[0]!=unit))
	{
		t=needTime(stats[(*pGoal)->getRace()][unit].facility[0]);
		if(t>max) max=t;
		t=0;
	}
		if((stats[(*pGoal)->getRace()][unit].facility[1])&&(!getMap()->location[0].force[1][stats[(*pGoal)->getRace()][unit].facility[1]]))
		{
				t=needTime(stats[(*pGoal)->getRace()][unit].facility[1]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[(*pGoal)->getRace()][unit].facility[2])&&(!getMap()->location[0].force[1][stats[(*pGoal)->getRace()][unit].facility[2]]))
		{
				t=needTime(stats[(*pGoal)->getRace()][unit].facility[2]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[(*pGoal)->getRace()][unit].prerequisite[0])&&(!getMap()->location[0].force[1][stats[(*pGoal)->getRace()][unit].prerequisite[0]]))
		{
				t=needTime(stats[(*pGoal)->getRace()][unit].prerequisite[0]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[(*pGoal)->getRace()][unit].prerequisite[1])&&(!getMap()->location[0].force[1][stats[(*pGoal)->getRace()][unit].prerequisite[1]]))
		{
				t=needTime(stats[(*pGoal)->getRace()][unit].prerequisite[1]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[(*pGoal)->getRace()][unit].prerequisite[2])&&(!getMap()->location[0].force[1][stats[(*pGoal)->getRace()][unit].prerequisite[2]]))
		{
				t=needTime(stats[(*pGoal)->getRace()][unit].prerequisite[2]);
				if(t>max) max=t;
				t=0;
		}
	return(max);*/
	return(0);
};

const int ANARACE::needTime(const int unit) const
{
//	if(!getMap()->location[0].force[1][unit])
//		return(stats[(*pGoal)->getRace()][unit].BT+maximum(unit)); //eigene Bauzeit + Bauzeit der Prerequisites/Facilities
//	else return(0);
	return(0);
};

void EXPORT ANARACE::countUnitsTotal()
{
	unitsTotal = 0;
	unitsTotalMax = 4;
	for (int i = GAS_SCV; i--;)
	{
		if (getLocationTotal(GLOBAL, i) > unitsTotalMax)
			unitsTotalMax = getLocationTotal(GLOBAL, i);
		if ((*pGoal)->allGoal[i] > unitsTotalMax)
			unitsTotalMax = (*pGoal)->allGoal[i];
		unitsTotal += getLocationTotal(GLOBAL, i);;
	}
};

// -----------------------------------
// ------ END OF MISC FUNCTIONS ------
// -----------------------------------

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const int EXPORT ANARACE::getUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if((unitsTotalMax<0)||(unitsTotalMax>MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::getUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotalMax);
};

const int EXPORT ANARACE::getUnitsTotal() const
{
#ifdef _SCC_DEBUG
	if((unitsTotal<0)||(unitsTotal>MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::getUnitsTotal): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotal);
};

const bool EXPORT ANARACE::isActive() const
{
	return(active);
};

void EXPORT ANARACE::setActive(const bool active)
{
	this->active=active;
};

const bool EXPORT ANARACE::isOptimizing() const
{
	return(optimizing);
};

void EXPORT ANARACE::setOptimizing(const bool optimizing)
{
	this->optimizing=optimizing;
};

const int EXPORT ANARACE::getMarker(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getMarker): Value IP out of range.");return(0);
	}
#endif
	return(Marker[IP]); 
};

/*const int EXPORT ANARACE::getProgramSuccessLocation(cons tint IP) const
{
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessLocation): Value IP [%i] out of range.",IP);return(0);
	}
	if((program[IP].successLocation<0)||(program[IP].successLocation>=MAX_LOCATIONS)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessLocation): Variable not initialized [%i].",program[IP].successLocation);return(0);
	}
	return(program[IP].successLocation);
};*/

const int EXPORT ANARACE::getProgramCode(int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramCode): Value IP out of range.");return(0);
	}
	if((Code[IP]<0)||(Code[IP]>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::getProgramCode): Variable Code not initialized.");return(0);
	}
#endif					
	return(Code[IP]);
}

const int EXPORT ANARACE::getProgramFacility(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Value IP out of range.");return(0);
	}
	if((program[IP].facility<0)||(program[IP].facility>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].facility);
}

const int EXPORT ANARACE::getProgramBT(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramBT): Value IP out of range.");return(0);
	}
	if((program[IP].BT<0)||(program[IP].BT>=MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getProgramBT): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].BT);
}																	  

const int EXPORT ANARACE::getProgramSuccessType(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Value IP out of range.");return(0);
	}
	if((program[IP].successType<0)||(program[IP].successType>=ERROR_MESSAGES)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].successType);
};

const int EXPORT ANARACE::getProgramSuccessUnit(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Value IP out of range.");return(0);
	}
	if((program[IP].successUnit<0)||(program[IP].successUnit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].successUnit);
};

/*void EXPORT ANARACE::setProgramIsConstant(const int IP, const bool isConstant)
{
		if((IP<0)||(IP>=MAX_LENGTH)) {
				toLog("DEBUG: (ANARACE::setProgramIsConstant): Value IP [%i] out of range.",IP);return(0);
		}
		program[IP].isConstant=isConstant;
};*/

void EXPORT ANARACE::setProgramAvailibleCount(const int IP, const int unit, const int count)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value IP out of range.");return;
	}
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value unit out of range.");return;
	}
	if((count<0)||(count>=MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value count out of range.");return;
	}
#endif
	program[IP].availibleCount[unit]=count;
};

void EXPORT ANARACE::setProgramTotalCount(const int IP, const int unit, const int count)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value IP out of range.");return;
	}
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value unit out of range.");return;
	}
	if((count<0)||(count>=MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value count out of range.");return;
	}
#endif
	program[IP].forceCount[unit]=count;
};

void EXPORT ANARACE::setStatisticsNeedSupply(const int time, const int needSupply)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::setStatisticsNeedSupply): Value time out of range.");return;
	}
	if((needSupply<0)||(needSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::setStatisticsNeedSupply): Value needSupply out of range.");return;
	}
#endif
	statistics[time].needSupply=needSupply;
};
void EXPORT ANARACE::setStatisticsHaveSupply(const int time, const int needSupply)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveSupply): Value time out of range.");return;
	}
	if((needSupply<0)||(needSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveSupply): Value needSupply out of range.");return;
	}
#endif
	statistics[time].haveSupply=needSupply;
};

void EXPORT ANARACE::setProgramTime(const int IP, const int time)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value IP out of range.");return;
	}
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value time out of range.");return;
	}
#endif
	program[IP].time=time;
};

void EXPORT ANARACE::setStatisticsHaveMinerals(const int time, const int minerals)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveMinerals): Value time out of range.");return;
	}
	if((minerals<0)||(minerals>=MAX_MINERALS)) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveMinerals): Value minerals out of range.");return;
	}
#endif
	statistics[time].minerals=minerals;
};
																				
void EXPORT ANARACE::setStatisticsHaveGas(const int time, const int gas)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveGas): Value time out of range.");return;
	}
	if((gas<0)||(gas>=MAX_GAS)) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveGas): Value minerals out of range.");return;
	}
#endif
	statistics[time].gas=gas;
};
void EXPORT ANARACE::setProgramBT(const int IP, const int time)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value IP out of range.");return;
	}
	if((time<0)||(time>=MAX_TIME)) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value time out of range.");return;
	}
#endif
	program[IP].BT=time;
};

void EXPORT ANARACE::setProgramIsBuilt(const int IP, const bool isBuilt)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramIsBuilt): Value IP out of range.");return;
	}
#endif
	program[IP].built=isBuilt;
};

void EXPORT ANARACE::setProgramLocation(const int IP, const int location)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value IP out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value location out of range.");return;
	}
#endif
	program[IP].location=location;
}

void EXPORT ANARACE::setStatisticsFitness(const int time, const int fitness)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::setStatisticsFitness): Value time out of range.");return;
	}
	if((fitness<0)||(fitness>100000)) { // TODO maxfitness
		toLog("DEBUG: (ANARACE::setStatisticsFitness): Value fitness out of range.");return;
	}
#endif
	statistics[time].fitness=fitness;
}

const int EXPORT ANARACE::getProgramTotalCount(const int IP, const int unit) const	   // how many units of the type at phaenoCode[s] do exist at that time?
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value IP out of range.");return(0);
	}
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value unit out of range.");return(0);
	}
#endif
	return(program[IP].forceCount[unit]);
};

// how many units of the type at phaenoCode[s] do exist at that time?
const int EXPORT ANARACE::getProgramAvailibleCount(const int IP, const int unit) const	   
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value IP out of range.");return(0);
	}
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value unit out of range.");return(0);
	}
#endif
	return(program[IP].availibleCount[unit]);
};

void EXPORT ANARACE::setProgramFacility(const int IP, const int facility)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value IP out of range.");return;
	}
	if((facility<0)||(facility>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value facility out of range.");return;
	}
#endif
	program[IP].facility=facility;
};

void EXPORT ANARACE::setProgramSuccessType(const int IP, const int type)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value IP out of range.");return;
	}
	if((type<0)||(type>=ERROR_MESSAGES)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value type out of range.");return;
	}
#endif
	program[IP].successType=type;
};

void EXPORT ANARACE::setProgramSuccessUnit(const int IP, const int unit)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value IP out of range.");return;
	}
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value unit out of range.");return;
	}
#endif
	program[IP].successUnit=unit;
};

/*void EXPORT ANARACE::setProgramSuccessLocation(const int num)
{
	if((num<0)||(num>=MAX_LOCATIONS)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessLocation): Value out of range.");return(0);
	}
	program[getIP()].successLocation=num;
};*/

const int EXPORT ANARACE::getStatisticsNeedSupply(const int time) const
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getStatisticsNeedSupply): Value time [%i] out of range.");return(0);
	}
	if((statistics[time].needSupply<0)||(statistics[time].needSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::getStatisticsNeedSupply): Variable not initialized [%i].");	return(0);
	}
#endif
	return(statistics[time].needSupply);
};

const int EXPORT ANARACE::getStatisticsHaveSupply(const int time) const
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveSupply): Value time out of range.");return(0);
	}
	if((statistics[time].haveSupply<0)||(statistics[time].haveSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveSupply): Variable not initialized.");return(0);
	}
#endif
	return(statistics[time].haveSupply);
};
const bool EXPORT ANARACE::getProgramIsBuilt(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsBuilt): Value IP out of range.");return(0);
	}
#endif
	return(program[IP].built);
};

const bool EXPORT ANARACE::getProgramIsBuilt2(const int IP) const
{
	return(getProgramIsBuilt(IP));
};

/*const bool EXPORT ANARACE::getProgramIsConstant(const int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsConstant): Value IP [%i] out of range.",IP);return(false);
	}
	return(program[IP].isConstant);
};*/

const int EXPORT ANARACE::getProgramTime(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTime): Value IP out of range.");return(0);
	}
	if((program[IP].time<0)||(program[IP].time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getProgramTime): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].time);
};

const int EXPORT ANARACE::getProgramTemp(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTemp): Value IP out of range.");return(0);
	}
#endif
	return(program[IP].temp);
};

const int EXPORT ANARACE::getStatisticsHaveMinerals(const int time) const
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveMinerals): Value time out of range.");return(0);
	}
	if((statistics[time].minerals<0)||(statistics[time].minerals>MAX_MINERALS)) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveMinerals): Variable not initialized.");return(0);
	}
#endif
	return(statistics[time].minerals);
};

const int EXPORT ANARACE::getStatisticsHaveGas(const int time) const
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveGas): Value timeout of range.");return(0);
	}
	if((statistics[time].gas<0)||(statistics[time].gas>MAX_GAS)) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveGas): Variable statistics.gas not initialized.");return(0);
	}
#endif
	return(statistics[time].gas);
};

const int EXPORT ANARACE::getProgramLocation(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Value IP out of range.");return(0);
	}
	if((program[IP].location<0)||(program[IP].location>MAX_LOCATIONS)) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Variable program.location not initialized.");return(0);
	}
#endif
	return(program[IP].location);
};

const int EXPORT ANARACE::getStatisticsFitness(const int time) const
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getStatisticsFitness): Value time out of range.");return(0);
	}
	// TODO max pfitness 
	if((statistics[time].fitness<0)||(statistics[time].fitness>100000)) {
		toLog("DEBUG: (ANARACE::getStatisticsFitness): Variable statistics.fitness not initialized.");return(0);
	}
#endif
	return(statistics[time].fitness);
};
const int EXPORT ANARACE::getTimePercentage() const
{
	return(timePercentage);
};

const int EXPORT ANARACE::getGoalPercentage() const
{
	return(goalPercentage);
};

void EXPORT ANARACE::setPhaenoCode(const int IP, const int num)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value IP out of range.");return;
	}
	if((num<0)||(num>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value num out of range.");return;
	}
#endif
	phaenoCode[IP]=num;
};

const int EXPORT ANARACE::getPhaenoCode(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getPhaenoCode): Value IP out of range.");return(0);
	}
#endif
	return(phaenoCode[IP]);
};

const int EXPORT ANARACE::getCurrentpFitness() const
{
	return(currentpFitness);
};

void ANARACE::setCurrentpFitness(const int num)
{
	currentpFitness=num;
};

const int EXPORT ANARACE::getUnchangedGenerations() const
{
#ifdef _SCC_DEBUG	
	if((unchangedGenerations<0)||(unchangedGenerations>MAX_GENERATIONS)) {
		toLog("DEBUG: (ANARACE::getUnchangedGenerations): Variable unchangedGenerations not initialized.");return(0);
	}
#endif
	return(unchangedGenerations);
};

const int EXPORT ANARACE::getRun() const
{
#ifdef _SCC_DEBUG	
	if((currentRun<0)||(currentRun>MAX_RUNS)) {
		toLog("DEBUG: (ANARACE::getRun): Variable currentRun not initialized.");return(0);
	}
#endif
	return(currentRun);
};

const int EXPORT ANARACE::getGeneration() const
{
#ifdef _SCC_DEBUG	
	//TODO ga->maxgeneration?
	if((generation<0)||(generation>MAX_GENERATIONS)) {
		toLog("DEBUG: (ANARACE::getGeneration): Variable generation not initialized.");return(0);
	}
#endif
	return(generation);
};

const int EXPORT ANARACE::getMaxpFitness() const
{
#ifdef _SCC_DEBUG	
// TODO max pfitness?
	if((maxpFitness<0)||(maxpFitness>50000)) {
		toLog("DEBUG: (ANARACE::getMaxpFitness): Variable maxpFitness not initialized.");return(0);
	}
#endif
	return(maxpFitness);
};

const int EXPORT ANARACE::getMaxsFitness() const
{
#ifdef _SCC_DEBUG	
	if((maxsFitness<0)||(maxsFitness>MAX_MINERALS+MAX_GAS)) {
		toLog("DEBUG: (ANARACE::getMaxsFitness): Variable maxsFitness not initialize.");return(0);
	}
#endif
	return(maxsFitness);
};

const int EXPORT EXPORT ANARACE::getMaxtFitness() const
{
#ifdef _SCC_DEBUG	
	if((maxtFitness<0)||(maxtFitness>MAX_TFITNESS)) {
		toLog("DEBUG: (ANARACE::getMaxtFitness): Variable maxtFitness not initialized.");return(0);
	}
#endif
	return(maxtFitness);
};

void EXPORT ANARACE::setUnchangedGenerations(const int unchangedGenerations)
{
#ifdef _SCC_DEBUG	
//	if((num<0)||(num>ga->maxGenerations))
	if((unchangedGenerations<0)||(unchangedGenerations>MAX_GENERATIONS)) {
		toLog("DEBUG: (ANARACE::setUnchangedGenerations): Value num out of range.");return;
	}
#endif
	this->unchangedGenerations=unchangedGenerations;
};

void EXPORT ANARACE::setRun(const int run)
{
#ifdef _SCC_DEBUG
	if((run<0)||(run>MAX_RUNS)) {
		toLog("DEBUG: (ANARACE::setRun): Value run out of range.");return;
	}
#endif
	currentRun=run;
};

void EXPORT ANARACE::setGeneration(const int generation)
{
#ifdef _SCC_DEBUG
//	if((num<0)||(num>ga->maxGenerations))
	if((generation<0)||(generation>MAX_GENERATIONS)) {
		toLog("DEBUG: (ANARACE::setGeneration): Value out of range.");return;
	}
#endif
	this->generation=generation;
};

void EXPORT ANARACE::setMaxpFitness(const int maxpFitness) 
{
#ifdef _SCC_DEBUG
	// TODO maxpitness
	if((maxpFitness<0)||(maxpFitness>50000)) {
		toLog("DEBUG: (ANARACE::setMaxpFitness): Value out of range.");return;
	}
#endif
	this->maxpFitness=maxpFitness;
};

void EXPORT ANARACE::setMaxsFitness(const int maxsFitness)
{
#ifdef _SCC_DEBUG
	if((maxsFitness<0)||(maxsFitness>MAX_MINERALS+MAX_GAS)) {
		toLog("DEBUG: (ANARACE::setMaxsFitness): Value out of range.");return;
	}
#endif
	this->maxsFitness=maxsFitness;
};

void EXPORT ANARACE::setMaxtFitness(const int maxtFitness)
{
#ifdef _SCC_DEBUG
	if((maxtFitness<0)||(maxtFitness>MAX_TFITNESS)) {
		toLog("DEBUG: (ANARACE::setMaxtFitness): Value out of range.");return;
	}
#endif
	this->maxtFitness=maxtFitness;
};

/*void EXPORT ANARACE::analyzeBuildOrder()
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
			tGoal[i]+=(*pGoal)->globalGoal[j][i]-pStartcondition->getLocationTotal(j,i);
	}

	for(int i=0;i<MAX_LENGTH;i++)
		if(tGoal[phaenoCode[i]]>0) //~~~~~~ location=0?
		{
			tGoal[phaenoCode[i]]--;
			setProgramIsGoal(i,true);
		}
	else setProgramIsGoal(i,false);
};*/


int EXPORT ANARACE::successType;
int EXPORT ANARACE::successUnit;
//int EXPORT ANARACE::successLocation;

