#include "boentry.hpp"

BUILD_ORDER::BUILD_ORDER(const BO_HEADER& bo_header, const GOAL_ENTRY& bo_goal, const std::list<PROGRAM>& bo_program):
	boHeader(bo_header),
	goal(bo_goal),
	programList(bo_program)
{}

BUILD_ORDER::~BUILD_ORDER()
{}

BO_HEADER::BO_HEADER():
	name(""),
	race(TERRA),
	time(0)
{}

BO_HEADER::~BO_HEADER() 
{}


// -------------------------------
// ------ SET/GET FUNCTIONS ------
// -------------------------------

