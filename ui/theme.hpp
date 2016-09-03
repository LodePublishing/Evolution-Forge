#ifndef _UI_THEME_HPP
#define _UI_THEME_HPP

#include "SDL_image.h"
#include "../core/game.hpp"
#include "configuration.hpp"

extern unsigned int FONT_SIZE;
//#define FONT_SIZE 6
// TODO!!!

#define MIN_DISTANCE 3
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
	DESCRIPTION_UNIT_LIST_WINDOW_CHAPTER = 220,
	DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER = 230,
	DESCRIPTION_BOGRAPH_WINDOW_CHAPTER = 240,
	DESCRIPTION_BUILD_ORDER_WINDOW_CHAPTER = 250,
	DESCRIPTION_DATABASE_WINDOW_CHAPTER = 260, 
// TODO evtl eigenes Kapitel
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
	MECHANICS_SIMULATION_CHAPTER = 530,
	MECHANICS_FINETUNE_CHAPTER = 540,

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
	DEVELOPMENT_TODO_CHAPTER = 920,
	DEVELOPMENT_CURRENT_CHAPTER = 930,
	DEVELOPMENT_FUTURE_CHAPTER = 940,
	DEVELOPMENT_NOTIMPLEMENTED_CHAPTER = 950,

	PHILOSOHPY_MAIN_CHAPTER = 1000,
	PHILOSOPHY_EVOLUTION_CHAPTER = 1010,
	PHILOSOPHY_GOD_CHAPTER = 1020,
	PHILOSOPHY_REALITY_CHAPTER = 1030,

	MAX_HELP_CHAPTER = 1031
};



enum eCursor
{
	ARROW_CURSOR,
	CLOCK_CURSOR,
	HAND_CURSOR,
	
	MAX_CURSORS
};

// ------ DATA TYPES ------
enum eDataType
{ 	
	ZERO_DATA_TYPE, 
	HELP_STRING_DATA_TYPE,
	STRING_DATA_TYPE,
	FONT_DATA_TYPE,
	WINDOW_DATA_TYPE,
	COLOR_DATA_TYPE,
	BITMAP_DATA_TYPE,
	THEME_BITMAP_DATA_TYPE,
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

// ------ FONTS ------
enum eFont
{
	NULL_FONT,
	SMALL_FONT,
	SMALL_BOLD_FONT,
	SMALL_SHADOW_FONT,
	SMALL_SHADOW_BOLD_FONT,
	SMALL_UNDERLINED_FONT,
	SMALL_UNDERLINED_BOLD_FONT,
	
	MIDDLE_FONT,
	MIDDLE_BOLD_FONT,
	MIDDLE_SHADOW_FONT,
	MIDDLE_SHADOW_BOLD_FONT,
	MIDDLE_UNDERLINED_FONT,
	MIDDLE_UNDERLINED_BOLD_FONT,
	
	LARGE_FONT,
	LARGE_BOLD_FONT,
	LARGE_SHADOW_FONT,
	LARGE_SHADOW_BOLD_FONT,
	LARGE_UNDERLINED_FONT,
	LARGE_UNDERLINED_BOLD_FONT,
	
	VERY_LARGE_FONT,
	VERY_LARGE_BOLD_FONT,
	VERY_LARGE_SHADOW_FONT,
	VERY_LARGE_SHADOW_BOLD_FONT,
	VERY_LARGE_UNDERLINED_FONT,
	VERY_LARGE_UNDERLINED_BOLD_FONT,
		
	HUGE_FONT, //TODO
	HUGE_BOLD_FONT, //TODO
	HUGE_SHADOW_FONT,
	HUGE_SHADOW_BOLD_FONT,
	HUGE_UNDERLINED_FONT,
	HUGE_UNDERLINED_BOLD_FONT,

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
	DARK_IMPORTANT_COLOR,

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

	START_LOAD_LANGUAGE_FILES_STRING,
	START_LOAD_UI_CONFIGURATION_STRING,
	START_LOAD_CORE_CONFIGURATION_STRING,
	START_LOAD_EF_CONFIGURATION_STRING,
	START_SET_LANGUAGE_STRING,

	START_COMMANDO_LINE_1_STRING,
	START_COMMANDO_LINE_2_STRING,
	START_COMMANDO_LINE_3_STRING,
	START_COMMANDO_LINE_4_STRING,
	START_COMMANDO_LINE_5_STRING,
	START_COMMANDO_LINE_6_STRING,
	
