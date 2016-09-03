#include "forcemenu.hpp"

ForceMenu::ForceMenu(UI_Object* force_parent, ANARACE* force_anarace, Rect force_rect):
	Menu(force_parent, force_rect), // 120
	anarace(force_anarace)
{
    for(int i=0;i<20;i++) // TODO
    {
        MenuEntry* entry = new MenuEntry(this,
                        Rect(0, 0, 110, FONT_SIZE+4),
                        Rect(0, 0, 110, FONT_SIZE+4), "Error");
        menuEntries.push_back(entry);
    }
}

ForceMenu& ForceMenu::operator=(const ForceMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	anarace = object.anarace;
	return(*this);
}

ForceMenu::ForceMenu(const ForceMenu& object) : 
    Menu((Menu)object), // 120
    anarace(object.anarace)
{ }
	
ForceMenu::~ForceMenu()
{ }

void ForceMenu::process()
{
	if(!isShown())
		return;
	Menu::process();
	if(menuLevel)
	{
		int i=0;
        for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		{
			Rect edge = Rect(Point(10, height*(FONT_SIZE+9)), Size(getParent()->getWidth()+20, FONT_SIZE));
			if(i >= settings.getStartconditionCount( anarace->getPlayerNum() )  )
			{
				(*m)->Hide();
				continue;
			}
			
//			if (parent->fitItemToRelativeRect(edge, 1)) 
			{
				(*m)->Show();
				(*m)->setButton(eButton(UNIT_TYPE_2_BUTTON));
				(*m)->updateText(settings.getStartcondition(anarace->getPlayerNum(), i)->getName());
				(*m)->adjustRelativeRect(edge);
			}
            if ((*m)->isLeftClicked())
            {
                pressedItem = i;
                open();
                break;
            }
			height++;
			i++;
		}
	} else
        for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
                (*m)->Hide();
}

void ForceMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

