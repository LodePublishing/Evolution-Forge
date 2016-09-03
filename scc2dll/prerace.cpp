#include "prerace.h"
#include "debug.h"

void PRERACE::setStartConditions(START* pStart)
{
#ifdef _SCC_DEBUG
    if(!pStart)
    {
        debug.toLog(0,"DEBUG: (PRERACE::setStartConditions): Variable pStart not initialized [%i].",pStart);
        return;
    }
#endif
	start=pStart;
	pMap=start->getMap();
};

void PRERACE::replaceCode(int dominant, int IP, int num)
{
#ifdef _SCC_DEBUG
    if((IP<0)||(IP>=MAX_LENGTH))
    {
        debug.toLog(0,"DEBUG: (PRERACE::replaceCode): Value IP [%i] out of range.",IP);
        return;
    }
    if((num<0)||(num>=getPlayer()->getGoal()->getMaxBuildTypes()))
    {
        debug.toLog(0,"DEBUG: (PRERACE::replaceCode): Value num [%i] out of range.",num);
        return;
    }
    if((dominant<0)||(dominant>1))
    {
        debug.toLog(0,"DEBUG: (PRERACE::replaceCode): Value dominant [%i] out of range.",dominant);
        return;
    }
#endif
	Code[dominant][IP]=num;
	markerCounter++;Marker[dominant][IP]=markerCounter;
};

void PRERACE::initializeMap() // initializes the Map (units[][]) with values from 'start'
{
	for(int i=MAX_PLAYER;i--;)
		for(int j=MAX_LOCATIONS;j--;)
			units[i][j].copy(start->getMap()->getLocation(j)->getUnits(i));
};
	
