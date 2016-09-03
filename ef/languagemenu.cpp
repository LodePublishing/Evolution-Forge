#include "languagemenu.hpp"

LanguageMenu::LanguageMenu(UI_Object* language_parent, const Rect language_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(language_parent, language_rect, distance_bottom_right, position_mode, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, 2, SETTING_ENGLISH_LANGUAGE_STRING, UNIT_TYPE_5_BUTTON)
{}

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

