#include "ga.hpp"

void GA::setMutationFactor(const unsigned int num)
{
	mutationFactor=num; //~~
}
void GA::setCrossOver(const unsigned int num)
{
	crossOver=num;
}

void GA::setBreedFactor(const unsigned int num)
{
	breedFactor=num;
}

const unsigned int GA::getMutationFactor() const
{
        return(mutationFactor); //~~
}
const unsigned int GA::getCrossOver() const
{
        return(crossOver);
}
const unsigned int GA::getBreedFactor() const
{
        return(breedFactor);
}


GA::GA(): 
	crossOver(0),
	breedFactor(5),
	mutationFactor(200),
	preprocessBuildOrder(false),
	allowGoalAdaption(true),
	maxTime(0),
	maxTimeOut(0),
	maxLength(0),
	maxRuns(5),
	maxGenerations(200),
	currentMap(0),
	noise(0)
{
}

GA::~GA()
{
}

