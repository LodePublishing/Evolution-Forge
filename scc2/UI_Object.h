#ifndef __UI_OBJECT_H
#define __UI_OBJECT_H

#include "controls.h"
#include "../scc2dll/settings.h"
#include "UI_Theme.h"
#include "util.h"

class UI_Object
{
	public:
        void Show(const bool show=true);
        void Hide(const bool show=false);
		bool isShown();

	    Point getAbsolutePosition();
	    Point getAbsolutePosition2();
	    Point getRelativePosition();
		void setSize(Size size);
    	Size getSize();
		
		int getRelativeUpperBound();
    	int getRelativeLowerBound();
	    int getRelativeLeftBound();
    	int getRelativeRightBound();
	    int getHeight();
    	int getWidth();

		bool isTopItem();
		Rect getRelativeRect();

//		void setOriginalValues(const int originalValues); TODO
//		void resetToOriginalValues();

//		int isMouseInside();
		void setParent(UI_Object* daddy);
		void addChild(UI_Object* child);
		void removeFromFamily();
		

//		draw the object and all its children
		virtual void draw(DC* dc);

		UI_Object(UI_Object* parent, const Rect rect = Rect(0,0,0,0), const Rect rect = Rect(0,0,0,0));
		virtual ~UI_Object();

		const Rect getMaxRect();


		static void assignStartTime();
		static long int getTimeStampMs(long int timeSpan);
		static bool isTimeSpanElapsed(long int time);
		
		int min_top_x, min_left_y, min_right_y, min_bottom_x;

		// we added an item at this y position
		int lastItemY;

		int getDoAdjustments();		
		void setDoAdjustments(const int doAdjustments=1);
		void adjustRelativeRect(Rect rect);
		void forceSmallY(const int dy);

		virtual void process();

		//		virtual void message(int id);~~ TODO

		UI_Object* getNextBrother();
		UI_Object* getPrevBrother();
		UI_Object* getParent();
		UI_Object* getChildren();


		void setFreeMove(const bool isFreeMove=true);
		int getTargetWidth();
		int getTargetHeight();

		static UI_Theme theme;
		void setAbsoluteCoord(Point p); // helper

		void setMaxRect(const Rect& rect) {this->relativeRect=rect;};
		void setRelativeRect(const Rect& rect) {this->maxRect=rect;};
	protected:
		bool shown;
		bool disabledFlag;
	private:
		UI_Object* prevBrother;
		UI_Object* nextBrother; 

		UI_Object* parent; // = NULL means that this is the screen (x=0/y=0)
		UI_Object* children; // pointer to the head of the linked list of children

		Rect relativeRect; // every object needs a current position and size, position is >> RELATIVE << to parent!
	// to adjust object smoothly
		Rect startRect;
		Rect targetRect;
		Rect maxRect;

		Point absoluteCoord; // modificator if rect.GetPosition != real position
			
// ignore maxRect for the next adjustWindow calls - important for tutorials
		bool isFreeMove;
		int doAdjustments;

		static long startTime;
		
	
//		int linkedToHotSpot; // is this item linked constantly to a hotspot? Or is his position determined by hardcoded functions?
//		int hotspot; // number to look up the hotspot table
		

//		int hasFocus; // make every object accessible with the keyboard (TAB)
//		int canTransform; // is the width/height constant or can it be transformed?
//		int isBoundToParent; // is the width/height somehow bound to the parent? (e.g. scrollBar)
	
// look up value in the array with original window positions & sizes
//		int originalValues; ~~
	
//		int hotkey;
};

#endif

