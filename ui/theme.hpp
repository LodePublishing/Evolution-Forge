#ifndef _UI_THEME_HPP
#define _UI_THEME_HPP

#include "sdlwrapper.hpp"
#include "../core/main.hpp"
/*const char gizmo[GIZMO_NUMBER][40]=
{
	"Perfection is the key",
	"Look at la Luna",
	"It is a good day to optimize",
	"The early ling catches the worm",
	"Build orders are best served optimized",
	"Pool Pool Pool Pool Dead - A newbie",
	"Good... Bad... I'm the guy with the gum",
	"LOOOOOOOOOOOOOOOOOOOOOOOOOL",
	"ALT+F4 - Zergling has left the game :/",
	"WHY ARE YOUR ZEALOTS INVISIBLE!?",
	"!!!!! 3VS3 BGH NO NOOBS !!!!",
	"<ý¦<¬ ;) ýJ<¼ýD Ìýt<Üý >_<",
	"60 Minutes no Rush!"
};*/


const int FONT_SIZE = 7;
// TODO!!!

const int MIN_DISTANCE = 3;

using std::string;

// ------ DATA TYPES ------
enum eDataType
{ 	
	ZERO_DATA_TYPE, 
	STRING_DATA_TYPE,
	FONT_DATA_TYPE,
	COLOUR_DATA_TYPE,
	BITMAP_DATA_TYPE,
	PEN_DATA_TYPE,
	BRUSH_DATA_TYPE,
	RECT_DATA_TYPE,
	MAX_RECT_DATA_TYPE,
	BUTTON_DATA_TYPE,

	MAX_DATA_TYPES
};
// ------ END DATA TYPES ------
                                                                                                                                                            
// ------ SUB DATA TYPES ------

enum eSubDataType
{
	ZERO_SUB_DATA_TYPE,
	LANGUAGE_SUB_DATA_TYPE,
	RESOLUTION_SUB_DATA_TYPE,
	COLOUR_THEME_SUB_DATA_TYPE,

	MAX_SUB_DATA_TYPES
};

// ------ END SUB DATA TYPES ------

// ------ SUB SUB DATA TYPES ------

enum eSubSubDataType
{
	ZERO_SUB_SUB_DATA_TYPE,
	LANGUAGE_SUB_SUB_DATA_TYPE,
	COLOUR_THEME_SUB_SUB_DATA_TYPE,
	TAB_SUB_SUB_DATA_TYPE,

	MAX_SUB_SUB_DATA_TYPES
};

// ------ END SUB DATA TYPES ------


// ------ LANGUAGES ------
enum eLanguage
{
	ZERO_LANGUAGE,
	ENGLISH_LANGUAGE,
	GERMAN_LANGUAGE,
	FINNISH_LANGUAGE,
	FRENCH_LANGUAGE,
	SPANISH_LANGUAGE,
	POLSKI_LANGUAGE,
	KOREAN_LANGUAGE,
	CHINESE_LANGUAGE,
	RUSSKI_LANGUAGE,

	MAX_LANGUAGES
};
// ------ END LANGUAGES ------

// ------ GENERAL THEMES ------
enum eTheme
{
	ZERO_THEME,
	DARK_BLUE_THEME,
	GREEN_THEME,
	RED_THEME,
	YELLOW_THEME,
	GREY_THEME,
	MONOCHROME_THEME,

	MAX_COLOUR_THEMES
};
// ------ END GENERAL THEMES ------

// ------ RESOLUTIONS ------
enum eResolution
{
	ZERO_RESOLUTION,
	RESOLUTION_320x200,
	RESOLUTION_640x480,
	RESOLUTION_800x600,
	RESOLUTION_1024x768,
	RESOLUTION_1280x1024,
	RESOLUTION_1600x1200,

	MAX_RESOLUTIONS,
};
// ------ END RESOLUTIONS ------

