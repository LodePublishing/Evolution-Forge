#include <stdlib.h>
#include <string.h>
#include "race.h"
#include "debug.h"


int RACE::calculateSecondaryFitness()
{
	int tsF=getHarvestedMins()+getHarvestedGas();
	//TODO: evtl gas und mins (wie urspruenglich eigentlich) in Verhaeltnis setyen wieviel es jeweils Geysire/Mineralien gibt...	
		for(int i=0;i<=GAS_SCV;i++)
			if((getPlayer()->getGoal()->allGoal[i]>0)&&(getPlayer()->getGoal()->allGoal[i]+getMap()->getLocationForce(0,getPlayerNum(),i)<getLocationForce(0,i)))
				tsF-=(getLocationForce(0,i)-getMap()->getLocationForce(0,getPlayerNum(),i)-getPlayer()->getGoal()->allGoal[i])*(stats[getPlayer()->getGoal()->getRace()][i].gas+stats[getPlayer()->getGoal()->getRace()][i].mins);
	return(tsF);
};


// TODO: reimplement/recheck the speed of the units
int RACE::calculateStep()
{
//ZERG:  CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
//mins, gas hier rein...
	if((!getTimer())||(ready=calculateReady())||(!getIP())||((getPlayer()->getGoal()->bestTime*4<ga->maxTime*3)&&(4*getTimer()<3*getPlayer()->getGoal()->bestTime))) //TODO calculateReady optimieren
	{
		setLength(ga->maxLength-getIP());
		if(!ready) setTimer(0);
		settFitness(gettFitness()-getLength());
		setsFitness(calculateSecondaryFitness());
		if(getPlayer()->getGoal()->getMode()==0)
			setpFitness(calculatePrimaryFitness(ready));
		buildingList.Clear();
		return(1);
	}
	int ok=1;
	int first=1;

//	for(int i=MAX_LOCATIONS;i--;)
//		for(int j=UNIT_TYPE_COUNT;j--;)
//			if((stats[getPlayer()->getGoal()->getRace()][j].autobuild))&&(getLocationForce(i, stats[getPlayer()->getGoal()->getRace()][j].autobuild))
//			{
//
//			}

	while((ok)&&(getIP()))
	{
		//TODO: Variable einfuegen, damit Benutzer z.B. Befehle pro Zeiteinheit (oder pro Location!?) einstellen kann
		neededMinerals=99999;
		neededGas=99999;
		ok=buildGene(getPlayer()->getGoal()->toPhaeno(Code[getIP()]));
		if((ok)||(!getTimeOut()))
		{
			if(!first)
				settFitness(gettFitness()-1);
			first=0;
			setIP(getIP()-1);
			if(!getTimeOut())
				settFitness(gettFitness()-5);
			setTimeOut(ga->maxTimeOut);
		};
	}

	int t=getTimer();
	BNODE* node=buildingList.GetHead();
	Building* build=0;
//	if(!buildingList.empty())
	if(node)
	{
		build=node->GetData();
//		int buildingRemaining=min_element(buildingList.begin(),buildingList.end())->second.getRemainingBuildTime();
		int buildingRemaining=build->getRemainingBuildTime();
		if(t>buildingRemaining) t=buildingRemaining;
	}
	
	if((harvestGas()>0)&&((harvestMinerals()==0)||(neededGas+(harvestGas()-neededGas%harvestGas()))/harvestGas() < (neededMinerals+(harvestMinerals()-neededMinerals%harvestMinerals()))/harvestMinerals()))
	{
		int gasRemaining=(neededGas+(harvestGas()-neededGas%harvestGas()))/harvestGas();
		if(t>gasRemaining) t=gasRemaining;
	}
	else if(harvestMinerals()>0)
	{
		int mineralsRemaining=(neededMinerals+(harvestMinerals()-neededMinerals%harvestMinerals()))/harvestMinerals();
		if(t>mineralsRemaining) t=mineralsRemaining;
	}
	if(t>getTimeOut()) t=getTimeOut();
//	if((getTimeOut()>1)&&(t==getTimeOut()))
//		t=getTimeOut()-1;

// TODO t wird da oben irgendwo null wenn ich zwischen den Spalten schalte
	int mult=0;
	for(int i=getTimer()-t;i<getTimer();i++)
	{
		mult+=noise[i];
	}
	mult=mult/t;

	setMins(getMins()+harvestMinerals()*(100+mult)*t/100);
		setHarvestedMins(getHarvestedMins()+harvestMinerals()*(100+mult)*t/100);
		setGas(getGas()+harvestGas()*(100+mult)*t/100);
		setHarvestedGas(getHarvestedGas()+harvestGas()*(100+mult)*t/100);

	setTimeOut(getTimeOut()-t);
	setTimer(getTimer()-t);

	while(node)
	{
		BNODE* oldnode=node;
		node=node->GetNext();
		build->setRemainingBuildTime(build->getRemainingBuildTime()-t);
		if(!build->getRemainingBuildTime())
		{
			const UNIT_STATISTICS* stat=&pStats[build->getType()];
			switch(stat->facilityType)
			{
				case NO_FACILITY:break;
				case IS_LOST:
					if(build->getFacility())
						addLocationForce(build->getLocation(),build->getFacility(),-1);
						//availible was already taken account when starting the building
					if(stat->facility2)
						addLocationForce(build->getLocation(),stat->facility2,-1);
					break;
				case NEEDED_ONCE:break;
				case NEEDED_UNTIL_COMPLETE:
					if(build->getFacility())
						addLocationAvailible(build->getLocation(),build->getFacility(),1);
					break; // fuer spaeter mal: Wenn in 2 Fabriken produziert wuerde wirds probmelatisch, da
//in Buiding nur eine facility gespeichert wird...
				case NEEDED_ONCE_IS_LOST:
					if(stat->facility2)
						addLocationForce(build->getLocation(),stat->facility2,-1);
					break;
				case NEEDED_UNTIL_COMPLETE_IS_LOST:
					if(build->getFacility())
					{
						addLocationAvailible(build->getLocation(),build->getFacility(),1);
						setSupply(getSupply()+pStats[build->getFacility()].supply); //<- ?
					}					
					if(stat->facility2)
						addLocationForce(0/*build->getLocation()*/,stat->facility2,-1);
//r_researches need location 0
					break;
				case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
					{
						if(build->getFacility())
							addLocationAvailible(build->getLocation(),build->getFacility(),1);
						if(stat->facility2) // special rule for upgrades!
						{
							addLocationForce(0/*build->getLocation()*/,stat->facility2,-1);
							addLocationAvailible(0/*build->getLocation()*/,stat->facility2,1);
						};
					}
					break;
				case NEEDED_ALWAYS:break;
				case NO_FACILITY_BEHAVIOUR_DEFINED:
				default:
						debug.toLog(0,"ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
						break;
			} // end switch facilityType

			if(stat->supply<0)
			{ //unitcount?
				if(getMaxSupply()-stat->supply>MAX_SUPPLY)
				{
					if(getMaxSupply()<MAX_SUPPLY)
					{
						setSupply(getSupply()+(MAX_SUPPLY-getMaxSupply()));
						setMaxSupply(MAX_SUPPLY);//problem beim abbrechen!
					}
				}
				else
				{
					setSupply(getSupply()-stat->supply);
					setMaxSupply(getMaxSupply()-stat->supply);
				}
			};
			//~~~~
			addLocationForce(build->getLocation(),build->getType(),build->getUnitCount());
			addLocationAvailible(build->getLocation(),build->getType(),build->getUnitCount());

			if(build->getType()==REFINERY) 
			{
				addMapLocationForce(0,build->getLocation(),VESPENE_GEYSIR,-1);
				adjustGasHarvest(build->getLocation());
				//TODO: ein scv zum Gas schicken... ueber buildgene? oder manuell hier?
			}
			else
			if((build->getType()==COMMAND_CENTER)&&(!getLocationForce(build->getLocation(),COMMAND_CENTER)))
			{
				adjustMineralHarvest(build->getLocation());
				adjustGasHarvest(build->getLocation());
			} else if((build->getType()==LARVA)&&(getPlayer()->getGoal()->getRace()==ZERG))
			{
				if(!buildGene(LARVA))
					larvaInProduction[build->getLocation()]--;
//						if(
//TODO Was wenn Gebaeude zerstoert? etc.? eigentlich is das hier net noetig...
//									(((getLocationForce(tloc, HATCHERY)+getLocationForce(tloc, LAIR)+getLocationForce(tloc, HIVE))*3>
//									  (larvaInProduction[tloc]+getLocationForce(tloc, LARVA)))&&
//									 ((getLocationForce(tloc, HATCHERY)+getLocationForce(tloc, LAIR)+getLocationForce(tloc, HIVE)<larvaInProduction[tloc])))) // => zuwenig Larven da!
//							{
//								if(buildGene(LARVA))
//									larvaInProduction[tloc]++;
//							}
//						};
//					};
			};
				
			last[lastcounter].unit=build->getType();
			last[lastcounter].count=build->getUnitCount();
			last[lastcounter].location=build->getLocation();

			if((stat->create)&&(!build->getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addLocationForce(build->getLocation(),stat->create,1);
				addLocationAvailible(build->getLocation(),stat->create,1);
				if(last[lastcounter].unit==stat->create) last[lastcounter].count++; //TODO ???
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}

			lastcounter++;

// berechne 'FinalTime', die angibt, wann alle Einheiten (bzw. die letzte Einheit) unsereres Zieles fertiggestellt wurden
// Quasi wie "ready" nur ohne Zeit
//TODO CHECKEN!!

				for(int i=MAX_GOALS;i--;)
// ist dieses goal belegt?
						if((getPlayer()->getGoal()->goal[i].unit>0)&&
// befinden wir uns am richtigen Ort?
						((getPlayer()->getGoal()->goal[i].location==0)||(build->getLocation()==getPlayer()->getGoal()->goal[i].location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
						(build->getType()==getPlayer()->getGoal()->goal[i].unit))
								setFinalTime(i,ga->maxTime-getTimer());
// Did we reach the right number at the right time?
//			  i=MAX_GOALS;  TODO? koennen wir mehrere goals gleichzeitig erfuell0rn?
			ready=calculateReady();
			buildingList.DeleteNode(oldnode);
// oder: irgendeine location... TODO: Problem: die Einheiten koennen irgendwo sein, also nicht gesammelt an einem Fleck...
/*           	map<int, Building>::iterator temp=buildNode;
           	temp++;
			buildingList.erase(buildNode);
           	buildNode=temp;*/
			
		} // END of if(!build->getRemainingBuildTime())
//		else buildNode++;
		if(node) 
			build=(Building*)node->GetData();
	} // END of while
	return(0);
	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
};

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)


//FEHLER MUSS HIER SEIN!! TODO

//TODO: pFitness wird total falsch berechnet fuer races die nicht alle Ziele erfuellt haben!1 [location problem halt...]


int RACE::buildGene(int unit)
{
	const UNIT_STATISTICS* stat=&pStats[unit];
	int ok=0;
	if(unit<=EXTRACTOR+1)
	{
		//TODO: Array und testen wo der comp am meisten haengenbleibt und abbricht... moeglichst dann nach oben bringen!
		if(
			((stat->prerequisite[0]==0)||(getLocationForce(0,stat->prerequisite[0])>0))&&
			((stat->prerequisite[1]==0)||(getLocationForce(0,stat->prerequisite[1])>0))&&
			((stat->prerequisite[2]==0)||(getLocationForce(0,stat->prerequisite[2])>0))
			
			&&
			( ((stat->facility[0]>0)&&(getLocationAvailible(0,stat->facility[0])>0))||
			  ((stat->facility[1]>0)&&(getLocationAvailible(0,stat->facility[1])>0))||
			  ((stat->facility[2]>0)&&(getLocationAvailible(0,stat->facility[2])>0))||
			  ((stat->facility[0]==0)&&(stat->facility[1]==0)&&(stat->facility[2]==0))
			)
			&&
			((getSupply()>=stat->supply)||(stat->supply<=0)) // TODO: IS_LOST einfuegen!
			&&
			((stat->facility2==0)||(getLocationAvailible(0,stat->facility2)>0)))
		{
		
			if
			(getMins()<stat->mins+stat->upgrade_cost*getLocationForce(0,unit))
			{
				if(neededMinerals>stat->mins+stat->upgrade_cost*getLocationForce(0,unit)-getMins())
					neededMinerals=stat->mins+stat->upgrade_cost*getLocationForce(0,unit)-getMins();
			}
			else
			if(getGas()<stat->gas+stat->upgrade_cost*getLocationForce(0,unit))
			{
				if(neededGas>stat->gas+stat->upgrade_cost*getLocationForce(0,unit)-getGas())
					neededGas=stat->gas+stat->upgrade_cost*getLocationForce(0,unit)-getGas();
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
						ok=1;
						break;
					}

				j=1;
				if(!ok)
					while(j<MAX_LOCATIONS)
					{
						ttloc=pMap->getNearestLocation(tloc,j);
//						if((stat->facility2==0)||(getLocationAvailible(ttloc,stat->facility2)>0)) TODO
//						{
						for(fac=3;fac--;)
							if( ((stat->facility[fac]>0)&&(getLocationAvailible(ttloc,stat->facility[fac])>((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)))) || ((stat->facility[fac]==0)&&(fac==0)))
							{
								tloc=ttloc;
								ok=1;
								break;
							}
						break;
//					}	
						j++;
					}

				if((ok)&&(unit==REFINERY))
				{
					if(getMapLocationAvailible(0,tloc,VESPENE_GEYSIR)<=0)
						ok=0;
					else 
						addMapLocationAvailible(0,tloc,VESPENE_GEYSIR,-1);
				}
//TODO: Wenn verschiedene facilities moeglich sind, dann das letzte nehmen						
//				bewegliche Sachen ueberdenken...
//					evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//					auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//		Phagen ueber Phagen...			
				if(ok)
				{
					if(getPlayer()->getGoal()->getRace()==ZERG)
					{
						if(pStats[unit].facility[0]==LARVA)
						{
							if(
									(((getLocationForce(tloc, HATCHERY)+getLocationForce(tloc, LAIR)+getLocationForce(tloc, HIVE))*3==
									  (larvaInProduction[tloc]+getLocationForce(tloc, LARVA)))&&
									 ((getLocationForce(tloc, HATCHERY)+getLocationForce(tloc, LAIR)+getLocationForce(tloc, HIVE)<larvaInProduction[tloc])))) // => zuwenig Larven da!
							{
								if(buildGene(LARVA))
									larvaInProduction[tloc]++;
							}
					
						};
					};
                                                                                                                                                            
                    BNODE *node=NULL;
                    for (node = buildingList.GetTail();node&&(((Building*)node->GetData())->getRemainingBuildTime()<stat->BT/*+3200*(stat->facility2==unit)*/);node = node->GetPrev());
                    Building* build=new Building();
                    if(node&&(((Building*)node->GetData())->getRemainingBuildTime()>=stat->BT/*+3200*(stat->facility2==unit)*/))
                        buildingList.Insert(node,build);
                    else if(!node) buildingList.Append(build);

                    build->setOnTheRun(0);
                    build->setFacility(stat->facility[fac]);
                    build->setLocation(tloc);
                    build->setUnitCount(1);
                    build->setRemainingBuildTime(stat->BT/*+3200*(stat->facility2==unit)*/); //~~ hack :/ TODO SINN???????
                    build->setTotalBuildTime(build->getRemainingBuildTime());
                    build->setType(unit);

					
					if(getMins()*3<4*stat->mins+stat->upgrade_cost*getLocationForce(0,unit)) settFitness(gettFitness()-2);
					if(getGas()*3<4*stat->gas+stat->upgrade_cost*getLocationForce(0,unit)) settFitness(gettFitness()-2);
					if((stat->supply>0)&&(getSupply()*4<5*stat->supply)) settFitness(gettFitness()-2); 
					if((getMins()*5/4<stat->mins+stat->upgrade_cost*getLocationForce(0,unit))||
					   (getGas()*5/4<stat->gas+stat->upgrade_cost*getLocationForce(0,unit)))
						settFitness(gettFitness()-1);
					if(lastunit==0) lastunit=unit;
					if(unit!=lastunit)//~~
					{
						settFitness(gettFitness()-1);
						lastunit=unit;
					}
// upgrade_cost is 0 if it's no upgrade
					setMins(getMins()-(stat->mins+stat->upgrade_cost*getLocationForce(0,unit)));
					setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationForce(0,unit)));
	
					
//					build.setIP(getIP()); needed only for Anarace!
					if((stat->supply>0)||((pStats[stat->facility[0]].supply<0)&&(stat->facilityType==IS_LOST))) 
						setSupply(getSupply()-stat->supply); //? Beschreibung!
					adjustAvailibility(tloc, fac, stat);
                //	buildingList.insert(pair<int, Building>(stat->BT/*+3200*(stat->facility2==unit)*/, build));
				} //end if(ok)
			} //end mins/gas else
		} //end prere/fac else
	} //end unit < EXTRACTOR
	else // unit > EXTRACTOR+1
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
			if((lastcounter>0)&&(getLocationAvailible(last[lastcounter-1].location,last[lastcounter-1].unit)>0)&&(pStats[last[lastcounter-1].unit].speed>0))
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
				setRemainingBuildTime(nr,pMap->location[last[lastcounter].location].getDistance(count)*100/pStats[last[lastcounter].unit].speed);
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
//				addLocationForce(last[lastcounter].location,build->getType(nr),-build->getUnitCount(nr));
//				setProgramIsBuilt(getIP(),1);
//				ok=1;
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
						peonmins++;
						mins+=stats[2][building[n].type].mins*0.75;
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
	return(ok);
}

