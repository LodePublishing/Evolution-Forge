#include "rect.hpp"
#include <string>
#include <sstream>

const std::string Rect::toString() const
{
	std::ostringstream os;
	os << "(" << getLeft() << " " << getTop() << ") (" << getRight() << " " << getBottom() << ")";
	return(os.str());
}


void Rect::moveSmooth(const Rect& start_rect, const Rect& target_rect)
{
	if(*this == target_rect)
		return; // no action
	if(start_rect == target_rect)
	{
		*this = target_rect;
		return;
	}
	topLeftCorner.move(start_rect.getTopLeft(), target_rect.getTopLeft());
	rectSize.move(start_rect.getSize(), target_rect.getSize());
	bottomRightCorner = topLeftCorner + rectSize;
}

void Rect::move(const Rect& target_rect)
{
	if(*this == target_rect)
		return;
	*this = target_rect;
}
// dieses Rect relativ zum anderen...
const eTouchType Rect::touches(const Rect& rect) const
{
	// ist 'rect' innerhalb dieses Rects?
	if(isRectInside(rect))
		return TOUCHES_IS_COMPLETELY_INSIDE;
	else if(isTopLeftCornerInside(rect.getTopLeft()) && isBottomLeftCornerInside(rect.getBottomLeft()))
		return TOUCHES_CROSSES_RIGHT_BORDER;
	else if(isTopLeftCornerInside(rect.getTopLeft()) && isTopRightCornerInside(rect.getTopRight()))
		return TOUCHES_CROSSES_LOWER_BORDER;
	else if(isTopRightCornerInside(rect.getTopRight()) && isBottomRightCornerInside(rect.getBottomRight()))
		return TOUCHES_CROSSES_LEFT_BORDER;
	else if(isBottomRightCornerInside(rect.getBottomRight()) && isBottomLeftCornerInside(rect.getBottomLeft()))
		return TOUCHES_CROSSES_UPPER_BORDER;
	else if(isTopLeftCornerInside(rect.getTopLeft()))
		return TOUCHES_CROSSES_BOTTOM_RIGHT_CORNER;
	else if(isTopRightCornerInside(rect.getTopRight()))
		return TOUCHES_CROSSES_BOTTOM_LEFT_CORNER;
	else if(isBottomRightCornerInside(rect.getBottomRight()))
		return TOUCHES_CROSSES_TOP_LEFT_CORNER;
	else if(isBottomLeftCornerInside(rect.getBottomLeft()))
		return TOUCHES_CROSSES_TOP_RIGHT_CORNER;
	
	else if((rect.getLeft() < getLeft()) && (rect.getRight() > getRight()) && (rect.getTop() >= getTop()) && (rect.getBottom() <= getBottom())) // ? =
		return TOUCHES_CROSSES_LEFT_AND_RIGHT_BORDER;
	else if((rect.getTop() < getTop()) && (rect.getBottom() > getBottom()) && (rect.getLeft() >= getLeft()) && (rect.getRight() <= getRight()))
		return TOUCHES_CROSSES_TOP_AND_BOTTOM_BORDER;

	else if((rect.getLeft() == getRight()) && (rect.getTop() == getTop()) && (rect.getBottom() == getBottom()))
		return TOUCHES_RIGHT_BORDER;
	else if((rect.getTop() == getBottom()) && (rect.getLeft() == getLeft()) && (rect.getRight() == getRight()))
		return TOUCHES_LOWER_BORDER;
	else if((rect.getRight() == getLeft()) && (rect.getTop() == getTop()) && (rect.getBottom() == getBottom()))
		return TOUCHES_LEFT_BORDER;
	else if((rect.getBottom() == getTop()) && (rect.getLeft() == getLeft()) && (rect.getRight() == getRight()))
		return TOUCHES_UPPER_BORDER;

	else if((rect.isTopLeftCornerInside(getTopLeft())) && (rect.isTopRightCornerInside(getTopRight())) && (!rect.isBottomRightCornerInside(getBottomRight())) && (!rect.isBottomLeftCornerInside(getBottomLeft())))
		return TOUCHES_CROSSES_UPPER_AREA;
	else if((!rect.isTopLeftCornerInside(getTopLeft())) && (rect.isTopRightCornerInside(getTopRight())) && (rect.isBottomRightCornerInside(getBottomRight())) && (!rect.isBottomLeftCornerInside(getBottomLeft())))
		return TOUCHES_CROSSES_RIGHT_AREA;
	else if((!rect.isTopLeftCornerInside(getTopLeft())) && (!rect.isTopRightCornerInside(getTopRight())) && (rect.isBottomRightCornerInside(getBottomRight())) && (rect.isBottomLeftCornerInside(getBottomLeft())))
		return TOUCHES_CROSSES_LOWER_AREA;
	else if((rect.isTopLeftCornerInside(getTopLeft())) && (!rect.isTopRightCornerInside(getTopRight())) && (!rect.isBottomRightCornerInside(getBottomRight())) && (rect.isBottomLeftCornerInside(getBottomLeft())))
		return TOUCHES_CROSSES_LEFT_AREA;
	
	else return TOUCHES_NO_TOUCH;
}

