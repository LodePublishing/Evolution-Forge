#include "theme.hpp"
#include <fstream>

UI_Theme::UI_Theme()
{
	for(int i = MAX_LANGUAGES;i--;)
		for(int j = MAX_STRINGS;j--;)
			stringList[i][j]=0;
	for(int i = MAX_RESOLUTIONS;i--;)
		for(int j = MAX_LANGUAGES;j--;)
			for(int k = MAX_FONTS;k--;)
				fontList[i][j][k]=0;
	for(int i = MAX_COLOUR_THEMES;i--;)
	{
		for(int j = MAX_COLOURS;j--;)
				colorList[i][j]=0;
  		for(int j = MAX_RESOLUTIONS;j--;)
		{
			for(int k = MAX_PENS;k--;)
				penList[i][j][k]=0;
			 for(int k = MAX_BITMAPS;k--;)
				bitmapList[i][j][k]=0;
		}
					
		for(int j = MAX_BRUSHES;j--;)
				brushList[i][j]=0;
	}
	for(int i = MAX_RESOLUTIONS;i--;)
		for(int j = MAX_TABS;j--;)
			for(int k = MAX_WINDOWS;k--;)
			{
  				rectList[i][j][k]=0;
				maxRectList[i][j][k]=0;
				xwindow[i][j][k]=NULL_WINDOW;
				ywindow[i][j][k]=NULL_WINDOW;
			}
	for(int i=MAX_BUTTONS;i--;)
		buttonAnimationList[i]=0;

	tab=BASIC_TAB;
	resolution=RESOLUTION_1280x1024;
	language=ENGLISH_LANGUAGE;
	colorTheme=DARK_BLUE_THEME;
};

UI_Theme::~UI_Theme()
{
 	for(int i = MAX_LANGUAGES;i--;)
 		for(int j = MAX_STRINGS;j--;)
			delete stringList[i][j];
	for(int i = MAX_RESOLUTIONS;i--;)
		for(int j = MAX_LANGUAGES;j--;)
			for(int k = MAX_FONTS;k--;)
				delete fontList[i][j][k];
	for(int i = MAX_COLOUR_THEMES;i--;)
	{
		for(int j = MAX_COLOURS;j--;)
			delete colorList[i][j];
		for(int j = MAX_RESOLUTIONS;j--;)
		{
			for(int k = MAX_BITMAPS;k--;)
				delete bitmapList[i][j][k];
	 		for(int k = MAX_PENS;k--;)
				delete penList[i][j][k];
		}
		for(int j = MAX_BRUSHES;j--;)
			delete brushList[i][j];
	}
 	for(int i = MAX_RESOLUTIONS;i--;)
		for(int j = MAX_TABS;j--;)
			for(int k = MAX_WINDOWS;k--;)
			{
				delete rectList[i][j][k];
				delete maxRectList[i][j][k];
			}	
	for(int i=MAX_BUTTONS;i--;)
		delete buttonAnimationList[i];
};

void UI_Theme::setColorTheme(const eTheme colorTheme)
{
	this->colorTheme=colorTheme;
};

const eTheme UI_Theme::getColorTheme() const
{
	return(colorTheme);
};

const eLanguage UI_Theme::getLanguage() const
{
	return(language);
};

void UI_Theme::setLanguage(const eLanguage language)
{
	this->language=language;
};

const eResolution UI_Theme::getResolution() const
{
	return(resolution);
};

void UI_Theme::setResolution(const eResolution resolution)
{
	this->resolution=resolution;
};

void UI_Theme::setTab(const eTab tab)
{
	this->tab=tab;
};

const eTab UI_Theme::getTab() const
{ 
	return(tab);
};

const ButtonAnimation* UI_Theme::lookUpButtonAnimation(const eButton id) const
{
	return(buttonAnimationList[id]);
};


const string* UI_Theme::lookUpString(const eString id) const
{
	return(stringList[language][id]);
};

void findandreplace( string& source, const string& find, const string& replace )
{
	size_t j = source.find(find);source.replace(j,find.length(),replace);
	// TODO wenn nix gefunden wird...
};

const string UI_Theme::lookUpFormattedString(const eString id, const string& text) const
{
	string bla=*(stringList[language][id]);
	findandreplace(bla, "%s", text);
	return(bla);
};

const string UI_Theme::lookUpFormattedString(const eString id, const int i) const
{
	string bla=*(stringList[language][id]);
	ostringstream os;
	os << i; 
	findandreplace(bla, "%i", os.str());
	return(bla);
};
const string UI_Theme::lookUpFormattedString(const eString id, const int i, const int j, const int k) const
{
	string bla=*(stringList[language][id]);
	ostringstream os;
	os << i;findandreplace(bla, "%i", os.str());os.str("");
	os << j;findandreplace(bla, "%i", os.str());os.str("");
	os << k;findandreplace(bla, "%i", os.str());
	return(bla);
};

