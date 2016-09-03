#include "setwindow.hpp"

#include "../core/configuration.hpp"
#include "../ui/configuration.hpp"
#include "configuration.hpp"

SettingsWindow::SettingsWindow(UI_Object* setwindow_parent):
	UI_Window(setwindow_parent, SETTINGS_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(SETTINGS_WINDOW), theme.lookUpGlobalMaxHeight(SETTINGS_WINDOW), NOT_SCROLLED),

	coreSettings(new UI_Group(this, Rect(Point(15, 35), Size(0,0)), Size(0, 5), TOP_LEFT, SETWINDOW_CORE_SETTINGS_STRING)),
	guiSettings(new UI_Group(this, Rect(Point(15, 0), Size(0,0)), Size(0, 5), BOTTOM_LEFT, SETWINDOW_GUI_SETTINGS_STRING)),
	uiSettingsRadio(new UI_Radio(this, Rect(Point(0, 0), Size(0, 0)), Size(10, 5), CENTER_RIGHT, SETWINDOW_UI_SETTINGS_STRING)), 
	loadSaveSettings(new UI_Group(this, Rect(Point(0, 0), Size(300, 0)), Size(10, 5), BOTTOM_RIGHT, SETWINDOW_GUI_SETTINGS_STRING/*SETWINDOW_LOADSAVE_SETTINGS_STRING*/)),
	defaultSettingsRadio(new UI_Radio(this, Rect(Point(0, 35), Size(300, 0)), Size(10, 0), TOP_RIGHT, TITLE_PREDEFINED_SETTINGS_STRING)),
	
	maxTime(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT, MIN_TIME, MAX_TIME, SETTING_MAX_TIME_STRING, SETTING_MAX_TIME_TOOLTIP_STRING, 6, coreConfiguration.getMaxTime(), TIME_NUMBER_TYPE)),
	maxLength(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_LENGTH, MAX_LENGTH, SETTING_MAX_LENGTH_STRING, SETTING_MAX_LENGTH_TOOLTIP_STRING, 1, coreConfiguration.getMaxLength())),
	maxRuns(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_RUNS, MAX_RUNS, SETTING_MAX_RUNS_STRING, SETTING_MAX_RUNS_TOOLTIP_STRING, 1, coreConfiguration.getMaxRuns())),

	maxGenerations(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_GENERATIONS, MAX_GENERATIONS, SETTING_MAX_GENERATIONS_STRING, SETTING_MAX_GENERATIONS_TOOLTIP_STRING, 10, coreConfiguration.getMaxGenerations())),
	maxTimeOut(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_TIMEOUT, MAX_TIMEOUT, SETTING_MAX_TIMEOUT_STRING, SETTING_MAX_TIMEOUT_TOOLTIP_STRING, 1, coreConfiguration.getMaxTimeOut())),
	breedFactor(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_BREED_FACTOR, MAX_BREED_FACTOR, SETTING_BREED_FACTOR_STRING, SETTING_BREED_FACTOR_TOOLTIP_STRING, 1, coreConfiguration.getBreedFactor(), PERCENT_NUMBER_TYPE)),
	//crossingOver(new UI_NumberField(coreSettings, Rect(Point(10, 90), Size(200, 15)), Size(5, 5), ARRANGE_LEFT,  MIN_CROSSING_OVER, MAX_CROSSING_OVER, 1, coreConfiguration.getCrossingOver(), SETTING_CROSSING_OVER_STRING, SETTING_CROSSING_OVER_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),

//TODO
	autoSaveRuns(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_AUTO_SAVE_RUNS_STRING, SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING, efConfiguration.isAutoSaveRuns())),
	restrictSC(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_RESTRICT_SC_STRING, SETTING_RESTRICT_SC_TOOLTIP_STRING, efConfiguration.isRestrictSC())),
 	facilityMode(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_FACILITY_MODE_STRING, SETTING_FACILITY_MODE_TOOLTIP_STRING, efConfiguration.isFacilityMode())),
