#include "radio.hpp"

UI_Radio& UI_Radio::operator=(const UI_Radio& object)
{
	((UI_Group)(*this)) = ((UI_Group)object);
	setButtonHasChanged(object.changed);
	reorder = object.reorder;
	for(unsigned int i = 0; i < 50; i++) 
		buttonId[i]=object.buttonId[i];
	return(*this);
}

UI_Radio::UI_Radio(const UI_Radio& object):
	UI_Group((UI_Group)object),
	changed(object.changed),
	reorder(object.reorder)
{
	for(unsigned int i = 0; i < 50; i++) 
		buttonId[i]=object.buttonId[i];
}

UI_Radio::UI_Radio(UI_Object* radio_parent, Rect initial_rect, Size bottom_right_distance, const ePositionMode position_mode, const eString radio_title) :
	UI_Group(radio_parent, initial_rect, bottom_right_distance, position_mode, radio_title),
	changed(true),
	reorder(false)
{ 

	for(unsigned int i = 0; i < 50; i++) 
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

	reorder=true;
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

	reorder=true;
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
	if(!getChildren())
		return;

	for(unsigned int i = 0; i < 50; i++)
		if(buttonId[i])
			buttonId[i]->forceUnpress();
}

void UI_Radio::forcePress(const unsigned int button_id)
{
#ifdef _SCC_DEBUG
        if(!buttonId[button_id]) {
                toLog("DEBUG: (UI_Radio::forcePress): Variable buttonId not initialized.");return;
        }
#endif
	forceUnpressAll();
	buttonId[button_id]->forcePress();
	setButtonHasChanged();
}

const unsigned int UI_Radio::getMarked()
{
	setButtonHasChanged(false);
	for(unsigned int i = 0; i < 50; i++)
		if((buttonId[i])&&(buttonId[i]->isLeftClicked()))
			return(i);
#ifdef _SCC_DEBUG
	toLog("DEBUG: (UI_Radio::getMarked): was changed but no button is pressed!");
#endif
	return(50);
}

void UI_Radio::leftButtonPressed(UI_Button* button) // Unpress all except one
{
	setButtonHasChanged();
	for(unsigned int i = 0; i < 50; i++)
		if((buttonId[i])&&(button!=buttonId[i]))
			buttonId[i]->forceUnpress();
}

void UI_Radio::leftButtonReleased(/*UI_Button* button*/) // allow release? TODO
{
	setButtonHasChanged();
	// TODO?
}


void UI_Radio::process()
{
// TODO
//		return;

	UI_Button* lastPressed = NULL;
	UI_Button* tmp;

	for(unsigned int i = 0; i < 50; i++)
		if((buttonId[i])&&(buttonId[i]->isCurrentlyActivated()))
		{
			lastPressed = buttonId[i];
			break;
		}
	UI_Group::process();
		
	if(lastPressed!=NULL)
	{
		bool allUnpressed=true;
		for(unsigned int i = 0; i < 50; i++)
			if((buttonId[i])&&(buttonId[i]->isCurrentlyActivated()))
			{
				allUnpressed = false;
				break;
			}
		if(allUnpressed)
		{
			lastPressed->forcePress();
			setButtonHasChanged();
		}
	}
	if(reorder)
	{
		reorder=false;
		resetMinXY();

		for(unsigned int i = 0; i < 50; i++)
			if(buttonId[i])
				buttonId[i]->adjustPositionAndSize(ADJUST_ONLY_POSITION);
	}

}

