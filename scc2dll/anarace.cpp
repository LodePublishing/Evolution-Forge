#include "anarace.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
// TODO: reimplement/recheck the speed of the units

int EXPORT ANARACE::getProgramCode(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
        {
                debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Value [%i] out of range.",IP);
                return(0);
        }
                                                                                                                                                            
        if((program[IP].facility<0)||(program[IP].facility>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Variable not initialized [%i].",Code[program[IP].dominant][IP]);
                return(0);
        }
        return(Code[program[IP].dominant][IP]);
}


int EXPORT ANARACE::getProgramFacility(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
        {
                debug.toLog(0,"DEBUG: (ANARACE::getProgramFacility): Value [%i] out of range.",IP);
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
                debug.toLog(0,"DEBUG: (ANARACE::getProgramBT): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessType): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessUnit): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessLocation): Value [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].successLocation<0)||(program[IP].successLocation>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramSuccessLocation): Variable not initialized [%i].",program[IP].successLocation);
		return(0);
	}
	return(program[IP].successLocation);
};*/


int EXPORT ANARACE::getProgramForceCount(int IP)       // how many units of the type at phaenoCode[s] do exist at that time?
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramForceCount): Value [%i] out of range.",IP);
		return(0);
	}
	return(program[getIP()].forceCount);
};


int EXPORT ANARACE::setProgramFacility(int num)
{
        if((num<0)||(num>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (ANARACE::setProgramFacility): Value [%i] out of range.",num);
                return(0);
        }
        program[getIP()].facility=num;
        return(1);
};

int EXPORT ANARACE::setProgramBT(int num)
{
        if((num<0)||(num>=MAX_TIME))
        {
                debug.toLog(0,"DEBUG: (ANARACE::setProgramBT): Value [%i] out of range.",num);
                return(0);
        }
        program[getIP()].BT=num;
        return(1);
};

int EXPORT ANARACE::setProgramSuccessType(int num)
{
	if((num<0)||(num>=ERROR_MESSAGES))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramSuccessType): Value [%i] out of range.",num);
		return(0);
	}
	program[getIP()].successType=num;
	return(1);
};


int EXPORT ANARACE::setProgramSuccessUnit(int num)
{
	if((num<0)||(num>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setProgramSuccessUnit): Value [%i] out of range.",num);
		return(0);
	}
	program[getIP()].successUnit=num;
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramNeedSupply): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveSupply): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramIsBuilt): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramIsGoal): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramTime): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramTemp): Value [%i] out of range.",IP);
		return(0);
	}
	return(program[IP].temp);
};

int EXPORT ANARACE::getProgramHaveMinerals(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveMinerals): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramHaveGas): Value [%i] out of range.",IP);
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
		debug.toLog(0,"DEBUG: (ANARACE::getProgramLocation): Value [%i] out of range.",IP);
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
                debug.toLog(0,"DEBUG: (ANARACE::getProgramFitness): Value [%i] out of range.",IP);
                return(0);
        }
                                                                                                                                                            
        if((program[IP].fitness<0)||(program[IP].fitness>100000)) //todo: ga werte einfuegen
        {
                debug.toLog(0,"DEBUG: (ANARACE::getProgramFitness): Variable not initialized [%i].",program[IP].fitness);
                return(0);
        }
	return(program[IP].fitness);
};


