#ifndef _UI_SCROLLBAR_HPP
#define _UI_SCROLLBAR_HPP

#include "button.hpp"

class UI_Window;

class UI_Scrollbar : public UI_Object
{
	public:
		UI_Scrollbar& operator=(const UI_Scrollbar& object);
		UI_Scrollbar(const UI_Scrollbar& object);
		UI_Scrollbar(UI_Object* scroll_parent, /*const Rect& clientrect?*/ const unsigned int start_y, const unsigned int max_height, const bool scroll_hideable = false);
		~UI_Scrollbar();

		void process(); // process messages, continue animation etc.
		void draw(DC* dc) const;

		void moveUp();
		void moveDown();
		void moveToTop();
		void moveToBottom();

		void setClientHeight(const unsigned int height);
		void setClientTargetHeight(const unsigned int height);

		void reloadStrings();
		
		void setFirstItemY(const signed int first_item_y);
		void setLastItemY(const signed int last_item_y);
		const signed int getFirstItemY() const;
		const signed int getLastItemY() const;
	
	protected:
	private:
		signed int firstItemY;
		signed int lastItemY;
		
		UI_Button* add;
		UI_Button* sub;

		bool hideable; // hides if total height is smaller maxheight
		signed int internalScrollY;
		unsigned int internalHeight;
		signed int currentScrollY;
		unsigned int barHeight;
		
		unsigned int clientHeight;
		unsigned int clientTargetHeight;
		unsigned int totalHeight;

		unsigned int maxHeight;
};

inline void UI_Scrollbar::setFirstItemY(const signed int first_item_y) 
{
#ifdef _SCC_DEBUG
	if(first_item_y > 90000)
	{
		toLog("DEBUG: (UI_Scrollbar::setFirstItemY): Value first_item_y out of range.");return;
	}
#endif
	firstItemY = first_item_y;
}

inline void UI_Scrollbar::setLastItemY(const signed int last_item_y) 
{
#ifdef _SCC_DEBUG
	if(last_item_y > 90000)
	{
		toLog("DEBUG: (UI_Scrollbar::setLastItemY): Value last_item_y out of range.");return;
	}
#endif
	lastItemY = last_item_y;
}

inline const signed int UI_Scrollbar::getFirstItemY() const 
{
#ifdef _SCC_DEBUG
	if(firstItemY > 90000)
	{
		toLog("DEBUG: (UI_Scrollbar::getFirstItemY): Value firstItemY out of range.");return(0);
	}
#endif
	return(firstItemY);
}

inline const signed int UI_Scrollbar::getLastItemY() const 
{
#ifdef _SCC_DEBUG
	if(lastItemY > 90000)
	{
		toLog("DEBUG: (UI_Scrollbar::getLastItemY): Value lastItemY out of range.");return(0);
	}
#endif
	return(lastItemY);
}

#endif // _UI_SCROLLBAR_HPP

