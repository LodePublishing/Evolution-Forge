#ifndef _CORE_GOAL_HPP
#define _CORE_GOAL_HPP

#include "configuration.hpp"

class GOAL
{
	public:
		GOAL();
		~GOAL();
		bool operator<(const GOAL& goal);
		GOAL(const GOAL& goal);
		GOAL& operator=(const GOAL& goal);

		void setUnit(const unsigned int goal_unit);
		void setTime(const unsigned int goal_time);
		void setCount(const unsigned int goal_count);
		void setLocation(const unsigned int goal_location);
		void setFinalTime(const unsigned int goal_final_time);

		const unsigned int getUnit() const;
		const unsigned int getTime() const;
		const unsigned int getCount() const;
		const unsigned int getLocation() const;
		const unsigned int getFinalTime() const;

	private:
		unsigned int unit;//!
		unsigned int time;
		unsigned int count;
		unsigned int location;

		unsigned int finalTime; // temporary! ?

};

inline void GOAL::setUnit(const unsigned int goal_unit)
{
#ifdef _SCC_DEBUG
	if((goal_unit >= RACE::UNIT_TYPE_COUNT)) {
		 toErrorLog("DEBUG: (GOAL::setUnit): Value goal_unit out of range.");return;
	}
#endif
	unit = goal_unit;
}

inline void GOAL::setTime(const unsigned int goal_time)
{
#ifdef _SCC_DEBUG
	if((goal_time > coreConfiguration.getMaxTime())) {
		 toErrorLog("DEBUG: (GOAL::setTime): Value goal_time out of range.");return;
	}
#endif
	time = goal_time;
}

inline void GOAL::setCount(const unsigned int goal_count)
{
#ifdef _SCC_DEBUG
	if((goal_count >= GAME::MAX_SUPPLY)) {
		 toErrorLog("DEBUG: (GOAL::setCount): Value goal_count out of range.");return;
	}
#endif
	count = goal_count;
}

inline void GOAL::setLocation(const unsigned int goal_location)
{
#ifdef _SCC_DEBUG
	if((goal_location >= MAX_LOCATIONS)) { // TODO
		 toErrorLog("DEBUG: (GOAL::setLocation): Value goal_location out of range.");return;
	}
#endif
	location = goal_location;
}


inline const unsigned int GOAL::getUnit() const
{
#ifdef _SCC_DEBUG
	if((unit >= RACE::UNIT_TYPE_COUNT)) {
		 toErrorLog("DEBUG: (GOAL::getUnit): Variable not initialized.");return(0);
	}
#endif
	return(unit);
}

inline const unsigned int GOAL::getTime() const
{
#ifdef _SCC_DEBUG
	if((time > coreConfiguration.getMaxTime())) {
		 toErrorLog("DEBUG: (GOAL::getTime): Variable not initialized.");return(0);
	}
#endif
	return(time);
}

inline const unsigned int GOAL::getCount() const
{
#ifdef _SCC_DEBUG
	if((count >= GAME::MAX_SUPPLY)) {
		 toErrorLog("DEBUG: (GOAL::getCount): Variable not initialized.");return(0);
	}
#endif
	return(count);
}

inline const unsigned int GOAL::getLocation() const
{
#ifdef _SCC_DEBUG
	if((location >= MAX_LOCATIONS)) { // TODO
		 toErrorLog("DEBUG: (GOAL::getLocation): Variable not initialized.");return(0);
	}
#endif
	return(location);
}

inline const unsigned int GOAL::getFinalTime() const
{
#ifdef _SCC_DEBUG
	if((finalTime > coreConfiguration.getMaxTime())) {
		 toErrorLog("DEBUG: (GOAL::getFinalTime): Variable not initialized.");return(0);
	}
#endif
	return(finalTime);
}

#endif // _CORE_GOAL_HPP

