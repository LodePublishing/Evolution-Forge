#include "soup.h"
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include "race.h"
#include "anarace.h"
#include "debug.h"

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


int SOUP::setMap(MAP* map)
{
	if(!map)
	{
                debug.toLog(0,"DEBUG: (SOUP::setMap): Variable map not initialized [%i].",map);
		return(0);
	}
	mapInitialized=1;
	pMap=map;
	return(1);
};


int SOUP::setGoal(GOAL_ENTRY* goal, int player)
{
	if(!mapInitialized)
	{
                debug.toLog(0,"DEBUG: (SOUP::setGoal): Map not initialized.");
		return(0);
	}
	if((player<=0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (SOUP::setGoal): Value player [%i] out of range.",player);
		return(0);
	}
	if(!goal)
	{
		debug.toLog(0,"DEBUG: (SOUP::setGoal): Value goal [%i] not initialized.",goal);
		return(0);
	}
	goalCount++;
	if(goalCount==pMap->getMaxPlayer()-1)
		goalsInitialized=1;
	this->goal[player]=goal;
	return(pMap->player[player].setGoal(goal));
};

int SOUP::setParameters(GA* ga)
{
	if(!ga)
	{
                debug.toLog(0,"DEBUG: (SOUP::setParameters): Value ga [%i] not initialized.",ga);
		return(0);
	}
	gaInitialized=1;
	this->ga=ga;
	PRERACE::ga=ga;
	return(1);
};

int SOUP::initSoup()
{
	int k,i,t;
	if(!mapInitialized)
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
	}
	if(playerInitialized)
	{
		debug.toLog(0,"ERROR: (SOUP::initSoup) SOUP is already initialzed.");
		return(0);
	}
	PRERACE::resetMapInitialized(); //todo: map wieder static machen
	if(!(PRERACE::setMap(pMap)))
	{
		debug.toLog(0,"ERROR: (SOUP::initSoup) Map not initialized.");
		return(0);
	}
	t=MAX_PROGRAMS/(pMap->getMaxPlayer()-1);
	
//differenzieren, damit auch restarts/updates moeglich sind waehrend dem run!
	for(k=0;k<pMap->getMaxPlayer()-1;k++)
	{
		for(i=0;i<t;i++)
		{
			player[i+k*t]=new RACE();
			player[i+k*t]->loadPlayer(k+1);
			player[i+k*t]->resetGeneCode();
		}
		anaplayer[k]=new ANARACE();
		anaplayer[k]->loadPlayer(k+1);
	};
	RACE::bestTime=0;
	for(;k<MAX_PLAYER;k++)
	{
		anaplayer[k]=0;
//		new ANARACE();
//		anaplayer[k]->loadPlayer(1);
	};
	playerInitialized=1;
	return(1);
};


/*int compare(const void* a,const void* b)
{
	if(( (*(RACE*)a).pFitness<(*(RACE*)b).pFitness)||(((*(RACE*)a).pFitness==(*(RACE*)b).pFitness)&&((*(RACE*)a).sFitness<(*(RACE*)b).sFitness)) || ( ((*(RACE*)a).pFitness==(*(RACE*)b).pFitness)&&((*(RACE*)a).sFitness==(*(RACE*)b).sFitness)&& ((*(RACE*)a).tFitness>=(*(RACE*)b).tFitness)) )
		return (1);
	else if(( (*(RACE*)a).pFitness>(*(RACE*)b).pFitness)|| ((*(RACE*)a).sFitness>(*(RACE*)b).sFitness))
		return (-1);
	else return(0);
};*/

//TODO: Ueber Optionen einstellen, welche Fitness ueberhaupt bzw. wie stark gewertet wird (oder ob z.B. die Fitnesswerte zusammengeschmissen werden sollen etc.)

ANARACE* SOUP::newGeneration()
{
	int i,j,k,l,m,t,complete,tournaments;
	if(!mapInitialized)
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
	}
	if(anaplayer[0]->getRun()>=ga->maxRuns) //~~
		return(0);
