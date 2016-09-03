#include "prerace.hpp"

/* PRERACE.CPP - last changed: 6/23/04								*
 * Author: Clemens Lode												*
 * Copyright: Clemens Lode											*
 *																	*
 *  PURPPOSE OF THIS FILE:										    *
 *		Supply various functions for the classes RACE and ANARACE,  *
 * 		mainly concerning harvesting, supply and units				*/


PRERACE::PRERACE()
{
	location=0;
	pStartCondition=0;
	pGoal=0;
	minerals=0;
	gas=0;
	lastcounter=0;
	lastunit=0;
	timer=MAX_TIME;
	setNeedSupply(0);
	setHaveSupply(0);
	harvestedGas=0;
	harvestedMinerals=0;
	playerNum=0;
	setLength(0);
	for(int i=MAX_LOCATIONS;i--;)
	{
		for(int j=45;j--;)
			setMineralHarvestPerSecond(i,j,0);
		for(int j=5;j--;)
			setGasHarvestPerSecond(i,j,0);
	}
	for(int i=MAX_LENGTH;i--;)
	{
		Code[i]=0;
		last[i].unit=0;
		last[i].location=0;
		last[i].count=0;
	}
	resetSpecial();
}


void PRERACE::prepareForNewGeneration()
{
    setHarvestedGas(0);setHarvestedMinerals(0);
    setWastedGas(0);setWastedMinerals(0);

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
    ready=false;

    resetSpecial();
}                                                                                                                                                            

PRERACE::~PRERACE()
{
}

const eRace PRERACE::getRace() const
{
	return(pStart->getPlayerRace(playerNum));
}

// -----------------------------------
// ------ CONTROLS FROM OUTSIDE ------
// -----------------------------------

void PRERACE::replaceCode(const int IP, const int code)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::replaceCode): Value IP out of range.");return;
	}
	if((code<0)||(code>=(*pGoal)->getMaxBuildTypes())) {
		toLog("DEBUG: (PRERACE::replaceCode): Value code out of range.");return;
	}
#endif
	this->Code[IP]=code;
	markerCounter++;Marker[IP]=markerCounter;
}

// ------------------------------------------
// ------ END OF CONTROLS FROM OUTSIDE ------
// ------------------------------------------



// --------------------------------------------------------------
// ------ FUNCTIONS THAT ARE PRIMARILY USED DURING THE RUN ------
// --------------------------------------------------------------

const int PRERACE::calculateIdleTime() const
{
    int idle_time=getTimer();
	int harvest_minerals = harvestMinerals();
	int harvest_gas = harvestGas();

    if(!buildingQueue.empty()) {
		// buildingRemaining = time until the building will be completed
        int buildingRemaining = getTimer() - buildingQueue.top().getBuildFinishedTime();
        if(idle_time > buildingRemaining) 
			idle_time = buildingRemaining;
    }

    if( harvest_gas ) //&& 
//	  ( harvest_minerals || 
  //    	(neededGas + harvest_gas - neededGas % harvest_gas)/harvestGas() < (neededMinerals+(harvest_minerals - neededMinerals % harvest_minerals))/harvest_minerals)) 
	{
        int gasRemaining=(neededGas+(harvest_gas-neededGas % harvest_gas))/harvest_gas;
        if(idle_time>gasRemaining) 
			idle_time=gasRemaining;
    }
    else if(harvest_minerals) {
        int mineralsRemaining=(neededMinerals+(harvest_minerals-neededMinerals%harvest_minerals))/harvest_minerals;
        if(idle_time>mineralsRemaining) idle_time=mineralsRemaining;
    }
    if(idle_time>getTimeOut()) {
        idle_time=getTimeOut();
    }
#ifdef _SCC_DEBUG
	if(idle_time<0)
		toLog("ERROR idle time < 0");
#endif
	return(idle_time);
}

