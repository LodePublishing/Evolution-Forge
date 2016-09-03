#ifndef _GUI_LOCATIONMENU_HPP
#define _GUI_LOCATIONMENU_HPP

#include "../ui/menu.hpp"
#include "../core/basicmap.hpp"

class LocationMenu : public UI_Menu
{
	public:
		LocationMenu(UI_Object* location_parent, const BASIC_MAP* location_map, const Rect& location_rect);
		LocationMenu(UI_Object* location_parent, const std::list<std::string>& location_list, const Rect& location_rect);
		~LocationMenu();
		void process();
		void draw() const;
		void resetData();
		void assignMap(const BASIC_MAP* location_map);
		void assignMap(const std::list<std::string>& string_list);
		void reloadOriginalSize();
	private:
		std::list<std::string> locationList;
		LocationMenu(const LocationMenu& object);
		LocationMenu& operator=(const LocationMenu& object);
};

#endif // _GUI_LOCATIONMENU_HPP