void RACE::eraseIllegalCode()
{
	int allUnits[UNIT_TYPE_COUNT];
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
		allUnits[i]=getLocationForce(0,i);
	for(int i=MAX_LENGTH;i--;)
	{
		int ok=1;
		for(int k=0;k<3;k++)
			ok&=((stats[getPlayer()->getGoal()->getRace()][getPlayer()->getGoal()->toPhaeno(Code[i])].prerequisite[k]==0)||
				 (allUnits[stats[getPlayer()->getGoal()->getRace()][getPlayer()->getGoal()->toPhaeno(Code[i])].prerequisite[k]]));
//WTF? allUnits mit prerequisite vergleichen!?

//TODO so ganz sauber is des net
		if(!ok)
		{
			for(int k=i;k--;)
				Code[k+1]=Code[k];
			Code[0]=0;
		}
		else
			allUnits[getPlayer()->getGoal()->toPhaeno(Code[i])]++;
	}
}

void RACE::mutateGeneCode()
{
// TODO logger machen, welche Mutationsart besonders erfolgreich ist
	if(ga->getMutationFactor()==0)
		return;
	if(getLength()==0) 
		setLength(MAX_LENGTH);
//		return;
/*	if(rand()%100==0)
	{
	   		if(mutationRate>50)
			mutationRate-=rand()%50;
		else mutationRate+=rand()%50;
	}*/
	int force[GAS_SCV+1];
	int buildable[GAS_SCV+1];
	int tMaxBuildTypes;
	int tGeno[GAS_SCV+1]; // !! keine anderen units drueber nehmen!
	for(int i=GAS_SCV+1;i--;)
	{
		force[i]=(getMap()->getLocationForce(0,getPlayerNum(),i)>0); //set start force
		buildable[i]=force[i];		
		tGeno[i]=0;
	}

	for(int x=MAX_LENGTH;x--;) //length
	{
		if(x<MAX_LENGTH-1) // erstes nicht einbeziehen
		{
			force[getPlayer()->getGoal()->toPhaeno(Code[x+1])]=1;
		}
		
		for(int i=GAS_SCV+1;i--;)
		{
			if(force[i]) buildable[i]=1;
			if((pStats[i].prerequisite[0])&&(force[pStats[i].prerequisite[0]])) buildable[i]=1;
			if((pStats[i].prerequisite[1])&&(force[pStats[i].prerequisite[1]])) buildable[i]=1;
			if((pStats[i].prerequisite[2])&&(force[pStats[i].prerequisite[2]])) buildable[i]=1;
			if((pStats[i].facility[0])&&(force[pStats[i].facility[0]])) buildable[i]=1;
			if((pStats[i].facility[1])&&(force[pStats[i].facility[1]])) buildable[i]=1;
			if((pStats[i].facility[2])&&(force[pStats[i].facility[2]])) buildable[i]=1;
		}
		tMaxBuildTypes=0;
		for(int i=GAS_SCV+1;i--;)
			if(buildable[i])
			{
				if(i==WRAITH)
				{
					i=WRAITH;
				};
				if(!getPlayer()->getGoal()->isBuildable[i])
					buildable[i]=0;
				else
				{
					tGeno[tMaxBuildTypes]=getPlayer()->getGoal()->toGeno(i);
					tMaxBuildTypes++;
				}
			}
		
//alle ueberpruefen, ob die ueberhaupt baubar sind... aber von hinten her!

		if(rand()%(MAX_LENGTH*100/ga->getMutationFactor())==0)
		{
			switch(rand()%4)
			{
				//TODO: wenn generateBuildOrder==1 dann bleibts stehen!
				case 0://delete one variabel entry and move - Mehrere Schmieden/Kasernen etc. zulassen!
				{
//					if(((getPlayer()->getGoal()->isVariable[Code[0][x]]==1)&&(getPlayer()->getGoal()->isVariable[Code[1][x]]==1))||(!ga->preprocessBuildOrder)) TODO
						//TODO: ueberlegen, ob Code evtl struct sein sollte... mmmh
						for(int i=x;i<MAX_LENGTH-1;i++)
						{
							Code[i]=Code[i+1];
						
							Marker[i]=Marker[i+1];
						};
					// TODO hier auch das buildable und tMaxBuildTypes rein... irgendwie den Code als "mutier mich" markieren und spaetereinfuegen
					markerCounter++;Marker[MAX_LENGTH-1]=markerCounter;
					int y;
//					if(ga->preprocessBuildOrder) // TODO
//					while(getPlayer()->getGoal()->isVariable[y]==0) y=rand()%getPlayer()->getGoal()->getMaxBuildTypes();
//					else	
					y=tGeno[rand()%tMaxBuildTypes];//getPlayer()->getGoal()->getMaxBuildTypes();
					Code[MAX_LENGTH-1]=y;
//					if(ga->preprocessBuildOrder) // TODO
//					while(getPlayer()->getGoal()->isVariable[y]==0) y=rand()%getPlayer()->getGoal()->getMaxBuildTypes();
//					else
//					y=tGeno[rand()%tMaxBuildTypes];//getPlayer()->getGoal()->getMaxBuildTypes();
//					Code[1][MAX_LENGTH-1]=y;
				};break;
				case 1://add one variable entry
				{
					for(int i=MAX_LENGTH-1;i>x;i--)
					{
						Code[i]=Code[i-1];
						Marker[i]=Marker[i-1];
					
					}
					//todo: BUG! player not initialized!very rare
					markerCounter++;Marker[x]=markerCounter;
					int y;
//					if(ga->preprocessBuildOrder) TODO
//						while(getPlayer()->getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//getPlayer()->getGoal()->getMaxBuildTypes();
//					else
					y=tGeno[rand()%tMaxBuildTypes];//getPlayer()->getGoal()->getMaxBuildTypes();
					Code[x]=y;
//					if(ga->preprocessBuildOrder)
//						while(getPlayer()->getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getPlayer()->getGoal()->getMaxBuildTypes();
//					else
				};break;
				case 2://change one entry
				{
//					if(getPlayer()->getGoal()->isVariable[Code[k][x]]==1) TODO
					{
						int y;
						//int y=rand()%getPlayer()->getGoal()->getMaxBuildTypes();//Optimieren
//						if(ga->preprocessBuildOrder) TODO
//							while(getPlayer()->getGoal()->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%getPlayer()->getGoal()->getMaxBuildTypes();
//						else						
						y=tGeno[rand()%tMaxBuildTypes];//getPlayer()->getGoal()->getMaxBuildTypes();
						Code[x]=y;
						markerCounter++;Marker[x]=markerCounter;
					};
				};break;
				case 3://exchange two entries
					{
						//hier erst recht
						int y=rand()%MAX_LENGTH/2+rand()%MAX_LENGTH/2-x;
//						y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//						if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH;
						if((y>0)&&(x!=y)&&
						   (buildable[getPlayer()->getGoal()->toPhaeno(Code[y])]))
						{
							int l;
							l=Code[x];Code[x]=Code[y];Code[y]=l;
							l=Marker[x];Marker[x]=Marker[y];Marker[y]=l;
						}
					};break;
//				case 5://move a block of orders  [a..b..ta..tb..c..d] -> [a..ta..tb..b..c..d]
					//~~~TODO bug, marker und code wird nicht richtig verschoben
/*					if(getLength()>2)
					{
						int i,ta,tb,ttt,y;
						ta=rand()%(getLength()-2)+1; //>0	<getLength()-2
						tb=rand()%(getLength()-1)+2; //>1   <getLength()-1 
						while(ta==tb) tb=rand()%(getLength()-1)+2;
						if(ta>tb) {ttt=tb;tb=ta;ta=ttt;}; //~~
						y=rand()%getLength(); //move it here
						while((y<=tb)&&(y>=ta)) y=rand()%getLength();
						int tmp[MAX_LENGTH];
						if(y>tb)
						{
							for(i=0;i<y-tb;i++) tmp[i]=Code[0][i+tb];
							for(i=ta;i<tb;i++) Code[0][i+y-tb]=Code[0][i];
							for(i=0;i<y-tb;i++) Code[0][ta+i]=tmp[i];

							for(i=0;i<y-tb;i++) tmp[i]=Code[1][i+tb];
							for(i=ta;i<tb;i++) Code[1][i+y-tb]=Code[1][i];
							for(i=0;i<y-tb;i++) Code[1][ta+i]=tmp[i];

							for(i=0;i<y-tb;i++) tmp[i]=Marker[0][i+tb];
							for(i=ta;i<tb;i++) Marker[0][i+y-tb]=Marker[0][i];
							for(i=0;i<y-tb;i++) Marker[0][ta+i]=tmp[i];
																				
							for(i=0;i<y-tb;i++) tmp[i]=Marker[1][i+tb];
							for(i=ta;i<tb;i++) Marker[1][i+y-tb]=Marker[1][i];
							for(i=0;i<y-tb;i++) Marker[1][ta+i]=tmp[i];

						}
						else
						if(y<ta)
						{
							for(i=0;i<ta-y;i++) tmp[i]=Code[0][i+y];
							for(i=ta;i<tb;i++) Code[0][y+i-ta]=Code[0][i];
							for(i=0;i<ta-y;i++) Code[0][tb-y]=tmp[i];

							for(i=0;i<ta-y;i++) tmp[i]=Code[1][i+y];
							for(i=ta;i<tb;i++) Code[1][y+i-ta]=Code[1][i];
							for(i=0;i<ta-y;i++) Code[1][tb-y]=tmp[i];

							for(i=0;i<ta-y;i++) tmp[i]=Marker[0][i+y];
							for(i=ta;i<tb;i++) Marker[0][y+i-ta]=Marker[0][i];
							for(i=0;i<ta-y;i++) Marker[0][tb-y]=tmp[i];
																				
							for(i=0;i<ta-y;i++) tmp[i]=Marker[1][i+y];
							for(i=ta;i<tb;i++) Marker[1][y+i-ta]=Marker[1][i];
							for(i=0;i<ta-y;i++) Marker[1][tb-y]=tmp[i];
						}
					};*///break;
//				case 6://flip entries
//					{
						// neutral :o
//						int l;
//						l=Code[0][x];Code[0][x]=Code[1][x];Code[1][x]=l;
//						l=Marker[0][x];Marker[0][x]=Marker[1][x];Marker[1][x]=l;
//					};break;
			}
		}
	}
}


