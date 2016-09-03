#include "goal.h"
#include <string.h>
#include "debug.h"

int EXPORT GOAL_ENTRY::isChanged()
{
	return(changed);
};

void EXPORT GOAL_ENTRY::changeAccepted()
{
	changed=0;
};

const char* EXPORT GOAL_ENTRY::getName()
{
	return name;
};

int EXPORT GOAL_ENTRY::getRace()
{
	if((race<MIN_RACE)||(race>MAX_RACE))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::getRace): Variable not initialized [%i].",race);
		return(0);
	}
	return race;
};

int EXPORT GOAL_ENTRY::setName(const char* line)
{
	strcpy(name,line);
	return(1);
};

int EXPORT GOAL_ENTRY::isRaceInitialized()
{
	if(raceInitialized==1)
		return(1);
	else return(0);
};


int EXPORT GOAL_ENTRY::setRace(int num)
{
	if((num<0)||(num>2))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::setRace): Value [%i] out of range.",num);
		return(0);
	}
	pStats=&(stats[race=num][0]);
	raceInitialized=1;
	changed=1;
	return(1);
};

const UNIT_STATISTICS* EXPORT GOAL_ENTRY::getpStats()
{
	return(pStats);
};


int EXPORT GOAL_ENTRY::adjustGoals(int allowGoalAdaption)
{
	int i,j,k;
	int oldGoal[UNIT_TYPE_COUNT]; //goals we got from the user which we MAY NOT ignore
	for(i=0;i<UNIT_TYPE_COUNT;i++)
		oldGoal[i]=allGoal[i];

//TODO: Reset hier rein!
	
//        isBuildable[MOVE_ONE_1_FORWARD]=1;isVariable[MOVE_ONE_1_FORWARD]=1;
  //      isBuildable[MOVE_ONE_3_FORWARD]=1;isVariable[MOVE_ONE_3_FORWARD]=1;
    //    isBuildable[MOVE_ONE_1_BACKWARD]=1;isVariable[MOVE_ONE_1_BACKWARD]=1;
                                                                                                                                                            
//        isBuildable[INTRON]=1; // :-)
                                                                                                                                                            
                switch(getRace())
                {
                        case TERRA:isBuildable[SUPPLY_DEPOT]=1;isVariable[SUPPLY_DEPOT]=1;break;
                        case PROTOSS:isBuildable[PYLON]=1;isVariable[PYLON]=1;break;
                        case ZERG:isBuildable[OVERLORD]=1;isVariable[OVERLORD]=1;isBuildable[LARVA]=0;isVariable[LARVA]=0;break;
                        default:break;
                }

	for(j=6;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(i=UNIT_TYPE_COUNT;i--;)
			if((allGoal[i])||(isBuildable[i]))
			{
				if((i==GAS_SCV)&&(allGoal[REFINERY]==0))
					addGoal(REFINERY,1,0,0); //~~
				isBuildable[i]=1;
				//gather all prerequisites and mark them as goals
				for(k=0;k<3;k++)
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
	for(i=UNIT_TYPE_COUNT;i--;)
		if((allGoal[i]>0)||(isBuildable[i]>0))
		{
			for(j=3;j--;)
				if(allGoal[pStats[i].facility[j]]>0)
				{
					for(k=0;k<j;k++)
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
        for(i=UNIT_TYPE_COUNT;i--;)
                Need_Gas+=(allGoal[i]*pStats[i].gas);
        if(Need_Gas>0)
        {
                isBuildable[REFINERY]=1;
                if(allGoal[REFINERY]==0)
                       addGoal(REFINERY,1,0,0); //ASSIMILATOR == EXTRACTOR == REFINERY
                isBuildable[GAS_SCV]=1; //ONE_MINERAL_SCV... = ONE_MINERAL_PROBE... = ONE_MINERAL_DRONE...
                isVariable[GAS_SCV]=1;
        }; 
	maxBuildTypes=0;
	for(i=UNIT_TYPE_COUNT;i--;)
		if(isBuildable[i]==1)
		{
			genoToPhaenotype[maxBuildTypes]=i;
			phaenoToGenotype[i]=maxBuildTypes;
			maxBuildTypes++;
		};
	//TODO: ueberlegen ob nicht einfach Move+ und Move- reichen...
	return(1);
};


int EXPORT GOAL_ENTRY::isGoal(int unit)
{
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
	return(allGoal[unit]>0);
};

int EXPORT GOAL_ENTRY::addGoal(int unit, int count, int time, int location)
{
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
	if((maxBuildTypes<0)||(maxBuildTypes>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (GOAL_ENTRY::getMaxBuildTypes): Variable not initialized [%i].",maxBuildTypes);
		return(0);
	}
	return(maxBuildTypes);
};


int EXPORT GOAL_ENTRY::getInitialized()
{
	return(initialized);
};

int EXPORT GOAL_ENTRY::toGeno(int num)
{
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
	return(phaenoToGenotype[num]);
}

int EXPORT GOAL_ENTRY::toPhaeno(int num)
{
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
	return(genoToPhaenotype[num]);
}



GOAL_ENTRY::GOAL_ENTRY()
{
	int i,j;
	raceInitialized=0;
	strcpy(name,"Error!");
	goalCount=0;
	for(i=0;i<MAX_GOALS;i++)
	{
		goal[i].count=0;
		goal[i].time=0;
		goal[i].unit=0;
		goal[i].location=0;
	}
	for(i=UNIT_TYPE_COUNT;i--;)
	{
		allGoal[i]=0;
		isVariable[i]=0;
		isBuildable[i]=0;
		genoToPhaenotype[i]=0;
		phaenoToGenotype[i]=0;
		for(j=0;j<MAX_LOCATIONS;j++)
			globalGoal[j][i]=0;
	}
	initialized=1;
};

GOAL_ENTRY::~GOAL_ENTRY()
{
};

