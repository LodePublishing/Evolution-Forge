#include "ga.hpp"

EXPORT void GA::setMutationFactor(const int num)
{
	mutationFactor=num; //~~
}
EXPORT void GA::setCrossOver(const int num)
{
	crossOver=num;
}

EXPORT void GA::setBreedFactor(const int num)
{
	breedFactor=num;
}

EXPORT const int GA::getMutationFactor() const
{
        return(mutationFactor); //~~
}
EXPORT const int GA::getCrossOver() const
{
        return(crossOver);
}
const int EXPORT GA::getBreedFactor() const
{
        return(breedFactor);
}


GA::GA()
{
	goalCount=0;
	mapCount=0;
	startconditionCount=0;
	maxTime=0;
	maxTimeOut=0;
	maxLength=0;
	maxRuns=0;
	maxGenerations=0;
	preprocessBuildOrder=0;
	currentMap=0;
	breedFactor=5; //~~
	mutationFactor=200;
	mode=0;
	noise=0;
}

GA::~GA()
{
}

