#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include "wx/wx.h"

const int SCROLLED=1;
const int NOT_SCROLLED=0;
const int TABBED=1;
const int NOT_TABBED=0;
const int PERM_BUTTON=1;

struct Button
{
	int bitmapButton; //text or Bitmap
	int permButton;
//	wxString text;
	wxBitmap* bitmap;
	wxRect edge; //position
	int pressed;
	int used;
};

class GraphixScrollWindow
{
public:
        int getScrollY();
	int getUpperBound();
	int getLowerBound();
	int getLeftBound();
	int getRightBound();
	int getInnerUpperBound();
	int getRelativeInnerUpperBound();
	int getInnerLeftBound();
	int getInnerHeight();
	int getInnerWidth();
	int getHeight();
	int getWidth();

//todo: angeben in welchem Fenster sich das Fenster befindet!
	

	void drawArrow(wxDC* dc, int x,int y, int dir, int length=20);

	bool insideClientArea(int x, int y);
	bool fitToClientArea(wxRect& rectangle);
	void moveToClientArea(wxRect& rectangle);
        wxRect getScrollBalken();
	void adjustWindow(wxRect edge);
//        void moveScrollBalken(int dy);
	void moveScrollBalkenTo(int y);

	int lastEntry;

	void setFreeMove(int move);
	

        void setMaxScrollY(int y);
        void setTitleColour(wxColour titleColour);
        void setBackground(wxBrush backgroundColour);
        void setRahmenPen(wxColour rahmenPenColour);
        void setRahmen(wxRect rahmen);
        void addTitle(int x, const char* Title);
	void addDescription(int x, const char* Description);
	void setDescription(int nr, int x, const char* Description);
	void addTab(int x,const char* tab);
        void Draw(wxDC* dc);
	void DrawButtons(wxDC* dc);
	void updateWindow();
	void OnMouseLeftDown();
	void OnMouseLeftUp();
	void OnScrollMouse(int msy);
	void setMouse(int x,int y);
	wxBrush getBackground();
//        void adjustClientWindow(int width, int height);
	void transformClientWidth(int width); //handle with care O_O
	void transformClientHeight(int height); //handle with care O_O

	void textButton(wxString bla, wxDC* dc);
	void newTextPage();
	void writeText(wxString bla, wxDC* dc);





	int isPressed(int num);
	int addButton(wxRect edge, int permButton=0);
	int addBitmapButton(wxRect edge, wxBitmap& bitmap, int permButton=0);
	void clearButtons();
	wxRect getButtonRect(int num);
//	void setButtonText(int num, const wxString text);
//	void setButtonBitmap(int num, const wxBitmap& bitmap);
//	void removeButton(int num);

	int currentTab;
        void setScrollY(int y);
	GraphixScrollWindow();
        GraphixScrollWindow(int level, wxRect rahmen, wxRect maxSize, int x=0, int y=0, int scrolled=1, int tabbed=0, wxRect clientArea=wxRect(0,0,9999,9999));
	static wxFont font;
	static wxFont font2;
	static wxFont font3;
	static wxFont font4;
	static wxFont font5;
	static wxFont font6;
	int isShown();
	void Show(int show);
private:
	Button button[1000];	
	int ScrollBalkenMove,ScrollBalkenMoveY,scrolled;
	int WindowMove,WindowMoveX,WindowMoveY;
	int PfeilUpPressed,PfeilDownPressed;
	int currentRow;
	wxRect clientArea;
	wxRect originalClientArea;
	wxRect originalBorder;
	wxRect maxSize;
	int x,y;
	int startx,starty,startheight,startwidth,targetx,targety,targetwidth,targetheight;

	int shown;

        int startcx,startcy,startcheight,startcwidth,targetcx,targetcy,targetcwidth,targetcheight,maxcheight;
	int freeMove;

	int numButtons;
        int maxScrollY;
        int scrollY;
	wxRect titlePosition;
        wxString Title;
	wxString Description[10];
	int descriptionPosition[10];
	int descriptionNumber;

	int tabNumber;
	wxString tab[10];
	int tabButton[10];
	int tabPosition[10];

        wxColour TitleColour;
        wxBrush Background;
	wxColour disabledItem;
	wxColour enabledItemPen;
	wxColour disabledItemPen;
	wxColour mouseOverItem;
	wxColour clickedItem;
	wxColour disabledItemBorder;
        wxColour RahmenPen,RahmenPen2;
        wxRect Rahmen;
	wxRect OuterRahmen;
	wxRect Border;
        wxRect ScrollBalken;
	wxRect ScrollArea;
	wxRect PfeilUp;
	wxRect PfeilDown;
	int mouseX,mouseY;
};
#endif
