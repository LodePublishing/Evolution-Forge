#include "intro.hpp"

std::string intro_string = "$Welcome to Evolution Forge " + CORE_VERSION + " :)$##~CLAWSOFTWARE_MONO_BITMAP~ # $ABOUT THE BETA TEST:$# #- $How can I help?$# Post your ideas, discuss or report bugs at the forums at $clawsoftware.de$!#- $It's a beta test... so what do I have to do?$#Test the program on different pcs, different configurations, color settings, drivers etc and report back any crashes, bugs etc#Try out many different, especially unusual goal lists to test the core, let the program run some hours, change the settings, ...# Please do not mess with the data files, the loading routines do not take notice of every error. In the worst case the programm will crash.# # $ABOUT THE PROGRAM ITSELF:$# # - $What does this program?$#The program simulates an abstract StarCraft : Broodwar environment, calculates the time a certain build order needs and optimizes randomly created build orders to a given goal list using evolutionary algorithms.# # $USER INTERFACE:$# # $Keyboard$# - $SPACE$: deactivate drawing (less CPU usage / faster calculation)# - $ALT + ENTER$: switch between fullscreen and window mode# - $ESC$: quits the program without saving# - $PAUSE$: stop/continue calculation# # $Mouse$# - $LEFT BUTTON$: activates buttons and adds items# - $RIGHT BUTTON$: removes items (units) or adds very many items (+/- buttons) # # - $Saving/Loading$: Saved build orders are placed in output/bos/<the race>/, goals are placed in settings/goals/<the race>/ # # NOW HAVE FUN! 8-D # # Best regards, # Clemens Lode ## ~TITLE_SC_BITMAP~ # ~TITLE_BW_BITMAP~ ##";

IntroWindow::IntroWindow(UI_Object* intro_parent) :
	UI_Window(intro_parent, INTRO_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(INTRO_WINDOW), theme.lookUpGlobalMaxHeight(INTRO_WINDOW), SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 20, 1280, 1024)),			
	text(new UI_LongText(getScrollBar(), Rect(Point(20, 30), Size(getSize()-Size(80, 60))), Size(0, 0), intro_string, BRIGHT_TEXT_COLOR, FORCE_TEXT_COLOR, LARGE_FONT, TEXT_BUTTON, VISITED_TEXT_BUTTON, getScrollBar())),
	doneButton(new UI_Button(this, Rect(0, 0, 50, 0), Size(20, 10), MY_BUTTON, false, PRESS_BUTTON_MODE, SAVE_BOX_OK_STRING, BOTTOM_RIGHT, HUGE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	was_initialized(false)
{
	setPosition(Point(UI_Object::max_x, getRelativeUpperBound()));
	adjustRelativeRect(theme.lookUpGlobalRect(INTRO_WINDOW));
	UI_Object::focus = doneButton;
}

IntroWindow::~IntroWindow()
{
	delete doneButton;
	delete text;
}

void IntroWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(INTRO_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(INTRO_WINDOW));
	text->setOriginalSize(Size(getSize()-Size(80, 60)));
	UI_Window::reloadOriginalSize();
}

void IntroWindow::draw(DC* dc) const
{
	if(!isShown()) return;
	UI_Window::draw(dc);
}

void IntroWindow::init()
{
	if(was_initialized)
		return;
	else was_initialized = true;
	getScrollBar()->setLastItemY(text->getTextHeight());
}

void IntroWindow::process()
{
	if(!isShown()) return;
	
	UI_Window::process();
	init();
	
	if(doneButton->isLeftClicked())
	{
		Hide();
		UI_Object::focus = NULL;
	}
	
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound()+10, getAbsoluteClientRectLowerBound());
}

