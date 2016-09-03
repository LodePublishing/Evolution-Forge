#include <stdlib.h>
#include <string.h>
#include "race.h"
#include "debug.h"

// TODO: reimplement/recheck the speed of the units

int RACE::calculateStep()
{
//ZERG:  CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
//mins, gas hier rein...
//TODO: evtl ueberlegen IP zu springen... also erstmal feststellen, warum nicht gebuildet werden kann und dann in einem Ruck resources und btimes hochsetzen...
	if((!time)||(ready)||(!getIP())||((bestTime*4<ga->maxTime*3)&&(4*time<3*bestTime)))
	{
		setLength(ga->maxLength-getIP());
		settFitness(gettFitness()-getLength());
		if(ready)
			setTimer(time);
		else
			setTimer(0);
		setpFitness(getTimer());
		calculateFitness(ready);
		return(1);
	}
	int dominant=0;
	int ok=1;
	int first=1;
	int i,j;
	while((ok)&&(getIP()))
	{
		//TODO: Variable einfuegen, damit Benutzer z.B. Befehle pro Zeiteinheit (oder pro Location!?) einstellen kann
		if(Code[0][getIP()]>Code[1][getIP()]) //dominance
		{
			if(!(ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=0][getIP()]))))
				ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=1][getIP()]));
		}
		else
		{
			if(!(ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=1][getIP()]))))
				ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=0][getIP()]));
		};
		//TODO: Eine Art Funktion "TimeAdvance" schreiben um Zeiten zu ueberspringen, in denen nix passiert (halt zum naechsten Zeitpunkt springen, zu dem ein Building fertig wird...
		//TODO evtl buildings in eine linked Liste schreiben und dann immer das vorderste abarbeiten
		if((ok)||(!timeout))
		{
			if(!first)
				settFitness(gettFitness()-1);
			first=0;
			setIP(getIP()-1);
			if(!timeout)
				settFitness(gettFitness()-5);
			timeout=ga->maxTimeOut;
		};
	}

