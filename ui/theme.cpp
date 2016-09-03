#include "theme.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

unsigned int FONT_SIZE=6;

UI_Theme::UI_Theme():
	resolution(RESOLUTION_640x480),
	bitdepth(DEPTH_32BIT),
	language(GERMAN_LANGUAGE),
	colorTheme(DARK_BLUE_THEME),
	mainColorTheme(DARK_BLUE_THEME),
	loadedBitmaps()	
{
	for(unsigned int i = MAX_LANGUAGES;i--;)
		for(unsigned int j = MAX_STRINGS;j--;)
			stringList[i][j] = "";
	for(unsigned int i = MAX_RESOLUTIONS;i--;)
//		for(unsigned int j = MAX_LANGUAGES;j--;)
			for(unsigned int k = MAX_FONTS;k--;)
				fontList[i][k] = NULL;
	for(unsigned int i = MAX_COLOR_THEMES;i--;)
	{
		for(unsigned int j = MAX_COLORS;j--;)
			colorList[i][j] = NULL;
		for(unsigned int j = MAX_PENS;j--;)
			penList[i][j] = NULL; // !
		for(unsigned int j = MAX_BRUSHES;j--;)
			brushList[i][j] = NULL;
	}
	for(unsigned int i = MAX_RESOLUTIONS;i--;)
	{
  		for(unsigned int j = MAX_COLOR_THEMES;j--;)
			 for(unsigned int k = MAX_BITMAPS;k--;)
				bitmapList[i][j][k] = NULL;

		for(unsigned int j = MAX_GLOBAL_WINDOWS;j--;)
		{
			globalRectList[i][j] = NULL;
			maxGlobalHeightList[i][j] = 0;
		}
		for(unsigned int j = MAX_COMPARE_GAMES;j--;)
			for(unsigned int k = MAX_COMPARE_GAMES;k--;)
			{
				for(unsigned int l = MAX_GAME_WINDOWS;l--;)
				{
					gameRectList[i][j][k][l] = NULL;
					maxGameHeightList[i][j][k][l] = 0;
				}
		                for(unsigned int l = MAX_PLAYER;l--;)
                		        for(unsigned int m = MAX_PLAYER;m--;)
                                		for(unsigned int n = MAX_PLAYER_WINDOWS;n--;)
		                                {
							playerRectList[i][j][k][l][m][n] = NULL;
							maxPlayerHeightList[i][j][k][l][m][n] = 0;
						}
			}
		for(unsigned int j=MAX_BUTTON_WIDTH_TYPES;j--;)
			buttonWidthList[i][j] = 0;
	}
	for(unsigned int i = MAX_BUTTON_COLORS_TYPES;i--;)
		buttonColorsList[i] = NULL;
	for(unsigned int i = MAX_SOUNDS;i--;)
		soundList[i] = NULL;

}

UI_Theme::~UI_Theme()
{
	for(unsigned int i = MAX_RESOLUTIONS;i--;)
//		for(unsigned int j = MAX_LANGUAGES;j--;)
			for(unsigned int k = MAX_FONTS;k--;)
				delete fontList[i][k];
	for(unsigned int i = MAX_COLOR_THEMES;i--;)
	{
		for(unsigned int j = MAX_COLORS;j--;)
			delete colorList[i][j];
		for(unsigned int j = MAX_BRUSHES;j--;)
			delete brushList[i][j];
		for(unsigned int j = MAX_PENS;j--;)
			delete penList[i][j];
	}
	for(std::list<BitmapEntry>::iterator l = loadedBitmaps.begin(); l!=loadedBitmaps.end();++l)
		SDL_FreeSurface(l->bitmap);
	
        for(unsigned int i = MAX_RESOLUTIONS;i--;)
        {
                for(unsigned int j = MAX_GLOBAL_WINDOWS;j--;)
			delete globalRectList[i][j];
                for(unsigned int j = MAX_COMPARE_GAMES;j--;)
                        for(unsigned int k = MAX_COMPARE_GAMES;k--;)
			{
                                for(unsigned int l = MAX_GAME_WINDOWS;l--;)
                                        delete gameRectList[i][j][k][l];
				for(unsigned int l = MAX_PLAYER;l--;)
					for(unsigned int m = MAX_PLAYER;m--;)
						for(unsigned int n = MAX_PLAYER_WINDOWS;n--;)
							delete playerRectList[i][j][k][l][m][n];
			}
        }

	for(unsigned int i=MAX_BUTTON_COLORS_TYPES;i--;)
		delete buttonColorsList[i];
}

void UI_Theme::unloadGraphicsAndSounds()
{
	for(std::list<BitmapEntry>::iterator i = loadedBitmaps.begin(); i!=loadedBitmaps.end(); ++i)
		if(!i->used)
		{
			for(unsigned int j = MAX_RESOLUTIONS;j--;)
				for(unsigned int k = MAX_COLOR_THEMES;k--;)
					for(unsigned int l = MAX_BITMAPS;l--;)
						if(i->bitmap == bitmapList[j][k][l])
							bitmapList[j][k][l] = NULL;
			SDL_FreeSurface(i->bitmap);
			i->bitmap = NULL;
			i->used = true;
		} else if(i->bitmap!=NULL)
			i->used = false;
/*	for(std::list<SoundEntry>::iterator i = loadedSounds.begin(); i!=loadedSounds.end(); ++i)
		if(!i->used)
		{
			for(unsigned int j = MAX_SOUNDS;j--;)
				if(i->sound == soundList[j])
					soundList[j] = NULL;
			soundEngine.unload(i->sound);
			i->sound = NULL;
			i->used = true;
		} else if(i->sound!=NULL)
			i->used = false;*/
}

void UI_Theme::setResolution(const eResolution theme_resolution) 
{
	// TODO auslagern in Conf Datei
	resolution = theme_resolution;
	switch(resolution)
	{
		case RESOLUTION_640x480:FONT_SIZE=3;break;
		case RESOLUTION_800x600:FONT_SIZE=6;break;
		case RESOLUTION_1024x768:FONT_SIZE=8;break;
		case RESOLUTION_1280x1024:FONT_SIZE=10;break;
		default:break;
	}
	// TODO update whole engine
}


const std::string& UI_Theme::lookUpHelpChapterString(const eHelpChapter id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_HELP_CHAPTER)||(helpChapterStringMap[language].find(id) == helpChapterStringMap[language].end())) {
		toLog("ERROR: (UI_Theme::lookUpHelpChapterString) id out of range.");
		return(helpChapterStringMap[language].find(INDEX_CHAPTER)->second);
	}
#endif
	return(helpChapterStringMap[language].find(id)->second);
}

void findandreplace( std::string& source, const std::string& find, const std::string& replace )
{
	size_t j = source.find(find);
	if(j==std::string::npos) {
#ifdef _SCC_DEBUG
		toLog("WARNING: (UI_Theme::findandreplace) string not found.");
#endif
		return;
	}
	source.replace(j,find.length(),replace);
}

const std::string UI_Theme::lookUpFormattedString(const eString id, const std::string& text) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_STRINGS)) {
		toLog("ERROR: (UI_Theme::lookUpFormattedString) id out of range.");return("");
	}
#endif
	std::string bla = stringList[language][id];
	findandreplace(bla, "%s", text);
	return(bla);
}

const std::string UI_Theme::lookUpFormattedString(const eString id, const unsigned int i) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_STRINGS)) {
		toLog("ERROR: (UI_Theme::lookUpFormattedString) id out of range.");return("");
	}
#endif
	std::string bla = stringList[language][id];
	std::ostringstream os;
	os.str("");
	os << i; 
	findandreplace(bla, "%i", os.str());
	return(bla);
}
const std::string UI_Theme::lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j, const unsigned int k) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_STRINGS)) {
		toLog("ERROR: (UI_Theme::lookUpFormattedString) id out of range.");return("");
	}
#endif
	std::string bla = stringList[language][id];
	std::ostringstream os;
	os.str("");
	os << i;findandreplace(bla, "%i", os.str());os.str("");
	os << j;findandreplace(bla, "%i", os.str());os.str("");
	os << k;findandreplace(bla, "%i", os.str());
	return(bla);
}

const std::string UI_Theme::lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_STRINGS)) {
		toLog("ERROR: (UI_Theme::lookUpFormattedString) id out of range.");return("");
	}
