#include "soup.h"
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include "race.h"
#include "anarace.h"
#include "debug.h"
#include <stdio.h>

//todo: debug.tolog einbaun
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

void SOUP::initializeMap(MAP* map)
{
	mapInitialized=1;
	pMap=map;//start->getMap();
	initSoup();
	PRERACE::initializeMap();
};


void SOUP::setGoal(GOAL_ENTRY* goal, int player)
{
#ifdef _SCC_DEBUG
	if(!mapInitialized)
	{
		debug.toLog(0,"DEBUG: (SOUP::setGoal): Map not initialized.");
		return;
	}
	if((player<=0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (SOUP::setGoal): Value player [%i] out of range.",player);
		return;
	}
	if(!goal)
	{
		debug.toLog(0,"DEBUG: (SOUP::setGoal): Value goal [%i] not initialized.",goal);
		return;
	}
#endif
//	goalCount++; //wtf? TODO
//	if(goalCount==pMap->getMaxPlayer()-1)
	goalsInitialized=1;
	this->goal[player]=goal;
	pMap->setStartPlayerGoal(player, goal);
};

int SOUP::setParameters(GA* ga, START* start)
{
#ifdef _SCC_DEBUG
	if(!ga)
	{
		debug.toLog(0,"DEBUG: (SOUP::setParameters): Value ga [%i] not initialized.",ga);
		return(0);
	}
#endif
	gaInitialized=1;
	this->ga=ga;
	this->start=start;
	PRERACE::ga=ga;
	PRERACE::setStartConditions(start);
	return(1);
};

int SOUP::initSoup()
{
#ifdef _SCC_DEBUG
/*	if(!mapInitialized)
	{
		debug.toLog(0,"ERROR: (SOUP::initSoup) Map not initialized.");
		return(0);
	}
	if(!goalsInitialized)
	{
		debug.toLog(0,"ERROR: (SOUP::initSoup) Goals not initialized.");
		return(0);
	}
	if(!gaInitialized)
	{
		debug.toLog(0,"ERROR: (SOUP::initSoup) GA not initialized.");
		return(0);
	}*/
#endif
//	if(playerInitialized)
//	{
//		debug.toLog(0,"ERROR: (SOUP::initSoup) SOUP is already initialzed.");
//		return(0);
//	}
//	PRERACE::resetMapInitialized();
/*	if(!(PRERACE::setMap(pMap)))
	{
		debug.toLog(0,"ERROR: (SOUP::initSoup) Map not initialized.");
		return(0);
	}*/
	int t=MAX_PROGRAMS/(pMap->getMaxPlayer()-1);
	
//differenzieren, damit auch restarts/updates moeglich sind waehrend dem run! TODO
	PRERACE::resetGeneMarker();
	int k;
	for(k=0;k<pMap->getMaxPlayer()-1;k++)
	{
		for(int i=0;i<t;i++)
		{
			if(player[i+k*t])
			{
				delete player[i+k*t];
				player[i+k*t]=0;
			}
			player[i+k*t]=new RACE();
			player[i+k*t]->setPlayer(k+1);
			player[i+k*t]->resetGeneCode();
		}
		if(anaplayer[k])
		{
			delete anaplayer[k];
			anaplayer[k]=0;
		}
		anaplayer[k]=new ANARACE();
		anaplayer[k]->setPlayer(k+1);
	};
	for(;k<MAX_PLAYER;k++)
	{
		if(anaplayer[k])
		{
			delete anaplayer[k];
			anaplayer[k]=0;
		}
		//what about 'player'?
	}
	playerInitialized=1;
	return(1);
};


int compare(const void* a,const void* b)
{
	if(( (*(RACE*)a).getpFitness()<(*(RACE*)b).getpFitness())||(((*(RACE*)a).getpFitness()==(*(RACE*)b).getpFitness())&&((*(RACE*)a).getsFitness()<(*(RACE*)b).getsFitness())) || ( ((*(RACE*)a).getpFitness()==(*(RACE*)b).getpFitness())&&((*(RACE*)a).getsFitness()==(*(RACE*)b).getsFitness())&& ((*(RACE*)a).gettFitness()>=(*(RACE*)b).gettFitness())) )
		return (1);
	else if(( (*(RACE*)a).getpFitness()>(*(RACE*)b).getpFitness())|| ((*(RACE*)a).getsFitness()>(*(RACE*)b).getsFitness()))
		return (-1);
	else return(0);
};

//TODO: Ueber Optionen einstellen, welche Fitness ueberhaupt bzw. wie stark gewertet wird (oder ob z.B. die Fitnesswerte zusammengeschmissen werden sollen etc.)


void SOUP::checkForChange()
{
	int changed=0;
	for(int k=0;k<pMap->getMaxPlayer()-1;k++)
		if(anaplayer[k]->getPlayer()->isChanged())
		{
			changed=1;
			anaplayer[k]->getPlayer()->changeAccepted();
			anaplayer[k]->setGeneration(0); //?
			anaplayer[k]->setMaxpFitness(0);
			anaplayer[k]->setMaxsFitness(0);
			anaplayer[k]->setMaxtFitness(0);
			anaplayer[k]->setUnchangedGenerations(0);
		}
	if(changed)
		calculateAnaplayer();
}


void EXPORT SOUP::calculateAnaplayer()
{
	PRERACE::initializeMap();

	for(int k=0;k<pMap->getMaxPlayer()-1;k++)
//		if(anaplayer[k]->isActive())
		{
			anaplayer[k]->resetData();
			anaplayer[k]->initializePlayer();
			anaplayer[k]->adjustHarvest();
		}
// supply is nicht resettet... bzw. falsch... in resetSupply
	int complete=0;
	while(!complete)
	{
		complete=1;
		for(int k=0;k<pMap->getMaxPlayer()-1;k++)
//			if(anaplayer[k]->isActive())
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
		debug.toLog(0,"ERROR: (SOUP::newGeneration) Map not initialized.");
		return(0);
	}
	if(!goalsInitialized)
	{
		debug.toLog(0,"ERROR: (SOUP::newGeneration) Goals not initialized.");
		return(0);
	}
	if(!gaInitialized)
	{
		debug.toLog(0,"ERROR: (SOUP::newGeneration) GA not initialized.");
		return(0);
	}*/
#endif
	if(anaplayer[0]->getRun()>=ga->maxRuns) //~~
		return(0);
	int t=MAX_PROGRAMS/(pMap->getMaxPlayer()-1);

	PRERACE::initNoise();


// Set players on given code and parameters
	for(int j=0;j<pMap->getMaxPlayer()-1;j++)
		if(oldAnarace[j]) //TODO
		{
			for(int i=MAX_LENGTH;i--;)
			{
				player[j*t]->Code[i]=oldAnarace[j]->Code[i];
				player[j*t]->Marker[i]=oldAnarace[j]->Marker[i];
			}
//			anaplayer[j]->setActive(oldAnarace[j]->isActive());
			anaplayer[j]->setOptimizing(oldAnarace[j]->isOptimizing());
		}

// check whether goals etc. have changed
		checkForChange();


	for(int i=0;i<t;i++)
	{
// Map mit Startwerten initialisieren, muss JEDEN Durchlauf passieren!! sonst sammeln sich in der statischen loc variable Haufenweise Commando Centers an 8-)

		PRERACE::initializeMap();

		//reset code && calculate 
		for(int k=0;k<pMap->getMaxPlayer()-1;k++)
//			if(anaplayer[k]->isActive())
			{
				player[k*t+i]->resetData();
				player[k*t+i]->initializePlayer();
				player[k*t+i]->adjustHarvest();
//				if(anaplayer[k]->isOptimizing())
				{
					if(i!=0)
						player[k*t+i]->mutateGeneCode();
//					player[k*t+i]->eraseIllegalCode(); //TODO Problem beim switchen, falls schon goals gesetzt waren
				}
			}
		int complete=0;
		while(!complete)
		{
			complete=1;
			for(int k=0;k<pMap->getMaxPlayer()-1;k++)
				//if(anaplayer[k]->isActive())
					complete&=player[k*t+i]->calculateStep();
		//TODO Sleep Funktion einbauen
		}
	}


//NOW: all pFtiness of the players are calculated
	for(int k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
//		if(anaplayer[k]->isActive())
		{
			for(int i=k*t;i<(k+1)*t;i++)
				for(int j=k*t;j<i;j++)
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
	//	      qsort(player[0],MAX_PROGRAMS/2,sizeof(RACE),compare);
	//	      qsort(player[MAX_PROGRAMS/2],MAX_PROGRAMS/2,sizeof(RACE),compare);

			for(int i=0;i<ga->getBreedFactor()*t/100;i++) // % are replaced by the uber-program :-o
			{
				int l=rand()%(t*(ga->getBreedFactor())/100) + t*(100-ga->getBreedFactor())/100;
				if((player[k*t+l]->getpFitness()*1.1<player[k*t]->getpFitness())||
					  ((player[k*t+l]->getpFitness()==player[k*t]->getpFitness())&&(player[k*t+l]->getsFitness()*1.1<player[k*t]->getsFitness()))||
					  ((player[k*t+l]->getpFitness()==player[k*t]->getpFitness())&&(player[k*t+l]->getsFitness()==player[k*t]->getsFitness())&&(player[k*t+l]->gettFitness()*1.1<player[k*t]->gettFitness())) )

				for(int j=0;j<MAX_LENGTH;j++)
				{
					player[k*t+l]->Code[j]=player[k*t]->Code[j];
					player[k*t+l]->Marker[j]=player[k*t]->Marker[j];

	//			      memcpy(player[t]->Code[0],player[0]->Code[0],MAX_LENGTH); 
	//			      memcpy(player[t]->Code[1],player[0]->Code[1],MAX_LENGTH);
				}
			}
		}

		newcalc=0;
		for(int k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
//			if(anaplayer[k]->isActive())
			{
/*				if((player[k*t]->getpFitness()>anaplayer[k]->getMaxpFitness())||

				  ((player[k*t]->getpFitness()>=anaplayer[k]->getMaxpFitness())
				 &&(player[k*t]->getsFitness()>anaplayer[k]->getMaxsFitness()))||

				  ((player[k*t]->getpFitness()>=anaplayer[k]->getMaxpFitness())
				 &&(player[k*t]->getsFitness()>=anaplayer[k]->getMaxsFitness())
				 &&(player[k*t]->gettFitness()>anaplayer[k]->getMaxtFitness())))*/
				{

					if(player[k*t]->gettFitness()>anaplayer[k]->getMaxtFitness())
						anaplayer[k]->setMaxtFitness(player[t*k]->gettFitness());
					if(player[k*t]->getsFitness()>anaplayer[k]->getMaxsFitness())
					{
						anaplayer[k]->setMaxsFitness(player[t*k]->getsFitness());
						anaplayer[k]->setMaxtFitness(player[t*k]->gettFitness());
					}
					if(player[t*k]->getpFitness()>anaplayer[k]->getMaxpFitness())
					{
						anaplayer[k]->setMaxpFitness(player[t*k]->getpFitness());
						anaplayer[k]->setMaxsFitness(player[t*k]->getsFitness());
						anaplayer[k]->setMaxtFitness(player[t*k]->gettFitness());
					}
					anaplayer[k]->setUnchangedGenerations(0);
					newcalc=1;
					for(int i=0;i<MAX_LENGTH;i++)
					{
	// assign the 'best of breed' to anaplayer
						anaplayer[k]->Code[i]=player[t*k]->Code[i];
						anaplayer[k]->Marker[i]=player[t*k]->Marker[i];
					//memcpy(anaplayer[j]->Code[0],player[j*MAX_PROGRAMS/2]->Code[0],MAX_LENGTH*4);
					//memcpy(anaplayer[j]->Code[1],player[j*MAX_PROGRAMS/2]->Code[1],MAX_LENGTH*4);
					}
				}
			}

//TODO: Kinder sofort neuberechnen
// Anzahl Tournaments pro Spieler: (MAX_PROGRAMS/pMap->getMaxPlayer())/(100/ga->getCrossOver())
/*	int tournaments=t*ga->getCrossOver()/100;
	if(tournaments>0)
	{
//jetzt: sortieren
		for(int k=0;k<pMap->getMaxPlayer()-1;k++)
		if(anaplayer[k]->isActive())
			for(int i=0;i<tournaments;i++)
			{
				for(int j=(k*t)+i*(100/ga->getCrossOver());j<(k*t)+(i+1)*(100/ga->getCrossOver());j++) //diese (100/ga->crossOver) Programme untereinander sortieren
				{
					for(int l=(k*t)+i*(100/ga->getCrossOver());l<j;l++)
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
																			    
																			    
//		for(int k=0;k<pMap->getMaxPlayer()-1;k++)
//		if(anaplayer[k]->isActive())
//			for(int i=0;i<tournaments;i++)
//			{
//				int p1=i*(100/ga->getCrossOver())+(k*t);
//				int p2=i*(100/ga->getCrossOver())+(k*t)+1; //evtl nur unterschiedlichen nehmen? => phaenocode zusammenzaehlen ~
//				int c1=(i+1)*(100/ga->getCrossOver())+(k*t)-1;
//				int c2=(i+1)*(100/ga->getCrossOver())+(k*t)-2;
//				player[p1]->crossOver(player[p2],player[c1],player[c2]);
//			}
	}*/
																			    
//evtl breed hinter crossover, aber vorher neue Kinder neu berechnen!

//	if(newcalc)
//	{
	
	calculateAnaplayer();
	


// SOME POST PROCESSING
// CALCULATE FITNESS AVERAGE & VARIANCE
	for(int k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
//	if(anaplayer[k]->isActive()) //~~ TODO evtl isOptimizing stattdessen...
	{
		anaplayer[k]->fitnessAverage=0;
		for(int i=k*t;i<(k+1)*t;i++)
			anaplayer[k]->fitnessAverage+=player[i]->getpFitness();
		anaplayer[k]->fitnessAverage/=(MAX_PROGRAMS/(pMap->getMaxPlayer()-1));
		anaplayer[k]->fitnessVariance=0;
		
		for(int i=k*t;i<(k+1)*t;i++)
		{
			int z=anaplayer[k]->fitnessAverage-player[i]->getpFitness();
			anaplayer[k]->fitnessVariance+=(z*z);
		}
		anaplayer[k]->fitnessVariance/=MAX_PROGRAMS;
		anaplayer[k]->analyzeBuildOrder();
		anaplayer[k]->getPlayer()->getGoal()->bestTime=anaplayer[k]->getTimer();
		anaplayer[k]->setUnchangedGenerations(anaplayer[k]->getUnchangedGenerations()+1);
		anaplayer[k]->setGeneration(anaplayer[k]->getGeneration()+1);
		if(anaplayer[k]->getUnchangedGenerations()>ga->maxGenerations)
		{
			for(int i=k*t;i<(k+1)*t;i++)
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

			s[k]->setPlayer(anaplayer[k]->getPlayer());
			s[k]->setPlayerNum(anaplayer[k]->getPlayerNum());
			s[k]->setpStats(anaplayer[k]->getpStats());
			s[k]->setCalculated(anaplayer[k]->getCalculated());
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

//			debug.toLog(0,"FITNESS: %s: [%.2i:%.2i]",s[k]->getPlayer()->getGoal()->getName(),(ga->maxTime-s[k]->getTimer())/60,(ga->maxTime-s[k]->getTimer())%60);
			return(anaplayer/*Save[anaplayer[k]->getRun()]*/); //~~~~
		}
	}
	//	~~

	return(anaplayer);
};

SOUP::SOUP()
{
	run_number=0;
	playerInitialized=0;
	mapInitialized=0;
	goalCount=0;
	gaInitialized=0;
	goalsInitialized=0;
	newcalc=1;
};

SOUP::~SOUP()
{
	if(playerInitialized)
	{
		for(int i=0;i<MAX_PROGRAMS;i++)
			delete player[i];
		for(int i=0;i<MAX_PLAYER;i++)
			if(anaplayer[i]) delete anaplayer[i];
	};
};

