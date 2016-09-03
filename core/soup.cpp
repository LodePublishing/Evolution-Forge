#include "soup.hpp"
#include <sstream>

SOUP::SOUP() :
	mapPlayerNum(0),
	mapPlayerNumInitialized(false)
{
	memset(buildOrder, 0, MAX_PROGRAMS * sizeof(BUILDORDER*));
	memset(analyzedBuildOrder, 0, MAX_PLAYER * sizeof(ANABUILDORDER*));
}

SOUP::~SOUP()
{
	toInitLog("* Freeing build orders..."); // TODO... :/
	for(unsigned int i=MAX_PROGRAMS;i--;)
		delete buildOrder[i];
	toInitLog("* Freeing analyzed data..."); // TODO... :/
	for(unsigned int i=MAX_PLAYER;i--;)
		delete analyzedBuildOrder[i];
}


void SOUP::initSoup(unsigned int player_number, START* start)
{
#ifdef _SCC_DEBUG
	if(!mapPlayerNumInitialized) {
		toErrorLog("DEBUG (SOUP::initSoup()): Variable mapPlayerNum not initialized.");return;
	}
	if(player_number >= mapPlayerNum) {
		toErrorLog("DEBUG (SOUP::initSoup()): Value player_number out of range.");return;
	}
#endif

	unsigned int groupSize = MAX_PROGRAMS / mapPlayerNum;
//differenzieren, damit auch restarts/updates moeglich sind waehrend dem run! TODO
	for(unsigned int i = groupSize; i--;)
	{
		if(buildOrder[i + player_number*groupSize] == NULL)
			buildOrder[i + player_number*groupSize]=new BUILDORDER();
		else
			buildOrder[i + player_number*groupSize]->resetData();
		buildOrder[i + player_number*groupSize]->assignStart(start);
		buildOrder[i + player_number*groupSize]->setPlayerNumber(player_number+1);
		buildOrder[i + player_number*groupSize]->assignUnits(&temporaryForce);
	}

	if(analyzedBuildOrder[player_number] == NULL)
		analyzedBuildOrder[player_number] = new ANABUILDORDER();
	else
		analyzedBuildOrder[player_number]->resetData();
	
	analyzedBuildOrder[player_number]->assignStart(start);
	analyzedBuildOrder[player_number]->setPlayerNumber(player_number+1);
	analyzedBuildOrder[player_number]->assignUnits(&temporaryForce);
}

void SOUP::initSoup(START* (*start)[MAX_INTERNAL_PLAYER])
{
#ifdef _SCC_DEBUG
	if(!mapPlayerNumInitialized) {
		toErrorLog("DEBUG (SOUP::initSoup()): Variable mapPlayerNum not initialized.");return;
	}
#endif
	unsigned int groupSize = MAX_PROGRAMS / mapPlayerNum;
	
	unsigned int player;
	for(player = mapPlayerNum; player--;)
	{
		for(unsigned int i = groupSize; i--;)
		{
			if(buildOrder[i + player*groupSize] == NULL)
				buildOrder[i + player*groupSize]=new BUILDORDER();
			else 
				buildOrder[i + player*groupSize]->resetData();
			buildOrder[i + player*groupSize]->assignStart((*start)[player+1]);
			buildOrder[i + player*groupSize]->setPlayerNumber(player+1);
			buildOrder[i + player*groupSize]->assignUnits(&temporaryForce);
		}
		if(analyzedBuildOrder[player] == NULL)
			analyzedBuildOrder[player] = new ANABUILDORDER();
		else
			analyzedBuildOrder[player]->resetData();
		analyzedBuildOrder[player]->assignStart((*start)[player+1]);
		analyzedBuildOrder[player]->setPlayerNumber(player+1);
		analyzedBuildOrder[player]->assignUnits(&temporaryForce);
	}
	for(player = mapPlayerNum; player < MAX_PLAYER; ++player)
	{
		delete analyzedBuildOrder[player];
		analyzedBuildOrder[player] = NULL;
	}
}