// ------ FONTS ------
enum eFont
{
	NULL_FONT,
	SMALL_NORMAL_BOLD_FONT,
	SMALL_ITALICS_BOLD_FONT,
	MIDDLE_NORMAL_BOLD_FONT,
	LARGE_NORMAL_BOLD_FONT,
	VERY_LARGE_NORMAL_BOLD_FONT,
	HUGE_DEFAULT_BOLD_FONT, //TODO

	MAX_FONTS
};
// ------ END FONTS ------

// ------ COLOURS ------
enum eColor
{
	NULL_COLOUR,
	TEXT_COLOUR,
	WINDOW_TEXT_COLOUR,
	BRIGHT_TEXT_COLOUR,
	TITLE_COLOUR,
	IMPORTANT_COLOUR,

// forcewindow
	FULFILLED_TEXT_COLOUR,
	NOT_FULFILLED_TEXT_COLOUR,

	FORCE_ADD_GOAL_TEXT_COLOUR,

	FORCE_TEXT_COLOUR, //~~

	BIG_BUTTONS_TEXT_COLOUR,

// bodiagram
	MINERALS_TEXT_COLOUR,
	BRIGHT_MINERALS_TEXT_COLOUR,
	GAS_TEXT_COLOUR,
	BRIGHT_GAS_TEXT_COLOUR,
	SUPPLY_TEXT_COLOUR,
	BRIGHT_SUPPLY_TEXT_COLOUR,

/*	MINERALS_COLOUR,
	GAS_COLOUR,
	SUPPLY_COLOUR,*/

// bograph time steps font color
	TIMESTEPS_TEXT_COLOUR,	

// statistics window
	STATISTICS_FORCE_TEXT_COLOUR,
	TIME_TEXT_COLOUR,
	FITNESS_AVERAGE_TEXT_COLOUR,
	FITNESS_VARIANCE_TEXT_COLOUR,	
	BOLENGTH_TEXT_COLOUR,
	GENERATIONS_LEFT_TEXT_COLOUR,

	MAX_COLOURS
};
// ------ END COLOURS ------

//------ PENS ------
enum ePen
{
	NULL_PEN,
	BORDER_PEN,
	OUTER_BORDER_PEN,
	INNER_BORDER_PEN,
	INNER_BORDER_HIGHLIGHT_PEN,

	BIG_BUTTONS_PEN,
	RECTANGLE_PEN,
	BITMAP_MARKED_PEN,
	
// bodiagram
	DASHED_MINERALS_PEN,
	DASHED_GAS_PEN,
	DASHED_SUPPLY_PEN,
	DASHED_MARKER_PEN,
// bowindow
	SELECT_PEN,
// transparent
//	TRANSPARENT_PEN,
	DARK_BACKGROUND_PEN,
// buttons
	MARKED_BITMAP_GREEN_PEN,
	MARKED_BITMAP_YELLOW_PEN,
	MARKED_BITMAP_RED_PEN,
	MARKED_BITMAP_BASIC_PEN,
// bodiagram
	BODIAGRAM_SUPPLY_PEN,
	BODIAGRAM_MINERALS_PEN,
	BODIAGRAM_GAS_PEN,

    UNIT_TYPE_0_PEN,
    UNIT_TYPE_1_PEN,
    UNIT_TYPE_2_PEN,
    UNIT_TYPE_3_PEN,
    UNIT_TYPE_4_PEN,
    UNIT_TYPE_5_PEN,
    UNIT_TYPE_6_PEN,
    UNIT_TYPE_7_PEN,
    UNIT_TYPE_8_PEN,
    UNIT_TYPE_9_PEN,
    UNIT_TYPE_10_PEN,
                                                                                                                 
    BRIGHT_UNIT_TYPE_0_PEN,
    BRIGHT_UNIT_TYPE_1_PEN,
    BRIGHT_UNIT_TYPE_2_PEN,
    BRIGHT_UNIT_TYPE_3_PEN,
    BRIGHT_UNIT_TYPE_4_PEN,
    BRIGHT_UNIT_TYPE_5_PEN,
    BRIGHT_UNIT_TYPE_6_PEN,
    BRIGHT_UNIT_TYPE_7_PEN,
    BRIGHT_UNIT_TYPE_8_PEN,
    BRIGHT_UNIT_TYPE_9_PEN,
    BRIGHT_UNIT_TYPE_10_PEN,
	