int EXPORT ANARACE::calculateFitness()
{
        int i,j,tpF,sumup;
        int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; // temporary data to check whether a bonus is already given (only applies if force > goal)
//TODO: Nicht alle Einheiten am Ort? => Ort egal sein lassen aber zur Zeit hinzuzaehlen
        // Nicht alle Einheiten ueberhaupt gebaut UND nicht alle am Ort => nur viertel Bonus fuer Einheiten die nicht am Ort sind
	tpF=0;
	for(i=MAX_GOALS;i--;)
        	if(getPlayer()->goal->goal[i].count>0)
                        {
                                if( ((getPlayer()->goal->goal[i].location==0)&&(getPlayer()->goal->goal[i].count>location[0].force[getPlayer()->goal->goal[i].unit])) || ( (getPlayer()->goal->goal[i].location>0)&&(getPlayer()->goal->goal[i].count>location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit])) )                                 {
                                        //total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
                                        //TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen                                         //evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!                                         int sumup=0;
                                        int bon=0;
                                        if(getPlayer()->goal->goal[i].location==0)
                                                sumup=location[0].force[getPlayer()->goal->goal[i].unit]*100;
                                        else
                                        {
                                                bon=getPlayer()->goal->goal[i].count;
                                                int j=1;
                                                while((j<MAX_LOCATIONS)&&(bon>location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].force[getPlayer()->goal->goal[i].unit]))                                                 {
                                                        sumup+=location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].force[getPlayer()->goal->goal[i].unit]*(100-pMap->location[j].getDistance(getPlayer()->goal->goal[i].location));                                                         bon-=location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].force[getPlayer()->goal->goal[i].unit];                                                         j++;
                                        }
                                                //rest
                                        if(j<MAX_LOCATIONS)
                                        sumup+=bon*(100-pMap->location[pMap->locationList[getPlayer()->goal->goal[i].location][j]].getDistance(getPlayer()->goal->goal[i].location));                                         }
                                        //TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
                                        // evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
                                        // bissl komplex da mans ja den einzelnen goals verteilen muss...
                                        if(getPlayer()->goal->goal[i].time>0)
                                        {
                                                if(getFinalTime(i)>0)
                                                        tpF+=(getPlayer()->goal->goal[i].time*sumup)/(getPlayer()->goal->goal[i].count*getFinalTime(i));                                                 else
                                                        tpF+=(getPlayer()->goal->goal[i].time*sumup)/(getPlayer()->goal->goal[i].count*ga->maxTime);
                                        }
                                        else
                                                tpF+=sumup/getPlayer()->goal->goal[i].count;
                                }
                                else if( ((getPlayer()->goal->goal[i].location==0)&&(getPlayer()->goal->goal[i].count<=location[0].force[getPlayer()->goal->goal[i].unit])) || ( (getPlayer()->goal->goal[i].location>0)&&(getPlayer()->goal->goal[i].count<=location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit])) )
                                                //force >= goal
                                {
                                        if((getPlayer()->goal->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->goal->goal[i].time))
                                                tpF+=(getPlayer()->goal->goal[i].time*100/getFinalTime(i));
                                        else tpF+=100;
// does not work yet, if this is uncommented, sFitness occasionally jumps to -1222000 or something like that... :/
// include the final location maybe...
//                              if(getPlayer()->goal->goal[i].count<location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit])
//                                      setsFitness(getsFitness()-location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit])*(pStats[i].mins+pStats[i].gas));
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
                                bonus[getPlayer()->goal->goal[i].location][getPlayer()->goal->goal[i].unit]+=getPlayer()->goal->goal[i].count-location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit];                 for(i=MAX_BUILDINGS;i--;)
                        if((building[i].RB>0)&&(bonus[building[i].type][building[i].location]>0))
                        {                         //erstmal ohne Zeit...
                                tpF+=((building[i].RB*100)/((location[building[i].location].force[building[i].type]+bonus[building[i].type][building[i].location])*pStats[building[i].type].BT));
                                                                                                                                                            
                                if((getPlayer()->goal->goal[building[i].type].time>0)&&(location[building[i].location].force[building[i].type]==0))
                                        tpF+=(building[i].RB*100*getPlayer()->goal->goal[building[i].type].time*location[0].force[i])/(getPlayer()->goal->goal[building[i].type].count*pStats[building[i].type].BT*ga->maxTime);
                                else                                         tpF+=((building[i].RB*100)/(getPlayer()->goal->goal[building[i].type].count*pStats[building[i].type].BT));
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
	if((!time)||(ready)||(!getIP()))
	{
		setLength(ga->maxLength-getIP());
		if(ready)
			setTimer(time);
		else setTimer(ga->maxTime);
		for(i=0;i<MAX_LENGTH;i++)
			phaenoCode[i]=getPlayer()->goal->toPhaeno(Code[program[i].dominant][i]);
		return(1);
	}

		tm=getMins();tg=getGas();
//TODO: evtl ueberlegen IP zu springen... also erstmal feststellen, warum nicht gebuildet werden kann und dann in einem Ruck resources und btimes hochsetzen...
		int dominant=0;
		int ok=0;
					//evtl versuchen umzuschreiben, dass er mehrere Befehle pro Sekunde machen kann ... TODO
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
			setProgramSuccessType(successType);
//			setProgramSuccessLocation(successLocation);
			setProgramSuccessUnit(successUnit);
		}

		if((ok)||(!timeout))
		{
			if(timeout) 
			{
				program[getIP()].time=ga->maxTime-time; //ANA~
		                program[getIP()].dominant=dominant; //ANA~
			}
			else 
			{
				program[getIP()].time=ga->maxTime+1;
				setProgramSuccessType(TIMEOUT);
				setProgramSuccessUnit(0);
//				setProgramSuccessLocation(0);
			}
			program[getIP()].needSupply=getMaxSupply()-getSupply();
			program[getIP()].haveSupply=getMaxSupply();
			program[getIP()].mins=tm;//getMins();
			program[getIP()].gas=tg;//getGas();
//			program[getIP()].temp=location[1].availible[COMMAND_CENTER];
			program[getIP()].forceCount+=location[0].force[getPlayer()->goal->toPhaeno(Code[program[getIP()].dominant][getIP()])];
			program[getIP()].fitness=calculateFitness();
	
			timeout=ga->maxTimeOut;
			setIP(getIP()-1);
		}
	
		BUILDING* build;//=&(building[MAX_BUILDINGS-1]);