//TODO: Ueber Optionen einstellen, welche Fitness ueberhaupt bzw. wie stark gewertet wird (oder ob z.B. die Fitnesswerte zusammengeschmissen werden sollen etc.)

struct SoupPlayerDescendingFitnessSort {
        bool operator()(BUILDORDER* const& first_build_order, BUILDORDER* const& last_build_order) {
                return ((first_build_order->getpFitness()>last_build_order->getpFitness()) || ((first_build_order->getpFitness()==last_build_order->getpFitness())  &&  ( first_build_order->getsFitness() > last_build_order->getsFitness()) ) || ((first_build_order->getpFitness()==last_build_order->getpFitness())&&(first_build_order->getsFitness() == last_build_order->getsFitness())&&(first_build_order->gettFitness()>last_build_order->gettFitness())) );
        }
};

void SOUP::calculateAnalyzedBuildOrder()
{
#ifdef _SCC_DEBUG
	if(!mapPlayerNumInitialized) {
		toErrorLog("DEBUG (SOUP::calculateAnalyzedBuildOrder()): Variable mapPlayerNum not initialized.");return;
	}
#endif
	for(unsigned int k = mapPlayerNum; k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			analyzedBuildOrder[k]->prepareForNewGeneration();
			analyzedBuildOrder[k]->initializePlayer();
			analyzedBuildOrder[k]->adjustHarvestAllLocations();
		}
// supply is nicht resettet... bzw. falsch... in resetSupply
	bool complete=false;
	while(!complete)
	{
		complete = true;
		for(unsigned int k = mapPlayerNum; k--;)
			if(analyzedBuildOrder[k]->isOptimizing())
				complete &= analyzedBuildOrder[k]->calculateStep();
	}
//		analyzedBuildOrder[0]->backupMap();  backup&&restore map currently off-line!!!
//	} else
//		analyzedBuildOrder[0]->restoreMap();
}

void SOUP::calculateBuildOrder(const unsigned int bo_num)
{
#ifdef _SCC_DEBUG
	if(!mapPlayerNumInitialized) {
		toErrorLog("DEBUG (SOUP::calculateBuildOrder()): Variable mapPlayerNum not initialized.");return;
	}
#endif
// Map mit Startwerten initialisieren, muss JEDEN Durchlauf passieren!! sonst sammeln sich in der statischen loc variable Haufenweise Commando Centers an 8-)
/* copies the precalculated startforce from pStart into our map (temporaryForce[][])
 * this is done 1:1, i.e. with memcpy */
	const unsigned int groupSize=MAX_PROGRAMS/mapPlayerNum;
	//reset code && calculate 
	for(unsigned int k = mapPlayerNum; k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			buildOrder[k*groupSize+bo_num]->setAlwaysBuildWorkers( analyzedBuildOrder[k]->isAlwaysBuildWorkers());
			buildOrder[k*groupSize+bo_num]->setOnlySwapOrders( analyzedBuildOrder[k]->isOnlySwapOrders());
			buildOrder[k*groupSize+bo_num]->prepareForNewGeneration();
			buildOrder[k*groupSize+bo_num]->initializePlayer();
			buildOrder[k*groupSize+bo_num]->adjustHarvestAllLocations();
		
		// evtl noch switch ohne mutation...
			buildOrder[k*groupSize+bo_num]->eraseIllegalCode();
//			buildOrder[k*groupSize+bo_num]->eraseUselessCode(); //TODO Problem beim switchen, falls schon goals gesetzt waren
// preserve buildOrder[0]s genes					
			if(bo_num!=0)
				buildOrder[k*groupSize+bo_num]->mutateGeneCode(/*analyzedBuildOrder[k]->getFixed()*/);
		}
			
	bool complete=false;
	while(!complete)
	{
		complete=true;
		for(unsigned int k=mapPlayerNum;k--;)
			if(analyzedBuildOrder[k]->isOptimizing())
				complete&=buildOrder[k*groupSize+bo_num]->calculateStep();
	}
}


