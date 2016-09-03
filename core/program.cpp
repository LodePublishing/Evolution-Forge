#include "program.hpp"

PROGRAM::PROGRAM():
	before(),
	after(),
	successType(OK),
	successUnit(0),
	location(0),
	facility(0),
	usedFacilityCount(0),
	unit(0),
	BT(0)
{}

PROGRAM::~PROGRAM() {}

void PROGRAM::resetData()
{
	successType = OK;
	successUnit = 0;
	location = 0;
	facility = 0;
	usedFacilityCount = 0;
	unit = 0;
	BT = 0;
}


PROGRAM& PROGRAM::operator=(const PROGRAM& object)
{
	before = object.before;
	after = object.after;
	successType = object.successType;
	successUnit = object.successUnit;
	location = object.location;
	facility = object.facility;
	usedFacilityCount = object.usedFacilityCount;
	unit = object.unit;
	BT = object.BT;
	buildingQueue = object.buildingQueue;
	return(*this);
}

PROGRAM::PROGRAM(const PROGRAM& object) :
	before(object.before),
	after(object.after),
	buildingQueue(object.buildingQueue),
	successType(object.successType),
	successUnit(object.successUnit),
	location(object.location),
	facility(object.facility),
	usedFacilityCount(object.usedFacilityCount),
	unit(object.unit),
	BT(object.BT)
{ }
