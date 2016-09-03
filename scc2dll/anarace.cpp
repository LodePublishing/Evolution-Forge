#include "anarace.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
// TODO: reimplement/recheck the speed of the units

ANARACE::ANARACE()
{
	setGeneration(0);
	setMaxpFitness(0);
	setMaxsFitness(0);
	setMaxtFitness(MAX_TFITNESS);
	setUnchangedGenerations(0);
	setRun(0);
	optimizing=0;
	active=1;
	totalUnitForce=4;
	maxUnitForce=4;
	timePercentage=0;
	goalPercentage=0;
};

ANARACE::~ANARACE()
{
};


int EXPORT ANARACE::getMaxUnitForce()
{
	return(maxUnitForce);
};

int EXPORT ANARACE::getTotalUnitForce()
{
	return(totalUnitForce);
};

bool EXPORT ANARACE::isActive()
{
	return(active);
};

void EXPORT ANARACE::setActive(bool active)
{
	this->active=active;
};

bool EXPORT ANARACE::isOptimizing()
{
	return(optimizing);
};

void EXPORT ANARACE::setOptimizing(bool optimizing)
{
	this->optimizing=optimizing;
};


int EXPORT ANARACE::getMarker(int IP)
{
#ifdef _SCC_DEBUG
		if((IP<0)||(IP>MAX_LENGTH))
		{
				debug.toLog(0,"DEBUG: (ANARACE::getMarker): Value IP [%i] out of range.",IP);
				return(0);
		}
#endif
		return(Marker[IP]); 
};

void EXPORT ANARACE::removeOrder(int IP)
{
		for(int j=IP;j--;)
		{
				Code[j+1]=Code[j];
				Marker[j+1]=Marker[j];
				program[j+1].built=program[j].built;
		}
	for(int j=0;j<MAX_LENGTH;j++)
				phaenoCode[j]=getPlayer()->getGoal()->toPhaeno(Code[j]);
		getPlayer()->wasChanged(); // to allow update of time etc. of anarace
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
	
	if(getPlayer()->getGoal()->allGoal[unit]==0)
	{
		getPlayer()->getGoal()->addGoal(unit,1,0,0);
		getPlayer()->changeAccepted();
	}
	replaceCode(i,getPlayer()->getGoal()->toGeno(unit));
	
	program[i].built=1;

	for(int j=0;j<MAX_LENGTH;j++)
		phaenoCode[j]=getPlayer()->getGoal()->toPhaeno(Code[j]); 
	getPlayer()->wasChanged(); // to allow update of time etc. of anarace
}

int EXPORT ANARACE::backupMap()
{
/*	for(int i=0;i<pMap->getMaxPlayer()-1;i++)
		for(int j=0;j<MAX_LOCATIONS;j++)
			for(int k=0;k<UNIT_TYPE_COUNT;k++)
			{
				backupLoc[i][j].availible[k]=getMapLocationAvailible(i,j,k);
				backupLoc[i][j].force[k]=getMapLocationForce(i,j,k);
			}*/
	return(1);
}

int EXPORT ANARACE::restoreMap()
{
/*	for(int i=0;i<pMap->getMaxPlayer()-1;i++)
		for(int j=0;j<MAX_LOCATIONS;j++)
			for(int k=0;k<UNIT_TYPE_COUNT;k++)
			{
				setMapLocationAvailible(i,j,k,backupLoc[i][j].availible[k]);
				setMapLocationForce(i,j,k,backupLoc[i][j].force[k]);
			}*/
	return(1);
};

int EXPORT ANARACE::getProgramCode(int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Value IP [%i] out of range.",IP);
		return(0);
	}
	if((Code[IP]<0)||(Code[IP]>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramCode): Variable Code[%i] not initialized [%i].",Code[IP], IP);
		return(0);
	}
#endif					
	return(Code[IP]);
}


int EXPORT ANARACE::getProgramFacility(int IP)
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].facility);
}

int EXPORT ANARACE::getProgramBT(int IP)
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].BT);
}																	  

int EXPORT ANARACE::getProgramSuccessType(int IP)
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].successType);
};


int EXPORT ANARACE::getProgramSuccessUnit(int IP)
{
#ifdef _SCC_DEBUG
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
#endif
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
#ifdef _SCC_DEBUG
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
#endif
	program[IP].availibleCount[unit]=count;
	return(1);
};


int EXPORT ANARACE::setProgramForceCount(int IP, int unit, int count)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].forceCount[unit]=count;
	return(1);
};