const bool SOUP::newGeneration(ANABUILDORDER* previous_analyzed_buildorder[MAX_PLAYER], const UNIT (*start_force)[MAX_INTERNAL_PLAYER][MAX_LOCATIONS]) 
	//reset: have the goals/settings been changed?
{
#ifdef _SCC_DEBUG
	if(!mapPlayerNumInitialized) {
		toErrorLog("DEBUG (SOUP::newGeneration()): Variable mapPlayerNum not initialized.");return(false);
	}
#endif
// TODO evtl checken ob alles richtig initialisiert wurde
//	if(analyzedBuildOrder[0]->getRun()>=ga->getMaxRuns()) //~~
//		return(0);

	const unsigned int groupSize=MAX_PROGRAMS/mapPlayerNum;

	// Veraendert? Dann zurueckkopieren
/*	for(unsigned int k = mapPlayerNum; k--;)
		if((previous_analyzed_buildorder[k])&&(analyzedBuildOrder[k]->writeProgramBackToCode(previous_analyzed_buildorder[k]->getProgramList()))) // irrefuehrend, previous ist ja dasselbe...
			for(unsigned int i = groupSize;i--;)
				buildOrder[k*groupSize+i]->copyCode(*(analyzedBuildOrder[k]));*/
	// TODO: wenn Ziele veraendert wurden wird u.U. alter falscher Code mit neuem guten Code vermixt (andere phaeno/geno Tabellen)
	// TODO Zurueckkopieren ist auch nur wichtig wenn man Editieren erlaubt

//	PREBUILDORDER::initNoise();

// This part needs the most CPU power:
	for(unsigned int i=groupSize;i--;)
	{
		memcpy(&temporaryForce, &((*start_force)[0][0]), sizeof(temporaryForce));
		calculateBuildOrder(i); // <- this command :o
	}
//NOW: all pFitness of the players are calculated

// SORT players	
	for(unsigned int k=mapPlayerNum;k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			std::sort(buildOrder+k*groupSize, buildOrder+(k+1)*groupSize, SoupPlayerDescendingFitnessSort());
	//NOW: all players are sorted
			
			for(unsigned int i=coreConfiguration.getBreedFactor()*groupSize/100;i--;) // % are replaced by the uber-program :-o
			{
				int l=rand() % (groupSize*coreConfiguration.getBreedFactor()/100) + groupSize*(100-coreConfiguration.getBreedFactor())/100;
                                if((buildOrder[k*groupSize+l]->getpFitness()*1.1<buildOrder[k*groupSize]->getpFitness())||
                                          ((buildOrder[k*groupSize+l]->getpFitness()==buildOrder[k*groupSize]->getpFitness())&&(buildOrder[k*groupSize+l]->getsFitness()*1.1<buildOrder[k*groupSize]->getsFitness()))||
                                          ((buildOrder[k*groupSize+l]->getpFitness()==buildOrder[k*groupSize]->getpFitness())&&(buildOrder[k*groupSize+l]->getsFitness()==buildOrder[k*groupSize]->getsFitness())&&(buildOrder[k*groupSize+l]->gettFitness()*1.1<buildOrder[k*groupSize]->gettFitness())) )
					buildOrder[k*groupSize+l]->copyCode(*buildOrder[k*groupSize]);
			}
		}

// Do we have a new best player?
//
		bool newcalc = false;
                for(unsigned int k=mapPlayerNum;k--;)
                        if(analyzedBuildOrder[k]->isOptimizing())
                        {
                                if((buildOrder[k*groupSize]->getpFitness()>analyzedBuildOrder[k]->getMaxpFitness())||

                                  ((buildOrder[k*groupSize]->getpFitness()>=analyzedBuildOrder[k]->getMaxpFitness())
                                 &&(buildOrder[k*groupSize]->getsFitness()>analyzedBuildOrder[k]->getMaxsFitness()))||

                                  ((buildOrder[k*groupSize]->getpFitness()>=analyzedBuildOrder[k]->getMaxpFitness())
                                 &&(buildOrder[k*groupSize]->getsFitness()>=analyzedBuildOrder[k]->getMaxsFitness())
                                 &&(buildOrder[k*groupSize]->gettFitness()>analyzedBuildOrder[k]->getMaxtFitness())))
                                {

                                        if(buildOrder[k*groupSize]->gettFitness()>analyzedBuildOrder[k]->getMaxtFitness())
                                                analyzedBuildOrder[k]->setMaxtFitness(buildOrder[k*groupSize]->gettFitness());
                                        if(buildOrder[k*groupSize]->getsFitness()>analyzedBuildOrder[k]->getMaxsFitness())
                                        {
                                                analyzedBuildOrder[k]->setMaxsFitness(buildOrder[k*groupSize]->getsFitness());
                                                analyzedBuildOrder[k]->setMaxtFitness(buildOrder[k*groupSize]->gettFitness());
                                        }
                                        if(buildOrder[k*groupSize]->getpFitness()>analyzedBuildOrder[k]->getMaxpFitness())
                                        {
                                                analyzedBuildOrder[k]->setMaxpFitness(buildOrder[k*groupSize]->getpFitness());
                                                analyzedBuildOrder[k]->setMaxsFitness(buildOrder[k*groupSize]->getsFitness());
                                                analyzedBuildOrder[k]->setMaxtFitness(buildOrder[k*groupSize]->gettFitness());
                                        }
                                        newcalc = true;
                                        analyzedBuildOrder[k]->copyCode(*buildOrder[k*groupSize]);
                                }
                        }

//TODO: Kinder sofort neuberechnen
// Anzahl Tournaments pro Spieler: (MAX_PROGRAMS/mapPlayerNum)/(100/coreConfiguration.getCrossOver())
/*	int tournaments=t*coreConfiguration.getCrossOver()/100;
	if(tournaments>0)
	{
//jetzt: sortieren
		for(int k=0;k<mapPlayerNum-1; ++k)
		if(analyzedBuildOrder[k]->isOptimizing())
			for(int i=0;i<tournaments; ++i)
			{
				for(int j=(k*groupSize)+i*(100/ga->getCrossOver());j<(k*groupSize)+(i+1)*(100/ga->getCrossOver()); ++j) //diese (100/ga->crossOver) Programme untereinander sortieren
				{
					for(int l=(k*groupSize)+i*(100/ga->getCrossOver());l<j; ++l)
						if((buildOrder[j]->getpFitness()>buildOrder[l]->getpFitness())||
						  ((buildOrder[j]->getpFitness()==buildOrder[l]->getpFitness())&&(buildOrder[j]->getsFitness()>buildOrder[l]->getsFitness()))))
							  std::swap(buildOrder[l], buildOrder[j]);
				}
			}
//JETZT: Player in z.B. 20er (bei crossOver=5) Gruppen sortiert => besten 2 herausnehmen, schlechtesten 2 ersetzen
																				
																				
//		for(int k=0;k<mapPlayerNum-1; ++k)
//		if(analyzedBuildOrder[k]->isOptimizing())
//			for(int i=0;i<tournaments; ++i)
//			{
//				int p1=i*(100/ga->getCrossOver())+(k*groupSize);
//				int p2=i*(100/ga->getCrossOver())+(k*groupSize)+1; //evtl nur unterschiedlichen nehmen? => phaenocode zusammenzaehlen ~
//				int c1=(i+1)*(100/ga->getCrossOver())+(k*groupSize)-1;
//				int c2=(i+1)*(100/ga->getCrossOver())+(k*groupSize)-2;
//				buildOrder[p1]->crossOver(buildOrder[p2],buildOrder[c1],buildOrder[c2]);
//			}
	}*/
																				
//evtl breed hinter crossover, aber vorher neue Kinder neu berechnen!

	if(newcalc)
	{
		memcpy(&temporaryForce, &((*start_force)[0][0]), sizeof(temporaryForce));
		calculateAnalyzedBuildOrder();

#ifdef _SCC_DEBUG
		for(unsigned int k=mapPlayerNum;k--;)
			if((analyzedBuildOrder[k]->getHarvestedMinerals() != buildOrder[k*groupSize]->getHarvestedMinerals())||
				(analyzedBuildOrder[k]->getHarvestedGas() != buildOrder[k*groupSize]->getHarvestedGas())||
				(analyzedBuildOrder[k]->getTimer() != buildOrder[k*groupSize]->getTimer()))
				{
					std::ostringstream os;
					for(unsigned int i = MAX_LENGTH;i--;)
						os << analyzedBuildOrder[k]->getCode(i);
					toErrorLog("CurrentCode: " + os.str());
					toErrorLog("^^^^^^^^^^^^ WARNING: genetic core and analysis core are not synchronized!");
				}
#endif			


// SOME POST PROCESSING
// CALCULATE FITNESS AVERAGE & VARIANCE
		for(unsigned int k=mapPlayerNum;k--;)
			if(analyzedBuildOrder[k]->isOptimizing())
			{
				analyzedBuildOrder[k]->fitnessAverage=0;
				for(unsigned int i=k*groupSize;i<(k+1)*groupSize; ++i)
					analyzedBuildOrder[k]->fitnessAverage+=buildOrder[i]->getpFitness();
				analyzedBuildOrder[k]->fitnessAverage/=(MAX_PROGRAMS/mapPlayerNum);
				analyzedBuildOrder[k]->fitnessVariance=0;
				
				for(unsigned int i=k*groupSize;i<(k+1)*groupSize; ++i)
				{
					unsigned int z = analyzedBuildOrder[k]->fitnessAverage-buildOrder[i]->getpFitness();
					analyzedBuildOrder[k]->fitnessVariance += (z*z);
				}
				analyzedBuildOrder[k]->fitnessVariance/=MAX_PROGRAMS;
			}
	}
	for(unsigned int i=MAX_PLAYER;i--;)
		previous_analyzed_buildorder[i]=analyzedBuildOrder[i];

	//	~~
	return(true);
//	return(&(analyzedBuildOrder[0])); TODO
}