	START_PARSE_COMMAND_LINE_STRING,
	START_INIT_NOSOUND_STRING,
	START_WARNING_VO_ARGUMENT_STRING,
	START_SDL_USING_DRIVER_STRING,
	START_ERROR_NO_DRIVER_AVAILIBLE_STRING,
	START_ERROR_DRIVER_NOT_SUPPORTED_STRING,
	
	START_INIT_SDL_STRING,	
	START_AVAILIBLE_GRAPHIC_DRIVERS_STRING,
	START_UNABLE_TO_INIT_SDL_STRING,
	START_ERROR_SETTING_VIDEO_MODE_STRING,
	START_CREATED_SURFACE_STRING,
	START_SET_WINDOW_MODE_STRING,	
	START_SET_FULLSCREEN_MODE_STRING,	
	START_INIT_SDL_TRUETYPE_FONTS_STRING,
	START_INIT_SDL_TTF_ERROR_STRING,
	START_INIT_FRAMERATE_STRING,
	START_INIT_SOUND_STRING,
	START_INIT_FMOD_VERSION_ERROR_STRING,
	START_INIT_GRAPHIC_ENGINE_CORE_STRING,	
	START_LOAD_UI_BITMAPS_FONTS_STRING,
	START_ASSIGNING_DEFAULT_VARIABLES_STRING,	
	START_READING_PARSING_STRING,	
	START_LOAD_HARVEST_STRING,	
	START_LOAD_MAPS_STRING,	
	START_LOAD_STARTCONDITIONS_STRING,	
	START_LOAD_GOALS_STRING,	
	START_LOAD_BUILD_ORDERS_STRING,
	START_LOADING_STRING, // just 'loading'

	START_LOAD_COORDINATES_SINGLE_VIEW_STRING,
	START_LOAD_COORDINATES_LEFT_SPLIT_VIEW_STRING,
	START_LOAD_COORDINATES_RIGHT_SPLIT_VIEW_STRING,

	START_LOAD_DATA_FILE_STRING,
	START_LOAD_COLORS_STRING,
	START_LOAD_PENS_STRING,
	START_LOAD_BRUSHES_STRING,
	START_LOAD_FONTS_STRING,
	START_LOAD_MAIN_COORDINATES_STRING,
	START_LOAD_BITMAPS_STRING,
	START_LOAD_BUTTONS_STRING,
	START_LOAD_SOUNDS_STRING,
	START_LOAD_MUSIC_STRING,

	START_SET_DESIRED_FRAMERATE_STRING,
	START_SET_DESIRED_CPU_STRING,
		
	START_ASSIGN_AND_ANALYZE_STRING,	
	START_PREPARE_FIRST_RUN_STRING,	
	START_INIT_GUI_STRING,

	START_INIT_MAIN_WINDOW_STRING,
	START_INIT_HELP_WINDOW_STRING,
	START_INIT_SETTINGS_WINDOW_STRING,
	START_INIT_DATABASE_WINDOW_STRING,
	START_INIT_MAP_WINDOW_STRING,
	START_INIT_MSG_WINDOW_STRING,
	START_INIT_TECHTREE_WINDOW_STRING,
	START_INIT_INTRO_WINDOW_STRING,
	START_HIDING_WINDOWS_STRING,
	
	START_INIT_CORE_STRING,
	START_MAIN_INIT_COMPLETE_STRING,	
	START_SYSTEM_READY_STRING,
	START_INITIALIZATION_TIME_STRING,

	START_CHOOSING_GAME_TAB_STRING,
	START_CREATING_GAME_STRING,
	START_ASSIGNING_MAP_STRING,
	START_ASSIGNING_START_CONDITION_STRING,
	START_SETTING_RACE_STRING,
	START_ASSIGNING_GOAL_STRING,
	START_CREATING_START_UNITS_STRING,
	START_ASSIGNING_HARVEST_SPEED_STRING,
	START_SETTING_START_POSITION_STRING,
	START_CREATING_BUILD_ORDERS_STRING,
	START_MEASURING_FITNESS_STRING,
	START_SHOWING_GAME_STRING,


