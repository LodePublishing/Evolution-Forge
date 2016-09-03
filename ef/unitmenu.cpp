#include "unitmenu.hpp"
#include "configuration.hpp"

const Rect calculateRect(const unsigned int width, const unsigned int height)
{
	return(Rect(Point((width/2-10)*(height%2), (1+height/2)*(FONT_SIZE+8)), Size(width/2-15, FONT_SIZE+7)));
}

UnitMenu::UnitMenu(UI_Object* unit_parent, const Rect unit_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(unit_parent, unit_rect, distance_bottom_right, position_mode, false),
	anarace(NULL),
	facilityNumber(1)
{
	for(unsigned int i=0;i<UNIT_TYPE_COUNT;++i) //TODO
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, calculateRect(unit_parent->getWidth(), 0), "ERROR"); //TODO maybe already initialize with name string
		menuEntries.push_back(entry);
	}
}

/*UnitMenu::UnitMenu(const UnitMenu& object) :
	UI_Menu((UI_Menu)object),
	anarace(object.anarace),
	facilityNumber(object.facilityNumber);
{
	for(unsigned int i = GAS_SCV+1; i--;)
		facility[i] = object.facility[i];
}

UnitMenu& UnitMenu::operator=(const UnitMenu& object)
{
	((UnitMenu)(*this)) = ((UnitMenu)object);
	anarace = object.anarace;
	facilityNumber = object.facilityNumber;
	for(unsigned int i = GAS_SCV+1; i--;)
		facility[i] = object.facility[i];
	return(*this);
}*/


UnitMenu::~UnitMenu()
{ }

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
	for(unsigned int i=1;i<=GAS_SCV;++i)
		// cut out those [CS], [NS] temporary facilities
//	if(stats[(*anarace->getStartCondition())->getRace()][i].unitType != ADD_ON_UNIT_TYPE)
	{
		for(unsigned int j=1;j<=GAS_SCV;++j)
		if((!efConfiguration.isRestrictSC())||(stats[(*anarace->getStartCondition())->getRace()][i].bwunit==0))
		if((stats[(*anarace->getStartCondition())->getRace()][j].facility[0] == i)||
		   (stats[(*anarace->getStartCondition())->getRace()][j].facility[1] == i)||
		   (stats[(*anarace->getStartCondition())->getRace()][j].facility[2] == i))
		{
			facility[facilityNumber] = i;
			++facilityNumber;
			break;
		}
	}
}
		
void UnitMenu::assignAnarace(ANABUILDORDER* goal_anarace)
{
	anarace = goal_anarace;
	resetData();
}

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
					if((i >=10 ))// || ((efConfiguration.isRestrictSC())&&(stats[(*anarace->getStartCondition())->getRace()][i].bwunit==1)))					
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
	//				if (fitItemToClientRect(edge, 1))
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+i));
						if(i==1)
							(*m)->updateText(theme.lookUpString((eString)(SCV+UNIT_NULL_STRING+UNIT_TYPE_COUNT*anarace->getRace())));
						else 
						if(i==2)
							(*m)->updateText(theme.lookUpString((eString)(GAS_SCV+UNIT_NULL_STRING+UNIT_TYPE_COUNT*anarace->getRace())));
						else
							(*m)->updateText(theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i)));
						(*m)->adjustRelativeRect(edge);
					}
					++height;
				}
//				height-=2;
//				height*=2;
			} else 
			{
				for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					++i;
					if((i >= facilityNumber)) //|| ((efConfiguration.isRestrictSC())/*&&(stats[(*anarace->getStartCondition())->getRace()][i].bwunit==1)*/))
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
	//			  if (fitItemToClientRect(edge, 1))
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][facility[i]].unitType));
						(*m)->updateText(UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+facility[i]+UNIT_NULL_STRING)) + "...");
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
				for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					++i;
					if(/*(i >= facilityNumber ) ||*/ (stats[(*anarace->getStartCondition())->getRace()][i].unitType != (signed int)(menuLevel)) || ((efConfiguration.isRestrictSC())&&(stats[(*anarace->getStartCondition())->getRace()][i].bwunit==1)))
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = calculateRect(getParent()->getWidth(), height);
		//			if (parent->fitItemToRelativeRect(edge, 1)) 
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+menuLevel));
						(*m)->updateText(UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING)));
						(*m)->adjustRelativeRect(edge);
					}
					++height;
				}
			} else
			{
				for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					++i;
					if((i > GAS_SCV ) || 
						(((stats[(*anarace->getStartCondition())->getRace()][i].facility[0] != facility[menuLevel-1])&&
				   		(stats[(*anarace->getStartCondition())->getRace()][i].facility[1] != facility[menuLevel-1])&&
					   (stats[(*anarace->getStartCondition())->getRace()][i].facility[2] != facility[menuLevel-1]))))
						{
							(*m)->Hide();
							continue;
						}
					Rect edge = calculateRect(getParent()->getWidth(), height);
			//		if (parent->fitItemToRelativeRect(edge, 1)) 
					{
						(*m)->Show();
						(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][i].unitType));
						(*m)->updateText(
						UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING)));
						(*m)->adjustRelativeRect(edge);
					}
					++height;
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
	switch((*anarace->getStartCondition())->getRace())
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
//	height+=height%2;
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
