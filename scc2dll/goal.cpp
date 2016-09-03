#include "goal.h"
#include <string.h>
#include "debug.h"

void EXPORT GOAL_ENTRY::copy(GOAL_ENTRY* goal)
{
#ifdef _SCC_DEBUG
    if(!goal)
    {
        debug.toLog(0,"DEBUG: (GOAL_ENTRY:copy): Variable goal not initialized [%i].",goal);
        return;
    }
#endif
//	race=goal->getRace();
	maxBuildTypes=goal->getMaxBuildTypes();
	initialized=goal->getInitialized();
	raceInitialized=1; //TODO!
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		allGoal[i]=goal->allGoal[i];
		isBuildable[i]=goal->isBuildable[i];
		isVariable[i]=goal->isVariable[i];
		for(int j=MAX_LOCATIONS;j--;)
			globalGoal[j][i]=goal->globalGoal[j][i];
		genoToPhaenotype[i]=goal->genoToPhaenotype[i];
		phaenoToGenotype[i]=goal->phaenoToGenotype[i];
	};
	pStats=goal->getpStats();
	for(int i=MAX_GOALS;i--;)
	{
		this->goal[i].unit=goal->goal[i].unit;
		this->goal[i].time=goal->goal[i].time;
		this->goal[i].count=goal->goal[i].count;
		this->goal[i].location=goal->goal[i].location;
	};
};


int EXPORT GOAL_ENTRY::isChanged()
{
#ifdef _SCC_DEBUG
		if((changed<0)||(changed>1))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::isChanged): Variable changed not initialized [%i].",changed);
				return(0);
		}
#endif
	return(changed);
};

void EXPORT GOAL_ENTRY::changeAccepted()
{
	adjustGoals(1);
	bestTime=0;
	changed=0;
};

/*const char* EXPORT GOAL_ENTRY::getName()
{
#ifdef _SCC_DEBUG
		if(name==NULL)
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::getName): Variable name not initialized [%i].",name);
				return(0);
		}
#endif
	return name;
};*/

int EXPORT GOAL_ENTRY::getRace()
{
#ifdef _SCC_DEBUG
	if((race<MIN_RACE)||(race>MAX_RACE))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::getRace): Variable race not initialized [%i].",race);
		return(0);
	}
#endif
	return race;
};

/*int EXPORT GOAL_ENTRY::setName(const char* line)
{
#ifdef _SCC_DEBUG
		if(line==NULL)
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::setName): Variable line not initialized [%i].",line);
				return(0);
		}
#endif
	strcpy(name,line);
	return(1);
};*/

int EXPORT GOAL_ENTRY::isRaceInitialized()
{
#ifdef _SCC_DEBUG
		if((raceInitialized<0)||(raceInitialized>1))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::isRaceInitialized): Variable raceInitialized not initialized [%i].",raceInitialized);
				return(0);
		}
#endif
	if(raceInitialized==1)
		return(1);
	else return(0);
};


int EXPORT GOAL_ENTRY::setRace(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>2))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::setRace): Value [%i] out of range.",num);
		return(0);
	}
#endif
	resetData();	
	pStats=&(stats[race=num][0]);
	raceInitialized=1;
	changed=1;
	return(1);
};

const UNIT_STATISTICS* EXPORT GOAL_ENTRY::getpStats()
{
#ifdef _SCC_DEBUG
		if(pStats==NULL)
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::getpStats): Variable pStats not initialized [%i].",pStats);
				return(0);
		}
#endif

	return(pStats);
};


