#include "menu.hpp"

const unsigned int Menu::getHeight() const
{
	if(!isOpen())
		return(0);
	else
		return(height);
}

Menu::Menu(const Menu& object) :
	UI_Object((UI_Object)object),
	menuEntries(object.menuEntries),
	menuLevel(object.menuLevel),
	pressedItem(object.pressedItem),
	markedItem(object.markedItem),
	height(object.height),
	chooseMenu(object.chooseMenu),
	p1(object.p1),
	p2(object.p2),
	menuChanged(false)
{ }

Menu& Menu::operator=(const Menu& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	menuEntries = object.menuEntries;
	menuLevel = object.menuLevel;
	pressedItem = object.pressedItem;
	markedItem = object.markedItem;
	height = object.height;
	chooseMenu = object.chooseMenu;
	p1 = object.p1;
	p2 = object.p2;
	setMenuHasChanged(false);
	return(*this);
}

Menu::Menu(UI_Object* menu_parent, Rect menu_rect, const bool choose_menu):
	UI_Object(menu_parent, menu_rect),
	menuEntries(),
	menuLevel(0),
	pressedItem(-1),
	markedItem(-1),
	height(0),
	chooseMenu(choose_menu),
	p1(),
	p2(),
	menuChanged(false)
{ }

Menu::Menu(UI_Object* menu_parent, Rect rect, const unsigned int entryNumber, const unsigned int coloumns, const Size& s, const eString firstString, const eButton button, const bool choose_menu):
	UI_Object(menu_parent, rect),
	menuEntries(),
	menuLevel(0),
	pressedItem(-1),
	markedItem(-1),
	height(0),
	chooseMenu(choose_menu),
	p1(),
	p2(),
	menuChanged(false)
{
	for(unsigned int i=0;i<entryNumber;i++)
	{
		Rect edge = Rect(Point(10 + (i%coloumns) * (s.GetWidth()+10), (i/coloumns)*(s.GetHeight()+12)), s);
		MenuEntry* entry = new MenuEntry(this, edge, (eString)(firstString+i));
		entry->setButton(button);
		menuEntries.push_back(entry);
	}
}

Menu::~Menu()
{
	for(std::list<MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
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
	setMenuHasChanged();

//	setNeedRedrawNotMoved();

	if(menuLevel)
		Show();
	else Hide();
}

void Menu::close()
{
	if(menuLevel!=0)
		setMenuHasChanged();
	menuLevel=0;
	Hide();
}

void Menu::setMenuLevel(const unsigned int menu_level)
{
	if(menuLevel == menu_level)
		return;
	menuLevel = menu_level;
	setMenuHasChanged();
}

void Menu::setMenuHasChanged(const bool has_changed)
{
	if(has_changed == menuChanged)
		return;
	menuChanged = has_changed;
}

const signed int Menu::getMarkedItem() const
{
	return(markedItem);
}

void Menu::process()
{
	if(!isShown())
		return;
	setMenuHasChanged(false);
	
	UI_Object::process();
	pressedItem = -1;
	markedItem = -1;
	p1 = Point(9999, 9999);
	p2 = Point(0, 0);

	unsigned int i = 0;

	for(std::list<MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(!(*m)->isShown())
		{
			(*m)->setPosition(Point(0, 0));
			(*m)->frameReset();
		}
		else
		{
			if((*m)->getAbsoluteLeftBound() < p1.x)
				p1.x = (*m)->getAbsoluteLeftBound();
			if((*m)->getAbsoluteUpperBound() < p1.y)
				p1.y = (*m)->getAbsoluteUpperBound();
			if((*m)->getAbsoluteRightBound() > p2.x)
				p2.x = (*m)->getAbsoluteRightBound();
			if((*m)->getAbsoluteLowerBound() > p2.y)
				p2.y = (*m)->getAbsoluteLowerBound();

			if ((*m)->isLeftClicked())
				pressedItem = i;
			if ((*m)->isCurrentlyHighlighted())
				markedItem = i;
		}
		i++;
	}

	if((pressedItem >= 0)&&(chooseMenu))
		close();
}

void Menu::draw(DC* dc) const
{
	if((!isShown())||(p1 > p2))
		return;
	UI_Object::draw(dc);
	if(!checkForNeedRedraw())
		return;
	Rect edge = Rect(p1 - Size(3,3), Size(p2.x-p1.x+6, p2.y-p1.y+6) );
	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->DrawRoundedRectangle(edge,4);
}

const bool Menu::menuHasChanged() const
{
	return(menuChanged);
}


