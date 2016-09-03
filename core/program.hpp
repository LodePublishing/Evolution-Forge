#ifndef _CORE_PROGRAM_HPP
#define _CORE_PROGRAM_HPP

#include "statistics.hpp"
#include "configuration.hpp"

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
//		const unsigned int getTotalCount(const unsigned int unit_type) const;	// how many units of the type at phaenoCode[s] do exist at that time?
//		const unsigned int getAvailibleCount(const unsigned int unit_type) const;	 // how many units of the type at phaenoCode[s] do exist at that time?

		const unsigned int getUnit() const;
		const unsigned int getFacility() const;
		const unsigned int getBT() const;
//		void setIsConstant(const bool isConstant);
		void setFacility(const unsigned int program_facility);
		void setBT(const unsigned int program_bt);
		void setSuccessType(const unsigned int program_success_type);
		void setSuccessUnit(const unsigned int program_success_unit);
		void setLocation(const unsigned int program_location);
		void setTime(const unsigned int program_time);
		void setUsedFacilityCount(const unsigned int used_facility_count);
//		void setIsGoal(const bool isGoal);	
//		const bool getIsConstant() const;

//		void setTotalCount(const unsigned int unit_type, const unsigned int unit_count);	
//		void setAvailibleCount(const unsigned int unit_type, const unsigned int unit_count);
		void setUnit(const unsigned int unit_type);

		const STATISTICS& getStatisticsAfter() const;
		const STATISTICS& getStatisticsBefore() const;
		STATISTICS before;
		STATISTICS after;
//		const bool wasBuilt();
//		void setBuild(const bool was_built = true);
	private:
//		bool built;
		unsigned int time;			// at which time this order was started
		unsigned int location;		// at which location was this unit built
		unsigned int successType;	// type of error
		unsigned int successUnit;	// unit number
	//	int isGoal;			// is this unit part of the goal list? DOES NOT WORK YET :)
//		unsigned int forceCount[UNIT_TYPE_COUNT];
//		unsigned int availibleCount[UNIT_TYPE_COUNT];
		unsigned int facility; 	// where it was produced
		unsigned int usedFacilityCount; // how much facilities were used at that time
		unsigned int unit;
		unsigned int BT;		//real buildtime, incl. moving scv etc.
//		int isConstant; //this build order may not be moved. In addition no order may be inserted before the order, if the previous order is also constant.
// TODO: Zeitfenster machen!
};

/*inline const bool PROGRAM::wasBuilt() {
	return(built);
}

inline void PROGRAM::setBuild(const bool was_built) {
	built = was_built;
}*/

inline const STATISTICS& PROGRAM::getStatisticsAfter() const {
	return(after);
}

inline const STATISTICS& PROGRAM::getStatisticsBefore() const {
	return(before);
}

inline const unsigned int PROGRAM::getFacility() const
{
#ifdef _SCC_DEBUG
	if(facility >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::getFacility): Variable not initialized.");return(0);
	}
#endif
	return(facility);
}

inline const unsigned int PROGRAM::getUsedFacilityCount() const
{
#ifdef _SCC_DEBUG
	if(usedFacilityCount >= MAX_SUPPLY) {
		toLog("DEBUG: (PROGRAM::getUsedFacilityCount): Variable not initialized.");return(0);
	}
#endif
	return(usedFacilityCount);
}

inline const unsigned int PROGRAM::getBT() const
{
#ifdef _SCC_DEBUG
	if(BT >= MAX_TIME) {
		toLog("DEBUG: (PROGRAM::getBT): Variable not initialized.");return(0);
	}
#endif
	return(BT);
}																	  

inline const unsigned int PROGRAM::getSuccessType() const
{
#ifdef _SCC_DEBUG
	if(successType >= ERROR_MESSAGES) {
		toLog("DEBUG: (PROGRAM::getSuccessType): Variable not initialized.");return(0);
	}
#endif
	return(successType);
}

inline const unsigned int PROGRAM::getSuccessUnit() const
{
#ifdef _SCC_DEBUG
	if(successUnit >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::getSuccessUnit): Variable not initialized.");return(0);
	}
#endif
	return(successUnit);
}

inline const unsigned int PROGRAM::getUnit() const
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::getUnit): Variable not initialized.");return(0);
	}
#endif
	return(unit);
}


