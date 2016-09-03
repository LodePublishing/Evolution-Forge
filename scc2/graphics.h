#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include "wx/wx.h"

class GraphixScrollWindow
{
public:
        int getScrollY();
	int getUpperBound();
	int getLowerBound();
	int getLeftBound();
	int getRightBound();
	int getInnerUpperBound();
	int getInnerLeftBound();
	int getInnerHeight();
	int getInnerWidth();
	
	bool insideClientArea(int x, int y);
	bool fitToClientArea(wxRect& rectangle);
        wxRect getScrollBalken();
	void moveWindow(int x, int y);
//        void moveScrollBalken(int dy);
	void moveScrollBalkenTo(int y);

        void setMaxScrollY(int y);
        void setTitleColour(wxColour titleColour);
        void setBackground(wxColour backgroundColour);
        void setRahmenPen(wxColour rahmenPenColour);
        void setRahmen(wxRect rahmen);
        void addTitle(int x, const char* Title);
	void addDescription(int x, const char* Description);
	void setDescription(int nr, int x, const char* Description);
        void Draw(wxDC* dc);

	void OnMouseLeftDown();
	void OnMouseLeftUp();
	void OnScrollMouse(int msy);
	void setMouse(int x,int y);

	GraphixScrollWindow();
        GraphixScrollWindow(wxColour TitleColour, wxColour BackgroundColour, wxColour RahmenPenColour, wxRect rahmen, int description=0, int x=0, int y=0, int scrolled=1, wxRect clientArea=wxRect(0,0,9999,9999));
private:
	int ScrollBalkenMove,ScrollBalkenMoveY,scrolled;
	int PfeilUpPressed,PfeilDownPressed;
	wxRect clientArea;
	int x,y;
        void setScrollY(int y);
        int maxScrollY;
        int scrollY;
        wxString Title[5];
	wxString Description[10];
        int titlePosition[5];
	int descriptionPosition[10];
        int titleNumber;
	int descriptionNumber;
        wxColour TitleColour;
        wxBrush Background;
	wxColour disabledItem;
	wxColour enabledItemPen;
	wxColour disabledItemPen;
	wxColour mouseOverItem;
	wxColour clickedItem;
	wxColour disabledItemBorder;
        wxColour RahmenPen;
        wxRect Rahmen;
	wxRect OuterRahmen;
        wxRect ScrollBalken;
	wxRect ScrollArea;
	wxRect PfeilUp;
	wxRect PfeilDown;
	int mouseX,mouseY;
};

#endif
