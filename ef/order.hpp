#ifndef _EF_ORDER_HPP
#define _EF_ORDER_HPP

#include "../ui/object.hpp"

class Order
{
	public:
		Order(const Order& order);
		Order();
		~Order();
		Order& operator=(const Order& order);

		// Ascending IP sorting function
		struct OrderDescendingIPSort {
			const bool operator()(const Order* const& orderStart, const Order* const& orderEnd) const {
				return orderStart->getIP() > orderEnd->getIP();
			}
		};
		
		void setMarker(const unsigned long order_marker);
		void setUnit(const unsigned int unit_type);
		void setIP(const unsigned int ip);

		const unsigned long getMarker() const;
		const unsigned int getUnit() const;
		const unsigned int getIP() const;
	private:
		unsigned int unit;
		unsigned int IP;
		unsigned long marker;
};

inline void Order::setMarker(const unsigned long order_marker)
{
	marker = order_marker;
}

inline const unsigned long Order::getMarker() const
{
	return(marker);
}

inline void Order::setUnit(const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("WARNING: (Order::setUnit): Value out of range.");return;
	}
#endif
	unit = unit_type;
}

inline void Order::setIP(const unsigned int ip)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("WARNING: (Order::setIP): Value out of range.");return;
	}
#endif
	IP = ip;
}

inline const unsigned int Order::getUnit() const
{
#ifdef _SCC_DEBUG
	if(unit >= UNIT_TYPE_COUNT) {
		toLog("WARNING: (Order::getUnit): Variable not initialized.");return(0);
	}
#endif
	return(unit);
}

inline const unsigned int Order::getIP() const
{
#ifdef _SCC_DEBUG
	if(IP >= MAX_LENGTH) {
		toLog("WARNING: (Order::getIP): Variable not initialized.");return(0);
	}
#endif
	return(IP);
}

#endif // _EF_ORDER_HPP