int PRERACE::calculatePrimaryFitness(int ready)
{
	int tpF=0;
//TODO evtl noch uebrige availible miteinbeziehen
//	int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; // temporary data to check whether a bonus is already given (only applies if total > goal)
//TODO: Nicht alle Einheiten am Ort? => Ort egal sein lassen aber zur Zeit hinzuzaehlen
	// Nicht alle Einheiten ueberhaupt gebaut UND nicht alle am Ort => nur viertel Bonus fuer Einheiten die nicht am Ort sind
	if(!ready)
	{
		//calculate number of fulfilled goals & their time & their distance to goal position
		for(int i=MAX_GOALS;i--;)
			if(getPlayer()->getGoal()->goal[i].count>0)
			{
// Haben wir zuwenig Einheiten gebaut?  total < goal
if( /*((getPlayer()->getGoal()->goal[i].location==0)&&(getPlayer()->getGoal()->goal[i].count>getLocationForce(0,getPlayer()->getGoal()->goal[i].unit))) || unnuetz*/
  (  /*(getPlayer()->getGoal()->goal[i].location>0)&&*/(getPlayer()->getGoal()->goal[i].count>getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit))) )
				{
					//total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
					//TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen
					//evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!
					int sumup=0;
					int bon=0;
// location 0? Dann ist es trivial
					if(getPlayer()->getGoal()->goal[i].location==0)
						sumup=getLocationForce(0,getPlayer()->getGoal()->goal[i].unit)*100;
					else
					{
				// location != 0 ? Dann schaun wir mal, ob wir auf der Karte noch wo Units verstreut haben
				// mehr als goalcount koennen wir keinen Bonus vergeben
						bon=getPlayer()->getGoal()->goal[i].count;
						int j=1;
						int loc=pMap->getNearestLocation(getPlayer()->getGoal()->goal[i].location,j);
						while((j<MAX_LOCATIONS)&&(bon>getLocationForce(loc,getPlayer()->getGoal()->goal[i].unit)))
						{
							sumup+=getLocationForce(loc,getPlayer()->getGoal()->goal[i].unit)*(100-pMap->getDistance(loc,getPlayer()->getGoal()->goal[i].location)); //was pMap->location[j]...
							bon-=getLocationForce(loc,getPlayer()->getGoal()->goal[i].unit);
							j++;
						}
						// Falls j<MAX_LOCATIONS => unser "Bon" wurde schon vorher aufgebraucht => An dieser Stelle j den Rest draufgeben... 
						if(j<MAX_LOCATIONS)
							sumup+=bon*(100-pMap->getDistance(loc,getPlayer()->getGoal()->goal[i].location));
					}

//jetzt steht in sumup die gesammelten totals gewichtet mit den Entfernungen zum Ziel
										//TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
										// evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
										// bissl komplex da mans ja den einzelnen goals verteilen muss...
										if((getPlayer()->getGoal()->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->getGoal()->goal[i].time))
//									  {
//											  if(getFinalTime(i)>0) //??? TODO
//											  if(getFinalTime(i)>getPlayer()->getGoal()->goal[i].time)
														tpF+=(getPlayer()->getGoal()->goal[i].time*sumup)/(getPlayer()->getGoal()->goal[i].count*getFinalTime(i));
//											  else setpFitness(getpFitness()+sumup/getPlayer()->getGoal()->goal[i].count);
//TODO...~~~
//											  else while(true); // <- kann eigentlich nicht auftreten!
//													  setpFitness(getpFitness()+(getPlayer()->getGoal()->goal[i].time*sumup)/(getPlayer()->getGoal()->goal[i].count*ga->maxTime));
//									  }
										else
												tpF+=sumup/getPlayer()->getGoal()->goal[i].count;
								} // END total < goal
								else
//if( /*((getPlayer()->getGoal()->goal[i].location==0)&&(getPlayer()->getGoal()->goal[i].count<=getLocationForce(0,getPlayer()->getGoal()->goal[i].unit))) || ( (getPlayer()->getGoal()->goal[i].location>0)&&*/(getPlayer()->getGoal()->goal[i].count<=getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit)))
								//total >= goal ?
								{
// Checken wann wir das Ziel erreicht haetten
										if((getPlayer()->getGoal()->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->getGoal()->goal[i].time))
// aha, wir haben die Zeit ueberschritten => trotzdem anteilig Bonus geben
												tpF+=(getPlayer()->getGoal()->goal[i].time*100/getFinalTime(i));
// keine Zeitbeschraenkung + wir haben genuegend Einheiten am Zielort => gg
										else tpF+=100;
// does not work yet, if this is uncommented, sFitness occasionally jumps to -1222000 or something like that... :/
// include the final location maybe...
								}
						} //end of goal checking
// TODO: Check for very small 'goal.time' values, probably in scc.cpp!!
															                                                                                                
//Bonus: Hier werden Einheiten verarbeitet die noch in Produktion sind, aber Teil der goals sind
															                                                                                                
//Erstmal Maximalbonus errechnen (nicht, dass die engine dann 50 Battlecruiser kurz vor Schluss noch anfaengt oder so :P )
/*			  for(i=MAX_LOCATIONS;i--;)
					   for(j=UNIT_TYPE_COUNT;j--;)
							   bonus[i][j]=0;
				for(i=MAX_GOALS;i--;)
						if(getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit)<getPlayer()->getGoal()->goal[i].count)
								bonus[getPlayer()->getGoal()->goal[i].location][getPlayer()->getGoal()->goal[i].unit]+=getPlayer()->getGoal()->goal[i].count-getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit);
//bonus ist jetzt mit den uebrigen Einheiten bis zur Zielerfuellung belegt
				for(i=MAX_BUILDINGS;i--;)
						if((build->getRemainingBuildTime(i)>0)&&(bonus[build->getLocation(i)][build->getType(i)]>0))
						{
						//erstmal ohne Zeit...
								pFitness+=((build->getRemainingBuildTime(i)*100)/((getLocationForce(build->getLocation(i),build->getType(i))+bonus[build->getLocation(i)][build->getType(i)])*pStats[build->getType(i)].BT)); //TODO in ProgramBT aendern
															                                                                                                
								if((getPlayer()->getGoal()->goal[build->getType(i)].time>0)&&(getLocationForce(build->getLocation(i),build->getType(i))==0))
										pFitness+=(build->getRemainingBuildTime(i)*100*getPlayer()->getGoal()->goal[build->getType(i)].time*getLocationForce(0,i))/(getPlayer()->getGoal()->goal[build->getType(i)].count*pStats[build->getType(i)].BT*ga->maxTime);//hier auch ProgramBT
								else
										pFitness+=((build->getRemainingBuildTime(i)*100)/(getPlayer()->getGoal()->goal[build->getType(i)].count*pStats[build->getType(i)].BT));
								bonus[build->getLocation(i)][build->getType(i)]--;
						}*/
															                                                                                                
		} // end of ready=false
		else   // all goals fulfilled, fitness <- timer
	{
		tpF+=getTimer();
				for(int i=MAX_GOALS;i--;)
				{
						if(getPlayer()->getGoal()->goal[i].count>0)
								tpF+=100;
/*					  if((getPlayer()->getGoal()->goal[i].unit!=GAS_SCV)&&(getPlayer()->getGoal()->goal[i].unit!=SCV)) //do not punish 'too much' workers!
								if(getPlayer()->getGoal()->goal[i].count<getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit))
										setsFitness(getsFitness()+(-getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit)+getPlayer()->getGoal()->goal[i].count)*(stats[getPlayer()->getGoal()->getRace()][getPlayer()->getGoal()->goal[i].unit].mins+stats[getPlayer()->getGoal()->getRace()][getPlayer()->getGoal()->goal[i].unit].gas));*/
				}
	}
	return(tpF);
}
// end of calculatePrimaryFitness

