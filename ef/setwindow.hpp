#ifndef _GUI_SETTINGS_HPP
#define _GUI_SETTINGS_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"
#include "../ui/radio.hpp"
#include "../ui/checkbutton.hpp"
#include "../ui/numberfield.hpp"

#include "../core/database.hpp"
#include "bitdepthmenu.hpp"
#include "languagemenu.hpp"
#include "resolutionmenu.hpp"

class SettingsWindow:public UI_Window
{
	public:
		SettingsWindow(UI_Object* setwindow_parent);
		~SettingsWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
		void updateItems();

		void forceLanguageChange();
		void reloadOriginalSize();
		
		const bool hasLanguageChanged() const;
		const bool hasResolutionChanged() const;
		const bool hasBitDepthChanged() const;
		const bool hasFullScreenChanged() const;
	private:

		void reloadFromFile();
		void loadFailsafeDefaults();
		void closeMenus();

		UI_Group* coreSettings;
		UI_Group* guiSettings;
		UI_Radio* uiSettingsRadio;
//		UI_Group* defaultSettings;
		UI_Group* loadSaveSettings;
		
		UI_Radio* defaultSettingsRadio;

		UI_NumberField* maxTime;
		UI_NumberField* maxLength;
		UI_NumberField* maxRuns;
		UI_NumberField* maxGenerations;
		UI_NumberField* maxTimeOut;
		UI_NumberField* breedFactor;
//		UI_NumberField* crossingOver;

		UI_CheckButton* autoSaveRuns;
		UI_CheckButton* alwaysBuildWorker;
		UI_CheckButton* onlySwapOrders;
		UI_CheckButton* restrictSC;
		UI_CheckButton* facilityMode;
//		UI_CheckButton* preprocessBuildorder;
//		UI_CheckButton* allowGoalAdaption;
		UI_CheckButton* glowingButtons;
		UI_CheckButton* dnaSpiral;
		UI_CheckButton* backgroundBitmap;
		UI_CheckButton* fullscreen;
		UI_CheckButton* tooltips;
//		UI_CheckButton* softwareMouse;

//		UI_CheckButton* transparency;
		UI_CheckButton* smoothMovement;
	
		UI_NumberField* desiredFramerate;
		UI_NumberField* desiredCPU;
	
		UI_Button* minimalistButton;
		UI_Button* fullButton;
		UI_Button* customButton;

		UI_Button* reloadFromFileButton;
		UI_Button* loadFailsafeDefaultsButton;
		UI_Button* saveToFileButton;

// number
	
	
//		ThemeMenu* themeMenu;
//		UI_Button* themeMenuButton;
		UI_Button* languageMenuButton;
		UI_Button* resolutionMenuButton;
		UI_Button* bitDepthMenuButton;
		LanguageMenu* languageMenu;
		ResolutionMenu* resolutionMenu;
		BitDepthMenu* bitDepthMenu;

		bool languageHasChanged;
		bool resolutionHasChanged;
		bool bitDepthHasChanged;
		bool fullScreenHasChanged;
};

inline const bool SettingsWindow::hasLanguageChanged() const {
	return(languageHasChanged);
}

inline const bool SettingsWindow::hasResolutionChanged() const {
	return(resolutionHasChanged);
}

inline const bool SettingsWindow::hasBitDepthChanged() const {
	return(bitDepthHasChanged);
}

inline const bool SettingsWindow::hasFullScreenChanged() const {
	return(fullScreenHasChanged);
}

#endif

