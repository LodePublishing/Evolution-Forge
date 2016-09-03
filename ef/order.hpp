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
		
		void setUnit(const unsigned int unit_type);
		void setIP(const unsigned int ip);
		void setRow(const unsigned int order_row);

		const unsigned int getUnit() const;
		const unsigned int getIP() const;
		const unsigned int getRow() const;
	
		unsigned int blend;
		unsigned int blendTarget;
		unsigned int blendStart;
//build order list
		Rect rect;
		Rect brect;
		Rect target;
		Rect btarget;
		Rect start;
		Rect bstart;
		unsigned long marker;
		bool bonew;
		bool checked;

		
	private:
//build order graph
		unsigned int unit;
//	int mins,gas,time,location,needSupply,haveSupply,forceFacilityCount,availibleFacilityCount,successType,successUnit,facility,code,forceCount;
		unsigned int IP;
		unsigned int row;
//	int mins, color  etc.
};

#endif // _EF_ORDER_HPP
