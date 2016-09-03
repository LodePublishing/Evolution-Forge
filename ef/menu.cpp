#include "menu.hpp"

const unsigned int Menu::getHeight() const
{
	return(height);
}

Menu::Menu(const Menu& object) :
    UI_Object((UI_Object)object),
	menuEntries(object.menuEntries),
    menuLevel(object.menuLevel),
    pressedItem(object.pressedItem),
    height(object.height),
    p1(object.p1),
    p2(object.p2)
{ }

Menu& Menu::operator=(const Menu& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	menuEntries = object.menuEntries;
	menuLevel = object.menuLevel;
	pressedItem = object.pressedItem;
	height = object.height;
	p1 = object.p1;
	p2 = object.p2;
	return(*this);
}

Menu::Menu(UI_Object* menu_parent, Rect menu_rect):
	UI_Object(menu_parent, menu_rect),
	menuLevel(0),
	pressedItem(-1),
	height(0),
	p1(),
	p2()
{ }

Menu::~Menu()
{
    for(list<MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		delete (*m);	
}

const bool Menu::isOpen() const
{
	if(menuLevel==0)
		return(false);
	else return(true);
}

const signed int Menu::getPressedItem() const
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
	UI_Object::process();
	pressedItem = -1;
	height = 1;
	if(!isShown())
		return;
	p1=Point(9999,9999);
	p2=Point(0,0);

	for(list<MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(!(*m)->isShown())
		{
			(*m)->setPosition(Point(0,0));
			(*m)->targetRect=(*m)->getRelativeRect();
			(*m)->startRect=(*m)->getRelativeRect();
			(*m)->adjustButtonPlacementArea();
			(*m)->frameReset();
		}
		else
		{
			if((*m)->getAbsolutePosition().x < p1.x)
				p1.x = (*m)->getAbsolutePosition().x;
			if((*m)->getAbsolutePosition().y < p1.y)
				p1.y = (*m)->getAbsolutePosition().y;
			if((*m)->getAbsolutePosition().x + ((signed int)((*m)->getWidth())) > p2.x)
				p2.x = (*m)->getAbsolutePosition().x + ((signed int)((*m)->getWidth()));
			if((*m)->getAbsolutePosition().y + ((signed int)((*m)->getHeight())) > p2.y)
				p2.y = (*m)->getAbsolutePosition().y + ((signed int)((*m)->getHeight()));
		}
	}
	height+=2;
}

void Menu::draw(DC* dc) const
{
	if((!isShown())||(p1 > p2))
		return;
	UI_Object::draw(dc);
	Rect edge = Rect(p1 - Size(3,3), Size(p2.x-p1.x+6, p2.y-p1.y+6) );
	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->DrawRoundedRectangle(edge,4);
}