//sauber	
	t=MAX_PROGRAMS/(pMap->getMaxPlayer()-1);

	for(i=0;i<t;i++)
	{
		for(k=0;k<pMap->getMaxPlayer();k++)
			for(l=0;l<pMap->getMaxLocations();l++) // War -1 ??
				for(m=0;m<UNIT_TYPE_COUNT;m++)
				{
					player[0]->setMapLocationForce(k,l,m,pMap->location[l].force[k][m]);
					player[0]->setMapLocationAvailible(k,l,m,pMap->location[l].force[k][m]);
				}

		for(k=0;k<pMap->getMaxPlayer()-1;k++)
		{
			player[k*t+i]->resetData();
			player[k*t+i]->resetSupply();
			player[k*t+i]->adjustHarvest();
			if(i!=0)
				player[k*t+i]->mutateGeneCode();
		}
		complete=0;
		while(!complete)
		{
			complete=1;
			for(k=0;k<pMap->getMaxPlayer()-1;k++)
				complete&=player[k*t+i]->calculateStep();
		}
	}
//ende sauber
//NOW: all pFtiness of the players are calculated


	for(k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
	{
		
		for(i=k*t;i<(k+1)*t;i++)
			for(j=k*t;j<i;j++)
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

		for(i=0;i<ga->breedFactor*t/100;i++) // % are replaced by the uber-program :-o
		{
			l=rand()%(t*(ga->breedFactor)/100) + t*(100-ga->breedFactor)/100;
			for(j=0;j<MAX_LENGTH;j++)
			{
				player[k*t+l]->Code[0][j]=player[k*t]->Code[0][j];
				player[k*t+l]->Code[1][j]=player[k*t]->Code[1][j];
//			      memcpy(player[t]->Code[0],player[0]->Code[0],MAX_LENGTH); 
//			      memcpy(player[t]->Code[1],player[0]->Code[1],MAX_LENGTH);
			}
		}
	}



	newcalc=0;
	
	for(k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
	{
		if((player[k*t]->getpFitness()>anaplayer[k]->getMaxpFitness())||
		  ((player[k*t]->getpFitness()>=anaplayer[k]->getMaxpFitness())
		 &&(player[k*t]->getsFitness()>anaplayer[k]->getMaxsFitness()))||
		  ((player[k*t]->getpFitness()>=anaplayer[k]->getMaxpFitness())
		 &&(player[k*t]->getsFitness()>=anaplayer[k]->getMaxsFitness())
		 &&(player[k*t]->gettFitness()>anaplayer[k]->getMaxtFitness())))
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
			for(i=0;i<MAX_LENGTH;i++)
			{
				anaplayer[k]->Code[0][i]=player[t*k]->Code[0][i];
				anaplayer[k]->Code[1][i]=player[t*k]->Code[1][i];
				//memcpy(anaplayer[j]->Code[0],player[j*MAX_PROGRAMS/2]->Code[0],MAX_LENGTH*4);
				//memcpy(anaplayer[j]->Code[1],player[j*MAX_PROGRAMS/2]->Code[1],MAX_LENGTH*4);
			}
		}
	}

//TODO: Kinder sofort neuberechnen
// Anzahl Tournaments pro Spieler: (MAX_PROGRAMS/pMap->getMaxPlayer())/(100/ga->crossOver)
	tournaments=t*ga->crossOver/100;
	if(tournaments>0)
	{
																			    
//jetzt: sortieren
		for(k=0;k<pMap->getMaxPlayer()-1;k++)
			for(i=0;i<tournaments;i++)
			{
				for(j=(k*t)+i*(100/ga->crossOver);j<(k*t)+(i+1)*(100/ga->crossOver);j++) //diese (100/ga->crossOver) Programme untereinander sortieren
				{
					for(l=(k*t)+i*(100/ga->crossOver);l<j;l++)
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
																			    
																			    
		for(k=0;k<pMap->getMaxPlayer()-1;k++)
			for(i=0;i<tournaments;i++)
			{
				int p1=i*(100/ga->crossOver)+(k*t);
				int p2=i*(100/ga->crossOver)+(k*t)+1; //evtl nur unterschiedlichen nehmen? => phaenocode zusammenzaehlen ~
				int c1=(i+1)*(100/ga->crossOver)+(k*t)-1;
				int c2=(i+1)*(100/ga->crossOver)+(k*t)-2;
				player[p1]->crossOver(player[p2],player[c1],player[c2]);
			}
	}
																			    
//evtl breed hinter crossover, aber vorher neue Kinder neu berechnen!

//	if(newcalc)
//	{
		for(k=0;k<pMap->getMaxPlayer();k++)
			for(l=0;l<pMap->getMaxLocations();l++) // warum -1 !??!?
				for(m=0;m<UNIT_TYPE_COUNT;m++)
				{
					anaplayer[0]->setMapLocationForce(k,l,m,pMap->location[l].force[k][m]);
					anaplayer[0]->setMapLocationAvailible(k,l,m,pMap->location[l].force[k][m]);
				}

		for(k=0;k<pMap->getMaxPlayer()-1;k++)
		{
			anaplayer[k]->resetData();
			anaplayer[k]->resetSupply();
			anaplayer[k]->adjustHarvest();
		}
		complete=0;

		while(!complete)
		{
			complete=1;
			for(k=0;k<pMap->getMaxPlayer()-1;k++)
				complete&=anaplayer[k]->calculateStep();
		}
//		anaplayer[0]->backupMap();
//	} else
//		anaplayer[0]->restoreMap();

	for(k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
	{
		anaplayer[k]->setUnchangedGenerations(anaplayer[k]->getUnchangedGenerations()+1);
		anaplayer[k]->setGeneration(anaplayer[k]->getGeneration()+1);
		if(anaplayer[k]->getUnchangedGenerations()>ga->maxGenerations)
		{
			for(i=k*t;i<(k+1)*t;i++)
				player[i]->resetGeneCode();

			anaplayer[k]->setRun(anaplayer[k]->getRun()+1);
			anaplayer[k]->setGeneration(0);
			anaplayer[k]->setMaxpFitness(0);
			anaplayer[k]->setMaxsFitness(0);
			anaplayer[k]->setMaxtFitness(0);
			anaplayer[k]->setUnchangedGenerations(0);		
			//TODO: Saven!
		}
	}
	//	~~

	for(k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
	{
		anaplayer[k]->fitnessAverage=0;
       		for(i=k*t;i<(k+1)*t;i++)
			anaplayer[k]->fitnessAverage+=player[i]->getpFitness();
		anaplayer[k]->fitnessAverage/=(MAX_PROGRAMS/(pMap->getMaxPlayer()-1));
	}
	
	for(k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
	{
		anaplayer[k]->fitnessVariance=0;
		for(i=k*t;i<(k+1)*t;i++)
		{
			int z=anaplayer[k]->fitnessAverage-player[i]->getpFitness();
			anaplayer[k]->fitnessVariance+=(z*z);
		}
		anaplayer[k]->fitnessVariance/=MAX_PROGRAMS;
	}
																			    
	for(k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
	{
		for(j=0;j<k;j++)
			if((anaplayer[k]->getMaxpFitness()>anaplayer[j]->getMaxpFitness())||
				((anaplayer[k]->getMaxpFitness()==anaplayer[j]->getMaxpFitness())&&(anaplayer[k]->getMaxsFitness()>anaplayer[j]->getMaxsFitness()))||
				((anaplayer[k]->getMaxpFitness()==anaplayer[j]->getMaxpFitness())&&(anaplayer[k]->getMaxsFitness()==anaplayer[j]->getMaxsFitness())&&(anaplayer[k]->getMaxtFitness()>anaplayer[j]->getMaxtFitness())) )
			{
				ANARACE* temp;
				temp=anaplayer[k];
				anaplayer[k]=anaplayer[j];
				anaplayer[j]=temp;
			}
	}

	//return the best solution ~~ 
	// TODO multiple parties
	anaplayer[0]->analyzeBuildOrder();
	RACE::bestTime=anaplayer[0]->getTimer();
	return(anaplayer[0]);
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
	int i;
	if(playerInitialized)
	{
		for(i=0;i<MAX_PROGRAMS;i++)
			delete player[i];
		for(i=0;i<MAX_PLAYER;i++)
			if(anaplayer[i]) delete anaplayer[i];
	};
};