inline void PROGRAM::setUnit(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::setUnit): Value out of range.");return;
	}
#endif
	unit = unit_type;
}

/*inline void PROGRAM::setIsConstant(, const bool isConstant)
{
	isConstant = isConstant;
};*/

/*inline void PROGRAM::setAvailibleCount(const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::setAvailibleCount): Value unit_type out of range.");return;
	}
	if(unit_count >= MAX_TOTAL_UNITS) {
		toLog("DEBUG: (PROGRAM::setAvailibleCount): Value unit_count out of range.");return;
	}
#endif
	availibleCount[unit_type] = unit_count;
}


inline void PROGRAM::setTotalCount(const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::setTotalCount): Value unit_type out of range.");return;
	}
	if(unit_count >= MAX_TOTAL_UNITS) {
		toLog("DEBUG: (PROGRAM::setTotalCount): Value unit_count out of range.");return;
	}
#endif
	forceCount[unit_type] = unit_count;
}*/

// first time is 0!
inline void PROGRAM::setTime(const unsigned int program_time)
{
#ifdef _SCC_DEBUG
	if(program_time > configuration.getMaxTime()) {
		toLog("DEBUG: (PROGRAM::setTime): Value program_time out of range.");return;
	}
#endif
	time = program_time;
}

inline void PROGRAM::setBT(const unsigned int program_bt)
{
#ifdef _SCC_DEBUG
	if(program_bt >= MAX_TIME) {
		toLog("DEBUG: (PROGRAM::setBT): Value program_bt out of range.");return;
	}
#endif
	BT = program_bt;
}

inline void PROGRAM::setLocation(const unsigned int program_location)
{
#ifdef _SCC_DEBUG
	if(program_location >= MAX_LOCATIONS) //(*getMap())->getMaxLocations()) { // TODO
	{
		toLog("DEBUG: (PROGRAM::setLocation): Value program_location out of range.");return;
	}
#endif
	location = program_location;
}

/*inline const unsigned int PROGRAM::getTotalCount(const unsigned int unit_type) const	   // how many units of the type at phaenoCode[s] do exist at that time?
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::getTotalCount): Value unit_type out of range.");return(0);
	}
#endif
	return(forceCount[unit_type]);
}

// how many units of the type at phaenoCode[s] do exist at that time?
inline const unsigned int PROGRAM::getAvailibleCount(const unsigned int unit_type) const	   
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::getAvailibleCount): Value unit_type out of range.");return(0);
	}
#endif
	return(availibleCount[unit_type]);
}*/

inline void PROGRAM::setFacility(const unsigned int program_facility)
{
#ifdef _SCC_DEBUG
	if(program_facility >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::setFacility): Value program_facility out of range.");return;
	}
#endif
	facility = program_facility;
}

inline void PROGRAM::setUsedFacilityCount(const unsigned int used_facility_count)
{
#ifdef _SCC_DEBUG
	if(used_facility_count >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::setUsedFacilityCount): Value used_facility_count out of range.");return;
	}
#endif
	usedFacilityCount = used_facility_count;
}

inline void PROGRAM::setSuccessType(const unsigned int type)
{
#ifdef _SCC_DEBUG
	if(type >= ERROR_MESSAGES) {
		toLog("DEBUG: (PROGRAM::setSuccessType): Value type out of range.");return;
	}
#endif
	successType = type;
}

inline void PROGRAM::setSuccessUnit(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PROGRAM::setSuccessUnit): Value unit_type out of range.");return;
	}
#endif
	successUnit = unit_type;
}

/*const bool PROGRAM::getIsConstant()
{
	return(isConstant);
};*/

// first program time is
inline const unsigned int PROGRAM::getTime() const
{
#ifdef _SCC_DEBUG
	if(time > configuration.getMaxTime()) {
		toLog("DEBUG: (PROGRAM::getTime): Variable not initialized.");return(0);
	}
#endif
	return(time);
}

inline const unsigned int PROGRAM::getRealTime() const {
	return(configuration.getMaxTime() - getTime());
}

inline const unsigned int PROGRAM::getLocation() const
{
#ifdef _SCC_DEBUG
	if(location > MAX_LOCATIONS) //(*getMap())->getMaxLocations()) { TODO
	{
		toLog("DEBUG: (PROGRAM::getLocation): Variable program.location not initialized.");return(0);
	}
#endif
	return(location);
}






#endif

