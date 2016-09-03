#include "soup.hpp"

SOUP::SOUP() :
	mapPlayerNum(0),
	runNumber(0),
	newcalc(true),
	isNewRun(false)
{
	memset(buildOrder, 0, MAX_PROGRAMS * sizeof(BUILDORDER*));
	memset(analyzedBuildOrder, 0, MAX_PLAYER * sizeof(ANABUILDORDER*));
//	buildOrderInitialized=0;
//	goalCount=0;
//	goalsInitialized=0;
}

SOUP::~SOUP()
{
	for(unsigned int i=MAX_PROGRAMS;i--;)
		delete buildOrder[i];
	for(unsigned int i=MAX_PLAYER;i--;)
		delete analyzedBuildOrder[i];
}

SOUP::SOUP(const SOUP& object):
	mapPlayerNum(object.mapPlayerNum),
	runNumber(object.runNumber),
	newcalc(object.newcalc),
	isNewRun(object.isNewRun)
{
	for(unsigned int i=MAX_PROGRAMS;i--;)
		if(buildOrder[i]!=NULL)
			buildOrder[i] = new BUILDORDER(*(object.buildOrder[i]));
	for(unsigned int i=MAX_PLAYER;i--;)
		if(analyzedBuildOrder[i]!=NULL)
			analyzedBuildOrder[i] = new ANABUILDORDER(*(object.analyzedBuildOrder[i]));
}

SOUP& SOUP::operator=(const SOUP& object)
{
	mapPlayerNum = object.mapPlayerNum;
	runNumber = object.runNumber;
	newcalc = object.newcalc;
	isNewRun = object.isNewRun;
	for(unsigned int i=MAX_PROGRAMS;i--;)
		if(buildOrder[i]!=NULL)
			buildOrder[i] = new BUILDORDER(*(object.buildOrder[i]));
	for(unsigned int i=MAX_PLAYER;i--;)
		if(analyzedBuildOrder[i]!=NULL)
			analyzedBuildOrder[i] = new ANABUILDORDER(*(object.analyzedBuildOrder[i]));
	return(*this);
}

ANABUILDORDER* SOUP::getAnalyzedBuildOrder(const unsigned int player_num)
{
	return(analyzedBuildOrder[player_num]);
}

void SOUP::initSoup(START* (*start)[MAX_INTERNAL_PLAYER])
{
/* benoetigt folgende initialisierte Daten:
   mapPlayerNum
*/
#ifdef _SCC_DEBUG
/*f(player_number >= mapPlayerNum)
	{
		toLog("ERROR: (SOUP::initSoup) player_number out of range.");
		return;
	}*/
/*	if(!mapInitialized)
	{
		toLog("ERROR: (SOUP::initSoup) Map not initialized.");
		return;
	}
	if(!goalsInitialized)
	{
		toLog("ERROR: (SOUP::initSoup) Goals not initialized.");
		return;
	}
	*/
#endif
//	if(buildOrderInitialized)
//	{
//		toLog("ERROR: (SOUP::initSoup) SOUP is already initialzed.");
//		return(0);
//	}
//	PREBUILDORDER::resetMapInitialized();
/*	if(!(PREBUILDORDER::setMap(pMap)))
	{
		toLog("ERROR: (SOUP::initSoup) Map not initialized.");
		return(0);
	}*/
	unsigned int groupSize = MAX_PROGRAMS / mapPlayerNum;
	
//differenzieren, damit auch restarts/updates moeglich sind waehrend dem run! TODO
	
	unsigned int k;
	for(k = mapPlayerNum; k--;)
	{
		for(unsigned int i = groupSize; i--;)
		{
			if(buildOrder[i+k*groupSize])
				buildOrder[i+k*groupSize]->resetData();
			else 
			{
				delete buildOrder[i+k*groupSize];
				buildOrder[i+k*groupSize]=new BUILDORDER();
			}
			buildOrder[i+k*groupSize]->assignStart((*start)[k+1]);
			buildOrder[i+k*groupSize]->setPlayerNumber(k+1);
			buildOrder[i+k*groupSize]->resetGeneCode();
			buildOrder[i+k*groupSize]->assignUnits(&temporaryForce);
		}
		if(analyzedBuildOrder[k])
			analyzedBuildOrder[k]->resetData();
		else
		{
			delete analyzedBuildOrder[k];
			analyzedBuildOrder[k] = new ANABUILDORDER();
			analyzedBuildOrder[k]->assignUnits(&temporaryForce);
		}
		analyzedBuildOrder[k]->assignStart((*start)[k+1]);
		analyzedBuildOrder[k]->setPlayerNumber(k+1);
	}
	for(k = mapPlayerNum; k < MAX_PLAYER; k++)
	{
		delete analyzedBuildOrder[k];
		analyzedBuildOrder[k] = NULL;
	}
}

