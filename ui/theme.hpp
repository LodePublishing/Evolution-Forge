#ifndef _UI_THEME_HPP
#define _UI_THEME_HPP

#include "../sdl/dc.hpp"
#include "../core/defs.hpp"
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
	"<��<� ;) �J<��D ��t<�� >_<",
	"60 Minutes no Rush!"
}*/


const unsigned int FONT_SIZE = 7;
// TODO!!!

const unsigned int MIN_DISTANCE = 3;

using std::string;

// ------ DATA TYPES ------
enum eDataType
{ 	
	ZERO_DATA_TYPE, 
	STRING_DATA_TYPE,
	FONT_DATA_TYPE,
	COLOR_DATA_TYPE,
	BITMAP_DATA_TYPE,
	PEN_DATA_TYPE,
	BRUSH_DATA_TYPE,
	RECT_DATA_TYPE,
	MAX_HEIGHT_DATA_TYPE,
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
	COLOR_THEME_SUB_DATA_TYPE,

	MAX_SUB_DATA_TYPES
};

// ------ END SUB DATA TYPES ------

// ------ SUB SUB DATA TYPES ------

enum eSubSubDataType
{
	ZERO_SUB_SUB_DATA_TYPE,
	LANGUAGE_SUB_SUB_DATA_TYPE,
	COLOR_THEME_SUB_SUB_DATA_TYPE,
	TAB_SUB_SUB_DATA_TYPE,

	MAX_SUB_SUB_DATA_TYPES
};

// ------ END SUB DATA TYPES ------


// ------ GENERAL THEMES ------
enum eTheme
{
	ZERO_THEME,
	DARK_RED_THEME,
	DARK_BLUE_THEME,
	GREEN_THEME,
	YELLOW_THEME,
	GREY_THEME,
	MONOCHROME_THEME,

	MAX_COLOR_THEMES
};
// ------ END GENERAL THEMES ------

// ------ RESOLUTIONS ------
enum eResolution
{
	ZERO_RESOLUTION,
	RESOLUTION_800x600,
	RESOLUTION_1024x768,
	RESOLUTION_1280x1024,
	MAX_RESOLUTIONS
};
// ------ END RESOLUTIONS ------

// ------ FONTS ------
enum eFont
{
	NULL_FONT,
	SMALL_NORMAL_BOLD_FONT,
	SMALL_ITALICS_BOLD_FONT,
	SMALL_MIDDLE_NORMAL_FONT,
	MIDDLE_NORMAL_BOLD_FONT,
	LARGE_NORMAL_BOLD_FONT,
	VERY_LARGE_NORMAL_BOLD_FONT,
	HUGE_DEFAULT_BOLD_FONT, //TODO

	MAX_FONTS
};
// ------ END FONTS ------

// ------ COLORS ------
enum eColor
{
	NULL_COLOR,
	TEXT_COLOR,
	WINDOW_TEXT_COLOR,
	BRIGHT_TEXT_COLOR,
	TITLE_COLOR,
	IMPORTANT_COLOR,

// forcewindow
	NOT_FULFILLED_TEXT_COLOR,
	FULFILLED_TEXT_COLOR,

	FORCE_ADD_GOAL_TEXT_COLOR,

	FORCE_TEXT_COLOR, //~~

	BIG_BUTTONS_TEXT_COLOR,

// bodiagram
	BRIGHT_MINERALS_TEXT_COLOR,
	BRIGHT_GAS_TEXT_COLOR,
	SUPPLY_TEXT_COLOR,
	BRIGHT_SUPPLY_TEXT_COLOR,

/*	MINERALS_COLOR,
	GAS_COLOR,
	SUPPLY_COLOR,*/

// bograph time steps font color
	TIMESTEPS_TEXT_COLOR,	

// statistics window
	MINERALS_TEXT_COLOR,
	GAS_TEXT_COLOR,
	TIME_TEXT_COLOR,
	STATISTICS_FORCE_TEXT_COLOR,
	BOLENGTH_TEXT_COLOR,
	FITNESS_AVERAGE_TEXT_COLOR,
	FITNESS_VARIANCE_TEXT_COLOR,	
	GENERATIONS_LEFT_TEXT_COLOR,