// called within the calculateStep function
void PRERACE::adjustLocationUnitsAfterCompletion(const int location, const eFacilityType facilityType, const int facility, const int facility2)
{
	switch(facilityType)
	{
		case NO_FACILITY:break;
		case IS_LOST:
			if(facility)
				removeOneLocationTotal(location, facility);
				//availible was already taken account when starting the building
			if(facility2)
				removeOneLocationTotal(location, facility2);
			break;
		case NEEDED_ONCE:break;
		case NEEDED_UNTIL_COMPLETE:
			if(facility)
				addOneLocationAvailible(location, facility);
			break; // fuer spaeter mal: Wenn in 2 Fabriken produziert wuerde wirds probmelatisch, da
//in Buiding nur eine facility gespeichert wird...
		case NEEDED_ONCE_IS_LOST:
			if(facility2)
				removeOneLocationTotal(location, facility2);
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST:
			// this is a research, no supply needed, applied to GLOBAL
			if(facility)
				addOneLocationAvailible(location, facility);
			if(facility2)
				removeOneLocationTotal(GLOBAL, facility2);
//r_researches need location 0
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
			// this is an update, no supply, applies to GLOBAL
			if(facility)
				addOneLocationAvailible(location, facility);
			if(facility2) // special rule for upgrades!
			{
				removeOneLocationTotal(GLOBAL, facility2);
				addOneLocationAvailible(GLOBAL, facility2);
			}
			break;
		case NEEDED_ALWAYS:break;
		case NO_FACILITY_BEHAVIOUR_DEFINED:
		default:
#ifdef _SCC_DEBUG
				toLog("ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
#endif
				break;
	} // end switch facilityType
}

void PRERACE::adjustAvailibility(const int loc, const int fac, const UNIT_STATISTICS* stat)
{
	switch(stat->facilityType)
	{
		case NO_FACILITY:break;
		case IS_LOST:
			if(stat->facility[fac]>0)
			{
				addLocationAvailible(loc,stat->facility[fac],-1);
				setNeedSupply(getNeedSupply()-(*pStats)[stat->facility[fac]].needSupply);
				setHaveSupply(getHaveSupply()-(*pStats)[stat->facility[fac]].haveSupply);
			}
			if(stat->facility2>0)
			{
				addLocationAvailible(loc,stat->facility2,-1);
				setNeedSupply(getNeedSupply()-(*pStats)[stat->facility2].needSupply);
				setHaveSupply(getHaveSupply()-(*pStats)[stat->facility2].haveSupply);
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
				setNeedSupply(getNeedSupply()-(*pStats)[stat->facility2].needSupply);
				setHaveSupply(getHaveSupply()-(*pStats)[stat->facility2].haveSupply);	
				// <- nicht noetig :/ eigentlich schon... bei gas scv z.B. :/
			}
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST:
			if(stat->facility2>0)
			{
				addLocationAvailible(0/*location*/ ,stat->facility2,-1); // primarily for temporary R_researches, have to be location 0
// TODO				 setNeedSupply(getNeedSupply()+(*pStats)[stat->facility2].needSupply); // <- nicht noetig :/
			}
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
			if(stat->facility2>0)
				addLocationAvailible(0/*loc*/,stat->facility2,-1);
			// no needSupply gain as the item is recovered... well or not? mmmh... ~~~~
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			break;
		case NEEDED_ALWAYS:
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
				// needSupply?
			if(stat->facility2>0)
				addLocationAvailible(loc,stat->facility2,-1);
			break;
		case NO_FACILITY_BEHAVIOUR_DEFINED:
		default:
#ifdef _SCC_DEBUG
			  toLog("ERROR: UNDEFINED FACILITY BEHAVIOUR DETECTED!");
#endif
				   break;
	}
}

/* compares the current force on the map and the finnishing times of the unit types with our predefined goals */
// TODO optimize this
const bool PRERACE::calculateReady() const
{
	return((*pGoal)->calculateReady(location));
}



const int PRERACE::calculatePrimaryFitness(const bool ready) const
{
	int tpF=0;
//TODO evtl noch uebrige availible miteinbeziehen
//	int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; // temporary data to check whether a bonus is already given (only applies if total > goal)
//TODO: Nicht alle Einheiten am Ort? => Ort egal sein lassen aber zur Zeit hinzuzaehlen
	// Nicht alle Einheiten ueberhaupt gebaut UND nicht alle am Ort => nur viertel Bonus fuer Einheiten die nicht am Ort sind
	if(!ready)
	{
		list<GOAL>::const_iterator i;
		bool first=true;
		while((*pGoal)->getNextGoal(i, first))
		{
			first=false;
		//calculate number of fulfilled goals & their time & their distance to goal position
// Haben wir zuwenig Einheiten gebaut?  total < goal
				if(i->count>getLocationTotal(i->location,i->unit))
				{
					//total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
					//TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen
					//evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!
					int sumup=0;
// location 0? Dann ist es trivial
					if(i->location==0)
						sumup=getLocationTotal(0,i->unit)*100;
					else
					{
				// location != 0 ? Dann schaun wir mal, ob wir auf der Karte noch wo Units verstreut haben
				// mehr als goalcount koennen wir keinen Bonus vergeben
						int j=1;
						int bon=i->count;
						int loc=(*pMap)->getLocation(i->location)->getNearest(j);
						while((j<MAX_LOCATIONS)&&(bon>getLocationTotal(loc,i->unit)))
						{
							sumup+=getLocationTotal(loc,i->unit)*(100-(*pMap)->getLocation(loc)->getDistance(i->location)); //was pMap->location[j]...
							bon-=getLocationTotal(loc,i->unit);
							j++;
						}
						// Falls j<MAX_LOCATIONS => unser "Bon" wurde schon vorher aufgebraucht => An dieser Stelle j den Rest draufgeben... 
						if(j<MAX_LOCATIONS)
							sumup+=bon*(100-(*pMap)->getLocation(loc)->getDistance(i->location));
					}

//jetzt steht in sumup die gesammelten totals gewichtet mit den Entfernungen zum Ziel
										//TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
										// evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
										// bissl komplex da mans ja den einzelnen goals verteilen muss...
					if((i->time>0)&&(i->finalTime>i->time))
//					{
//						if(getFinalTime(i)>0) //??? TODO
//						if(getFinalTime(i)>i->time)
						tpF+=(i->time*sumup)/(i->count*i->finalTime);
//						else setpFitness(getpFitness()+sumup/i->count);
//TODO...~~~
//						else while(true); // <- kann eigentlich nicht auftreten!
//						  setpFitness(getpFitness()+(i->time*sumup)/(i->count*ga->maxTime));
//					  }
					else
						tpF+=sumup/i->count;
				} // END total < goal
				else
//if( /*((i->location==0)&&(i->count<=getLocationTotal(0,i->unit))) || ( (i->location>0)&&*/(i->count<=getLocationTotal(i->location,i->unit)))
								//total >= goal ?
				{
// Checken wann wir das Ziel erreicht haetten
					if((i->time>0)&&(i->finalTime>i->time))
// aha, wir haben die Zeit ueberschritten => trotzdem anteilig Bonus geben
						tpF+=(i->time*100/i->finalTime);
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
						if(getLocationTotal(i->location,i->unit)<i->count)
								bonus[i->location][i->unit]+=i->count-getLocationTotal(i->location,i->unit);
//bonus ist jetzt mit den uebrigen Einheiten bis zur Zielerfuellung belegt
				for(i=MAX_BUILDINGS;i--;)
						if((build->getRemainingBuildTime(i)>0)&&(bonus[build->getLocation(i)][build->getType(i)]>0))
						{
						//erstmal ohne Zeit...
								pFitness+=((build->getRemainingBuildTime(i)*100)/((getLocationTotal(build->getLocation(i),build->getType(i))+bonus[build->getLocation(i)][build->getType(i)])*(*pStats)[build->getType(i)].BT)); //TODO in ProgramBT aendern
								if(((*pGoal)->goal[build->getType(i)].time>0)&&(getLocationTotal(build->getLocation(i),build->getType(i))==0))
										pFitness+=(build->getRemainingBuildTime(i)*100*(*pGoal)->goal[build->getType(i)].time*getLocationTotal(0,i))/((*pGoal)->goal[build->getType(i)].count*(*pStats)[build->getType(i)].BT*ga->maxTime);//hier auch ProgramBT
								else
										pFitness+=((build->getRemainingBuildTime(i)*100)/((*pGoal)->goal[build->getType(i)].count*(*pStats)[build->getType(i)].BT));
								bonus[build->getLocation(i)][build->getType(i)]--;
						}*/
	} // end of ready=false
	else   // all goals fulfilled, fitness <- timer
	{
		tpF+=getTimer();
		tpF+=(*pGoal)->countGoals()*100;
//		for(list<GOAL>::const_iterator i = (*pGoal)->goal.begin();i!=(*pGoal)->goal.end();++i)
//				{
//						if(i->count>0)
//								tpF+=100;
/*					  if((i->unit!=GAS_SCV)&&(i->unit!=SCV)) //do not punish 'too much' workers!
								if(i->count<getLocationTotal(i->location,i->unit))
										setsFitness(getsFitness()+(-getLocationTotal(i->location,i->unit)+i->count)*(stats[(*pGoal)->getRace()][i->unit].minerals+stats[(*pGoal)->getRace()][i->unit].gas));*/
//				}
	}
	return(tpF);
}
// end of calculatePrimaryFitness




// ------------------------------
// ------ HARVEST ROUTINES ------
// ------------------------------

void EXPORT PRERACE::adjustMineralHarvest(const int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::adjustMineralHarvest): Value location out of range.");return;
	}
#endif
	//TODO Zerg hatchery,lair etc.
	
// WAYNE Location,	  kein Command Center 							und keine Mineralien
	if((location==0)||((!getLocationTotal(location, COMMAND_CENTER))&&(!getMapLocationTotal(0, location, MINERAL_PATCH))))
	{
		for(int i=45;i--;)
			setMineralHarvestPerSecond(location,i,0);
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
	else if(pStart->getBasicMineralHarvestSpeedPerSecond(getPlayerNum(), 1)) // SONST: Falls wir ne minimalSammelgeschwindigkeit haben eintragen
	{
//		int k;
		for(int i=45;i--;)
		{
//			k=0;
//			for(j=0;j<45;j++)
//				if(i*8<=j*pMap->location[num].getTotal(0][MINERAL_PATCH]) 
//				{ k=j;j=45;}
			setMineralHarvestPerSecond(location, i, pStart->getBasicMineralHarvestSpeedPerSecond(getPlayerNum(), i/*k*/));//*pMap->location[num].getTotal(0][MINERAL_PATCH])/8;
		}	//ab hier funzt alles... nur scheint startPlayer->getBasic... nicht richtig initialisiert zu sein...
	
	}
}

void EXPORT PRERACE::adjustGasHarvest(const int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::adjustGasHarvest): Value location out of range.");return;
	}
#endif
	if((location==0)||((!getLocationTotal(location,COMMAND_CENTER))&&(!getLocationTotal(location,REFINERY))))
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
	else if(pStart->getBasicMineralHarvestSpeedPerSecond(getPlayerNum(), 1))
	{
//		int k;
		for(int i=5;i--;)
		{
//			k=0;
//			for(j=0;j<5;j++)
//				if(i<=j*pMap->location[num].getTotal(playerNum][REFINERY]) { k=j;j=5;}
					setGasHarvestPerSecond(location, i, pStart->getBasicGasHarvestSpeedPerSecond(getPlayerNum(), i/*k*/)*getLocationTotal(location, REFINERY));

//					hier liegts problem wegen gas
//						gasharvestps ist zu gross, evtl wegen zu vieler refineries!
		}
	}
}