#endif
	std::string bla = stringList[language][id];
	std::ostringstream os;
	os.str("");
	if(bla.find("%2i")!=std::string::npos)
	{
		os << std::setfill('0') << std::setw(2) << i;
		findandreplace(bla, "%2i", os.str());os.str("");
	}
	else
	{
		os << i;
		findandreplace(bla, "%i", os.str());os.str("");
	}
	if(bla.find("%2i")!=std::string::npos)
	{
		os << std::setfill('0') << std::setw(2) << j;
		findandreplace(bla, "%2i", os.str());os.str("");
	}
	else
	{
		os << j;
		findandreplace(bla, "%i", os.str());os.str("");
	}
	return(bla);
}

const Rect UI_Theme::lookUpGlobalRect(const eGlobalWindow id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_GLOBAL_WINDOWS)) {
		toLog("ERROR: (UI_Theme::lookUpGlobalRect) id out of range.");return(Rect(0,0,0,0));
	}
#endif
	return(*globalRectList[resolution][id]);
}

const unsigned int UI_Theme::lookUpGlobalMaxHeight(const eGlobalWindow id) const 
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_GLOBAL_WINDOWS)) {
		toLog("ERROR: (UI_Theme::lookUpGlobalMaxHeight) id out of range.");return(0);
	}
#endif
	return(maxGlobalHeightList[resolution][id]);
}

const Rect UI_Theme::lookUpGameRect(const eGameWindow id, const unsigned int gameNumber, const unsigned int maxGames) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_GAME_WINDOWS)) {
		toLog("ERROR: (UI_Theme::lookUpGameRect) id out of range.");return(Rect(0,0,0,0));
	}
	if(maxGames==0) {
		toLog("ERROR: (UI_Theme::lookUpGameRect) maxGames out of range.");return(Rect(0,0,0,0));
	}
	if((gameNumber>=maxGames)||(maxGames>MAX_COMPARE_GAMES)) {
		toLog("ERROR: (UI_Theme::lookUpGameRect) game out of range.");return(Rect(0,0,0,0));
	}
#endif
	return(*gameRectList[resolution][gameNumber][maxGames-1][id]);
}

const unsigned int UI_Theme::lookUpGameMaxHeight(const eGameWindow id, const unsigned int gameNumber, const unsigned int maxGames) const 
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_GAME_WINDOWS)) {
		toLog("ERROR: (UI_Theme::lookUpGameMaxHeight) id out of range.");return(0);
	}
	if(maxGames==0) {
		toLog("ERROR: (UI_Theme::lookUpGameMaxHeight) maxGames out of range.");return(0);
	}
	if((gameNumber>=maxGames)||(maxGames>MAX_COMPARE_GAMES)) {
		toLog("ERROR: (UI_Theme::lookUpGameMaxHeight) game out of range.");return(0);
	}
#endif
	return(maxGameHeightList[resolution][gameNumber][maxGames-1][id]);
}

const Rect UI_Theme::lookUpPlayerRect(const ePlayerWindow id, const unsigned int gameNumber, const unsigned int maxGames, const unsigned int playerNumber, const unsigned int maxPlayer) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_PLAYER_WINDOWS)) {
		toLog("ERROR: (UI_Theme::lookUpPlayerRect) id out of range.");return(Rect(0,0,0,0));
	}
	if(maxPlayer==0) {
		toLog("ERROR: (UI_Theme::lookUpPlayerRect) maxPlayer out of range.");return(Rect(0,0,0,0));
	}
	if((playerNumber>=maxPlayer)||(maxPlayer>MAX_PLAYER)) {
		toLog("ERROR: (UI_Theme::lookUpPlayerRect) player out of range.");return(Rect(0,0,0,0));
	}	
#endif
	return(*playerRectList[resolution][gameNumber][maxGames-1][maxPlayer-1][playerNumber][id]);
}

const unsigned int UI_Theme::lookUpPlayerMaxHeight(const ePlayerWindow id, const unsigned int gameNumber, const unsigned int maxGames, const unsigned int playerNumber, const unsigned int maxPlayer) const 
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_PLAYER_WINDOWS)) {
		toLog("ERROR: (UI_Theme::lookUpMaxHeight) id out of range.");return(0);
	}
	if(maxPlayer==0) {
		toLog("ERROR: (UI_Theme::lookUpPlayerMaxHeight) maxPlayer out of range.");return(0);
	}
	if((playerNumber>=maxPlayer)||(maxPlayer>MAX_PLAYER)) {
		toLog("ERROR: (UI_Theme::lookUpPlayerRect) player out of range.");return(0);
	}
#endif
	return(maxPlayerHeightList[resolution][gameNumber][maxGames-1][maxPlayer-1][playerNumber][id]);
}

const ePlayerType getPlayerType(const std::string& item)
{
	if(item=="@PLAYER_ONE") return(PLAYER_ONE_TYPE); else
	if(item=="@PLAYER_TWO") return(PLAYER_TWO_TYPE); else
	if(item=="@PLAYER_THREE") return(PLAYER_THREE_TYPE); else
	if(item=="@PLAYER_FOUR") return(PLAYER_FOUR_TYPE); else
	return(ZERO_PLAYER_TYPE);
}

const eGameType getGameType(const std::string& item)
{
	if(item=="@MAIN") return(GAME_MAIN_TYPE); else
	if(item=="@1PLAYER") return(GAME_1PLAYER_TYPE); else
	if(item=="@2PLAYER") return(GAME_2PLAYER_TYPE); else
	if(item=="@3PLAYER") return(GAME_3PLAYER_TYPE); else
	if(item=="@4PLAYER") return(GAME_4PLAYER_TYPE); else
	return(ZERO_GAME_TYPE);
}

const eDataType getDataType(const std::string& item)
{
	if(item=="@STRINGS") return(STRING_DATA_TYPE);else
	if(item=="@HELP_STRINGS") return(HELP_STRING_DATA_TYPE);else
	if(item=="@FONTS") return(FONT_DATA_TYPE);else
	if(item=="@MAIN") return(WINDOW_DATA_TYPE);else
	if(item=="@COLORS") return(COLOR_DATA_TYPE);else
	if(item=="@PENS") return(PEN_DATA_TYPE);else
	if(item=="@BRUSHES") return(BRUSH_DATA_TYPE);else
	if(item=="@BITMAPS") return(BITMAP_DATA_TYPE);else
	if(item=="@BUTTON_COLORS") return(BUTTON_COLOR_DATA_TYPE);else
	if(item=="@SOUNDS") return(SOUND_DATA_TYPE);else
	if(item=="@BUTTON_WIDTH") return(BUTTON_WIDTH_DATA_TYPE);else
	return(ZERO_DATA_TYPE);
}

const eSubDataType getSubDataType(const eDataType mode)
{
	switch(mode)
	{	
		case HELP_STRING_DATA_TYPE:return(LANGUAGE_SUB_DATA_TYPE);
		case STRING_DATA_TYPE:return(LANGUAGE_SUB_DATA_TYPE);
		case FONT_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case BUTTON_WIDTH_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case WINDOW_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case COLOR_DATA_TYPE:return(COLOR_THEME_SUB_DATA_TYPE);
		case BITMAP_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case PEN_DATA_TYPE:return(COLOR_THEME_SUB_DATA_TYPE);
		case BRUSH_DATA_TYPE:return(COLOR_THEME_SUB_DATA_TYPE);
		default:return(ZERO_SUB_DATA_TYPE);
	}
}

const eSubSubDataType getSubSubDataType(const eDataType mode)
{
	switch(mode)
	{
//		case FONT_DATA_TYPE:return(LANGUAGE_SUB_SUB_DATA_TYPE);
		case BITMAP_DATA_TYPE:return(COLOR_THEME_SUB_SUB_DATA_TYPE);
//		case PEN_DATA_TYPE:return(COLOR_THEME_SUB_SUB_DATA_TYPE);
		default:return(ZERO_SUB_SUB_DATA_TYPE);
	}
}