int EXPORT ANARACE::setStatisticsNeedSupply(int time, int supply)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsNeedSupply): Value time [%i] out of range.",time);
		return(0);
	}
	if((supply<0)||(supply>=MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsNeedSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
#endif
	statistics[time].needSupply=supply;
	return(1);
};

int EXPORT ANARACE::setStatisticsHaveSupply(int time, int supply)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsHaveSupply): Value time [%i] out of range.",time);
		return(0);
	}
	if((supply<0)||(supply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsHaveSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
#endif
	statistics[time].haveSupply=supply;
	return(1);
};

int EXPORT ANARACE::setProgramTime(int IP, int time)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].time=time;
	return(1);
};

int EXPORT ANARACE::setStatisticsHaveMinerals(int time, int mins)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsHaveMinerals): Value time [%i] out of range.",time);
		return(0);
	}
	if((mins<0)||(mins>=9999999))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsHaveMinerals): Value mins [%i] out of range.",mins);
		return(0);
	}
#endif
	statistics[time].mins=mins;
	return(1);
};
																				
int EXPORT ANARACE::setStatisticsHaveGas(int time, int gas)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsHaveGas): Value time [%i] out of range.",time);
		return(0);
	}
	if((gas<0)||(gas>=9999999))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setStatisticsHaveGas): Value mins [%i] out of range.",gas);
		return(0);
	}
#endif
	statistics[time].gas=gas;
	return(1);
};

int EXPORT ANARACE::setProgramIsGoal(int IP, int num)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].isGoal=num;
	return(1);
};

/*int EXPORT ANARACE::setProgramIsConstant(int IP, int num)
{
		if((IP<0)||(IP>=MAX_LENGTH))
		{
				debug.toLog(0,"DEBUG: (ANARACE::setProgramIsConstant): Value IP [%i] out of range.",IP);
				return(0);
		}
		if((num<0)||(num>=2))
		{
				debug.toLog(0,"DEBUG: (ANARACE::setProgramIsConstant): Value num [%i] out of range.",num);
				return(0);
		}
		program[IP].isConstant=num;
		return(1);
};*/





int EXPORT ANARACE::setProgramBT(int IP, int time)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].BT=time;
	return(1);
};


int EXPORT ANARACE::setProgramIsBuilt(int IP, int num)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].built=num;
	return(1);
};

int EXPORT ANARACE::setProgramLocation(int IP, int location)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].location=location;
	return(1);
}

int EXPORT ANARACE::setStatisticsFitness(int time,int fitness)
{
#ifdef _SCC_DEBUG
		if((time<0)||(time>MAX_TIME))
		{
				debug.toLog(0,"DEBUG: (ANARACE::setStatisticsFitness): Value time [%i] out of range.",time);
				return(0);
		}
		if((fitness<0)||(fitness>100000))
		{
				debug.toLog(0,"DEBUG: (ANARACE::setStatisticsFitness): Value fitness [%i] out of range.",fitness);
				return(0);
		}
#endif
		statistics[time].fitness=fitness;
		return(1);
}



int EXPORT ANARACE::getProgramForceCount(int IP, int unit)	   // how many units of the type at phaenoCode[s] do exist at that time?
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].forceCount[unit]);
};

int EXPORT ANARACE::getProgramAvailibleCount(int IP, int unit)	   // how many units of the type at phaenoCode[s] do exist at that time?
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].availibleCount[unit]);
};



int EXPORT ANARACE::setProgramFacility(int IP, int facility)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].facility=facility;
	return(1);
};

int EXPORT ANARACE::setProgramSuccessType(int IP, int type)
{
#ifdef _SCC_DEBUG
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
#endif
	program[IP].successType=type;
	return(1);
};


int EXPORT ANARACE::setProgramSuccessUnit(int IP, int unit)
{
#ifdef _SCC_DEBUG
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
#endif
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

int EXPORT ANARACE::getStatisticsNeedSupply(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsNeedSupply): Value time [%i] out of range.",time);
		return(0);
	}

	if((statistics[time].needSupply<0)||(statistics[time].needSupply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsNeedSupply): Variable not initialized [%i].",statistics[time].needSupply);
		return(0);
	}
#endif
	return(statistics[time].needSupply);
};

int EXPORT ANARACE::getStatisticsHaveSupply(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsHaveSupply): Value time [%i] out of range.",time);
		return(0);
	}

	if((statistics[time].haveSupply<0)||(statistics[time].haveSupply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsHaveSupply): Variable not initialized [%i].",statistics[time].haveSupply);
		return(0);
	}
#endif
	return(statistics[time].haveSupply);
};

int EXPORT ANARACE::getProgramIsBuilt(int IP)
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].built);
};

int EXPORT ANARACE::getProgramIsBuilt2(int IP)
{
	return(getProgramIsBuilt(IP));
};