//		BUILDING* build;//=&(building[MAX_BUILDINGS-1]);
//!!!???		for(j=MAX_BUILDINGS;j--;build=&(building[j]))
	for(j=MAX_BUILDINGS;j--;)
	{
//		build=&building[j];
		if(getBuildingRemainingBuildTime(j))
		{
			setBuildingRemainingBuildTime(j,getBuildingRemainingBuildTime(j)-1);
			if(!getBuildingRemainingBuildTime(j))
			{
                                const UNIT_STATISTICS* stat=&pStats[getBuildingType(j)];

				switch(stat->facility_type)
				{
					case IS_LOST:
						if(getBuildingFacility(j))
							addLocationForce(getBuildingLocation(j),getBuildingFacility(j),-1);
							//availible was already taken account when starting the building
						if(stat->facility2)
							addLocationForce(getBuildingLocation(j),stat->facility2,-1);
						break;
					case NEEDED_ONCE:break;
					case NEEDED_UNTIL_COMPLETE:
						if(getBuildingFacility(j))
							addLocationAvailible(getBuildingLocation(j),getBuildingFacility(j),1);
						break; // fuer spaeter mal: Wenn in 2 Fabriken produziert wuerde wirds probmelatisch, da
//in Buiding nur eine facility gespeichert wird...
					case NEEDED_ONCE_IS_LOST:
						if(stat->facility2)
							addLocationForce(getBuildingLocation(j),stat->facility2,-1);
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST:
						if(getBuildingFacility(j))
							addLocationAvailible(getBuildingLocation(j),getBuildingFacility(j),1);
						if(stat->facility2)
							addLocationForce(0/*getBuildingLocation(j)*/,stat->facility2,-1);
//r_researches need location 0
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
						{
							if(getBuildingFacility(j))
								addLocationAvailible(getBuildingLocation(j),getBuildingFacility(j),1);
							if(stat->facility2) // special rule for upgrades!
							{
								addLocationForce(0/*getBuildingLocation(j)*/,stat->facility2,-1);
								addLocationAvailible(0/*getBuildingLocation(j)*/,stat->facility2,1);
							};
						}
						break;
					case NEEDED_ALWAYS:break;
					default:break;
				}

				if(getBuildingType(j)==REFINERY)
					addMapLocationForce(0,getBuildingLocation(j),VESPENE_GEYSIR,-1);
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
				//else? mmmh ne!
				//~~~~
				addLocationForce(getBuildingLocation(j),getBuildingType(j),getBuildingUnitCount(j));
				addLocationAvailible(getBuildingLocation(j),getBuildingType(j),getBuildingUnitCount(j));
				if(getBuildingType(j)==REFINERY) 
					adjustGasHarvest(getBuildingLocation(j));
				if(getBuildingType(j)==COMMAND_CENTER) 
				{
					adjustMineralHarvest(getBuildingLocation(j));
					adjustGasHarvest(getBuildingLocation(j));
				}
				last[lastcounter].unit=getBuildingType(j);
				last[lastcounter].count=getBuildingUnitCount(j);
				last[lastcounter].location=getBuildingLocation(j);

				if((stat->create)&&(!getBuildingOnTheRun(j))) //one additional unit (zerglings, scourge, comsat, etc.)
				{ //here no unitCount! ~~~
					addLocationForce(getBuildingLocation(j),stat->create,1);
					addLocationAvailible(getBuildingLocation(j),stat->create,1);
					if(last[lastcounter].unit==stat->create) last[lastcounter].count++; //TODO ???
					if((getPlayer()->goal->getRace()==ZERG)&&(stat->create==LARVA))
					{
						larva[larvacounternumber].location=getBuildingLocation(j);
						larva[larvacounternumber].larvacount=1;
						larva[larvacounternumber].counter=20;
						larvacounternumber++; //~~mei... ne linked list...
					}
					// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
				}
                                lastcounter++;

// berechne 'FinalTime', die angibt, wann alle Einheiten (bzw. die letzte Einheit) unsereres Zieles fertiggestellt wurden
// Quasi wie "ready" nur ohne Zeit
//TODO CHECKEN!!

				for(i=MAX_GOALS;i--;)
// ist dieses goal belegt?
				if((getPlayer()->goal->goal[i].unit>0)&&
// befinden wir uns am richtigen Ort?
				  ((getPlayer()->goal->goal[i].location==0)||(getBuildingLocation(j)==getPlayer()->goal->goal[i].location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
				   (getBuildingType(j)==getPlayer()->goal->goal[i].unit))
						setFinalTime(i,ga->maxTime-time);
// Did we reach the right number at the right time?
//		i=MAX_GOALS;  TODO? koennen wir mehrere goals gleichzeitig erfuell0rn?
			ready=1;
			for(i=MAX_GOALS;(i--)&&(ready);)
                                if(getPlayer()->goal->goal[i].count)
					ready&=((getPlayer()->goal->goal[i].count<=getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit))&&((getPlayer()->goal->goal[i].time>=getFinalTime(i))||(getPlayer()->goal->goal[i].time==0)));

// oder: irgendeine location... TODO: Problem: die Einheiten koennen irgendwo sein, also nicht gesammelt an einem Fleck...
			} // END of if(!getBuildingRemainingBuildTime(j))
		} // END of if(getBuildingRemainingBuildTime(j))
	} // END of for(j=MAX_BUILDINGS;j--;)
//	resetLocations();
//OK: Problem ist, dass pMap->location 0 ist... ?
	harvestResources();
	createSpecial(); //Larvae and later energy for wizards, lifepoints etc.
	time--;
	timeout--;
	return(0);
	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
};

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)


//FEHLER MUSS HIER SEIN!! TODO

