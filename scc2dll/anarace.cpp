#include "anarace.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
// TODO: reimplement/recheck the speed of the units


int EXPORT ANARACE::backupMap()
{
	int i,j,k;
	for(i=0;i<pMap->getMaxPlayer()-1;i++)
		for(j=0;j<MAX_LOCATIONS;j++)
			for(k=0;k<UNIT_TYPE_COUNT;k++)
			{
				backupLoc[i][j].availible[k]=getMapLocationAvailible(i,j,k);
				backupLoc[i][j].force[k]=getMapLocationForce(i,j,k);
			}
	return(1);
}

int EXPORT ANARACE::restoreMap()
{
	int i,j,k;
	for(i=0;i<pMap->getMaxPlayer()-1;i++)
		for(j=0;j<MAX_LOCATIONS;j++)
			for(k=0;k<UNIT_TYPE_COUNT;k++)
			{
				setMapLocationAvailible(i,j,k,backupLoc[i][j].availible[k]);
				setMapLocationForce(i,j,k,backupLoc[i][j].force[k]);
			}
	return(1);
}

int EXPORT ANARACE::getProgramDominant(int IP) 
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Value IP [%i] out of range.",IP);
		return(0);
	}
																			    
	if((program[IP].dominant<0)||(program[IP].dominant>=2))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Variable program[%i].dominant not initialized [%i].",IP,program[IP].dominant);
		return(0);
	}
	return(program[IP].dominant);
}


int EXPORT ANARACE::getProgramCode(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Value IP [%i] out of range.",IP);
		return(0);
	}
																			    
	if((Code[getProgramDominant(IP)][IP]<0)||(Code[getProgramDominant(IP)][IP]>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Variable not initialized [%i].",Code[getProgramDominant(IP)][IP]);
		return(0);
	}
	return(Code[getProgramDominant(IP)][IP]);
}


int EXPORT ANARACE::getProgramFacility(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramFacility): Value IP [%i] out of range.",IP);
		return(0);
	}
																		  
	if((program[IP].facility<0)||(program[IP].facility>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramFacility): Variable not initialized [%i].",program[IP].facility);
		return(0);
	}
	return(program[IP].facility);
}

int EXPORT ANARACE::getProgramBT(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramBT): Value IP [%i] out of range.",IP);
		return(0);
	}
																		  
	if((program[IP].BT<0)||(program[IP].BT>=MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramBT): Variable not initialized [%i].",program[IP].BT);
		return(0);
	}
	return(program[IP].BT);
}																      

int EXPORT ANARACE::getProgramSuccessType(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessType): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].successType<0)||(program[IP].successType>=ERROR_MESSAGES))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessType): Variable not initialized [%i].",program[IP].successType);
		return(0);
	}
	return(program[IP].successType);
};


int EXPORT ANARACE::getProgramSuccessUnit(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessUnit): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].successUnit<0)||(program[IP].successUnit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessUnit): Variable not initialized [%i].",program[IP].successUnit);
		return(0);
	}
	return(program[IP].successUnit);
};

/*int EXPORT ANARACE::getProgramSuccessLocation(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessLocation): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].successLocation<0)||(program[IP].successLocation>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessLocation): Variable not initialized [%i].",program[IP].successLocation);
		return(0);
	}
	return(program[IP].successLocation);
};*/

int EXPORT ANARACE::setProgramAvailibleCount(int IP, int unit, int count)
{
        if((IP<0)||(IP>=MAX_LENGTH))
        {
                debug.toLog(0,"DEBUG: (ANARACE::setProgramAvailibleCount): Value IP [%i] out of range.",IP);
                return(0);
        }
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (ANARACE::setProgramAvailibleCount): Value unit [%i] out of range.",unit);
                return(0);
        }
        if((count<0)||(count>=MAX_SUPPLY))
        {
                debug.toLog(0,"DEBUG: (ANARACE::setProgramAvailibleCount): Value count [%i] out of range.",count);
                return(0);
        }
        program[IP].availibleCount[unit]=count;
        return(1);
};


int EXPORT ANARACE::setProgramForceCount(int IP, int unit, int count)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramForceCount): Value IP [%i] out of range.",IP);
		return(0);
	}
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (ANARACE::setProgramForceCount): Value unit [%i] out of range.",unit);
                return(0);
        }
	if((count<0)||(count>=MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramForceCount): Value count [%i] out of range.",count);
		return(0);
	}
	program[IP].forceCount[unit]=count;
	return(1);
};



