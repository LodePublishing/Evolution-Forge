#include "ga.h"

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
	mode=0;
	noise=0;
}

GA::~GA()
{
};