void EXPORT PRERACE::createSpecial()
{
	if(getPlayer()->getGoal()->getRace()==ZERG)
		for(int i=0;i<larvacounternumber;i++)
			if(!--larva[i].counter)
			{
				larva[i].counter=20;
				if(larva[i].larvacount<3)
				{
					larva[i].larvacount++;
					addLocationForce(larva[i].location,LARVA,1);
					addLocationAvailible(larva[i].location,LARVA,1);					
				}
			}
	//TODO energy, regeneration life/shield etc.
	// TODO linked list draus machen!
};

int EXPORT PRERACE::getMapLocationAvailible(int player, int loc, int type)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationAvailible): Value player [%i] out of range.",player);
		return(0);
	}
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
#endif
	return(units[player][loc].getAvailible(type));
};

int EXPORT PRERACE::getMapLocationForce(int player, int loc, int type)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationForce): Value player [%i] out of range.",player);
		return(0);
	}
																				
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
#endif
	return(units[player][loc].getTotal(type));
};

//TODO Static machen
int EXPORT PRERACE::setMapLocationAvailible(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationAvailible): Value player [%i] out of range.",player);
		return(0);
	}
																				
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
#endif
	units[player][loc].setAvailible(type, num);
	return(1);
};
																				
int EXPORT PRERACE::setMapLocationForce(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value player [%i] out of range.",player);
		return(0);
	}
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((num<0)||(num>=MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	units[player][loc].setTotal(type, num);
	return(1);
};

int EXPORT PRERACE::addMapLocationAvailible(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Value player [%i] out of range.",player);
		return(0);
	}
																				
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
#endif
	units[player][loc].addAvailible(type, num);
	return(1);
};


int EXPORT PRERACE::addMapLocationForce(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Value player [%i] out of range.",player);
		return(0);
	}
																				
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
#endif
	units[player][loc].addTotal(type, num);
	return(1);
};
																				

//----

int EXPORT PRERACE::getLocationAvailible(int loc, int type)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].getAvailible(type)<0)||(location[loc].getAvailible(type)>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationAvailible): Variable location[%i].getAvailible(%i] not initialized [%i].",loc,type,location[loc].getAvailible(type));
		return(0);
	}
#endif
	return(location[loc].getAvailible(type));
};

int EXPORT PRERACE::getLocationForce(int loc, int type)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].getTotal(type)<0)||(location[loc].getTotal(type)>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationForce): Variable location[%i].getTotal(%i) not initialized [%i].",loc,type,location[loc].getTotal(type));
		return(0);
	}
#endif
	return(location[loc].getTotal(type));
};


int EXPORT PRERACE::setLocationAvailible(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationAvailible): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	location[loc].setAvailible(type,num);
	return(1);
};

int EXPORT PRERACE::setLocationForce(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationForce): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	location[loc].setTotal(type, num);
	return(1);
};


int EXPORT PRERACE::addLocationAvailible(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].getAvailible(type)<0)||(location[loc].getAvailible(type)>MAX_SUPPLY)) //location[0] auch pruefen?
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Variable location[%i].getAvailible(%i] not initialized [%i].",loc,type,location[loc].getAvailible(type));
		return(0);
	}
	if((location[loc].getAvailible(type)+num<0)||(location[loc].getAvailible(type)+num>MAX_SUPPLY)) //location[0] auch pruefen?
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Value num [%i] out of range.",num);
		return(0);
	}
#endif	
	location[loc].addAvailible(type, num);
	if(loc>0) //sonst waers ja doppelt...
		location[0].addAvailible(type, num);
	return(1);
};
																				