int EXPORT ANARACE::setProgramNeedSupply(int IP, int supply)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramNeedSupply): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((supply<0)||(supply>=MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramNeedSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
	program[IP].needSupply=supply;
	return(1);
};

int EXPORT ANARACE::setProgramHaveSupply(int IP, int supply)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramHaveSupply): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((supply<0)||(supply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramHaveSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
	program[IP].haveSupply=supply;
	return(1);
};

int EXPORT ANARACE::setProgramTime(int IP, int time)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramTime): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramTime): Value time [%i] out of range.",time);
		return(0);
	}
	program[IP].time=time;
	return(1);
};

int EXPORT ANARACE::setProgramHaveMinerals(int IP, int mins)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramHaveMinerals): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((mins<0)||(mins>=9999999))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramHaveMinerals): Value mins [%i] out of range.",mins);
		return(0);
	}
	program[IP].mins=mins;
	return(1);
};
																			    
int EXPORT ANARACE::setProgramHaveGas(int IP, int gas)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramHaveGas): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((gas<0)||(gas>=9999999))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramHaveGas): Value mins [%i] out of range.",gas);
		return(0);
	}
	program[IP].gas=gas;
	return(1);
};

int EXPORT ANARACE::setProgramIsGoal(int IP, int num)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramIsGoal): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((num<0)||(num>=2))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramIsGoal): Value num [%i] out of range.",num);
		return(0);
	}
	program[IP].isGoal=num;
	return(1);
}

int EXPORT ANARACE::setProgramDominant(int IP, int num)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramDominant): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((num<0)||(num>=2))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramDominant): Value num [%i] out of range.",num);
		return(0);
	}
	program[IP].dominant=num;
	return(1);
}


int EXPORT ANARACE::setProgramBT(int IP, int time)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramBT): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((time<0)||(time>=MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramBT): Value time [%i] out of range.",time);
		return(0);
	}
	program[IP].BT=time;
	return(1);
};


int EXPORT ANARACE::setProgramIsBuilt(int IP, int num)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramIsBuilt): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((num<0)||(num>=2))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramIsBuilt): Value num [%i] out of range.",num);
		return(0);
	}
	program[IP].built=num;
	return(1);
}

int EXPORT ANARACE::setProgramLocation(int IP, int location)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramLocation): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramLocation): Value location [%i] out of range.",location);
		return(0);
	}
	program[IP].location=location;
	return(1);
}


int EXPORT ANARACE::getProgramForceCount(int IP, int unit)       // how many units of the type at phaenoCode[s] do exist at that time?
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramForceCount): Value IP [%i] out of range.",IP);
		return(0);
	}
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (ANARACE::getProgramForceCount): Value unit [%i] out of range.",unit);
                return(0);
        }
	return(program[IP].forceCount[unit]);
};

int EXPORT ANARACE::getProgramAvailibleCount(int IP, int unit)       // how many units of the type at phaenoCode[s] do exist at that time?
{
        if((IP<0)||(IP>=MAX_LENGTH))
        {
                debug.toLog(0,"DEBUG: (ANARACE::getProgramAvailibleCount): Value IP [%i] out of range.",IP);
                return(0);
        }
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (ANARACE::getProgramAvailibleCount): Value unit [%i] out of range.",unit);
                return(0);
        }
        return(program[IP].availibleCount[unit]);
};



int EXPORT ANARACE::setProgramFacility(int IP, int facility)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramFacility): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((facility<0)||(facility>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramFacility): Value facility [%i] out of range.",facility);
		return(0);
	}
	program[IP].facility=facility;
	return(1);
};

int EXPORT ANARACE::setProgramSuccessType(int IP, int type)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramSuccessType): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((type<0)||(type>=ERROR_MESSAGES))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramSuccessType): Value type [%i] out of range.",type);
		return(0);
	}
	program[IP].successType=type;
	return(1);
};


int EXPORT ANARACE::setProgramSuccessUnit(int IP, int unit)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramSuccessUnit): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((unit<0)||(unit>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramSuccessUnit): Value unit [%i] out of range.",unit);
		return(0);
	}
	program[IP].successUnit=unit;
	return(1);
};

/*int EXPORT ANARACE::setProgramSuccessLocation(int num)
{
	if((num<0)||(num>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramSuccessLocation): Value [%i] out of range.",num);
		return(0);
	}
	program[getIP()].successLocation=num;
	return(1);
};*/

int EXPORT ANARACE::getProgramNeedSupply(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramNeedSupply): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].needSupply<0)||(program[IP].needSupply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramNeedSupply): Variable not initialized [%i].",program[IP].needSupply);
		return(0);
	}
	return(program[IP].needSupply);
};

int EXPORT ANARACE::getProgramHaveSupply(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveSupply): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].haveSupply<0)||(program[IP].haveSupply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveSupply): Variable not initialized [%i].",program[IP].haveSupply);
		return(0);
	}
	return(program[IP].haveSupply);
};