// Reset all ongoing data (between two runs)
void RACE::resetData() // resets all data to standard starting values
{
	for(int i=MAX_GOALS;i--;)
		setFinalTime(i,0);
	resetSpecial();
	for(int i=0;i<4;i++) //TODO ???
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
	
	setpFitness(0);
	setsFitness(0);
	settFitness(MAX_TFITNESS);

	setHarvestedGas(0);
	setHarvestedMins(0);
	//location 0 is for globalForce, so take location 1 for starting point instead
	setMins(getPlayer()->getMins());
	setGas(getPlayer()->getGas());
	setTimer(ga->maxTime-getPlayer()->getTimer());
	setSupply(getPlayer()->getSupply()); //?
	setMaxSupply(getPlayer()->getMaxSupply()); //?
	setTimeOut(ga->maxTimeOut);
	setIP(ga->maxLength-1);
	setCalculated(0);
	ready=0;
};


/*void RACE::generateBasicBuildOrder()
{
	int i;
	//problem wieder mit keep/castle
	//pre[GOALS]
//	int build_max,sup;
//	build_max=0;
	for(i=MAX_LENGTH;i--;)
	{
		basicBuildOrder[0][i]=0;
		basicBuildOrder[1][i]=0;
	}
	basicLength=0;
	while((supply<maxSupply)&&(basicLength<20))
	{
		if(supply<maxSupply)
		{
			basicBuildOrder[0][basicLength]=phaenoToGenotype[SCV];
			supply++;
		} else
		{
			basicBuildOrder[0][basicLength]=phaenoToGenotype[SUPPLY_DEPOT];
		}
		basicLength++;
	}*/
	
