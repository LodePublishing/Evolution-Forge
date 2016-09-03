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

void GOAL::setUnit(const unsigned int goal_unit)
{
#ifdef _SCC_DEBUG
    if((goal_unit >= UNIT_TYPE_COUNT)) {
         toLog("DEBUG: (GOAL::setUnit): Value goal_unit out of range.");return;
    }
#endif
	unit = goal_unit;
}

void GOAL::setTime(const unsigned int goal_time)
{
#ifdef _SCC_DEBUG
    if((goal_time > configuration.getMaxTime())) {
         toLog("DEBUG: (GOAL::setTime): Value goal_time out of range.");return;
    }
#endif
	time = goal_time;
}

void GOAL::setCount(const unsigned int goal_count)
{
#ifdef _SCC_DEBUG
    if((goal_count >= MAX_SUPPLY)) {
         toLog("DEBUG: (GOAL::setCount): Value goal_count out of range.");return;
    }
#endif
	count = goal_count;
}

void GOAL::setLocation(const unsigned int goal_location)
{
#ifdef _SCC_DEBUG
    if((goal_location >= MAX_LOCATIONS)) {
         toLog("DEBUG: (GOAL::setLocation): Value goal_location out of range.");return;
    }
#endif
	location = goal_location;
}

void GOAL::setFinalTime(const unsigned int goal_final_time)
{
#ifdef _SCC_DEBUG
    if((goal_final_time > configuration.getMaxTime())) {
         toLog("DEBUG: (GOAL::setTime): Value goal_final_time out of range.");return;
    }
#endif
	finalTime = goal_final_time;
}


const unsigned int GOAL::getUnit() const
{
#ifdef _SCC_DEBUG
    if((unit >= UNIT_TYPE_COUNT)) {
         toLog("DEBUG: (GOAL::getUnit): Variable not initialized.");return(0);
    }
#endif
    return(unit);
}

const unsigned int GOAL::getTime() const
{
#ifdef _SCC_DEBUG
    if((time > configuration.getMaxTime())) {
         toLog("DEBUG: (GOAL::getTime): Variable not initialized.");return(0);
    }
#endif
    return(time);
}

const unsigned int GOAL::getCount() const
{
#ifdef _SCC_DEBUG
    if((count >= MAX_SUPPLY)) {
         toLog("DEBUG: (GOAL::getCount): Variable not initialized.");return(0);
    }
#endif
    return(count);
}

const unsigned int GOAL::getLocation() const
{
#ifdef _SCC_DEBUG
    if((location >= MAX_LOCATIONS)) {
         toLog("DEBUG: (GOAL::getLocation): Variable not initialized.");return(0);
    }
#endif
    return(location);
}

const unsigned int GOAL::getFinalTime() const
{
#ifdef _SCC_DEBUG
    if((finalTime > configuration.getMaxTime())) {
         toLog("DEBUG: (GOAL::getFinalTime): Variable not initialized.");return(0);
    }
#endif
    return(finalTime);
}

