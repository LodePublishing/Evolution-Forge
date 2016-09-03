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
        const int getTotalBuildTime() const;
        const int getRemainingBuildTime() const;
        const int getType() const;
        const int getUnitCount() const;
        const int getFacility() const;
        const int getIP() const;
        const int getLocation() const;
        const int getGoal() const;
        const int getOnTheRun() const;
                                                                                                                                                            
        void setTotalBuildTime(const int time);
        void setRemainingBuildTime(const int time);
        void setType(const int type);
        void setUnitCount(const int count);
        void setFacility(const int facility);
        void setIP(const int IP);
        void setLocation(const int location);
        void setGoal(const int goal);
        void setOnTheRun(const int onTheRun);
	Building();
};

#endif