        START_SDL_NO_MODES_AVAILIBLE_STRING,
        START_SDL_ALL_RESOLUTIONS_AVAILIBLE_STRING,
        START_SDL_AVAILIBLE_MODES_STRING,
        START_SDL_MAX_COLOR_DEPTH_STRING,
        START_SDL_HARDWARE_SURFACES_POSSIBLE_STRING,
        START_SDL_WINDOW_MANAGER_AVAILIBLE_STRING,
        START_SDL_HARDWARE_TO_HARDWARE_BLITS_ACCELERATED_STRING,
        START_SDL_HARDWARE_TO_HARDWARE_COLORKEY_BLITS_ACCELERATED_STRING,
        START_SDL_HARDWARE_TO_HARDWARE_ALPHA_BLITS_ACCELERATED_STRING,
        START_SDL_SOFTWARE_TO_HARDWARE_BLITS_ACCELERATED_STRING,
        START_SDL_SOFTWARE_TO_HARDWARE_COLORKEY_BLITS_ACCELERATED_STRING,
        START_SDL_SOFTWARE_TO_HARDWARE_ALPHA_BLITS_ACCELERATED_STRING,
        START_SDL_COLOR_FILLS_ACCELERATED_STRING,
        START_SDL_TOTAL_VIDEO_MEMORY_STRING,
        START_SDL_SURFACE_STORED_IN_SYSTEM_MEMORY_STRING,
        START_SDL_SURFACE_STORED_IN_VIDEO_MEMORY_STRING,
        START_SDL_SURFACE_USES_ASYNCHRONOUS_BLITS_STRING,
        START_SDL_SURFACE_ALLOWS_ANY_PIXEL_FORMAT_STRING,
        START_SDL_SURFACE_HAS_EXCLUSIVE_PALETTE_STRING,
        START_SDL_SURFACE_IS_DOUBLE_BUFFERED_STRING,
        START_SDL_SURFACE_HAS_OPENGL_CONTEXT_STRING,
        START_SDL_SURFACE_SUPPORTS_OPENGL_BLITTING_STRING,
        START_SDL_SURFACE_IS_RESIZABLE_STRING,
        START_SDL_SURFACE_BLIT_USES_HARDWARE_ACCELERATION_STRING,
        START_SDL_SURFACE_USES_COLORKEY_BLITTING_STRING,
        START_SDL_COLORKEY_BLITTING_RLE_ACCELERATED_STRING,
        START_SDL_BLIT_USES_ALPHA_BLENDING_STRING,
	START_SDL_SURFACE_USES_PREALLOCATED_MEMORY_STRING,
	START_SDL_SURFACE_NEEDS_LOCKING_STRING,


//	END_FREEING_BUILD_ORDERS_STRING,
//	END_FREEING_ANALYZED_DATA_STRING, // TODO :/
	END_RESET_MOUSE_CURSOR_STRING,
	END_FREEING_WINDOWS_STRING,
	END_FREEING_GAMES_STRING,
	END_FREEING_COLORS_BRUSHES_PENS_STRING,
	END_FREEING_BITMAPS_STRING,
	END_FREEING_COORDINATES_STRING,
	END_FREEING_BUTTONS_STRING,
	END_FREEING_FONTS_STRING,
	END_FREEING_SOUNDS_STRING,
	END_CLOSING_SOUND_ENGINE_STRING,
	END_RELEASING_SOUND_ENGINE_STRING,
	END_CLOSING_SDL_STRING,

	CHANGED_BIT_DEPTH_STRING,
	CHANGED_RESOLUTION_STRING,
	
	INTRO_WINDOW_TITLE_STRING,
	MESSAGE_WINDOW_TITLE_STRING,
	HELP_WINDOW_TITLE_STRING,
	SETTINGS_WINDOW_TITLE_STRING, 
	MAP_WINDOW_TITLE_STRING,
	DATABASE_WINDOW_TITLE_STRING,
	
	INFO_WINDOW_TITLE_STRING,
	TECHTREE_WINDOW_TITLE_STRING,
	DEBUG_WINDOW_TITLE_STRING,
	
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
	SAVED_BUILD_ORDER_STRING,

// game window:
	COMPARE_GAME_STRING,
	REMOVE_GAME_STRING,

// help window:
	HELP_WINDOW_INDEX_STRING,
	HELP_WINDOW_BACK_STRING,
	
// edit field window:
	SAVE_BOX_OK_STRING,
	SAVE_BOX_CANCEL_STRING,

// debug window:
	DRAW_TICKS_STRING,
	GENERATION_TICKS_STRING,
	PROCESS_TICKS_STRING,
	MESSAGE_TICKS_STRING,
	SOUND_TICKS_STRING,
	IDLE_TICKS_STRING,
	FPS_TEXT_STRING,
	GPS_TEXT_STRING,

	
// force window:
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

