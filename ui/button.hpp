#ifndef _UI_BUTTON_HPP
#define _UI_BUTTON_HPP

#include "statictext.hpp"
#include "bitmap.hpp"

enum eButtonMode
{
	NO_BUTTON_MODE,
	STATIC_BUTTON_MODE, // stays in its position
	PRESS_BUTTON_MODE,  // returns to unpressed
	PUSH_BUTTON_MODE,   // calls several messages when being pressed
	TAB_BUTTON_MODE,
	TOP_TAB_BUTTON_MODE,
	CHECK_BUTTON_MODE, // no '3d effect', allows button to be set with 'check'
//	MENU_TAB_BUTTON_MODE,
	BOGRAPH_BUTTON_MODE, // just a rectangle, not rounded
	
	MAX_BUTTON_MODES
};

class UI_Radio;

class UI_Button : public UI_Object
{
	public:
// TODO Beschreibung der Konstruktoren
		UI_Button(UI_Object* button_parent, 
				const Rect button_rect, 
				const Size distance_bottom_right,
				const eButtonColorsType button_colors_type, 
				const eBitmap button_bitmap,
				const eButtonMode button_mode,
				
				const eString button_text, 
				
				const ePositionMode button_position_mode = DO_NOT_ADJUST, 
				const eFont button_font = SMALL_BOLD_FONT, 
				const eAutoSize button_auto_size = NO_AUTO_SIZE);
	
		UI_Button(UI_Object* button_parent, 
				const Rect button_rect, 
				const Size distance_bottom_right,
				const eButtonColorsType button_colors_type, 
				const eBitmap button_bitmap,
				const eButtonMode button_mode,
				
				const eString button_text, 
				const eString button_tooltip,
				
				const ePositionMode button_position_mode = DO_NOT_ADJUST, 
				const eFont button_font = SMALL_BOLD_FONT, 
				const eAutoSize button_auto_size = NO_AUTO_SIZE);

		UI_Button(UI_Object* button_parent, 
				const Rect button_rect, 
				const Size distance_bottom_right, 
				const eButtonColorsType button_colors_type,
				const eBitmap button_bitmap,
				const eButtonMode button_mode, 
				
				const std::string& button_text, 
				
				const ePositionMode button_position_mode = DO_NOT_ADJUST,
				const eFont button_font = SMALL_BOLD_FONT, 
				const eAutoSize button_auto_size = NO_AUTO_SIZE);
		~UI_Button();
		
		const bool isLeftClicked();			// has it been selected (ie clicked on)
		const bool isRightClicked();
		const bool isCurrentlyPressed() const; // is the button depressed?
		const bool isCurrentlyHighlighted() const; // is the mouse over this button?
		const bool isCurrentlyActivated() const;

		const unsigned int getGradient() const;

		void reloadOriginalSize();																																							
		void doHighlight(const bool high_light=true);	// force button to be highlighted

		void check(const bool is_checked=true);
		void forcePress(const bool click_message=true);	  // force button to get pressed
		void forceUnpress(const bool click_message=true);

		void updateText(const std::string& utext);
		void updateText(const eString utext);

		void process(); // process messages, continue animation etc.
		UI_Object* checkToolTip();
		UI_Object* checkHighlight();

		void mouseHasMoved();
		void frameReset();
		
		void mouseHasEnteredArea();
		void mouseHasLeftArea();
		void mouseLeftButtonPressed();
		void mouseLeftButtonReleased();
		void mouseRightButtonPressed();
		void mouseRightButtonReleased();

		const bool addKey(unsigned int key, unsigned int mod);
		
		void object_info();
		void draw() const;
		void setButtonColorsType(const eButtonColorsType button_colors_type);
		const eButtonColorsType getButtonColorsType() const;

		void setBitmap(const eBitmap button_bitmap);
		
		void resetGradient();
		UI_Radio* radio;

// relative 'button placement area'
		const unsigned int getTextWidth() const;
		const unsigned int getTextHeight() const;
		const Size getTextSize() const;
		void setAllowMoveByMouse(const bool allow_move_by_mouse = true);

		static void resetButton();
		static UI_Button* getCurrentButton();

		static const bool isCurrentButtonPressed();
		static const bool isCurrentButtonHasAlreadyLeft();
		static const Point& getMouseMovePoint();

		static const bool isMoveByMouse();
		static void setMoveByMouse(const bool move_by_mouse = true);


		static void setCurrentButtonPressed(const bool current_button_pressed = true);
		static void setCurrentButtonHasAlreadyLeft(const bool button_has_already_left = true);
		static void setMouseMovePoint(const Point& mouse_move_point);

		static void setCurrentButton(UI_Button* current_button);
		
		UI_StaticText* getText() const;
		static bool wasResetted;

