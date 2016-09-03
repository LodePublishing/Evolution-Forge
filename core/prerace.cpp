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
	setLength(0);
	for(int i=MAX_LOCATIONS;i--;)
	{
		for(int j=45;j--;)
			setMineralHarvestPerSecond(i,j,0);
		for(int j=5;j--;)
			setGasHarvestPerSecond(i,j,0);
	};
	for(int i=MAX_LENGTH;i--;)
	{
		Code[i]=0;
		last[i].unit=0;
		last[i].location=0;
		last[i].count=0;
	};
	resetSpecial();
};

PRERACE::~PRERACE()
{
};

// -----------------------------------
// ------ CONTROLS FROM OUTSIDE ------
// -----------------------------------

void PRERACE::replaceCode(const int IP, const int num)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::replaceCode): Value IP out of range.");return;
	}
	if((num<0)||(num>=(*pGoal)->getMaxBuildTypes())) {
		toLog("DEBUG: (PRERACE::replaceCode): Value num out of range.");return;
	}
#endif
	Code[IP]=num;
	markerCounter++;Marker[IP]=markerCounter;
};

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

    if(!buildingList.isEmpty()) {
        int buildingRemaining = buildingList.GetHead()->GetData()->getRemainingBuildTime();
        if(idle_time > buildingRemaining) 
			idle_time = buildingRemaining;
    }

    if( harvest_gas && 
	  ( harvest_minerals || 
      	(neededGas + harvest_gas - neededGas % harvest_gas)/harvestGas() < (neededMinerals+(harvest_minerals - neededMinerals % harvest_minerals))/harvest_minerals)) {
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
	return(idle_time);
	
};


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
const bool PRERACE::calculateReady() const
{
	bool ready=true;
	for(int i=MAX_GOALS;(i--)&&(ready);)
		if((*pGoal)->goal[i].count)
			ready&=(((*pGoal)->goal[i].count<=getLocationTotal((*pGoal)->goal[i].location, (*pGoal)->goal[i].unit))&&(((*pGoal)->goal[i].time>=getFinalTime(i))||((*pGoal)->goal[i].time==0)));
	return(ready);
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
		//calculate number of fulfilled goals & their time & their distance to goal position
		for(int i=MAX_GOALS;i--;)
			if((*pGoal)->goal[i].count>0)
			{
// Haben wir zuwenig Einheiten gebaut?  total < goal
if( /*(((*pGoal)->goal[i].location==0)&&((*pGoal)->goal[i].count>getLocationTotal(0,(*pGoal)->goal[i].unit))) || unnuetz*/
  (  /*((*pGoal)->goal[i].location>0)&&*/((*pGoal)->goal[i].count>getLocationTotal((*pGoal)->goal[i].location,(*pGoal)->goal[i].unit))) )
				{
					//total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
					//TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen
					//evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!
					int sumup=0;
					int bon=0;
// location 0? Dann ist es trivial
					if((*pGoal)->goal[i].location==0)
						sumup=getLocationTotal(0,(*pGoal)->goal[i].unit)*100;
					else
					{
				// location != 0 ? Dann schaun wir mal, ob wir auf der Karte noch wo Units verstreut haben
				// mehr als goalcount koennen wir keinen Bonus vergeben
						bon=(*pGoal)->goal[i].count;
						int j=1;
						int loc=(*pMap)->getLocation((*pGoal)->goal[i].location)->getNearest(j);
						while((j<MAX_LOCATIONS)&&(bon>getLocationTotal(loc,(*pGoal)->goal[i].unit)))
						{
							sumup+=getLocationTotal(loc,(*pGoal)->goal[i].unit)*(100-(*pMap)->getLocation(loc)->getDistance((*pGoal)->goal[i].location)); //was pMap->location[j]...
							bon-=getLocationTotal(loc,(*pGoal)->goal[i].unit);
							j++;
						}
						// Falls j<MAX_LOCATIONS => unser "Bon" wurde schon vorher aufgebraucht => An dieser Stelle j den Rest draufgeben... 
						if(j<MAX_LOCATIONS)
							sumup+=bon*(100-(*pMap)->getLocation(loc)->getDistance((*pGoal)->goal[i].location));
					}

//jetzt steht in sumup die gesammelten totals gewichtet mit den Entfernungen zum Ziel
										//TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
										// evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
										// bissl komplex da mans ja den einzelnen goals verteilen muss...
										if(((*pGoal)->goal[i].time>0)&&(getFinalTime(i)>(*pGoal)->goal[i].time))
//									  {
//											  if(getFinalTime(i)>0) //??? TODO
//											  if(getFinalTime(i)>(*pGoal)->goal[i].time)
														tpF+=((*pGoal)->goal[i].time*sumup)/((*pGoal)->goal[i].count*getFinalTime(i));
//											  else setpFitness(getpFitness()+sumup/(*pGoal)->goal[i].count);
//TODO...~~~
//											  else while(true); // <- kann eigentlich nicht auftreten!
//													  setpFitness(getpFitness()+((*pGoal)->goal[i].time*sumup)/((*pGoal)->goal[i].count*ga->maxTime));
//									  }
										else
												tpF+=sumup/(*pGoal)->goal[i].count;
								} // END total < goal
								else
//if( /*(((*pGoal)->goal[i].location==0)&&((*pGoal)->goal[i].count<=getLocationTotal(0,(*pGoal)->goal[i].unit))) || ( ((*pGoal)->goal[i].location>0)&&*/((*pGoal)->goal[i].count<=getLocationTotal((*pGoal)->goal[i].location,(*pGoal)->goal[i].unit)))
								//total >= goal ?
								{
// Checken wann wir das Ziel erreicht haetten
										if(((*pGoal)->goal[i].time>0)&&(getFinalTime(i)>(*pGoal)->goal[i].time))
// aha, wir haben die Zeit ueberschritten => trotzdem anteilig Bonus geben
												tpF+=((*pGoal)->goal[i].time*100/getFinalTime(i));
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
						if(getLocationTotal((*pGoal)->goal[i].location,(*pGoal)->goal[i].unit)<(*pGoal)->goal[i].count)
								bonus[(*pGoal)->goal[i].location][(*pGoal)->goal[i].unit]+=(*pGoal)->goal[i].count-getLocationTotal((*pGoal)->goal[i].location,(*pGoal)->goal[i].unit);
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
				for(int i=MAX_GOALS;i--;)
				{
						if((*pGoal)->goal[i].count>0)
								tpF+=100;
/*					  if(((*pGoal)->goal[i].unit!=GAS_SCV)&&((*pGoal)->goal[i].unit!=SCV)) //do not punish 'too much' workers!
								if((*pGoal)->goal[i].count<getLocationTotal((*pGoal)->goal[i].location,(*pGoal)->goal[i].unit))
										setsFitness(getsFitness()+(-getLocationTotal((*pGoal)->goal[i].location,(*pGoal)->goal[i].unit)+(*pGoal)->goal[i].count)*(stats[(*pGoal)->getRace()][(*pGoal)->goal[i].unit].minerals+stats[(*pGoal)->getRace()][(*pGoal)->goal[i].unit].gas));*/
				}
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
//				{ k=j;j=45;};
			setMineralHarvestPerSecond(location, i, pStart->getBasicMineralHarvestSpeedPerSecond(getPlayerNum(), i/*k*/));//*pMap->location[num].getTotal(0][MINERAL_PATCH])/8;
		}	//ab hier funzt alles... nur scheint startPlayer->getBasic... nicht richtig initialisiert zu sein...
	
	}
};

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
};

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
};

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
};
/* copies the precalculated startforce from pStart into our map (unit[][]
 * this is done 1:1, i.e. with memcpy */
