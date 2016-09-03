#ifndef _UI_BUTTON_HPP
#define _UI_BUTTON_HPP

#include "statictext.hpp"

using std::string;

// Button terminology:
//   Up = button is in up state (also called pressed)
//   Down = button is in down state (also called released)
//   Just pressed = button has just gone from up to down state
//   Just released = button has just gone from down to up state
//   Clicked = a trigger type effect caused by 'just pressed' event or repeat while 'down'
//   Double clicked = 2 'just pressed' events occuring within a short amount of time

// Button flags
//const int BF_UP                          = (1<<0) ??
const int BF_DOWN  = (1<<0);
const int BF_JUST_PRESSED            = (1<<1);
const int BF_JUST_RELEASED           = (1<<2);
const int BF_CLICKED                 = (1<<3);
const int BF_DOUBLE_CLICKED          = (1<<4);
const int BF_HIGHLIGHTED             = (1<<5);  // button is not highlighted (ie mouse is not over)
const int BF_JUST_HIGHLIGHTED    = (1<<6);  // button has just been highlighted, true for 1 frame
const int BF_REPEATS = (1<<7);
const int BF_STATIC	= (1<<8);
const int BF_STATIC_PRESSED = (1<<9);
const int BF_NOT_CLICKABLE = (1<<10);
const int BF_IS_TAB = (1<<11);
//const int BF_IGNORE_FOCUS            = (1<<8)  // button should not use focus to accept space/enter keypresses
//const int BF_HOTKEY_JUST_PRESSED = (1<<9)  // button hotkey was just pressed
//const int BF_REPEATS                 = (1<<10) // if held down, generates repeating presses
//const int BF_SKIP_FIRST_HIGHLIGHT_CALLBACK (1<<11)    // skip first callback for mouse over event


/*class UI_LongText:public UI_StaticText
{
	public:
		draw(DC* dc);
		nexTextPage();
	private:
		Rect textCursor;
};


class UI_InputBox:public UI_StaticText
{

	private:
		int inputType; // text, numbers
		int hasSpinButtons; // only if numeric
		UI_BitmapButton* up;
		UI_BitmapButton* down;
};


class UI_TextButton:public UI_Button
{
	public:
	protected;
		UI_StaticText text;
	private:
};


class UI_3dTextButton:public UI_TextButton
{
	public:
		void draw(DC* dc);
	private:
		int color;
		
};

class UI_TabButton:public UI_TextButton
{
	public:
	private:
};


// general things - move it later to EF_Gadget!
// -------
                                                                                                                                                            
// ------
                                                                                                                                                            
// ------ button behaviour ------
//      int m_press_linger; // timestamp for hold a pressed state animation ?
//      int hotkey_if_focus;    // hotkey for button that only works if it has focus ?
//      int force_draw_frame;   // frame number to draw next time (override default) ?
//      int first_callback;  // true until first time callback function is called for button highlight ?
// ------
                                                                                                                                                            
//  Used to index into bmap_ids[] array to locate right bitmap for button
        enum { B_NORMAL = 0 };
        enum { B_HIGHLIGHT = 1 };
        enum { B_PRESSED = 2 };
        enum { B_DISABLED = 3 };
        enum { B_REPEAT_TIME = 100 };  // ms
                                                                                                                                                            
//      void frame_reset(); // reset flags ?
//      virtual void process(int focus = 0); ?
//      virtual void destroy(); ?
                                                                                                                                                            
                                                                                                                                                            
    public:
};*/

enum ePositionMode
{
	DO_NOT_ADJUST,
	TOTAL_CENTERED,
	HORIZONTAL_CENTERED,
	VERTICALLY_CENTERED,
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
	CENTER_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_CENTER,
	BOTTOM_LEFT,
	CENTER_LEFT,
	ARRANGE_TOP,
	ARRANGE_BOTTOM,
	ARRANGE_LEFT,
	ARRANGE_RIGHT,
};

enum eAutoSize
{
	NO_AUTO_SIZE,
	AUTO_SIZE,
	AUTO_HEIGHT_FULL_WIDTH, // 
	CONSTANT_SIZE //~~ constant width
};