		const bool isBitmapButton() const;
	private:
		static bool doClickedSound;
		static bool doClickSound;
		void setPressDepth(const unsigned int press);
		bool allowMoveByMouse;
		static Point mouseMovePoint;
		static bool moveByMouse;
	
		static bool currentButtonPressed;
		static bool currentButtonHasAlreadyLeft;
		static UI_Button* currentButton;

		static bool doMouseEnterSound;

		bool moved; // did this item move one pixel down (pressed)
		bool isOriginalPosition; // always false (not pressed) for non-static buttons
		bool wasPressed;

		unsigned int gradient;
		unsigned int oldGradient;

		unsigned int pressdepth;
		
		eFont font;
		eButtonColorsType buttonColorsType;	
		UI_Bitmap* bitmap;
		
		unsigned int statusFlags;
		unsigned int frameNumber;
// Button flags

		static const unsigned int BF_DOWN = 1;
		static const unsigned int BF_JUST_PRESSED = 2;
		static const unsigned int BF_JUST_RELEASED = 4;
		static const unsigned int BF_LEFT_CLICKED = 8;
		static const unsigned int BF_RIGHT_CLICKED = 16;
		static const unsigned int BF_DOUBLE_CLICKED = 32;
		static const unsigned int BF_HIGHLIGHTED = 64;  // button is not highlighted (ie mouse is not above the item)
		static const unsigned int BF_JUST_HIGHLIGHTED = 128;  // button has just been highlighted, true for 1 frame
		static const unsigned int BF_REPEATS = 256;
		static const unsigned int BF_STATIC = 512;
		static const unsigned int BF_STATIC_PRESSED = 1024;
		static const unsigned int BF_NOT_CLICKABLE = 2048;
		static const unsigned int BF_IS_TAB = 4096;
		static const unsigned int BF_WAS_PRESSED = 8192; // button will be DOWN again, wenn mouse gets over button, without pressing the button again
		static const unsigned int BF_IS_RECTANGLE = 16384;
		static const unsigned int BF_IS_CHECKBUTTON = 32768; // no '3d effect', allows to be 'checked'
		static const unsigned int BF_IS_TOP_TAB = 65536; 
		// TODO irgendwie mit Zeit machen, dass z.B. nach 100ms die Taste automatisch losgelassen wird
	protected:
		UI_StaticText* text;
	private:
		UI_Button& operator=(const UI_Button& object);		
		UI_Button(const UI_Button& object);
};

inline void UI_Button::setBitmap(const eBitmap button_bitmap) {
	bitmap->setBitmap(button_bitmap);
}

inline const bool UI_Button::isBitmapButton() const {
	return(bitmap != NULL);
}

inline UI_StaticText* UI_Button::getText() const {
	return(text);
}

inline UI_Button* UI_Button::getCurrentButton() {
	return(currentButton);
}

inline void UI_Button::setAllowMoveByMouse(const bool allow_move_by_mouse) {
	allowMoveByMouse = allow_move_by_mouse;
}

inline const unsigned int UI_Button::getGradient() const {
	return(gradient);
}

inline const eButtonColorsType UI_Button::getButtonColorsType() const {
	return(buttonColorsType);
}

inline void UI_Button::setButtonColorsType(const eButtonColorsType button_colors_type)
{
#ifdef _SCC_DEBUG
	if((button_colors_type<0)||(button_colors_type>=MAX_BUTTON_COLORS_TYPES)) {
		toErrorLog("WARNING (UI_Button::setButtonColorsType()): Value button_colors_type out of range.");return;
	}
#endif
	buttonColorsType = button_colors_type;
}

inline const bool UI_Button::isCurrentButtonPressed() {
	return(currentButtonPressed);
}

inline const bool UI_Button::isCurrentButtonHasAlreadyLeft() {
	return(currentButtonHasAlreadyLeft);
}

inline void UI_Button::setCurrentButtonPressed(const bool current_button_pressed) {
	currentButtonPressed = current_button_pressed;
}

inline void UI_Button::setCurrentButtonHasAlreadyLeft(const bool button_has_already_left) {
	currentButtonHasAlreadyLeft = button_has_already_left;
}

inline const Point& UI_Button::getMouseMovePoint() {
	return(mouseMovePoint);
}

inline void UI_Button::setMouseMovePoint(const Point& mouse_move_point) {
	mouseMovePoint = mouse_move_point;
}

inline const bool UI_Button::isMoveByMouse() {
	return(moveByMouse);
}

inline void UI_Button::setMoveByMouse(const bool move_by_mouse) {
	moveByMouse = move_by_mouse;
}

inline void UI_Button::setCurrentButton(UI_Button* current_button) {
	currentButton = current_button;
}
	
#endif // _UI_BUTTON_HPP

