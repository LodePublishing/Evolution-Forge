#ifndef _GUI_BODIAGRAM_HPP
#define _GUI_BODIAGRAM_HPP

#include "../ui/window.hpp"
#include "../core/database.hpp"
#include "../core/anabuildorder.hpp"

enum eHaveStats
{
	HAVE_MINERALS,
	HAVE_GAS,
	HAVE_SUPPLY,
	NEED_SUPPLY,
	TOTAL_STATS
};

class BoDiagramPoint
{
	public:
		BoDiagramPoint();
		~BoDiagramPoint();
		void setTargetY(eHaveStats stat_num, unsigned int number_y1, unsigned int number_y2);
		void setTargetX(unsigned int number_x);
		unsigned int current_x;
		unsigned int start_x;
		unsigned int target_x;
		unsigned int current_y1[TOTAL_STATS];
		unsigned int start_y1[TOTAL_STATS];
		unsigned int target_y1[TOTAL_STATS];
		unsigned int current_y2[TOTAL_STATS];
		unsigned int start_y2[TOTAL_STATS];
		unsigned int target_y2[TOTAL_STATS];
		bool initialized;
		static unsigned int max[TOTAL_STATS];

		unsigned int highlight[TOTAL_STATS];
		unsigned int unit;
};



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