//TODO: pFitness wird total falsch berechnet fuer races die nicht alle Ziele erfuellt haben!1
void RACE::calculateFitness(int ready)
{
//TODO evtl noch uebrige availible miteinbeziehen
	int i,j;
//	int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; // temporary data to check whether a bonus is already given (only applies if force > goal)
	setsFitness(getHarvestedMins()+getHarvestedGas()); //TODO: evtl gas und mins (wie urspruenglich eigentlich) in Verhaeltnis setyen wieviel es jeweils Geysire/Mineralien gibt...
//TODO: Nicht alle Einheiten am Ort? => Ort egal sein lassen aber zur Zeit hinzuzaehlen
	// Nicht alle Einheiten ueberhaupt gebaut UND nicht alle am Ort => nur viertel Bonus fuer Einheiten die nicht am Ort sind
	for(i=0;i<UNIT_TYPE_COUNT;i++)
		if((getPlayer()->goal->allGoal[i]>0)&&(getPlayer()->goal->allGoal[i]<getLocationForce(0,i)))
			setsFitness(getsFitness()-(getLocationForce(0,i)-getPlayer()->goal->allGoal[i]));
	if(!ready)
	{
		//calculate number of fulfilled goals & their time & their distance to goal position
		for(i=MAX_GOALS;i--;)
			if(getPlayer()->goal->goal[i].count>0)
			{
// Haben wir zuwenig Einheiten gebaut?  force < goal
if( /*((getPlayer()->goal->goal[i].location==0)&&(getPlayer()->goal->goal[i].count>getLocationForce(0,getPlayer()->goal->goal[i].unit))) || unnuetz*/
  (  /*(getPlayer()->goal->goal[i].location>0)&&*/(getPlayer()->goal->goal[i].count>getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit))) )
				{
					//total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
					//TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen
					//evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!
					int sumup=0;
					int bon=0;
// location 0? Dann ist es trivial
					if(getPlayer()->goal->goal[i].location==0)
						sumup=getLocationForce(0,getPlayer()->goal->goal[i].unit)*100;
					else
					{
				// location != 0 ? Dann schaun wir mal, ob wir auf der Karte noch wo Units verstreut haben
				// mehr als goalcount koennen wir keinen Bonus vergeben
						bon=getPlayer()->goal->goal[i].count;
						j=1;
						while((j<MAX_LOCATIONS)&&(bon>getLocationForce(pMap->locationList[getPlayer()->goal->goal[i].location][j],getPlayer()->goal->goal[i].unit)))
						{
							sumup+=getLocationForce(pMap->locationList[getPlayer()->goal->goal[i].location][j],getPlayer()->goal->goal[i].unit)*(100-pMap->location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].getDistance(getPlayer()->goal->goal[i].location)); //was pMap->location[j]...
							bon-=getLocationForce(pMap->locationList[getPlayer()->goal->goal[i].location][j],getPlayer()->goal->goal[i].unit);
							j++;
						}
						// Falls j<MAX_LOCATIONS => unser "Bon" wurde schon vorher aufgebraucht => An dieser Stelle j den Rest draufgeben... 
						if(j<MAX_LOCATIONS)
							sumup+=bon*(100-pMap->location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].getDistance(getPlayer()->goal->goal[i].location));
					}

//jetzt steht in sumup die gesammelten forces gewichtet mit den Entfernungen zum Ziel

					//TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
					// evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
					// bissl komplex da mans ja den einzelnen goals verteilen muss...
					if((getPlayer()->goal->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->goal->goal[i].time))
//					{
//						if(getFinalTime(i)>0) //??? TODO
//						if(getFinalTime(i)>getPlayer()->goal->goal[i].time)
							setpFitness(getpFitness()+(getPlayer()->goal->goal[i].time*sumup)/(getPlayer()->goal->goal[i].count*getFinalTime(i)));
//						else setpFitness(getpFitness()+sumup/getPlayer()->goal->goal[i].count);
//TODO...~~~
//						else while(true); // <- kann eigentlich nicht auftreten!
//							setpFitness(getpFitness()+(getPlayer()->goal->goal[i].time*sumup)/(getPlayer()->goal->goal[i].count*ga->maxTime));
//					}
					else
						setpFitness(getpFitness()+sumup/getPlayer()->goal->goal[i].count);
				} // END force < goal 
				else 