	CHOOSE_GOAL_NAME_STRING,
	
	SAVE_BUILD_ORDER_AS_STRING,
	GIVE_BO_A_NAME_STRING, //40

	BODIAGRAM_SUPPLY_STRING,
	BODIAGRAM_TIME_STRING,

// bowindow
	CLICK_TO_INSERT_ORDER_STRING, //44
//	CLICK_TO_CLOSE_ORDER_STRING, schon in force definiert
//	CLICK_TO_GO_BACK_STRING,

	OPTIMIZE_EVERYTHING_STRING,
	OPTIMIZE_SELECTED_STRING,
	RESET_BUILD_ORDER_STRING,
	SAVE_BUILD_ORDER_STRING,
	LOAD_BUILD_ORDER_STRING,

	BOWINDOW_BUILD_TIME_STRING,
	BOWINDOW_EACH_TOTAL_STRING,

	PLAYER_STRING,
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

// Tabs
	HELP_TAB_STRING,
	SETTINGS_TAB_STRING,
	DATABASE_TAB_STRING,
	MAP_TAB_STRING, 

// tooltips
	ADD_UNIT_TOOLTIP_STRING,
	REMOVE_UNIT_TOOLTIP_STRING,
	REMOVE_GOAL_TOOLTIP_STRING,

	CHOOSE_RACE_TOOLTIP_STRING,
	ADD_GOALS_TOOLTIP_STRING, // 90
	CHOOSE_GOALS_TOOLTIP_STRING,
	CHOOSE_STARTING_FORCE_TOOLTIP_STRING,

	SAVE_GOAL_TOOLTIP_STRING,

	RESET_BUILD_ORDER_TOOLTIP_STRING,
	SAVE_BUILD_ORDER_TOOLTIP_STRING,
	LOAD_BUILD_ORDER_TOOLTIP_STRING,

// timer:
	CONTINUE_OPTIMIZATION_TOOLTIP_STRING, 
	PAUSE_OPTIMIZATION_TOOLTIP_STRING, 
	OF_GOALS_FULFILLED_TOOLTIP_STRING,
	OF_TIME_FULFILLED_TOOLTIP_STRING,

	HELP_TAB_TOOLTIP_STRING,	
	SETTINGS_TAB_TOOLTIP_STRING, //110
	DATABASE_TAB_TOOLTIP_STRING,
	MAP_TAB_TOOLTIP_STRING,

	FORCEENTRY_TIME_TOOLTIP_STRING,
	FORCEENTRY_ALWAYS_BUILD_TOOLTIP_STRING,
	FORCEENTRY_BUILD_MUCH_TOOLTIP_STRING,

// settings:
	SETTING_FAST_CALCULATION_STRING,
	SETTING_ALLOW_WAIT_ORDERS_STRING,
	SETTING_WAIT_ACCURACY_STRING,
	SETTING_GAME_SPEED_STRING,
	SETTING_AUTO_RUNS_STRING,
	SETTING_MAX_GENERATIONS_STRING,

	SETTING_USE_MUSIC_STRING,
	SETTING_USE_SOUND_STRING,
	SETTING_MUSIC_VOLUME_STRING,
	SETTING_SOUND_VOLUME_STRING,
	SETTING_CHANNELS_STRING,
	
	SETTING_BACKGROUND_BITMAP_STRING,
	SETTING_SMOOTH_MOVEMENT_STRING,
	SETTING_WAIT_AFTER_CHANGE_STRING,
	SETTING_TOOLTIPS_STRING,
	SETTING_DNA_SPIRAL_STRING,
	SETTING_RACE_SPECIFIC_THEME_STRING,
	SETTING_GLOWING_BUTTONS_STRING,
	SETTING_COMPACT_DISPLAY_MODE_STRING,
	SETTING_FACILITY_MODE_STRING,

	SETTING_FULLSCREEN_STRING,
	SETTING_UNLOAD_GRAPHICS_STRING,
	SETTING_SHOW_DEBUG_STRING,
	SETTING_DESIRED_FRAMERATE_STRING,
	SETTING_DESIRED_CPU_USAGE_STRING,

