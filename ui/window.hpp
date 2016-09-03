#ifndef _UI_WINDOW_HPP
#define _UI_WINDOW_HPP

extern const unsigned int MIN_HEIGHT; // TODO in bograph?

#include "object.hpp"
#include "radio.hpp"
//#include "scrollbar.hpp"

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

		const unsigned int getScrollY() const;

// returns position and size of the client area
		const Rect& getRelativeClientRect() const {
			return(clientRect);
		}

		const Rect getAbsoluteClientRect() const {
			return(Rect(getAbsoluteClientRectPosition(), getClientRectSize()));
		}

		const Point getRelativeClientRectPosition() const {
			return(clientRect.GetTopLeft());
		}

		const Point getAbsoluteClientRectPosition() const {
    		return(clientRect.GetTopLeft()+getAbsolutePosition());
		}
                                                                            
		const unsigned int getClientRectHeight() const {
		    return(clientRect.GetHeight());
		}
                                                                            
		const unsigned int getClientRectWidth() const {
		    return(clientRect.GetWidth());
		}
                                                                            
		const Size& getClientRectSize() const {
		    return(clientRect.GetSize());
		}
                                                                            
		const signed int getRelativeClientRectUpperBound() const {
		    return(clientRect.GetTop());
		}
                                                                            
		const signed int getRelativeClientRectLeftBound() const {
		    return(clientRect.GetLeft());
        }
                                                                            
		const signed int getRelativeClientRectLowerBound() const {
		    return(clientRect.GetBottom());
		}
                                                                            
		const signed int getRelativeClientRectRightBound() const {
		    return(clientRect.GetRight());
		}
                                                                            
		const signed int getAbsoluteClientRectUpperBound() const {
		    return(getAbsoluteClientRectPosition().y);
		}

		const signed int getAbsoluteClientRectLeftBound() const {
	    return(getAbsoluteClientRectPosition().x);
		}
                                                                            
		const signed int getAbsoluteClientRectLowerBound() const {
		    return(getAbsoluteClientRectPosition().y + getClientRectHeight() -1 ); // TODO?
		}

		const signed int getAbsoluteClientRectRightBound() const {
		    return(getAbsoluteClientRectPosition().x + getClientRectWidth() -1 );
		}

/*		const bool insideClientRect(const Point pos) const
		{
		    return(clientRect.Inside(pos - getAbsolutePosition())); //?
		//  return(clientArea.Inside(rectangle.x,rectangle.y)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y+rectangle.height)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y)||clientArea.Inside(rectangle.x,rectangle.y+rectangle.height));
		//  TODO: wenns auf beiden Seiten ueberlappt?
		}*/ // TODO
		
// do windows size changes smoothly		
		void adjustClientRect();

//	    void setRahmen(Rect rahmen);
	    void addTab(UI_Button* tab_button);
		const bool tabWasChanged() const;

		void process();
	    void draw(DC* dc) const;

		UI_Object* checkTooltip();
		UI_Object* checkHighlight();

// 		reconfigure rectangles depending on current theme settings
		void updateRectangles(const unsigned int maxPlayer);

		
		const bool getChangedFlag() const;
		void setChangedFlag(const bool flag=true);
		void changeAccepted();

		void setTitleParameter(const string& p);

		void updateBorders();

		const eTab getCurrentTab() const;

		void forcePressTab(const eTab press_tab);

		static unsigned int rectnumber;
		const bool fitItemToRelativeClientRect(Rect& rectangle, const unsigned int adjust=0);
		const bool fitItemToAbsoluteClientRect(Rect& rectangle, const unsigned int adjust=0);

	protected:
//		bool isMouseInsideClientArea();
	private:
		UI_Radio* tabRow;
		UI_Button* tab[MAX_TABS];
		
	
		eWindow window;
		
		eTab currentTab; // maybe move to 'theme'
		

// no set/get for title as this is unique and does not change
		eString titleString;
		string titleParameter;
		
// this windows may have a scroll bar		
//		UI_Scrollbar* scrollBar;
		
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

	    void drawTabs(DC* dc) const;
    	void drawTitle(DC* dc) const;
	    void drawToolTip(DC* dc, const Point p, const string* tip) const;

		unsigned int windowNumber;
		eIsAutoAdjust isAutoAdjust;
// has this window a ScrollBar?
		eIsScrolled isScrollable;
// has this window tab buttons at the top?
		eIsTabbed isTabbed;
	
		bool highlighted;
		bool changedFlag; // TODO, raus hier und static oder irgendsowas!
	
};

#endif

