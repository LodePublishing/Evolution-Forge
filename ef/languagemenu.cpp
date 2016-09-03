#include "languagemenu.hpp"

LanguageMenu::LanguageMenu(UI_Object* language_parent, Rect language_rect) :
	Menu(language_parent, language_rect, false)
{
    for(int i=0;i<2;i++)
    {
		Rect edge = Rect(Point(10 + (i%2) * 75, (i/2)*20 + height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3));
        MenuEntry* entry = new MenuEntry(this, edge, (eString)(SETTING_ENGLISH_LANGUAGE_STRING+i));
		entry->setButton(eButton(UNIT_TYPE_5_BUTTON));
   		menuEntries.push_back(entry);
    }
}

// Anzahl, Position, coloumns, height / width, first string, Button

LanguageMenu::~LanguageMenu() 
{ }

void LanguageMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}
	
void LanguageMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}	

