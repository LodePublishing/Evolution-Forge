#include "order.hpp"

Order::Order():
	unit(0),
	IP(0),
	marker(0)
{ }

Order::~Order()
{ }

Order::Order(const Order& object) :
	unit(object.unit),
	IP(object.IP),
	marker(object.marker)
{ }

Order& Order::operator=(const Order& order)
{
	unit=order.unit;
	IP=order.IP;
	marker=order.marker;
	return(*this);
}

void Order::setMarker(const unsigned long order_marker)
{
	marker = order_marker;
}

const unsigned long Order::getMarker() const
{
	return(marker);
}

void Order::setUnit(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("WARNING: (Order::setUnit): Value out of range.");return;
	}
#endif
	unit = unit_type;
}

void Order::setIP(const unsigned int ip)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("WARNING: (Order::setIP): Value out of range.");return;
	}
#endif
	IP = ip;
}

const unsigned int Order::getUnit() const
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT) {
		toLog("WARNING: (Order::getUnit): Variable not initialized.");return(0);
	}
#endif
	return(unit);
}

const unsigned int Order::getIP() const
{
#ifdef _SCC_DEBUG
	if(IP >= MAX_LENGTH) {
		toLog("WARNING: (Order::getIP): Variable not initialized.");return(0);
	}
#endif
	return(IP);
}

