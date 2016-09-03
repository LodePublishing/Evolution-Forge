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



