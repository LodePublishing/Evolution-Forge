#include "unitmenu.hpp"
#include "configuration.hpp"

UnitMenu::UnitMenu(UI_Object* unit_parent, const Rect unit_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(unit_parent, unit_rect, distance_bottom_right, position_mode, false),
	anarace(NULL),
	facilityNumber(1),
	facility(RACE::UNIT_TYPE_COUNT),
	lastRace(0)
{
	for(unsigned int i=0; i<RACE::UNIT_TYPE_COUNT; ++i) //TODO
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), "");
		menuEntries.push_back(entry);
	}
}

UnitMenu::~UnitMenu()
{ }

const Rect calculateRect(const unsigned int width, const unsigned int height)
{
	return(Rect(Point((width/2-8)*(height%2), (height/2)*(FONT_SIZE+8)) + Size(3,3), Size(width/2-8, FONT_SIZE+7)));
}

const bool UnitMenu::addKey(unsigned int key, unsigned int mod)
{
	if((key < SDLK_0) || (key > SDLK_9) || (!isOpen()))
		return(false);
	unsigned int i = key - SDLK_0;
	forcePressItem(i);
	return(true);
}
// TODO hier evtl zwei Ziffern ermoeglichen, 0-9 muessten dann ueber 00-09 eingegeben werden
 
void UnitMenu::reloadOriginalSize()
{
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		(*m)->setOriginalSize(Size(getParent()->getWidth()/2-8, FONT_SIZE+7));

	processMenu();
	UI_Menu::reloadOriginalSize();
}

void UnitMenu::resetData()
{
	facilityNumber=1;
	for(unsigned int i=1; i < RACE::UNIT_TYPE_COUNT; ++i)
		// cut out those [CS], [NS] temporary facilities
//	if(GAME::race[lastRace].stats[i].unitType != ADD_ON_UNIT_TYPE)
	{
		for(unsigned int j=1; j < RACE::UNIT_TYPE_COUNT; ++j)
		{
			if((GAME::race[lastRace].stats[j].facility[0] == i)||
			   (GAME::race[lastRace].stats[j].facility[1] == i)||
			   (GAME::race[lastRace].stats[j].facility[2] == i))
			{
				facility[facilityNumber] = i;
				++facilityNumber;
				break;
			}
		}
	}
}
		
void UnitMenu::assignAnarace(ANABUILDORDER* goal_anarace)
{
	bool race_has_changed = false;
	if((anarace==NULL)||(lastRace != goal_anarace->getRace()))
		race_has_changed = true;
	anarace = goal_anarace;
	if(race_has_changed)
	{
		lastRace = anarace->getRace();
		resetData();
		processMenu();
	}
}

