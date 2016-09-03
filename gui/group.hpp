#ifndef _UI_TITLE_GROUP_HPP
#define _UI_TITLE_GROUP_HPP

#include "statictext.hpp"

enum eGroupType
{
	CUSTOM_GROUP,
	ONE_COLOUMN_GROUP,
	TWO_COLOUMNS_GROUP,
	HORIZONTAL_GROUP,
	TWO_LINES_GROUP
};


// TODO eString erlauben!

// simplified window class, maybe inheritate UI_Window from UI_Group...
class UI_Group : public UI_Object
{
	public:
		UI_Group(UI_Object* group_parent, 
				const Rect& initial_rect, 
				const Size bottom_right_distance, 
				const eGroupType group_type, 
				const bool draw_background = false, 
				const ePositionMode position_mode = DO_NOT_ADJUST, 
				const eString txt = NULL_STRING);
		~UI_Group();

		void draw() const;
		void process();
		void reloadOriginalSize();

		void alignWidth(const unsigned int width);
		void calculateBoxSize();
	private:
		UI_StaticText* title;
		unsigned int number;
		bool highlighted;
		eGroupType groupType;
		bool drawBackground;
	
		UI_Group(const UI_Group& object);
		UI_Group& operator=(const UI_Group& object);
};

#endif