/*int EXPORT ANARACE::getProgramIsConstant(int IP)
{
		if((IP<0)||(IP>=MAX_LENGTH))
		{
				debug.toLog(0,"DEBUG: (ANARACE::getProgramIsConstant): Value IP [%i] out of range.",IP);
				return(0);
		}
		if((program[IP].isConstant<0)||(program[IP].isConstant>1))
		{
				debug.toLog(0,"DEBUG: (ANARACE::getProgramIsConstant): Variable not initialized [%i].",program[IP].isConstant);
				return(0);
		}
	return(program[IP].isConstant);
};*/

int EXPORT ANARACE::getProgramIsGoal(int IP)
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].isGoal);
};


int EXPORT ANARACE::getProgramTime(int IP)
{
#ifdef _SCC_DEBUG
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
#endif
	return(program[IP].time);
};

int EXPORT ANARACE::getProgramTemp(int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramTemp): Value IP [%i] out of range.",IP);
		return(0);
	}
#endif
	return(program[IP].temp);
};

int EXPORT ANARACE::getStatisticsHaveMinerals(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsHaveMinerals): Value time [%i] out of range.",time);
		return(0);
	}
	if((statistics[time].mins<0)||(statistics[time].mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsHaveMinerals): Variable not initialized [%i].",statistics[time].mins);
		return(0);
	}
#endif
	return(statistics[time].mins);
};
int EXPORT ANARACE::getStatisticsHaveGas(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsHaveGas): Value time [%i] out of range.",time);
		return(0);
	}
	if((statistics[time].gas<0)||(statistics[time].gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsHaveGas): Variable statistics[%i].gas not initialized [%i].",time,statistics[time].gas);
		return(0);
	}
#endif
	return(statistics[time].gas);
};

int EXPORT ANARACE::getProgramLocation(int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramLocation): Value IP [%i] out of range.",IP);
		return(0);
	}

	if((program[IP].location<0)||(program[IP].location>MAX_LOCATIONS)) //todo: ga werte einfuegen
	{
		debug.toLog(0,"DEBUG: (ANARACE::getProgramLocation): Variable program[%i].location not initialized [%i].",IP,program[IP].location);
		return(0);
	}
#endif
	return(program[IP].location);
};

int EXPORT ANARACE::getStatisticsFitness(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsFitness): Value time [%i] out of range.",time);
		return(0);
	}
																		
	if((statistics[time].fitness<0)||(statistics[time].fitness>100000)) //todo: ga werte einfuegen
	{
		debug.toLog(0,"DEBUG: (ANARACE::getStatisticsFitness): Variable statistics[%i].fitness not initialized [%i].",time,statistics[time].fitness);
		return(0);
	}
#endif
	return(statistics[time].fitness);
};

int ANARACE::maximum(int unit)
{
/*	int max=0;
	int t=0;
	if((stats[getPlayer()->getGoal()->getRace()][unit].facility[0])&&(!getMap()->location[0].force[1][stats[getPlayer()->getGoal()->getRace()][unit].facility[0]])&&(stats[getPlayer()->getGoal()->getRace()][unit].facility[0]!=unit))
	{
		t=needTime(stats[getPlayer()->getGoal()->getRace()][unit].facility[0]);
		if(t>max) max=t;
		t=0;
	}
		if((stats[getPlayer()->getGoal()->getRace()][unit].facility[1])&&(!getMap()->location[0].force[1][stats[getPlayer()->getGoal()->getRace()][unit].facility[1]]))
		{
				t=needTime(stats[getPlayer()->getGoal()->getRace()][unit].facility[1]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[getPlayer()->getGoal()->getRace()][unit].facility[2])&&(!getMap()->location[0].force[1][stats[getPlayer()->getGoal()->getRace()][unit].facility[2]]))
		{
				t=needTime(stats[getPlayer()->getGoal()->getRace()][unit].facility[2]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[0])&&(!getMap()->location[0].force[1][stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[0]]))
		{
				t=needTime(stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[0]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[1])&&(!getMap()->location[0].force[1][stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[1]]))
		{
				t=needTime(stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[1]);
				if(t>max) max=t;
				t=0;
		}
		if((stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[2])&&(!getMap()->location[0].force[1][stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[2]]))
		{
				t=needTime(stats[getPlayer()->getGoal()->getRace()][unit].prerequisite[2]);
				if(t>max) max=t;
				t=0;
		}
	return(max);*/
	return(0);
};

int ANARACE::needTime(int unit)
{
//	if(!getMap()->location[0].force[1][unit])
//		return(stats[getPlayer()->getGoal()->getRace()][unit].BT+maximum(unit)); //eigene Bauzeit + Bauzeit der Prerequisites/Facilities
//	else return(0);
	return(0);
};

