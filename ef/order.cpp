#include "order.hpp"

Order::Order():
	blend(0),
	blendTarget(0),
	blendStart(0),
	rect(),
	brect(),
	target(),
	btarget(),
	start(),
	bstart(),
	marker(0),
	bonew(false),
	checked(false),
	unit(0),
	IP(0),
	row(0)
{ }

Order::~Order()
{ }

Order::Order(const Order& object) :
    blend(object.blend),
    blendTarget(object.blendTarget),
    blendStart(object.blendStart),
    rect(object.rect),
    brect(object.brect),
    target(object.target),
    btarget(object.btarget),
    start(object.start),
    bstart(object.bstart),
    marker(object.marker),
    bonew(object.bonew),
    checked(object.checked),
    unit(object.unit),
    IP(object.IP),
    row(object.row)
{ }

Order& Order::operator=(const Order& order)
{
	blend=order.blend;
	blendTarget=order.blendTarget;
	blendStart=order.blendStart;
	rect=order.rect;
	brect=order.brect; //?
	target=order.target;
	btarget=order.btarget; //?
	start=order.start;
	bstart=order.bstart; //?
	marker=order.marker;
	bonew=order.bonew;
	checked=order.checked;
	unit=order.unit;
	IP=order.IP;
	row=order.row;
	return(*this);
}

void Order::setUnit(const unsigned int unit)
{
#ifdef _SCC_DEBUG
    if(unit>=UNIT_TYPE_COUNT) {
        toLog("WARNING: (Order::setUnit): Value out of range.");return;
    }
#endif
	this->unit=unit;
}

void Order::setIP(const unsigned int IP)
{
#ifdef _SCC_DEBUG
    if(IP>=MAX_LENGTH) {
        toLog("WARNING: (Order::setIP): Value out of range.");return;
    }
#endif
	this->IP=IP;
}

void Order::setRow(const unsigned int row)
{
#ifdef _SCC_DEBUG
    if(row>=MAX_LENGTH) {
        toLog("WARNING: (Order::setRow): Value out of range.");return;
    }
#endif
	this->row=row;
}

const unsigned int Order::getUnit() const
{
#ifdef _SCC_DEBUG
    if(unit>=UNIT_TYPE_COUNT) {
	        toLog("WARNING: (Order::getUnit): Variable not initialized.");return(0);
    }
#endif
	return(unit);
}

const unsigned int Order::getIP() const
{
#ifdef _SCC_DEBUG
    if(IP>=MAX_LENGTH) {
	        toLog("WARNING: (Order::getIP): Variable not initialized.");return(0);
    }
#endif
	return(IP);
}

const unsigned int Order::getRow() const
{
#ifdef _SCC_DEBUG
    if(row>=MAX_LENGTH) {
        toLog("WARNING: (Order::getRow): Variable not initialized.");return(0);
    }
#endif
	return(row);
}
	
