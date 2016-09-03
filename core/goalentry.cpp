#include "goalentry.hpp"

GOAL_ENTRY::GOAL_ENTRY():
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

	memcpy(isStatic, object.isStatic, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isBuildable, object.isBuildable, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isHaveable, object.isHaveable, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isGoal, object.isGoal, UNIT_TYPE_COUNT * sizeof(bool));

}

GOAL_ENTRY& GOAL_ENTRY::operator=(const GOAL_ENTRY& object)
{
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

	memcpy(isStatic, object.isStatic, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isBuildable, object.isBuildable, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isHaveable, object.isHaveable, UNIT_TYPE_COUNT * sizeof(bool));
	memcpy(isGoal, object.isGoal, UNIT_TYPE_COUNT * sizeof(bool));

		
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
		isGoal[i] = false;
	}

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

const unsigned int GOAL_ENTRY::calculateFitness(const UNIT* units, unsigned int (&bonus)[MAX_LOCATIONS][UNIT_TYPE_COUNT]) const
{
	unsigned int tpF = 0;
	for(std::list<GOAL>::const_iterator i = goal.begin(); i!= goal.end(); ++i)
// goal nicht erfuellt?
		if(i->getCount() > units[i->getLocation()].getTotal(i->getUnit()))
		{
			bonus[i->getLocation()][i->getUnit()] += i->getCount() - units[i->getLocation()].getTotal(i->getUnit());
		
			if((i->getTime()>0)&&(i->getFinalTime()>i->getTime()))
				tpF+=(i->getTime()*units[i->getLocation()].getTotal(i->getUnit()))*100/(i->getCount()*i->getFinalTime());
			else
				tpF+=units[i->getLocation()].getTotal(i->getUnit())*100/i->getCount();
		}
		else
		{
			if((i->getTime()>0)&&(i->getFinalTime()>i->getTime()))
				tpF+=(i->getTime()*100/i->getFinalTime());
			else tpF+=100;
		}
	return(tpF);
}

const bool GOAL_ENTRY::operator==(const GOAL_ENTRY& other) const
{
	if(goal.size() != other.goal.size())
		return(false);
	for(std::list<GOAL>::const_iterator i = goal.begin(); i!=goal.end(); ++i)
	{
		bool is_equal = false;
		for(std::list<GOAL>::const_iterator j = other.goal.begin(); (j!=other.goal.end())&&(!is_equal); ++j)
			if((j->getCount() == i->getCount()) && (j->getUnit() == i->getUnit()) && (j->getTime() == i->getTime()))
				is_equal = true;
		if(!is_equal)
			return(false);
	}
//	toLog(name + " equals " + other.name); TODO, wird zu oft aufgerufen
	return(true);
}


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