/*	
//~~~ allgemeines supply, arbeiter, gas/min, Main, ref, ... einfuehren... is teilweise schon, aber v.a. bei hatch etc. noch machen!
	
//TODO: Arbeiter, Raffinerie und Supplysachen deklarieren und hier allgemein durchlaufen lassen
	switch(gRace)
	{
		case TERRA:
			basicBuildOrder[0][0]=phaenoToGenotype[SCV];
			basicBuildOrder[0][1]=phaenoToGenotype[SCV];
			basicBuildOrder[0][2]=phaenoToGenotype[SCV];
			basicBuildOrder[0][3]=phaenoToGenotype[SCV];
			basicBuildOrder[0][4]=phaenoToGenotype[SCV];
			basicBuildOrder[0][5]=phaenoToGenotype[SCV];
			basicBuildOrder[0][6]=phaenoToGenotype[SUPPLY_DEPOT];
			if(isBuildable[REFINERY]==1)
			{
				basicBuildOrder[0][7]=phaenoToGenotype[REFINERY];
				basicBuildOrder[0][8]=phaenoToGenotype[GAS_SCV];
				basicBuildOrder[0][9]=phaenoToGenotype[GAS_SCV];
				basicBuildOrder[0][10]=phaenoToGenotype[GAS_SCV];
			};
			basicLength=10;sup=8;
			break;
		case PROTOSS:
			basicBuildOrder[0][0]=phaenoToGenotype[PROBE];
			basicBuildOrder[0][1]=phaenoToGenotype[PROBE];
			basicBuildOrder[0][2]=phaenoToGenotype[PROBE];
			basicBuildOrder[0][3]=phaenoToGenotype[PROBE];
			basicBuildOrder[0][4]=phaenoToGenotype[PROBE];
			basicBuildOrder[0][5]=phaenoToGenotype[PYLON];
			if(isBuildable[ASSIMILATOR]==1)
			{
				basicBuildOrder[0][6]=phaenoToGenotype[ASSIMILATOR];
				basicBuildOrder[0][7]=phaenoToGenotype[GAS_PROBE];
				basicBuildOrder[0][8]=phaenoToGenotype[GAS_PROBE];
				basicBuildOrder[0][9]=phaenoToGenotype[GAS_PROBE];
			};
			basicLength=9;sup=8;
			break;
		case ZERG:
			basicBuildOrder[0][0]=phaenoToGenotype[DRONE];
			basicBuildOrder[0][1]=phaenoToGenotype[DRONE];
			basicBuildOrder[0][2]=phaenoToGenotype[DRONE];
			basicBuildOrder[0][3]=phaenoToGenotype[DRONE];
			basicBuildOrder[0][4]=phaenoToGenotype[DRONE];
			basicBuildOrder[0][5]=phaenoToGenotype[OVERLORD];

			if(isBuildable[EXTRACTOR]==1)
			{
				basicBuildOrder[0][6]=phaenoToGenotype[EXTRACTOR];
				basicBuildOrder[0][7]=phaenoToGenotype[GAS_DRONE];
				basicBuildOrder[0][8]=phaenoToGenotype[GAS_DRONE];
				basicBuildOrder[0][9]=phaenoToGenotype[GAS_DRONE];
			};
			basicLength=9;sup=8;
			break;
	}
	//bis hier: 08/15 Start

//	for(j=0;j<MAX_GOALS;j++)
//		if(force[j]>0) pre[j]=force[j]; else pre[j]=0;	
//TODO: Problem bei KEEPs
	if(settings.generateBuildOrder==1)
	{
	for(k=0;k<MAX_LENGTH;k++)
	{
		j=0;
		while((j<build_max)&&(i<MAX_LENGTH-20))
		{
			if(((pre[j]<goal[j].count)&&(goal[j].count>0))||((isBuildable[j]>0)&&(force[j]==0)&&(pre[j]<isBuildable[j])))
			{
				if((gpStats[j].facility>0)&&(pre[gpStats[j].facility]==0))
					j=gpStats[j].facility;
				else if((gpStats[j].prerequisite[0]>0)&&(pre[gpStats[j].prerequisite[0]]==0))
					j=gpStats[j].prerequisite[0];
				else if((gpStats[j].prerequisite[1]>0)&&(pre[gpStats[j].prerequisite[1]]==0))
					j=gpStats[j].prerequisite[1];
				else if((gpStats[j].prerequisite[2]>0)&&(pre[gpStats[j].prerequisite[2]]==0))
					j=gpStats[j].prerequisite[2];
				else if((gpStats[j].type==UPGRADE)&&(gpStats[j].special>0)&&(pre[TIER3]==0))
					j=TIER3;
				else if((gpStats[j].special>0)&&(pre[TIER2]==0))
					j=TIER2;
				else if((gpStats[j].special>0)&&(pre[TIER3]==0))
					j=TIER3;
				else
				{
					//building!
					if(gpStats[j].type==BUILDING)//&&(gRace=ZERG))
					{
						i++;
						basicBuildOrder[i][0]=phaenoToGenotype[DRONE];
					} else
					
					if((gpStats[j].type==UNIT)&&(gpStats[j].supply>sup))
					{
						i++;
						switch(gRace)
						{
							case TERRA:basicBuildOrder[i][0]=phaenoToGenotype[SUPPLY_DEPOT];pre[SUPPLY_DEPOT]++;sup+=8;break;
							case PROTOSS:basicBuildOrder[i][0]=phaenoToGenotype[PYLON];pre[PYLON]++;sup+=8;break;
							case ZERG:basicBuildOrder[i][0]=phaenoToGenotype[OVERLORD];pre[OVERLORD]++;sup+=8;break;
							default:break;
						};
					};
					i++;
					if((gpStats[j].type==UNIT)&&(gpStats[j].supply>0)) sup-=pStats[j].supply;
					basicBuildOrder[i][0]=phaenoToGenotype[j];
					pre[j]++;
					j++;
				}
			} else j++;
		}
	}
	}*/
