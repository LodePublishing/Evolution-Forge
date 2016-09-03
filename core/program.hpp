#ifndef _CORE_PROGRAM_HPP
#define _CORE_PROGRAM_HPP

#include "statistics.hpp"
#include "configuration.hpp"
#include "building.hpp"

#include <queue>

class PROGRAM
{
	public:
		PROGRAM();
		~PROGRAM();
		PROGRAM(const PROGRAM& object);
		PROGRAM& operator=(const PROGRAM& object);
	
		void resetData();
		const unsigned int getSuccessType() const;	// determines the type of the last error before the item was built at that IP
		const unsigned int getSuccessUnit() const;	// what unit was missing? (connected to successtype)
	
		const unsigned int getTime() const;			// at which time this order was started
		const unsigned int getRealTime() const;
		const unsigned int getUsedFacilityCount() const;
//		const unsigned int getRealTime() const;			// at which time this order was started
		const unsigned int getLocation() const;		// at which location was this unit built
//		const bool getIsGoal() const;		// is this unit part of the goal list? NOT YET WORKING!
		const unsigned int getUnit() const;
		const unsigned int getFacility() const;
		const unsigned int getBT() const;
		void setFacility(const unsigned int program_facility);
		void setBT(const unsigned int program_bt);
		void setSuccessType(const eErrorMessages program_success_type);
		void setSuccessUnit(const unsigned int program_success_unit);
		void setLocation(const unsigned int program_location);
//		void setTime(const unsigned int program_time);
		void setUsedFacilityCount(const unsigned int used_facility_count);
//		void setIsGoal(const bool isGoal);	

		void setUnit(const unsigned int unit_type);
		STATISTICS before;
		STATISTICS after;

		const STATISTICS& getStatisticsAfter() const;
		const STATISTICS& getStatisticsBefore() const;
		std::priority_queue<Building, std::vector<Building> > buildingQueue;

		eErrorMessages successType;	// type of error
		unsigned int successUnit;	// unit number
	private:

//		unsigned int time;			// at which time this order was started
		unsigned int location;		// at which location was this unit built
	//	int isGoal;			// is this unit part of the goal list? DOES NOT WORK YET :)
		unsigned int facility; 	// where it was produced
		unsigned int usedFacilityCount; // how much facilities were used at that time
		unsigned int unit;
		unsigned int BT;		//real buildtime, incl. moving scv etc.
// TODO: Zeitfenster machen!
};

inline const STATISTICS& PROGRAM::getStatisticsAfter() const {
	return(after);
}

inline const STATISTICS& PROGRAM::getStatisticsBefore() const {
	return(before);
}

inline const unsigned int PROGRAM::getFacility() const
{
#ifdef _SCC_DEBUG
	if(facility >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (PROGRAM::getFacility): Variable not initialized.");return(0);
	}
#endif
	return(facility);
}

inline const unsigned int PROGRAM::getUsedFacilityCount() const
{
#ifdef _SCC_DEBUG
	if(usedFacilityCount >= GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG: (PROGRAM::getUsedFacilityCount): Variable not initialized.");return(0);
	}
#endif
	return(usedFacilityCount);
}

inline const unsigned int PROGRAM::getBT() const
{
#ifdef _SCC_DEBUG
	if(BT >= MAX_TIME) {
		toErrorLog("DEBUG: (PROGRAM::getBT): Variable not initialized.");return(0);
	}
#endif
	return(BT);
}																	  

inline const unsigned int PROGRAM::getSuccessType() const
{
#ifdef _SCC_DEBUG
	if(successType >= ERROR_MESSAGES) {
		toErrorLog("DEBUG: (PROGRAM::getSuccessType): Variable not initialized.");return(0);
	}
#endif
	return(successType);
}

inline const unsigned int PROGRAM::getSuccessUnit() const
{
#ifdef _SCC_DEBUG
	if(successUnit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (PROGRAM::getSuccessUnit): Variable not initialized.");return(0);
	}
#endif
	return(successUnit);
}

inline const unsigned int PROGRAM::getUnit() const
{
#ifdef _SCC_DEBUG
	if(unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (PROGRAM::getUnit): Variable not initialized.");return(0);
	}
#endif
	return(unit);
}


inline void PROGRAM::setUnit(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (PROGRAM::setUnit): Value out of range.");return;
	}
#endif
	unit = unit_type;
}

// first time is 0!
/*inline void PROGRAM::setTime(const unsigned int program_time)
{
#ifdef _SCC_DEBUG
	if(program_time > coreConfiguration.getMaxTime()) {
		toErrorLog("DEBUG: (PROGRAM::setTime): Value program_time out of range.");return;
	}
#endif
	time = program_time;
}*/

inline void PROGRAM::setBT(const unsigned int program_bt)
{
#ifdef _SCC_DEBUG
	if(program_bt >= MAX_TIME) {
		toErrorLog("DEBUG: (PROGRAM::setBT): Value program_bt out of range.");return;
	}
#endif
	BT = program_bt;
}

inline void PROGRAM::setLocation(const unsigned int program_location)
{
#ifdef _SCC_DEBUG
	if(program_location >= MAX_LOCATIONS) //(*getMap())->getMaxLocations()) { // TODO
	{
		toErrorLog("DEBUG: (PROGRAM::setLocation): Value program_location out of range.");return;
	}
#endif
	location = program_location;
}

inline void PROGRAM::setFacility(const unsigned int program_facility)
{
#ifdef _SCC_DEBUG
	if(program_facility >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (PROGRAM::setFacility): Value program_facility out of range.");return;
	}
#endif
	facility = program_facility;
}

inline void PROGRAM::setUsedFacilityCount(const unsigned int used_facility_count)
{
#ifdef _SCC_DEBUG
	if(used_facility_count >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (PROGRAM::setUsedFacilityCount): Value used_facility_count out of range.");return;
	}
#endif
	usedFacilityCount = used_facility_count;
}

inline void PROGRAM::setSuccessType(const eErrorMessages type)
{
#ifdef _SCC_DEBUG
	if(type >= ERROR_MESSAGES) {
		toErrorLog("DEBUG: (PROGRAM::setSuccessType): Value type out of range.");return;
	}
#endif
	successType = type;
}

inline void PROGRAM::setSuccessUnit(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG: (PROGRAM::setSuccessUnit): Value unit_type out of range.");return;
	}
#endif
	successUnit = unit_type;
}

// first program time is
inline const unsigned int PROGRAM::getTime() const
{
#ifdef _SCC_DEBUG
	if(before.getTime() > coreConfiguration.getMaxTime()) {
		toErrorLog("DEBUG: (PROGRAM::getTime): Variable not initialized.");return(0);
	}
#endif
	return(before.getTime());
}

inline const unsigned int PROGRAM::getRealTime() const {
	return(coreConfiguration.getMaxTime() - getTime());
}

inline const unsigned int PROGRAM::getLocation() const
{
#ifdef _SCC_DEBUG
	if(location > MAX_LOCATIONS) //(*getMap())->getMaxLocations()) { TODO
	{
		toErrorLog("DEBUG: (PROGRAM::getLocation): Variable program.location not initialized.");return(0);
	}
#endif
	return(location);
}






#endif

