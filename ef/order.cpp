#include "order.hpp"

Order::Order()
{
	blend=0;
	blendTarget=0;
	blendStart=0;
	rect=Rect(0,0,0,0);
	brect=Rect(0,0,0,0);
	target=Rect(0,0,0,0);
	btarget=Rect(0,0,0,0);
	start=Rect(0,0,0,0);
	bstart=Rect(0,0,0,0);
	marker=0;
	bonew=false;
	checked=false;
	unit=0;
	IP=0;
	row=0;
}

Order::~Order()
{
}

void Order::setUnit(const int unit)
{
#ifdef _SCC_DEBUG
    if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
        toLog("WARNING: (Order::setUnit): Value out of range.");return;
    }
#endif
	this->unit=unit;
}

void Order::setIP(const int IP)
{
#ifdef _SCC_DEBUG
    if((IP<0)||(IP>=MAX_LENGTH)) {
        toLog("WARNING: (Order::setIP): Value out of range.");return;
    }
#endif
	this->IP=IP;
}

void Order::setRow(const int row)
{
#ifdef _SCC_DEBUG
    if((row<0)||(row>=MAX_LENGTH)) {
        toLog("WARNING: (Order::setRow): Value out of range.");return;
    }
#endif
	this->row=row;
}

const int Order::getUnit() const
{
#ifdef _SCC_DEBUG
    if((unit<0)||(unit>=UNIT_TYPE_COUNT)) {
	        toLog("WARNING: (Order::getUnit): Variable not initialized.");return(0);
    }
#endif
	return(unit);
}

const int Order::getIP() const
{
#ifdef _SCC_DEBUG
    if((IP<0)||(IP>=MAX_LENGTH)) {
	        toLog("WARNING: (Order::getIP): Variable not initialized.");return(0);
    }
#endif
	return(IP);
}

const int Order::getRow() const
{
#ifdef _SCC_DEBUG
    if((row<0)||(row>=MAX_LENGTH)) {
        toLog("WARNING: (Order::getRow): Variable not initialized.");return(0);
    }
#endif
	return(row);
}
	
