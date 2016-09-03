#ifndef _UI_OBJECT_HPP
#define _UI_OBJECT_HPP

#include "theme.hpp"
#include <list>
#include <string>

#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
// Windows kotz

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
	FULL_WIDTH,
	AUTO_HEIGHT_CONST_WIDTH,
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

class UI_Object
{
	public:
		UI_Object(UI_Object* parent_object, const Rect relative_rect = Rect(0, 0, 0, 0), const Size distance_bottom_right = Size(0, 0), const ePositionMode position_mode = DO_NOT_ADJUST, const eAutoSize auto_size = NOTHING);
		UI_Object(UI_Object* parent_object, UI_Object* position_parent_object, const Rect relative_rect = Rect(0, 0, 0, 0), const Size distance_bottom_right = Size(0, 0), const ePositionMode position_mode = DO_NOT_ADJUST, const eAutoSize auto_size = NOTHING);

		virtual ~UI_Object();
	
		void Show(const bool show=true);
		void Hide(const bool hide=true);

		const bool isShown() const;
		const bool isTopItem() const;
		
		const Point& getRelativePosition() const;
	
		const Size& getSize() const;
		const signed int getRelativeUpperBound() const;
		const signed int getRelativeLowerBound() const;
		const signed int getRelativeLeftBound() const;
		const signed int getRelativeRightBound() const;
		const unsigned int getHeight() const;
		const unsigned int getWidth() const;
		const Rect& getRelativeRect() const;
		const Point getParentAbsolutePosition() const;
		const Point getAbsolutePosition() const;
		const Rect getAbsoluteRect() const;
		const signed int getAbsoluteUpperBound() const;
		const signed int getAbsoluteLowerBound() const;
		const signed int getAbsoluteLeftBound() const;
		const signed int getAbsoluteRightBound() const;

		const unsigned int getTargetWidth() const;
		const unsigned int getTargetHeight() const;
		const Point getTargetPosition() const;
		const Rect& getTargetRect() const;

		void adjustRelativeRect(const Rect& rect);
		
		const bool isMouseInside() const;

		const bool checkForNeedRedraw() const;
		void setNeedRedrawMoved(const bool need_redraw=true);
		void setNeedRedrawNotMoved(const bool need_redraw=true);
		void setNeedRedrawNotMovedFirstChild(const bool need_redraw=true);

// returns the object the mouse cursor is currently over
		virtual UI_Object* checkToolTip();
		virtual UI_Object* checkHighlight();
		virtual void reloadStrings();
		virtual void reloadOriginalSize();

		virtual void draw(DC* dc) const;
		virtual void process();
		virtual void resetData();
		
		void clearRedrawFlag();

		void updateToolTip(const eString tool_tip_string);
		void updateToolTip(const std::string& tool_tip_string);
		
		const eString getToolTipEString() const;
		const std::string& getToolTipString() const;
		const bool hasToolTip() const;
		
		static UI_ToolTip* tooltip;
		static UI_Object* toolTipParent;

		static unsigned int redrawnObjects;
		static void setResolution(const Size resolution);
		static UI_Theme theme;
		static UI_Window* currentWindow;
		static bool windowSelected;
		
		static unsigned int mouseType;
		static Point mouse;

		static std::list<std::string> msgList;

		static unsigned int max_x;
		static unsigned int max_y;

		static void resetWindow();	
		
		const Point& getOriginalPosition() const;
		const Rect& getOriginalRect() const;
		const Size& getDistanceBottomRight() const;
	
/*		static void addToProcessArray(UI_Object* item);
		static void addToNextProcessArray(UI_Object* item);
		static void copyToNextProcessArray();
		static std::list<UI_Object*> processArray;
		static std::list<UI_Object*> nextProcessArray;*/
	
		virtual const bool addKey(unsigned int key, unsigned int mod);
		static UI_Object* focus;

		static bool toolTipWasDeleted;
		const bool isMoving() const;
		
		void adjustSize(const eAdjustMode adjust_mode, const Size& size = Size(0,0));
		void setParent(UI_Object* daddy);
		void setPositionParent(UI_Object* daddy);
		void setOriginalPosition(const Point& position);
		UI_Object* getNextBrother() const;
		void setPosition(const Point& position);
		void setPosition(const unsigned int x, const unsigned int y);
		void setSize(const Size size);
		void setOriginalSize(const Size& size);
		void setOriginalRect(const Rect& rect);
		void setOriginalWidth(const unsigned int width);
		void removeFromFamily();
		void makeFirstChild();

		bool isClipped;
		Rect clipRect;
		void setWidth(const unsigned int width);
	protected:
		void setRect(const Rect& rect);

		void checkForChildrenOverlap(const Rect& rect);
		UI_Object* children; // pointer to the head of the linked list of children
		UI_Object* getPrevBrother() const;
		UI_Object* getParent() const; // TODO make const correctness!
		UI_Object* getPositionParent() const; // TODO make const correctness!
		UI_Object* getChildren() const;
		
		bool childrenWereChanged;
	private:
//		bei wechsel alle rekursiv (-> virtual) durchlaufen und Liste bilden, das aktuelle heraussuchen und aktivieren
//		Ansonsten bei klick Focus legen, Esc/anderer Klick entfernt den Focus (NULL bzw. anderer Fokus)

		void setLeft(const signed int x);
		void setTop(const signed int y);


		
		void adjustPosition();
		Rect relativeRect; // every object needs a current position and size, position is >> RELATIVE << to parent!
		Rect startRect;
		Rect targetRect;
	
		Rect originalRect;
		Size distanceBottomRight;
		Size oldSize;

