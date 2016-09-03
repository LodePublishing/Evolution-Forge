#include "soup.hpp"


SOUP::SOUP()
{
	run_number=0;
//	playerInitialized=0;
//	goalCount=0;
//	gaInitialized=0;
//	goalsInitialized=0;
	newcalc=1;
}

SOUP::~SOUP()
{
	for(int i=MAX_PROGRAMS;i--;)
		delete player[i];
	for(int i=MAX_PLAYER;i--;)
		delete anaplayer[i];
}
	
void SOUP::initSoup()
{
/* benoetigt folgende initialisierte Daten:
   mapPlayerNum
*/
#ifdef _SCC_DEBUG
/*	if(!mapInitialized)
	{
		toLog("ERROR: (SOUP::initSoup) Map not initialized.");
		return(0);
	}
	if(!goalsInitialized)
	{
		toLog("ERROR: (SOUP::initSoup) Goals not initialized.");
		return(0);
	}
	if(!gaInitialized)
	{
		toLog("ERROR: (SOUP::initSoup) GA not initialized.");
		return(0);
	}*/
#endif
//	if(playerInitialized)
//	{
//		toLog("ERROR: (SOUP::initSoup) SOUP is already initialzed.");
//		return(0);
//	}
//	PRERACE::resetMapInitialized();
/*	if(!(PRERACE::setMap(pMap)))
	{
		toLog("ERROR: (SOUP::initSoup) Map not initialized.");
		return(0);
	}*/
	int groupSize=MAX_PROGRAMS/mapPlayerNum;
	
//differenzieren, damit auch restarts/updates moeglich sind waehrend dem run! TODO
	PRERACE::resetGeneMarker();
	int k;
	for(k=mapPlayerNum;k--;)
	{
		for(int i=groupSize;i--;)
		{
			delete player[i+k*groupSize];
			player[i+k*groupSize]=new RACE();
			player[i+k*groupSize]->setPlayerNum(k+1);
			player[i+k*groupSize]->resetGeneCode();
		}
		delete anaplayer[k];
		anaplayer[k]=new ANARACE();
		anaplayer[k]->setPlayerNum(k+1);
	}
	for(k=mapPlayerNum;k<MAX_PLAYER;k++)
			delete anaplayer[k];
		//what about 'player'?
}


const int compare(const void* a,const void* b)
{
	if(( (*(RACE*)a).getpFitness()<(*(RACE*)b).getpFitness())||(((*(RACE*)a).getpFitness()==(*(RACE*)b).getpFitness())&&((*(RACE*)a).getsFitness()<(*(RACE*)b).getsFitness())) || ( ((*(RACE*)a).getpFitness()==(*(RACE*)b).getpFitness())&&((*(RACE*)a).getsFitness()==(*(RACE*)b).getsFitness())&& ((*(RACE*)a).gettFitness()>=(*(RACE*)b).gettFitness())) )
		return (1);
	else if(( (*(RACE*)a).getpFitness()>(*(RACE*)b).getpFitness())|| ((*(RACE*)a).getsFitness()>(*(RACE*)b).getsFitness()))
		return (-1);
	else return(0);
}

//TODO: Ueber Optionen einstellen, welche Fitness ueberhaupt bzw. wie stark gewertet wird (oder ob z.B. die Fitnesswerte zusammengeschmissen werden sollen etc.)


void SOUP::checkForChange() const
{
//	int changed=0;
/*	for(int k=mapPlayerNum-1;k--;)
		if(anaplayer[k]->getPlayer()->isChanged())
		{
			changed=1;
//			anaplayer[k]->getPlayer()->changeAccepted(); TODO
			anaplayer[k]->setGeneration(0); //?
			anaplayer[k]->setMaxpFitness(0);
			anaplayer[k]->setMaxsFitness(0);
			anaplayer[k]->setMaxtFitness(0);
			anaplayer[k]->setUnchangedGenerations(0);
		}
	if(changed)
		calculateAnaplayer(); // TODO */
}


void EXPORT SOUP::calculateAnaplayer() const
{
	PRERACE::copyMap(); // copy all units from start map to 'work sheet' ;)

	for(int k=mapPlayerNum;k--;)
		if(anaplayer[k]->isActive())
		{
			anaplayer[k]->prepareForNewGeneration();
			anaplayer[k]->initializePlayer();
			anaplayer[k]->adjustHarvestAllLocations();
		}
// supply is nicht resettet... bzw. falsch... in resetSupply
	bool complete=false;
	while(!complete)
	{
		complete=true;
		for(int k=mapPlayerNum;k--;)
			if(anaplayer[k]->isActive())
				complete&=anaplayer[k]->calculateStep();
	}
//		anaplayer[0]->backupMap();  backup&&restore map currently off-line!!!
//	} else
//		anaplayer[0]->restoreMap();
}