const eLanguage getLanguageSubDataEntry(const std::string& item)
{
	if(item=="@ENGLISH") return(ENGLISH_LANGUAGE);else
	if(item=="@GERMAN") return(GERMAN_LANGUAGE);else
	if(item=="@ITALIAN") return(ITALIAN_LANGUAGE);else
	if(item=="@PORTUGESE") return(PORTUGESE_LANGUAGE);else
	if(item=="@DUTCH") return(DUTCH_LANGUAGE);else
	if(item=="@FINNISH") return(FINNISH_LANGUAGE);else
	if(item=="@GREEK") return(GREEK_LANGUAGE);else
	if(item=="@FRENCH") return(FRENCH_LANGUAGE);else
	if(item=="@SPANISH") return(SPANISH_LANGUAGE);else
	if(item=="@POLSKI") return(POLSKI_LANGUAGE);else
	if(item=="@KOREAN") return(KOREAN_LANGUAGE);else
	if(item=="@CHINESE") return(CHINESE_LANGUAGE);else
	if(item=="@RUSSIAN") return(RUSSIAN_LANGUAGE);else
	return(ZERO_LANGUAGE);
}

const eResolution getResolutionSubDataEntry(const std::string& item)
{
	if(item=="@640x480") return(RESOLUTION_640x480);else
	if(item=="@800x600") return(RESOLUTION_800x600);else
	if(item=="@1024x768") return(RESOLUTION_1024x768);else
	if(item=="@1280x1024") return(RESOLUTION_1280x1024);else
//	if(item=="@1600x1200") return(RESOLUTION_1600x1200);else
	return(ZERO_RESOLUTION);
}

const eTheme getThemeSubDataEntry(const std::string& item)
{
	if(item=="@DARK_RED_THEME") return(DARK_RED_THEME);else
	if(item=="@DARK_BLUE_THEME") return(DARK_BLUE_THEME);else
	if(item=="@GREEN_THEME") return(GREEN_THEME);else
	if(item=="@YELLOW_THEME") return(YELLOW_THEME);else
	if(item=="@GREY_THEME") return(GREY_THEME);else
	return(ZERO_THEME);
}


const eBrushStyle get_brush_style(const std::string& item)
{
	if(item=="SOLID") return(SOLID_BRUSH_STYLE);else
	if(item=="BDIAGONAL_HATCH") return(BDIAGONAL_HATCH_BRUSH_STYLE);else
	if(item=="CROSSDIAG_HATCH") return(CROSSDIAG_HATCH_BRUSH_STYLE);else
	if(item=="FDIAGONAL_HATCH") return(FDIAGONAL_HATCH_BRUSH_STYLE);else
	if(item=="CROSSH_ATCH") return(CROSS_HATCH_BRUSH_STYLE);else
	if(item=="HORIZONTAL_HATCH") return(HORIZONTAL_HATCH_BRUSH_STYLE);else
	if(item=="VERTICAL_HATCH") return(VERTICAL_HATCH_BRUSH_STYLE);else
	if(item=="STIPPLE") return(STIPPLE_BRUSH_STYLE);else
	if(item=="STIPPLE_MASK_OPAQUE") return(STIPPLE_MASK_OPAQUE_BRUSH_STYLE);else
	return(TRANSPARENT_BRUSH_STYLE);
}


const ePenStyle get_pen_style(const std::string& item)
{
	if(item=="SOLID") return(SOLID_PEN_STYLE);else
	if(item=="DOT") return(DOT_PEN_STYLE);else
	if(item=="LONG_DASH") return(LONG_DASH_PEN_STYLE);else
	if(item=="SHORT_DASH") return(SHORT_DASH_PEN_STYLE);else
	if(item=="DOT_DASH") return(DOT_DASH_PEN_STYLE);else
	return(TRANSPARENT_PEN_STYLE);
}
/*
int get_font_style1(const std::string& item)
{
			if(item=="decorative") return(DECORATIVE);else
			if(item=="normal") return(DEFAULT);else
			return(DEFAULT);
// TODO!
}
int get_font_style2(const std::string& item)
{
			if(item=="italics") return(FONTSTYLE_ITALIC);else
			if(item=="normal") return(DEFAULT);else
			return(DEFAULT);
// TODO!
}
int get_font_style3(const std::string& item)
{
			if(item=="bold") return(BOLD);else
			if(item=="normal") return(DEFAULT);else
			return(DEFAULT);
// TODO!
}*/

eGlobalWindow parse_global_window(const std::string& item)
{
	if(item=="Main window") return(MAIN_WINDOW);else
	if(item=="Intro window") return(INTRO_WINDOW);else
	if(item=="Message window") return(MESSAGE_WINDOW);else
	if(item=="Help window") return(HELP_WINDOW);else
	if(item=="Settings window") return(SETTINGS_WINDOW);else
	if(item=="Database window") return(DATABASE_WINDOW);else
	if(item=="Map window") return(MAP_WINDOW);else
	if(item=="Info window") return(INFO_WINDOW);else
	if(item=="Tech tree window") return(TECHTREE_WINDOW);else
	if(item=="Save box window") return(SAVE_BOX_WINDOW);else
	return(NULL_GLOBAL_WINDOW);
}

eGameWindow parse_game_window(const std::string& item)
{
	if(item=="Game window") return(GAME_WINDOW);else
	if(item=="Score window") return(SCORE_WINDOW);else
//	if(item=="Database window") return(DATABASE_WINDOW);else TODO
	return(NULL_GAME_WINDOW);
}

ePlayerWindow parse_player_window(const std::string& item)
{
	if(item=="Player window") return(PLAYER_WINDOW);else
	if(item=="Force window") return(FORCE_WINDOW);else
	if(item=="Build order window") return(BUILD_ORDER_WINDOW);else
	if(item=="Build order diagram window") return(BUILD_ORDER_DIAGRAM_WINDOW);else
	if(item=="Build order graph window") return(BUILD_ORDER_GRAPH_WINDOW);else
	return(NULL_PLAYER_WINDOW);
}