	SETTING_RELOAD_FROM_FILE_STRING,
	SETTING_SAVE_TO_FILE_STRING,
	SETTING_LOAD_DEFAULTS_STRING,

// tooltips	
	SETTING_FAST_CALCULATION_TOOLTIP_STRING,
	SETTING_ALLOW_WAIT_ORDERS_TOOLTIP_STRING,
	SETTING_WAIT_ACCURACY_TOOLTIP_STRING,
	SETTING_GAME_SPEED_TOOLTIP_STRING,
	SETTING_AUTO_RUNS_TOOLTIP_STRING,
	SETTING_MAX_GENERATIONS_TOOLTIP_STRING,

	SETTING_USE_MUSIC_TOOLTIP_STRING,
	SETTING_USE_SOUND_TOOLTIP_STRING,
	SETTING_MUSIC_VOLUME_TOOLTIP_STRING,
	SETTING_SOUND_VOLUME_TOOLTIP_STRING,
	SETTING_CHANNELS_TOOLTIP_STRING,

	SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING,
	SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING,
	SETTING_WAIT_AFTER_CHANGE_TOOLTIP_STRING,
	SETTING_TOOLTIPS_TOOLTIP_STRING,
	SETTING_DNA_SPIRAL_TOOLTIP_STRING,
	SETTING_RACE_SPECIFIC_THEME_TOOLTIP_STRING,
	SETTING_GLOWING_BUTTONS_TOOLTIP_STRING,
	SETTING_COMPACT_DISPLAY_MODE_TOOLTIP_STRING,
	SETTING_FACILITY_MODE_TOOLTIP_STRING,
	
	SETTING_FULLSCREEN_TOOLTIP_STRING,
	SETTING_UNLOAD_GRAPHICS_TOOLTIP_STRING,
	SETTING_SHOW_DEBUG_TOOLTIP_STRING,
	SETTING_DESIRED_FRAMERATE_TOOLTIP_STRING,
	SETTING_DESIRED_CPU_USAGE_TOOLTIP_STRING,

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
	SETTING_DARK_RED_THEME_STRING,
	SETTING_DARK_BLUE_THEME_STRING,
	SETTING_GREEN_THEME_STRING,
	SETTING_YELLOW_THEME_STRING,
	SETTING_GREY_THEME_STRING,

	SETWINDOW_CORE_SETTINGS_STRING,
	SETWINDOW_SOUND_SETTINGS_STRING,
	SETWINDOW_GUI_SETTINGS_STRING,
	SETWINDOW_GRAPHIC_SETTINGS_STRING,
	
	SETWINDOW_UI_SETTINGS_STRING,
	SETWINDOW_LOADSAVE_SETTINGS_STRING,

	DATA_ENTRY_OPEN_BUTTON_TOOLTIP_STRING,
	DATA_ENTRY_CHECK_BUTTON_TOOLTIP_STRING,
	


	MAX_STRINGS
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
	LOCATION_BUTTON_BITMAP,
	TIME_BUTTON_BITMAP,
	MOUSE_NONE,
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_BOTH,

	RADIO_OFF,
	RADIO_ON,

	BACKGROUND_SC_BITMAP,

	BAR_BITMAP,
	KEY_BITMAP,

	CLAWSOFTWARE_BITMAP,
	CLAWSOFTWARE_MONO_BITMAP,

	NEW_BITMAP, //21
	LOAD_BITMAP, 
	SAVE_BITMAP, 
	REFRESH_BITMAP, 

	BACK_BITMAP, //25
	FORWARD_BITMAP, 
	ADD_BITMAP, 
	DELETE_BITMAP, 

	HELP_BITMAP, 
	MAP_BITMAP, // 30
	SETTING_BITMAP, 
	BACKGROUND_WH40K_BITMAP, 

	BULLET_BITMAP, 
	SCORE_ACTIVE_BITMAP,
	SCORE_EMPTY_BITMAP, //35

	FORCE_WINDOW_BITMAP,
	BODIAGRAM_WINDOW_BITMAP,
	BOGRAPH_WINDOW_BITMAP,
	BO_WINDOW_BITMAP,
	CLEMENS_BITMAP, //40
	LIST_BITMAP,

//	OPEN_TREE_BITMAP,	
	
	ARROW_LEFT_BITMAP, //42
	ARROW_LEFT_CLICKED_BITMAP,
	ARROW_RIGHT_BITMAP,
	ARROW_RIGHT_CLICKED_BITMAP,

	ARROW_UP_BITMAP,
	ARROW_UP_CLICKED_BITMAP,
	ARROW_DOWN_BITMAP,
	ARROW_DOWN_CLICKED_BITMAP,