//if( /*((getPlayer()->goal->goal[i].location==0)&&(getPlayer()->goal->goal[i].count<=getLocationForce(0,getPlayer()->goal->goal[i].unit))) || ( (getPlayer()->goal->goal[i].location>0)&&*/(getPlayer()->goal->goal[i].count<=getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit))) 
				//force >= goal ?
				{
// Checken wann wir das Ziel erreicht haetten
					if((getPlayer()->goal->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->goal->goal[i].time))
// aha, wir haben die Zeit ueberschritten => trotzdem anteilig Bonus geben
						setpFitness(getpFitness()+(getPlayer()->goal->goal[i].time*100/getFinalTime(i)));
// keine Zeitbeschraenkung + wir haben genuegend Einheiten am Zielort => gg
					else setpFitness(getpFitness()+100);
// does not work yet, if this is uncommented, sFitness occasionally jumps to -1222000 or something like that... :/
// include the final location maybe...
				}
			} //end of goal checking
// TODO: Check for very small 'goal.time' values, probably in scc.cpp!!	

//Bonus: Hier werden Einheiten verarbeitet die noch in Produktion sind, aber Teil der goals sind

//Erstmal Maximalbonus errechnen (nicht, dass die engine dann 50 Battlecruiser kurz vor Schluss noch anfaengt oder so :P )
/*		for(i=MAX_LOCATIONS;i--;)
		       for(j=UNIT_TYPE_COUNT;j--;)
			       bonus[i][j]=0;
		for(i=MAX_GOALS;i--;)
			if(getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit)<getPlayer()->goal->goal[i].count)
				bonus[getPlayer()->goal->goal[i].location][getPlayer()->goal->goal[i].unit]+=getPlayer()->goal->goal[i].count-getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit);

		for(i=MAX_BUILDINGS;i--;)
			if((getBuildingRemainingBuildTime(i)>0)&&(bonus[getBuildingLocation(i)][getBuildingType(i)]>0))
			{
			//erstmal ohne Zeit...
				pFitness+=((getBuildingRemainingBuildTime(i)*100)/((getLocationForce(getBuildingLocation(i),getBuildingType(i))+bonus[getBuildingLocation(i)][getBuildingType(i)])*pStats[getBuildingType(i)].BT)); //TODO in ProgramBT aendern
				
				if((getPlayer()->goal->goal[getBuildingType(i)].time>0)&&(getLocationForce(getBuildingLocation(i),getBuildingType(i))==0))
					pFitness+=(getBuildingRemainingBuildTime(i)*100*getPlayer()->goal->goal[getBuildingType(i)].time*getLocationForce(0,i))/(getPlayer()->goal->goal[getBuildingType(i)].count*pStats[getBuildingType(i)].BT*ga->maxTime);//hier auch ProgramBT
				else 				   		
					pFitness+=((getBuildingRemainingBuildTime(i)*100)/(getPlayer()->goal->goal[getBuildingType(i)].count*pStats[getBuildingType(i)].BT));
				bonus[getBuildingLocation(i)][getBuildingType(i)]--;
			}*/

	} // end of ready=false
	else   // all goals fulfilled, fitness <- timer 
		for(i=MAX_GOALS;i--;)
			if(getPlayer()->goal->goal[i].count>0)
				setpFitness(getpFitness()+100);

}
// end of calculateFitness


