#ifndef _UI_DIAGRAM_HPP
#define _UI_DIAGRAM_HPP

#include "graphpoint.hpp"
#include "statictext.hpp"
#include <string>

enum eGraphType {
	GRAPH_LINE,
	GRAPH_FILLED_LINE,
	GRAPH_BOX,
	GRAPH_SECOND_BOX, // need_supply!

	MAX_GRAPH_TYPES
};

class UI_Diagram : public UI_Object
{
	public:
		UI_Diagram(UI_Object* diagram_parent, 
				const Rect& diagram_rect, 
				const Size distance_bottom_right, 
				const ePositionMode position_mode);
		~UI_Diagram();

		void resetData();
		void process();
		void draw() const;

		void reloadOriginalSize();

		void showValues(const bool show = true);

		void addItem(const unsigned int item_text, const eColor text_color, const eColor value_color);
		void addLineGraph(const ePen graph_pen, const ePen graph_dashed_pen);
		void addBoxGraph();
		void addSecondBoxGraph(const ePen graph_pen, const eBrush graph_brush);
		void setItemValues(const std::vector<std::string>& value_list);

		std::vector<std::list<UI_GraphPoint> >& getGraphList();
		std::vector<unsigned int> graphMax;
	private:
		std::vector<std::list<UI_GraphPoint> > graphList;
		std::vector<eGraphType> graphType;
		std::vector<ePen> graphPen;
		std::vector<ePen> graphDashedPen;
		std::vector<eBrush> graphBrush;
	
		std::vector<UI_StaticText*> itemText;
		std::vector<UI_StaticText*> itemValue;
	
		UI_Diagram(const UI_Diagram& object);
		UI_Diagram& operator=(const UI_Diagram& object);
};

inline std::vector<std::list<UI_GraphPoint> >& UI_Diagram::getGraphList() {
	return(graphList);
}


#endif // _UI_DIAGRAM_HPP