void EXPORT PRERACE::adjustHarvestAllLocations()
{
	for(int i=MAX_LOCATIONS;i--;) {
		adjustMineralHarvest(i);
		adjustGasHarvest(i);
	}
}

const int EXPORT PRERACE::harvestMinerals() const
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


const int EXPORT PRERACE::harvestGas() const
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

void PRERACE::assignGA(GA* pga)
{
#ifdef _SCC_DEBUG
	if(!pga) {
		toLog("DEBUG: (PRERACE::assignGA): Variable pga not initialized.");return;
	}
#endif
	ga=pga;
}

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
//	if(ga->noise>0)
//		for(int j=0;j<MAX_TIME;j++)
//			noise[j]=rand()%ga->noise-rand()%ga->noise;
//	else TODO
		for(int j=MAX_TIME;j--;)
			noise[j]=0;
}

void PRERACE::resetGeneMarker()
{
	markerCounter=1;
}

void EXPORT PRERACE::resetSpecial()
{
	for(int i=MAX_LOCATIONS;i--;)
		larvaInProduction[i]=0;
}

void PRERACE::removeLarvaFromQueue(const int location)
{
#ifdef _SCC_DEBUG
    if((location<1)||(location>=MAX_LOCATIONS)) {
	        toLog("DEBUG: (PRERACE::removeLarvaFromQueue): Value location out of range.");return;
    }
	if((larvaInProduction[location]<1)||(larvaInProduction[location]>=MAX_SUPPLY)) {
	        toLog("DEBUG: (PRERACE::removeLarvaFromQueue): Variable larvaInProduction not initialized or out of range.");return;
    }
#endif
	larvaInProduction[location]--;
}