#include <sstream>
void UnitMenu::processMenu()
{
	height=0;
	if(menuLevel)
	{
		if(menuLevel==1)
		{
			unsigned int i=0;
			if(!efConfiguration.isFacilityMode())
			{
				for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					++i;
					if(i >=10 )
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
	//				if (fitItemToClientRect(edge, true))
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+i));
						if((i==1)||(i==2))
						{
							int unit_type = 0;
							if(i==1); // TODO
//								unit_type = SCV;
							else if(i==2); // TODO
//								unit_type = GAS_SCV;
							std::ostringstream os;
							
							os << GAME::lookUpUnitString(lastRace, unit_type);
							if(UI_Object::focus == this)
								os << " [" << i << "]";
							(*m)->updateText(os.str());
							os.str("");
							os << GAME::lookUpUnitString(lastRace, unit_type) << "#" << "  " << UI_Object::theme.lookUpFormattedString(BOWINDOW_BUILD_TIME_STRING, GAME::race[lastRace].stats[unit_type].BT/(efConfiguration.getGameSpeed()*3+6)) << "#";
							for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
								if(GAME::race[lastRace].stats[unit_type].resource[k]>0)
									os << "  $" << GAME::race[lastRace].stats[unit_type].resource[k]/100 << "$ " << GAME::lookUpGameString(GAME::FIRST_RESOURCE_STRING+k) << "#";
							(*m)->updateToolTip(os.str());
						}
						else
						{
							std::ostringstream os;
							os << GAME::lookUpGameString(GAME::FIRST_UNIT_MENU_STRING+i);
							if(UI_Object::focus == this)
								os << " [" << i << "]";
							(*m)->updateText(os.str());
							os.str("");
							os << "$" << GAME::lookUpGameString(GAME::FIRST_UNIT_MENU_STRING+i) << "$#";

							for(unsigned int j = 0; j < RACE::UNIT_TYPE_COUNT; ++j)
								if(GAME::race[lastRace].stats[j].unitType == i) 
									os << GAME::lookUpUnitString(lastRace, j) << "#";
								
							(*m)->updateToolTip(os.str());
						}
						(*m)->adjustRelativeRect(edge);
					}
					++height;
				}
			} else 
			{
				for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					++i;
					if(i >= facilityNumber)
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
	//			  if (fitItemToClientRect(edge, true))
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+GAME::race[lastRace].stats[facility[i]].unitType));
						std::ostringstream os;
						os << GAME::lookUpUnitString(lastRace, facility[i]) << "...";
						if(UI_Object::focus == this)
							os << " [" << i << "]";
						(*m)->updateText(os.str());
						os.str("");
						os << "$" << GAME::lookUpUnitString(lastRace, facility[i]) << "$#";

						for(unsigned int j = 0; j < RACE::UNIT_TYPE_COUNT; ++j)
							if( ( (GAME::race[lastRace].stats[j].facility[0] == facility[i])||
								(GAME::race[lastRace].stats[j].facility[1] == facility[i])||
								(GAME::race[lastRace].stats[j].facility[2] == facility[i])))
								os << GAME::lookUpUnitString(lastRace, j) << "#";
						(*m)->updateToolTip(os.str());

						(*m)->adjustRelativeRect(edge);
					}
					++height;
				}
			}
		} // end 'if menuLevel == 1'
		else if(menuLevel>1)
		{
			unsigned int i =0;
			if(!efConfiguration.isFacilityMode())
			{
				unsigned int unit_number = 1;
				for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					++i;
					if(/*(i >= facilityNumber ) ||*/ (GAME::race[lastRace].stats[i].unitType != (signed int)(menuLevel)))
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
					
					(*m)->Show();
					(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+menuLevel));
					std::ostringstream os;
					os << GAME::lookUpUnitString(lastRace, i);
					if(UI_Object::focus == this)
						os << " [" << unit_number << "]";
					(*m)->updateText(os.str());
					os.str("");
					os << "$" << GAME::lookUpUnitString(lastRace, i) << "$#" <<
						"  build time $" << GAME::race[lastRace].stats[i].BT << "$ sec.#";
					for(unsigned int j = RACE::MAX_RESOURCE_TYPES; j--;)
						if(GAME::race[lastRace].stats[i].resource[j]>0)
							os << "  $" << GAME::race[lastRace].stats[i].resource[j]/100 << "$ " << GAME::lookUpGameString(GAME::FIRST_RESOURCE_STRING+j) << "#";

/*							<< GAME::race[lastRace].stats[unit_type].prerequisite[]
							<< GAME::race[lastRace].stats[unit_type].facility[]
							<< GAME::race[lastRace].stats[unit_type].needSupply
							<< GAME::race[lastRace].stats[unit_type].haveSupply
							<< GAME::race[lastRace].stats[unit_type].facilityType
							<< GAME::race[lastRace].stats[unit_type].create*/
								
					(*m)->updateToolTip(os.str());					
					(*m)->adjustRelativeRect(edge);
					++height;
					++unit_number;
				}
			} else
			{
				unsigned int unit_number = 1;
				for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					++i;
					if((i >= RACE::UNIT_TYPE_COUNT  ) || 
						(((GAME::race[lastRace].stats[i].facility[0] != facility[menuLevel-1])&&
				   		(GAME::race[lastRace].stats[i].facility[1] != facility[menuLevel-1])&&
					   (GAME::race[lastRace].stats[i].facility[2] != facility[menuLevel-1]))))
						{
							(*m)->Hide();
							continue;
						}
					Rect edge = calculateRect(getParent()->getWidth(), height);
			//		if (parent->fitItemToRelativeRect(edge, true)) 
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+GAME::race[lastRace].stats[i].unitType));
						std::ostringstream os;
						os << GAME::lookUpUnitString(lastRace, i);
						if(UI_Object::focus == this)
							os << " [" << unit_number << "]";
						(*m)->updateText(os.str());
						os.str("");
						os << "$" << GAME::lookUpUnitString(lastRace, i) << "$#" <<
							"  build time $" << GAME::race[lastRace].stats[i].BT << "$ sec.#";
						for(unsigned int j = RACE::MAX_RESOURCE_TYPES; j--;)
							if(GAME::race[lastRace].stats[i].resource[j]>0)
								os << "  $" << GAME::race[lastRace].stats[i].resource[j]/100 << "$ " << GAME::lookUpGameString(GAME::FIRST_RESOURCE_STRING+j) << "#";