#if 0
int EXPORT ANARACE::calculateFitness() // Fuer den Uebersichtsgraphen unten rechts
{

// TODO mit RACE abgleichen!
	int tpF=0;
//	int bonus[MAX_LOCATIONS][UNIT_TYPE_COUNT]; // temporary data to check whether a bonus is already given (only applies if force > goal)
//TODO: Nicht alle Einheiten am Ort? => Ort egal sein lassen aber zur Zeit hinzuzaehlen
	// Nicht alle Einheiten ueberhaupt gebaut UND nicht alle am Ort => nur viertel Bonus fuer Einheiten die nicht am Ort sind
	int otpF=0;
	for(int i=MAX_GOALS;i--;)
		if(getPlayer()->getGoal()->goal[i].count>0)
		{
			otpF=tpF;
			if( /*((getPlayer()->getGoal()->goal[i].location==0)&&(getPlayer()->getGoal()->goal[i].count>getLocationForce(0,getPlayer()->getGoal()->goal[i].unit))) || ( (getPlayer()->getGoal()->goal[i].location>0)&&*/(getPlayer()->getGoal()->goal[i].count>getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit)))
			{
					//total points: (Am Ort befindliche Einheiten + (Summe aller Locations(100-distance)/100)) / Goalcount
					//TODO: Absteigen und markieren der benutzten wbfs! Also zuerst die eigentliche location abchecken, dann nach links und rechts die naehesten hinzuziehen					 //evtl direkt von den locations die wbfs erstmal abziehen und am Schluss nochmal alle goals durchlaufen und den Rest verteilen!					 
				int sumup=0;
				int bon=0;
				if(getPlayer()->getGoal()->goal[i].location==0)
					sumup=getLocationForce(0,getPlayer()->getGoal()->goal[i].unit)*100;
				else
				{
					bon=getPlayer()->getGoal()->goal[i].count;
					int j=1;
					int loc;
					while((j<MAX_LOCATIONS)&&(bon>getLocationForce(loc=pMap->getNearestLocation(getPlayer()->getGoal()->goal[i].location,j),getPlayer()->getGoal()->goal[i].unit)))						 
					{
						sumup+=getLocationForce(loc,getPlayer()->getGoal()->goal[i].unit)*(100-pMap->getDistance(loc,getPlayer()->getGoal()->goal[i].location));							 
						bon-=getLocationForce(loc,getPlayer()->getGoal()->goal[i].unit);						  
						j++;
					}
						//rest
					if(j<MAX_LOCATIONS)
						sumup+=bon*(100-pMap->getDistance(loc,getPlayer()->getGoal()->goal[i].location));					 
				}
					//TODO: Hier gibts Probleme wenn mehrere goals gleicher Units an unterschiedlichen Orten existieren...
					// evtl funktionsglobales bonus System wie bei den '@' in scc.cpp einfuegen
					// bissl komplex da mans ja den einzelnen goals verteilen muss...
				if((getPlayer()->getGoal()->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->getGoal()->goal[i].time))
//				{
//					if(getFinalTime(i)>0)
						tpF+=(getPlayer()->getGoal()->goal[i].time*sumup)/(getPlayer()->getGoal()->goal[i].count*getFinalTime(i));
//					else
//						tpF+=(getPlayer()->getGoal()->goal[i].time*sumup)/(getPlayer()->getGoal()->goal[i].count*ga->maxTime);
//				}
				else
					tpF+=sumup/getPlayer()->getGoal()->goal[i].count;
			} //END force < goal
			else 
//if( ((getPlayer()->getGoal()->goal[i].location==0)&&(getPlayer()->getGoal()->goal[i].count<=getLocationForce(0,getPlayer()->getGoal()->goal[i].unit))) || ( (getPlayer()->getGoal()->goal[i].location>0)&&(getPlayer()->getGoal()->goal[i].count<=getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit))) )
		//force >= goal
			{
				if((getPlayer()->getGoal()->goal[i].time>0)&&(getFinalTime(i)>getPlayer()->getGoal()->goal[i].time))
					tpF+=(getPlayer()->getGoal()->goal[i].time*100/getFinalTime(i));
				else tpF+=100;
// does not work yet, if this is uncommented, sFitness occasionally jumps to -1222000 or something like that... :/
// include the final location maybe...

			}
//			fitnessCode[i]=tpF-otpF;
		}
// TODO: Check for very small 'goal.time' values, probably in scc.cpp!!
																				