//	basicLength++;
/*	memcpy(basicBuildOrder[1],basicBuildOrder[0],MAX_LENGTH*4);
}

*/
//Reinitialize programs with random orders
void RACE::resetGeneCode()
{
//	mutationRate=500+rand()%1000;
	if((ga->preprocessBuildOrder)&&(basicLength>0))
	{
		memcpy(Code,basicBuildOrder,MAX_LENGTH*4);
		for(int i=0;i<MAX_LENGTH;i++)
		{
			markerCounter++;Marker[i]=markerCounter;
		}
	}
	else
	{
		int y=0;
		switch(getPlayer()->getGoal()->getRace())
		{
			case TERRA:y=SUPPLY_DEPOT;break;
			case PROTOSS:y=PYLON;break;
			case ZERG:y=OVERLORD;break;
		};
		for(int i=MAX_LENGTH;i--;)
		{
//			if((i+4)%stats[getPlayer()->getGoal()->getRace()][y].supply==0)
//			{
				Code[i]=rand()%getPlayer()->getGoal()->getMaxBuildTypes();
//			} else
//			{
//				Code[0][i]=/*rand()%*/getPlayer()->getGoal()->toGeno(SCV);//getMaxBuildTypes();
//				Code[1][i]=/*rand()%*/getPlayer()->getGoal()->toGeno(SCV);//getMaxBuildTypes();
//			}
			markerCounter++;Marker[i]=markerCounter;
		}
	}
}

