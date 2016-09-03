#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include "input.h"
#include "wx/wx.h"
#include "util.h"

#ifdef __WIN32__		                                                                        
const int FONT_SIZE=6;
#else
const int FONT_SIZE=7;
#endif

#include "../scc2dll/settings.h"

const wxColour BOcolor[11]={wxColour(0,0,0),wxColour(25,25,25),wxColour(0,50,0),wxColour(50,0,0),wxColour(50,0,50),wxColour(0,0,50),wxColour(0,50,50),wxColour(50,50,0),wxColour(0,0,25),wxColour(25,0,0),wxColour(0,25,0)};

const wxString BOnames[11]={_T("rest"),_T("SCV"),_T("Gas SCV"),_T("Combat units..."),_T("Support units..."),_T("Supply/Refinery..."),_T("Producing buildings..."),_T("Research buildings..."),_T("Building addons..."),_T("Researchs..."),_T("Special...")};


const int MIN_HEIGHT=2;

const int SCROLLED=1;
const int NOT_SCROLLED=0;
const int TABBED=1;
const int NOT_TABBED=0;
const int PERM_BUTTON=1;

const int AUTO_ADJUST=1;
const int NO_AUTO_ADJUST=0;

const int COLOR1R=100;
const int COLOR1G=100;
const int COLOR1B=100;

const int COLOR2R=150;
const int COLOR2G=150;
const int COLOR2B=150;

const int PEN1R=20;
const int PEN1G=20;
const int PEN1B=20;

const int TEXT1R=0;
const int TEXT1G=0;
const int TEXT1B=0;

const int INFOWINDOWPENR=40;
const int INFOWINDOWPENG=150;
const int INFOWINDOWPENB=20;

const int INFOWINDOWR=10;
const int INFOWINDOWG=50;
const int INFOWINDOWB=0;

const int INFOWINDOWTEXTR=125;
const int INFOWINDOWTEXTG=125;
const int INFOWINDOWTEXTB=250;

const int FIRST_COLOUMN=300;
const int SECOND_COLOUMN=500;
const int THIRD_COLOUMN=200;
//const int FIRST_ROW=0;
const int SECOND_ROW=125;



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
	static SETTINGS settings;
	void checkButtons();
	
	void wasChanged();
	int hasChanged();
	void changeAccepted();

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
	int getTargetHeight();
	int getTargetWidth();

	wxRect getTarget();

	void setWindow(wxRect rect);
	void setMaxWindow(wxRect max);

	wxPoint getPosition();
	wxSize getSize();

	wxPoint getInnerPosition();
	wxSize getInnerSize();

//todo: angeben in welchem Fenster sich das Fenster befindet!
	static Controls controls;	

	void drawArrow(wxDC* dc, wxPoint position, int dir, int length=20);

	bool mouseInside();

	bool insideClientArea(wxPoint position);
	bool fitToClientArea(wxRect& rectangle, int adjust=0);
	void moveToClientArea(wxRect& rectangle);
		wxRect getScrollBalken();
	void adjustWindow(wxRect edge);
//		void moveScrollBalken(int dy);
	void moveScrollBalkenTo(int y);

	int lastEntry;
	int doAdjustments;

	void setFreeMove(int move);
	

		void setMaxScrollY(int y);
		void setTitleColour(wxColour titleColour);
		void setBackground(wxBrush backgroundColour);
		void setRahmenPen(wxColour rahmenPenColour);
		void setRahmen(wxRect rahmen);
		void setTitle(int x, const char* Title);
	void addDescription(int x, const char* Description);
	void setDescription(int nr, int x, const char* Description);
	void addTab(int x,const char* tab);
		void Draw(wxDC* dc);
	void DrawTabs(wxDC* dc);
	void DrawTitle(wxDC* dc);


	void DrawButtons(wxDC* dc);
	void updateWindow();
	void OnScrollMouse(int msy);
	wxBrush getBackground();
//		void adjustClientWindow(int width, int height);
	void transformClientWidth(int width); //handle with care O_O
	void transformClientHeight(int height); //handle with care O_O

	void textButton(wxString text, wxDC* dc);
	void newTextPage();
	void writeText(wxString text, wxDC* dc);
	void writeHeader(wxString text, wxDC* dc, int mode);
	void writeLongText(wxString text, wxDC* dc, wxColour color=wxColour(150,150,200));


	int isActivated(int num);
	int getPressCondition(int num);
	int addButton(wxRect edge, int permButton=0);
	int addBitmapButton(wxRect edge, wxBitmap& bitmap, int permButton=0);
	void clearButtons();
	wxRect getButtonRect(int num);
//	void setButtonText(int num, const wxString text);
//	void setButtonBitmap(int num, const wxBitmap& bitmap);
//	void removeButton(int num);

	int currentTab;
	void setScrollY(int y);
	GraphixScrollWindow(int level, wxRect rahmen, wxRect maxSize, int scrolled=1, int autoAdjust=0, int tabbed=0, wxRect clientArea=wxRect(0,0,9999,9999));
	static wxFont font;
	static wxFont font2;
	static wxFont font3;
	static wxFont font4;
	static wxFont font5;
	static wxFont font6;
	static wxFont font7;
	int isShown();
	void Show(int show);

	static wxBitmap bmpCancel;
	static wxBitmap bmpAdd;
	static wxBitmap bmpSub;
	static wxBitmap bmpArrowLeft;
	static wxBitmap bmpArrowUp;
	static wxBitmap bmpArrowRight;
	static wxBitmap bmpArrowDown;
	static wxBitmap bmpClemens;
	static wxBitmap bmpClawsoftware;
	static wxBitmap bmpBack;


private:
	int changed;
	Button button[1000];	
	int ScrollBalkenMove,ScrollBalkenMoveY,scrolled;
	int WindowMove,WindowMoveX,WindowMoveY;
	int PfeilUpPressed,PfeilDownPressed;

	wxRect textCursor;

	wxRect originalInnerRect;
	wxRect originalRect;

	wxRect maxSize;
	wxRect maxInnerSize;

	wxRect rect;
	wxRect start;
	wxRect target;

	wxRect innerRect;
	wxRect innerStart;
	wxRect innerTarget;

	int shown;
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
	wxColour disabledItemColour;
	wxColour clickedItemColour;
		wxColour RahmenColour;
	wxColour BrightRahmenColour;
		wxPen disabledItemPen;
		wxPen RahmenPen;
	wxPen BrightRahmenPen;
	wxPen clickedItemPen;

	wxBrush RahmenBrush;
		wxBrush clickedItemBrush;
		wxBrush Background;

	
	int autoAdjust;

	wxRect Rahmen;
	wxRect OuterRahmen;
	wxRect ScrollBalken;
	wxRect ScrollArea;
	wxRect PfeilUp;
	wxRect PfeilDown;

};
#endif