	LEMMINGS_BITMAP,

	UGLY_UP_BITMAP,
	UGLY_KING_BITMAP,
	OH_BITMAP,
	COOL_BITMAP,
	BIGGRIN_BITMAP,
	BIGGRINHAT_BITMAP,

	TITLE_SC_BITMAP,
	TITLE_BW_BITMAP,
	TITLE_WC3_BITMAP,
	
/*	CLICKED_SMALL_ARROW_LEFT_BITMAP,
	CLICKED_SMALL_ARROW_RIGHT_BITMAP,
	CLICKED_ARROW_LEFT_BITMAP,
	CLICKED_ARROW_RIGHT_BITMAP,
	CLICKED_SMALL_ARROW_UP_BITMAP,
	CLICKED_SMALL_ARROW_DOWN_BITMAP,
	CLICKED_ARROW_UP_BITMAP,
	CLICKED_ARROW_DOWN_BITMAP,*/
	
	HELP_MAIN_BITMAP,

	THE_QUESTION_BITMAP,
	
	MAX_BITMAPS
};
// ------ END BITMAPS ------

// ------ GENERAL RECTANGLES ------
enum eGlobalWindow
{
	NULL_GLOBAL_WINDOW,
	
	MAIN_WINDOW,
	INTRO_WINDOW,
	MESSAGE_WINDOW,
	HELP_WINDOW,
	SETTINGS_WINDOW,
	DATABASE_WINDOW,
	DATALIST_WINDOW,
	MAP_WINDOW,
	
	INFO_WINDOW,
	TECHTREE_WINDOW,
	SAVE_BOX_WINDOW,
	DEBUG_WINDOW,
	
	
	MAX_GLOBAL_WINDOWS
};

enum eGameWindow
{
	NULL_GAME_WINDOW,

	GAME_WINDOW,
	SCORE_WINDOW,
//	DATABASE_WINDOW, TODO... was ueberlegen

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

	DOCK_CLOSE_WITH_LEFT_BORDER_OF_COMMAND,
	DOCK_CLOSE_WITH_RIGHT_BORDER_OF_COMMAND,
	DOCK_CLOSE_WITH_LOWER_BORDER_OF_COMMAND,
	DOCK_CLOSE_WITH_UPPER_BORDER_OF_COMMAND,
	DOCK_CLOSE_BOTTOM_CENTER_INSIDE_OF_COMMAND,
	DOCK_CLOSE_TOP_CENTER_INSIDE_OF_COMMAND,
	DOCK_CLOSE_LEFT_INSIDE_OF_COMMAND,
	DOCK_CLOSE_RIGHT_INSIDE_OF_COMMAND,
	DOCK_CLOSE_TOP_INSIDE_OF_COMMAND,
	DOCK_CLOSE_TOP_LEFT_INSIDE_OF_COMMAND,
	DOCK_CLOSE_TOP_RIGHT_INSIDE_OF_COMMAND,
	DOCK_CLOSE_BOTTOM_INSIDE_OF_COMMAND,
	MAX_COMMANDS
};


enum eButtonColorsType // TODO ueberarbeiten... 
{
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
	
	STANDARD_BUTTON,
	FORCE_ENTRY_BUTTON,
	MODIFY_BUTTON,
	TAB_BUTTON,
	MENU_ENTRY_BUTTON,
	EDIT_FIELD_BUTTON,
	ARROW_BUTTON,
	GOAL_LOCATION_BUTTON,
	GOAL_TIME_BUTTON,
	CHECK_BUTTON,
	OPEN_TREE_BUTTON,
	NON_GOAL_ENTRY_BUTTON,
	TEXT_BUTTON,
	VISITED_TEXT_BUTTON,
	MENU_BUTTON,
	HELP_BUTTON,
	
//	COMPARE_GAME_BUTTON TODO
//	REMOVE_GAME_BUTTON,
	
	MAX_BUTTON_COLORS_TYPES
};

enum eButtonWidthType
{
	TINY_BUTTON_WIDTH,
	SMALL_BUTTON_WIDTH,
	STANDARD_BUTTON_WIDTH,
	LARGE_BUTTON_WIDTH,
	
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
	
//	eBitmap bitmap[MAX_BUTTON_ANIMATION_PHASES];// bitmap animation is fixed... for now

