#ifndef _UI_THEME_HPP
#define _UI_THEME_HPP

#include "../sdl/dc.hpp"
#include "../core/starcraft.hpp"
#include "configuration.hpp"

extern unsigned int FONT_SIZE;
//#define FONT_SIZE 6
// TODO!!!

#define MIN_DISTANCE 3

//const unsigned int MAX_PLAYER = 4;
#define MAX_COMPARE_GAMES 2 // two per page at maximum


enum eHelpChapter {
	INDEX_CHAPTER = 0,
	
	INTRODUCTION_MAIN_CHAPTER = 100,
	INTRODUCTION_CHAPTER = 110,
	INTRODUCTION_QUICKSTART_CHAPTER = 120,
	INTRODUCTION_OVERVIEW_CHAPTER = 130,
	INTRODUCTION_USE_CHAPTER = 140,
	INTRODUCTION_LIMITS_CHAPTER = 150,
	INTRODUCTION_PROBLEMS_CHAPTER = 160,

	DESCRIPTION_MAIN_CHAPTER = 200,
	DESCRIPTION_SCORE_WINDOW_CHAPTER = 210,
	DESCRIPTION_GOAL_WINDOW_CHAPTER = 220,
	DESCRIPTION_FORCE_WINDOW_CHAPTER = 230,
	DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER = 240,
	DESCRIPTION_BOGRAPH_WINDOW_CHAPTER = 250,
	DESCRIPTION_BUILD_ORDER_WINDOW_CHAPTER = 260,
	DESCRIPTION_MAP_WINDOW_CHAPTER = 270,

	RESULTS_MAIN_CHAPTER = 300,
	RESULTS_TIME_AND_NUMBERS_CHAPTER = 310,
	RESULTS_OUTPUT_CHAPTER = 320,
	RESULTS_DATABASE_CHAPTER = 330,
	RESULTS_INGAME_CHAPTER = 340,

	CONFIGURATION_MAIN_CHAPTER = 400,
	CONFIGURATION_LOADING_SAVING_CHAPTER = 410,
	CONFIGURATION_RESOLUTION_DEPTH_LANGUAGE_CHAPTER = 420,
	CONFIGURATION_CORE_SETTINGS_CHAPTER = 430,
	CONFIGURATION_GUI_SETTINGS_CHAPTER = 440,
	CONFIGURATION_RULES_SETTINGS_CHAPTER = 450,
	
	MECHANICS_MAIN_CHAPTER = 500,
	MECHANICS_ORDER_CHAPTER = 510,
	MECHANICS_MULTIPLAYER_CHAPTER = 520,
	MECHANICS_FINETUNE_CHAPTER = 530,
	MECHANICS_SIMULATION_CHAPTER = 540,

	INTERACTIVE_MAIN_CHAPTER = 600,
	INTERACTIVE_EDITING_CHAPTER = 610,
	INTERACTIVE_PLAYING_CHAPTER = 620,
	INTERACTIVE_TOURNAMENTS_CHAPTER = 630,
	INTERACTIVE_WHATTOLEARN_CHAPTER = 640,

	CORE_MAIN_CHAPTER = 700,
	CORE_ALGORITHM_CHAPTER = 710,
	CORE_BUILDORDER_CHAPTER = 720,
	CORE_SCALABILITY_CHAPTER = 730,
	CORE_COMPARISON_CHAPTER = 740,
	CORE_PROBLEMS_GAS_CHAPTER = 750,

	COMMUNICATION_MAIN_CHAPTER = 800,
	COMMUNICATION_CLAWSOFTWARE_CHAPTER = 810,
	COMMUNICATION_ABOUT_CHAPTER = 830,

	DEVELOPMENT_MAIN_CHAPTER = 900,
	DEVELOPMENT_HISTORY_CHAPTER = 910,
	DEVELOPMENT_NOTIMPLEMENTED_CHAPTER = 920,
	DEVELOPMENT_TODO_CHAPTER = 930,
	DEVELOPMENT_CURRENT_CHAPTER = 940,
	DEVELOPMENT_FUTURE_CHAPTER = 950,

