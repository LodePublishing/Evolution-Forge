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
	if(totalBuildTime > MAX_TIME) {
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
	if(type > UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::getType): Variable not initialized.");return(0);
	}
#endif
	return(type);
}

const unsigned int Building::getUnitCount() const
{
#ifdef _SCC_DEBUG
	if(unitCount > MAX_TOTAL_UNITS) {
		toLog("DEBUG: Building::getUnitCount): Variable not initialized.");return(0);
	}
#endif
	return(unitCount);
}
																				
const unsigned int Building::getFacility() const
{
#ifdef _SCC_DEBUG
	if(facility > UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::getFacility): Variable not initialized.");return(0);
	}
#endif
	return(facility);
}
																				
const unsigned int Building::getIP() const
{
#ifdef _SCC_DEBUG
	if(IP > MAX_LENGTH) {
		toLog("DEBUG: Building::getIP): Variable not initialized.");return(0);
	}
#endif
	return(IP);
}
																				
const unsigned int Building::getLocation() const
{
#ifdef _SCC_DEBUG
	if(location > MAX_LOCATIONS) {
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

void Building::setTotalBuildTime(const unsigned int total_build_time)
{
#ifdef _SCC_DEBUG
	if(total_build_time>=MAX_TIME) {
		toLog("DEBUG: Building::setTotalBuildTime): Value out of range.");return;
	}
#endif
	totalBuildTime = total_build_time;
}
																				
void Building::setBuildFinishedTime(const signed int build_finished_time)
{
#ifdef _SCC_DEBUG
	if((build_finished_time < -MAX_TIME)||(build_finished_time > MAX_TIME)) {
		toLog("DEBUG: Building::setBuildFinishedTime): Value out of range.");return;
	}
#endif
	buildFinishedTime = build_finished_time;
}
																				
void Building::setType(const unsigned int building_type)
{
#ifdef _SCC_DEBUG
	if(building_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::setType): Value out of range.");return;
	}
#endif
	type = building_type;
}
																				
void Building::setUnitCount(const unsigned int building_count)
{
#ifdef _SCC_DEBUG
	if(building_count >= 10) {
		toLog("DEBUG: Building::setUnitCount): Value out of range.");return;
	}
#endif
	unitCount = building_count;
}
																				
void Building::setFacility(const unsigned int building_facility)
{
#ifdef _SCC_DEBUG
	if(building_facility >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::setFacility): Value out of range.");return;
	}
#endif
	facility = building_facility;
}
																				
void Building::setIP(const unsigned int building_IP)
{
#ifdef _SCC_DEBUG
	if(building_IP >= MAX_LENGTH) {
		toLog("DEBUG: Building::setIP): Value out of range.");return;
	}
#endif
	IP = building_IP;
}
																				
void Building::setLocation(const unsigned int building_location)
{
#ifdef _SCC_DEBUG
	if(building_location >= MAX_LOCATIONS) {
		toLog("DEBUG: Building::setLocation): Value out of range.");return;
	}
#endif
	location = building_location;
}
																				
void Building::setGoal(const unsigned int building_goal)
{
#ifdef _SCC_DEBUG
	if(building_goal >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: Building::setGoal): Value out of range.");return;
	}
#endif
	goal = building_goal;
}
																				
void Building::setOnTheRun(const bool building_on_the_run)
{
	onTheRun = building_on_the_run;
}