int EXPORT ANARACE::getProgramIsBuilt(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramIsBuilt): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].built<0)||(program[IP].built>1))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramIsBuilt): Variable not initialized [%i].",program[IP].built);
		return(0);
	}
	return(program[IP].built);
};

int EXPORT ANARACE::getProgramIsGoal(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramIsGoal): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].isGoal<0)||(program[IP].isGoal>1))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramIsGoal): Variable not initialized [%i].",program[IP].isGoal);
		return(0);
	}
	return(program[IP].isGoal);
};


int EXPORT ANARACE::getProgramTime(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramTime): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].time<0)||(program[IP].time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramTime): Variable not initialized [%i].",program[IP].time);
		return(0);
	}
	return(program[IP].time);
};

int EXPORT ANARACE::getProgramTemp(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramTemp): Value IP [%i] out of range.",IP);
		return(0);
	}
	return(program[IP].temp);
};

int EXPORT ANARACE::getProgramHaveMinerals(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveMinerals): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].mins<0)||(program[IP].mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveMinerals): Variable not initialized [%i].",program[IP].mins);
		return(0);
	}
	return(program[IP].mins);
};
int EXPORT ANARACE::getProgramHaveGas(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveGas): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].gas<0)||(program[IP].gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveGas): Variable not initialized [%i].",program[IP].gas);
		return(0);
	}
	return(program[IP].gas);
};

int EXPORT ANARACE::getProgramLocation(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramLocation): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].location<0)||(program[IP].location>MAX_LOCATIONS)) //todo: ga werte einfuegen
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramLocation): Variable not initialized [%i].",program[IP].location);
		return(0);
	}
	return(program[IP].location);
};

int EXPORT ANARACE::getProgramFitness(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramFitness): Value IP [%i] out of range.",IP);
		return(0);
	}
																			    
	if((program[IP].fitness<0)||(program[IP].fitness>100000)) //todo: ga werte einfuegen
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramFitness): Variable not initialized [%i].",program[IP].fitness);
		return(0);
	}
	return(program[IP].fitness);
};


int EXPORT ANARACE::calculateFitness() // Fuer den Uebersichtsgraphen unten rechts
{
	int i,tpF;
//	int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; // temporary data to check whether a bonus is already given (only applies if force > goal)
//TODO: Nicht alle Einheiten am Ort? => Ort egal sein lassen aber zur Zeit hinzuzaehlen
	// Nicht alle Einheiten ueberhaupt gebaut UND nicht alle am Ort => nur viertel Bonus fuer Einheiten die nicht am Ort sind
	tpF=0;
	for(i=MAX_GOALS;i--;)
		if(getPlayer()->goal->goal[i].count>0)
		{
			if( /*((getPlayer()->goal->goal[i].location==0)&&(getPlayer()->goal->goal[i].count>getLocationForce(0,getPlayer()->goal->goal[i].unit))) || ( (getPlayer()->goal->goal[i].location>0)&&*/(getPlayer()->goal->goal[i].count>getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit)))
			{
					//total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
					//TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen					 //evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!					 
				int sumup=0;
				int bon=0;
				if(getPlayer()->goal->goal[i].location==0)
					sumup=getLocationForce(0,getPlayer()->goal->goal[i].unit)*100;
				else
				{
					bon=getPlayer()->goal->goal[i].count;
					int j=1;
					while((j<MAX_LOCATIONS)&&(bon>getLocationForce(pMap->locationList[getPlayer()->goal->goal[i].location][j],getPlayer()->goal->goal[i].unit)))						 
					{
						sumup+=getLocationForce(pMap->locationList[getPlayer()->goal->goal[i].location][j],getPlayer()->goal->goal[i].unit)*(100-pMap->location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].getDistance(getPlayer()->goal->goal[i].location));							 
						bon-=getLocationForce(pMap->locationList[getPlayer()->goal->goal[i].location][j],getPlayer()->goal->goal[i].unit);						  
						j++;
					}
						//rest
					if(j<MAX_LOCATIONS)
						sumup+=bon*(100-pMap->location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].getDistance(getPlayer()->goal->goal[i].location));					 
				}
					//TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
					// evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
					// bissl komplex da mans ja den einzelnen goals verteilen muss...
				if((getPlayer()->goal->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->goal->goal[i].time))
//				{
//					if(getFinalTime(i)>0)
						tpF+=(getPlayer()->goal->goal[i].time*sumup)/(getPlayer()->goal->goal[i].count*getFinalTime(i));
//					else
//						tpF+=(getPlayer()->goal->goal[i].time*sumup)/(getPlayer()->goal->goal[i].count*ga->maxTime);
//				}
				else
					tpF+=sumup/getPlayer()->goal->goal[i].count;
			} //END force < goal
			else 
