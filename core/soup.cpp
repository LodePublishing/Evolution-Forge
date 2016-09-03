#include "soup.hpp"
#include "database.hpp"
#include <sstream>

// analyzedBO direkt in GAME spaeter

SOUP::SOUP() :
	map(NULL),
	mapInitialized(false),
	mapPlayerCount(999),
	mapPlayerCountInitialized(false)
{
	memset(buildOrder, 0, MAX_PROGRAMS * sizeof(BUILDORDER*));
	memset(analyzedBuildOrder, 0, MAX_PLAYER * sizeof(ANABUILDORDER*));
		
	for(unsigned int i = MAX_PLAYER; i--;)
		start[i] = new START(startForce[i]);
}

SOUP::~SOUP()
{
	toInitLog("* Freeing build orders..."); // TODO... :/
	for(unsigned int i=MAX_PROGRAMS;i--;)
		delete buildOrder[i];
	toInitLog("* Freeing analyzed data..."); // TODO... :/
	for(unsigned int i=MAX_PLAYER;i--;)
		delete analyzedBuildOrder[i];

	toInitLog(" Freeing start data..."); // TODO ... :|
	for(unsigned int i=MAX_PLAYER;i--;)
		delete start[i];
}


void SOUP::initSoup(unsigned int player_number)
{
#ifdef _SCC_DEBUG
	if(player_number >= getMapPlayerCount()) {
		toErrorLog("DEBUG (SOUP::initSoup()): Value player_number out of range.");return;
	}
#endif
	unsigned int groupSize = MAX_PROGRAMS / getMapPlayerCount();
	unsigned int first_player = player_number * groupSize;
	for(unsigned int i = groupSize; i--;)
	{
		if(buildOrder[first_player + i] == NULL)
			buildOrder[first_player + i] = new BUILDORDER(player_number, start[player_number], temporaryForce);
		else
			buildOrder[first_player + i]->resetData();
	}

	if(analyzedBuildOrder[player_number] == NULL)
		analyzedBuildOrder[player_number] = new ANABUILDORDER(player_number, start[player_number], temporaryForce);
	else
		analyzedBuildOrder[player_number]->resetData();
}

void SOUP::initSoup()
{
	unsigned int groupSize = MAX_PROGRAMS / getMapPlayerCount();
	
	for(unsigned int player_number = getMapPlayerCount(); player_number--;)
	{
		unsigned int first_player = player_number * groupSize;
		for(unsigned int i = groupSize; i--;)
		{
			if(buildOrder[first_player + i] == NULL)
				buildOrder[first_player + i] = new BUILDORDER(player_number, start[player_number], temporaryForce);
			else 
				buildOrder[first_player + i]->resetData();
		}
		if(analyzedBuildOrder[player_number] == NULL)
			analyzedBuildOrder[player_number] = new ANABUILDORDER(player_number, start[player_number], temporaryForce);
		else
			analyzedBuildOrder[player_number]->resetData();
	}
	for(unsigned int player_number = getMapPlayerCount(); player_number < MAX_PLAYER; ++player_number)
	{
		delete analyzedBuildOrder[player_number];
		analyzedBuildOrder[player_number] = NULL;
	}
}

void SOUP::assignGoal(const unsigned int player_num, const unsigned int player_goal)
{
#ifdef _SCC_DEBUG
        if(player_num >= getMapPlayerCount()) {
                toErrorLog("DEBUG (SOUP::assignGoal()): Value player_num out of range.");return;
        }       
#endif
	start[player_num]->assignGoal(database.getGoal(start[player_num]->getPlayerRace(), player_goal));
}

void SOUP::assignRace(const unsigned int player_num, const unsigned int assigned_race)
{
	start[player_num]->setPlayerRace(assigned_race);
	start[player_num]->assignStartCondition(database.getStartCondition(assigned_race, 0)); // assign default startcondition, make a menu later
	if(mapInitialized)
		start[player_num]->fillMapWithStartUnits(); // ? oder alle?
	assignGoal(player_num, 0); // assign default goal
//TODO	
//	setChangedFlag(); ?
	initSoup(player_num);
}


//TODO: Ueber Optionen einstellen, welche Fitness ueberhaupt bzw. wie stark gewertet wird (oder ob z.B. die Fitnesswerte zusammengeschmissen werden sollen etc.)

struct SoupPlayerDescendingFitnessSort {
        bool operator()(BUILDORDER* const& first_build_order, BUILDORDER* const& last_build_order) {
                return ((first_build_order->getpFitness()>last_build_order->getpFitness()) || ((first_build_order->getpFitness()==last_build_order->getpFitness())  &&  ( first_build_order->getsFitness() > last_build_order->getsFitness()) ));
        }
};

