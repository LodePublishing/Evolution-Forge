#ifndef _GUI_BODIAPOINT_HPP
#define _GUI_BODIAPOINT_HPP

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

#endif


