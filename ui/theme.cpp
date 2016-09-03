#include "theme.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

UI_Theme::UI_Theme():
	resolution(RESOLUTION_1280x1024),
	language(GERMAN_LANGUAGE),
	colorTheme(DARK_BLUE_THEME)
{
	for(unsigned int i = MAX_LANGUAGES;i--;)
		for(unsigned int j = MAX_STRINGS;j--;)
			stringList[i][j] = NULL;
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
		
  		for(unsigned int j = MAX_RESOLUTIONS;j--;)
			 for(unsigned int k = MAX_BITMAPS;k--;)
				bitmapList[i][j][k] = NULL;
					
		for(unsigned int j = MAX_BRUSHES;j--;)
			brushList[i][j] = NULL;
	}
	for(unsigned int i = MAX_RESOLUTIONS;i--;)
	{
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
	}
	for(unsigned int i=MAX_BUTTONS;i--;)
		buttonAnimationList[i]=0;

}

UI_Theme::~UI_Theme()
{
 	for(unsigned int i = MAX_LANGUAGES;i--;)
 		for(unsigned int j = MAX_STRINGS;j--;)
			delete stringList[i][j];
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
		
		for(unsigned int j = MAX_RESOLUTIONS;j--;)
			for(unsigned int k = MAX_BITMAPS;k--;)
				SDL_FreeSurface(bitmapList[i][j][k]);
	}
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

	for(unsigned int i=MAX_BUTTONS;i--;)
		delete buttonAnimationList[i];
}



void findandreplace( std::string& source, const std::string& find, const std::string& replace )
{
	size_t j = source.find(find);source.replace(j,find.length(),replace);
	// TODO wenn nix gefunden wird...
}

const std::string UI_Theme::lookUpFormattedString(const eString id, const std::string& text) const
{
#ifdef _SCC_DEBUG
	if((id<0)||(id>=MAX_STRINGS)) {
		toLog("ERROR: (UI_Theme::lookUpFormattedString) id out of range.");return("");
	}
#endif
	std::string bla = *(stringList[language][id]);
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
	std::string bla = *(stringList[language][id]);
	std::ostringstream os;
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
	std::string bla = *(stringList[language][id]);
	std::ostringstream os;
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
	std::string bla=*(stringList[language][id]);
	std::ostringstream os;
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
	if(item=="@FONTS") return(FONT_DATA_TYPE);else
	if(item=="@MAIN") return(WINDOW_DATA_TYPE);else
	if(item=="@COLORS") return(COLOR_DATA_TYPE);else
	if(item=="@PENS") return(PEN_DATA_TYPE);else
	if(item=="@BRUSHES") return(BRUSH_DATA_TYPE);else
	if(item=="@BITMAPS") return(BITMAP_DATA_TYPE);else
	if(item=="@BUTTONS") return(BUTTON_DATA_TYPE);else
	return(ZERO_DATA_TYPE);
}

const eSubDataType getSubDataType(const eDataType mode)
{
	switch(mode)
	{	
		case STRING_DATA_TYPE:return(LANGUAGE_SUB_DATA_TYPE);
		case FONT_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
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
	if(item=="@MONOCHROME_THEME") return(MONOCHROME_THEME);else
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
	if(item=="Message window") return(MESSAGE_WINDOW);else
	if(item=="Tutorial window") return(TUTORIAL_WINDOW);else
	if(item=="Settings window") return(SETTINGS_WINDOW);else
	if(item=="Info window") return(INFO_WINDOW);else
	if(item=="Tech tree window") return(TECHTREE_WINDOW);else
	if(item=="Edit field window") return(EDIT_FIELD_WINDOW);else
	return(NULL_GLOBAL_WINDOW);
}

eGameWindow parse_game_window(const std::string& item)
{
	if(item=="Game window") return(GAME_WINDOW);else
	if(item=="Score window") return(SCORE_WINDOW);else
	if(item=="Database window") return(DATABASE_WINDOW);else
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
		if(item=="dock bottom inside of") return(DOCK_BOTTOM_INSIDE_OF_COMMAND);else
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
		if(item=="dock bottom inside of") return(DOCK_BOTTOM_INSIDE_OF_COMMAND);else
		return(NO_COMMAND);
	}
}

