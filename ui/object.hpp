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

	    const Point getAbsolutePosition() const	{
		    if(parent)
				        return(relativeRect.GetPosition() + parent->getAbsolutePosition());
			    else return(relativeRect.GetPosition());
		};
		
	    const Point getRelativePosition() const {
		    return(relativeRect.GetPosition());
		};
				
			
		void setWidth(const int width) {
		    relativeRect.width=width;
		}
				
			
		void setHeight(const int height) {
		    relativeRect.height=height;
		}
				
		void setSize(const Size size) {
		    relativeRect.SetSize(size);
		}
	
		void setPosition(const Point position) {
    		relativeRect.SetPosition(position);
		}
		
		void setLeft(const int x) {
			relativeRect.x=x;
		}
		
		void setTop(const int y) {
		    relativeRect.y=y;
		}
				
    	const Size getSize() const {
		    return(relativeRect.GetSize());
		}
	
		const int getRelativeUpperBound() const {
		    return(relativeRect.y);
		}
				
    	const int getRelativeLowerBound() const {
		    return(relativeRect.y+relativeRect.height);
		}
	
	    const int getRelativeLeftBound() const {
		    return(relativeRect.x);
		}
	
    	const int getRelativeRightBound() const {
		    return(relativeRect.x+relativeRect.width);
		}
	
	    const int getHeight() const {
		    return(relativeRect.height);
		}
	
    	const int getWidth() const {
    		return(relativeRect.width);
		}
	
		const bool isTopItem() const;
		const Rect getRelativeRect() const {
		    return(relativeRect);
		}
				
		const Rect getAbsoluteRect() const {
		    if(parent)
		        return(Rect(relativeRect.GetPosition() + parent->getAbsolutePosition(), getSize()));
		    else return(Rect(relativeRect.GetPosition(), getSize()));
		}
		

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
		static const bool isTimeSpanElapsed(const long int timeSpan);
		
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

		const bool doesNeedRedraw() const;
		void setNeedRedraw(const bool needRedraw=true);

		static SDL_Rect rectlist[3000];
		static int rectnumber;
		
	protected:
		void move(int& x, const int sx, const int tx);
		void move(Point& p, const Point sp, const Point tp);
		void move(Rect& r, const Rect sr, const Rect tr);

		bool shown;
		bool disabledFlag;

	private:
		UI_Object* prevBrother;
		UI_Object* nextBrother; 

		UI_Object* parent; // = NULL means that this is the screen (x=0/y=0)
		UI_Object* children; // pointer to the head of the linked list of children

		Rect relativeRect; // every object needs a current position and size, position is >> RELATIVE << to parent!
		Rect lastRect;
	// to adjust object smoothly
//		Rect startRect;
//		Rect targetRect;
		Rect maxRect;

		// needs redraw?
		bool needRedraw;
			
// ignore maxRect for the next adjustWindow calls - important for tutorials
		bool isFreeMove;
		int doAdjustments;

		static long int startTime;

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