void PRERACE::copyMap()
{
	pStart->copyStartForce(&(unit[0][0]));
};


void PRERACE::initNoise()
{
//	if(ga->noise>0)
//		for(int j=0;j<MAX_TIME;j++)
//			noise[j]=rand()%ga->noise-rand()%ga->noise;
//	else TODO
		for(int j=MAX_TIME;j--;)
			noise[j]=0;
};

void PRERACE::resetGeneMarker()
{
	markerCounter=1;
};

void EXPORT PRERACE::resetSpecial()
{
	for(int i=MAX_LOCATIONS;i--;)
		larvaInProduction[i]=0;
}

void EXPORT PRERACE::setPlayerNum(const int playerNum)
{
#ifdef _SCC_DEBUG
	if((playerNum<0)||(playerNum>=MAX_PLAYER)) {
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
};

void EXPORT PRERACE::initializePlayer()
{
	setMinerals((*pStartCondition)->getMinerals());
	setGas((*pStartCondition)->getGas());
	setTimer(ga->maxTime-(*pStartCondition)->getStartTime());
	setNeedSupply((*pStartCondition)->getNeedSupply());
	setHaveSupply((*pStartCondition)->getHaveSupply());
};


// -------------------------------------------------------------------
// ------ BELOW ALL THE GET/SET FUNCTIONS, PRETTY UNINTERESTING ------
// -------------------------------------------------------------------


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
};

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
};

void EXPORT PRERACE::setMapLocationAvailible(const int player, const int location, const int unittype, const int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value location out of range.");return;
	}
#endif
	unit[player][location].setAvailible(unittype, num);
};

void EXPORT PRERACE::setMapLocationTotal(const int player, const int location, const int unittype, const int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value location out of range.");return;
	}
#endif
	unit[player][location].setTotal(unittype, num);
};