	FPS_TEXT_COLOR,
	FITNESS_TEXT_COLOR,
	BRIGHT_FITNESS_TEXT_COLOR,

	DISABLED_TEXT_COLOR,

	MAX_COLORS
};
// ------ END COLORS ------

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
	BODIAGRAM_SUPPLY_BOLD_PEN,
	BODIAGRAM_MINERALS_BOLD_PEN,
	BODIAGRAM_GAS_BOLD_PEN,

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

	UNIT_TYPE_0_BOLD_PEN,
	UNIT_TYPE_1_BOLD_PEN,
	UNIT_TYPE_2_BOLD_PEN,
	UNIT_TYPE_3_BOLD_PEN,
	UNIT_TYPE_4_BOLD_PEN,
	UNIT_TYPE_5_BOLD_PEN,
	UNIT_TYPE_6_BOLD_PEN,
	UNIT_TYPE_7_BOLD_PEN,
	UNIT_TYPE_8_BOLD_PEN,
	UNIT_TYPE_9_BOLD_PEN,
	UNIT_TYPE_10_BOLD_PEN,
																											 
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

	BRIGHT_UNIT_TYPE_0_BOLD_PEN,
	BRIGHT_UNIT_TYPE_1_BOLD_PEN,
	BRIGHT_UNIT_TYPE_2_BOLD_PEN,
	BRIGHT_UNIT_TYPE_3_BOLD_PEN,
	BRIGHT_UNIT_TYPE_4_BOLD_PEN,
	BRIGHT_UNIT_TYPE_5_BOLD_PEN,
	BRIGHT_UNIT_TYPE_6_BOLD_PEN,
	BRIGHT_UNIT_TYPE_7_BOLD_PEN,
	BRIGHT_UNIT_TYPE_8_BOLD_PEN,
	BRIGHT_UNIT_TYPE_9_BOLD_PEN,
	BRIGHT_UNIT_TYPE_10_BOLD_PEN,


	GREEN_TIMESTEPS_PEN,

	BODIAGRAM_FITNESS_PEN,

	DISABLED_PEN,
	
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
	VERY_BRIGHT_UNIT_TYPE_BRUSH,

	CONTINUE_BUTTON_BRUSH,
	DARK_CONTINUE_BUTTON_BRUSH,
	BRIGHT_CONTINUE_BUTTON_BRUSH,

	BODIAGRAM_BACK1,
	BODIAGRAM_BACK2,
	BODIAGRAM_BAR,
	BODIAGRAM_SUPPLY_BRUSH,
		
// statistics window
	GENERATIONS_LEFT_BRUSH,	

	TOOLTIP_BRUSH,

	DISABLED_BRUSH,

	MAX_BRUSHES
};
// ------ END BRUSHES ------

// ------ STRINGS ------
enum eString
{
	NULL_STRING,

	UNIT_NULL_STRING=1,
	LAST_UNIT_STRING=315,
	
	START_START_STRING=316,	
	START_LOAD_CORE_SETTINGS_STRING,	
	START_INIT_SDL_STRING,	
	START_UNABLE_TO_INIT_SDL_STRING,
	START_INIT_VIDEO_STRING,
	START_ERROR_SETTING_VIDEO_MODE_STRING,
	START_SET_WINDOW_MODE_STRING,	
	START_SET_FULLSCREEN_MODE_STRING,	
	START_INIT_SDL_TRUETYPE_FONTS_STRING,
	START_INIT_GRAPHIC_ENGINE_CORE_STRING,	
	START_LOAD_UI_BITMAPS_FONTS_STRING,
	START_UI_BITMAPS_FONTS_LOADED_STRING,
	START_ASSIGNING_DEFAULT_VARIABLES_STRING,	
	START_READING_PARSING_STRING,	
	START_LOAD_HARVEST_STRING,	
	START_LOAD_MAPS_STRING,	
	START_LOAD_STARTCONDITIONS_STRING,	
	START_LOAD_GOALS_STRING,	
	START_ASSIGN_AND_ANALYZE_STRING,	
	START_PREPARE_FIRST_RUN_STRING,	
	START_INIT_GUI_STRING,	
	START_MAIN_INIT_COMPLETE_STRING,	
	START_SYSTEM_READY_STRING,
	START_INITIALIZATION_TIME_STRING,
	
