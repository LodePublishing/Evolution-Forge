#include "goalentry.hpp"

GOAL_ENTRY::GOAL_ENTRY(const std::string& goal_name, const unsigned int goal_race) :
	goalList(),
	name(goal_name),
	race(goal_race),
	pStats(GAME::race[race].stats),
	maxBuildTypes(0),
	maxBuildTypesAtStart(0),
	changed(true),
	isInitialized(false),
	wasRemoved(999)
{
	resetData();
}


GOAL_ENTRY::~GOAL_ENTRY()
{ }

void GOAL_ENTRY::resetData() {
	wasRemoved = 999;
	maxBuildTypes = 0;
	maxBuildTypesAtStart = 0;
	genoToPhaenotype.assign(RACE::UNIT_TYPE_COUNT, 999);
	phaenoToGenotype.assign(RACE::UNIT_TYPE_COUNT, 999);
	isBuildable.assign(RACE::UNIT_TYPE_COUNT, false);
	isBuildableAtStart.assign(RACE::UNIT_TYPE_COUNT, false);
	isHaveable.assign(RACE::UNIT_TYPE_COUNT, false);
	isCheckedAtStart.assign(RACE::UNIT_TYPE_COUNT, false);
	isGoal.assign(RACE::UNIT_TYPE_COUNT, false);
	genoAtStart.assign(RACE::UNIT_TYPE_COUNT, false);
	needsAtStart = GAME::race[getRace()].needs;
}

const bool GOAL_ENTRY::calculateReady(const UNIT (&units)[MAX_LOCATIONS]) const
{
#ifdef _SCC_DEBUG
	if(!isInitialized) {
		toErrorLog("DEBUG (GOAL_ENTRY::calculateReady()): Goal not initialized");return(true);
	}
#endif
	for(std::list<GOAL>::const_iterator i = goalList.begin(); i != goalList.end(); ++i)
		if(i->getCount())
		{
			if(!( (i->getCount() <= units[i->getLocation()].getTotal(i->getUnit())) && ((i->getTime()==0) || (i->getTime()>=i->getFinalTime())) ))
				return(false);
		}
	return(true);
}

