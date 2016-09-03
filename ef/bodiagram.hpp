#ifndef _GUI_BODIAGRAM_HPP
#define _GUI_BODIAGRAM_HPP

#include "../ui/window.hpp"
#include "../core/database.hpp"
#include "../core/anabuildorder.hpp"
#include "configuration.hpp"
#include "bodiapoint.hpp"

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
		ANABUILDORDER* anarace;
		
		std::list<unsigned int> selectedItems;
		
		unsigned int count;
		bool bold;
		unsigned int mouseTime;
		unsigned int totalTime;
		Point oldMouse;


		std::list<BoDiagramPoint> diagramList;

		unsigned int gameNumber;
		unsigned int gameMax;
		unsigned int playerNumber;
		unsigned int playerMax;

		UI_StaticText* minerals;
		UI_StaticText* gas;
		UI_StaticText* supply;
		UI_StaticText* time;
		UI_StaticText* mineralsNumber;
		UI_StaticText* gasNumber;
		UI_StaticText* supplyNumber;
		UI_StaticText* timeNumber;	
};

inline const std::list<unsigned int>& BoDiagramWindow::getSelectedItems() const {
	return(selectedItems);
}

#endif // _GUI_BODIAGRAM_HPP