void PRERACE::addLarvaToQueue(const int location)
{
#ifdef _SCC_DEBUG
    if((location<1)||(location>=MAX_LOCATIONS)) {
	        toLog("DEBUG: (PRERACE::addLarvaFromQueue): Value location out of range.");return;
    }
	if((larvaInProduction[location]<0)||(larvaInProduction[location]>=MAX_SUPPLY)) {
	        toLog("DEBUG: (PRERACE::addLarvaFromQueue): Variable larvaInProduction not initialized or out of range.");return;
    }
#endif
	larvaInProduction[location]++;
}

void EXPORT PRERACE::setPlayerNum(const int playerNum)
{
#ifdef _SCC_DEBUG
	if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
		toLog("DEBUG: (PRERACE::setPlayer): Value out of range.");return;
	}
#endif
	if(this->playerNum==playerNum)
		return;
	this->playerNum=playerNum;
	location=&(unit[playerNum][0]);

	pStartCondition=pStart->getStartcondition(playerNum);
	setCurrentGoal(pStart->getCurrentGoal(playerNum));
	setpStats(pStart->getpStats(playerNum));

//	global=&unit[playerNum][0];
}

void EXPORT PRERACE::initializePlayer()
{
	setMinerals((*pStartCondition)->getMinerals());
	setGas((*pStartCondition)->getGas());
	setTimer(ga->maxTime-(*pStartCondition)->getStartTime());
	setNeedSupply((*pStartCondition)->getNeedSupply());
	setHaveSupply((*pStartCondition)->getHaveSupply());
}

void EXPORT PRERACE::eraseIllegalCode()
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

