#ifndef _GUI_INFO_HPP
#define _GUI_INFO_HPP

#include "../ui/window.hpp"
#include "../core/anarace.hpp"
#include "boentry.hpp"
#include "bograph.hpp"

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

		const unsigned int getUnit() const;
		void setUnit(const unsigned int unit_type);
		void setIP(const unsigned int ip);
		const unsigned int getIP() const; 
		void assignAnarace(ANARACE* info_anarace);
		void assignBo(const BoEntry* info_bo);
		void assignBg(const BoGraphEntry* info_bg);
		
	private:
		unsigned int unit;
		UI_StaticText* text;
		unsigned int IP;
		ANARACE* anarace;
		const BoEntry* bo;
		const BoGraphEntry* bg;
};

#endif // _GUI_INFO_HPP