eCommand parse_commands(const std::string& item, const bool horizontal_mirror)
{
	if(horizontal_mirror)
	{
		if(item=="absolute coordinates") return(ABSOLUTE_COORDINATES_COMMAND);else
		if(item=="absolute x coordinate") return(ABSOLUTE_X_COORDINATE_COMMAND);else
		if(item=="absolute y coordinate") return(ABSOLUTE_Y_COORDINATE_COMMAND);else
		if(item=="dock with left border of") return(DOCK_WITH_RIGHT_BORDER_OF_COMMAND);else
		if(item=="dock with right border of") return(DOCK_WITH_LEFT_BORDER_OF_COMMAND);else
		if(item=="dock with lower border of") return(DOCK_WITH_LOWER_BORDER_OF_COMMAND);else
		if(item=="dock with upper border of") return(DOCK_WITH_UPPER_BORDER_OF_COMMAND);else
		if(item=="dock center inside of") return(DOCK_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock bottom center inside of") return(DOCK_BOTTOM_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock top center inside of") return(DOCK_TOP_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock left inside of") return(DOCK_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock right inside of") return(DOCK_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock top inside of") return(DOCK_TOP_INSIDE_OF_COMMAND);else
		if(item=="dock top left inside of") return(DOCK_TOP_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock top right inside of") return(DOCK_TOP_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock bottom inside of") return(DOCK_BOTTOM_INSIDE_OF_COMMAND);else

		if(item=="dock close with left border of") return(DOCK_CLOSE_WITH_RIGHT_BORDER_OF_COMMAND);else
		if(item=="dock close with right border of") return(DOCK_CLOSE_WITH_LEFT_BORDER_OF_COMMAND);else
		if(item=="dock close with lower border of") return(DOCK_CLOSE_WITH_LOWER_BORDER_OF_COMMAND);else
		if(item=="dock close with upper border of") return(DOCK_CLOSE_WITH_UPPER_BORDER_OF_COMMAND);else
		if(item=="dock close bottom center inside of") return(DOCK_CLOSE_BOTTOM_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock close top center inside of") return(DOCK_CLOSE_TOP_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock close left inside of") return(DOCK_CLOSE_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock close right inside of") return(DOCK_CLOSE_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock close top inside of") return(DOCK_CLOSE_TOP_INSIDE_OF_COMMAND);else
		if(item=="dock close top left inside of") return(DOCK_CLOSE_TOP_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock close top right inside of") return(DOCK_CLOSE_TOP_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock close bottom inside of") return(DOCK_CLOSE_BOTTOM_INSIDE_OF_COMMAND);else
			
		return(NO_COMMAND);
	} else
	{
		if(item=="absolute coordinates") return(ABSOLUTE_COORDINATES_COMMAND);else
		if(item=="absolute x coordinate") return(ABSOLUTE_X_COORDINATE_COMMAND);else
		if(item=="absolute y coordinate") return(ABSOLUTE_Y_COORDINATE_COMMAND);else
		if(item=="dock with left border of") return(DOCK_WITH_LEFT_BORDER_OF_COMMAND);else
		if(item=="dock with right border of") return(DOCK_WITH_RIGHT_BORDER_OF_COMMAND);else
		if(item=="dock with lower border of") return(DOCK_WITH_LOWER_BORDER_OF_COMMAND);else
		if(item=="dock with upper border of") return(DOCK_WITH_UPPER_BORDER_OF_COMMAND);else
		if(item=="dock center inside of") return(DOCK_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock bottom center inside of") return(DOCK_BOTTOM_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock top center inside of") return(DOCK_TOP_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock left inside of") return(DOCK_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock right inside of") return(DOCK_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock top inside of") return(DOCK_TOP_INSIDE_OF_COMMAND);else
		if(item=="dock top left inside of") return(DOCK_TOP_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock top right inside of") return(DOCK_TOP_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock bottom inside of") return(DOCK_BOTTOM_INSIDE_OF_COMMAND);else
	
		if(item=="dock close with left border of") return(DOCK_CLOSE_WITH_LEFT_BORDER_OF_COMMAND);else
		if(item=="dock close with right border of") return(DOCK_CLOSE_WITH_RIGHT_BORDER_OF_COMMAND);else
		if(item=="dock close with lower border of") return(DOCK_CLOSE_WITH_LOWER_BORDER_OF_COMMAND);else
		if(item=="dock close with upper border of") return(DOCK_CLOSE_WITH_UPPER_BORDER_OF_COMMAND);else
		if(item=="dock close bottom center inside of") return(DOCK_CLOSE_BOTTOM_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock close top center inside of") return(DOCK_CLOSE_TOP_CENTER_INSIDE_OF_COMMAND);else
		if(item=="dock close left inside of") return(DOCK_CLOSE_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock close right inside of") return(DOCK_CLOSE_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock close top inside of") return(DOCK_CLOSE_TOP_INSIDE_OF_COMMAND);else
		if(item=="dock close top left inside of") return(DOCK_CLOSE_TOP_LEFT_INSIDE_OF_COMMAND);else
		if(item=="dock close top right inside of") return(DOCK_CLOSE_TOP_RIGHT_INSIDE_OF_COMMAND);else
		if(item=="dock close bottom inside of") return(DOCK_CLOSE_BOTTOM_INSIDE_OF_COMMAND);else
		
		return(NO_COMMAND);
	}
}

Rect* parse_window(const std::string* parameter, Rect** windows, unsigned int& maxheight, int type, const bool horizontal_mirror)
{
	Rect* rect = new Rect();
	bool xpart=false; bool ypart=false; bool xypart=false; bool dxpart=true; bool dypart=false; bool hpart=false;

	for(unsigned int i = 1; i < 49; ++i)
	{
		if(dxpart)
		{
			rect->SetWidth(atoi(parameter[i].c_str()));
			dxpart=false;dypart=true;
		} else
		if(dypart)
		{
			rect->SetHeight(atoi(parameter[i].c_str()));
			dypart=false;hpart=true;
		} else
		if(hpart)
		{
			maxheight = atoi(parameter[i].c_str());
			hpart=false;
		} else
		if(xpart)
		{
			rect->SetLeft(atoi(parameter[i].c_str()));
			xpart=false;
			if(xypart)
			{
				ypart=true;
				xypart=false;
			}
		} else
		if(ypart)
		{
			rect->SetTop(atoi(parameter[i].c_str()));
			ypart=false;
		} else // Befehl
		{
			eCommand command = parse_commands(parameter[i], horizontal_mirror);
			++i;
			int win=0;
			if(type==0) win = parse_global_window(parameter[i]);else
			if(type==1) win = parse_game_window(parameter[i]);else
			if(type==2) win = parse_player_window(parameter[i]);
			if((win==0)&&(command>=DOCK_WITH_LEFT_BORDER_OF_COMMAND))
			{
				toLog(parameter[i]);
			}
			switch(command)
			{
				case ABSOLUTE_COORDINATES_COMMAND:--i;xpart=true;xypart=true;break;
				case ABSOLUTE_X_COORDINATE_COMMAND:--i;xpart=true;break;
				case ABSOLUTE_Y_COORDINATE_COMMAND:--i;ypart=true;break;
				case DOCK_WITH_LEFT_BORDER_OF_COMMAND:rect->SetLeft(-10 + windows[win]->GetLeft() - rect->GetWidth());break;
				case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:rect->SetLeft(0 + windows[win]->GetRight());break;
				case DOCK_WITH_LOWER_BORDER_OF_COMMAND:rect->SetTop(10 + windows[win]->GetBottom());break;
				case DOCK_WITH_UPPER_BORDER_OF_COMMAND:rect->SetTop(20 + windows[win]->GetTop() - rect->GetHeight());break;

				case DOCK_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(15 + windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, 15 + windows[win]->GetHeight() / 2 - rect->GetHeight() / 2));break;
				case DOCK_BOTTOM_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, windows[win]->GetHeight() - rect->GetHeight() - 10));break;
				case DOCK_TOP_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, 15));break;
				case DOCK_LEFT_INSIDE_OF_COMMAND:rect->SetLeft(0);break;
				case DOCK_RIGHT_INSIDE_OF_COMMAND:rect->SetLeft(windows[win]->GetWidth() - rect->GetWidth());break;
				case DOCK_TOP_INSIDE_OF_COMMAND:rect->SetTop(0);break;
				case DOCK_TOP_LEFT_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(0, 0));break;
				case DOCK_TOP_RIGHT_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() - rect->GetWidth(), 0));break;
				case DOCK_BOTTOM_INSIDE_OF_COMMAND:rect->SetTop(-15+windows[win]->GetHeight() - rect->GetHeight());break;
	
								   
				case DOCK_CLOSE_WITH_LEFT_BORDER_OF_COMMAND:rect->SetLeft(windows[win]->GetLeft() - rect->GetWidth());break;
				case DOCK_CLOSE_WITH_RIGHT_BORDER_OF_COMMAND:rect->SetLeft(windows[win]->GetRight());break;
				case DOCK_CLOSE_WITH_LOWER_BORDER_OF_COMMAND:rect->SetTop(windows[win]->GetBottom());break;
				case DOCK_CLOSE_WITH_UPPER_BORDER_OF_COMMAND:rect->SetTop(windows[win]->GetTop() - rect->GetHeight());break;

				case DOCK_CLOSE_BOTTOM_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, windows[win]->GetHeight() - rect->GetHeight()));break;
				case DOCK_CLOSE_TOP_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, 0));break;
				case DOCK_CLOSE_LEFT_INSIDE_OF_COMMAND:rect->SetLeft(0);break;
				case DOCK_CLOSE_RIGHT_INSIDE_OF_COMMAND:rect->SetLeft(windows[win]->GetWidth() - rect->GetWidth());break;
				case DOCK_CLOSE_TOP_INSIDE_OF_COMMAND:rect->SetTop(0);break;
				case DOCK_CLOSE_TOP_LEFT_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(0, 0));break;
				case DOCK_CLOSE_TOP_RIGHT_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() - rect->GetWidth(), 0));break;
				case DOCK_CLOSE_BOTTOM_INSIDE_OF_COMMAND:rect->SetTop(windows[win]->GetHeight() - rect->GetHeight());break;							   
								   
/*				case DOCK_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetLeft() + windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, windows[win]->GetTop() + windows[win]->GetHeight() / 2 - rect->GetHeight() / 2));break;
				case DOCK_BOTTOM_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetLeft() + windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, windows[win]->GetBottom() - rect->GetHeight() - 35));break;
				case DOCK_LEFT_INSIDE_OF_COMMAND:rect->SetLeft(10+windows[win]->GetLeft());break;
				case DOCK_RIGHT_INSIDE_OF_COMMAND:rect->SetLeft(-15+windows[win]->GetRight() - rect->GetWidth());break;
				case DOCK_TOP_INSIDE_OF_COMMAND:rect->SetTop(windows[win]->GetTop());break;
				case DOCK_BOTTOM_INSIDE_OF_COMMAND:rect->SetTop(-15+windows[win]->GetBottom() - rect->GetHeight());break;*/
				default:--i;;break;
			}
		}
	}
	return(rect);
}


