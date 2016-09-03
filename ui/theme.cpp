#include "theme.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

#include <fmod_errors.h>

#include "clock1.xpm"
#include "clock2.xpm"
#include "arrow.xpm"
#include "hand.xpm"

unsigned int FONT_SIZE=6;

UI_Theme::UI_Theme():
	sound(NULL),
	resolution(RESOLUTION_640x480),
	bitdepth(DEPTH_32BIT),
	language(ENGLISH_LANGUAGE),
	colorTheme(DARK_BLUE_THEME),
	mainColorTheme(DARK_BLUE_THEME),
	loadedBitmaps(),
	defaultCursor(NULL)
{
	for(unsigned int i = MAX_LANGUAGES; i--;)
	{
		for(unsigned int j = MAX_STRINGS; j--;)
			if(stringList[i][j] == "")
				stringList[i][j] = "ERROR";
		languageInitialized[i] = false;
	}
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

	for(unsigned int i = MAX_CURSORS; i--;)
		for(unsigned int j = 2; j--;)
			cursorList[i][j] = NULL;
	initStringIdentifier();
	initBitmapIdentifier();
}

UI_Theme::~UI_Theme()
{
	toLog("* Resetting mouse cursor...");
	SDL_SetCursor(defaultCursor);
	for(unsigned int i = MAX_CURSORS; i--;)
		for(unsigned int j = 2; j--;)
			SDL_FreeCursor(cursorList[i][j]);

	toLog("* Freeing colors, brushes and pens...");
	for(unsigned int i = MAX_COLOR_THEMES;i--;)
	{
		for(unsigned int j = MAX_COLORS;j--;)
			delete colorList[i][j];
		for(unsigned int j = MAX_BRUSHES;j--;)
			delete brushList[i][j];
		for(unsigned int j = MAX_PENS;j--;)
			delete penList[i][j];
	}

	toLog("* Freeing bitmaps..."); 
	for(std::list<BitmapEntry>::iterator l = loadedBitmaps.begin(); l!=loadedBitmaps.end();++l)
		SDL_FreeSurface(l->bitmap);

	toLog("* Freeing coordinates...");
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

	toLog("* Freeing buttons...");
	for(unsigned int i=MAX_BUTTON_COLORS_TYPES;i--;)
		delete buttonColorsList[i];

	toLog("* Freeing fonts...");
	for(unsigned int i = MAX_RESOLUTIONS;i--;)
//		for(unsigned int j = MAX_LANGUAGES;j--;)
			for(unsigned int k = MAX_FONTS;k--;)
				delete fontList[i][k];

	toLog("* Freeing sounds...");
	for(unsigned int i=MAX_SOUNDS;i--;)
		soundList[i]->release();

	toLog("* Closing sound engine...");
	sound->close();

	toLog("* Releasing sound engine...");
	sound->release();
}

bool UI_Theme::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::ostringstream os;
		os << "FMOD error! ( "<< result << ") " << FMOD_ErrorString(result);
		toLog(os.str());
		return(false);
	}
	return(true);
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

const Size UI_Theme::getResolutionSize() const
{
	switch(resolution)
	{
		case RESOLUTION_640x480:return(Size(640, 480));break;
		case RESOLUTION_800x600:return(Size(800, 600));break;
		case RESOLUTION_1024x768:return(Size(1024, 768));break;
		case RESOLUTION_1280x1024:return(Size(1280, 1024));break;
//		case RESOLUTION_1600x1200:break;
		default:return(Size(0,0));break;
	}
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
	if(item=="@GENERAL_BITMAPS") return(GENERAL_BITMAP_DATA_TYPE);else
	if(item=="@GENERAL_RESOLUTION_BITMAPS") return(GENERAL_RESOLUTION_BITMAP_DATA_TYPE);else
	if(item=="@GENERAL_THEME_BITMAPS") return(GENERAL_THEME_BITMAP_DATA_TYPE);else
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
		case GENERAL_RESOLUTION_BITMAP_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case BITMAP_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case GENERAL_THEME_BITMAP_DATA_TYPE:return(COLOR_THEME_SUB_DATA_TYPE);
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


const bool UI_Theme::loadHelpChapterStringFile(const std::string& help_file)
{
	if((help_file.substr(help_file.size()-2,2) == "..") ||(help_file.substr(help_file.size()-1,1) == "."))
		return(true);
	eDataType mode = ZERO_DATA_TYPE;
	eSubDataType sub_mode = ZERO_SUB_DATA_TYPE;
	eSubSubDataType sub_sub_mode = ZERO_SUB_SUB_DATA_TYPE;
	eLanguage current_language = ZERO_LANGUAGE;
	eResolution current_resolution=ZERO_RESOLUTION;

	eTheme current_theme=ZERO_THEME;
	eHelpChapter chapter = MAX_HELP_CHAPTER;
	
	std::ifstream pFile(help_file.c_str());
	if(!checkStreamIsOpen(pFile, "UI_Theme::loadHelpChapterStringFile", help_file))
		return(false);
	
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "UI_Theme::loadHelpChapterStringFile", help_file))
			return(false);

		//line[strlen(line)-1]='\0';
		if((line[0]=='#')||(line[0]=='\0')||(line=="")) 
			continue;
		
		char* line2=line;		
		while(((*line2)==32)||((*line2)==9))
			++line2;
		if((*line2)=='\0')
			continue;
		std::string entry = line2;
		
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
	return(true);
}

/*

Moeglichkeit 1:
zweite Datei mit geordneter (wie in UI_Theme) Liste um die in den Sprachdateien gelesenen Strings in Zahlen umzuwandeln
Moeglichkeit 2:
Zugriff auf Strings direkt ueber Erkennungsstring (langsamer aber schneller beim proggen :/ )
*/
const bool UI_Theme::loadStringFile(const std::string& string_file)
{
	if((string_file.substr(string_file.size()-2,2) == "..") ||(string_file.substr(string_file.size()-1,1) == "."))
		return(true);
	eDataType mode=ZERO_DATA_TYPE;
	eSubDataType sub_mode=ZERO_SUB_DATA_TYPE;
	eSubSubDataType sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;

	eLanguage current_language=ZERO_LANGUAGE;

	std::ifstream pFile(string_file.c_str());

	if(!checkStreamIsOpen(pFile, "UI_Theme::loadStringFile", string_file))
		return(false);

	toLog("* Loading " + string_file);

	bool found_any_language_block = false;
	bool found_language_block[MAX_LANGUAGES];
	for(unsigned int i = MAX_LANGUAGES; i--;)
		found_language_block[i] = false;
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "UI_Theme::loadStringFile", string_file))
			return(false);
		
		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start == std::string::npos) || (text[0] == '#') || (text[0] == '\0'))
			continue; // ignore line
		size_t stop = text.find_first_of("\t ", start);
		if(stop == std::string::npos) 
			stop = text.size();
		std::string index = text.substr(start, stop);
		std::string value;
		if(mode==ZERO_DATA_TYPE)
		{
			mode = getDataType(index);
			if(mode==ZERO_DATA_TYPE)
			{
				if(index=="@END")
					toLog("WARNING (UI_Theme::loadStringFile()): Lonely @END => ignoring line.");
				else
					toLog("WARNING (UI_Theme::loadStringFile()): Line '" + index + "' is outside a block but is not marked as comment => ignoring line.");
			} else
			{
				sub_mode=getSubDataType(mode);
				sub_sub_mode=getSubSubDataType(mode);
			}
		}  else
		if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(current_language==ZERO_LANGUAGE))
		{
			switch(sub_mode)
			{
				case LANGUAGE_SUB_DATA_TYPE:
					current_language = getLanguageSubDataEntry(index);
					if(current_language==ZERO_LANGUAGE)
					{
						toLog("ERROR (UI_Theme::loadStringFile()): Invalid language entry '" + index + "'.");
						return(false);
					} else if(languageInitialized[current_language])
					{
						toLog("ERROR (UI_Theme::loadStringFile()): Language '" + index + "' already initialized.");
						return(false);
					}
					else
					{
						found_language_block[current_language] = true;
						found_any_language_block = true;
					}
					    break;
				default:break;
			}
		}
		// => hat nur 1 Ebene => Position festgestellt!
		else if((sub_mode != ZERO_SUB_DATA_TYPE)&&(sub_sub_mode == ZERO_SUB_SUB_DATA_TYPE)&&(current_language!=ZERO_LANGUAGE))
		{
			std::map<std::string, std::list<std::string> >::iterator i;
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			if(!parse_block_map(pFile, block))
			{
				toLog("WARNING (UI_Theme::loadStringFile()): No concluding @END for @STRINGS block was found in file " + string_file + " => trying to parse what we have so far.");
			}
			for(unsigned int j = 0; j < MAX_STRINGS; j++)
			{
				if((i=block.find(stringIdentifier[j]))!=block.end())
				{
					i->second.pop_front();
					stringList[current_language][j] = i->second.front();
					block.erase(i);
				}
			}
			// TODO nicht gefundene Eintraege bemaengeln
		}
		old_pos = pFile.tellg();
	} // end while

	if(!found_any_language_block)
	{
		toLog("ERROR (UI_Theme::loadStringFile()): No language block (@ENGLISH, @GERMAN etc.) was found in file " + string_file + " => ignoring file.");
		return(false);
	}
	for(unsigned int i = MAX_LANGUAGES; i--;)
		if(found_language_block[i])
			languageInitialized[i] = true;
	return(true);
}

const bool UI_Theme::loadWindowDataFile(const std::string& window_data_file, const unsigned int game_number, const unsigned int max_games)
{
	const unsigned int MAX_PARAMETERS = 50;
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

	std::ifstream pFile(window_data_file.c_str());
	if(!checkStreamIsOpen(pFile, "UI_Theme::loadWindowDataFile", window_data_file))
		return(false);
	
	if((game_number==0) && (max_games==1))
		toLog("* Loading window coordinates for single view...");
	else 
	if((game_number==0) && (max_games==2))
		toLog("* Loading left side window coordinates for split view...");
	else 
	if((game_number==1) && (max_games==2))
		toLog("* Loading right side window coordinates for split view...");

	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "UI_Theme::loadHelpChapterStringFile", window_data_file))
			return(false);
			
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
				gameRectList[current_resolution][game_number][max_games-1][current_line+1] = parse_window(parameter, gameRectList[current_resolution][game_number][max_games-1], max_height, 1, (game_number==1));
				if((game_number==1)&&(current_line==0))
					gameRectList[current_resolution][game_number][max_games-1][current_line+1]->SetTopLeft(gameRectList[current_resolution][game_number][max_games-1][current_line+1]->GetTopLeft() + Size(globalRectList[current_resolution][MAIN_WINDOW]->GetWidth()/2,0));
				setMaxGameHeight(current_resolution, game_number, max_games-1, current_line+1, max_height);
				++current_line;
			}
			else if(player_type==ZERO_PLAYER_TYPE)
			{
				player_type=getPlayerType(line2);
				current_line=0;
			} else
			{
				unsigned int max_height=0;
				playerRectList[current_resolution][game_number][max_games-1][game_type-2][player_type-1][current_line+1] = parse_window(parameter, playerRectList[current_resolution][game_number][max_games-1][game_type-2][player_type-1], max_height, 2, (game_number==1));
				setMaxPlayerHeight(current_resolution, game_number, max_games-1, game_type-2, player_type-1, current_line+1, max_height);
				++current_line;
			}
		} 
	} 
	return(true);
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