//Bonus: Sind noch Plaetze offen?
// TODO: Problem: wenn ich das reintu gibts so tolle Zacken auf der fitnesskurve...
/*		for(i=MAX_LOCATIONS;i--;)
			   for(j=UNIT_TYPE_COUNT;j--;)
				   bonus[i][j]=0;
																				
		for(i=MAX_GOALS;i--;)
			if(getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit)<getPlayer()->getGoal()->goal[i].count)
				bonus[getPlayer()->getGoal()->goal[i].location][getPlayer()->getGoal()->goal[i].unit]+=getPlayer()->getGoal()->goal[i].count-getLocationForce(getPlayer()->getGoal()->goal[i].location,getPlayer()->getGoal()->goal[i].unit);		 
		for(i=MAX_BUILDINGS;i--;)
			if((getRemainingBuildTime(i)>0)&&(bonus[getType(i)][getLocation(i)]>0))
			{			 //erstmal ohne Zeit...
				tpF+=((getRemainingBuildTime(i)*100)/((getLocationForce(getLocation(i),getType(i))+bonus[getType(i)][getLocation(i)])*pStats[getType(i)].BT));
																				
				if((getPlayer()->getGoal()->goal[getType(i)].time>0)&&(getLocationForce(getLocation(i),getType(i))==0))
					tpF+=(getRemainingBuildTime(i)*100*getPlayer()->getGoal()->goal[getType(i)].time*getLocationForce(0,i))/(getPlayer()->getGoal()->goal[getType(i)].count*pStats[getType(i)].BT*ga->maxTime);
				else					 tpF+=((getRemainingBuildTime(i)*100)/(getPlayer()->getGoal()->goal[getType(i)].count*pStats[getType(i)].BT));
				bonus[getLocation(i)][getType(i)]--;
			}*/
/*	if(ready)
	{
		int totalBT=0;
		j=0;
		for(i=0;i<UNIT_TYPE_COUNT;i++)
		if(getPlayer()->getGoal()->allGoal[i])
		{
			j=needTime(i);
			if(j>totalBT)
				totalBT=j;
		}
		if(totalBT==(ga->maxTime-getTimer()))
			timePercentage=100;
		else 
			timePercentage=100*totalBT/(ga->maxTime-getTimer());
	}	
	else*/ timePercentage=0;
	
   int count=0;
        for(int i=0;i<MAX_GOALS;i++)
            if(getPlayer()->getGoal()->goal[i].count>0)
                count++;
        goalPercentage=0;
        if(count>0)
            goalPercentage=getCurrentpFitness()/count;
        else goalPercentage=100;
	return(tpF);
}
#endif

int EXPORT ANARACE::getTimePercentage()
{
	return(timePercentage);
};

int EXPORT ANARACE::getGoalPercentage()
{
	return(goalPercentage);
};

void EXPORT ANARACE::countForce()
{
	totalUnitForce = 0;
    maxUnitForce = 4;
    for (int i = 0; i <= GAS_SCV; i++)
    {
        if (getLocationForce(0, i) > maxUnitForce)
            maxUnitForce = getLocationForce(0, i);
        if (getPlayer()->getGoal()->allGoal[i] > maxUnitForce)
            maxUnitForce = getPlayer()->getGoal()->allGoal[i];
        totalUnitForce += getLocationForce(0, i);;
    }
};