//if( ((getPlayer()->goal->goal[i].location==0)&&(getPlayer()->goal->goal[i].count<=getLocationForce(0,getPlayer()->goal->goal[i].unit))) || ( (getPlayer()->goal->goal[i].location>0)&&(getPlayer()->goal->goal[i].count<=getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit))) )
		//force >= goal
			{
				if((getPlayer()->goal->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->goal->goal[i].time))
					tpF+=(getPlayer()->goal->goal[i].time*100/getFinalTime(i));
				else tpF+=100;
// does not work yet, if this is uncommented, sFitness occasionally jumps to -1222000 or something like that... :/
// include the final location maybe...
//			      if(getPlayer()->goal->goal[i].count<location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit])
//				      setsFitness(getsFitness()-location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit])*(pStats[i].mins+pStats[i].gas));
			}
		}
// TODO: Check for very small 'goal.time' values, probably in scc.cpp!!
																			    
//Bonus: Sind noch Plaetze offen?
																			    /* Absturz 8[
		for(i=MAX_LOCATIONS;i--;)
		       for(j=UNIT_TYPE_COUNT;j--;)
			       bonus[i][j]=0;
																			    
		for(i=MAX_GOALS;i--;)
			if(location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit]<getPlayer()->goal->goal[i].count)
				bonus[getPlayer()->goal->goal[i].location][getPlayer()->goal->goal[i].unit]+=getPlayer()->goal->goal[i].count-location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit];		 for(i=MAX_BUILDINGS;i--;)
			if((building[i].RB>0)&&(bonus[building[i].type][building[i].location]>0))
			{			 //erstmal ohne Zeit...
				tpF+=((building[i].RB*100)/((location[building[i].location].force[building[i].type]+bonus[building[i].type][building[i].location])*pStats[building[i].type].BT));
																			    
				if((getPlayer()->goal->goal[building[i].type].time>0)&&(location[building[i].location].force[building[i].type]==0))
					tpF+=(building[i].RB*100*getPlayer()->goal->goal[building[i].type].time*location[0].force[i])/(getPlayer()->goal->goal[building[i].type].count*pStats[building[i].type].BT*ga->maxTime);
				else					 tpF+=((building[i].RB*100)/(getPlayer()->goal->goal[building[i].type].count*pStats[building[i].type].BT));
				bonus[building[i].location][building[i].type]--;
			}*/
	return(tpF);
}


int EXPORT ANARACE::calculateStep()
{
//ZERG: Larvenproduktion!  CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
	int tm,tg;
	int i,j;
	if((!time)||(ready)||(!getIP())||(4*time<3*bestTime))
	{
		setLength(ga->maxLength-getIP());
		if(ready)
			setTimer(time);
		else setTimer(0);
		for(i=0;i<MAX_LENGTH;i++)
			phaenoCode[i]=getPlayer()->goal->toPhaeno(Code[getProgramDominant(i)][i]);
		return(1);
	}

	tm=getMins();tg=getGas();
//TODO: evtl ueberlegen IP zu springen... also erstmal feststellen, warum nicht gebuildet werden kann und dann in einem Ruck resources und btimes hochsetzen...
	int dominant=0;
	int ok=1;
	while((ok)&&(getIP()))
	{
		if(Code[0][getIP()]>Code[1][getIP()]) //dominance
		{
			if(!(ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=0][getIP()]))))
				ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=1][getIP()]));
		}
		else
		{
			if(!(ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=1][getIP()]))))
				ok=buildGene(getPlayer()->goal->toPhaeno(Code[dominant=0][getIP()]));
		}
		if(successType>0)
		{
			setProgramSuccessType(getIP(),successType);
//			setProgramSuccessLocation(successLocation);
			setProgramSuccessUnit(getIP(),successUnit);
		}

		if((ok)||(!timeout))
		{
			if(timeout) 
			{
				setProgramTime(getIP(),ga->maxTime-time); //ANA~
				setProgramDominant(getIP(),dominant); //ANA~
			}
			else 
			{
				setProgramTime(getIP(),ga->maxTime);
				setProgramSuccessType(getIP(),TIMEOUT);
				setProgramSuccessUnit(getIP(),0);
//				setProgramSuccessLocation(0);
			}
			setProgramNeedSupply(getIP(),getMaxSupply()-getSupply());
			setProgramHaveSupply(getIP(),getMaxSupply());
			setProgramHaveMinerals(getIP(),tm);
			setProgramHaveGas(getIP(),tg);
//			program[getIP()].temp=location[1].availible[COMMAND_CENTER];
			for(i=0;i<UNIT_TYPE_COUNT;i++)
			{
				setProgramForceCount(getIP(),i,getLocationForce(0,i));
				setProgramAvailibleCount(getIP(),i,getLocationAvailible(0,i));
			}

//TODO Warum addiert er das hier und setzt es nicht einfach??

//TODO ueber getPlayer etc. den PhaenoCode rauszufinden erscheint mir etwas umstaendlich... evtl in "program" integrieren...
			program[getIP()].fitness=calculateFitness();
	
			timeout=ga->maxTimeOut;
			setIP(getIP()-1);
		}
	}
	
	//	BUILDING* build;//=&(building[MAX_BUILDINGS-1]);
