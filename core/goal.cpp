#include "goal.hpp"
#include "defs.hpp"

GOAL::GOAL() :
	unit(0),
	time(0),
	count(0),
	location(0),
	finalTime(MAX_TIME)
{ }

GOAL::~GOAL()
{ }

GOAL::GOAL(const GOAL& object) :
	unit(object.unit),
	time(object.time),
	count(object.count),
	location(object.location),
	finalTime(object.finalTime)
{ }

GOAL& GOAL::operator=(const GOAL& object)
{
	unit = object.unit;
	time = object.time;
	count = object.count;
	location = object.location;
	finalTime = object.finalTime;
	return(*this);
}

bool GOAL::operator<(const GOAL& goal)
{
	return(getLocation() < goal.getLocation());
}




