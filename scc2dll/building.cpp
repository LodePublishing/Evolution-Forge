#include "building.h"
#include "debug.h"

int EXPORT Building::getTotalBuildTime()
{
#ifdef _SCC_DEBUG
	if((TB<0)||(TB>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::getTotalBuildTime): Variable TB not initialized [%i].",TB);
		return(0);
	}
#endif
	return(TB);
};
																			    
int EXPORT Building::getRemainingBuildTime()
{
#ifdef _SCC_DEBUG
	if((RB<0)||(RB>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::getRemainingBuildTime): Variable not initialized [%i].",RB);
		return(0);
	}
#endif
	return(RB);
};
																			    
int EXPORT Building::getType()
{
#ifdef _SCC_DEBUG
	if((type<0)||(type>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::getType): Variable not initialized [%i].",type);
		return(0);
	}
#endif
	return(type);
};

int EXPORT Building::getUnitCount()
{
#ifdef _SCC_DEBUG
	if((unitCount<0)||(unitCount>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: Building::getUnitCount): Variable not initialized [%i].",unitCount);
		return(0);
	}
#endif
	return(unitCount);
};
																			    
int EXPORT Building::getFacility()
{
#ifdef _SCC_DEBUG
	if((facility<0)||(facility>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::getFacility): Variable not initialized [%i].",facility);
		return(0);
	}
#endif
	return(facility);
};
																			    
int EXPORT Building::getIP()
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: Building::getIP): Variable not initialized [%i].",IP);
		return(0);
	}
#endif
	return(IP);
};
																			    
int EXPORT Building::getLocation()
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: Building::getLocation): Variable not initialized [%i].",location);
		return(0);
	}
#endif
	return(location);
};
																			    
int EXPORT Building::getGoal()
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::getGoal): Variable not initialized [%i].",goal);
		return(0);
	}
#endif
	return(goal);
};
																			    
int EXPORT Building::getOnTheRun()
{
#ifdef _SCC_DEBUG
	if((onTheRun<0)||(onTheRun>2))
	{
		debug.toLog(0,"DEBUG: Building::getOnTheRun): Variable not initialized [%i].",onTheRun);
		return(0);
	}
#endif
	return(onTheRun);
};

int EXPORT Building::setTotalBuildTime(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>=MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::setTotalBuildTime): Value [%i] out of range.",time);
		return(0);
	}
#endif
	TB=time;
	return(1);
};
																			    
int EXPORT Building::setRemainingBuildTime(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>=MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::setRemainingBuildTime): Value [%i] out of range.",time);
		return(0);
	}
#endif
	RB=time;
	return(1);
};
																			    
int EXPORT Building::setType(int type)
{
#ifdef _SCC_DEBUG
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::setType): Value [%i] out of range.",type);
		return(0);
	}
#endif
	this->type=type;
	return(1);
};
																			    
int EXPORT Building::setUnitCount(int count)
{
#ifdef _SCC_DEBUG
	if((count<0)||(count>=10))
	{
		debug.toLog(0,"DEBUG: Building::setUnitCount): Value [%i] out of range.",count);
		return(0);
	}
#endif
	unitCount=count;
	return(1);
};
																			    
int EXPORT Building::setFacility(int facility)
{
#ifdef _SCC_DEBUG
	if((facility<0)||(facility>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::setFacility): Value [%i] out of range.",facility);
		return(0);
	}
#endif
	this->facility=facility;
	return(1);
};
																			    
int EXPORT Building::setIP(int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: Building::setIP): Value [%i] out of range.",IP);
		return(0);
	}
#endif
	this->IP=IP;
	return(1);
};
																			    
int EXPORT Building::setLocation(int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: Building::setLocation): Value [%i] out of range.",location);
		return(0);
	}
#endif
	this->location=location;
	return(1);
};
																			    
int EXPORT Building::setGoal(int goal)
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::setGoal): Value [%i] out of range.",goal);
		return(0);
	}
#endif
	this->goal=goal;
	return(1);
};
																			    
int EXPORT Building::setOnTheRun(int onTheRun)
{
#ifdef _SCC_DEBUG
	if((onTheRun<0)||(onTheRun>=2))
	{
		debug.toLog(0,"DEBUG: Building::setOnTheRun): Value [%i] out of range.",onTheRun);
		return(0);
	}
#endif
	this->onTheRun=onTheRun;
	return(1);
};
																			    
Building::Building()
{
	setTotalBuildTime(0);
	setRemainingBuildTime(0);
	setType(0);
	setUnitCount(0);
	setFacility(0);
	setIP(0);
	setLocation(0);
	setGoal(0);
	setOnTheRun(0);
}