//!!!???		for(j=MAX_BUILDINGS;j--;build=&(building[j]))
	for(j=MAX_BUILDINGS;j--;)
	{
//			build=&building[j];

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
						{
							addLocationForce(getBuildingLocation(j),getBuildingFacility(j),-1);
							setProgramFacility(getBuildingIP(j),getBuildingFacility(j));
							//availible was already taken account when starting the building
						}
						if(stat->facility2)
							addLocationForce(getBuildingLocation(j),stat->facility2,-1);
						break;
					case NEEDED_ONCE:break;
					case NEEDED_UNTIL_COMPLETE:
						if(getBuildingFacility(j))
						{
							addLocationAvailible(getBuildingLocation(j),getBuildingFacility(j),1);
							setProgramFacility(getBuildingIP(j),getBuildingFacility(j));
						}
						break; // TODO: fuer spaeter mal: Wenn in 2 Fabriken produziert wuerde wirds probmelatisch, da
//in Buiding nur eine facility gespeichert wird...??
					case NEEDED_ONCE_IS_LOST:
						if(stat->facility2)
							addLocationForce(getBuildingLocation(j),stat->facility2,-1);
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST:
						if(getBuildingFacility(j))
						{
							addLocationAvailible(getBuildingLocation(j),getBuildingFacility(j),1);
							setProgramFacility(getBuildingIP(j),getBuildingFacility(j));
						}
						if(stat->facility2)
							addLocationForce(getBuildingLocation(j),stat->facility2,-1);
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
						{
							if(getBuildingFacility(j))
							{
								addLocationAvailible(getBuildingLocation(j),getBuildingFacility(j),1);
								if(getBuildingFacility(j)) setProgramFacility(getBuildingIP(j),getBuildingFacility(j));
							}
							if(stat->facility2) // special rule for upgrades!
							{
								addLocationForce(getBuildingLocation(j),stat->facility2,-1);
								addLocationAvailible(getBuildingLocation(j),stat->facility2,1);
							};
						}
						break;
					case NEEDED_ALWAYS:break;
					default:debug.toLog(0,"DEBUG: (ANARACE::calculateStep) default case reached!");break;
				}

				if(getBuildingType(j)==REFINERY)
					addMapLocationForce(0,getBuildingLocation(j),VESPENE_GEYSIR,-1);

				if(stat->supply<0) 
				{
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
					//evtl noch location==0 als 'egal wo' einfuehren
				//ANA~
				setProgramIsBuilt(getBuildingIP(j),1);
				setProgramLocation(getBuildingIP(j),getBuildingLocation(j));
				setProgramBT(getIP(),getBuildingTotalBuildTime(j)); //~~~
				lastcounter++;
//			IP zeugs checken... length is immer 2 :/	 ??

                //                ready=1;
                                for(i=MAX_GOALS;i--;)
// ist dieses goal belegt?
                                if((getPlayer()->goal->goal[i].unit>0)&&
// befinden wir uns am richtigen Ort?
                                  ((getPlayer()->goal->goal[i].location==0)||(getBuildingLocation(j)==getPlayer()->goal->goal[i].location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
                                   (getBuildingType(j)==getPlayer()->goal->goal[i].unit))
                    //                    {
                                                setFinalTime(i,ga->maxTime-time);
// Did we reach the right number at the right time?
                                                ready&=(((getPlayer()->goal->goal[i].time==0)||(getFinalTime(i)<=getPlayer()->goal->goal[i].time))&&(getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit)>=getPlayer()->goal->goal[i].count));
//              i=MAX_GOALS;  TODO? koennen wir mehrere goals gleichzeitig erfuell0rn?
                  //                      }
				ready=1;
	                        for(i=MAX_GOALS;i--;)
				if(getPlayer()->goal->goal[i].count)
					
        	                        ready&=(
		(getPlayer()->goal->goal[i].count<=getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit))&&
		( (getPlayer()->goal->goal[i].time>=getFinalTime(i)) || (getPlayer()->goal->goal[i].time==0)) 
						);

	
/*				for(i=MAX_GOALS;i--;)
					if((getPlayer()->goal->goal[i].unit>0)&&((getPlayer()->goal->goal[i].location==0)||(getBuildingLocation(j)==getPlayer()->goal->goal[i].location))&&(getBuildingType(j)==getPlayer()->goal->goal[i].unit)&&((getPlayer()->goal->goal[i].time==0)||(time<=getPlayer()->goal->goal[i].time)))
						setFinalTime(getBuildingType(j),time);
					
				ready=1;
				for(i=MAX_GOALS;i--;)
					ready&=( ( ((getPlayer()->goal->goal[i].location==0)&&
(getPlayer()->goal->goal[i].count<=getLocationForce(0,getPlayer()->goal->goal[i].unit)))||((getPlayer()->goal->goal[i].location>0)&&
(getPlayer()->goal->goal[i].count<=getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit))) )&&
((getPlayer()->goal->goal[i].time>=getFinalTime(i))||(getPlayer()->goal->goal[i].time==0)));*/
			}
		}
	}
	harvestResources(); // TODO: tFitness runtersetzen, wenn zuviele scvs sammeln
	createSpecial(); //Larvae
	time--;
	timeout--;
	 //end while