// TODO: Sounds (wie Bitmaps) dynamisch zur Laufzeit laden
void UI_Theme::loadData(const std::string& data_file, const std::string& bitmap_dir, const std::string& sound_dir, const std::string& font_dir, DC* dc)
{
	const unsigned int MAX_PARAMETERS = 50;
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

	toLog("* Loading data file");

	bool loading_fonts = false;
	bool loading_main_coordinates = false;
	bool loading_colors = false;
	bool loading_pens = false;
	bool loading_brushes = false;
	bool loading_bitmaps = false;
	bool loading_buttons = false;
	bool loading_sounds = false;
	
	std::ifstream pFile(data_file.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadGraphicData) Could not open data file!");
#endif
		return;
	}
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
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
					case COLOR_DATA_TYPE:
						if(!loading_colors)
						{
							loading_colors = true;
							toLog("  - Loading colors...");
						}
						colorList[current_theme][current_line]=new Color(dc->GetSurface(),(Uint8)value[0],(Uint8)value[1],(Uint8)value[2]);
					break;
					case PEN_DATA_TYPE:
						if(!loading_pens)
						{
							loading_pens = true;
							toLog("  - Loading pens...");
						}
						penList[current_theme][current_line]=new Pen(dc->GetSurface(),value[1],value[2],value[3],value[0],get_pen_style(parameter[4]));
						break;
					case BRUSH_DATA_TYPE:
						if(!loading_brushes)
						{
							loading_brushes = true;
							toLog("  - Loading brushes...");
						}
						brushList[current_theme][current_line]=new Brush(dc->GetSurface(),(Uint8)value[0],(Uint8)value[1],(Uint8)value[2],get_brush_style(parameter[3]));
						break;
					case FONT_DATA_TYPE:
						if(!loading_fonts)
						{
							loading_fonts = true;
							toLog("  - Loading fonts...");
						}
						{
							std::string font_name=font_dir+parameter[0]+".ttf";
							bool is_under_lined = (parameter[2] == "underlined") || (parameter[3] == "underlined");
							bool is_shadow = (parameter[2] == "shadow") || (parameter[3] == "shadow");
							fontList[current_resolution]/*[current_language]*/[current_line] = new Font(font_name, value[1], is_under_lined, is_shadow/*, get_font_style1(parameter[2]), get_font_style2(parameter[3]), get_font_style3(parameter[4]), false, _T(""), FONTENCODING_DEFAULT*/);
						}
						break;
					case BUTTON_WIDTH_DATA_TYPE:
					{
						buttonWidthList[current_resolution][current_line] = value[0];
					}break;
					case WINDOW_DATA_TYPE:
						if(!loading_main_coordinates)
						{
							loading_main_coordinates = true;
							toLog("  - Loading main coordinates...");
						}

						{
							unsigned int max_height = 0;
							globalRectList[current_resolution][current_line+1] = parse_window(parameter, globalRectList[current_resolution], max_height, 0, false);
							setMaxGlobalHeight(current_resolution, current_line+1, max_height);
						}
						break;
					case GENERAL_RESOLUTION_BITMAP_DATA_TYPE:
					{
						if(!loading_bitmaps)
						{
							loading_bitmaps = true;
							toLog("  - Loading bitmaps...");
						}
			                        std::map<std::string, std::list<std::string> > block;
			                        pFile.seekg(old_pos);
			                        if(!parse_block_map(pFile, block))
			                        {
			                                toLog("WARNING (UI_Theme::loadDataFile()): No concluding @END for @GENERAL_RESOLUTION_BITMAP_DATA_TYPE block was found in file " + data_file + " => trying to parse what we have so far.");
			                        }
			                        for(unsigned int j = 0; j < MAX_BITMAPS; j++)
			                        {
							std::map<std::string, std::list<std::string> >::iterator i;
                        			        if((i = block.find(bitmapIdentifier[j])) != block.end())
			                                {
			                                        i->second.pop_front(); // Identifier loeschen
								std::string name = i->second.front();
								if((name.size()<4)||(name[name.size()-4]!='.'))
									name = bitmap_dir + name + ".bmp";
								else name = bitmap_dir + name;
								bool found_bitmap = false;
								for(std::list<BitmapEntry>::iterator l = loadedBitmaps.begin(); l!=loadedBitmaps.end(); ++l)
							// already loaded?
									if(l->name == name)
									{
										found_bitmap = true;
										for(unsigned int n = MAX_COLOR_THEMES; n--;)
											bitmapAccessTable[current_resolution][n][j] = &(*l);
										break;
									}
								if(!found_bitmap)
								{
									BitmapEntry entry;
//									entry.resolution = current_resolution; //?
//									entry.theme = current_theme;
									entry.line = j;
									entry.name = name;
									entry.bitmap = NULL;//temp;
									entry.used = false;
									i->second.pop_front();
									entry.solid = ((i->second.size()>0)&&(i->second.front() == "(SOLID)"));
									loadedBitmaps.push_back(entry);
									for(unsigned int n = MAX_COLOR_THEMES; n--;)
										bitmapAccessTable[current_resolution][n][j] = &(loadedBitmaps.back());
								}
			                                        block.erase(i);
			                                }
                        			}
						current_resolution = ZERO_RESOLUTION;
					}break;
					case GENERAL_THEME_BITMAP_DATA_TYPE:
					{
						if(!loading_bitmaps)
						{
							loading_bitmaps = true;
							toLog("  - Loading bitmaps...");
						}					
			                        std::map<std::string, std::list<std::string> > block;
			                        pFile.seekg(old_pos);
			                        if(!parse_block_map(pFile, block))
			                        {
			                                toLog("WARNING (UI_Theme::loadDataFile()): No concluding @END for @GENERAL_THEME_BITMAP_DATA_TYPE block was found in file " + data_file + " => trying to parse what we have so far.");
			                        }
			                        for(unsigned int j = 0; j < MAX_BITMAPS; j++)
			                        {
							std::map<std::string, std::list<std::string> >::iterator i;
                        			        if((i = block.find(bitmapIdentifier[j])) != block.end())
			                                {
			                                        i->second.pop_front(); // Identifier loeschen
								std::string name = i->second.front();
								if((name.size()<4)||(name[name.size()-4]!='.'))
									name = bitmap_dir + name + ".bmp";
								else name = bitmap_dir + name;
								bool found_bitmap = false;
								for(std::list<BitmapEntry>::iterator l = loadedBitmaps.begin(); l!=loadedBitmaps.end(); ++l)
							// already loaded?
									if(l->name == name)
									{
										found_bitmap = true;
										for(unsigned int n = MAX_RESOLUTIONS; n--;)
											bitmapAccessTable[n][current_theme][j] = &(*l);
										break;
									}
								if(!found_bitmap)
								{
									BitmapEntry entry;
//									entry.resolution = current_resolution; //?
//									entry.theme = current_theme;
									entry.line = j;
									entry.name = name;
									entry.bitmap = NULL;//temp;
									entry.used = false;
									i->second.pop_front();
									entry.solid = ((i->second.size()>0)&&(i->second.front() == "(SOLID)"));
									loadedBitmaps.push_back(entry);
									for(unsigned int n = MAX_RESOLUTIONS; n--;)
										bitmapAccessTable[n][current_theme][j] = &(loadedBitmaps.back());
								}
			                                        block.erase(i);
			                                }
                        			}
                        // TODO nicht gefundene Eintraege bemaengeln
						current_theme = ZERO_THEME;
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
					if(!loading_buttons)
					{
						loading_buttons = true;
						toLog("  - Loading buttons...");
					}
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
					if(!loading_sounds)
					{
						loading_sounds = true;
						toLog("  - Loading sounds...");
					}
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
						FMOD::Sound* temp;
						FMOD_RESULT result;
						if(parameter[1] == "LOOP")
							result = sound->createSound(soundAccessTable[current_line]->name.c_str(), FMOD_LOOP_NORMAL | FMOD_SOFTWARE, 0, &temp);
						else 
							result = sound->createSound(soundAccessTable[current_line]->name.c_str(), FMOD_SOFTWARE, 0, &temp);
						if(!ERRCHECK(result))
						{
							toLog("Could not load " + soundAccessTable[current_line]->name);
							return;
						}
						soundAccessTable[current_line]->sound = temp;
						soundList[current_line] = temp;
					}
					soundAccessTable[current_line]->used = true;
					++current_line;
				}
				else if(mode == GENERAL_BITMAP_DATA_TYPE)
				{
					if(!loading_bitmaps)
					{
						loading_bitmaps = true;
						toLog("  - Loading bitmaps...");
					}					
		                        std::map<std::string, std::list<std::string> > block;
		                        pFile.seekg(old_pos);
		                        if(!parse_block_map(pFile, block))
		                        {
		                                toLog("WARNING (UI_Theme::loadDataFile()): No concluding @END for @GENERAL_BITMAP_DATA_TYPE block was found in file " + data_file + " => trying to parse what we have so far.");
		                        }
		                        for(unsigned int j = 0; j < MAX_BITMAPS; j++)
		                        {
						std::map<std::string, std::list<std::string> >::iterator i;
                       			        if((i = block.find(bitmapIdentifier[j])) != block.end())
		                                {
		                                        i->second.pop_front(); // Identifier loeschen
							std::string name = i->second.front();
							if((name.size()<4)||(name[name.size()-4]!='.'))
								name = bitmap_dir + name + ".bmp";
							else name = bitmap_dir + name;
							bool found_bitmap = false;
							for(std::list<BitmapEntry>::iterator l = loadedBitmaps.begin(); l!=loadedBitmaps.end(); ++l)
						// already loaded?
								if(l->name == name)
								{
									found_bitmap = true;
									for(unsigned int m = MAX_RESOLUTIONS; m--;)
										for(unsigned int n = MAX_COLOR_THEMES; n--;)
											bitmapAccessTable[m][n][j] = &(*l);
									break;
								}
							if(!found_bitmap)
							{
								BitmapEntry entry;
//								entry.resolution = current_resolution; //?
//								entry.theme = current_theme;
								entry.line = j;
								entry.name = name;
								entry.bitmap = NULL;//temp;
								entry.used = false;
								i->second.pop_front();
								entry.solid = ((i->second.size()>0)&&(i->second.front() == "(SOLID)"));
								loadedBitmaps.push_back(entry);
								for(unsigned int m = MAX_RESOLUTIONS; m--;)
									for(unsigned int n = MAX_COLOR_THEMES; n--;)
										bitmapAccessTable[m][n][j] = &(loadedBitmaps.back());
							}
		                                        block.erase(i);
		                                }
                       			}
					mode = ZERO_DATA_TYPE;
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
						if(!loading_bitmaps)
						{
							loading_bitmaps = true;
							toLog("  - Loading bitmaps...");
						}					
			                        std::map<std::string, std::list<std::string> > block;
			                        pFile.seekg(old_pos);
			                        if(!parse_block_map(pFile, block))
			                        {
			                                toLog("WARNING (UI_Theme::loadDataFile()): No concluding @END for @BITMAP_DATA_TYPE block was found in file " + data_file + " => trying to parse what we have so far.");
			                        }
			                        for(unsigned int j = 0; j < MAX_BITMAPS; j++)
			                        {
							std::map<std::string, std::list<std::string> >::iterator i;
                        			        if((i = block.find(bitmapIdentifier[j])) != block.end())
			                                {
			                                        i->second.pop_front(); // Identifier loeschen
								std::string name = i->second.front();
								if((name.size()<4)||(name[name.size()-4]!='.'))
									name = bitmap_dir + name + ".bmp";
								else name = bitmap_dir + name;
								bool found_bitmap = false;
								for(std::list<BitmapEntry>::iterator l = loadedBitmaps.begin(); l!=loadedBitmaps.end(); ++l)
							// already loaded?
									if(l->name == name)
									{
										found_bitmap = true;
										bitmapAccessTable[current_resolution][current_theme][j] = &(*l);
										break;
									}
								if(!found_bitmap)
								{
									BitmapEntry entry;
//									entry.resolution = current_resolution; //?
//									entry.theme = current_theme;
									entry.line = j;
									entry.name = name;
									entry.bitmap = NULL;//temp;
									entry.used = false;
									i->second.pop_front();
									entry.solid = ((i->second.size()>0)&&(i->second.front() == "(SOLID)"));
									loadedBitmaps.push_back(entry);
									bitmapAccessTable[current_resolution][current_theme][j] = &(loadedBitmaps.back());
								}
			                                        block.erase(i);
			                                }
                        			}
                        // TODO nicht gefundene Eintraege bemaengeln
						current_theme = ZERO_THEME;
					}break;
					default:break;
				}
				++current_line;
			}
		} // end if mode != ZERO_DATA_TYPE
		old_pos = pFile.tellg();
	} // end while

	cursorList[ARROW_CURSOR][0] = DC::createCursor(arrow_xpm);cursorList[ARROW_CURSOR][1] = NULL;
	cursorList[CLOCK_CURSOR][0] = DC::createCursor(clock1_xpm);cursorList[CLOCK_CURSOR][1] = DC::createCursor(clock2_xpm);
	cursorList[HAND_CURSOR][0] = DC::createCursor(hand_xpm);cursorList[HAND_CURSOR][1] = NULL;
	defaultCursor = SDL_GetCursor();
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
		
		SDL_Surface* temp = NULL;
		
		if(bitmapAccessTable[resolution][colorTheme][id]==NULL)
		{
			toLog("Bitmap " + bitmapIdentifier[id] + " was not initialized for resolution '" + lookUpString((eString)(SETTING_RESOLUTION_ZERO_STRING + resolution)) + "' and theme '" + lookUpString((eString)(SETTING_ZERO_THEME_STRING + colorTheme)) + "'. Check 'settings/ui/default.ui' and 'data/bitmaps'.");
			return(NULL);
		} else if((temp=IMG_Load(bitmapAccessTable[resolution][colorTheme][id]->name.c_str()))==NULL)
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

