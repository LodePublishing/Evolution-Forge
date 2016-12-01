#ifndef _UI_RECT_HPP
#define _UI_RECT_HPP

#include "size.hpp"
#include "point.hpp"
#include <list>

enum eTouchType 
{
	TOUCHES_NO_TOUCH,
	TOUCHES_IS_COMPLETELY_INSIDE,
	TOUCHES_CROSSES_RIGHT_BORDER,
	TOUCHES_CROSSES_LOWER_BORDER,
	TOUCHES_CROSSES_LEFT_BORDER,
	TOUCHES_CROSSES_UPPER_BORDER,
	TOUCHES_CROSSES_BOTTOM_RIGHT_CORNER,
	TOUCHES_CROSSES_BOTTOM_LEFT_CORNER,
	TOUCHES_CROSSES_TOP_LEFT_CORNER,
	TOUCHES_CROSSES_TOP_RIGHT_CORNER,
	TOUCHES_CROSSES_LEFT_AND_RIGHT_BORDER,
	TOUCHES_CROSSES_TOP_AND_BOTTOM_BORDER,
	TOUCHES_CROSSES_UPPER_AREA,
	TOUCHES_CROSSES_RIGHT_AREA,
	TOUCHES_CROSSES_LOWER_AREA,
	TOUCHES_CROSSES_LEFT_AREA,

	TOUCHES_RIGHT_BORDER,
	TOUCHES_LOWER_BORDER,
	TOUCHES_LEFT_BORDER,
	TOUCHES_UPPER_BORDER
};

class Rect
{
	public:
		Rect();
		Rect(const signed int x, const signed int y, const unsigned int w, const unsigned int h);
		Rect(const Point& pos, const Size& size);
		Rect(const Point& top_left_pos, const Point& bottom_right_pos);
		Rect(const Rect& rect);
		
		const std::string toString() const;
	
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
		const bool isPixelInside(const signed int x, const signed int y) const;
		const bool isPixelInside(const Point& point) const;
		const bool isTopLeftCornerInside(const signed int x, const signed int y) const;
		const bool isTopRightCornerInside(const signed int x, const signed int y) const;
		const bool isBottomRightCornerInside(const signed int x, const signed int y) const;
		const bool isBottomLeftCornerInside(const signed int x, const signed int y) const;
		const bool isTopLeftCornerInside(const Point& point) const;
		const bool isTopRightCornerInside(const Point& point) const;
		const bool isBottomRightCornerInside(const Point& point) const;
		const bool isBottomLeftCornerInside(const Point& point) const;
		const bool isRectInside(const Rect& rect) const;

		const eTouchType touches(const Rect& rect) const;

		static void connectRects(std::list<Rect>& rect);
		void cutFromList(std::list<Rect>& rects) const;
		
		const Rect commonRect(const Rect& rect) const; // Schnittmenge
		const std::list<Rect> connectRect(const Rect& rect, bool& change) const; // Vereinigungsmenge
		std::list<Rect> withoutRect(Rect rect) const; // Ohne

		void moveSmooth(const Rect& start_rect, const Rect& target_rect);
		void move(const Rect& target_rect);

		static void testCommonRect();
		static void testConnectRect();
		static void testConnectRects();
		static void testWithoutRect();
		static void testCutFromList();
	private:
		Point topLeftCorner;
		Point bottomRightCorner;
		Size rectSize;
};

	
inline Rect::Rect():topLeftCorner(0,0),bottomRightCorner(0,0), rectSize(0,0) 
{ }

inline Rect::Rect(const signed int x, const signed int y, const unsigned int w, const unsigned int h) :
	topLeftCorner(x,y),
	bottomRightCorner(x + w, y + h),
	rectSize(w, h)
{ }

	
inline Rect::Rect(const Point& pos, const Size& size) :
	topLeftCorner(pos),
	bottomRightCorner(pos + size),
	rectSize(size)
{ }

