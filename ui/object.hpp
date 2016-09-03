#ifndef _UI_OBJECT_HPP
#define _UI_OBJECT_HPP

#include "theme.hpp"

class UI_StaticText;

class UI_Object
{
	public:
		UI_Object& operator=(const UI_Object& object);
		UI_Object(const UI_Object& object);
		
        void Show(const bool show=true);
        void Hide(const bool hide=true);

		void Enable(const bool enable=true);
		void Disable(const bool disable=true);
		
		const bool isShown() const;
		const bool isDisabled() const;

		const bool isTopItem() const;
		
	    const Point& getRelativePosition() const {
		    return(relativeRect.GetTopLeft());
		}
			
		void setWidth(const unsigned int width) {
		    relativeRect.SetWidth(width);
		}
				
		void setHeight(const unsigned int height) {
		    relativeRect.SetHeight(height);
		}
				
		void setSize(const Size size) {
		    relativeRect.SetSize(size);
		}
	
		void setPosition(const Point& position);
		
		void setLeft(const signed int x) {
			relativeRect.SetLeft(x);
		}
		
		void setTop(const signed int y) {
		    relativeRect.SetTop(y);
		}
				
    	const Size& getSize() const {
		    return(relativeRect.GetSize());
		}
	
		const signed int getRelativeUpperBound() const {
		    return(relativeRect.GetTop());
		}
				
    	const signed int getRelativeLowerBound() const {
		    return(relativeRect.GetBottom());
		}
	
	    const signed int getRelativeLeftBound() const {
		    return(relativeRect.GetLeft());
		}
	
    	const signed int getRelativeRightBound() const {
		    return(relativeRect.GetRight());
		}
	
	    const unsigned int getHeight() const {
		    return(relativeRect.GetHeight());
		}
	
    	const unsigned int getWidth() const {
    		return(relativeRect.GetWidth());
		}
		
		const Rect& getRelativeRect() const {
		    return(relativeRect);
		}
		
		const Point getParentAbsolutePosition() const	{
		    if(parent)
		        return(parent->getAbsolutePosition());
		    else return(Point(0,0));
		}

	    const Point getAbsolutePosition() const;
		/*{
		    if(parent)
		        return(relativeRect.GetTopLeft() + parent->getAbsolutePosition());
		    else return(relativeRect.GetTopLeft());
		}*/
		
		const Rect getAbsoluteRect() const {
		    if(parent)
		        return(Rect(relativeRect.GetTopLeft() + parent->getAbsolutePosition(), getSize()));
		    else return(relativeRect);
		}
		
		const signed int getAbsoluteUpperBound() const {
			if(parent)
				return(relativeRect.GetTop() + parent->getAbsoluteUpperBound());
			else
			    return(relativeRect.GetTop());
		}
				
    	const signed int getAbsoluteLowerBound() const {
            if(parent)
                return(relativeRect.GetBottom() + parent->getAbsoluteUpperBound());            else
                return(relativeRect.GetBottom());
		}
	
	    const signed int getAbsoluteLeftBound() const {
            if(parent)
                return(relativeRect.GetLeft() + parent->getAbsoluteLeftBound());            else
                return(relativeRect.GetLeft());
		}
	
    	const signed int getAbsoluteRightBound() const {
            if(parent)
                return(relativeRect.GetRight() + parent->getAbsoluteLeftBound());            else
                return(relativeRect.GetRight());
		}

		void setFreeMove(const bool is_free_move=true);
		
		const unsigned int getTargetWidth() const;
		const unsigned int getTargetHeight() const;

		void setMaxRect(const Rect& rect) {
			this->maxRect=rect;
		}
		
		void setRelativeRect(const Rect& rect) {
			this->relativeRect=rect;
		}

		const Rect& getMaxRect() const;
		const unsigned int getDoAdjustments() const;
		void setDoAdjustments(const unsigned int doAdjustments=1);
		void adjustRelativeRect(Rect rect);

//		void resetToOriginalValues(); // ~~

		virtual UI_Object* checkHighlight();
		const bool isMouseInside() const;

		void setParent(UI_Object* daddy);
		void removeFromFamily();
		
		virtual void draw(DC* dc) const;

		UI_Object(UI_Object* parent_object, const Rect relative_rect = Rect(0,0,0,0), const Rect max_rect = Rect(0,0,0,0));
		virtual ~UI_Object();

		virtual void process();

		UI_Object* getNextBrother() const;
		UI_Object* getPrevBrother() const;
		UI_Object* getParent() const; // TODO make const correctness!
		UI_Object* getChildren() const;


	
		void updateToolTip(const eString tooltip);

		static void assignStartTime();
		static const unsigned long int getTimeStampMs(const unsigned long int timeSpan);
		static const bool isTimeSpanElapsed(const unsigned long int timeSpan);
		
		// we added an item at this y position

		Rect startRect; // TODO private machen
		Rect targetRect;
		unsigned int filledHeight;


		const bool doesNeedRedraw() const;
		void setNeedRedraw(const bool need_redraw=true);

		static SDL_Rect rectlist[3000];
		static unsigned int rectnumber;
		static unsigned int max_x;
		static unsigned int max_y;
		static UI_Theme theme;

		static void maybeShowToolTip(DC* dc);
		static void hideToolTip();

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
		static UI_StaticText* editTextFeld; //~~

//		const bool setFocus(); TODO
	protected:
/*		void move(signed int& x, const signed int sx, const signed int tx);
		void move(Point& p, const Point sp, const Point tp);
		void move(Rect& r, const Rect sr, const Rect tr);
		void move(unsigned int& x, const unsigned int sx, const unsigned int tx);*/

		UI_Object* children; // pointer to the head of the linked list of children

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
			
// ignore maxRect for the next adjustWindow calls - important for tutorials
		bool isFreeMove;

		SDL_Surface* tempSurface;
		
		UI_Object* prevBrother;
		UI_Object* nextBrother; 

		UI_Object* parent; // = NULL means that this is the screen (x=0/y=0)

		Rect relativeRect; // every object needs a current position and size, position is >> RELATIVE << to parent!
		Rect lastRect;
		
	// to adjust object smoothly
//		Rect startRect;
//		Rect targetRect;
		Rect maxRect;
		unsigned int doAdjustments;
		
		eString toolTip;
	
		static unsigned long int startTime;


		static bool toolTipIsShown;
		static string toolTipString;
		static Point toolTipPosition;
		static Rect lastToolTipRect;
		static bool needToolTipRedraw;

		static void addRectToBeDrawn(Rect& lastRect, const Rect currentRect);
//		int linkedToHotSpot; // is this item linked constantly to a hotspot? Or is his position determined by hardcoded functions?

//		int hasFocus; // make every object accessible with the keyboard (TAB)
//		int canTransform; // is the width/height constant or can it be transformed?
//		int isBoundToParent; // is the width/height somehow bound to the parent? (e.g. scrollBar)
	
	
//		int hotkey;
};

#endif

