#include "introwindow.hpp"

std::string intro_string = "`$Welcome to Evolution Forge " + CORE_VERSION + " :)`$##\
~CLAWSOFTWARE_BITMAP~\
* $What's it all about?$:#\
Evolution Forge simulates the game rules of an abstract StarCraft, Broodwar or any other similar game and calculates the fastest way to get a certain unit, building or upgrade. It uses evolutionary algorithms which builds up the build order step by step. You do not need to understand the background although I encourage you to read my articles in the help section.#\
#\	
ABOUT THE BETA TEST:$#\
- Post your ideas, discuss or report bugs at the forums at $clawsoftware.de$!#\
- Test the program on different pcs, different configurations, color settings, drivers etc and report back any crashes, bugs etc.#\
- Try out many different, especially unusual goal lists to test the core, let the program run some hours, change the settings, ...#\
#\
$(VERY) QUICKSTART:$#\
Choose the game, click and choose your race, click on + and choose some units, click on 'paused' and watch the progress. For more see the help section.#\
#\	
$USER INTERFACE:$# #\
$KEYBOARD$#\
- $SPACE$: Starts / pauses the calculation#\
- $ALT + ENTER$: switch between fullscreen and window mode#\
- $ESC$: quits the program without saving#\
#\
$Mouse$#\
- $LEFT BUTTON$: activates buttons and adds items#\
- $RIGHT BUTTON$: removes items (units) or adds a lot of items (+/- buttons) # #\
#\
$In/Output$#\
- $Saving$: Saved build orders, goals, maps and start conditions are placed in the game directory. Besides the program internal format there a html file is saved for im- and export.#\
- $Loading/Managing$: Single goals and build orders can be loaded on the main screen (in the unit list window and the build order window) while big changes can be made in the database window#\
~THE_QUESTION_BITMAP~ Have fun and I hope you will learn something :)# #\
Best regards, #\
Clemens 'Claw' Lode ##";

std::string choose_game_string = "\
$`Choose the game rules`$##\
@~TITLE_SC_BITMAP~StarCraft@0@## Ah, yes, Starcraft :)^#\
I have worked on this ruleset quite some time so it should work flawlessly, even special rules like larvas, building add-ons, canceling buildings, morphing etc. is implemented.^#\
@~TITLE_BW_BITMAP~StarCraft: Broodwar@1@ Essentially the Broodwar ruleset is the same as the StarCraft ruleset. The only difference is that you can also build the units from this expansion set. And as there were only minor changes (concerning the game rules - not the game balance) the difference is only minor. This might change in future versions when the traits of the units themselves are taken into account.^#";/*\
@~TITLE_WC3_BITMAP~WarCraft 3@2@ This is still just a 'proof of concept' implementation. Although the ruls itself are implemented there are still some special cases and that I did not program yet. Essential things like expansions, so called 'creeps' and of course a 'battle-calculator' is not part of this version.#";*/

IntroWindow::IntroWindow(UI_Object* intro_parent) :
	UI_Window(intro_parent, INTRO_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(INTRO_WINDOW), theme.lookUpGlobalMaxHeight(INTRO_WINDOW), SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 0, 1280, 1024)),			
//	text(NULL),
	text(new UI_LongText(getScrollBar(), Rect(Point(20, 30), Size(getSize()-Size(80, 60))), Size(0, 0), intro_string, BRIGHT_TEXT_COLOR, FORCE_TEXT_COLOR, LARGE_FONT, TEXT_BUTTON, VISITED_TEXT_BUTTON, getScrollBar())),
//	doneButton(NULL),
	doneButton(new UI_Button(this, Rect(0, 0, UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0), Size(20, 10), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, SAVE_BOX_OK_STRING, BOTTOM_RIGHT, HUGE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	was_initialized(false),
	gameType(0),
	done(false)
{
	setPosition(Point(getParent()->getRelativeRightBound(), getRelativeUpperBound()));
	adjustRelativeRect(theme.lookUpGlobalRect(INTRO_WINDOW));
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

	doneButton->setOriginalSize(Size(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
//	text->setOriginalSize(Size(getSize()-Size(80, 60)));
	UI_Window::reloadOriginalSize();
}

void IntroWindow::draw() const
{
	UI_Window::draw();
}

void IntroWindow::init()
{
	if(was_initialized)
		return;
	else was_initialized = true;
//	getScrollBar()->setLastItemY(200);
	getScrollBar()->setLastItemY(text->getTextHeight());
}

void IntroWindow::process()
{
	UI_Window::process();
	if(!isShown()) 
		return;

	if(doneButton->isShown())
		UI_Object::focus = doneButton;
	
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound()+10, getAbsoluteClientRectLowerBound());
	init();
	int game_type = -1;
	if(doneButton->isLeftClicked())
	{
		text->updateText(choose_game_string);
		doneButton->Hide();
		UI_Object::focus = NULL;
	} 
	else if((game_type = text->getPressed())>=0)
	{
		gameType = game_type+1;
		UI_Object::focus = NULL;
		done = true;
	}
}

