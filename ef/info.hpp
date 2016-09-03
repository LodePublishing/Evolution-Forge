#ifndef _GUI_INFO_HPP
#define _GUI_INFO_HPP

#include "../ui/window.hpp"
#include "../core/anabuildorder.hpp"
#include "boentry.hpp"
#include "bograph.hpp"

class InfoWindow : public UI_Window
{
	public:
		InfoWindow& operator=(const InfoWindow& object);
		InfoWindow(const InfoWindow& object);
		InfoWindow(UI_Object* parent, ANABUILDORDER* anarace, const unsigned int windowNumber);
		~InfoWindow();
		void resetData();
		void process();
		void draw(DC* dc) const;

		const unsigned int getUnit() const;
		void setProgram(const PROGRAM& info_program);
		void assignAnarace(ANABUILDORDER* info_anarace);
		void assignBo(const BoEntry* info_bo);
		void assignBg(const BoGraphEntry* info_bg);
		const BoEntry* bo;
		const BoGraphEntry* bg;

	private:
		unsigned int unit;
		UI_StaticText* text;
		PROGRAM program;
		ANABUILDORDER* anarace;
};

#endif // _GUI_INFO_HPP