	MAIN_WINDOW_TITLE_STRING,
	MESSAGE_WINDOW_TITLE_STRING,
	CORE_WINDOW_TITLE_STRING,
	TUTORIAL_WINDOW_TITLE_STRING,
	INFO_WINDOW_TITLE_STRING,
	BOGRAPH_WINDOW_TITLE_STRING,
	BODIAGRAM_WINDOW_TITLE_STRING,
	STATISTICS_WINDOW_TITLE_STRING,
	TIMER_WINDOW_TITLE_STRING,
	FORCE_WINDOW_TITLE_STRING, // 10
	BOWINDOW_TITLE_STRING,

// message window:
	WELCOME_MSG1_STRING,
	WELCOME_MSG2_STRING,
	PLAYERS_LOADED_STRING,
	ADDED_GOAL_STRING,
	SET_NEW_GOAL_LIST_STRING,
	SET_RACE_STRING,
	ADDED_ONE_GOAL_STRING,
	REMOVED_ONE_GOAL_STRING,
	MOVED_NON_GOAL_STRING,
	SAVED_GOAL_STRING,

// edit field window:
	EDIT_FIELD_OK_STRING,
	EDIT_FIELD_CANCEL_STRING,


// force window:
	UNIT_TYPE_0_STRING,
	UNIT_TYPE_1_STRING,
	UNIT_TYPE_2_STRING,
	UNIT_TYPE_3_STRING,
	UNIT_TYPE_4_STRING,
	UNIT_TYPE_5_STRING,	//20
	UNIT_TYPE_6_STRING,
	UNIT_TYPE_7_STRING,
	UNIT_TYPE_8_STRING,
	UNIT_TYPE_9_STRING,
	UNIT_TYPE_10_STRING,

	STARTING_FORCE_STRING,
	NON_GOALS_STRING,
	GOALS_STRING,
	LEGEND_STRING,
	TIME_LEGEND_STRING,
	CLICK_TO_ADD_GOAL_STRING, //30
	BACK_STRING,
	CLOSE_STRING,
	ADD_GOAL_STRING,
	GOAL_LIST_STRING,
	STARTFORCE_STRING,
	CHOOSE_RACE_STRING,

	SAVE_GOAL_STRING,

	SAVE_GOALS_AS_STRING,
	GIVE_GOAL_A_NAME_STRING,
	
	SAVE_BUILD_ORDER_AS_STRING,
	GIVE_BO_A_NAME_STRING, //40

	TERRA_STRING,
	PROTOSS_STRING,
	ZERG_STRING,
// bowindow
	CLICK_TO_INSERT_ORDER_STRING, //44
//	CLICK_TO_CLOSE_ORDER_STRING, schon in force definiert
//	CLICK_TO_GO_BACK_STRING,

	OPTIMIZE_EVERYTHING_STRING,
	OPTIMIZE_SELECTED_STRING,
	RESET_BUILD_ORDER_STRING,
	SAVE_BUILD_ORDER_STRING,
	SPEED_STRING,
	
// timer window
	OF_GOALS_FULFILLED_STRING, //50
	OF_TIME_FULFILLED_STRING,
	PAUSED_STRING,
	SEARCHING_STRING,
	THEORETICAL_OPTIMUM_STRING,
	OPTIMIZING_STRING,
	RES_UNITS_STRUCT_STRING,
	TOTAL_STRING,
	CLICK_TO_CONTINUE_STRING,
	CLICK_TO_PAUSE_STRING,

// statistics window
	MINERALS_STAT_STRING,
	GAS_STAT_STRING,
	TIME_STAT_STRING,
	FORCE_STAT_STRING,	// 60
	AVERAGE_BO_LENGTH_STAT_STRING,
	FITNESS_AVERAGE_STAT_STRING, 
	FITNESS_VARIANCE_STAT_STRING,
	GENERATIONS_LEFT_STAT_STRING,
	

// window title parameters
	HIDE_MODE_STRING,
	BASIC_PLAYER_MODE_STRING,
	ADVANCED_PLAYER_MODE_STRING, //70
	EXPERT_PLAYER_MODE_STRING,
	EXPERT_COMPUTER_MODE_STRING,
	GOSU_PLAYER_MODE_STRING,
	GOSU_COMPUTER_MODE_STRING, 
	COMPARE_PLAYERMODE_STRING,
	ERROR_MODE_STRING,
// Tabs
	ZERO_TAB_STRING,
	BASIC_TAB_STRING,
	ADVANCED_TAB_STRING,
	EXPERT_TAB_STRING, // 80
	GOSU_TAB_STRING,
	
