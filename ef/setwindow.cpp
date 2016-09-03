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
	
	maxTime(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(1.5*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 15)), Size(5,5), ARRANGE_LEFT, MIN_TIME, MAX_TIME, 6, coreConfiguration.getMaxTime(), SETTING_MAX_TIME_STRING, SETTING_MAX_TIME_TOOLTIP_STRING, TIME_NUMBER_TYPE)),
	maxLength(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(1.5*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 15)), Size(5,5), ARRANGE_LEFT,  MIN_LENGTH, MAX_LENGTH, 1, coreConfiguration.getMaxLength(), SETTING_MAX_LENGTH_STRING, SETTING_MAX_LENGTH_TOOLTIP_STRING)),
	maxRuns(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(1.5*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 15)), Size(5,5), ARRANGE_LEFT,  MIN_RUNS, MAX_RUNS, 1, coreConfiguration.getMaxRuns(), SETTING_MAX_RUNS_STRING, SETTING_MAX_RUNS_TOOLTIP_STRING)),

	maxGenerations(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(1.5*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 15)), Size(5,5), ARRANGE_LEFT,  MIN_GENERATIONS, MAX_GENERATIONS, 10, coreConfiguration.getMaxGenerations(), SETTING_MAX_GENERATIONS_STRING, SETTING_MAX_GENERATIONS_TOOLTIP_STRING)),
	maxTimeOut(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(1.5*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 15)), Size(5,5), ARRANGE_LEFT,  MIN_TIMEOUT, MAX_TIMEOUT, 1, coreConfiguration.getMaxTimeOut(), SETTING_MAX_TIMEOUT_STRING, SETTING_MAX_TIMEOUT_TOOLTIP_STRING)),
	breedFactor(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(1.5*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 15)), Size(5,5), ARRANGE_LEFT,  MIN_BREED_FACTOR, MAX_BREED_FACTOR, 1, coreConfiguration.getBreedFactor(), SETTING_BREED_FACTOR_STRING, SETTING_BREED_FACTOR_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),
	//crossingOver(new UI_NumberField(coreSettings, Rect(Point(10, 90), Size(200, 15)), Size(5, 5), ARRANGE_LEFT,  MIN_CROSSING_OVER, MAX_CROSSING_OVER, 1, coreConfiguration.getCrossingOver(), SETTING_CROSSING_OVER_STRING, SETTING_CROSSING_OVER_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),

//TODO
	autoSaveRuns(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_AUTO_SAVE_RUNS_STRING, SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING, efConfiguration.isAutoSaveRuns())),
	alwaysBuildWorker(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_ALWAYS_BUILD_WORKER_STRING, SETTING_ALWAYS_BUILD_WORKER_TOOLTIP_STRING, coreConfiguration.isAlwaysBuildWorker())),

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
//	transparency(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_TRANSPARENCY_STRING, SETTING_TRANSPARENCY_TOOLTIP_STRING, uiConfiguration.isTransparency())),
	smoothMovement(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_SMOOTH_MOVEMENT_STRING, SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING, uiConfiguration.isSmoothMovements())),
 	allowStaticFramerate(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_ALLOW_STATIC_FRAMERATE_STRING, SETTING_ALLOW_STATIC_FRAMERATE_TOOLTIP_STRING, efConfiguration.isAllowStaticFramerate())),
 //	staticFramerate(new UI_NumberField(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5,5), ARRANGE_LEFT,  EF_Configuration::MIN_STATIC_FRAMERATE, EF_Configuration::MAX_STATIC_FRAMERATE, 1, efConfiguration.getStaticFramerate(), SETTING_STATIC_FRAMERATE_STRING, SETTING_STATIC_FRAMERATE_TOOLTIP_STRING)),
 // 	dynamicFramerate(new UI_NumberField(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5,5), ARRANGE_LEFT,  EF_Configuration::MIN_DYNAMIC_FRAMERATE, EF_Configuration::MAX_DYNAMIC_FRAMERATE, 1, efConfiguration.getDynamicFramerate(), SETTING_DYNAMIC_FRAMERATE_STRING, SETTING_DYNAMIC_FRAMERATE_TOOLTIP_STRING)),

	minimalistButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), SETTING_MINIMALIST_STRING, MY_BUTTON, TAB_BUTTON_MODE, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	fullButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), SETTING_FULL_STRING, MY_BUTTON, TAB_BUTTON_MODE, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	customButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), SETTING_CUSTOM_STRING, MY_BUTTON, TAB_BUTTON_MODE, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	
	reloadFromFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), SETTING_RELOAD_FROM_FILE_STRING, MY_BUTTON, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	loadFailsafeDefaultsButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), SETTING_LOAD_FAILSAFE_DEFAULTS_STRING, MY_BUTTON, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	saveToFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), SETTING_SAVE_TO_FILE_STRING, MY_BUTTON, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	
