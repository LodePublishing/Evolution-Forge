#include "menu.hpp"

const unsigned int UI_Menu::getHeight() const
{
	if(!isOpen())
		return(0);
	else
		return(height);
}

UI_Menu::UI_Menu(const UI_Menu& object) :
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

UI_Menu& UI_Menu::operator=(const UI_Menu& object)
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

UI_Menu::UI_Menu(UI_Object* menu_parent, const Rect& menu_rect, const Size distance_bottom_right, const ePositionMode position_mode, const bool choose_menu):
	UI_Object(menu_parent, menu_rect, distance_bottom_right, position_mode),
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

UI_Menu::UI_Menu(UI_Object* menu_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const unsigned int entryNumber, const unsigned int coloumns, const Size& s, const eString firstString, const eButtonColorsType button_colors_type, const bool choose_menu):
	UI_Object(menu_parent, rect, distance_bottom_right, position_mode),
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
	for(unsigned int i=0;i<entryNumber;++i)
	{
		Rect edge = Rect(Point(10 + (i%coloumns) * (s.GetWidth()+10), (i/coloumns)*(s.GetHeight()+12)), s);
		UI_MenuEntry* entry = new UI_MenuEntry(this, edge, (eString)(firstString+i));
		entry->setButtonColorsType(button_colors_type);
		menuEntries.push_back(entry);
	}
}

UI_Menu::~UI_Menu()
{
	for(std::list<UI_MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		delete (*m);	
}

void UI_Menu::updateItemSizes(const unsigned int width)
{
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		(*m)->setOriginalSize(Size(width, 0));
}

void UI_Menu::updateItemPositions(const eMenuType menu_type)
{
	unsigned int i = 0;
	switch(menu_type)
	{
		case CUSTOM_MENU:break;
		case ONE_COLOUMN_MENU:
		for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		{
			(*m)->setOriginalPosition(Point(0, i*((*m)->getTextHeight()+(*m)->getDistanceBottomRight().GetHeight())));
			i++;
		}break;			 
		case TWO_COLOUMN_MENU:
		for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		{
			(*m)->setOriginalPosition(Point((i%2) * ((*m)->getWidth() + (*m)->getDistanceBottomRight().GetWidth()), (i/2)*((*m)->getTextHeight()+(*m)->getDistanceBottomRight().GetHeight())));
			i++;
		}break;
		default:break;
	}
}

const bool UI_Menu::isOpen() const
{
	if(menuLevel==0)
		return(false);
	else return(true);
}

const signed int UI_Menu::getPressedItem() const
{
	return(pressedItem);
}

void UI_Menu::open()
{
	if(menuLevel==0)
		menuLevel=1;
	else if(menuLevel==1)
		menuLevel=0;
	else menuLevel=1;
	setMenuHasChanged();


	if(menuLevel)
		Show();
	else Hide();
	if(isShown())
	{
		UI_Object* tmp = children;  
		if (tmp) {
			do {
				((UI_Button*)tmp)->resetGradient();
				tmp = tmp->getNextBrother();
			} while (tmp != children);
		}
	}
}

void UI_Menu::close()
{
	if(menuLevel!=0)
		setMenuHasChanged();
	menuLevel=0;
	Hide();
}

void UI_Menu::setMenuLevel(const unsigned int menu_level)
{
	if(menuLevel == menu_level)
		return;
	menuLevel = menu_level;
	setMenuHasChanged();
}

void UI_Menu::setMenuHasChanged(const bool has_changed)
{
	if(has_changed == menuChanged)
		return;
	menuChanged = has_changed;
	setNeedRedrawNotMoved();
}

const signed int UI_Menu::getMarkedItem() const
{
	return(markedItem);
}

void UI_Menu::process()
{
	pressedItem = -1;
	markedItem = -1;
	if(!isShown())
		return;
	setMenuHasChanged(false);
	
	UI_Object::process();
	
	p1 = Point(9999, 9999);
	p2 = Point(0, 0);

	unsigned int i = 0;

	for(std::list<UI_MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
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
		++i;
	}

	if((pressedItem >= 0)&&(chooseMenu))
		close();
}

void UI_Menu::draw(DC* dc) const
{
	if((!isShown())||(p1 > p2))
		return;
	if(checkForNeedRedraw())
	{
		Rect edge = Rect(p1 - Size(3,3), Size(p2.x-p1.x+6, p2.y-p1.y+6) );
		dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
//		dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		dc->DrawRoundedRectangle(edge,4);
	}
	UI_Object::draw(dc);
}

const bool UI_Menu::menuHasChanged() const
{
	return(menuChanged);
}


