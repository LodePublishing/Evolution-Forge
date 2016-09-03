#ifndef _GUI_BOMENU_HPP
#define _GUI_BOMENU_HPP

#include "../ui/menu.hpp"
#include "../core/database.hpp"

class BoMenu : public UI_Menu
{
	public:
		BoMenu(UI_Object* bo_parent, const Rect bo_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		BoMenu(const BoMenu& object);
		BoMenu& operator=(const BoMenu& object);
		~BoMenu();
		void process();
		void draw(DC* dc) const;
		void resetData();
		void assignAnarace(ANABUILDORDER* bo_anarace);
		void reloadOriginalSize();
		const unsigned int getNumber() const;
	private:
		unsigned int number;
		ANABUILDORDER* anarace;
};

inline const unsigned int BoMenu::getNumber() const {
	return(number);
}

#endif // _GUI_BOMENU_HPP
