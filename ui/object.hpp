#ifndef _UI_OBJECT_HPP
#define _UI_OBJECT_HPP

#include "theme.hpp"
#include <list>
#include <string>

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
	ARRANGE_TOP_LEFT,
	ARRANGE_TOP_RIGHT,
	ARRANGE_BOTTOM_LEFT,
	ARRANGE_BOTTOM_RIGHT,
	ARRANGE_LEFT,
	ARRANGE_RIGHT,
	SPECIAL_BUTTON_LEFT, // normal button but arrange left text
	SPECIAL_BUTTON_ARRANGE_TOP_LEFT,
	SPECIAL_BUTTON_ARRANGE_TOP_RIGHT
};

enum eAutoSize
{
	NOTHING,
	NO_AUTO_SIZE,
	AUTO_SIZE_ONCE,
	AUTO_SIZE_TWICE,
	AUTO_SIZE,
	AUTO_HEIGHT_FULL_WIDTH, // fehlerhaft! TODO
	FULL_WIDTH,
	AUTO_HEIGHT_CONST_WIDTH,
	CONST_SIZE //~~ constant width
};

enum eAdjustMode
{
	ADJUST_ONLY_POSITION,
	ADJUST_AFTER_PARENT_WAS_CHANGED,
	ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED
};

class UI_Object;
class UI_EditField;
class UI_ToolTip;
class UI_Button;
class UI_Window;

class UI_Object
{
	public:
		UI_Object& operator=(const UI_Object& object);
		UI_Object(const UI_Object& object);
		UI_Object(UI_Object* parent_object, const Rect relative_rect = Rect(0, 0, 0, 0), const Size distance_bottom_right = Size(0, 0), const ePositionMode position_mode = DO_NOT_ADJUST, const eAutoSize auto_size = NO_AUTO_SIZE);

		virtual ~UI_Object();
	
		void Show(const bool show=true);
		void Hide(const bool hide=true);

		const bool isShown() const;
		const bool isTopItem() const;
		
		const Point& getRelativePosition() const;
		void setPosition(const Point& position);
		void setPosition(const unsigned int x, const unsigned int y);

		void setRect(const Rect& rect);
		
		void setLeft(const signed int x);
		void setTop(const signed int y);
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

		
		
		
		void checkForChildrenOverlap(const Rect& rect);
		const bool checkForNeedRedraw() const;
		void setNeedRedrawMoved(const bool need_redraw=true);
		void setNeedRedrawNotMoved(const bool need_redraw=true);

		virtual UI_Object* checkHighlight();
		virtual UI_Object* checkToolTip();
		virtual void reloadStrings();
		virtual void reloadOriginalSize();

		UI_Object* getNextBrother() const;
		UI_Object* getPrevBrother() const;
		UI_Object* getParent() const; // TODO make const correctness!
		UI_Object* getChildren() const;
		void setParent(UI_Object* daddy);
		void removeFromFamily();
		void makeFirstChild();
		
		virtual void draw(DC* dc) const;
		virtual void process();
		virtual void resetData();

		void updateToolTip(const eString tooltip);
		const eString getToolTipString() const;

		static unsigned int redrawnObjects;
		static void setResolution(const Size resolution);
		static UI_Theme theme;
		static UI_Window* currentWindow;
		static bool windowSelected;
	
		
//		static void maybeShowToolTip(DC* dc);
//		static void hideToolTip();
//
//		

		
		void resetMinXY();
		
		void addMinTopLeftX(signed int dx);
		void addMinLeftY(signed int dy);
		void addMinRightY(signed int dy);
		void addMinBottomLeftX(signed int dx);
		void addMinTopRightX(signed int dx);
		void addMinBottomRightX(signed int dx);
	
		signed int getMinTopLeftX() const { return min_top_left_x;};
		signed int getMinLeftY() const { return min_left_y;};
		signed int getMinRightY() const { return min_right_y;};
		signed int getMinBottomLeftX() const { return min_bottom_left_x;};
		signed int getMinTopRightX() const { return min_top_right_x;};
		signed int getMinBottomRightX() const { return min_bottom_right_x;};
	
		static unsigned int mouseType;
		static Point mouse;
		static UI_EditField* editTextField; //~~
//		static UI_EndRunDialog* endRunDialog;

		static UI_ToolTip* tooltip;
		static UI_Object* toolTipParent;

		void clearRedrawFlag();

		static std::list<std::string> msgList;

		void adjustPositionAndSize(const eAdjustMode adjust_mode, const Size& size = Size(0,0));
		void setSize(const Size size);
		bool sizeHasChanged;
		static unsigned int max_x;
		static unsigned int max_y;
		void setOriginalPosition(const Point& position);
		void setOriginalSize(const Size& size);
		void setOriginalRect(const Rect& rect);

		const Rect& getOriginalRect() const;
		const Size& getDistanceBottomRight() const;
	

	protected:
		UI_Object* children; // pointer to the head of the linked list of children
	private:

				
		Rect relativeRect; // every object needs a current position and size, position is >> RELATIVE << to parent!
		Rect startRect; // TODO private machen
		Rect targetRect;
	
