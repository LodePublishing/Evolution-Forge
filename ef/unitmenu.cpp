#include "unitmenu.hpp"

UnitMenu::UnitMenu(UI_Object* parent, ANARACE* anarace, Rect rect) : Menu(parent, anarace, rect)
{
	markedUnit=0;
}

UnitMenu::~UnitMenu()
{
}

const int UnitMenu::getMarkedUnit() const
{
	return(markedUnit);
}

void UnitMenu::process()
{
	Menu::process();
	
	markedUnit = 0;
	// check for Pressed Units
	pressedItem = -1;
	if(menuLevel)
	for (int i = 1; i <= GAS_SCV; i++)
		if (menuEntry[i]->isPressed())
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
		}
		
	if(menuLevel)
	{
		if(menuLevel==1)
		{
			for (int i = 1; i < 10; i++)
			{
				Rect edge = Rect(Point(10, height * (FONT_SIZE + 6) /*- getScrollY()*/), Size(120, FONT_SIZE + 5));
//				if (fitItemToClientRect(edge, 1))
				{
					menuEntry[i]->Show();
					if (menuEntry[i]->isCurrentlyHighlighted())
						markedUnit = i;
					menuEntry[i]->setButton(eButton(UNIT_TYPE_0_BUTTON+i));
					menuEntry[i]->updateText(*theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i)));
					menuEntry[i]->adjustRelativeRect(edge);
				}
				height++;
			}
			for(int i=11;i<=GAS_SCV;i++)
				menuEntry[i]->Hide();
			menuEntry[0]->Hide();
		}
		else if(menuLevel>1)
		for(int i = 0; i<=GAS_SCV; i ++)
		if (stats[(*anarace->getStartCondition())->getRace()][i].unitType == menuLevel)
		{
			Rect edge = Rect(Point(10, height*(FONT_SIZE+6)), Size(120, FONT_SIZE+5));
			menuEntry[i]->Show();
//			menuEntry[i]->setColor((eUnitType)(menuLevel));
	//		if (parent->fitItemToRelativeRect(edge, 1)) 
			{
				menuEntry[i]->Show();
				if (menuEntry[i]->isCurrentlyHighlighted())
					markedUnit = i;
				menuEntry[i]->setButton(eButton(UNIT_TYPE_0_BUTTON+menuLevel));
				menuEntry[i]->updateText(stats[(*anarace->getStartCondition())->getRace()][i].name);
				menuEntry[i]->adjustRelativeRect(edge);
			}
			height++;
		} else menuEntry[i]->Hide();
	} else
		for(int i=0;i<=GAS_SCV;i++)
			menuEntry[i]->Hide();

}

void UnitMenu::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Object::draw(dc);
}