	PHILOSOHPY_MAIN_CHAPTER = 1000,
	PHILOSOPHY_EVOLUTION_CHAPTER = 1010,
	PHILOSOPHY_GOD_CHAPTER = 1020,
	PHILOSOPHY_REALITY_CHAPTER = 1030,

	MAX_HELP_CHAPTER = 1031
};


// ------ DATA TYPES ------
enum eDataType
{ 	
	ZERO_DATA_TYPE, 
	STRING_DATA_TYPE,
	FONT_DATA_TYPE,
	WINDOW_DATA_TYPE,
	COLOR_DATA_TYPE,
	BITMAP_DATA_TYPE,
	PEN_DATA_TYPE,
	BRUSH_DATA_TYPE,
	BUTTON_COLOR_DATA_TYPE,
	BUTTON_WIDTH_DATA_TYPE,

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


// ------ FONTS ------
enum eFont
{
	NULL_FONT,
	SMALL_FONT,
	SMALL_BOLD_FONT,
	MIDDLE_FONT,
	MIDDLE_BOLD_FONT,
	LARGE_FONT,
	LARGE_BOLD_FONT,
	VERY_LARGE_FONT,
	VERY_LARGE_BOLD_FONT,
	HUGE_FONT, //TODO
	HUGE_BOLD_FONT, //TODO

	MAX_FONTS
};
// ------ END FONTS ------

enum eGameType
{
	ZERO_GAME_TYPE,
	GAME_MAIN_TYPE,
	GAME_1PLAYER_TYPE,
	GAME_2PLAYER_TYPE,
	GAME_3PLAYER_TYPE,
	GAME_4PLAYER_TYPE,

	MAX_GAME_TYPE
};

enum ePlayerType
{
	ZERO_PLAYER_TYPE,
	PLAYER_ONE_TYPE,
	PLAYER_TWO_TYPE,
	PLAYER_THREE_TYPE,
	PLAYER_FOUR_TYPE,

	MAX_PLAYER_TYPE
};




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
	
	START_LOAD_CONFIGURATION_STRING=316,
	START_PARSE_COMMAND_LINE_STRING,
	START_WARNING_VO_ARGUMENT_STRING,
	START_SDL_USING_DRIVER_STRING,
	START_ERROR_NO_DRIVER_AVAILIBLE_STRING,
	START_ERROR_DRIVER_NOT_SUPPORTED_STRING,
	
	START_INIT_SDL_STRING,	
	START_UNABLE_TO_INIT_SDL_STRING,
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

	CHANGED_BIT_DEPTH_STRING,
	CHANGED_RESOLUTION_STRING,
	
	MAIN_WINDOW_TITLE_STRING,
	MESSAGE_WINDOW_TITLE_STRING,
	HELP_WINDOW_TITLE_STRING,
	SETTINGS_WINDOW_TITLE_STRING, 
	MAP_WINDOW_TITLE_STRING,
	
	INFO_WINDOW_TITLE_STRING,
	TECHTREE_WINDOW_TITLE_STRING,
	BOGRAPH_WINDOW_TITLE_STRING,
	BODIAGRAM_WINDOW_TITLE_STRING,
	STATISTICS_WINDOW_TITLE_STRING,
	TIMER_WINDOW_TITLE_STRING,
	FORCE_WINDOW_TITLE_STRING, // 10
	BOWINDOW_TITLE_STRING,
	GAME_WINDOW_TITLE_STRING,
	PLAYER_WINDOW_TITLE_STRING,

	GAME_NUMBER_STRING,
	NEW_GAME_STRING,
	
// message window:
	WELCOME_MSG1_STRING,
	WELCOME_MSG2_STRING,
	PLAYERS_LOADED_STRING,
	ADDED_GOAL_STRING,
	SET_NEW_GOAL_LIST_STRING,
	SET_NEW_MAP_STRING,
	SET_RACE_STRING,
	ADDED_ONE_GOAL_STRING,
	REMOVED_ONE_GOAL_STRING,
	MOVED_NON_GOAL_STRING,
	SAVED_GOAL_STRING,

// game window:
	COMPARE_GAME_STRING,
	REMOVE_GAME_STRING,

// help window:
	HELP_WINDOW_INDEX_STRING,
	HELP_WINDOW_BACK_STRING,
	
// edit field window:
	SAVE_BOX_OK_STRING,
	SAVE_BOX_CANCEL_STRING,


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
	