int EXPORT PRERACE::addLocationForce(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].getTotal(type)<0)||(location[loc].getTotal(type)>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Variable location[%i].getTotal[%i] not initialized [%i].",loc,type,location[loc].getTotal(type));
		return(0);
	}
	if((location[loc].getTotal(type)+num<0)||(location[loc].getTotal(type)+num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Variable num not initialized [%i].",num);
		return(0);
	}
#endif
	location[0].addTotal(type,num);
	if(loc>0)
		location[loc].addTotal(type, num);
	return(1);
};

MAP* PRERACE::getMap()
{
//TODO Fehlerbehandlung
	return(pMap);
};

int EXPORT PRERACE::setMins(int mins)
{
#ifdef _SCC_DEBUG
	if((mins<0)||(mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMins): Value mins [%i] out of range.",mins);
		return(0);
	}
#endif
	this->mins=mins;
	return(1);
};

int EXPORT PRERACE::setGas(int gas)
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setGas): Value gas [%i] out of range.",gas);
		return(0);
	}
#endif
	this->gas=gas;
	return(1);
};

int EXPORT PRERACE::setTimer(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setTimer): Value time [%i] out of range.",time);
		return(0);
	}
#endif
	if(time==0)
	{
		timer=0;
	}
   	else
	timer=time;
	return(1);
};

int EXPORT PRERACE::setSupply(int supply)
{
#ifdef _SCC_DEBUG
	if((supply>MAX_SUPPLY)||(supply<0))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
#endif
	this->supply=supply;
	return(1);
};

int EXPORT PRERACE::setMaxSupply(int supply)
{
#ifdef _SCC_DEBUG
	if((supply>MAX_SUPPLY)||(supply<0))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMaxSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
#endif
	maxSupply=supply;
	return(1);
};
/*
int EXPORT PRERACE::setMap(MAP* map)
{
#ifdef _SCC_DEBUG
	if(!map)
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMap): Map not initialized.");
		return(0);
	}
#endif
	pMap=map;
	mapInitialized=1;
	return(1);
};*/


int EXPORT PRERACE::getPlayerNum()
{
#ifdef _SCC_DEBUG
	if((playerNum<0)||(playerNum>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getPlayer): Variable not initialized [%i].",playerNum);
		return(0);
	}
#endif
	return(playerNum);
};

int EXPORT PRERACE::getSupply()
{
#ifdef _SCC_DEBUG
	if((supply<0)||(supply>200))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getSupply): Variable not initialized [%i].",supply);
		return(0);
	}
#endif
	return(supply);
};
int EXPORT PRERACE::getMaxSupply()
{
#ifdef _SCC_DEBUG
	if((maxSupply<0)||(maxSupply>200))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMaxSupply): Variable not initialized [%i].",maxSupply);
		return(0);
	}
#endif
	return(maxSupply);
};
int EXPORT PRERACE::getMins()
{
#ifdef _SCC_DEBUG
	if((mins<0)||(mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMins): Variable mins not initialized [%i].",mins);
		return(0);
	}
#endif
	return(mins);
};
int EXPORT PRERACE::getGas()
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGas): Variable gas not initialized [%i].",gas);
		return(0);
	}
#endif
	return(gas);
};
int EXPORT PRERACE::getTimer()
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getTimer): Variable timer not initialized [%i].",timer);
		return(0);
	}
#endif
	return(timer);
};


int EXPORT PRERACE::adjustMineralHarvest(int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustMineralHarvest): Value location [%i] out of range.",location);
		return(0);
	}
#endif
	//TODO Zerg hatchery,lair etc.
	if((location==0)||((!getLocationForce(location,COMMAND_CENTER))&&(!getMapLocationForce(0,location,MINERALS))))
	{
		for(int i=45;i--;)
			setMineralHarvestPerSecond(location,i,0);
	}
//	else if((!pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(pMap->location[num].getTotal(0][MINERALS]))
//	{
	//nach naehestem command_center suchen
//	}
//	else if((pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(!pMap->location[num].getTotal(0][MINERALS]))
//	{
	//nach naehesten Mineralien suchen
//	} TODO

	//TODO: Wenn 2 SPieler an einem sammeln, beide einberechnen!
	else if(startPlayer->getBasicMineralHarvestPerSecond(1)>0) //???
	{
//		int k;
		for(int i=45;i--;)
		{
//			k=0;
//			for(j=0;j<45;j++)
//				if(i*8<=j*pMap->location[num].getTotal(0][MINERALS]) 
//				{ k=j;j=45;};
			setMineralHarvestPerSecond(location,i,startPlayer->getBasicMineralHarvestPerSecond(i/*k*/));//*pMap->location[num].getTotal(0][MINERALS])/8;
		}	//ab hier funzt alles... nur scheint startPlayer->getBasic... nicht richtig initialisiert zu sein...
	
	}
	return(1);
};

