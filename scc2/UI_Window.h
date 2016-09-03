#ifndef __UI_WINDOW_H
#define __UI_WINDOW_H

#include "UI_Object.h"
#include "UI_Button.h"

const int MIN_HEIGHT=2;

// evtl enum draus machen, andererseits waer das zu grosser Aufwand...
const bool SCROLLED=true;
const bool NOT_SCROLLED=false;

const bool TABBED=true;
const bool NOT_TABBED=false;

const bool AUTO_SIZE_ADJUST=true;
const bool NO_AUTO_SIZE_ADJUST=false;

class UI_Window:public UI_Object
{
	public:
		
// returns position and size of the client area
		Rect getRelativeClientRect();
		Rect getAbsoluteClientRect();
		Point getRelativeClientRectPosition();
		Point getAbsoluteClientRectPosition();
	   	int getClientRectHeight();
	    int getClientRectWidth();
	
		Size getClientRectSize();
		
		int getRelativeClientRectUpperBound();
	    int getRelativeClientRectLeftBound();
   		int getRelativeClientRectLowerBound();
		int getRelativeClientRectRightBound();

		int getAbsoluteClientRectUpperBound();
	    int getAbsoluteClientRectLeftBound();
   		int getAbsoluteClientRectLowerBound();
		int getAbsoluteClientRectRightBound();

		
		bool insideClientRect(Point pos);


// do windows size changes smoothly		
		void adjustClientRect();

//	    void setRahmen(Rect rahmen);
	    void addTab(UI_Button* tab);
		bool tabWasChanged();

		void process();
	    void draw(DC* dc);
		// reconfigure rectangles depending on current theme settings
		void updateRectangles();

		UI_Window(UI_Object* parent, eString titleString, eWindow window, const int windowNumber, const bool isScrolled=SCROLLED, const bool isAutoAdjust=NO_AUTO_SIZE_ADJUST, const bool tabNumber=NOT_TABBED, Rect clientArea=Rect(0,0,9999,9999));
		~UI_Window();

		bool getChangedFlag();
		void setChangedFlag(bool flag=true);
		void changeAccepted();

		void setTitleParameter(string p);

		void updateBorders();

		const eTab getCurrentTab();
		void setCurrentTab(const eTab tab);

	protected:
//		bool isMouseInsideClientArea();

// 		returns true if items is within clientarea + adapts item's size + maybe adjusts the window later to let the item fit into the client area later
		bool fitItemToRelativeClientRect(Rect& rectangle, const int adjust=0);
		bool fitItemToAbsoluteClientRect(Rect& rectangle, const int adjust=0);

		int getScrollY();

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
		string* titleParameter;
		
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

	    void drawTabs(DC* dc);
    	void drawTitle(DC* dc);
	    void drawToolTip(DC* dc, Point p, string* tip);

// entry in array for original values of this window.
		int originalValues;

		int windowNumber;
		bool isAutoAdjust;
// do call adjustWindow - size needs to be changed
		bool haveToDoAdjustments;
// has this window a ScrollBar?
		bool isScrollable;
// has this window tab buttons at the top?
		bool isTabbed;
};

#endif

