#include "goalentry.hpp"

GOAL_ENTRY::GOAL_ENTRY():
	number(0),
	name("ERROR"),
	race(TERRA),
	maxBuildTypes(0),
	pStats(NULL),
	changed(false),
	raceInitialized(false),
	goal()	
{
	resetData();
}

GOAL_ENTRY::GOAL_ENTRY(const GOAL_ENTRY& object) :
	number(object.number),
	name(object.name),
	race(object.race),
	maxBuildTypes(object.maxBuildTypes),
	pStats(object.pStats),
	changed(object.changed),
	raceInitialized(object.raceInitialized),
	goal(object.goal) // optimize?
{
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		genoToPhaenotype[i]=object.genoToPhaenotype[i];
		phaenoToGenotype[i]=object.phaenoToGenotype[i];
		allGoal[i]=object.allGoal[i];
		isVariable[i]=object.isVariable[i];
		isBuildable[i]=object.isBuildable[i];
		isHaveable[i]=object.isHaveable[i];
		for(int j=MAX_LOCATIONS;j--;)
			globalGoal[j][i]=object.globalGoal[j][i];
	}
}

GOAL_ENTRY& GOAL_ENTRY::operator=(const GOAL_ENTRY& object)
{
	number = object.number;
	name = object.name;
	race = object.race;
	maxBuildTypes = object.maxBuildTypes;
	pStats = object.pStats;
	changed = object.changed;
	raceInitialized = object.raceInitialized;
	this->goal.clear();
	for(std::list<GOAL>::const_iterator i = object.goal.begin();i!=object.goal.end();++i)
		this->goal.push_back(*i); // optimize?
	//goal = object.goal; // optimize?
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		genoToPhaenotype[i]=object.genoToPhaenotype[i];
		phaenoToGenotype[i]=object.phaenoToGenotype[i];
		allGoal[i]=object.allGoal[i];
		isVariable[i]=object.isVariable[i];
		isBuildable[i]=object.isBuildable[i];
		isHaveable[i]=object.isHaveable[i];
		for(int j=MAX_LOCATIONS;j--;)
			globalGoal[j][i]=object.globalGoal[j][i];
	}
	return(*this);
}

GOAL_ENTRY::~GOAL_ENTRY()
{ }

void GOAL_ENTRY::resetData()
{
//	mode=0;
	raceInitialized=false;
	maxBuildTypes=0;
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		genoToPhaenotype[i]=999;
		phaenoToGenotype[i]=999;
		allGoal[i]=0;
		isVariable[i]=false;
		isBuildable[i]=false;
		isHaveable[i]=false;
		for(int j=MAX_LOCATIONS;j--;)
			globalGoal[j][i]=0;
	}
//	initialized=true;
}

const unsigned int GOAL_ENTRY::countGoals() const
{
	int goalNum=0;
	for(std::list<GOAL>::const_iterator i = goal.begin(); i!=goal.end();++i)
		if(i->getCount()>0)
			goalNum++;
	return(goalNum);
	// TODO evtl bei addGoal mitprotokollieren
}

const bool GOAL_ENTRY::calculateReady(const UNIT* units) const
{
	bool ready=true;
	for(std::list<GOAL>::const_iterator i = goal.begin(); (i!=goal.end())&&(ready); ++i)
		if(i->getCount())
			ready&=( (i->getCount() <= units[i->getLocation()].getTotal(i->getUnit())) && ((i->getTime()==0) || (i->getTime()>=i->getFinalTime())) );
	return(ready);
}


/*const GOAL& GOAL_ENTRY::getGoal(const int goal_number) const
{
	int j = 0;
	for(std::list<GOAL>::const_iterator i=goal.begin(); i!=goal.end(); ++i)
	{
		if(j == goal_number)
			return(*i);
		j++;
	}
	
}*/

/*const bool GOAL_ENTRY::getNextGoal(std::list<GOAL>::const_iterator& current, const bool first) const
{
	if(first)
		current=goal.begin();
	else
	{
		++current;
		while((current!=goal.end())&&(current->getCount()==0))
			++current;
	}
	if(current==goal.end()) return(false);
	else return(true);
}*/	