//problem bei touches wenn this ein Rechteck ist, das in das andere Rechteck halb eintaucht
 

// Faelle: 
// - rect komplett in object enthalten
// - object komplett in rect enthalten
// - zwei Objekt Punkte liegen in rect
// - zwei rect Punkte liegen im Objekt
// - ein Punkt liegt in rect (und umgekehrt)
// - Schnitt zweier Kanten

// Schnitt
const Rect Rect::commonRect(const Rect& rect) const
{
	
	eTouchType k = touches(rect);
//	toErrorLog(k);
	switch(k)
	{
		case TOUCHES_NO_TOUCH:
		{
			if(rect.touches(*this) == TOUCHES_IS_COMPLETELY_INSIDE)
				return *this;
			else return Rect();
		}break;				
		case TOUCHES_IS_COMPLETELY_INSIDE:return rect;break;
		case TOUCHES_CROSSES_RIGHT_BORDER:return Rect(rect.getTopLeft(), Point(getRight(), rect.getBottom()));break;
		case TOUCHES_CROSSES_LOWER_BORDER:return Rect(rect.getTopLeft(), Point(rect.getRight(), getBottom()));break;
		case TOUCHES_CROSSES_LEFT_BORDER:return Rect(Point(getLeft(), rect.getTop()), rect.getBottomRight());break;
		case TOUCHES_CROSSES_UPPER_BORDER:return Rect(Point(rect.getLeft(), getTop()), rect.getBottomRight());break;
		case TOUCHES_CROSSES_BOTTOM_RIGHT_CORNER:return Rect(rect.getTopLeft(), getBottomRight());break;
		case TOUCHES_CROSSES_BOTTOM_LEFT_CORNER:return Rect(Point(getLeft(), rect.getTop()), Point(rect.getRight(), getBottom()));break;
		case TOUCHES_CROSSES_TOP_LEFT_CORNER:return Rect(getTopLeft(), rect.getBottomRight());break;
		case TOUCHES_CROSSES_TOP_RIGHT_CORNER:return Rect(Point(rect.getLeft(), getTop()), Point(getRight(), rect.getBottom()));break;
		case TOUCHES_CROSSES_LEFT_AND_RIGHT_BORDER:return Rect(Point(getLeft(), rect.getTop()), Point(getRight(), rect.getBottom()));break;
		case TOUCHES_CROSSES_TOP_AND_BOTTOM_BORDER:return Rect(Point(rect.getLeft(), getTop()), Point(rect.getRight(), getBottom()));break;

		case TOUCHES_CROSSES_UPPER_AREA:return Rect(getTopLeft(), Point(getRight(), rect.getBottom()));break;
		case TOUCHES_CROSSES_RIGHT_AREA:return Rect(Point(rect.getLeft(), getTop()), getBottomRight());break;
		case TOUCHES_CROSSES_LOWER_AREA:return Rect(Point(getLeft(), rect.getTop()), getBottomRight());break;
		case TOUCHES_CROSSES_LEFT_AREA:return Rect(getTopLeft(), Point(rect.getRight(), getBottom()));break;
		default:return Rect();break;
	}
}

