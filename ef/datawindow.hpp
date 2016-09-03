#ifndef _GUI_DATABASE_WINDOW_HPP
#define _GUI_DATABASE_WINDOW_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"
#include "../ui/radio.hpp"
#include "../ui/editfield.hpp"
#include "../ui/checkbutton.hpp"
#include "../ui/numberfield.hpp"

#include "../core/database.hpp"
#include "bitdepthmenu.hpp"
#include "languagemenu.hpp"
#include "resolutionmenu.hpp"
#include "locationmenu.hpp"

/*enum eDataBaseWindowMenu
{
	MAPWINDOW_NEW_BUTTON,
	MAPWINDOW_RESET_BUTTON,
	MAPWINDOW_LOAD_BUTTON,
	MAPWINDOW_SAVE_BUTTON,
	
	MAX_MAPWINDOW_BUTTONS
};*/

class DataBaseWindow:public UI_Window
{
	public:
		DataBaseWindow(UI_Object* data_window_parent);
		~DataBaseWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
//		void updateItems();

//		void forceLanguageChange();
		void reloadOriginalSize();
		
//		const bool hasLanguageChanged() const;
//		const bool hasResolutionChanged() const;
//		const bool hasBitDepthChanged() const;
//		const bool hasFullScreenChanged() const;
	private:
/*		BASIC_MAP* map;

		UI_Group* mapSettings;
		UI_EditField* mapName;

		UI_NumberField* maxPlayer;
		UI_NumberField* maxLocations;
		UI_CheckButton* symmetryButton;

		UI_Group* locationSettings;
		UI_EditField* locationName[MAX_LOCATIONS];

		UI_Radio* playerSettings;
		UI_Button* playerStart[MAX_INTERNAL_PLAYER];
		LocationMenu* locationMenu;

		UI_Radio* menuRadio;
		UI_Button* menuButton[MAX_MAPWINDOW_BUTTONS];


		UI_Group* locationContents;
		UI_NumberField* mineralBlocks;
		UI_NumberField* vespeneGeysirs;
		UI_NumberField* mineralDistance;
		UI_NumberField* distance[MAX_LOCATIONS];
		
		void calculateMap();*/
		
//		void calculateStartCondition();
		
//		void reloadFromFile();
//		void loadFailsafeDefaults();
		void closeMenus();
#if 0
		UI_Group* coreSettings;
		UI_Group* guiSettings;
		UI_Radio* uiSettingsRadio;
//		UI_Group* defaultSettings;
		UI_Group* loadSaveSettings;
		
		UI_Radio* defaultSettingsRadio;

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
#endif
};

/*inline const bool DataBaseWindow::hasLanguageChanged() const {
	return(languageHasChanged);
}

inline const bool DataBaseWindow::hasResolutionChanged() const {
	return(resolutionHasChanged);
}

inline const bool DataBaseWindow::hasBitDepthChanged() const {
	return(bitDepthHasChanged);
}

inline const bool DataBaseWindow::hasFullScreenChanged() const {
	return(fullScreenHasChanged);
}*/

#endif

