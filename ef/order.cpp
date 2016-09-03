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
	unit = order.unit;
	IP = order.IP;
	marker = order.marker;
	return(*this);
}