ANARACE** SOUP::newGeneration(ANARACE* oldAnarace[MAX_PLAYER]) //reset: have the goals/settings been changed?
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
	if(!gaInitialized)
	{
		toLog("ERROR: (SOUP::newGeneration) GA not initialized.");
		return(0);
	}*/
#endif
//	if(anaplayer[0]->getRun()>=ga->maxRuns) //~~
//		return(0);
	const int groupSize=MAX_PROGRAMS/mapPlayerNum;

//	PRERACE::initNoise();
// Set players on given code and parameters
/*	for(int j=mapPlayerNum;j--;)
		if(oldAnarace[j]) //TODO
		{
			for(int i=MAX_LENGTH;i--;)
			{
				player[j*groupSize]->Code[i]=oldAnarace[j]->Code[i];
				player[j*groupSize]->Marker[i]=oldAnarace[j]->Marker[i];
			}
//			anaplayer[j]->setActive(oldAnarace[j]->isActive());
//			anaplayer[j]->setOptimizing(oldAnarace[j]->isOptimizing());
		}
	if(!oldAnarace[0])
	{
		anaplayer[0]->setActive(true);
		anaplayer[0]->setOptimizing(false);
	}*/
	

// check whether goals etc. have changed
//		checkForChange();

	for(int i=groupSize;i--;)
	{
// Map mit Startwerten initialisieren, muss JEDEN Durchlauf passieren!! sonst sammeln sich in der statischen loc variable Haufenweise Commando Centers an 8-)
		PRERACE::copyMap();

		//reset code && calculate 
		for(int k=mapPlayerNum;k--;)
			if(anaplayer[k]->isActive())
			{
				player[k*groupSize+i]->prepareForNewGeneration();
				player[k*groupSize+i]->initializePlayer();
				player[k*groupSize+i]->adjustHarvestAllLocations();
				if(anaplayer[k]->isOptimizing())
				{
					player[k*groupSize+i]->eraseIllegalCode();
// preserve player[0]s genes					
					if(i!=0)
						player[k*groupSize+i]->mutateGeneCode();
					player[k*groupSize+i]->eraseUselessCode(); //TODO Problem beim switchen, falls schon goals gesetzt waren
				}
			}
		int complete=0;
		while(!complete)
		{
			complete=1;
			for(int k=mapPlayerNum;k--;)
				if(anaplayer[k]->isActive())
					complete&=player[k*groupSize+i]->calculateStep();
		}
	}
//NOW: all pFitness of the players are calculated

// SORT players	
	for(int k=mapPlayerNum;k--;)
		if(anaplayer[k]->isActive())
		{
			for(int i=k*groupSize;i<(k+1)*groupSize;i++)
				for(int j=k*groupSize;j<i;j++)
					if((player[i]->getpFitness()>player[j]->getpFitness())||
					  ((player[i]->getpFitness()==player[j]->getpFitness())&&(player[i]->getsFitness()>player[j]->getsFitness()))||
					  ((player[i]->getpFitness()==player[j]->getpFitness())&&(player[i]->getsFitness()==player[j]->getsFitness())&&(player[i]->gettFitness()>player[j]->gettFitness())) )
					{
						RACE* temp;
						temp=player[i];
						player[i]=player[j];
						player[j]=temp;
					}
	//NOW: all players are sorted
			
	//		  qsort(player[0],MAX_PROGRAMS/2,sizeof(RACE),compare);
	//		  qsort(player[MAX_PROGRAMS/2],MAX_PROGRAMS/2,sizeof(RACE),compare);

			for(int i=ga->getBreedFactor()*groupSize/100;i--;) // % are replaced by the uber-program :-o
			{
				int l=rand()%(groupSize*ga->getBreedFactor()/100) + groupSize*(100-ga->getBreedFactor())/100;
				if((player[k*groupSize+l]->getpFitness()*1.1<player[k*groupSize]->getpFitness())||
					  ((player[k*groupSize+l]->getpFitness()==player[k*groupSize]->getpFitness())&&(player[k*groupSize+l]->getsFitness()*1.1<player[k*groupSize]->getsFitness()))||
					  ((player[k*groupSize+l]->getpFitness()==player[k*groupSize]->getpFitness())&&(player[k*groupSize+l]->getsFitness()==player[k*groupSize]->getsFitness())&&(player[k*groupSize+l]->gettFitness()*1.1<player[k*groupSize]->gettFitness())) )

				player[k*groupSize+l]->copyCode(*player[k*groupSize]);
/*				for(int j=MAX_LENGTH;j--;)
				{
					player[k*groupSize+l]->Code[j]=player[k*groupSize]->Code[j];
					player[k*groupSize+l]->Marker[j]=player[k*groupSize]->Marker[j];

	//				  memcpy(player[t]->Code[0],player[0]->Code[0],MAX_LENGTH); 
	//				  memcpy(player[t]->Code[1],player[0]->Code[1],MAX_LENGTH);
				}*/
			}
		}

