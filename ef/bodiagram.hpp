#ifndef _GUI_BODIAGRAM_HPP
#define _GUI_BODIAGRAM_HPP

#include "../ui/window.hpp"
#include "../core/database.hpp"
#include "../core/anabuildorder.hpp"

class BoDiagramWindow : public UI_Window 
{
	public:
		BoDiagramWindow(UI_Object* bod_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		BoDiagramWindow(const BoDiagramWindow& object);
		BoDiagramWindow& operator=(const BoDiagramWindow& object);
		~BoDiagramWindow();

		void resetData();
		void process();
		void processList();
		void draw(DC * dc) const;

		void assignAnarace(ANABUILDORDER* bod_anarace);
		void setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		void reloadOriginalSize();

		const std::list<unsigned int>& getSelectedItems() const;
	private:
		enum haves
		{
			HAVE_MINERALS,
			HAVE_GAS,
			HAVE_SUPPLY,
			NEED_SUPPLY,
			TOTAL_STATS
		};
	
		ANABUILDORDER* anarace;
		
		std::list<unsigned int> selectedItems;
		
		unsigned int count;
		bool bold;
		unsigned int mouseTime;
		Point oldMouse;

		Point current[TOTAL_STATS][MAX_LENGTH*2+2];
		Point start[TOTAL_STATS][MAX_LENGTH*2+2];
		Point target[TOTAL_STATS][MAX_LENGTH*2+2];

		unsigned int gameNumber;
		unsigned int gameMax;
		unsigned int playerNumber;
		unsigned int playerMax;
};

inline const std::list<unsigned int>& BoDiagramWindow::getSelectedItems() const {
	return(selectedItems);
}

#endif // _GUI_BODIAGRAM_HPP