void GOAL_ENTRY::calculateFinalTimes(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time)
{
	for(std::list<GOAL>::iterator i=goal.begin();i!=goal.end();++i)
	{
// ist dieses goal belegt?
		if(( i->getUnit() == unit )&&
		   ( i->getUnit() > 0 )&&
		   ( i->getCount() == count) && // !!
// befinden wir uns am richtigen Ort?
		   (( i->getLocation()==0 )||( i->getLocation() == location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
			( i->getUnit() == unit ))
			  i->setFinalTime(time);
	}
}

const unsigned int GOAL_ENTRY::getAllGoal(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit>GAS_SCV) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Value unit out of range.");return(0);
	}
	if(allGoal[unit]>200) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Variable allGoal out of range.");return(0);
	}	
#endif
	return(allGoal[unit]);		
}

const unsigned int GOAL_ENTRY::getGlobalGoal(const unsigned int location, const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Value location out of range.");return(0);
	}
	if(unit>GAS_SCV) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Value unit out of range.");return(0);
	}
	if(globalGoal[location][unit]>200) {
		toLog("DEBUG: (GOAL_ENTRY::getAllGoal): Variable globalGoal out of range.");return(0);
	}	
#endif
	return(globalGoal[location][unit]);		
}
// TODO 

void GOAL_ENTRY::setRace(const eRace goal_race)
{
	resetData();
	race = goal_race;
	pStats = &(stats[race][0]);
	raceInitialized = true;
	changed = true;
}

const std::list<unsigned int> GOAL_ENTRY::allowDeletion(const unsigned int unitType)
{
	std::list<unsigned int> prefList;
#ifdef _SCC_DEBUG
	if((unitType==0)||(unitType>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::allowDeletion): Value unitType out of range.");return(prefList);
	}
#endif

	for(int i=UNIT_TYPE_COUNT;i--;)
		if((allGoal[i])||(isBuildable[i]))
		{
			bool error=false;

			if(pStats[i].facility[0] > 0)
			{
				if(pStats[i].facility[0] == unitType)
				{
				// pruefe ob andere Gebaeude als goals drin sind
					if(pStats[i].facility[1]>0)
					{
						if(allGoal[pStats[i].facility[1]]==0)
						{
							if(pStats[i].facility[2]>0)
							{
								if(allGoal[pStats[i].facility[2]]==0)
									error=true;
							} else error=true;
						}
					} else error=true;
				} else
				if(pStats[i].facility[1] == unitType)
				{
				// pruefe ob andere Gebaeude als goals drin sind
					if(allGoal[pStats[i].facility[0]]==0)
					{
						if(pStats[i].facility[2]>0)
						{
							if(allGoal[pStats[i].facility[2]]==0)
								error=true;
						} else error=true;
						   	}
						} else
						if(pStats[i].facility[2] == unitType)
						{
					// pruefe ob andere Gebaeude als goals drin sind
					if(allGoal[pStats[i].facility[0]]==0)
					{
						if(allGoal[pStats[i].facility[2]]==0)
							error=true;
					}
				}
			}
			if(!((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_ALWAYS)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE_IS_LOST)))
				error=false;

			for(int k=0;k<3;k++)
				if(pStats[i].prerequisite[k]==unitType)
					error=true;
			if((pStats[i].facility2>0)&&(pStats[i].facility2 == unitType)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST))
				error=false;
			if((pStats[i].gas>0)&&(unitType==REFINERY))
				error=true;
			if(pStats[i].unitType==REMAINING_UNIT_TYPE)
				error=false;			
			if(error)
				prefList.push_back(i);
		} 
	if(allGoal[unitType]==0)
		prefList.push_back(unitType);
	
	return(prefList);
}

const std::list<unsigned int> GOAL_ENTRY::allowAddition(const unsigned int unitType)
{
	std::list<unsigned int> prefList;
#ifdef _SCC_DEBUG
	if((unitType==0)||(unitType>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::allowAddition): Value unitType out of range.");return(prefList);
	}
#endif
	if(((pStats[unitType].upgrade[0]>0)&&(allGoal[unitType]>=3))||((pStats[unitType].upgrade[0]==0)&&(pStats[unitType].unitType == RESEARCH_UNIT_TYPE)&&(allGoal[unitType]>=1))) // TODO HACK :/
		prefList.push_back(unitType);
	return(prefList);
}


