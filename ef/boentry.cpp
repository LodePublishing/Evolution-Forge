#include "boentry.hpp"

BoEntry::BoEntry(const BoEntry& object) :
	UI_Button((UI_Button)object),
	program(),
	number(0)
//	fixed(object.fixed)
{ }
	
BoEntry& BoEntry::operator=(const BoEntry& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
	program = object.program;
//	fixed = object.fixed;
	return(*this);
}

BoEntry::BoEntry(UI_Object* bo_parent, const Point top_left, const Size distance_bottom_right, const eString bo_unit, const PROGRAM& bo_program, const unsigned int my_number) :
	UI_Button(bo_parent, Rect(top_left, Size(0,0)), distance_bottom_right, FORCE_ENTRY_BUTTON, false, PRESS_BUTTON_MODE, bo_unit, DO_NOT_ADJUST, SMALL_BOLD_FONT, NOTHING),
	program(bo_program),
	number(my_number)
//	fixed(false)
	// TODO!
{
/*	addUnit = new UI_Button(this, Rect(Point(getWidth()-117,2),Size(8,8)), Rect(Point(0,0),getSize()), INCREASE_BUTTON, PRESS_BUTTON_MODE);
	subUnit = new UI_Button(this, Rect(Point(getWidth()-107,2),Size(8,8)), Rect(Point(0,0),getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
	cancelUnit = new UI_Button(this, Rect(Point(getWidth()-97,2),Size(8,8)), Rect(Point(0,0),getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);*/
}

BoEntry::~BoEntry()
{
/*	delete(addUnit);
	delete(subUnit);
	delete(cancelUnit);*/
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
//	if(!isShown())
//		return;
	bool different = isMoving();
	UI_Button::process();
	if(different && (!isMoving()))
		UI_Object::theme.playSound(SWISHLOCK_SOUND, (getAbsolutePosition() + getSize()/2).x);
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