FMOD::Sound* UI_Theme::lookUpSound(const eSound id)
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

void UI_Theme::playSound(const eSound id, const unsigned int x)
{
	soundsToPlay.push_back(std::pair<FMOD::Sound*, float>(lookUpSound(id), 2*((float)(2*x) - (float)getResolutionSize().GetWidth())/(float)(3*getResolutionSize().GetWidth())));
}

const bool UI_Theme::setLanguage(const eLanguage theme_language) {
	if(languageInitialized[theme_language])
	{
		language = theme_language;
		return(true);
	}
	else
	{
		toLog("ERROR (UI_Theme::setLanguage()): Cannot set language " + lookUpString((eString)(SETTING_ZERO_LANGUAGE_STRING + theme_language)) + ", language was not initialized.");
		return(false);
	}
}

void UI_Theme::printSoundInformation() const
{
	int driver_num;
	sound->getNumDrivers(&driver_num);
	std::ostringstream os; os << "* Availible sound drivers: ";
	for(unsigned int i = driver_num; i--;)
	{
		char driver_name[128];
		sound->getDriverName(i, driver_name, 128);
		os << driver_name << " ";
	}
	toLog(os.str());
	os.str("");
	int current_driver;
	sound->getDriver(&current_driver);

	os << "* Driver used: ";
	if(current_driver == -1)
	{
		os << "Primary or main sound device as selected by the operating system settings";
		if(driver_num == 1)
		{
			char driver_name[128];
			sound->getDriverName(current_driver, driver_name, 128);
			os << "(probably '" << driver_name << "')";
		}
		toLog(os.str());
	}
	else
	{
		char driver_name[128];
		sound->getDriverName(current_driver, driver_name, 128);
		os << driver_name;
		toLog(os.str());
	}
}


void UI_Theme::initBitmapIdentifier()
{
	for(unsigned int i = MAX_BITMAPS; i--;)
		bitmapIdentifier[i] = "null";
	bitmapIdentifier[NULL_BITMAP] = "NULL_BITMAP";
	bitmapIdentifier[INCREASE_BITMAP] = "INCREASE_BITMAP";
	bitmapIdentifier[SUB_BITMAP] = "SUB_BITMAP";
	bitmapIdentifier[CANCEL_BITMAP] = "CANCEL_BITMAP";
	bitmapIdentifier[SMALL_ARROW_LEFT_BITMAP] = "SMALL_ARROW_LEFT_BITMAP";
	bitmapIdentifier[SMALL_ARROW_RIGHT_BITMAP] = "SMALL_ARROW_RIGHT_BITMAP";
	bitmapIdentifier[SMALL_ARROW_UP_BITMAP] = "SMALL_ARROW_UP_BITMAP";
	bitmapIdentifier[SMALL_ARROW_DOWN_BITMAP] = "SMALL_ARROW_DOWN_BITMAP";
	bitmapIdentifier[LOCATION_BUTTON_BITMAP] = "LOCATION_BUTTON_BITMAP";
	bitmapIdentifier[TIME_BUTTON_BITMAP] = "TIME_BUTTON_BITMAP";
	bitmapIdentifier[MOUSE_NONE] = "MOUSE_NONE";
	bitmapIdentifier[MOUSE_LEFT] = "MOUSE_LEFT";
	bitmapIdentifier[MOUSE_RIGHT] = "MOUSE_RIGHT";
	bitmapIdentifier[MOUSE_BOTH] = "MOUSE_BOTH";
	bitmapIdentifier[RADIO_OFF] = "RADIO_OFF";
	bitmapIdentifier[RADIO_ON] = "RADIO_ON";
	bitmapIdentifier[BACKGROUND_SC_BITMAP] = "BACKGROUND_SC_BITMAP";
	bitmapIdentifier[BAR_BITMAP] = "BAR_BITMAP";
	bitmapIdentifier[KEY_BITMAP] = "KEY_BITMAP";
	bitmapIdentifier[CLAWSOFTWARE_BITMAP] = "CLAWSOFTWARE_BITMAP";
	bitmapIdentifier[CLAWSOFTWARE_MONO_BITMAP] = "CLAWSOFTWARE_MONO_BITMAP";
	bitmapIdentifier[NEW_BITMAP] = "NEW_BITMAP";
	bitmapIdentifier[LOAD_BITMAP] = "LOAD_BITMAP";
	bitmapIdentifier[SAVE_BITMAP] = "SAVE_BITMAP";
	bitmapIdentifier[REFRESH_BITMAP] = "REFRESH_BITMAP";
	bitmapIdentifier[BACK_BITMAP] = "BACK_BITMAP";
	bitmapIdentifier[FORWARD_BITMAP] = "FORWARD_BITMAP";
	bitmapIdentifier[ADD_BITMAP] = "ADD_BITMAP";
	bitmapIdentifier[DELETE_BITMAP] = "DELETE_BITMAP";
	bitmapIdentifier[HELP_BITMAP] = "HELP_BITMAP";
	bitmapIdentifier[MAP_BITMAP] = "MAP_BITMAP";
	bitmapIdentifier[SETTING_BITMAP] = "SETTING_BITMAP";
	bitmapIdentifier[BACKGROUND_WH40K_BITMAP] = "BACKGROUND_WH40K_BITMAP";
	bitmapIdentifier[BULLET_BITMAP] = "BULLET_BITMAP";
	bitmapIdentifier[SCORE_ACTIVE_BITMAP] = "SCORE_ACTIVE_BITMAP";
	bitmapIdentifier[SCORE_EMPTY_BITMAP] = "SCORE_EMPTY_BITMAP";
	bitmapIdentifier[FORCE_WINDOW_BITMAP] = "FORCE_WINDOW_BITMAP";
	bitmapIdentifier[BODIAGRAM_WINDOW_BITMAP] = "BODIAGRAM_WINDOW_BITMAP";
	bitmapIdentifier[BOGRAPH_WINDOW_BITMAP] = "BOGRAPH_WINDOW_BITMAP";
	bitmapIdentifier[BO_WINDOW_BITMAP] = "BO_WINDOW_BITMAP";
	bitmapIdentifier[CLEMENS_BITMAP] = "CLEMENS_BITMAP";
}

