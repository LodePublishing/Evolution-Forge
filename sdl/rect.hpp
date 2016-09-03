#ifndef _SDL_RECT_HPP
#define _SDL_RECT_HPP

#include "size.hpp"
#include "point.hpp"

class Rect
{
	public:
		Rect();
		Rect(const signed int x, const signed int y, const unsigned int w, const unsigned int h);
		Rect(const Point& pos, const Size& size);
		Rect(const Rect& rect);
	
		const Point& GetTopLeft() const;
		const Point GetBottomLeft() const;
		void SetTopLeft( const Point& point );
		const Point& GetBottomRight() const;
		const Point GetTopRight() const;

/*		void SetBottomRight( const Point& point) {
			rectSize.width += point.x  
			bottomRightCorner = point;
		} TODO */
	
		const Size& GetSize() const;
		void SetSize( const Size& size );
		const unsigned int GetWidth() const;
		void SetWidth(const unsigned int width);
		const unsigned int GetHeight() const;
		void SetHeight(const unsigned int height);
		const signed int GetLeft() const;
		const signed int GetTop() const;
		const signed int GetBottom() const;
		const signed int GetRight()  const;
		void SetLeft(const signed int left);
		void SetTop(const signed int top); 
		void SetRight(const signed int right);
/*		{
		#ifdef _SCC_DEBUG
			if(right < GetLeft()) {
				toLog("ERROR: SetRight was put left of GetLeft!");return;
			}				
		#endif
			SetWidth(right + 1 - GetLeft());
		}*/

		void SetBottom(const signed int bottom);
				
/*		const Rect operator+(const Rect& rect) const {
			return(Rect(p + rect.p, s + rect.s));
		} TODO
		
		Rect& operator+=(const Rect& rect)
		{
			return ( *this = (*this + rect) );
		}*/
	
		Rect& operator=(const Rect& rect);
		const bool operator==(const Rect& rect) const ;
		const bool operator!=(const Rect& rect) const;
		const bool Inside(const signed int x, const signed int y) const;
		const bool Inside(const Point& point) const;

		const bool overlaps(const Rect& rect) const;

		const bool move(const Rect startRect, const Rect targetRect);
	private:
		Point topLeftCorner;
		Point bottomRightCorner;
		Size rectSize;
};


inline Rect::Rect():topLeftCorner(0,0),bottomRightCorner(0,0), rectSize(0,0) 
{ }

inline Rect::Rect(const signed int x, const signed int y, const unsigned int w, const unsigned int h) :
	topLeftCorner(x,y),
	bottomRightCorner(x+w, y+h),
	rectSize(w, h)
{
	if((w > 1280)||(h> 1024))
		toLog("ERROR, width/height too large");
}

	
inline Rect::Rect(const Point& pos, const Size& size) :
	topLeftCorner(pos),
	bottomRightCorner(pos + size),
	rectSize(size)
{ }
		
inline Rect::Rect(const Rect& rect) : 
	topLeftCorner(rect.GetTopLeft()), 
	bottomRightCorner(rect.GetBottomRight()),
	rectSize(rect.GetSize()) 
{ }
	
inline const Point& Rect::GetTopLeft() const { 
	return topLeftCorner; 
}

inline const Point Rect::GetTopRight() const {
	return(Point(bottomRightCorner.x, topLeftCorner.y));
}

inline const Point Rect::GetBottomLeft() const {
	return(Point(topLeftCorner.x, bottomRightCorner.y));
}

inline void Rect::SetTopLeft( const Point& point ) { 
	topLeftCorner = point; 
	bottomRightCorner = point + rectSize;
}
		
inline const Point& Rect::GetBottomRight() const {
	return bottomRightCorner;
}

/*		void SetBottomRight( const Point& point) {
			rectSize.width += point.x  
			bottomRightCorner = point;
		} TODO */
	
inline const Size& Rect::GetSize() const { 
	return rectSize; 
}
		
inline void Rect::SetSize( const Size& size ) {
	rectSize=size;
	bottomRightCorner = topLeftCorner + size;
}
	
inline const unsigned int Rect::GetWidth() const { 
	return rectSize.GetWidth();
}
		
inline void Rect::SetWidth(const unsigned int width) {
	rectSize.SetWidth(width);
	bottomRightCorner.x = topLeftCorner.x + (signed int)width;
}
		
inline const unsigned int Rect::GetHeight() const { 
	return rectSize.GetHeight(); 
}
		
inline void Rect::SetHeight(const unsigned int height) { 
	rectSize.SetHeight(height);
	bottomRightCorner.y = topLeftCorner.y + (signed int)height;
}
		
inline const signed int Rect::GetLeft() const {
	return topLeftCorner.x;
}
		
inline const signed int Rect::GetTop() const {
	return topLeftCorner.y;
}
		
inline const signed int Rect::GetBottom() const { 
	return bottomRightCorner.y;
}
		
inline const signed int Rect::GetRight()  const { 
	return bottomRightCorner.x;
}

inline void Rect::SetLeft(const signed int left) { 
	topLeftCorner.x = left; 
	bottomRightCorner.x = left + rectSize.GetWidth();
}
		
inline void Rect::SetTop(const signed int top) { 
	topLeftCorner.y = top; 
	bottomRightCorner.y = top + rectSize.GetHeight();
}
		
inline void Rect::SetBottom(const signed int bottom) 
{
#ifdef _SCC_DEBUG
	if(bottom < GetTop()) {
		toLog("ERROR: SetBottom was put above of GetTop!");return;
	}
#endif
	bottomRightCorner.y = bottom;
	SetHeight(bottom - GetTop());
}
				
inline Rect& Rect::operator=(const Rect& rect) 
{	
	topLeftCorner = rect.GetTopLeft();
	bottomRightCorner = rect.GetBottomRight();
	rectSize = rect.GetSize();
	return(*this);
}
		
inline const bool Rect::operator==(const Rect& rect) const 
{ 
	if( ( GetTopLeft() != rect.GetTopLeft() ) || ( GetSize() != rect.GetSize() ) ) 
		return false;
	else return true; 
}

inline const bool Rect::operator!=(const Rect& rect) const {
	return (!(*this == rect));
}

inline const bool Rect::Inside(const signed int x, const signed int y) const {
	return(Inside(Point(x,y)));
}
		
inline const bool Rect::Inside(const Point& point) const 
{
	if( ( point >= topLeftCorner ) && ( point <= bottomRightCorner ) )
		return(true);
	else return(false);
}

inline void Rect::SetRight(const signed int right) 
{
#ifdef _SCC_DEBUG
	if(right < GetLeft()) {
		toLog("ERROR: SetRight was put left of GetLeft!");return;
	}				
#endif
	SetWidth(right - GetLeft());
	bottomRightCorner.x = right;
}


#endif // _SDL_RECT_HPP

