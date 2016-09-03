#include "building.h"
#include "debug.h"

int EXPORT Building::getTotalBuildTime()
{
	if((TB<0)||(TB>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::getTotalBuildTime): Variable TB not initialized [%i].",TB);
		return(0);
	}
	return(TB);
};
																			    
int EXPORT Building::getRemainingBuildTime()
{
	if((RB<0)||(RB>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::getRemainingBuildTime): Variable not initialized [%i].",RB);
		return(0);
	}
	return(RB);
};
																			    
int EXPORT Building::getType()
{
	if((type<0)||(type>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::getType): Variable not initialized [%i].",type);
		return(0);
	}
	return(type);
};

int EXPORT Building::getUnitCount()
{
	if((unitCount<0)||(unitCount>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: Building::getUnitCount): Variable not initialized [%i].",unitCount);
		return(0);
	}
	return(unitCount);
};
																			    
int EXPORT Building::getFacility()
{
	if((facility<0)||(facility>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::getFacility): Variable not initialized [%i].",facility);
		return(0);
	}
	return(facility);
};
																			    
int EXPORT Building::getIP()
{
	if((IP<0)||(IP>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: Building::getIP): Variable not initialized [%i].",IP);
		return(0);
	}
	return(IP);
};
																			    
int EXPORT Building::getLocation()
{
	if((location<0)||(location>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: Building::getLocation): Variable not initialized [%i].",location);
		return(0);
	}
	return(location);
};
																			    
int EXPORT Building::getGoal()
{
	if((goal<0)||(goal>UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::getGoal): Variable not initialized [%i].",goal);
		return(0);
	}
	return(goal);
};
																			    
int EXPORT Building::getOnTheRun()
{
	if((onTheRun<0)||(onTheRun>2))
	{
		debug.toLog(0,"DEBUG: Building::getOnTheRun): Variable not initialized [%i].",onTheRun);
		return(0);
	}
	return(onTheRun);
};

int EXPORT Building::setTotalBuildTime(int time)
{
	if((time<0)||(time>=MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::setTotalBuildTime): Value [%i] out of range.",time);
		return(0);
	}
	TB=time;
	return(1);
};
																			    
int EXPORT Building::setRemainingBuildTime(int time)
{
	if((time<0)||(time>=MAX_TIME))
	{
		debug.toLog(0,"DEBUG: Building::setRemainingBuildTime): Value [%i] out of range.",time);
		return(0);
	}
	RB=time;
	return(1);
};
																			    
int EXPORT Building::setType(int type)
{
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::setType): Value [%i] out of range.",type);
		return(0);
	}
	this->type=type;
	return(1);
};
																			    
int EXPORT Building::setUnitCount(int count)
{
	if((count<0)||(count>=10))
	{
		debug.toLog(0,"DEBUG: Building::setUnitCount): Value [%i] out of range.",count);
		return(0);
	}
	unitCount=count;
	return(1);
};
																			    
int EXPORT Building::setFacility(int facility)
{
	if((facility<0)||(facility>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::setFacility): Value [%i] out of range.",facility);
		return(0);
	}
	this->facility=facility;
	return(1);
};
																			    
int EXPORT Building::setIP(int IP)
{
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: Building::setIP): Value [%i] out of range.",IP);
		return(0);
	}
	this->IP=IP;
	return(1);
};
																			    
int EXPORT Building::setLocation(int location)
{
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: Building::setLocation): Value [%i] out of range.",location);
		return(0);
	}
	this->location=location;
	return(1);
};
																			    
int EXPORT Building::setGoal(int goal)
{
	if((goal<0)||(goal>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: Building::setGoal): Value [%i] out of range.",goal);
		return(0);
	}
	this->goal=goal;
	return(1);
};
																			    
int EXPORT Building::setOnTheRun(int onTheRun)
{
	if((onTheRun<0)||(onTheRun>=2))
	{
		debug.toLog(0,"DEBUG: Building::setOnTheRun): Value [%i] out of range.",onTheRun);
		return(0);
	}
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

