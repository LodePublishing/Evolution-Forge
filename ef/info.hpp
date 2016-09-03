#ifndef _GUI_INFO_HPP
#define _GUI_INFO_HPP

#include "../ui/window.hpp"
#include "../core/anarace.hpp"
#include "order.hpp"

class InfoWindow : public UI_Window
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

		void setBx(const signed int b_x);
		void setBWidth(const unsigned int b_width);
		void setUnit(const unsigned int unit_type);
		void setIP(const unsigned int ip);
		const unsigned int getIP() const; 
		const unsigned int isSet() const;
		void setupOk(const unsigned int ok); // already setuped by bowindow
		void assignAnarace(ANARACE* info_anarace);
		
	private:
		signed int bx;
		unsigned int bwidth, unit, key;
		UI_StaticText* text;
		unsigned int setup;
		unsigned int IP;
		ANARACE* anarace;
};

#endif // _GUI_INFO_HPP

