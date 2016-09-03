#ifndef _GUI_MAINWINDOW_HPP
#define _GUI_MAINWINDOW_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"

enum eTabs
{
	HELP_TAB=28,
	SETTINGS_TAB,
//	MAP_TAB,
	DATABASE_TAB,	
	MAX_TABS
};

class MainWindow : public UI_Window
{
	public:
		MainWindow();
		~MainWindow();
		void process();
		void draw( DC* dc ) const;
		void reloadOriginalSize();
	
		void addNewGameTab();
		void removeGameTab(const unsigned int game_number);
		
		const unsigned int getGameTabCount() const;

		void reloadStrings();

		// absolute Nummer
		void activateTab(const eTabs tab_number);
		// nach Reihenfolge:
		void activateTabNumber(unsigned int tab_number);
		const eTabs getCurrentTab() const;

		const bool  markedForRemove() const;
		const bool  markedForNewGame() const;
		const bool hasTabChanged() const;
		const unsigned int getNewGameTab() const;
	private:
		UI_Button* tab[MAX_TABS];

		void scrollLeft();
		void scrollRight();
		UI_Button* removeCurrentTabButton;
		UI_Button* scrollLeftButton;
		UI_Button* scrollRightButton;

		unsigned int gameTabCount;
		unsigned int gameNumber;
		unsigned int gameNumbers[MAX_TABS];

		void adjustView();
		unsigned int viewTabs;
	
		UI_Group* leftTabs;
		UI_Group* rightTabs;
		eTabs oldTab;

		bool markForRemove;
		bool markForNewGame;
		bool tabHasChanged;
		unsigned int newGameTab;
};

inline const bool MainWindow::hasTabChanged() const {
	return(tabHasChanged);
}

inline const unsigned int MainWindow::getGameTabCount() const {
	return gameTabCount;
}

inline const unsigned int MainWindow::getNewGameTab() const {
	return(newGameTab);
}

inline const bool MainWindow::markedForNewGame() const {
	return(markForNewGame);
}
inline const bool MainWindow::markedForRemove() const {
	return(markForRemove);
}
#endif