	unsigned int speed; // 100 = 100 steps for full animation
	eButtonAnimationType type;
};
/*
1. Eintraege in den Datendateien mit Nummern versehen
2. In eine map einlesen
3. Nicht zeilenweise sondern direkt nach Nummer zuweisen

Alternativ: Globale Namen fuer jedes item (Konstantennamen?) der in den Konfigurationsdateien auftaucht -> besser, jedoch mehr Arbeit :o
*/

struct BitmapEntry
{
	std::string name;
	bool scale;
	unsigned int line;
	SDL_Surface* bitmap;
	bool used;
	bool solid;
};

class UI_Theme
{
	public:
		UI_Theme();
		~UI_Theme();
		
		const eLanguage getLanguage() const;
		const bool setLanguage(const eLanguage language);

		const eTheme getColorTheme() const;
		const eTheme getMainColorTheme() const;
		void setColorTheme(const eTheme color_theme);
		void setMainColorTheme(const eTheme main_color_theme);
		
		const eResolution getResolution() const;
		void setResolution(const eResolution theme_resolution);

		const Size getResolutionSize(const eResolution resolution) const;
		const Size getCurrentResolutionSize() const;

		const eBitDepth getBitDepth() const;
		void setBitDepth(const eBitDepth theme_bitdepth);
		
		void unloadGraphics();

		const bool loadHelpChapterStringFile(const std::string& help_file);
		void loadStringFiles();
		const bool loadUnitsStringFile(const std::string& units_string_file);
		
		void loadData(const std::string& data_file, const std::string& bitmap_dir, const std::string& font_dir, DC* dc); //currently all data hard coded, move it to file later! TODO
		const bool loadWindowDataFile(const std::string& window_data_file, const unsigned int game_number, const unsigned int max_games);
		void initCursors();

		const std::string& lookUpString(const eString id) const;
		const std::string& lookUpHelpChapterString(const eHelpChapter id) const;
		const std::string lookUpFormattedString(const eString id, const std::string& text) const;
		const std::string lookUpFormattedString(const eString id, const unsigned int i) const;
		const std::string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j) const;
		const std::string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j, const unsigned int k) const;
		Color* lookUpColor(const eColor id) const;
		/*const */SDL_Surface* lookUpBitmap(const eBitmap id);
		SDL_Cursor* lookUpCursor(const eCursor id, const unsigned int animation_phase = 0) const;
		Pen* lookUpPen(const ePen id) const;
		Brush* lookUpBrush(const eBrush id) const;
		Font* lookUpFont(const eFont id) const;

		const Rect lookUpGlobalRect(const eGlobalWindow id) const;
		const Rect lookUpGameRect(const eGameWindow id, const unsigned int game_number, const unsigned int max_games) const;
		const Rect lookUpPlayerRect(const ePlayerWindow id, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_player) const;
		
		const unsigned int lookUpGlobalMaxHeight(const eGlobalWindow id) const;
		const unsigned int lookUpGameMaxHeight(const eGameWindow id, const unsigned int game_number, const unsigned int max_games) const;
		const unsigned int lookUpPlayerMaxHeight(const ePlayerWindow id, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_player) const;

		const ButtonColorsType* lookUpButtonColors(const eButtonColorsType id) const;
		const unsigned int lookUpButtonWidth(const eButtonWidthType id) const;

		void updateColors(SDL_Surface* surface);
		
		const eBitmap getBitmapFromIdentifier(const std::string& identifier) const;

		const bool isLanguageInitialized(const eLanguage language) const;
		std::string printHardwareInformation();
		std::string printSurfaceInformation(DC* surface);
	private:
		void setMaxGlobalHeight(unsigned int current_resolution, unsigned int id, unsigned int max_height);
		void setMaxGameHeight(unsigned int current_resolution, unsigned int game_number, unsigned int max_games, unsigned int id, unsigned int max_height);
		void setMaxPlayerHeight(unsigned int current_resolution, unsigned int game_number, unsigned int max_games, unsigned int player_max, unsigned int player_number, unsigned int id, unsigned int max_height);
 		eResolution currentResolution;
		eBitDepth currentBitDepth;
		eLanguage currentLanguage;
		eTheme currentColorTheme;
		eTheme currentMainColorTheme;

		SDL_Cursor* cursorList[MAX_CURSORS][2];
		SDL_Cursor* defaultCursor;

		void initBitmapIdentifier();
		std::string bitmapIdentifier[MAX_BITMAPS];
		SDL_Surface* bitmapList[MAX_RESOLUTIONS][MAX_COLOR_THEMES][MAX_BITMAPS];
		std::list<BitmapEntry> loadedBitmaps;
		BitmapEntry* bitmapAccessTable[MAX_RESOLUTIONS][MAX_COLOR_THEMES][MAX_BITMAPS];
		
		void initStringIdentifier();
		std::vector<std::string> stringIdentifier;
		std::vector< std::vector<std::string> > stringList;
		std::vector<bool> languageInitialized;
		std::map<eHelpChapter, const std::string> helpChapterStringMap[MAX_LANGUAGES];
		
		Color* colorList[MAX_COLOR_THEMES][MAX_COLORS];
		
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
		std::string buttonIdentifier[MAX_BUTTON_COLORS_TYPES];
		void initButtonIdentifier();
		unsigned int buttonWidthList[MAX_RESOLUTIONS][MAX_BUTTON_WIDTH_TYPES];
};