// just recalculate the soup, don't mutate
const bool SOUP::recalculateGeneration(ANABUILDORDER* previous_analyzed_buildorder[MAX_PLAYER], const UNIT (*start_force)[MAX_INTERNAL_PLAYER][MAX_LOCATIONS], const bool active[MAX_PLAYER]) 
{
#ifdef _SCC_DEBUG
	if(!mapPlayerNumInitialized) {
		toErrorLog("DEBUG (SOUP::recalculateGeneration()): Variable mapPlayerNum not initialized.");return(false);
	}
#endif
//	TODO  Abfragen was initialisiert wurde...
	bool changed_bo=false;
	for(unsigned int k = mapPlayerNum; (k--)&&(!changed_bo);)
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
	toErrorLog("recalculate");
	const unsigned int groupSize=MAX_PROGRAMS/mapPlayerNum;

	for(unsigned int k=mapPlayerNum;k--;)
		if(active[k])
//		if(analyzedBuildOrder[k]->isOnlySwapOrders())  //?
//			for(unsigned int i=k*groupSize;i<(k+1)*groupSize; ++i)
//				buildOrder[i]->copyCode(*analyzedBuildOrder[k]);
//		else
			buildOrder[k*groupSize]->copyCode(*analyzedBuildOrder[k]);
		
	memcpy(&temporaryForce, &((*start_force)[0][0]), sizeof(temporaryForce));

	//reset code && calculate 
	for(unsigned int k = mapPlayerNum; k--;)
		if(active[k])
		{
			buildOrder[k*groupSize]->assignUnits(&temporaryForce); //?
			buildOrder[k*groupSize]->prepareForNewGeneration();
			buildOrder[k*groupSize]->initializePlayer();
			buildOrder[k*groupSize]->adjustHarvestAllLocations();
			// evtl noch switch ohne mutation...
//			buildOrder[k*groupSize]->eraseIllegalCode();
		}
	bool complete=false;
	while(!complete)
	{
		complete=true;
		for(unsigned int k=mapPlayerNum;k--;)
			if(active[k])
				complete&=buildOrder[k*groupSize]->calculateStep();
	}

	for(unsigned int k = mapPlayerNum; k--;)
		if(active[k])
		{
			analyzedBuildOrder[k]->setMaxpFitness(buildOrder[k*groupSize]->getpFitness());
			analyzedBuildOrder[k]->setMaxsFitness(buildOrder[k*groupSize]->getsFitness());
			analyzedBuildOrder[k]->setMaxtFitness(buildOrder[k*groupSize]->gettFitness());
		}
	memcpy(&temporaryForce, &((*start_force)[0][0]), sizeof(temporaryForce));

	for(unsigned int k = mapPlayerNum; k--;)
		if(active[k])
		{
		//	if(previous_analyzed_buildorder[k])
	//		analyzedBuildOrder[k]->writeProgramBackToCode(previous_analyzed_buildorder[k]->getProgramList());
			analyzedBuildOrder[k]->assignUnits(&temporaryForce); //?
			analyzedBuildOrder[k]->prepareForNewGeneration();
			analyzedBuildOrder[k]->initializePlayer();
			analyzedBuildOrder[k]->adjustHarvestAllLocations();
		}
// supply is nicht resettet... bzw. falsch... in resetSupply
	complete=false;
	while(!complete)
	{
		complete = true;
		for(unsigned int k = mapPlayerNum; k--;)
			if(active[k])
				complete&=analyzedBuildOrder[k]->calculateStep();
	}
//		analyzedBuildOrder[0]->backupMap();  backup&&restore map currently off-line!!!
//	} else
//		analyzedBuildOrder[0]->restoreMap();


// SOME POST PROCESSING
// CALCULATE FITNESS AVERAGE & VARIANCE
	for(unsigned int k=mapPlayerNum;k--;)
		if(active[k])
		{
			analyzedBuildOrder[k]->fitnessAverage=0;
			for(unsigned int i=k*groupSize;i<(k+1)*groupSize; ++i)
				analyzedBuildOrder[k]->fitnessAverage+=buildOrder[i]->getpFitness();
			analyzedBuildOrder[k]->fitnessAverage/=(MAX_PROGRAMS/mapPlayerNum);
			analyzedBuildOrder[k]->fitnessVariance=0;
		
			for(unsigned int i=k*groupSize;i<(k+1)*groupSize; ++i)
			{
				unsigned int z = analyzedBuildOrder[k]->fitnessAverage-buildOrder[i]->getpFitness();
				analyzedBuildOrder[k]->fitnessVariance += (z*z);
			}
			analyzedBuildOrder[k]->fitnessVariance/=MAX_PROGRAMS;
		}

// SOME POST PROCESSING
// CALCULATE FITNESS AVERAGE & VARIANCE
	for(unsigned int i=MAX_PLAYER;i--;)
		previous_analyzed_buildorder[i]=analyzedBuildOrder[i];

	//	~~

	
		
	return(true);
//	return(&(analyzedBuildOrder[0])); TODO
}

void SOUP::setMapPlayerNum(const unsigned int map_player_num)
{
#ifdef _SCC_DEBUG
	if((map_player_num < 1) || (map_player_num > MAX_PLAYER)) {
		toErrorLog("DEBUG (SOUP::setMapPlayerNum()): Value map_player_num out of range.");return;
	}
#endif
	mapPlayerNum = map_player_num;
	mapPlayerNumInitialized = true;
}