void SOUP::calculateAnalyzedBuildOrder()
{
	for(unsigned int k = getMapPlayerCount(); k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			analyzedBuildOrder[k]->eraseIllegalCode();
			analyzedBuildOrder[k]->prepareForNewGeneration();
		}
	
	bool complete=false;
	while(!complete)
	{
		complete = true;
		for(unsigned int k = getMapPlayerCount(); k--;)
			if(analyzedBuildOrder[k]->isOptimizing())
				complete &= analyzedBuildOrder[k]->calculateStep();
	}
	for(unsigned int k = getMapPlayerCount(); k--;)
		analyzedBuildOrder[k]->getGoal()->resetWasRemoved();
}

void SOUP::calculateBuildOrder(const unsigned int bo_num)
{
	const unsigned int groupSize = MAX_PROGRAMS/getMapPlayerCount();
	//reset code && calculate 
	for(unsigned int k = getMapPlayerCount(); k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			buildOrder[k*groupSize+bo_num]->setAlwaysBuildWorkers( analyzedBuildOrder[k]->isAlwaysBuildWorkers());
			buildOrder[k*groupSize+bo_num]->setOnlySwapOrders( analyzedBuildOrder[k]->isOnlySwapOrders());

// !! mutateGeneCode() has to be executed BEFORE prepareForNewGeneration() because IP has to be set to the first item in Code!
			
			buildOrder[k*groupSize+bo_num]->eraseIllegalCode();
	
	// preserve buildOrder[0]s genes					
			if(bo_num!=0)
				buildOrder[k*groupSize+bo_num]->mutateGeneCode();
			buildOrder[k*groupSize+bo_num]->prepareForNewGeneration();
	
		// evtl noch switch ohne mutation...
		}

			
	bool complete = false;
	while(!complete)
	{
		complete = true;
		for(unsigned int k = getMapPlayerCount(); k--;)
			if(analyzedBuildOrder[k]->isOptimizing())
				complete &= buildOrder[k*groupSize+bo_num]->calculateStep();
	}
	
	for(unsigned int k = getMapPlayerCount(); k--;)
		analyzedBuildOrder[k]->getGoal()->resetWasRemoved();
}