inline const bool UI_Theme::isLanguageInitialized(const eLanguage language) const 
{
	return(languageInitialized[language]);
}

inline void UI_Theme::setColorTheme(const eTheme color_theme) {
	currentColorTheme = color_theme;
}

inline void UI_Theme::setMainColorTheme(const eTheme main_color_theme) {
	currentMainColorTheme = main_color_theme;
}

inline const eTheme UI_Theme::getColorTheme() const {
	return(currentColorTheme);
}

inline const eTheme UI_Theme::getMainColorTheme() const {
	return(currentMainColorTheme);
}


inline const eLanguage UI_Theme::getLanguage() const {
	return(currentLanguage);
}

inline const eResolution UI_Theme::getResolution() const {
	return(currentResolution);
}

inline const eBitDepth UI_Theme::getBitDepth() const {
	return(currentBitDepth);
}

inline void UI_Theme::setBitDepth(const eBitDepth theme_bitdepth) {
	currentBitDepth = theme_bitdepth;
	// TODO update dc
}

inline SDL_Cursor* UI_Theme::lookUpCursor(const eCursor id, const unsigned int animation_phase) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_CURSORS)||(animation_phase>1)||(cursorList[id][animation_phase]==NULL)) {
		toErrorLog("ERROR: (UI_Theme::lookUpCursor) id/phase out of range.");return(cursorList[0][0]);
	}
#endif
	return(cursorList[id][animation_phase]);
}

inline const Size UI_Theme::getCurrentResolutionSize() const {
	return(getResolutionSize(currentResolution));
}
	

inline const ButtonColorsType* UI_Theme::lookUpButtonColors(const eButtonColorsType id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BUTTON_COLORS_TYPES)||(buttonColorsList[id]==NULL)) {
		toErrorLog("ERROR: (UI_Theme::lookUpButtonColors) id out of range.");return(buttonColorsList[0]);
	}
#endif
	return(buttonColorsList[id]);
}

inline const unsigned int UI_Theme::lookUpButtonWidth(const eButtonWidthType id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BUTTON_WIDTH_TYPES)||(buttonWidthList[id]==NULL)) {
		toErrorLog("ERROR: (UI_Theme::lookUpButtonWidth) id out of range.");return(buttonWidthList[0][0]);
	}
#endif
	return(buttonWidthList[currentResolution][id]);
}

inline const std::string& UI_Theme::lookUpString(const eString id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_STRINGS)) {
		toErrorLog("ERROR: (UI_Theme::lookUpString) id out of range.");return(stringList[currentLanguage][0]);
	}
#endif
	return(stringList[currentLanguage][id]);
}

inline Color* UI_Theme::lookUpColor(const eColor id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_COLORS)) {
		toErrorLog("ERROR: (UI_Theme::lookUpColor) id out of range.");return(colorList[currentColorTheme][id]);
	}
#endif
	return(colorList[currentColorTheme][id]);
}



inline Pen* UI_Theme::lookUpPen(const ePen id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_PENS)) {
		toErrorLog("ERROR: (UI_Theme::lookUpPen) id out of range.");return(penList[currentColorTheme][id]);
	}
#endif
	return(penList[currentColorTheme][id]);
}

inline Brush* UI_Theme::lookUpBrush(const eBrush id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BRUSHES)) {
		toErrorLog("ERROR: (UI_Theme::lookUpBrush) id out of range.");return(brushList[currentColorTheme][id]);
	}
#endif
	return(brushList[currentColorTheme][id]);
}

#endif // _UI_THEME_HPP