//	preprocessBuildorder(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_PREPROCESS_BUILDORDER_STRING, SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING, coreConfiguration.isPreprocessBuildOrder())),
//	allowGoalAdaption(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_ALLOW_GOAL_ADAPTION_STRING, SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING, coreConfiguration.isAllowGoalAdaption())),

 	glowingButtons(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_GLOWING_BUTTONS_STRING, SETTING_GLOWING_BUTTONS_TOOLTIP_STRING, uiConfiguration.isGlowingButtons())),
 	dnaSpiral(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_DNA_SPIRAL_STRING, SETTING_DNA_SPIRAL_TOOLTIP_STRING, efConfiguration.isDnaSpiral())),
 	backgroundBitmap(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_BACKGROUND_BITMAP_STRING, SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING, efConfiguration.isBackgroundBitmap())),
 	fullscreen(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_FULLSCREEN_STRING, SETTING_FULLSCREEN_TOOLTIP_STRING, efConfiguration.isFullScreen())),
 	tooltips(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_TOOLTIPS_STRING, SETTING_TOOLTIPS_TOOLTIP_STRING, efConfiguration.isToolTips())),
//	softwareMouse(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_SOFTWARE_MOUSE_STRING, SETTING_SOFTWARE_MOUSE_TOOLTIP_STRING, efConfiguration.isSoftwareMouse())),
	unloadGraphics(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_UNLOAD_GRAPHICS_STRING, SETTING_UNLOAD_GRAPHICS_TOOLTIP_STRING, uiConfiguration.isUnloadGraphics())),
//	transparency(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_TRANSPARENCY_STRING, SETTING_TRANSPARENCY_TOOLTIP_STRING, uiConfiguration.isTransparency())),
	smoothMovement(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_SMOOTH_MOVEMENT_STRING, SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING, uiConfiguration.isSmoothMovements())),
	showDebug(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_SHOW_DEBUG_STRING, SETTING_SHOW_DEBUG_TOOLTIP_STRING, efConfiguration.isShowDebug())),
	desiredFramerate(new UI_NumberField(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5,5), ARRANGE_LEFT,  EF_Configuration::MIN_DESIRED_FRAMERATE, EF_Configuration::MAX_DESIRED_FRAMERATE, SETTING_DESIRED_FRAMERATE_STRING, SETTING_DESIRED_FRAMERATE_TOOLTIP_STRING, 1, efConfiguration.getDesiredFramerate())),
 	desiredCPU(new UI_NumberField(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5,5), ARRANGE_LEFT,  EF_Configuration::MIN_CPU_USAGE, EF_Configuration::MAX_CPU_USAGE, SETTING_DESIRED_CPU_USAGE_STRING, SETTING_DESIRED_CPU_USAGE_TOOLTIP_STRING, 1, efConfiguration.getDesiredCPU(), PERCENT_NUMBER_TYPE)),

	minimalistButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, TAB_BUTTON_MODE, SETTING_MINIMALIST_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	fullButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, TAB_BUTTON_MODE, SETTING_FULL_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	customButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, TAB_BUTTON_MODE, SETTING_CUSTOM_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	
	reloadFromFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, PRESS_BUTTON_MODE, SETTING_RELOAD_FROM_FILE_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	loadFailsafeDefaultsButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, PRESS_BUTTON_MODE, SETTING_LOAD_FAILSAFE_DEFAULTS_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	saveToFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, PRESS_BUTTON_MODE, SETTING_SAVE_TO_FILE_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	
	languageMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_LANGUAGE_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	resolutionMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_RESOLUTION_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	bitDepthMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_BITDEPTH_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	themeMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_THEME_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),

	languageMenu(new LanguageMenu(this, Rect(Point(0, 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST)),
	resolutionMenu(new ResolutionMenu(this, Rect(Point(0, 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST)),
	bitDepthMenu(new BitDepthMenu(this, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST)),
	themeMenu(new ThemeMenu(this, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 20), Size(50, 0)), Size(0, 0), DO_NOT_ADJUST)),

	languageHasChanged(false),
	resolutionHasChanged(false),
	bitDepthHasChanged(false),
	themeHasChanged(false),
	fullScreenHasChanged(false)
{

//	reloadFromFile();

	defaultSettingsRadio->addButton(minimalistButton, 0);
	defaultSettingsRadio->addButton(fullButton, 1);
	defaultSettingsRadio->addButton(customButton, 2);

	uiSettingsRadio->addButton(languageMenuButton, 0);
	uiSettingsRadio->addButton(resolutionMenuButton, 1);
	uiSettingsRadio->addButton(bitDepthMenuButton, 2);
	uiSettingsRadio->addButton(themeMenuButton, 3);

	defaultSettingsRadio->calculateBoxSize();
	coreSettings->calculateBoxSize();
	guiSettings->calculateBoxSize();
	uiSettingsRadio->calculateBoxSize(true);
	loadSaveSettings->calculateBoxSize();

	languageMenu->setPositionParent(languageMenuButton);
	resolutionMenu->setPositionParent(resolutionMenuButton);
	bitDepthMenu->setPositionParent(bitDepthMenuButton);
	themeMenu->setPositionParent(themeMenuButton);

	customButton->updateToolTip(SETTING_CUSTOM_TOOLTIP_STRING);
	fullButton->updateToolTip(SETTING_FULL_TOOLTIP_STRING);
	minimalistButton->updateToolTip(SETTING_MINIMALIST_TOOLTIP_STRING);
	defaultSettingsRadio->forcePress(2);

	languageMenu->close();
	resolutionMenu->close();
	bitDepthMenu->close();
	themeMenu->close();
}

void SettingsWindow::reloadOriginalSize()
{
	setOriginalRect(theme.lookUpGlobalRect(SETTINGS_WINDOW));
	setMaxHeight(theme.lookUpGlobalMaxHeight(SETTINGS_WINDOW));

	languageMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	resolutionMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	bitDepthMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	themeMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	
	reloadFromFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	loadFailsafeDefaultsButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	saveToFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));

	minimalistButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	fullButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	customButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));

	UI_Window::reloadOriginalSize();

