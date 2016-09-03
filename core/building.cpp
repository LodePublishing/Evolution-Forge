#include "building.hpp"

Building::Building()
{
	setTotalBuildTime(0);
	setBuildFinishedTime(0);
	setType(0);
	setUnitCount(0);
	setFacility(0);
	setIP(0);
	setLocation(0);
	setGoal(0);
	setOnTheRun(false);
}

const int EXPORT Building::getTotalBuildTime() const
{
#ifdef _SCC_DEBUG
	if((totalBuildTime<0)||(totalBuildTime>MAX_TIME)) {
		toLog("DEBUG: Building::getTotalBuildTime): Variable totalBuildTime not initialized.");return(0);
	}
#endif
	return(totalBuildTime);
}
																				
const int EXPORT Building::getBuildFinishedTime() const
{
#ifdef _SCC_DEBUG
	if((buildFinishedTime<-MAX_TIME)||(buildFinishedTime>MAX_TIME)) {
		toLog("DEBUG: Building::getBuildFinishedTime): Variable not initialized.");return(0);
	}
#endif
	return(buildFinishedTime);
}
																				
const int EXPORT Building::getType() const
{
#ifdef _SCC_DEBUG
	if((type<0)||(type>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: Building::getType): Variable not initialized.");return(0);
	}
#endif
	return(type);
}

const int EXPORT Building::getUnitCount() const
{
#ifdef _SCC_DEBUG
	if((unitCount<0)||(unitCount>MAX_TOTAL_UNITS)) {
		toLog("DEBUG: Building::getUnitCount): Variable not initialized.");return(0);
	}
#endif
	return(unitCount);
}
																				
const int EXPORT Building::getFacility() const
{
#ifdef _SCC_DEBUG
	if((facility<0)||(facility>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: Building::getFacility): Variable not initialized.");return(0);
	}
#endif
	return(facility);
}
																				
const int EXPORT Building::getIP() const
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH)) {
		toLog("DEBUG: Building::getIP): Variable not initialized.");return(0);
	}
#endif
	return(IP);
}
																				
const int EXPORT Building::getLocation() const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>MAX_LOCATIONS)) {
		toLog("DEBUG: Building::getLocation): Variable not initialized.");return(0);
	}
#endif
	return(location);
}
																				
const int EXPORT Building::getGoal() const
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>UNIT_TYPE_COUNT)) {
		toLog("DEBUG: Building::getGoal): Variable not initialized.");return(0);
	}
#endif
	return(goal);
}
																				
const bool EXPORT Building::getOnTheRun() const
{
	return(onTheRun);
}

void EXPORT Building::setTotalBuildTime(const int totalBuildTime)
{
#ifdef _SCC_DEBUG
	if((totalBuildTime<0)||(totalBuildTime>=MAX_TIME)) {
		toLog("DEBUG: Building::setTotalBuildTime): Value out of range.");return;
	}
#endif
	this->totalBuildTime=totalBuildTime;
}
																				
void EXPORT Building::setBuildFinishedTime(const int buildFinishedTime)
{
#ifdef _SCC_DEBUG
	if((buildFinishedTime<-MAX_TIME)||(buildFinishedTime>MAX_TIME)) {
		toLog("DEBUG: Building::setBuildFinishedTime): Value out of range.");return;
	}
#endif
	this->buildFinishedTime=buildFinishedTime;
}
																				
void EXPORT Building::setType(const int type)
{
#ifdef _SCC_DEBUG
	if((type<0)||(type>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: Building::setType): Value out of range.");return;
	}
#endif
	this->type=type;
}
																				
void EXPORT Building::setUnitCount(const int count)
{
#ifdef _SCC_DEBUG
	if((count<0)||(count>=10)) {
		toLog("DEBUG: Building::setUnitCount): Value out of range.");return;
	}
#endif
	unitCount=count;
}
																				
void EXPORT Building::setFacility(const int facility)
{
#ifdef _SCC_DEBUG
	if((facility<0)||(facility>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: Building::setFacility): Value out of range.");return;
	}
#endif
	this->facility=facility;
}
																				
void EXPORT Building::setIP(const int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH)) {
		toLog("DEBUG: Building::setIP): Value out of range.");return;
	}
#endif
	this->IP=IP;
}
																				
void EXPORT Building::setLocation(const int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: Building::setLocation): Value out of range.");return;
	}
#endif
	this->location=location;
}
																				
void EXPORT Building::setGoal(const int goal)
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=UNIT_TYPE_COUNT)) {
		toLog("DEBUG: Building::setGoal): Value out of range.");return;
	}
#endif
	this->goal=goal;
}
																				
void EXPORT Building::setOnTheRun(const bool onTheRun)
{
	this->onTheRun=onTheRun;
}