int EXPORT ANARACE::calculateStep()
{
//ZERG: CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
	//TODO: Fehler hier, getMaxSupply - getSupply kann -1 werden!
	// supply war 11, maxsupply war 10 :/
	setStatisticsNeedSupply(getTimer(),getMaxSupply()-getSupply());
	setStatisticsHaveSupply(getTimer(),getMaxSupply());
	setStatisticsHaveMinerals(getTimer(),getMins());
	setStatisticsHaveGas(getTimer(),getGas());
	setStatisticsFitness(getTimer(),calculatePrimaryFitness(ready));

	if((!getTimer())||(ready=calculateReady())||(!getIP())||((getPlayer()->getGoal()->bestTime*4<3*ga->maxTime)&&(4*getTimer()<3*getPlayer()->getGoal()->bestTime))) //TODO calculateReady optimieren
	{
		setLength(ga->maxLength-getIP());
		if(!ready) setTimer(0);
		for(int i=0;i<MAX_LENGTH;i++)
			phaenoCode[i]=getPlayer()->getGoal()->toPhaeno(Code[i]);
		if(getPlayer()->getGoal()->getMode()==0)
			setCurrentpFitness(calculatePrimaryFitness(ready));
		buildingList.Clear();
		countForce();
		return(1);
	}

	int ok=1;
	while((ok)&&(getIP()))
	{
		neededMinerals=99999;
		neededGas=99999;
		ok=buildGene(getPlayer()->getGoal()->toPhaeno(Code[getIP()]));
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
				setProgramForceCount(getIP(),i,getLocationForce(0,i));
				setProgramAvailibleCount(getIP(),i,getLocationAvailible(0,i));
			}

//TODO Warum addiert er das hier und setzt es nicht einfach??

//TODO ueber getPlayer etc. den PhaenoCode rauszufinden erscheint mir etwas umstaendlich... evtl in "program" integrieren...
	
			setTimeOut(ga->maxTimeOut);
			setIP(getIP()-1);
		}
	}

	int t=getTimer();
	BNODE *node=buildingList.GetHead();
	Building* build=0;
    if(node)
    {
		build=node->GetData();
//        int buildingRemaining=min_element(buildingList.begin(),buildingList.end()).second.getRemainingBuildTime();
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
			setProgramFacility(build->getIP(),build->getFacility());
			int drinne=1;
			for(int i=0;i<3;i++)
				drinne&=stat->facility[i]!=build->getFacility();
			switch(stat->facilityType)
			{
				case NO_FACILITY:break;
				case IS_LOST:
					if(build->getFacility())
					{
						addLocationForce(build->getLocation(),build->getFacility(),-1);
//						setProgramFacility(build->getIP(),build->getFacility());
						//availible was already taken account when starting the building
					}
					if(stat->facility2)
						addLocationForce(build->getLocation(),stat->facility2,-1);
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
						addLocationForce(build->getLocation(),stat->facility2,-1);
					break;
				case NEEDED_UNTIL_COMPLETE_IS_LOST:
					if(build->getFacility())
					{
						addLocationAvailible(build->getLocation(),build->getFacility(),1);
//						setProgramFacility(build->getIP(),build->getFacility());
						setSupply(getSupply()+pStats[build->getFacility()].supply); //<- ?

					}
					if(stat->facility2)
						addLocationForce(0/*build->getLocation()*/,stat->facility2,-1);
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
			}

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
			addLocationForce(build->getLocation(),build->getType(),build->getUnitCount());
			addLocationAvailible(build->getLocation(),build->getType(),build->getUnitCount());

			if(build->getType()==REFINERY) 
			{
				addMapLocationForce(0,build->getLocation(),VESPENE_GEYSIR,-1);
				adjustGasHarvest(build->getLocation());
			}
			else
			if((build->getType()==COMMAND_CENTER)&&(!getLocationForce(build->getLocation(),COMMAND_CENTER)))
			{
				adjustMineralHarvest(build->getLocation());
				adjustGasHarvest(build->getLocation());
			}
 else if((build->getType()==LARVA)&&(getPlayer()->getGoal()->getRace()==ZERG))
            {
                if(!buildGene(LARVA))
                    larvaInProduction[build->getLocation()]--;
//                      if(
//TODO Was wenn Gebaeude zerstoert? etc.? eigentlich is das hier net noetig...
//                                  (((getLocationForce(tloc, HATCHERY)+getLocationForce(tloc, LAIR)+getLocationForce(tloc, HIVE))*3>
//                                    (larvaInProduction[tloc]+getLocationForce(tloc, LARVA)))&&
//                                   ((getLocationForce(tloc, HATCHERY)+getLocationForce(tloc, LAIR)+getLocationForce(tloc, HIVE)<larvaInProduction[tloc])))) // => zuwenig Larven da!
//                          {
//                              if(buildGene(LARVA))
//                                  larvaInProduction[tloc]++;
//                          }
//                      };
//                  };
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
					//evtl noch location==0 als 'egal wo' einfuehren
			lastcounter++;

			//ANA~
			setProgramIsBuilt(build->getIP(),1);
			setProgramLocation(build->getIP(),build->getLocation());
			setProgramBT(build->getIP(),build->getTotalBuildTime()); //~~~
//			IP zeugs checken... length is immer 2 :/	 ??

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
/*            map<int, Building>::iterator temp=buildNode;
            temp++;
            buildingList.erase(buildNode);
            buildNode=temp;*/
		} // end while(getremainingbuildorder) ~|~
		if(node)
			build=(Building*)node->GetData();
//		else buildNode++;
	} //end while
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

