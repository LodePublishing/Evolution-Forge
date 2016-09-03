// TODO: SEttings ueberarbeiten
// AUTORUN [x] Checkbutton
// |-> Number of unchanged generations -- NumberField
//
// timeout, maxLength, maxTimeOut, ... in ein 'fast calculate' CheckButton zusammenfassen
//
// 
//
// breedfactor in CheckButton (Tiefen/Breitensuche), evtl 'max programs' wieder dazu
//
// maxLength raus => dynamisch anpassen (und grosser interner Maximalwert)
//
// Option bowindow, bodiagram window oder bograph window nicht anzuzeigen
//
// GAMMA NumberField!
// 
// TODO 'Wait' Befehle!
// TODO SDL_SetAlpha(surface, SDL_SRCALPHA, 128) !!!!!!!!!!
//  
// TODO Option: Warten nach Veraenderung

#include "setwindow.hpp"

#include "../core/configuration.hpp"
#include "../core/database.hpp"
#include "../ui/configuration.hpp"
#include "configuration.hpp"

SettingsWindow::SettingsWindow(UI_Object* setwindow_parent):
	UI_Window(setwindow_parent, SETTINGS_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(SETTINGS_WINDOW), theme.lookUpGlobalMaxHeight(SETTINGS_WINDOW), NOT_SCROLLED),

// core:

// timeout, maxLength, maxTimeOut, ... in ein 'fast calculate' CheckButton zusammenfassen
// breedfactor in CheckButton (Tiefen/Breitensuche), evtl 'max programs' wieder dazu
		
		
//		UI_Button* transparency;
// Option bowindow, bodiagram window oder bograph window nicht anzuzeigen
//		UI_Button* softwareMouse;

	uberSettings(new UI_Group(this, Rect(Point(15, 20), Size(100,0)), Size(0,10), ONE_COLOUMN_GROUP, true, TOP_LEFT, NULL_STRING)),
	coreSettings(new UI_Group(uberSettings, Rect(Point(15, 40), Size(100,0)), Size(0, 15), ONE_COLOUMN_GROUP, true, DO_NOT_ADJUST, SETWINDOW_CORE_SETTINGS_STRING)),
	soundSettings(new UI_Group(uberSettings, Rect(Point(15, 40), Size(100,0)), Size(0, 15), ONE_COLOUMN_GROUP, true, DO_NOT_ADJUST, SETWINDOW_SOUND_SETTINGS_STRING)),
	guiSettings(new UI_Group(uberSettings, Rect(Point(15, 40), Size(100,0)), Size(0, 15), ONE_COLOUMN_GROUP, true, DO_NOT_ADJUST, SETWINDOW_GUI_SETTINGS_STRING)),
	graphicSettings(new UI_Group(uberSettings, Rect(Point(15, 40), Size(100,0)), Size(0, 15), ONE_COLOUMN_GROUP, true, DO_NOT_ADJUST, SETWINDOW_GRAPHIC_SETTINGS_STRING)),
	
	uiSettingsRadio(new UI_Radio(this, Rect(Point(0, 30), Size(0, 0)), Size(10, 5), ONE_COLOUMN_GROUP, true, TOP_RIGHT, SETWINDOW_UI_SETTINGS_STRING)),
	loadSaveSettings(new UI_Group(this, Rect(Point(0, 0), Size(300, 0)), Size(10, 5), ONE_COLOUMN_GROUP, true, BOTTOM_RIGHT, SETWINDOW_LOADSAVE_SETTINGS_STRING)),