int EXPORT GOAL_ENTRY::adjustGoals(int allowGoalAdaption, UNITS* units)
{
// if free-mode  set all isbuildable to 1
// else ...
	int oldGoal[UNIT_TYPE_COUNT]; //goals we got from the user which we MAY NOT ignore
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
		oldGoal[i]=allGoal[i];
//	fill(oldGoal)
//TODO: Reset hier rein!
	
//		isBuildable[MOVE_ONE_1_FORWARD]=1;isVariable[MOVE_ONE_1_FORWARD]=1;
  //	  isBuildable[MOVE_ONE_3_FORWARD]=1;isVariable[MOVE_ONE_3_FORWARD]=1;
	//	isBuildable[MOVE_ONE_1_BACKWARD]=1;isVariable[MOVE_ONE_1_BACKWARD]=1;
											                                                                                                                
		  //isBuildable[INTRON]=1; // :-)
	if(units)
		for(int i=GAS_SCV;i--;)
			if((units->getTotal(i))&&(allGoal[i]<units->getTotal(i)))
				addGoal(i, units->getTotal(i)-allGoal[i], 0, 0);
	//TODO addgoal evtl machen hier!

	for(int j=6;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(int i=UNIT_TYPE_COUNT;i--;)
			if((allGoal[i])||(isBuildable[i]))
			{
				if((i==GAS_SCV)&&(allGoal[REFINERY]==0))
					addGoal(REFINERY,1,0,0); //~~
				isBuildable[i]=1;
				//gather all prerequisites and mark them as goals
				for(int k=0;k<3;k++)
					if((pStats[i].prerequisite[k]>0)&&(allGoal[pStats[i].prerequisite[k]]==0))
						addGoal(pStats[i].prerequisite[k],1,0,0);
				if((pStats[i].facility2>0)&&(allGoal[pStats[i].facility2]==0)&&(pStats[i].facility_type!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[i].facility_type!=NEEDED_UNTIL_COMPLETE_IS_LOST))
				{
					isVariable[pStats[i].facility2]=1;
					if(pStats[i].facility_type!=IS_LOST) //do not set those morph-facilities as goals...
						addGoal(pStats[i].facility2,1,0,0);
					else isBuildable[pStats[i].facility2]=1;// ... but make them buildable :)
				}
				if((pStats[i].facility[0]>0)&&(allGoal[pStats[i].facility[0]]==0)&&((pStats[i].facility[1]==0)||(allGoal[pStats[i].facility[1]]==0))&&((pStats[i].facility[2]==0)||(allGoal[pStats[i].facility[2]]==0)))
// only facility[0] is important
				{
					isVariable[pStats[i].facility[0]]=1;
					if((pStats[i].facility_type==NEEDED_ONCE)||(pStats[i].facility_type==NEEDED_UNTIL_COMPLETE)||(pStats[i].facility_type==NEEDED_ALWAYS))
						addGoal(pStats[i].facility[0],1,0,0);
					else isBuildable[pStats[i].facility[0]]=1; // ... same as above...
				}
			}

//	now recheck whether we got too many goals
//	>_<
//	alle durchlaufen
//	jeweils alle facilities von rechts nach links anguggn
//	sobald eins gefunden das Teil der goals ist, alle weiter links streichen (aber buildable lassen)
	for(int i=UNIT_TYPE_COUNT;i--;)
		if((allGoal[i]>0)||(isBuildable[i]>0))
		{
			for(int j=3;j--;)
				if(allGoal[pStats[i].facility[j]]>0)
				{
					for(int k=0;k<j;k++)
					{
						if(allowGoalAdaption) allGoal[pStats[i].facility[k]]=0;//~~
						else allGoal[pStats[i].facility[k]]=oldGoal[pStats[i].facility[k]];
					}
					j=0;
				}
			isBuildable[pStats[i].create]=0; //~~
			isVariable[pStats[i].create]=0;
		}
	
		long Need_Gas=0;
		for(int i=UNIT_TYPE_COUNT;i--;)
				Need_Gas+=(allGoal[i]*pStats[i].gas);
		if(Need_Gas>0)
		{
				isBuildable[REFINERY]=1;
				if(allGoal[REFINERY]==0)
					   addGoal(REFINERY,1,0,0); //ASSIMILATOR == EXTRACTOR == REFINERY
				isBuildable[GAS_SCV]=1; //ONE_MINERAL_SCV... = ONE_MINERAL_PROBE... = ONE_MINERAL_DRONE...
				isVariable[GAS_SCV]=1;
		}; 

	switch(getRace())
	{
		case TERRA:isBuildable[SUPPLY_DEPOT]=1;isVariable[SUPPLY_DEPOT]=1;break;
		case PROTOSS:isBuildable[PYLON]=1;isVariable[PYLON]=1;break;
		case ZERG:isBuildable[OVERLORD]=1;isVariable[OVERLORD]=1;isBuildable[LARVA]=0;isVariable[LARVA]=0;break;
		default:break;
	}

	for(int i=UNIT_TYPE_COUNT;i--;)
		if((isBuildable[i]==1)&&(phaenoToGenotype[i]==-1)) // a goal && not set yet
		{
			genoToPhaenotype[maxBuildTypes]=i;
			phaenoToGenotype[i]=maxBuildTypes;
			maxBuildTypes++;
		};

	// hack for units who cannot be built but needs to be translated by phaenoToGenotype! (for the forcewindow)
	if(getRace()==ZERG)
	{
		genoToPhaenotype[maxBuildTypes]=LARVA;
		phaenoToGenotype[LARVA]=maxBuildTypes;
		// further units need to temporarily increment maxBuildTypes
	}

	//TODO: ueberlegen ob nicht einfach Move+ und Move- reichen...
	return(1);
};


int EXPORT GOAL_ENTRY::isGoal(int unit)
{
#ifdef _SCC_DEBUG
		if((unit<0)||(unit>=UNIT_TYPE_COUNT))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::isGoal): Value unit [%i] out of range.",unit);
				return(0);
		}

		if((allGoal[unit]<0)||(allGoal[unit]>=UNIT_TYPE_COUNT))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::isGoal): Variable allGoal[%i] not initialized [%i].",unit,allGoal[unit]);
				return(0);
		}