	MAX_PENS
};
// ------ END PENS ------

// ------ BRUSHES ------
enum eBrush
{
	NULL_BRUSH,
	TRANSPARENT_BRUSH,
	WINDOW_FOREGROUND_BRUSH,
	WINDOW_BACKGROUND_BRUSH,

	BO_DARK_BRUSH,
	BO_BRIGHT_BRUSH,

	FORCE_LIST_BRUSH,
	FORCE_LIST_BUTTON_BRUSH,

	UNIT_TYPE_0_BRUSH,
	UNIT_TYPE_1_BRUSH,
	UNIT_TYPE_2_BRUSH,
	UNIT_TYPE_3_BRUSH,
	UNIT_TYPE_4_BRUSH,
	UNIT_TYPE_5_BRUSH,
	UNIT_TYPE_6_BRUSH,
	UNIT_TYPE_7_BRUSH,
	UNIT_TYPE_8_BRUSH,
	UNIT_TYPE_9_BRUSH,
	UNIT_TYPE_10_BRUSH,

	BRIGHT_UNIT_TYPE_0_BRUSH,
	BRIGHT_UNIT_TYPE_1_BRUSH,
	BRIGHT_UNIT_TYPE_2_BRUSH,
	BRIGHT_UNIT_TYPE_3_BRUSH,
	BRIGHT_UNIT_TYPE_4_BRUSH,
	BRIGHT_UNIT_TYPE_5_BRUSH,
	BRIGHT_UNIT_TYPE_6_BRUSH,
	BRIGHT_UNIT_TYPE_7_BRUSH,
	BRIGHT_UNIT_TYPE_8_BRUSH,
	BRIGHT_UNIT_TYPE_9_BRUSH,
	BRIGHT_UNIT_TYPE_10_BRUSH,

	CONTINUE_BUTTON_BRUSH,
	DARK_CONTINUE_BUTTON_BRUSH,
	BRIGHT_CONTINUE_BUTTON_BRUSH,

	BODIAGRAM_BACK1,
	BODIAGRAM_BACK2,
	BODIAGRAM_BAR,
	
// statistics window
	GENERATIONS_LEFT_BRUSH,	

	MAX_BRUSHES
};
// ------ END BRUSHES ------

// ------ STRINGS ------
enum eString
{
	NULL_STRING,
	MAIN_WINDOW_TITLE_STRING,
	MESSAGE_WINDOW_TITLE_STRING,
	CORE_WINDOW_TITLE_STRING,
	TUTORIAL_WINDOW_TITLE_STRING,
	INFO_WINDOW_TITLE_STRING,
	BOGRAPH_WINDOW_TITLE_STRING,
	BODIAGRAM_WINDOW_TITLE_STRING,
	STATISTICS_WINDOW_TITLE_STRING,
	TIMER_WINDOW_TITLE_STRING,
	FORCE_WINDOW_TITLE_STRING,
	BOWINDOW_TITLE_STRING,

// message window:
	WELCOME_MSG1_STRING,
	WELCOME_MSG2_STRING,
	PLAYERS_LOADED_STRING,

// force window:
	UNIT_TYPE_0_STRING,
	UNIT_TYPE_1_STRING,
	UNIT_TYPE_2_STRING,
	UNIT_TYPE_3_STRING,
	UNIT_TYPE_4_STRING,
	UNIT_TYPE_5_STRING,
	UNIT_TYPE_6_STRING,
	UNIT_TYPE_7_STRING,
	UNIT_TYPE_8_STRING,
	UNIT_TYPE_9_STRING,
	UNIT_TYPE_10_STRING,

