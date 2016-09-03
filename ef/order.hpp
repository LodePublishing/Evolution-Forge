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

#endif // _EF_ORDER_HPP
