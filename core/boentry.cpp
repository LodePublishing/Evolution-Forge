#include "boentry.hpp"

BUILD_ORDER::BUILD_ORDER(const eRace bo_race, const GOAL_ENTRY& bo_goal, const std::string& bo_name, const unsigned int bo_time, std::list<PROGRAM>& bo_program):
	name(bo_name),
	race(bo_race),
	programList(bo_program),
	goal(bo_goal),
	time(bo_time)
{}

BUILD_ORDER::BUILD_ORDER(const BUILD_ORDER& object) :
	name(object.name),
	race(object.race),
	programList(object.programList),
	goal(object.goal),
	time(object.time)
{}

BUILD_ORDER& BUILD_ORDER::operator=(const BUILD_ORDER& object)
{
	name = object.name;
	race = object.race;
	programList = object.programList;
	goal = object.goal;
	time = object.time;
	return(*this);
}

BUILD_ORDER::~BUILD_ORDER()
{}


// -------------------------------
// ------ SET/GET FUNCTIONS ------
// -------------------------------

