#ifndef __UI_WINDOW_H
#define __UI_WINDOW_H

#include "UI_Object.h"
#include "wx/wx.h"

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
		void setCurrentTab(eTab tab);
		eTab getCurrentTab();
		
// returns position and size of the client area
		wxRect getRelativeClientRect();
		wxPoint getRelativeClientRectPosition();
		
		wxPoint getClientRectPosition();
		wxSize getClientRectSize();
		
		int getClientRectUpperBound();
   		int getRelativeClientRectUpperBound();
		int getRelativeClientRectLeftBound();
	    int getClientRectLeftBound();
    	int getClientRectHeight();
	    int getClientRectWidth();

		
		bool insideClientRect(wxPoint pos);


// immediately change windows size
//	    void setWindowRect(const wxRect rect); // obsolete
//    	void setMaxWindowRect(const wxRect maxRect);  // obsolete
		
// do windows size changes smoothly		
		void adjustClientRect();

//	    void setTitleColour(const wxColour titleColour);
//	    void setBackground(const wxBrush backgroundColour);
//	    void setRahmenPen(const wxColour rahmenPenColour);
	
//	    void setRahmen(wxRect rahmen);
//	    void setTitle(const int x, const char* Title);
//    	void addDescription(const int x, const char* Description);
//		void setDescription(const int nr, const int x, const char* Description);
//	    void addTab(const int x, const char* tab, const char* tip=0);

		void process();
	    void draw(wxDC* dc);

		UI_Window(UI_Object* parent, eString titleString, wxRect rect, wxRect maxSize, const bool isScrolled=SCROLLED, const bool isAutoAdjust=NO_AUTO_SIZE_ADJUST, const bool tabNumber=NOT_TABBED, wxRect clientArea=wxRect(0,0,9999,9999));
		~UI_Window();

		bool getChangedFlag();
		void setChangedFlag(bool flag=true);
		void changeAccepted();

		void setTitleParameter(wxString p);

		void updateBorders();

	protected:
//		bool isMouseInsideClientArea();

// 		returns true if items is within clientarea + adapts item's size + maybe adjusts the window later to let the item fit into the client area later
		bool fitItemToClientRect(wxRect& rectangle, const int adjust=0);

		int getScrollY();

	private:
// key to communicate with other windows		
		int key;
		
		eTab currentTab; // maybe move to 'theme'
		
		bool changedFlag; // TODO, raus hier und static oder irgendsowas!

// no set/get for title as this is unique and does not change
		eString titleString;
		wxString* titleParameter;
		
// this windows may have a scroll bar		
//		UI_ScrollBar* scrollBar;
		

		wxRect originalRect;

// actual painted borders
		wxRect border;
		wxRect outerBorder;

// client area
		wxRect clientRect;
		wxRect clientStartRect;
		wxRect clientTargetRect;
// rectangle for max size of client area
		wxRect maxClientRect;

		wxRect originalClientRect;

	    void drawTabs(wxDC* dc);
    	void drawTitle(wxDC* dc);
	    void drawToolTip(wxDC* dc, wxPoint p, wxString* tip);

// entry in array for original values of this window.
		int originalValues;


		bool isAutoAdjust;
// do call adjustWindow - size needs to be changed
		bool haveToDoAdjustments;
// has this window a ScrollBar?
		bool isScrollable;
// has this window tab buttons at the top?
		bool isTabbed;
};

#endif

