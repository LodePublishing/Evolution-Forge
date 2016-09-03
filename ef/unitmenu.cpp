#include "unitmenu.hpp"
#include "configuration.hpp"

const Rect calculateRect(const unsigned int width, const unsigned int height)
{
	return(Rect(Point((width/2-10)*(height%2), (1+height/2)*(FONT_SIZE+8)), Size(width/2-15, FONT_SIZE+7)));
}

UnitMenu::UnitMenu(UI_Object* unit_parent, const Rect unit_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(unit_parent, unit_rect, distance_bottom_right, position_mode, false),
	anarace(NULL),
	facilityNumber(1),
	lastRace(TERRA)
{
	for(unsigned int i=0;i<LAST_UNIT;++i) //TODO
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), "");
		menuEntries.push_back(entry);
	}
}

UnitMenu::~UnitMenu()
{ }

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
		(*m)->setOriginalSize(Size(getParent()->getWidth()/2-15, FONT_SIZE+7));

	processMenu();
	UI_Menu::reloadOriginalSize();
}

void UnitMenu::resetData()
{
	facilityNumber=1;
	for(unsigned int i=1;i<LAST_UNIT;++i)
		// cut out those [CS], [NS] temporary facilities
//	if(stats[lastRace][i].unitType != ADD_ON_UNIT_TYPE)
	{
		for(unsigned int j=1;j<LAST_UNIT;++j)
			if((coreConfiguration.isExpansionSet())||(stats[lastRace][i].bwunit==0))
			{
				if((stats[lastRace][j].facility[0] == i)||
				   (stats[lastRace][j].facility[1] == i)||
				   (stats[lastRace][j].facility[2] == i))
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
					if((i >=10 ))// || ((coreConfiguration.isExpansionSet())&&(stats[lastRace][i].bwunit==1)))					
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
	//				if (fitItemToClientRect(edge, 1))
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+i));
						if((i==1)||(i==2))
						{
							int unit_type = 0;
							if(i==1)
								unit_type = SCV;
							else if(i==2)
								unit_type = GAS_SCV;
							std::ostringstream os;
							os << theme.lookUpString((eString)(unit_type+UNIT_TYPE_COUNT*lastRace));
							if(UI_Object::focus == this)
								os << " [" << i << "]";
							(*m)->updateText(os.str());
							os.str("");
							os << "$" << stats[lastRace][unit_type].name << "$#" <<
								"  build time $" << stats[lastRace][unit_type].BT << "$ sec.#";
								if(stats[lastRace][unit_type].minerals>0)
									os << "  $" << stats[lastRace][unit_type].minerals/100 << "$ minerals#";
								if(stats[lastRace][unit_type].gas>0)
									os << "  $" << stats[lastRace][unit_type].gas/100 << "$ gas#";
/*								<< stats[lastRace][unit_type].prerequisite[]
								<< stats[lastRace][unit_type].facility[]
								<< stats[lastRace][unit_type].needSupply
								<< stats[lastRace][unit_type].haveSupply
								<< stats[lastRace][unit_type].facilityType
								<< stats[lastRace][unit_type].create
								<< stats[lastRace][unit_type].bwunit*/
								
							(*m)->updateToolTip(os.str());
						}
						else
						{
							std::ostringstream os;
							os << theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i));
							if(UI_Object::focus == this)
								os << " [" << i << "]";
							(*m)->updateText(os.str());
							os.str("");
							os << "$" << theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i)) << "$#";

							for(unsigned int j = 0; j < LAST_UNIT; ++j)
								if((stats[lastRace][j].unitType == i) &&((coreConfiguration.isExpansionSet())||(stats[lastRace][i].bwunit==0)))
									os << stats[lastRace][j].name << "#";
								
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
					if((i >= facilityNumber)) //|| ((efConfiguration.isExpansionSet())/*&&(stats[lastRace][i].bwunit==1)*/))
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
	//			  if (fitItemToClientRect(edge, 1))
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+stats[lastRace][facility[i]].unitType));
						std::ostringstream os;
						os << UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*lastRace+facility[i])) << "...";
						if(UI_Object::focus == this)
							os << " [" << i << "]";
						(*m)->updateText(os.str());
						os.str("");
						os << "$" << stats[lastRace][facility[i]].name << "$#";

						for(unsigned int j = 0; j < LAST_UNIT; ++j)
							if( ( (stats[lastRace][j].facility[0] == facility[i])||
								(stats[lastRace][j].facility[1] == facility[i])||
								(stats[lastRace][j].facility[2] == facility[i]))									&&((coreConfiguration.isExpansionSet())||(stats[lastRace][i].bwunit==0)))
								os << stats[lastRace][j].name << "#";
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
					if(/*(i >= facilityNumber ) ||*/ (stats[lastRace][i].unitType != (signed int)(menuLevel)) || ((!coreConfiguration.isExpansionSet())&&(stats[lastRace][i].bwunit==1)))
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
					
					(*m)->Show();
					(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+menuLevel));
					std::ostringstream os;
					os << UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*lastRace+i));
					if(UI_Object::focus == this)
						os << " [" << unit_number << "]";
					(*m)->updateText(os.str());
					os.str("");
					os << "$" << stats[lastRace][i].name << "$#" <<
						"  build time $" << stats[lastRace][i].BT << "$ sec.#";
						if(stats[lastRace][i].minerals>0)
							os << "  $" << stats[lastRace][i].minerals/100 << "$ minerals#";
						if(stats[lastRace][i].gas>0)
							os << "  $" << stats[lastRace][i].gas/100 << "$ gas#";
