#include "setwindow.hpp"

#include "../core/configuration.hpp"
#include "../ui/configuration.hpp"
#include "configuration.hpp"

SettingsWindow::SettingsWindow(UI_Object* setwindow_parent):
	UI_Window(setwindow_parent, SETTINGS_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(SETTINGS_WINDOW), theme.lookUpGlobalMaxHeight(SETTINGS_WINDOW), SCROLLED),
	coreSettings(new UI_Group(this, Point(20, 50), SETWINDOW_CORE_SETTINGS_STRING)),
	guiSettings(new UI_Group(this, Point(20, 200), SETWINDOW_GUI_SETTINGS_STRING)),
	loadSaveSettings(new UI_Group(this, Point(245, getHeight() - 81), SETWINDOW_GUI_SETTINGS_STRING/*SETWINDOW_LOADSAVE_SETTINGS_STRING*/)),
	
	maxTime(new UI_NumberField(coreSettings, Rect(Point(20, 0), Size(200, 15)), MIN_TIME, MAX_TIME, 6, coreConfiguration.getMaxTime(), SETTING_MAX_TIME_STRING, SETTING_MAX_TIME_TOOLTIP_STRING, TIME_NUMBER_TYPE)),
	maxLength(new UI_NumberField(coreSettings, Rect(Point(20, 15), Size(200, 15)), MIN_LENGTH, MAX_LENGTH, 1, coreConfiguration.getMaxLength(), SETTING_MAX_LENGTH_STRING, SETTING_MAX_LENGTH_TOOLTIP_STRING)),
	maxRuns(new UI_NumberField(coreSettings, Rect(Point(20, 30), Size(200, 15)), MIN_RUNS, MAX_RUNS, 1, coreConfiguration.getMaxRuns(), SETTING_MAX_RUNS_STRING, SETTING_MAX_RUNS_TOOLTIP_STRING)),
	maxGenerations(new UI_NumberField(coreSettings, Rect(Point(20, 45), Size(200, 15)), MIN_GENERATIONS, MAX_GENERATIONS, 10, coreConfiguration.getMaxGenerations(), SETTING_MAX_GENERATIONS_STRING, SETTING_MAX_GENERATIONS_TOOLTIP_STRING)),
	maxTimeOut(new UI_NumberField(coreSettings, Rect(Point(20, 60), Size(200, 15)), MIN_TIMEOUT, MAX_TIMEOUT, 1, coreConfiguration.getMaxTimeOut(), SETTING_MAX_TIMEOUT_STRING, SETTING_MAX_TIMEOUT_TOOLTIP_STRING)),
	breedFactor(new UI_NumberField(coreSettings, Rect(Point(20, 75), Size(200, 15)), MIN_BREED_FACTOR, MAX_BREED_FACTOR, 1, coreConfiguration.getBreedFactor(), SETTING_BREED_FACTOR_STRING, SETTING_BREED_FACTOR_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),
	//crossingOver(new UI_NumberField(coreSettings, Rect(Point(20, 90), Size(200, 15)), MIN_CROSSING_OVER, MAX_CROSSING_OVER, 1, coreConfiguration.getCrossingOver(), SETTING_CROSSING_OVER_STRING, SETTING_CROSSING_OVER_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),

	autoSaveRuns(new UI_CheckButton(coreSettings, Rect(Point(20, 90), Size(200, 15)), SETTING_AUTO_SAVE_RUNS_STRING, SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING, efConfiguration.isAutoSaveRuns())),
	alwaysBuildWorker(new UI_CheckButton(coreSettings, Rect(Point(20, 105), Size(200, 15)), SETTING_ALWAYS_BUILD_WORKER_STRING, SETTING_ALWAYS_BUILD_WORKER_TOOLTIP_STRING, coreConfiguration.isAlwaysBuildWorker())),

	restrictSC(new UI_CheckButton(guiSettings, Rect(Point(20, 0), Size(200, 15)), SETTING_RESTRICT_SC_STRING, SETTING_RESTRICT_SC_TOOLTIP_STRING, efConfiguration.isRestrictSC())),
	facilityMode(new UI_CheckButton(guiSettings, Rect(Point(20, 15), Size(200, 15)), SETTING_FACILITY_MODE_STRING, SETTING_FACILITY_MODE_TOOLTIP_STRING, efConfiguration.isFacilityMode())),
//	preprocessBuildorder(new UI_CheckButton(coreSettings, Rect(Point(20, 105), Size(200, 15)), SETTING_PREPROCESS_BUILDORDER_STRING, SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING, coreConfiguration.isPreprocessBuildOrder())),
//	allowGoalAdaption(new UI_CheckButton(coreSettings, Rect(Point(20, 120), Size(200, 15)), SETTING_ALLOW_GOAL_ADAPTION_STRING, SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING, coreConfiguration.isAllowGoalAdaption())),

	glowingButtons(new UI_CheckButton(guiSettings, Rect(Point(20, 30), Size(200, 15)), SETTING_GLOWING_BUTTONS_STRING, SETTING_GLOWING_BUTTONS_TOOLTIP_STRING, uiConfiguration.isGlowingButtons())),
	dnaSpiral(new UI_CheckButton(guiSettings, Rect(Point(20, 45), Size(200, 15)), SETTING_DNA_SPIRAL_STRING, SETTING_DNA_SPIRAL_TOOLTIP_STRING, efConfiguration.isDnaSpiral())),
	backgroundBitmap(new UI_CheckButton(guiSettings, Rect(Point(20, 60), Size(200, 15)), SETTING_BACKGROUND_BITMAP_STRING, SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING, efConfiguration.isBackgroundBitmap())),
	fullscreen(new UI_CheckButton(guiSettings, Rect(Point(20, 75), Size(200, 15)), SETTING_FULLSCREEN_STRING, SETTING_FULLSCREEN_TOOLTIP_STRING, efConfiguration.isFullScreen())),
	tooltips(new UI_CheckButton(guiSettings, Rect(Point(20, 90), Size(200, 15)), SETTING_TOOLTIPS_STRING, SETTING_TOOLTIPS_TOOLTIP_STRING, efConfiguration.isToolTips())),
//	softwareMouse(new UI_CheckButton(guiSettings, Rect(Point(20, 90), Size(200, 15)), SETTING_SOFTWARE_MOUSE_STRING, SETTING_SOFTWARE_MOUSE_TOOLTIP_STRING, efConfiguration.isSoftwareMouse())),
//	transparency(new UI_CheckButton(guiSettings, Rect(Point(20, 105), Size(200, 15)), SETTING_TRANSPARENCY_STRING, SETTING_TRANSPARENCY_TOOLTIP_STRING, uiConfiguration.isTransparency())),
	smoothMovement(new UI_CheckButton(guiSettings, Rect(Point(20,  105), Size(200, 15)), SETTING_SMOOTH_MOVEMENT_STRING, SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING, uiConfiguration.isSmoothMovements())),
	allowStaticFramerate(new UI_CheckButton(guiSettings, Rect(Point(20,  120), Size(200, 15)), SETTING_ALLOW_STATIC_FRAMERATE_STRING, SETTING_ALLOW_STATIC_FRAMERATE_TOOLTIP_STRING, efConfiguration.isAllowStaticFramerate())),
	staticFramerate(new UI_NumberField(guiSettings, Rect(Point(20, 135), Size(200, 15)), EF_Configuration::MIN_STATIC_FRAMERATE, EF_Configuration::MAX_STATIC_FRAMERATE, 1, efConfiguration.getStaticFramerate(), SETTING_STATIC_FRAMERATE_STRING, SETTING_STATIC_FRAMERATE_TOOLTIP_STRING)),
	dynamicFramerate(new UI_NumberField(guiSettings, Rect(Point(20, 135), Size(200, 15)), EF_Configuration::MIN_DYNAMIC_FRAMERATE, EF_Configuration::MAX_DYNAMIC_FRAMERATE, 1, efConfiguration.getDynamicFramerate(), SETTING_DYNAMIC_FRAMERATE_STRING, SETTING_DYNAMIC_FRAMERATE_TOOLTIP_STRING)),

	minimalistButton(new UI_Button(this, Rect(Point(0,0), Size(0, 15)), SETTING_MINIMALIST_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, CONSTANT_SIZE/*AUTO_SIZE_ONCE*/)),
	

	fullButton(new UI_Button(this, Rect(Point(0,0), Size(0, 15)), SETTING_FULL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, CONSTANT_SIZE/*AUTO_SIZE_ONCE*/)),
	
	customButton(new UI_Button(this, Rect(Point(0,0), Size(0, 15)), SETTING_CUSTOM_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, CONSTANT_SIZE/*AUTO_SIZE_ONCE*/)),
	
	defaultSettingsRadio(new UI_Radio(this, Point(245, 55), true, TITLE_PREDEFINED_SETTINGS_STRING)),
	
	reloadFromFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(0, 15)), SETTING_RELOAD_FROM_FILE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, CONSTANT_SIZE/*AUTO_SIZE_ONCE*/)),
	loadFailsafeDefaultsButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(0, 15)), SETTING_LOAD_FAILSAFE_DEFAULTS_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, CONSTANT_SIZE/*AUTO_SIZE_ONCE*/)),
	saveToFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(0, 15)), SETTING_SAVE_TO_FILE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, CONSTANT_SIZE/*AUTO_SIZE_ONCE*/)),
	languageMenu(new LanguageMenu(this, Rect(Point(230, 145),Size(200,100)))),
	languageMenuButton(new UI_Button(this, Rect(240, 120, 0,0), SETTING_LANGUAGE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	languageHasChanged(false)
{


/*	resolutionMenuButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_RESOLUTION_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	resolutionMenu = new ResolutionMenu(this, Rect(Point(230,50),Size(200,100)));
	resolutionMenu->Hide();

	themeMenuButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_THEME_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	themeMenu = new ThemeMenu(this, Rect(Point(230,50),Size(200,100)));
	themeMenu->Hide();*/

	languageMenu->Hide();

/*	menuRadio = new UI_Radio(this);
	menuRadio->addButton(resolutionMenuButton);
	menuRadio->addButton(themeMenuButton);
	menuRadio->addButton(languageMenuButton);*/
//	reloadFromFile();

	defaultSettingsRadio->addButton(minimalistButton, 0);
	defaultSettingsRadio->addButton(fullButton, 1);
	defaultSettingsRadio->addButton(customButton, 2);

	defaultSettingsRadio->calculateSameWidthOfButtons();
	coreSettings->calculateBoxSize();
	guiSettings->calculateBoxSize();
	loadSaveSettings->calculateSameWidthOfButtons();

	customButton->updateToolTip(SETTING_CUSTOM_TOOLTIP_STRING);
	fullButton->updateToolTip(SETTING_FULL_TOOLTIP_STRING);
	minimalistButton->updateToolTip(SETTING_MINIMALIST_TOOLTIP_STRING);
	customButton->forcePress();
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
	delete staticFramerate;
	delete dynamicFramerate;

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
	
}

void SettingsWindow::resetData()
{}

void SettingsWindow::process()
{
	if(!isShown())
		return;
	UI_Window::process();


	if(languageMenuButton->isLeftClicked())
	{
		languageMenu->open();

		if(!languageMenu->isOpen())
			languageMenu->close();
		else
		{
			languageMenu->close();
			languageMenu->open();
		}
		languageMenu->setNeedRedrawMoved();
	} 
	/*else
	if(resolutionMenuButton->isLeftClicked())
	{
		resolutionMenu->open();
		if(!resolutionMenu->isOpen())
			resolutionMenu->Hide();
		else resolutionMenu->Show();
	} else
	if(themeMenuButton->isLeftClicked())
	{
		themeMenu->open();
		if(!themeMenu->isOpen())
			themeMenu->Hide();
		else themeMenu->Show();
	}*/

	if(languageMenu->getPressedItem()>=0)
	{
		eLanguage newLanguage = (eLanguage)(languageMenu->getPressedItem()+1);
		languageHasChanged=false;
		if(newLanguage!=UI_Object::theme.getLanguage())
		{
			UI_Object::theme.setLanguage((eLanguage)(languageMenu->getPressedItem()+1));
			uiConfiguration.setLanguage(UI_Object::theme.getLanguage());
			languageHasChanged=true;
		}
		languageMenuButton->forceUnpress();
	}

	if(!allowStaticFramerate->isChecked())
	{
		staticFramerate->Hide();
		dynamicFramerate->Show();
	}
	else 
	{
		staticFramerate->Show();
		dynamicFramerate->Hide();
	}
	
	coreConfiguration.setMaxTime ( maxTime->getNumber() );
	coreConfiguration.setMaxLength ( maxLength->getNumber() );
	coreConfiguration.setMaxGenerations ( maxGenerations->getNumber() );
	coreConfiguration.setMaxRuns ( maxRuns->getNumber() );
	coreConfiguration.setMaxTimeOut ( maxTimeOut->getNumber() );
	coreConfiguration.setBreedFactor ( breedFactor->getNumber() );
//	coreConfiguration.setCrossingOver ( crossingOver->getNumber() );
	efConfiguration.setStaticFramerate ( staticFramerate->getNumber() );
	efConfiguration.setDynamicFramerate ( dynamicFramerate->getNumber() );

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
		staticFramerate->updateNumber ( 10 );
		dynamicFramerate->updateNumber ( 1 );
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
		staticFramerate->updateNumber ( 35 );
		dynamicFramerate->updateNumber ( 10 );
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
	staticFramerate->updateNumber ( efConfiguration.getStaticFramerate() );
	dynamicFramerate->updateNumber ( efConfiguration.getDynamicFramerate() );
	
	eLanguage newLanguage = uiConfiguration.getLanguage();
	languageHasChanged=false;
	if(newLanguage!=UI_Object::theme.getLanguage())
	{
		UI_Object::theme.setLanguage( newLanguage );
		uiConfiguration.setLanguage( newLanguage );
		languageHasChanged=true;
	}
	
	restrictSC->check ( efConfiguration.isRestrictSC() );
	facilityMode->check ( efConfiguration.isFacilityMode() );
	autoSaveRuns->check ( efConfiguration.isAutoSaveRuns() );
	alwaysBuildWorker->check ( coreConfiguration.isAlwaysBuildWorker() );
//  preprocessBuildorder->check ( coreConfiguration.isPreprocessBuildOrder() );
//  allowGoalAdaption->check ( coreConfiguration.isAllowGoalAdaption() );
	allowStaticFramerate->check ( efConfiguration.isAllowStaticFramerate() );
	glowingButtons->check ( uiConfiguration.isGlowingButtons() );
	dnaSpiral->check ( efConfiguration.isDnaSpiral() );
	backgroundBitmap->check ( efConfiguration.isBackgroundBitmap() );
	fullscreen->check ( efConfiguration.isFullScreen() );
	tooltips->check ( efConfiguration.isToolTips() );
//  softwareMouse->check ( efConfiguration.isSoftwareMouse() );
//  transparency->check ( uiConfiguration.isTransparency() );
	smoothMovement->check ( uiConfiguration.isSmoothMovements() );
}

void SettingsWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
	dc->setFullscreen(fullscreen->isChecked());
}

void SettingsWindow::forceLanguageChange()
{
	languageHasChanged=true;
}

const bool SettingsWindow::hasLanguageChanged() 
{
	if(languageHasChanged)
	{
		languageHasChanged=false;
		return(true);
	}
	else return(false);
}