void UI_Theme::loadHelpChapterStringFile(const std::string& data_file)
{
	if((data_file.substr(data_file.size()-2,2) == "..") ||(data_file.substr(data_file.size()-1,1) == "."))
		return;
	char line[1024];
	std::string entry;
	eDataType mode = ZERO_DATA_TYPE;
	eSubDataType sub_mode = ZERO_SUB_DATA_TYPE;
	eSubSubDataType sub_sub_mode = ZERO_SUB_SUB_DATA_TYPE;
	eLanguage current_language = ZERO_LANGUAGE;
	eResolution current_resolution=ZERO_RESOLUTION;

	eTheme current_theme=ZERO_THEME;
	eHelpChapter chapter = MAX_HELP_CHAPTER;
	
	std::ifstream pFile(data_file.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadHelpChapterStringFile) Could not open file! [" + data_file + "]");
#endif
		return;
	}
	
	while(pFile.getline(line, 1024))
	{
		
		if(pFile.fail())
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadHelpChapterStringFile) Long line!");
#endif
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		}
			
		//line[strlen(line)-1]='\0';
		if((line[0]=='#')||(line[0]=='\0')||(line=="")) continue;
		
		char* line2=line;		
		while(((*line2)==32)||((*line2)==9))
			++line2;
		if((*line2)=='\0')
			continue;
		entry = line2;
		
		// mode	
		if(mode==ZERO_DATA_TYPE)
		{
			mode=getDataType(entry);
#ifdef _SCC_DEBUG
			if(mode==ZERO_DATA_TYPE)
			{
				if(entry=="@END")
					toLog("WARNING: (UI_Theme::loadHelpChapterStringFile) Lonely @END.");
				else
					toLog("WARNING: (UI_Theme::loadHelpChapterStringFile) Line is outside a block but is not marked as comment.");
			}
#endif				
			sub_mode=getSubDataType(mode);
			sub_sub_mode=getSubSubDataType(mode);
		}
		else if(mode!=ZERO_DATA_TYPE)
		{
			if(entry=="@END")
			{
			// TODO! 
				// @END of 1st sub area => return to begin of data type

// - deepness |1|: end of SUB-MODE
				if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE)&&
				  ((current_language!=ZERO_LANGUAGE)||(current_resolution!=ZERO_RESOLUTION)||(current_theme!=ZERO_THEME)))
				{
					if(chapter!=MAX_HELP_CHAPTER)
						chapter = MAX_HELP_CHAPTER;
					else
					{
						current_language=ZERO_LANGUAGE;
						current_resolution=ZERO_RESOLUTION;
						current_theme=ZERO_THEME;
					}
				}
				// @END of 2nd sub area => return to begin of sub data type
				else 
				if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&
// - deepness |2|: end of SUB-SUB-MODE
				((current_language!=ZERO_LANGUAGE)||(current_theme!=ZERO_THEME)))
				{
					current_language=ZERO_LANGUAGE;
					current_theme=ZERO_THEME;
				}
				// @END  of 1st sub area (with an existing sub sub area...)
// - deepness |2|: end of SUB-MODE
				else if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&
// sub-sub-items already closed -> close sub-item
						(current_resolution!=ZERO_RESOLUTION)&&(current_language==ZERO_LANGUAGE)&&(current_theme==ZERO_THEME))
				{
					current_resolution=ZERO_RESOLUTION;
				}
				// @END of 0 area => reset mode
				else
				{
					mode=ZERO_DATA_TYPE;
					sub_mode=ZERO_SUB_DATA_TYPE;
					sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;
				}
			}
			else
			if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(current_language==ZERO_LANGUAGE)&&(current_resolution==ZERO_RESOLUTION)&&(current_theme==ZERO_THEME))
			{
				switch(sub_mode)
				{
					case LANGUAGE_SUB_DATA_TYPE:current_language = getLanguageSubDataEntry(entry);break;
					case RESOLUTION_SUB_DATA_TYPE:current_resolution = getResolutionSubDataEntry(entry);break;
					case COLOR_THEME_SUB_DATA_TYPE:current_theme = getThemeSubDataEntry(entry);break;
					default:break;
				}
			}
			// => hat nur 1 Ebene => Position festgestellt!
			else if((sub_mode != ZERO_SUB_DATA_TYPE)&&(sub_sub_mode == ZERO_SUB_SUB_DATA_TYPE))
			{
				if(chapter == MAX_HELP_CHAPTER)
				{
					if(entry[0] == '@')
						chapter = (eHelpChapter)atoi(entry.substr(1).c_str());
				} else
				switch(mode)
				{
					case HELP_STRING_DATA_TYPE:
						if(helpChapterStringMap[current_language].find(chapter)==helpChapterStringMap[current_language].end())
							helpChapterStringMap[current_language].insert(std::pair<const eHelpChapter, const std::string>(chapter, entry));
						else
						{
							entry = helpChapterStringMap[current_language][chapter] + entry;
							helpChapterStringMap[current_language].erase(chapter);
							helpChapterStringMap[current_language].insert(std::pair<const eHelpChapter, const std::string>(chapter, entry));
						}
						break;
					default:break;
				}
			}
			// 0 ebenen -> buttons :) BUTTON_COLORS_DATA_TYPE?? TODO
		} // end if mode != ZERO_DATA_TYPE
	} // end while
}


void UI_Theme::loadStringFile(const std::string& data_file)
{
	if((data_file.substr(data_file.size()-2,2) == "..") ||(data_file.substr(data_file.size()-1,1) == "."))
		return;
	const unsigned int MAX_PARAMETERS = 50;
	char line[1024], old[1024];
	char* buffer;
	std::string parameter[MAX_PARAMETERS];
	unsigned int value[MAX_PARAMETERS];
	eDataType mode=ZERO_DATA_TYPE;
	eSubDataType sub_mode=ZERO_SUB_DATA_TYPE;
	eSubSubDataType sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;

	eLanguage current_language=ZERO_LANGUAGE;
	eTheme current_theme=ZERO_THEME;
	eResolution current_resolution=ZERO_RESOLUTION;

	unsigned int current_line = 0;
	
 	for(unsigned int i=MAX_PARAMETERS;i--;)
	{
		parameter[i]="";
		value[i]=0;
	}

	std::ifstream pFile(data_file.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadStringFile) Could not open file! [" + data_file + "]");
#endif
		return;
	}
	
	while(pFile.getline(line, 1024))
	{
		if(pFile.fail())
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadStringFile) Long line!");
#endif
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		}
			
		//line[strlen(line)-1]='\0';
		if((line[0]=='#')||(line[0]=='\0')||(line=="")) continue;
		for(unsigned int i=MAX_PARAMETERS;i--;)
		{
			parameter[i]="";
			value[i]=0;
		}
		char* line2=line;		
		while(((*line2)==32)||((*line2)==9))
			++line2;
		if((*line2)=='\0')
			continue;
		
		strcpy(old,line2);
		
		if((buffer=strtok(line2,",\0"))!=NULL)
			parameter[0]=buffer;
		unsigned int k=1;
		
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
			while(((*buffer)==32)||((*buffer)==9))
				++buffer;
			parameter[k]=buffer;
			++k;
		}
		if((buffer=strtok(NULL,",\0"))!=NULL)
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadStringFile) Too many parameters.");
#endif
			continue;
		}
		for(unsigned int j=MAX_PARAMETERS;j--;)
			value[j]=atoi(parameter[j].c_str());
		// mode	
		if(mode==ZERO_DATA_TYPE)
		{
			mode=getDataType(parameter[0]);
#ifdef _SCC_DEBUG
//			if(mode!=ZERO_DATA_TYPE)
//				toLog("Loading "+parameter[0]+"...");
			if(mode==ZERO_DATA_TYPE)
			{
				if(parameter[0]=="@END")
					toLog("WARNING: (UI_Theme::loadStringFile) Lonely @END.");
				else
					toLog("WARNING: (UI_Theme::loadStringFile) Line is outside a block but is not marked as comment.");
			}
#endif				
			sub_mode=getSubDataType(mode);
			sub_sub_mode=getSubSubDataType(mode);
		}
		else if(mode!=ZERO_DATA_TYPE)
		{
			if(parameter[0]=="@END")
			{
			// TODO! 
				// @END of 1st sub area => return to begin of data type

// - deepness |1|: end of SUB-MODE
				if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE)&&
				  ((current_language!=ZERO_LANGUAGE)||(current_resolution!=ZERO_RESOLUTION)||(current_theme!=ZERO_THEME)))
				{
					current_language=ZERO_LANGUAGE;
					current_resolution=ZERO_RESOLUTION;
					current_theme=ZERO_THEME;
					current_line=0;
				}
				// @END of 2nd sub area => return to begin of sub data type
				else 
				if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&
