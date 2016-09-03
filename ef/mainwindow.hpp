#ifndef _GUI_MAINWINDOW_HPP
#define _GUI_MAINWINDOW_HPP

#include "../ui/window.hpp"

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

		void reloadOriginalSize();
	private:
		const Size helper(DC* dc, Point point, const unsigned int dx, const int i, const std::string& str) const;
		UI_Button* tab[MAX_TABS];
		unsigned int ani, ani2;
		bool gizmo;
		unsigned int gameTabCount;
		unsigned int gameNumber;
};

inline void MainWindow::setGizmo(const bool do_gizmo)
{
	gizmo = do_gizmo;
}

#endif

