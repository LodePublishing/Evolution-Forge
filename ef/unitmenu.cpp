#include "unitmenu.hpp"

UnitMenu::UnitMenu(UI_Object* unit_parent, ANARACE* unit_anarace, Rect unit_rect) : 
	Menu(unit_parent, unit_rect, false),
	anarace(unit_anarace),
	facilityMode(false),
	facilityNumber(1)
{
    for(int i=0;i<UNIT_TYPE_COUNT;i++) //TODO
    {
        MenuEntry* entry = new MenuEntry(this,
                        Rect(0, 0,120, FONT_SIZE+4), "ERROR"); //TODO maybe already initialize with name string
        menuEntries.push_back(entry);
    }
	
	resetData();
}

UnitMenu::UnitMenu(const UnitMenu& object) :
    Menu((Menu)object),
    anarace(object.anarace),
    facilityMode(object.facilityMode),
    facilityNumber(object.facilityNumber)
{
	for(int i = GAS_SCV+1; i--;)
		facility[i] = object.facility[i];
}

UnitMenu& UnitMenu::operator=(const UnitMenu& object)
{
	((UnitMenu)(*this)) = ((UnitMenu)object);
	anarace = object.anarace;
	facilityMode = object.facilityMode;
	facilityNumber = object.facilityNumber;
	for(int i = GAS_SCV+1; i--;)
		facility[i] = object.facility[i];
	return(*this);
}


UnitMenu::~UnitMenu()
{ }

void UnitMenu::resetData()
{
	facilityNumber=1;
	for(unsigned int i=1;i<=GAS_SCV;i++)
		// cut out those [CS], [NS] temporary facilities
//    if(stats[(*anarace->getStartCondition())->getRace()][i].unitType != ADD_ON_UNIT_TYPE)
	{
		for(int j=1;j<=GAS_SCV;j++)
        if((stats[(*anarace->getStartCondition())->getRace()][j].facility[0] == i)||
		   (stats[(*anarace->getStartCondition())->getRace()][j].facility[1] == i)||
		   (stats[(*anarace->getStartCondition())->getRace()][j].facility[2] == i))
		{
			facility[facilityNumber]=i;
			facilityNumber++;
			break;
		}
	}
}

void UnitMenu::setFacilityModus(const bool facilityMode)
{
	this->facilityMode=facilityMode;
}

void UnitMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
	height = 3;
	// check for Pressed Units
	eButton color;
    switch((*anarace->getStartCondition())->getRace())
    {
 	   case TERRA:color=UNIT_TYPE_5_BUTTON;break;
	   case PROTOSS:color=UNIT_TYPE_7_BUTTON;break;
	   case ZERG:color=UNIT_TYPE_3_BUTTON;break;
	   default:break;
	}
			
	if(menuLevel)
	{
		if(menuLevel==1)
		{
			unsigned int i=0;
			if(!facilityMode)
			{
		        for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					i++;
					if(i >=10 )
					{
						(*m)->Hide();
						continue;
					}
					Rect edge = Rect(Point(10/*+130*(height%2)*/, ((height+1)/*/2*/+1-2) * (FONT_SIZE + 9) /*- getScrollY()*/), Size(0,0));
	//				if (fitItemToClientRect(edge, 1))
					{
						(*m)->Show();
						(*m)->setButton(eButton(UNIT_TYPE_0_BUTTON+i));
						(*m)->updateText(*theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i)));
						(*m)->adjustRelativeRect(edge);
					}
					height++;
				}
				height-=2;
				height*=2;
			} else 
			{
                for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
                {
					i++;
                    if(i >= facilityNumber )
                    {
                        (*m)->Hide();
                        continue;
                    }
                    Rect edge = Rect(Point(10+130*(height%2), ((height+1)/2+1) * (FONT_SIZE + 9) /*- getScrollY()*/), Size(0,0));
    //              if (fitItemToClientRect(edge, 1))
                    {
                        (*m)->Show();
                        (*m)->setButton(color); // TODO
                        (*m)->updateText(stats[(*anarace->getStartCondition())->getRace()][facility[i]].name);
                        (*m)->adjustRelativeRect(edge);
                    }
                    height++;
                }
			}
		} // end 'if menuLevel == 1'
		else if(menuLevel>1)
		{
			unsigned int i =0;
			if(!facilityMode)
			{
                for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
                {
                    i++;
                    if(/*(i >= facilityNumber ) ||*/ (stats[(*anarace->getStartCondition())->getRace()][i].unitType != (signed int)(menuLevel)))
                    {
                        (*m)->Hide();
                        continue;
                    }
					Rect edge = Rect(Point(10+130*(height%2), ((height+1)/2+1)*(FONT_SIZE+9)), Size(0,0));
		//			if (parent->fitItemToRelativeRect(edge, 1)) 
					{
						(*m)->Show();
						(*m)->setButton(eButton(UNIT_TYPE_0_BUTTON+menuLevel));
						(*m)->updateText(stats[(*anarace->getStartCondition())->getRace()][i].name);
						(*m)->adjustRelativeRect(edge);
					}
					height++;
				}
			} else
			{
                for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
                {
					i++;
                    if((i > GAS_SCV ) || 
						(((stats[(*anarace->getStartCondition())->getRace()][i].facility[0] != facility[menuLevel-1])&&
        		   		(stats[(*anarace->getStartCondition())->getRace()][i].facility[1] != facility[menuLevel-1])&&
			           (stats[(*anarace->getStartCondition())->getRace()][i].facility[2] != facility[menuLevel-1]))))
	                    {
    	                    (*m)->Hide();
        	                continue;
            	        }
					Rect edge = Rect(Point(10+130*(height%2), ((height+1)/2+1)*(FONT_SIZE+9)), Size(0,0));
			//		if (parent->fitItemToRelativeRect(edge, 1)) 
					{
						(*m)->Show();
    							
						(*m)->setButton(color); 
						(*m)->updateText(stats[(*anarace->getStartCondition())->getRace()][i].name);
						(*m)->adjustRelativeRect(edge);
					}
					height++;
				}
			} 
		} // end 'if menuLevel > 1'
	} // end 'if menuLevel'
	else
		for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
			(*m)->Hide();

// special rules for sub menu...
	if(pressedItem>-1)
	{
		int i = pressedItem+1;
		pressedItem = -1;
		if(!facilityMode)
		{
			if ((menuLevel == 1) && (i == 1))  //scv
					pressedItem = SCV;
		// TODO: pruefen ob das Goal schon vorhanden ist, wenn ja => hasChanged nicht aufrufen
				else if ((menuLevel == 1) && (i == 2))   //gasscv
					pressedItem = GAS_SCV;
				else if (menuLevel == 1)
					menuLevel = i;
				else if (menuLevel > 1)
					pressedItem = i;
		} else
		{
			if(menuLevel == 1)
				menuLevel = i+1;
			else if(menuLevel > 1) 
				pressedItem=i;
		}
	}
	if(markedItem>-1)
	{
		int i = markedItem;
		markedItem = -1;
		if(!facilityMode)
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
	height+=4;
}

void UnitMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