	TUTORIAL_TAB_STRING,
	SETTINGS_TAB_STRING,
	MAP_TAB_STRING, 
	COMPARE_TAB_STRING,

// tooltips
	ADD_UNIT_TOOLTIP_STRING,
	REMOVE_UNIT_TOOLTIP_STRING,
	REMOVE_GOAL_TOOLTIP_STRING,

	CHOOSE_RACE_TOOLTIP_STRING,
	ADD_GOALS_TOOLTIP_STRING, // 90
	CHOOSE_GOALS_TOOLTIP_STRING,
	CHOOSE_STARTING_FORCE_TOOLTIP_STRING,

	MINERALS_STAT_TOOLTIP_STRING,
	GAS_STAT_TOOLTIP_STRING,
	TIME_STAT_TOOLTIP_STRING, 
	FORCE_STAT_TOOLTIP_STRING,
	AVERAGE_BO_LENGTH_STAT_TOOLTIP_STRING,
	FITNESS_AVERAGE_STAT_TOOLTIP_STRING,
	FITNESS_VARIANCE_STAT_TOOLTIP_STRING,
	GENERATIONS_LEFT_STAT_TOOLTIP_STRING, // 100
	FPS_STAT_TOOLTIP_STRING,

	RESET_BUILD_ORDER_TOOLTIP_STRING,
	SAVE_BUILD_ORDER_TOOLTIP_STRING,
// timer:
	CONTINUE_OPTIMIZATION_TOOLTIP_STRING, 
	PAUSE_OPTIMIZATION_TOOLTIP_STRING, 
	GOALS_FULFILLED_TOOLTIP_STRING,

	BASIC_TAB_TOOLTIP_STRING,
	ADVANCED_TAB_TOOLTIP_STRING,
	EXPERT_TAB_TOOLTIP_STRING,
	GOSU_TAB_TOOLTIP_STRING,

	TUTORIAL_TAB_TOOLTIP_STRING,	
	SETTINGS_TAB_TOOLTIP_STRING, //110
	MAP_TAB_TOOLTIP_STRING,
	COMPARE_TAB_TOOLTIP_STRING,

	FORCEENTRY_TIME_TOOLTIP_STRING,

// settings:
	SETTINGS_WINDOW_TITLE_STRING, 
	TITLE_PREDEFINED_SETTINGS_STRING,
// core-settings:
	SETTING_MAX_TIME_STRING,
	SETTING_RESTRICT_SC_STRING,
	SETTING_FACILITY_MODE_STRING,
	SETTING_AUTO_SAVE_RUNS_STRING,
	SETTING_ALWAYS_BUILD_WORKER_STRING,
	SETTING_PREPROCESS_BUILDORDER_STRING,
	SETTING_MAX_LENGTH_STRING,
	SETTING_MAX_RUNS_STRING,
	SETTING_MAX_GENERATIONS_STRING,
	SETTING_MAX_TIMEOUT_STRING, // 120
	SETTING_ALLOW_GOAL_ADAPTION_STRING,
	SETTING_BREED_FACTOR_STRING,
	SETTING_CROSSING_OVER_STRING,
// gui-settings:
	SETTING_MINIMALIST_STRING,
	SETTING_FULL_STRING,
	SETTING_CUSTOM_STRING,
	SETTING_ALLOW_STATIC_FRAMERATE_STRING,
	SETTING_STATIC_FRAMERATE_STRING,
	SETTING_DYNAMIC_FRAMERATE_STRING,
	SETTING_SMOOTH_MOVEMENT_STRING,
	SETTING_GLOWING_BUTTONS_STRING, //130
	SETTING_DNA_SPIRAL_STRING,
	SETTING_ROUNDED_RECTANGLES_STRING,
	SETTING_BACKGROUND_BITMAP_STRING,
	SETTING_TRANSPARENCY_STRING,
	SETTING_FULLSCREEN_STRING,
	SETTING_TOOLTIPS_STRING,
	SETTING_SOFTWARE_MOUSE_STRING,

