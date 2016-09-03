#ifndef _GUI_SETTINGS_HPP
#define _GUI_SETTINGS_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"
#include "../ui/radio.hpp"

#include "../core/settings.hpp"
#include "numberfield.hpp"
#include "languagemenu.hpp"
#include "resolutionmenu.hpp"
#include "thememenu.hpp"
#include "checkbutton.hpp"

class SettingsWindow:public UI_Window
{
	public:
		SettingsWindow(UI_Object* parent);
		~SettingsWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
	private:

		void updateItems();
		void reloadFromFile();
		void loadFailsafeDefaults();

		UI_Group* coreSettings;
		UI_Group* guiSettings;
		UI_Group* defaultSettings;

// boolean
		CheckButton* preprocessBuildorder;
		CheckButton* allowGoalAdaption;
		CheckButton* allowStaticFramerate;
		CheckButton* glowingButtons;
		CheckButton* backgroundBitmap;
		CheckButton* fullscreen;
		CheckButton* tooltips;

		CheckButton* transparency;
		CheckButton* smoothMovement;

// radio
		UI_Button* minimalistButton;
		UI_Button* fullButton;
		UI_Button* customButton;
		UI_Radio* graphicRadio;

		UI_Button* reloadFromFileButton;
		UI_Button* loadFailsafeDefaultsButton;
		UI_Button* saveToFileButton;

// number
		NumberField* maxTime;
		NumberField* maxLength;
		NumberField* maxRuns;
		NumberField* maxGenerations;
		NumberField* maxTimeOut;
		NumberField* breedFactor;
		NumberField* crossingOver;
		NumberField* staticFramerate;
		NumberField* dynamicFramerate;
	
	
		ThemeMenu* themeMenu;
		UI_Button* themeMenuButton;
		ResolutionMenu* resolutionMenu;
		UI_Button* resolutionMenuButton;
		LanguageMenu* languageMenu;
		UI_Button* languageMenuButton;
		UI_Radio* menuRadio;
};

#endif