/*	defaultSettingsRadio->calculateBoxSize();
	coreSettings->calculateBoxSize();
	guiSettings->calculateBoxSize();
	uiSettingsRadio->calculateBoxSize(true);
	loadSaveSettings->calculateBoxSize();*/

}


SettingsWindow::~SettingsWindow()
{
	delete maxTime;
	delete maxLength;
	delete maxRuns;
	delete maxGenerations;
	delete maxTimeOut;
	delete breedFactor;
//	delete crossingOver;
	delete desiredFramerate;
	delete desiredCPU;

	delete restrictSC;
	delete facilityMode;
	delete autoSaveRuns;
//	delete preprocessBuildorder;
//	delete allowGoalAdaption;
	delete glowingButtons;
	delete dnaSpiral;
	delete backgroundBitmap;
	delete fullscreen;
	delete tooltips;
//	delete softwareMouse;
	delete unloadGraphics;
//	delete transparency;
	delete smoothMovement;
	delete showDebug;
	

	delete minimalistButton;
	delete fullButton;
	delete customButton;
	delete defaultSettingsRadio;
	delete reloadFromFileButton;
	delete loadFailsafeDefaultsButton;
	delete saveToFileButton;

	delete resolutionMenu;
	delete resolutionMenuButton;
	delete languageMenu;
	delete languageMenuButton;
	delete bitDepthMenu;
	delete bitDepthMenuButton;
	delete themeMenu;
	delete themeMenuButton;
	
	delete loadSaveSettings;
	delete coreSettings;
	delete guiSettings;
	delete uiSettingsRadio;
	
}

void SettingsWindow::resetData()
{ }

void SettingsWindow::resetDataChange()
{
	languageHasChanged=false;
	bitDepthHasChanged=false;
	resolutionHasChanged=false;
	themeHasChanged=false;
	fullScreenHasChanged=false;
}

void SettingsWindow::closeMenus()
{
	if(languageMenu->isOpen()||resolutionMenu->isOpen()||bitDepthMenu->isOpen()||themeMenu->isOpen())
	{
		languageMenu->close();
		resolutionMenu->close();
		bitDepthMenu->close();
		themeMenu->close();
		
		setNeedRedrawNotMoved();
	}
}