void EXPORT PRERACE::eraseUselessCode()
{
	int allUnits[UNIT_TYPE_COUNT];
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

void EXPORT PRERACE::mutateGeneCode()
{
//	return;
// TODO logger machen, welche Mutationsart besonders erfolgreich ist
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
    bool buildable[GAS_SCV+1];
    int tMaxBuildTypes;
    int tGeno[GAS_SCV+1]; // !! keine anderen units drueber nehmen!
    for(int i=GAS_SCV+1;i--;)
    if((*pGoal)->getIsBuildable(i))
    {
        force[i]=((*pStartCondition)->getLocationTotal(GLOBAL,i)>0); //set start force
        if(force[i]) buildable[i]=true;
        tGeno[i]=0;
    }
    else
    {
        force[i]=0;
        buildable[i]=false;
        tGeno[i]=0;
    }
                                                                                                                                                            
    for(int x=MAX_LENGTH;x--;) //length
    {
        if(x<MAX_LENGTH-1) { // erstes nicht einbeziehen
            force[(*pGoal)->toPhaeno(Code[x+1])]=1;
        }
                                                                                                                                                            
        for(int i=GAS_SCV+1;i--;)
        {
            if(force[i]) buildable[i]=true;
            if(((*pStats)[i].prerequisite[0])&&(force[(*pStats)[i].prerequisite[0]])) buildable[i]=true;
            if(((*pStats)[i].prerequisite[1])&&(force[(*pStats)[i].prerequisite[1]])) buildable[i]=true;
            if(((*pStats)[i].prerequisite[2])&&(force[(*pStats)[i].prerequisite[2]])) buildable[i]=true;
            if(((*pStats)[i].facility[0])&&(force[(*pStats)[i].facility[0]])) buildable[i]=true;
            if(((*pStats)[i].facility[1])&&(force[(*pStats)[i].facility[1]])) buildable[i]=true;
            if(((*pStats)[i].facility[2])&&(force[(*pStats)[i].facility[2]])) buildable[i]=true;
        }
        tMaxBuildTypes=0;
        for(int i=GAS_SCV+1;i--;)
            if(buildable[i])
            {
                if(!(*pGoal)->getIsBuildable(i))
                    buildable[i]=false;
                else
                {
                    tGeno[tMaxBuildTypes]=(*pGoal)->toGeno(i);
                    tMaxBuildTypes++;
                }
            }
                                                                                                                                                            
//alle ueberpruefen, ob die ueberhaupt baubar sind... aber von hinten her!
                                                                                                                                                            
        if(ga->getMutationFactor()==0)
            return;
        if(rand()%(MAX_LENGTH*100/ga->getMutationFactor())==0)
        {
            switch(rand()%4)
            {
                //TODO: wenn generateBuildOrder==1 dann bleibts stehen!
                case 0://delete one variabel entry and move - Mehrere Schmieden/Kasernen etc. zulassen!
                {
//                  if((((*pGoal)->isVariable[Code[0][x]]==1)&&((*pGoal)->isVariable[Code[1][x]]==1))||(!ga->preprocessBuildOrder)) TODO
                        //TODO: ueberlegen, ob Code evtl struct sein sollte... mmmh
                        for(int i=x;i<MAX_LENGTH-1;i++)
                        {
                            Code[i]=Code[i+1];
                            Marker[i]=Marker[i+1];
                        }
                    // TODO hier auch das buildable und tMaxBuildTypes rein... irgendwie den Code als "mutier mich" markieren und spaetereinfuegen
                    markerCounter++;Marker[MAX_LENGTH-1]=markerCounter;
                    int y;
//                  if(ga->preprocessBuildOrder) // TODO
//                  while((*pGoal)->isVariable[y]==0) y=rand()%(*pGoal)->getMaxBuildTypes();
//                  else
                    y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
                    Code[MAX_LENGTH-1]=y;
//                  if(ga->preprocessBuildOrder) // TODO
//                  while((*pGoal)->isVariable[y]==0) y=rand()%(*pGoal)->getMaxBuildTypes();
//                  else
//                  y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
//                  Code[1][MAX_LENGTH-1]=y;
                }break;
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
//                  if(ga->preprocessBuildOrder) TODO
//                      while((*pGoal)->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
//                  else
                    y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
                    Code[x]=y;
//                  if(ga->preprocessBuildOrder)
//                      while((*pGoal)->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%(*pGoal)->getMaxBuildTypes();
//                  else
                }break;
                case 2://change one entry
                {
//                  if((*pGoal)->isVariable[Code[k][x]]==1) TODO
                    {
                        int y;
                        //int y=rand()%(*pGoal)->getMaxBuildTypes();//Optimieren
//                      if(ga->preprocessBuildOrder) TODO
//                          while((*pGoal)->isVariable[y]==0) y=tGeno[rand()%tMaxBuildTypes];//rand()%(*pGoal)->getMaxBuildTypes();
//                      else
                        y=tGeno[rand()%tMaxBuildTypes];//(*pGoal)->getMaxBuildTypes();
                        Code[x]=y;
                        markerCounter++;Marker[x]=markerCounter;
                    }
                }break;
                case 3://exchange two entries
                    {
                        //hier erst recht
                        int y=rand()%MAX_LENGTH/2+rand()%MAX_LENGTH/2-x;
//                      y=rand()%MAX_LENGTH; //TODO: Aendern in bevorzugtes Ziel => Naehe
//                      if(abs(x-y)>(MAX_LENGTH/2)) y=rand()%MAX_LENGTH;
                        if((y>0)&&(x!=y)&&
                           (buildable[(*pGoal)->toPhaeno(Code[y])]))
                        {
                            int l;
                            l=Code[x];Code[x]=Code[y];Code[y]=l;
                            l=Marker[x];Marker[x]=Marker[y];Marker[y]=l;
                        }
                    }break;
//              case 5://move a block of orders  [a..b..ta..tb..c..d] -> [a..ta..tb..b..c..d]
                    //~~~TODO bug, marker und code wird nicht richtig verschoben
/*                  if(getLength()>2)
                    {
                        int i,ta,tb,ttt,y;
                        ta=rand()%(getLength()-2)+1; //>0   <getLength()-2
                        tb=rand()%(getLength()-1)+2; //>1   <getLength()-1
                        while(ta==tb) tb=rand()%(getLength()-1)+2;
                        if(ta>tb) {ttt=tb;tb=ta;ta=ttt;} //~~
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
                    }*///break;
//              case 6://flip entries
//                  {
                        // neutral :o
//                      int l;
//                      l=Code[0][x];Code[0][x]=Code[1][x];Code[1][x]=l;
//                      l=Marker[0][x];Marker[0][x]=Marker[1][x];Marker[1][x]=l;
//                  }break;
            }
        }
    }
}




//Reinitialize programs with random orders
void PRERACE::resetGeneCode()
{
//	mutationRate=500+rand()%1000;
/*	if((ga->preprocessBuildOrder)&&(basicLength>0))
	{
		memcpy(Code,basicBuildOrder,MAX_LENGTH*4);
		for(int i=MAX_LENGTH;i--;)
		{
			markerCounter++;Marker[i]=markerCounter;
		}
	}
	else*/
	{
		int y=0;
		switch((*pGoal)->getRace())
		{
			case TERRA:y=SUPPLY_DEPOT;break;
			case PROTOSS:y=PYLON;break;
			case ZERG:y=OVERLORD;break;
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



// -------------------------------------------------------------------
// ------ BELOW ALL THE GET/SET FUNCTIONS, PRETTY UNINTERESTING ------
// -------------------------------------------------------------------

const int PRERACE::getCode(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
        toLog("DEBUG: (PRERACE::getCode): Value IP out of range.");return(0);
	}	
	if((Code[IP]<0)||(Code[IP]>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (PRERACE::getCode): Variable Code not initialized.");return(0);
	}
#endif
	return(Code[IP]);
}

const int PRERACE::getCurrentCode() const
{
#ifdef _SCC_DEBUG
    if((Code[getIP()]<0)||(Code[getIP()]>=UNIT_TYPE_COUNT)) {
        toLog("DEBUG: (PRERACE::getCurrentCode): Variable Code not initialized.");return(0);
    }
#endif
	return(Code[getIP()]);
}

const int PRERACE::getMarker(const int IP) const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::getMarker): Value IP out of range.");return(0);
	}
#endif
	return(Marker[IP]); 
}
void PRERACE::copyCode(PRERACE& player)
{
	for(int i=MAX_LENGTH;i--;)
	{
		Code[i]=player.Code[i];
		Marker[i]=player.Marker[i];
	}
}

