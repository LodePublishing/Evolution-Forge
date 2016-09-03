#ifndef _GUI_STATISTICS_HPP
#define _GUI_STATISTICS_HPP

#include "../ui/window.hpp"
#include "../core/anabuildorder.hpp"

enum eStatEntry
{
    MINERALS_STAT_ENTRY,
    GAS_STAT_ENTRY,
    TIME_STAT_ENTRY,
    FORCE_STAT_ENTRY,
    AVERAGE_BO_LENGTH_STAT_ENTRY,
    FITNESS_AVERAGE_STAT_ENTRY,
    FITNESS_VARIANCE_STAT_ENTRY,
//    GENERATIONS_LEFT_STAT_ENTRY,
    FPS_STAT_ENTRY,
    MAX_STAT_ENTRY
};


class StatisticsWindow : public UI_Window
{
	public:
		StatisticsWindow(UI_Object* stat_parent, const unsigned int stat_window_number);
		~StatisticsWindow();
		
		void process();
		void draw(DC* dc) const;
		
		void resetData();
		void assignAnarace(ANABUILDORDER* stat_anarace);
		
	private:
		void showGraph(DC* dc, const unsigned int* data, const unsigned int min, const unsigned int max, const Color col, const bool bold=false) const;
		
		int ani;
		
		UI_Button* statEntry[MAX_STAT_ENTRY];
		long unsigned int start_time;
		const ANABUILDORDER* anarace;
		unsigned int data[MAX_STAT_ENTRY][200];
		unsigned int maxdata[MAX_STAT_ENTRY];
		unsigned int oldDataCounter[MAX_STAT_ENTRY][20];
		unsigned int oldData[MAX_STAT_ENTRY][20];

		unsigned int average[100];
		unsigned int averagecounter;
		unsigned int graph_ani;
		bool wasResetted;
};
#endif