void Rect::testCommonRect()
{
	bool test_success = true;
	Rect r1(Point(100, 100), Point(200, 200));
	Rect r2(Point(80, 120), Point(250, 140));
	Rect r3 = r1.commonRect(r2);

	if((r3.getLeft() != 100) || (r3.getRight() != 200) || (r3.getTop() != 120) || (r3.getBottom() != 140))
		test_success = false;
	r2.setRight(100);
	r3 = r1.commonRect(r2);
	if(r3 != Rect())
		test_success = false;
	if(r2.commonRect(r1) != r3)
		test_success = false;
	
// ok...
	r1 = Rect(Point(100, 100), Point(200, 200));
	r2 = Rect(Point(80, 120), Point(150, 180));
	r3 = r1.commonRect(r2);
	if(r3 == Rect())
		test_success = false;
	toErrorLog("//");
	Rect r4 = r2.commonRect(r1);
	if(r4 != r3) // !
		test_success = false;

	toErrorLog(r3.toString());
	toErrorLog(r4.toString());

	toErrorLog("//");

	r2 = Rect(Point(120, 120), Point(180, 121));

	r3 = r1.commonRect(r2);
	if(r3 != r2)
		test_success = false;

	r3 = r2.commonRect(r1);
	if(r3 != r2)
		test_success = false;

	r1 = Rect(Point(87, 132), Point(264, 216));
	r2 = Rect(Point(98, 170), Point(243, 278));

	if((r1.commonRect(r2) != r2.commonRect(r1)))// || (r1.commonRect(r2) != Rect(Point(98, 170), Point(243, 216))))
		test_success=false;
	toErrorLog(r1.commonRect(r2).toString());
	toErrorLog(r2.commonRect(r1).toString());

	if(!test_success)
		toErrorLog("----------> Common Rect Test failed.");
	else toErrorLog("* Common Rect Test success.");
}

// Vereinigung, falls es ein schoenes Rect ergibt
const std::list<Rect> Rect::connectRect(const Rect& rect, bool& change) const
{
	eTouchType k = touches(rect);
//	toErrorLog(k);
	std::list<Rect> t_list;
	change = false;
	if( k != TOUCHES_NO_TOUCH)
	{

		if((k == TOUCHES_CROSSES_LEFT_BORDER) || (k == TOUCHES_LEFT_BORDER))
		{
			if((getTop() == rect.getTop()) && (getBottom() == rect.getBottom()))
				t_list.push_back(Rect(rect.getTopLeft(), getBottomRight()));
		}
		else if((k == TOUCHES_CROSSES_RIGHT_BORDER) || (k == TOUCHES_RIGHT_BORDER))
		{
			if((getTop() == rect.getTop()) && (getBottom() == rect.getBottom()))
				t_list.push_back(Rect(getTopLeft(), rect.getBottomRight()));
		}				
		else if((k == TOUCHES_CROSSES_UPPER_BORDER) || (k == TOUCHES_UPPER_BORDER))
		{
			if((getLeft() == rect.getLeft()) && (getRight() == rect.getRight()))
				t_list.push_back(Rect(rect.getTopLeft(), getBottomRight()));
		}
		else if((k == TOUCHES_CROSSES_LOWER_BORDER) || (k == TOUCHES_LOWER_BORDER))
		{
			if((getLeft() == rect.getLeft()) && (getRight() == rect.getRight()))
				t_list.push_back(Rect(getTopLeft(), rect.getBottomRight()));
		}
		if(t_list.size() == 0)
		{
			std::list<Rect> t_list_1;
			std::list<Rect> t_list_2;
			t_list_1 = withoutRect(rect);
			t_list_2 = rect.withoutRect(*this);
			if((t_list_1.size() == 1)&& // glatter Schnitt
				(t_list_1.front() != *this)) // if there was actually a 'cut'
			{
				t_list.push_back(t_list_1.front());
				t_list.push_back(rect);
				change = true;
			} else
			if((t_list_2.size() == 1)&& // glatter Schnitt
				(t_list_1.front() != rect)) // if there was actually a 'cut'
			{
				t_list.push_back(t_list_2.front());
				t_list.push_back(*this);
				change = true;
			}
		} else change = true;
	}
	return t_list;
}

void Rect::testConnectRect()
{
	bool test_success = true;
	bool change = false;
	std::list<Rect> rl;
	Rect r1(Point(100, 100), Point(200, 200));
	Rect r2(Point(80, 100), Point(100, 200));
	rl = r2.connectRect(r1, change);
	if(!rl.size())
		test_success = false;
	else toErrorLog("1");

	r2.setRight(200);

	rl = r1.connectRect(r2, change);
	if(rl.size()!=1)
		test_success = false;
	else toErrorLog("2");
	
	r2 = Rect(Point(80, 120), Point(100, 140));
	Rect r3(Point(80, 100), Point(100, 120));
	Rect r4(Point(80, 140), Point(100, 200));

	std::list<Rect> rl2;
	std::list<Rect> rl3;
	rl2 = r3.connectRect(r2, change);
	rl3 = r4.connectRect(r2, change);
	toErrorLog("---");
	for(std::list<Rect>::iterator i = rl2.begin(); i != rl2.end(); ++i)
		toErrorLog(i->toString());
	toErrorLog("---");
	for(std::list<Rect>::iterator i = rl3.begin(); i != rl3.end(); ++i)
		toErrorLog(i->toString());
	toErrorLog("---");

	if(!test_success)
		toErrorLog("----------> Connect Rect Test failed.");
	else toErrorLog("* Connect Rect Test success.");
	for(std::list<Rect>::iterator i = rl.begin(); i != rl.end(); ++i)
		toErrorLog(i->toString());


	
}

