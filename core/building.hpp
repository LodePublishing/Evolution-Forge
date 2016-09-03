#ifndef _CORE_BUILDING_HPP
#define _CORE_BUILDING_HPP

#include "defs.hpp"

class EXPORT Building
{
private:
	int totalBuildTime;
	int buildFinishedTime; // timestamp when item is finnished (3600 = beginning)
	int type;			  // Type of Building
	int unitCount;  // number of units which are moving... currently ONLY for movements...
	int facility;   // in what facility it was produced
	int IP;		  // for back-tracking certain buildings (especially for the 'cancel building' option of zerg)
	int location;   // where the building was finished
	int goal;			  // For movement, where the units will move to, not yet fully implemented
	bool onTheRun;   // is this building/unit moving around or is it under construction at some place?
public:
	friend bool operator<(const Building& x, const Building& y)
	{
		if(x.getBuildFinishedTime() < y.getBuildFinishedTime())
			return(true);
		return(false);
	};
	const int getTotalBuildTime() const;
	const int getBuildFinishedTime() const;
	
	const int getType() const;
	const int getUnitCount() const;
	const int getFacility() const;
	const int getIP() const;
	const int getLocation() const;
	const int getGoal() const;
	const bool getOnTheRun() const;
		
	void setTotalBuildTime(const int totalBuildTime);
	void setBuildFinishedTime(const int buildFinishedTime);
	
	void setType(const int type);
	void setUnitCount(const int count);
	void setFacility(const int facility);
	void setIP(const int IP);
	void setLocation(const int location);
	void setGoal(const int goal);
	void setOnTheRun(const bool onTheRun);
	Building();
};

#endif

