#ifndef _GUI_INFO_HPP
#define _GUI_INFO_HPP

#include "../ui/window.hpp"
#include "../core/anarace.hpp"
#include "order.hpp"

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
		InfoWindow& operator=(const InfoWindow& object);
		InfoWindow(const InfoWindow& object);
		InfoWindow(UI_Object* parent, ANARACE* anarace, const unsigned int windowNumber);
		~InfoWindow();
		void resetData();
		void process();
		void draw(DC* dc) const;

		const signed int getBx() const;
		const unsigned int getBWidth() const;
		const unsigned int getUnit() const;

		void setBx(const signed int bx);
		void setBWidth(const unsigned int bwidth);
		void setUnit(const unsigned int unit);
		void setIP(const unsigned int key);
		const unsigned int getIP() const; 
		const unsigned int isSet() const;
		void setupOk(const unsigned int ok); // already setuped by bowindow
		
	private:
		signed int bx;
		unsigned int bwidth, unit, key;
		UI_StaticText* text;
		unsigned int setup;
		unsigned int IP;
		ANARACE* anarace;
};

#endif // _GUI_INFO_HPP