//!!!???		for(j=MAX_BUILDINGS;j--;build=&(building[j]))
		for(j=MAX_BUILDINGS;j--;)
		{
			build=&building[j];

			if(build->RB)
			{
				if(!(--(build->RB)))
				{
					const UNIT_STATISTICS* stat=&pStats[build->type];
					switch(stat->facility_type)
					{
						case IS_LOST:
							if(build->facility)
							{
								location[build->location].force[build->facility]--;
								location[0].force[build->facility]--;
								//availible was already taken account when starting the building
							}
							if(stat->facility2)
							{
								location[build->location].force[stat->facility2]--;
								location[0].force[stat->facility2]--;
							}
							break;
						case NEEDED_ONCE:break;
						case NEEDED_UNTIL_COMPLETE:
							if(build->facility)
							{
								location[build->location].availible[build->facility]++;
								location[0].availible[build->facility]++;
                                                                setProgramFacility(build->facility);
							}
							break; // fuer spaeter mal: Wenn in 2 Fabriken produziert wuerde wirds probmelatisch, da
//in Buiding nur eine facility gespeichert wird...
						case NEEDED_ONCE_IS_LOST:
							if(stat->facility2)
							{
								location[build->location].force[stat->facility2]--;
								location[0].force[stat->facility2]--;
							}
							break;
						case NEEDED_UNTIL_COMPLETE_IS_LOST:
							if(build->facility)
							{
								location[build->location].availible[build->facility]++;
								location[0].availible[build->facility]++;
                                                                setProgramFacility(build->facility);
							}
							if(stat->facility2)
							{
								location[build->location].force[stat->facility2]--;
								location[0].force[stat->facility2]--;
							}
							break;
						case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
							{
								if(build->facility)
								{
									location[build->location].availible[build->facility]++;
									location[0].availible[build->facility]++;
	                                                                setProgramFacility(build->facility);
								}
								if(stat->facility2) // special rule for upgrades!
								{
									location[build->location].force[stat->facility2]--;
									location[build->location].availible[stat->facility2]++;
									location[0].force[stat->facility2]--;
									location[0].availible[stat->facility2]++;
								};
							}
							break;
						case NEEDED_ALWAYS:break;
						default:break;
					}

					if(build->type==REFINERY)
						loc[0][build->location].force[VESPENE_GEYSIR]--;

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
					location[0].force[build->type]+=build->unitCount;
					location[0].availible[build->type]+=build->unitCount;
					location[build->location].force[build->type]+=build->unitCount;
					location[build->location].availible[build->type]+=build->unitCount;
					
					if(build->type==REFINERY) 
						adjustGasHarvest(build->location);
					if(build->type==COMMAND_CENTER) 
					{
						adjustMineralHarvest(build->location);
						adjustGasHarvest(build->location);
					}
					last[lastcounter].unit=build->type;
					last[lastcounter].count=build->unitCount;
					last[lastcounter].location=build->location;

					if((stat->create)&&(!build->onTheRun)) //one additional unit (zerglings, scourge, comsat, etc.)
					{ //here no unitCount! ~~~
						location[build->location].force[stat->create]++;
						location[build->location].availible[stat->create]++;
						location[0].force[stat->create]++;
						location[0].availible[stat->create]++;
						// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
					}
					//evtl noch location==0 als 'egal wo' einfuehren
					//ANA~

					program[build->IP].built=1;
					program[build->IP].location=build->location;
					setProgramBT(build->TB); //~~~

					lastcounter++;
	//			IP zeugs checken... length is immer 2 :/	
					for(i=MAX_GOALS;i--;)
						if((getPlayer()->goal->goal[i].unit>0)&&((getPlayer()->goal->goal[i].location==0)||(build->location==getPlayer()->goal->goal[i].location))&&(build->type==getPlayer()->goal->goal[i].unit)&&((getPlayer()->goal->goal[i].time==0)||(time<=getPlayer()->goal->goal[i].time)))
							setFinalTime(build->type,time);
					
					ready=1;
					for(i=MAX_GOALS;i--;)
						ready&=( ( ((getPlayer()->goal->goal[i].location==0)&&(getPlayer()->goal->goal[i].count<=location[0].force[getPlayer()->goal->goal[i].unit]))||((getPlayer()->goal->goal[i].location>0)&&(getPlayer()->goal->goal[i].count<=location[getPlayer()->goal->goal[i].location].force[getPlayer()->goal->goal[i].unit])) )&&((getPlayer()->goal->goal[i].time>=getFinalTime(i))||(getPlayer()->goal->goal[i].time==0)));
					// oder: irgendeine location... TODO: Problem: die Einheiten koennen irgendwo sein, also nicht gesammelt an einem Fleck...
				}
			}
		}
		harvestResources();
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
	const UNIT_STATISTICS* stat=&pStats[unit];
	int ok=0;

	successType=0;
	successUnit=0;
