#ifndef _UI_TOOLTIP_HPP
#define _UI_TOOLTIP_HPP

#include "statictext.hpp"

class UI_Tooltip : UI_StaticText
{
	public:
		UI_Tooltip& operator=(const UI_Tooltip& object);
		UI_Tooltip(const UI_Tooltip& object);
		UI_Tooltip(UI_Object* parent_object, const eString text);
		~UI_Tooltip();
	
		void draw(DC* dc) const;
		void process();
	private:
		Rect boxSize;
};

#endif