	SETTING_MAX_TIME_TOOLTIP_STRING,
	SETTING_RESTRICT_SC_TOOLTIP_STRING,
	SETTING_FACILITY_MODE_TOOLTIP_STRING,
	SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING,
	SETTING_ALWAYS_BUILD_WORKER_TOOLTIP_STRING,
	SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING,
	SETTING_MAX_LENGTH_TOOLTIP_STRING,
	SETTING_MAX_RUNS_TOOLTIP_STRING, //140
	SETTING_MAX_GENERATIONS_TOOLTIP_STRING,
	SETTING_MAX_TIMEOUT_TOOLTIP_STRING,
	SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING,
	SETTING_BREED_FACTOR_TOOLTIP_STRING, 
	SETTING_CROSSING_OVER_TOOLTIP_STRING,
	SETTING_MINIMALIST_TOOLTIP_STRING,
	SETTING_FULL_TOOLTIP_STRING,
	SETTING_CUSTOM_TOOLTIP_STRING,
	SETTING_ALLOW_STATIC_FRAMERATE_TOOLTIP_STRING,
	SETTING_STATIC_FRAMERATE_TOOLTIP_STRING,
	SETTING_DYNAMIC_FRAMERATE_TOOLTIP_STRING, //150
	SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING,
	SETTING_GLOWING_BUTTONS_TOOLTIP_STRING,
	SETTING_DNA_SPIRAL_TOOLTIP_STRING,
	SETTING_ROUNDED_RECTANGLES_TOOLTIP_STRING,
	SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING,
	SETTING_TRANSPARENCY_TOOLTIP_STRING,
	SETTING_FULLSCREEN_TOOLTIP_STRING,
	SETTING_TOOLTIPS_TOOLTIP_STRING,
	SETTING_SOFTWARE_MOUSE_TOOLTIP_STRING,

	SETTINGS_SAVED_STRING,

	LANGUAGE_HAS_CHANGED_STRING,
	
	SETTING_LANGUAGE_STRING, //160
	SETTING_ZERO_LANGUAGE_STRING,
	SETTING_ENGLISH_LANGUAGE_STRING,
	SETTING_GERMAN_LANGUAGE_STRING,
	SETTING_ITALIAN_LANGUAGE_STRING,
	SETTING_PORTUGESE_LANGUAGE_STRING,
	SETTING_DUTCH_LANGUAGE_STRING,
	SETTING_FINNISH_LANGUAGE_STRING,
	SETTING_GREEK_LANGUAGE_STRING,
	SETTING_FRENCH_LANGUAGE_STRING,
	SETTING_SPANISH_LANGUAGE_STRING,
	SETTING_POLSKI_LANGUAGE_STRING,
	SETTING_KOREAN_LANGUAGE_STRING,
	SETTING_CHINESE_LANGUAGE_STRING,
	SETTING_RUSSIAN_LANGUAGE_STRING, //170

	SETTING_RESOLUTION_STRING,
	SETTING_RESOLUTION_ZERO_STRING,
	SETTING_RESOLUTION_800x600_STRING,
	SETTING_RESOLUTION_1024x768_STRING,
	SETTING_RESOLUTION_1280x1024_STRING,
				
