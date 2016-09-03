#ifndef _UI_INPUTOBJECT_HPP
#define _UI_INPUTOBJECT_HPP

#include "statictext.hpp"

class UI_InputObject : public UI_Object
{
	public:
		UI_InputObject(UI_Object* input_parent, 
				const Rect& edit_rect, 
				const Size& edit_size, 
				const signed int zcoord,
				const ePositionMode position_mode, 
				const eColor st_color, 
				const eFont st_font, 
				const std::string& name_proposal);
		~UI_InputObject();
		UI_InputObject(const UI_InputObject& object);
		UI_InputObject& operator=(const UI_InputObject& object);

		void draw() const;
		void process();
		
		void addChar(char a);
		void removeCharBackspace();
		void removeCharDelete();
		void enterWasPressed();
		void escapeWasPressed();
		const bool done() const;
		const bool canceled() const;

		void resetData();

		void moveLeft();
		void moveRight();

		void addKey(unsigned int key, unsigned int mod);
	private:
		int position;
		int ani;
		bool pressedEnter;
		bool pressedEscape;
};

inline void UI_InputObject::enterWasPressed() {
	pressedEnter = true;
}

inline void UI_InputObject::escapeWasPressed() {
	pressedEscape = true;
}

inline const bool UI_InputObject::done() const {
	return(pressedEnter);
}

inline const bool UI_InputObject::canceled() const {
	return(pressedEscape);
}


#endif