/*							<< GAME::race[lastRace].stats[unit_type].prerequisite[]
							<< GAME::race[lastRace].stats[unit_type].facility[]
							<< GAME::race[lastRace].stats[unit_type].needSupply
							<< GAME::race[lastRace].stats[unit_type].haveSupply
							<< GAME::race[lastRace].stats[unit_type].facilityType
							<< GAME::race[lastRace].stats[unit_type].create*/
								
							(*m)->updateToolTip(os.str());					
					
						(*m)->adjustRelativeRect(edge);
					}
					++height;
					++unit_number;
				}
			} 
		} // end 'if menuLevel > 1'
	} // end 'if menuLevel'
	else
		for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		{
		//	Rect edge = Rect(0, 0, 10,10);
		//	(*m)->adjustRelativeRect(edge);
			(*m)->Hide();
		}

}

void UnitMenu::process()
{
	if(!isShown())
		return;
	UI_Menu::process();
	if(menuHasChanged());
		processMenu();
	// check for Pressed Units
	eButtonColorsType button_colors_type = UNIT_TYPE_5_BUTTON; // TODO
/*	switch(lastRace)
	{
		case TERRA:button_colors_type=UNIT_TYPE_5_BUTTON;break;
		case PROTOSS:button_colors_type=UNIT_TYPE_7_BUTTON;break;
		case ZERG:button_colors_type=UNIT_TYPE_3_BUTTON;break;
		default:break;
	}*/
			

// special rules for sub menu...
	if(pressedItem>-1)
	{
		unsigned int i = pressedItem+1;
		pressedItem = -1;
		if(!efConfiguration.isFacilityMode())
		{
			if ((menuLevel == 1) && (i == 1));  //scv TODO
//					pressedItem = SCV;
		// TODO: pruefen ob das Goal schon vorhanden ist, wenn ja => hasChanged nicht aufrufen
				else if ((menuLevel == 1) && (i == 2));   //gasscv
//					pressedItem = GAS_SCV; // TODO
				else if (menuLevel == 1)
					setMenuLevel(i);
				else if (menuLevel > 1)
					pressedItem = i;
		} else
		{
			if(menuLevel == 1)
				setMenuLevel(i+1);
			else if(menuLevel > 1) 
				pressedItem=i;
		}
	}
	
	if(markedItem>-1)
	{
		unsigned int i = markedItem;
		markedItem = -1;
		if(!efConfiguration.isFacilityMode())
		{
			if ((menuLevel == 1) && (i == 1));  //scv TODO
//				markedItem = SCV;
// TODO: pruefen ob das Goal schon vorhanden ist, wenn ja => hasChanged nicht aufrufen
			else if ((menuLevel == 1) && (i == 2));   //gasscv TODO
//				markedItem = GAS_SCV;
			else if (menuLevel > 1)
				markedItem = i;
		} else
		{
			if(menuLevel > 1)
				markedItem=i;
		}
	}
}


void UnitMenu::draw() const
{
	UI_Menu::draw();
}

const bool UnitMenu::secondLevel() const
{
	return(menuLevel>1);
}


//Problem: Einerseits absolute Nummerierung aller Units $UNIT_TYPE_COUNT Eintraege, andererseits Darstellung nur eines Teils...
//evtl bei Menu nur die soundsovielten SICHTBAREN Eintrag aktivieren...
