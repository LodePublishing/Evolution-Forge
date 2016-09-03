#include "goal.hpp"

GOAL_ENTRY::GOAL_ENTRY()
{
	resetData();
}

GOAL_ENTRY::~GOAL_ENTRY()
{
}

void GOAL_ENTRY::resetData()
{
//	mode=0;
	raceInitialized=false;
	maxBuildTypes=0;
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		genoToPhaenotype[i]=-1;
		phaenoToGenotype[i]=-1;
		allGoal[i]=0;
		isVariable[i]=0;
		isBuildable[i]=0;
		for(int j=MAX_LOCATIONS;j--;)
			globalGoal[j][i]=0;
	}
//	initialized=true;
}

const int GOAL_ENTRY::countGoals() const
{
	int goalNum=0;
	for(list<GOAL>::const_iterator i = goal.begin(); i!=goal.end();++i)
		if(i->count>0)
			goalNum++;
	return(goalNum);
	// TODO evtl bei addGoal mitprotokollieren
}

const bool GOAL_ENTRY::calculateReady(const UNIT* units) const
{
    bool ready=true;
    for(list<GOAL>::const_iterator i = goal.begin(); (i!=goal.end())&&(ready);++i)
        if(i->count)
            ready&=((i->count<=units[i->location].getTotal(i->unit))&&((i->time>=i->finalTime)||(i->time==0)));
    return(ready);
}

const bool GOAL_ENTRY::getNextGoal(list<GOAL>::const_iterator& current, const bool first) const
{
	if(first)
		current=goal.begin();
	else 
	{
		++current;
		while((current!=goal.end())&&(current->count==0))
			++current;
	}
	if(current==goal.end()) return(false);
	else return(true);
}	

void GOAL_ENTRY::calculateFinalTimes(const int location, const int unit, const int time)
{
    for(list<GOAL>::iterator i=goal.begin();i!=goal.end();++i)
    {
// ist dieses goal belegt?
        if(( i->unit>0 )&&
// befinden wir uns am richtigen Ort?
          (( i->location==0 )||( i->location == location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
            ( i->unit == unit ) )
              i->finalTime = time;
    }
}

const int GOAL_ENTRY::getAllGoal(const int unit) const
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>GAS_SCV)) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Value unit out of range.");return(0);
    }
	if((allGoal[unit]<0)||(allGoal[unit]>200)) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Variable allGoal out of range.");return(0);
    }	
#endif
	return(allGoal[unit]);		
}

const int GOAL_ENTRY::getGlobalGoal(const int location, const int unit) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Value location out of range.");return(0);
    }
	if((unit<0)||(unit>GAS_SCV)) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Value unit out of range.");return(0);
    }
	if((globalGoal[location][unit]<0)||(globalGoal[location][unit]>200)) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Variable globalGoal out of range.");return(0);
    }	
#endif
	return(globalGoal[location][unit]);		
}
const bool GOAL_ENTRY::getIsBuildable(const int unit) const
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>GAS_SCV)) {
		toLog("DEBUG: (GOAL_ENTRY::getIsBuildable): Value unit out of range.");return(false);
    }
#endif
	return(isBuildable[unit]);		
}

void EXPORT GOAL_ENTRY::copy(const GOAL_ENTRY* goal, const UNIT* unit)
{
#ifdef _SCC_DEBUG
	if(!goal) {
		toLog("DEBUG: (GOAL_ENTRY::copy): Variable goal not initialized.");return;
	}
#endif
	setRace(goal->getRace());
	maxBuildTypes=goal->getMaxBuildTypes();
//	initialized=goal->getInitialized();
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		allGoal[i]=goal->allGoal[i];
		isBuildable[i]=goal->isBuildable[i];
		isVariable[i]=goal->isVariable[i];
		for(int j=MAX_LOCATIONS;j--;)
			globalGoal[j][i]=goal->globalGoal[j][i];
		genoToPhaenotype[i]=goal->genoToPhaenotype[i];
		phaenoToGenotype[i]=goal->phaenoToGenotype[i];
	}
	for(list<GOAL>::const_iterator i = goal->goal.begin();i!=goal->goal.end();++i)
	{
		this->goal.push_back(*i);
/*		this->goal[i].unit=goal->goal[i].unit;
		this->goal[i].time=goal->goal[i].time;
		this->goal[i].count=goal->goal[i].count;
		this->goal[i].location=goal->goal[i].location;*/
	}
	adjustGoals(true, unit);
}

