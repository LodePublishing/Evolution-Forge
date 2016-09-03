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
	for(unsigned int i=MAX_LOCATIONS;i--;)
		location[i].resetData();
	name = "ERROR";
	maxLocations = 0;
	maxPlayer = 0;
}

unsigned int BASIC_MAP::calculateDistance(unsigned int i, unsigned int j)
{
	if(i==j)
		return(0);
	if(location[i].getDistance(j) < 9999)
		return(location[i].getDistance(j));
	unsigned int min = 9999;
	for(unsigned int k=1;k<getMaxLocations();++k)
		if(location[i].getDistance(k) < 9999)
		{
			unsigned int temp = location[i].getDistance(k) + calculateDistance(j, k);
			if(temp < min)
				min = temp;
		}
	return(min);	
}


void BASIC_MAP::calculateLocationDistances()
{
	for(unsigned int i = 1; i<getMaxLocations(); ++i) // MaxLocations^2 Durchlaeufe damit wir sicher alle Verbindungen bekommen
		for(unsigned int j = 1; j<getMaxLocations(); ++j)
		{
			if(i == j)
				location[j].setDistance(i, 0);
			else
			if(location[i].getDistance(j) < 9999)
				location[j].setDistance(i, location[i].getDistance(j));
		}
/*	for(unsigned int i = 1; i < getMaxLocations(); ++i)
	{
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < getMaxLocations(); ++j)
			std::cout << getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}
				
	for(unsigned int i=1;i<getMaxLocations(); ++i)
		for(unsigned int j=1;j<getMaxLocations(); ++j)
			location[i].setDistance(j, calculateDistance(i, j));

	for(unsigned int i = 1; i < getMaxLocations(); ++i)
	{
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < getMaxLocations(); ++j)
			std::cout << getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}*/

	for(unsigned int i=1;i<getMaxLocations(); ++i)
		location[i].calculateDistances(i);
}

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

	

