#include "point.hpp"
#include "../core/defs.hpp"

const Size Point::operator-(const Point& point) const
		{
			if(((x > point.x)&&(y < point.y))||((x < point.x)&&(y > point.y)))
			{
				toLog("POINT: wrong subtraction!");
			}
			return Size(x > point.x?x - point.x:point.x - x, y > point.y?y - point.y:point.y - y);
			

			// TODO ERROR wenn nicht beide > bzw beide <
		}
