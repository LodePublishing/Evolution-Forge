#ifndef _SDL_RECT_HPP
#define _SDL_RECT_HPP

#include "size.hpp"
#include "point.hpp"

class Rect
{
	public:
//		signed int x, y;
//		unsigned int width, height;

		Rect() : 
			topLeftCorner(0,0), 
			bottomRightCorner(0,0), 
			rectSize(0,0) 
		{ }
		
		Rect(const signed int x, const signed int y, const unsigned int w, const unsigned int h);

		/*{
			if((w > 1280)||(h>=1024))
				toLog("ERROR, width/height too large");
			topLeftCorner.x=x;
			topLeftCorner.y=y;
			bottomRightCorner.x=x+w;
			bottomRightCorner.y=y+h;
			rectSize.width=w;
			rectSize.height=h;
		}*/
		
/*		Rect(const Point& topLeft, const Point& bottomRight) :
			topLeftCorner(topLeft),
			bottomRightCorner(bottomRight),
			rectSize(bottomRight - topLeft)
		{ } TODO */
		
		Rect(const Point& pos, const Size& size) :
			topLeftCorner(pos),
			bottomRightCorner(pos + size),
			rectSize(size)
		{ }
		
		Rect(const Rect& rect) : 
			topLeftCorner(rect.GetTopLeft()), 
			bottomRightCorner(rect.GetBottomRight()),
			rectSize(rect.GetSize()) 
		{ }
	
		const Point& GetTopLeft() const { 
			return topLeftCorner; 
		}

		void SetTopLeft( const Point& point ) { 
			topLeftCorner = point; 
			bottomRightCorner = point + rectSize;
		}
		
		const Point& GetBottomRight() const {
			return bottomRightCorner;
		}

/*		void SetBottomRight( const Point& point) {
			rectSize.width += point.x  
			bottomRightCorner = point;
		} TODO */
	
		const Size& GetSize() const { 
			return rectSize; 
		}
		
		void SetSize( const Size& size ) {
			rectSize=size;
			bottomRightCorner = topLeftCorner + size;
		}
	
		const unsigned int GetWidth() const { 
			return rectSize.GetWidth();
		}
		
		void SetWidth(const unsigned int width) {
			rectSize.SetWidth(width);
			bottomRightCorner.x = topLeftCorner.x + (signed int)width;
		}
		
		const unsigned int GetHeight() const { 
			return rectSize.GetHeight(); 
		}
		
		void SetHeight(const unsigned int height) { 
			rectSize.SetHeight(height);
            bottomRightCorner.y = topLeftCorner.y + (signed int)height;
		}
		
		const signed int GetLeft() const {
			return topLeftCorner.x;
		}
		
		const signed int GetTop() const {
			return topLeftCorner.y;
		}
		
		const signed int GetBottom() const { 
			return bottomRightCorner.y;
		}
		
		const signed int GetRight()  const { 
			return bottomRightCorner.x;
		}

		void SetLeft(const signed int left) { 
			topLeftCorner.x = left; 
			bottomRightCorner.x = left + rectSize.GetWidth();
		}
		
		void SetTop(const signed int top) { 
			topLeftCorner.y = top; 
			bottomRightCorner.y = top + rectSize.GetHeight();
		}
		
		void SetRight(const signed int right);
/*		{
		#ifdef _SCC_DEBUG
			if(right < GetLeft()) {
				toLog("ERROR: SetRight was put left of GetLeft!");return;
			}				
		#endif
			SetWidth(right + 1 - GetLeft());
		}*/

		void SetBottom(const signed int bottom) 
		{
        #ifdef _SCC_DEBUG
            if(bottom < GetTop()) {
                toLog("ERROR: SetBottom was put above of GetTop!");return;
            }
        #endif
			bottomRightCorner.y = bottom;
            SetHeight(bottom - GetTop());
		}
				
/*		const Rect operator+(const Rect& rect) const {
			return(Rect(p + rect.p, s + rect.s));
		} TODO
		
		Rect& operator+=(const Rect& rect)
		{
    		return ( *this = (*this + rect) );
		}*/
	
		Rect& operator=(const Rect& rect) 
		{	
			topLeftCorner = rect.GetTopLeft();
			bottomRightCorner = rect.GetBottomRight();
			rectSize = rect.GetSize();
			return(*this);
		}
		
		const bool operator==(const Rect& rect) const 
		{ 
			if( ( GetTopLeft() != rect.GetTopLeft() ) || ( GetSize() != rect.GetSize() ) ) 
				return false;
			else return true; 
		}
		
		const bool operator!=(const Rect& rect) const {
			return (!(*this == rect));
		}
		
		const bool Inside(const signed int x, const signed int y) const {
			return(Inside(Point(x,y)));
		}
		
		const bool Inside(const Point& point) const 
		{
			if( ( point >= topLeftCorner ) && ( point <= bottomRightCorner ) )
				return(true);
			else return(false);
		}

		void move(const Rect startRect, const Rect targetRect);
	private:
		Point topLeftCorner;
		Point bottomRightCorner;
		Size rectSize;
};

#endif // _SDL_RECT_HPP