void EXPORT GOAL_ENTRY::setRace(const eRace race)
{
	resetData();	
	pStats=&(stats[this->race=race][0]);
	raceInitialized=true;
	changed=true;
}

void EXPORT GOAL_ENTRY::adjustGoals(const bool allowGoalAdaption, const UNIT* unit)
{
    for(int i=UNIT_TYPE_COUNT;i--;)
	{
		genoToPhaenotype[i]=-1;
		phaenoToGenotype[i]=-1;
		isVariable[i]=0; //?
		isBuildable[i]=0;
	}
	maxBuildTypes=0;

	// if free-mode  set all isbuildable to 1
// else ...
	int oldGoal[UNIT_TYPE_COUNT]; //goals we got from the user which we MAY NOT ignore
	for(int i=UNIT_TYPE_COUNT;i--;) {
		oldGoal[i]=allGoal[i];
	}
//	fill(oldGoal)
//TODO: Reset hier rein!
	
//		isBuildable[MOVE_ONE_1_FORWARD]=1;isVariable[MOVE_ONE_1_FORWARD]=1;
  //	  isBuildable[MOVE_ONE_3_FORWARD]=1;isVariable[MOVE_ONE_3_FORWARD]=1;
	//	isBuildable[MOVE_ONE_1_BACKWARD]=1;isVariable[MOVE_ONE_1_BACKWARD]=1;
		  //isBuildable[INTRON]=1; // :-)
	if(unit)
		for(int i=GAS_SCV+1;i--;)
			if(unit->getTotal(i))
			{
				if(allGoal[i]<unit->getTotal(i))
					addGoal(i, unit->getTotal(i)-allGoal[i], 0, 0);
			}
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
				if((pStats[i].facility2>0)&&(allGoal[pStats[i].facility2]==0)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST))
				{
					isVariable[pStats[i].facility2]=1;
					if(pStats[i].facilityType!=IS_LOST) //do not set those morph-facilities as goals...
						addGoal(pStats[i].facility2,1,0,0);
					else isBuildable[pStats[i].facility2]=1;// ... but make them buildable :)
				}
				if((pStats[i].facility[0]>0)&&(allGoal[pStats[i].facility[0]]==0)&&((pStats[i].facility[1]==0)||(allGoal[pStats[i].facility[1]]==0))&&((pStats[i].facility[2]==0)||(allGoal[pStats[i].facility[2]]==0)))
// only facility[0] is important
				{
					isVariable[pStats[i].facility[0]]=1;
					if((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_ALWAYS))
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
		} 

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
		}

	// hack for unit who cannot be built but needs to be translated by phaenoToGenotype! (for the forcewindow)
	if(getRace()==ZERG)
	{
		genoToPhaenotype[maxBuildTypes]=LARVA;
		phaenoToGenotype[LARVA]=maxBuildTypes;
		// further unit need to temporarily increment maxBuildTypes
	}

	//TODO: ueberlegen ob nicht einfach Move+ und Move- reichen...
}

// -------------------------------
// ------ SET/GET FUNCTIONS ------
// -------------------------------

const UNIT_STATISTICS* EXPORT GOAL_ENTRY::getpStats() const
{
#ifdef _SCC_DEBUG
	if(pStats==NULL) {
		toLog("DEBUG: (GOAL_ENTRY::getpStats): Variable pStats not initialized.");return(0);
	}
#endif
	return(pStats);
}

void EXPORT GOAL_ENTRY::addGoal(const int unit, const int count, const int time, const int location)
{
#ifdef _SCC_DEBUG
	if((unit<=0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value unit out of range.");return;
	}
	if((count+globalGoal[location][unit]<0)||(count>MAX_TOTAL_UNITS)) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value count out of range.");return;
	}
	if((time<0)||(time>=MAX_TIME)) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value time out of range.");return;
	}
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value location out of range.");return;
	}
