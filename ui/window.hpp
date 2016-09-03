#ifndef _UI_WINDOW_HPP
#define _UI_WINDOW_HPP

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

enum eIsTransparent
{
	TRANSPARENT,
	NOT_TRANSPARENT
};


enum eTabs
{
	HELP_TAB=27,
	SETTINGS_TAB,
	MAP_TAB,
	MAX_TABS
};

class UI_Window : public UI_Object
{
	public:
		UI_Window& operator=(const UI_Window& object);
		UI_Window(const UI_Window& object);

		UI_Window(UI_Object* window_parent, const eString window_title_string, const Rect rect, const unsigned int max_height, const eIsScrolled window_is_scrolled=NOT_SCROLLED, const eIsAutoAdjust win_is_auto_adjust=NO_AUTO_SIZE_ADJUST, const eIsTabbed win_is_tabbed=NOT_TABBED, const Rect win_client_area=Rect(0,0,1280,1024), eIsTransparent transparent = NOT_TRANSPARENT);
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
		const unsigned int getClientTargetWidth() const;
		
		void addTab(UI_Button* tab_button, const unsigned int button_id);
		void removeTab(const unsigned int button_id);
		const bool tabWasChanged() const;

		void process();

		void drawWindow(DC* dc) const;
		
		void draw(DC* dc) const;

		UI_Object* checkToolTip();
		UI_Object* checkHighlight();

// 		reconfigure rectangles depending on current theme settings
//		void updateRectangles(const Rect rect, const unsigned int max_height);
		
		static const bool getChangedFlag();
		static void setChangedFlag(const bool flag=true);
		static void changeAccepted();

		static const bool getResetFlag();
		static void setResetFlag(const bool flag=true);
		static void resetAccepted();

		void setTitleParameter(const std::string& p);

		const unsigned int getCurrentTab() const;
		void forcePressTab(const unsigned int press_tab);

		static unsigned int rectnumber;
		const bool fitItemToRelativeClientRect(const Rect& rectangle, const unsigned int adjust = 0);
		const bool fitItemToAbsoluteClientRect(const Rect& rectangle, const unsigned int adjust = 0);

		UI_Object* getScrollbar() const; 
		const unsigned int getMaxHeight() const;

		void setMaxHeight(const unsigned int max_height);

		void wheelUp();
		void wheelDown();
		void moveScrollbarToTop();
		void moveScrollbarToBottom();

		void reloadOriginalSize();
		static signed int gotoHelpChapter;
		void addHelpButton(eHelpChapter help_chapter);
	protected:
		UI_Radio* tabRow;
	private:
	// do windows size changes smoothly		
		void adjustClientRect();
		unsigned int filledHeight;

		bool doAdjustments;
		UI_Button* tab[MAX_TABS];

		eIsTransparent isTransparent;

// no set/get for title as this is unique and does not change
		eString titleString;
		std::string titleParameter;
		
// client area
		Rect clientRect;
		Rect clientStartRect;
		Rect clientTargetRect;
// rectangle for max size of client area

		Rect originalClientRect;

		unsigned int maxHeight;

		void calculateClientRect();
		void drawTabs(DC* dc) const;
		void drawTitle(DC* dc) const;
		void drawToolTip(DC* dc, const Point p, const std::string* tip) const;

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
		UI_Button* helpButton;
		eHelpChapter helpChapter;
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

inline const Point UI_Window::getRelativeClientRectPosition() const {
	return(clientRect.GetTopLeft());
}

inline const Point UI_Window::getAbsoluteClientRectPosition() const {
	return(clientRect.GetTopLeft()+getAbsolutePosition());
}

inline const Size& UI_Window::getClientRectSize() const {
	return(clientRect.GetSize());
}

inline const Rect UI_Window::getAbsoluteClientRect() const {
	return(Rect(getAbsoluteClientRectPosition(), getClientRectSize()));
}

inline const unsigned int UI_Window::getClientRectHeight() const {
	return(clientRect.GetHeight());
}

inline const unsigned int UI_Window::getClientRectWidth() const {
	return(clientRect.GetWidth());
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

inline const unsigned int UI_Window::getClientTargetWidth() const {
	return(clientTargetRect.GetWidth());
}

inline const unsigned int UI_Window::getClientTargetHeight() const {
	return(clientTargetRect.GetHeight());
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

