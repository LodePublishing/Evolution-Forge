#ifndef _GUI_BODIAPOINT_HPP
#define _GUI_BODIAPOINT_HPP
#include <vector>

class BoDiagramPoint
{
	public:
		BoDiagramPoint();
		~BoDiagramPoint();
		void setTargetY(unsigned int stat_num, unsigned int number_y1, unsigned int number_y2);
		void setTargetX(unsigned int number_x);
		unsigned int current_x;
		unsigned int start_x;
		unsigned int target_x;
		std::vector<unsigned int> current_y1;
		std::vector<unsigned int> start_y1;
		std::vector<unsigned int> target_y1;
		std::vector<unsigned int> current_y2;
		std::vector<unsigned int> start_y2;
		std::vector<unsigned int> target_y2;
		static std::vector<unsigned int> max;
		static unsigned int MAX_ENTRIES;
		std::vector<unsigned int> highlight;
		bool initialized;
		unsigned int unit;
		unsigned int id;
};

#endif


