#include "languagemenu.hpp"

LanguageMenu::LanguageMenu(UI_Object* language_parent, const Rect language_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(language_parent, language_rect, distance_bottom_right, position_mode, false)
{
	for(unsigned int i=0;i<2;++i)
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), (eString)(SETTING_ENGLISH_LANGUAGE_STRING+i));
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_5_BUTTON+i));
   		menuEntries.push_back(entry);
	}
	reloadOriginalSize();
}

void LanguageMenu::reloadOriginalSize()
{
	updateItemSizes(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH));
	UI_Object::reloadOriginalSize();
	updateItemPositions(TWO_COLOUMNS_MENU);
}


LanguageMenu::~LanguageMenu() 
{ }

void LanguageMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}
	
void LanguageMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}	