	CHOOSE_MAP_STRING,

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
	LOAD_BUILD_ORDER_STRING,
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

	ADD_PLAYER_STRING,

// statistics window
	MINERALS_STAT_STRING,
	GAS_STAT_STRING,
	TIME_STAT_STRING,
	FORCE_STAT_STRING,	// 60
	AVERAGE_BO_LENGTH_STAT_STRING,
	FITNESS_AVERAGE_STAT_STRING, 
	FITNESS_VARIANCE_STAT_STRING,
	GENERATIONS_LEFT_STAT_STRING,

// Tabs
	HELP_TAB_STRING,
	SETTINGS_TAB_STRING,
	MAP_TAB_STRING, 

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

	SAVE_GOAL_TOOLTIP_STRING,

	RESET_BUILD_ORDER_TOOLTIP_STRING,
	SAVE_BUILD_ORDER_TOOLTIP_STRING,
	LOAD_BUILD_ORDER_TOOLTIP_STRING,
// timer:
	CONTINUE_OPTIMIZATION_TOOLTIP_STRING, 
	PAUSE_OPTIMIZATION_TOOLTIP_STRING, 
	GOALS_FULFILLED_TOOLTIP_STRING,

	HELP_TAB_TOOLTIP_STRING,	
	SETTINGS_TAB_TOOLTIP_STRING, //110
	MAP_TAB_TOOLTIP_STRING,

	FORCEENTRY_TIME_TOOLTIP_STRING,

// settings:
	TITLE_PREDEFINED_SETTINGS_STRING,
// core-settings:
	SETTING_MAX_TIME_STRING,
	SETTING_RESTRICT_SC_STRING,
	SETTING_FACILITY_MODE_STRING,
	SETTING_AUTO_SAVE_RUNS_STRING,
	SETTING_ALWAYS_BUILD_WORKER_STRING,
	SETTING_ONLY_SWAP_ORDERS_STRING,
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
	SETTING_DESIRED_FRAMERATE_STRING,
	SETTING_DESIRED_CPU_USAGE_STRING,
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
	SETTING_ONLY_SWAP_ORDERS_TOOLTIP_STRING,
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
	SETTING_DESIRED_FRAMERATE_TOOLTIP_STRING,
	SETTING_DESIRED_CPU_USAGE_TOOLTIP_STRING, //150
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
	SETTING_RESOLUTION_640x480_STRING,
	SETTING_RESOLUTION_800x600_STRING,
	SETTING_RESOLUTION_1024x768_STRING,
	SETTING_RESOLUTION_1280x1024_STRING,

	SETTING_BITDEPTH_STRING,
	SETTING_DEPTH_8BIT_STRING,
	SETTING_DEPTH_16BIT_STRING,
	SETTING_DEPTH_24BIT_STRING,
	SETTING_DEPTH_32BIT_STRING,
				
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
	SETWINDOW_UI_SETTINGS_STRING,
	SETWINDOW_LOADSAVE_SETTINGS_STRING,

	MAPWINDOW_MAP_SETTINGS_STRING,
	MAPWINDOW_MAX_PLAYER_STRING,
	MAPWINDOW_MAX_LOCATIONS_STRING,
	MAPWINDOW_MAP_NAME_STRING,
	MAPWINDOW_SYMMETRY_STRING,

	MAPWINDOW_LOCATION_SETTINGS_STRING,
	MAPWINDOW_PLAYER_SETTINGS_STRING,
	MAPWINDOW_LOCATION_CONTENT_STRING,
	MAPWINDOW_DISTANCES_STRING,
	
