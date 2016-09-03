#ifndef _EF_ORDER_HPP
#define _EF_ORDER_HPP

#include "../ui/object.hpp"

class Order
{
	public:
		Order();
		~Order();
		void operator=(const Order& order);
		int blend;
		int blendTarget;
		int blendStart;
//build order list
		Rect rect;
		Rect brect;
		Rect target;
		Rect btarget;
		Rect start;
		Rect bstart;
		long marker;
		bool bonew;
		bool checked;

		void setUnit(const int unit);
		void setIP(const int IP);
		void setRow(const int row);

		const int getUnit() const;
		const int getIP() const;
		const int getRow() const;
		
	private:
//build order graph
		int unit;
//	int mins,gas,time,location,needSupply,haveSupply,forceFacilityCount,availibleFacilityCount,successType,successUnit,facility,code,forceCount;
		int IP;
		int row;
//	int mins, color  etc.
};

#endif // _EF_ORDER_HPP