// fastest time to reach the goals, ignoring workers, minerals, gas, count etc. < - TODO 
const unsigned int GOAL_ENTRY::calculateFastestBO(const UNIT* startForce) const
{
	unsigned int min_time[GAS_SCV+1];
	unsigned int goals[GAS_SCV+1];

	for(int i = GAS_SCV+1;i--;)
	{
		if(startForce->getTotal(i))
		{
			goals[i]=0;
			min_time[i]=0;
		}
		else
		{ 
			if((allGoal[i])||(isBuildable[i]))
				goals[i]=1;
			else goals[i]=0;
			min_time[i]=MAX_TIME;
		}
	}

	for(int j=12;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(int i=GAS_SCV+1;i--;)
			if(goals[i])
			{
				unsigned int pre=0;
				for(int k=0;k<3;k++)
					if(pStats[i].prerequisite[k]>0)
					{
						if(pre < min_time[pStats[i].prerequisite[k]])// + pStats[pStats[i].prerequisite[k]].BT)
							pre = min_time[pStats[i].prerequisite[k]];// + pStats[pStats[i].prerequisite[k]].BT;
					}
				if(pStats[i].facility2>0)
				{
					if(pre < min_time[pStats[i].facility2])// + pStats[pStats[i].facility2].BT)
						pre = min_time[pStats[i].facility2];// + pStats[pStats[i].facility2].BT;
				}

				unsigned int fac=2*MAX_TIME;
				for(int k=0;k<3;k++)
					if(pStats[i].facility[k]>0)
					{
						if(fac > min_time[pStats[i].facility[k]])// + pStats[pStats[i].facility[k]].BT)
							fac = min_time[pStats[i].facility[k]];//+ pStats[pStats[i].facility[k]].BT;
					}
				if(fac==2*MAX_TIME) // ~~
					fac=0;
				if(pre < fac)
					pre = fac;
				if(pre < MAX_TIME)
					min_time[i]=pre + pStats[i].BT;
			}
	unsigned int min = 0;
	for(int i = GAS_SCV+1;i--;)
		if(allGoal[i])
			if(min < min_time[i])
				min = min_time[i];
	return(min);
}


const GOAL_TREE GOAL_ENTRY::getGoalTree(const UNIT* startForce, const unsigned int currentGoalUnit) const
{
	bool buildable[GAS_SCV+1];
	for(int i = 1;i<=GAS_SCV;i++)
	{
		if(isBuildable[i])
			buildable[i]=true;
		else buildable[i]=false;
	}
	
	if(currentGoalUnit>0)
	{
		buildable[currentGoalUnit]=true;
		for(int j = 0;j<10;j++)
			for(int i=1;i<=GAS_SCV;i++)
			if(buildable[i])
			{
				for(int k=0;k<3;k++)
				{
					if(pStats[i].prerequisite[k]>0)
						buildable[pStats[i].prerequisite[k]]=true;
				}
//				if(pStats[i].facility2>0)
//					buildable[pStats[i].facility2]=true;
				if( !(((pStats[i].facility[0]==0)||(buildable[pStats[i].facility[0]]))&&				
					((pStats[i].facility[1]==0)||(buildable[pStats[i].facility[1]]))&&
					((pStats[i].facility[2]==0)||(buildable[pStats[i].facility[2]]))))
				{
					if(pStats[i].facility[0]>0)
						buildable[pStats[i].facility[0]]=true;
				}
			}
	}
	
	GOAL_TREE tree;
	for(unsigned int i = 0; i <= GAS_SCV; i++)
	{
		tree.level[i]=0;
		tree.coloumn[i]=0;
	}
	for(int i=10;i--;)
		tree.width[i]=0;

	int c=0;
	for(int i = GAS_SCV+1;i--;)
		if(startForce->getTotal(i))
		{
			tree.unit[0].push_back(i);
			tree.coloumn[i]=c;
			c++;
			tree.checked[i]=true;
		}
		else
			tree.checked[i]=false;
	tree.width[0]=c;
	for(int k = 1; k<12;k++)
	{
		c=0;
		for(unsigned int i = 1; i <= GAS_SCV; i++)
			if(buildable[i])
			{
//				if((pStats[i].facility2>0)&&(tree.checked[pStats[i].facility2]))
//					tree.con[i].push_back(pStats[i].facility2);

				if((pStats[i].facility[0]>0)&&(tree.checked[pStats[i].facility[0]]))
					tree.con[i].push_back(pStats[i].facility[0]);
				if((pStats[i].facility[1]>0)&&(tree.checked[pStats[i].facility[1]]))
					tree.con[i].push_back(pStats[i].facility[1]);
				if((pStats[i].facility[2]>0)&&(tree.checked[pStats[i].facility[2]]))
					tree.con[i].push_back(pStats[i].facility[2]);

				if((pStats[i].prerequisite[0]>0)&&(tree.checked[pStats[i].prerequisite[0]]))
					tree.con[i].push_back(pStats[i].prerequisite[0]);
				if((pStats[i].prerequisite[1]>0)&&(tree.checked[pStats[i].prerequisite[1]]))
					tree.con[i].push_back(pStats[i].prerequisite[1]);
				if((pStats[i].prerequisite[2]>0)&&(tree.checked[pStats[i].prerequisite[2]]))
					tree.con[i].push_back(pStats[i].prerequisite[2]);
			}

		bool tcheck[GAS_SCV+1];
		for(unsigned int i = 1; i <= GAS_SCV; i++)
			tcheck[i]=false;

		for(unsigned int i = 1; i <= GAS_SCV; i++)
			if((buildable[i])&&(!tree.checked[i]))
			{
				if( //((pStats[i].facility2==0)||(tree.checked[pStats[i].facility2]))&&
				  	((pStats[i].facility[0]==0)||(tree.checked[pStats[i].facility[0]])||
					((pStats[i].facility[1]>0)&&(tree.checked[pStats[i].facility[1]]))||
					((pStats[i].facility[2]>0)&&(tree.checked[pStats[i].facility[2]])))&&

					((pStats[i].prerequisite[0]==0)||(tree.checked[pStats[i].prerequisite[0]]))&&
					((pStats[i].prerequisite[1]==0)||(tree.checked[pStats[i].prerequisite[1]]))&&
					((pStats[i].prerequisite[2]==0)||(tree.checked[pStats[i].prerequisite[2]])))
				{
					tcheck[i]=true;
					tree.unit[k].push_back(i);
					tree.coloumn[i]=c;
					c++;
					tree.level[i]=k;
				}
			}
			
		for(unsigned int i = 1; i <= GAS_SCV; i++)
			if(tcheck[i])
				tree.checked[i]=true;
		tree.width[k]=c;
	}
	return(tree);
}

const bool GOAL_ENTRY::getIsHaveable(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit>GAS_SCV) {
		toLog("DEBUG: (GOAL_ENTRY::getIsHaveable): Value unit out of range.");return(false);
	}
#endif
	return(isHaveable[unit]);
}
							