		const bool hasSizeChanged() const;
		void setSizeHasChanged(const bool size_has_changed = true);
		bool sizeHasChanged;

		void setHeight(const unsigned int height);
		void setSize(const unsigned int width, const unsigned int height);


		ePositionMode positionMode;
		eAutoSize autoSize;
		
		bool shown;

		bool needRedraw;
			
		void addChild(UI_Object* child);
		UI_Object* parent; // = NULL means that this is the screen (x=0/y=0)
		UI_Object* positionParent; // = NULL means that this is the screen (x=0/y=0)
		UI_Object* prevBrother;
		UI_Object* nextBrother; 

		eString toolTipEString;
		std::string toolTipString;


		UI_Object& operator=(const UI_Object& object);
		UI_Object(const UI_Object& object);
	
};

inline const Size& UI_Object::getDistanceBottomRight() const {
	return(distanceBottomRight);
}

inline const Rect& UI_Object::getOriginalRect() const {
	return(originalRect);
}

inline const Point& UI_Object::getOriginalPosition() const {
	return(originalRect.getTopLeft());
}


inline void UI_Object::setOriginalPosition(const Point& position) {
	originalRect.setTopLeft(position);
	setPosition(position);
}

inline void UI_Object::Hide(const bool hide) {
	Show(!hide);
}

inline const bool UI_Object::isShown() const {
	return(shown);
}

inline const Point& UI_Object::getRelativePosition() const {
	return(relativeRect.getTopLeft());
}
			
			
inline const Size& UI_Object::getSize() const {
	return(relativeRect.getSize());
}
	
inline const signed int UI_Object::getRelativeUpperBound() const {
	return(relativeRect.getTop());
}
				
inline const signed int UI_Object::getRelativeLowerBound() const {
	return(relativeRect.getBottom());
}
	
inline const signed int UI_Object::getRelativeLeftBound() const {
	return(relativeRect.getLeft());
}
	
inline const signed int UI_Object::getRelativeRightBound() const {
	return(relativeRect.getRight());
}
	
inline const unsigned int UI_Object::getHeight() const {
	return(relativeRect.getHeight());
}
	
inline const unsigned int UI_Object::getWidth() const {
	return(relativeRect.getWidth());
}
	
inline const Rect& UI_Object::getRelativeRect() const {
	return(relativeRect);
}
	
inline const Point UI_Object::getAbsolutePosition() const
{
	if(positionParent)
		return(relativeRect.getTopLeft() + positionParent->getAbsolutePosition());
	else return(relativeRect.getTopLeft());
}
	
inline const Point UI_Object::getParentAbsolutePosition() const	{
	if(positionParent)
		return(positionParent->getAbsolutePosition());
	else return(Point(0,0));
}
		
inline const Rect UI_Object::getAbsoluteRect() const {
	if(positionParent)
		return(Rect(relativeRect.getTopLeft() + positionParent->getAbsolutePosition(), getSize()));
	else 
		return(relativeRect);
}
		
inline const signed int UI_Object::getAbsoluteUpperBound() const {
	if(positionParent)
		return(relativeRect.getTop() + positionParent->getAbsoluteUpperBound());
	else
		return(relativeRect.getTop());
}
				
inline const signed int UI_Object::getAbsoluteLowerBound() const {
	if(positionParent)
		return(relativeRect.getBottom() + positionParent->getAbsoluteUpperBound());			
	else
		return(relativeRect.getBottom());
}
	
inline const signed int UI_Object::getAbsoluteLeftBound() const {
	if(positionParent)
		return(relativeRect.getLeft() + positionParent->getAbsoluteLeftBound());			
	else
		return(relativeRect.getLeft());
}
	
inline const signed int UI_Object::getAbsoluteRightBound() const {
	if(positionParent)
		return(relativeRect.getRight() + positionParent->getAbsoluteLeftBound());			
	else
		return(relativeRect.getRight());
}

inline UI_Object* UI_Object::getPrevBrother() const {
	return(prevBrother);
}

inline UI_Object* UI_Object::getNextBrother() const {
	return(nextBrother);
}

inline UI_Object* UI_Object::getParent() const {
	return(parent);
}

inline UI_Object* UI_Object::getPositionParent() const {
	return(positionParent);
}

inline UI_Object* UI_Object::getChildren() const {
	return(children);
}

inline void UI_Object::updateToolTip(const eString tool_tip_string) {
	toolTipEString = tool_tip_string;
}

inline void UI_Object::updateToolTip(const std::string& tool_tip_string) {
	toolTipString = tool_tip_string;
}

inline const bool UI_Object::hasToolTip() const {
	return((toolTipString!="")||(toolTipEString!=NULL_STRING));
}

inline const unsigned int UI_Object::getTargetWidth() const {
	return(targetRect.getWidth());
}

inline const unsigned int UI_Object::getTargetHeight() const {
	return(targetRect.getHeight());
}

inline const Point UI_Object::getTargetPosition() const {
	return(targetRect.getTopLeft());
}

inline const Rect& UI_Object::getTargetRect() const {
	return(targetRect);
}

inline const bool UI_Object::isMouseInside() const {
	return(getAbsoluteRect().Inside(mouse));
}



inline void UI_Object::setPosition(const unsigned int x, const unsigned int y) {
	setPosition(Point(x, y));
}

inline const bool UI_Object::isTopItem() const {
	return(positionParent==NULL);
}

inline const eString UI_Object::getToolTipEString() const {
	return(toolTipEString);
}

inline const std::string& UI_Object::getToolTipString() const {
	return(toolTipString);
}

#endif

