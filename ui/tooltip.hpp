#ifndef _UI_TOOLTIP_HPP
#define _UI_TOOLTIP_HPP

#include "statictext.hpp"

class UI_ToolTip : public UI_StaticText
{
	public:
		UI_ToolTip& operator=(const UI_ToolTip& object);
		UI_ToolTip(const UI_ToolTip& object);
		UI_ToolTip(UI_Object* parent_object, const eString text);
		~UI_ToolTip();
	
		void draw(DC* dc) const;
		void process();
	private:
		Rect boxSize;
};

#endif

