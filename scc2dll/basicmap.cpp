#include "basicmap.h"
#include <string>
#include "debug.h"

const string& BASIC_MAP::getName() const
{
	return(name);
};

const int BASIC_MAP::getMaxLocations() const
{
#ifdef _SCC_DEBUG
	if((maxLocations<0)||(maxLocations>MAX_LOCATIONS))
	{
		toLog("DEBUG: (BASIC_MAP::getMaxLocations): Variable not initialized.");
		return(0);
	}
#endif
	return(maxLocations);
};

const int BASIC_MAP::getMaxPlayer() const
{
#ifdef _SCC_DEBUG
	if((maxPlayer<0)||(maxPlayer>MAX_PLAYER))
	{
		toLog("DEBUG: (BASIC_MAP::getMaxPlayer): Variable not initialized.");
		return(0);
	}
#endif
	return(maxPlayer);
};

void BASIC_MAP::setName(const string& name)
{
	this->name=name;
};

void BASIC_MAP::setLocationName(const int location, const string& name)
{
#ifdef _SCC_DEBUG
    if((location<=0)||(location>MAX_LOCATIONS))
    {
        toLog("DEBUG: (BASIC_MAP::setLocationName): Value out of range.");
        return;
    }
#endif
	this->location[location].setName(name);
};

void BASIC_MAP::setLocationMineralDistance(const int location, const int mineralDistance)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (BASIC_MAP::setMineralDistance): Value loc out of range.");
        return;     
	}
#endif
    this->location[location].setMineralDistance(mineralDistance);
};
	
void BASIC_MAP::setLocationDistance(const int location, const list<string> distances)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (MAP::setLocationDistance): Value location out of range.");
        return;
    }
	if(location>=(int)distances.size())
	{
        toLog("DEBUG: (MAP::setLocationDistance): Too many locations.");
   	    return;
	};
#endif
	list<string>::const_iterator i=distances.begin();
	int j=0;
	while(i!=distances.end())
	{
		this->location[location].setDistance(j, atoi(i->c_str()));
		j++;
	}
#ifdef _SCC_DEBUG
	if(j<location)
	{
    	toLog("DEBUG: (MAP::setLocationDistance): Not enough locations.");
		return;
	};
#endif		
};

void BASIC_MAP::setMaxLocations(const int maxLocations)
{
#ifdef _SCC_DEBUG
	if((maxLocations<=0)||(maxLocations>MAX_LOCATIONS))
	{
		toLog("DEBUG: (BASIC_MAP::setMaxLocations): Value out of range.");
		return;
	}
#endif
	this->maxLocations=maxLocations;
};

void BASIC_MAP::setMaxPlayer(const int maxPlayer)
{
#ifdef _SCC_DEBUG
	if((maxPlayer<1)||(maxPlayer>MAX_PLAYER))
	{
		toLog("DEBUG: (BASIC_MAP::setMaxPlayer): Value num out of range.");
		return;
	}
#endif
	this->maxPlayer=maxPlayer;
};



const MAP_LOCATION* EXPORT BASIC_MAP::getLocation(const int location) const
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (BASIC_MAP::getLocation): Value location out of range.");
        return(0);
    }
#endif
	return(&this->location[location]);
};

void BASIC_MAP::resetData()
{
	for(int i=MAX_LOCATIONS;i--;)
		location[i].resetData();
	for(int i=MAX_PLAYER;i--;)
		startPosition[i]=0;
	name="ERROR";
	maxLocations=0;
	maxPlayer=0;
};


void BASIC_MAP::calculateLocationsDistances()
{
	for(int i=1;i<MAX_LOCATIONS;i++)
		location[i].calculateDistances();
};

/*void BASIC_MAP::switchTVBRace(int player, int race)
{
	//TODO evtl in ne externe Datei schreiben
	setLocationForce(0,player,SCV,4);			
	setLocationForce(getStartPlayerPosition(player),i,SCV,4);
	setStartPlayerRace(player, race);
	switch(race)
	{
		case TERRA:
			{
				setLocationForce(0,player,COMMAND_CENTER,1);
				setLocationForce(getStartPlayerPosition(player),player,COMMAND_CENTER,1);
			};break;
		case PROTOSS:
			{
				setLocationForce(0,player,NEXUS,1);
				setLocationForce(getStartPlayerPosition(player),player,NEXUS,1);
			};break;						
		case ZERG:
			{
			   setLocationForce(0,player,HATCHERY,1);
			   setLocationForce(getStartPlayerPosition(player),player,HATCHERY,1);
			   setLocationForce(0,player,OVERLORD,1);
			   setLocationForce(getStartPlayerPosition(player),player,OVERLORD,1);
			   setLocationForce(0,player,LARVA,3);
			   setLocationForce(getStartPlayerPosition(player),player,LARVA,3);
			};break;					
		}
	}
}*/

	
BASIC_MAP::BASIC_MAP()
{
	resetData();
};

BASIC_MAP::~BASIC_MAP()
{
};