// - deepness |2|: end of SUB-SUB-MODE
				((current_language!=ZERO_LANGUAGE)||(current_theme!=ZERO_THEME)))
				{
					current_language=ZERO_LANGUAGE;
					current_theme=ZERO_THEME;
					current_line=0;
				}
				// @END  of 1st sub area (with an existing sub sub area...)
// - deepness |2|: end of SUB-MODE
				else if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&
// sub-sub-items already closed -> close sub-item
						(current_resolution!=ZERO_RESOLUTION)&&(current_language==ZERO_LANGUAGE)&&(current_theme==ZERO_THEME))
				{
					current_resolution=ZERO_RESOLUTION;
					current_line=0;
				}
				// @END of 0 area => reset mode
				else
				{
					mode=ZERO_DATA_TYPE;
					sub_mode=ZERO_SUB_DATA_TYPE;
					sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;
					current_line=0;
				}
			}
			else
			if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(current_language==ZERO_LANGUAGE)&&(current_resolution==ZERO_RESOLUTION)&&(current_theme==ZERO_THEME))
			{
				switch(sub_mode)
				{
					case LANGUAGE_SUB_DATA_TYPE:current_language = getLanguageSubDataEntry(line2);break;
					case RESOLUTION_SUB_DATA_TYPE:current_resolution = getResolutionSubDataEntry(line2);break;
					case COLOR_THEME_SUB_DATA_TYPE:current_theme = getThemeSubDataEntry(line2);break;
					default:break;
				}
				current_line = 0;
			}
			// => hat nur 1 Ebene => Position festgestellt!
			else if((sub_mode != ZERO_SUB_DATA_TYPE)&&(sub_sub_mode == ZERO_SUB_SUB_DATA_TYPE))
			{
				switch(mode)
				{
					case STRING_DATA_TYPE:stringList[current_language][current_line] = parameter[0];
					default:break;
				}
				++current_line;
			}
			// 0 ebenen -> buttons :) BUTTON_COLORS_DATA_TYPE?? TODO
		} // end if mode != ZERO_DATA_TYPE
	} // end while

	for(unsigned int i = MAX_LANGUAGES; i--;)
		for(unsigned int j = MAX_STRINGS; j--;)
			if(stringList[i][j] == "")
				stringList[i][j] = "ERROR";

}

void UI_Theme::loadWindowData(const std::string& data_file, const unsigned int gameNumber, const unsigned int maxGames)
{
	const unsigned int MAX_PARAMETERS = 50;
	char line[1024], old[1024];
	char* buffer;
	std::string parameter[MAX_PARAMETERS];
	unsigned int value[MAX_PARAMETERS];
	
	eGameType game_type = ZERO_GAME_TYPE;
	ePlayerType player_type = ZERO_PLAYER_TYPE;
	eResolution current_resolution = ZERO_RESOLUTION;

	unsigned int current_line = 0;
	
 	for(unsigned int i=MAX_PARAMETERS;i--;)
	{
		parameter[i]="";
		value[i]=0;
	}

	std::ifstream pFile(data_file.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadWindowData) Could not open data file!");
#endif
		return;
	}
	
	while(pFile.getline(line, 1024))
	{
		if(pFile.fail())
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadWindowData) Long line!");
#endif
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		}
			
		//line[strlen(line)-1]='\0';
		if((line[0]=='#')||(line[0]=='\0')||(line=="")) continue;
		for(unsigned int i=MAX_PARAMETERS;i--;)
		{
			parameter[i]="";
			value[i]=0;
		}
		char* line2=line;		
		while(((*line2)==32)||((*line2)==9)) ++line2;
		if((*line2)=='\0') continue;
		
		strcpy(old,line2);
		
		if((buffer=strtok(line2,",\0"))!=NULL) parameter[0]=buffer;
		
		unsigned int k=1;
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
			while(((*buffer)==32)||((*buffer)==9))
				++buffer;
			parameter[k]=buffer;
			++k;
		}
		if((buffer=strtok(NULL,",\0"))!=NULL)
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadWindowData) Too many parameters.");
#endif
			continue;
		}
		for(unsigned int j=MAX_PARAMETERS;j--;)
			value[j]=atoi(parameter[j].c_str());
	
	
		if(current_resolution==ZERO_RESOLUTION)
		{
			current_resolution = getResolutionSubDataEntry(parameter[0]);
#ifdef _SCC_DEBUG
//			if(mode!=ZERO_DATA_TYPE)
//				toLog("Loading "+parameter[0]+"...");
			if(current_resolution==ZERO_RESOLUTION)
			{
				if(parameter[0]=="@END")
					toLog("WARNING: (UI_Theme::loadWindowData) Lonely @END.");
				else
					toLog("WARNING: (UI_Theme::loadWindowData) Line is outside a block but is not marked as comment.");
			}
#endif
		}
		else if(current_resolution!=ZERO_RESOLUTION)
		{
			if(parameter[0]=="@END")
			{
				current_line=0;
				if(game_type==ZERO_GAME_TYPE) current_resolution=ZERO_RESOLUTION;
				else if(game_type==GAME_MAIN_TYPE) game_type=ZERO_GAME_TYPE;
				else if(player_type==ZERO_PLAYER_TYPE) game_type=ZERO_GAME_TYPE;
				else player_type=ZERO_PLAYER_TYPE;
			}
			else if(current_resolution==ZERO_RESOLUTION)
			{
				current_resolution = getResolutionSubDataEntry(line2);
				current_line=0;
			} else if(game_type==ZERO_GAME_TYPE)
			{
				game_type=getGameType(line2);
				current_line=0;
			} else if(game_type==GAME_MAIN_TYPE)
			{
				unsigned int max_height=0;
				gameRectList[current_resolution][gameNumber][maxGames-1][current_line+1] = parse_window(parameter, gameRectList[current_resolution][gameNumber][maxGames-1], max_height, 1, (gameNumber==1));
				if((gameNumber==1)&&(current_line==0))
					gameRectList[current_resolution][gameNumber][maxGames-1][current_line+1]->SetTopLeft(gameRectList[current_resolution][gameNumber][maxGames-1][current_line+1]->GetTopLeft() + Size(globalRectList[current_resolution][MAIN_WINDOW]->GetWidth()/2,0));
				setMaxGameHeight(current_resolution, gameNumber, maxGames-1, current_line+1, max_height);
				++current_line;
			}
			else if(player_type==ZERO_PLAYER_TYPE)
			{
				player_type=getPlayerType(line2);
				current_line=0;
			} else
			{
				unsigned int max_height=0;
				playerRectList[current_resolution][gameNumber][maxGames-1][game_type-2][player_type-1][current_line+1] = parse_window(parameter, playerRectList[current_resolution][gameNumber][maxGames-1][game_type-2][player_type-1], max_height, 2, (gameNumber==1));
				setMaxPlayerHeight(current_resolution, gameNumber, maxGames-1, game_type-2, player_type-1, current_line+1, max_height);
				++current_line;
			}
		} 
	} 
}

// after a bit-depth change
void UI_Theme::updateColors(SDL_Surface* surface)
{
	for(unsigned int i = MAX_COLOR_THEMES;i--;)
	{
		for(unsigned int j = MAX_COLORS;j--;)
			if(colorList[i][j]!=NULL)
				colorList[i][j]->updateColor(surface);
		for(unsigned int j = MAX_BRUSHES;j--;)
			if(brushList[i][j]!=NULL)
				brushList[i][j]->updateColor(surface);
		for(unsigned int j = MAX_PENS;j--;)
			if(penList[i][j]!=NULL)
				penList[i][j]->updateColor(surface);
	}
}

