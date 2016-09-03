#include "boentry.hpp"

BoEntry::BoEntry(const BoEntry& object) :
	UI_Button((UI_Button)object)
//	fixed(object.fixed)
{ }
	
BoEntry& BoEntry::operator=(const BoEntry& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
//	fixed = object.fixed;
	return(*this);
}

BoEntry::BoEntry(UI_Object* bo_parent, Rect bo_rect, Rect bo_max_rect, const string& bo_unit) :
	UI_Button(bo_parent, bo_rect, bo_max_rect, bo_unit, FORCE_ENTRY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NOTHING)
//	fixed(false)
	// TODO!
{
/*	addUnit = new UI_Button(this, Rect(Point(getWidth()-117,2),Size(8,8)), Rect(Point(0,0),getSize()), ADD_BUTTON, PRESS_BUTTON_MODE);
	subUnit = new UI_Button(this, Rect(Point(getWidth()-107,2),Size(8,8)), Rect(Point(0,0),getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
	cancelUnit = new UI_Button(this, Rect(Point(getWidth()-97,2),Size(8,8)), Rect(Point(0,0),getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);*/
}

BoEntry::~BoEntry()
{
/*	delete(addUnit);
	delete(subUnit);
	delete(cancelUnit);*/
}

void BoEntry::setUnit(const unsigned int bo_unit)
{
	unit = bo_unit;
}

const unsigned int BoEntry::getUnit() const
{
	return(unit);
}

void BoEntry::setIP(const unsigned int bo_ip)
{
	ip = bo_ip;
}

const unsigned int BoEntry::getIP() const
{
	return(ip);
}



void BoEntry::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Button::draw(dc);
//	if(fixed)
//		dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(KEY_BITMAP), getAbsolutePosition() + Size(getWidth()-24, 2));
}

void BoEntry::process()
{
	if(!isShown())
		return;
	if(getTargetHeight()!=12)
		toLog("entry to high");
	UI_Button::process();
//	if(isRightClicked())
//		lock(!fixed);
}

const unsigned int BoEntry::changed()
{
//	if(addUnit->isLeftClicked()) return(1);
  //  if(subUnit->isLeftClicked()) return(2);
//	if(cancelUnit->isLeftClicked()) return(3);
	return(0);
}
/*
void BoEntry::lock(const bool close)
{
//	fixed = close;
}

void BoEntry::unlock(const bool open)
{
	fixed = !open;
}

const bool BoEntry::locked() const
{
	return(fixed);
}*/