int EXPORT PRERACE::adjustGasHarvest(int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustGasHarvest): Value location [%i] out of range.",location);
		return(0);
	}
#endif
	if((location==0)||((!getLocationForce(location,COMMAND_CENTER))&&(!getLocationForce(location,REFINERY))))
	{
		for(int i=5;i--;)
			setGasHarvestPerSecond(location,i,0);
	}
/*	else if((!pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(pMap->location[num].getTotal(playerNum][REFINERY]))
	{
	//nach naehestem command_center suchen
	}
	else if((pMap->location[num].getTotal(playerNum][COMMAND_CENTER])&&(!pMap->location[num].getTotal(playerNum][REFINERY]))
	{
	//nach naehesten Mineralien suchen
	}*/
	else
	{
		int k;
		for(int i=5;i--;)
		{
			k=0;
//			for(j=0;j<5;j++)
//				if(i<=j*pMap->location[num].getTotal(playerNum][REFINERY]) { k=j;j=5;}
					setGasHarvestPerSecond(location,i,startPlayer->getBasicGasHarvestPerSecond(i/*k*/)*getLocationForce(location,REFINERY));

//					hier liegts problem wegen gas
//						gasharvestps ist zu gross, evtl wegen zu vieler refineries!
		}
	}
	return(1);
};


void EXPORT PRERACE::setPlayer(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setPlayer): Value [%i] out of range.",num);
		return;
	}
#endif
	playerNum=num; //~```~  player[0]?
	startPlayer=start->getMap()->getStartPlayer(num);
	setpStats(startPlayer->getpStats());
	location=&units[num][0];
	global=&units[num][0];
};


PLAYER* EXPORT PRERACE::getPlayer()
{
#ifdef _SCC_DEBUG
	if(!startPlayer->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PRERACE::getPlayer): Variable not initialized.");
		return(0);
	}
#endif
	return(startPlayer);
};


/*int EXPORT PRERACE::setPlayer(PLAYER* player)
{
#ifdef _SCC_DEBUG
	if(!startPlayer->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PRERACE::setPlayer): Variable not initialized.");
		return(0);
	}
#endif
	this->player=player;
	return(1);
};*/

int EXPORT PRERACE::setpStats(const UNIT_STATISTICS* pStats)
{
#ifdef _SCC_DEBUG
	if(pStats[0].mins!=0)//~~
	{
		debug.toLog(0,"DEBUG: (PRERACE::setpStats): Variable not initialized.");
		return(0);
	}
#endif
	this->pStats=pStats;
	return(1);
};


const UNIT_STATISTICS* EXPORT PRERACE::getpStats()
{
#ifdef _SCC_DEBUG
	if(!pStats)
	{
		debug.toLog(0,"DEBUG: (PRERACE::getpStats): Variable not initialized.");
		return(0);
	}
#endif
	return(pStats);
};


void EXPORT PRERACE::initializePlayer()
{
	setMins(startPlayer->getMins());
	setGas(startPlayer->getGas());
	setTimer(ga->maxTime-startPlayer->getTimer());
	setSupply(startPlayer->getSupply());
	setMaxSupply(startPlayer->getMaxSupply());
};

int EXPORT PRERACE::adjustHarvest()
{
#ifdef _SCC_DEBUG
/*	if(!mapInitialized) 
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustHarvest): Map was not initialized.");
		return(0);
	}*/
	//TODO
#endif
	for(int i=0;i<MAX_LOCATIONS;i++)
	{
		adjustMineralHarvest(i);
		adjustGasHarvest(i);
	}
	return(1);
}


int EXPORT PRERACE::harvestMinerals()
{
	int sum=0;
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
			setMins(getMins()+getMineralHarvestPerSecond(i,s));
			//~~~ neu berechnen / Tabelle anlegen! sind ja nicht bei jeder Exe gleichviel Mineralien... <- das sollte sich mit adjustHarvest erledigt haben
			harvestedMins+=getMineralHarvestPerSecond(i,s); //~~~
		}
		else
		{
			setMins(getMins()+getMineralHarvestPerSecond(i,44)); //war 56
			harvestedMins+=getMineralHarvestPerSecond(i,44);
		}*/
		}
	}
	return(sum);
}