Rect* parse_window(const std::string* parameter, Rect** windows, unsigned int& maxheight, int type, const bool horizontal_mirror)
{
	Rect* rect = new Rect();
	bool xpart=false; bool ypart=false; bool xypart=false; bool dxpart=true; bool dypart=false; bool hpart=false;

	for(unsigned int i = 1; i <49;i++)
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
			i++;
			int win;
			if(type==0) win = parse_global_window(parameter[i]);else
			if(type==1) win = parse_game_window(parameter[i]);else
			if(type==2) win = parse_player_window(parameter[i]);
			if((win==0)&&(command>=DOCK_WITH_LEFT_BORDER_OF_COMMAND))
			{
				toLog(parameter[i]);
			}
			switch(command)
			{
				case ABSOLUTE_COORDINATES_COMMAND:i--;xpart=true;xypart=true;break;
				case ABSOLUTE_X_COORDINATE_COMMAND:i--;xpart=true;break;
				case ABSOLUTE_Y_COORDINATE_COMMAND:i--;ypart=true;break;
				case DOCK_WITH_LEFT_BORDER_OF_COMMAND:rect->SetLeft(-10 + windows[win]->GetLeft() - rect->GetWidth());break;
				case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:rect->SetLeft(10 + windows[win]->GetRight());break;
				case DOCK_WITH_LOWER_BORDER_OF_COMMAND:rect->SetTop(25 + windows[win]->GetBottom());break;
				case DOCK_WITH_UPPER_BORDER_OF_COMMAND:rect->SetTop(25 + windows[win]->GetTop() - rect->GetHeight());break;

				case DOCK_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(15 + windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, 15 + windows[win]->GetHeight() / 2 - rect->GetHeight() / 2));break;
				case DOCK_BOTTOM_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, windows[win]->GetHeight() - rect->GetHeight() - 35));break;
				case DOCK_TOP_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, 15));break;
				case DOCK_LEFT_INSIDE_OF_COMMAND:rect->SetLeft(15);break;
				case DOCK_RIGHT_INSIDE_OF_COMMAND:rect->SetLeft(-15+windows[win]->GetWidth() - rect->GetWidth());break;
				case DOCK_TOP_INSIDE_OF_COMMAND:rect->SetTop(15);break;
				case DOCK_BOTTOM_INSIDE_OF_COMMAND:rect->SetTop(-15+windows[win]->GetHeight() - rect->GetHeight());break;
								   
/*				case DOCK_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetLeft() + windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, windows[win]->GetTop() + windows[win]->GetHeight() / 2 - rect->GetHeight() / 2));break;
				case DOCK_BOTTOM_CENTER_INSIDE_OF_COMMAND:rect->SetTopLeft(Point(windows[win]->GetLeft() + windows[win]->GetWidth() / 2 - rect->GetWidth() / 2, windows[win]->GetBottom() - rect->GetHeight() - 35));break;
				case DOCK_LEFT_INSIDE_OF_COMMAND:rect->SetLeft(10+windows[win]->GetLeft());break;
				case DOCK_RIGHT_INSIDE_OF_COMMAND:rect->SetLeft(-15+windows[win]->GetRight() - rect->GetWidth());break;
				case DOCK_TOP_INSIDE_OF_COMMAND:rect->SetTop(windows[win]->GetTop());break;
				case DOCK_BOTTOM_INSIDE_OF_COMMAND:rect->SetTop(-15+windows[win]->GetBottom() - rect->GetHeight());break;*/
				default:i--;;break;
			}
		}
	}
	return(rect);
}

void UI_Theme::loadStringFile(const std::string& dataFile)
{
	if((dataFile.substr(dataFile.size()-2,2) == "..") ||(dataFile.substr(dataFile.size()-1,1) == "."))
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

	std::ifstream pFile(dataFile.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadStringFile) Could not open file! [" + dataFile + "]");
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
			line2++;
		if((*line2)=='\0')
			continue;
		
		strcpy(old,line2);
		
		if((buffer=strtok(line2,",\0"))!=NULL)
			parameter[0]=buffer;
		unsigned int k=1;
		
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
			while(((*buffer)==32)||((*buffer)==9))
				buffer++;
			parameter[k]=buffer;
			k++;
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
					case STRING_DATA_TYPE:stringList[current_language][current_line]=new std::string(parameter[0]);
					default:break;
				}
				current_line++;
			}
			// 0 ebenen -> buttons :) BUTTON_DATA_TYPE?? TODO
		} // end if mode != ZERO_DATA_TYPE
	} // end while

	for(unsigned int i = MAX_LANGUAGES; i--;)
		for(unsigned int j = MAX_STRINGS; j--;)
			if(stringList[i][j]==NULL)
				stringList[i][j] = new std::string("ERROR");

}

void UI_Theme::loadWindowData(const std::string& dataFile, const unsigned int gameNumber, const unsigned int maxGames)
{
	const unsigned int MAX_PARAMETERS = 50;
	char line[1024], old[1024];
	char* buffer;
	std::string parameter[MAX_PARAMETERS];
	unsigned int value[MAX_PARAMETERS];
	
	eGameType game_type=ZERO_GAME_TYPE;
	ePlayerType player_type=ZERO_PLAYER_TYPE;
	eResolution current_resolution=ZERO_RESOLUTION;

	unsigned int current_line = 0;
	
 	for(unsigned int i=MAX_PARAMETERS;i--;)
	{
		parameter[i]="";
		value[i]=0;
	}

	std::ifstream pFile(dataFile.c_str());
	
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
		while(((*line2)==32)||((*line2)==9)) line2++;
		if((*line2)=='\0') continue;
		
		strcpy(old,line2);
		
		if((buffer=strtok(line2,",\0"))!=NULL) parameter[0]=buffer;
		
		unsigned int k=1;
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
			while(((*buffer)==32)||((*buffer)==9))
				buffer++;
			parameter[k]=buffer;
			k++;
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
			current_resolution=getResolutionSubDataEntry(parameter[0]);
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
				current_resolution=getResolutionSubDataEntry(line2);
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
				current_line++;
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
				current_line++;
			}
		} 
	} 
}




