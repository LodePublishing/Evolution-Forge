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


class EF_GADGET
{
	friend UI_WINDOW;
	friend UI_BUTTON;
	friend UI_KEYTRAP;
	friend UI_CHECKBOX;
	friend UI_RADIO;
	friend UI_SCROLLBAR;
	friend UI_LISTBOX;
	friend UI_INPUTBOX;
	// friend UI_SLIDER;
	friend UI_DOT_SLIDER;
	friend UI_DOT_SLIDER_NEW;
																                                                                                            
	protected:
		char *bm_filename;
		int kind;
		int hotkey;
	
		wxRect edge;

		int m_flags;
		void (*user_function)(void);
		int disabled_flag;
		int base_dragging;
		int base_drag_x, base_drag_y;
		int base_start_x, base_start_y;
		int hidden;
																                                                                                            
		// Data for supporting linking controls to hotspots
		int linked_to_hotspot;
		int hotspot_num;
																                                                                                            
		// Data for supporting bitmaps associated with different states of the control
		int uses_bmaps;
		int m_num_frames;
//	  ubyte	   *bmap_storage[MAX_BMAPS_PER_GADGET];
																                                                                                            
		void drag_with_children( int dx, int dy );
		void start_drag_with_children();
		void stop_drag_with_children();
																                                                                                            
		EF_GADGET *parent;
		EF_GADGET *children;
		EF_GADGET *prev;
		EF_GADGET *next;
														                                                                                            
		int is_mouse_on_children();
		void remove_from_family();
		void set_parent(UI_GADGET *_parent);
																                                                                                            
		EF_GADGET *get_next();
		EF_GADGET *get_prev();
		//UI_WINDOW *my_wnd; ?
																                                                                                            
		virtual void process(int focus = 0);
		virtual void destroy();
		int check_move();

		// pointer to the mouse & keyboard
		static Controls* controls; 

	public:
		int bmap_ids[MAX_BMAPS_PER_GADGET];
																                                                                                            
		UI_GADGET();	// constructor
		~UI_GADGET();   // destructor
																                                                                                            
		void base_create( UI_WINDOW *wnd, int kind, int x, int y, int w, int h );
		virtual void draw();
		void set_focus();
		void clear_focus();
		int has_focus();
		void set_hotkey(int keycode);
		void set_callback(void (*user_function)(void));
		void disable();
		void enable(int n = 1);
		void capture_mouse();
		int mouse_captured(UI_GADGET *gadget = NULL);
		int disabled();
		int enabled();
		virtual void hide(int n = 1);
		virtual void unhide();
		void update_dimensions(int x, int y, int w, int h);
		void get_dimensions(int *x, int *y, int *w, int *h);
		
		int isMouseInside();

		void get_mouse_pos(int *x, int *y);
																                                                                                            
		void link_hotspot(int num);
		int get_hotspot();
		int bmaps_used() { return uses_bmaps; }
																                                                                                            
		// loads nframes bitmaps, starting at index start_frame.
		// anything < start_frame will not be loaded.
		// this keeps the loading code from trying to load bitmaps which don't exist
		// and taking an unnecessary disk hit.
		int set_bmaps(char *ani_filename, int nframes = 3, int start_frame = 1);		// extracts MAX_BMAPS_PER_GADGET from .ani file
																                                                                                            
		void reset();  // zero out m_flags
		int is_hidden() { return hidden; }
};



/*class EFObject
{
	wxRect edge; // position
	wxString* tip;

	EFObject*
}*/

class EF_Button //: public EF_Gadget
{
	private:
//		friend UI_SCROLLBAR;
		// friend UI_SLIDER;
//		friend UI_DOT_SLIDER;
//		friend UI_DOT_SLIDER_NEW;

// general things - move it later to EF_Gadget!
		wxRect edge;
		int shown;
		void Show();
		void Hide();
// -------

		int textPosition; // 0: no text, 1: left, 2: right
		int positon;		   // indicates position of button (0 - up, 1 - down by mouse click 2 - down by keypress
// ------

// ------ button behaviour ------
		int nextRepeat;		// timestamp for next repeat if held down TODO
//		int m_press_linger; // timestamp for hold a pressed state animation ?
//		int hotkey_if_focus;	// hotkey for button that only works if it has focus ?
//		int force_draw_frame;   // frame number to draw next time (override default) ?
//		int first_callback;	 // true until first time callback function is called for button highlight ?
// ------

// 	Used to index into bmap_ids[] array to locate right bitmap for button
		enum { B_NORMAL = 0 };
		enum { B_HIGHLIGHT = 1 };
		enum { B_PRESSED = 2 };
		enum { B_DISABLED = 3 };
		enum { B_REPEAT_TIME = 100 };  // ms

// ------ button functions when mouse is over the item ------
		void (*m_just_highlighted_function)(void);  // call-back that gets called when button gets highlighted
		void (*m_disabled_function)(void);			  // callback that gets called when disabled button gets pressed (sound, popup, etc)
		wxString* toolTip; // tool tip text shown when mouse over button
// ------

//		void frame_reset(); // reset flags ?
//		virtual void process(int focus = 0); ?
//		virtual void destroy(); ?