const string UI_Theme::lookUpFormattedString(const eString id, const int i, const int j) const
{
	string bla=*(stringList[language][id]);
	ostringstream os;
	if(bla.find("%2i")!=string::npos)
	{
		os << setfill('0') << setw(2) << i;
		findandreplace(bla, "%2i", os.str());os.str("");
	}
	else
	{
		os << i;
		findandreplace(bla, "%i", os.str());os.str("");
	}
	if(bla.find("%2i")!=string::npos)
	{
		os << setfill('0') << setw(2) << j;
		findandreplace(bla, "%2i", os.str());os.str("");
	}
	else
	{
		os << j;
		findandreplace(bla, "%i", os.str());os.str("");
	}
	return(bla);
};


Color* UI_Theme::lookUpColor(const eColor id) const
{
	return(colorList[colorTheme][id]);
};

const Bitmap* UI_Theme::lookUpBitmap(const eBitmap id) const 
{
	return(bitmapList[resolution][colorTheme][id]);
};

Pen* UI_Theme::lookUpPen(const ePen id) const
{
	return(penList[resolution][colorTheme][id]);
};

Brush* UI_Theme::lookUpBrush(const eBrush id) const
{
	return(brushList[colorTheme][id]);
};

Font* UI_Theme::lookUpFont(const eFont id) const
{
	return(fontList[resolution][language][id]);
};

const Point UI_Theme::lookUpRealDistance(const eWindow id, const int windowNumber) const // ~~ Name
{
	if(id==MAIN_WINDOW) return(Point(0,0));
	switch(arrangeDirection[resolution][tab][id])
	{
		case ARRANGE_LEFT_TO_RIGHT:return(Point(windowNumber*rectList[resolution][tab][id]->width,0));break;
		case ARRANGE_RIGHT_TO_LEFT:return(Point(-windowNumber*rectList[resolution][tab][id]->width,0));break;
		case ARRANGE_TOP_TO_DOWN:return(Point(0,windowNumber*rectList[resolution][tab][id]->height));break;
		case ARRANGE_DOWN_TO_TOP:return(Point(0,-windowNumber*rectList[resolution][tab][id]->height));break;
		default:return(Point(0,0));
	};
};

const Point UI_Theme::lookUpMaxRealDistance(const eWindow id, const int windowNumber) const // ~~ Name
{
	if(id==MAIN_WINDOW) return(Point(0,0));
	switch(arrangeDirection[resolution][tab][id])
	{
		case ARRANGE_LEFT_TO_RIGHT:return(Point(windowNumber*maxRectList[resolution][tab][id]->width, 0));break;
		case ARRANGE_RIGHT_TO_LEFT:return(Point(-windowNumber*maxRectList[resolution][tab][id]->width, 0));break;
		case ARRANGE_TOP_TO_DOWN:return(Point(0, windowNumber*maxRectList[resolution][tab][id]->height));break;
		case ARRANGE_DOWN_TO_TOP:return(Point(0, -windowNumber*maxRectList[resolution][tab][id]->height));break;
		default:return(Point(0,0));
	};
};

const Rect UI_Theme::lookUpRect(const eWindow id, const int windowNumber, const int maxPlayer) const
{
	Point p;
	switch(arrangeDirection[resolution][tab][id])
	{
        case ARRANGE_LEFT_TO_RIGHT:
        case ARRANGE_RIGHT_TO_LEFT:p=lookUpRealDistance(id, windowNumber) + Point(lookUpRealDistance(xwindow[resolution][tab][id], maxPlayer).x, 0);break;
        case ARRANGE_TOP_TO_DOWN:
        case ARRANGE_DOWN_TO_TOP:p=lookUpRealDistance(id, windowNumber) + Point(0,lookUpRealDistance(ywindow[resolution][tab][id], maxPlayer).y);break;
    };
	return(Rect(rectList[resolution][tab][id]->GetPosition()+p, rectList[resolution][tab][id]->GetSize()));
};

const Rect UI_Theme::lookUpMaxRect(const eWindow id, const int windowNumber, const int maxPlayer) const 
{
	Point p;
	switch(arrangeDirection[resolution][tab][id])
	{
        case ARRANGE_LEFT_TO_RIGHT:
        case ARRANGE_RIGHT_TO_LEFT:p=lookUpMaxRealDistance(id, windowNumber) + Point(lookUpMaxRealDistance(xwindow[resolution][tab][id], maxPlayer).x, 0);break;
        case ARRANGE_TOP_TO_DOWN:
        case ARRANGE_DOWN_TO_TOP:p=lookUpMaxRealDistance(id, windowNumber) + Point(0,lookUpMaxRealDistance(ywindow[resolution][tab][id], maxPlayer).y);break;
    };
	return(Rect(maxRectList[resolution][tab][id]->GetPosition()+p, maxRectList[resolution][tab][id]->GetSize()));
};

