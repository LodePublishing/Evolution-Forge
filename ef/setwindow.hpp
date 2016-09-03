#ifndef _GUI_SETTINGS_HPP
#define _GUI_SETTINGS_HPP

#include "../ui/window.hpp"

#include "../core/settings.hpp"
#include "numberfield.hpp"
#include "menu.hpp"


class LanguageMenu : public Menu
{
    public:
        LanguageMenu(UI_Object* language_parent, Rect language_rect):
			Menu(language_parent, language_rect/*, 120, MAX_LANGUAGES-1*/)
		{
			int i = 1;
			for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
			{
				(*m)->updateText(*theme.lookUpString((eString)(SETTING_LANGUAGE_STRING+i)));
				(*m)->setButton(eButton(UNIT_TYPE_5_BUTTON));
				i++;
			}
		}
		~LanguageMenu() {};
        void process()
		{
			if(!isShown())
				return;
			Menu::process();
			if(menuLevel)
			{
				int i=0;
				for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				{
					Rect edge = Rect(Point(10 + i * 90, height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3));
		//          if (parent->fitItemToRelativeRect(edge, 1))
        		    {
		                (*m)->adjustRelativeRect(edge);
        		    }
		            if ((*m)->isLeftClicked())
        		    {
                		pressedItem = i;
		                open();
        		        break;
		            }
					i++;
				}
				height++;
			}
		}
	
        void draw(DC* dc) const
		{
		    if(!isShown())
		        return;
		    Menu::draw(dc);
		}	
};

/*class ResolutionMenu : public Menu
{
    public:
        ResolutionMenu(UI_Object* resolution_parent, Rect resolution_rect):
			Menu(resolution_parent, NULL, resolution_rect, 120)
		{ }
		~ResolutionMenu() {};
        void process();
        void draw(DC* dc) const;
};*/

class SettingsWindow:public UI_Window
{
	public:
		SettingsWindow(UI_Object* parent);
		~SettingsWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
	private:
/*
//		UI_Radio* radio;
		UI_Radio* menuRadio;
//		UI_Button* goalButton[3];
		UI_Button* menuButton[MAX_MENUS];*/


/*		UI_Button* addMaxTime, subMaxTime;
		UI_Button* addMaxLength, subMaxLength;
		UI_Button* addMaxTimeOut, subMaxTimeOut;
		UI_Button* addFramerate, subFramerate;*/
		


// boolean
/*		UI_StaticText* preprocessText;
		UI_StaticText* allowGoalAdaptionText;
		UI_StaticText* staticFramerateText;

// radio
		UI_StaticText* minimalistText;
		UI_StaticText* fullText;
		UI_StaticText* customText;

		UI_StaticText* smoothMovementText;
		
		UI_StaticText* glowingText;
		UI_StaticText* backgroundBitmapText;
		UI_StaticText* transparencyText;
		UI_StaticText* resolutionText;
		UI_StaticText* fullscreenText;

// number
		NumberField* maxTime;
		NumberField* maxLength;
		NumberField* maxRuns;
		NumberField* maxTimeOut;
		NumberField* breedFactor;
		NumberField* crossingOver;
		NumberField* framerate;
//		NumberField* dynamicFramerateText;

		UI_Radio* languageRadio;
		UI_StaticText* languageText;

		UI_

		UI_Button* languageButton[MAX_LANGUAGES];
		UI_Button* resolutionButton[MAX_RESOLUTIONS];
		UI_Button* themeButton[MAX_THEMES];*/

/*
		void closeMenus();

		ForceEntry* forceEntry[GAS_SCV+1];
		
		int markedUnit;
//		int oldForceList[UNIT_TYPE_COUNT];
		int currentUnitType;

		int unitTypeCount[UNIT_TYPE_TYPES];
		int startUnitTypeCount[UNIT_TYPE_TYPES];
		int currentUnitTypeCount[UNIT_TYPE_TYPES];
		int targetUnitTypeCount[UNIT_TYPE_TYPES];
	    int totalUnits;

		UnitMenu* unitMenu;
		GoalMenu* goalMenu;
		ForceMenu* forceMenu;
		RaceMenu* raceMenu;*/

		LanguageMenu* langMenu;
};

#endif