const bool SOUP::newGeneration(ANABUILDORDER* previous_analyzed_buildorder[MAX_PLAYER]) 
	//reset: have the goals/settings been changed?
{
	const unsigned int groupSize = MAX_PROGRAMS/getMapPlayerCount();
// This part needs the most CPU power:
	for(unsigned int i = groupSize; i--;)
	{
		for(unsigned int p = MAX_PLAYER; p--;)
			for(unsigned int l = MAX_LOCATIONS; l--;)
				temporaryForce[p][l] = startForce[p][l];
//		memcpy(&temporaryForce, &(startForce[0][0]), sizeof(temporaryForce));
//		toErrorLog(sizeof(temporaryForce));
		calculateBuildOrder(i); // <- this command :o
	}
//NOW: all pFitness of the players are calculated

// SORT players	
	for(unsigned int k = getMapPlayerCount(); k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			unsigned int i = k*groupSize;
			std::stable_sort(buildOrder+i, buildOrder+i+groupSize, SoupPlayerDescendingFitnessSort());
	//NOW: all players are sorted
			
			for(unsigned int b = coreConfiguration.getBreedFactor()*groupSize / 100; b--;) // % are replaced by the uber-program :-o
			{
				int l = rand() % (groupSize*coreConfiguration.getBreedFactor()/100) + groupSize*(100-coreConfiguration.getBreedFactor())/100;
                                if((buildOrder[i+l]->getpFitness()*1.1<buildOrder[i]->getpFitness())||
                                          ((buildOrder[i+l]->getpFitness()==buildOrder[i]->getpFitness())&&(buildOrder[i+l]->getsFitness()*1.1<buildOrder[i]->getsFitness())))
					buildOrder[i+l]->assignCode(*buildOrder[i]);
			}
		}

// Do we have a new best player?
//
		bool newcalc = false;
                for(unsigned int k = getMapPlayerCount(); k--;)
                        if(analyzedBuildOrder[k]->isOptimizing())
                        {
				BUILDORDER& bo = *buildOrder[k*groupSize];
                                if((bo.getpFitness()>analyzedBuildOrder[k]->getMaxpFitness()) || ((bo.getpFitness()==analyzedBuildOrder[k]->getMaxpFitness())&&(bo.getsFitness()>analyzedBuildOrder[k]->getMaxsFitness())) || ((bo.getpFitness()==analyzedBuildOrder[k]->getMaxpFitness())&&(bo.getsFitness()==analyzedBuildOrder[k]->getMaxsFitness()) &&(bo.getLength() < analyzedBuildOrder[k]->getLength())))
                                {
                                        if(bo.getsFitness()>analyzedBuildOrder[k]->getMaxsFitness())
                                                analyzedBuildOrder[k]->setMaxsFitness(bo.getsFitness());
                                        if(bo.getpFitness()>analyzedBuildOrder[k]->getMaxpFitness())
                                        {
                                                analyzedBuildOrder[k]->setMaxpFitness(bo.getpFitness());
                                                analyzedBuildOrder[k]->setMaxsFitness(bo.getsFitness());
                                        }
                                        newcalc = true;
                                        analyzedBuildOrder[k]->assignCode(bo);
                                }
                        }

	if(newcalc)
	{
		for(unsigned int p = MAX_PLAYER; p--;)
			for(unsigned int l = MAX_LOCATIONS; l--;)
				temporaryForce[p][l] = startForce[p][l];

//		memcpy(&temporaryForce, &(startForce[0][0]), sizeof(temporaryForce));
		calculateAnalyzedBuildOrder();

#if 0
#ifdef _SCC_DEBUG
		for(unsigned int k=getMapPlayerCount();k--;)
		{
			if((analyzedBuildOrder[k]->getHarvestedMinerals() != buildOrder[k*groupSize]->getHarvestedMinerals()) ||
				(analyzedBuildOrder[k]->getHarvestedGas() != buildOrder[k*groupSize]->getHarvestedGas()) ||
			((analyzedBuildOrder[k]->getLength() != buildOrder[k*groupSize]->getLength())) ||
			(analyzedBuildOrder[k]->getTimer() != buildOrder[k*groupSize]->getTimer()))
			{
				std::ostringstream os;
/*				{
					std::list<PROGRAM>::const_iterator i;
					std::list<PROGRAM>::const_iterator j;
					std::list<PROGRAM>::const_iterator end;
					if(buildOrder[k*groupSize]->getProgramList().size() > analyzedBuildOrder[k]->getProgramList().size())
					{
						end = analyzedBuildOrder[k]->getProgramList().end();
						i = analyzedBuildOrder[k]->getProgramList().begin();
						j = buildOrder[k*groupSize]->getProgramList().begin();
						toErrorLog("ana / bo");
					}
					else
					{
						end = buildOrder[k*groupSize]->getProgramList().end();
						i = buildOrder[k*groupSize]->getProgramList().begin();
						j = analyzedBuildOrder[k]->getProgramList().begin();
						toErrorLog("bo / ana");
					}
					for(;i != end; ++i, ++j)
					{
						os << "\n" << stats[analyzedBuildOrder[k]->getRace()][i->getUnit()].name << "/" << stats[analyzedBuildOrder[k]->getRace()][j->getUnit()].name << ":" << i->getRealTime() << "/" << j->getRealTime() << " - " << i->before.getHaveMinerals()/100 << "/" << j->before.getHaveMinerals()/100 << "{" << i->successType << "|" << i->successUnit << " - " << j->successType << "|" << j->successUnit << "}";
					
						std::priority_queue<Building, std::vector<Building> > bq1 = i->buildingQueue;
						std::priority_queue<Building, std::vector<Building> > bq2 = j->buildingQueue;
						while((!bq1.empty()) && (!bq2.empty()))
						{
							os << "[" << stats[analyzedBuildOrder[k]->getRace()][bq1.top().getType()].name << "/" << stats[analyzedBuildOrder[k]->getRace()][bq2.top().getType()].name << ":" << bq1.top().getBuildFinishedTime() << "/" << bq2.top().getBuildFinishedTime() << "] ";
							bq1.pop();
							bq2.pop();
						}
					}
					toErrorLog(os.str());
					os.str("");
					for(std::list<unsigned int>::const_iterator l = analyzedBuildOrder[k]->getCode().begin(); l != analyzedBuildOrder[k]->getCode().end(); ++l)
						os << analyzedBuildOrder[k]->getGoal()->toPhaeno(*l) << ", ";
					toErrorLog(os.str());
				}*/
				os.str("");
				os << "Ressources: " << buildOrder[k*groupSize]->getHarvestedMinerals() << " / " << analyzedBuildOrder[k]->getHarvestedMinerals();
				os << "\nTime: " << buildOrder[k*groupSize]->getTimer() << " / " << analyzedBuildOrder[k]->getTimer();
				toErrorLog(os.str());
				toErrorLog("^^^^^^^^^^^^ WARNING: genetic core and analysis core are not synchronized! [time]");
			}
		}
#endif
#endif
	}
	for(unsigned int i = MAX_PLAYER; i--;)
		previous_analyzed_buildorder[i] = analyzedBuildOrder[i];
	return(true);
}