void Rect::connectRects(std::list<Rect>& rects)
{
	for(std::list<Rect>::iterator i = rects.begin(); i != rects.end(); ++i)
		for(std::list<Rect>::iterator j = rects.begin(); j != rects.end(); )
			if(i == j)
				++j;
			else if(i->isRectInside(*j))
				j = rects.erase(j);
			else if((j->getWidth() == 0) || (j->getHeight() == 0))
				j = rects.erase(j);
			else
			{
				std::list<Rect> t_list;
//				toErrorLog("--IN:");
//				toErrorLog(i->toString());
//				toErrorLog(j->toString());
				bool change = false;
				t_list = i->connectRect(*j, change);
//				toErrorLog("--OUT:");
//				for(std::list<Rect>::iterator k = t_list.begin(); k != t_list.end(); ++k)
//					toErrorLog(k->toString());
//				toErrorLog("---");
				if(change)
				{
//					toErrorLog("change");
					rects.erase(i);
					rects.erase(j);
					rects.splice(rects.begin(), t_list);
					j = rects.begin();
					i = rects.begin();
				} else ++j;
			}
}

void Rect::testConnectRects()
{
	bool test_failed = false;
	Rect r1 = Rect(Point(10, 10), Point(100, 100));
	Rect r2 = Rect(Point(100, 10), Point(200, 100));
	std::list<Rect> rects;
	rects.push_back(r1);
	rects.push_back(r2);
	connectRects(rects);
	if((rects.size() != 1) || (rects.back() != Rect(Point(10, 10), Point(200, 100))))
		test_failed = true;
	else toErrorLog("1");

	r1 = Rect(Point(100, 100), Point(200, 120));
	r2 = Rect(Point(150, 120), Point(200, 200));
	rects.clear();
	rects.push_back(r1);
	rects.push_back(r2);
	toErrorLog("--------");
	connectRects(rects);
	if(rects.size()!=2)
//	if(rects.front() == Rect(Point(100, 100), Point(200, 120)))
		test_failed = true;
	else toErrorLog("2");
	toErrorLog(rects.front().toString());
	toErrorLog(rects.back().toString());
	

//      --IN:
//      //      (17 2) (73 26)
//      //      (8 2) (90 26)
//      //      --OUT:
//      //      (17 2) (73 26)
//      //      (8 2) (90 26)
//      //      ---
//      


	r1 = Rect(Point(17, 2), Point(73, 26));
	r2 = Rect(Point(8, 2), Point(90, 26));
	rects.clear();
	rects.push_back(r1);
	rects.push_back(r2);
	connectRects(rects);

	if((rects.size()!=1)||(rects.front() != r2))
		test_failed = true;
	else toErrorLog("3");
	
	r1 = Rect(Point(1, 1), Point(100, 100));
	rects.clear();
	rects.push_back(r1);
	rects.push_back(r1);
	connectRects(rects);
	
	if((rects.size()!=1)||(rects.front() != r1))
		test_failed = true;
	else toErrorLog("4");
	
	if(test_failed)
		toErrorLog("* ---------> connectRects test failed.");
	else toErrorLog("* Connect Rects Test success.");
}


