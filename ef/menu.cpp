#include "menu.hpp"

const int Menu::getHeight() const
{
	return(height);
}

Menu::Menu(UI_Object* parent, ANARACE* anarace, Rect rect):UI_Object(parent, rect)
{
	menuLevel = 0;
	height=0;
	this->anarace = anarace;
	pressedItem=0;

	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		menuEntry[i]=new MenuEntry(this, Rect(0,0,120,FONT_SIZE+5), Rect(0,0,120,FONT_SIZE+5), "NULL");
		menuEntry[i]->Hide();
	}
}

Menu::~Menu()
{
	for(int i=UNIT_TYPE_COUNT;i--;)
		delete menuEntry[i];
}

const bool Menu::isOpen() const
{
	if(menuLevel==0)
		return(false);
	else return(true);
}

const int Menu::getPressedItem() const
{
	return(pressedItem);
}

void Menu::open()
{
	if(menuLevel==0)
		menuLevel=1;
	else if(menuLevel==1)
		menuLevel=0;
	else menuLevel=1;
}

void Menu::close()
{
	menuLevel=0;
}

void Menu::process()
{
	if(!shown)
		return;
	UI_Object::process();
	pressedItem = -1;
	height = 2;
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		if(!menuEntry[i]->isShown())
		{
			menuEntry[i]->setPosition(Point(0,0));
			menuEntry[i]->targetRect=menuEntry[i]->getRelativeRect();
			menuEntry[i]->startRect=menuEntry[i]->getRelativeRect();
			menuEntry[i]->buttonPlacementArea=menuEntry[i]->getRelativeRect();
			menuEntry[i]->frameReset();
		}
	//	else
	//		menuEntry[i]->Hide();
	}
	
	height+=2;
}
void Menu::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Object::draw(dc);
}

