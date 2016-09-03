#include "bomenu.hpp"

BoMenu::BoMenu(const BoMenu& object) :
	UI_Menu((UI_Menu)object),
	number(object.number),
	anarace(object.anarace)
{ }

BoMenu& BoMenu::operator=(const BoMenu& object)
{
	((UI_Menu)(*this)) = ((UI_Menu)object);
	number = object.number;
	anarace = object.anarace;
	return(*this);
}

BoMenu::BoMenu(UI_Object* bo_parent, const Rect bo_rect, const Size distance_bottom_right, const ePositionMode position_mode):
	UI_Menu(bo_parent, bo_rect, distance_bottom_right, position_mode, true),
	number(0),
	anarace(NULL)
{ }

void BoMenu::assignAnarace(ANABUILDORDER* bo_anarace)
{
	bool race_has_changed = false;
	if((anarace==NULL)||(anarace->getRace() != bo_anarace->getRace()))
		race_has_changed = true;
	anarace = bo_anarace;
	if(race_has_changed)
		resetData();
}

void BoMenu::reloadOriginalSize()
{
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		(*m)->setOriginalSize(Size(getParent()->getWidth()-40, FONT_SIZE+7));
	resetData();
	UI_Menu::reloadOriginalSize();
}
#include <sstream>
void BoMenu::resetData()
{
	if(!anarace)
		return;
	unsigned int i=0;
	height=0;
	number = database.getBuildOrderCount(anarace->getRace(), anarace->getGoal());
// check old entries
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(i >= number)
		{
			(*m)->Hide();
			continue;
		}
		(*m)->Show();
		if(i==0)
			(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_2_BUTTON));
		else
			(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_1_BUTTON));
		
		BUILD_ORDER* my_bo = database.getBuildOrder(anarace->getRace(), anarace->getGoal(), i);
		(*m)->updateText(my_bo->getName());

		std::ostringstream os;
		os << "$" << my_bo->getName() << " ["<< formatTime(my_bo->getTime()) << "]$#";
		unsigned int entries = 0;
		for(std::list<PROGRAM>::iterator j = my_bo->getProgramList().begin();(entries < 20) && (j!=my_bo->getProgramList().end());++j)
		{
			entries++;
			os << stats[anarace->getRace()][j->getUnit()].name;
			if(j->getTime()>0)
				os << " [" << formatTime(j->getTime()) << "]"; 
			os << "#";
		}
		if(entries==20)
		os << "...";
		(*m)->updateToolTip(os.str());
		
		Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(getParent()->getWidth()-40, FONT_SIZE+7));
		(*m)->adjustRelativeRect(edge);
		++height;
		++i;
	}
// create new entries
	for(;i<number;++i)
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(getParent()->getWidth()-40, FONT_SIZE+7)), database.getBuildOrder(anarace->getRace(), anarace->getGoal(), i)->getName());
		if(i==0)
			entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_2_BUTTON));
		else
			entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_1_BUTTON));
		menuEntries.push_back(entry);
		++height;
	}
	++height;
}


BoMenu::~BoMenu()
{ }

void BoMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}

void BoMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}