void UI_Theme::loadGraphicData(const std::string& dataFile, const std::string& bitmapDir, const std::string& fontDir, DC* dc)
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

	std::ifstream pFile(dataFile.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadGraphicData) Could not open data file!");
#endif
		return;
	}
	
	while(pFile.getline(line,1024))
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
			line2++;
		if((*line2)=='\0')
			continue;
		
		strcpy(old,line2);
		
		if((buffer=strtok(line2,",\0"))!=NULL)
			parameter[0]=buffer;
		unsigned int k=1;
		
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
			while(((*buffer)==32)||((*buffer)==9))
				buffer++;
			parameter[k]=buffer;
			k++;
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
					case STRING_DATA_TYPE:stringList[current_language][current_line]=new std::string(parameter[0]);
					//toLog(parameter[0]);
					break;
					case COLOR_DATA_TYPE:
								  colorList[current_theme][current_line]=new Color(dc->GetSurface(),(Uint8)value[0],(Uint8)value[1],(Uint8)value[2]);break;
					case PEN_DATA_TYPE:penList[current_theme][current_line]=new Pen(dc->GetSurface(),value[1],value[2],value[3],value[0],get_pen_style(parameter[4]));break;
					case BRUSH_DATA_TYPE:brushList[current_theme][current_line]=new Brush(dc->GetSurface(),(Uint8)value[0],(Uint8)value[1],(Uint8)value[2],get_brush_style(parameter[3]));break;
					case FONT_DATA_TYPE:
					{
						std::string t=fontDir+parameter[0]+".ttf";
						fontList[current_resolution]/*[current_language]*/[current_line]=new Font(t, value[1]/*, get_font_style1(parameter[2]), get_font_style2(parameter[3]), get_font_style3(parameter[4]), false, _T(""), FONTENCODING_DEFAULT*/);
//						std::ostringstream os;
//						os << "- " << current_resolution << " " << current_line << " " << t << " " << value[1];
//						toLog(os.str());
					}break;
					case WINDOW_DATA_TYPE:
					{
						unsigned int max_height = 0;
						globalRectList[current_resolution][current_line+1] = parse_window(parameter, globalRectList[current_resolution], max_height, 0, false);
						setMaxGlobalHeight(current_resolution, current_line+1, max_height);
					}break;
					default:break;
				}
				current_line++;
			}
			// 0 ebenen -> buttons :) BUTTON_DATA_TYPE?? TODO
			else if((sub_mode==ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE))
			{
				buttonAnimationList[current_line] = new ButtonAnimation;
				buttonAnimationList[current_line]->speed=value[0];
				buttonAnimationList[current_line]->type=(eButtonAnimationType)value[1];
				for(unsigned int i=MAX_BUTTON_ANIMATION_PHASES;i--;)
				{
					buttonAnimationList[current_line]->startBrush[i]=(eBrush)(value[0*MAX_BUTTON_ANIMATION_PHASES+2+i]);
					buttonAnimationList[current_line]->endBrush[i]=(eBrush)(value[1*MAX_BUTTON_ANIMATION_PHASES+2+i]);
					buttonAnimationList[current_line]->startTextColor[i]=(eColor)(value[2*MAX_BUTTON_ANIMATION_PHASES+2+i]);
					buttonAnimationList[current_line]->endTextColor[i]=(eColor)(value[3*MAX_BUTTON_ANIMATION_PHASES+2+i]);
					buttonAnimationList[current_line]->startBorderPen[i]=(ePen)(value[4*MAX_BUTTON_ANIMATION_PHASES+2+i]);
					buttonAnimationList[current_line]->endBorderPen[i]=(ePen)(value[5*MAX_BUTTON_ANIMATION_PHASES+2+i]);
					buttonAnimationList[current_line]->bitmap[i]=(eBitmap)(value[6*MAX_BUTTON_ANIMATION_PHASES+2+i]);
//					buttonAnimationList[current_line]->text[i]=(eString)(value[7*MAX_BUTTON_ANIMATION_PHASES+2+i]);
				}
				current_line++;				
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
							std::string name = bitmapDir+parameter[0]+".bmp";
							SDL_Surface* temp = SDL_LoadBMP(name.c_str());
							if(temp == NULL)
							{
								toLog("Could not load Bitmap " + name);
								SDL_FreeSurface(temp);
								bitmapList[current_resolution][current_theme][current_line] = NULL;
							}
							else 
								bitmapList[current_resolution][current_theme][current_line] = temp;
							
//							SDL_SetColorKey(bitmapList[current_resolution][current_theme][current_line]->getSurface(), SDL_SRCCOLORKEY , SDL_MapRGB(bitmapList[current_resolution][current_theme][current_line]->getFormat(), 0,0,0));
		
						}break;
					default:break;
				}
				current_line++;
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
//	os << "getting id " << id << " (with resolution " << resolution << ")";
//	toLog(os.str());
	return(fontList[resolution]/*[language]*/[id]);
}


