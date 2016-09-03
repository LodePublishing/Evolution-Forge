#ifndef _SDL_POINT_HPP
#define _SDL_POINT_HPP

#include "size.hpp"

class Point
{
	public:
	    Point():
			x(0),
			y(0)
		{ }
	
		Point(const signed int xx, const signed int yy) : 
			x(xx), 
			y(yy)
		{ }

		Point& operator=(const Point& point)
		{
		    x = point.x;
		    y = point.y;
		    return(*this);
		}

		const bool operator==(const Point& point) const {
		    return ((x == point.x) && (y == point.y));
		}

		const bool operator!=(const Point& point) const {
		    return !(*this == point);
		}

		const Point operator+(const Point& point) const	{
			return Point(x + point.x, y + point.y );
		}

		const Point operator+(const Size& size) const	{
			return Point(x + size.GetWidth(), y + size.GetHeight() );
		}

		const Point operator-(const Size& size) const {
			return Point(x - size.GetWidth(), y - size.GetHeight() );
		}
			
	
		const Size operator-(const Point& point) const;
		/*{
		
		    return Size(x > point.x?x - point.x:point.x - x, y > point.y?y - point.y:point.y - y);

			// TODO ERROR wenn nicht beide > bzw beide <
		}*/
		
		Point& operator+=(const Size& size) {
		    x += size.GetWidth();
		    y += size.GetHeight();
		    return *this;
		}
		
		Point& operator-=(const Size& size)
		{
		    x -= size.GetWidth();
		    y -= size.GetHeight();
		    return *this;
		}

		const bool operator>=(const Point& point) const
		{
			if((x >= point.x) && (y >= point.y))
				return(true);
			else return(false);
		}

		const bool operator<=(const Point& point) const
		{
			if((x <= point.x) && (y <= point.y))
				return(true);
			else return(false);
		}
	
		const bool operator>(const Point& point) const
		{
			if((x > point.x) && (y > point.y))
				return(true);
			else return(false);
		}
	
		const bool operator<(const Point& point) const
		{
			if((x < point.x) && (y < point.y))
				return(true);
			else return(false);
		}	

		void move(const Point& startPoint, const Point& targetPoint)
		{
			mv(x, startPoint.x, targetPoint.x);
			mv(y, startPoint.y, targetPoint.y);
		}	
	
		static void mv(signed int& x, const signed int sx, const signed int tx)
		{
			signed int z = (signed int)(((x>sx)?(2*(x-sx)<(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2):(2*(x-sx)>(tx-sx)?(double)(x-sx)/1.5:(double)(tx-x)/1.5)));
			if(z==0)
				z=((x>tx)?-1:0)+((x<tx)?1:0);
			x+=z;
		}	
		
	    signed int x;
		signed int y;
};



#endif // _SDL_POINT_HPP
