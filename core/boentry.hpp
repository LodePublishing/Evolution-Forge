#ifndef _CORE_BOENTRY_HPP
#define _CORE_BOENTRY_HPP

#include "goalentry.hpp"
#include "program.hpp"
#include <list>

class BO_HEADER
{
	private:
		std::string name;
		unsigned int race;
		unsigned int time;
	public:
		BO_HEADER();
		~BO_HEADER();
		const std::string& getName() const;
		const unsigned int getRace() const;
		const unsigned int getTime() const;
		void setName(const std::string& bo_name);
		void setRace(const unsigned int bo_race);
		void setTime(const unsigned int bo_time);
};

class BUILD_ORDER
{
	public:
		BUILD_ORDER(const BO_HEADER& bo_header, const GOAL_ENTRY& bo_goal, const std::list<PROGRAM>& bo_program);
		~BUILD_ORDER();

		const std::string& getName() const;
		const unsigned int getRace() const;
		const unsigned int getTime() const;
		const GOAL_ENTRY& getGoal() const;
		const std::list<PROGRAM>& getProgramList() const;
		const unsigned int countBuildOrders() const;
	private:
		BO_HEADER boHeader;
		GOAL_ENTRY goal;
		const std::list<PROGRAM> programList;
		BUILD_ORDER& operator=(const BUILD_ORDER& object);
		BUILD_ORDER(const BUILD_ORDER& object);
};

inline const std::string& BO_HEADER::getName() const {
	return(name);
}

inline const unsigned int BO_HEADER::getRace() const {
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (BO_HEADER::getRace()): Variable race not initialized.");return(0);
	}		
#endif
	return(race);
}

inline const unsigned int BO_HEADER::getTime() const {
	return(time);
}

inline void BO_HEADER::setName(const std::string& bo_name) {
	name = bo_name;
}

inline void BO_HEADER::setRace(const unsigned int bo_race) {
#ifdef _SCC_DEBUG
	if(bo_race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (BO_HEADER::setRace()): Value bo_race out of range.");return;
	}		
#endif
	race = bo_race;
}

inline void BO_HEADER::setTime(const unsigned int bo_time) {
	time = bo_time;
}
	
inline const std::string& BUILD_ORDER::getName() const {
	return(boHeader.getName());
}

inline const unsigned int BUILD_ORDER::getRace() const {
	return(boHeader.getRace());
}

inline const unsigned int BUILD_ORDER::getTime() const {
	return(boHeader.getTime());
}

inline const GOAL_ENTRY& BUILD_ORDER::getGoal() const {
	return(goal);
}

inline const unsigned int BUILD_ORDER::countBuildOrders() const {
	return(programList.size());
}

inline const std::list<PROGRAM>& BUILD_ORDER::getProgramList() const {
	return(programList);
}

#endif // _CORE_BUILDORDER_HPP