#endif
//TODO goal loeschen einbauen
	if((race==ZERG)&&(unit==LARVA))
		return;
	allGoal[unit]+=count;
	if(allGoal[unit]==0)
	{
		isBuildable[unit]=0;
		isVariable[unit]=0;
	}

	globalGoal[location][unit]+=count;

	bool found=false;
	// TODO wenn Einheiten an mehreren verschiedenen Positionen und location 0 geloescht wird aufsammeln!!
	for(list<GOAL>::iterator i=goal.begin(); (i!=goal.end())&&(!found); ++i)
	{
		if((i->unit==unit)&&(i->time==time)&&(i->location==location)) {
	//TODO goal loeschen..!!!!!!!!!!!!!!11
//			asdf
			i->count += count;
			found=true;
		}
	}
	// neue goal erstellen
	if(!found)
	{
		GOAL new_goal;
		new_goal.unit=unit;
		new_goal.time=time;
		new_goal.location=location;
		new_goal.count=count;
		new_goal.finalTime=0;
		goal.push_back(new_goal);
	}
	changed=true;
}

const bool EXPORT GOAL_ENTRY::isGoal(const int unit) const
{
#ifdef _SCC_DEBUG
	if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::isGoal): Value unit out of range.");return(0);
	}
	// TODO UNIT_TYPE_COUNT ist nicht obere Grenze fuer Zahl der Units...
	if((allGoal[unit]<0)||(allGoal[unit]>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::isGoal): Variable allGoal not initialized.");return(0);
	}
#endif
	return(allGoal[unit]>0);
}
const int EXPORT GOAL_ENTRY::getMaxBuildTypes() const
{
#ifdef _SCC_DEBUG
	if((maxBuildTypes<0)||(maxBuildTypes>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::getMaxBuildTypes): Variable not initialized.");return(0);
	}
#endif
	return(maxBuildTypes);
}

/*const bool EXPORT GOAL_ENTRY::getInitialized() const
{
	return(initialized);
}*/

const int EXPORT GOAL_ENTRY::toGeno(const int phaeno) const
{
#ifdef _SCC_DEBUG
	if((phaeno<0)||(phaeno>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::toGeno): Value out of range.");return(0);
	}
	if((phaenoToGenotype[phaeno]<0)||(phaenoToGenotype[phaeno]>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::toGeno): Variable not initialized.");return(0);
	}
#endif
	return(phaenoToGenotype[phaeno]);
}

const int EXPORT GOAL_ENTRY::toPhaeno(const int geno) const
{
#ifdef _SCC_DEBUG
// TODO irgendwie maxbuildtypes statt UNIT_TYPE_COUNT?
	if((geno<0)||(geno>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::toPhaeno): Value out of range.");return(0);
	}
	if((genoToPhaenotype[geno]<0)||(genoToPhaenotype[geno]>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::toPhaeno): Variable not initialized.");return(0);
	}
#endif
	return(genoToPhaenotype[geno]);
}

const bool EXPORT GOAL_ENTRY::isChanged() const
{
	return(changed);
}

void EXPORT GOAL_ENTRY::changeAccepted()
{
//	adjustGoals(true); //PROBLEM: unitforce wird nicht mit einbezogen!
	changed=false;
}

const string& EXPORT GOAL_ENTRY::getName() const
{
	return name;
}

const eRace EXPORT GOAL_ENTRY::getRace() const
{
#ifdef _SCC_DEBUG
// TODO irgendwie maxbuildtypes statt UNIT_TYPE_COUNT?
    if(!raceInitialized) {
        toLog("DEBUG: (GOAL_ENTRY::getRace): race not initialized.");return(TERRA);
    }
#endif
	return race;
}

void EXPORT GOAL_ENTRY::setName(const string& name)
{
	this->name=name;
}

/*const int EXPORT GOAL_ENTRY::getMode() const // TODO
{
	return(mode);
}

void EXPORT GOAL_ENTRY::setMode(const int mode)
{
	this->mode=mode;
}*/