void UI_Theme::loadData(const std::string& data_file, const std::string& bitmap_dir, const std::string& sound_dir, const std::string& font_dir, DC* dc, SDL_snd& sound)
{
	const unsigned int MAX_PARAMETERS = 50;
	char line[1024], old[1024];
	char* buffer;
	std::string parameter[MAX_PARAMETERS];
	unsigned int value[MAX_PARAMETERS];
	eDataType mode=ZERO_DATA_TYPE;
	eSubDataType sub_mode=ZERO_SUB_DATA_TYPE;
	eSubSubDataType sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;

	eLanguage current_language=ZERO_LANGUAGE;
	eTheme current_theme=ZERO_THEME;
	eResolution current_resolution=ZERO_RESOLUTION;

	unsigned int current_line = 0;
	
 	for(unsigned int i=MAX_PARAMETERS;i--;)
	{
		parameter[i]="";
		value[i]=0;
	}

	std::ifstream pFile(data_file.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadGraphicData) Could not open data file!");
#endif
		return;
	}
	
	while(pFile.getline(line, 1024))
	{
		if(pFile.fail())
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadGraphicData) Long line!");
#endif
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		}
			
		//line[strlen(line)-1]='\0';
		if((line[0]=='#')||(line[0]=='\0')||(line=="")) continue;
		for(unsigned int i=MAX_PARAMETERS;i--;)
		{
			parameter[i]="";
			value[i]=0;
		}
		char* line2=line;		
		while(((*line2)==32)||((*line2)==9))
			++line2;
		if((*line2)=='\0')
			continue;
		
		strcpy(old,line2);
		
		if((buffer=strtok(line2,",\0"))!=NULL)
			parameter[0]=buffer;
		unsigned int k=1;
		
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
			while(((*buffer)==32)||((*buffer)==9))
				++buffer;
			parameter[k]=buffer;
			++k;
		}
		
		if((buffer=strtok(NULL,",\0"))!=NULL)
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadGraphicData) Too many parameters.");
#endif
			continue;
		}
		for(unsigned int j=MAX_PARAMETERS;j--;)
			value[j]=atoi(parameter[j].c_str());
		// mode	
		if(mode==ZERO_DATA_TYPE)
		{
			mode=getDataType(parameter[0]);
#ifdef _SCC_DEBUG
//			if(mode!=ZERO_DATA_TYPE)
//				toLog("Loading "+parameter[0]+"...");
			if(mode==ZERO_DATA_TYPE)
			{
				if(parameter[0]=="@END")
					toLog("WARNING: (UI_Theme::loadGraphicData) Lonely @END.");
				else
					toLog("WARNING: (UI_Theme::loadGraphicData) Line is outside a block but is not marked as comment.");
			}
#endif				
			sub_mode=getSubDataType(mode);
			sub_sub_mode=getSubSubDataType(mode);
		}
		else if(mode!=ZERO_DATA_TYPE)
		{
			if(parameter[0]=="@END")
			{

			
			// TODO! 
				// @END of 1st sub area => return to begin of data type

// - deepness |1|: end of SUB-MODE
				if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE)&&
				  ((current_language!=ZERO_LANGUAGE)||(current_resolution!=ZERO_RESOLUTION)||(current_theme!=ZERO_THEME)))
				{
					current_language=ZERO_LANGUAGE;
					current_resolution=ZERO_RESOLUTION;
					current_theme=ZERO_THEME;
					current_line=0;
				}
				// @END of 2nd sub area => return to begin of sub data type
				else 
				if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&
// - deepness |2|: end of SUB-SUB-MODE
				((current_language!=ZERO_LANGUAGE)||(current_theme!=ZERO_THEME)))
				{
					current_language=ZERO_LANGUAGE;
					current_theme=ZERO_THEME;
					current_line=0;
				}
				// @END  of 1st sub area (with an existing sub sub area...)
// - deepness |2|: end of SUB-MODE
				else if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&
// sub-sub-items already closed -> close sub-item
						(current_resolution!=ZERO_RESOLUTION)&&(current_language==ZERO_LANGUAGE)&&(current_theme==ZERO_THEME))
				{
					current_resolution=ZERO_RESOLUTION;
					current_line=0;
				}
				// @END of 0 area => reset mode
				else
				{
					mode=ZERO_DATA_TYPE;
					sub_mode=ZERO_SUB_DATA_TYPE;
					sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;
					current_line=0;
				}
			}
			else
			if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(current_language==ZERO_LANGUAGE)&&(current_resolution==ZERO_RESOLUTION)&&(current_theme==ZERO_THEME))
			{
				switch(sub_mode)
				{
					case LANGUAGE_SUB_DATA_TYPE:current_language=getLanguageSubDataEntry(line2);break;
					case RESOLUTION_SUB_DATA_TYPE:current_resolution=getResolutionSubDataEntry(line2);break;
					case COLOR_THEME_SUB_DATA_TYPE:current_theme=getThemeSubDataEntry(line2);break;
					default:break;
				}
				current_line=0;
			}
			// => hat nur 1 Ebene => Position festgestellt!
			else if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE))
			{
				switch(mode)
				{
					case STRING_DATA_TYPE:stringList[current_language][current_line] = parameter[0]; //? TODO raus?
					//toLog(parameter[0]);
					break;
					case COLOR_DATA_TYPE:
								  colorList[current_theme][current_line]=new Color(dc->GetSurface(),(Uint8)value[0],(Uint8)value[1],(Uint8)value[2]);break;
					case PEN_DATA_TYPE:penList[current_theme][current_line]=new Pen(dc->GetSurface(),value[1],value[2],value[3],value[0],get_pen_style(parameter[4]));break;
					case BRUSH_DATA_TYPE:brushList[current_theme][current_line]=new Brush(dc->GetSurface(),(Uint8)value[0],(Uint8)value[1],(Uint8)value[2],get_brush_style(parameter[3]));break;
					case FONT_DATA_TYPE:
					{
						std::string t=font_dir+parameter[0]+".ttf";
						bool is_under_lined = (parameter[2] == "underlined") || (parameter[3] == "underlined");
						bool is_shadow = (parameter[2] == "shadow") || (parameter[3] == "shadow");
						fontList[current_resolution]/*[current_language]*/[current_line] = new Font(t, value[1], is_under_lined, is_shadow/*, get_font_style1(parameter[2]), get_font_style2(parameter[3]), get_font_style3(parameter[4]), false, _T(""), FONTENCODING_DEFAULT*/);
//						std::ostringstream os;
//						os.str("");
//						os << "- " << current_resolution << " " << current_line << " " << t << " " << value[1];
//						toLog(os.str());
					}break;
					case BUTTON_WIDTH_DATA_TYPE:
					{
						buttonWidthList[current_resolution][current_line] = value[0];
					}break;
					case WINDOW_DATA_TYPE:
					{
						unsigned int max_height = 0;
						globalRectList[current_resolution][current_line+1] = parse_window(parameter, globalRectList[current_resolution], max_height, 0, false);
						setMaxGlobalHeight(current_resolution, current_line+1, max_height);
					}break;
					default:break;
				}
				++current_line;
			}
			// 0 ebenen -> buttons :) BUTTON_COLORS_DATA_TYPE?? TODO
			else if((sub_mode==ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE))
			{
				if(mode==BUTTON_COLOR_DATA_TYPE)
				{
					buttonColorsList[current_line] = new ButtonColorsType;
					buttonColorsList[current_line]->speed=value[0];
					buttonColorsList[current_line]->type=(eButtonAnimationType)value[1];
					for(unsigned int i=MAX_BUTTON_ANIMATION_PHASES;i--;)
					{
						buttonColorsList[current_line]->startBrush[i]=(eBrush)(value[0*MAX_BUTTON_ANIMATION_PHASES+2+i]);
						buttonColorsList[current_line]->endBrush[i]=(eBrush)(value[1*MAX_BUTTON_ANIMATION_PHASES+2+i]);
						buttonColorsList[current_line]->startTextColor[i]=(eColor)(value[2*MAX_BUTTON_ANIMATION_PHASES+2+i]);
						buttonColorsList[current_line]->endTextColor[i]=(eColor)(value[3*MAX_BUTTON_ANIMATION_PHASES+2+i]);
						buttonColorsList[current_line]->startBorderPen[i]=(ePen)(value[4*MAX_BUTTON_ANIMATION_PHASES+2+i]);
						buttonColorsList[current_line]->endBorderPen[i]=(ePen)(value[5*MAX_BUTTON_ANIMATION_PHASES+2+i]);
						buttonColorsList[current_line]->bitmap[i]=(eBitmap)(value[6*MAX_BUTTON_ANIMATION_PHASES+2+i]);
	//					buttonColorsList[current_line]->text[i]=(eString)(value[7*MAX_BUTTON_ANIMATION_PHASES+2+i]);
					}
					++current_line;
				} else if(mode == SOUND_DATA_TYPE)
				{
					std::string name;
						
					if((parameter[0].size()<4)||(parameter[0][parameter[0].size()-4]!='.'))
						name = sound_dir + parameter[0] + ".mp3";
					else name = sound_dir + parameter[0];
					bool found_sound = false;
					for(std::list<SoundEntry>::iterator i = loadedSounds.begin(); i!=loadedSounds.end(); ++i)
						// already loaded?
						if(i->name == name)
						{
							found_sound = true;
							soundAccessTable[current_line] = &(*i);
							break;
						}
					if(!found_sound)
					{
						SoundEntry entry;
						entry.line = current_line;
						entry.name = name;
						entry.sound = NULL;//temp;
						entry.used = false;
						loadedSounds.push_back(entry);
						soundAccessTable[current_line] = &(loadedSounds.back());
					}
					
//					if(soundList[id] == NULL)
// reload
					{
						toLog("Loading " + soundAccessTable[current_line]->name);
						const SndInfo* temp = sound.load(soundAccessTable[current_line]->name.c_str());
						if(temp == NULL)
						{
							toLog("Could not load sound " + soundAccessTable[current_line]->name + " : " + SDL_GetError());
							return;
						}
						soundAccessTable[current_line]->sound = temp;
						soundList[current_line] = temp;
					}
					soundAccessTable[current_line]->used = true;
					++current_line;
				}
			}
			// => es gibt noch eine 2. Ebene
			else if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&(current_language==ZERO_LANGUAGE)&&(current_theme==ZERO_THEME))
			{
				switch(sub_sub_mode)
				{
					case LANGUAGE_SUB_SUB_DATA_TYPE:current_language=getLanguageSubDataEntry(line2);break;
					case COLOR_THEME_SUB_SUB_DATA_TYPE:current_theme=getThemeSubDataEntry(line2);break;
					default:break;
				}			
 				current_line=0;				
			}
			// => hat 2 Ebenen => Position festgestellt!
			else
			{
				switch(mode)
				{
					case BITMAP_DATA_TYPE:
					{
						std::string name;
						
						if((parameter[0].size()<4)||(parameter[0][parameter[0].size()-4]!='.'))
							name = bitmap_dir + parameter[0] + ".bmp";
						else name = bitmap_dir + parameter[0];
						bool found_bitmap = false;
						for(std::list<BitmapEntry>::iterator i = loadedBitmaps.begin(); i!=loadedBitmaps.end(); ++i)
							// already loaded?
							if(i->name == name)
							{
								found_bitmap = true;
								bitmapAccessTable[current_resolution][current_theme][current_line] = &(*i);
								break;
							}
						if(!found_bitmap)
						{
							BitmapEntry entry;
							entry.resolution = current_resolution;
							entry.theme = current_theme;
							entry.line = current_line;
							entry.name = name;
							entry.bitmap = NULL;//temp;
							entry.used = false;
							entry.solid = (parameter[1] == "(SOLID)");
							loadedBitmaps.push_back(entry);
							bitmapAccessTable[current_resolution][current_theme][current_line] = &(loadedBitmaps.back());
						}
					}break;
					default:break;
				}
				++current_line;
			}
		} // end if mode != ZERO_DATA_TYPE
	} // end while

}

