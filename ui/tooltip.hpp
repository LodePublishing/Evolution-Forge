#ifndef _UI_TOOLTIP_HPP
#define _UI_TOOLTIP_HPP

#include "longtext.hpp"

class UI_ToolTip : public UI_LongText
{
	public:
		UI_ToolTip(UI_Object* parent_object, const std::string& tooltip_text);
		UI_ToolTip(UI_Object* parent_object, const eString tooltip_text);
		~UI_ToolTip();
	
		void reloadOriginalSize();
		void draw(DC* dc) const;
		void process();
	private:
		Rect boxSize;
	
		UI_ToolTip& operator=(const UI_ToolTip& object);
		UI_ToolTip(const UI_ToolTip& object);
};

#endif