void SOUP::setMapPlayerNum(const unsigned int map_player_num)
{
#ifdef _SCC_DEBUG
	if((map_player_num < 1) || (map_player_num > MAX_PLAYER)) {
		toLog("DEBUG: (SOUP::setMapPlayerNum): map_player_num not initialized.");return;
	}
#endif
	mapPlayerNum = map_player_num;
	// TODO irgendwie von hier initSoup aufrufen
}


//TODO: Ueber Optionen einstellen, welche Fitness ueberhaupt bzw. wie stark gewertet wird (oder ob z.B. die Fitnesswerte zusammengeschmissen werden sollen etc.)

void SOUP::checkForChange() const
{
//	unsigned int changed=0;
/*	for(unsigned int k=mapPlayerNum-1;k--;)
		if(analyzedBuildOrder[k]->getPlayer()->isChanged())
		{
			changed=1;
//			analyzedBuildOrder[k]->getPlayer()->changeAccepted(); TODO
			analyzedBuildOrder[k]->setTotalGeneration(0); //?
			analyzedBuildOrder[k]->setMaxpFitness(0);
			analyzedBuildOrder[k]->setMaxsFitness(0);
			analyzedBuildOrder[k]->setUnchangedGenerations(0);
		}
	if(changed)
		calculateAnalyzedBuildOrder(); // TODO */
}


struct SoupPlayerDescendingFitnessSort {
        bool operator()(BUILDORDER* const& first_build_order, BUILDORDER* const& last_build_order) {
                return ((first_build_order->getpFitness()>last_build_order->getpFitness()) || ((first_build_order->getpFitness()==last_build_order->getpFitness())  &&  ( first_build_order->getsFitness() > last_build_order->getsFitness()) ) || ((first_build_order->getpFitness()==last_build_order->getpFitness())&&(first_build_order->getsFitness() == last_build_order->getsFitness())&&(first_build_order->gettFitness()>last_build_order->gettFitness())) );
        }
};

void SOUP::calculateAnalyzedBuildOrder()
{
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
				complete&=analyzedBuildOrder[k]->calculateStep();
	}
//		analyzedBuildOrder[0]->backupMap();  backup&&restore map currently off-line!!!
//	} else
//		analyzedBuildOrder[0]->restoreMap();
}

