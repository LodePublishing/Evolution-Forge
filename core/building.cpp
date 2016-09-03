#include "building.hpp"

Building::Building():
	totalBuildTime(0),
	buildFinishedTime(-1),
	type(0),
	unitCount(0),
	facility(0),
	IP(0),
	location(0),
	goal(0),
	onTheRun(false)
{ }

Building::Building(const Building& object) :
    totalBuildTime(object.totalBuildTime),
    buildFinishedTime(object.buildFinishedTime),
    type(object.type),
    unitCount(object.unitCount),
    facility(object.facility),
    IP(object.IP),
    location(object.location),
    goal(object.goal),
    onTheRun(object.onTheRun)
{ }

Building& Building::operator=(const Building& object)
{
	totalBuildTime = object.totalBuildTime;
    buildFinishedTime = object.buildFinishedTime;
    type = object.type;
    unitCount = object.unitCount;
    facility = object.facility;
    IP = object.IP;
    location = object.location;
    goal = object.goal;
    onTheRun = object.onTheRun;
	return(*this);
}


const unsigned int Building::getTotalBuildTime() const
{
#ifdef _SCC_DEBUG
	if(totalBuildTime>MAX_TIME) {
		toLog("DEBUG: Building::getTotalBuildTime): Variable totalBuildTime not initialized.");return(0);
	}
#endif
	return(totalBuildTime);
}
																				
const unsigned int Building::getBuildFinishedTime() const
{
#ifdef _SCC_DEBUG
	if((buildFinishedTime<-MAX_TIME)||(buildFinishedTime>MAX_TIME)) {
		toLog("DEBUG: Building::getBuildFinishedTime): Variable not initialized.");return(0);
	}
#endif
	if(buildFinishedTime < 0)
		return(0);
	return(buildFinishedTime);
}
																				
const unsigned int Building::getType() const
{
#ifdef _SCC_DEBUG
	if(type>UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::getType): Variable not initialized.");return(0);
	}
#endif
	return(type);
}

const unsigned int Building::getUnitCount() const
{
#ifdef _SCC_DEBUG
	if(unitCount>MAX_TOTAL_UNITS) {
		toLog("DEBUG: Building::getUnitCount): Variable not initialized.");return(0);
	}
#endif
	return(unitCount);
}
																				
const unsigned int Building::getFacility() const
{
#ifdef _SCC_DEBUG
	if(facility>UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::getFacility): Variable not initialized.");return(0);
	}
#endif
	return(facility);
}
																				
const unsigned int Building::getIP() const
{
#ifdef _SCC_DEBUG
	if(IP>MAX_LENGTH) {
		toLog("DEBUG: Building::getIP): Variable not initialized.");return(0);
	}
#endif
	return(IP);
}
																				
const unsigned int Building::getLocation() const
{
#ifdef _SCC_DEBUG
	if(location>MAX_LOCATIONS) {
		toLog("DEBUG: Building::getLocation): Variable not initialized.");return(0);
	}
#endif
	return(location);
}
																				
const unsigned int Building::getGoal() const
{
#ifdef _SCC_DEBUG
	if(goal>UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::getGoal): Variable not initialized.");return(0);
	}
#endif
	return(goal);
}
																				
const bool Building::getOnTheRun() const
{
	return(onTheRun);
}

const bool Building::canBeCompleted() const
{
	return(	buildFinishedTime >= 0 );
}

void Building::setTotalBuildTime(const unsigned int totalBuildTime)
{
#ifdef _SCC_DEBUG
	if(totalBuildTime>=MAX_TIME) {
		toLog("DEBUG: Building::setTotalBuildTime): Value out of range.");return;
	}
#endif
	this->totalBuildTime=totalBuildTime;
}
																				
void Building::setBuildFinishedTime(const signed int buildFinishedTime)
{
#ifdef _SCC_DEBUG
	if((buildFinishedTime<-MAX_TIME)||(buildFinishedTime>MAX_TIME)) {
		toLog("DEBUG: Building::setBuildFinishedTime): Value out of range.");return;
	}
#endif
	this->buildFinishedTime=buildFinishedTime;
}
																				
void Building::setType(const unsigned int type)
{
#ifdef _SCC_DEBUG
	if(type>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::setType): Value out of range.");return;
	}
#endif
	this->type=type;
}
																				
void Building::setUnitCount(const unsigned int count)
{
#ifdef _SCC_DEBUG
	if(count>=10) {
		toLog("DEBUG: Building::setUnitCount): Value out of range.");return;
	}
#endif
	unitCount=count;
}
																				
void Building::setFacility(const unsigned int facility)
{
#ifdef _SCC_DEBUG
	if(facility>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::setFacility): Value out of range.");return;
	}
#endif
	this->facility=facility;
}
																				
void Building::setIP(const unsigned int IP)
{
#ifdef _SCC_DEBUG
	if(IP>=MAX_LENGTH) {
		toLog("DEBUG: Building::setIP): Value out of range.");return;
	}
#endif
	this->IP=IP;
}
																				
void Building::setLocation(const unsigned int location)
{
#ifdef _SCC_DEBUG
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: Building::setLocation): Value out of range.");return;
	}
#endif
	this->location=location;
}
																				
void Building::setGoal(const unsigned int goal)
{
#ifdef _SCC_DEBUG
	if(goal>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::setGoal): Value out of range.");return;
	}
#endif
	this->goal=goal;
}
																				
void Building::setOnTheRun(const bool onTheRun)
{
	this->onTheRun=onTheRun;
}

