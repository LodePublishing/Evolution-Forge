#ifndef _GUI_MAINWINDOW_HPP
#define _GUI_MAINWINDOW_HPP

#include "../ui/window.hpp"
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
		void addNewGameTab();
		void removeGameTab(const unsigned int game_number);
		void setGizmo(const bool do_gizmo = true);
		void continueOptimizationAnimation(const bool running);
		
		const unsigned int getGameTabCount() const;

		void reloadStrings();
		void reloadOriginalSize();

		// absolute Nummer
		void activateTab(const eTabs tab_number);
		// nach Reihenfolge:
		void activateTabNumber(unsigned int tab_number);
		const eTabs getCurrentTab() const;
	private:
		const Size helper(DC* dc, Point point, const unsigned int dx, const int i, const std::string& str) const;
		UI_Button* tab[MAX_TABS];

		unsigned int ani, ani2;
		bool gizmo;
		unsigned int gameTabCount;
		unsigned int gameNumber;
		unsigned int gameNumbers[MAX_TABS];
	
		UI_Group* leftTabs;
		UI_Group* rightTabs;
		eTabs oldTab;	
};

inline void MainWindow::setGizmo(const bool do_gizmo)
{
	gizmo = do_gizmo;
}

#endif

