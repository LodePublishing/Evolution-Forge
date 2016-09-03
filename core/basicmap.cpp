#include "basicmap.hpp"

BASIC_MAP::BASIC_MAP():
	name("ERROR"),
	maxLocations(0),
	maxPlayer(0)
{
	resetData();
}

BASIC_MAP::~BASIC_MAP()
{ }

void BASIC_MAP::resetData()
{
	for(int i=MAX_LOCATIONS;i--;)
		location[i].resetData();
	for(int i=MAX_PLAYER;i--;)
		startPosition[i] = 0;
	name = "ERROR";
	maxLocations = 0;
	maxPlayer = 0;
}

void BASIC_MAP::calculateLocationsDistances()
{
	for(int i=1;i<MAX_LOCATIONS;i++)
		location[i].calculateDistances();
}

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

	