	STARTING_FORCE_STRING,
	CLICK_TO_ADD_GOAL_STRING,
	CLICK_TO_CLOSE_STRING,
	CLICK_TO_GO_BACK_STRING,
	NON_GOALS_STRING,
	ADD_AS_GOAL_STRING,
	LOAD_GOAL_LIST_STRING,
	CLOSE_GOAL_LIST_STRING,

	USE_MAP_SETTINGS_STRING,
	TERRA_STRING,
	PROTOSS_STRING,
	ZERG_STRING,
// bowindow
	CLICK_TO_INSERT_ORDER_STRING,
//	CLICK_TO_CLOSE_ORDER_STRING, schon in force definiert
//	CLICK_TO_GO_BACK_STRING,

	OPTIMIZE_EVERYTHING_STRING,
	OPTIMIZE_SELECTED_STRING,
	
// timer window
	OF_GOALS_FULFILLED_STRING,
	PAUSED_STRING,
	SEARCHING_STRING,
	THEORETICAL_OPTIMUM_STRING,
	OPTIMIZING_STRING,
	RES_UNITS_STRUCT_STRING,
	TOTAL_STRING,
	CLICK_TO_CONTINUE_STRING,
	CLICK_TO_PAUSE_STRING,

// statistics window
	FORCE_STRING,
	TIME_STRING,
	MINERALS_STRING,
	GAS_STRING,
	FITNESS_AVERAGE_STRING,
	FITNESS_VARIANCE_STRING,
	BUILD_ORDER_LENGTH_STRING,
	GENERATIONS_LEFT_STRING,

// window title parameters
    HIDE_MODE_STRING,
    BASIC_PLAYER_MODE_STRING,
    ADVANCED_PLAYER_MODE_STRING,
    EXPERT_PLAYER_MODE_STRING,
    EXPERT_COMPUTER_MODE_STRING,
    GOSU_PLAYER_MODE_STRING,
    GOSU_COMPUTER_MODE_STRING,
    TRANSCENDEND_COMPUTER_MODE_STRING,
	ERROR_MODE_STRING,
// Tabs
    ZERO_TAB_STRING,
    BASIC_TAB_STRING,
    ADVANCED_TAB_STRING,
    EXPERT_TAB_STRING,
    GOSU_TAB_STRING,
    TRANSCENDEND_TAB_STRING,
    MAP_TAB_STRING,
    SETTINGS_TAB_STRING,
    TUTORIAL_TAB_STRING,
	
	MAX_STRINGS
};
// ------ END STRINGS ------

// ------ BITMAPS ------
enum eBitmap
{
	NULL_BITMAP,

	ADD_BITMAP,
	SUB_BITMAP,
	CANCEL_BITMAP,
	ARROW_LEFT_BITMAP,
	ARROW_RIGHT_BITMAP,
	ARROW_UP_BITMAP,
	ARROW_DOWN_BITMAP,
	CLAW_BITMAP,
	CLAWSOFTWARE_BITMAP,
	BACKGROUND_BITMAP,
	IMP1_BITMAP,
	IMP2_BITMAP,
	IMP3_BITMAP,
	IMP4_BITMAP,
	IMP5_BITMAP,
	IMP6_BITMAP,

	MAX_BITMAPS
};
// ------ END BITMAPS ------

// ------ GENERAL RECTANGLES ------
enum eWindow
{
	NULL_WINDOW,
	MAIN_WINDOW,
	MESSAGE_WINDOW,
	THE_CORE_WINDOW,
	TUTORIAL_WINDOW,
    BUILD_ORDER_WINDOW,
    FORCE_WINDOW,
    TIMER_WINDOW,
    STATISTICS_WINDOW,
    BO_DIAGRAM_WINDOW,
    BO_GRAPH_WINDOW,
    INFO_WINDOW,

	MAX_WINDOWS
};
// ------ END GENERAL RECTANGLES ------