void SettingsWindow::process()
{

	if(!isShown())
		return;
	UI_Window::process();

	defaultSettingsRadio->calculateBoxSize();
	coreSettings->calculateBoxSize();
	guiSettings->calculateBoxSize();
	uiSettingsRadio->calculateBoxSize(true);
	loadSaveSettings->calculateBoxSize();

	if((coreSettings->checkForNeedRedraw())||(guiSettings->checkForNeedRedraw())||(uiSettingsRadio->checkForNeedRedraw())||(loadSaveSettings->checkForNeedRedraw())||(defaultSettingsRadio->checkForNeedRedraw()))
		setNeedRedrawMoved();

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
	
	if(languageMenu->getPressedItem()>=0)
	{
		eLanguage newLanguage = (eLanguage)(languageMenu->getPressedItem()+1);
		if(newLanguage!=UI_Object::theme.getLanguage())
		{
			UI_Object::theme.setLanguage(newLanguage);
			uiConfiguration.setLanguage(UI_Object::theme.getLanguage());
			languageHasChanged = true;
		}
		languageMenuButton->forceUnpress();
		languageMenu->close();
		setNeedRedrawNotMoved();
	} 
	else if(resolutionMenu->getPressedItem()>=0)
	{
		eResolution newResolution = (eResolution)(resolutionMenu->getPressedItem()+1);
		if(newResolution != UI_Object::theme.getResolution())
		{
			UI_Object::theme.setResolution(newResolution);
			uiConfiguration.setResolution(UI_Object::theme.getResolution());
			resolutionHasChanged = true;
		}
		resolutionMenuButton->forceUnpress();
		resolutionMenu->close();
		setNeedRedrawNotMoved();
	} 
	else if(bitDepthMenu->getPressedItem()>=0)
	{
		eBitDepth newBitDepth = (eBitDepth)bitDepthMenu->getPressedItem();
		if(newBitDepth != UI_Object::theme.getBitDepth())
		{
			UI_Object::theme.setBitDepth(newBitDepth);
			uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
			bitDepthHasChanged = true;
		}
		bitDepthMenuButton->forceUnpress();
		bitDepthMenu->close();
		setNeedRedrawNotMoved(); // to update mainwindow
	}
	else if(themeMenu->getPressedItem()>=0)
	{
		eTheme newTheme = (eTheme)(themeMenu->getPressedItem()+1);
		if(newTheme != UI_Object::theme.getMainColorTheme())
		{
			UI_Object::theme.setMainColorTheme(newTheme);
			uiConfiguration.setTheme(UI_Object::theme.getMainColorTheme());
			themeHasChanged = true;
		}
		themeMenuButton->forceUnpress();
		themeMenu->close();
		setNeedRedrawNotMoved(); // to update mainwindow
	}

	bool packet_change = false;
	if(minimalistButton->isLeftClicked())
	{
//		desiredFramerate->updateNumber ( 10 ); ~TODO
//		desiredCPU->updateNumber ( 50 );
		glowingButtons->check ( false );
		dnaSpiral->check ( false );
		backgroundBitmap->check ( false );
		fullscreen->check ( false );
		tooltips->check ( false );
//		softwareMouse->check ( false );
		unloadGraphics->check ( true );		
//		transparency->check ( false );
		smoothMovement->check ( false );
		packet_change = true;
	} else
	if(fullButton->isLeftClicked())
	{
//		desiredFramerate->updateNumber ( 35 );
//		desiredCPU->updateNumber ( 50 );
		glowingButtons->check ( true );
		dnaSpiral->check ( true );
		backgroundBitmap->check ( true );
		fullscreen->check ( true );
		tooltips->check ( true );
		unloadGraphics->check ( false );		
//	  softwareMouse->check ( true );
//	  transparency->check ( true );
		smoothMovement->check ( true );
		packet_change = true;
	}
	

	coreConfiguration.setMaxTime ( maxTime->getNumber() );
	coreConfiguration.setMaxLength ( maxLength->getNumber() );
	coreConfiguration.setMaxGenerations ( maxGenerations->getNumber() );
	coreConfiguration.setMaxRuns ( maxRuns->getNumber() );
	coreConfiguration.setMaxTimeOut ( maxTimeOut->getNumber() );
	coreConfiguration.setBreedFactor ( breedFactor->getNumber() );
//	coreConfiguration.setCrossingOver ( crossingOver->getNumber() );


	if(	uiConfiguration.setGlowingButtons ( glowingButtons->isChecked()) || 
		efConfiguration.setDnaSpiral ( dnaSpiral->isChecked()) || 
		efConfiguration.setBackgroundBitmap ( backgroundBitmap->isChecked()) ||
		(fullScreenHasChanged = efConfiguration.setFullScreen ( fullscreen->isChecked() )) || 
		efConfiguration.setToolTips ( tooltips->isChecked() ) ||
//		efConfiguration.setSoftwareMouse ( softwareMouse->isChecked() ) ||
//		uiConfiguration.setTransparency ( transparency->isChecked() ) || 
		uiConfiguration.setSmoothMovements ( smoothMovement->isChecked() ) )
		{
			// TODO
			if(!packet_change)
				defaultSettingsRadio->forcePress(2); // custombutton
			setNeedRedrawMoved();
		}
	uiConfiguration.setUnloadGraphics( unloadGraphics->isChecked());
	
	if(efConfiguration.setShowDebug( showDebug->isChecked()))
		setNeedRedrawMoved();
	efConfiguration.setDesiredFramerate ( desiredFramerate->getNumber() );
	efConfiguration.setDesiredCPU ( desiredCPU->getNumber() );
	efConfiguration.setRestrictSC( restrictSC->isChecked() );
	efConfiguration.setFacilityMode( facilityMode->isChecked() );
	efConfiguration.setAutoSaveRuns ( autoSaveRuns->isChecked() );

//	coreConfiguration.setPreprocessBuildOrder ( preprocessBuildorder->isChecked() );
//	coreConfiguration.setAllowGoalAdaption ( allowGoalAdaption->isChecked() );

	if(reloadFromFileButton->isLeftClicked())
		reloadFromFile();
	if(loadFailsafeDefaultsButton->isLeftClicked())
		loadFailsafeDefaults();
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
	efConfiguration.loadConfigurationFile();
	uiConfiguration.loadConfigurationFile();
	coreConfiguration.loadConfigurationFile();
	updateItems();
}