int EXPORT PRERACE::harvestGas()
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
};

int EXPORT PRERACE::getCalculated()
{
#ifdef _SCC_DEBUG
	if((calculated<0)||(calculated>1))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getCalculated): Variable is not initialized [%i].",calculated);
		return(0);
	}
#endif
	return(calculated);
};

int EXPORT PRERACE::setCalculated(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>1))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setCalculated): Value [%i] out of range.",num);
		return(0);
	}
#endif
	calculated=num;
	return(1);
};


// with that deals already the player class!

/*int EXPORT PRERACE::resetSupply()
{
#ifdef _SCC_DEBUG
	if(!mapInitialized)
	{
		debug.toLog(0,"DEBUG: (PRERACE::resetSupply): Map was not initialized.");
		return(0);
	}
#endif
	setMaxSupply(0);
	int temp=0;
	for(int i=1;i<MAX_LOCATIONS;i++)
		for(int j=UNIT_TYPE_COUNT;j--;)
		{
			if(pStats[j].supply<0) // -> supply building like depot, cc etc.
			{
				if(getMaxSupply()-pStats[j].supply*location[i].getTotal(playerNum,j)>MAX_SUPPLY)
				{
					if(getMaxSupply()<MAX_SUPPLY)
					{
	//TODO!!				
						temp=getMaxSupply()+(MAX_SUPPLY-getMaxSupply());
						setMaxSupply(MAX_SUPPLY);
					}
				}
				else
				{
	//TODO handle negative supply at beginning
					temp-=pStats[j].supply*location[i].getTotal(playerNum,j);
					setMaxSupply(getMaxSupply()-pStats[j].supply*location[i].getTotal(playerNum,j));
				}
			} else //unit or something
				temp-=pStats[j].supply*location[i].getTotal(playerNum,j);
		}
	setSupply(temp);
	return(1);
};*/


int EXPORT PRERACE::getIP()
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getIP): Variable IP not initialized [%i].",IP);
		return(0);
	}
#endif
	return(IP);
}

int EXPORT PRERACE::getMineralHarvestPerSecond(int location, int worker)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Value location [%i] out of range.",location);
		return(0);
	}
	if((worker<0)||(worker>=45))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Value worker [%i] out of range.",worker);
		return(0);
	}

	if((mineralHarvestPerSecond[location][worker]<0)||(mineralHarvestPerSecond[location][worker]>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Variable mineralHarvestPerSecond[%i][%i] not initialized [%i].",location,worker,mineralHarvestPerSecond[location][worker]);
		return(0);
	}
#endif
	return(mineralHarvestPerSecond[location][worker]);
}

int EXPORT PRERACE::getGasHarvestPerSecond(int location,int worker)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Value location [%i] out of range.",location);
		return(0);
	}
	if((worker<0)||(worker>=5))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Value worker [%i] out of range.",worker);
		return(0);
	}
	if((gasHarvestPerSecond[location][worker]<0)||(gasHarvestPerSecond[location][worker]>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Variable gasHarvestPerSecond[%i][%i] not initialized [%i].",location,worker,gasHarvestPerSecond[location][worker]);
		return(0);
	}
#endif
	return(gasHarvestPerSecond[location][worker]);
}

int EXPORT PRERACE::getHarvestedMins()
{
#ifdef _SCC_DEBUG
	if((harvestedMins<0)||(harvestedMins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getHarvestedMins): Variable harvestedMins not initialized [%i].",harvestedMins);
		return(0);
	}
#endif
	return(harvestedMins);
};

int EXPORT PRERACE::getHarvestedGas()
{
#ifdef _SCC_DEBUG
	if((harvestedGas<0)||(harvestedGas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getHarvestedGas): Variable harvestedGas not initialized [%i].",harvestedGas);
		return(0);
	}
#endif
	return(harvestedGas);
};

int EXPORT PRERACE::getFinalTime(int goal)
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=MAX_GOALS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getFinalTime): Value goal [%i] out of range.",goal);
		return(0);
	}
	if((ftime[goal]<0)||(ftime[goal]>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getFinalTime): Variable ftime[%i] not initialized [%i].",goal,ftime[goal]);
		return(0);
	}
#endif
	return(ftime[goal]);
};

int EXPORT PRERACE::getLength()
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLength): Variable length not initialized [%i].",length);
		return(0);
	}
