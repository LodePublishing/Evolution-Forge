#include "helpwindow.hpp"
	
const std::string helpChaptersDataBase[57] = 
{
//	INDEX_CHAPTER = 0
"$Help Index$###\
@Chapter 1: Introduction@100@##\
@Chapter 2: Detailed description@200@##\
@Chapter 3: How to use the results@300@##\
@Chapter 4: Configuration@400@##\
@Chapter 4: Program Mechanics@500@##\
@Chapter 5: Interactive Mode@600@##\
@Chapter 6: The core of the program@700@##\
@Chapter 7: Communication@800@##\
@Chapter 8: Development of EF@900@##\
@Chapter 9: Philosophy and EF@1000@#",

//	INTRODUCTION_MAIN_CHAPTER = 100
"@Chapter 1: Introduction@0@###\
@Chapter 1.1: What does this program?@110@##\
@Chapter 1.2: Quickstart@120@##\
@Chapter 1.3: Overview of the elements@130@##\
@Chapter 1.4: Is it useful?@140@##\
@Chapter 1.5: Limits of the Program@150@##\
@Chapter 1.6: Problems of the Program@160@##",

// INTRODUCTION_CHAPTER = 110

"@1.1: What does this program?@100@##\
The program simulates an abstract $StarCraft:Broodwar$ environment and calculates the time a certain build order needs.#By comparing different build orders, randomly changing them and taking the best build order for the next generation, the build order is improved step by step.#$StarCraft$ build orders fit perfectly for this algorithm called $HillClimbing$.#",

//	INTRODUCTION_QUICKSTART_CHAPTER = 120
"@1.2: Quickstart@100@##\
It's easy:#\
- Click on the left side in the goal list window $'Click here to add goal'$, choose a category and pick your goals.#\
- Click on the top right on the flashing $'Click to continue'$ button and sit back to watch the progress.#\
- You will see items moving around in window named $'Build Order'$ on the bottom right. The program now will optimize the build order by rearranging the items.#\
- On the top right you will see the current best time in the $'Timer window'$. If you read there a $'Searching'$ it means, the program is still searching for a valid solution which will need a moment depending how many goals you have entered while $'Optimizing'$ means that a valid solution is found and the program tries to optimize it.#\
- On the top you will see the $'Statistical Data'$ window which displays changes in time, fitness and ressources of the best program over the last few seconds. It is interesting for the advanced user, this window is availible on $'Advanced'$ mode or higher.#\
- Evolution forge also allows you to see a graphical illustration (through the $'Graphical illustration of the build order'$ window) of the build order to see what buildings are built at the same time, how long they need and when to build them. In addition there is the $'Overview of the build order'$ window where you can see how much minerals, gas and supply you have at a certain time if you play this build order.#\
- After a while you will notice that there are no more changes on the screen. Probably the best build order is found. Press $ALT+T$ to stop and let program print out the build order via the File menu.",

//	INTRODUCTION_OVERVIEW_CHAPTER = 130
"@1.3: Overview of the elements@100@##\
TEXT  ~2~   ",

//	INTRODUCTION_USE_CHAPTER = 140
"@1.4: Is it useful?@100@##\
TEXT",
//	INTRODUCTION_LIMITS_CHAPTER = 150
"@1.5: Where are the limits?@100@##\
- It only calculates a general build order. Depending on the map, the latency, your mousespeed and many other small factors there might be a better build order.#\
- Goals consisting of many units will result in exponential growth of time needed for the calculation.#\
- The program is based on one-second steps. This might lead to some inaccurate calculation.#\
- In most cases the build orders cannot be applied directly 1:1 in the game but you have to exercise them and adapt them to your needs.",

//	INTRODUCTION_PROBLEMS_CHAPTER = 160

// TODO
"@1.6: Is it cheating?@100@##\
Well... in a way... yes.#\
If you use the program correctly you might get an advantage over your enemy: You learn to get a better starting in the game so that you can concentrate on the main thing, strategies and tactics!#\
While you can get the same information by watching replays, reading strategy articles or just playing the game it is much more fun to interactivly trying out combinations to compare them.",



//	DESCRIPTION_MAIN_CHAPTER = 200
"@2. Detailed Description@0@###\
@2.1: The timer window@210@##\
@2.2: The goal window@220@##\
@2.3: The force window@230@##\
@2.4: The build order diagram window@240@##\
@2.5: The build order graph window@250@##\
@2.6: The build order window@260@#",

//	DESCRIPTION_SCORE_WINDOW_CHAPTER = 210
"@2.1: The timer window@200@##\
TEXT",
	
//	DESCRIPTION_GOAL_WINDOW_CHAPTER = 220
"@2.2: The goal window@200@##\
TEXT",

//	DESCRIPTION_FORCE_WINDOW_CHAPTER = 230
"@2.3: The force window@200@##\
TEXT",
	
//	DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER = 240
"@2.4: The build order diagram window@200@##\
TEXT",

//	DESCRIPTION_BOGRAPH_WINDOW_CHAPTER = 250
"@2.5: The build order graph window@200@##\
TEXT",

//	DESCRIPTION_BUILD_ORDER_WINDOW_CHAPTER = 260
"@2.6: The build order window@200@##\
TEXT",

//	DESCRIPTION_FORCE_WINDOW_CHAPTER = 270
"@2.7: The map window@200@##\
TEXT",
	

// TODO
//	RESULTS_MAIN_CHAPTER = 300
"@3. How to use the results@0@###\
@3.1: About time and numbers@310@##\
@3.2: HTML output@320@##\
@3.3: Using the database@330@##\
@3.4: Results during the run@340@#",

//	RESULTS_TIME_AND_NUMBERS_CHAPTER = 310
"@3.1: About time and numbers@300@##\
TEXT",

//	RESULTS_OUTPUT_CHAPTER = 320
"@3.2: HTML output@300@##\
TEXT",

//	RESULTS_DATABASE_CHAPTER = 330
"@3.3: Using the database@300@##\
TEXT",

//	RESULTS_INGAME_CHAPTER = 340
"@3.4: Results during the run@300@##\
TEXT",


//	CONFIGURATION_MAIN_CHAPTER = 400
"@4. Configuration of the program@0@###\
@4.1: Loading/Saving Settings@410@##\
@4.2: Resolution, Color Depth and Language@420@##\
@4.3: Core Settings@430@##\
@4.4: GUI Settings@440@##\
@4.5: Rules settings@450@##",


//	CONFIGURATION_LOADING_SAVING_CHAPTER = 410
"@4.1: How the build order is processed@400@##\
TEXT",

//	CONFIGURATION_RESOLUTION_DEPTH_LANGUAGE_CHAPTER = 420
"@4.2: How multiplayer works@400@##\
TEXT",

//	CONFIGURATION_CORE_SETTINGS_CHAPTER = 430
"@4.3: How to finetune it additional@400@##\
TEXT",

//	CONFIGURATION_GUI_SETTINGS_CHAPTER = 440
"@4.4: How the Game is simulated@400@##\
TEXT",

// 	ONFIGURATION_RULES_SETTINGS_CHAPTER = 450
"@4.5: How the Game is simulated@400@##\
TEXT",


//	MECHANICS_MAIN_CHAPTER = 500
"@5. Mechanics of the program@0@###\
@5.1: How the build order is processed@510@##\
@5.2: How multiplayer works@520@##\
@5.3: How to finetune it additional@530@##\
@5.4: How the algorithm works@540@##",


//	MECHANICS_ORDER_CHAPTER = 510
"@5.1: How the build order is processed@500@##\
TEXT",

//	MECHANICS_MULTIPLAYER_CHAPTER = 420
"@5.2: How multiplayer works@500@##\
TEXT",

//	MECHANICS_FINETUNE_CHAPTER = 530
"@5.3: How to finetune it additional@500@##\
TEXT",

//	MECHANICS_SIMULATION_CHAPTER = 540
"@5.4: How the Game is simulated@500@##\
TEXT",



//	INTERACTIVE_MAIN_CHAPTER = 600
"@6. Interactive mode@0@###\
@6.1: Editing the build order@610@##\
@6.2: Playing against the computer@620@##\
@6.3: Letting it play against itself@630@##\
@6.4: What to learn from the program@640@#",
	
//	INTERACTIVE_EDITING_CHAPTER = 610
"@6.1: Editing the build order@600@##\
TEXT",

//	INTERACTIVE_PLAYING_CHAPTER = 620
"@6.2: Playing against the computer@600@##\
TEXT",

//	INTERACTIVE_TOURNAMENTS_CHAPTER = 630
"@6.3: Letting it play against itself@600@##\
TEXT",

//	INTERACTIVE_WHATTOLEARN_CHAPTER = 640
"@6.4: What to learn from the program@600@##\
TEXT",



//	CORE_MAIN_CHAPTER = 700
"@7. The core of the program@0@###\
@7.1: The algorithm of the core@710@##\
@7.2: Buildorder TODO@720@##\
@7.3: Scalability@730@##\
@7.4: Comparison?@740@##\
@7.5: Problems with the algorithm@750@#",


//	CORE_ALGORITHM_CHAPTER = 710
"@7.1: The algorithm of the core@700@##\
TEXT",

//	CORE_BUILDORDER_CHAPTER = 720
"@7.2: Buildorder TODO@700@##\
TEXT",

//	CORE_SCALABILITY_CHAPTER = 730
"@7.3: Scalability@700@##\
TEXT",

//	CORE_COMPARISON_CHAPTER = 740
"@7.4: Comparison?@700@##\
TEXT",

//	CORE_PROBLEMS_GAS_CHAPTER = 750
"@7.5: Problems with the algorithm@700@##\
TEXT",


//	COMMUNICATION_MAIN_CHAPTER = 800
"@8. Communication@0@###\
@8.1: ClawSoftware.de@810@##\
@8.3: About myself@830@#",

//	COMMUNICATION_CLAWSOFTWARE_CHAPTER = 810
"@8.1: ClawSoftware.de@800@##\
~19~TEXTb lbsdlfasdf asdf asdaaagiopjerpgjpgojdgadj kldagj ldkfjg kldfjglkdjglsd gfklsdjf gklsdjfg lskdf gjskldfjldfjg lsdkfjg lskdjf glksdj glksdj gsldkjfg lsdkjgf lskdfjg lksdjfg klsdjf glskdjfklsdj fgklsj dfgjsdlkfjg sldkjfl skdjfgl skdfjgl ksdjf gklsdj gfjkls dflgjk sdlfkgj sldkfg lkjf gsdfg askdfjasl kdflaskdfj alksjdfalejiwjerfl sdifjal sidjf liajsdf liasjdfliasjefijs lidjf alsidj flaisdj filajefiljf lisjdlifjasl idfj alsidfj aslidfj alsidfj alsidjfl asidjf ilasdj filasjdf ailsd fjasildjf asildjf lasidjf lasidjf alsidfj alsidjf alsidfj l",


//	COMMUNICATION_ABOUT_CHAPTER = 830
"@8.3: About myself@800@##\
TEXT",

//	DEVELOPMENT_MAIN_CHAPTER = 900
"@9. Development@0@###\
@9.1: History@910@##\
@9.2: Things will never be implemented@920@##\
@9.3: Things that will come later@930@##\
@9.4: Current developments@940@##\
@9.5: Future plans with Evolution Forge@950@#",

//	DEVELOPMENT_HISTORY_CHAPTER = 910
"@9.1: History@900@##\
TEXT",

//	DEVELOPMENT_NOTIMPLEMENTED_CHAPTER = 920
"@9.2: Things will never be implemented@900@##\
TEXT",

//	DEVELOPMENT_TODO_CHAPTER = 930
"@9.3: Things that will come later@900@##\
TEXT",

//	DEVELOPMENT_CURRENT_CHAPTER = 940
"@9.4: Current developments@900@##\
TEXT",

//	DEVELOPMENT_FUTURE_CHAPTER = 950
"@9.5: Future plans with Evolution Forge@900@##\
TEXT",


//	PHILOSOHPY_MAIN_CHAPTER = 1000
"@10. Philosophy@0@###\
@10.1: Evolution@1010@##\
@10.2: TODO@1020@##\
@10.3: Reality check@1030@#",

//	PHILOSOPHY_EVOLUTION_CHAPTER = 1010
"@10.1 Evolution@1000@##\
TEXT",	

//	PHILOSOPHY_GOD_CHAPTER = 1020
"@10.2 TODO@1000@##\
TEXT",

//	PHILOSOPHY_REALITY_CHAPTER = 1030
"@10.3 Reality check@1000@##\
TEXT"};
	





