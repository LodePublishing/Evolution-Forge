#ifndef _GUI_INFO_HPP
#define _GUI_INFO_HPP

#include "../ui/window.hpp"
#include "../core/anarace.hpp"

struct Order
{
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
//build order graph
	int unit;
	int checked;
//	int mins,gas,time,location,needSupply,haveSupply,forceFacilityCount,availibleFacilityCount,successType,successUnit,facility,code,forceCount;
	int bonew;
	int IP;
	int row;
	long marker;
//	int mins, color  etc.
};

const char error_message[ERROR_MESSAGES][25]=
{
	"possible.", //= OK
	"enough minerals.", 
	"enough gas.",
	"supply is satisfied.",
	"prerequisite",
	"facility",
	"hell freezes.", //timeout
	"SC2 comes out." //unknown
};

class InfoWindow:public UI_Window
{
	public:
		InfoWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber);
		~InfoWindow();
		void resetData();
		void process();
		void draw(DC* dc) const;

		const int getBx() const;
		const int getBWidth() const;
		const int getUnit() const;

		void setBx(const int bx);
		void setBWidth(const int bwidth);
		void setUnit(const int unit);
		void setIP(const int key);
		const int getIP() const; 
		const int isSet() const;
		void setupOk(const int ok); // already setuped by bowindow
		
	private:
		int bx,bwidth,unit,key;
		int setup;
		int IP;
		ANARACE* anarace;
};

#endif

