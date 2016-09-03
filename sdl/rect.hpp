#ifndef _SDL_RECT_HPP
#define _SDL_RECT_HPP

#include "size.hpp"
#include "point.hpp"

enum eRectMovement {
	NO_CHANGE,
	GOT_BIGGER,
	GOT_SMALLER_OR_MOVED
};

class Rect
{
	public:
		Rect();
		Rect(const signed int x, const signed int y, const unsigned int w, const unsigned int h);
		Rect(const Point& pos, const Size& size);
		Rect(const Rect& rect);
	
		const Point& getTopLeft() const;
		const Point getBottomLeft() const;
		void setTopLeft( const Point& point );
		const Point& getBottomRight() const;
		const Point getTopRight() const;
	
		const Size& getSize() const;
		void setSize( const Size& size );
		const unsigned int getWidth() const;
		void setWidth(const unsigned int width);
		const unsigned int getHeight() const;
		void setHeight(const unsigned int height);
		const signed int getLeft() const;
		const signed int getTop() const;
		const signed int getBottom() const;
		const signed int getRight()  const;
		void setLeft(const signed int left);
		void setTop(const signed int top); 
		void setRight(const signed int right);
		void setBottom(const signed int bottom);
				
		const Rect operator+(const Point p) const;
		Rect& operator=(const Rect& rect);
		const bool operator==(const Rect& rect) const;
		const bool operator!=(const Rect& rect) const;
		const bool isInside(const signed int x, const signed int y) const;
		const bool isInside(const Point& point) const;
		const bool isInside(const Rect& rect) const;

		const bool overlaps(const Rect& rect) const;

		const eRectMovement moveSmooth(const Rect& start_rect, const Rect& target_rect);
		const eRectMovement move(const Rect& target_rect);
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
{}

	
inline Rect::Rect(const Point& pos, const Size& size) :
	topLeftCorner(pos),
	bottomRightCorner(pos + size),
	rectSize(size)
{ }

inline const Rect Rect::operator+(const Point p) const {
	return(Rect(topLeftCorner + p, rectSize));
}

inline const Point& Rect::getTopLeft() const { 
	return topLeftCorner; 
}

inline const Size& Rect::getSize() const { 
	return rectSize; 
}
inline const Point& Rect::getBottomRight() const {
	return bottomRightCorner;
}

inline Rect::Rect(const Rect& rect) : 
	topLeftCorner(rect.getTopLeft()), 
	bottomRightCorner(rect.getBottomRight()),
	rectSize(rect.getSize()) 
{ }
	

inline const Point Rect::getTopRight() const {
	return(Point(bottomRightCorner.x, topLeftCorner.y));
}

inline const Point Rect::getBottomLeft() const {
	return(Point(topLeftCorner.x, bottomRightCorner.y));
}

inline void Rect::setTopLeft( const Point& point ) { 
	topLeftCorner = point; 
	bottomRightCorner = point + rectSize;
}
		

inline void Rect::setSize( const Size& size ) {
	rectSize=size;
	bottomRightCorner = topLeftCorner + size;
}
	
inline const unsigned int Rect::getWidth() const { 
	return rectSize.getWidth();
}
		
inline void Rect::setWidth(const unsigned int width) {
	rectSize.setWidth(width);
	bottomRightCorner.x = topLeftCorner.x + (signed int)width;
}
		
inline const unsigned int Rect::getHeight() const { 
	return rectSize.getHeight(); 
}
		
inline void Rect::setHeight(const unsigned int height) { 
	rectSize.setHeight(height);
	bottomRightCorner.y = topLeftCorner.y + (signed int)height;
}
		
inline const signed int Rect::getLeft() const {
	return topLeftCorner.x;
}
		
inline const signed int Rect::getTop() const {
	return topLeftCorner.y;
}
		
inline const signed int Rect::getBottom() const { 
	return bottomRightCorner.y;
}
		
inline const signed int Rect::getRight()  const { 
	return bottomRightCorner.x;
}

inline void Rect::setLeft(const signed int left) { 
	topLeftCorner.x = left; 
	bottomRightCorner.x = left + rectSize.getWidth();
}
		
inline void Rect::setTop(const signed int top) { 
	topLeftCorner.y = top; 
	bottomRightCorner.y = top + rectSize.getHeight();
}
		
inline void Rect::setBottom(const signed int bottom) 
{
	bottomRightCorner.y = bottom;
	setHeight(bottom - getTop());
}
				
inline Rect& Rect::operator=(const Rect& rect) 
{	
	topLeftCorner = rect.getTopLeft();
	bottomRightCorner = rect.getBottomRight();
	rectSize = rect.getSize();
	return(*this);
}
	
inline const bool Rect::overlaps(const Rect& rect) const {
	return(isInside(rect.getTopLeft()) || isInside(rect.getBottomLeft()) || isInside(rect.getBottomRight()) || isInside(rect.getTopRight()));	
}

inline const bool Rect::isInside(const Rect& rect) const {
	return( ( rect.getTopLeft() >= topLeftCorner ) && ( rect.getBottomRight() <= bottomRightCorner ) );
}

inline const bool Rect::operator==(const Rect& rect) const 
{ 
	if( ( getTopLeft() != rect.getTopLeft() ) || ( getSize() != rect.getSize() ) ) 
		return false;
	else return true; 
}

inline const bool Rect::operator!=(const Rect& rect) const {
	return (!(*this == rect));
}

inline const bool Rect::isInside(const Point& point) const 
{
	if( ( point >= topLeftCorner ) && ( point <= bottomRightCorner ) )
		return(true);
	else return(false);
}

inline const bool Rect::isInside(const signed int x, const signed int y) const {
	return(isInside(Point(x,y)));
}

inline void Rect::setRight(const signed int right) 
{
	setWidth(right - getLeft());
	bottomRightCorner.x = right;
}


#endif // _SDL_RECT_HPP

