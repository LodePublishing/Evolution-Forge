#include "bomenu.hpp"
#include "configuration.hpp"

BoMenu::BoMenu(UI_Object* bo_parent, const Rect bo_rect, const Size distance_bottom_right, const ePositionMode position_mode):
	UI_Menu(bo_parent, bo_rect, distance_bottom_right, position_mode, true, ONE_COLOUMN_MENU),
	number(0),
	anarace(NULL),
	lastRace(TERRA),
	lastGoal(NULL)
{ }

BoMenu::~BoMenu()
{ }

void BoMenu::assignAnarace(ANABUILDORDER* bo_anarace)
{
	bool has_changed = false;
	if((anarace==NULL)||(lastRace != bo_anarace->getRace())||(lastGoal != bo_anarace->getGoal()))
		has_changed = true;
	anarace = bo_anarace;
	if(has_changed)
		resetData();
}

void BoMenu::reloadOriginalSize()
{
	resetData();
	UI_Object::reloadOriginalSize(); // => custom widths
}

const bool BoMenu::addKey(unsigned int key, unsigned int mod)
{
	if((key < SDLK_0) || (key > SDLK_9) || (!isOpen()))
		return(false);
	unsigned int i = key - SDLK_0;
	forcePressItem(i);
	return(true);
}

#include <sstream>
void BoMenu::resetData()
{
	if(!anarace)
		return;
	lastRace = anarace->getRace();
	lastGoal = anarace->getGoal();
	unsigned int i=0;
	number = database.getBuildOrderCount(lastRace, lastGoal);
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
		
		BUILD_ORDER* my_bo = database.getBuildOrder(lastRace, lastGoal, i);
		std::ostringstream os;
		os << my_bo->getName();
		if(UI_Object::focus == this)
			os << " [" << i << "]";
		(*m)->updateText(os.str());
		os.str("");
		os << "$" << my_bo->getName() << " ["<< formatTime(my_bo->getTime(), efConfiguration.getGameSpeed()) << "]$#";
		unsigned int entries = 0;
		for(std::list<PROGRAM>::const_iterator j = my_bo->getProgramList().begin();(entries < 20) && (j!=my_bo->getProgramList().end());++j)
		{
			entries++;
			os << stats[lastRace][j->getUnit()].name;
			if(j->getTime()>0)
				os << " [" << formatTime(j->getTime(), efConfiguration.getGameSpeed()) << "]"; 
			os << "#";
		}
		if(entries==20)
		os << "...";
		(*m)->updateToolTip(os.str());
		
		Rect edge = Rect(Point(10, i * (FONT_SIZE + 9)), Size(getParent()->getWidth()-40, FONT_SIZE+7));
		(*m)->adjustRelativeRect(edge);
		++i;
	}
// create new entries
	for(;i<number;++i)
	{
		std::ostringstream os;
		os << database.getBuildOrder(lastRace, lastGoal, i)->getName();
		if(UI_Object::focus == this)
			os << " [" << i << "]";
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(Point(10, i * (FONT_SIZE + 9)), Size(getParent()->getWidth()-40, FONT_SIZE+7)), os.str());
		if(i==0)
			entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_2_BUTTON));
		else
			entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_1_BUTTON));
		menuEntries.push_back(entry);
	}
	updateItemPositions();
}



