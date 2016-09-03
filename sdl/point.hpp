#ifndef _SDL_POINT_HPP
#define _SDL_POINT_HPP

#include "size.hpp"

class Point
{
	public:
		Point();
		Point(const signed int xx, const signed int yy);
		Point& operator=(const Point& point);
		const bool operator==(const Point& point) const;
		const bool operator!=(const Point& point) const;
		const Point operator+(const Point& point) const;
		const Point operator*(const signed int scalar) const;
		const Point operator+(const Size& size) const;
		const Point operator-(const Size& size) const;
		const Size operator-(const Point& point) const;
		Point& operator+=(const Size& size);
		Point& operator-=(const Size& size);
		const bool operator>=(const Point& point) const;
		const bool operator<=(const Point& point) const;
		const bool operator>(const Point& point) const;
		const bool operator<(const Point& point) const;
		void move(const Point& startPoint, const Point& targetPoint);
	
		static void mv(signed int& x, const signed int sx, const signed int tx);
		static void mv2(signed int& x, const signed int sx, const signed int tx);
		
		signed int x;
		signed int y;
};


inline Point::Point():	x(0), y(0)
{ }
	
inline Point::Point(const signed int xx, const signed int yy) : 
	x(xx), 
	y(yy)
{ }

inline Point& Point::operator=(const Point& point)
{
	x = point.x;
	y = point.y;
	return(*this);
}

inline const bool Point::operator==(const Point& point) const {
	return ((x == point.x) && (y == point.y));
}

inline const bool Point::operator!=(const Point& point) const {
	return !(*this == point);
}

inline const Point Point::operator*(const signed int scalar) const	{
	return Point(x * scalar, y * scalar );
}

inline const Point Point::operator+(const Point& point) const	{
	return Point(x + point.x, y + point.y );
}

inline const Point Point::operator+(const Size& size) const	{
	return Point(x + size.GetWidth(), y + size.GetHeight() );
}

inline const Point Point::operator-(const Size& size) const {
	return Point(x - size.GetWidth(), y - size.GetHeight() );
}
	
inline const Size Point::operator-(const Point& point) const
{
/*#ifdef _SCC_DEBUG
	if(((x > point.x)&&(y < point.y))||((x < point.x)&&(y > point.y)))
		toLog("POINT: wrong subtraction!");
#endif*/
	return Size(x > point.x?x - point.x:point.x - x, y > point.y?y - point.y:point.y - y);
	// TODO ERROR wenn nicht beide > bzw beide <
}

inline Point& Point::operator+=(const Size& size) 
{
	x += size.GetWidth();
	y += size.GetHeight();
	return *this;
}
		
inline Point& Point::operator-=(const Size& size)
{
	x -= size.GetWidth();
	y -= size.GetHeight();
	return *this;
}

inline const bool Point::operator>=(const Point& point) const
{
	if((x >= point.x) && (y >= point.y))
		return(true);
	else return(false);
}

inline const bool Point::operator<=(const Point& point) const
{
	if((x <= point.x) && (y <= point.y))
		return(true);
	else return(false);
}
	
inline const bool Point::operator>(const Point& point) const
{
	if((x > point.x) && (y > point.y))
		return(true);
	else return(false);
}
	
inline const bool Point::operator<(const Point& point) const
{
	if((x < point.x) && (y < point.y))
		return(true);
	else return(false);
}	

inline void Point::move(const Point& startPoint, const Point& targetPoint)
{
	mv(x, startPoint.x, targetPoint.x);
	mv(y, startPoint.y, targetPoint.y);
}	

#endif // _SDL_POINT_HPP