void GOAL_ENTRY::adjustGoals(const bool allowGoalAdaption, const UNIT* unit)
{
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		genoToPhaenotype[i]=999;
		phaenoToGenotype[i]=999;
		isVariable[i]=false; //?
		isBuildable[i]=false;
		isHaveable[i]=false;
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
	
//		isBuildable[MOVE_ONE_1_FORWARD]=true;isVariable[MOVE_ONE_1_FORWARD]=true;
  //	  isBuildable[MOVE_ONE_3_FORWARD]=true;isVariable[MOVE_ONE_3_FORWARD]=true;
	//	isBuildable[MOVE_ONE_1_BACKWARD]=true;isVariable[MOVE_ONE_1_BACKWARD]=true;
		  //isBuildable[INTRON]=true; // :-)
	if(unit)
		for(int i=GAS_SCV+1;i--;)
			if(unit->getTotal(i))
			{
				isBuildable[i]=true;
//				if(allGoal[i]<unit->getTotal(i))
//					addGoal(i, unit->getTotal(i)-allGoal[i], 0, 0);
			}
	//TODO addgoal evtl machen hier!
// TODO Locations fehlen hier, man muesste durch alle goals laufen und die jeweiligen Bedingungen u.U. AM ORT durchfuehren... mmmh... da ist natuerlich die Frage an welchem Ort... ok, ueberzeugt, Ort ist egal
	for(int j=6;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(int i=UNIT_TYPE_COUNT;i--;)
			if((allGoal[i])||(isBuildable[i]))
			{
			
				if((i==GAS_SCV)&&(allGoal[REFINERY]==0))
					addGoal(REFINERY,1,0,0); //~~
				isBuildable[i]=true;
				//gather all prerequisites and mark them as goals
				for(int k=0;k<3;k++)
					if((pStats[i].prerequisite[k]>0)&&(allGoal[pStats[i].prerequisite[k]]==0))
						addGoal(pStats[i].prerequisite[k], 1, 0, 0);
						
				if((pStats[i].facility2>0)&&(allGoal[pStats[i].facility2]==0)&&
				   (pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST))
				{
					isVariable[pStats[i].facility2]=true;
					if(pStats[i].facilityType!=IS_LOST) //do not set those morph-facilities as goals...
						addGoal(pStats[i].facility2, 1, 1, 0);
					else isBuildable[pStats[i].facility2]=true;// ... but make them buildable :)
				}
				int fac = 0;
				if(((pStats[i].facility[0]>0)&&(allGoal[pStats[i].facility[0]]==0))&&
				   ((pStats[i].facility[1]==0)||(allGoal[pStats[i].facility[1]]==0))&&
				   ((pStats[i].facility[2]==0)||(allGoal[pStats[i].facility[2]]==0)))
					fac = pStats[i].facility[0];
				else
				if(((pStats[i].facility[0]>0)&&(allGoal[pStats[i].facility[0]]==0))&&
				   ((pStats[i].facility[1]>0)&&(allGoal[pStats[i].facility[1]]==0))&&
				   ((pStats[i].facility[2]==0)||(allGoal[pStats[i].facility[2]]==0)))
					fac = pStats[i].facility[1];
				else
				if(((pStats[i].facility[0]>0)&&(allGoal[pStats[i].facility[0]]==0))&&
				   ((pStats[i].facility[1]>0)&&(allGoal[pStats[i].facility[1]]==0))&&
				   ((pStats[i].facility[2]>0)&&(allGoal[pStats[i].facility[2]]==0)))
					fac = pStats[i].facility[2];
				if(fac>0)
				{
					isVariable[fac]=true;
					if((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE_IS_LOST)||(pStats[i].facilityType==NEEDED_ALWAYS))
						addGoal(fac, 1, 1, 0);
					else isBuildable[fac]=true; // ... same as above...
				}
				
				if((allGoal[i]>1)&&(pStats[i].upgrade[0]>0) && (allGoal[pStats[i].upgrade[0]]==0))
				{
					isVariable[pStats[i].upgrade[0]]=true;
//					if((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_ALWAYS))
					addGoal(pStats[i].upgrade[0], 1, 0, 0);
//					else isBuildable[pStats[i].facility[0]]=1; // ... same as above...				
				}
				if((allGoal[i]>2)&& (pStats[i].upgrade[1]>0) && (allGoal[pStats[i].upgrade[1]]==0))
				{
					isVariable[pStats[i].upgrade[1]]=true;
//					if((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_ALWAYS))
					addGoal(pStats[i].upgrade[1], 1, 0, 0);
//					else isBuildable[pStats[i].facility[0]]=1; // ... same as above...				
				}
			
			}


//	now recheck whether we got too many goals
//	>_<
//	alle durchlaufen
//	jeweils alle facilities von rechts nach links anguggn
//	sobald eins gefunden das Teil der goals ist, alle weiter links streichen (aber buildable lassen)
// Beispiel: Wenn wir schon 'Starport[CT]' haben brauchen wir u.U. kein 'Starport' mehr
	for(int i=UNIT_TYPE_COUNT;i--;)
		if((allGoal[i]>0)||(isBuildable[i]))
		{
			for(int j=3;j--;)
				if((pStats[i].facility[j]>0)&&(allGoal[pStats[i].facility[j]]>0))
				{
					for(int k=0;k<j;k++)
					{
						if((allowGoalAdaption)&&(allGoal[pStats[i].facility[k]]>0))
						{
							for(std::list<GOAL>::iterator l=goal.begin(); l!=goal.end(); ++l)
								if(l->getUnit()==pStats[i].facility[k])
									l=goal.erase(l); //~~
									// evtl addgoal(-1,...	
							allGoal[pStats[i].facility[k]] = 0;//~~
//							addGoal(pStats[i].facility[k],  TODO!
						}
// TODO wenn Locations wichtig werden!!
						else 
							allGoal[pStats[i].facility[k]] = oldGoal[pStats[i].facility[k]];
					}
					j=0;
				}
				// why?
//			isBuildable[pStats[i].create]=false; //~~ ?? TODO?
//			isVariable[pStats[i].create]=false; ??
		}
	
		long Need_Gas=0;
		for(int i=UNIT_TYPE_COUNT;i--;)
			Need_Gas+=(allGoal[i]*pStats[i].gas);
		if(Need_Gas>0)
		{
			isBuildable[REFINERY]=true;
			if(allGoal[REFINERY]==0)
				   addGoal(REFINERY, 1, 0, 0); //ASSIMILATOR == EXTRACTOR == REFINERY
			isBuildable[GAS_SCV]=true; //ONE_MINERAL_SCV... = ONE_MINERAL_PROBE... = ONE_MINERAL_DRONE...
			isVariable[GAS_SCV]=true;
		} 

	switch(getRace())
	{
		case TERRA:isBuildable[SUPPLY_DEPOT]=true;isVariable[SUPPLY_DEPOT]=true;break;
		case PROTOSS:isBuildable[PYLON]=true;isVariable[PYLON]=true;break;
		case ZERG:isBuildable[OVERLORD]=true;isVariable[OVERLORD]=true;isBuildable[LARVA]=false;isVariable[LARVA]=false;break;
		default:break;
	}

	for(int i=UNIT_TYPE_COUNT;i--;)
		if((isBuildable[i])&&(phaenoToGenotype[i]==999)) // a goal && not set yet
		{
			genoToPhaenotype[maxBuildTypes]=i;
			phaenoToGenotype[i]=maxBuildTypes;
			maxBuildTypes++;
		}

	// hack for unit who cannot be built but needs to be translated by phaenoToGenotype! (for the forcewindow)
//	if(getRace()==ZERG)
//	{
//		genoToPhaenotype[maxBuildTypes]=LARVA;
//		phaenoToGenotype[LARVA]=maxBuildTypes;
//		// further unit need to temporarily increment maxBuildTypes
//	}

	
	for(int i=1;i<UNIT_TYPE_COUNT;i++)
		if(((isBuildable[i])||(allGoal[i]))&&(pStats[i].create>0))
			isHaveable[pStats[i].create]=true;

	for(int i=1;i<UNIT_TYPE_COUNT;i++)
	{
		if(allGoal[i])
			isHaveable[i]=false;
		else if(isBuildable[i])
			isHaveable[i]=true;
	}
	//TODO: ueberlegen ob nicht einfach Move+ und Move- reichen...

	// sort by location:
	//goal.sort(GOAL::GoalAscendingLocationSort());
//	goal.sort();
}

// -------------------------------
// ------ SET/GET FUNCTIONS ------
// -------------------------------

const UNIT_STATISTICS* GOAL_ENTRY::getpStats() const
{
#ifdef _SCC_DEBUG
	if(pStats==NULL) {
		toLog("DEBUG: (GOAL_ENTRY::getpStats): Variable pStats not initialized.");return(0);
	}
#endif
	return(pStats);
}

void GOAL_ENTRY::addGoal(const unsigned int unit, const signed int count, const unsigned int time, const unsigned int location)
{
#ifdef _SCC_DEBUG
	if(unit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value unit out of range.");return;
	}
	if(globalGoal[location][unit] + count>MAX_TOTAL_UNITS) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value count out of range.");return;
	}
	if(time>configuration.getMaxTime()) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value time out of range.");return;
	}
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value location out of range.");return;
	}