const std::string& getChapterString(const eHelpChapter chapter)
{
	switch(chapter)
	{
		case INDEX_CHAPTER:return(helpChaptersDataBase[0]);break;
		case INTRODUCTION_MAIN_CHAPTER:return(helpChaptersDataBase[1]);break;
		case INTRODUCTION_CHAPTER:return(helpChaptersDataBase[2]);break;
		case INTRODUCTION_QUICKSTART_CHAPTER:return(helpChaptersDataBase[3]);break;
		case INTRODUCTION_OVERVIEW_CHAPTER:return(helpChaptersDataBase[4]);break;
		case INTRODUCTION_USE_CHAPTER:return(helpChaptersDataBase[5]);break;
		case INTRODUCTION_LIMITS_CHAPTER:return(helpChaptersDataBase[6]);break;
		case INTRODUCTION_PROBLEMS_CHAPTER:return(helpChaptersDataBase[7]);break;
		case DESCRIPTION_MAIN_CHAPTER:return(helpChaptersDataBase[8]);break;
		case DESCRIPTION_SCORE_WINDOW_CHAPTER:return(helpChaptersDataBase[9]);break;
		case DESCRIPTION_GOAL_WINDOW_CHAPTER:return(helpChaptersDataBase[10]);break;
		case DESCRIPTION_FORCE_WINDOW_CHAPTER:return(helpChaptersDataBase[11]);break;
		case DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER:return(helpChaptersDataBase[12]);break;
		case DESCRIPTION_BOGRAPH_WINDOW_CHAPTER:return(helpChaptersDataBase[13]);break;
		case DESCRIPTION_BUILD_ORDER_WINDOW_CHAPTER:return(helpChaptersDataBase[14]);break;
		case DESCRIPTION_MAP_WINDOW_CHAPTER:return(helpChaptersDataBase[15]);break;
		case RESULTS_MAIN_CHAPTER:return(helpChaptersDataBase[16]);break;
		case RESULTS_TIME_AND_NUMBERS_CHAPTER:return(helpChaptersDataBase[17]);break;
		case RESULTS_OUTPUT_CHAPTER:return(helpChaptersDataBase[18]);break;
		case RESULTS_DATABASE_CHAPTER:return(helpChaptersDataBase[19]);break;
		case RESULTS_INGAME_CHAPTER:return(helpChaptersDataBase[20]);break;
		case CONFIGURATION_MAIN_CHAPTER:return(helpChaptersDataBase[21]);break;
		case CONFIGURATION_LOADING_SAVING_CHAPTER:return(helpChaptersDataBase[22]);break;
		case CONFIGURATION_RESOLUTION_DEPTH_LANGUAGE_CHAPTER:return(helpChaptersDataBase[23]);break;
		case CONFIGURATION_CORE_SETTINGS_CHAPTER:return(helpChaptersDataBase[24]);break;
		case CONFIGURATION_GUI_SETTINGS_CHAPTER:return(helpChaptersDataBase[25]);break;
		case CONFIGURATION_RULES_SETTINGS_CHAPTER:return(helpChaptersDataBase[26]);break;
		case MECHANICS_MAIN_CHAPTER:return(helpChaptersDataBase[27]);break;
		case MECHANICS_ORDER_CHAPTER:return(helpChaptersDataBase[28]);break;
		case MECHANICS_MULTIPLAYER_CHAPTER:return(helpChaptersDataBase[29]);break;
		case MECHANICS_FINETUNE_CHAPTER:return(helpChaptersDataBase[30]);break;
		case MECHANICS_SIMULATION_CHAPTER:return(helpChaptersDataBase[31]);break;
		case INTERACTIVE_MAIN_CHAPTER:return(helpChaptersDataBase[32]);break;
		case INTERACTIVE_EDITING_CHAPTER:return(helpChaptersDataBase[33]);break;
		case INTERACTIVE_PLAYING_CHAPTER:return(helpChaptersDataBase[34]);break;
		case INTERACTIVE_TOURNAMENTS_CHAPTER:return(helpChaptersDataBase[35]);break;
		case INTERACTIVE_WHATTOLEARN_CHAPTER:return(helpChaptersDataBase[36]);break;
		case CORE_MAIN_CHAPTER:return(helpChaptersDataBase[37]);break;
		case CORE_ALGORITHM_CHAPTER:return(helpChaptersDataBase[38]);break;
		case CORE_BUILDORDER_CHAPTER:return(helpChaptersDataBase[39]);break;
		case CORE_SCALABILITY_CHAPTER:return(helpChaptersDataBase[40]);break;
		case CORE_COMPARISON_CHAPTER:return(helpChaptersDataBase[41]);break;
		case CORE_PROBLEMS_GAS_CHAPTER:return(helpChaptersDataBase[42]);break;
		case COMMUNICATION_MAIN_CHAPTER:return(helpChaptersDataBase[43]);break;
		case COMMUNICATION_CLAWSOFTWARE_CHAPTER:return(helpChaptersDataBase[44]);break;
		case COMMUNICATION_ABOUT_CHAPTER:return(helpChaptersDataBase[45]);break;
		case DEVELOPMENT_MAIN_CHAPTER:return(helpChaptersDataBase[46]);break;
		case DEVELOPMENT_HISTORY_CHAPTER:return(helpChaptersDataBase[47]);break;
		case DEVELOPMENT_NOTIMPLEMENTED_CHAPTER:return(helpChaptersDataBase[48]);break;
		case DEVELOPMENT_TODO_CHAPTER:return(helpChaptersDataBase[49]);break;
		case DEVELOPMENT_CURRENT_CHAPTER:return(helpChaptersDataBase[50]);break;
		case DEVELOPMENT_FUTURE_CHAPTER:return(helpChaptersDataBase[51]);break;
		case PHILOSOHPY_MAIN_CHAPTER:return(helpChaptersDataBase[52]);break;
		case PHILOSOPHY_EVOLUTION_CHAPTER:return(helpChaptersDataBase[53]);break;
		case PHILOSOPHY_GOD_CHAPTER:return(helpChaptersDataBase[54]);break;
		case PHILOSOPHY_REALITY_CHAPTER:return(helpChaptersDataBase[55]);break;
		default:return("");break;	
	}
}

