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
	memcpy(genoToPhaenotype, object.genoToPhaenotype, UNIT_TYPE_COUNT * sizeof(int));
	memcpy(phaenoToGenotype, object.phaenoToGenotype, UNIT_TYPE_COUNT * sizeof(int));
	memcpy(allGoal, object.allGoal, UNIT_TYPE_COUNT * sizeof(int));

	memcpy(isStatic, object.isStatic, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isBuildable, object.isBuildable, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isHaveable, object.isHaveable, UNIT_TYPE_COUNT * sizeof(bool));

	memcpy(globalGoal, object.globalGoal, UNIT_TYPE_COUNT * MAX_LOCATIONS * sizeof(int));
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
	goal.clear();
	for(std::list<GOAL>::const_iterator i = object.goal.begin();i!=object.goal.end();++i)
		this->goal.push_back(*i); // optimize?
	//goal = object.goal; // optimize?
	memcpy(genoToPhaenotype, object.genoToPhaenotype, UNIT_TYPE_COUNT * sizeof(int));
	memcpy(phaenoToGenotype, object.phaenoToGenotype, UNIT_TYPE_COUNT * sizeof(int));
	memcpy(allGoal, object.allGoal, UNIT_TYPE_COUNT * sizeof(int));
		
	memcpy(isStatic, object.isStatic, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isBuildable, object.isBuildable, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isHaveable, object.isHaveable, UNIT_TYPE_COUNT * sizeof(bool));

	memcpy(globalGoal, object.globalGoal, UNIT_TYPE_COUNT * MAX_LOCATIONS * sizeof(int));
		
	return(*this);
}

GOAL_ENTRY::~GOAL_ENTRY()
{ }

void GOAL_ENTRY::resetData()
{
//	mode=0;
	raceInitialized = false;
	maxBuildTypes = 0;
	// DO NOT MEMSET!
	for(unsigned int i = UNIT_TYPE_COUNT; i--; )
	{
		genoToPhaenotype[i] = 999;
		phaenoToGenotype[i] = 999;
		isStatic[i] = true;
		isBuildable[i] = false;
		isHaveable[i] = false;
	}

	memset(allGoal, 0, UNIT_TYPE_COUNT * sizeof(int));
	memset(globalGoal, 0, UNIT_TYPE_COUNT * MAX_LOCATIONS * sizeof(int));
//	initialized=true;
}

const unsigned int GOAL_ENTRY::countGoals() const
{
//	int goalNum=0;
//	for(std::list<GOAL>::const_iterator i = goal.begin(); i!=goal.end();++i)
//		if(i->getCount()>0)
//			++goalNum;
	return(goal.size());
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
		++j;
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
	for(std::list<GOAL>::iterator i=goal.begin();i!=goal.end(); ++i)
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


// TODO 

void GOAL_ENTRY::setRace(const eRace goal_race)
{
	resetData();
	race = goal_race;
	pStats = &(stats[race][0]);
	raceInitialized = true;
	changed = true;
}

const std::list<unsigned int> GOAL_ENTRY::allowDeletion(const unsigned int unitType) const
{
	std::list<unsigned int> prefList;
#ifdef _SCC_DEBUG
	if((unitType==0)||(unitType>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::allowDeletion): Value unitType out of range.");return(prefList);
	}
#endif

	for(unsigned int i=UNIT_TYPE_COUNT;i--;)
		if((allGoal[i]))//||(isBuildable[i]))
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

			for(unsigned int k=0;k<3; ++k)
				if(pStats[i].prerequisite[k]==unitType)
					error=true;
			if((pStats[i].facility2>0)&&(pStats[i].facility2 == unitType)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST))
				error=false;
			if((pStats[i].gas>0)&&(unitType==REFINERY))
				error=true;
			if(pStats[i].unitType==REMAINING_UNIT_TYPE)
				error=false;			
			if((pStats[unitType].create>0)&&(pStats[unitType].create==i)&&(allGoal[pStats[unitType].create]>0)&&(pStats[unitType].create!=unitType))
				error=true;
			if(error)
				prefList.push_back(i);
		} 
	if(allGoal[unitType]==0)
		prefList.push_back(unitType);
	
	return(prefList);
}