const eDataType getDataType(const string& item)
{
	if(item=="@STRINGS") return(STRING_DATA_TYPE);else
	if(item=="@FONTS") return(FONT_DATA_TYPE);else
	if(item=="@COLOURS") return(COLOUR_DATA_TYPE);else
	if(item=="@PENS") return(PEN_DATA_TYPE);else
	if(item=="@BRUSHES") return(BRUSH_DATA_TYPE);else
	if(item=="@BITMAPS") return(BITMAP_DATA_TYPE);else
	if(item=="@RECTANGLES") return(RECT_DATA_TYPE);else
	if(item=="@MAX_RECTANGLES") return(MAX_RECT_DATA_TYPE);else
	if(item=="@BUTTONS") return(BUTTON_DATA_TYPE);else
	return(ZERO_DATA_TYPE);
};

const eSubDataType getSubDataType(const eDataType mode)
{
	switch(mode)
	{	
		case STRING_DATA_TYPE:return(LANGUAGE_SUB_DATA_TYPE);
		case FONT_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case COLOUR_DATA_TYPE:return(COLOUR_THEME_SUB_DATA_TYPE);
		case BITMAP_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case PEN_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case BRUSH_DATA_TYPE:return(COLOUR_THEME_SUB_DATA_TYPE);
		case RECT_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		case MAX_RECT_DATA_TYPE:return(RESOLUTION_SUB_DATA_TYPE);
		default:return(ZERO_SUB_DATA_TYPE);
	};
};

const eSubSubDataType getSubSubDataType(const eDataType mode)
{
	switch(mode)
	{
		case FONT_DATA_TYPE:return(LANGUAGE_SUB_SUB_DATA_TYPE);
		case BITMAP_DATA_TYPE:return(COLOUR_THEME_SUB_SUB_DATA_TYPE);
		case PEN_DATA_TYPE:return(COLOUR_THEME_SUB_SUB_DATA_TYPE);
		case RECT_DATA_TYPE:return(TAB_SUB_SUB_DATA_TYPE);
		case MAX_RECT_DATA_TYPE:return(TAB_SUB_SUB_DATA_TYPE);
		default:return(ZERO_SUB_SUB_DATA_TYPE);
	};
};

const eLanguage getLanguageSubDataEntry(const string& item)
{
	if(item=="@ENGLISH") return(ENGLISH_LANGUAGE);else
	if(item=="@GERMAN") return(GERMAN_LANGUAGE);else
	if(item=="@FINNISH") return(FINNISH_LANGUAGE);else
	if(item=="@FRENCH") return(FRENCH_LANGUAGE);else
	if(item=="@SPANISH") return(SPANISH_LANGUAGE);else
	if(item=="@POLSKI") return(POLSKI_LANGUAGE);else
	if(item=="@KOREAN") return(KOREAN_LANGUAGE);else
	if(item=="@CHINESE") return(CHINESE_LANGUAGE);else
	if(item=="@RUSSKI") return(RUSSKI_LANGUAGE);else
	return(ZERO_LANGUAGE);
};

const eResolution getResolutionSubDataEntry(const string& item)
{
	if(item=="@320x200") return(RESOLUTION_320x200);else
	if(item=="@640x480") return(RESOLUTION_640x480);else
	if(item=="@800x600") return(RESOLUTION_800x600);else
	if(item=="@1024x768") return(RESOLUTION_1024x768);else
	if(item=="@1280x1024") return(RESOLUTION_1280x1024);else
	if(item=="@1600x1200") return(RESOLUTION_1600x1200);else
	return(ZERO_RESOLUTION);
};

const eTheme getThemeSubDataEntry(const string& item)
{
	if(item=="@DARK_BLUE_THEME") return(DARK_BLUE_THEME);else
	if(item=="@GREEN_THEME") return(GREEN_THEME);else
	if(item=="@RED_THEME") return(RED_THEME);else
	if(item=="@YELLOW_THEME") return(YELLOW_THEME);else
	if(item=="@GREY_THEME") return(GREY_THEME);else
	if(item=="@MONOCHROME_THEME") return(MONOCHROME_THEME);else
	return(ZERO_THEME);
};

const eTab getTabSubDataEntry(const string& item)
{
	if(item=="@BASIC_TAB") return(BASIC_TAB);else
	if(item=="@ADVANCED_TAB") return(ADVANCED_TAB);else
	if(item=="@EXPERT_TAB") return(EXPERT_TAB);else
	if(item=="@GOSU_TAB") return(GOSU_TAB);else
	if(item=="@TRANSCENDEND_TAB") return(TRANSCENDEND_TAB);else
	if(item=="@MAP_TAB") return(MAP_TAB);else
	if(item=="@SETTINGS_TAB") return(SETTINGS_TAB);else
	if(item=="@TUTORIAL_TAB") return(TUTORIAL_TAB);else
	return(ZERO_TAB);
};

const eBrushStyle get_brush_style(const string& item)
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