// Do we have a new best player?
		newcalc=0;
		for(int k=mapPlayerNum;k--;)
			if(anaplayer[k]->isActive())
			{
				if((player[k*groupSize]->getpFitness()>anaplayer[k]->getMaxpFitness())||

				  ((player[k*groupSize]->getpFitness()>=anaplayer[k]->getMaxpFitness())
				 &&(player[k*groupSize]->getsFitness()>anaplayer[k]->getMaxsFitness()))||

				  ((player[k*groupSize]->getpFitness()>=anaplayer[k]->getMaxpFitness())
				 &&(player[k*groupSize]->getsFitness()>=anaplayer[k]->getMaxsFitness())
				 &&(player[k*groupSize]->gettFitness()>anaplayer[k]->getMaxtFitness())))
				{

					if(player[k*groupSize]->gettFitness()>anaplayer[k]->getMaxtFitness())
						anaplayer[k]->setMaxtFitness(player[k*groupSize]->gettFitness());
					if(player[k*groupSize]->getsFitness()>anaplayer[k]->getMaxsFitness())
					{
						anaplayer[k]->setMaxsFitness(player[k*groupSize]->getsFitness());
						anaplayer[k]->setMaxtFitness(player[k*groupSize]->gettFitness());
					}
					if(player[k*groupSize]->getpFitness()>anaplayer[k]->getMaxpFitness())
					{
						anaplayer[k]->setMaxpFitness(player[k*groupSize]->getpFitness());
						anaplayer[k]->setMaxsFitness(player[k*groupSize]->getsFitness());
						anaplayer[k]->setMaxtFitness(player[k*groupSize]->gettFitness());
					}
					anaplayer[k]->setUnchangedGenerations(0);
					newcalc=1;
					anaplayer[k]->copyCode(*player[k*groupSize]);
/*					for(int i=MAX_LENGTH;i--;)
					{
	// assign the 'best of breed' to anaplayer
						anaplayer[k]->Code[i]=player[k*groupSize]->Code[i];
						anaplayer[k]->Marker[i]=player[k*groupSize]->Marker[i];
					//memcpy(anaplayer[j]->Code[0],player[j*MAX_PROGRAMS/2]->Code[0],MAX_LENGTH*4);
					//memcpy(anaplayer[j]->Code[1],player[j*MAX_PROGRAMS/2]->Code[1],MAX_LENGTH*4);
					}*/
				}
			}

//TODO: Kinder sofort neuberechnen
// Anzahl Tournaments pro Spieler: (MAX_PROGRAMS/mapPlayerNum)/(100/ga->getCrossOver())
/*	int tournaments=t*ga->getCrossOver()/100;
	if(tournaments>0)
	{
//jetzt: sortieren
		for(int k=0;k<mapPlayerNum-1;k++)
		if(anaplayer[k]->isActive())
			for(int i=0;i<tournaments;i++)
			{
				for(int j=(k*groupSize)+i*(100/ga->getCrossOver());j<(k*groupSize)+(i+1)*(100/ga->getCrossOver());j++) //diese (100/ga->crossOver) Programme untereinander sortieren
				{
					for(int l=(k*groupSize)+i*(100/ga->getCrossOver());l<j;l++)
						if((player[j]->getpFitness()>player[l]->getpFitness())||
						  ((player[j]->getpFitness()==player[l]->getpFitness())&&(player[j]->getsFitness()>player[l]->getsFitness()))||
						  ((player[j]->getpFitness()==player[l]->getpFitness())&&(player[j]->getsFitness()==player[l]->getsFitness())&&(player[j]->gettFitness()>player[l]->gettFitness())) )
						{
							RACE* temp;
							temp=player[l];
							player[l]=player[j];
							player[j]=temp;
						}
				}
			}
//JETZT: Player in z.B. 20er (bei crossOver=5) Gruppen sortiert => besten 2 herausnehmen, schlechtesten 2 ersetzen
																				
																				
//		for(int k=0;k<mapPlayerNum-1;k++)
//		if(anaplayer[k]->isActive())
//			for(int i=0;i<tournaments;i++)
//			{
//				int p1=i*(100/ga->getCrossOver())+(k*groupSize);
//				int p2=i*(100/ga->getCrossOver())+(k*groupSize)+1; //evtl nur unterschiedlichen nehmen? => phaenocode zusammenzaehlen ~
//				int c1=(i+1)*(100/ga->getCrossOver())+(k*groupSize)-1;
//				int c2=(i+1)*(100/ga->getCrossOver())+(k*groupSize)-2;
//				player[p1]->crossOver(player[p2],player[c1],player[c2]);
//			}
	}*/
																				