void UI_Theme::initStringIdentifier()
{
	for(unsigned int i = MAX_STRINGS; i--;)
		stringIdentifier[i] = "NULL_STRING";
	stringIdentifier[NONE] = "NONE";
	stringIdentifier[SCV] = "SCV";
	stringIdentifier[MARINE] = "MARINE";
	stringIdentifier[GHOST] = "GHOST";
	stringIdentifier[VULTURE] = "VULTURE";
	stringIdentifier[GOLIATH] = "GOLIATH";
	stringIdentifier[SIEGE_TANK] = "SIEGE_TANK";
	stringIdentifier[FIREBAT] = "FIREBAT";
	stringIdentifier[MEDIC] = "MEDIC";
	stringIdentifier[WRAITH] = "WRAITH";
	stringIdentifier[SCIENCE_VESSEL] = "SCIENCE_VESSEL";
	stringIdentifier[DROPSHIP] = "DROPSHIP";
	stringIdentifier[BATTLE_CRUISER] = "BATTLE_CRUISER";
	stringIdentifier[VALKYRIE] = "VALKYRIE";
	stringIdentifier[NUCLEAR_WARHEAD] = "NUCLEAR_WARHEAD";
	stringIdentifier[SUPPLY_DEPOT] = "SUPPLY_DEPOT";
	stringIdentifier[BARRACKS] = "BARRACKS";
	stringIdentifier[ACADEMY] = "ACADEMY";
	stringIdentifier[FACTORY] = "FACTORY";
	stringIdentifier[COMMAND_CENTER] = "COMMAND_CENTER";
	stringIdentifier[STARPORT] = "STARPORT";
	stringIdentifier[SCIENCE_FACILITY] = "SCIENCE_FACILITY";
	stringIdentifier[ENGINEERING_BAY] = "ENGINEERING_BAY";
	stringIdentifier[ARMORY] = "ARMORY";
	stringIdentifier[MISSILE_TURRET] = "MISSILE_TURRET";
	stringIdentifier[BUNKER] = "BUNKER";
	stringIdentifier[COMSAT_STATION] = "COMSAT_STATION";
	stringIdentifier[NUCLEAR_SILO] = "NUCLEAR_SILO";
	stringIdentifier[CONTROL_TOWER] = "CONTROL_TOWER";
	stringIdentifier[COVERT_OPS] = "COVERT_OPS";
	stringIdentifier[PHYSICS_LAB] = "PHYSICS_LAB";
	stringIdentifier[MACHINE_SHOP] = "MACHINE_SHOP";
	stringIdentifier[COMMAND_CENTER_CS] = "COMMAND_CENTER_CS";
	stringIdentifier[COMMAND_CENTER_NS] = "COMMAND_CENTER_NS";
	stringIdentifier[STARPORT_CT] = "STARPORT_CT";
	stringIdentifier[SCIENCE_FACILITY_CO] = "SCIENCE_FACILITY_CO";
	stringIdentifier[SCIENCE_FACILITY_PL] = "SCIENCE_FACILITY_PL";
	stringIdentifier[FACTORY_MS] = "FACTORY_MS";
	stringIdentifier[STIM_PACKS] = "STIM_PACKS";
	stringIdentifier[LOCKDOWN] = "LOCKDOWN";
	stringIdentifier[EMP_SHOCKWAVE] = "EMP_SHOCKWAVE";
	stringIdentifier[SPIDER_MINES] = "SPIDER_MINES";
	stringIdentifier[TANK_SIEGE_MODE] = "TANK_SIEGE_MODE";
	stringIdentifier[IRRADIATE] = "IRRADIATE";
	stringIdentifier[YAMATO_GUN] = "YAMATO_GUN";
	stringIdentifier[CLOAKING_FIELD] = "CLOAKING_FIELD";
	stringIdentifier[PERSONNEL_CLOAKING] = "PERSONNEL_CLOAKING";
	stringIdentifier[RESTORATION] = "RESTORATION";
	stringIdentifier[OPTICAL_FLARE] = "OPTICAL_FLARE";
	stringIdentifier[U238_SHELLS] = "U238_SHELLS";
	stringIdentifier[ION_THRUSTERS] = "ION_THRUSTERS";
	stringIdentifier[TITAN_REACTOR] = "TITAN_REACTOR";
	stringIdentifier[OCULAR_IMPLANTS] = "OCULAR_IMPLANTS";
	stringIdentifier[MOEBIUS_REACTOR] = "MOEBIUS_REACTOR";
	stringIdentifier[APOLLO_REACTOR] = "APOLLO_REACTOR";
	stringIdentifier[COLOSSUS_REACTOR] = "COLOSSUS_REACTOR";
	stringIdentifier[CADUCEUS_REACTOR] = "CADUCEUS_REACTOR";
	stringIdentifier[CHARON_BOOSTER] = "CHARON_BOOSTER";
	stringIdentifier[INFANTRY_ARMOR] = "INFANTRY_ARMOR";
	stringIdentifier[INFANTRY_WEAPONS] = "INFANTRY_WEAPONS";
	stringIdentifier[VEHICLE_PLATING] = "VEHICLE_PLATING";
	stringIdentifier[VEHICLE_WEAPONS] = "VEHICLE_WEAPONS";
	stringIdentifier[SHIP_PLATING] = "SHIP_PLATING";
	stringIdentifier[SHIP_WEAPONS] = "SHIP_WEAPONS";
	stringIdentifier[REFINERY] = "REFINERY";
	stringIdentifier[GAS_SCV] = "GAS_SCV";
	stringIdentifier[BUILD_PARALLEL_2] = "BUILD_PARALLEL_2";
	stringIdentifier[BUILD_PARALLEL_4] = "BUILD_PARALLEL_4";
	stringIdentifier[BUILD_PARALLEL_8] = "BUILD_PARALLEL_8";
	stringIdentifier[BUILD_PARALLEL_16] = "BUILD_PARALLEL_16";
	stringIdentifier[FROM_GAS_TO_MINERALS] = "FROM_GAS_TO_MINERALS";
	stringIdentifier[LAST_UNIT] = "LAST_UNIT";
	stringIdentifier[VESPENE_GEYSIR] = "VESPENE_GEYSIR";
	stringIdentifier[MINERAL_PATCH] = "MINERAL_PATCH";
	stringIdentifier[R_STIM_PACKS] = "R_STIM_PACKS";
	stringIdentifier[R_LOCKDOWN] = "R_LOCKDOWN";
	stringIdentifier[R_EMP_SHOCKWAVE] = "R_EMP_SHOCKWAVE";
	stringIdentifier[R_SPIDER_MINES] = "R_SPIDER_MINES";
	stringIdentifier[R_TANK_SIEGE_MODE] = "R_TANK_SIEGE_MODE";
	stringIdentifier[R_IRRADIATE] = "R_IRRADIATE";
	stringIdentifier[R_YAMATO_GUN] = "R_YAMATO_GUN";
	stringIdentifier[R_CLOAKING_FIELD] = "R_CLOAKING_FIELD";
	stringIdentifier[R_PERSONNEL_CLOAKING] = "R_PERSONNEL_CLOAKING";
	stringIdentifier[R_RESTORATION] = "R_RESTORATION";
	stringIdentifier[R_OPTICAL_FLARE] = "R_OPTICAL_FLARE";
	stringIdentifier[R_U238_SHELLS] = "R_U238_SHELLS";
	stringIdentifier[R_ION_THRUSTERS] = "R_ION_THRUSTERS";
	stringIdentifier[R_TITAN_REACTOR] = "R_TITAN_REACTOR";
	stringIdentifier[R_OCULAR_IMPLANTS] = "R_OCULAR_IMPLANTS";
	stringIdentifier[R_MOEBIUS_REACTOR] = "R_MOEBIUS_REACTOR";
	stringIdentifier[R_APOLLO_REACTOR] = "R_APOLLO_REACTOR";
	stringIdentifier[R_COLOSSUS_REACTOR] = "R_COLOSSUS_REACTOR";
	stringIdentifier[R_CADUCEUS_REACTOR] = "R_CADUCEUS_REACTOR";
	stringIdentifier[R_CHARON_BOOSTER] = "R_CHARON_BOOSTER";
	stringIdentifier[R_INFANTRY_ARMOR] = "R_INFANTRY_ARMOR";
	stringIdentifier[R_INFANTRY_WEAPONS] = "R_INFANTRY_WEAPONS";
	stringIdentifier[R_VEHICLE_PLATING] = "R_VEHICLE_PLATING";
	stringIdentifier[R_VEHICLE_WEAPONS] = "R_VEHICLE_WEAPONS";
	stringIdentifier[R_SHIP_PLATING] = "R_SHIP_PLATING";
	stringIdentifier[R_SHIP_WEAPONS] = "R_SHIP_WEAPONS";
	stringIdentifier[F_FACTORY_ADDON] = "F_FACTORY_ADDON";
	stringIdentifier[F_STARPORT_ADDON] = "F_STARPORT_ADDON";
	stringIdentifier[F_COMMAND_CENTER_ADDON] = "F_COMMAND_CENTER_ADDON";
	stringIdentifier[F_SCIENCE_FACILITY_ADDON] = "F_SCIENCE_FACILITY_ADDON";
	stringIdentifier[INTRON] = "INTRON";

	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+PROBE] = "PROBE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+DARK_TEMPLAR] = "DARK_TEMPLAR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+DARK_ARCHON] = "DARK_ARCHON";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ZEALOT] = "ZEALOT";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+DRAGOON] = "DRAGOON";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+HIGH_TEMPLAR] = "HIGH_TEMPLAR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ARCHON] = "ARCHON";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+REAVER] = "REAVER";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+HALF_SCARAB] = "HALF_SCARAB";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+FULL_SCARAB] = "FULL_SCARAB";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+CORSAIR] = "CORSAIR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+SHUTTLE] = "SHUTTLE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+SCOUT] = "SCOUT";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ARBITER] = "ARBITER";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+CARRIER] = "CARRIER";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+HALF_INTERCEPTOR] = "HALF_INTERCEPTOR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+FULL_INTERCEPTOR] = "FULL_INTERCEPTOR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+OBSERVER] = "OBSERVER";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+NEXUS] = "NEXUS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ROBOTICS_FACILITY] = "ROBOTICS_FACILITY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+PYLON] = "PYLON";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+OBSERVATORY] = "OBSERVATORY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+GATEWAY] = "GATEWAY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+PHOTON_CANNON] = "PHOTON_CANNON";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+CYBERNETICS_CORE] = "CYBERNETICS_CORE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+CITADEL_OF_ADUN] = "CITADEL_OF_ADUN";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+TEMPLAR_ARCHIVES] = "TEMPLAR_ARCHIVES";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+FORGE] = "FORGE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+STARGATE] = "STARGATE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+FLEET_BEACON] = "FLEET_BEACON";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ARBITER_TRIBUNAL] = "ARBITER_TRIBUNAL";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ROBOTICS_SUPPORT_BAY] = "ROBOTICS_SUPPORT_BAY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+SHIELD_BATTERY] = "SHIELD_BATTERY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+PSIONIC_STORM] = "PSIONIC_STORM";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+HALLUCINATION] = "HALLUCINATION";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+RECALL] = "RECALL";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+STASIS_FIELD] = "STASIS_FIELD";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+DISRUPTION_WEB] = "DISRUPTION_WEB";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+MIND_CONTROL] = "MIND_CONTROL";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+MAELSTROM] = "MAELSTROM";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+SINGULARITY_CHARGE] = "SINGULARITY_CHARGE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+LEG_ENHANCEMENTS] = "LEG_ENHANCEMENTS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+SCARAB_DAMAGE] = "SCARAB_DAMAGE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+REAVER_CAPACITY] = "REAVER_CAPACITY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+GRAVITIC_DRIVE] = "GRAVITIC_DRIVE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+SENSOR_ARRAY] = "SENSOR_ARRAY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+GRAVITIC_BOOSTERS] = "GRAVITIC_BOOSTERS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+KHAYDARIN_AMULET] = "KHAYDARIN_AMULET";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+APIAL_SENSORS] = "APIAL_SENSORS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+GRAVITIC_THRUSTERS] = "GRAVITIC_THRUSTERS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+CARRIER_CAPACITY] = "CARRIER_CAPACITY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+KHAYDARIN_CORE] = "KHAYDARIN_CORE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ARGUS_JEWEL] = "ARGUS_JEWEL";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ARGUS_TALISMAN] = "ARGUS_TALISMAN";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ARMOR] = "ARMOR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+PLATING] = "PLATING";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+GROUND_WEAPONS] = "GROUND_WEAPONS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+AIR_WEAPONS] = "AIR_WEAPONS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+PLASMA_SHIELDS] = "PLASMA_SHIELDS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+ASSIMILATOR] = "ASSIMILATOR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+GAS_PROBE] = "GAS_PROBE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_PSIONIC_STORM] = "R_PSIONIC_STORM";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_HALLUCINATION] = "R_HALLUCINATION";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_RECALL] = "R_RECALL";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_STASIS_FIELD] = "R_STASIS_FIELD";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_DISRUPTION_WEB] = "R_DISRUPTION_WEB";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_MIND_CONTROL] = "R_MIND_CONTROL";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_MAELSTROM] = "R_MAELSTROM";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_SINGULARITY_CHARGE] = "R_SINGULARITY_CHARGE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_LEG_ENHANCEMENTS] = "R_LEG_ENHANCEMENTS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_SCARAB_DAMAGE] = "R_SCARAB_DAMAGE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_REAVER_CAPACITY] = "R_REAVER_CAPACITY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_GRAVITIC_DRIVE] = "R_GRAVITIC_DRIVE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_SENSOR_ARRAY] = "R_SENSOR_ARRAY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_GRAVITIC_BOOSTERS] = "R_GRAVITIC_BOOSTERS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_KHAYDARIN_AMULET] = "R_KHAYDARIN_AMULET";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_APIAL_SENSORS] = "R_APIAL_SENSORS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_GRAVITIC_THRUSTERS] = "R_GRAVITIC_THRUSTERS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_CARRIER_CAPACITY] = "R_CARRIER_CAPACITY";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_KHAYDARIN_CORE] = "R_KHAYDARIN_CORE";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_ARGUS_JEWEL] = "R_ARGUS_JEWEL";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_ARGUS_TALISMAN] = "R_ARGUS_TALISMAN";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_ARMOR] = "R_ARMOR";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_PLATING] = "R_PLATING";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_GROUND_WEAPONS] = "R_GROUND_WEAPONS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_AIR_WEAPONS] = "R_AIR_WEAPONS";
	stringIdentifier[UNIT_TYPE_COUNT*PROTOSS+R_PLASMA_SHIELDS] = "R_PLASMA_SHIELDS";

	stringIdentifier[UNIT_TYPE_COUNT*ZERG+DRONE] = "DRONE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+LARVA] = "LARVA";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+ZERGLING] = "ZERGLING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+HYDRALISK] = "HYDRALISK";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+ULTRALISK] = "ULTRALISK";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+DEFILER] = "DEFILER";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+LURKER] = "LURKER";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+OVERLORD] = "OVERLORD";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+MUTALISK] = "MUTALISK";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+GUARDIEN] = "GUARDIEN";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+QUEEN] = "QUEEN";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+SCOURGE] = "SCOURGE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+DEVOURER] = "DEVOURER";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+LAIR] = "LAIR";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+HIVE] = "HIVE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+NYDUS_CANAL] = "NYDUS_CANAL";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+HYDRALISK_DEN] = "HYDRALISK_DEN";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+DEFILER_MOUND] = "DEFILER_MOUND";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+HATCHERY] = "HATCHERY";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+GREATER_SPIRE] = "GREATER_SPIRE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+QUEENS_NEST] = "QUEENS_NEST";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+EVOLUTION_CHAMBER] = "EVOLUTION_CHAMBER";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+ULTRALISK_CAVERN] = "ULTRALISK_CAVERN";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+SPIRE] = "SPIRE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+SPAWNING_POOL] = "SPAWNING_POOL";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+CREEP_COLONY] = "CREEP_COLONY";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+SPORE_COLONY] = "SPORE_COLONY";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+SUNKEN_COLONY] = "SUNKEN_COLONY";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+VENTRAL_SACKS] = "VENTRAL_SACKS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+ANTENNAE] = "ANTENNAE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+PNEUMATIZED_CARAPACE] = "PNEUMATIZED_CARAPACE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+METABOLIC_BOOST] = "METABOLIC_BOOST";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+ADRENAL_GLANDS] = "ADRENAL_GLANDS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+MUSCULAR_AUGMENTS] = "MUSCULAR_AUGMENTS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+GROOVED_SPINES] = "GROOVED_SPINES";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+GAMETE_MEIOSIS] = "GAMETE_MEIOSIS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+METASYNAPTIC_NODE] = "METASYNAPTIC_NODE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+CHITINOUS_PLATING] = "CHITINOUS_PLATING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+ANABOLIC_SYNTHESIS] = "ANABOLIC_SYNTHESIS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+BURROWING] = "BURROWING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+SPAWN_BROODLING] = "SPAWN_BROODLING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+PLAGUE] = "PLAGUE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+CONSUME] = "CONSUME";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+ENSNARE] = "ENSNARE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+LURKER_ASPECT] = "LURKER_ASPECT";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+CARAPACE] = "CARAPACE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+FLYER_CARAPACE] = "FLYER_CARAPACE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+MELEE_ATTACKS] = "MELEE_ATTACKS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+MISSILE_ATTACKS] = "MISSILE_ATTACKS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+FLYER_ATTACKS] = "FLYER_ATTACKS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+EXTRACTOR] = "EXTRACTOR";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+GAS_DRONE] = "GAS_DRONE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+BREAK_UP_BUILDING] = "BREAK_UP_BUILDING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_VENTRAL_SACKS] = "R_VENTRAL_SACKS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_ANTENNAE] = "R_ANTENNAE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_PNEUMATIZED_CARAPACE] = "R_PNEUMATIZED_CARAPACE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_METABOLIC_BOOST] = "R_METABOLIC_BOOST";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_ADRENAL_GLANDS] = "R_ADRENAL_GLANDS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_MUSCULAR_AUGMENTS] = "R_MUSCULAR_AUGMENTS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_GROOVED_SPINES] = "R_GROOVED_SPINES";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_GAMETE_MEIOSIS] = "R_GAMETE_MEIOSIS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_METASYNAPTIC_NODE] = "R_METASYNAPTIC_NODE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_CHITINOUS_PLATING] = "R_CHITINOUS_PLATING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_ANABOLIC_SYNTHESIS] = "R_ANABOLIC_SYNTHESIS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_BURROWING] = "R_BURROWING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_SPAWN_BROODLING] = "R_SPAWN_BROODLING";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_PLAGUE] = "R_PLAGUE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_CONSUME] = "R_CONSUME";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_ENSNARE] = "R_ENSNARE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_LURKER_ASPECT] = "R_LURKER_ASPECT";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_CARAPACE] = "R_CARAPACE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_FLYER_CARAPACE] = "R_FLYER_CARAPACE";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_MELEE_ATTACKS] = "R_MELEE_ATTACKS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_MISSILE_ATTACKS] = "R_MISSILE_ATTACKS";
	stringIdentifier[UNIT_TYPE_COUNT*ZERG+R_FLYER_ATTACKS] = "R_FLYER_ATTACKS";

	stringIdentifier[START_LOAD_CONFIGURATION_STRING] = "START_LOAD_CONFIGURATION_STRING";
	stringIdentifier[START_PARSE_COMMAND_LINE_STRING] = "START_PARSE_COMMAND_LINE_STRING";
	stringIdentifier[START_WARNING_VO_ARGUMENT_STRING] = "START_WARNING_VO_ARGUMENT_STRING";
	stringIdentifier[START_SDL_USING_DRIVER_STRING] = "START_SDL_USING_DRIVER_STRING";
	stringIdentifier[START_ERROR_NO_DRIVER_AVAILIBLE_STRING] = "START_ERROR_NO_DRIVER_AVAILIBLE_STRING";
	stringIdentifier[START_ERROR_DRIVER_NOT_SUPPORTED_STRING] = "START_ERROR_DRIVER_NOT_SUPPORTED_STRING";
	stringIdentifier[START_INIT_SDL_STRING] = "START_INIT_SDL_STRING";
	stringIdentifier[START_UNABLE_TO_INIT_SDL_STRING] = "START_UNABLE_TO_INIT_SDL_STRING";
	stringIdentifier[START_CREATED_SURFACE_STRING] = "START_CREATED_SURFACE_STRING";
	stringIdentifier[START_ERROR_SETTING_VIDEO_MODE_STRING] = "START_ERROR_SETTING_VIDEO_MODE_STRING";
	stringIdentifier[START_SET_WINDOW_MODE_STRING] = "START_SET_WINDOW_MODE_STRING";
	stringIdentifier[START_SET_FULLSCREEN_MODE_STRING] = "START_SET_FULLSCREEN_MODE_STRING";
	stringIdentifier[START_INIT_SDL_TRUETYPE_FONTS_STRING] = "START_INIT_SDL_TRUETYPE_FONTS_STRING";
	stringIdentifier[START_INIT_FRAMERATE_STRING] = "START_INIT_FRAMERATE_STRING";
	stringIdentifier[START_INIT_SOUND_STRING] = "START_INIT_SOUND_STRING";
	stringIdentifier[START_INIT_GRAPHIC_ENGINE_CORE_STRING] = "START_INIT_GRAPHIC_ENGINE_CORE_STRING";
	stringIdentifier[START_LOAD_UI_BITMAPS_FONTS_STRING] = "START_LOAD_UI_BITMAPS_FONTS_STRING";
	stringIdentifier[START_ASSIGNING_DEFAULT_VARIABLES_STRING] = "START_ASSIGNING_DEFAULT_VARIABLES_STRING";
	stringIdentifier[START_READING_PARSING_STRING] = "START_READING_PARSING_STRING";
	stringIdentifier[START_LOAD_HARVEST_STRING] = "START_LOAD_HARVEST_STRING";
	stringIdentifier[START_LOAD_MAPS_STRING] = "START_LOAD_MAPS_STRING";
	stringIdentifier[START_LOAD_STARTCONDITIONS_STRING] = "START_LOAD_STARTCONDITIONS_STRING";
	stringIdentifier[START_LOAD_GOALS_STRING] = "START_LOAD_GOALS_STRING";
	stringIdentifier[START_LOAD_BUILD_ORDERS_STRING] = "START_LOAD_BUILD_ORDERS_STRING";
	stringIdentifier[START_ASSIGN_AND_ANALYZE_STRING] = "START_ASSIGN_AND_ANALYZE_STRING";
	stringIdentifier[START_PREPARE_FIRST_RUN_STRING] = "START_PREPARE_FIRST_RUN_STRING";
	stringIdentifier[START_INIT_GUI_STRING] = "START_INIT_GUI_STRING";
	stringIdentifier[START_INIT_MAIN_WINDOW_STRING] = "START_INIT_MAIN_WINDOW_STRING";
	stringIdentifier[START_INIT_HELP_WINDOW_STRING] = "START_INIT_HELP_WINDOW_STRING";
	stringIdentifier[START_INIT_SETTINGS_WINDOW_STRING] = "START_INIT_SETTINGS_WINDOW_STRING";
	stringIdentifier[START_INIT_DATABASE_WINDOW_STRING] = "START_INIT_DATABASE_WINDOW_STRING";
	stringIdentifier[START_INIT_MAP_WINDOW_STRING] = "START_INIT_MAP_WINDOW_STRING";
	stringIdentifier[START_INIT_MSG_WINDOW_STRING] = "START_INIT_MSG_WINDOW_STRING";
	stringIdentifier[START_INIT_TECHTREE_WINDOW_STRING] = "START_INIT_TECHTREE_WINDOW_STRING";
	stringIdentifier[START_INIT_INTRO_WINDOW_STRING] = "START_INIT_INTRO_WINDOW_STRING";
	stringIdentifier[START_HIDING_WINDOWS_STRING] = "START_HIDING_WINDOWS_STRING";

	stringIdentifier[START_INIT_CORE_STRING] = "START_INIT_CORE_STRING";
	stringIdentifier[START_MAIN_INIT_COMPLETE_STRING] = "START_MAIN_INIT_COMPLETE_STRING";
	stringIdentifier[START_SYSTEM_READY_STRING] = "START_SYSTEM_READY_STRING";
	stringIdentifier[START_INITIALIZATION_TIME_STRING] = "START_INITIALIZATION_TIME_STRING";
	stringIdentifier[CHANGED_BIT_DEPTH_STRING] = "CHANGED_BIT_DEPTH_STRING";
	stringIdentifier[CHANGED_RESOLUTION_STRING] = "CHANGED_RESOLUTION_STRING";
	stringIdentifier[MAIN_WINDOW_TITLE_STRING] = "MAIN_WINDOW_TITLE_STRING";
	stringIdentifier[MESSAGE_WINDOW_TITLE_STRING] = "MESSAGE_WINDOW_TITLE_STRING";
	stringIdentifier[HELP_WINDOW_TITLE_STRING] = "HELP_WINDOW_TITLE_STRING";
	stringIdentifier[SETTINGS_WINDOW_TITLE_STRING] = "SETTINGS_WINDOW_TITLE_STRING";
	stringIdentifier[MAP_WINDOW_TITLE_STRING] = "MAP_WINDOW_TITLE_STRING";
	stringIdentifier[DATABASE_WINDOW_TITLE_STRING] = "DATABASE_WINDOW_TITLE_STRING";
	stringIdentifier[INFO_WINDOW_TITLE_STRING] = "INFO_WINDOW_TITLE_STRING";
	stringIdentifier[TECHTREE_WINDOW_TITLE_STRING] = "TECHTREE_WINDOW_TITLE_STRING";
	stringIdentifier[BOGRAPH_WINDOW_TITLE_STRING] = "BOGRAPH_WINDOW_TITLE_STRING";
	stringIdentifier[BODIAGRAM_WINDOW_TITLE_STRING] = "BODIAGRAM_WINDOW_TITLE_STRING";
	stringIdentifier[STATISTICS_WINDOW_TITLE_STRING] = "STATISTICS_WINDOW_TITLE_STRING";
	stringIdentifier[TIMER_WINDOW_TITLE_STRING] = "TIMER_WINDOW_TITLE_STRING";
	stringIdentifier[FORCE_WINDOW_TITLE_STRING] = "FORCE_WINDOW_TITLE_STRING";
	stringIdentifier[BOWINDOW_TITLE_STRING] = "BOWINDOW_TITLE_STRING";
	stringIdentifier[GAME_WINDOW_TITLE_STRING] = "GAME_WINDOW_TITLE_STRING";
	stringIdentifier[PLAYER_WINDOW_TITLE_STRING] = "PLAYER_WINDOW_TITLE_STRING";
	stringIdentifier[GAME_NUMBER_STRING] = "GAME_NUMBER_STRING";
	stringIdentifier[NEW_GAME_STRING] = "NEW_GAME_STRING";
	stringIdentifier[WELCOME_MSG1_STRING] = "WELCOME_MSG1_STRING";
	stringIdentifier[WELCOME_MSG2_STRING] = "WELCOME_MSG2_STRING";
	stringIdentifier[PLAYERS_LOADED_STRING] = "PLAYERS_LOADED_STRING";
	stringIdentifier[ADDED_GOAL_STRING] = "ADDED_GOAL_STRING";
	stringIdentifier[SET_NEW_GOAL_LIST_STRING] = "SET_NEW_GOAL_LIST_STRING";
	stringIdentifier[SET_NEW_MAP_STRING] = "SET_NEW_MAP_STRING";
	stringIdentifier[SET_RACE_STRING] = "SET_RACE_STRING";
	stringIdentifier[ADDED_ONE_GOAL_STRING] = "ADDED_ONE_GOAL_STRING";
	stringIdentifier[REMOVED_ONE_GOAL_STRING] = "REMOVED_ONE_GOAL_STRING";
	stringIdentifier[MOVED_NON_GOAL_STRING] = "MOVED_NON_GOAL_STRING";
	stringIdentifier[SAVED_GOAL_STRING] = "SAVED_GOAL_STRING";
	stringIdentifier[COMPARE_GAME_STRING] = "COMPARE_GAME_STRING";
	stringIdentifier[REMOVE_GAME_STRING] = "REMOVE_GAME_STRING";
	stringIdentifier[HELP_WINDOW_INDEX_STRING] = "HELP_WINDOW_INDEX_STRING";
	stringIdentifier[HELP_WINDOW_BACK_STRING] = "HELP_WINDOW_BACK_STRING";
	stringIdentifier[SAVE_BOX_OK_STRING] = "SAVE_BOX_OK_STRING";
	stringIdentifier[SAVE_BOX_CANCEL_STRING] = "SAVE_BOX_CANCEL_STRING";
	stringIdentifier[UNIT_TYPE_0_STRING] = "UNIT_TYPE_0_STRING";
	stringIdentifier[UNIT_TYPE_1_STRING] = "UNIT_TYPE_1_STRING";
	stringIdentifier[UNIT_TYPE_2_STRING] = "UNIT_TYPE_2_STRING";
	stringIdentifier[UNIT_TYPE_3_STRING] = "UNIT_TYPE_3_STRING";
	stringIdentifier[UNIT_TYPE_4_STRING] = "UNIT_TYPE_4_STRING";
	stringIdentifier[UNIT_TYPE_5_STRING] = "UNIT_TYPE_5_STRING";
	stringIdentifier[UNIT_TYPE_6_STRING] = "UNIT_TYPE_6_STRING";
	stringIdentifier[UNIT_TYPE_7_STRING] = "UNIT_TYPE_7_STRING";
	stringIdentifier[UNIT_TYPE_8_STRING] = "UNIT_TYPE_8_STRING";
	stringIdentifier[UNIT_TYPE_9_STRING] = "UNIT_TYPE_9_STRING";
	stringIdentifier[UNIT_TYPE_10_STRING] = "UNIT_TYPE_10_STRING";
	stringIdentifier[STARTING_FORCE_STRING] = "STARTING_FORCE_STRING";
	stringIdentifier[NON_GOALS_STRING] = "NON_GOALS_STRING";
	stringIdentifier[GOALS_STRING] = "GOALS_STRING";
	stringIdentifier[LEGEND_STRING] = "LEGEND_STRING";
	stringIdentifier[TIME_LEGEND_STRING] = "TIME_LEGEND_STRING";
	stringIdentifier[CLICK_TO_ADD_GOAL_STRING] = "CLICK_TO_ADD_GOAL_STRING";
	stringIdentifier[BACK_STRING] = "BACK_STRING";
	stringIdentifier[CLOSE_STRING] = "CLOSE_STRING";
	stringIdentifier[ADD_GOAL_STRING] = "ADD_GOAL_STRING";
	stringIdentifier[GOAL_LIST_STRING] = "GOAL_LIST_STRING";
	stringIdentifier[STARTFORCE_STRING] = "STARTFORCE_STRING";
	stringIdentifier[CHOOSE_RACE_STRING] = "CHOOSE_RACE_STRING";
	stringIdentifier[CHOOSE_MAP_STRING] = "CHOOSE_MAP_STRING";
	stringIdentifier[SAVE_GOAL_STRING] = "SAVE_GOAL_STRING";
	stringIdentifier[SAVE_GOALS_AS_STRING] = "SAVE_GOALS_AS_STRING";
	stringIdentifier[GIVE_GOAL_A_NAME_STRING] = "GIVE_GOAL_A_NAME_STRING";
	stringIdentifier[SAVE_BUILD_ORDER_AS_STRING] = "SAVE_BUILD_ORDER_AS_STRING";
	stringIdentifier[GIVE_BO_A_NAME_STRING] = "GIVE_BO_A_NAME_STRING";
	stringIdentifier[TERRA_STRING] = "TERRA_STRING";
	stringIdentifier[PROTOSS_STRING] = "PROTOSS_STRING";
	stringIdentifier[ZERG_STRING] = "ZERG_STRING";
	stringIdentifier[CLICK_TO_INSERT_ORDER_STRING] = "CLICK_TO_INSERT_ORDER_STRING";
	stringIdentifier[OPTIMIZE_EVERYTHING_STRING] = "OPTIMIZE_EVERYTHING_STRING";
	stringIdentifier[OPTIMIZE_SELECTED_STRING] = "OPTIMIZE_SELECTED_STRING";
	stringIdentifier[RESET_BUILD_ORDER_STRING] = "RESET_BUILD_ORDER_STRING";
	stringIdentifier[SAVE_BUILD_ORDER_STRING] = "SAVE_BUILD_ORDER_STRING";
	stringIdentifier[LOAD_BUILD_ORDER_STRING] = "LOAD_BUILD_ORDER_STRING";
	stringIdentifier[SPEED_STRING] = "SPEED_STRING";
	stringIdentifier[OF_GOALS_FULFILLED_STRING] = "OF_GOALS_FULFILLED_STRING";
	stringIdentifier[OF_TIME_FULFILLED_STRING] = "OF_TIME_FULFILLED_STRING";
	stringIdentifier[PAUSED_STRING] = "PAUSED_STRING";
	stringIdentifier[SEARCHING_STRING] = "SEARCHING_STRING";
	stringIdentifier[THEORETICAL_OPTIMUM_STRING] = "THEORETICAL_OPTIMUM_STRING";
	stringIdentifier[OPTIMIZING_STRING] = "OPTIMIZING_STRING";
	stringIdentifier[RES_UNITS_STRUCT_STRING] = "RES_UNITS_STRUCT_STRING";
	stringIdentifier[TOTAL_STRING] = "TOTAL_STRING";
	stringIdentifier[ADD_PLAYER_STRING] = "ADD_PLAYER_STRING";
	stringIdentifier[MINERALS_STAT_STRING] = "MINERALS_STAT_STRING";
	stringIdentifier[GAS_STAT_STRING] = "GAS_STAT_STRING";
	stringIdentifier[TIME_STAT_STRING] = "TIME_STAT_STRING";
	stringIdentifier[FORCE_STAT_STRING] = "FORCE_STAT_STRING";
	stringIdentifier[AVERAGE_BO_LENGTH_STAT_STRING] = "AVERAGE_BO_LENGTH_STAT_STRING";
	stringIdentifier[FITNESS_AVERAGE_STAT_STRING] = "FITNESS_AVERAGE_STAT_STRING";
	stringIdentifier[FITNESS_VARIANCE_STAT_STRING] = "FITNESS_VARIANCE_STAT_STRING";
	stringIdentifier[GENERATIONS_LEFT_STAT_STRING] = "GENERATIONS_LEFT_STAT_STRING";
	stringIdentifier[HELP_TAB_STRING] = "HELP_TAB_STRING";
	stringIdentifier[SETTINGS_TAB_STRING] = "SETTINGS_TAB_STRING";
	stringIdentifier[DATABASE_TAB_STRING] = "DATABASE_TAB_STRING";
	stringIdentifier[MAP_TAB_STRING] = "MAP_TAB_STRING";
	stringIdentifier[ADD_UNIT_TOOLTIP_STRING] = "ADD_UNIT_TOOLTIP_STRING";
	stringIdentifier[REMOVE_UNIT_TOOLTIP_STRING] = "REMOVE_UNIT_TOOLTIP_STRING";
	stringIdentifier[REMOVE_GOAL_TOOLTIP_STRING] = "REMOVE_GOAL_TOOLTIP_STRING";
	stringIdentifier[CHOOSE_RACE_TOOLTIP_STRING] = "CHOOSE_RACE_TOOLTIP_STRING";
	stringIdentifier[ADD_GOALS_TOOLTIP_STRING] = "ADD_GOALS_TOOLTIP_STRING";
	stringIdentifier[CHOOSE_GOALS_TOOLTIP_STRING] = "CHOOSE_GOALS_TOOLTIP_STRING";
	stringIdentifier[CHOOSE_STARTING_FORCE_TOOLTIP_STRING] = "CHOOSE_STARTING_FORCE_TOOLTIP_STRING";
	stringIdentifier[MINERALS_STAT_TOOLTIP_STRING] = "MINERALS_STAT_TOOLTIP_STRING";
	stringIdentifier[GAS_STAT_TOOLTIP_STRING] = "GAS_STAT_TOOLTIP_STRING";
	stringIdentifier[TIME_STAT_TOOLTIP_STRING] = "TIME_STAT_TOOLTIP_STRING";
	stringIdentifier[FORCE_STAT_TOOLTIP_STRING] = "FORCE_STAT_TOOLTIP_STRING";
	stringIdentifier[AVERAGE_BO_LENGTH_STAT_TOOLTIP_STRING] = "AVERAGE_BO_LENGTH_STAT_TOOLTIP_STRING";
	stringIdentifier[FITNESS_AVERAGE_STAT_TOOLTIP_STRING] = "FITNESS_AVERAGE_STAT_TOOLTIP_STRING";
	stringIdentifier[FITNESS_VARIANCE_STAT_TOOLTIP_STRING] = "FITNESS_VARIANCE_STAT_TOOLTIP_STRING";
	stringIdentifier[GENERATIONS_LEFT_STAT_TOOLTIP_STRING] = "GENERATIONS_LEFT_STAT_TOOLTIP_STRING";
	stringIdentifier[FPS_STAT_TOOLTIP_STRING] = "FPS_STAT_TOOLTIP_STRING";
	stringIdentifier[SAVE_GOAL_TOOLTIP_STRING] = "SAVE_GOAL_TOOLTIP_STRING";
	stringIdentifier[RESET_BUILD_ORDER_TOOLTIP_STRING] = "RESET_BUILD_ORDER_TOOLTIP_STRING";
	stringIdentifier[SAVE_BUILD_ORDER_TOOLTIP_STRING] = "SAVE_BUILD_ORDER_TOOLTIP_STRING";
	stringIdentifier[LOAD_BUILD_ORDER_TOOLTIP_STRING] = "LOAD_BUILD_ORDER_TOOLTIP_STRING";
	stringIdentifier[CONTINUE_OPTIMIZATION_TOOLTIP_STRING] = "CONTINUE_OPTIMIZATION_TOOLTIP_STRING";
	stringIdentifier[PAUSE_OPTIMIZATION_TOOLTIP_STRING] = "PAUSE_OPTIMIZATION_TOOLTIP_STRING";
	stringIdentifier[OF_GOALS_FULFILLED_TOOLTIP_STRING] = "OF_GOALS_FULFILLED_TOOLTIP_STRING";
	stringIdentifier[OF_TIME_FULFILLED_TOOLTIP_STRING] = "OF_TIME_FULFILLED_TOOLTIP_STRING";
	stringIdentifier[HELP_TAB_TOOLTIP_STRING] = "HELP_TAB_TOOLTIP_STRING";
	stringIdentifier[SETTINGS_TAB_TOOLTIP_STRING] = "SETTINGS_TAB_TOOLTIP_STRING";
	stringIdentifier[DATABASE_TAB_TOOLTIP_STRING] = "DATABASE_TAB_TOOLTIP_STRING";
	stringIdentifier[MAP_TAB_TOOLTIP_STRING] = "MAP_TAB_TOOLTIP_STRING";
	stringIdentifier[FORCEENTRY_TIME_TOOLTIP_STRING] = "FORCEENTRY_TIME_TOOLTIP_STRING";
	stringIdentifier[TITLE_PREDEFINED_SETTINGS_STRING] = "TITLE_PREDEFINED_SETTINGS_STRING";
	stringIdentifier[SETTING_MAX_TIME_STRING] = "SETTING_MAX_TIME_STRING";
	stringIdentifier[SETTING_RESTRICT_SC_STRING] = "SETTING_RESTRICT_SC_STRING";
	stringIdentifier[SETTING_FACILITY_MODE_STRING] = "SETTING_FACILITY_MODE_STRING";
	stringIdentifier[SETTING_AUTO_SAVE_RUNS_STRING] = "SETTING_AUTO_SAVE_RUNS_STRING";
	stringIdentifier[SETTING_ALWAYS_BUILD_WORKER_STRING] = "SETTING_ALWAYS_BUILD_WORKER_STRING";
	stringIdentifier[SETTING_ONLY_SWAP_ORDERS_STRING] = "SETTING_ONLY_SWAP_ORDERS_STRING";
	stringIdentifier[SETTING_PREPROCESS_BUILDORDER_STRING] = "SETTING_PREPROCESS_BUILDORDER_STRING";
	stringIdentifier[SETTING_MAX_LENGTH_STRING] = "SETTING_MAX_LENGTH_STRING";
	stringIdentifier[SETTING_MAX_RUNS_STRING] = "SETTING_MAX_RUNS_STRING";
	stringIdentifier[SETTING_MAX_GENERATIONS_STRING] = "SETTING_MAX_GENERATIONS_STRING";
	stringIdentifier[SETTING_MAX_TIMEOUT_STRING] = "SETTING_MAX_TIMEOUT_STRING";
	stringIdentifier[SETTING_ALLOW_GOAL_ADAPTION_STRING] = "SETTING_ALLOW_GOAL_ADAPTION_STRING";
	stringIdentifier[SETTING_BREED_FACTOR_STRING] = "SETTING_BREED_FACTOR_STRING";
	stringIdentifier[SETTING_CROSSING_OVER_STRING] = "SETTING_CROSSING_OVER_STRING";
	stringIdentifier[SETTING_MINIMALIST_STRING] = "SETTING_MINIMALIST_STRING";
	stringIdentifier[SETTING_FULL_STRING] = "SETTING_FULL_STRING";
	stringIdentifier[SETTING_CUSTOM_STRING] = "SETTING_CUSTOM_STRING";
	stringIdentifier[SETTING_DESIRED_FRAMERATE_STRING] = "SETTING_DESIRED_FRAMERATE_STRING";
	stringIdentifier[SETTING_DESIRED_CPU_USAGE_STRING] = "SETTING_DESIRED_CPU_USAGE_STRING";
	stringIdentifier[SETTING_SMOOTH_MOVEMENT_STRING] = "SETTING_SMOOTH_MOVEMENT_STRING";
	stringIdentifier[SETTING_SHOW_DEBUG_STRING] = "SETTING_SHOW_DEBUG_STRING";
	stringIdentifier[SETTING_GLOWING_BUTTONS_STRING] = "SETTING_GLOWING_BUTTONS_STRING";
	stringIdentifier[SETTING_DNA_SPIRAL_STRING] = "SETTING_DNA_SPIRAL_STRING";
	stringIdentifier[SETTING_ROUNDED_RECTANGLES_STRING] = "SETTING_ROUNDED_RECTANGLES_STRING";
	stringIdentifier[SETTING_BACKGROUND_BITMAP_STRING] = "SETTING_BACKGROUND_BITMAP_STRING";
	stringIdentifier[SETTING_TRANSPARENCY_STRING] = "SETTING_TRANSPARENCY_STRING";
	stringIdentifier[SETTING_FULLSCREEN_STRING] = "SETTING_FULLSCREEN_STRING";
	stringIdentifier[SETTING_TOOLTIPS_STRING] = "SETTING_TOOLTIPS_STRING";
	stringIdentifier[SETTING_SOFTWARE_MOUSE_STRING] = "SETTING_SOFTWARE_MOUSE_STRING";
	stringIdentifier[SETTING_UNLOAD_GRAPHICS_STRING] = "SETTING_UNLOAD_GRAPHICS_STRING";
	stringIdentifier[SETTING_USE_MUSIC_STRING] = "SETTING_USE_MUSIC_STRING";
	stringIdentifier[SETTING_USE_SOUND_STRING] = "SETTING_USE_SOUND_STRING";
	stringIdentifier[SETTING_MUSIC_VOLUME_STRING] = "SETTING_MUSIC_VOLUME_STRING";
	stringIdentifier[SETTING_SOUND_VOLUME_STRING] = "SETTING_SOUND_VOLUME_STRING";
	stringIdentifier[SETTING_CHANNELS_STRING] = "SETTING_CHANNELS_STRING";
	stringIdentifier[SETTING_MAX_TIME_TOOLTIP_STRING] = "SETTING_MAX_TIME_TOOLTIP_STRING";
	stringIdentifier[SETTING_RESTRICT_SC_TOOLTIP_STRING] = "SETTING_RESTRICT_SC_TOOLTIP_STRING";
	stringIdentifier[SETTING_FACILITY_MODE_TOOLTIP_STRING] = "SETTING_FACILITY_MODE_TOOLTIP_STRING";
	stringIdentifier[SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING] = "SETTING_AUTO_SAVE_RUNS_TOOLTIP_STRING";
	stringIdentifier[SETTING_ALWAYS_BUILD_WORKER_TOOLTIP_STRING] = "SETTING_ALWAYS_BUILD_WORKER_TOOLTIP_STRING";
	stringIdentifier[SETTING_ONLY_SWAP_ORDERS_TOOLTIP_STRING] = "SETTING_ONLY_SWAP_ORDERS_TOOLTIP_STRING";
	stringIdentifier[SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING] = "SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING";
	stringIdentifier[SETTING_MAX_LENGTH_TOOLTIP_STRING] = "SETTING_MAX_LENGTH_TOOLTIP_STRING";
	stringIdentifier[SETTING_MAX_RUNS_TOOLTIP_STRING] = "SETTING_MAX_RUNS_TOOLTIP_STRING";
	stringIdentifier[SETTING_MAX_GENERATIONS_TOOLTIP_STRING] = "SETTING_MAX_GENERATIONS_TOOLTIP_STRING";
	stringIdentifier[SETTING_MAX_TIMEOUT_TOOLTIP_STRING] = "SETTING_MAX_TIMEOUT_TOOLTIP_STRING";
	stringIdentifier[SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING] = "SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING";
	stringIdentifier[SETTING_BREED_FACTOR_TOOLTIP_STRING] = "SETTING_BREED_FACTOR_TOOLTIP_STRING";
	stringIdentifier[SETTING_CROSSING_OVER_TOOLTIP_STRING] = "SETTING_CROSSING_OVER_TOOLTIP_STRING";
	stringIdentifier[SETTING_MINIMALIST_TOOLTIP_STRING] = "SETTING_MINIMALIST_TOOLTIP_STRING";
	stringIdentifier[SETTING_FULL_TOOLTIP_STRING] = "SETTING_FULL_TOOLTIP_STRING";
	stringIdentifier[SETTING_CUSTOM_TOOLTIP_STRING] = "SETTING_CUSTOM_TOOLTIP_STRING";
	stringIdentifier[SETTING_DESIRED_FRAMERATE_TOOLTIP_STRING] = "SETTING_DESIRED_FRAMERATE_TOOLTIP_STRING";
	stringIdentifier[SETTING_DESIRED_CPU_USAGE_TOOLTIP_STRING] = "SETTING_DESIRED_CPU_USAGE_TOOLTIP_STRING";
	stringIdentifier[SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING] = "SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING";
	stringIdentifier[SETTING_SHOW_DEBUG_TOOLTIP_STRING] = "SETTING_SHOW_DEBUG_TOOLTIP_STRING";
	stringIdentifier[SETTING_GLOWING_BUTTONS_TOOLTIP_STRING] = "SETTING_GLOWING_BUTTONS_TOOLTIP_STRING";
	stringIdentifier[SETTING_DNA_SPIRAL_TOOLTIP_STRING] = "SETTING_DNA_SPIRAL_TOOLTIP_STRING";
	stringIdentifier[SETTING_ROUNDED_RECTANGLES_TOOLTIP_STRING] = "SETTING_ROUNDED_RECTANGLES_TOOLTIP_STRING";
	stringIdentifier[SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING] = "SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING";
	stringIdentifier[SETTING_TRANSPARENCY_TOOLTIP_STRING] = "SETTING_TRANSPARENCY_TOOLTIP_STRING";
	stringIdentifier[SETTING_FULLSCREEN_TOOLTIP_STRING] = "SETTING_FULLSCREEN_TOOLTIP_STRING";
	stringIdentifier[SETTING_TOOLTIPS_TOOLTIP_STRING] = "SETTING_TOOLTIPS_TOOLTIP_STRING";
	stringIdentifier[SETTING_SOFTWARE_MOUSE_TOOLTIP_STRING] = "SETTING_SOFTWARE_MOUSE_TOOLTIP_STRING";
	stringIdentifier[SETTING_UNLOAD_GRAPHICS_TOOLTIP_STRING] = "SETTING_UNLOAD_GRAPHICS_TOOLTIP_STRING";
	stringIdentifier[SETTING_USE_MUSIC_TOOLTIP_STRING] = "SETTING_USE_MUSIC_TOOLTIP_STRING";
	stringIdentifier[SETTING_USE_SOUND_TOOLTIP_STRING] = "SETTING_USE_SOUND_TOOLTIP_STRING";
	stringIdentifier[SETTING_MUSIC_VOLUME_TOOLTIP_STRING] = "SETTING_MUSIC_VOLUME_TOOLTIP_STRING";
	stringIdentifier[SETTING_SOUND_VOLUME_TOOLTIP_STRING] = "SETTING_SOUND_VOLUME_TOOLTIP_STRING";
	stringIdentifier[SETTING_CHANNELS_TOOLTIP_STRING] = "SETTING_CHANNELS_TOOLTIP_STRING";
	stringIdentifier[SETTINGS_SAVED_STRING] = "SETTINGS_SAVED_STRING";
	stringIdentifier[LANGUAGE_HAS_CHANGED_STRING] = "LANGUAGE_HAS_CHANGED_STRING";
	stringIdentifier[SETTING_LANGUAGE_STRING] = "SETTING_LANGUAGE_STRING";
	stringIdentifier[SETTING_ZERO_LANGUAGE_STRING] = "SETTING_ZERO_LANGUAGE_STRING";
	stringIdentifier[SETTING_ENGLISH_LANGUAGE_STRING] = "SETTING_ENGLISH_LANGUAGE_STRING";
	stringIdentifier[SETTING_GERMAN_LANGUAGE_STRING] = "SETTING_GERMAN_LANGUAGE_STRING";
	stringIdentifier[SETTING_ITALIAN_LANGUAGE_STRING] = "SETTING_ITALIAN_LANGUAGE_STRING";
	stringIdentifier[SETTING_PORTUGESE_LANGUAGE_STRING] = "SETTING_PORTUGESE_LANGUAGE_STRING";
	stringIdentifier[SETTING_DUTCH_LANGUAGE_STRING] = "SETTING_DUTCH_LANGUAGE_STRING";
	stringIdentifier[SETTING_FINNISH_LANGUAGE_STRING] = "SETTING_FINNISH_LANGUAGE_STRING";
	stringIdentifier[SETTING_GREEK_LANGUAGE_STRING] = "SETTING_GREEK_LANGUAGE_STRING";
	stringIdentifier[SETTING_FRENCH_LANGUAGE_STRING] = "SETTING_FRENCH_LANGUAGE_STRING";
	stringIdentifier[SETTING_SPANISH_LANGUAGE_STRING] = "SETTING_SPANISH_LANGUAGE_STRING";
	stringIdentifier[SETTING_POLSKI_LANGUAGE_STRING] = "SETTING_POLSKI_LANGUAGE_STRING";
	stringIdentifier[SETTING_KOREAN_LANGUAGE_STRING] = "SETTING_KOREAN_LANGUAGE_STRING";
	stringIdentifier[SETTING_CHINESE_LANGUAGE_STRING] = "SETTING_CHINESE_LANGUAGE_STRING";
	stringIdentifier[SETTING_RUSSIAN_LANGUAGE_STRING] = "SETTING_RUSSIAN_LANGUAGE_STRING";
	stringIdentifier[SETTING_RESOLUTION_STRING] = "SETTING_RESOLUTION_STRING";
	stringIdentifier[SETTING_RESOLUTION_ZERO_STRING] = "SETTING_RESOLUTION_ZERO_STRING";
	stringIdentifier[SETTING_RESOLUTION_640x480_STRING] = "SETTING_RESOLUTION_640x480_STRING";
	stringIdentifier[SETTING_RESOLUTION_800x600_STRING] = "SETTING_RESOLUTION_800x600_STRING";
	stringIdentifier[SETTING_RESOLUTION_1024x768_STRING] = "SETTING_RESOLUTION_1024x768_STRING";
	stringIdentifier[SETTING_RESOLUTION_1280x1024_STRING] = "SETTING_RESOLUTION_1280x1024_STRING";
	stringIdentifier[SETTING_BITDEPTH_STRING] = "SETTING_BITDEPTH_STRING";
	stringIdentifier[SETTING_DEPTH_8BIT_STRING] = "SETTING_DEPTH_8BIT_STRING";
	stringIdentifier[SETTING_DEPTH_16BIT_STRING] = "SETTING_DEPTH_16BIT_STRING";
	stringIdentifier[SETTING_DEPTH_24BIT_STRING] = "SETTING_DEPTH_24BIT_STRING";
	stringIdentifier[SETTING_DEPTH_32BIT_STRING] = "SETTING_DEPTH_32BIT_STRING";
	stringIdentifier[SETTING_THEME_STRING] = "SETTING_THEME_STRING";
	stringIdentifier[SETTING_ZERO_THEME_STRING] = "SETTING_ZERO_THEME_STRING";
	stringIdentifier[SETTING_DARK_RED_THEME_STRING] = "SETTING_DARK_RED_THEME_STRING";
	stringIdentifier[SETTING_DARK_BLUE_THEME_STRING] = "SETTING_DARK_BLUE_THEME_STRING";
	stringIdentifier[SETTING_GREEN_THEME_STRING] = "SETTING_GREEN_THEME_STRING";
	stringIdentifier[SETTING_YELLOW_THEME_STRING] = "SETTING_YELLOW_THEME_STRING";
	stringIdentifier[SETTING_GREY_THEME_STRING] = "SETTING_GREY_THEME_STRING";
	stringIdentifier[SETTING_RELOAD_FROM_FILE_STRING] = "SETTING_RELOAD_FROM_FILE_STRING";
	stringIdentifier[SETTING_LOAD_FAILSAFE_DEFAULTS_STRING] = "SETTING_LOAD_FAILSAFE_DEFAULTS_STRING";
	stringIdentifier[SETTING_SAVE_TO_FILE_STRING] = "SETTING_SAVE_TO_FILE_STRING";
	stringIdentifier[SETWINDOW_CORE_SETTINGS_STRING] = "SETWINDOW_CORE_SETTINGS_STRING";
	stringIdentifier[SETWINDOW_GUI_SETTINGS_STRING] = "SETWINDOW_GUI_SETTINGS_STRING";
	stringIdentifier[SETWINDOW_SOUND_SETTINGS_STRING] = "SETWINDOW_SOUND_SETTINGS_STRING";
	stringIdentifier[SETWINDOW_UI_SETTINGS_STRING] = "SETWINDOW_UI_SETTINGS_STRING";
	stringIdentifier[SETWINDOW_LOADSAVE_SETTINGS_STRING] = "SETWINDOW_LOADSAVE_SETTINGS_STRING";
	stringIdentifier[MAPWINDOW_MAP_SETTINGS_STRING] = "MAPWINDOW_MAP_SETTINGS_STRING";
	stringIdentifier[MAPWINDOW_MAX_PLAYER_STRING] = "MAPWINDOW_MAX_PLAYER_STRING";
	stringIdentifier[MAPWINDOW_MAX_LOCATIONS_STRING] = "MAPWINDOW_MAX_LOCATIONS_STRING";
	stringIdentifier[MAPWINDOW_MAP_NAME_STRING] = "MAPWINDOW_MAP_NAME_STRING";
	stringIdentifier[MAPWINDOW_SYMMETRY_STRING] = "MAPWINDOW_SYMMETRY_STRING";
	stringIdentifier[MAPWINDOW_LOCATION_SETTINGS_STRING] = "MAPWINDOW_LOCATION_SETTINGS_STRING";
	stringIdentifier[MAPWINDOW_PLAYER_SETTINGS_STRING] = "MAPWINDOW_PLAYER_SETTINGS_STRING";
	stringIdentifier[MAPWINDOW_LOCATION_CONTENT_STRING] = "MAPWINDOW_LOCATION_CONTENT_STRING";
	stringIdentifier[MAPWINDOW_DISTANCES_STRING] = "MAPWINDOW_DISTANCES_STRING";
	stringIdentifier[MAPWINDOW_MINERAL_BLOCKS_STRING] = "MAPWINDOW_MINERAL_BLOCKS_STRING";
	stringIdentifier[MAPWINDOW_VESPENE_GEYSIRS_STRING] = "MAPWINDOW_VESPENE_GEYSIRS_STRING";
	stringIdentifier[MAPWINDOW_MINERAL_DISTANCE_STRING] = "MAPWINDOW_MINERAL_DISTANCE_STRING";
	stringIdentifier[MAPWINDOW_MAX_PLAYER_TOOLTIP_STRING] = "MAPWINDOW_MAX_PLAYER_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_MAX_LOCATIONS_TOOLTIP_STRING] = "MAPWINDOW_MAX_LOCATIONS_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_SYMMETRY_TOOLTIP_STRING] = "MAPWINDOW_SYMMETRY_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_CREATE_NEW_MAP_TOOLTIP_STRING] = "MAPWINDOW_CREATE_NEW_MAP_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_RESET_MAP_TOOLTIP_STRING] = "MAPWINDOW_RESET_MAP_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_LOAD_MAP_TOOLTIP_STRING] = "MAPWINDOW_LOAD_MAP_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_SAVE_MAP_TOOLTIP_STRING] = "MAPWINDOW_SAVE_MAP_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_MINERAL_BLOCKS_TOOLTIP_STRING] = "MAPWINDOW_MINERAL_BLOCKS_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_VESPENE_GEYSIRS_TOOLTIP_STRING] = "MAPWINDOW_VESPENE_GEYSIRS_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_MINERAL_DISTANCE_TOOLTIP_STRING] = "MAPWINDOW_MINERAL_DISTANCE_TOOLTIP_STRING";
	stringIdentifier[MAPWINDOW_DISTANCES_TOOLTIP_STRING] = "MAPWINDOW_DISTANCES_TOOLTIP_STRING";
	stringIdentifier[OUTPUT_UNITNAME_STRING] = "OUTPUT_UNITNAME_STRING";
	stringIdentifier[OUTPUT_SUPPLY_STRING] = "OUTPUT_SUPPLY_STRING";
	stringIdentifier[OUTPUT_MINERALS_STRING] = "OUTPUT_MINERALS_STRING";
	stringIdentifier[OUTPUT_GAS_STRING] = "OUTPUT_GAS_STRING";
	stringIdentifier[OUTPUT_LOCATION_STRING] = "OUTPUT_LOCATION_STRING";
	stringIdentifier[OUTPUT_TIME_STRING] = "OUTPUT_TIME_STRING";
	stringIdentifier[ENDRUN_FINISHED_STRING] = "ENDRUN_FINISHED_STRING";
	stringIdentifier[ENDRUN_SAVED_BUILDORDER_STRING] = "ENDRUN_SAVED_BUILDORDER_STRING";
	stringIdentifier[ENDRUN_DIALOG_TITLE_STRING] = "ENDRUN_DIALOG_TITLE_STRING";
	stringIdentifier[ENDRUN_QUESTION_STRING] = "ENDRUN_QUESTION_STRING";
	stringIdentifier[ENDRUN_SAVE_AND_CONTINUE_STRING] = "ENDRUN_SAVE_AND_CONTINUE_STRING";
	stringIdentifier[ENDRUN_DONT_SAVE_AND_CONTINUE_STRING] = "ENDRUN_DONT_SAVE_AND_CONTINUE_STRING";
	stringIdentifier[SUCCESS_OK_STRING] = "SUCCESS_OK_STRING";
	stringIdentifier[SUCCESS_MINERALS_STRING] = "SUCCESS_MINERALS_STRING";
	stringIdentifier[SUCCESS_GAS_STRING] = "SUCCESS_GAS_STRING";
	stringIdentifier[SUCCESS_SUPPLY_STRING] = "SUCCESS_SUPPLY_STRING";
	stringIdentifier[SUCCESS_PREREQUISITE_STRING] = "SUCCESS_PREREQUISITE_STRING";
	stringIdentifier[SUCCESS_FACILITY_STRING] = "SUCCESS_FACILITY_STRING";
	stringIdentifier[SUCCESS_TIMEOUT_STRING] = "SUCCESS_TIMEOUT_STRING";
	stringIdentifier[SUCCESS_UNKNOWN_STRING] = "SUCCESS_UNKNOWN_STRING";
	stringIdentifier[INFO_BUILD_STRING] = "INFO_BUILD_STRING";
	stringIdentifier[INFO_AS_SOON_AS_STRING] = "INFO_AS_SOON_AS_STRING";
	stringIdentifier[INFO_BECOMES_AVAILIBLE_STRING] = "INFO_BECOMES_AVAILIBLE_STRING";
	stringIdentifier[INFO_AT_STRING] = "INFO_AT_STRING";
	stringIdentifier[INFO_WHEN_STRING] = "INFO_WHEN_STRING";
	stringIdentifier[INFO_HAVING_STRING] = "INFO_HAVING_STRING";
	stringIdentifier[INFO_MINERALS_STRING] = "INFO_MINERALS_STRING";
	stringIdentifier[INFO_GAS_STRING] = "INFO_GAS_STRING";
	stringIdentifier[INFO_SUPPLY_STRING] = "INFO_SUPPLY_STRING";
	stringIdentifier[INFO_TIME_STRING] = "INFO_TIME_STRING";
}
