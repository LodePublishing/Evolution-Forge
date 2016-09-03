#include "building.hpp"

Building::Building():
	totalBuildTime(0),
	buildFinishedTime(-1),
	type(0),
	unitCount(0),
	facility(0),
//	IP(0),
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
//	IP(object.IP),
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
//	IP = object.IP;
	location = object.location;
	goal = object.goal;
	onTheRun = object.onTheRun;
	return(*this);
}

const unsigned int Building::getBuildFinishedTime() const
{
#ifdef _SCC_DEBUG
	if((buildFinishedTime<-MAX_TIME)||(buildFinishedTime>MAX_TIME)) {
		toErrorLog("DEBUG: Building::getBuildFinishedTime): Variable not initialized.");return(0);
	}
#endif
	if(buildFinishedTime < 0)
		return(0);
	else 
	return(buildFinishedTime);
}