//TODO: Alles rausschmeissen, was schon von race berechnet wurde!
	
	return(0);

	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
}

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)


int ANARACE::buildGene(int unit)
{
	int i;
	const UNIT_STATISTICS* stat=&pStats[unit];
	int ok=0;

	successType=0;
	successUnit=0;
//	successLocation=0;

	if(unit==1)
		successType=0;

	if(unit<=EXTRACTOR+1)
	{
		if((stat->prerequisite[0]>0)&&(getLocationForce(0,stat->prerequisite[0])==0))
		{
			successUnit=stat->prerequisite[0];
//			successLocation=0;
			successType=PREREQUISITE_FAILED;
		}
		else		
		if((stat->prerequisite[1]>0)&&(getLocationForce(0,stat->prerequisite[1])==0))
		{
			successUnit=stat->prerequisite[1];
//			successLocation=0;
			successType=PREREQUISITE_FAILED;
		}
		else 
		if((stat->prerequisite[2]>0)&&(getLocationForce(0,stat->prerequisite[2])==0))
		{
			successUnit=stat->prerequisite[2];
//			successLocation=0;
			successType=PREREQUISITE_FAILED;
		}
		else
		if //ANA~

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
			successType=FACILITY_FAILED;
		}
		else
		if((stat->facility2>0)&&(getLocationAvailible(0,stat->facility2)==0))
		{
			successUnit=stat->facility2;
//		    successLocation=0;
			successType=FACILITY_FAILED;
		}
//TODO: evtl success 2 Schritte zurueckverfolgen...
		else
		if(getMins()<stat->mins+stat->upgrade_cost*getLocationForce(0,unit))
		{
			successUnit=0;
//			successLocation=0;
		    successType=ENOUGH_MINERALS;
		}
		else
		if(getGas()<stat->gas+stat->upgrade_cost*getLocationForce(0,unit))
		{
			successUnit=0;
//			successLocation=0;
		    successType=ENOUGH_GAS;
		}
		else
		if((getSupply()<stat->supply)&&(stat->supply>0))
		{
			successUnit=0;
//			successLocation=0;
		    successType=SUPPLY_SATISFIED;
		}
		else 
		{
//Zuerst: availible pruefen ob am Ort gebaut werden kann
//Wenn nicht => +/- absteigen bis alle locations durch sind
			int fac=0;
			int tloc=1;
	    
			if(lastcounter>0)
			{
				lastcounter--;
				tloc=last[lastcounter].location;
			}

			if((stat->facility2==0)||(getLocationAvailible(tloc,stat->facility2)>0))
				for(fac=3;fac--;)
					if( ((stat->facility[fac]>0)&&(getLocationAvailible(tloc,stat->facility[fac])>0)) || ((stat->facility[fac]==0)&&(fac==0))) 
					{
						ok=1;
						break;
					}
/*			if(!ok)
				for(tloc=1;tloc<MAX_LOCATIONS;tloc++)
					if((stat->facility2==0)||(getLocationAvailible(tloc,stat->facility2)>0))
					{
						for(fac=3;fac--;)
							if( ((stat->facility[fac]>0)&&(getLocationAvailible(tloc,stat->facility[fac])>0)) || ((stat->facility[fac]==0)&&(fac==0)))
							{
								ok=1;
								break;
							}
						break;
					}	*/
//				bewegliche Sachen ueberdenken...
//					evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//					auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//		Phagen ueber Phagen...

			if((ok)&&(unit==EXTRACTOR))
			{
				if(!getMapLocationAvailible(0,tloc,VESPENE_GEYSIR))
					ok=0;
				else
					addMapLocationAvailible(0,tloc,VESPENE_GEYSIR,-1);
			};

			if(ok)
			{
				int nr=0;
				while((nr<MAX_BUILDINGS-1)&&(getBuildingRemainingBuildTime(nr)))
				{
					nr++;
				//	nr=MAX_BUILDINGS;
				}
				//TODO kommt hier nicht hin und wird trotzdem gebaut!??
				setBuildingFacility(nr,stat->facility[fac]);
				setBuildingLocation(nr,tloc);
				setBuildingUnitCount(nr,1); //~~
				setBuildingRemainingBuildTime(nr,stat->BT+3200*(stat->facility2==unit)); //~~ hack :/ TODO ???
				setBuildingTotalBuildTime(nr,getBuildingRemainingBuildTime(nr));
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
						break;
					case NEEDED_ONCE_IS_LOST:
						if(stat->facility2>0)
						{
							addLocationAvailible(tloc,stat->facility2,-1);
							setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/
						}
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST:
						if(stat->facility2>0)
						{
							addLocationAvailible(tloc,stat->facility2,-1);
							setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/
						}
						if(stat->facility[fac]>0)
							addLocationAvailible(tloc,stat->facility[fac],-1);
						break;
					case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
						if(stat->facility2>0)
							addLocationAvailible(tloc,stat->facility2,-1);
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
					default:debug.toLog(0,"DEBUG: (ANARACE::buildGene) default case reached!");break;
				}
		//		building[nr].on_the_run=0;
				setBuildingIP(nr,getIP());
			} //kk!=1?
		}
	}
	else // unit > EXTRACTOR+1
	{
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
					while((nr<MAX_BUILDINGS-1)&&(getBuildingRemainingBuildTime(nr)))
							nr++;
					//TODO: Fehler wenn nicht genug buildings
					if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>last[lastcounter].count)
						setBuildingUnitCount(nr,last[lastcounter].count);
					else setBuildingUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
					setBuildingFacility(nr,0);
					setBuildingLocation(nr,last[lastcounter].location);
					setBuildingType(nr,last[lastcounter].unit);
					setBuildingRemainingBuildTime(nr,pMap->location[last[lastcounter].location].getDistance(last[lastcounter].location+count)*100/pStats[last[lastcounter].unit].speed);
					setBuildingOnTheRun(nr,1);
					setBuildingIP(nr,getIP()); // ~ANA
						// 2x Unit => send 8/All instead of just one unit there
					if((getIP()>1)&&((Code[0][getIP()-1]==unit)||(Code[1][getIP()-1]==unit)))
					{
		    							if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>=6)
									setBuildingUnitCount(nr,6);
								else setBuildingUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
								setIP(getIP()-1);
								//TODO dominance... und 8 checken... evtl weniger
				       }
					addLocationAvailible(last[lastcounter].location,getBuildingType(nr),-getBuildingUnitCount(nr));
					addLocationForce(last[lastcounter].location,getBuildingType(nr),-getBuildingUnitCount(nr));
						ok=1;
				}
		}*/
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
//	if(ok) setProgramIsBuilt(getIP(),1); //??? TODO ???
//	else setProgramIsBuilt(getIP(),0);
	return(ok);
}


