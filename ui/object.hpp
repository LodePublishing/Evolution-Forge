#ifndef _UI_OBJECT_HPP
#define _UI_OBJECT_HPP

#include "theme.hpp"
#include <list>

class UI_Object;
class UI_EditField;
class UI_Tooltip;
class UI_Button;
class UI_Window;

class UI_Object
{
	public:
		UI_Object& operator=(const UI_Object& object);
		UI_Object(const UI_Object& object);
		UI_Object(UI_Object* parent_object, const Rect relative_rect = Rect(0,0,0,0));
		virtual ~UI_Object();
	
		void setRelativeRect(const Rect& rect);
		void Show(const bool show=true);
		void Hide(const bool hide=true);

		void Enable(const bool enable=true);
		void Disable(const bool disable=true);
		
		const bool isShown() const;
		const bool isDisabled() const;

		const bool isTopItem() const;
		
		const Point& getRelativePosition() const;
		void setWidth(const unsigned int width);
		void setHeight(const unsigned int height);
		void setSize(const Size size);
		void setPosition(const Point& position);
//		void jumpToPosition(const Point& position);
		
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

		void checkForChildrenOverlap(const Rect& rect);
		const bool checkForNeedRedraw() const;
		void setNeedRedrawMoved(const bool need_redraw=true);
		void setNeedRedrawNotMoved(const bool need_redraw=true);

		const unsigned int getTargetWidth() const;
		const unsigned int getTargetHeight() const;
		const Point getTargetPosition() const;

		const unsigned int getDoAdjustments() const;
		void setDoAdjustments(const unsigned int doAdjustments=1);
		void adjustRelativeRect(Rect rect);

//		void resetToOriginalValues(); // ~~

		virtual UI_Object* checkHighlight();
		virtual UI_Object* checkTooltip();
		virtual void reloadStrings();
		virtual void adjustButtonPlacementSize();
		const bool isMouseInside() const;

		void setParent(UI_Object* daddy);
		void removeFromFamily();
		void makeFirstChild();
		
		virtual void draw(DC* dc) const;

		
		virtual void process();

		UI_Object* getNextBrother() const;
		UI_Object* getPrevBrother() const;
		UI_Object* getParent() const; // TODO make const correctness!
		UI_Object* getChildren() const;


	
		void updateToolTip(const eString tooltip);
		const eString getToolTipString() const;

		static void assignStartTime();
		static const unsigned long int getTimeStampMs(const unsigned long int timeSpan);
		static const bool isTimeSpanElapsed(const unsigned long int timeSpan);
		
		// we added an item at this y position

		Rect startRect; // TODO private machen
		Rect targetRect;
		unsigned int filledHeight;
		
		signed int firstItemY;
		signed int lastItemY;



		static unsigned int redrawnObjects;

//		static SDL_Rect rectlist[3000];
//		static unsigned int rectnumber;
		static unsigned int max_x;
		static unsigned int max_y;
		static UI_Theme theme;

		static bool currentButtonPressed;
		static bool currentButtonHasAlreadyLeft;
		static UI_Button* currentButton;
		static UI_Window* currentWindow;
		static bool windowSelected;
		static void resetButton();

//		static void maybeShowToolTip(DC* dc);
//		static void hideToolTip();

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

		static UI_Tooltip* tooltip;
		static UI_Object* toolTipParent;

		void clearRedrawFlag();

//		void addRect(const Rect& rect);

//		const bool setFocus(); TODO
	protected:
/*		void move(signed int& x, const signed int sx, const signed int tx);
		void move(Point& p, const Point sp, const Point tp);
		void move(Rect& r, const Rect sr, const Rect tr);
		void move(unsigned int& x, const unsigned int sx, const unsigned int tx);*/

		UI_Object* children; // pointer to the head of the linked list of children
//		std::list<Rect> notDrawRectList;
		SDL_Surface* background;
		Rect oldRect;

	private:
		
		bool shown;
		bool disabledFlag;

		signed int min_top_left_x, min_left_y, min_right_y, min_bottom_left_x;
		signed int min_top_right_x/*, min_left_y, min_right_y*/, min_bottom_right_x;
	
		void addChild(UI_Object* child);
//		bool canGetFocus;
//		bool hasFocus; TODO
		// returns false if it (and none of its children) cannot get focus

// needs redraw?
		bool needRedraw;
			
		UI_Object* prevBrother;
		UI_Object* nextBrother; 

		UI_Object* parent; // = NULL means that this is the screen (x=0/y=0)

		Rect relativeRect; // every object needs a current position and size, position is >> RELATIVE << to parent!
//		Rect lastRect;
		
	// to adjust object smoothly
//		Rect startRect;
//		Rect targetRect;
		unsigned int doAdjustments;
		
		eString toolTipString;

		static unsigned long int startTime;

//		list<Rect> changedRects;

//		static void addRectToBeDrawn(Rect& lastRect, const Rect currentRect);
//		int linkedToHotSpot; // is this item linked constantly to a hotspot? Or is his position determined by hardcoded functions?

//		int hasFocus; // make every object accessible with the keyboard (TAB)
//		int canTransform; // is the width/height constant or can it be transformed?
//		int isBoundToParent; // is the width/height somehow bound to the parent? (e.g. scrollBar)
	
	
//		int hotkey;

};

inline void UI_Object::setRelativeRect(const Rect& rect) {
	this->relativeRect=rect;
}

inline void UI_Object::Hide(const bool hide) {
	Show(!hide);
}

inline void UI_Object::Enable(const bool enable) {
	disabledFlag = !enable;
}

inline void UI_Object::Disable(const bool disable) {
	disabledFlag = disable;
}

inline const bool UI_Object::isShown() const {
	return(shown);
}

inline const bool UI_Object::isDisabled() const {
	return(disabledFlag);
}

inline void UI_Object::assignStartTime() {
	startTime = SDL_GetTicks();
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
		
inline const Point UI_Object::getParentAbsolutePosition() const	{
	if(parent)
		return(parent->getAbsolutePosition());
	else return(Point(0,0));
}

inline const Point UI_Object::getAbsolutePosition() const
{
	if(parent)
		return(relativeRect.GetTopLeft() + parent->getAbsolutePosition());
	else return(relativeRect.GetTopLeft());
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

inline const unsigned int UI_Object::getDoAdjustments() const {
	return(doAdjustments);
}

inline void UI_Object::setDoAdjustments(const unsigned int do_adjustments) {
	doAdjustments=do_adjustments;
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

inline const bool UI_Object::isMouseInside() const {
	return(getAbsoluteRect().Inside(mouse));
}



#endif