void SOUP::calculateBuildOrder(const unsigned int bo_num)
{
// Map mit Startwerten initialisieren, muss JEDEN Durchlauf passieren!! sonst sammeln sich in der statischen loc variable Haufenweise Commando Centers an 8-)
/* copies the precalculated startforce from pStart into our map (temporaryForce[][])
 * this is done 1:1, i.e. with memcpy */
	const unsigned int groupSize=MAX_PROGRAMS/mapPlayerNum;

	//reset code && calculate 
	for(unsigned int k = mapPlayerNum; k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			buildOrder[k*groupSize+bo_num]->assignUnits(&temporaryForce);
			buildOrder[k*groupSize+bo_num]->prepareForNewGeneration();
			buildOrder[k*groupSize+bo_num]->initializePlayer();
			buildOrder[k*groupSize+bo_num]->adjustHarvestAllLocations();
		
		// evtl noch switch ohne mutation...
			buildOrder[k*groupSize+bo_num]->eraseIllegalCode();
//			buildOrder[k*groupSize+bo_num]->eraseUselessCode(); //TODO Problem beim switchen, falls schon goals gesetzt waren
// preserve buildOrder[0]s genes					
			if((bo_num!=0)&&(rand()%2))
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


const bool SOUP::newGeneration(ANABUILDORDER* previous_analyzed_buildorder[MAX_PLAYER], const UNIT (*startForce)[MAX_INTERNAL_PLAYER][MAX_LOCATIONS]) //reset: have the goals/settings been changed?
{
#ifdef _SCC_DEBUG
	/*if(!mapInitialized)
	{
		toLog("ERROR: (SOUP::newGeneration) Map not initialized.");
		return(0);
	}
	if(!goalsInitialized)
	{
		toLog("ERROR: (SOUP::newGeneration) Goals not initialized.");
		return(0);
	}
	*/
#endif
//	if(analyzedBuildOrder[0]->getRun()>=ga->getMaxRuns()) //~~
//		return(0);
	const unsigned int groupSize=MAX_PROGRAMS/mapPlayerNum;

//	PREBUILDORDER::initNoise();
// Set players on given code and parameters
/*	for(int j=mapPlayerNum;j--;)
		if(previous_analyzed_buildorder[j]) //TODO
		{
			for(int i=MAX_LENGTH;i--;)
			{
				buildOrder[j*groupSize]->Code[i]=previous_analyzed_buildorder[j]->Code[i];
				buildOrder[j*groupSize]->Marker[i]=previous_analyzed_buildorder[j]->Marker[i];
			}
//			analyzedBuildOrder[j]->setOptimizing(previous_analyzed_buildorder[j]->isOptimizing());
		}
	if(!previous_analyzed_buildorder[0])
	{
		analyzedBuildOrder[0]->setOptimizing(true);
	}*/
	

// check whether goals etc. have changed
//		checkForChange();


	for(unsigned int i=groupSize;i--;)
	{
		memcpy(&temporaryForce, &((*startForce)[0][0]), sizeof(temporaryForce));
		calculateBuildOrder(i);
	}
//NOW: all pFitness of the players are calculated

// SORT players	
	for(unsigned int k=mapPlayerNum;k--;)
		if(analyzedBuildOrder[k]->isOptimizing())
		{
			std::sort(buildOrder+k*groupSize, buildOrder+(k+1)*groupSize, SoupPlayerDescendingFitnessSort());
	//NOW: all players are sorted
			
			for(int i=coreConfiguration.getBreedFactor()*groupSize/100;i--;) // % are replaced by the uber-program :-o
			{
				int l=rand() % (groupSize*coreConfiguration.getBreedFactor()/100) + groupSize*(100-coreConfiguration.getBreedFactor())/100;
                                if((buildOrder[k*groupSize+l]->getpFitness()*1.1<buildOrder[k*groupSize]->getpFitness())||
                                          ((buildOrder[k*groupSize+l]->getpFitness()==buildOrder[k*groupSize]->getpFitness())&&(buildOrder[k*groupSize+l]->getsFitness()*1.1<buildOrder[k*groupSize]->getsFitness()))||
                                          ((buildOrder[k*groupSize+l]->getpFitness()==buildOrder[k*groupSize]->getpFitness())&&(buildOrder[k*groupSize+l]->getsFitness()==buildOrder[k*groupSize]->getsFitness())&&(buildOrder[k*groupSize+l]->gettFitness()*1.1<buildOrder[k*groupSize]->gettFitness())) )
					buildOrder[k*groupSize+l]->copyCode(*buildOrder[k*groupSize]);

/*				for(int j=MAX_LENGTH;j--;)
				{
					buildOrder[k*groupSize+l]->Code[j]=buildOrder[k*groupSize]->Code[j];
					buildOrder[k*groupSize+l]->Marker[j]=buildOrder[k*groupSize]->Marker[j];

	//				  memcpy(buildOrder[t]->Code[0],buildOrder[0]->Code[0],MAX_LENGTH); 
	//				  memcpy(buildOrder[t]->Code[1],buildOrder[0]->Code[1],MAX_LENGTH);
				}*/
			}
		}

// Do we have a new best player?
		newcalc = false;
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
//                                        analyzedBuildOrder[k]->setUnchangedGenerations(0); TODO
                                        newcalc = true;
                                        analyzedBuildOrder[k]->copyCode(*buildOrder[k*groupSize]);
/*                                      for(int i=MAX_LENGTH;i--;)
                                        {
        // assign the 'best of breed' to analyzedBuildOrder
                                                analyzedBuildOrder[k]->Code[i]=buildOrder[k*groupSize]->Code[i];
                                                analyzedBuildOrder[k]->Marker[i]=buildOrder[k*groupSize]->Marker[i];
                                        //memcpy(analyzedBuildOrder[j]->Code[0],buildOrder[j*MAX_PROGRAMS/2]->Code[0],MAX_LENGTH*4);
                                        //memcpy(analyzedBuildOrder[j]->Code[1],buildOrder[j*MAX_PROGRAMS/2]->Code[1],MAX_LENGTH*4);
                                        }*/
                                }
                        }

//TODO: Kinder sofort neuberechnen
// Anzahl Tournaments pro Spieler: (MAX_PROGRAMS/mapPlayerNum)/(100/coreConfiguration.getCrossOver())
/*	int tournaments=t*coreConfiguration.getCrossOver()/100;
	if(tournaments>0)
	{
//jetzt: sortieren
		for(int k=0;k<mapPlayerNum-1;k++)
		if(analyzedBuildOrder[k]->isOptimizing())
			for(int i=0;i<tournaments;i++)
			{
				for(int j=(k*groupSize)+i*(100/ga->getCrossOver());j<(k*groupSize)+(i+1)*(100/ga->getCrossOver());j++) //diese (100/ga->crossOver) Programme untereinander sortieren
				{
					for(int l=(k*groupSize)+i*(100/ga->getCrossOver());l<j;l++)
						if((buildOrder[j]->getpFitness()>buildOrder[l]->getpFitness())||
						  ((buildOrder[j]->getpFitness()==buildOrder[l]->getpFitness())&&(buildOrder[j]->getsFitness()>buildOrder[l]->getsFitness()))))
							  std::swap(buildOrder[l], buildOrder[j]);
				}
			}
//JETZT: Player in z.B. 20er (bei crossOver=5) Gruppen sortiert => besten 2 herausnehmen, schlechtesten 2 ersetzen
																				
																				
//		for(int k=0;k<mapPlayerNum-1;k++)
//		if(analyzedBuildOrder[k]->isOptimizing())
//			for(int i=0;i<tournaments;i++)
//			{
//				int p1=i*(100/ga->getCrossOver())+(k*groupSize);
//				int p2=i*(100/ga->getCrossOver())+(k*groupSize)+1; //evtl nur unterschiedlichen nehmen? => phaenocode zusammenzaehlen ~
//				int c1=(i+1)*(100/ga->getCrossOver())+(k*groupSize)-1;
//				int c2=(i+1)*(100/ga->getCrossOver())+(k*groupSize)-2;
//				buildOrder[p1]->crossOver(buildOrder[p2],buildOrder[c1],buildOrder[c2]);
//			}
	}*/
																				
//evtl breed hinter crossover, aber vorher neue Kinder neu berechnen!

//	if(newcalc)
//	{
	memcpy(&temporaryForce, &((*startForce)[0][0]), sizeof(temporaryForce));
	calculateAnalyzedBuildOrder();

// SOME POST PROCESSING
// CALCULATE FITNESS AVERAGE & VARIANCE
	for(unsigned int k=mapPlayerNum;k--;)
		if(analyzedBuildOrder[k]->isOptimizing()) //~~ TODO evtl isOptimizing stattdessen...
		{
			analyzedBuildOrder[k]->fitnessAverage=0;
			for(unsigned int i=k*groupSize;i<(k+1)*groupSize;i++)
				analyzedBuildOrder[k]->fitnessAverage+=buildOrder[i]->getpFitness();
			analyzedBuildOrder[k]->fitnessAverage/=(MAX_PROGRAMS/mapPlayerNum);
			analyzedBuildOrder[k]->fitnessVariance=0;
			
			for(unsigned int i=k*groupSize;i<(k+1)*groupSize;i++)
			{
				unsigned int z = analyzedBuildOrder[k]->fitnessAverage-buildOrder[i]->getpFitness();
				analyzedBuildOrder[k]->fitnessVariance += (z*z);
			}
			analyzedBuildOrder[k]->fitnessVariance/=MAX_PROGRAMS;
//			analyzedBuildOrder[k]->analyzeBuildOrder(); TODO?
//			analyzedBuildOrder[k]->getPlayer()->getGoal()->bestTime=analyzedBuildOrder[k]->getTimer(); TODO !!
//			analyzedBuildOrder[k]->setUnchangedGenerations(analyzedBuildOrder[k]->getUnchangedGenerations()+1);
//			analyzedBuildOrder[k]->setTotalGeneration(analyzedBuildOrder[k]->getTotalGeneration()+1); TODO
#if 0
if(analyzedBuildOrder[k]->getUnchangedGenerations()>=coreConfiguration.getMaxGenerations())
			{
				for(unsigned int i=k*groupSize;i<(k+1)*groupSize;i++)
					buildOrder[i]->resetGeneCode();

				analyzedBuildOrder[k]->setRun(analyzedBuildOrder[k]->getRun()+1);
				analyzedBuildOrder[k]->setTotalGeneration(0);
				analyzedBuildOrder[k]->setMaxpFitness(0);
				analyzedBuildOrder[k]->setMaxsFitness(0);
				analyzedBuildOrder[k]->setMaxtFitness(0);
				analyzedBuildOrder[k]->setUnchangedGenerations(0);

				isNewRun=true;

				

			return(&(analyzedBuildOrder[0])/*Save[analyzedBuildOrder[k]->getRun()]*/); //~~~~
		}
	#endif
	}
	for(unsigned int i=MAX_PLAYER;i--;)
		previous_analyzed_buildorder[i]=analyzedBuildOrder[i];

	//	~~
	return(true);
//	return(&(analyzedBuildOrder[0])); TODO
}

const bool SOUP::getIsNewRun()
{
	if(isNewRun)
	{
		isNewRun = false;
		return(true);
	} else return(false);
}

