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
		void reset();
		void resetToEnd();

		void setClientHeight(const unsigned int height);
		void setClientTargetHeight(const unsigned int height);

	protected:
	private:
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
};

#endif // _UI_SCROLLBAR_HPP