const std::list<unsigned int> GOAL_ENTRY::allowAddition(const unsigned int unitType) const
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
	unsigned int min_time[UNIT_TYPE_COUNT];
	unsigned int goals[UNIT_TYPE_COUNT];

	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
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

	for(unsigned int j=10;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(unsigned int i=UNIT_TYPE_COUNT;i--;)
			if(goals[i])
			{
				unsigned int pre=0;
				for(unsigned int k=0;k<3; ++k)
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

				unsigned int fac = 2*MAX_TIME;
				for(unsigned int k = 0;k < 3; ++k)
					if(pStats[i].facility[k]>0)
					{
						if(fac > min_time[pStats[i].facility[k]])// + pStats[pStats[i].facility[k]].BT)
							fac = min_time[pStats[i].facility[k]];//+ pStats[pStats[i].facility[k]].BT;
					}
				if(fac==2*MAX_TIME) // ~~
					fac = 0;
				if(pre < fac)
					pre = fac;
				if(pre < MAX_TIME)
				{
					min_time[i] = pre + pStats[i].BT;
					if(pStats[i].create>0)
						min_time[pStats[i].create] = pre + pStats[i].BT;
				}
			}
	unsigned int min = 0;
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		if(allGoal[i])
			if(min < min_time[i])
				min = min_time[i];
	return(min);
}


const GOAL_TREE GOAL_ENTRY::getGoalTree(const UNIT* startForce, const unsigned int currentGoalUnit) const
{
	bool buildable[GAS_SCV+1];
	for(unsigned int i = 1;i<=GAS_SCV; ++i)
	{
		if(isBuildable[i])
			buildable[i]=true;
		else buildable[i]=false;
	}
// TODO evtl die neuen Units hell darstellen!	
	if(currentGoalUnit>0)
	{
		buildable[currentGoalUnit]=true;
		for(unsigned int j = 0;j<10; ++j)
			for(unsigned int i=1;i<=GAS_SCV; ++i)
			if(buildable[i])
			{
				for(unsigned int k=0;k<3; ++k)
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
				if( (pStats[i].facility[0]>0) && (pStats[pStats[i].facility[0]].unitType == REMAINING_UNIT_TYPE ))
				{
					unsigned int l = 0;
					for(l = GAS_SCV;l--;)
						if(pStats[l].create == pStats[i].facility[0])
							break;
					if(l)
						buildable[l] = true;
				}
			}
	}
	
	GOAL_TREE tree;

	memset(tree.level, 0, (GAS_SCV+1) * sizeof(int));
	memset(tree.coloumn, 0, (GAS_SCV+1) * sizeof(int));

	for(unsigned int i=10;i--;)
		tree.width[i]=0;

	unsigned int c=0;
	for(unsigned int i = GAS_SCV+1;i--;)
		if(startForce->getTotal(i))
		{
			tree.unit[0].push_back(i);
			tree.coloumn[i]=c;
			++c;
			tree.checked[i]=true;
		}
		else
			tree.checked[i]=false;
	tree.width[0]=c;
	for(unsigned int k = 1; k<10; ++k)
	{
		c=0;
		for(unsigned int i = 1; i <= GAS_SCV; ++i)
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
				if( (pStats[i].facility[0]>0) && (pStats[pStats[i].facility[0]].unitType == REMAINING_UNIT_TYPE ))
				{
					unsigned int j = 0;
					for(j = GAS_SCV;j--;)
						if(pStats[j].create == pStats[i].facility[0])
							break;
					if(j)
						tree.con[i].push_back(j);
				}
			}

		bool tcheck[GAS_SCV+1];
		memset(tcheck, 0, (GAS_SCV+1) * sizeof(bool)); // ?

		for(unsigned int i = 1; i <= GAS_SCV; ++i)
			if((buildable[i])&&(!tree.checked[i]))
			{
			// TODO
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
					++c;
					tree.level[i]=k;
				}
				
				if( (pStats[i].facility[0]>0) && (pStats[pStats[i].facility[0]].unitType == REMAINING_UNIT_TYPE ))
				{
					unsigned int j = 0;
					for(j = GAS_SCV;j--;)
						if(pStats[j].create == pStats[i].facility[0])
							break;
					if((j)&&(tree.checked[j]))
					{
						tcheck[i]=true;
						tree.unit[k].push_back(i);
						tree.coloumn[i]=c;
						++c;
						tree.level[i]=k;
					}
				}
			}
			
		for(unsigned int i = 1; i <= GAS_SCV; ++i)
			if(tcheck[i])
				tree.checked[i]=true;
		tree.width[k]=c;
	}
	return(tree);
}