// Reset all ongoing data (between two runs)
void EXPORT ANARACE::resetData() // resets all data to standard starting values
{
	int i,j;
	for(i=UNIT_TYPE_COUNT;i--;)
		setFinalTime(i,0);
	resetBuildings();
	resetSpecial();
	for(i=MAX_LENGTH;i--;)//ANA~
	{
		setProgramSuccessType(i,0);
		setProgramSuccessUnit(i,0);
//		program[i].successLocation=0;
		setProgramIsBuilt(i,0);
		setProgramHaveSupply(i,0);
		setProgramNeedSupply(i,0);
		program[i].time=MAX_TIME;
	//	setProgramTime(i,MAX_TIME);
		setProgramHaveMinerals(i,0);
		setProgramHaveGas(i,0);
		for(j=0;j<UNIT_TYPE_COUNT;j++)
		{
			setProgramAvailibleCount(i,j,0);
			setProgramForceCount(i,j,0);
		}
		//program[i].temp=0;
		setProgramDominant(i,0);
		setProgramLocation(i,0);
		setProgramIsGoal(i,0);
		setProgramFacility(i,0);
		setProgramBT(i,0);
		phaenoCode[i]=0;
	}
	setHarvestedGas(0);
	setHarvestedMins(0);
	setMins(getPlayer()->getMins());
	setGas(getPlayer()->getGas());
	setTimer(getPlayer()->getTimer());
	setSupply(0);
	setMaxSupply(0);

	for(i=0;i<4;i++) //TODO
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
	setTimeOut(ga->maxTimeOut);
	setIP(ga->maxLength-1);
	setCalculated(0);
	time=ga->maxTime;
	ready=0;
}


