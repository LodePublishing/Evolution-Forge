#include "languagemenu.hpp"

LanguageMenu::LanguageMenu(UI_Object* language_parent, Rect language_rect) :
	Menu(language_parent, language_rect/*, 120, MAX_LANGUAGES-1*/)
{
    for(int i=2;i<MAX_LANGUAGES;i++)
    {
		Rect edge = Rect(Point(10 + ((i-2)%4) * 90, ((i-2)/4)*20 + height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3));
        MenuEntry* entry = new MenuEntry(this,
                     		edge, edge, *theme.lookUpString((eString)(SETTING_LANGUAGE_STRING+i))); //TODO maybe already initialize with name string
		entry->setButton(eButton(UNIT_TYPE_5_BUTTON));
   		menuEntries.push_back(entry);
    }
}

LanguageMenu::~LanguageMenu() 
{ }

void LanguageMenu::process()
{
	if(!isShown())
		return;
	Menu::process();
	if(menuLevel)
	{
		int i=0;
		for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		{
			Rect edge = Rect(Point(10 + (i%4) * 90, (i/4)*20 + height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3));
//          if (parent->fitItemToRelativeRect(edge, 1))
   		    {
				(*m)->adjustRelativeRect(edge);
   		    }
            if ((*m)->isLeftClicked())
   		    {
           		pressedItem = i;
                open();
   		        break;
            }
			i++;
		}
		height++;
	}
}
	
void LanguageMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}	

