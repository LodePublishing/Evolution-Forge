#ifndef _CORE_BUILDING_HPP
#define _CORE_BUILDING_HPP

#include "defs.hpp"

class Building
{
private:
	unsigned int totalBuildTime;
	signed int buildFinishedTime; // timestamp when item is finnished (3600 = beginning)
	unsigned int type;			  // Type of Building
	unsigned int unitCount;  // number of units which are moving... currently ONLY for movements...
	unsigned int facility;   // in what facility it was produced
	unsigned int IP;		  // for back-tracking certain buildings (especially for the 'cancel building' option of zerg)
	unsigned int location;   // where the building was finished
	unsigned int goal;			  // For movement, where the units will move to, not yet fully implemented
	bool onTheRun;   // is this building/unit moving around or is it under construction at some place?
public:
	friend bool operator<(const Building& x, const Building& y)
	{
		if(x.getBuildFinishedTime() < y.getBuildFinishedTime())
			return(true);
		return(false);
	}
	Building();
	Building(const Building& object);
	Building& operator=(const Building& object);
	
	const unsigned int getTotalBuildTime() const;
	const unsigned int getBuildFinishedTime() const;

// is the finnishing time >= 0, i.e. can this building be finnished within max_time?
	const bool canBeCompleted() const;
	
	const unsigned int getType() const;
	const unsigned int getUnitCount() const;
	const unsigned int getFacility() const;
	const unsigned int getIP() const;
	const unsigned int getLocation() const;
	const unsigned int getGoal() const;
	const bool getOnTheRun() const;
		
	void setTotalBuildTime(const unsigned int totalBuildTime);
	void setBuildFinishedTime(const signed int buildFinishedTime);
	
	void setType(const unsigned int type);
	void setUnitCount(const unsigned int count);
	void setFacility(const unsigned int facility);
	void setIP(const unsigned int IP);
	void setLocation(const unsigned int location);
	void setGoal(const unsigned int goal);
	void setOnTheRun(const bool onTheRun);
};

#endif