const unsigned int GOAL_ENTRY::calculateFitness(const UNIT (&units)[MAX_LOCATIONS], std::vector<unsigned int> (&bonus)[MAX_LOCATIONS]) const
{
#ifdef _SCC_DEBUG
	if(!isInitialized) {
		toErrorLog("DEBUG (GOAL_ENTRY::calculateFitness): Goal not initialized");return(0);
	}
#endif
	unsigned int tpF = 0;
	std::vector<bool> checked(RACE::UNIT_TYPE_COUNT);
	checked.assign(RACE::UNIT_TYPE_COUNT, false);
	unsigned int bonus_value = 100;
	for(std::list<GOAL>::const_iterator i = goalList.begin(); i!= goalList.end(); ++i)
	{
		unsigned int count = units[i->getLocation()].getTotal(i->getUnit());
		if(!count)
			continue;
		if(!checked[i->getUnit()])
		{
			tpF += bonus_value * bonusTable[i->getUnit()];
			checked[i->getUnit()] = true;
		}				
// goal nicht erfuellt?
		if(i->getCount() > count)
		{
			bonus[i->getLocation()][i->getUnit()] += i->getCount() - count;
		
			if( (i->getTime()>0) && (i->getFinalTime() > i->getTime()))
				tpF += i->getTime() * count * bonus_value / ( i->getCount() * i->getFinalTime() );
			else
				tpF += count * bonus_value / i->getCount();
		}
		else
		{
			if((i->getTime() > 0) && (i->getFinalTime() > i->getTime()))
				tpF += i->getTime() * bonus_value / i->getFinalTime();
			else tpF += bonus_value;
		}
	}

	for(unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
		if(getIsBuildable(i) && (checked[i]==false) && units[GLOBAL].getTotal(i))
		{
			checked[i] = true;
			tpF += bonus_value * bonusTable[i];
		}	
	return(tpF);
}

const bool GOAL_ENTRY::operator==(const GOAL_ENTRY& other) const
{
	if(goalList.size() != other.goalList.size())
		return(false);
	for(std::list<GOAL>::const_iterator i = goalList.begin(); i!=goalList.end(); ++i)
	{
		bool is_equal = false;
		for(std::list<GOAL>::const_iterator j = other.goalList.begin(); (j!=other.goalList.end())&&(!is_equal); ++j)
			if((j->getCount() == i->getCount()) && (j->getUnit() == i->getUnit()) && (j->getTime() == i->getTime()))
				is_equal = true;
		if(!is_equal)
			return(false);
	}
//	toLog(name + " equals " + other.name); TODO, wird zu oft aufgerufen
	return(true);
}

const bool GOAL_ENTRY::hasSimilarGoal(const unsigned int unit, const unsigned int location) 
{
#ifdef _SCC_DEBUG
	if(!isInitialized) {
		toErrorLog("DEBUG (GOAL_ENTRY::hasSimilarGoal()): Goal not initialized");return(true);
	}
#endif
	if(!getIsGoal(unit))
		return(false);
	for(std::list<GOAL>::iterator i=goalList.begin();i!=goalList.end(); ++i)
		if( (i->getUnit() == unit) && (i->getLocation() == location))
			return(true);
	return(false);
}

void GOAL_ENTRY::calculateFinalTimes(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time)
{
#ifdef _SCC_DEBUG
	if(!isInitialized) {
		toErrorLog("DEBUG (GOAL_ENTRY::calculateFinalTimes()): Goal not initialized");return;
	}
#endif
	for(std::list<GOAL>::iterator i=goalList.begin();i!=goalList.end(); ++i)
	{
// ist dieses goal belegt?
		if(( i->getUnit() == unit )&&
		   ( i->getUnit() > 0 )&&
		   ( i->getCount() == count) && // !!
// befinden wir uns am richtigen Ort?
		   (( i->getLocation() == GLOBAL )||( i->getLocation() == location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
			( i->getUnit() == unit ))
			  i->setFinalTime(time);
	}
}

void GOAL_ENTRY::calculateFinalTimesAtBeginning(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time)
{
#ifdef _SCC_DEBUG
	if(!isInitialized) {
		toErrorLog("DEBUG (GOAL_ENTRY::calculateFinalTimesAtBeginning()): Goal not initialized");return;
	}
#endif
	for(std::list<GOAL>::iterator i=goalList.begin();i!=goalList.end(); ++i)
	{
// ist dieses goal belegt?
		if(( i->getUnit() == unit )&&
		   ( i->getUnit() > 0 )&&
		   ( i->getCount() <= count) && // !! DIFFERENCE TO CALCULATEFINALTIMES
// befinden wir uns am richtigen Ort?
		   (( i->getLocation() == GLOBAL )||( i->getLocation() == location))&&
// und untersuchen wir das zum Unittype gehoerende Goal?
			( i->getUnit() == unit ))
			  i->setFinalTime(time);
	}
}

// TODO 
/*void GOAL_ENTRY::setRace(const unsigned int goal_race)
{
#ifdef _SCC_DEBUG
	if(goal_race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (GOAL_ENTRY::setRace()): goal_race out of range.");return;
	}
#endif
	race = goal_race;
//	raceInitialized = true;
	isInitialized = false;
	changed = true;
}*/




// fastest time to reach the goals, ignoring workers, minerals, gas, count etc. < - TODO 
const unsigned int GOAL_ENTRY::calculateFastestBO(const UNIT& startForce) const
{
#ifdef _SCC_DEBUG
	if(!isInitialized) {
		toErrorLog("DEBUG (GOAL_ENTRY::calculateFastestBO()): Goal not initialized");return(true);
	}
#endif
	return(0);
#if 0
	std::vector<unsigned int> min_time(RACE::UNIT_TYPE_COUNT);

	for(unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
		if(startForce.getTotal(i))
			min_time[i] = 0;
		else
			min_time[i] = MAX_TIME;

	for(unsigned int j = 10; j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
		for(unsigned int i = UNIT_TYPE_COUNT; i--;)
			if( (getIsGoal(i)) || (getIsBuildable(i)) )
			{
				unsigned int pre=0;
				for(unsigned int k=0;k<3; ++k)
					if(pStats[i].prerequisite[k])
					{
						if(pre < min_time[pStats[i].prerequisite[k]])// + pStats[pStats[i].prerequisite[k]].BT)
							pre = min_time[pStats[i].prerequisite[k]];// + pStats[pStats[i].prerequisite[k]].BT;
					}
				if(pStats[i].facility2)
				{
					if(pre < min_time[pStats[i].facility2])// + pStats[pStats[i].facility2].BT)
						pre = min_time[pStats[i].facility2];// + pStats[pStats[i].facility2].BT;
				}

				unsigned int fac = 2*MAX_TIME;
				for(unsigned int k = 0;k < 3; ++k)
					if(pStats[i].facility[k])
					{
						if(fac > min_time[pStats[i].facility[k]])// + pStats[pStats[i].facility[k]].BT)
							fac = min_time[pStats[i].facility[k]];//+ pStats[pStats[i].facility[k]].BT;
					}
				if(fac == 2*MAX_TIME) // ~~
					fac = 0;
				if(pre < fac)
					pre = fac;
				if(pre < MAX_TIME)
				{
					min_time[i] = pre + pStats[i].BT;
					if(pStats[i].create)
						min_time[pStats[i].create] = pre + pStats[i].BT;
				}
			}
	unsigned int min = 0;
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		if(getIsGoal(i))
			if(min < min_time[i])
				min = min_time[i];
	return(min);
#endif
}


const GOAL_TREE GOAL_ENTRY::getGoalTree(const UNIT& startForce, const unsigned int currentGoalUnit) const
{
#if 0
	NEED t_need[UNIT_TYPE_COUNT];
	bool t_buildable[UNIT_TYPE_COUNT];
	for(unsigned int i = 1; i < UNIT_TYPE_COUNT; ++i)
	{
		t_buildable[i] = false;
		t_need[i] = getGoal()->getNeed(i);
		t_checked[i] = false;
	}
// erstelle 
	std::list<unsigned int> new_buildable;
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		if(getIsBuildable(i))
		{
			t_buildable[i]=true;
			if((*(getStartCondition()))->getLocationTotal(GLOBAL,i))
			{
				updateBuildableList(getGoal()->getAllow(i), t_need, new_buildable, i);
				t_checked[i] = true;
			}
		}
		for(std::list<unsigned int>::iterator j = new_buildable.begin(); j!=new_buildable.end(); ++j)
		{
			t_buildable[*j]=true;
			t_geno[t_max_build_types] = getGoal()->toGeno(*j);
			t_max_build_types++;
		}
	}	


//TODO hier evtl das schon berechnete 'need' und 'allow' benutzen! ist im grunde dasselbe wie bei mutate und x wie hier bei currentGoalUnit
	
// TODO evtl die neuen Units hell darstellen!	
	if(currentGoalUnit>0)
	{
		buildable[currentGoalUnit]=true;



		int i = getGoal()->toPhaeno(*x);
		std::list<unsigned int> new_buildable;
		updateBuildableList(getGoal()->getAllow(i), t_need, new_buildable, i);
		}
		if((*getpStats())[i].create>0)
		{
			i = (*getpStats())[i].create;
			updateBuildableList(getGoal()->getAllow(i), t_need, new_buildable, i);
		}
		for(std::list<unsigned int>::iterator j = new_buildable.begin(); j != new_buildable.end(); ++j)
			buildable[*j]=true;

/*
		for(unsigned int j = 0; j < 10; ++j)
			for(unsigned int i = 1; i < UNIT_TYPE_COUNT; ++i)
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
					for(l = UNIT_TYPE_COUNT;l--;)
						if(pStats[l].create == pStats[i].facility[0])
							break;
					if(l)
						buildable[l] = true;
				}
			}*/
	}
	

//TODO
#endif
	GOAL_TREE tree;
#if 0
	memset(tree.level, 0, (UNIT_TYPE_COUNT) * sizeof(int));
	memset(tree.coloumn, 0, (UNIT_TYPE_COUNT) * sizeof(int));

	for(unsigned int i=10;i--;)
		tree.width[i]=0;

	unsigned int c=0;
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		if(startForce.getTotal(i))
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
		for(unsigned int i = 1; i < UNIT_TYPE_COUNT; ++i)
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
					for(j = UNIT_TYPE_COUNT;j--;)
						if(pStats[j].create == pStats[i].facility[0])
							break;
					if(j)
						tree.con[i].push_back(j);
				}
			}

		bool tcheck[LAST_UNIT]; wtf
		memset(tcheck, 0, (LAST_UNIT) * sizeof(bool)); // ?

		for(unsigned int i = 1; i < LAST_UNIT; ++i)
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
					for(j = LAST_UNIT;j--;)
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
			
		for(unsigned int i = 1; i < LAST_UNIT; ++i)
			if(tcheck[i])
				tree.checked[i]=true;
		tree.width[k]=c;
	}
#endif
	return(tree);
}