	MAPWINDOW_MINERAL_BLOCKS_STRING,
	MAPWINDOW_VESPENE_GEYSIRS_STRING,
	MAPWINDOW_MINERAL_DISTANCE_STRING,


	MAPWINDOW_MAX_PLAYER_TOOLTIP_STRING,
	MAPWINDOW_MAX_LOCATIONS_TOOLTIP_STRING,
	MAPWINDOW_SYMMETRY_TOOLTIP_STRING,

	MAPWINDOW_CREATE_NEW_MAP_TOOLTIP_STRING,
	MAPWINDOW_RESET_MAP_TOOLTIP_STRING,
	MAPWINDOW_LOAD_MAP_TOOLTIP_STRING,
	MAPWINDOW_SAVE_MAP_TOOLTIP_STRING,

	MAPWINDOW_MINERAL_BLOCKS_TOOLTIP_STRING,
	MAPWINDOW_VESPENE_GEYSIRS_TOOLTIP_STRING,
	MAPWINDOW_MINERAL_DISTANCE_TOOLTIP_STRING,
	MAPWINDOW_DISTANCES_TOOLTIP_STRING,

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
	INCREASE_BITMAP,
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

	CLAWSOFTWARE_BITMAP,

	NEW_BITMAP,
	LOAD_BITMAP,
	SAVE_BITMAP,
	REFRESH_BITMAP,

	BACK_BITMAP,
	FORWARD_BITMAP,
	ADD_BITMAP,
	DELETE_BITMAP,

	HELP_BITMAP,
	MAP_BITMAP,
	SETTING_BITMAP,
		
	MAX_BITMAPS
};
// ------ END BITMAPS ------

// ------ GENERAL RECTANGLES ------
enum eGlobalWindow
{
	NULL_GLOBAL_WINDOW,
	
	MAIN_WINDOW,
	MESSAGE_WINDOW,
	HELP_WINDOW,
//	STATISTICS_WINDOW, ?
	SETTINGS_WINDOW,
	MAP_WINDOW,
	
	INFO_WINDOW,
	TECHTREE_WINDOW,
	SAVE_BOX_WINDOW,
	
	MAX_GLOBAL_WINDOWS
};

enum eGameWindow
{
	NULL_GAME_WINDOW,

	GAME_WINDOW,
	SCORE_WINDOW,
	DATABASE_WINDOW,

	MAX_GAME_WINDOWS
};

enum ePlayerWindow
{
	NULL_PLAYER_WINDOW,
	
	PLAYER_WINDOW,
	FORCE_WINDOW,
	BUILD_ORDER_WINDOW,
	BUILD_ORDER_DIAGRAM_WINDOW,
	BUILD_ORDER_GRAPH_WINDOW,

	MAX_PLAYER_WINDOWS
};

// ------ END GENERAL RECTANGLES ------

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
	DOCK_BOTTOM_CENTER_INSIDE_OF_COMMAND,
	DOCK_TOP_CENTER_INSIDE_OF_COMMAND,
	DOCK_LEFT_INSIDE_OF_COMMAND,
	DOCK_RIGHT_INSIDE_OF_COMMAND,
	DOCK_TOP_INSIDE_OF_COMMAND,
	DOCK_TOP_LEFT_INSIDE_OF_COMMAND,
	DOCK_TOP_RIGHT_INSIDE_OF_COMMAND,
	DOCK_BOTTOM_INSIDE_OF_COMMAND,

	MAX_COMMANDS
};


enum eButtonColorsType
{
// timer window
	MY_BUTTON,
	FORCE_ENTRY_BUTTON,
// force window
	INCREASE_BUTTON,
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