// fastest time to reach the goals, ignoring workers, minerals, gas, count etc. < - TODO 
const unsigned int GOAL_ENTRY::calculateFastestBO(const UNIT* startForce) const
{
	unsigned int min_time[UNIT_TYPE_COUNT];
	unsigned int goals[UNIT_TYPE_COUNT];

	for(unsigned int i = LAST_UNIT;i--;)
	{
		if(startForce->getTotal(i))
		{
			goals[i]=0;
			min_time[i]=0;
		}
		else
		{ 
			if((getIsGoal(i))||(getIsBuildable(i)))
				goals[i]=1;
			else goals[i]=0;
			min_time[i]=MAX_TIME;
		}
	}

	for(unsigned int j=10;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(unsigned int i=LAST_UNIT;i--;)
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
	for(unsigned int i = LAST_UNIT;i--;)
		if(getIsGoal(i))
			if(min < min_time[i])
				min = min_time[i];
	return(min);
}


const GOAL_TREE GOAL_ENTRY::getGoalTree(const UNIT* startForce, const unsigned int currentGoalUnit) const
{
	bool buildable[GAS_SCV+1];
	for(unsigned int i = 1;i<=GAS_SCV; ++i)
	{
		if(getIsBuildable(i))
			buildable[i] = true;
		else buildable[i] = false;
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

/*
void GOAL_ENTRY::removeDouble(const unsigned int goal_unit, const unsigned int goal_location, const unsigned int goal_time, const unsigned int goal_count)
{
	for(std::list<GOAL>::iterator l=goal.begin(); l!=goal.end(); ++l)
		if((l->getUnit() == goal_unit) && (l->getLocation() == goal_location) && (l->getTime() == 0))
		{
			if(l->getCount() < goal_count)
				addGoal(goal_unit, goal_count - l->getCount() , 0, goal_location);
			break;
		}
//	remove the goal
	addGoal(goal_unit, -goal_count, goal_time, goal_location);
}
*/
//	if(!getIsGoal(unit))

void GOAL_ENTRY::adjustGoals(const bool allowGoalAdaption, const UNIT* unit)
{
	for(unsigned int i = UNIT_TYPE_COUNT; i--; )
	{
		genoToPhaenotype[i] = 999;
		phaenoToGenotype[i] = 999;
		isStatic[i] = true;
		isBuildable[i] = false;
		isHaveable[i] = false;
//		isGoal[i] = false;
	}
	
	maxBuildTypes=0;

	// if free-mode  set all isbuildable to 1
// else ...
//	fill(oldGoal)
//TODO: Reset hier rein!
	
		  //isBuildable[INTRON]=true; // :-)
	if(unit)
		for(unsigned int i=LAST_UNIT; i--;)
			if(unit->getTotal(i))
				setIsBuildable(i);
	//TODO addgoal evtl machen hier!
// TODO Locations fehlen hier, man muesste durch alle goals laufen und die jeweiligen Bedingungen u.U. AM ORT durchfuehren... mmmh... da ist natuerlich die Frage an welchem Ort... ok, ueberzeugt, Ort ist egal
	for(unsigned int j=6;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
	{
		for(unsigned int i=LAST_UNIT;i--;)
			if(getIsGoal(i)||(getIsBuildable(i)))
			{
			
				if((i==GAS_SCV)&&(getIsGoal(REFINERY)==false))
					addNewGoalToList(REFINERY, 0, 0, 1); //~~
				setIsBuildable(i);
				//gather all prerequisites and mark them as goals
				for(unsigned int k=3;k--;)
					if((pStats[i].prerequisite[k]>0)&&(getIsGoal(pStats[i].prerequisite[k])==false))
						addNewGoalToList(pStats[i].prerequisite[k], 0, 0, 1);
						
				if((pStats[i].facility2>0)&&(!getIsGoal(pStats[i].facility2))&&
				   (pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[i].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST)&&(pStats[pStats[i].facility2].unitType!=REMAINING_UNIT_TYPE)) 
				{
					if((pStats[i].facilityType!=IS_LOST)&&(pStats[i].facilityType!=IS_MORPHING))//do not set those morph-facilities as goals...
						addNewGoalToList(pStats[i].facility2, 0, 0, 1);
					else setIsBuildable(pStats[i].facility2);// ... but make them buildable :)
				}
				unsigned int fac = 0;
				if(((pStats[i].facility[0]>0)&&(!getIsGoal(pStats[i].facility[0])))&&
				   ((pStats[i].facility[1]==0)||(!getIsGoal(pStats[i].facility[1])))&&
				   ((pStats[i].facility[2]==0)||(!getIsGoal(pStats[i].facility[2]))))
					fac = pStats[i].facility[0];
				else
				if(((pStats[i].facility[0]>0)&&(!getIsGoal(pStats[i].facility[0])))&&
				   ((pStats[i].facility[1]>0)&&(!getIsGoal(pStats[i].facility[1])))&&
				   ((pStats[i].facility[2]==0)||(!getIsGoal(pStats[i].facility[2]))))
					fac = pStats[i].facility[1];
				else
				if(((pStats[i].facility[0]>0)&&(!getIsGoal(pStats[i].facility[0])))&&
				   ((pStats[i].facility[1]>0)&&(!getIsGoal(pStats[i].facility[1])))&&
				   ((pStats[i].facility[2]>0)&&(!getIsGoal(pStats[i].facility[2]))))
					fac = pStats[i].facility[2];
				if(fac>0)
				{
					if((pStats[i].facilityType==NEEDED_ONCE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[i].facilityType==NEEDED_UNTIL_COMPLETE_IS_LOST)||(pStats[i].facilityType==NEEDED_ALWAYS))
						addNewGoalToList(fac, 0, 0, 1);
					else setIsBuildable(fac); // ... same as above...
				}
			}

		for(std::list<GOAL>::iterator l=goal.begin(); l!=goal.end(); ++l)
		{
			if((l->getCount()>1)&&(pStats[l->getUnit()].upgrade[0]>0) && (!getIsGoal(pStats[l->getUnit()].upgrade[0])))
				addNewGoalToList(pStats[l->getUnit()].upgrade[0], 0, 0, 1);
			if((l->getCount()>2)&&(pStats[l->getUnit()].upgrade[1]>0) && (!getIsGoal(pStats[l->getUnit()].upgrade[1])))
				addNewGoalToList(pStats[l->getUnit()].upgrade[1], 0, 0, 1);
		}
	}


//	now recheck whether we got too many goals
//	>_<
//	alle durchlaufen
//	jeweils alle facilities von rechts nach links anguggn
//	sobald eins gefunden das Teil der goals ist, alle weiter links streichen (aber buildable lassen)
// Beispiel: Wenn wir schon 'Starport[CT]' haben brauchen wir u.U. kein 'Starport' mehr
	for(unsigned int i=LAST_UNIT;i--;)
		if((getIsGoal(i))||(getIsBuildable(i)))
		{
			for(unsigned int j=3;j--;)
				if((pStats[i].facility[j]>0)&&(getIsGoal(pStats[i].facility[j])))
				{
					for(unsigned int k=0; k<j; ++k)
					{
						if((allowGoalAdaption)&&(getIsGoal(pStats[i].facility[k])))
						{
							for(std::list<GOAL>::iterator l=goal.begin(); l!=goal.end(); ++l)
								if(l->getUnit()==pStats[i].facility[k])
									l=goal.erase(l); //~~
									// evtl addgoal(-1,...	
						}
// TODO wenn Locations wichtig werden!!
					}
					j=0;
				}
				// why?
//			setIsBuildable(pStats[i].create, false); //~~ ?? TODO?
		}
	
	bool need_gas = false;
	for(std::list<GOAL>::const_iterator l=goal.begin(); l!=goal.end(); ++l)
		if(pStats[l->getUnit()].gas)
		{
			need_gas = true;
			break;
		}
	if(need_gas)
	{
		setIsBuildable(REFINERY);
		if(!getIsGoal(REFINERY))
			   addNewGoalToList(REFINERY, 0, 0, 1); //ASSIMILATOR == EXTRACTOR == REFINERY
		setIsBuildable(GAS_SCV); //ONE_MINERAL_SCV... = ONE_MINERAL_PROBE... = ONE_MINERAL_DRONE...
		setIsStatic(GAS_SCV, false);
	} 

	switch(getRace())
	{
		case TERRA:setIsBuildable(SUPPLY_DEPOT);setIsStatic(SUPPLY_DEPOT, false);break;
		case PROTOSS:setIsBuildable(PYLON);setIsStatic(PYLON, false);break;
		case ZERG:setIsBuildable(OVERLORD);setIsStatic(OVERLORD, false);setIsBuildable(LARVA, false);break;
		default:break;
	}

//	isBuildable[BUILD_PARALLEL_2] = true;
//	isBuildable[BUILD_PARALLEL_4] = true;
//	isBuildable[BUILD_PARALLEL_8] = true;
//	isBuildable[BUILD_PARALLEL_16] = true;
//	isBuildable[FROM_GAS_TO_MINERALS] = true;

	for(unsigned int i=LAST_UNIT;i--;)
		if((getIsBuildable(i))&&(phaenoToGenotype[i]==999)) // a goal && not set yet
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


	
	for(unsigned int i=1;i<LAST_UNIT; ++i)
		if(((getIsBuildable(i))||(getIsGoal(i)))&&(pStats[i].create>0))
			setIsHaveable(pStats[i].create);

	for(unsigned int i=1;i<LAST_UNIT; ++i)
	{
		if(getIsGoal(i))
			setIsHaveable(i, false);
		else if(getIsBuildable(i))
			setIsHaveable(i);
	}
	//TODO: ueberlegen ob nicht einfach Move+ und Move- reichen...

	// sort by location:
	//goal.sort(GOAL::GoalAscendingLocationSort());
//	goal.sort();


	for(unsigned int i = LAST_UNIT;i--;)
		if(getIsGoal(i))
		{
			setIsStatic(pStats[i].facility[0], false);
			setIsStatic(pStats[i].facility[1], false);
			setIsStatic(pStats[i].facility[2], false);
			setIsStatic(pStats[i].facility2, false);
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

	for(unsigned int i = LAST_UNIT;i--;)
		if(getIsBuildable(i))
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

const bool GOAL_ENTRY::isError(const unsigned int j, const unsigned int unit) const
{
	bool error = false;
	if(pStats[j].facility[0] > 0)
	{
		if(pStats[j].facility[0] == unit)
		{
		// pruefe ob andere Gebaeude als goals drin sind
			if(pStats[j].facility[1]>0)
			{
				if(!getIsGoal(pStats[j].facility[1]))
				{
					if(pStats[j].facility[2]>0)
					{
						if(!getIsGoal(pStats[j].facility[2]))
							error = true;
					} else error = true;
				}
			} else error=true;
		} else
		if(pStats[j].facility[1] == unit)
		{
		// pruefe ob andere Gebaeude als goals drin sind
			if(getIsGoal(pStats[j].facility[0]))
			{
				if(pStats[j].facility[2]>0)
				{
					if(!getIsGoal(pStats[j].facility[2]))
						error=true;
				} else error=true;
			}
		} else
		if(pStats[j].facility[2] == unit)
		{
	// pruefe ob andere Gebaeude als goals drin sind
			if(!getIsGoal(pStats[j].facility[0]))
			{
				if(!getIsGoal(pStats[j].facility[2]))
					error=true;
			}
		}
	}
	if(!((pStats[j].facilityType==NEEDED_ONCE)||(pStats[j].facilityType==NEEDED_UNTIL_COMPLETE)||(pStats[j].facilityType==NEEDED_ALWAYS)||(pStats[j].facilityType==NEEDED_UNTIL_COMPLETE_IS_LOST)))
		error=false;

	for(unsigned int k=0;k<3; ++k)
		if(pStats[j].prerequisite[k]==unit)
			error=true;
	if((pStats[j].facility2>0)&&(pStats[j].facility2 == unit)&&(pStats[j].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE)&&(pStats[j].facilityType!=NEEDED_UNTIL_COMPLETE_IS_LOST))
		error=false;
	if((pStats[j].gas>0)&&(unit==REFINERY))
		error=true;
	if(pStats[j].unitType==REMAINING_UNIT_TYPE)
		error=false;			
	if((pStats[unit].create>0)&&(pStats[unit].create==j)&&(getIsGoal(pStats[unit].create))&&(pStats[unit].create!=unit))
		error = true;
	return(error);
}
// -------------------------------
// ------ SET/GET FUNCTIONS ------
// -------------------------------


const std::list<GOAL> GOAL_ENTRY::tryToAddGoal(const unsigned int unit, const unsigned int time, const unsigned int location, const signed int count)
{
	std::list<GOAL> prefList;
#ifdef _SCC_DEBUG
	if((unit==0)||(unit>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: (GOAL_ENTRY::tryToAddGoal): Value unitType out of range.");return(prefList);
	}
#endif
	for(std::list<GOAL>::iterator i=goal.begin(); i!=goal.end(); ++i)
		if((i->getUnit() == unit) && (i->getLocation() == location)&&(i->getTime() == time))
		{
			if(count > 0)
			{
				// too much units? (upgrades, researches)
				if(((pStats[unit].upgrade[0]>0)&&(i->getCount() + count>3))||
				   ((pStats[unit].upgrade[0]==0)&&(pStats[unit].unitType == RESEARCH_UNIT_TYPE)&&(i->getCount() + count > 1)))
				{
					prefList.push_back(*i);
					return(prefList);
				}
				i->setCount(i->getCount() + count);
				return(prefList);
			} else if(count < 0)
			{

				if(i->getCount() + count > 0)
				{
					i->setCount(i->getCount() + count);
					return(prefList);
				}
				else // -> delete!
				{
					std::list<GOAL>::iterator temp = i;
					++i;
					for(; i!=goal.end(); ++i)
						if((i->getUnit() == unit) && (i->getLocation() == location))
						{
							// anderes goal noch verfuegbar, keine Pruefung auf Voraussetzungen noetig
							i = goal.erase(i);
							setIsBuildable(unit, false);
							return(prefList);
						}
					i = temp;
					
// diesen Loeschen, aber vorher noch schauen ob ein anderes Goal das Gebaeude abdeckt!
// Nein, dies ist das einzige Gebaeude
// => pruefen ob andere Goals das Goal benoetigen
					
					for(unsigned int j=UNIT_TYPE_COUNT;j--;)
						if((getIsGoal(j)))//||(getIsBuildable(i)))
						{
							if(isError(j, unit))
							{
								GOAL goal_error;
								goal_error.setUnit(j);
								goal_error.setLocation(GLOBAL);
								goal_error.setTime(0);
								goal_error.setCount(1);
								prefList.push_back(goal_error);
							}
						}
					// kein Fehler? -> Loeschen
					if(!prefList.size())
					{
						i = goal.erase(i);
						setIsGoal(unit, false);
						setIsBuildable(unit, false);
					}
					return(prefList);
				}
			}
		}
// nothing found? => create new goal
#if _SCC_DEBUG
	if(count<=0)
	{
		toLog("DEBUG: (GOAL_ENTRY::tryToAddGoal): Tried to remove units that does not exist.");return(prefList);
	}
#endif
	addNewGoalToList(unit, time, location, count);
	return(prefList);
}

void GOAL_ENTRY::addNewGoalToList(const unsigned int unit, const unsigned int time, const unsigned int location, const signed int count)
{
	if((race == ZERG)&&(unit == LARVA))
		return;
	setIsGoal(unit, true);
	GOAL new_goal;
	new_goal.setUnit(unit);
	new_goal.setTime(time);
	new_goal.setLocation(location);
	new_goal.setCount(count);
	new_goal.setFinalTime(coreConfiguration.getMaxTime());
	goal.push_back(new_goal);
	changed=true;
}