void GOAL_ENTRY::giveBonusTo(const unsigned int unit, unsigned int caller, std::vector< std::vector<bool> >& checked)
{
	if(unit == 0)
		return;
	if(caller == unit)
		return;
	if(checked[unit][caller])
		return;
	checked[unit][caller] = true;
	bonusTable[unit]++;
	giveBonusTo(pStats[unit].facility2, unit, checked);
	giveBonusTo(pStats[unit].facility[0], unit, checked);
	giveBonusTo(pStats[unit].facility[1], unit, checked);
	giveBonusTo(pStats[unit].facility[2], unit, checked);
	giveBonusTo(pStats[unit].prerequisite[0], unit, checked);
	giveBonusTo(pStats[unit].prerequisite[1], unit, checked);
	giveBonusTo(pStats[unit].prerequisite[2], unit, checked);
}

void GOAL_ENTRY::fillBonusTable()
{
	std::vector< std::vector<bool> > checked;
	checked.resize(RACE::UNIT_TYPE_COUNT);
	for(unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
		checked[i].assign(RACE::UNIT_TYPE_COUNT, false);
	
	bonusTable.assign(RACE::UNIT_TYPE_COUNT, 0);
	for(unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
		if(getIsGoal(i)||getIsBuildable(i))
			giveBonusTo(i, 0, checked);
}

void GOAL_ENTRY::fillBuildableListAndAddGoals()
{
	for(unsigned int j=6;j--;) // Nuclear Warhead needs 6 steps (?) ~~~~
	{
		for(unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
			if(getIsGoal(i)||(getIsBuildable(i)))
			{
				setIsBuildable(i);
				//gather all prerequisites and mark them as goals
				for(unsigned int k=3;k--;)
					if((pStats[i].prerequisite[k]) && (getIsGoal(pStats[i].prerequisite[k])==false))
						addNewGoalToList(pStats[i].prerequisite[k], 0, GLOBAL, 1);
				if((pStats[i].facility2) && (!getIsGoal(pStats[i].facility2)) &&
//				   (pStats[i].facilityType != NEEDED_UNTIL_COMPLETE_IS_LOST) &&  ?
				   (pStats[pStats[i].facility2].unitType != INVALID_UNIT_TYPE)) 
				{
				// do not set those morph-facilities as goals...
					if((pStats[i].facilityType != IS_LOST) && (pStats[i].facilityType != IS_MORPHING) &&
				// ... neither the researches...
					(pStats[i].facilityType != NEEDED_UNTIL_COMPLETE_IS_LOST_EVERYWHERE))
						addNewGoalToList(pStats[i].facility2, 0, GLOBAL, 1);
				// ... but make them buildable :)
					else setIsBuildable(pStats[i].facility2);
				}
				unsigned int fac = 0;
				if(((pStats[i].facility[0] > 0)&&(!getIsGoal(pStats[i].facility[0])))&&
				   ((pStats[i].facility[1] == 0)||(!getIsGoal(pStats[i].facility[1])))&&
				   ((pStats[i].facility[2] == 0)||(!getIsGoal(pStats[i].facility[2]))))
					fac = pStats[i].facility[0];
				else
				if(((pStats[i].facility[0] > 0)&&(!getIsGoal(pStats[i].facility[0])))&&
				   ((pStats[i].facility[1] > 0)&&(!getIsGoal(pStats[i].facility[1])))&&
				   ((pStats[i].facility[2] == 0)||(!getIsGoal(pStats[i].facility[2]))))
					fac = pStats[i].facility[1];
				else
				if(((pStats[i].facility[0] > 0) && (!getIsGoal(pStats[i].facility[0])))&&
				   ((pStats[i].facility[1] > 0) && (!getIsGoal(pStats[i].facility[1])))&&
				   ((pStats[i].facility[2] > 0) && (!getIsGoal(pStats[i].facility[2]))))
					fac = pStats[i].facility[2];
				if((fac>0))//&&(pStats[fac].unitType != INVALID_UNIT_TYPE)) TODO?
				{
					if((pStats[i].facilityType==NEEDED_ONCE) || 
					   (pStats[i].facilityType==NEEDED_UNTIL_COMPLETE) ||
					   (pStats[i].facilityType==NEEDED_UNTIL_COMPLETE_IS_LOST) ||
					   (pStats[i].facilityType==NEEDED_ALWAYS))
						addNewGoalToList(fac, 0, GLOBAL, 1);
					else setIsBuildable(fac); // ... same as above...
				}
				
				for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
					if(pStats[i].resource[k]>0)
					{
						setIsBuildable(GAME::race[getRace()].resource[k].gatherer); 
//						setIsBuildable(SCV_FROM_GAS_TO_MINERALS); ?
					}

				if(pStats[i].needSupply)
				{
					for(std::list<unsigned int>::const_iterator k = GAME::race[getRace()].supply.begin(); k != GAME::race[getRace()].supply.end(); ++k)
						setIsBuildable(*k);
				}
			}
	}
}

void GOAL_ENTRY::recheckTooManyGoals()
{
//	now recheck whether we got too many goals
//	alle durchlaufen
//	jeweils alle facilities von rechts nach links anguggn
//	sobald eins gefunden das Teil der goals ist, alle weiter links streichen (aber buildable lassen)
// Beispiel: Wenn wir schon 'Starport[CT]' haben brauchen wir u.U. kein 'Starport' mehr
// facility2?
	for(unsigned int i=RACE::UNIT_TYPE_COUNT; i--;)
		if((getIsGoal(i)) || (getIsBuildable(i)))
		{
			for(unsigned int j=3;j--;)
				if((pStats[i].facility[j]>0)&&(getIsGoal(pStats[i].facility[j])))
				{
					for(unsigned int k=0; k<j; ++k)
					{
						if(/*(allowGoalAdaption)&&*/(getIsGoal(pStats[i].facility[k])))
						{
							for(std::list<GOAL>::iterator l=goalList.begin(); l!=goalList.end(); ++l)
								if(l->getUnit()==pStats[i].facility[k])
									l=goalList.erase(l); //~~
									// evtl addgoal(-1,...	
						}
// TODO wenn Locations wichtig werden!!
					}
					j=0;
				}
		}
	fillBonusTable();
}


void GOAL_ENTRY::addSpecialUnitsToBuildableList()
{
}


void GOAL_ENTRY::calculateBonus(std::vector<unsigned int> (&bonus)[MAX_LOCATIONS]) const
{
	for(std::list<GOAL>::const_iterator i = goalList.begin(); i != goalList.end(); ++i)
	{
		if((i->getTime() == 0) || (i->getCount() == 0))
			continue;
			
		bool ok = true;
		for(unsigned int j = RACE::MAX_RESOURCE_TYPES; j--;)
		{
			if((i->getUnit() == GAME::race[getRace()].resource[j].provider) ||
			   (i->getUnit() == GAME::race[getRace()].resource[j].gatherer))
			{
				ok = false;
				break;
			}
			for(std::list<unsigned int>::const_iterator k = GAME::race[getRace()].resource[j].accepter.begin(); k != GAME::race[getRace()].resource[j].accepter.end(); ++k)
				if(i->getUnit() == *k)
				{
					ok = false;
					break;
				}
			if(!ok)
				break;
		}
		if(!ok)
			continue;
		bonus[i->getLocation()][i->getUnit()] += i->getCount();
	}
}

void GOAL_ENTRY::adjustGoals(const UNIT& start_units)
{
	maxBuildTypes = 0;
	maxBuildTypesAtStart = 0;
	genoToPhaenotype.assign(RACE::UNIT_TYPE_COUNT, 999);
	phaenoToGenotype.assign(RACE::UNIT_TYPE_COUNT, 999);
	isBuildable.assign(RACE::UNIT_TYPE_COUNT, false);
	isBuildableAtStart.assign(RACE::UNIT_TYPE_COUNT, false);
	isHaveable.assign(RACE::UNIT_TYPE_COUNT, false);
	isCheckedAtStart.assign(RACE::UNIT_TYPE_COUNT, false);
	genoAtStart.assign(RACE::UNIT_TYPE_COUNT, false);
	needsAtStart = GAME::race[getRace()].needs;


	std::list<unsigned int> new_buildable;
	for(unsigned int i=RACE::UNIT_TYPE_COUNT; i--;)
		if( (pStats[i].facility[0] == 0) && (pStats[i].facility[1] == 0) && (pStats[i].facility[2] == 0) && (pStats[i].facility2 == 0) && (pStats[i].prerequisite[0] == 0) && (pStats[i].prerequisite[1] == 0) && (pStats[i].prerequisite[2] == 0) && (pStats[i].unitType != INVALID_UNIT_TYPE))
		{
	// add to start and always buildable list:
			setIsBuildable(i);
			new_buildable.push_back(i);
//			setIsBuildableAtStart(i);
		}
	
// ------ SPECIAL ORDER: BREAK UP BUILDING ------
/*	if(getRace() == ZERG)
	{
		setIsBuildable(CREEP_COLONY); // ~~
		setIsBuildable(EXTRACTOR); // ~~
	}*/

	
//	addSpecialUnitsToBuildableList();
	fillBuildableListAndAddGoals();
	recheckTooManyGoals();
// => 'isBuildable' is filled
	for(unsigned int i=RACE::UNIT_TYPE_COUNT;i--;)
		if(getIsBuildable(i))
		{
			genoToPhaenotype[maxBuildTypes] = i;
			phaenoToGenotype[i] = maxBuildTypes;
			++maxBuildTypes;
		}
// => 'maxBuildTypes', 'genoToPhaenotype' and 'phaenoToGenotype' are filled

	// calculate the buildable units based on the start units:

	for(unsigned int i = RACE::UNIT_TYPE_COUNT; i--;)
		if((start_units.getTotal(i)) && (isCheckedAtStart[i] == false))
		{
			if(pStats[i].unitType!=INVALID_UNIT_TYPE)
				setIsHaveable(i);
// if it is marked as singular item then remove entry, don't build this anymore from now on
			GAME::race[getRace()].allows[i].updateBuildableList(needsAtStart, new_buildable, i);
			isCheckedAtStart[i] = true;
			if(getpStats(i).create)
			{
				i = getpStats(i).create;
				if(!isCheckedAtStart[i])
				{
					GAME::race[getRace()].allows[i].updateBuildableList(needsAtStart, new_buildable, i);
					isCheckedAtStart[i]=true;
				}
			}
		}
		
	for(std::list<unsigned int>::iterator j = new_buildable.begin(); j != new_buildable.end(); ++j)
	// only insert those units that are part of the goal list
		if(getIsBuildable(*j)) 
		{
			setIsHaveable(*j);
			setIsBuildableAtStart(*j);
			genoAtStart[maxBuildTypesAtStart] = toGeno(*j);
			++maxBuildTypesAtStart;
		}
	isInitialized = true;
// => 'maxBuildTypesAtStart', 'genoAtStart' and 'buildableAtStart' are filled
}

void GOAL_ENTRY::setIsBuildable(const unsigned int unit, const bool is_buildable) {
#ifdef _SCC_DEBUG
	if(unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::setIsBuildable()): Value unit out of range.");return;
	}
#endif
	isBuildable[unit] = is_buildable;
	isHaveable[unit] = is_buildable;
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
	if(!((pStats[j].facilityType == NEEDED_ONCE) || (pStats[j].facilityType == NEEDED_UNTIL_COMPLETE) || (pStats[j].facilityType == NEEDED_ALWAYS) || (pStats[j].facilityType == NEEDED_UNTIL_COMPLETE_IS_LOST)))
		error = false;

	for(unsigned int k = 0; k < 3; ++k)
		if(pStats[j].prerequisite[k] == unit)
			error = true;
	if((pStats[j].facility2 > 0) && (pStats[j].facility2 == unit) && (pStats[j].facilityType != NEEDED_UNTIL_COMPLETE_IS_LOST))
		error = false;
// und was ist mit anderen locations? mmmh...
//	if((pStats[j].gas>0)&&(unit==REFINERY))
//		error=true;
//	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
//		if(pStats[j].resource[i] > 0)
//		TODO
	if(pStats[j].unitType == INVALID_UNIT_TYPE)
		error = false;			
	if((pStats[unit].create > 0) && (pStats[unit].create == j) && (getIsGoal(pStats[unit].create)) && (pStats[unit].create!=unit))
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
	if((unit==0)||(unit>=RACE::UNIT_TYPE_COUNT)) {
		toErrorLog("DEBUG: (GOAL_ENTRY::tryToAddGoal): Value unitType out of range.");return(prefList);
	}
#endif
	signed goal_count = count;
	if((GAME::race[race].stats[unit].create == unit) && ((count == 1) || (count == -1))) // zerglings, scourges
		goal_count *= 2;
	
	for(std::list<GOAL>::iterator i = goalList.begin(); i != goalList.end(); ++i)
		if((i->getUnit() == unit) && (i->getLocation() == location)&&(i->getTime() == time))
		{
			if(count > 0)
			{
				// TODO ersetzen durch Regel die die buildable-kette der prerequisites prueft, dazu ist die startForce notwendig

//				if(checkBuildable(pStats[unit].prerequisite[0])
				
				// too much units? (upgrades, researches)
				if((pStats[unit].unitType == RESEARCH_UNIT_TYPE)&&(i->getCount() + goal_count > 1))
				{
					prefList.push_back(*i);
					return(prefList);
				}
				i->setCount(i->getCount() + goal_count);
				return(prefList);
			} else if(goal_count < 0)
			{
				// Nur marginale Veraenderung? => Durchfuehren, ok
				if(i->getCount() + goal_count > 0)
				{
					i->setCount(i->getCount() + goal_count);
					return(prefList);
				}
				// auf null setzen? => loeschen!
				else 
				{
					std::list<GOAL>::iterator temp = i;
					++i;
					for(; i!=goalList.end(); ++i)
					// anderes goal mit gleicher unit und location nach Loeschung noch verfuegbar, keine Pruefung auf Voraussetzungen noetig => dieses loeschen und zurueck
						if((i->getUnit() == unit) && (i->getLocation() == location))
						{
							i = goalList.erase(i);
							setIsBuildable(unit, false);
							return(prefList);
						}
					i = temp;
					
// => Nein, dies ist das einzige Gebaeude, also wuerde diese Einheit permanent benoetigt
// => pruefen ob andere Goals diese Einheit noch benoetigen
					
					for(unsigned int j=RACE::UNIT_TYPE_COUNT;j--;)
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
					// => wird von niemandem gebraucht? Dann loeschen!
					if(!prefList.size())
					{
						wasRemoved = toGeno(unit);
						i = goalList.erase(i);
						setIsGoal(unit, false);
						setIsBuildable(unit, false);
					}
					return(prefList);
				}
			}
		}
// nothing found? => create new goal
#if _SCC_DEBUG
	if(goal_count<=0) {
		toErrorLog("DEBUG: (GOAL_ENTRY::tryToAddGoal): Tried to remove units that do not exist.");
		return(prefList);
	}
#endif
	addNewGoalToList(unit, time, location, goal_count);
	return(prefList);
}

const unsigned int GOAL_ENTRY::getWasRemoved() const
{
	return wasRemoved;
}

void GOAL_ENTRY::resetWasRemoved()
{
	wasRemoved = 999;
}

void GOAL_ENTRY::addNewGoalToList(const unsigned int unit, const unsigned int time, const unsigned int location, const signed int count)
{
	setIsGoal(unit, true);
	GOAL new_goal;
	new_goal.setUnit(unit);
	new_goal.setTime(time);
	new_goal.setLocation(location);
	new_goal.setCount(count);
	new_goal.setFinalTime(coreConfiguration.getMaxTime());
	goalList.push_back(new_goal);
	changed = true;
}

GOAL_ENTRY::GOAL_ENTRY(const GOAL_ENTRY& object):
	name(object.name),
	race(object.race),
	pStats(object.pStats),
	maxBuildTypes(object.maxBuildTypes),
	maxBuildTypesAtStart(object.maxBuildTypesAtStart),
	changed(object.changed),
	isInitialized(object.isInitialized)
{
	goalList.clear();
	for(std::list<GOAL>::const_iterator i = object.goalList.begin();i!=object.goalList.end();++i)
		this->goalList.push_back(*i); // optimize?
	genoToPhaenotype = object.genoToPhaenotype;
	phaenoToGenotype = object.phaenoToGenotype;
	isCheckedAtStart = object.isCheckedAtStart;
	isBuildable = object.isBuildable;
	isHaveable = object.isHaveable;
	isGoal = object.isGoal;
	genoAtStart = object.genoAtStart;
	needsAtStart = object.needsAtStart;

//	resetData();
}

GOAL_ENTRY& GOAL_ENTRY::operator=(const GOAL_ENTRY& object)
{
	name = object.name;
	race = object.race;
	maxBuildTypes = object.maxBuildTypes;
	pStats = object.pStats;
	changed = object.changed;
	goalList.clear();
	for(std::list<GOAL>::const_iterator i = object.goalList.begin();i!=object.goalList.end();++i)
		this->goalList.push_back(*i); // optimize?
	
	genoToPhaenotype = object.genoToPhaenotype;
	phaenoToGenotype = object.phaenoToGenotype;

	isBuildable = object.isBuildable;
	isHaveable =object.isHaveable;
	isGoal = object.isGoal;
		
	return(*this);
}