// TODO checkbuttons!!
// 
	fastCalculation(new UI_Button(coreSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_FAST_CALCULATION_STRING, SETTING_FAST_CALCULATION_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
//	alwaysBuildWorkers(new UI_Button(coreSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_ALWAYS_BUILD_WORKERS_STRING, SETTING_ALWAYS_BUILD_WORKERS_STRING)), TODO ... :-/
	allowWaitOrders(new UI_Button(coreSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_ALLOW_WAIT_ORDERS_STRING, SETTING_ALLOW_WAIT_ORDERS_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	waitAccuracy(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(2,2), DO_NOT_ADJUST,  1, 1200, SETTING_WAIT_ACCURACY_STRING, SETTING_WAIT_ACCURACY_TOOLTIP_STRING, 100, coreConfiguration.getWaitAccuracy(), true)),
//	gameSpeed(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(2,2), DO_NOT_ADJUST,  (int)GAME_SPEED_SLOWEST_STRING, (int)GAME_SPEED_FASTEST_STRING, SETTING_GAME_SPEED_STRING, SETTING_GAME_SPEED_TOOLTIP_STRING, 1, efConfiguration.getGameSpeed() + (int)GAME_SPEED_SLOWEST_STRING, false, STRING_NUMBER_TYPE)), TODO

	autoRuns(new UI_Button(coreSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_AUTO_RUNS_STRING, SETTING_AUTO_RUNS_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	maxGenerations(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(2,2), DO_NOT_ADJUST,  MIN_GENERATIONS, MAX_GENERATIONS, SETTING_MAX_GENERATIONS_STRING, SETTING_MAX_GENERATIONS_TOOLTIP_STRING, 10, efConfiguration.getMaxGenerations(), true)),
		
	useMusic(new UI_Button(soundSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_USE_MUSIC_STRING, SETTING_USE_MUSIC_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	musicVolume(new UI_NumberField(soundSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(2,2), DO_NOT_ADJUST,  0, 100, SETTING_MUSIC_VOLUME_STRING, SETTING_MUSIC_VOLUME_TOOLTIP_STRING, 1, uiConfiguration.getMusicVolume(), true, PERCENT_NUMBER_TYPE)),
	useSound(new UI_Button(soundSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_USE_SOUND_STRING, SETTING_USE_SOUND_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	soundVolume(new UI_NumberField(soundSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(2,2), DO_NOT_ADJUST,  0, 100, SETTING_SOUND_VOLUME_STRING, SETTING_SOUND_VOLUME_TOOLTIP_STRING, 1, uiConfiguration.getSoundVolume(), true, PERCENT_NUMBER_TYPE)),
	channels(new UI_NumberField(soundSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(2,2), DO_NOT_ADJUST,  2, 255, SETTING_CHANNELS_STRING, SETTING_CHANNELS_TOOLTIP_STRING, 1, uiConfiguration.getChannels(), true)),
 	backgroundBitmap(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_BACKGROUND_BITMAP_STRING, SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	smoothMovement(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_SMOOTH_MOVEMENT_STRING, SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	waitAfterChange(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_WAIT_AFTER_CHANGE_STRING, SETTING_WAIT_AFTER_CHANGE_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
 	tooltips(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_TOOLTIPS_STRING, SETTING_TOOLTIPS_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
 	dnaSpiral(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_DNA_SPIRAL_STRING, SETTING_DNA_SPIRAL_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
 	raceSpecificTheme(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_RACE_SPECIFIC_THEME_STRING, SETTING_RACE_SPECIFIC_THEME_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
 	glowingButtons(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_GLOWING_BUTTONS_STRING, SETTING_GLOWING_BUTTONS_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
 	compactDisplayMode(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_COMPACT_DISPLAY_MODE_STRING, SETTING_COMPACT_DISPLAY_MODE_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
 	facilityMode(new UI_Button(guiSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_FACILITY_MODE_STRING, SETTING_FACILITY_MODE_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	
 	fullscreen(new UI_Button(graphicSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_FULLSCREEN_STRING, SETTING_FULLSCREEN_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	unloadGraphics(new UI_Button(graphicSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_UNLOAD_GRAPHICS_STRING, SETTING_UNLOAD_GRAPHICS_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	showDebug(new UI_Button(graphicSettings, Rect(Point(10, 0), Size(200, 12)), Size(2, 2), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, SETTING_SHOW_DEBUG_STRING, SETTING_SHOW_DEBUG_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	desiredFramerate(new UI_NumberField(graphicSettings, Rect(Point(10, 0), Size(200, 12)), Size(2,2), DO_NOT_ADJUST,  UI_Configuration::MIN_DESIRED_FRAMERATE, UI_Configuration::MAX_DESIRED_FRAMERATE, SETTING_DESIRED_FRAMERATE_STRING, SETTING_DESIRED_FRAMERATE_TOOLTIP_STRING, 1, uiConfiguration.getDesiredFramerate())),
 	desiredCPU(new UI_NumberField(graphicSettings, Rect(Point(10, 0), Size(200, 12)), Size(2,2), DO_NOT_ADJUST, UI_Configuration::MIN_CPU_USAGE, UI_Configuration::MAX_CPU_USAGE, SETTING_DESIRED_CPU_USAGE_STRING, SETTING_DESIRED_CPU_USAGE_TOOLTIP_STRING, 1, uiConfiguration.getDesiredCPU(), false, PERCENT_NUMBER_TYPE)),

	reloadFromFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0)), Size(0, 2), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, SETTING_RELOAD_FROM_FILE_STRING, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	saveToFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0)), Size(0, 2), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, SETTING_SAVE_TO_FILE_STRING, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	loadDefaultsButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0)), Size(0, 2), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, SETTING_LOAD_DEFAULTS_STRING, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	
	languageMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 2), STANDARD_BUTTON, NULL_BITMAP, STATIC_BUTTON_MODE, SETTING_LANGUAGE_STRING, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	resolutionMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 2), STANDARD_BUTTON, NULL_BITMAP, STATIC_BUTTON_MODE, SETTING_RESOLUTION_STRING, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	bitDepthMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 2), STANDARD_BUTTON, NULL_BITMAP, STATIC_BUTTON_MODE, SETTING_BITDEPTH_STRING, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	themeMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 2), STANDARD_BUTTON, NULL_BITMAP, STATIC_BUTTON_MODE, SETTING_THEME_STRING, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),

	languageMenu(new UI_Menu(this, Rect(Point(0, 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, 2, SETTING_ENGLISH_LANGUAGE_STRING, UNIT_TYPE_5_BUTTON)),
	
	resolutionMenu(new UI_Menu(this, Rect(Point(0, 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, MAX_RESOLUTIONS-1, SETTING_RESOLUTION_640x480_STRING, UNIT_TYPE_6_BUTTON)),		

	bitDepthMenu(new UI_Menu(this, Rect(Point(0, 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, 1+DEPTH_32BIT-DEPTH_8BIT, SETTING_DEPTH_8BIT_STRING, UNIT_TYPE_7_BUTTON)),
	themeMenu(new UI_Menu(this, Rect(Point(0, 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, MAX_COLOR_THEMES-1, SETTING_DARK_RED_THEME_STRING, UNIT_TYPE_4_BUTTON)),

	languageHasChanged(false),
	resolutionHasChanged(false),
	bitDepthHasChanged(false),
	themeHasChanged(false),
	fullScreenHasChanged(false),
	compactDisplayModeHasChanged(false),
	allowWaitOrdersHasChanged(false),
	backGroundHasChanged(false)
//	gameSpeedHasChanged(false)
{
	updateItems();
	if(!efConfiguration.isAutoRuns())
		maxGenerations->Hide();
	if(!coreConfiguration.isAllowWaitOrders())
		waitAccuracy->Hide();
	if(!uiConfiguration.isMusic())
		musicVolume->Hide();
	if(!uiConfiguration.isSound())
	{
		soundVolume->Hide();
		channels->Hide();
	}

	uiSettingsRadio->addButton(languageMenuButton, 0);
	uiSettingsRadio->addButton(resolutionMenuButton, 1);
	uiSettingsRadio->addButton(bitDepthMenuButton, 2);
	uiSettingsRadio->addButton(themeMenuButton, 3);

	coreSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));
	soundSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));
	guiSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));
	graphicSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));

	languageMenu->setPositionParent(themeMenuButton);
	resolutionMenu->setPositionParent(themeMenuButton);
	bitDepthMenu->setPositionParent(themeMenuButton);
	themeMenu->setPositionParent(themeMenuButton);

	languageMenu->close();
	resolutionMenu->close();
	bitDepthMenu->close();
	themeMenu->close();
}

SettingsWindow::~SettingsWindow()
{
	delete coreSettings;
	delete soundSettings;
	delete guiSettings;
	delete graphicSettings;
	delete uberSettings;
	
	delete uiSettingsRadio;
	delete loadSaveSettings;

	delete fastCalculation;
//	delete alwaysBuildWorkers;
	delete allowWaitOrders;
	delete waitAccuracy;
//	delete gameSpeed;
	delete autoRuns;
	delete maxGenerations;
	
	delete useMusic;
	delete useSound;
	delete musicVolume;
	delete soundVolume;
	delete channels;

	delete backgroundBitmap;
	delete smoothMovement;
	delete waitAfterChange;
	delete tooltips;
	delete dnaSpiral;
	delete raceSpecificTheme;
	delete glowingButtons;
	delete compactDisplayMode;
	delete facilityMode;
	
	
	delete fullscreen;
	delete unloadGraphics;
	delete showDebug;
	delete desiredFramerate;
	delete desiredCPU;

	delete reloadFromFileButton;
	delete saveToFileButton;
	delete loadDefaultsButton;

	delete resolutionMenu;
	delete resolutionMenuButton;
	delete languageMenu;
	delete languageMenuButton;
	delete bitDepthMenu;
	delete bitDepthMenuButton;
	delete themeMenu;
	delete themeMenuButton;
	
	
}

void SettingsWindow::reloadOriginalSize()
{
	setOriginalRect(theme.lookUpGlobalRect(SETTINGS_WINDOW));
	setMaxHeight(theme.lookUpGlobalMaxHeight(SETTINGS_WINDOW));

	languageMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	resolutionMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	bitDepthMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	themeMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	
	reloadFromFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	saveToFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	loadDefaultsButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));

	UI_Window::reloadOriginalSize();

	coreSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));
	soundSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));
	
	guiSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));
	graphicSettings->alignWidth(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH));
}


void SettingsWindow::resetDataChange()
{
	languageHasChanged = false;
	bitDepthHasChanged = false;
	resolutionHasChanged = false;
	themeHasChanged = false;
	fullScreenHasChanged = false;
	compactDisplayModeHasChanged = false;
	backGroundHasChanged = false;
	allowWaitOrdersHasChanged = false;
//	gameSpeedHasChanged = false;
}

void SettingsWindow::closeMenus()
{
	if(languageMenu->isOpen()||resolutionMenu->isOpen()||bitDepthMenu->isOpen()||themeMenu->isOpen())
	{
		languageMenu->close();
		resolutionMenu->close();
		bitDepthMenu->close();
		themeMenu->close();
		makePufferInvalid();
	}
}

void SettingsWindow::process()
{

	if(!isShown())
		return;
	UI_Window::process();
// TODO
//	if((uberSettings->checkForNeedRedraw())/*(coreSettings->checkForNeedRedraw())||(guiSettings->checkForNeedRedraw())||(graphicSettings->checkForNeedRedraw())||(soundSettings->checkForNeedRedraw())*/||(uiSettingsRadio->checkForNeedRedraw())||(loadSaveSettings->checkForNeedRedraw()))
//	{
//		setNeedRedrawMoved();
		fitItemToRelativeClientRect(uberSettings->getRelativeRect(), true);
//	} // ?

	switch(uiSettingsRadio->getMarked())
	{
		case 0:
			languageMenu->open();
			if(!languageMenu->isOpen())
			{
				uiSettingsRadio->forceUnpressAll();
				closeMenus();
			} else
			{
				languageMenuButton->forcePress();
				closeMenus();
				languageMenu->open();
			}break;
		case 1:
			resolutionMenu->open();
			if(!resolutionMenu->isOpen())
			{
				uiSettingsRadio->forceUnpressAll();
				closeMenus();
			} else
			{
				resolutionMenuButton->forcePress();
				closeMenus();
				resolutionMenu->open();
			}break;
		case 2:
			bitDepthMenu->open();
			if(!bitDepthMenu->isOpen())
			{
				uiSettingsRadio->forceUnpressAll();
				closeMenus();
			} else
			{
				bitDepthMenuButton->forcePress();
				closeMenus();
					bitDepthMenu->open();
			}break;
			
		case 3:
			themeMenu->open();
			if(!themeMenu->isOpen())
			{
				uiSettingsRadio->forceUnpressAll();
				closeMenus();
			} else
			{
				themeMenuButton->forcePress();
				closeMenus();
				themeMenu->open();
			}break;
			
		default:break;
	}
	
	if(!isMouseInside())
	{
		uiSettingsRadio->forceUnpressAll();
		closeMenus();
	}
	
	int pressed;
	if((pressed=languageMenu->getPressedItem())>=0)
	{
		eLanguage newLanguage = (eLanguage)(pressed+1);
		if(newLanguage!=UI_Object::theme.getLanguage())
		{
			UI_Object::theme.setLanguage(newLanguage);
			uiConfiguration.setLanguage(UI_Object::theme.getLanguage());
			languageHasChanged = true;
		}
		languageMenuButton->forceUnpress();
		languageMenu->close();
		makePufferInvalid();
	} 
	else if((pressed=resolutionMenu->getPressedItem())>=0)
	{
		eResolution newResolution = (eResolution)(pressed+1);
		if(newResolution != UI_Object::theme.getResolution())
		{
			UI_Object::theme.setResolution(newResolution);
			uiConfiguration.setResolution(UI_Object::theme.getResolution());
			resolutionHasChanged = true;
		}
		resolutionMenuButton->forceUnpress();
		resolutionMenu->close();
		makePufferInvalid(); //?
	} 
	else if((pressed=bitDepthMenu->getPressedItem())>=0)
	{
		eBitDepth newBitDepth = (eBitDepth)pressed;
		if(newBitDepth != UI_Object::theme.getBitDepth())
		{
			UI_Object::theme.setBitDepth(newBitDepth);
			uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
			bitDepthHasChanged = true;
		}
		bitDepthMenuButton->forceUnpress();
		bitDepthMenu->close();
		makePufferInvalid(); //?
	}
	else if((pressed=themeMenu->getPressedItem())>=0)
	{
		eTheme newTheme = (eTheme)(pressed+1);
		if(newTheme != UI_Object::theme.getMainColorTheme())
		{
			UI_Object::theme.setMainColorTheme(newTheme);
			uiConfiguration.setTheme(UI_Object::theme.getMainColorTheme());
			themeHasChanged = true;
		}
		themeMenuButton->forceUnpress();
		themeMenu->close();
		makePufferInvalid();
	}

	coreConfiguration.setFastCalculation ( fastCalculation->isCurrentlyActivated() );
//	coreConfiguration.setAlwaysBuildWorkers ( alwaysBuildWorkers->isCurrentlyActivated() );
	
	if(coreConfiguration.setAllowWaitOrders ( allowWaitOrders->isCurrentlyActivated() ) )
	{
		allowWaitOrdersHasChanged = true;
		if(coreConfiguration.isAllowWaitOrders())
			waitAccuracy->Show();
		else waitAccuracy->Hide();
	}
	if(coreConfiguration.setWaitAccuracy( waitAccuracy->getNumber() ))
		allowWaitOrdersHasChanged = true;

//	if(efConfiguration.setGameSpeed( gameSpeed->getNumber() - (int)GAME_SPEED_SLOWEST_STRING ))
//		gameSpeedHasChanged = true;

	if(efConfiguration.setAutoRuns ( autoRuns->isCurrentlyActivated() ) )
	{
		if(efConfiguration.isAutoRuns())
			maxGenerations->Show();
		else maxGenerations->Hide();
	}
		
	efConfiguration.setMaxGenerations ( maxGenerations->getNumber() );

	uiConfiguration.setMusicVolume( musicVolume->getNumber() );
	uiConfiguration.setSoundVolume( soundVolume->getNumber() );
	if(uiConfiguration.setMusic( useMusic->isCurrentlyActivated() ))
	{
		if(uiConfiguration.isMusic())
			musicVolume->Show();
		else musicVolume->Hide();
	}
	if(uiConfiguration.setSound( useSound->isCurrentlyActivated() )) {
		if(uiConfiguration.isSound()) {
			soundVolume->Show();
			channels->Show();
		} else {
			soundVolume->Hide();
			channels->Hide();
		}
	}
	uiConfiguration.setChannels( channels->getNumber() );
	
	if( uiConfiguration.setBackgroundBitmap ( backgroundBitmap->isCurrentlyActivated()))
		backGroundHasChanged = true;
	uiConfiguration.setSmoothMovements ( smoothMovement->isCurrentlyActivated() );
	efConfiguration.setWaitAfterChange ( waitAfterChange->isCurrentlyActivated() );
	efConfiguration.setToolTips ( tooltips->isCurrentlyActivated() );
	efConfiguration.setDnaSpiral ( dnaSpiral->isCurrentlyActivated());
	efConfiguration.setRaceSpecificTheme ( raceSpecificTheme->isCurrentlyActivated());
	uiConfiguration.setGlowingButtons ( glowingButtons->isCurrentlyActivated());
	if( efConfiguration.setCompactDisplayMode( compactDisplayMode->isCurrentlyActivated() ) )
		compactDisplayModeHasChanged = true;
	efConfiguration.setFacilityMode( facilityMode->isCurrentlyActivated() );
	
	fullScreenHasChanged = uiConfiguration.setFullScreen ( fullscreen->isCurrentlyActivated());
	uiConfiguration.setUnloadGraphics( unloadGraphics->isCurrentlyActivated());
	efConfiguration.setShowDebug( showDebug->isCurrentlyActivated());
	uiConfiguration.setDesiredFramerate ( desiredFramerate->getNumber() );
	uiConfiguration.setDesiredCPU ( desiredCPU->getNumber() );

	if(reloadFromFileButton->isLeftClicked())
		reloadFromFile();
	if(loadDefaultsButton->isLeftClicked())
		loadDefaults();
	if(saveToFileButton->isLeftClicked())
	{
		coreConfiguration.saveToFile();
		uiConfiguration.saveToFile();
		efConfiguration.saveToFile();
	}

// TODO reihenfolge und Radiobuttons...
}

// reload from file
void SettingsWindow::reloadFromFile()
{
	toInitLog(UI_Object::theme.lookUpString(START_LOAD_UI_CONFIGURATION_STRING));
	uiConfiguration.loadConfigurationFile();

	toInitLog(UI_Object::theme.lookUpString(START_LOAD_CORE_CONFIGURATION_STRING));
	coreConfiguration.loadConfigurationFile();

	toInitLog(UI_Object::theme.lookUpString(START_LOAD_EF_CONFIGURATION_STRING));
	efConfiguration.loadConfigurationFile();

	updateItems();
}

// use failsafe default settings
void SettingsWindow::loadDefaults()
{
	uiConfiguration.initDefaults();
	efConfiguration.initDefaults();
	coreConfiguration.initDefaults();
	updateItems();
}

void SettingsWindow::updateItems()
{
	fastCalculation->check( coreConfiguration.isFastCalculation() );
//	alwaysBuildWorkers->check( coreConfiguration.isAlwaysBuildWorkers() );
	allowWaitOrders->check( coreConfiguration.isAllowWaitOrders() );
	waitAccuracy->updateNumber( coreConfiguration.getWaitAccuracy() );
	autoRuns->check( efConfiguration.isAutoRuns() );
	maxGenerations->updateNumber( efConfiguration.getMaxGenerations() );
//	gameSpeed->updateNumber( efConfiguration.getGameSpeed() + (int)GAME_SPEED_SLOWEST_STRING);
	
	useMusic->check( uiConfiguration.isMusic() );
	useSound->check( uiConfiguration.isSound() );
	musicVolume->updateNumber( uiConfiguration.getMusicVolume() );
	soundVolume->updateNumber( uiConfiguration.getSoundVolume() );
	channels->updateNumber( uiConfiguration.getChannels() );

	if(backgroundBitmap->isCurrentlyActivated() != uiConfiguration.isBackgroundBitmap())
		backGroundHasChanged = true;
	backgroundBitmap->check ( uiConfiguration.isBackgroundBitmap() );
	smoothMovement->check ( uiConfiguration.isSmoothMovements() );
	waitAfterChange->check ( efConfiguration.isWaitAfterChange() );
	tooltips->check ( efConfiguration.isToolTips() );
	dnaSpiral->check ( efConfiguration.isDnaSpiral() );
	raceSpecificTheme->check( efConfiguration.isRaceSpecificTheme() );
	glowingButtons->check ( uiConfiguration.isGlowingButtons() );
	if(compactDisplayMode->isCurrentlyActivated() != efConfiguration.isCompactDisplayMode())
		compactDisplayModeHasChanged = true;
	compactDisplayMode->check ( efConfiguration.isCompactDisplayMode() );
	facilityMode->check ( efConfiguration.isFacilityMode() );

	if(fullscreen->isCurrentlyActivated() != uiConfiguration.isFullScreen())
		fullScreenHasChanged=true;
	fullscreen->check ( uiConfiguration.isFullScreen() );
	unloadGraphics->check( uiConfiguration.isUnloadGraphics() );
	showDebug->check(efConfiguration.isShowDebug() );
	desiredFramerate->updateNumber ( uiConfiguration.getDesiredFramerate() );
	desiredCPU->updateNumber ( uiConfiguration.getDesiredCPU() );
	
	eLanguage newLanguage = uiConfiguration.getLanguage();
	if(newLanguage!=UI_Object::theme.getLanguage())
	{
		UI_Object::theme.setLanguage( newLanguage );
		uiConfiguration.setLanguage( newLanguage );
		languageHasChanged = true;
		resolutionHasChanged = true;
	}
	
	eResolution newResolution = uiConfiguration.getResolution();
	if(newResolution != UI_Object::theme.getResolution())
	{
		UI_Object::theme.setResolution( newResolution );
		uiConfiguration.setResolution( newResolution );
		resolutionHasChanged = true;
	}

	eBitDepth newBitDepth = uiConfiguration.getBitDepth();
	if(newBitDepth != UI_Object::theme.getBitDepth())
	{
		UI_Object::theme.setBitDepth( newBitDepth );
		uiConfiguration.setBitDepth( newBitDepth );
		bitDepthHasChanged = true;
	}
	
	eTheme newTheme = uiConfiguration.getTheme();
	if(newTheme != UI_Object::theme.getMainColorTheme())
	{
		UI_Object::theme.setMainColorTheme( newTheme );
		uiConfiguration.setTheme( newTheme );
		themeHasChanged = true;
	}
}

void SettingsWindow::forceFullScreenChange()
{
	fullScreenHasChanged = true;
	fullscreen->check(!fullscreen->isCurrentlyActivated());
	if(!isShown())
		uiConfiguration.setFullScreen(!uiConfiguration.isFullScreen());
//	fullscreen->setNeedRedrawMoved(); TODO
}