	BRIGHT_UNIT_TYPE_0_BUTTON,
	BRIGHT_UNIT_TYPE_1_BUTTON,
	BRIGHT_UNIT_TYPE_2_BUTTON,
	BRIGHT_UNIT_TYPE_3_BUTTON,
	BRIGHT_UNIT_TYPE_4_BUTTON,
	BRIGHT_UNIT_TYPE_5_BUTTON,
	BRIGHT_UNIT_TYPE_6_BUTTON,
	BRIGHT_UNIT_TYPE_7_BUTTON,
	BRIGHT_UNIT_TYPE_8_BUTTON,
	BRIGHT_UNIT_TYPE_9_BUTTON,
	BRIGHT_UNIT_TYPE_10_BUTTON,

	
	VERY_BRIGHT_UNIT_TYPE_BUTTON,
	TAB_BUTTON,
	ADD_GOAL_BUTTON,

	EDIT_FIELD_BUTTON,

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
	

	TEXT_BUTTON,

	NEW_BUTTON,
	LOAD_BUTTON,
	SAVE_BUTTON,
	REFRESH_BUTTON,
	
	BACK_BUTTON,
	FORWARD_BUTTON,
	ADD_BUTTON,
	DELETE_BUTTON,

	HELP_BUTTON,
	
	HELP_TAB_BUTTON,
	MAP_TAB_BUTTON,
	SETTINGS_TAB_BUTTON,

	MAX_BUTTON_COLORS_TYPES
};

enum eButtonWidthType
{
	STANDARD_BUTTON_WIDTH,
	SMALL_BUTTON_WIDTH,
	
	MAX_BUTTON_WIDTH_TYPES
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



struct ButtonColorsType
{
	eBrush startBrush[MAX_BUTTON_ANIMATION_PHASES];
	eBrush endBrush[MAX_BUTTON_ANIMATION_PHASES];
	eColor startTextColor[MAX_BUTTON_ANIMATION_PHASES];
	eColor endTextColor[MAX_BUTTON_ANIMATION_PHASES];
	
	ePen startBorderPen[MAX_BUTTON_ANIMATION_PHASES];
	ePen endBorderPen[MAX_BUTTON_ANIMATION_PHASES];
	
	eBitmap bitmap[MAX_BUTTON_ANIMATION_PHASES];// bitmap animation is fixed... for now

	unsigned int speed; // 100 = 100 steps for full animation
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
		void setResolution(const eResolution theme_resolution);

		const eBitDepth getBitDepth() const;
		void setBitDepth(const eBitDepth theme_bitdepth);
		


		void loadStringFile(const std::string& dataFile);
		void loadGraphicData(const std::string& dataFile, const std::string& bitmapDir, const std::string& fontDir, DC* dc); //currently all data hard coded, move it to file later! TODO
		void loadWindowData(const std::string& dataFile, const unsigned int gameNumber, const unsigned int maxGames);

		const std::string& lookUpString(const eString id) const;
		const std::string lookUpFormattedString(const eString id, const std::string& text) const;
		const std::string lookUpFormattedString(const eString id, const unsigned int i) const;
		const std::string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j) const;
		const std::string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j, const unsigned int k) const;
		Color* lookUpColor(const eColor id) const;
		/*const */SDL_Surface* lookUpBitmap(const eBitmap id) const;
		Pen* lookUpPen(const ePen id) const;
		Brush* lookUpBrush(const eBrush id) const;
		Font* lookUpFont(const eFont id) const;
		
		
		const Rect lookUpGlobalRect(const eGlobalWindow id) const;
		const Rect lookUpGameRect(const eGameWindow id, const unsigned int gameNumber, const unsigned int maxGames) const;
		const Rect lookUpPlayerRect(const ePlayerWindow id, const unsigned int gameNumber, const unsigned int maxGames, const unsigned int playerNumber, const unsigned int maxPlayer) const;
		
		const unsigned int lookUpGlobalMaxHeight(const eGlobalWindow id) const;
		const unsigned int lookUpGameMaxHeight(const eGameWindow id, const unsigned int gameNumber, const unsigned int maxGames) const;
		const unsigned int lookUpPlayerMaxHeight(const ePlayerWindow id, const unsigned int gameNumber, const unsigned int maxGames, const unsigned int playerNumber, const unsigned int maxPlayer) const;

		const ButtonColorsType* lookUpButtonColors(const eButtonColorsType id) const;
		const unsigned int lookUpButtonWidth(const eButtonWidthType id) const;

		void updateColors(SDL_Surface* surface);

	private:
		void setMaxGlobalHeight(unsigned int current_resolution, unsigned int id, unsigned int max_height);
		void setMaxGameHeight(unsigned int current_resolution, unsigned int gameNumber, unsigned int maxGames, unsigned int id, unsigned int max_height);
		void setMaxPlayerHeight(unsigned int current_resolution, unsigned int gameNumber, unsigned int maxGames, unsigned int player_max, unsigned int playerNumber, unsigned int id, unsigned int max_height);