/*	languageMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), SETTING_LANGUAGE_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	resolutionMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), SETTING_RESOLUTION_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	bitDepthMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), SETTING_BITDEPTH_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),

	languageMenu(new LanguageMenu(languageMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT)),
	resolutionMenu(new ResolutionMenu(resolutionMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT)),
	bitDepthMenu(new BitDepthMenu(bitDepthMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT)),*/
	

	languageHasChanged(false),
	resolutionHasChanged(false),
	bitDepthHasChanged(false),
	fullScreenHasChanged(false)
{

	languageMenuButton = new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), SETTING_LANGUAGE_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	resolutionMenuButton = new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), SETTING_RESOLUTION_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	bitDepthMenuButton = new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), SETTING_BITDEPTH_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);

	languageMenu = new LanguageMenu(languageMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT);
	resolutionMenu = new ResolutionMenu(resolutionMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT);
	bitDepthMenu = new BitDepthMenu(bitDepthMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT);


	
	languageMenu->Hide();
	resolutionMenu->Hide();
	bitDepthMenu->Hide();

//	reloadFromFile();


	defaultSettingsRadio->addButton(minimalistButton, 0);
	defaultSettingsRadio->addButton(fullButton, 1);
	defaultSettingsRadio->addButton(customButton, 2);

	uiSettingsRadio->addButton(languageMenuButton, 0);
	uiSettingsRadio->addButton(resolutionMenuButton, 1);
	uiSettingsRadio->addButton(bitDepthMenuButton, 2);

	defaultSettingsRadio->calculateBoxSize();
	coreSettings->calculateBoxSize();
	guiSettings->calculateBoxSize();
	uiSettingsRadio->calculateBoxSize(true);
	loadSaveSettings->calculateBoxSize();

	customButton->updateToolTip(SETTING_CUSTOM_TOOLTIP_STRING);
	fullButton->updateToolTip(SETTING_FULL_TOOLTIP_STRING);
	minimalistButton->updateToolTip(SETTING_MINIMALIST_TOOLTIP_STRING);
	customButton->forcePress();
}

void SettingsWindow::reloadOriginalSize()
{
	setOriginalRect(theme.lookUpGlobalRect(SETTINGS_WINDOW));
	setMaxHeight(theme.lookUpGlobalMaxHeight(SETTINGS_WINDOW));

	languageMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	resolutionMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	bitDepthMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	
	reloadFromFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	loadFailsafeDefaultsButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	saveToFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));

	languageMenu->setOriginalPosition(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30));
	resolutionMenu->setOriginalPosition(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30));
	bitDepthMenu->setOriginalPosition(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30));

	minimalistButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	fullButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	customButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));

	UI_Window::reloadOriginalSize();

/*defaultSettingsRadio->calculateBoxSize();
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
 //	delete staticFramerate;
 //	delete dynamicFramerate;

	delete restrictSC;
	delete facilityMode;
	delete autoSaveRuns;
	delete alwaysBuildWorker;
//	delete preprocessBuildorder;
//	delete allowGoalAdaption;
	delete allowStaticFramerate;
	delete glowingButtons;
	delete dnaSpiral;
	delete backgroundBitmap;
	delete fullscreen;
	delete tooltips;
//	delete softwareMouse;
//	delete transparency;
	delete smoothMovement;
	

	delete minimalistButton;
	delete fullButton;
	delete customButton;
	delete defaultSettingsRadio;
	delete reloadFromFileButton;
	delete loadFailsafeDefaultsButton;
	delete saveToFileButton;

/*	delete resolutionMenu;
	delete resolutionMenuButton;
	delete themeMenu;
	delete themeMenuButton;*/
	delete languageMenu;
	delete languageMenuButton;
/*	delete menuRadio;*/
	
	delete loadSaveSettings;
	delete coreSettings;
	delete guiSettings;
	delete uiSettingsRadio;
	
}

void SettingsWindow::resetData()
{}

void SettingsWindow::closeMenus()
{
	languageMenu->close();
	resolutionMenu->close();
	bitDepthMenu->close();
}

