#include "program.hpp"

PROGRAM::PROGRAM():
//	built(false),
	before(),
	after(),
	time(0),
	location(0),
	successType(0),
	successUnit(0),
	facility(0),
	usedFacilityCount(0),
	unit(0),
	BT(0)
{
//	memset(forceCount, 0, UNIT_TYPE_COUNT * sizeof(int));
//	memset(availibleCount, 0, UNIT_TYPE_COUNT * sizeof(int));
}

PROGRAM::~PROGRAM() {}

void PROGRAM::resetData()
{
//	built = false;
	time = 0;
	location = 0;
	successType = 0;
	successUnit = 0;
	facility = 0;
	usedFacilityCount = 0;
	unit = 0;
	BT = 0;
//	memset(forceCount, 0, UNIT_TYPE_COUNT * sizeof(int));
//	memset(availibleCount, 0, UNIT_TYPE_COUNT * sizeof(int));
}


PROGRAM& PROGRAM::operator=(const PROGRAM& object)
{
//	built = object.built;
	before = object.before;
	after = object.after;
	time = object.time;
	location = object.location;
	successType = object.successType;
	successUnit = object.successUnit;
	facility = object.facility;
	usedFacilityCount = object.usedFacilityCount;
	unit = object.unit;
	BT = object.BT;
	return(*this);
}

PROGRAM::PROGRAM(const PROGRAM& object) :
//	built(object.built),
	before(object.before),
	after(object.after),
	time(object.time),
	location(object.location),
	successType(object.successType),
	successUnit(object.successUnit),
	facility(object.facility),
	usedFacilityCount(object.usedFacilityCount),
	unit(object.unit),
	BT(object.BT)
{ }