void EXPORT PRERACE::addMapLocationAvailible(const int player, const int location, const int unittype, const int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::addMapLocationAvailible): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addMapLocationAvailible): Value location out of range.");return;
	}
#endif
	unit[player][location].addAvailible(unittype, num);
};


void EXPORT PRERACE::addMapLocationTotal(const int player, const int location, const int unittype, const int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=(*pMap)->getMaxPlayer())) {
		toLog("DEBUG: (PRERACE::addMapLocationTotal): Value player out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addMapLocationTotal): Value location out of range.");return;
	}
#endif
	unit[player][location].addTotal(unittype, num);
};

const int EXPORT PRERACE::getLocationAvailible(const int location, const int unittype) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getLocationAvailible): Value location out of range.");return(0);
	}
#endif
	return(this->location[location].getAvailible(unittype));
};

const int EXPORT PRERACE::getLocationTotal(const int location, const int unittype) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::getLocationTotal): Value location out of range.");return(0);
	}
#endif
	return(this->location[location].getTotal(unittype));
};


void EXPORT PRERACE::setLocationAvailible(const int location, const int unittype, const int num)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setLocationAvailible): Value location out of range.");return;
	}
#endif
	this->location[location].setAvailible(unittype, num);
};

void EXPORT PRERACE::setLocationTotal(const int location, const int unittype, const int num)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setLocationTotal): Value location out of range.");return;
	}
#endif
	this->location[location].setTotal(unittype, num);
};

void EXPORT PRERACE::addLocationAvailible(const int location, const int unit, const int num)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addLocationAvailible): Value location out of range.");return;
	}
#endif	
	this->location[location].addAvailible(unit, num);
	if(location>0) //sonst waers ja doppelt...
		this->location[0].addAvailible(unit, num);
};

void EXPORT PRERACE::addLocationTotal(const int location, const int unittype, const int num)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::addLocationTotal): Value location out of range.");return;
	}
#endif
	this->location[location].addTotal( unittype, num );
	if(location>0) // sonst waers ja doppelt wenn location = 0
		this->location[0].addTotal(unittype, num);
};

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
};

const int EXPORT PRERACE::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if((needSupply<0)||(needSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (PRERACE::getNeedSupply): Variable not initialized.");return(0);
	}
#endif
	return(needSupply);
};

void EXPORT PRERACE::setHaveSupply(const int haveSupply)
{
#ifdef _SCC_DEBUG
	if((haveSupply<0)||(haveSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (PRERACE::setHaveSupply): Value haveSupply out of range.");return;
	}
#endif
	this->haveSupply=haveSupply;
};

const int EXPORT PRERACE::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if((haveSupply<0)||(haveSupply>10*MAX_SUPPLY)) {
		toLog("DEBUG: (PRERACE::getHaveSupply): Variable not initialized.");return(0);
	}
#endif
	return(haveSupply);
};
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
};

const int EXPORT PRERACE::getMinerals() const
{
#ifdef _SCC_DEBUG
	if((minerals<0)||(minerals>MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::getMinerals): Variable minerals not initialized.");return(0);
	}
#endif
	return(minerals);
};

void EXPORT PRERACE::setGas(const int gas)
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS)) {
		toLog("DEBUG: (PRERACE::setGas): Value gas out of range.");return;
	}
#endif
	this->gas=gas;
};

const int EXPORT PRERACE::getGas() const
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS)) {
		toLog("DEBUG: (PRERACE::getGas): Variable gas not initialized.");return(0);
	}
#endif
	return(gas);
};

void EXPORT PRERACE::setMineralHarvestPerSecond( const int location, const int worker, const int minerals )
{
#ifdef _SCC_DEBUG
	if((minerals<0)||(minerals>=MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value minerals out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value location out of range.");return;
	}
	if((worker<0)||(worker>=45)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value worker out of range.");return;
	}
#endif
	mineralHarvestPerSecond[location][worker]=minerals;
};

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

void EXPORT PRERACE::setGasHarvestPerSecond( const int location, const int worker, const int num )
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_GAS)||(location<0)||(location>=MAX_LOCATIONS)||(worker<0)||(worker>=5)) {
		toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value out of range.");return;
	}
#endif
	gasHarvestPerSecond[location][worker]=num;
};

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

void EXPORT PRERACE::setHarvestedMinerals( const int num )
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::setHarvestedMinerals): Valueout of range.");return;
	}
#endif
	harvestedMinerals=num;
};

const int EXPORT PRERACE::getHarvestedMinerals() const
{
#ifdef _SCC_DEBUG
	if((harvestedMinerals<0)||(harvestedMinerals>MAX_MINERALS)) {
		toLog("DEBUG: (PRERACE::getHarvestedMinerals): Variable harvestedMinerals not initialized.");return(0);
	}
#endif
	return(harvestedMinerals);
};