enum eButtonMode
{
	NO_BUTTON_MODE,
	STATIC_BUTTON_MODE, // stays in its position
	PRESS_BUTTON_MODE,  // returns to unpressed
	PUSH_BUTTON_MODE,   // calls several messages when being pressed
	TAB_BUTTON_MODE,

	MAX_BUTTON_MODES
};



class UI_Button:public UI_Object
{
	public:
		UI_Button(UI_Object* parent, Rect rect, Rect maxRect, eString normalText, eString pressedText, eButton button, eTextMode textMode=HORIZONTALLY_CENTERED_TEXT_MODE, eButtonMode buttonMode = STATIC_BUTTON_MODE, ePositionMode mode = DO_NOT_ADJUST, eFont font=SMALL_NORMAL_BOLD_FONT, eAutoSize autoSize=NO_AUTO_SIZE);
		UI_Button(UI_Object* parent, Rect rect, Rect maxRect, string normalText, string pressedText, eButton button, eTextMode textMode=HORIZONTALLY_CENTERED_TEXT_MODE, eButtonMode buttonMode = STATIC_BUTTON_MODE, ePositionMode mode = DO_NOT_ADJUST, eFont font=SMALL_NORMAL_BOLD_FONT, eAutoSize autoSize=NO_AUTO_SIZE);
		UI_Button(UI_Object* parent, Rect rect, Rect maxRect, eButton button, eButtonMode buttonMode = STATIC_BUTTON_MODE, ePositionMode mode = DO_NOT_ADJUST);
		~UI_Button();
		
//      void set_hotkey_if_focus(int key); TODO
        const bool isPressed() const;            // has it been selected (ie clicked on)
//      int double_clicked();   // button was double clicked on
        const bool isJustPressed() const;      // button has just been selected
		const bool isJustReleased() const;
        const bool isJustHighlighted() const; // button has just had mouse go over it
        const bool isCurrentlyPressed() const; // is the button depressed?
        const bool isCurrentlyHighlighted() const; // is the mouse over this button?
                                                                                                                                                            
        void forceHighlighted();    // force button to be highlighted
        void forcePressed();      // force button to get pressed
		void forceUnpress();

		void updateNormalText(const string utext);
		void updatePressedText(const string utext);

        //void setHighlightAction( void (*user_function)(void) );
        //void setDisabledAction( void (*user_function)(void) );
//      void draw_forced(int frame_num); ?
        void resetTimestamps();
  //      void skip_first_highlight_callback();
//        void set_custom_cursor_bmap(int bmap_id) { custom_cursor_bmap = bmap_id; };

		void process(); // process messages, continue animation etc.
		void draw(DC* dc) const;
		bool forcedPress;
		void setButton(const eButton button);
		void setFrameNumber(const int num);
	protected:
		UI_StaticText* normalText;
		UI_StaticText* pressedText;
	
        long int nextRepeat;     // timestamp for next repeat if held down
	private:

//		int gradient;
		long int timeStamp;
		
		void adjustButtonSize(const Size size);
		Rect currentSize;
	
		eFont font;
		eButton button;	
		eAutoSize autoSize;
		ePositionMode mode;

		
		void frameReset();
		int frameNumber;
		int statusFlags;
		Rect buttonRect;
        bool firstPressed; // first press-message after the button was not pressed
		
//	TODO    int onMouseOverCursor;                 // bmap handle of special cursor used on mouseovers
  // TODO      int previousCursor;              // store old cursor
//        void maybeShowCustomCursor();   // show different cursor when onmouseOverCursor is set => process()
  //      void restorePreviousCursor();   // reset to original state => frame_reset()
	//	void maybeShowToolTip();
	//	void hideToolTip();

		bool hasToolTip;
		bool hasBitmap;
//		UI_ToolTip* toolTip;

};

class UI_Radio:public UI_Object
{
	public:
		UI_Radio(UI_Object* parent);
		~UI_Radio();
		void addButton(UI_Button* button);
		const int getMarked() const;
		void forceTabPressed(const int tab);
		void process();
		void draw(DC* dc) const;
		const bool hasChanged() const;
	private:
		bool changed;
};

#endif // _UI_BUTTON_HPP