//	if(unit==1)
//		successType=0; //?

	if(unit<=EXTRACTOR+1)
	{
		if((stat->prerequisite[0]>0)&&(getLocationForce(0,stat->prerequisite[0])==0))
		{
			successUnit=stat->prerequisite[0];
//			successLocation=0;
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else		
		if((stat->prerequisite[1]>0)&&(getLocationForce(0,stat->prerequisite[1])==0))
		{
			successUnit=stat->prerequisite[1];
//			successLocation=0;
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else 
		if((stat->prerequisite[2]>0)&&(getLocationForce(0,stat->prerequisite[2])==0))
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
		if((getSupply()<stat->supply)&&(stat->supply>0))
		{
			successUnit=0;
//			  successLocation=0;
			successType=SUPPLY_WAS_SATISFIED;
		}
		else
		if(getMins()<stat->mins+stat->upgrade_cost*getLocationForce(0,unit))
		{
			successUnit=0;
//			successLocation=0;
			successType=ENOUGH_MINERALS_WERE_GATHERED;
			if(neededMinerals>stat->mins+stat->upgrade_cost*getLocationForce(0,unit)-getMins())
				neededMinerals=stat->mins+stat->upgrade_cost*getLocationForce(0,unit)-getMins();
//Fehler is wahrscheinlich hier irgendwoe...

		}
		else
		if(getGas()<stat->gas+stat->upgrade_cost*getLocationForce(0,unit))
		{
			successUnit=0;
//			successLocation=0;
			successType=ENOUGH_GAS_WAS_GATHERED;
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

//			if((stat->facility2==0)||(getLocationAvailible(tloc,stat->facility2)>0))
			for(fac=3;fac--;)
				if( ((stat->facility[fac]>0)&&(getLocationAvailible(tloc,stat->facility[fac])>((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)))) || ((stat->facility[fac]==0)&&(fac==0))) 
				{
					ok=1;
					break;
				}
		
			j=1;
			if(!ok)
				while(j<MAX_LOCATIONS)
				{
					ttloc=pMap->getNearestLocation(tloc,j);
//					if((stat->facility2==0)||(getLocationAvailible(ttloc,stat->facility2)>0))
//					{
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
//				bewegliche Sachen ueberdenken...
//					evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//					auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//		Phagen ueber Phagen...

			if((ok)&&(unit==REFINERY))
			{
				if(getMapLocationAvailible(0,tloc,VESPENE_GEYSIR)<=0)
					ok=0;
				else
					addMapLocationAvailible(0,tloc,VESPENE_GEYSIR,-1);
			};

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
					
				/*Building build;
				build.setOnTheRun(0);
				build.setFacility(stat->facility[fac]);
				build.setLocation(tloc);
				build.setUnitCount(1);
				build.setRemainingBuildTime(stat->BT/*+3200*(stat->facility2==unit)*///); //~~ hack :/ TODO ???????????? SINN?
//				build.setTotalBuildTime(build.getRemainingBuildTime());
//				build.setType(unit);

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

					
				setMins(getMins()-(stat->mins+stat->upgrade_cost*getLocationForce(0,unit)));
				setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationForce(0,unit)));
				if((stat->supply>0)||((pStats[stat->facility[0]].supply<0)&&(stat->facilityType==IS_LOST))) 
					setSupply(getSupply()-stat->supply);
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
			if((lastcounter>0)&&(getLocationAvailible(last[lastcounter-1].location,last[lastcounter-1].unit)>0)&&(pStats[last[lastcounter-1].unit].speed>0))
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
					setRemainingBuildTime(nr,pMap->getDistance(last[lastcounter].location,count)*100/pStats[last[lastcounter].unit].speed);
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
					addLocationForce(last[lastcounter].location,getType(nr),-getUnitCount(nr));
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


void EXPORT ANARACE::setPhaenoCode(int IP, int num)
{
#ifdef _SCC_DEBUG
		if((IP<0)||(IP>=MAX_LENGTH))
		{
				debug.toLog(0,"DEBUG: (ANARACE::setPhaenoCode): Value IP [%i] out of range.",IP);
				return;
		}
		if((num<0)||(num>=UNIT_TYPE_COUNT))
		{
				debug.toLog(0,"DEBUG: (ANARACE::setPhaenoCode): Value num [%i] out of range.",num);
				return;
		}
#endif
	phaenoCode[IP]=num;
};

int EXPORT ANARACE::getPhaenoCode(int IP)
{
#ifdef _SCC_DEBUG
		if((IP<0)||(IP>=MAX_LENGTH))
		{
				debug.toLog(0,"DEBUG: (ANARACE::getPhaenoCode): Value IP [%i] out of range.",IP);
				return(0);
		}
		if((program[IP].isGoal<0)||(program[IP].isGoal>1))
		{
				debug.toLog(0,"DEBUG: (ANARACE::getPhaenoCode): Variable not initialized [%i].",phaenoCode[IP]);
				return(0);
		}
#endif
	return(phaenoCode[IP]);
};


int EXPORT ANARACE::getCurrentpFitness()
{
	return(currentpFitness);
};

void ANARACE::setCurrentpFitness(int num)
{
	currentpFitness=num;
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
		setProgramIsBuilt(i,0);
		setProgramTime(i,MAX_TIME);
		for(int j=0;j<UNIT_TYPE_COUNT;j++)
		{
			setProgramAvailibleCount(i,j,0);
			setProgramForceCount(i,j,0);
		}
		//program[i].temp=0;
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
	setTimer(ga->maxTime-getPlayer()->getTimer());
	setSupply(getPlayer()->getSupply());
	setMaxSupply(getPlayer()->getMaxSupply());

	for(int i=0;i<4;i++) //TODO
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
	setCalculated(0);
	ready=0;
}


int EXPORT ANARACE::getUnchangedGenerations()
{
#ifdef _SCC_DEBUG	
	if((unchangedGenerations<0)||(unchangedGenerations>MAX_GENERATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getUnchangedGenerations): Variable unchangedGenerations not initialized [%i].",unchangedGenerations);
		return(0);
	}
#endif
	return(unchangedGenerations);
};

int EXPORT ANARACE::getRun()
{
#ifdef _SCC_DEBUG	
	if((currentRun<0)||(currentRun>MAX_RUNS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getRun): Variable currentRun not initialized [%i].",currentRun);
		return(0);
	}
#endif
	return(currentRun);
};

int EXPORT ANARACE::getGeneration()
{
#ifdef _SCC_DEBUG	
	if((generation<0)||(generation>MAX_GENERATIONS)) //ga->maxgeneration?
	{
		debug.toLog(0,"DEBUG: (ANARACE::getGeneration): Variable generation not initialized [%i].",generation);
		return(0);
	}
#endif
	return(generation);
};

int EXPORT ANARACE::getMaxpFitness()
{
#ifdef _SCC_DEBUG	
	if((maxpFitness<0)||(maxpFitness>50000))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getMaxpFitness): Variable maxpFitness not initialized [%i].",maxpFitness);
		return(0);
	}
#endif
	return(maxpFitness);
};

int EXPORT ANARACE::getMaxsFitness()
{
#ifdef _SCC_DEBUG	
	if((maxsFitness<0)||(maxsFitness>MAX_MINS+MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getMaxsFitness): Variable maxsFitness not initialized [%i].",maxsFitness);
		return(0);
	}
#endif
	return(maxsFitness);
};

int EXPORT EXPORT ANARACE::getMaxtFitness()
{
#ifdef _SCC_DEBUG	
	if((maxtFitness<0)||(maxtFitness>MAX_TFITNESS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::getMaxtFitness): Variable maxtFitness not initialized [%i].",maxtFitness);
		return(0);
	}
#endif
	return(maxtFitness);
};

int EXPORT ANARACE::setUnchangedGenerations(int num)
{
#ifdef _SCC_DEBUG	
//	if((num<0)||(num>ga->maxGenerations))
	if((num<0)||(num>MAX_GENERATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setUnchangedGenerations): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	unchangedGenerations=num;
	return(1);
};

int EXPORT ANARACE::setRun(int run)
{
#ifdef _SCC_DEBUG
	if((run<0)||(run>MAX_RUNS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setRun): Value run [%i] out of range.",run);
		return(0);
	}
#endif
	currentRun=run;
	return(1);
};

int EXPORT ANARACE::setGeneration(int num)
{
#ifdef _SCC_DEBUG
//	if((num<0)||(num>ga->maxGenerations))
	if((num<0)||(num>MAX_GENERATIONS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setGeneration): Value [%i] out of range.",num);
		return(0);
	}
#endif
	generation=num;
	return(1);
};

int EXPORT ANARACE::setMaxpFitness(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>50000))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setMaxpFitness): Value [%i] out of range.",num);
		return(0);
	}
