#include "radio.hpp"

UI_Radio::UI_Radio(UI_Object* radio_parent, 
		Rect initial_rect, 
		Size bottom_right_distance, 
		const eGroupType group_type, 
		const bool draw_background, 
		const ePositionMode position_mode, 
		const eString radio_title) :
	UI_Group(radio_parent, initial_rect, bottom_right_distance, group_type, draw_background, position_mode, radio_title),
	markedItem(-1),
	reorder(false),
	buttonWasPressed(NULL)
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
                toErrorLog("DEBUG: (UI_Radio::addButton): Variable button not initialized.");return;
        }
#endif
	button->setParent(this);
	button->radio = this;
	buttonId[id] = button;

	calculateBoxSize();
	reorder=true;
}

void UI_Radio::removeButton(const unsigned int button_id)
{
#ifdef _SCC_DEBUG
	if((button_id>=50)||(buttonId[button_id]==NULL)) {
                toErrorLog("DEBUG: (UI_Radio::removeButton): Value button_id out of range.");return;
        }
#endif
	buttonId[button_id]->setParent(NULL);
	buttonId[button_id]->radio = NULL;
	buttonId[button_id] = NULL;

	calculateBoxSize();
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

void UI_Radio::draw() const
{
	UI_Group::draw();
}

void UI_Radio::forceUnpressAll()
{
	if(!getChildren())
		return;

	for(unsigned int i = 0; i < 50; i++)
		if(buttonId[i])
			buttonId[i]->forceUnpress();
}


void UI_Radio::process()
{
// TODO
	UI_Button* lastPressed = NULL;
	UI_Button* tmp;
	markedItem = -1;
	if(buttonWasPressed != NULL)
	{
		for(unsigned int i = 0; i < 50; i++)
			if(buttonId[i])
			{
				if(buttonWasPressed!=buttonId[i])
					buttonId[i]->forceUnpress();
				else markedItem = i;
			}
		buttonWasPressed = NULL;
	}

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
			lastPressed->forcePress();
	}
	if(reorder)
	{
		reorder=false;
//		calculateBoxSize(); // TODO
/*		resetMinXY();
		for(unsigned int i = 0; i < 50; i++)
			if(buttonId[i])
				buttonId[i]->adjustPositionAndSize(ADJUST_ONLY_POSITION);*/
	}
}