		Rect originalRect;
		Size distanceBottomRight;
		Size oldSize;

		void setWidth(const unsigned int width);
		void setHeight(const unsigned int height);
		void setSize(const unsigned int width, const unsigned int height);


		ePositionMode positionMode;
		eAutoSize autoSize;
		
		bool shown;

		signed int min_top_left_x, min_left_y, min_right_y, min_bottom_left_x;
		signed int min_top_right_x/*, min_left_y, min_right_y*/, min_bottom_right_x;
	
		bool needRedraw;
			
		void addChild(UI_Object* child);
		UI_Object* parent; // = NULL means that this is the screen (x=0/y=0)
		UI_Object* prevBrother;
		UI_Object* nextBrother; 

		eString toolTipString;
};

inline const Size& UI_Object::getDistanceBottomRight() const {
	return(distanceBottomRight);
}

inline const Rect& UI_Object::getOriginalRect() const {
	return(originalRect);
}

inline void UI_Object::setOriginalPosition(const Point& position) {
	originalRect.SetTopLeft(position);
	setPosition(position);
}

inline void UI_Object::setOriginalSize(const Size& size) {
	originalRect.SetSize(size);
	setSize(size);
}

inline void UI_Object::Hide(const bool hide) {
	Show(!hide);
}

inline const bool UI_Object::isShown() const {
	return(shown);
}

inline const Point& UI_Object::getRelativePosition() const {
	return(relativeRect.GetTopLeft());
}
			
			
inline const Size& UI_Object::getSize() const {
	return(relativeRect.GetSize());
}
	
inline const signed int UI_Object::getRelativeUpperBound() const {
	return(relativeRect.GetTop());
}
				
inline const signed int UI_Object::getRelativeLowerBound() const {
	return(relativeRect.GetBottom());
}
	
inline const signed int UI_Object::getRelativeLeftBound() const {
	return(relativeRect.GetLeft());
}
	
inline const signed int UI_Object::getRelativeRightBound() const {
	return(relativeRect.GetRight());
}
	
inline const unsigned int UI_Object::getHeight() const {
	return(relativeRect.GetHeight());
}
	
inline const unsigned int UI_Object::getWidth() const {
	return(relativeRect.GetWidth());
}
	
inline const Rect& UI_Object::getRelativeRect() const {
	return(relativeRect);
}
	
inline const Point UI_Object::getAbsolutePosition() const
{
	if(parent)
		return(relativeRect.GetTopLeft() + parent->getAbsolutePosition());
	else return(relativeRect.GetTopLeft());
}
	
inline const Point UI_Object::getParentAbsolutePosition() const	{
	if(parent)
		return(parent->getAbsolutePosition());
	else return(Point(0,0));
}
		
inline const Rect UI_Object::getAbsoluteRect() const {
	if(parent)
		return(Rect(relativeRect.GetTopLeft() + parent->getAbsolutePosition(), getSize()));
	else 
		return(relativeRect);
}
		
inline const signed int UI_Object::getAbsoluteUpperBound() const {
	if(parent)
		return(relativeRect.GetTop() + parent->getAbsoluteUpperBound());
	else
		return(relativeRect.GetTop());
}
				
inline const signed int UI_Object::getAbsoluteLowerBound() const {
	if(parent)
		return(relativeRect.GetBottom() + parent->getAbsoluteUpperBound());			
	else
		return(relativeRect.GetBottom());
}
	
inline const signed int UI_Object::getAbsoluteLeftBound() const {
	if(parent)
		return(relativeRect.GetLeft() + parent->getAbsoluteLeftBound());			
	else
		return(relativeRect.GetLeft());
}
	
inline const signed int UI_Object::getAbsoluteRightBound() const {
	if(parent)
		return(relativeRect.GetRight() + parent->getAbsoluteLeftBound());			
	else
		return(relativeRect.GetRight());
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

inline UI_Object* UI_Object::getChildren() const {
	return(children);
}

inline void UI_Object::updateToolTip(const eString tool_tip) {
	toolTipString = tool_tip;
}

inline const eString UI_Object::getToolTipString() const {
	return(toolTipString);
}

inline const unsigned int UI_Object::getTargetWidth() const {
	return(targetRect.GetWidth());
}

inline const unsigned int UI_Object::getTargetHeight() const {
	return(targetRect.GetHeight());
}

inline const Point UI_Object::getTargetPosition() const {
	return(targetRect.GetTopLeft());
}

inline const Rect& UI_Object::getTargetRect() const {
	return(targetRect);
}

inline const bool UI_Object::isMouseInside() const {
	return(getAbsoluteRect().Inside(mouse));
}

inline void UI_Object::setSize(const unsigned int width, const unsigned int height) {
	setSize(Size(width, height));
}

inline void UI_Object::setPosition(const unsigned int x, const unsigned int y) {
	setPosition(Point(x, y));
}

inline const bool UI_Object::isTopItem() const {
	return(parent==NULL);
}

#endif