HelpWindow::HelpWindow(UI_Window* parent_window) :
	UI_Window(parent_window, HELP_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(HELP_WINDOW), theme.lookUpGlobalMaxHeight(HELP_WINDOW), NOT_SCROLLED),
	helpChapter(INDEX_CHAPTER),
	text(new UI_LongText(this, Rect(Point(20, 30), Size(getSize()-Size(80, 60))), Size(0, 0), getChapterString(helpChapter), BRIGHT_TEXT_COLOR, FORCE_TEXT_COLOR, LARGE_FONT, TEXT_BUTTON)),
	indexButton(new UI_Button(this, Rect(0, 12, 0, 0), Size(10, 0), MY_BUTTON, false, PRESS_BUTTON_MODE, HELP_WINDOW_INDEX_STRING, ARRANGE_TOP_RIGHT, SMALL_BOLD_FONT, AUTO_SIZE)),
	backButton(new UI_Button(this, Rect(0, 12, 0, 0), Size(10, 0), MY_BUTTON, false, PRESS_BUTTON_MODE, HELP_WINDOW_BACK_STRING, ARRANGE_TOP_RIGHT, SMALL_BOLD_FONT, AUTO_SIZE)),
	lastChapter()
{
		
}

HelpWindow::~HelpWindow()
{
	delete indexButton;
	delete backButton;
	delete text;
}



void HelpWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(HELP_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(HELP_WINDOW));
	text->setOriginalSize(Size(getSize()-Size(80, 60)));
	UI_Window::reloadOriginalSize();
}

void HelpWindow::draw(DC* dc) const
{
	if(!isShown()) return;
	UI_Window::draw(dc);
}

void HelpWindow::process()
{
	UI_Window::process();
	bool chapter_changed = false;
	
	if(indexButton->isLeftClicked())
		gotoChapter(INDEX_CHAPTER);
	
	else
	if(backButton->isLeftClicked())
	{
		if((!lastChapter.empty())&&(lastChapter.back()!=helpChapter))
		{
			gotoChapter(lastChapter.back());
			lastChapter.pop_back();
			lastChapter.pop_back();
		}
	} else
	{
		signed int pressed = text->getPressed();
		if((pressed>=0)&&(helpChapter!=pressed))
			gotoChapter(pressed);
	}

}

void HelpWindow::gotoChapter(unsigned int chapter)
{
	if(helpChapter==chapter)
		return;
	lastChapter.push_back(helpChapter);
	helpChapter = (eHelpChapter)chapter;
	text->updateText(getChapterString(helpChapter));
	setNeedRedrawMoved();
}