int RACE::buildGene(int unit)
{
	int i;
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
			(getMins()>=stat->mins+stat->upgrade_cost*getLocationForce(0,unit))&&
			(getGas()>=stat->gas+stat->upgrade_cost*getLocationForce(0,unit))
			&&
			((getSupply()>=stat->supply)||(stat->supply<=0))
			&&
			((stat->facility2==0)||(getLocationAvailible(0,stat->facility2)>0)))
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
//			if((stat->facility2==0)||(getLocationAvailible(/*tloc*/0,stat->facility2)>0))
				for(fac=3;fac--;)
					if( ((stat->facility[fac]>0)&&(getLocationAvailible(tloc,stat->facility[fac])>0)) || ((stat->facility[fac]==0)&&(fac==0))) 
					{
						ok=1;
						break;
					}

                        j=1;
                        if(!ok)
                                while(j<MAX_LOCATIONS)
                                {
                                        ttloc=pMap->locationList[tloc][j];
//                                      if((stat->facility2==0)||(getLocationAvailible(ttloc,stat->facility2)>0))
                                        {
                                                for(fac=3;fac--;)
                                                        if( ((stat->facility[fac]>0)&&(getLocationAvailible(ttloc,stat->facility[fac])>0)) || ((stat->facility[fac]==0)&&(fac==0)))
                                                        {
								tloc=ttloc;
                                                                ok=1;
                                                                break;
                                                        }
                                                break;
                                        }
                                        j++;
                                }

			if((ok)&&(unit==EXTRACTOR))
			{
				if(!getMapLocationAvailible(0,tloc,VESPENE_GEYSIR))
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
				int nr=0;
				while((nr<MAX_BUILDINGS-1)&&(getBuildingRemainingBuildTime(nr)))
				{
					nr++;
					//nr=MAX_BUILDINGS;
				}
				setBuildingOnTheRun(nr,0);
				setBuildingFacility(nr,stat->facility[fac]);
				setBuildingLocation(nr,tloc);
				setBuildingUnitCount(nr,1);
				setBuildingRemainingBuildTime(nr,stat->BT+3200*(stat->facility2==unit)); //~~ hack :/ TODO
                                                                                                                                                            
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
				setMins(getMins()-(stat->mins+stat->upgrade_cost*getLocationForce(0,unit)));
				setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationForce(0,unit)));
				setBuildingType(nr,unit);

				if((stat->supply>0)||((pStats[stat->facility[0]].supply<0)&&(stat->facility_type==IS_LOST))) 
					setSupply(getSupply()-stat->supply);
				switch(stat->facility_type)
				{
					case IS_LOST:
						if(stat->facility[fac]>0)
						{
							addLocationAvailible(tloc,stat->facility[fac],-1);
							setSupply(getSupply()+pStats[stat->facility[fac]].supply); 
							if((getPlayer()->goal->getRace()==ZERG)&&(stat->facility[fac]==LARVA))
							{
								int bestPlace=0;//unschoen hier :/
								int bestCounter=0;
								for(i=0;i<larvacounternumber;i++)
									if((larva[i].location==tloc)&&(larva[i].larvacount>bestCounter))
									{bestCounter=larva[i].larvacount;bestPlace=i;}
								larva[bestPlace].larvacount--;
							}
						}
						if(stat->facility2>0)
						{
							addLocationAvailible(tloc,stat->facility2,-1);
							setSupply(getSupply()+pStats[stat->facility2].supply);
						}
						break;
					case NEEDED_ONCE:
						break;
					case NEEDED_UNTIL_COMPLETE:
						if(stat->facility[fac]>0)
							addLocationAvailible(tloc,stat->facility[fac],-1);
						if(stat->facility2>0)
							addLocationAvailible(tloc,stat->facility2,-1);
							//kommt glaub nicht vor...
						break;
					case NEEDED_ONCE_IS_LOST:
						if(stat->facility2>0)
						{
							addLocationAvailible(tloc,stat->facility2,-1);
							setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/ eigentlich schon... bei gas scv z.B. :/
						}
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST:
						if(stat->facility2>0)
						{
							addLocationAvailible(0/*location*/ ,stat->facility2,-1); // primarily for temporary R_researches, have to be location 0
// TODO 						setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/
						}
						if(stat->facility[fac]>0)
							addLocationAvailible(tloc,stat->facility[fac],-1);
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
						if(stat->facility2>0)
							addLocationAvailible(0/*tloc*/,stat->facility2,-1);
							// no supply gain as the item is recovered... well or not? mmmh... ~~~~
						if(stat->facility[fac]>0)
							addLocationAvailible(tloc,stat->facility[fac],-1);
						break;
					case NEEDED_ALWAYS:
						if(stat->facility[fac]>0)
							addLocationAvailible(tloc,stat->facility[fac],-1);
							// supply?
						if(stat->facility2>0)
							addLocationAvailible(tloc,stat->facility2,-1);
						break;
					default:break;
				}
	