void SOUP::setStartPosition(const unsigned int player_num, const unsigned int player_position) 
{
#ifdef _SCC_DEBUG
        if(player_num >= getMapPlayerCount()) {
                toErrorLog("DEBUG (SOUP::setStartPosition()): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setStartPosition(player_position);
}

void SOUP::assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition) 
{
#ifdef _SCC_DEBUG
	if(player_num >= getMapPlayerCount()) {
		toErrorLog("DEBUG (SOUP::assignStartCondition()): Value player_num out of range.");return;
	}
#endif
	start[player_num]->assignStartCondition(start_condition);
}


void SOUP::assignMap(const BASIC_MAP* basic_map)
{
#ifdef _SCC_DEBUG
	if(basic_map == NULL) {
		toErrorLog("DEBUG (SOUP::assignMap()): Value basic_map not initialized.");return;
	}
#endif
	setMapPlayerCount(basic_map->getMaxPlayer());
	map = basic_map;
	for(unsigned int i = getMapPlayerCount(); i--;)
	{
		start[i]->assignMap(basic_map);
//		if(startConditionInitialized)
//			start[i]->fillMapWithStartUnits(); ? mmh
	}
	mapInitialized = true;
}


void SOUP::setMapPlayerCount(const unsigned int map_player_count)
{
#ifdef _SCC_DEBUG
	if(map_player_count > MAX_PLAYER) {
		toErrorLog("DEBUG (SOUP::setMapPlayerCount()): Value map_player_count out of range.");return;
	}
#endif
	mapPlayerCount = map_player_count;
	mapPlayerCountInitialized = true;
}



// just recalculate the soup, don't mutate
const bool SOUP::recalculateGeneration(ANABUILDORDER* previous_analyzed_buildorder[MAX_PLAYER], const bool active[MAX_PLAYER]) 
{
	bool changed_bo=false;
	for(unsigned int k = getMapPlayerCount(); (k--)&&(!changed_bo);)
		if(active[k])
		{
			if(previous_analyzed_buildorder[k]!=NULL)
			{
				// TODO: bei goal change werden Code und programList ungueltig
				if(previous_analyzed_buildorder[k]->haveConditionsChanged())
				{
					previous_analyzed_buildorder[k]->setConditionsChanged(false);
					changed_bo = true;
				}
//				if(analyzedBuildOrder[k]->writeProgramBackToCode(previous_analyzed_buildorder[k]->getProgramList()))
//					changed_bo=true;
			} else changed_bo=true;
		}
	if(!changed_bo)
		return(false);
	const unsigned int groupSize=MAX_PROGRAMS/getMapPlayerCount();

	for(unsigned int k = getMapPlayerCount(); k--;)
		if(active[k])
//		if(analyzedBuildOrder[k]->isOnlySwapOrders())  //?
//			for(unsigned int i=k*groupSize;i<(k+1)*groupSize; ++i)
//				buildOrder[i]->copyCode(*analyzedBuildOrder[k]);
//		else
			buildOrder[k*groupSize]->assignCode(*analyzedBuildOrder[k]);
	
		for(unsigned int p = MAX_PLAYER; p--;)
			for(unsigned int l = MAX_LOCATIONS; l--;)
				temporaryForce[p][l] = startForce[p][l];

//	memcpy(&temporaryForce, &(startForce[0][0]), sizeof(temporaryForce));

	//reset code && calculate 
	for(unsigned int k = getMapPlayerCount(); k--;)
		if(active[k])
			buildOrder[k*groupSize]->prepareForNewGeneration();
	
	bool complete=false;
	while(!complete)
	{
		complete=true;
		for(unsigned int k = getMapPlayerCount(); k--;)
			if(active[k])
				complete&=buildOrder[k*groupSize]->calculateStep();
	}

	for(unsigned int k = getMapPlayerCount(); k--;)
		if(active[k])
		{
			analyzedBuildOrder[k]->setMaxpFitness(buildOrder[k*groupSize]->getpFitness());
			analyzedBuildOrder[k]->setMaxsFitness(buildOrder[k*groupSize]->getsFitness());
		}
		for(unsigned int p = MAX_PLAYER; p--;)
			for(unsigned int l = MAX_LOCATIONS; l--;)
				temporaryForce[p][l] = startForce[p][l];

//	memcpy(&temporaryForce, &(startForce[0][0]), sizeof(temporaryForce));

	for(unsigned int k = getMapPlayerCount(); k--;)
		if(active[k])
			analyzedBuildOrder[k]->prepareForNewGeneration();
// supply is nicht resettet... bzw. falsch... in resetSupply
	complete=false;
	while(!complete)
	{
		complete = true;
		for(unsigned int k = getMapPlayerCount(); k--;)
			if(active[k])
				complete&=analyzedBuildOrder[k]->calculateStep();
	}
	for(unsigned int i = getMapPlayerCount(); i--;)
		previous_analyzed_buildorder[i] = analyzedBuildOrder[i];
	return(true);
}