	SETTING_THEME_STRING,
	SETTING_ZERO_THEME_STRING,
	SETTING_DARK_BLUE_THEME_STRING,
	SETTING_GREEN_THEME_STRING,
	SETTING_RED_THEME_STRING, //180
	SETTING_YELLOW_THEME_STRING,
	SETTING_GREY_THEME_STRING,
	SETTING_MONOCHROME_THEME_STRING,

	SETTING_RELOAD_FROM_FILE_STRING,
	SETTING_LOAD_FAILSAFE_DEFAULTS_STRING,
	SETTING_SAVE_TO_FILE_STRING,

	SETWINDOW_CORE_SETTINGS_STRING,
	SETWINDOW_GUI_SETTINGS_STRING,
	SETWINDOW_LOADSAVE_SETTINGS_STRING,

	OUTPUT_UNITNAME_STRING, // 190
	OUTPUT_SUPPLY_STRING,
	OUTPUT_MINERALS_STRING,
	OUTPUT_GAS_STRING,
	OUTPUT_LOCATION_STRING,
	OUTPUT_TIME_STRING,

	ENDRUN_FINISHED_STRING,
	ENDRUN_SAVED_BUILDORDER_STRING,
	ENDRUN_DIALOG_TITLE_STRING,
	ENDRUN_QUESTION_STRING,
	ENDRUN_SAVE_AND_CONTINUE_STRING,
	ENDRUN_DONT_SAVE_AND_CONTINUE_STRING,

	SUCCESS_OK_STRING,
	SUCCESS_MINERALS_STRING,
	SUCCESS_GAS_STRING,
	SUCCESS_SUPPLY_STRING,
	SUCCESS_PREREQUISITE_STRING,
	SUCCESS_FACILITY_STRING,
	SUCCESS_TIMEOUT_STRING,
	SUCCESS_UNKNOWN_STRING,

	INFO_BUILD_STRING,
	INFO_AS_SOON_AS_STRING,
	INFO_BECOMES_AVAILIBLE_STRING,
	INFO_AT_STRING,
	INFO_WHEN_STRING,
	INFO_HAVING_STRING,
	INFO_MINERALS_STRING,
	INFO_GAS_STRING,
	INFO_SUPPLY_STRING,
	INFO_TIME_STRING,

	MAX_STRINGS //207
};
// ------ END STRINGS ------

// ------ BITMAPS ------
enum eBitmap
{
	NULL_BITMAP,
	ADD_BITMAP,
	SUB_BITMAP,
	CANCEL_BITMAP,
	SMALL_ARROW_LEFT_BITMAP,
	SMALL_ARROW_RIGHT_BITMAP,
	SMALL_ARROW_UP_BITMAP,
	SMALL_ARROW_DOWN_BITMAP,
//	ARROW_LEFT_BITMAP,
//	ARROW_RIGHT_BITMAP,

//	ARROW_UP_BITMAP,
//	ARROW_DOWN_BITMAP,
/*	CLICKED_SMALL_ARROW_LEFT_BITMAP,
	CLICKED_SMALL_ARROW_RIGHT_BITMAP,
	CLICKED_ARROW_LEFT_BITMAP,
	CLICKED_ARROW_RIGHT_BITMAP,
	CLICKED_SMALL_ARROW_UP_BITMAP,
	CLICKED_SMALL_ARROW_DOWN_BITMAP,
	CLICKED_ARROW_UP_BITMAP,
	CLICKED_ARROW_DOWN_BITMAP,*/
	LOCATION_BUTTON_BITMAP,
	TIME_BUTTON_BITMAP,
	MOUSE_NONE,
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_BOTH,

	RADIO_OFF,
	RADIO_ON,

	BACKGROUND_BITMAP,

	BAR_BITMAP,
	KEY_BITMAP,

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

	SETTINGS_WINDOW,
	EDIT_FIELD_WINDOW,

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

	TUTORIAL_TAB,
	SETTINGS_TAB,
	MAP_TAB,
	COMPARE_TAB,
	
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
	SAME_POSITION_AS_ABOVE_COMMAND,
	SAME_SIZE_AS_ABOVE_COMMAND,
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
	VERY_BRIGHT_UNIT_TYPE_BUTTON,
	TAB_BUTTON,