inline Rect::Rect(const Point& top_left_pos, const Point& bottom_right_pos) :
	topLeftCorner(top_left_pos),
	bottomRightCorner(bottom_right_pos),
	rectSize(bottom_right_pos - top_left_pos)
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
{}
	

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
	rectSize = size;
	bottomRightCorner = topLeftCorner + size;
}
	
inline const unsigned int Rect::getWidth() const { 
	return rectSize.getWidth();
}
inline const unsigned int Rect::getHeight() const { 
	return rectSize.getHeight(); 
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

inline void Rect::setWidth(const unsigned int width) {
	rectSize.setWidth(width);
	bottomRightCorner.x = topLeftCorner.x + (signed int)width;
}
		
inline void Rect::setHeight(const unsigned int height) { 
	rectSize.setHeight(height);
	bottomRightCorner.y = topLeftCorner.y + (signed int)height;
}

inline void Rect::setLeft(const signed int left) { 
	topLeftCorner.x = left; 
	bottomRightCorner.x = left + rectSize.getWidth();
}

inline void Rect::setRight(const signed int right) {
	rectSize.setWidth(right - getLeft());
	bottomRightCorner.x = right;
}

inline void Rect::setTop(const signed int top) { 
	topLeftCorner.y = top; 
	bottomRightCorner.y = top + rectSize.getHeight();

}
		
inline void Rect::setBottom(const signed int bottom) {
	rectSize.setHeight(bottom - getTop());
	bottomRightCorner.y = bottom;
}
				
inline Rect& Rect::operator=(const Rect& rect) 
{	
	topLeftCorner = rect.getTopLeft();
	bottomRightCorner = rect.getBottomRight();
	rectSize = rect.getSize();
	return(*this);
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

inline const bool Rect::isRectInside(const Rect& rect) const {
	return( ( rect.getTopLeft() >= topLeftCorner ) && ( rect.getBottomRight() <= bottomRightCorner ) );
}

inline const bool Rect::isPixelInside(const Point& point) const {
	if( ( point >= topLeftCorner ) && ( point < bottomRightCorner ) )
		return(true);
	else return(false);
}
inline const bool Rect::isTopLeftCornerInside(const Point& point) const {
	if( ( point >= topLeftCorner ) && (point < bottomRightCorner))
		return(true);
	else return(false);
}
inline const bool Rect::isTopRightCornerInside(const Point& point) const {
	if( ( point.x > topLeftCorner.x ) && ( point.y < bottomRightCorner.y ) && (point.y >= topLeftCorner.y) && (point.x <= bottomRightCorner.x))
		return(true);
	else return(false);
}
inline const bool Rect::isBottomRightCornerInside(const Point& point) const {
	if( ( point > topLeftCorner ) && ( point <= bottomRightCorner ) )
		return(true);
	else return(false);
}
inline const bool Rect::isBottomLeftCornerInside(const Point& point) const {
	if( ( point.x >= topLeftCorner.x ) && ( point.y > topLeftCorner.y) && ( point.x < bottomRightCorner.x ) && (point.y <= bottomRightCorner.y ))
		return(true);
	else return(false);
}

inline const bool Rect::isTopLeftCornerInside(const signed int x, const signed int y) const {
	return isTopLeftCornerInside(Point(x, y));
}
inline const bool Rect::isTopRightCornerInside(const signed int x, const signed int y) const {
	return isTopRightCornerInside(Point(x, y));
}
inline const bool Rect::isBottomRightCornerInside(const signed int x, const signed int y) const {
	return isBottomRightCornerInside(Point(x, y));
}
inline const bool Rect::isBottomLeftCornerInside(const signed int x, const signed int y) const {
	return isBottomLeftCornerInside(Point(x, y));
}
inline const bool Rect::isPixelInside(const signed int x, const signed int y) const {
	return(isPixelInside(Point(x,y)));
}


#endif // _UI_RECT_HPP