#endif
	maxpFitness=num;
	return(1);
};

int EXPORT ANARACE::setMaxsFitness(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_MINS+MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setMaxsFitness): Value [%i] out of range.",num);
		return(0);
	}
#endif
	maxsFitness=num;
	return(1);
};

int EXPORT ANARACE::setMaxtFitness(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_TFITNESS))
	{
		debug.toLog(0,"DEBUG: (ANARACE::setMaxtFitness): Value [%i] out of range.",num);
		return(0);
	}
#endif
	maxtFitness=num;
	return(1);
};

void EXPORT ANARACE::analyzeBuildOrder()
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
			tGoal[i]+=getPlayer()->getGoal()->globalGoal[j][i]-getMap()->getLocationForce(j,getPlayerNum(),i);
	}

	for(int i=0;i<MAX_LENGTH;i++)
		if(tGoal[phaenoCode[i]]>0) //~~~~~~ location=0?
		{
			tGoal[phaenoCode[i]]--;
			setProgramIsGoal(i,1);
		}
	else setProgramIsGoal(i,0);
};




int EXPORT ANARACE::successType;
int EXPORT ANARACE::successUnit;
//int EXPORT ANARACE::successLocation;
MAP_LOCATION EXPORT ANARACE::backupLoc[MAX_PLAYER][MAX_LOCATIONS];

