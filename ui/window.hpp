#ifndef _UI_WINDOW_HPP
#define _UI_WINDOW_HPP

extern const unsigned int MIN_HEIGHT; // TODO in bograph?

#include "object.hpp"
#include "radio.hpp"
#include "scrollbar.hpp"

enum eIsScrolled
{
	NOT_SCROLLED,
	SCROLLED
};

enum eIsTabbed
{
	NOT_TABBED,
	TABBED
};

enum eIsAutoAdjust
{
	NO_AUTO_SIZE_ADJUST,
	AUTO_SIZE_ADJUST
};


class UI_Window : public UI_Object
{
	public:
		UI_Window& operator=(const UI_Window& object);
		UI_Window(const UI_Window& object);

		UI_Window(UI_Object* window_parent, const eString window_title_string, const eWindow window_type, const unsigned int window_number, const eIsScrolled window_is_scrolled=NOT_SCROLLED, const eIsAutoAdjust win_is_auto_adjust=NO_AUTO_SIZE_ADJUST, const eIsTabbed win_is_tabbed=NOT_TABBED, const Rect win_client_area=Rect(0,0,1280,1024));
		~UI_Window();

// returns position and size of the client area
		const Rect& getRelativeClientRect() const;
		const Rect getAbsoluteClientRect() const;
		const Point getRelativeClientRectPosition() const;
		const Point getAbsoluteClientRectPosition() const;
		const unsigned int getClientRectHeight() const;
		const unsigned int getClientRectWidth() const;
		const Size& getClientRectSize() const;
		const signed int getRelativeClientRectUpperBound() const;
		const signed int getRelativeClientRectLeftBound() const;
		const signed int getRelativeClientRectLowerBound() const;
		const signed int getRelativeClientRectRightBound() const;
		const signed int getAbsoluteClientRectUpperBound() const;
		const signed int getAbsoluteClientRectLeftBound() const;
		const signed int getAbsoluteClientRectLowerBound() const;
		const signed int getAbsoluteClientRectRightBound() const;

		const unsigned int getClientTargetHeight() const;
		
// do windows size changes smoothly		
		void adjustClientRect();

//		void setRahmen(Rect rahmen);
		void addTab(UI_Button* tab_button);
		const bool tabWasChanged() const;

		void process();
		void draw(DC* dc) const;

		UI_Object* checkToolTip();
		UI_Object* checkHighlight();

// 		reconfigure rectangles depending on current theme settings
		void updateRectangles(const unsigned int maxPlayer);
		
		static const bool getChangedFlag();
		static void setChangedFlag(const bool flag=true);
		static void changeAccepted();

		static const bool getResetFlag();
		static void setResetFlag(const bool flag=true);
		static void resetAccepted();

		void setTitleParameter(const std::string& p);

		void updateBorders();

		const eTab getCurrentTab() const;

		void forcePressTab(const eTab press_tab);

		static unsigned int rectnumber;
		const bool fitItemToRelativeClientRect(const Rect& rectangle, const unsigned int adjust = 0);
		const bool fitItemToAbsoluteClientRect(const Rect& rectangle, const unsigned int adjust = 0);

		UI_Object* getScrollbar() const; 
		const unsigned int getMaxHeight() const;

		void wheelUp();
		void wheelDown();
		void moveScrollbarToTop();
		void moveScrollbarToBottom();

	protected:
//		bool isMouseInsideClientArea();
		UI_Radio* tabRow;
	private:
		UI_Button* tab[MAX_TABS];
		
		eWindow window;
		
		eTab currentTab; // maybe move to 'theme'

// no set/get for title as this is unique and does not change
		eString titleString;
		std::string titleParameter;
		
		Rect originalRect;

// actual painted borders
		Rect border;
		Rect outerBorder;

// client area
		Rect clientRect;
		Rect clientStartRect;
		Rect clientTargetRect;
// rectangle for max size of client area

		Rect originalClientRect;

		unsigned int maxHeight;

		void drawTabs(DC* dc) const;
		void drawTitle(DC* dc) const;
		void drawToolTip(DC* dc, const Point p, const std::string* tip) const;

		unsigned int windowNumber;
		eIsAutoAdjust isAutoAdjust;
// has this window a ScrollBar?
		eIsScrolled isScrollable;
// has this window tab buttons at the top?
		eIsTabbed isTabbed;
	
		bool highlighted;

		static bool changedFlag; 
		static bool resetFlag;
		
// this windows may have a scroll bar		
		UI_Scrollbar* scrollBar;
};

inline const unsigned int UI_Window::getMaxHeight() const {
	return(maxHeight);
}

inline UI_Object* UI_Window::getScrollbar() const {
	return(scrollBar);
}

inline const Rect& UI_Window::getRelativeClientRect() const {
	return(clientRect);
}

inline const Rect UI_Window::getAbsoluteClientRect() const {
	return(Rect(getAbsoluteClientRectPosition(), getClientRectSize()));
}

inline const Point UI_Window::getRelativeClientRectPosition() const {
	return(clientRect.GetTopLeft());
}

inline const Point UI_Window::getAbsoluteClientRectPosition() const {
	return(clientRect.GetTopLeft()+getAbsolutePosition());
}

inline const unsigned int UI_Window::getClientRectHeight() const {
	return(clientRect.GetHeight());
}

inline const unsigned int UI_Window::getClientRectWidth() const {
	return(clientRect.GetWidth());
}

inline const Size& UI_Window::getClientRectSize() const {
	return(clientRect.GetSize());
}

inline const signed int UI_Window::getRelativeClientRectUpperBound() const {
	return(clientRect.GetTop());
}

inline const signed int UI_Window::getRelativeClientRectLeftBound() const {
	return(clientRect.GetLeft());
}

inline const signed int UI_Window::getRelativeClientRectLowerBound() const {
	return(clientRect.GetBottom());
}

inline const signed int UI_Window::getRelativeClientRectRightBound() const {
	return(clientRect.GetRight());
}

inline const signed int UI_Window::getAbsoluteClientRectUpperBound() const {
	return(getAbsoluteClientRectPosition().y);
}

inline const signed int UI_Window::getAbsoluteClientRectLeftBound() const {
	return(getAbsoluteClientRectPosition().x);
}

inline const signed int UI_Window::getAbsoluteClientRectLowerBound() const {
	return(getAbsoluteClientRectPosition().y + getClientRectHeight() -1 ); // TODO?
}

inline const signed int UI_Window::getAbsoluteClientRectRightBound() const {
	return(getAbsoluteClientRectPosition().x + getClientRectWidth() -1 );
}

inline void UI_Window::setChangedFlag(const bool flag) {
	changedFlag=flag;
}

inline const bool UI_Window::getChangedFlag() {
	return(changedFlag);
}

inline void UI_Window::changeAccepted() {
	changedFlag=false;
}

inline void UI_Window::setResetFlag(const bool flag) {
	resetFlag=flag;
}

inline const bool UI_Window::getResetFlag() {
	return(resetFlag);
}

inline void UI_Window::resetAccepted() {
	resetFlag=false;
}


#endif

