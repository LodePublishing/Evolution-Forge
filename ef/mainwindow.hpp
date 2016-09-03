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
	private:
		UI_Button* tab[MAX_TABS];

		unsigned int gameTabCount;
		unsigned int gameNumber;
		unsigned int gameNumbers[MAX_TABS];
	
		UI_Group* leftTabs;
		UI_Group* rightTabs;
		eTabs oldTab;	
};

#endif

