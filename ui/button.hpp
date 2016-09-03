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


enum eButtonMode
{
	NO_BUTTON_MODE,
	STATIC_BUTTON_MODE, // stays in its position
	PRESS_BUTTON_MODE,  // returns to unpressed
	PUSH_BUTTON_MODE,   // calls several messages when being pressed
	TAB_BUTTON_MODE,
//	MENU_TAB_BUTTON_MODE,

	MAX_BUTTON_MODES
};

class UI_Radio;

class UI_Button:public UI_Object
{
	public:
		UI_Button& operator=(const UI_Button& object);		
		UI_Button(const UI_Button& object);

// TODO Beschreibung der Konstruktoren
		UI_Button(UI_Object* button_parent, 
				const Rect button_rect, 
				const Size distance_bottom_right,
				const eString button_text, 
				const eButtonColorsType button_colors_type, 
				const eButtonMode button_mode = STATIC_BUTTON_MODE,
				const ePositionMode button_position_mode = DO_NOT_ADJUST, 
				const eFont button_font = SMALL_BOLD_FONT, 
				const eAutoSize button_auto_size = NO_AUTO_SIZE);
		
		UI_Button(UI_Object* button_parent, 
				const Rect button_rect, 
				const Size distance_bottom_right, 
				const std::string& button_text, 
				const eButtonColorsType button_colors_type, 
				const eButtonMode button_mode = STATIC_BUTTON_MODE, 
				const ePositionMode button_position_mode = DO_NOT_ADJUST, 
				const eFont button_font = SMALL_BOLD_FONT, 
				const eAutoSize button_auto_size = NO_AUTO_SIZE);
// Bitmap button
		UI_Button(UI_Object* button_parent, 
				const Rect button_rect, 
				const Size distance_bottom_right, 
				const eButtonColorsType button_colors_type, 
				const eButtonMode button_mode = STATIC_BUTTON_MODE, 
				const ePositionMode button_position_mode = DO_NOT_ADJUST);
		~UI_Button();
		
		const bool isLeftClicked();			// has it been selected (ie clicked on)
		const bool isRightClicked();
		const bool isCurrentlyPressed() const; // is the button depressed?
		const bool isCurrentlyHighlighted() const; // is the mouse over this button?
		const bool isCurrentlyActivated() const;

		const unsigned int getGradient() const;

		void reloadOriginalSize();																																							
		void forceHighlighted();	// force button to be highlighted
		void forceDelighted();	// force button to be de-highlighted
		void forcePress();	  // force button to get pressed
		void forceUnpress();

		void setPressDepth(const unsigned int press);

		void updateText(const std::string& utext);
		void updateText(const eString utext);

		void process(); // process messages, continue animation etc.
		UI_Object* checkTooltip();
		UI_Object* checkHighlight();

		void mouseHasMoved();
		void frameReset();
		
		void mouseHasEnteredArea();
		void mouseHasLeftArea();
		void mouseLeftButtonPressed();
		void mouseLeftButtonReleased();
		void mouseRightButtonPressed();
		void mouseRightButtonReleased();

		
		void draw(DC* dc) const;
		void setButtonColorsType(const eButtonColorsType button_colors_type);
		void resetGradient();
		UI_Radio* radio;

		bool forcedPress;
// relative 'button placement area'
		const unsigned int getTextWidth() const;
		const unsigned int getTextHeight() const;
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

		
		
	private:
		bool allowMoveByMouse;
		static Point mouseMovePoint;
		static bool moveByMouse;
	
		static bool currentButtonPressed;
		static bool currentButtonHasAlreadyLeft;
		static UI_Button* currentButton;
	
		
		bool moved; // did this item move one pixel down (pressed)
		bool isOriginalPosition; // always false (not pressed) for non-static buttons
		bool hasBitmap;
		bool wasPressed;

		unsigned int gradient;

		unsigned int pressdepth;
		
		eFont font;
		eButtonColorsType buttonColorsType;	
		
		unsigned int statusFlags;
		unsigned int frameNumber;
// Button flags

		static const unsigned int BF_DOWN = 1;
		static const unsigned int BF_JUST_PRESSED = 2;
		static const unsigned int BF_JUST_RELEASED = 4;
		static const unsigned int BF_LEFT_CLICKED = 8;
		static const unsigned int BF_RIGHT_CLICKED = 16;
		static const unsigned int BF_DOUBLE_CLICKED = 32;
		static const unsigned int BF_HIGHLIGHTED = 64;  // button is not highlighted (ie mouse is not over)
		static const unsigned int BF_JUST_HIGHLIGHTED = 128;  // button has just been highlighted, true for 1 frame
		static const unsigned int BF_REPEATS = 256;
		static const unsigned int BF_STATIC = 512;
		static const unsigned int BF_STATIC_PRESSED = 1024;
		static const unsigned int BF_NOT_CLICKABLE = 2048;
		static const unsigned int BF_IS_TAB = 4096;
		static const unsigned int BF_WAS_PRESSED = 8192; // button will be DOWN again, wenn mouse gets over button, without pressing the button again
	protected:
		UI_StaticText* text;
};

inline UI_Button* UI_Button::getCurrentButton() {
	return(currentButton);
}

inline void UI_Button::setAllowMoveByMouse(const bool allow_move_by_mouse) {
	allowMoveByMouse = allow_move_by_mouse;
}

inline const unsigned int UI_Button::getGradient() const {
	return(gradient);
}

inline const unsigned int UI_Button::getTextWidth() const {
	return(text->getTextSize().GetWidth());
}

inline const unsigned int UI_Button::getTextHeight() const {
	return(text->getTextSize().GetHeight());
}

inline void UI_Button::setButtonColorsType(const eButtonColorsType button_colors_type)
{
#ifdef _SCC_DEBUG
	if((button_colors_type<0)&&(button_colors_type>=MAX_BUTTON_COLORS_TYPES)) {
		toLog("WARNING: (UI_Button::setButtonColorsType): Value button_colors_type out of range.");return;
	}
#endif
	buttonColorsType = button_colors_type;
}

inline void UI_Button::updateText(const std::string& utext) {
	text->updateText(utext);
}

inline void UI_Button::updateText(const eString utext) {
	text->updateText(utext);
}

// Is the mouse over this button?
inline void UI_Button::forceHighlighted() {
	statusFlags |= BF_HIGHLIGHTED;
}

inline void UI_Button::forceDelighted() {
	statusFlags &= ~BF_HIGHLIGHTED;
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