		wxCursor* onMouseOverCursor;				 // bmap handle of special cursor used on mouseovers
		wxCursor* previousCursor;			   // store old cursor

		void maybeShowCustomCursor();	// show different cursor when onmouseOverCursor is set => process()
		void restorePreviousCursor();	// reset to original state => frame_reset() 

		int firstPressed; // first press-message after the button was not pressed
	public:
		virtual void draw();
		
//		void set_hotkey_if_focus(int key); TODO

		int pressed();			  // has it been selected (ie clicked on)
//		int double_clicked();   // button was double clicked on
		int justPressed();		// button has just been selected
		int justHighlighted(); // button has just had mouse go over it
		int currentlyPressed(); // is the button depressed?
		int currentlyHighlighted(); // is the mouse over this button?

		void forceButtonHighlight();	// force button to be highlighted
		void forceButtonPressed();		// force button to get pressed

//		void create(UI_WINDOW *wnd, char *_text, int _x, int _y, int _w, int _h, int do_repeat=0, int ignore_focus = 0);

		void setHighlightAction( void (*user_function)(void) );
		void setDisabledAction( void (*user_function)(void) );
//		void draw_forced(int frame_num); ?
		void resetStatus();
		void resetTimestamps();
		void skip_first_highlight_callback();
		void repeatable(int yes);
		void set_custom_cursor_bmap(int bmap_id) { custom_cursor_bmap = bmap_id; };
};

struct Button
{
	int bitmapButton; //text or Bitmap
	int permButton;
	wxString* tip; // tool tip text
	wxBitmap* bitmap;
	wxRect edge; //position
	int pressed;
	int used;
};

class ScrollBar
{
	private:
		Button moveUp, moveDown, scrollArea, scrollBar;
		wxRect owner;
		int scrollY;
		int maxHeight;
	public:	
		void update(Controls* controls);
		void draw(wxDC* dc);
		void setOwner(wxRect owner);
		void setVirtualHeight(int y); // = maxScrollY
		int getScrollY();
		ScrollBar();
		~ScrollBar();
}

class GraphixScrollWindow
{
public:
	static SETTINGS settings;
	void checkButtons();
	
	void wasChanged();
	int hasChanged();
	void changeAccepted();

	int getScrollY();
	int getInnerUpperBound();
	int getRelativeInnerUpperBound();
	int getInnerLeftBound();
	int getInnerHeight();
	int getInnerWidth();

	wxRect getTarget(); // obsolete

//todo: angeben in welchem Fenster sich das Fenster befindet!
	void drawArrow(wxDC* dc, wxPoint position, int dir, int length=20);

//	void moveToClientArea(wxRect& rectangle); obsolete
	wxRect getScrollBalken();
//		void moveScrollBalken(int dy);
	void moveScrollBalkenTo(int y);

	void setMaxScrollY(int y);
	
	void showTip(wxDC* dc, wxPoint p, wxString* tip);

	void DrawButtons(wxDC* dc);
	void updateWindow();
	void OnScrollMouse(int msy);
	wxBrush getBackground();
//		void adjustClientWindow(int width, int height);
	void transformClientWidth(int width); //handle with care O_O
	void transformClientHeight(int height); //handle with care O_O

	void writeText(wxString text, wxDC* dc);
	void newTextPage();
	void newRectTextPage();
	void writeHeader(wxString text, wxDC* dc, int mode);

	void writeLongText(wxString text, wxDC* dc, wxColour color=wxColour(150,150,200));
	void writeLongRectangleText(wxString text, wxRect rect, wxDC* dc, wxColour color=wxColour(150,150,200));

	int isActivated(int num);
	int getPressCondition(int num);
	
	void textButton(wxString text, wxDC* dc, const char* tip=0);
	int addButton(wxRect edge, const char* tip=0, int permButton=0);
	int addBitmapButton(wxRect edge, wxBitmap& bitmap, const char* tip=0, int permButton=0);
	
	void clearButtons();
	wxRect getButtonRect(int num);
//	void setButtonText(int num, const wxString text);
//	void setButtonBitmap(int num, const wxBitmap& bitmap);
//	void removeButton(int num);

    int getTargetHeight();  // for transforms
    int getTargetWidth();
				
	
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

	static wxBitmap bmpImp1;
	static wxBitmap bmpImp2;
	static wxBitmap bmpImp3;
	static wxBitmap bmpImp4;
	static wxBitmap bmpImp5;
	static wxBitmap bmpImp6;
private:
	int changed;
	Button button[1000];	 // TODO!!!
	int ScrollBalkenMoveY,scrolled;
	int WindowMove,WindowMoveX,WindowMoveY;
	int PfeilUpPressed,PfeilDownPressed;
	int ScrollBalkenPressed;

	wxRect originalInnerRect;
	wxRect originalRect;

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
