#ifndef _UI_OBJECT_HPP
#define _UI_OBJECT_HPP

#include "configuration.hpp"

enum eDrawType
{
	SOLID_OBJECT,
	TRANSPARENT_OBJECT,
	ANTI_ALIASED_OBJECT	
};

enum eIsScrolled
{
	NOT_SCROLLED,
	SCROLLED
};

enum ePositionMode
{
	DO_NOT_ADJUST,
	TOTAL_CENTERED,
	HORIZONTALLY_CENTERED,
	VERTICALLY_CENTERED,
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
	CENTER_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_CENTER,
	BOTTOM_LEFT,
	CENTER_LEFT,
	SPECIAL_BUTTON_LEFT // normal button but put text left
};

enum eAutoSize
{
	NOTHING,
	NO_AUTO_SIZE,
	AUTO_SIZE,
	AUTO_HEIGHT_FULL_WIDTH,
	AUTO_DOUBLE_HEIGHT_FULL_WIDTH,
	FULL_WIDTH,
	AUTO_HEIGHT_CONST_WIDTH,
	AUTO_DOUBLE_HEIGHT_CONST_WIDTH,
	CONST_SIZE //~~ constant width
};

enum eAdjustMode
{
	PARENT_WAS_CHANGED,
	CHILD_WAS_CHANGED
};

class UI_Object;
class UI_ToolTip;
class UI_Window;
class UI_Sound;

// singleton class
class UI
{
	public:
		UI* Instance();
		void clearAllFlags();

		UI_ToolTip* tooltip;
		UI_Object* toolTipParent;

		unsigned int redrawnObjects;
		const bool setResolution(const eResolution resolution, const bool first_call = false);
		const bool setBitDepth(const eBitDepth bitdepth);
		
		UI_Sound* sound;
		UI_Window* currentWindow;
		bool windowSelected;
		
		unsigned int mouseType;
		Point mouse;

		std::list<std::string> msgList;

		void resetWindow();	
		
		UI_Object* focus;

		const bool initSDL(std::string window_title);
		DC* dc;

		void updateToolTip(const eString tool_tip_string);
		void updateToolTip(const std::string& tool_tip_string);
		
		const eString getToolTipEString() const;
		const std::string& getToolTipString() const;
		const bool toolTipExists() const;


		void updateScreen();
		void processAll();

		void addMessage(const std::string& msg);
		void addMessage(const eString msg);

		std::list<UI_Object*> objectList;
		void addNewRectToList(const Rect rect, const unsigned int z_coord);
	protected:
		UI();
		~UI(); 
		std::list<std::string> remainingMessages;
	private:
		std::list<Rect> oldRectList;
		std::list<std::pair<signed int, Rect> > newRectList;
		
//		bei wechsel alle rekursiv (-> virtual) durchlaufen und Liste bilden, das aktuelle heraussuchen und aktivieren
//		Ansonsten bei klick Focus legen, Esc/anderer Klick entfernt den Focus (NULL bzw. anderer Fokus)

//		UI_Object& operator=(const UI_Object& object);
//		UI_Object(const UI_Object& object);
	
};

inline void UI_Object::updateToolTip(const eString tool_tip_string) {
	toolTipEString = tool_tip_string;
}

inline void UI_Object::updateToolTip(const std::string& tool_tip_string) {
	toolTipString = tool_tip_string;
}

inline const bool UI_Object::toolTipExists() const {
	return((toolTipString!="")||(toolTipEString!=NULL_STRING));
}

inline const eString UI_Object::getToolTipEString() const {
	return(toolTipEString);
}

inline const std::string& UI_Object::getToolTipString() const {
	return(toolTipString);
}

#endif

