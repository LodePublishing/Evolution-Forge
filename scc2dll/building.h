#ifndef __BUILDING_H
#define __BUILDING_H

#include "main.h"

class EXPORT Building
{
private:
        int TB,RB;                    // Remaining Buildtime
        int type;              // Type of Building
        int unitCount;  // number of units which are moving... currently ONLY for movements...
        int facility;   // in what facility it was produced
        int IP;          // for back-tracking certain buildings (especially for the 'cancel building' option of zerg)
        int location;   // where the building was finished
        int goal;              // For movement, where the units will move to, not yet fully implemented
        int onTheRun;   // is this building/unit moving around or is it under construction at some place?
public:
        int getTotalBuildTime();
        int getRemainingBuildTime();
        int getType();
        int getUnitCount();
        int getFacility();
        int getIP();
        int getLocation();
        int getGoal();
        int getOnTheRun();
                                                                                                                                                            
        int setTotalBuildTime( int time);
        int setRemainingBuildTime(int time);
        int setType(int type);
        int setUnitCount(int count);
        int setFacility(int facility);
        int setIP(int IP);
        int setLocation(int location);
        int setGoal(int goal);
        int setOnTheRun(int onTheRun);
	Building();
};

#endif

