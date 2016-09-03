#ifndef _UI_WINDOW_HPP
#define _UI_WINDOW_HPP

#include "scrollbar.hpp"

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


struct SaveBoxParameter
{
	eString saveboxText, descriptionText, okString, cancelString;
	std::string inputProposal;
};


class UI_Window : public UI_Object
{
	public:

		UI_Window(UI_Object* window_parent, const eString window_title_string, const Rect rect, const unsigned int max_height, const eIsScrolled window_is_scrolled=NOT_SCROLLED, const eIsAutoAdjust win_is_auto_adjust=NO_AUTO_SIZE_ADJUST, const Rect win_client_area=Rect(0,0,1280,1024), eIsTransparent transparent = NOT_TRANSPARENT);
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

		void setTitleParameter(const std::string& p);


		static unsigned int rectnumber;
		const bool fitItemToRelativeClientRect(const Rect& rectangle, const bool adjust = false);
		const bool fitItemToAbsoluteClientRect(const Rect& rectangle, const bool adjust = false);

		UI_ScrollBar* getScrollBar() const;
		const unsigned int getMaxHeight() const;

		void setMaxHeight(const unsigned int max_height);

		void wheelUp();
		void wheelDown();
		void moveScrollBarToTop();
		void moveScrollBarToBottom();

		void reloadOriginalSize();
		static signed int gotoHelpChapter;
		void addHelpButton(eHelpChapter help_chapter);

		static bool needSaveBox;
		static bool saveBoxCanceled;
		static bool saveBoxDone;
		static std::string saveBoxString;
		static SaveBoxParameter saveBoxParameter;
	protected:
	private:
	// do windows size changes smoothly		
		void adjustClientRect();
		unsigned int filledHeight;

		bool doAdjustments;

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
		void drawTitle(DC* dc) const;
		void drawToolTip(DC* dc, const Point p, const std::string* tip) const;

		eIsAutoAdjust isAutoAdjust;
// has this window a ScrollBar?
		eIsScrolled isScrollable;

		bool highlighted;

		static bool changedFlag; 
		
// this windows may have a scroll bar		
		UI_ScrollBar* scrollBar;
		UI_Button* helpButton;
		eHelpChapter helpChapter;
	
		UI_Window& operator=(const UI_Window& object);
		UI_Window(const UI_Window& object);
};

inline const unsigned int UI_Window::getMaxHeight() const {
	return(maxHeight);
}

inline UI_ScrollBar* UI_Window::getScrollBar() const {
	return(scrollBar);
}

inline const Rect& UI_Window::getRelativeClientRect() const {
	return(clientRect);
}

inline const Point UI_Window::getRelativeClientRectPosition() const {
	return(clientRect.getTopLeft());
}

inline const Point UI_Window::getAbsoluteClientRectPosition() const {
	return(clientRect.getTopLeft()+getAbsolutePosition());
}

inline const Size& UI_Window::getClientRectSize() const {
	return(clientRect.getSize());
}

inline const Rect UI_Window::getAbsoluteClientRect() const {
	return(Rect(getAbsoluteClientRectPosition(), getClientRectSize()));
}

inline const unsigned int UI_Window::getClientRectHeight() const {
	return(clientRect.getHeight());
}

inline const unsigned int UI_Window::getClientRectWidth() const {
	return(clientRect.getWidth());
}

inline const signed int UI_Window::getRelativeClientRectUpperBound() const {
	return(clientRect.getTop());
}

inline const signed int UI_Window::getRelativeClientRectLeftBound() const {
	return(clientRect.getLeft());
}

inline const signed int UI_Window::getRelativeClientRectLowerBound() const {
	return(clientRect.getBottom());
}

inline const signed int UI_Window::getRelativeClientRectRightBound() const {
	return(clientRect.getRight());
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
	return(clientTargetRect.getWidth());
}

inline const unsigned int UI_Window::getClientTargetHeight() const {
	return(clientTargetRect.getHeight());
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

inline void UI_Window::setMaxHeight(const unsigned int max_height) {
	maxHeight = max_height;
}




#endif