int EXPORT ANARACE::getUnchangedGenerations()
{
	if((unchangedGenerations<0)||(unchangedGenerations>MAX_GENERATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getUnchangedGenerations): Variable not initialized [%i].",unchangedGenerations);
		return(0);
	}
	return(unchangedGenerations);
};

int EXPORT ANARACE::getRun()
{
	if((run<0)||(run>MAX_RUNS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getRun): Variable not initialized [%i].",run);
		return(0);
	}
	return(run);
};

int EXPORT ANARACE::getGeneration()
{
	if((generation<0)||(generation>MAX_GENERATIONS)) //ga->maxgeneration?
	{
		debug.toLog(0,"DEBUG: (ANARACE::getGeneration): Variable not initialized [%i].",generation);
		return(0);
	}
	return(generation);
};

int EXPORT ANARACE::getMaxpFitness()
{
	if((maxpFitness<0)||(maxpFitness>50000))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getMaxpFitness): Variable not initialized [%i].",maxpFitness);
		return(0);
	}
	return(maxpFitness);
};

int EXPORT ANARACE::getMaxsFitness()
{
	if((maxsFitness<0)||(maxsFitness>MAX_MINS+MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getMaxsFitness): Variable not initialized [%i].",maxsFitness);
		return(0);
	}
	return(maxsFitness);
};

int EXPORT EXPORT ANARACE::getMaxtFitness()
{
	if((maxtFitness<0)||(maxtFitness>50000))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getMaxtFitness): Variable not initialized [%i].",maxtFitness);
		return(0);
	}
	return(maxtFitness);
};

int EXPORT ANARACE::setUnchangedGenerations(int num)
{
//	if((num<0)||(num>ga->maxGenerations))
	if((num<0)||(num>MAX_GENERATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setUnchangedGenerations): Value [%i] out of range.",num);
		return(0);
	}
	unchangedGenerations=num;
	return(1);
};

int EXPORT ANARACE::setRun(int run)
{
//	if((num<0)||(num>ga->maxRuns))
	if((run<0)||(run>MAX_RUNS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setRun): Value run [%i] out of range.",run);
		return(0);
	}
	this->run=run;
	return(1);
};

int EXPORT ANARACE::setGeneration(int num)
{
//	if((num<0)||(num>ga->maxGenerations))
	if((num<0)||(num>MAX_GENERATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setGeneration): Value [%i] out of range.",num);
		return(0);
	}
	generation=num;
	return(1);
};

int EXPORT ANARACE::setMaxpFitness(int num)
{
	if((num<0)||(num>50000))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setMaxpFitness): Value [%i] out of range.",num);
		return(0);
	}
	maxpFitness=num;
	return(1);
};

int EXPORT ANARACE::setMaxsFitness(int num)
{
	if((num<0)||(num>MAX_MINS+MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setMaxsFitness): Value [%i] out of range.",num);
		return(0);
	}
	maxsFitness=num;
	return(1);
};

int EXPORT ANARACE::setMaxtFitness(int num)
{
	if((num<0)||(num>50000))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setMaxtFitness): Value [%i] out of range.",num);
		return(0);
	}
	maxtFitness=num;
	return(1);
};

void EXPORT ANARACE::analyzeBuildOrder()
{
//keeps track of the '@' symbol on the left of each build order entry
//if the goal is set to 10 marines, the eleventh won't be marked as a 'fulfilled goal' with a '@'
	int tGoal[UNIT_TYPE_COUNT];
	int s,t;

// reset the tgGoals (to sign with '@' the units which are part of the goal list)
// and subtract the units that are already on the map
	for(t=0;t<UNIT_TYPE_COUNT;t++)
	{
		tGoal[t]=0;
		for(s=1;s<MAX_LOCATIONS;s++)
			tGoal[t]+=getPlayer()->goal->globalGoal[s][t]-getMap()->location[s].force[getPlayerNum()][t];
	}

	for(s=0;s<MAX_LENGTH;s++)
		if(tGoal[phaenoCode[s]]>0) //~~~~~~ location=0?
		{
			tGoal[phaenoCode[s]]--;
			setProgramIsGoal(s,1);
		}
		else setProgramIsGoal(s,0);
};



ANARACE::ANARACE()
{
	setGeneration(0);
	setMaxpFitness(0);
	setMaxsFitness(0);
	setMaxtFitness(0);
	setUnchangedGenerations(0);
	setRun(0);
};

ANARACE::~ANARACE()
{

};

int EXPORT ANARACE::successType;
int EXPORT ANARACE::successUnit;
//int EXPORT ANARACE::successLocation;
MAP_LOCATION EXPORT ANARACE::backupLoc[MAX_PLAYER][MAX_LOCATIONS];

