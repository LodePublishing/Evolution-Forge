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


#endif // _CORE_GOAL_HPP

