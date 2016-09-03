#include "goal.hpp"

GOAL::GOAL() :
	unit(0),
	time(0),
	count(0),
	location(0),
	finalTime(MAX_TIME),
	isAlwaysBuild(false),
	isBuildAsMuchAsPossible(false)
{ }

GOAL::~GOAL()
{ }

GOAL::GOAL(const GOAL& object) :
	unit(object.unit),
	time(object.time),
	count(object.count),
	location(object.location),
	finalTime(object.finalTime),
	isAlwaysBuild(object.isAlwaysBuild),
	isBuildAsMuchAsPossible(object.isBuildAsMuchAsPossible)
{ }

GOAL& GOAL::operator=(const GOAL& object)
{
	unit = object.unit;
	time = object.time;
	count = object.count;
	location = object.location;
	finalTime = object.finalTime;
	isAlwaysBuild = object.isAlwaysBuild;
	isBuildAsMuchAsPossible = object.isBuildAsMuchAsPossible;
	return(*this);
}

bool GOAL::operator<(const GOAL& goal)
{
	return(getLocation() < goal.getLocation());
}

void GOAL::setFinalTime(const unsigned int goal_final_time)
{
#ifdef _SCC_DEBUG
	if((goal_final_time > coreConfiguration.getMaxTime())) {
		 toErrorLog("DEBUG: (GOAL::setTime): Value goal_final_time out of range.");return;
	}
#endif
	finalTime = goal_final_time;
}