#endif
	return(allGoal[unit]>0);
};

int EXPORT GOAL_ENTRY::addGoal(int unit, int count, int time, int location)
{
#ifdef _SCC_DEBUG
		if((unit<=0)||(unit>=UNIT_TYPE_COUNT))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::addGoal): Value unit [%i] out of range.",unit);
				return(0);
		}

		if((count+globalGoal[location][unit]<0)||(count>MAX_SUPPLY)) //~~
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::addGoal): Value count [%i] out of range.",count);
				return(0);
		}
		if((time<0)||(time>=MAX_TIME))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::addGoal): Value time [%i] out of range.",time);
				return(0);
		}

		if((location<0)||(location>=MAX_LOCATIONS))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::addGoal): Value location [%i] out of range.",location);
				return(0);
		}
#endif

//TODO goal loeschen einbauen
	allGoal[unit]+=count;
	if(allGoal[unit]==0)
	{
		isBuildable[unit]=0;
		isVariable[unit]=0;
	}

	globalGoal[location][unit]+=count;

	int i=0;
	for(i=0;i<goalCount;i++)
		if((goal[i].unit==unit)&&(goal[i].time==time)&&(goal[i].location==location))
		{
	//TODO goal loeschen..
			goal[i].count+=count;
			i=goalCount+1;
		}
	if(i<goalCount+1)
	{
		goal[goalCount].unit=unit;
		goal[goalCount].time=time;
		goal[goalCount].location=location;
		goal[goalCount].count=count;
		goalCount++;
	}
	changed=1;
	return(1);
};

int EXPORT GOAL_ENTRY::getMaxBuildTypes()
{
#ifdef _SCC_DEBUG
	if((maxBuildTypes<0)||(maxBuildTypes>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::getMaxBuildTypes): Variable not initialized [%i].",maxBuildTypes);
		return(0);
	}
#endif
	return(maxBuildTypes);
};


int EXPORT GOAL_ENTRY::getInitialized()
{
#ifdef _SCC_DEBUG
		if((initialized<0)||(initialized>1))
		{
				debug.toLog(0,"DEBUG: (GOAL_ENTRY::getInitialized): Variable initialized not initialized [%i].",initialized);
				return(0);
		}
#endif
	return(initialized);
};

int EXPORT GOAL_ENTRY::toGeno(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::toGeno): Value [%i] out of range.",num);
		return(0);
	}
	if((phaenoToGenotype[num]<0)||(phaenoToGenotype[num]>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::toGeno): Variable not initialized [%i].",phaenoToGenotype[num]);
		return(0);
	}
#endif
	if(num==FACTORY)
	{
		int i=1;
	}
	return(phaenoToGenotype[num]);
}

int EXPORT GOAL_ENTRY::toPhaeno(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::toPhaeno): Value [%i] out of range.",num);
		return(0);
	}
	if((genoToPhaenotype[num]<0)||(genoToPhaenotype[num]>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::toPhaeno): Variable not initialized [%i].",genoToPhaenotype[num]);
		return(0);
	}
#endif
	return(genoToPhaenotype[num]);
}

void GOAL_ENTRY::resetData()
{
	raceInitialized=0;
	goalCount=0;
	maxBuildTypes=0;
	for(int i=0;i<MAX_GOALS;i++)
	{
		goal[i].count=0;
		goal[i].time=0;
		goal[i].unit=0;
		goal[i].location=0;
	}
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		genoToPhaenotype[i]=-1;
		phaenoToGenotype[i]=-1;
		allGoal[i]=0;
		isVariable[i]=0;
		isBuildable[i]=0;
		for(int j=0;j<MAX_LOCATIONS;j++)
			globalGoal[j][i]=0;
	}
	initialized=1;
	bestTime=0;
};

GOAL_ENTRY::GOAL_ENTRY()
{
	resetData();
};

GOAL_ENTRY::~GOAL_ENTRY()
{
};