//					building[nr].on_the_run=0;
				//	building[nr].IP=IP;
			} 
		}
	}
	else // unit > EXTRACTOR+1
	{

//hier checken!

// TODO ueberlegen was mit scv passiert, das gerade ein Gebaeude fertiggestellt hat... steht das in irgendeinem "last" drin?
		int count=0;
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
				while((nr<MAX_BUILDINGS-1)&&(getBuildingRemainingBuildTime(nr)))
						nr++;
				//TODO: Fehler wenn nicht genug buildings
				if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>last[lastcounter].count)
					setBuildingUnitCount(nr,last[lastcounter].count);
				else setBuildingUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
				setBuildingFacility(nr,0);
				if(last[lastcounter].location+count<1)
					count=MAX_LOCATIONS+count;
				else if(last[lastcounter].location+count>=MAX_LOCATIONS)
					count=1+last[lastcounter].location+count-MAX_LOCATIONS;
				else count=last[lastcounter].location+count;

				setBuildingLocation(nr,count);
				setBuildingType(nr,last[lastcounter].unit);
				setBuildingRemainingBuildTime(nr,pMap->location[last[lastcounter].location].getDistance(count)*100/pStats[last[lastcounter].unit].speed);
				setBuildingOnTheRun(nr,1);
//						building[nr].IP=IP; TODO?
						// 2x Unit => send 8/All instead of just one unit there
/*				if((getIP()>1)&&((Code[0][getIP()-1]==unit)||(Code[1][getIP()-1]==unit)))
				{
					if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>=6)
						setBuildingUnitCount(nr,6);
					else setBuildingUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
//	                                setProgramIsBuilt(getIP(),1);
					setIP(getIP()-1);
				}*/
				addLocationAvailible(last[lastcounter].location,getBuildingType(nr),-getBuildingUnitCount(nr));
				addLocationForce(last[lastcounter].location,getBuildingType(nr),-getBuildingUnitCount(nr));
//				setProgramIsBuilt(getIP(),1);
				ok=1;
			}
		}
	}

