#include "setwindow.hpp"

SettingsWindow::SettingsWindow(UI_Object* parent):
	UI_Window(parent, SETTINGS_WINDOW_TITLE_STRING, SETTINGS_WINDOW, 0, SCROLLED)
{
	coreSettings = new UI_Group(this, Rect(Point(20, 40), Size(180, 120)), "Core settings");
	guiSettings = new UI_Group(this, Rect(Point(20, 205), Size(180, 105)), "GUI settings");
	defaultSettings = new UI_Group(this, Rect(Point(20, getHeight() - 42), Size(180, 13)), "Predefined settings");

	maxTime = new NumberField(coreSettings, Rect(Point(0, 0), Size(200, 15)), MIN_TIME, MAX_TIME, 6, configuration.getMaxTime(), SETTING_MAX_TIME_STRING, SETTING_MAX_TIME_TOOLTIP_STRING, TIME_NUMBER_TYPE);
	maxLength = new NumberField(coreSettings, Rect(Point(0, 15), Size(200, 15)), MIN_LENGTH, MAX_LENGTH, 1, configuration.getMaxLength(), SETTING_MAX_LENGTH_STRING, SETTING_MAX_LENGTH_TOOLTIP_STRING);
	maxRuns = new NumberField(coreSettings, Rect(Point(0, 30), Size(200, 15)), MIN_RUNS, MAX_RUNS, 1, configuration.getMaxRuns(), SETTING_MAX_RUNS_STRING, SETTING_MAX_RUNS_TOOLTIP_STRING);
	maxGenerations = new NumberField(coreSettings, Rect(Point(0, 45), Size(200, 15)), MIN_GENERATIONS, MAX_GENERATIONS, 10, configuration.getMaxGenerations(), SETTING_MAX_GENERATIONS_STRING, SETTING_MAX_GENERATIONS_TOOLTIP_STRING);
	maxTimeOut = new NumberField(coreSettings, Rect(Point(0, 60), Size(200, 15)), MIN_TIMEOUT, MAX_TIMEOUT, 1, configuration.getMaxTimeOut(), SETTING_MAX_TIMEOUT_STRING, SETTING_MAX_TIMEOUT_TOOLTIP_STRING);
	breedFactor = new NumberField(coreSettings, Rect(Point(0, 75), Size(200, 15)), MIN_BREED_FACTOR, MAX_BREED_FACTOR, 1, configuration.getBreedFactor(), SETTING_BREED_FACTOR_STRING, SETTING_BREED_FACTOR_TOOLTIP_STRING, PERCENT_NUMBER_TYPE);
	crossingOver = new NumberField(coreSettings, Rect(Point(0, 90), Size(200, 15)), MIN_CROSSING_OVER, MAX_CROSSING_OVER, 1, configuration.getCrossingOver(), SETTING_CROSSING_OVER_STRING, SETTING_CROSSING_OVER_TOOLTIP_STRING, PERCENT_NUMBER_TYPE);
	preprocessBuildorder = new CheckButton(coreSettings, Rect(Point(0, 105), Size(200, 15)), SETTING_PREPROCESS_BUILDORDER_STRING, SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING, configuration.isPreprocessBuildOrder());
	allowGoalAdaption = new CheckButton(coreSettings, Rect(Point(0, 120), Size(200, 15)), SETTING_ALLOW_GOAL_ADAPTION_STRING, SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING, configuration.isAllowGoalAdaption());


	glowingButtons = new CheckButton(guiSettings, Rect(Point(0, 0), Size(200, 15)), SETTING_GLOWING_BUTTONS_STRING, SETTING_GLOWING_BUTTONS_TOOLTIP_STRING, configuration.isGlowingButtons());
	backgroundBitmap = new CheckButton(guiSettings, Rect(Point(0, 15), Size(200, 15)), SETTING_BACKGROUND_BITMAP_STRING, SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING, configuration.isBackgroundBitmap());
	fullscreen = new CheckButton(guiSettings, Rect(Point(0, 30), Size(200, 15)), SETTING_FULLSCREEN_STRING, SETTING_FULLSCREEN_TOOLTIP_STRING, configuration.isFullScreen());
	tooltips = new CheckButton(guiSettings, Rect(Point(0, 45), Size(200, 15)), SETTING_TOOLTIPS_STRING, SETTING_TOOLTIPS_TOOLTIP_STRING, configuration.isTooltips());
	transparency = new CheckButton(guiSettings, Rect(Point(0, 60), Size(200, 15)), SETTING_TRANSPARENCY_STRING, SETTING_TRANSPARENCY_TOOLTIP_STRING, configuration.isTransparency());
	smoothMovement = new CheckButton(guiSettings, Rect(Point(0, 75), Size(200, 15)), SETTING_SMOOTH_MOVEMENT_STRING, SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING, configuration.isSmoothMovements());
	allowStaticFramerate = new CheckButton(guiSettings, Rect(Point(0, 90), Size(200, 15)), SETTING_ALLOW_STATIC_FRAMERATE_STRING, SETTING_ALLOW_STATIC_FRAMERATE_TOOLTIP_STRING, configuration.isAllowStaticFramerate());
	staticFramerate = new NumberField(guiSettings, Rect(Point(0, 105), Size(200, 15)), MIN_STATIC_FRAMERATE, MAX_STATIC_FRAMERATE, 1, configuration.getStaticFramerate(), SETTING_STATIC_FRAMERATE_STRING, SETTING_STATIC_FRAMERATE_TOOLTIP_STRING);
	dynamicFramerate = new NumberField(guiSettings, Rect(Point(0, 105), Size(200, 15)), MIN_DYNAMIC_FRAMERATE, MAX_DYNAMIC_FRAMERATE, 1, configuration.getDynamicFramerate(), SETTING_DYNAMIC_FRAMERATE_STRING, SETTING_DYNAMIC_FRAMERATE_TOOLTIP_STRING);

	minimalistButton =  new UI_Button(this, Rect(Point(0,3), Size(200, 15)), Rect(Point(250,215), Size(200, 15)), SETTING_MINIMALIST_STRING, SETTING_MINIMALIST_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
    minimalistButton->updateToolTip(SETTING_MINIMALIST_TOOLTIP_STRING);

	fullButton = new UI_Button(this, Rect(Point(0,3), Size(200, 15)), Rect(Point(250,215), Size(200, 15)), SETTING_FULL_STRING, SETTING_FULL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
    fullButton->updateToolTip(SETTING_FULL_TOOLTIP_STRING);
	
	customButton = new UI_Button(this, Rect(Point(0,3), Size(200, 15)), Rect(Point(250,215), Size(200, 15)), SETTING_CUSTOM_STRING, SETTING_CUSTOM_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
    customButton->updateToolTip(SETTING_CUSTOM_TOOLTIP_STRING);
	graphicRadio = new UI_Radio(defaultSettings);
	graphicRadio->addButton(minimalistButton);
	graphicRadio->addButton(fullButton);
	graphicRadio->addButton(customButton);

	reloadFromFileButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_RELOAD_FROM_FILE_STRING, SETTING_RELOAD_FROM_FILE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	loadFailsafeDefaultsButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_LOAD_FAILSAFE_DEFAULTS_STRING, SETTING_LOAD_FAILSAFE_DEFAULTS_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	saveToFileButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_SAVE_TO_FILE_STRING, SETTING_SAVE_TO_FILE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);


/*	resolutionMenuButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_RESOLUTION_STRING, SETTING_RESOLUTION_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	resolutionMenu = new ResolutionMenu(this, Rect(Point(230,50),Size(200,100)));
	resolutionMenu->Hide();

	themeMenuButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_THEME_STRING, SETTING_THEME_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	themeMenu = new ThemeMenu(this, Rect(Point(230,50),Size(200,100)));
	themeMenu->Hide();

	languageMenuButton = new UI_Button(this, getRelativeRect(), Rect(Point(250,215), Size(200, 15)), SETTING_LANGUAGE_STRING, SETTING_LANGUAGE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	languageMenu = new LanguageMenu(this, Rect(Point(230,50),Size(200,100)));
	languageMenu->Hide();

    menuRadio = new UI_Radio(this);
	menuRadio->addButton(resolutionMenuButton);
    menuRadio->addButton(themeMenuButton);
    menuRadio->addButton(languageMenuButton);*/
	reloadFromFile();

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
    delete crossingOver;
	delete staticFramerate;
	delete dynamicFramerate;

	delete preprocessBuildorder;
	delete allowGoalAdaption;
	delete allowStaticFramerate;
	delete glowingButtons;
	delete backgroundBitmap;
	delete fullscreen;
	delete tooltips;
	delete transparency;
	delete smoothMovement;
	

	delete minimalistButton;
	delete fullButton;
	delete customButton;
	delete graphicRadio;
	delete reloadFromFileButton;
	delete loadFailsafeDefaultsButton;
	delete saveToFileButton;

/*	delete resolutionMenu;
	delete resolutionMenuButton;
	delete themeMenu;
	delete themeMenuButton;
	delete languageMenu;
	delete languageMenuButton;
	delete menuRadio;*/
	
	delete defaultSettings;
	delete coreSettings;
	delete guiSettings;
}

void SettingsWindow::process()
{
	if(!isShown())
		return;
	UI_Window::process();

/*	if(languageMenuButton->isLeftClicked())
	{
		languageMenu->open();
		if(!languageMenu->isOpen())
			languageMenu->Hide();
		else languageMenu->Show();
	} else
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
	}

	if(languageMenu->getPressedItem()>=0)
		UI_Object::theme.setLanguage((eLanguage)(languageMenu->getPressedItem()+1));*/

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
	
	configuration.setMaxTime ( maxTime->getNumber() );
	configuration.setMaxLength ( maxLength->getNumber() );
	configuration.setMaxRuns ( maxRuns->getNumber() );
	configuration.setMaxTimeOut ( maxTimeOut->getNumber() );
	configuration.setBreedFactor ( breedFactor->getNumber() );
	configuration.setCrossingOver ( crossingOver->getNumber() );
	configuration.setStaticFramerate ( staticFramerate->getNumber() );
	configuration.setDynamicFramerate ( dynamicFramerate->getNumber() );
	configuration.setPreprocessBuildOrder ( preprocessBuildorder->isChecked() );
	configuration.setAllowGoalAdaption ( allowGoalAdaption->isChecked() );
	configuration.setAllowStaticFramerate ( allowStaticFramerate->isChecked() );
	configuration.setGlowingButtons ( glowingButtons->isChecked() );
	configuration.setBackgroundBitmap ( backgroundBitmap->isChecked() );
	configuration.setFullScreen ( fullscreen->isChecked() );
	configuration.setTooltips ( tooltips->isChecked() );
	configuration.setTransparency ( transparency->isChecked() );
	configuration.setSmoothMovements ( smoothMovement->isChecked() );

	if(reloadFromFileButton->isLeftClicked())
		reloadFromFile();
	if(loadFailsafeDefaultsButton->isLeftClicked())
		loadFailsafeDefaults();
	if(saveToFileButton->isLeftClicked())
		configuration.saveToFile();

	if(minimalistButton->isLeftClicked())
	{
	    staticFramerate->updateNumber ( 10 );
	    dynamicFramerate->updateNumber ( 1 );
    	allowStaticFramerate->check ( false );
	    glowingButtons->check ( false );
    	backgroundBitmap->check ( false );
	    fullscreen->check ( false );
    	tooltips->check ( false );
    	transparency->check ( false );
	    smoothMovement->check ( false );
	} else
	if(fullButton->isLeftClicked())
	{
        staticFramerate->updateNumber ( 35 );
		dynamicFramerate->updateNumber ( 10 );
        allowStaticFramerate->check ( true );
        glowingButtons->check ( true );
        backgroundBitmap->check ( true );
        fullscreen->check ( true );
        tooltips->check ( true );
        transparency->check ( true );
        smoothMovement->check ( true );
		// TODO disable...
	}		
}

// reload from file
void SettingsWindow::reloadFromFile()
{
	configuration.loadConfigurationFile();
	updateItems();
}

// use failsafe default settings
void SettingsWindow::loadFailsafeDefaults()
{
	configuration.initDefaults();
	updateItems();
}

void SettingsWindow::updateItems()
{
    maxTime->updateNumber ( configuration.getMaxTime() );
    maxLength->updateNumber ( configuration.getMaxLength() );
    maxRuns->updateNumber ( configuration.getMaxRuns() );
    maxTimeOut->updateNumber ( configuration.getMaxTimeOut());
    breedFactor->updateNumber ( configuration.getBreedFactor() );
    crossingOver->updateNumber ( configuration.getCrossingOver() );
    staticFramerate->updateNumber ( configuration.getStaticFramerate() );
    dynamicFramerate->updateNumber ( configuration.getDynamicFramerate() );
    preprocessBuildorder->check ( configuration.isPreprocessBuildOrder() );
    allowGoalAdaption->check ( configuration.isAllowGoalAdaption() );
    allowStaticFramerate->check ( configuration.isAllowStaticFramerate() );
    glowingButtons->check ( configuration.isGlowingButtons() );
    backgroundBitmap->check ( configuration.isBackgroundBitmap() );
    fullscreen->check ( configuration.isFullScreen() );
    tooltips->check ( configuration.isTooltips() );
    transparency->check ( configuration.isTransparency() );
    smoothMovement->check ( configuration.isSmoothMovements() );
}

void SettingsWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
	if(fullscreen->isClicked())
		SDL_WM_ToggleFullScreen(dc->GetSurface());
}

