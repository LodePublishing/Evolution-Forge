#ifndef _UI_OBJECT_HPP
#define _UI_OBJECT_HPP

#include "controls.hpp"
//#include "../scc2dll/settings.hpp" ?
#include "theme.hpp"

class UI_Object
{
	public:
        void Show(const bool show=true);
        void Hide(const bool show=false);
		const bool isShown() const;

	    const Point getAbsolutePosition() const;
	    const Point getRelativePosition() const;
		void setWidth(const int width);
		void setHeight(const int height);
		void setSize(const Size size);
		void setPosition(const Point position);
		void setLeft(const int x);
		void setTop(const int y);
    	const Size getSize() const;
		
		const int getRelativeUpperBound() const;
    	const int getRelativeLowerBound() const;
	    const int getRelativeLeftBound() const;
    	const int getRelativeRightBound() const;
	    const int getHeight() const;
    	const int getWidth() const;

		const bool isTopItem() const;
		const Rect getRelativeRect() const;
		const Rect getAbsoluteRect() const;

//		void setOriginalValues(const int originalValues); TODO
//		void resetToOriginalValues();

		const bool isMouseInside() const;
		void setParent(UI_Object* daddy);
		void addChild(UI_Object* child);
		void removeFromFamily();
		
//		draw the object and all its children
		virtual void draw(DC* dc) const;

		UI_Object(UI_Object* parent, const Rect relativeRect = Rect(0,0,0,0), const Rect maxRect = Rect(0,0,0,0));
		virtual ~UI_Object();

		const Rect getMaxRect() const;


		static void assignStartTime();
		static const long int getTimeStampMs(const long int timeSpan);
		static const bool isTimeSpanElapsed(const long int time);
		
		int min_top_x, min_left_y, min_right_y, min_bottom_x;

		// we added an item at this y position
		int lastItemY;

		const int getDoAdjustments() const;
		void setDoAdjustments(const int doAdjustments=1);
		void adjustRelativeRect(Rect rect);
		void forceSmallY(const int dy);

		virtual void process();

		//		virtual void message(int id);~~ TODO

		UI_Object* getNextBrother() const;
		UI_Object* getPrevBrother() const;
		UI_Object* getParent() const; // TODO make const correctness!
		UI_Object* getChildren() const;


		void setFreeMove(const bool isFreeMove=true);
		const int getTargetWidth() const;
		const int getTargetHeight() const;

		static UI_Theme theme;
		void setAbsoluteCoord(const Point p); // helper

		void setMaxRect(const Rect& rect) {this->maxRect=rect;};
		void setRelativeRect(const Rect& rect) {this->relativeRect=rect;};

		void updateToolTip(const string& tooltip);

	
		Rect startRect;
		Rect targetRect;
		
	protected:
		void move(int& x, const int sx, const int tx);

		bool shown;
		bool disabledFlag;

	private:
		UI_Object* prevBrother;
		UI_Object* nextBrother; 

		UI_Object* parent; // = NULL means that this is the screen (x=0/y=0)
		UI_Object* children; // pointer to the head of the linked list of children

		Rect relativeRect; // every object needs a current position and size, position is >> RELATIVE << to parent!
	// to adjust object smoothly
//		Rect startRect;
//		Rect targetRect;
		Rect maxRect;

			
// ignore maxRect for the next adjustWindow calls - important for tutorials
		bool isFreeMove;
		int doAdjustments;

		static long startTime;

		string toolTip;
		void maybeShowToolTip(DC* dc) const;

//		int linkedToHotSpot; // is this item linked constantly to a hotspot? Or is his position determined by hardcoded functions?

//		int hasFocus; // make every object accessible with the keyboard (TAB)
//		int canTransform; // is the width/height constant or can it be transformed?
//		int isBoundToParent; // is the width/height somehow bound to the parent? (e.g. scrollBar)
	
// look up value in the array with original window positions & sizes
//		int originalValues; ~~
	
//		int hotkey;
};

#endif