// ------ UNITS ------
const int EXPORT PRERACE::getMapLocationAvailible(const int player, const int location, const int unittype)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::getMapLocationAvailible): Value player out of range.");return(0);
	}
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMapLocationAvailible): Value location out of range.");return(0);
	}
#endif
	return(unit[player][location].getAvailible(unittype));
}

const int EXPORT PRERACE::getMapLocationTotal(const int player, const int location, const int unittype)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::getMapLocationTotal): Value player out of range.");return(0);
	}
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMapLocationTotal): Value location out of range.");return(0);
	}
#endif
	return(unit[player][location].getTotal(unittype));
}

void EXPORT PRERACE::setMapLocationAvailible(const int player, const int location, const int unittype, const int availible)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value location out of range.");return;
	}
#endif
	unit[player][location].setAvailible(unittype, availible);
}

void EXPORT PRERACE::setMapLocationTotal(const int player, const int location, const int unittype, const int total)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value location out of range.");return;
	}
#endif
	unit[player][location].setTotal(unittype, total);
}

void EXPORT PRERACE::addMapLocationAvailible(const int player, const int location, const int unittype, const int availible)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::addMapLocationAvailible): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addMapLocationAvailible): Value location out of range.");return;
	}
#endif
	unit[player][location].addAvailible(unittype, availible);
}


void EXPORT PRERACE::addMapLocationTotal(const int player, const int location, const int unittype, const int total)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::addMapLocationTotal): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addMapLocationTotal): Value location out of range.");return;
	}
#endif
	unit[player][location].addTotal(unittype, total);
}

const int EXPORT PRERACE::getLocationAvailible(const int location, const int unittype) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getLocationAvailible): Value location out of range.");return(0);
	}
#endif
	return(this->location[location].getAvailible(unittype));
}

const int EXPORT PRERACE::getLocationTotal(const int location, const int unittype) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::getLocationTotal): Value location out of range.");return(0);
	}
#endif
	return(this->location[location].getTotal(unittype));
}


void EXPORT PRERACE::setLocationAvailible(const int location, const int unittype, const int availible)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setLocationAvailible): Value location out of range.");return;
	}
#endif
	this->location[location].setAvailible(unittype, availible);
}

void EXPORT PRERACE::setLocationTotal(const int location, const int unittype, const int total)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setLocationTotal): Value location out of range.");return;
	}
#endif
	this->location[location].setTotal(unittype, total);
}

void EXPORT PRERACE::addLocationAvailible(const int location, const int unittype, const int availible)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addLocationAvailible): Value location out of range.");return;
	}
#endif	
	this->location[location].addAvailible(unittype, availible);
	if(location>0) //sonst waers ja doppelt...
		this->location[0].addAvailible(unittype, availible);
}

void EXPORT PRERACE::addLocationTotal(const int location, const int unittype, const int total)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::addLocationTotal): Value location out of range.");return;
	}
#endif
	this->location[location].addTotal( unittype, total );
	if(location>GLOBAL) // sonst waers ja doppelt wenn location = 0
		this->location[GLOBAL].addTotal(unittype, total);
}

void PRERACE::addOneLocationAvailible(const int location, const int unittype)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS)) {
        toLog("DEBUG: (PRERACE::addLocationAvailible): Value location out of range.");return;
    }
#endif
    this->location[location].addOneAvailible( unittype );
// also add one unit to the global location if global location was not already specified
    if(location>GLOBAL) 
        this->location[GLOBAL].addOneAvailible( unittype );
}

void PRERACE::addOneLocationTotal(const int location, const int unittype)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS)) {
        toLog("DEBUG: (PRERACE::addLocationTotal): Value location out of range.");return;
    }
#endif
    this->location[location].addOneTotal( unittype );
    if(location>GLOBAL) // sonst waers ja doppelt wenn location = 0
        this->location[GLOBAL].addOneTotal( unittype );
}

void PRERACE::removeOneLocationAvailible(const int location, const int unittype)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS)) {
        toLog("DEBUG: (PRERACE::removeOneLocationAvailible): Value location out of range.");return;
    }
#endif
    this->location[location].removeOneAvailible( unittype );
// also add one unit to the global location if global location was not already specified
    if(location>GLOBAL)
        this->location[GLOBAL].removeOneAvailible( unittype );
}

void PRERACE::removeOneLocationTotal(const int location, const int unittype)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS)) {
        toLog("DEBUG: (PRERACE::removeOneLocationTotal): Value location out of range.");return;
    }
#endif
    this->location[location].removeOneTotal( unittype );
    if(location>0) 
        this->location[0].removeOneTotal( unittype );
}
// ------ END UNITS -----


