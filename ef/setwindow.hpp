#ifndef _GUI_SETTINGS_HPP
#define _GUI_SETTINGS_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"
#include "../ui/radio.hpp"
#include "../ui/checkbutton.hpp"
#include "../ui/numberfield.hpp"

#include "../core/settings.hpp"
#include "languagemenu.hpp"
#include "resolutionmenu.hpp"
#include "thememenu.hpp"

class SettingsWindow:public UI_Window
{
	public:
		SettingsWindow(UI_Object* setwindow_parent);
		~SettingsWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
		void updateItems();

		const bool hasLanguageChanged();
	private:

		void reloadFromFile();
		void loadFailsafeDefaults();

		UI_Group* coreSettings;
		UI_Group* guiSettings;
//		UI_Group* defaultSettings;
		UI_Group* loadSaveSettings;

		UI_NumberField* maxTime;
		UI_NumberField* maxLength;
		UI_NumberField* maxRuns;
		UI_NumberField* maxGenerations;
		UI_NumberField* maxTimeOut;
		UI_NumberField* breedFactor;
//		UI_NumberField* crossingOver;

		UI_CheckButton* autoSaveRuns;
		UI_CheckButton* facilityMode;
//		UI_CheckButton* preprocessBuildorder;
//		UI_CheckButton* allowGoalAdaption;
		UI_CheckButton* glowingButtons;
		UI_CheckButton* dnaSpiral;
//		UI_CheckButton* backgroundBitmap;
		UI_CheckButton* fullscreen;
		UI_CheckButton* tooltips;
//		UI_CheckButton* softwareMouse;

//		UI_CheckButton* transparency;
		UI_CheckButton* smoothMovement;
		UI_CheckButton* allowStaticFramerate;
	
		UI_NumberField* staticFramerate;
		UI_NumberField* dynamicFramerate;
	
		UI_Button* minimalistButton;
		UI_Button* fullButton;
		UI_Button* customButton;
		UI_Radio* defaultSettingsRadio;

		UI_Button* reloadFromFileButton;
		UI_Button* loadFailsafeDefaultsButton;
		UI_Button* saveToFileButton;

// number
	
	
//		ThemeMenu* themeMenu;
//		UI_Button* themeMenuButton;
//		ResolutionMenu* resolutionMenu;
//		UI_Button* resolutionMenuButton;
		LanguageMenu* languageMenu;
		UI_Button* languageMenuButton;
//		UI_Radio* menuRadio;

		bool languageHasChanged;
};

#endif