#endif
//TODO goal loeschen einbauen
	if((race==ZERG)&&(unit==LARVA))
		return;
	if((pStats[unit].upgrade[0]>0)&&(allGoal[unit]>=3)&&(count>0)) // TODO HACK :/
		return;
	if(pStats[unit].unitType==REMAINING_UNIT_TYPE)
		return;
	allGoal[unit]+=count;
	if(allGoal[unit]==0)
	{
		isBuildable[unit]=false;
		isVariable[unit]=false;
	}

	globalGoal[location][unit]+=count;

	bool found=false;
	// TODO wenn Einheiten an mehreren verschiedenen Positionen und location 0 geloescht wird aufsammeln!!
	for(std::list<GOAL>::iterator i=goal.begin(); (i!=goal.end())&&(!found); ++i)
	{
		if((i->getUnit()==unit)&&(i->getTime()==time)&&(i->getLocation()==location)) 
		{
			i->setCount( i->getCount() + count);
			changed=true;
			found=true;
			if(i->getCount() == 0)
				i = goal.erase(i);
			return;
		}
	}
	// neue goal erstellen
	if(!found)
	{
		GOAL new_goal;
		new_goal.setUnit(unit);
		new_goal.setTime(time);
		new_goal.setLocation(location);
		new_goal.setCount(count);
		new_goal.setFinalTime(0);
		goal.push_back(new_goal);
	}
	changed=true;
}