// ----- SUPPLY ------
void EXPORT PRERACE::setNeedSupply(const int needSupply)
{
#ifdef _SCC_DEBUG
	if((needSupply<0)||(needSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (PRERACE::setNeedSupply): Value needSupply out of range.");return;
	}
#endif
	this->needSupply=needSupply;
}

const int EXPORT PRERACE::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if((needSupply<0)||(needSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (PRERACE::getNeedSupply): Variable not initialized.");return(0);
	}
#endif
	return(needSupply);
}

void EXPORT PRERACE::setHaveSupply(const int haveSupply)
{
#ifdef _SCC_DEBUG
	if((haveSupply<0)||(haveSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (PRERACE::setHaveSupply): Value haveSupply out of range.");return;
	}
#endif
	this->haveSupply=haveSupply;
}

const int EXPORT PRERACE::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if((haveSupply<0)||(haveSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (PRERACE::getHaveSupply): Variable not initialized.");return(0);
	}
#endif
	return(haveSupply);
}
// ----- END SUPPLY ------

// ----- HARVEST ------

void EXPORT PRERACE::setMinerals(const int minerals)
{
#ifdef _SCC_DEBUG
	if((minerals<0)||(minerals>MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::setMinerals): Value minerals out of range.");return;
	}
#endif
	this->minerals=minerals;
}

const int EXPORT PRERACE::getMinerals() const
{
#ifdef _SCC_DEBUG
	if((minerals<0)||(minerals>MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::getMinerals): Variable minerals not initialized.");return(0);
	}
#endif
	return(minerals);
}

void EXPORT PRERACE::setGas(const int gas)
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS)) {
		toLog("DEBUG: (PRERACE::setGas): Value gas out of range.");return;
	}
#endif
	this->gas=gas;
}

const int EXPORT PRERACE::getGas() const
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS)) {
		toLog("DEBUG: (PRERACE::getGas): Variable gas not initialized.");return(0);
	}
#endif
	return(gas);
}

void EXPORT PRERACE::setMineralHarvestPerSecond( const int location, const int worker, const int mineralHarvestPerSecond )
{
#ifdef _SCC_DEBUG
	if((mineralHarvestPerSecond<0)||(mineralHarvestPerSecond>=MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value mineralHarvestPerSecond out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value location out of range.");return;
	}
	if((worker<0)||(worker>=45)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value worker out of range.");return;
	}
#endif
	this->mineralHarvestPerSecond[location][worker]=mineralHarvestPerSecond;
}

const int EXPORT PRERACE::getMineralHarvestPerSecond( const int location, const int worker ) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Value location out of range.");return(0);
	}
	if((worker<0)||(worker>=45)) {
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Value worker out of range.");return(0);
	}
	if((mineralHarvestPerSecond[location][worker]<0)||(mineralHarvestPerSecond[location][worker]>MAX_MINERALS))	{
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Variable mineralHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(mineralHarvestPerSecond[location][worker]);
}

void EXPORT PRERACE::setGasHarvestPerSecond( const int location, const int worker, const int gasHarvestPerSecond )
{
#ifdef _SCC_DEBUG
	if((gasHarvestPerSecond<0)||(gasHarvestPerSecond>=MAX_GAS)||(location<0)||(location>=MAX_LOCATIONS)||(worker<0)||(worker>=5)) {
		toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value out of range.");return;
	}
#endif
	this->gasHarvestPerSecond[location][worker]=gasHarvestPerSecond;
}

const int EXPORT PRERACE::getGasHarvestPerSecond( const int location, const int worker ) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Value location out of range.");return(0);
	}
	if((worker<0)||(worker>=5))	{
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Value worker out of range.");return(0);
	}
	if((gasHarvestPerSecond[location][worker]<0)||(gasHarvestPerSecond[location][worker]>MAX_GAS)) {
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Variable gasHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(gasHarvestPerSecond[location][worker]);
}

void EXPORT PRERACE::setHarvestedMinerals( const int harvestedMinerals )
{
#ifdef _SCC_DEBUG
	if((harvestedMinerals<0)||(harvestedMinerals>=MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::setHarvestedMinerals): Value out of range.");return;
	}
#endif
	this->harvestedMinerals=harvestedMinerals;
}

const int EXPORT PRERACE::getHarvestedMinerals() const
{
#ifdef _SCC_DEBUG
	if((harvestedMinerals<0)||(harvestedMinerals>MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::getHarvestedMinerals): Variable harvestedMinerals not initialized.");return(0);
	}
#endif
	return(harvestedMinerals);
}

void EXPORT PRERACE::setHarvestedGas( const int harvestedGas )
{
#ifdef _SCC_DEBUG
	if((harvestedGas<0)||(harvestedGas>=MAX_GAS)) {
		toLog("DEBUG: (PRERACE::setHarvestedGas): Value out of range.");return;
	}
#endif
	this->harvestedGas=harvestedGas;
}

const int EXPORT PRERACE::getHarvestedGas() const
{
#ifdef _SCC_DEBUG
	if((harvestedGas<0)||(harvestedGas>MAX_GAS)) {
		toLog("DEBUG: (PRERACE::getHarvestedGas): Variable harvestedGas not initialized.");return(0);
	}
#endif
	return(harvestedGas);
}


void EXPORT PRERACE::setWastedMinerals( const int wastedMinerals )
{
#ifdef _SCC_DEBUG
	if((wastedMinerals<0)||(wastedMinerals>=MAX_MINERALS*MAX_TIME)) {
		toLog("DEBUG: (PRERACE::setWastedMinerals): Value out of range.");return;
	}
#endif
	this->wastedMinerals=wastedMinerals;
}

