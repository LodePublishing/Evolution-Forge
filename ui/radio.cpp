#include "radio.hpp"

UI_Radio& UI_Radio::operator=(const UI_Radio& object)
{
	((UI_Group)(*this)) = ((UI_Group)object);
	setButtonHasChanged(object.changed);
	alwaysOn = object.alwaysOn;
	for(unsigned int i = 50;i--;) 
		buttonId[i]=object.buttonId[i];
	return(*this);
}

UI_Radio::UI_Radio(const UI_Radio& object):
	UI_Group((UI_Group)object),
	changed(object.changed),
	alwaysOn(object.alwaysOn)
{
	for(unsigned int i = 50;i--;) 
		buttonId[i]=object.buttonId[i];
}
#include <sstream>
UI_Radio::UI_Radio(UI_Object* radio_parent, Rect initial_rect, Size bottom_right_distance, const bool always_on, const eString radio_title, const ePositionMode position_mode) :
	UI_Group(radio_parent, initial_rect, bottom_right_distance, radio_title, position_mode),
	changed(false),
	alwaysOn(always_on)
{ 

	for(unsigned int i = 50;i--;) 
		buttonId[i]=NULL;
}

UI_Radio::~UI_Radio()
{ }

void UI_Radio::addButton(UI_Button* button, const unsigned int id)
{
#ifdef _SCC_DEBUG
	if((!button)||(buttonId[id]!=NULL)) {
                toLog("DEBUG: (UI_Radio::addButton): Variable button not initialized.");return;
        }
#endif
	button->setParent(this);
	button->radio = this;
	buttonId[id] = button;
}

void UI_Radio::removeButton(const unsigned int button_id)
{
#ifdef _SCC_DEBUG
	if((button_id>=50)||(buttonId[button_id]==NULL)) {
                toLog("DEBUG: (UI_Radio::removeButton): Value button_id out of range.");return;
        }
#endif
	buttonId[button_id]->setParent(NULL);
	buttonId[button_id]->radio = NULL;
	buttonId[button_id] = NULL;
}

void UI_Radio::updateIds()
{
	for(unsigned int i = 0; i < 50;i++)
	{
		while((i<50)&&(buttonId[i]!=NULL)) 
			i++;
		if(i<50)
		{
			for(int j = i;j<9;j++)
				buttonId[j] = buttonId[j+1];
			buttonId[9] = NULL;
			return;
		}
	}
}

void UI_Radio::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Group::draw(dc);
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

void UI_Radio::forcePress(const unsigned int button_id)
{
#ifdef _SCC_DEBUG
        if(!buttonId[button_id]) {
                toLog("DEBUG: (UI_Radio::forcePress): Variable buttonId not initialized.");return;
        }
#endif
	setButtonHasChanged();
	forceUnpressAll();
	buttonId[button_id]->forcePress();
}

const unsigned int UI_Radio::getMarked()
{
	setButtonHasChanged(false);
	for(unsigned int i = 50;i--;)
		if((buttonId[i])&&(buttonId[i]->isLeftClicked()))
			return(i);
#ifdef _SCC_DEBUG
	toLog("DEBUG: (UI_Radio::getMarked): was changed but no button is pressed!");
#endif
	return(0);
}

void UI_Radio::leftButtonPressed(UI_Button* button) // Unpress all except one
{
	setButtonHasChanged();
	unsigned int i=0;
	UI_Button* tmp = (UI_Button*)getChildren();
	do
	{
		if(tmp!=button)
			tmp->forceUnpress();
		i++;
		tmp=(UI_Button*)tmp->getNextBrother();
	} while(tmp!=(UI_Button*)getChildren());
}

void UI_Radio::leftButtonReleased(UI_Button* button) // allow release? TODO
{
	setButtonHasChanged();
//	UI_Button* tmp = (UI_Button*)getChildren();
//	do
//	{
//		tmp=(UI_Button*)tmp->getNextBrother();
//	} while(tmp!=(UI_Button*)getChildren());
	
	// allow it. :|
}


void UI_Radio::process()
{
// TODO
	if(!getChildren()) 
		return;

	UI_Button* lastPressed = NULL;
	UI_Button* tmp = (UI_Button*)getChildren();
	do
	{
		if(tmp->isCurrentlyActivated())//Pressed())
			lastPressed = tmp;
		tmp=(UI_Button*)tmp->getNextBrother();
	} while(tmp!=(UI_Button*)getChildren());

	UI_Group::process();
		
	if(lastPressed!=NULL)
	{
		bool allUnpressed=true;
		tmp=(UI_Button*)getChildren();
		do
		{
			if(tmp->isCurrentlyActivated())//Pressed())
				allUnpressed=false;
			tmp=(UI_Button*)tmp->getNextBrother();
		} while(tmp!=(UI_Button*)getChildren());
	
		if(allUnpressed)
		{
			lastPressed->forcePress();
			setButtonHasChanged();
		}
	}
/*
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

	UI_Group::process();

	tmp=(UI_Button*)getChildren();
	i=0;
	do
	{
		if(tmp->isCurrentlyActivated()&&(!old[i]))
		{
			setButtonHasChanged();
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
	} while(tmp!=(UI_Button*)getChildren());*/
}

