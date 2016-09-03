#include "bitdepthmenu.hpp"

BitDepthMenu::BitDepthMenu(UI_Object* bit_depth_menu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(bit_depth_menu_parent, rect, distance_bottom_right, position_mode, true)
{
	for(unsigned int i=DEPTH_8BIT;i<=DEPTH_32BIT;i++)
	{
 		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), (eString)(SETTING_DEPTH_8BIT_STRING+i));
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_7_BUTTON));
   		menuEntries.push_back(entry);
	}
	reloadOriginalSize();
}

void BitDepthMenu::reloadOriginalSize()
{
	updateItemSizes(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH));
	UI_Object::reloadOriginalSize();
	updateItemPositions(TWO_COLOUMNS_MENU);
}
BitDepthMenu::~BitDepthMenu() 
{ }

void BitDepthMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}
	
void BitDepthMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}	