void GOAL_ENTRY::adjustGoals(const bool allowGoalAdaption, const UNIT* unit)
{
	for(unsigned int i = UNIT_TYPE_COUNT; i--; )
	{
		genoToPhaenotype[i] = 999;
		phaenoToGenotype[i] = 999;
		isStatic[i] = true;
		isBuildable[i] = false;
		isHaveable[i] = false;
	}
	
	maxBuildTypes=0;

	// if free-mode  set all isbuildable to 1
// else ...
	unsigned int oldGoal[UNIT_TYPE_COUNT]; //goals we got from the user which we MAY NOT ignore
	memcpy(oldGoal, allGoal, UNIT_TYPE_COUNT * sizeof(int));

//	fill(oldGoal)
//TODO: Reset hier rein!
	
//		isBuildable[MOVE_ONE_1_FORWARD]=true;isVariable[MOVE_ONE_1_FORWARD]=true;
  //	  isBuildable[MOVE_ONE_3_FORWARD]=true;isVariable[MOVE_ONE_3_FORWARD]=true;
	//	isBuildable[MOVE_ONE_1_BACKWARD]=true;isVariable[MOVE_ONE_1_BACKWARD]=true;
		  //isBuildable[INTRON]=true; // :-)
	if(unit)
		for(unsigned int i=LAST_UNIT; i--;)
			if(unit->getTotal(i))
			{
				isBuildable[i]=true;
//				if(allGoal[i]<unit->getTotal(i))
//					addGoal(i, unit->getTotal(i)-allGoal[i], 0, 0);
			}
	//TODO addgoal evtl machen hier!
// TODO Locations fehlen hier, man muesste durch alle goals laufen und die jeweiligen Bedingungen u.U. AM ORT durchfuehren... mmmh... da ist natuerlich die Frage an welchem Ort... ok, ueberzeugt, Ort ist egal
	for(unsigned int j=6;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(unsigned int i=LAST_UNIT;i--;)
			if((allGoal[i])||(isBuildable[i]))
			{
			
				if((i==GAS_SCV)&&(allGoal[REFINERY]==0))
					addGoal(REFINERY,1,0,0); //~~
				isBuildable[i]=true;
				//gather all prerequisites and mark them as goals
				for(unsigned int k=3;k--;)
					if((pStats[i].prerequisite[k]>0)&&(allGoal[pStats[i].prerequisite[k]]==0))
						addGoal(pStats[i].prerequisite[k], 1, 0, 0);
						
				if((pStats[i].facility2>0)&&(allGoal[pStats[i].facility2]==0)&&
				   (pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST)&&(pStats[pStats[i].facility2].unitType!=REMAINING_UNIT_TYPE)) 
				{
//					isVariable[pStats[i].facility2] = true;
					if((pStats[i].facilityType!=IS_LOST)&&(pStats[i].facilityType!=IS_MORPHING))//do not set those morph-facilities as goals...
						addGoal(pStats[i].facility2, 1, 0, 0);
					else isBuildable[pStats[i].facility2]=true;// ... but make them buildable :)
				}
				unsigned int fac = 0;
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
//					isVariable[fac]=true;
					if((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE_IS_LOST)||(pStats[i].facilityType==NEEDED_ALWAYS))
						addGoal(fac, 1, 0, 0);
					else isBuildable[fac]=true; // ... same as above...
				}
				
				if((allGoal[i]>1)&&(pStats[i].upgrade[0]>0) && (allGoal[pStats[i].upgrade[0]]==0))
				{
//					isVariable[pStats[i].upgrade[0]]=true;
//					if((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_ALWAYS))
					addGoal(pStats[i].upgrade[0], 1, 0, 0);
//					else isBuildable[pStats[i].facility[0]]=1; // ... same as above...				
				}
				if((allGoal[i]>2)&& (pStats[i].upgrade[1]>0) && (allGoal[pStats[i].upgrade[1]]==0))
				{
//					isVariable[pStats[i].upgrade[1]]=true;
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
	for(unsigned int i=UNIT_TYPE_COUNT;i--;)
		if((allGoal[i]>0)||(isBuildable[i]))
		{
			for(unsigned int j=3;j--;)
				if((pStats[i].facility[j]>0)&&(allGoal[pStats[i].facility[j]]>0))
				{
					for(unsigned int k=0; k<j; ++k)
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
	
	long Need_Gas = 0;
	for(unsigned int i = UNIT_TYPE_COUNT; i--;)
		Need_Gas+=(allGoal[i]*pStats[i].gas);
	if(Need_Gas > 0)
	{
		isBuildable[REFINERY] = true;
		if(allGoal[REFINERY] == 0)
			   addGoal(REFINERY, 1, 0, 0); //ASSIMILATOR == EXTRACTOR == REFINERY
		isBuildable[GAS_SCV] = true; //ONE_MINERAL_SCV... = ONE_MINERAL_PROBE... = ONE_MINERAL_DRONE...
		isStatic[GAS_SCV] = false;
	} 

	switch(getRace())
	{
		case TERRA:isBuildable[SUPPLY_DEPOT]=true;isStatic[SUPPLY_DEPOT]=false;break;
		case PROTOSS:isBuildable[PYLON]=true;isStatic[PYLON]=false;break;
		case ZERG:isBuildable[OVERLORD]=true;isStatic[OVERLORD]=false;isBuildable[LARVA]=false;break;
		default:break;
	}

//	isBuildable[BUILD_PARALLEL_2] = true;
//	isBuildable[BUILD_PARALLEL_4] = true;
//	isBuildable[BUILD_PARALLEL_8] = true;
//	isBuildable[BUILD_PARALLEL_16] = true;
//	isBuildable[FROM_GAS_TO_MINERALS] = true;

	for(unsigned int i=UNIT_TYPE_COUNT;i--;)
		if((isBuildable[i])&&(phaenoToGenotype[i]==999)) // a goal && not set yet
		{
			genoToPhaenotype[maxBuildTypes] = i;
			phaenoToGenotype[i] = maxBuildTypes;
			++maxBuildTypes;
		}

	// hack for unit who cannot be built but needs to be translated by phaenoToGenotype! (for the forcewindow)
//	if(getRace()==ZERG)
//	{
//		genoToPhaenotype[maxBuildTypes]=LARVA;
//		phaenoToGenotype[LARVA]=maxBuildTypes;
//		// further unit need to temporarily increment maxBuildTypes
//	}


	
	for(unsigned int i=1;i<UNIT_TYPE_COUNT; ++i)
		if(((isBuildable[i])||(allGoal[i]))&&(pStats[i].create>0))
			isHaveable[pStats[i].create]=true;

	for(unsigned int i=1;i<UNIT_TYPE_COUNT; ++i)
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


	for(unsigned int i = LAST_UNIT;i--;)
		if(allGoal[i])
		{
			isStatic[pStats[i].facility[0]]=false;
			isStatic[pStats[i].facility[1]]=false;
			isStatic[pStats[i].facility[2]]=false;
			isStatic[pStats[i].facility2]=false;
		}

	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
	{
		allow[i].facility.clear();
		allow[i].facility2.clear();
		allow[i].prerequisite.clear();
		need[i].facility = false;
		need[i].facility2 = false;
		need[i].prerequisite = 0;;
	}

	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		if(isBuildable[i])
		{
			for(unsigned int j = 3;j--;)
			{
				if(pStats[i].facility[j]>0)
					allow[pStats[i].facility[j]].facility.push_back(i);
				if(pStats[i].prerequisite[j]>0)
					allow[pStats[i].prerequisite[j]].prerequisite.push_back(i);
			}
			if(pStats[i].facility2>0)
				allow[pStats[i].facility2].facility2.push_back(i);
			if((pStats[i].facility[0]>0)||(pStats[i].facility[1]>0)||(pStats[i].facility[2]>0))
				need[i].facility = true;
			for(unsigned int j=3;j--;)
				if(pStats[i].prerequisite[j])
					++need[i].prerequisite;
			if(pStats[i].facility2>0)
				need[i].facility2 = true;
		}	
}


const bool NEED::facilityIsDone()
{
	if(facility)
	{
		facility = false;
		if((!facility2)&&(prerequisite == 0))
			return(true);
	}
	return(false);
}

const bool NEED::facility2IsDone()
{
	if(facility2)
	{
		facility2 = false;
		if((!facility) && (prerequisite == 0))
			return(true);
	}
	return(false);
}

const bool NEED::prerequisiteIsDone()
{
	if(prerequisite>0)
	{
		--prerequisite;
		if((!facility) && (!facility2) && (prerequisite==0))
			return(true);
	}
	return(false);
}

NEED& NEED::operator=(const NEED& object)
{
	facility = object.facility;
	facility2 = object.facility2;
	prerequisite = object.prerequisite;
	return(*this);
}

// -------------------------------
// ------ SET/GET FUNCTIONS ------
// -------------------------------


void GOAL_ENTRY::addGoal(const unsigned int unit, const signed int count, const unsigned int time, const unsigned int location)
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value unit out of range.");return;
	}
	if(globalGoal[location][unit] + count > MAX_TOTAL_UNITS) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value count out of range.");return;
	}
	if(time > coreConfiguration.getMaxTime()) {
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value time out of range.");return;
	}
	if(location >= MAX_LOCATIONS) { // TODO
		toLog("DEBUG: (GOAL_ENTRY::addGoal): Value location out of range.");return;
	}
#endif
	signed int unit_count = count;// * (1 + pStats[unit].create == unit);
	
	if((race == ZERG)&&(unit == LARVA))
		return;
	if((pStats[unit].upgrade[0] > 0)&&(allGoal[unit] >= 3)&&(unit_count > 0)) // TODO HACK :/
		return;
//	if(pStats[unit].unitType==REMAINING_UNIT_TYPE)
//		return;
	allGoal[unit] += unit_count;
	if(allGoal[unit] == 0)
		isBuildable[unit] = false;

	globalGoal[location][unit] += unit_count;

	bool found=false;
	// TODO wenn Einheiten an mehreren verschiedenen Positionen und location 0 geloescht wird aufsammeln!!
	for(std::list<GOAL>::iterator i=goal.begin(); (i!=goal.end())&&(!found); ++i)
	{
		if((i->getUnit()==unit)&&(i->getTime()==time)&&(i->getLocation()==location))
		{
			i->setCount( i->getCount() + unit_count);
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
		new_goal.setCount(unit_count);
		new_goal.setFinalTime(coreConfiguration.getMaxTime());
		goal.push_back(new_goal);
	}
	changed=true;
}

