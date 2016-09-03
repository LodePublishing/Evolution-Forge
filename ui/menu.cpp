#include "menu.hpp"

UI_Menu::UI_Menu(UI_Object* menu_parent, 
		const Rect& rect, 
		const Size distance_bottom_right, 
		const ePositionMode position_mode, 
		const bool choose_menu, 
		const eMenuType menu_type, 
		const eButtonWidthType button_width_type, 
		const unsigned int entryNumber, 
		const eString firstString, 
		const eButtonColorsType button_colors_type) :
	UI_Object(menu_parent, rect, distance_bottom_right, position_mode),
	menuEntries(),
	menuLevel(0),
	pressedItem(-1),
	markedItem(-1),
	height(0),
	chooseMenu(choose_menu),
	p1(),
	p2(),
	menuChanged(false),
	buttonWidthType(button_width_type),
	menuType(menu_type)
{
	if(entryNumber > 0)
	{
		for(unsigned int i=0;i<entryNumber;++i)
		{
			UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), (eString)(firstString+i));
			entry->setButtonColorsType(button_colors_type);
			menuEntries.push_back(entry);
		}
		updateItemSizes(UI_Object::theme.lookUpButtonWidth(buttonWidthType));
		updateItemPositions();
	}
	Hide();
}

UI_Menu::~UI_Menu()
{
	for(std::list<UI_MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		delete (*m);	
}

void UI_Menu::reloadOriginalSize()
{
	if(buttonWidthType != MAX_BUTTON_WIDTH_TYPES)
		updateItemSizes(UI_Object::theme.lookUpButtonWidth(buttonWidthType));
	updateItemPositions();
	UI_Object::reloadOriginalSize();	 //?
}

void UI_Menu::updateItemSizes(const unsigned int width)
{
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		(*m)->setOriginalSize(Size(width, 0));
}

void UI_Menu::updateItemPositions()
{
	unsigned int i = 0;
	height = 0;
//	Size s;
//	if(positionMode == TOTAL_CENTERED) // TODO
//		s = 
		
	switch(menuType)
	{
		case CUSTOM_MENU:break;
		case ONE_COLOUMN_MENU:
		for(std::list<UI_MenuEntry*>::iterator m = menuEntries.begin(); m != menuEntries.end(); ++m)
			if((*m)->isShown())
			{
				unsigned int item_height;
				if((*m)->isBitmapButton())
					item_height = (*m)->getHeight();
				else item_height = (*m)->getTextHeight();
				(*m)->setOriginalPosition(Point(0, i*(item_height+(*m)->getDistanceBottomRight().getHeight())) + Size(3,3));
				i++;
				height++;
			}
		break;			 
		case TWO_COLOUMNS_MENU:
		for(std::list<UI_MenuEntry*>::iterator m = menuEntries.begin(); m != menuEntries.end(); ++m)
			if((*m)->isShown())
			{
				unsigned int item_height;
				if((*m)->isBitmapButton())
					item_height = (*m)->getHeight();
				else item_height = (*m)->getTextHeight();
				(*m)->setOriginalPosition(Point((i%2) * ((*m)->getWidth() + (*m)->getDistanceBottomRight().getWidth()), (i/2)*(item_height+(*m)->getDistanceBottomRight().getHeight())) + Size(3,3));
				i++;
				if(i%2==0) height++;
			}
		break;
		case HORIZONTAL_MENU:
		for(std::list<UI_MenuEntry*>::iterator m = menuEntries.begin(); m != menuEntries.end(); ++m)
			if((*m)->isShown())
			{
				(*m)->setOriginalPosition(Point(i * ((*m)->getWidth() + (*m)->getDistanceBottomRight().getWidth()), 0)  + Size(3,3));
				i++;
			}
		height = 1;
		break;	
		default:break;
	}

	unsigned int maxWidth = 0;
	unsigned int maxHeight = 0;
	UI_Object* tmp = getChildren();
	if(tmp==NULL)
		return;
	do
	{
		if(tmp->isShown())
		{
			Rect r = tmp->getTargetRect();
			if((r.getRight() > 0) && (maxWidth < (unsigned int)(r.getRight())))
				maxWidth = r.getRight();
			if((r.getBottom() > 0) && (maxHeight < (unsigned int)(r.getBottom())))
				maxHeight = r.getBottom();
		}
		tmp = tmp->getNextBrother();
	}
	while(tmp!=getChildren());
	Size s = Size(maxWidth+7, maxHeight+7);
//	setOriginalSize(s);
	adjustRelativeRect(Rect(getTargetPosition(), s));

}

const bool UI_Menu::isOpen() const
{
	if(menuLevel==0)
		return(false);
	else return(true);
}

const signed int UI_Menu::getPressedItem()
{
	signed int p = pressedItem;
	pressedItem = -1;
	return(p);
}

const unsigned int UI_Menu::getHeight() const
{
	if(!isOpen())
		return(0);
	else
		return(height);
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
	makePufferInvalid();
}

void UI_Menu::process()
{
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

void UI_Menu::draw() const
{
	return; // TODO
	if(p1 > p2)
		return;
	Rect edge = Rect(p1 - Size(3,3), Size(p2.x-p1.x, p2.y-p1.y) + Size(6,6) );
	dc->setBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
//	dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->DrawRoundedRectangle(edge,4);
	UI_Object::draw();
}


void UI_Menu::forcePressItem(const unsigned int number)
{
	if(number > menuEntries.size())
		return;
	pressedItem = number;
	if(chooseMenu)
		close();
/*	unsigned int n = 0;
	for(std::list<UI_MenuEntry*>::const_iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		if((*m)->isShown())
		{
			if(n == number)
			{
				(*m)->forcePress();
				return;
			}
			n++;
		}*/
}