const int EXPORT PRERACE::getWastedMinerals() const
{
#ifdef _SCC_DEBUG
	if((wastedMinerals<0)||(wastedMinerals>MAX_MINERALS*MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getWastedMinerals): Variable wastedMinerals not initialized.");return(0);
	}
#endif
	return(wastedMinerals);
}

void EXPORT PRERACE::setWastedGas(const int wastedGas)
{
#ifdef _SCC_DEBUG
	if((wastedGas<0)||(wastedGas>=MAX_GAS*MAX_TIME)) {
		toLog("DEBUG: (PRERACE::setWastedGas): Value out of range.");return;
	}
#endif
	this->wastedGas=wastedGas;
}

const int EXPORT PRERACE::getWastedGas() const
{
#ifdef _SCC_DEBUG
	if((wastedGas<0)||(wastedGas>MAX_GAS*MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getWastedGas): Variable wastedGas not initialized.");return(0);
	}
#endif
	return(wastedGas);
}
// ----- END HARVEST -----

void PRERACE::setCurrentGoal(GOAL_ENTRY** pGoal)
{
#ifdef _SCC_DEBUG
	if(!pGoal) {
		toLog("DEBUG: (PRERACE::setCurrentGoal): Variable pGoal not initialized.");return;
	}
#endif
	this->pGoal=pGoal;
}

const int EXPORT PRERACE::getPlayerNum() const
{
#ifdef _SCC_DEBUG
	if((playerNum<0)||(playerNum>=MAX_PLAYER)) {
		toLog("DEBUG: (PRERACE::getPlayer): Variable not initialized.");return(0);
	}
#endif
	return(playerNum);
}

const START_CONDITION* const* PRERACE::getStartCondition()
{
	return(pStartCondition);
}


void EXPORT PRERACE::setTimer(const int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (PRERACE::setTimer): Value time out of range.");return;
	}
#endif
	timer=time;
}

const int EXPORT PRERACE::getTimer() const
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(timer);
}


const int EXPORT PRERACE::getRealTimer() const
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getRealTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(ga->maxTime-timer);
}

void EXPORT PRERACE::setpStats(const UNIT_STATISTICS* const* pStats)
{
#ifdef _SCC_DEBUG
	if((*pStats)[0].minerals!=0) { // TODO
		toLog("DEBUG: (PRERACE::setpStats): Variable not initialized.");return;
	}
#endif
	this->pStats=pStats;
}

const UNIT_STATISTICS* const * EXPORT PRERACE::getpStats() const
{
#ifdef _SCC_DEBUG
	if(!pStats)	{
		toLog("DEBUG: (PRERACE::getpStats): Variable not initialized.");return(0);
	}
#endif
	return(pStats);
}

void EXPORT PRERACE::setIP(const int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::setIP): Value IP out of range.");return;
	}
#endif
	this->IP=IP;
}

const int EXPORT PRERACE::getIP() const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::getIP): Variable IP not initialized.");return(0);
	}
#endif
	return(IP);
}

/*const int EXPORT PRERACE::getFinalTime(const int goal) const
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=MAX_GOALS))	{
		toLog("DEBUG: (PRERACE::getFinalTime): Value goal out of range.");return(0);
	}
	if((ftime[goal]<0)||(ftime[goal]>MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getFinalTime): Variable ftime not initialized.");return(0);
	}
#endif
	return(ftime[goal]);
}*/

const int EXPORT PRERACE::getLength() const
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::getLength): Variable length not initialized.");return(0);
	}
#endif
	return(length);
}

/*void EXPORT PRERACE::setFinalTime(const int goal, const int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (PRERACE::setFinalTime): Value time out of range.");return;
	}
	if((goal<0)||(goal>=MAX_GOALS)) {
		toLog("DEBUG: (PRERACE::setFinalTime): Value goal out of range.");return;
	}
#endif
	ftime[goal]=time;
}*/

void EXPORT PRERACE::setLength(const int length)
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH))
	{
		toLog("DEBUG: (PRERACE::setLength): Value length out of range.");return;
	}
#endif
	this->length=length;
}


void EXPORT PRERACE::setTimeOut(const int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIMEOUT)) {
		toLog("DEBUG: (PRERACE::setTimeOut): Value time out of range.");return;
	}
#endif
	timeout=time;
}

const int EXPORT PRERACE::getTimeOut() const
{
#ifdef _SCC_DEBUG
	if((timeout<0)||(timeout>MAX_TIMEOUT)) {
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

START* PRERACE::pStart;
const BASIC_MAP* const* PRERACE::pMap;

int PRERACE::noise[MAX_TIME];
int PRERACE::markerCounter;
UNIT PRERACE::unit[MAX_PLAYER][MAX_LOCATIONS];

void PRERACE::assignGA(GA* pga);
void PRERACE::assignStart(START* start);
void PRERACE::initNoise();
void PRERACE::copyMap();

const int PRERACE::getMapLocationAvailible(const int player, const int loc, const int unit);
const int PRERACE::getMapLocationTotal(const int player, const int loc, const int unit);

void PRERACE::setMapLocationAvailible(const int player, const int loc, const int unit, const int availible);
void PRERACE::setMapLocationTotal(const int player, const int loc, const int unit, const int total);

void PRERACE::addMapLocationAvailible(const int player, const int loc, const int unit, const int availible);
void PRERACE::addMapLocationTotal(const int player, const int loc, const int unit, const int total);
void PRERACE::resetGeneMarker();
GA* PRERACE::ga;
BASIC_MAP* const* getMap();

