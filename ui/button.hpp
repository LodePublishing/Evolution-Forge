#ifndef _UI_BUTTON_HPP
#define _UI_BUTTON_HPP

#include "statictext.hpp"


// Button terminology:
//   Up = button is in up state (also called pressed)
//   Down = button is in down state (also called released)
//   Just pressed = button has just gone from up to down state
//   Just released = button has just gone from down to up state
//   Clicked = a trigger type effect caused by 'just pressed' event or repeat while 'down'
//   Double clicked = 2 'just pressed' events occuring within a short amount of time


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
	ARRANGE_TOP_LEFT,
	ARRANGE_TOP_RIGHT,
	ARRANGE_BOTTOM_LEFT,
	ARRANGE_BOTTOM_RIGHT,
	ARRANGE_LEFT,
	ARRANGE_RIGHT
};

enum eAutoSize
{
	NOTHING,
	NO_AUTO_SIZE,
	AUTO_SIZE_ONCE,
	AUTO_SIZE,
	AUTO_HEIGHT_FULL_WIDTH, // fehlerhaft! TODO
	FULL_WIDTH,
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

class UI_Radio;

class UI_Button:public UI_Object
{
	public:
		UI_Button& operator=(const UI_Button& object);		
		UI_Button(const UI_Button& object);

// TODO Beschreibung der Konstruktoren
		UI_Button(UI_Object* button_parent, const Rect button_rect, const Rect button_max_rect, 
		const eString button_text, 
		const eButton button_type, const eTextMode button_text_mode = HORIZONTALLY_CENTERED_TEXT_MODE, const eButtonMode button_mode = STATIC_BUTTON_MODE, const ePositionMode button_position_mode = DO_NOT_ADJUST, const eFont button_font = SMALL_NORMAL_BOLD_FONT, const eAutoSize button_auto_size = NO_AUTO_SIZE);

		UI_Button(UI_Object* button_parent, const Rect button_rect, const Rect button_max_rect, const string& button_text, const eButton button_type, const eTextMode button_text_mode = HORIZONTALLY_CENTERED_TEXT_MODE, const eButtonMode button_mode = STATIC_BUTTON_MODE, ePositionMode button_position_mode = DO_NOT_ADJUST, const eFont button_font = SMALL_NORMAL_BOLD_FONT, const eAutoSize button_auto_size = NO_AUTO_SIZE);
// Bitmap button
		UI_Button(UI_Object* button_parent, const Rect button_rect, const Rect button_max_rect, const eButton button_type, const eButtonMode button_mode = STATIC_BUTTON_MODE, const ePositionMode button_position_mode = DO_NOT_ADJUST);
		~UI_Button();
		
//      void set_hotkey_if_focus(int key); TODO
        const bool isLeftClicked();            // has it been selected (ie clicked on)
        const bool isRightClicked();
//      int double_clicked();   // button was double clicked on
        const bool isJustPressed() const;      // button has just been selected
		const bool isJustReleased() const;
        const bool isJustHighlighted() const; // button has just had mouse go over it
        const bool isCurrentlyPressed() const; // is the button depressed?
        const bool isCurrentlyHighlighted() const; // is the mouse over this button?
		const bool isCurrentlyActivated() const;

		const unsigned int getGradient() const;
                                                                                                                                                            
        void forceHighlighted();    // force button to be highlighted
        void forceDelighted();    // force button to be de-highlighted
        void forcePress();      // force button to get pressed
		void forceUnpress();

		void setPressDepth(const unsigned int press);

		void updateText(const string& utext);
		void updateText(const eString utext);

        //void setHighlightAction( void (*user_function)(void) );
        //void setDisabledAction( void (*user_function)(void) );
//      void draw_forced(int frame_num); ?
        void resetTimestamps();
  //      void skip_first_highlight_callback();
//        void set_custom_cursor_bmap(int bmap_id) { custom_cursor_bmap = bmap_id; };

		void process(); // process messages, continue animation etc.
		UI_Object* checkTooltip();
		UI_Object* checkHighlight();

		void mouseHasMoved();
		
		void mouseHasEnteredArea();
		void mouseHasLeftArea();
		void mouseLeftButtonPressed();
		void mouseLeftButtonReleased();
		void mouseRightButtonPressed();
		void mouseRightButtonReleased();

		
		void draw(DC* dc) const;
		void setButton(const eButton button);
		void setFrameNumber(const unsigned int frame_number);
	
		void frameReset();
		void resetGradient();
		UI_Radio* radio;

		bool forcedPress;
		void adjustButtonPlacementPosition(); // jump to relativeRect
		void adjustButtonPlacementSize();
		Rect buttonPlacementArea;

		const unsigned int getTextWidth() const;

	private:
		bool moved; // did this item move one pixel down (pressed)
		bool originalPosition; // always false (not pressed) for non-static buttons
		bool hasBitmap;


//		Rect buttonPlacementArea;
		unsigned int gradient;
		long unsigned int timeStamp;

//		long unsigned int forcedHighlightTimeout;
		
		void adjustButtonSize(const Size& size);

		unsigned int pressdepth;
		
		eFont font;
		eButton button;	
		eAutoSize autoSize;
		ePositionMode mode;
		
		unsigned int frameNumber;
		unsigned int statusFlags;

		
//	TODO    int onMouseOverCursor;                 // bmap handle of special cursor used on mouseovers
  // TODO      int previousCursor;              // store old cursor
//        void maybeShowCustomCursor();   // show different cursor when onmouseOverCursor is set => process()
  //      void restorePreviousCursor();   // reset to original state => frame_reset()

		void resetData();
protected:
		UI_StaticText* text;
	
        long unsigned int nextRepeat;     // timestamp for next repeat if held down

		bool moveByMouse;

};


#endif // _UI_BUTTON_HPP

