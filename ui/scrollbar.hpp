#ifndef _UI_SCROLLBAR_HPP
#define _UI_SCROLLBAR_HPP

#include "button.hpp"

class UI_Scrollbar:public UI_Object
{
	public:
		UI_Scrollbar& operator=(const UI_Scrollbar& object);
		UI_Scrollbar(const UI_Scrollbar& object);
		UI_Scrollbar(UI_Object* scroll_parent, Rect scroll_rect, Rect scroll_max_rect, const bool scroll_horizontal = false, const signed int scroll_discrete = 0, const bool scroll_hideable = false, const bool scroll_small = true);
		~UI_Scrollbar();

		void process(); // process messages, continue animation etc.
		void draw(DC* dc) const;
        const signed int getScrollY() const;

	protected:
	private:
		UI_Button* add;
		UI_Button* sub;

		bool horizontal;		
		bool hideable; // hides if total height is smaller maxheight
		bool small; // small version? for very small windows
		signed int discrete; // discrete >0 scrollbar move? here is the maxHeight

		signed int internalScrollY;
		unsigned int internalHeight;
		
		signed int currentScrollY;
		unsigned int currentHeight;
		
};

#endif // _UI_SCROLLBAR_HPP