/*      else
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
	return ok;
}

void RACE::mutateGeneCode()
{
//TODO: ne Art Mutationlogger ueber mehrere Generationen (weitervererbbar) machen um festzustellen, was passiert ist...
//ODER: jedem Befehl eine Zahl zuweisen, ne Art radioaktiver Marker... Zu ueberlegen ist nur, was mit dem bei crossing over passiert...

	int x,y,i,k,ta,tb,ttt;
	if(getLength()==0) 
		setLength(MAX_LENGTH);
//		return;
	if(rand()%100==0)
	{
       		if(mutationRate>50)
			mutationRate-=rand()%50;
		else mutationRate+=rand()%50;
	}
	for(x=MAX_LENGTH;x--;) //length
		if(rand()%mutationRate==0)
		{
			switch(rand()%7)
			{
				//TODO: wenn generateBuildOrder==1 dann bleibts stehen!
				case 0://delete one variabel entry and move - Mehrere Schmieden/Kasernen etc. zulassen!
					if(((getPlayer()->goal->isVariable[Code[0][x]]==1)&&(getPlayer()->goal->isVariable[Code[1][x]]==1))||(!ga->preprocessBuildOrder))
						//TODO: ueberlegen, ob Code evtl struct sein sollte... mmmh
					for(y=x;y<MAX_LENGTH-1;y++)
					{
						Code[0][y]=Code[0][y+1];
						Code[1][y]=Code[1][y+1];
						
						Marker[0][y]=Marker[0][y+1];
						Marker[1][y]=Marker[1][y+1];
					};
                                                y=rand()%getPlayer()->goal->getMaxBuildTypes();
                                                markerCounter++;Marker[0][MAX_LENGTH-1]=markerCounter;
                                                markerCounter++;Marker[1][MAX_LENGTH-1]=markerCounter;
                                                                                                                                                            
                                                if(ga->preprocessBuildOrder)
                                                        while(getPlayer()->goal->isVariable[y]==0) y=rand()%getPlayer()->goal->getMaxBuildTypes();
                                                Code[0][MAX_LENGTH-1]=y;
                                                y=rand()%getPlayer()->goal->getMaxBuildTypes();
                                                if(ga->preprocessBuildOrder)
                                                        while(getPlayer()->goal->isVariable[y]==0) y=rand()%getPlayer()->goal->getMaxBuildTypes();
	                                         Code[1][MAX_LENGTH-1]=y;

					break;
				case 1://add one variable entry
					{

						for(y=MAX_LENGTH-1;y>x;y--)
						{
							Code[0][y]=Code[0][y-1];
							Code[1][y]=Code[1][y-1];
							Marker[0][y]=Marker[0][y-1];
							Marker[1][y]=Marker[1][y-1];
							
						}
						//todo: BUG! player not initialized!very rare
						y=rand()%getPlayer()->goal->getMaxBuildTypes();
						markerCounter++;Marker[0][x]=markerCounter;
						markerCounter++;Marker[1][x]=markerCounter;
						
						if(ga->preprocessBuildOrder)
							while(getPlayer()->goal->isVariable[y]==0) y=rand()%getPlayer()->goal->getMaxBuildTypes();
						Code[0][x]=y;
						y=rand()%getPlayer()->goal->getMaxBuildTypes();
						if(ga->preprocessBuildOrder)
							while(getPlayer()->goal->isVariable[y]==0) y=rand()%getPlayer()->goal->getMaxBuildTypes();
						Code[1][x]=y;
					};break;
				case 2://change one entry
					{
						k=rand()%2;
						if(getPlayer()->goal->isVariable[Code[k][x]]==1)
						{
							y=rand()%getPlayer()->goal->getMaxBuildTypes();//Optimieren
							if(ga->preprocessBuildOrder)
								while(getPlayer()->goal->isVariable[y]==0) y=rand()%getPlayer()->goal->getMaxBuildTypes();
							Code[k][x]=y;
	                                                markerCounter++;Marker[k][x]=markerCounter;
						};
					};break;
				case 3://exchange two entries
					{
						y=rand()%MAX_LENGTH/2+rand()%MAX_LENGTH/2-x;
//						y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//						if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH; //TODO? Warum
						if((y>0)&&(x!=y))
						{
							k=rand()%2;
							ta=Code[k][x];Code[k][x]=Code[k][y];Code[k][y]=ta;
							ta=Marker[k][x];Marker[k][x]=Marker[k][y];Marker[k][y]=ta;
						}
					};break;
				case 4://exchange two entries
					{
						y=rand()%MAX_LENGTH/2+rand()%MAX_LENGTH/2-x;
//						y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//						if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH;
						if((y>0)&&(x!=y))
						{
							ta=Code[0][x];Code[0][x]=Code[0][y];Code[0][y]=ta;
							ta=Marker[1][x];Marker[1][x]=Marker[1][y];Marker[1][y]=ta;
						}
					};break;
				case 5://move a block of orders  [a..b..ta..tb..c..d] -> [a..ta..tb..b..c..d]
					//~~~TODO bug, marker und code wird nicht richtig verschoben
/*					if(getLength()>2)
					{
						ta=rand()%(getLength()-2)+1; //>0    <getLength()-2
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
					};*/break;
				case 6://flip entries
					{
						ta=Code[0][x];Code[0][x]=Code[1][x];Code[1][x]=ta;
						ta=Marker[0][x];Marker[0][x]=Marker[1][x];Marker[1][x]=ta;
					};break;
			}
		}
}