#endif
	return(length);
};

int EXPORT PRERACE::setIP(int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setIP): Value IP [%i] out of range.",IP);
		return(0);
	}
#endif
	this->IP=IP;
	return(1);
};

int EXPORT PRERACE::setMineralHarvestPerSecond(int location,int worker,int mins)
{
#ifdef _SCC_DEBUG
	if((mins<0)||(mins>=MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMineralHarvestPerSecond): Value mins [%i] out of range.",mins);
		return(0);
	}
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMineralHarvestPerSecond): Value location [%i] out of range.",location);
		return(0);
	}
	if((worker<0)||(worker>=45))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMineralHarvestPerSecond): Value worker [%i] out of range.",worker);
		return(0);
	}
#endif
	mineralHarvestPerSecond[location][worker]=mins;
	return(1);
};

int EXPORT PRERACE::setGasHarvestPerSecond(int tloc,int worker,int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_GAS)||(tloc<0)||(tloc>=MAX_LOCATIONS)||(worker<0)||(worker>=5))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setGasHarvestPerSecond): Value [%i/%i/%i] out of range.",tloc,worker,num);
		return(0);
	}
#endif
	gasHarvestPerSecond[tloc][worker]=num;
	return(1);
};

int EXPORT PRERACE::setHarvestedMins(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setHarvestedMins): Value [%i] out of range.",num);
		return(0);
	}
#endif
	harvestedMins=num;
	return(1);
};

int EXPORT PRERACE::setHarvestedGas(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setHarvestedGas): Value [%i] out of range.",num);
		return(0);
	}
#endif
	harvestedGas=num;
	return(1);
};

int EXPORT PRERACE::setFinalTime(int goal, int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setFinalTime): Value time [%i] out of range.",time);
		return(0);
	}
		if((goal<0)||(goal>=MAX_GOALS))
		{
				debug.toLog(0,"DEBUG: (PRERACE::setFinalTime): Value goal [%i] out of range.",goal);
				return(0);
		}

#endif
	ftime[goal]=time;
	return(1);
};

int EXPORT PRERACE::setLength(int length)
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLength): Value length [%i] out of range.",length);
		return(0);
	}
#endif
	this->length=length;
	return(1);
};


int EXPORT PRERACE::setTimeOut(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIMEOUT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setTimeOut): Value time [%i] out of range.",time);
		return(0);
	}
#endif
	timeout=time;
	return(1);
};

int EXPORT PRERACE::getTimeOut()
{
#ifdef _SCC_DEBUG
	if((timeout<0)||(timeout>MAX_TIMEOUT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getTimeOut): Variable timeout not initialized [%i].",timeout);
		return(0);
	}
#endif
	return(timeout);
};

//void EXPORT PRERACE::resetMapInitialized()
//{
	//mapInitialized=0;	
//};

void EXPORT PRERACE::resetSpecial()
{
	for(int i=0;i<20;i++)
	{
		larva[i].counter=0;
		larva[i].location=0;
		larva[i].larvacount=0;
	}
	larvacounternumber=0;
	if(getPlayer()->getGoal()->getRace()==ZERG)
	{
		for(int i=1;i<MAX_LOCATIONS;i++)
		{
			int j=(getLocationForce(i,HATCHERY)+getLocationForce(i,LAIR)+getLocationForce(i,HIVE));
			if(j)
			{
				for(int k=0;k<j;k++)
				{
					larva[larvacounternumber].counter=20;
					larva[larvacounternumber].location=i;
					larva[larvacounternumber].larvacount=1;
					larvacounternumber++;
					addLocationForce(i,LARVA,1);
					addLocationAvailible(i,LARVA,1);	
				}
			}
		}
	}
}