const ePenStyle get_pen_style(const string& item)
{
	if(item=="SOLID") return(SOLID_PEN_STYLE);else
	if(item=="DOT") return(DOT_PEN_STYLE);else
	if(item=="LONG_DASH") return(LONG_DASH_PEN_STYLE);else
	if(item=="SHORT_DASH") return(SHORT_DASH_PEN_STYLE);else
	if(item=="DOT_DASH") return(DOT_DASH_PEN_STYLE);else
	return(TRANSPARENT_PEN_STYLE);
};
/*
int get_font_style1(const string& item)
{
			if(item=="decorative") return(DECORATIVE);else
			if(item=="normal") return(DEFAULT);else
			return(DEFAULT);
// TODO!
};
int get_font_style2(const string& item)
{
			if(item=="italics") return(FONTSTYLE_ITALIC);else
			if(item=="normal") return(DEFAULT);else
			return(DEFAULT);
// TODO!
};
int get_font_style3(const string& item)
{
			if(item=="bold") return(BOLD);else
			if(item=="normal") return(DEFAULT);else
			return(DEFAULT);
// TODO!
};*/

eWindow parse_window(const string& item)
{
	if(item=="NULL window") return(NULL_WINDOW);else
	if(item=="Main window") return(MAIN_WINDOW);else
	if(item=="Message window") return(MESSAGE_WINDOW);else
	if(item=="The core window") return(THE_CORE_WINDOW);else
	if(item=="Tutorial window") return(TUTORIAL_WINDOW);else
	if(item=="Build order window") return(BUILD_ORDER_WINDOW);else
	if(item=="Force window") return(FORCE_WINDOW);else
	if(item=="Timer window") return(TIMER_WINDOW);else
	if(item=="Statistics window") return(STATISTICS_WINDOW);else
	if(item=="Build order diagram window") return(BO_DIAGRAM_WINDOW);else
	if(item=="Build order graph window") return(BO_GRAPH_WINDOW);else
	if(item=="Info window") return(INFO_WINDOW);else
	return(NULL_WINDOW);
};




eCommand parse_commands(const string& item)
{
	if(item=="absolute coordinates") return(ABSOLUTE_COORDINATES_COMMAND);else
	if(item=="absolute x coordinate") return(ABSOLUTE_X_COORDINATE_COMMAND);else
	if(item=="absolute y coordinate") return(ABSOLUTE_Y_COORDINATE_COMMAND);else
	if(item=="dock with left border of") return(DOCK_WITH_LEFT_BORDER_OF_COMMAND);else
	if(item=="dock with right border of") return(DOCK_WITH_RIGHT_BORDER_OF_COMMAND);else
	if(item=="dock with lower border of") return(DOCK_WITH_LOWER_BORDER_OF_COMMAND);else
	if(item=="dock with upper border of") return(DOCK_WITH_UPPER_BORDER_OF_COMMAND);else
	if(item=="dock center inside of") return(DOCK_CENTER_INSIDE_OF_COMMAND);else
	if(item=="dock left inside of") return(DOCK_LEFT_INSIDE_OF_COMMAND);else
	if(item=="dock right inside of") return(DOCK_RIGHT_INSIDE_OF_COMMAND);else
	if(item=="dock top inside of") return(DOCK_TOP_INSIDE_OF_COMMAND);else
	if(item=="dock bottom inside of") return(DOCK_BOTTOM_INSIDE_OF_COMMAND);else
	if(item=="calculate maxsize") return(CALCULATE_MAXSIZE_COMMAND);else
	if(item=="calculate maxwidth") return(CALCULATE_MAXWIDTH_COMMAND);else
	if(item=="calculate maxheight") return(CALCULATE_MAXHEIGHT_COMMAND);else
	if(item=="same as above") return(SAME_AS_ABOVE_COMMAND);else
	return(NO_COMMAND);
};

// Rueckgabewert: Richtung