// use failsafe default settings
void SettingsWindow::loadFailsafeDefaults()
{
	uiConfiguration.initDefaults();
	efConfiguration.initDefaults();
	coreConfiguration.initDefaults();
	updateItems();
}

void SettingsWindow::updateItems()
{
	maxTime->updateNumber ( coreConfiguration.getMaxTime() );
	maxLength->updateNumber ( coreConfiguration.getMaxLength() );
	maxGenerations->updateNumber ( coreConfiguration.getMaxGenerations() );
	maxRuns->updateNumber ( coreConfiguration.getMaxRuns() );
	maxTimeOut->updateNumber ( coreConfiguration.getMaxTimeOut());
	breedFactor->updateNumber ( coreConfiguration.getBreedFactor() );
//	crossingOver->updateNumber ( coreConfiguration.getCrossingOver() );
	desiredFramerate->updateNumber ( efConfiguration.getDesiredFramerate() );
	desiredCPU->updateNumber ( efConfiguration.getDesiredCPU() );
	
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

	restrictSC->check ( efConfiguration.isRestrictSC() );
	facilityMode->check ( efConfiguration.isFacilityMode() );
	autoSaveRuns->check ( efConfiguration.isAutoSaveRuns() );
//	preprocessBuildorder->check ( coreConfiguration.isPreprocessBuildOrder() );
//	allowGoalAdaption->check ( coreConfiguration.isAllowGoalAdaption() );
	glowingButtons->check ( uiConfiguration.isGlowingButtons() );
	dnaSpiral->check ( efConfiguration.isDnaSpiral() );
	backgroundBitmap->check ( efConfiguration.isBackgroundBitmap() );
	if(fullscreen->isChecked() != efConfiguration.isFullScreen())
		fullScreenHasChanged=true;
	fullscreen->check ( efConfiguration.isFullScreen() );
	tooltips->check ( efConfiguration.isToolTips() );
	unloadGraphics->check( uiConfiguration.isUnloadGraphics() );
//	softwareMouse->check ( efConfiguration.isSoftwareMouse() );
//	transparency->check ( uiConfiguration.isTransparency() );
	smoothMovement->check ( uiConfiguration.isSmoothMovements() );
	showDebug->check(efConfiguration.isShowDebug() );
}

void SettingsWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
}

void SettingsWindow::forceFullScreenChange()
{
	fullScreenHasChanged = true;
}

void SettingsWindow::forceResolutionChange()
{
	resolutionHasChanged = true;
}

void SettingsWindow::forceBitDepthChange()
{
	bitDepthHasChanged = true;
}

void SettingsWindow::forceLanguageChange()
{
	languageHasChanged=true;
}