	ADD_GOAL_BUTTON,
	
// statistics window
	MINERALS_STAT_BUTTON,
	GAS_STAT_BUTTON,
	TIME_STAT_BUTTON,
	FORCE_STAT_BUTTON,
	BUILD_ORDER_LENGTH_STAT_BUTTON,
	FITNESS_AVERAGE_STAT_BUTTON,
	FITNESS_VARIANCE_STAT_BUTTON,
	GENERATIONS_LEFT_STAT_BUTTON,
	FPS_STAT_BUTTON,
	
	SMALL_ARROW_LEFT_BUTTON,
	SMALL_ARROW_RIGHT_BUTTON,
	SMALL_ARROW_UP_BUTTON,
	SMALL_ARROW_DOWN_BUTTON,
	ARROW_LEFT_BUTTON,
	ARROW_RIGHT_BUTTON,
	ARROW_UP_BUTTON,
	ARROW_DOWN_BUTTON,

	GOAL_LOCATION_BUTTON,
	GOAL_TIME_BUTTON,

	CHECK_BUTTON,
	
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
	JUMPY_COLORS_ANIMATION, // gets bright and jumps back
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
//	eString text[MAX_BUTTON_ANIMATION_PHASES];

	unsigned int speed; // 100 = 100 steps for full animation
	eButtonAnimationType type;
};

class UI_Theme
{
	public:
		UI_Theme& operator=(const UI_Theme& object);
		UI_Theme(const UI_Theme& object);
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

		void loadStringFile(const string& dataFile);	
		void loadDataFiles(const string& dataFile, const string& bitmapDir, const string& fontDir, DC* dc); //currently all data hard coded, move it to file later! TODO

		const string* lookUpString(const eString id) const;
		const string lookUpFormattedString(const eString id, const string& text) const;
		const string lookUpFormattedString(const eString id, const unsigned int i) const;
		const string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j) const;
		const string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j, const unsigned int k) const;
		Color* lookUpColor(const eColor id) const;
		/*const */Bitmap* lookUpBitmap(const eBitmap id) const;
		Pen* lookUpPen(const ePen id) const;
		Brush* lookUpBrush(const eBrush id) const;
		Font* lookUpFont(const eFont id) const;
		
		
		const Point lookUpRealDistance(const eWindow id, const unsigned int windowNumber=0) const;
		const Point lookUpMaxRealDistance(const eWindow id, const unsigned int windowNumber=0) const;
		
		const Rect lookUpRect(const eWindow id, const unsigned int windowNumber=0, const unsigned int maxPlayer=0) const;
		const unsigned int lookUpMaxHeight(const eWindow id, const unsigned int windowNumber=0, const unsigned int maxPlayer=0) const;

		const ButtonAnimation* lookUpButtonAnimation(const eButton id) const;
		
// tools:
//		const Color mixColor(const Color* id1, const Color* id2);
//		const Color mixColor(const Color* id1, const Color* id2, const unsigned int gradient);
//		const Color brightenColor(const Color* id, const unsigned int brightness);

//		const Color lookUpMixedColor(const eColor id1, const eColor id2);
//		const Color lookUpMixedColor(const eColor id1, const eColor id2, const unsigned int gradient);
//		Color lookUpBrightenedColor(const eColor id, const unsigned int brightness);

//		const Brush lookUpMixedBrush(const eBrush id1, const eColor id2);
//		const Brush lookUpMixedBrush(const eBrush id1, const eColor id2, const unsigned int gradient);
//		const Brush lookUpMixedBrush(const eBrush id1, const eBrush id2);
//		const Brush lookUpMixedBrush(const eBrush id1, const eBrush id2, const unsigned int gradient);
//		Brush lookUpBrightenedBrush(eBrush id, int brightness);

//		const Pen lookUpMixedPen(const ePen id1, const eColor id2);
//		const Pen lookUpMixedPen(const ePen id1, const eColor id2, const unsigned int gradient);
//		const Pen lookUpMixedPen(const ePen id1, const ePen id2);
//		const Pen lookUpMixedPen(const ePen id1, ePen id2, const unsigned int gradient);
//		const Pen lookUpBrightenedPen(const ePen id, const unsigned int brightness);

	private:
//memory issue: load maybe all data after a change! TODO
 		eResolution resolution;
		eTab tab;
		eLanguage language;
		eTheme colorTheme;
		
		string* stringList[MAX_LANGUAGES][MAX_STRINGS];
		Color* colorList[MAX_COLOR_THEMES][MAX_COLORS];
		Bitmap* bitmapList[MAX_COLOR_THEMES][MAX_RESOLUTIONS][MAX_BITMAPS];
		Pen* penList[MAX_RESOLUTIONS][MAX_COLOR_THEMES][MAX_PENS];
		Brush* brushList[MAX_COLOR_THEMES][MAX_BRUSHES];

		Rect* rectList[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];
		unsigned int maxHeightList[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];

		eArrangeDirection arrangeDirection[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];

		Font* fontList[MAX_RESOLUTIONS][MAX_FONTS];
		ButtonAnimation* buttonAnimationList[MAX_BUTTONS];
		eWindow ywindow[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];
		eWindow xwindow[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS];
};

inline void UI_Theme::setColorTheme(const eTheme color_theme) {
	colorTheme=color_theme;
}

inline const eTheme UI_Theme::getColorTheme() const {
	return(colorTheme);
}

inline const eLanguage UI_Theme::getLanguage() const {
	return(language);
}

inline void UI_Theme::setLanguage(const eLanguage theme_language) {
	language = theme_language;
}

inline const eResolution UI_Theme::getResolution() const {
	return(resolution);
}

inline void UI_Theme::setResolution(const eResolution theme_resolution) {
	resolution = theme_resolution;
	// TODO update whole engine
}

inline void UI_Theme::setTab(const eTab theme_tab) {
	tab = theme_tab;
}

inline const eTab UI_Theme::getTab() const { 
	return(tab);
}

inline const ButtonAnimation* UI_Theme::lookUpButtonAnimation(const eButton id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BUTTONS)) {
		toLog("ERROR: (UI_Theme::lookUpButtonAnimation) id out of range.");return(buttonAnimationList[0]);
	}
#endif
	return(buttonAnimationList[id]);
}

inline const string* UI_Theme::lookUpString(const eString id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_STRINGS)) {
		toLog("ERROR: (UI_Theme::lookUpString) id out of range.");return(stringList[language][0]);
	}
#endif
	return(stringList[language][id]);
}

inline Color* UI_Theme::lookUpColor(const eColor id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_COLORS)) {
		toLog("ERROR: (UI_Theme::lookUpColor) id out of range.");return(colorList[colorTheme][id]);
	}
#endif
	return(colorList[colorTheme][id]);
}

inline /*const */Bitmap* UI_Theme::lookUpBitmap(const eBitmap id) const 
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BITMAPS)) {
		toLog("ERROR: (UI_Theme::lookUpBitmap) id out of range.");return(bitmapList[resolution][colorTheme][id]);
	}
#endif

	return(bitmapList[resolution][colorTheme][id]);
}

inline Pen* UI_Theme::lookUpPen(const ePen id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_PENS)) {
		toLog("ERROR: (UI_Theme::lookUpPen) id out of range.");return(penList[resolution][colorTheme][id]);
	}
#endif
	return(penList[resolution][colorTheme][id]);
}

inline Brush* UI_Theme::lookUpBrush(const eBrush id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BRUSHES)) {
		toLog("ERROR: (UI_Theme::lookUpBrush) id out of range.");return(brushList[colorTheme][id]);
	}
#endif
	return(brushList[colorTheme][id]);
}

inline Font* UI_Theme::lookUpFont(const eFont id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_FONTS)) {
		toLog("ERROR: (UI_Theme::lookUpFont) id out of range.");return(fontList[resolution]/*[language]*/[id]);
	}
#endif
	return(fontList[resolution]/*[language]*/[id]);
}


#endif // _UI_THEME_HPP

