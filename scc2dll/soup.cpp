#include "soup.h"
#include <stdlib.h>
#include <cstring>
#include <time.h>
//#include <windows.h>
#include "race.h"
#include "anarace.h"
#include "debug.h"


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
                case DLL_PROCESS_ATTACH:^M
                case DLL_THREAD_ATTACH:^M
                case DLL_THREAD_DETACH:^M
                case DLL_PROCESS_DETACH:^M
                        break;
    }
    return TRUE;
}
#endif


int SOUP::setMap(MAP* map)
{
	if(!map)
		return(0);
	mapInitialized=1;
	pMap=map;
	return(1);
};


int SOUP::setGoal(GOAL_ENTRY* goal, int player)
{
	if((!mapInitialized)||(player<=0)||(player>=pMap->getMaxPlayer())) // 0 not allowed because this is temporary player
		return(0);
//	if(!this->goal[player]) ~~~~~~~~
//	{
		goalCount++;
		if(goalCount==pMap->getMaxPlayer()-1)
			goalsInitialized=1;
//	}
	this->goal[player]=goal;
	return(pMap->player[player].setGoal(goal));
};

int SOUP::setParameters(GA* ga)
{
	if(!ga)
		return(0);
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
	int i,j,k,l,m,t,complete,newcalc;
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

	t=MAX_PROGRAMS/(pMap->getMaxPlayer()-1);
	for(i=0;i<t;i++)
	{
		for(k=0;k<pMap->getMaxPlayer();k++)
			for(l=0;l<pMap->getMaxLocations()-1;l++)
				for(m=0;m<UNIT_TYPE_COUNT;m++)
				{
					PRERACE::loc[k][l].force[m]=pMap->location[l].force[k][m];
					PRERACE::loc[k][l].availible[m]=pMap->location[l].force[k][m];
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



//  ga->crossOver

/*	for(t=7;t--;)
		for(i=t*16;i<t*16+MAX_PROGRAMS/16;i++)
			for(j=t*16;j<i;j++)
				if((player[j]->getpFitness()>player[i]->getpFitness())||
				  ((player[j]->getpFitness()==player[i]->getpFitness())&&(player[j]->getsFitness()>player[i]->getsFitness()))||
				  ((player[j]->getpFitness()==player[i]->getpFitness())&&(player[j]->getsFitness()==player[i]->getsFitness())&&(player[j]->gettFitness()>player[i]->gettFitness())) )
				{
					RACE* temp;
					temp=player[i];
					player[i]=player[j];
					player[j]=temp;
				}*/


//		qsort(player[i*16],MAX_PROGRAMS/16,sizeof(RACE),compare);

/*	for(i=7;i--;)
	{
		int p1=rand()%(MAX_PROGRAMS/16);
		int p2=rand()%(MAX_PROGRAMS/16);
		int c1=rand()%(MAX_PROGRAMS/16);
		int c2=c1;
		while(c1==c2)
			c2=rand()%(MAX_PROGRAMS/16);
		player[p1*16]->crossOver(player[p2*16],player[c1*16+7],player[c2*16+7]);
	}*/

	
/*		for(k=0;k<pMap->getMaxPlayer();k++)
		for(l=0;l<pMap->getMaxLocations()-1;l++)
		for(m=0;m<UNIT_TYPE_COUNT;m++)
		{
			PRERACE::loc[k][l].force[m]=pMap->location[l].force[k][m];
			PRERACE::loc[k][l].availible[m]=pMap->location[l].force[k][m];
		}*/
	newcalc=0;
	
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

//		qsort(player[0],MAX_PROGRAMS/2,sizeof(RACE),compare);
//		qsort(player[MAX_PROGRAMS/2],MAX_PROGRAMS/2,sizeof(RACE),compare);

		for(i=0;i<ga->breedFactor*t/100;i++) // % are replaced
		{
			l=rand()%(t*(ga->breedFactor)/100) + t*(100-ga->breedFactor)/100;
			for(j=0;j<MAX_LENGTH;j++)
			{
				player[k*t+l]->Code[0][j]=player[k*t]->Code[0][j];
				player[k*t+l]->Code[1][j]=player[k*t]->Code[1][j];
//				memcpy(player[t]->Code[0],player[0]->Code[0],MAX_LENGTH); 
//				memcpy(player[t]->Code[1],player[0]->Code[1],MAX_LENGTH);
			}
		}  




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
//TODO: newcalc wieder rein, aber obacht, loc ist static und aendert sich wegen der Anfragen der normalen player! was ueberlegen... entweder mehr aus prerace in race und anarace oder hier temporaer zwischenspeichern
//	if(newcalc)
//	{
		for(k=0;k<pMap->getMaxPlayer();k++)
			for(l=0;l<pMap->getMaxLocations()-1;l++)
				for(m=0;m<UNIT_TYPE_COUNT;m++)
				{
					PRERACE::loc[k][l].force[m]=pMap->location[l].force[k][m];
					PRERACE::loc[k][l].availible[m]=pMap->location[l].force[k][m];
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

//	}

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

	t=MAX_PROGRAMS/(pMap->getMaxPlayer()-1);
        for(k=0;k<pMap->getMaxPlayer()-1;k++) //-1 because of the 0 player
	{
	        anaplayer[k]->fitnessAverage=0;
       	        for(i=k*t;i<(k+1)*t;i++)
			anaplayer[k]->fitnessAverage+=player[i]->getpFitness();
		anaplayer[k]->fitnessAverage/=MAX_PROGRAMS;
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
                                                                                                                                                            
 		if((anaplayer[0]->getMaxpFitness()>anaplayer[1]->getMaxpFitness())||((anaplayer[0]->getMaxpFitness()>=anaplayer[1]->getMaxpFitness())&&(anaplayer[0]->getMaxsFitness()>anaplayer[1]->getMaxsFitness()))|| ((anaplayer[0]->getMaxpFitness()>=anaplayer[1]->getMaxpFitness())&&(anaplayer[0]->getMaxsFitness()>=anaplayer[1]->getMaxsFitness())&&(anaplayer[0]->getMaxtFitness()>anaplayer[1]->getMaxtFitness())))
		{
			anaplayer[0]->analyzeBuildOrder();
			return(anaplayer[0]);
		}
		else
		{
			anaplayer[1]->analyzeBuildOrder();
			return(anaplayer[1]);
		}
//TODO: sort the anaplayer and return the best

//	anaplayer[0]->analyzeBuildOrder();
//	return(anaplayer[0]);
};

SOUP::SOUP()
{
	run_number=0;
	playerInitialized=0;
	mapInitialized=0;
	goalCount=0;
	gaInitialized=0;
	goalsInitialized=0;

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