void UI_Theme::setMaxGlobalHeight(unsigned int current_resolution, unsigned int id, unsigned int max_height)
{
#ifdef _SCC_DEBUG
        if(current_resolution>MAX_RESOLUTIONS) {
                toLog("ERROR: (UI_Theme::setMaxGlobalHeightList) resolution out of range.");return;
        }
        if(id>=MAX_GLOBAL_WINDOWS) {
                toLog("ERROR: (UI_Theme::setMaxGlobalHeightList) window out of range.");return;
        }
#endif	
	maxGlobalHeightList[current_resolution][id] = max_height;
}

void UI_Theme::setMaxGameHeight(unsigned int current_resolution, unsigned int gameNumber, unsigned int maxGames, unsigned int id, unsigned int max_height)
{
#ifdef _SCC_DEBUG
        if(current_resolution>MAX_RESOLUTIONS) {
                toLog("ERROR: (UI_Theme::setMaxGameHeight) resolution out of range.");return;
        }
        if(gameNumber>=MAX_COMPARE_GAMES) {
                toLog("ERROR: (UI_Theme::setMaxGameHeight) gameNumber out of range.");return;
        }
        if(maxGames>=MAX_COMPARE_GAMES) {
                toLog("ERROR: (UI_Theme::setMaxGameHeight) maxGames out of range.");return;
        }
        if(id>=MAX_GAME_WINDOWS) {
                toLog("ERROR: (UI_Theme::setMaxGameHeight) id out of range.");return;
        }
#endif	
	maxGameHeightList[current_resolution][gameNumber][maxGames][id]=max_height;
}

void UI_Theme::setMaxPlayerHeight(unsigned int current_resolution, unsigned int gameNumber, unsigned int maxGames, unsigned int player_max, unsigned int playerNumber, unsigned int id, unsigned int max_height)
{
#ifdef _SCC_DEBUG
        if(current_resolution>MAX_RESOLUTIONS) {
                toLog("ERROR: (UI_Theme::setMaxPlayerHeight) resolution out of range.");return;
        }
        if(gameNumber>MAX_COMPARE_GAMES) {
                toLog("ERROR: (UI_Theme::setMaxPlayerHeight) gameNumber out of range.");return;
        }
        if(maxGames>MAX_COMPARE_GAMES) {
		toLog("ERROR: (UI_Theme::setMaxPlayerHeight) maxGames out of range.");return;
        }
	if(player_max>MAX_PLAYER) {
		toLog("ERROR: (UI_Theme::setMaxPlayerHeight) player_max out of range.");return;
        }
	if(playerNumber>MAX_PLAYER) {
		toLog("ERROR: (UI_Theme::setMaxPlayerHeight) playerNumber out of range.");return;
        }
        if(id>=MAX_PLAYER_WINDOWS) {
                toLog("ERROR: (UI_Theme::setMaxPlayerHeight) window out of range.");return;
        }
#endif	
	maxPlayerHeightList[current_resolution][gameNumber][maxGames][player_max][playerNumber][id] = max_height;
}

#include <sstream>
Font* UI_Theme::lookUpFont(const eFont id) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_FONTS)) {
		toLog("ERROR: (UI_Theme::lookUpFont) id out of range.");return(fontList[resolution]/*[language]*/[id]);
	}
#endif
//	std::ostringstream os;
//	os.str("");
//	os << "getting id " << id << " (with resolution " << resolution << ")";
//	toLog(os.str());
	return(fontList[resolution]/*[language]*/[id]);
}

SDL_Surface* UI_Theme::lookUpBitmap(const eBitmap id)
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_BITMAPS)) {
		toLog("ERROR: (UI_Theme::lookUpBitmap) id out of range.");return(NULL); // TODO
	}
#endif
	if(bitmapList[resolution][colorTheme][id] == NULL)
// reload
	{
//		toLog("Loading " + bitmapAccessTable[resolution][colorTheme][id]->name);
		SDL_Surface* temp = IMG_Load(bitmapAccessTable[resolution][colorTheme][id]->name.c_str());
		if(temp == NULL)
		{
			toLog("Could not load Bitmap " + bitmapAccessTable[resolution][colorTheme][id]->name + " : " + IMG_GetError());
			return(NULL);
		}
		if(!bitmapAccessTable[resolution][colorTheme][id]->solid)
			SDL_SetColorKey(temp, SDL_SRCCOLORKEY , SDL_MapRGB(temp->format, 0,0,0));
		bitmapAccessTable[resolution][colorTheme][id]->bitmap = temp;
		bitmapList[resolution][colorTheme][id] = temp;
	}
	bitmapAccessTable[resolution][colorTheme][id]->used = true;
	return(bitmapList[resolution][colorTheme][id]);
}

const SndInfo* UI_Theme::lookUpSound(const eSound id)
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_SOUNDS)) {
		toLog("ERROR: (UI_Theme::lookUpSound) id out of range.");return(NULL); // TODO
	}
#endif
/*	if(soundList[id] == NULL)
// reload
	{
		toLog("Loading " + soundAccessTable[id]->name);
		SndInfo* temp = sound.load(soundAccessTable[id]->name.c_str());
		if(temp == NULL)
		{
			toLog("Could not load Sound " + soundAccessTable[id]->name + " : " + SDL_GetError());
			return(NULL);
		}
		soundAccessTable[id]->sound = temp;
		soundList[id] = temp;
	}
	soundAccessTable[id]->used = true;*/
	return(soundList[id]);
}


