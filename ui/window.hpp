#ifndef _UI_WINDOW_HPP
#define _UI_WINDOW_HPP

#include "object.hpp"
#include "button.hpp"

const int MIN_HEIGHT=2;

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
// returns position and size of the client area
		const Rect getRelativeClientRect() const;
		const Rect getAbsoluteClientRect() const;
		const Point getRelativeClientRectPosition() const;
		const Point getAbsoluteClientRectPosition() const;
	   	const int getClientRectHeight() const;
	    const int getClientRectWidth() const;
	
		const Size getClientRectSize() const;
		
		const int getRelativeClientRectUpperBound() const;
	    const int getRelativeClientRectLeftBound() const;
   		const int getRelativeClientRectLowerBound() const;
		const int getRelativeClientRectRightBound() const;

		const int getAbsoluteClientRectUpperBound() const;
	    const int getAbsoluteClientRectLeftBound() const;
   		const int getAbsoluteClientRectLowerBound() const;
		const int getAbsoluteClientRectRightBound() const;

		
		const bool insideClientRect(const Point pos) const;


// do windows size changes smoothly		
		void adjustClientRect();

//	    void setRahmen(Rect rahmen);
	    void addTab(UI_Button* tab);
		const bool tabWasChanged() const;

		void process();
	    void draw(DC* dc) const;
		// reconfigure rectangles depending on current theme settings
		void updateRectangles(const int maxPlayer);

		UI_Window(UI_Object* parent, const eString titleString, const eWindow window, const int windowNumber, const eIsScrolled isScrolled=SCROLLED, const eIsAutoAdjust isAutoAdjust=NO_AUTO_SIZE_ADJUST, const eIsTabbed isTabbed=NOT_TABBED, const Rect clientArea=Rect(0,0,9999,9999));
		~UI_Window();

		const bool getChangedFlag() const;
		void setChangedFlag(const bool flag=true);
		void changeAccepted();

		void setTitleParameter(const string p);

		void updateBorders();

		const eTab getCurrentTab() const;

		void forcePressTab(const eTab tab);

	protected:
//		bool isMouseInsideClientArea();

		const bool insideRelativeClientRect(const Rect& rect) const;
		const bool insideAbsoluteClientRect(const Rect& rect) const;
// 		returns true if items is within clientarea + adapts item's size + maybe adjusts the window later to let the item fit into the client area later
		const bool fitItemToRelativeClientRect(Rect& rectangle, const int adjust=0);
		const bool fitItemToAbsoluteClientRect(Rect& rectangle, const int adjust=0);

		const int getScrollY() const;

	private:
		UI_Radio* tabRow;
		UI_Button* tab[MAX_TABS];
	
// key to communicate with other windows		
		int key;

		eWindow window;
		
		eTab currentTab; // maybe move to 'theme'
		
		bool changedFlag; // TODO, raus hier und static oder irgendsowas!

// no set/get for title as this is unique and does not change
		eString titleString;
		string titleParameter;
		
// this windows may have a scroll bar		
//		UI_ScrollBar* scrollBar;
		

		Rect originalRect;

// actual painted borders
		Rect border;
		Rect outerBorder;

// client area
		Rect clientRect;
		Rect clientStartRect;
		Rect clientTargetRect;
// rectangle for max size of client area
		Rect maxClientRect;

		Rect originalClientRect;

	    void drawTabs(DC* dc) const;
    	void drawTitle(DC* dc) const;
	    void drawToolTip(DC* dc, const Point p, const string* tip) const;

// entry in array for original values of this window.
		int originalValues;

		int windowNumber;
		eIsAutoAdjust isAutoAdjust;
// do call adjustWindow - size needs to be changed
		bool haveToDoAdjustments;
// has this window a ScrollBar?
		eIsScrolled isScrollable;
// has this window tab buttons at the top?
		eIsTabbed isTabbed;
};

#endif