// Reset all ongoing data (between two runs)
void RACE::resetData() // resets all data to standard starting values
{
	int i;
	for(i=UNIT_TYPE_COUNT;i--;)
		setFinalTime(i,0);
	resetBuildings();
	resetSpecial();
	for(i=0;i<4;i++) //TODO ???
	{
		last[i].location=1;
		last[i].unit=SCV;
		last[i].count=1;
	}

	for(i=4;i<MAX_LENGTH;i++)
	{
		last[i].location=0;
		last[i].unit=0;
		last[i].count=0;
	}
	lastcounter=4;
	
	setpFitness(0);
	setHarvestedGas(0);
	setHarvestedMins(0);
	//location 0 is for globalForce, so take location 1 for starting point instead
	setMins(getPlayer()->getMins());
	setGas(getPlayer()->getGas());
	setTimer(getPlayer()->getTimer());
	setSupply(0);
	setMaxSupply(0);
	setTimeOut(ga->maxTimeOut);
	setIP(ga->maxLength-1);
	setCalculated(0);
	time=ga->maxTime;
	ready=0;
	setpFitness(0);
	setsFitness(0);
	settFitness(999);
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
	int i;
	mutationRate=500+rand()%1000;
	if((ga->preprocessBuildOrder)&&(basicLength>0))
	{
		memcpy(Code[0],basicBuildOrder[0],MAX_LENGTH*4);
		memcpy(Code[1],basicBuildOrder[1],MAX_LENGTH*4);
		for(i=0;i<MAX_LENGTH;i++)
		{
			markerCounter++;Marker[0][i]=markerCounter;
		        markerCounter++;Marker[1][i]=markerCounter;
		}
	}
	else
	for(i=MAX_LENGTH;i--;)
	{
		Code[0][i]=rand()%getPlayer()->goal->getMaxBuildTypes();
		Code[1][i]=rand()%getPlayer()->goal->getMaxBuildTypes();
		markerCounter++;Marker[0][i]=markerCounter;
		markerCounter++;Marker[1][i]=markerCounter;
	}
}

void RACE::crossOver(RACE* parent2, RACE* child1, RACE* child2)
{
	int i;
	int counter=MAX_LENGTH;
	for(i=0;i<MAX_LENGTH;i++)
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
	child2->mutationRate=(2*parent2->mutationRate+mutationRate)/3;
}

int RACE::setpFitness(int fitness)
{
	if(fitness<0)
	{
		debug.toLog(0,"DEBUG: (RACE::setpFitness): Value fitness [%i] out of range.",fitness);
		return(0);
	};
	pFitness=fitness;
	return(1);
};

int RACE::setsFitness(int fitness)
{
	if((fitness<0)||(fitness>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (RACE::setsFitness): Value fitness [%i] out of range.",fitness);
		return(0);
	};
	sFitness=fitness;
	return(1);
};

int RACE::settFitness(int fitness)
{
	if(fitness<0)
	{
		debug.toLog(0,"DEBUG: (RACE::settFitness): Value fitness [%i] out of range.",fitness);
		return(0);
	};
	tFitness=fitness;
	return(1);
};

int RACE::getpFitness()
{
	if((pFitness<0)||(pFitness>30000))
	{
		debug.toLog(0,"DEBUG: (RACE::getpFitness): Variable pFitness not initialized [%i].",pFitness);
		return(0);
	};
	return(pFitness);
};

int RACE::getsFitness()
{
	if((sFitness<0)||(sFitness>MAX_MINS+MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (RACE::getsFitness): Variable sFitness not initialized [%i].",sFitness);
		return(0);
	};
	return(sFitness);
};

int RACE::gettFitness()
{
	if((tFitness<0)||(tFitness>30000))
	{
		debug.toLog(0,"DEBUG: (RACE::gettFitness): Variable tFitness not initialized [%i].",tFitness);
		return(0);
	};
	return(tFitness);
};



RACE::RACE()
{
	setpFitness(0);
	setsFitness(0);
	settFitness(999);
};

RACE::~RACE()
{
};


int RACE::basicBuildOrder[2][MAX_LENGTH];
int RACE::basicLength;

