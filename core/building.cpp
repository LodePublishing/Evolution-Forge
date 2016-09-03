#include "building.hpp"

Building::Building():
	totalBuildTime(0),
	buildFinishedTime(-1),
	type(0),
	unitCount(0),
	facility(0),
	location(0),
	goal(0)
{ }

Building::~Building()
{}

Building::Building(const Building& object) :
	totalBuildTime(object.totalBuildTime),
	buildFinishedTime(object.buildFinishedTime),
	type(object.type),
	unitCount(object.unitCount),
	facility(object.facility),
	location(object.location),
	goal(object.goal)
{ }

Building& Building::operator=(const Building& object)
{
	totalBuildTime = object.totalBuildTime;
	buildFinishedTime = object.buildFinishedTime;
	type = object.type;
	unitCount = object.unitCount;
	facility = object.facility;
	location = object.location;
	goal = object.goal;
	return(*this);
}


