#ifndef _CORE_BOENTRY_HPP
#define _CORE_BOENTRY_HPP

#include "goalentry.hpp"
#include "program.hpp"
#include <list>

class BUILD_ORDER
{
	private:
		std::string name;
		eRace race;
		std::list<PROGRAM> programList;
		GOAL_ENTRY goal;
		unsigned int time;
	public:
		BUILD_ORDER(const eRace race, const GOAL_ENTRY& bo_goal, const std::string& name, const unsigned int bo_time, std::list<PROGRAM>& bo_program);
		BUILD_ORDER(const BUILD_ORDER& object);
		BUILD_ORDER& operator=(const BUILD_ORDER& object);
		~BUILD_ORDER();

		const unsigned int countBuildOrders() const;
		const std::string& getName() const;
		const eRace getRace() const;
		const unsigned int getTime() const;
		std::list<PROGRAM>& getProgramList();
		const GOAL_ENTRY& getGoal() const;
};

inline const GOAL_ENTRY& BUILD_ORDER::getGoal() const {
	return(goal);
}

inline const unsigned int BUILD_ORDER::getTime() const {
	return(time);
}

inline std::list<PROGRAM>& BUILD_ORDER::getProgramList() {
	return(programList);
}

inline const std::string& BUILD_ORDER::getName() const {
	return name;
}

inline const eRace BUILD_ORDER::getRace() const {
	return race;
}

inline const unsigned int BUILD_ORDER::countBuildOrders() const {
	return(programList.size());
}


#endif // _CORE_BUILDORDER_HPP