//	successLocation=0;

	if(unit==1)
		successType=0;

	if(unit<=EXTRACTOR+1)
	{
		if((stat->prerequisite[0]>0)&&(location[0].force[stat->prerequisite[0]]==0))
		{
			successUnit=stat->prerequisite[0];
//			successLocation=0;
			successType=PREREQUISITE_FAILED;
		}
		else		
		if((stat->prerequisite[1]>0)&&(location[0].force[stat->prerequisite[1]]==0))
                {
                        successUnit=stat->prerequisite[1];
//                        successLocation=0;
                        successType=PREREQUISITE_FAILED;
                }
		else 
		if((stat->prerequisite[2]>0)&&(location[0].force[stat->prerequisite[2]]==0))
                {
                        successUnit=stat->prerequisite[2];
//                        successLocation=0;
                        successType=PREREQUISITE_FAILED;
                }
		else
		if //ANA~

                        ( ((stat->facility[0]==0)||(location[0].availible[stat->facility[0]]==0))&&
                          ((stat->facility[1]==0)||(location[0].availible[stat->facility[1]]==0))&&
                          ((stat->facility[2]==0)||(location[0].availible[stat->facility[2]]==0))&&
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
		if((stat->facility2>0)&&(location[0].availible[stat->facility2]==0))
        {
			successUnit=stat->facility2;
//            successLocation=0;
            successType=FACILITY_FAILED;
        }
//TODO: evtl success 2 Schritte zurueckverfolgen...
		else
		if(getMins()<stat->mins+stat->upgrade_cost*location[0].force[unit])
		{
			successUnit=0;
//			successLocation=0;
		    successType=ENOUGH_MINERALS;
		}
		else
		if(getGas()<stat->gas+stat->upgrade_cost*location[0].force[unit])
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

			if((stat->facility2==0)||(location[tloc].availible[stat->facility2]>0))
				for(fac=3;fac--;)
					if( ((stat->facility[fac]>0)&&(location[tloc].availible[stat->facility[fac]]>0)) || ((stat->facility[fac]==0)&&(fac==0))) 
					{
						ok=1;
						break;
					}
			if(!ok)
				for(tloc=1;tloc<MAX_LOCATIONS;tloc++)
					if((stat->facility2==0)||(location[tloc].availible[stat->facility2]>0))
					{
						for(fac=3;fac--;)
							if( ((stat->facility[fac]>0)&&(location[tloc].availible[stat->facility[fac]]>0)) || ((stat->facility[fac]==0)&&(fac==0)))
							{
								ok=1;
								break;
							}
						break;
					}	
//				bewegliche Sachen ueberdenken...
//					evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//					auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//		Phagen ueber Phagen...

				if((ok)&&(unit==EXTRACTOR))
				{
					if(!loc[0][tloc].availible[VESPENE_GEYSIR])
						ok=0;
					else
						loc[0][tloc].availible[VESPENE_GEYSIR]--;
				};

					if(ok)
					{
						int nr=0;
						while((nr<MAX_BUILDINGS-1)&&(building[nr].RB))
						{
							nr++;
						//	nr=MAX_BUILDINGS;
						}
						
						building[nr].facility=stat->facility[fac];
						building[nr].location=tloc;
						building[nr].unitCount=1; //~~
						building[nr].RB=stat->BT+3200*(stat->facility2==unit); //~~ hack :/ TODO ???
						building[nr].TB=building[nr].RB;
						setMins(getMins()-(stat->mins+stat->upgrade_cost*location[0].force[unit]));
						setGas(getGas()-(stat->gas+stat->upgrade_cost*location[0].force[unit]));
						building[nr].type=unit;
						if((stat->supply>0)||((pStats[stat->facility[0]].supply<0)&&(stat->facility_type==IS_LOST))) setSupply(getSupply()-stat->supply);
							switch(stat->facility_type)
							{
								case IS_LOST:
								if(stat->facility[fac]>0)
								{
									location[tloc].availible[stat->facility[fac]]--;
									location[0].availible[stat->facility[fac]]--;
									setSupply(getSupply()+pStats[stat->facility[fac]].supply); 
								}
								if(stat->facility2>0)
								{
									location[tloc].availible[stat->facility2]--;
									location[0].availible[stat->facility2]--;
									setSupply(getSupply()+pStats[stat->facility2].supply);
								}
								break;
							case NEEDED_ONCE:
								break;
							case NEEDED_UNTIL_COMPLETE:
								if(stat->facility[fac]>0)
								{
									location[tloc].availible[stat->facility[fac]]--;
									location[0].availible[stat->facility[fac]]--;
								}
								if(stat->facility2>0)
								{
									location[tloc].availible[stat->facility2]--;
									location[0].availible[stat->facility2]--; //kommt glaub nicht vor...
								}
								break;
							case NEEDED_ONCE_IS_LOST:
								if(stat->facility2>0)
								{
									location[tloc].availible[stat->facility2]--;
									location[0].availible[stat->facility2]--;
									setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/
								}
								break;
							case NEEDED_UNTIL_COMPLETE_IS_LOST:
								if(stat->facility2>0)
								{
									location[tloc].availible[stat->facility2]--;
									location[0].availible[stat->facility2]--;
									setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/
								}
								if(stat->facility[fac]>0)
								{
									location[tloc].availible[stat->facility[fac]]--;
									location[0].availible[stat->facility[fac]]--;
								}
								break;
							case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
								if(stat->facility2>0)
								{
									location[tloc].availible[stat->facility2]--;						
									location[0].availible[stat->facility2]--; // no supply gain as the item is recovered... well or not? mmmh... ~~~~
								}
								if(stat->facility[fac]>0)
								{
									location[tloc].availible[stat->facility[fac]]--;
									location[0].availible[stat->facility[fac]]--;
								}
								break;
							case NEEDED_ALWAYS:
								if(stat->facility[fac]>0)
								{
									location[tloc].availible[stat->facility[fac]]--;
									location[0].availible[stat->facility[fac]]--;
									// supply?
								}
								if(stat->facility2>0)
								{
									location[tloc].availible[stat->facility2]--;
									location[0].availible[stat->facility2]--;
								}
								break;
							default:break;
						}
	
//						building[nr].on_the_run=0;
						building[nr].IP=getIP();
					} //kk!=1?
				}
			}
	else // unit > EXTRACTOR+1
	{
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
                        if((lastcounter>0)&&(location[last[lastcounter-1].location].availible[last[lastcounter-1].unit]>0)&&(pStats[last[lastcounter-1].unit].speed>0))
                                {
					lastcounter--;
                                        int nr=0;
                                        while((nr<MAX_BUILDINGS-1)&&(building[nr].RB))
                                                        nr++;
                                        //TODO: Fehler wenn nicht genug buildings
					if(location[last[lastcounter].location].availible[last[lastcounter].unit]>last[lastcounter].count)
                                                building[nr].unitCount=last[lastcounter].count;
                                        else building[nr].unitCount=location[last[lastcounter].location].availible[last[lastcounter].unit];
                                        building[nr].facility=0;
                                        building[nr].location=last[lastcounter].location;
                                        building[nr].type=last[lastcounter].unit;
                                        building[nr].RB=pMap->location[last[lastcounter].location].getDistance(last[lastcounter].location+count)*100/pStats[last[lastcounter].unit].speed;
                                        building[nr].onTheRun=1;
					building[nr].IP=getIP(); // ~ANA
                                                // 2x Unit => send 8/All instead of just one unit there
                                        if((getIP()>1)&&((Code[0][getIP()-1]==unit)||(Code[1][getIP()-1]==unit)))
                                        {
                    							if(location[last[lastcounter].location].availible[last[lastcounter].unit]>=6)
                                                                        building[nr].unitCount=6;
                                                                else building[nr].unitCount=location[last[lastcounter].location].availible[last[lastcounter].unit];
                                                                setIP(getIP()-1);
                                                                //TODO dominance... und 8 checken... evtl weniger
                                       }
                                                location[last[lastcounter].location].availible[building[nr].type]-=building[nr].unitCount;
                                                location[last[lastcounter].location].force[building[nr].type]-=building[nr].unitCount;
                                                location[0].force[building[nr].type]-=building[nr].unitCount;
                                                location[0].availible[building[nr].type]-=building[nr].unitCount;
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
	if(ok)
		program[getIP()].built=1; //ANA~
	return ok;
}


// Reset all ongoing data (between two runs)
void EXPORT ANARACE::resetData() // resets all data to standard starting values
{
	int i;
	for(i=UNIT_TYPE_COUNT;i--;)
		setFinalTime(i,0);
	for(i=MAX_BUILDINGS;i--;)
	{
	       building[i].RB=0;
	building[i].TB=0;
	       building[i].type=255;
	       building[i].IP=0;
	       building[i].location=0;
	       building[i].unitCount=0;
	       building[i].onTheRun=0;
	}
	for(i=MAX_LENGTH;i--;)//ANA~
    {
        program[i].successType=0;
		program[i].successUnit=0;
//		program[i].successLocation=0;
						
		program[i].built=0;
		program[i].haveSupply=0;
        program[i].needSupply=0;
        program[i].time=MAX_TIME;
        program[i].mins=0;
        program[i].gas=0;
	program[i].forceCount=1;
        program[i].temp=0;
        program[i].dominant=0;
		program[i].location=0;
		program[i].isGoal=0;
	program[i].facility=0;
	program[i].BT=0;
		phaenoCode[i]=0;
	}
	setHarvestedGas(0);
	setHarvestedMins(0);
	setMins(getPlayer()->getMins());
	setGas(getPlayer()->getGas());
	setTimer(getPlayer()->getTimer());
	setSupply(0);
	setMaxSupply(0);

	for(i=0;i<4;i++)
	{
		last[i].location=1;
		last[i].unit=SCV;
		last[i].count=1;
	}
	for(i=4;i<MAX_LOCATIONS;i++)
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

int EXPORT ANARACE::setRun(int num)
{
//	if((num<0)||(num>ga->maxRuns))
	if((num<0)||(num>MAX_RUNS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setRun): Value [%i] out of range.",num);
		return(0);
	}
	run=num;
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
			program[s].isGoal=1;
		}
		else program[s].isGoal=0;
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

