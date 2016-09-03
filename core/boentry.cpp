#include "boentry.hpp"

BUILD_ORDER::BUILD_ORDER(const BO_HEADER& bo_header, const GOAL_ENTRY& bo_goal, std::list<PROGRAM>& bo_program):
	boHeader(bo_header),
	goal(bo_goal),
	programList(bo_program)
{}

BUILD_ORDER::BUILD_ORDER(const BUILD_ORDER& object) :
	boHeader(object.boHeader),
	goal(object.goal),
	programList(object.programList)
{}

BUILD_ORDER& BUILD_ORDER::operator=(const BUILD_ORDER& object)
{
	boHeader = object.boHeader;
	goal = object.goal;
	programList = object.programList;
	return(*this);
}

BUILD_ORDER::~BUILD_ORDER()
{}

BO_HEADER::BO_HEADER() 
{}

BO_HEADER::~BO_HEADER() 
{}


// -------------------------------
// ------ SET/GET FUNCTIONS ------
// -------------------------------

