#ifndef _CORE_BUILDING_HPP
#define _CORE_BUILDING_HPP

#include "game.hpp"
#include "../stl/misc.hpp"

class Building
{
	public:
		Building();
		~Building();
		Building& operator=(const Building& object);
		Building(const Building& object);
		friend bool operator<(const Building& x, const Building& y)
		{
			if(x.getBuildFinishedTime() < y.getBuildFinishedTime())
				return(true); else
			return(false);
		};
	
		const unsigned int getTotalBuildTime() const;
		const unsigned int getBuildFinishedTime() const;

// is the finishing time >= 0, i.e. can this building be finnished within max_time?
		const bool canBeCompleted() const;
	
		const unsigned int getType() const;
		const unsigned int getUnitCount() const;
		const unsigned int getFacility() const;
		const unsigned int getLocation() const;
		const unsigned int getGoal() const;
		
		void setTotalBuildTime(const unsigned int total_build_time);
		void setBuildFinishedTime(const signed int build_finished_time);
	
		void setType(const unsigned int building_type);
		void setUnitCount(const unsigned int building_count);
		void setFacility(const unsigned int building_facility);
		void setLocation(const unsigned int building_location);
		void setGoal(const unsigned int building_goal);
	private:
		unsigned int totalBuildTime;
		signed int buildFinishedTime; // timestamp when item is finnished (3600 = beginning)
		unsigned int type;			  // Type of Building
		unsigned int unitCount;  // number of units which are moving... currently ONLY for movements...
		unsigned int facility;   // in what facility it was produced
		unsigned int location;   // where the building was finished
		unsigned int goal;			  // For movement, where the units will move to, not yet fully implemented
};

inline const unsigned int Building::getTotalBuildTime() const
{
#ifdef _SCC_DEBUG
	if(totalBuildTime > MAX_TIME) {
		toErrorLog("DEBUG (Building::getTotalBuildTime()): Variable totalBuildTime not initialized.");return(0);
	}
#endif
	return(totalBuildTime);
}

inline const unsigned int Building::getType() const
{
#ifdef _SCC_DEBUG
	if(type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (Building::getType()): Variable not initialized.");return(0);
	}
#endif
	return(type);
}

inline const unsigned int Building::getUnitCount() const
{
#ifdef _SCC_DEBUG
	if(unitCount > GAME::MAX_TOTAL_UNITS) {
		toErrorLog("DEBUG (Building::getUnitCount()): Variable not initialized.");return(0);
	}
#endif
	return(unitCount);
}
																				
inline const unsigned int Building::getFacility() const
{
#ifdef _SCC_DEBUG
	if(facility >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (Building::getFacility()): Variable not initialized.");return(0);
	}
#endif
	return(facility);
}

inline const unsigned int Building::getLocation() const
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) { // TODO
		toErrorLog("DEBUG (Building::getLocation()): Variable not initialized.");return(0);
	}
#endif
	return(location);
}
																				
inline const unsigned int Building::getGoal() const
{
#ifdef _SCC_DEBUG
	if(goal >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (Building::getGoal()): Variable not initialized.");return(0);
	}
#endif
	return(goal);
}

inline const unsigned int Building::getBuildFinishedTime() const
{
#ifdef _SCC_DEBUG
	if((buildFinishedTime<-MAX_TIME)||(buildFinishedTime>MAX_TIME)) {
		toErrorLog("DEBUG Building::getBuildFinishedTime()): Variable buildFinishedTime not initialized.");return(0);
	}
#endif
	if(buildFinishedTime < 0)
		return(0);
	else 
	return(buildFinishedTime);
}

inline const bool Building::canBeCompleted() const {
#ifdef _SCC_DEBUG
	if((buildFinishedTime<-MAX_TIME)||(buildFinishedTime>MAX_TIME)) {
		toErrorLog("DEBUG Building::canBeCompleted()): Variable buildFinishedTime not initialized.");return(0);
	}
#endif
	return( buildFinishedTime >= 0 );
}

inline void Building::setTotalBuildTime(const unsigned int total_build_time)
{
#ifdef _SCC_DEBUG
	if(total_build_time >= MAX_TIME) {
		toErrorLog("DEBUG (Building::setTotalBuildTime()): Value out of range.");return;
	}
#endif
	totalBuildTime = total_build_time;
}
																				
inline void Building::setBuildFinishedTime(const signed int build_finished_time)
{
#ifdef _SCC_DEBUG
	if((build_finished_time < -MAX_TIME) || (build_finished_time > MAX_TIME)) {
		toErrorLog("DEBUG (Building::setBuildFinishedTime()): Value out of range.");return;
	}
#endif
	buildFinishedTime = build_finished_time;
}
																				
inline void Building::setType(const unsigned int building_type)
{
#ifdef _SCC_DEBUG
	if(building_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (Building::setType()): Value out of range.");return;
	}
#endif
	type = building_type;
}
																				
inline void Building::setUnitCount(const unsigned int building_count)
{
#ifdef _SCC_DEBUG
	if(building_count >= 10) {
		toErrorLog("DEBUG (Building::setUnitCount()): Value out of range.");return;
	}
#endif
	unitCount = building_count;
}
																				
inline void Building::setFacility(const unsigned int building_facility)
{
#ifdef _SCC_DEBUG
	if(building_facility >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (Building::setFacility()): Value out of range.");return;
	}
#endif
	facility = building_facility;
}
																				
inline void Building::setLocation(const unsigned int building_location)
{
#ifdef _SCC_DEBUG
	if(building_location >= MAX_LOCATIONS) {
		toErrorLog("DEBUG (Building::setLocation()): Value out of range.");return;
	}
#endif
	location = building_location;
}
																				
inline void Building::setGoal(const unsigned int building_goal)
{
#ifdef _SCC_DEBUG
	if(building_goal >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (Building::setGoal()): Value out of range.");return;
	}
#endif
	goal = building_goal;
}
																				
#endif