void SettingsWindow::process()
{
	languageHasChanged=false;
	bitDepthHasChanged=false;
	resolutionHasChanged=false;
	fullScreenHasChanged=false;

	if(!isShown())
		return;
	UI_Window::process();


	defaultSettingsRadio->calculateBoxSize();
	coreSettings->calculateBoxSize();
	guiSettings->calculateBoxSize();
	uiSettingsRadio->calculateBoxSize(true);
	loadSaveSettings->calculateBoxSize();


	if(uiSettingsRadio->buttonHasChanged())
	{
		switch(uiSettingsRadio->getMarked())
		{
			case 0:languageMenu->open();
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
				       
			case 1:resolutionMenu->open();
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
			       
			case 2:bitDepthMenu->open();
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
			default:break;
		}
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
	}

	if(fullscreen->isClicked())
		fullScreenHasChanged = true;

	if(!allowStaticFramerate->isChecked())
	{
 //		staticFramerate->Hide();
 //		dynamicFramerate->Show();
	}
	else 
	{
 //		staticFramerate->Show();
 //		dynamicFramerate->Hide();
	}
	
	coreConfiguration.setMaxTime ( maxTime->getNumber() );
	coreConfiguration.setMaxLength ( maxLength->getNumber() );
	coreConfiguration.setMaxGenerations ( maxGenerations->getNumber() );
	coreConfiguration.setMaxRuns ( maxRuns->getNumber() );
	coreConfiguration.setMaxTimeOut ( maxTimeOut->getNumber() );
	coreConfiguration.setBreedFactor ( breedFactor->getNumber() );
//	coreConfiguration.setCrossingOver ( crossingOver->getNumber() );
 //	efConfiguration.setStaticFramerate ( staticFramerate->getNumber() );
 //	efConfiguration.setDynamicFramerate ( dynamicFramerate->getNumber() );

	efConfiguration.setRestrictSC( restrictSC->isChecked() );
	efConfiguration.setFacilityMode( facilityMode->isChecked() );
	efConfiguration.setAutoSaveRuns ( autoSaveRuns->isChecked() );
	coreConfiguration.setAlwaysBuildWorker( alwaysBuildWorker->isChecked() );
//	coreConfiguration.setPreprocessBuildOrder ( preprocessBuildorder->isChecked() );
//	coreConfiguration.setAllowGoalAdaption ( allowGoalAdaption->isChecked() );
	efConfiguration.setAllowStaticFramerate ( allowStaticFramerate->isChecked() );
	uiConfiguration.setGlowingButtons ( glowingButtons->isChecked() );
	efConfiguration.setDnaSpiral ( dnaSpiral->isChecked() );
	efConfiguration.setBackgroundBitmap ( backgroundBitmap->isChecked() );
	if(efConfiguration.isFullScreen()!=fullscreen->isChecked())
		fullScreenHasChanged=true;
	efConfiguration.setFullScreen ( fullscreen->isChecked() ); 
	efConfiguration.setToolTips ( tooltips->isChecked() );
//	efConfiguration.setSoftwareMouse ( softwareMouse->isChecked() );
//	uiConfiguration.setTransparency ( transparency->isChecked() );
	uiConfiguration.setSmoothMovements ( smoothMovement->isChecked() );

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

	if(minimalistButton->isLeftClicked())
	{
 //		staticFramerate->updateNumber ( 10 );
 //		dynamicFramerate->updateNumber ( 1 );
		allowStaticFramerate->check ( false );
		glowingButtons->check ( false );
		dnaSpiral->check ( false );
		backgroundBitmap->check ( false );
		fullscreen->check ( false );
		tooltips->check ( false );
//		softwareMouse->check ( false );
//		transparency->check ( false );
		smoothMovement->check ( false );
	} else
	if(fullButton->isLeftClicked())
	{
 //		staticFramerate->updateNumber ( 35 );
 //		dynamicFramerate->updateNumber ( 10 );
		allowStaticFramerate->check ( true );
		glowingButtons->check ( true );
		dnaSpiral->check ( true );
		backgroundBitmap->check ( true );
		fullscreen->check ( true );
		tooltips->check ( true );
//	  softwareMouse->check ( true );
//	  transparency->check ( true );
		smoothMovement->check ( true );
		// TODO disable...
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
 //	staticFramerate->updateNumber ( efConfiguration.getStaticFramerate() );
 //	dynamicFramerate->updateNumber ( efConfiguration.getDynamicFramerate() );
	
	eLanguage newLanguage = uiConfiguration.getLanguage();
	if(newLanguage!=UI_Object::theme.getLanguage())
	{
		UI_Object::theme.setLanguage( newLanguage );
		uiConfiguration.setLanguage( newLanguage );
		languageHasChanged = true;
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
	
	restrictSC->check ( efConfiguration.isRestrictSC() );
	facilityMode->check ( efConfiguration.isFacilityMode() );
	autoSaveRuns->check ( efConfiguration.isAutoSaveRuns() );
	alwaysBuildWorker->check ( coreConfiguration.isAlwaysBuildWorker() );
//	preprocessBuildorder->check ( coreConfiguration.isPreprocessBuildOrder() );
//	allowGoalAdaption->check ( coreConfiguration.isAllowGoalAdaption() );
	allowStaticFramerate->check ( efConfiguration.isAllowStaticFramerate() );
	glowingButtons->check ( uiConfiguration.isGlowingButtons() );
	dnaSpiral->check ( efConfiguration.isDnaSpiral() );
	backgroundBitmap->check ( efConfiguration.isBackgroundBitmap() );
	if(fullscreen->isChecked() != efConfiguration.isFullScreen())
		fullScreenHasChanged=true;
	fullscreen->check ( efConfiguration.isFullScreen() );
	tooltips->check ( efConfiguration.isToolTips() );
//	softwareMouse->check ( efConfiguration.isSoftwareMouse() );
//	transparency->check ( uiConfiguration.isTransparency() );
	smoothMovement->check ( uiConfiguration.isSmoothMovements() );
}

void SettingsWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
}

void SettingsWindow::forceLanguageChange()
{
	languageHasChanged=true;
}