const bool GOAL_ENTRY::isGoal(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::isGoal): Value unit out of range.");return(0);
	}
	// TODO UNIT_TYPE_COUNT ist nicht obere Grenze fuer Zahl der Units...
	if(allGoal[unit]>=MAX_TOTAL_UNITS) {
		toLog("DEBUG: (GOAL_ENTRY::isGoal): Variable allGoal not initialized.");return(0);
	}
#endif
	return(allGoal[unit]>0);
}
const unsigned int GOAL_ENTRY::getMaxBuildTypes() const
{
#ifdef _SCC_DEBUG
	if(maxBuildTypes > UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::getMaxBuildTypes): Variable not initialized.");return(0);
	}
#endif
	return(maxBuildTypes);
}

/*const bool GOAL_ENTRY::getInitialized() const
{
	return(initialized);
}*/

const unsigned int GOAL_ENTRY::toGeno(const unsigned int phaeno) const
{
#ifdef _SCC_DEBUG
	if(phaeno>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::toGeno): Value out of range.");return(0);
	}
	if(phaenoToGenotype[phaeno]>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::toGeno): Variable not initialized.");return(0);
	}
#endif
	return(phaenoToGenotype[phaeno]);
}

const unsigned int GOAL_ENTRY::toPhaeno(const unsigned int geno) const
{
#ifdef _SCC_DEBUG
// TODO irgendwie maxbuildtypes statt UNIT_TYPE_COUNT?
	if(geno>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::toPhaeno): Value out of range.");return(0);
	}
	if(genoToPhaenotype[geno]>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::toPhaeno): Variable not initialized.");return(0);
	}
#endif
	return(genoToPhaenotype[geno]);
}

const bool GOAL_ENTRY::isChanged() const
{
	return(changed);
}

void GOAL_ENTRY::changeAccepted()
{
//	ajdjustGoals(true); //PROBLEM: unitforce wird nicht mit einbezogen!
	changed=false;
}

const std::string& GOAL_ENTRY::getName() const
{
	return name;
}

const eRace GOAL_ENTRY::getRace() const
{
#ifdef _SCC_DEBUG
// TODO irgendwie maxbuildtypes statt UNIT_TYPE_COUNT?
	if(!raceInitialized) {
		toLog("DEBUG: (GOAL_ENTRY::getRace): race not initialized.");return(TERRA);
	}
#endif
	return race;
}

void GOAL_ENTRY::setName(const std::string& goal_name)
{
	name.assign(goal_name);
}

/*const unsigned int GOAL_ENTRY::getMode() const // TODO
{
	return(mode);
}

void GOAL_ENTRY::setMode(const unsigned int mode)
{
	this->mode=mode;
}*/