// ok, laeuft perfekt.
std::list<Rect> Rect::withoutRect(Rect rect) const
{
//	r1 = Rect(Point(87, 132), Point(264, 216));
//	r2 = Rect(Point(98, 170), Point(243, 278));

// Schnittmenge:
	std::list<Rect> rects;
	Rect cut = commonRect(rect);

// Schnitt ist gleich dem Rechteck
	if((cut == *this) || (cut.getWidth() == 0) || (cut.getHeight() == 0))
		rects.push_back(*this);
	else
	{
// Sonst: bis zu 4 rectangles:
	// oben
		if(cut.getTop() > getTop())
//			rects.push_back(Rect(getTopLeft(), Point(getRight(), cut.getTop()-1)));
			rects.push_back(Rect(getTopLeft(), Point(getRight(), cut.getTop())));
	// links
		if(cut.getLeft() > getLeft())
//			rects.push_back(Rect(Point(getLeft(), cut.getTop()), cut.getBottomLeft() - Size(1,0)));
			rects.push_back(Rect(Point(getLeft(), cut.getTop()), cut.getBottomLeft()));
	// rechts
		if(cut.getRight() < getRight())
//			rects.push_back(Rect(cut.getTopRight()+Size(1,0), getBottomRight()));
			rects.push_back(Rect(cut.getTopRight(), getBottomRight()));
	// unten
		if(cut.getBottom() < getBottom())
//			rects.push_back(Rect(Point(getLeft(), cut.getBottom()+1), getBottomRight()));
			rects.push_back(Rect(Point(getLeft(), cut.getBottom()), getBottomRight()));
	}
	return rects;
}

void Rect::testWithoutRect()
{
	toErrorLog("testing without rect");
	bool test_success = true;
	std::list<Rect> rl;
	Rect r1(Point(100, 100), Point(200, 200));
	Rect r2(Point(80, 120), Point(150, 180));
	rl = r2.withoutRect(r1);

	if(rl.front() != Rect(Point(80, 120), Point(100, 180)))
		test_success = false;
	toErrorLog(rl.size());
	toErrorLog(rl.front().toString());

	rl = r1.withoutRect(r2);
	if(rl.size() != 3)
		test_success = false;
	r1 = rl.front();
	r2 = rl.back();
	if(r1.commonRect(r2) != Rect())
		test_success = false;

/*	r2 = Rect(Point(80, 120), Point(100, 140));
	Rect r3(Point(80, 100), Point(100, 120));
	Rect r4(Point(80, 140), Point(100, 200));

	rl.push_back(r1);
	rl.push_back(r2);

	std::list<Rect> rl2;
	std::list<Rect> rl3;
	rl2 = r3.connectRect(r2);
	rl3 = r4.connectRect(r2);
	toErrorLog("---");
	toErrorLog(r1.commonRect(r2).toString());
	toErrorLog("---");
	for(std::list<Rect>::iterator i = rl.begin(); i != rl.end(); ++i)
		toErrorLog(i->toString());
	toErrorLog("---");
	for(std::list<Rect>::iterator i = rl3.begin(); i != rl3.end(); ++i)
		toErrorLog(i->toString());
	toErrorLog("---");

	for(std::list<Rect>::iterator i = rl.begin(); i != rl.end(); ++i)
		toErrorLog(i->toString());

	connectRects(rl);
	if(rl.size()!=1)
		test_success = false;

	r1 = Rect(Point(87, 132), Point(264, 216));
	r2 = Rect(Point(98, 170), Point(243, 278));

	rl = r1.withoutRect(r2);

	if(rl.size()!=3)
		test_success = false;

	r1 = Rect(Point(10, 10), Point(100, 100));
	r2 = Rect(Point(110, 10), Point(200, 100));

	rl = r1.withoutRect(r2);
	if(rl.size()!=1)
		test_success = false;
*/	
//	rl = r2.withoutRect(r1);
//	if(rl.size()!=1)
//		test_success = false;
	
// 284 / 43 - 322 / 63
	
	if(!test_success)
		toErrorLog("----------> Without Rect Test failed.");
	else toErrorLog("* Without Rect Test success.");
	for(std::list<Rect>::iterator i = rl.begin(); i != rl.end(); ++i)
		toErrorLog(i->toString());
}


// ok, laeuft
void Rect::cutFromList(std::list<Rect>& rects) const
{
	for(std::list<Rect>::iterator i = rects.begin(); i != rects.end(); )
	{
		std::list<Rect> t_list = i->withoutRect(*this);
		rects.splice(i, t_list);
		i = rects.erase(i);
	}
	connectRects(rects);
}

void Rect::testCutFromList()
{
	Rect r1(Point(100, 100), Point(200, 200));
	std::list<Rect> rl;
	rl.push_back(Rect(Point(80, 80), Point(120, 120)));
	rl.push_back(Rect(Point(180, 80), Point(220, 120)));
	rl.push_back(Rect(Point(120, 180), Point(180, 220)));
	r1.cutFromList(rl);
	toErrorLog("*+*+*+");
	for(std::list<Rect>::iterator i = rl.begin(); i != rl.end(); ++i)
		toErrorLog(i->toString());
	toErrorLog("*+*+*+");
}

// ok...
