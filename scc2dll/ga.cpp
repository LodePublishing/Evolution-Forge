#include "ga.h"


EXPORT void GA::setMutationFactor(int num)
{
	mutationFactor=num; //~~
};
EXPORT void GA::setCrossOver(int num)
{
	crossOver=num;
};
EXPORT void GA::setBreedFactor(int num)
{
	breedFactor=num;
};


EXPORT int GA::getMutationFactor()
{
        return(mutationFactor); //~~
};
EXPORT int GA::getCrossOver()
{
        return(crossOver);
};
EXPORT int GA::getBreedFactor()
{
        return(breedFactor);
};


GA::GA()
{
	goalCount=0;
	mapCount=0;
	maxTime=0;
	maxTimeOut=0;
	maxLength=0;
	maxRuns=0;
	maxGenerations=0;
	preprocessBuildOrder=0;
	currentMap=0;
	breedFactor=0; //~~
	mutationFactor=100;
	mode=0;
	noise=0;
}

GA::~GA()
{
};

