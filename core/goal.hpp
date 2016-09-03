#ifndef _CORE_GOAL_HPP
#define _CORE_GOAL_HPP

#include "configuration.hpp"

class GOAL
{
	public:
		GOAL();
		~GOAL();
		GOAL(const GOAL& goal);
		GOAL& operator=(const GOAL& goal);
		bool operator<(const GOAL& goal);

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

		unsigned int finalTime; // temporary!
};

inline void GOAL::setUnit(const unsigned int goal_unit)
{
#ifdef _SCC_DEBUG
	if((goal_unit >= UNIT_TYPE_COUNT)) {
		 toLog("DEBUG: (GOAL::setUnit): Value goal_unit out of range.");return;
	}
#endif
	unit = goal_unit;
}

inline void GOAL::setTime(const unsigned int goal_time)
{
#ifdef _SCC_DEBUG
	if((goal_time > configuration.getMaxTime())) {
		 toLog("DEBUG: (GOAL::setTime): Value goal_time out of range.");return;
	}
#endif
	time = goal_time;
}

inline void GOAL::setCount(const unsigned int goal_count)
{
#ifdef _SCC_DEBUG
	if((goal_count >= MAX_SUPPLY)) {
		 toLog("DEBUG: (GOAL::setCount): Value goal_count out of range.");return;
	}
#endif
	count = goal_count;
}

inline void GOAL::setLocation(const unsigned int goal_location)
{
#ifdef _SCC_DEBUG
	if((goal_location >= MAX_LOCATIONS)) { // TODO
		 toLog("DEBUG: (GOAL::setLocation): Value goal_location out of range.");return;
	}
#endif
	location = goal_location;
}

inline void GOAL::setFinalTime(const unsigned int goal_final_time)
{
#ifdef _SCC_DEBUG
	if((goal_final_time > configuration.getMaxTime())) {
		 toLog("DEBUG: (GOAL::setTime): Value goal_final_time out of range.");return;
	}
#endif
	finalTime = goal_final_time;
}


inline const unsigned int GOAL::getUnit() const
{
#ifdef _SCC_DEBUG
	if((unit >= UNIT_TYPE_COUNT)) {
		 toLog("DEBUG: (GOAL::getUnit): Variable not initialized.");return(0);
	}
#endif
	return(unit);
}

inline const unsigned int GOAL::getTime() const
{
#ifdef _SCC_DEBUG
	if((time > configuration.getMaxTime())) {
		 toLog("DEBUG: (GOAL::getTime): Variable not initialized.");return(0);
	}
#endif
	return(time);
}

inline const unsigned int GOAL::getCount() const
{
#ifdef _SCC_DEBUG
	if((count >= MAX_SUPPLY)) {
		 toLog("DEBUG: (GOAL::getCount): Variable not initialized.");return(0);
	}
#endif
	return(count);
}

inline const unsigned int GOAL::getLocation() const
{
#ifdef _SCC_DEBUG
	if((location >= MAX_LOCATIONS)) { // TODO
		 toLog("DEBUG: (GOAL::getLocation): Variable not initialized.");return(0);
	}
#endif
	return(location);
}

inline const unsigned int GOAL::getFinalTime() const
{
#ifdef _SCC_DEBUG
	if((finalTime > configuration.getMaxTime())) {
		 toLog("DEBUG: (GOAL::getFinalTime): Variable not initialized.");return(0);
	}
#endif
	return(finalTime);
}

#endif // _CORE_GOAL_HPP