enum eTab
{
	ZERO_TAB,
    BASIC_TAB,
    ADVANCED_TAB,
    EXPERT_TAB,
    GOSU_TAB,
    TRANSCENDEND_TAB,
    MAP_TAB,
    SETTINGS_TAB,
    TUTORIAL_TAB,
    MAX_TABS
};

enum eArrangeDirection
{
	ARRANGE_LEFT_TO_RIGHT,
	ARRANGE_RIGHT_TO_LEFT,
	ARRANGE_TOP_TO_DOWN,
	ARRANGE_DOWN_TO_TOP
};


enum eCommand
{
	NO_COMMAND,
	ABSOLUTE_COORDINATES_COMMAND,
	ABSOLUTE_X_COORDINATE_COMMAND,
	ABSOLUTE_Y_COORDINATE_COMMAND,
	DOCK_WITH_LEFT_BORDER_OF_COMMAND,
	DOCK_WITH_RIGHT_BORDER_OF_COMMAND,
	DOCK_WITH_LOWER_BORDER_OF_COMMAND,
	DOCK_WITH_UPPER_BORDER_OF_COMMAND,
	DOCK_CENTER_INSIDE_OF_COMMAND,
	DOCK_LEFT_INSIDE_OF_COMMAND,
	DOCK_RIGHT_INSIDE_OF_COMMAND,
	DOCK_TOP_INSIDE_OF_COMMAND,
	DOCK_BOTTOM_INSIDE_OF_COMMAND,
	CALCULATE_MAXSIZE_COMMAND,
	CALCULATE_MAXWIDTH_COMMAND,
	CALCULATE_MAXHEIGHT_COMMAND,
	SAME_AS_ABOVE_COMMAND,

	MAX_COMMANDS
};

enum eButton
{
// timer window
	MY_BUTTON,
	FORCE_ENTRY_BUTTON,
// force window
	ADD_BUTTON,
	SUB_BUTTON,
	CANCEL_BUTTON,
/*	UMS_BUTTON,
	TERRA_BUTTON,
	PROTOSS_BUTTON,
	ZERG_BUTTON,*/

    UNIT_TYPE_0_BUTTON,
    UNIT_TYPE_1_BUTTON,
    UNIT_TYPE_2_BUTTON,
    UNIT_TYPE_3_BUTTON,
    UNIT_TYPE_4_BUTTON,
    UNIT_TYPE_5_BUTTON,
    UNIT_TYPE_6_BUTTON,
    UNIT_TYPE_7_BUTTON,
    UNIT_TYPE_8_BUTTON,
    UNIT_TYPE_9_BUTTON,
    UNIT_TYPE_10_BUTTON,

	TAB_BUTTON,
	
	MAX_BUTTONS
};

enum eButtonAnimationPhase
{
    DISABLED_BUTTON_PHASE,
    NORMAL_BUTTON_PHASE,
    PRESSED_BUTTON_PHASE,
    HIGHLIGHT_BUTTON_PHASE,
	PRESSED_HIGHLIGHTED_BUTTON_PHASE, // primarily for static buttons
	
    MAX_BUTTON_ANIMATION_PHASES
};


enum eButtonAnimationType
{
    NO_ANIMATION,
    JUMPY_COLOURS_ANIMATION, // gets bright and jumps back
    GLOWING_ANIMATION, // back and forth
    BLINKING_ANIMATION // jumps between start and end colors
};



struct ButtonAnimation
{
    eBrush startBrush[MAX_BUTTON_ANIMATION_PHASES];
    eBrush endBrush[MAX_BUTTON_ANIMATION_PHASES];
                                                                                                                                                            
    eColor startTextColor[MAX_BUTTON_ANIMATION_PHASES];
    eColor endTextColor[MAX_BUTTON_ANIMATION_PHASES];
                                                                                                                                                            
    ePen startBorderPen[MAX_BUTTON_ANIMATION_PHASES];
    ePen endBorderPen[MAX_BUTTON_ANIMATION_PHASES];
                                                                                                                                                            