//evtl breed hinter crossover, aber vorher neue Kinder neu berechnen!

//	if(newcalc)
//	{
	
	calculateAnaplayer();

// SOME POST PROCESSING
// CALCULATE FITNESS AVERAGE & VARIANCE
	for(int k=mapPlayerNum;k--;)
		if(anaplayer[k]->isActive()) //~~ TODO evtl isOptimizing stattdessen...
		{
			anaplayer[k]->fitnessAverage=0;
			for(int i=k*groupSize;i<(k+1)*groupSize;i++)
				anaplayer[k]->fitnessAverage+=player[i]->getpFitness();
			anaplayer[k]->fitnessAverage/=(MAX_PROGRAMS/mapPlayerNum);
			anaplayer[k]->fitnessVariance=0;
			
			for(int i=k*groupSize;i<(k+1)*groupSize;i++)
			{
				int z=anaplayer[k]->fitnessAverage-player[i]->getpFitness();
				anaplayer[k]->fitnessVariance+=(z*z);
			}
			anaplayer[k]->fitnessVariance/=MAX_PROGRAMS;
//			anaplayer[k]->analyzeBuildOrder(); TODO?
//			anaplayer[k]->getPlayer()->getGoal()->bestTime=anaplayer[k]->getTimer(); TODO !!
			anaplayer[k]->setUnchangedGenerations(anaplayer[k]->getUnchangedGenerations()+1);
			anaplayer[k]->setGeneration(anaplayer[k]->getGeneration()+1);
			if(anaplayer[k]->getUnchangedGenerations()>ga->maxGenerations)
			{
				for(int i=k*groupSize;i<(k+1)*groupSize;i++)
					player[i]->resetGeneCode();

/*			Save[anaplayer[k]->getRun()][k]=new ANARACE;
			ANARACE* s[MAX_PLAYER];
			s[k]=Save[anaplayer[k]->getRun()][k];
		//TODO...	

			for(int p=MAX_TIME;p--;)
			{
				s[k]->setStatisticsFitness(p,anaplayer[k]->getStatisticsFitness(p));
				s[k]->setStatisticsHaveMinerals(p,anaplayer[k]->getStatisticsHaveMinerals(p));
				s[k]->setStatisticsHaveGas(p,anaplayer[k]->getStatisticsHaveGas(p));
				s[k]->setStatisticsNeedSupply(p,anaplayer[k]->getStatisticsNeedSupply(p));
				s[k]->setStatisticsHaveSupply(p,anaplayer[k]->getStatisticsHaveSupply(p));
			}
			for(int p=MAX_LENGTH;p--;)
			{
				s[k]->setProgramFacility(p,anaplayer[k]->getProgramFacility(p));
				s[k]->setProgramBT(p,anaplayer[k]->getProgramBT(p));
				s[k]->setProgramSuccessType(p,anaplayer[k]->getProgramSuccessType(p));
				s[k]->setProgramSuccessUnit(p,anaplayer[k]->getProgramSuccessUnit(p));
				s[k]->setProgramIsBuilt(p,anaplayer[k]->getProgramIsBuilt(p));
				s[k]->setProgramLocation(p,anaplayer[k]->getProgramLocation(p));
				s[k]->setProgramTime(p,anaplayer[k]->getProgramTime(p));
				s[k]->setProgramIsGoal(p,anaplayer[k]->getProgramIsGoal(p));
				s[k]->setProgramDominant(p,anaplayer[k]->getProgramDominant(p));
	
				for(int q=UNIT_TYPE_COUNT;q--;)
				{
					s[k]->setProgramForceCount(p,q,anaplayer[k]->getProgramForceCount(p,q));
					s[k]->setProgramAvailibleCount(p,q,anaplayer[k]->getProgramAvailibleCount(p,q));
				}
				s[k]->setPhaenoCode(p,anaplayer[k]->getPhaenoCode(p));
			}
 			s[k]->setUnchangedGenerations(anaplayer[k]->getUnchangedGenerations()-1);
			s[k]->setRun(anaplayer[k]->getRun()+1);
			s[k]->setGeneration(anaplayer[k]->getGeneration());
			s[k]->setMaxpFitness(anaplayer[k]->getMaxpFitness());
			s[k]->setMaxsFitness(anaplayer[k]->getMaxsFitness());
			s[k]->setMaxtFitness(anaplayer[k]->getMaxtFitness());

			s[k]->setPlayerNum(anaplayer[k]->getPlayer());
			s[k]->setPlayerNum(anaplayer[k]->getPlayerNum());
			s[k]->setpStats(anaplayer[k]->getpStats());
//			s[k]->setCalculated(anaplayer[k]->getCalculated());
			//loadplayer?
			s[k]->setSupply(anaplayer[k]->getSupply());
			s[k]->setMaxSupply(anaplayer[k]->getMaxSupply());
			s[k]->setMins(anaplayer[k]->getMins());
			s[k]->setGas(anaplayer[k]->getGas());

			s[k]->setTimer(anaplayer[k]->getTimer());
			s[k]->setTimeOut(anaplayer[k]->getTimeOut());
			s[k]->setIP(anaplayer[k]->getIP());
			s[k]->setHarvestedMins(anaplayer[k]->getHarvestedMins());
			s[k]->setHarvestedGas(anaplayer[k]->getHarvestedGas());
			for(int p=MAX_GOALS;p--;)
				s[k]->setFinalTime(p,anaplayer[k]->getFinalTime(p));

			s[k]->setLength(anaplayer[k]->getLength());
			for(int p=2;p--;)
				for(int q=MAX_LENGTH;q--;)
				{
					s[k]->Code[p][q]=anaplayer[k]->Code[p][q];
					s[k]->Marker[p][q]=anaplayer[k]->Marker[p][q];
				}
		//TODO: statistical values ?!*/
					
			anaplayer[k]->setRun(anaplayer[k]->getRun()+1);
			anaplayer[k]->setGeneration(0);
			anaplayer[k]->setMaxpFitness(0);
			anaplayer[k]->setMaxsFitness(0);
			anaplayer[k]->setMaxtFitness(0);
			anaplayer[k]->setUnchangedGenerations(0);

//			toLog("FITNESS: %s: [%.2i:%.2i]",s[k]->getPlayer()->getGoal()->getName(),(ga->maxTime-s[k]->getTimer())/60,(ga->maxTime-s[k]->getTimer())%60);
			return(&(anaplayer[0])/*Save[anaplayer[k]->getRun()]*/); //~~~~
		}
	}
	//	~~
	return(&(anaplayer[0]));
}


void SOUP::setMapPlayerNum(const int mapPlayerNum)
{
#ifdef _SCC_DEBUG
	if((mapPlayerNum<1)||(mapPlayerNum>=MAX_PLAYER)) {
		toLog("DEBUG: (SOUP::setMapPlayerNum): mapPlayerNum not initialized.");return;
	}
#endif
	this->mapPlayerNum=mapPlayerNum;
}

void SOUP::setParameters(GA* ga, START* start)
{
#ifdef _SCC_DEBUG
	if((!ga)||(!start))	{
		toLog("DEBUG: (SOUP::setParameters): Value ga/start not initialized.");return;
	}
#endif
//	gaInitialized=1;
	this->ga=ga;
	this->start=start;
	setMapPlayerNum((*start->getMap())->getMaxPlayer()); // ~~~
	PRERACE::assignGA(ga);
	PRERACE::assignStart(start);
	initSoup();
}


#ifdef BUILD_DLL
#include <windows.h>
bool APIENTRY DllMain( HANDLE hModule, 
			   DWORD  ul_reason_for_call, 
			   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
#elif IMPORT_DLL
bool APIENTRY DllMain( HANDLE hModule,
			   DWORD  ul_reason_for_call, 
			   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
#endif