eArrangeDirection parse_complete_command(const string* p, eCommand* e, Rect& rect)
{
	int x=-1;int y=-1;int dx=-1;int dy=-1;
	bool xpart=false; bool ypart=false; bool xypart=false; bool dxpart=false; bool dypart=false;
	bool xcomplete=false;bool ycomplete=false;
	bool window=false;

// additional windows: arrange left to right = 0, arrange right to left = 1, arrange top to down = 2, arrange down to top = 3 :)
	eArrangeDirection direction=ARRANGE_LEFT_TO_RIGHT;
	
	for(int i = 2; i <50;i++)
	{
		if(xcomplete&&ycomplete)
		{
			dxpart=true;
			xcomplete=false;
			ycomplete=false;
		}
	
		e[i]=NO_COMMAND;
		if(window)
		{
			e[i]=(eCommand)parse_window(p[i]);
			window=false;
		}
		else
		if(xpart)
		{
			x=atoi(p[i].c_str());
			xpart=false;
			if(xypart)
			{
				ypart=true;
				xypart=false;
			}
		} else
		if(ypart)
		{
			y=atoi(p[i].c_str());
			ypart=false;
		} else
		if(dxpart)
		{
			dx=atoi(p[i].c_str());
			dxpart=false;dypart=true;
		} else
		if(dypart)
		{
			dy=atoi(p[i].c_str());
			dypart=false;
		} else
		{
			eCommand k=parse_commands(p[i]);
			switch(k)
			{
				case ABSOLUTE_COORDINATES_COMMAND:xpart=true;xypart=true;xcomplete=true;ycomplete=true;break;
				case ABSOLUTE_X_COORDINATE_COMMAND:xpart=true;xcomplete=true;direction=ARRANGE_LEFT_TO_RIGHT;break;
				case ABSOLUTE_Y_COORDINATE_COMMAND:ypart=true;ycomplete=true;direction=ARRANGE_TOP_TO_DOWN;break;
				case DOCK_WITH_LEFT_BORDER_OF_COMMAND:e[i]=k;window=true;xcomplete=true;direction=ARRANGE_RIGHT_TO_LEFT;break;
				case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:e[i]=k;window=true;xcomplete=true;direction=ARRANGE_LEFT_TO_RIGHT;break;
				case DOCK_WITH_LOWER_BORDER_OF_COMMAND:e[i]=k;window=true;ycomplete=true;direction=ARRANGE_TOP_TO_DOWN;break;
				case DOCK_WITH_UPPER_BORDER_OF_COMMAND:e[i]=k;window=true;ycomplete=true;direction=ARRANGE_DOWN_TO_TOP;break;
				case DOCK_CENTER_INSIDE_OF_COMMAND:e[i]=k;window=true;dxpart=true;break;
				case DOCK_LEFT_INSIDE_OF_COMMAND:e[i]=k;window=true;xcomplete=true;direction=ARRANGE_LEFT_TO_RIGHT;break;
				case DOCK_RIGHT_INSIDE_OF_COMMAND:e[i]=k;window=true;xcomplete=true;direction=ARRANGE_RIGHT_TO_LEFT;break;
				case DOCK_TOP_INSIDE_OF_COMMAND:
												  {
													  e[i]=k;window=true;ycomplete=true;direction=ARRANGE_TOP_TO_DOWN;
												  };break;
				case DOCK_BOTTOM_INSIDE_OF_COMMAND:e[i]=k;window=true;ycomplete=true;direction=ARRANGE_DOWN_TO_TOP;break;
				default:e[i]=k;break;			
			};
			
		}
	};
	rect=Rect(x,y,dx,dy);
	return(direction);
};

void UI_Theme::loadDataFiles(const string& dataFile, const string& bitmapDir, const string& fontDir, DC* dc)
{
	const int MAX_PARAMETERS = 50;
	char line[1024], old[1024];
	char* buffer;
	string p[MAX_PARAMETERS];
	int v[MAX_PARAMETERS];
	int ln=0;

	eDataType mode=ZERO_DATA_TYPE;
	eSubDataType sub_mode=ZERO_SUB_DATA_TYPE;
	eSubSubDataType sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;

	eLanguage current_language=ZERO_LANGUAGE;
	eTheme current_theme=ZERO_THEME;
	eResolution current_resolution=ZERO_RESOLUTION;
	eTab current_tab=ZERO_TAB;

	eCommand trectList[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS][MAX_PARAMETERS];
	eCommand tmaxRectList[MAX_RESOLUTIONS][MAX_TABS][MAX_WINDOWS][MAX_PARAMETERS];

	for(int i = MAX_RESOLUTIONS;i--;)
		for(int j = MAX_TABS;j--;)
			for(int k = MAX_WINDOWS;k--;)
				for(int l = MAX_PARAMETERS;l--;)
				{
					trectList[i][j][k][l]=NO_COMMAND;
					tmaxRectList[i][j][k][l]=NO_COMMAND;
				}

	int current_line = 0;
	
 	for(int i=MAX_PARAMETERS;i--;)
	{
		p[i]="";
		v[i]=0;
	};

	ifstream pFile(dataFile.c_str());
	
	if(!pFile.is_open())
	{
#ifdef _SCC_DEBUG
		toLog("ERROR: (UI_Theme::loadDataFiles) Could not open file!");
#endif
		return;
	}
	
	while(pFile.getline(line,1024))
	{
		if(pFile.fail())
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadDataFiles) Long line!");
#endif
			pFile.clear(pFile.rdstate() & ~ios::failbit);
		};
			
		++ln;
		//line[strlen(line)-1]='\0';
		if((line[0]=='#')||(line[0]=='\0')||(line=="")) continue;
		for(int i=MAX_PARAMETERS;i--;)
		{
			p[i]="";
			v[i]=0;
		};
		char* line2=line;		
		while(((*line2)==32)||((*line2)==9))
			line2++;
		if((*line2)=='\0')
			continue;
		
		strcpy(old,line2);
		
		if((buffer=strtok(line2,",\0"))!=NULL)
			p[0]=buffer;
		int k=1;
		
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
			while(((*buffer)==32)||((*buffer)==9))
				buffer++;
			p[k]=buffer;
			k++;
		};
		if((buffer=strtok(NULL,",\0"))!=NULL)
		{
#ifdef _SCC_DEBUG
			toLog("WARNING: (UI_Theme::loadDataFiles) Too many parameters.");
#endif
			continue;
		}
		for(int j=0;j<MAX_PARAMETERS;j++)
			v[j]=atoi(p[j].c_str());
