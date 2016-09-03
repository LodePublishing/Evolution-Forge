#include "button.hpp"


UI_Radio& UI_Radio::operator=(const UI_Radio& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	markedItem = object.markedItem;
	changed = object.changed;
	return(*this);
}

UI_Radio::UI_Radio(const UI_Radio& object):
	UI_Object((UI_Object)object),
    markedItem(object.markedItem),
    changed(object.changed)
{ }

UI_Radio::UI_Radio(UI_Object* radio_parent):
	UI_Object(radio_parent),
	markedItem(0),
	changed(false)
{ }

UI_Radio::~UI_Radio()
{ }

void UI_Radio::addButton(UI_Button* button)
{
	if(!button)
		return;
	button->setParent(this);
	button->radio=this;
}

void UI_Radio::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
}

const bool UI_Radio::hasChanged() const
{
	return(changed);
}

void UI_Radio::forceUnpressAll()
{
    UI_Button* tmp=(UI_Button*)getChildren();
    if(!tmp) return;
    do
    {
		tmp->forceUnpress();
        tmp=(UI_Button*)tmp->getNextBrother();
    } while(tmp!=getChildren());
}

void UI_Radio::forcePress(const unsigned int button)
{
    UI_Button* tmp=(UI_Button*)getChildren();
	if(!tmp) 
		return;
    unsigned int i=1;
    do
    {
		if(i==button)
		{
			tmp->forcePress();
			return;
		}
        i++;
        tmp=(UI_Button*)tmp->getNextBrother();
    } while(tmp!=getChildren());
}

const unsigned int UI_Radio::getMarked()
{
	changed=false;
	unsigned int i=markedItem;
	markedItem=0;
	return(i);
	/*
	UI_Button* tmp=(UI_Button*)getChildren();
	changed=false;
	if(!tmp) return(0);
	int i=0;
	do
	{
		if(tmp->isCurrentlyActivated())
			return(i);
		i++;
		tmp=(UI_Button*)tmp->getNextBrother();
	} while(tmp!=getChildren());
	return(0);*/
}



void UI_Radio::leftButtonPressed(UI_Button* button)
{
	changed=true;
	unsigned int i=0;
	UI_Button* tmp = (UI_Button*)getChildren();
	do
	{
		if(tmp!=button)
			tmp->forceUnpress();
		else markedItem=i;
		i++;
		tmp=(UI_Button*)tmp->getNextBrother();
	}
	while(tmp!=(UI_Button*)getChildren());
}

void UI_Radio::leftButtonReleased(UI_Button* button) // allow release?
{
	changed=true;
    unsigned int i=0;
    UI_Button* tmp = (UI_Button*)getChildren();
    do
    {
        if(tmp==button)
        	markedItem=i;
        i++;
        tmp=(UI_Button*)tmp->getNextBrother();
    }
    while(tmp!=(UI_Button*)getChildren());
	
	// allow it. :|
}

void UI_Radio::rightButtonPressed(UI_Button* button)
{
	leftButtonPressed(button);
}

void UI_Radio::rightButtonReleased(UI_Button* button) // allow release?
{
	leftButtonReleased(button);
}



void UI_Radio::process()
{
	UI_Object::process();
}
/*	if(!getChildren()) return;
	changed=false;
	bool old[10];
	for(int k=10;k--;)
		old[k]=false;
    UI_Button* tmp=(UI_Button*)getChildren();
    int i=0;
    do
    {
        if((tmp->isCurrentlyActivated())&&(!tmp->forcedPress))
			old[i]=true;
		else old[i]=false;
        i++;
        tmp=(UI_Button*)tmp->getNextBrother();
    } while(tmp!=(UI_Button*)getChildren());

	UI_Object::process();

    tmp=(UI_Button*)getChildren();
    i=0;
    do
    {
        if(tmp->isCurrentlyActivated()&&(!old[i]))
        {
			changed=true;
			UI_Button* tmp2=(UI_Button*)getChildren();
			int j=0;
			do
			{
				if(j!=i)
					tmp2->forceUnpress();
				j++;
				tmp2=(UI_Button*)tmp2->getNextBrother();
			} while(tmp2!=(UI_Button*)getChildren());
			return;
        }
	  	else if((!tmp->isCurrentlyActivated())&&(old[i]))
		{
			tmp->forcePress();
		}
        i++;
        tmp=(UI_Button*)tmp->getNextBrother();
    } while(tmp!=(UI_Button*)getChildren());
}*/