void EXPORT PRERACE::setHarvestedGas(const int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_GAS)) {
		toLog("DEBUG: (PRERACE::setHarvestedGas): Value out of range.");return;
	}
#endif
	harvestedGas=num;
};

const int EXPORT PRERACE::getHarvestedGas() const
{
#ifdef _SCC_DEBUG
	if((harvestedGas<0)||(harvestedGas>MAX_GAS)) {
		toLog("DEBUG: (PRERACE::getHarvestedGas): Variable harvestedGas not initialized.");return(0);
	}
#endif
	return(harvestedGas);
};

// ----- END HARVEST -----

void PRERACE::setCurrentGoal(GOAL_ENTRY** pGoal)
{
#ifdef _SCC_DEBUG
	if(!pGoal) {
		toLog("DEBUG: (PRERACE::setCurrentGoal): Variable pGoal not initialized.");return;
	}
#endif
	this->pGoal=pGoal;
};

const int EXPORT PRERACE::getPlayerNum() const
{
#ifdef _SCC_DEBUG
	if((playerNum<0)||(playerNum>=MAX_PLAYER)) {
		toLog("DEBUG: (PRERACE::getPlayer): Variable not initialized.");return(0);
	}
#endif
	return(playerNum);
};

const START_CONDITION* const* PRERACE::getStartCondition()
{
	return(pStartCondition);
};


void EXPORT PRERACE::setTimer(const int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME)) {
		toLog("DEBUG: (PRERACE::setTimer): Value time out of range.");return;
	}
#endif
	timer=time;
};

const int EXPORT PRERACE::getTimer() const
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(timer);
};

void EXPORT PRERACE::setpStats(const UNIT_STATISTICS* const* pStats)
{
#ifdef _SCC_DEBUG
	if((*pStats)[0].minerals!=0) { // TODO
		toLog("DEBUG: (PRERACE::setpStats): Variable not initialized.");return;
	}
#endif
	this->pStats=pStats;
};

const UNIT_STATISTICS* const * EXPORT PRERACE::getpStats() const
{
#ifdef _SCC_DEBUG
	if(!pStats)	{
		toLog("DEBUG: (PRERACE::getpStats): Variable not initialized.");return(0);
	}
#endif
	return(pStats);
};

void EXPORT PRERACE::setIP(const int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::setIP): Value IP out of range.");return;
	}
#endif
	this->IP=IP;
};

const int EXPORT PRERACE::getIP() const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::getIP): Variable IP not initialized.");return(0);
	}
#endif
	return(IP);
}

const int EXPORT PRERACE::getFinalTime(const int goal) const
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
};

const int EXPORT PRERACE::getLength() const
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH)) {
		toLog("DEBUG: (PRERACE::getLength): Variable length not initialized.");return(0);
	}
#endif
	return(length);
};

void EXPORT PRERACE::setFinalTime(const int goal, const int time)
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
};

void EXPORT PRERACE::setLength(const int length)
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH))
	{
		toLog("DEBUG: (PRERACE::setLength): Value length out of range.");return;
	}
#endif
	this->length=length;
};


void EXPORT PRERACE::setTimeOut(const int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIMEOUT)) {
		toLog("DEBUG: (PRERACE::setTimeOut): Value time out of range.");return;
	}
#endif
	timeout=time;
};

const int EXPORT PRERACE::getTimeOut() const
{
#ifdef _SCC_DEBUG
	if((timeout<0)||(timeout>MAX_TIMEOUT)) {
		toLog("DEBUG: (PRERACE::getTimeOut): Variable timeout not initialized.");return(0);
	}
#endif
	return(timeout);
};

GOAL_ENTRY** PRERACE::getCurrentGoal() const
{
	return(pGoal);
};

// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

START* PRERACE::pStart;
const BASIC_MAP* const* PRERACE::pMap;

int PRERACE::noise[MAX_TIME];
int PRERACE::markerCounter;
UNIT PRERACE::unit[MAX_PLAYER][MAX_LOCATIONS];

void PRERACE::assignStart(START* start);
void PRERACE::initNoise();
void PRERACE::copyMap();

const int PRERACE::getMapLocationAvailible(const int player, const int loc, const int unit);
const int PRERACE::getMapLocationTotal(const int player, const int loc, const int unit);

void PRERACE::setMapLocationAvailible(const int player, const int loc, const int unit, const int num);
void PRERACE::setMapLocationTotal(const int player, const int loc, const int unit, const int num);

void PRERACE::addMapLocationAvailible(const int player, const int loc, const int unit, const int num);
void PRERACE::addMapLocationTotal(const int player, const int loc, const int unit, const int num);
void PRERACE::resetGeneMarker();
GA* PRERACE::ga;

