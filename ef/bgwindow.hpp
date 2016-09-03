#ifndef _GUI_BOGRAPH_WINDOW_HPP
#define _GUI_BOGRAPH_WINDOW_HPP

#include "../ui/window.hpp"
#include "bgline.hpp"
#include "../core/anabuildorder.hpp"

#define BOGRAPH_MAX_LINES 30


class BoGraphWindow:public UI_Window
{
	public:
		BoGraphWindow(UI_Object* bograph_parent, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_players);
		BoGraphWindow(UI_Object* bograph_parent, const unsigned int bograph_window_number);
		~BoGraphWindow();

		void reloadOriginalSize();
		void resetData();
		void process();
		void processList();
		void draw(DC* dc) const;
	    	void mouseHasMoved();
		void assignAnarace(ANABUILDORDER* bograph_anarace);
		
		const signed int getSelectedItem() const;
		void setSelected(const unsigned int selected);
	private:
//		BoGraphWindow(const BoGraphWindow& object); // don't allow copying... wouldn't make sense... :-/
//		BoGraphWindow& operator=(const BoGraphWindow& object);
	
		std::list<BoGraphLine*> boGraphLine;

		void checkForInfoWindow();
		unsigned int markAni;
		ANABUILDORDER* anarace;
		signed int selectedItem;
//		std::list<UI_StaticText*> legend;
};

inline const signed int BoGraphWindow::getSelectedItem() const {
	return(selectedItem);
}

#endif