/*		if((value1<0)||(value2<0)||(value3<0)||(value4<0)||(value5<0))
		{
			debug.toLog(0,"WARNING: (UI_Theme::loadDataFiles) %s: Line %d [%s]: Value below zero.",dataFile.c_str(),ln,old);
			continue;
		}*/

		// mode	
		if(mode==ZERO_DATA_TYPE)
		{
			mode=getDataType(p[0]);
#ifdef _SCC_DEBUG
			if(mode==ZERO_DATA_TYPE)
			{
				if(p[0]=="@END")
					toLog("WARNING: (UI_Theme::loadDataFiles) Lonely @END.");
				else
					toLog("WARNING: (UI_Theme::loadDataFiles) Line is outside a block but is not marked as comment.");
			}
#endif				
			sub_mode=getSubDataType(mode);
			sub_sub_mode=getSubSubDataType(mode);
		}
		else if(mode!=ZERO_DATA_TYPE)
		{
			if(p[0]=="@END")
			{
				// @END of 1st sub area => return to begin of data type
				if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE)&&((current_language!=ZERO_LANGUAGE)||(current_resolution!=ZERO_RESOLUTION)||(current_theme!=ZERO_THEME)))
				{
					current_language=ZERO_LANGUAGE;
					current_resolution=ZERO_RESOLUTION;
					current_theme=ZERO_THEME;
					current_line=0;
				}
				// @END of 2nd sub area => return to begin of sub data type
				else 
				if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&((current_tab!=ZERO_TAB)||(current_language!=ZERO_LANGUAGE)||(current_theme!=ZERO_THEME)))
				{
					current_tab=ZERO_TAB;
					current_language=ZERO_LANGUAGE;
					current_theme=ZERO_THEME;
					current_line=0;
				}
				// @END  of 1st sub area (with an existing sub sub area...)
				else if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&(current_tab==ZERO_TAB)&&(current_language==ZERO_LANGUAGE)&&(current_theme==ZERO_THEME))
				{
					sub_mode=ZERO_SUB_DATA_TYPE;
					sub_sub_mode=ZERO_SUB_SUB_DATA_TYPE;
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
				};
			}
			else
			if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(current_language==ZERO_LANGUAGE)&&(current_resolution==ZERO_RESOLUTION)&&(current_theme==ZERO_THEME))
			{
				switch(sub_mode)
				{
					case LANGUAGE_SUB_DATA_TYPE:current_language=getLanguageSubDataEntry(line2);break;
					case RESOLUTION_SUB_DATA_TYPE:current_resolution=getResolutionSubDataEntry(line2);break;
					case COLOUR_THEME_SUB_DATA_TYPE:current_theme=getThemeSubDataEntry(line2);break;
					default:break;
				};
				current_line=0;
			}
			// => hat nur 1 Ebene => Position festgestellt!
			else if((sub_mode!=ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE))
			{
				switch(mode)
				{
					case STRING_DATA_TYPE:stringList[current_language][current_line]=new string(p[1]);break;
					case COLOUR_DATA_TYPE:colorList[current_theme][current_line]=new Color(dc->GetSurface(),v[1],v[2],v[3]);break;
					case BRUSH_DATA_TYPE:brushList[current_theme][current_line]=new Brush(dc->GetSurface(),v[1],v[2],v[3],get_brush_style(p[4]));break;
					default:break;
				};
				current_line++;
			}
			// 0 ebenen -> buttons :) BUTTON_DATA_TYPE?? TODO
			else if((sub_mode==ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE))
			{
				buttonAnimationList[current_line] = new ButtonAnimation;
				buttonAnimationList[current_line]->speed=v[1];
				buttonAnimationList[current_line]->type=(eButtonAnimationType)v[2];
				for(int i=0;i<MAX_BUTTON_ANIMATION_PHASES;i++)
				{
					buttonAnimationList[current_line]->startBrush[i]=(eBrush)(v[0*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->endBrush[i]=(eBrush)(v[1*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->startTextColor[i]=(eColor)(v[2*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->endTextColor[i]=(eColor)(v[3*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->startBorderPen[i]=(ePen)(v[4*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->endBorderPen[i]=(ePen)(v[5*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->bitmap[i]=(eBitmap)(v[6*MAX_BUTTON_ANIMATION_PHASES+3+i]);
//					buttonAnimationList[current_line]->text[i]=(eString)(v[7*MAX_BUTTON_ANIMATION_PHASES+3+i]);
				}
				current_line++;				
			}
			// => es gibt noch eine 2. Ebene
			else if((sub_sub_mode!=ZERO_SUB_SUB_DATA_TYPE)&&(current_language==ZERO_LANGUAGE)&&(current_tab==ZERO_TAB)&&(current_theme==ZERO_THEME))
			{
				switch(sub_sub_mode)
				{
					case LANGUAGE_SUB_SUB_DATA_TYPE:current_language=getLanguageSubDataEntry(line2);break;
					case COLOUR_THEME_SUB_SUB_DATA_TYPE:current_theme=getThemeSubDataEntry(line2);break;
					case TAB_SUB_SUB_DATA_TYPE:current_tab=getTabSubDataEntry(line2);break;
					default:break;
				};			
 				current_line=0;				
			}
			// => hat 2 Ebenen => Position festgestellt!
			else
			{
				switch(mode)
				{
					case FONT_DATA_TYPE:
						{
							string t=fontDir+"/"+p[1]+".ttf";
							fontList[current_resolution][current_language][current_line]=new Font(t, v[2]/*, get_font_style1(p[2]), get_font_style2(p[3]), get_font_style3(p[4]), false, _T(""), FONTENCODING_DEFAULT*/);
						};break;
					case BITMAP_DATA_TYPE:
						{
							string t=bitmapDir+"/"+p[1]+".bmp";
							bitmapList[current_resolution][current_theme][current_line]=new Bitmap(t);
						};break;
					case PEN_DATA_TYPE:penList[current_resolution][current_theme][current_line]=new Pen(dc->GetSurface(),v[2],v[3],v[4],v[1],get_pen_style(p[5]));break;
					case RECT_DATA_TYPE:
						{
							rectList[current_resolution][current_tab][parse_window(p[1])]=new Rect();
							arrangeDirection[current_resolution][current_tab][parse_window(p[1])]=parse_complete_command(p, &(trectList[current_resolution][current_tab][parse_window(p[1])][0]), *(rectList[current_resolution][current_tab][parse_window(p[1])]));
						};break;
					case MAX_RECT_DATA_TYPE:
						{
							maxRectList[current_resolution][current_tab][parse_window(p[1])]=new Rect();
							parse_complete_command(p, &(tmaxRectList[current_resolution][current_tab][parse_window(p[1])][0]), *(maxRectList[current_resolution][current_tab][parse_window(p[1])]));
						};break;					
					default:break;
				};
				current_line++;
			};
		}; // end if mode != ZERO_DATA_TYPE
	}; // end while

	int change=1;
	while(change)
	{	
		change=0;
		for(int i=0;i<MAX_RESOLUTIONS;i++)
			for(int j=0;j<MAX_TABS;j++)
				for(int k=0;k<MAX_WINDOWS;k++)
					if(rectList[i][j][k])
					{
					int oldx=rectList[i][j][k]->GetLeft();
					int oldy=rectList[i][j][k]->GetTop();
					for(int l=1;l<MAX_PARAMETERS;l++)
						switch(trectList[i][j][k][l])
						{
							case NO_COMMAND:break;
							case DOCK_WITH_LEFT_BORDER_OF_COMMAND:
							{
								rectList[i][j][k]->SetLeft(-5+rectList[i][j][trectList[i][j][k][l+1]]->GetLeft()-rectList[i][j][k]->GetWidth());
								xwindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:
							{
								rectList[i][j][k]->SetLeft(5+rectList[i][j][trectList[i][j][k][l+1]]->GetLeft()+rectList[i][j][trectList[i][j][k][l+1]]->GetWidth());
								xwindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							case DOCK_WITH_LOWER_BORDER_OF_COMMAND:
							{
								rectList[i][j][k]->SetTop(5+rectList[i][j][trectList[i][j][k][l+1]]->GetTop()+rectList[i][j][trectList[i][j][k][l+1]]->GetHeight());
	                            ywindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							case DOCK_WITH_UPPER_BORDER_OF_COMMAND:
							{
								rectList[i][j][k]->SetTop(-5+rectList[i][j][trectList[i][j][k][l+1]]->GetLeft()-rectList[i][j][k]->GetHeight());
	                            ywindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							case DOCK_CENTER_INSIDE_OF_COMMAND:
							{
								rectList[i][j][k]->SetLeft((rectList[i][j][trectList[i][j][k][l+1]]->GetLeft()+rectList[i][j][trectList[i][j][k][l+1]]->GetWidth()-rectList[i][j][k]->GetWidth())/2);
								rectList[i][j][k]->SetTop((rectList[i][j][trectList[i][j][k][l+1]]->GetTop()+rectList[i][j][trectList[i][j][k][l+1]]->GetHeight()-rectList[i][j][k]->GetHeight())/2);
	                            xwindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							case DOCK_LEFT_INSIDE_OF_COMMAND:
							{
								rectList[i][j][k]->SetLeft(10+rectList[i][j][trectList[i][j][k][l+1]]->GetLeft());
	                            xwindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
								
							case DOCK_RIGHT_INSIDE_OF_COMMAND:
							{
								rectList[i][j][k]->SetLeft(-10+rectList[i][j][trectList[i][j][k][l+1]]->GetLeft()+rectList[i][j][trectList[i][j][k][l+1]]->GetWidth()-rectList[i][j][k]->GetWidth());
	                            xwindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							case DOCK_TOP_INSIDE_OF_COMMAND:
							{
								rectList[i][j][k]->SetTop(10+rectList[i][j][trectList[i][j][k][l+1]]->GetTop());
	                            ywindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							case DOCK_BOTTOM_INSIDE_OF_COMMAND:
							{
								rectList[i][j][k]->SetTop(-30+rectList[i][j][trectList[i][j][k][l+1]]->GetTop()+rectList[i][j][trectList[i][j][k][l+1]]->GetHeight()-rectList[i][j][k]->GetHeight());
	                            ywindow[i][j][k]=(eWindow)trectList[i][j][k][l+1];
								l++;
							};break;
							default:
#ifdef _SCC_DEBUG								
								toLog("error... same as above oder so");
#endif				
								break;
						}
					if((rectList[i][j][k]->GetLeft()!=oldx)||(rectList[i][j][k]->GetTop()!=oldy))
					{
						change=1;
					}
				}
}
change=1;
while(change)
{
	change=0;
	for(int i=0;i<MAX_RESOLUTIONS;i++)
		for(int j=0;j<MAX_TABS;j++)
			for(int k=0;k<MAX_WINDOWS;k++)
				if(maxRectList[i][j][k])
				{
					int oldx=maxRectList[i][j][k]->GetLeft();
					int oldy=maxRectList[i][j][k]->GetTop();
					for(int l=1;l<MAX_PARAMETERS;l++)
						switch(tmaxRectList[i][j][k][l])
						{
							case NO_COMMAND:break;
							case DOCK_WITH_LEFT_BORDER_OF_COMMAND:
								maxRectList[i][j][k]->SetLeft(5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetLeft()-maxRectList[i][j][k]->GetWidth());l++;break;
							case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:
								maxRectList[i][j][k]->SetLeft(-5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetLeft()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetWidth());l++;break;
							case DOCK_WITH_LOWER_BORDER_OF_COMMAND:
								maxRectList[i][j][k]->SetTop(5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetTop()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetHeight());l++;break;
							case DOCK_WITH_UPPER_BORDER_OF_COMMAND:
								maxRectList[i][j][k]->SetTop(-5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetLeft()-maxRectList[i][j][k]->GetHeight());l++;break;
							case DOCK_CENTER_INSIDE_OF_COMMAND:								 
								maxRectList[i][j][k]->SetLeft(maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetLeft()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetWidth()-maxRectList[i][j][k]->GetWidth());
								maxRectList[i][j][k]->SetTop(maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetTop()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetHeight()-maxRectList[i][j][k]->GetHeight());l++;break;
							case DOCK_LEFT_INSIDE_OF_COMMAND:
								maxRectList[i][j][k]->SetLeft(10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetLeft());l++;break;
							case DOCK_RIGHT_INSIDE_OF_COMMAND:
								maxRectList[i][j][k]->SetLeft(-10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetLeft()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetWidth()-maxRectList[i][j][k]->GetWidth());l++;break;
							case DOCK_TOP_INSIDE_OF_COMMAND:;
								maxRectList[i][j][k]->SetTop(10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetTop());l++;break;
							case DOCK_BOTTOM_INSIDE_OF_COMMAND:
								maxRectList[i][j][k]->SetTop(-10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetTop()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetHeight()-maxRectList[i][j][k]->GetHeight());l++;break;
							case CALCULATE_MAXHEIGHT_COMMAND:maxRectList[i][j][k]->SetPosition(rectList[i][j][k]->GetPosition());
															 maxRectList[i][j][k]->SetSize(Size(rectList[i][j][k]->GetWidth(), rectList[i][j][MAIN_WINDOW]->GetHeight() + rectList[i][j][MAIN_WINDOW]->GetTop() - rectList[i][j][k]->GetTop() - 30));break;
															 // main window mal hernehmen... TODO andere Fenster miteinberechnen!!
							
							case CALCULATE_MAXSIZE_COMMAND:
							case CALCULATE_MAXWIDTH_COMMAND: //TODO
							case SAME_AS_ABOVE_COMMAND:
								{
									maxRectList[i][j][k]->SetPosition(rectList[i][j][k]->GetPosition());
									maxRectList[i][j][k]->SetSize(rectList[i][j][k]->GetSize());
								};break;
							default:
#ifdef _SCC_DEBUG								
								toLog("max error... same as above oder so");
#endif
								break;
						}
					if((maxRectList[i][j][k]->GetLeft()!=oldx)||(maxRectList[i][j][k]->GetTop()!=oldy))
						change=1;
				}
	}


};


