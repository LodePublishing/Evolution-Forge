#ifndef _GUI_SETTINGS_HPP
#define _GUI_SETTINGS_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"
#include "../ui/radio.hpp"
#include "../ui/numberfield.hpp"

#include "../core/database.hpp"
#include "bitdepthmenu.hpp"
#include "languagemenu.hpp"
#include "resolutionmenu.hpp"
#include "thememenu.hpp"

class SettingsWindow:public UI_Window
{
	public:
		SettingsWindow(UI_Object* setwindow_parent);
		~SettingsWindow();
		
		void resetData();
		void resetDataChange();
		void process();
		void draw(DC* dc) const;
		void updateItems();

		void forceLanguageChange();
		void forceResolutionChange();
		void forceBitDepthChange();
		void forceFullScreenChange();
		void reloadOriginalSize();
		
		const bool hasLanguageChanged() const;
		const bool hasResolutionChanged() const;
		const bool hasBitDepthChanged() const;
		const bool hasThemeChanged() const;
		const bool hasFullScreenChanged() const;
		const bool hasCompactDisplayModeChanged() const;
	private:

		void reloadFromFile();
		void loadDefaults();
		void closeMenus();

		// TODO: SEttings ueberarbeiten
//
//
// 
//
//
// maxLength raus => dynamisch anpassen (und grosser interner Maximalwert)
//
// GAMMA NumberField!
// TODO SDL_SetAlpha(surface, SDL_SRCALPHA, 128) !!!!!!!!!!
//  
// waitorders, breedfactor, timeout, maxLength, maxTimeOut, ... in ein 'fast calculate' CheckButton zusammenfassen
// breedfactor in CheckButton (Tiefen/Breitensuche), evtl 'max programs' wieder dazu
//		UI_Button* transparency;
// Option bowindow, bodiagram window oder bograph window nicht anzuzeigen
//		UI_Button* softwareMouse;

		UI_Group* uberSettings;

		UI_Group* coreSettings;
		UI_Group* soundSettings;
		UI_Group* guiSettings;
		UI_Group* graphicSettings;

		UI_Radio* uiSettingsRadio;
		UI_Group* loadSaveSettings;

		UI_Button* fastCalculation;
		UI_Button* expansionSet;
		UI_Button* alwaysBuildWorkers;
		UI_Button* allowWaitOrders;
		UI_NumberField* waitAccuracy;
		UI_Button* autoRuns;
		UI_NumberField* maxGenerations;
	
		UI_Button* useMusic;
		UI_NumberField* musicVolume;
		UI_Button* useSound;
		UI_NumberField* soundVolume;
		UI_NumberField* channels; // TODO evtl raus
		
		UI_Button* backgroundBitmap;
		UI_Button* smoothMovement;
		UI_Button* waitAfterChange;
		UI_Button* tooltips;
		UI_Button* dnaSpiral;
		UI_Button* glowingButtons;
		UI_Button* compactDisplayMode;
		UI_Button* facilityMode;
		
		UI_Button* fullscreen;
		UI_Button* unloadGraphics;
		UI_Button* showDebug;
		UI_NumberField* desiredFramerate;
		UI_NumberField* desiredCPU;
	
		UI_Button* reloadFromFileButton;
		UI_Button* saveToFileButton;
		UI_Button* loadDefaultsButton;

// number
	
		UI_Button* languageMenuButton;
		UI_Button* resolutionMenuButton;
		UI_Button* bitDepthMenuButton;
		UI_Button* themeMenuButton;
		LanguageMenu* languageMenu;
		ResolutionMenu* resolutionMenu;
		BitDepthMenu* bitDepthMenu;
		ThemeMenu* themeMenu;

		bool languageHasChanged;
		bool resolutionHasChanged;
		bool bitDepthHasChanged;
		bool themeHasChanged;
		bool fullScreenHasChanged;
		bool compactDisplayModeHasChanged;
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

inline const bool SettingsWindow::hasThemeChanged() const {
	return(themeHasChanged);
}

inline const bool SettingsWindow::hasFullScreenChanged() const {
	return(fullScreenHasChanged);
}

inline const bool SettingsWindow::hasCompactDisplayModeChanged() const {
	return(compactDisplayModeHasChanged);
}

#endif