//memory issue: load maybe all data after a change! TODO
 		eResolution resolution;
		eBitDepth bitdepth;
		eLanguage language;
		eTheme colorTheme;

		
		
		std::string stringList[MAX_LANGUAGES][MAX_STRINGS];
		std::map<eHelpChapter, std::string> helpChapterStringMap;
		
		Color* colorList[MAX_COLOR_THEMES][MAX_COLORS];
		SDL_Surface* bitmapList[MAX_COLOR_THEMES][MAX_RESOLUTIONS][MAX_BITMAPS];
		Pen* penList[MAX_COLOR_THEMES][MAX_PENS];
		Brush* brushList[MAX_COLOR_THEMES][MAX_BRUSHES];

		Rect* globalRectList[MAX_RESOLUTIONS][MAX_GLOBAL_WINDOWS];
		Rect* gameRectList[MAX_RESOLUTIONS][MAX_COMPARE_GAMES][MAX_COMPARE_GAMES][MAX_GAME_WINDOWS];
		Rect* playerRectList[MAX_RESOLUTIONS][MAX_COMPARE_GAMES][MAX_COMPARE_GAMES][MAX_PLAYER][MAX_PLAYER][MAX_PLAYER_WINDOWS];
		unsigned int maxGlobalHeightList[MAX_RESOLUTIONS][MAX_GLOBAL_WINDOWS];
		unsigned int maxGameHeightList[MAX_RESOLUTIONS][MAX_COMPARE_GAMES][MAX_COMPARE_GAMES][MAX_GAME_WINDOWS];
		unsigned int maxPlayerHeightList[MAX_RESOLUTIONS][MAX_COMPARE_GAMES][MAX_COMPARE_GAMES][MAX_PLAYER][MAX_PLAYER][MAX_PLAYER_WINDOWS];

		Font* fontList[MAX_RESOLUTIONS][MAX_FONTS];
		ButtonColorsType* buttonColorsList[MAX_BUTTON_COLORS_TYPES];
		unsigned int buttonWidthList[MAX_RESOLUTIONS][MAX_BUTTON_WIDTH_TYPES];
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

inline const eBitDepth UI_Theme::getBitDepth() const {
	return(bitdepth);
}



inline void UI_Theme::setBitDepth(const eBitDepth theme_bitdepth) {
	bitdepth = theme_bitdepth;
	// TODO update dc
}

inline const ButtonColorsType* UI_Theme::lookUpButtonColors(const eButtonColorsType id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BUTTON_COLORS_TYPES)) {
		toLog("ERROR: (UI_Theme::lookUpButtonColors) id out of range.");return(buttonColorsList[0]);
	}
#endif
	return(buttonColorsList[id]);
}

inline const unsigned int UI_Theme::lookUpButtonWidth(const eButtonWidthType id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BUTTON_WIDTH_TYPES)) {
		toLog("ERROR: (UI_Theme::lookUpButtonWidth) id out of range.");return(buttonWidthList[0][0]);
	}
#endif
	return(buttonWidthList[resolution][id]);
}

inline const std::string& UI_Theme::lookUpString(const eString id) const
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

inline /*const */SDL_Surface* UI_Theme::lookUpBitmap(const eBitmap id) const 
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
		toLog("ERROR: (UI_Theme::lookUpPen) id out of range.");return(penList[colorTheme][id]);
	}
#endif
	return(penList[colorTheme][id]);
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

#endif // _UI_THEME_HPP