void RACE::crossOver(RACE* parent2, RACE* child1, RACE* child2)
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

int RACE::setpFitness(int fitness)
{
#ifdef _SCC_DEBUG
	if(fitness<0)
	{
		debug.toLog(0,"DEBUG: (RACE::setpFitness): Value fitness [%i] out of range.",fitness);
		return(0);
	};
#endif
	pFitness=fitness;
	return(1);
};

int RACE::setsFitness(int fitness)
{
#ifdef _SCC_DEBUG
	if((fitness<0)||(fitness>MAX_MINS+MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (RACE::setsFitness): Value fitness [%i] out of range.",fitness);
		return(0);
	};
#endif
	sFitness=fitness;
	return(1);
};

int RACE::settFitness(int fitness)
{
#ifdef _SCC_DEBUG
	if(fitness<0)
	{
		debug.toLog(0,"DEBUG: (RACE::settFitness): Value fitness [%i] out of range.",fitness);
		return(0);
	};
#endif
	tFitness=fitness;
	return(1);
};

int RACE::getpFitness()
{
#ifdef _SCC_DEBUG
	if((pFitness<0)||(pFitness>30000))
	{
		debug.toLog(0,"DEBUG: (RACE::getpFitness): Variable pFitness not initialized [%i].",pFitness);
		return(0);
	};
#endif
	return(pFitness);
};

int RACE::getsFitness()
{
#ifdef _SCC_DEBUG
	if((sFitness<0)||(sFitness>MAX_MINS+MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (RACE::getsFitness): Variable sFitness not initialized [%i].",sFitness);
		return(0);
	};
#endif
	return(sFitness);
};

int RACE::gettFitness()
{
#ifdef _SCC_DEBUG
	if((tFitness<0)||(tFitness>MAX_TFITNESS))
	{
		debug.toLog(0,"DEBUG: (RACE::gettFitness): Variable tFitness not initialized [%i].",tFitness);
		return(0);
	};
#endif
	return(tFitness);
};



RACE::RACE()
{
		setpFitness(0);
		setsFitness(0);
		settFitness(99999);
};

RACE::~RACE()
{
};


int RACE::basicBuildOrder[2][MAX_LENGTH];
int RACE::basicLength;