void PRERACE::adjustAvailibility(int loc,int fac,const UNIT_STATISTICS* stat)
{
	switch(stat->facility_type)
	{
		case IS_LOST:
			if(stat->facility[fac]>0)
			{
				addLocationAvailible(loc,stat->facility[fac],-1);
				setSupply(getSupply()+pStats[stat->facility[fac]].supply);
				if((getPlayer()->getGoal()->getRace()==ZERG)&&(stat->facility[fac]==LARVA))
				{
					int bestPlace=0;//unschoen hier :/
					int bestCounter=0;
					for(int i=0;i<larvacounternumber;i++)
						if((larva[i].location==loc)&&(larva[i].larvacount>bestCounter))
							{bestCounter=larva[i].larvacount;bestPlace=i;}
					larva[bestPlace].larvacount--;
				}
			}
			if(stat->facility2>0)
			{
				addLocationAvailible(loc,stat->facility2,-1);
				setSupply(getSupply()+pStats[stat->facility2].supply);
			}
			break;
		case NEEDED_ONCE:
			break;
		case NEEDED_UNTIL_COMPLETE:
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			if(stat->facility2>0)
				addLocationAvailible(loc,stat->facility2,-1);
				//kommt glaub nicht vor...
			break;
		case NEEDED_ONCE_IS_LOST:
			if(stat->facility2>0)
			{
				addLocationAvailible(loc,stat->facility2,-1);
				setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/ eigentlich schon... bei gas scv z.B. :/
			}
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST:
			if(stat->facility2>0)
			{
				addLocationAvailible(0/*location*/ ,stat->facility2,-1); // primarily for temporary R_researches, have to be location 0
// TODO				 setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/
			}
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
			if(stat->facility2>0)
				addLocationAvailible(0/*loc*/,stat->facility2,-1);
			// no supply gain as the item is recovered... well or not? mmmh... ~~~~
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			break;
		case NEEDED_ALWAYS:
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
				// supply?
			if(stat->facility2>0)
				addLocationAvailible(loc,stat->facility2,-1);
			break;
		default:debug.toLog(0,"DEBUG: (ANARACE::buildGene) default case reached!");break;
	}
}


int PRERACE::calculateReady()
{
	int ready=1;
	for(int i=MAX_GOALS;(i--)&&(ready);)
		if(getPlayer()->getGoal()->goal[i].count)
			ready&=((getPlayer()->getGoal()->goal[i].count<=getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit))&&((getPlayer()->getGoal()->goal[i].time>=getFinalTime(i))||(getPlayer()->getGoal()->goal[i].time==0)));
	return(ready);
}


PRERACE::PRERACE()
{
	calculated=0;
	startPlayer=0;
	mins=0;
	gas=0;
	lastcounter=0;
	lastunit=0;
	timer=MAX_TIME;
	setSupply(0);
	setMaxSupply(0);
	harvestedGas=0;
	harvestedMins=0;
	setLength(0);
	//todo: mehr auf 0 setzen!
	for(int i=0;i<MAX_LOCATIONS;i++)
	{
		for(int j=0;j<45;j++)
			setMineralHarvestPerSecond(i,j,0);
		for(int j=0;j<5;j++)
			setGasHarvestPerSecond(i,j,0);
	};
	for(int i=0;i<MAX_LENGTH;i++)
	{
		Code[0][i]=0;
		Code[1][i]=0;
		last[i].unit=0;
		last[i].location=0;
		last[i].count=0;
	};
	for(int i=0;i<20;i++)
	{
		larva[i].counter=0;
		larva[i].location=0;
		larva[i].larvacount=0;
	}
	larvacounternumber=0;
};

PRERACE::~PRERACE()
{
};

void PRERACE::initNoise()
{
//	if(ga->noise>0)
//		for(int j=0;j<MAX_TIME;j++)
//			noise[j]=rand()%ga->noise-rand()%ga->noise;
//	else TODO
		for(int j=0;j<MAX_TIME;j++)
			noise[j]=0;
};

void PRERACE::resetGeneMarker()
{
	markerCounter=1;
};

//void PRERACE::resetMapInitialized();
//int PRERACE::setMap(MAP* map);
//MAP* PRERACE::getMap();
START* PRERACE::start;
MAP* PRERACE::pMap;
int PRERACE::noise[MAX_TIME];
int PRERACE::markerCounter;
UNITS PRERACE::units[MAX_PLAYER][MAX_LOCATIONS];
int PRERACE::startInitialized;

void PRERACE::setStartConditions(START* pStart);
void PRERACE::initNoise();
void PRERACE::initializeMap();

int PRERACE::getMapLocationAvailible(int player, int loc, int type);
int PRERACE::getMapLocationForce(int player, int loc, int type);
                                                                                
int PRERACE::setMapLocationAvailible(int player, int loc, int type, int num);
int PRERACE::setMapLocationForce(int player, int loc, int type, int num);
                                                                                
int PRERACE::addMapLocationAvailible(int player, int loc, int type, int num);
int PRERACE::addMapLocationForce(int player, int loc, int type, int num);
void PRERACE::resetGeneMarker();

GA* PRERACE::ga;
//int PRERACE::mapInitialized;