    eBitmap bitmap[MAX_BUTTON_ANIMATION_PHASES];// bitmap animation is fixed... for now
//    eString text[MAX_BUTTON_ANIMATION_PHASES];

	int speed; // 100 = 100 steps for full animation
    eButtonAnimationType type;
};

class UI_Theme
{
    public:
        UI_Theme();
        ~UI_Theme();
        
		const eLanguage getLanguage() const;
        void setLanguage(const eLanguage language);

		const eTheme getColorTheme() const;
		void setColorTheme(const eTheme colorTheme);
		
		const eResolution getResolution() const;
		void setResolution(const eResolution resolution);

		const eTab getTab() const;
		void setTab(const eTab tab);	

		void loadDataFiles(const string& dataFile, const string& bitmapDir, const string& fontDir, DC* dc); //currently all data hard coded, move it to file later! TODO

        const string* lookUpString(const eString id) const;
		const string lookUpFormattedString(const eString id, const string& text) const;
		const string lookUpFormattedString(const eString id, const int i) const;
		const string lookUpFormattedString(const eString id, const int i, const int j) const;
		const string lookUpFormattedString(const eString id, const int i, const int j, const int k) const;
        Color* lookUpColor(const eColor id) const;
        const Bitmap* lookUpBitmap(const eBitmap id) const;
        Pen* lookUpPen(const ePen id) const;
        Brush* lookUpBrush(const eBrush id) const;
        Font* lookUpFont(const eFont id) const;
		
		const Rect lookUpRect(const eWindow id, int windowNumber=0) const;
		const Rect lookUpMaxRect(const eWindow id, int windowNumber=0) const;

		const ButtonAnimation* lookUpButtonAnimation(const eButton id) const;
		
// tools:
		const Color mixColor(const Color* id1, const Color* id2);
		const Color mixColor(const Color* id1, const Color* id2, const int gradient);
		const Color brightenColor(const Color* id, const int brightness);

		const Color lookUpMixedColor(const eColor id1, const eColor id2);
		const Color lookUpMixedColor(const eColor id1, const eColor id2, const int gradient);
		Color lookUpBrightenedColor(const eColor id, const int brightness);

		const Brush lookUpMixedBrush(const eBrush id1, const eColor id2);
		const Brush lookUpMixedBrush(const eBrush id1, const eColor id2, const int gradient);
		const Brush lookUpMixedBrush(const eBrush id1, const eBrush id2);
		const Brush lookUpMixedBrush(const eBrush id1, const eBrush id2, const int gradient);
		Brush lookUpBrightenedBrush(eBrush id, int brightness);

		const Pen lookUpMixedPen(const ePen id1, const eColor id2);
		const Pen lookUpMixedPen(const ePen id1, const eColor id2, const int gradient);
		const Pen lookUpMixedPen(const ePen id1, const ePen id2);
		const Pen lookUpMixedPen(const ePen id1, ePen id2, const int gradient);
		const Pen lookUpBrightenedPen(const ePen id, const int brightness);

    private:
//memory issue: load maybe all data after a change! TODO
 		eResolution resolution;
		eTab tab;
        eLanguage language;
		eTheme colorTheme;
		
        string* stringList[MAX_LANGUAGES][MAX_STRINGS];
        Color* colorList[MAX_COLOUR_THEMES][MAX_COLOURS];
        Bitmap* bitmapList[MAX_RESOLUTIONS][MAX_COLOUR_THEMES][MAX_BITMAPS];
		Pen* penList[MAX_RESOLUTIONS][MAX_COLOUR_THEMES][MAX_PENS];
        Brush* brushList[MAX_COLOUR_THEMES][MAX_BRUSHES];

		Rect* rectList[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];
		Rect* maxRectList[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];

		eArrangeDirection arrangeDirection[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];

		Font* fontList[MAX_RESOLUTIONS][MAX_LANGUAGES][MAX_FONTS];
		ButtonAnimation* buttonAnimationList[MAX_BUTTONS];
};

#endif // _UI_THEME_HPP