/*							<< stats[lastRace][unit_type].prerequisite[]
							<< stats[lastRace][unit_type].facility[]
							<< stats[lastRace][unit_type].needSupply
							<< stats[lastRace][unit_type].haveSupply
							<< stats[lastRace][unit_type].facilityType
							<< stats[lastRace][unit_type].create
							<< stats[lastRace][unit_type].bwunit*/
								
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
					if((i >= LAST_UNIT  ) || 
						(((stats[lastRace][i].facility[0] != facility[menuLevel-1])&&
				   		(stats[lastRace][i].facility[1] != facility[menuLevel-1])&&
					   (stats[lastRace][i].facility[2] != facility[menuLevel-1]))))
						{
							(*m)->Hide();
							continue;
						}
					Rect edge = calculateRect(getParent()->getWidth(), height);
			//		if (parent->fitItemToRelativeRect(edge, 1)) 
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+stats[lastRace][i].unitType));
						std::ostringstream os;
						os << UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*lastRace+i));
						if(UI_Object::focus == this)
							os << " [" << unit_number << "]";
						(*m)->updateText(os.str());
						os.str("");
						os << "$" << stats[lastRace][i].name << "$#" <<
							"  build time $" << stats[lastRace][i].BT << "$ sec.#";
							if(stats[lastRace][i].minerals>0)
								os << "  $" << stats[lastRace][i].minerals/100 << "$ minerals#";
							if(stats[lastRace][i].gas>0)
								os << "  $" << stats[lastRace][i].gas/100 << "$ gas#";
/*							<< stats[lastRace][unit_type].prerequisite[]
							<< stats[lastRace][unit_type].facility[]
							<< stats[lastRace][unit_type].needSupply
							<< stats[lastRace][unit_type].haveSupply
							<< stats[lastRace][unit_type].facilityType
							<< stats[lastRace][unit_type].create
							<< stats[lastRace][unit_type].bwunit*/
								
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
	{
		height=0;
		processMenu();
	}
	// check for Pressed Units
	eButtonColorsType button_colors_type;
	switch(lastRace)
	{
		case TERRA:button_colors_type=UNIT_TYPE_5_BUTTON;break;
		case PROTOSS:button_colors_type=UNIT_TYPE_7_BUTTON;break;
		case ZERG:button_colors_type=UNIT_TYPE_3_BUTTON;break;
		default:break;
	}
			

// special rules for sub menu...
	if(pressedItem>-1)
	{
		unsigned int i = pressedItem+1;
		pressedItem = -1;
		if(!efConfiguration.isFacilityMode())
		{
			if ((menuLevel == 1) && (i == 1))  //scv
					pressedItem = SCV;
		// TODO: pruefen ob das Goal schon vorhanden ist, wenn ja => hasChanged nicht aufrufen
				else if ((menuLevel == 1) && (i == 2))   //gasscv
					pressedItem = GAS_SCV;
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
			if ((menuLevel == 1) && (i == 1))  //scv
				markedItem = SCV;
// TODO: pruefen ob das Goal schon vorhanden ist, wenn ja => hasChanged nicht aufrufen
			else if ((menuLevel == 1) && (i == 2))   //gasscv
				markedItem = GAS_SCV;
			else if (menuLevel > 1)
				markedItem = i;
		} else
		{
			if(menuLevel > 1)
				markedItem=i;
		}
	}
	height+=3;
}


void UnitMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}

const bool UnitMenu::secondLevel() const
{
	return(menuLevel>1);
}


//Problem: Einerseits absolute Nummerierung aller Units $UNIT_TYPE_COUNT Eintraege, andererseits Darstellung nur eines Teils...
//evtl bei Menu nur die soundsovielten SICHTBAREN Eintrag aktivieren...
