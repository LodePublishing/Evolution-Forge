#ifndef _UI_GRAPH_POINT_HPP
#define _UI_GRAPH_POINT_HPP
#include <string>

class UI_GraphPoint
{
	public:
		UI_GraphPoint();
		UI_GraphPoint(const unsigned int target_x, const unsigned int number_y1, const unsigned int number_y2, const std::string& point_text = "");
		~UI_GraphPoint();
		
		void adjustTargetY(const unsigned int number_y1, const unsigned number_y2);
		void setTargetY(const unsigned int number_y1, const unsigned int number_y2);
		void setTargetX(const unsigned int target_x);
		void setText(const std::string& point_text);

		const bool move();
		unsigned int current_y1;
		unsigned int start_y1;
		unsigned int target_y1;
		unsigned int current_y2;
		unsigned int start_y2;
		unsigned int target_y2;
		
		unsigned int x;
		
		unsigned int highlight;
		
		std::string text;
};

#endif

