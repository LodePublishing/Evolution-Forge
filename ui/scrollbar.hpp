#ifndef _UI_SCROLLBAR_HPP
#define _UI_SCROLLBAR_HPP

#include "button.hpp"

class UI_Window;

class UI_ScrollBar : public UI_Object
{
	public:
		UI_ScrollBar(UI_Object* scroll_parent, /*const Rect& clientrect?*/ const unsigned int start_y, const unsigned int max_height, const bool scroll_hideable = false);
		~UI_ScrollBar();

		void process(); // process messages, continue animation etc.
		void draw(DC* dc) const;

		void moveUp();
		void moveDown();
		void moveToTop();
		void moveToBottom();

		void setClientHeight(const unsigned int height);
		void setClientTargetHeight(const unsigned int height);
		
		void setFirstItemY(const signed int first_item_y);
		void setLastItemY(const signed int last_item_y);
		const signed int getFirstItemY() const;
		const signed int getLastItemY() const;

		void checkBoundsOfChildren(const signed int upper_bound, const signed int lower_bound);
	
	protected:
	private:
		signed int firstItemY;
		signed int lastItemY;

		signed int startY;
		
		UI_Button* add;
		UI_Button* sub;

		bool hideable; // hides if total height is smaller maxheight
		signed int internalScrollY;
		unsigned int internalHeight;
		signed int currentScrollY;
		signed targetScrollY;
		unsigned int barHeight;
		
		unsigned int clientHeight;
		unsigned int clientTargetHeight;
		unsigned int totalHeight;

		unsigned int maxHeight;

		UI_ScrollBar& operator=(const UI_ScrollBar& object);
		UI_ScrollBar(const UI_ScrollBar& object);
};

inline void UI_ScrollBar::setFirstItemY(const signed int first_item_y) 
{
#ifdef _SCC_DEBUG
	if(first_item_y > 90000)
	{
		toErrorLog("DEBUG: (UI_ScrollBar::setFirstItemY): Value first_item_y out of range.");return;
	}
#endif
	firstItemY = first_item_y;
}

inline void UI_ScrollBar::setLastItemY(const signed int last_item_y) 
{
#ifdef _SCC_DEBUG
	if(last_item_y > 90000)
	{
		toErrorLog("DEBUG: (UI_ScrollBar::setLastItemY): Value last_item_y out of range.");return;
	}
#endif
	lastItemY = last_item_y;
}

inline const signed int UI_ScrollBar::getFirstItemY() const 
{
#ifdef _SCC_DEBUG
	if(firstItemY > 90000)
	{
		toErrorLog("DEBUG: (UI_ScrollBar::getFirstItemY): Value firstItemY out of range.");return(0);
	}
#endif
	return(firstItemY);
}

inline const signed int UI_ScrollBar::getLastItemY() const 
{
#ifdef _SCC_DEBUG
	if(lastItemY > 90000)
	{
		toErrorLog("DEBUG: (UI_ScrollBar::getLastItemY): Value lastItemY out of range.");return(0);
	}
#endif
	return(lastItemY);
}

#endif // _UI_SCROLLBAR_HPP

