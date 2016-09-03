#include "UI_Theme.h"
#include "../scc2dll/debug.h"

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
                colourList[i][j]=0;
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
			}
	for(int i=MAX_BUTTONS;i--;)
		buttonAnimationList[i]=0;
	
	tab=BASIC_TAB;
	resolution=RESOLUTION_1280x1024;
	language=GERMAN_LANGUAGE;
	colourTheme=DARK_BLUE_THEME;
};

UI_Theme::~UI_Theme()
{
 	for(int i = MAX_LANGUAGES;i--;)
 		for(int j = MAX_STRINGS;j--;)
			if(stringList[i][j])
			{
				delete stringList[i][j];
				stringList[i][j]=0;
			}
	for(int i = MAX_RESOLUTIONS;i--;)
		for(int j = MAX_LANGUAGES;j--;)
			for(int k = MAX_FONTS;k--;)
					if(fontList[i][j][k])
					{
						delete fontList[i][j][k];
						fontList[i][j][k]=0;
					}
	for(int i = MAX_COLOUR_THEMES;i--;)
	{
		for(int j = MAX_COLOURS;j--;)
			if(colourList[i][j])
			{
				delete colourList[i][j];
				colourList[i][j]=0;
			}
		for(int j = MAX_RESOLUTIONS;j--;)
		{
			for(int k = MAX_BITMAPS;k--;)
				if(bitmapList[i][j][k])
				{
					delete bitmapList[i][j][k];
					bitmapList[i][j][k]=0;
				}
	 		for(int k = MAX_PENS;k--;)
				if(penList[i][j][k])
				{
// TODO SPEICHERLECK: mmmh, anscheinend stuerzt der beim color-deleten von penlist ab... managt wxPen etwa die pen-colours zusammen mit den normalen colours 
					delete penList[i][j][k];
					penList[i][j][k]=0;
				}
		}
		
		for(int j = MAX_BRUSHES;j--;)
			if(brushList[i][j])
			{
				delete brushList[i][j];
				brushList[i][j]=0;
			}
	}
 	for(int i = MAX_RESOLUTIONS;i--;)
		for(int j = MAX_TABS;j--;)
			for(int k = MAX_WINDOWS;k--;)
			{
				if(rectList[i][j][k])
				{
					delete rectList[i][j][k];
					rectList[i][j][k]=0;
				}
				if(maxRectList[i][j][k])
				{
					delete maxRectList[i][j][k];
					maxRectList[i][j][k]=0;
				}
			}	
	for(int i=MAX_BUTTONS;i--;)
		if(buttonAnimationList[i])
		{
			delete buttonAnimationList[i];
			buttonAnimationList[i]=0;
		};
};

void UI_Theme::setColourTheme(eTheme colourTheme)
{
	this->colourTheme=colourTheme;
};

eTheme UI_Theme::getColourTheme()
{
	return(colourTheme);
};

eLanguage UI_Theme::getLanguage()
{
	return(language);
};

void UI_Theme::setLanguage(eLanguage language)
{
	this->language=language;
};

eResolution UI_Theme::getResolution()
{
	return(resolution);
};

void UI_Theme::setResolution(eResolution resolution)
{
	this->resolution=resolution;
};

void UI_Theme::setTab(eTab tab)
{
	this->tab=tab;
};

eTab UI_Theme::getTab()
{
	return(tab);
};


ButtonAnimation* UI_Theme::lookUpButtonAnimation(eButton id)
{
	return(buttonAnimationList[id]);
};


wxString* UI_Theme::lookUpString(eString id)
{
	return(stringList[language][id]);
};


wxString UI_Theme::lookUpFormattedString(eString id, ...)
{
	va_list args;
	wxString bla=*(stringList[language][id]);
	va_start(args, id);
	bla = wxString::FormatV(bla, args);
	va_end(args);
	return(bla);
};


wxColour* UI_Theme::lookUpColour(eColour id)
{
	return(colourList[colourTheme][id]);
};

wxBitmap* UI_Theme::lookUpBitmap(eBitmap id)
{
	return(bitmapList[resolution][colourTheme][id]);
};

wxPen* UI_Theme::lookUpPen(ePen id)
{
	return(penList[resolution][colourTheme][id]);
};

wxBrush* UI_Theme::lookUpBrush(eBrush id)
{
	return(brushList[colourTheme][id]);
};

wxFont* UI_Theme::lookUpFont(eFont id)
{
	return(fontList[resolution][language][id]);
};
                                                                               
wxRect* UI_Theme::lookUpRect(eWindow id)
{
	return(rectList[resolution][tab][id]);
};

wxRect* UI_Theme::lookUpMaxRect(eWindow id)
{
	return(maxRectList[resolution][tab][id]);
};

/*

parser:
1. datei lesen
langes array mit max 10 parametern fuellen
und max tiefe 2
pointer zurueckgeben
   */

eDataType getDataType(const char* item)
{
            if(!strcmp(item,"@STRINGS")) return(STRING_DATA_TYPE);else
            if(!strcmp(item,"@FONTS")) return(FONT_DATA_TYPE);else
            if(!strcmp(item,"@COLOURS")) return(COLOUR_DATA_TYPE);else
            if(!strcmp(item,"@PENS")) return(PEN_DATA_TYPE);else
            if(!strcmp(item,"@BRUSHES")) return(BRUSH_DATA_TYPE);else
            if(!strcmp(item,"@BITMAPS")) return(BITMAP_DATA_TYPE);else
            if(!strcmp(item,"@RECTANGLES")) return(RECT_DATA_TYPE);else
            if(!strcmp(item,"@MAX_RECTANGLES")) return(MAX_RECT_DATA_TYPE);else
            if(!strcmp(item,"@BUTTONS")) return(BUTTON_DATA_TYPE);else
			return(ZERO_DATA_TYPE);
};

eSubDataType getSubDataType(const eDataType mode)
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

eSubSubDataType getSubSubDataType(const eDataType mode)
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

eLanguage getLanguageSubDataEntry(const char* item)
{
            if(!strcmp(item,"@ENGLISH")) return(ENGLISH_LANGUAGE);else
            if(!strcmp(item,"@GERMAN")) return(GERMAN_LANGUAGE);else
            if(!strcmp(item,"@FINNISH")) return(FINNISH_LANGUAGE);else
            if(!strcmp(item,"@FRENCH")) return(FRENCH_LANGUAGE);else
            if(!strcmp(item,"@SPANISH")) return(SPANISH_LANGUAGE);else
            if(!strcmp(item,"@POLSKI")) return(POLSKI_LANGUAGE);else
            if(!strcmp(item,"@KOREAN")) return(KOREAN_LANGUAGE);else
            if(!strcmp(item,"@CHINESE")) return(CHINESE_LANGUAGE);else
            if(!strcmp(item,"@RUSSKI")) return(RUSSKI_LANGUAGE);else
            return(ZERO_LANGUAGE);
};

eResolution getResolutionSubDataEntry(const char* item)
{
            if(!strcmp(item,"@320x200")) return(RESOLUTION_320x200);else
            if(!strcmp(item,"@640x480")) return(RESOLUTION_640x480);else
            if(!strcmp(item,"@800x600")) return(RESOLUTION_800x600);else
            if(!strcmp(item,"@1024x768")) return(RESOLUTION_1024x768);else
            if(!strcmp(item,"@1280x1024")) return(RESOLUTION_1280x1024);else
            if(!strcmp(item,"@1600x1200")) return(RESOLUTION_1600x1200);else
            return(ZERO_RESOLUTION);
};

eTheme getThemeSubDataEntry(const char* item)
{
            if(!strcmp(item,"@DARK_BLUE_THEME")) return(DARK_BLUE_THEME);else
            if(!strcmp(item,"@GREEN_THEME")) return(GREEN_THEME);else
            if(!strcmp(item,"@RED_THEME")) return(RED_THEME);else
            if(!strcmp(item,"@YELLOW_THEME")) return(YELLOW_THEME);else
            if(!strcmp(item,"@GREY_THEME")) return(GREY_THEME);else
            if(!strcmp(item,"@MONOCHROME_THEME")) return(MONOCHROME_THEME);else
            return(ZERO_THEME);
};

eTab getTabSubDataEntry(const char* item)
{
            if(!strcmp(item,"@BASIC_TAB")) return(BASIC_TAB);else
            if(!strcmp(item,"@ADVANCED_TAB")) return(ADVANCED_TAB);else
            if(!strcmp(item,"@EXPERT_TAB")) return(EXPERT_TAB);else
            if(!strcmp(item,"@GOSU_TAB")) return(GOSU_TAB);else
            if(!strcmp(item,"@TRANSCENDEND_TAB")) return(TRANSCENDEND_TAB);else
            if(!strcmp(item,"@MAP_TAB")) return(MAP_TAB);else
            if(!strcmp(item,"@SETTINGS_TAB")) return(SETTINGS_TAB);else
            if(!strcmp(item,"@TUTORIAL_TAB")) return(TUTORIAL_TAB);else
            return(ZERO_TAB);
};

int get_brush_style(const char* item)
{
            if(!strcmp(item,"TRANSPARENT")) return(wxTRANSPARENT);else
            if(!strcmp(item,"SOLID")) return(wxSOLID);else
            if(!strcmp(item,"BDIAGONAL_HATCH")) return(wxBDIAGONAL_HATCH);else
            if(!strcmp(item,"CROSSDIAG_HATCH")) return(wxCROSSDIAG_HATCH);else
            if(!strcmp(item,"FDIAGONAL_HATCH")) return(wxFDIAGONAL_HATCH);else
            if(!strcmp(item,"CROSSH_ATCH")) return(wxCROSS_HATCH);else
            if(!strcmp(item,"HORIZONTAL_HATCH")) return(wxHORIZONTAL_HATCH);else
            if(!strcmp(item,"VERTICAL_HATCH")) return(wxVERTICAL_HATCH);else
            if(!strcmp(item,"STIPPLE")) return(wxSTIPPLE);else
            if(!strcmp(item,"STIPPLE_MASK_OPAQUE")) return(wxSTIPPLE_MASK_OPAQUE);else
			return(wxTRANSPARENT);
}


int get_pen_style(const char* item)
{
            if(!strcmp(item,"TRANSPARENT")) return(wxTRANSPARENT);else
            if(!strcmp(item,"SOLID")) return(wxSOLID);else
            if(!strcmp(item,"DOT")) return(wxDOT);else
            if(!strcmp(item,"LONG_DASH")) return(wxLONG_DASH);else
            if(!strcmp(item,"SHORT_DASH")) return(wxSHORT_DASH);else
            if(!strcmp(item,"DOT_DASH")) return(wxDOT_DASH);else
            if(!strcmp(item,"USER_DASH")) return(wxUSER_DASH);else
            if(!strcmp(item,"BDIAGONAL_HATCH")) return(wxBDIAGONAL_HATCH);else
            if(!strcmp(item,"CROSSDIAG_HATCH")) return(wxCROSSDIAG_HATCH);else
            if(!strcmp(item,"FDIAGONAL_HATCH")) return(wxFDIAGONAL_HATCH);else
            if(!strcmp(item,"CROSSH_ATCH")) return(wxCROSS_HATCH);else
            if(!strcmp(item,"HORIZONTAL_HATCH")) return(wxHORIZONTAL_HATCH);else
            if(!strcmp(item,"VERTICAL_HATCH")) return(wxVERTICAL_HATCH);else
            if(!strcmp(item,"STIPPLE")) return(wxSTIPPLE);else
			return(wxTRANSPARENT);
};

int get_font_style1(const char* item)
{
            if(!strcmp(item,"decorative")) return(wxDECORATIVE);else
            if(!strcmp(item,"normal")) return(wxDEFAULT);else
            return(wxDEFAULT);
// TODO!
};
int get_font_style2(const char* item)
{
            if(!strcmp(item,"italics")) return(wxFONTSTYLE_ITALIC);else
            if(!strcmp(item,"normal")) return(wxDEFAULT);else
            return(wxDEFAULT);
// TODO!
};
int get_font_style3(const char* item)
{
            if(!strcmp(item,"bold")) return(wxBOLD);else
            if(!strcmp(item,"normal")) return(wxDEFAULT);else
            return(wxDEFAULT);
// TODO!
};

eWindow parse_window(const char* item)
{
            if(!strcmp(item,"NULL window")) return(NULL_WINDOW);else
            if(!strcmp(item,"Main window")) return(MAIN_WINDOW);else
            if(!strcmp(item,"Message window")) return(MESSAGE_WINDOW);else
            if(!strcmp(item,"The core window")) return(THE_CORE_WINDOW);else
            if(!strcmp(item,"Tutorial window")) return(TUTORIAL_WINDOW);else
            if(!strcmp(item,"Build order window")) return(BUILD_ORDER_WINDOW);else
            if(!strcmp(item,"Force window")) return(FORCE_WINDOW);else
            if(!strcmp(item,"Timer window")) return(TIMER_WINDOW);else
            if(!strcmp(item,"Statistics window")) return(STATISTICS_WINDOW);else
            if(!strcmp(item,"Build order diagram window")) return(BO_DIAGRAM_WINDOW);else
            if(!strcmp(item,"Build order graph window")) return(BO_GRAPH_WINDOW);else
            if(!strcmp(item,"Info window")) return(INFO_WINDOW);else
            return(NULL_WINDOW);
};




eCommand parse_commands(const char* item)
{
	if(!strcmp(item,"absolute coordinates")) return(ABSOLUTE_COORDINATES_COMMAND);else
            if(!strcmp(item,"absolute x coordinate"))  return(ABSOLUTE_X_COORDINATE_COMMAND);else
            if(!strcmp(item,"absolute y coordinate")) return(ABSOLUTE_Y_COORDINATE_COMMAND);else
            if(!strcmp(item,"dock with left border of")) return(DOCK_WITH_LEFT_BORDER_OF_COMMAND);else
            if(!strcmp(item,"dock with right border of")) return(DOCK_WITH_RIGHT_BORDER_OF_COMMAND);else
            if(!strcmp(item,"dock with lower border of")) return(DOCK_WITH_LOWER_BORDER_OF_COMMAND);else
            if(!strcmp(item,"dock with upper border of")) return(DOCK_WITH_UPPER_BORDER_OF_COMMAND);else
            if(!strcmp(item,"dock center inside of")) return(DOCK_CENTER_INSIDE_OF_COMMAND);else
            if(!strcmp(item,"dock left inside of")) return(DOCK_LEFT_INSIDE_OF_COMMAND);else
            if(!strcmp(item,"dock right inside of")) return(DOCK_RIGHT_INSIDE_OF_COMMAND);else
            if(!strcmp(item,"dock top inside of")) return(DOCK_TOP_INSIDE_OF_COMMAND);else
            if(!strcmp(item,"dock bottom inside of")) return(DOCK_BOTTOM_INSIDE_OF_COMMAND);else
            if(!strcmp(item,"calculate maxsize")) return(CALCULATE_MAXSIZE_COMMAND);else
            if(!strcmp(item,"calculate maxwidth")) return(CALCULATE_MAXWIDTH_COMMAND);else
            if(!strcmp(item,"calculate maxheight")) return(CALCULATE_MAXHEIGHT_COMMAND);else
            if(!strcmp(item,"same as above")) return(SAME_AS_ABOVE_COMMAND);else
            return(NO_COMMAND);
};

void parse_complete_command(const char p[50][1024], eCommand* e, wxRect& rect)
{
	int x=-1;int y=-1;int dx=-1;int dy=-1;
	bool xpart=false; bool ypart=false; bool xypart=false; bool dxpart=false; bool dypart=false;
	bool window=false;
	
	for(int i = 2; i <50;i++)
	{
		e[i]=NO_COMMAND;
		if(window)
		{
			e[i]=(eCommand)parse_window(p[i]);
			window=false;
		}
		else
		if(xpart)
		{
			x=atoi(p[i]);
			xpart=false;
			if(xypart)
			{
				ypart=true;
				xypart=false;
			}
		} else
		if(ypart)
		{
			y=atoi(p[i]);
			ypart=false;
			dxpart=true;
		} else
		if(dxpart)
		{
			dx=atoi(p[i]);
			dxpart=false;dypart=true;
		} else
		if(dypart)
		{
			dy=atoi(p[i]);
			dypart=false;
		} else
		{
			eCommand k=parse_commands(p[i]);
			switch(k)
			{
				case ABSOLUTE_COORDINATES_COMMAND:xpart=true;xypart=true;break;
            	case ABSOLUTE_X_COORDINATE_COMMAND:xpart=true;break;
				case ABSOLUTE_Y_COORDINATE_COMMAND:ypart=true;break;
				case DOCK_WITH_LEFT_BORDER_OF_COMMAND:e[i]=k;window=true;break;
				case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:e[i]=k;window=true;break;
				case DOCK_WITH_LOWER_BORDER_OF_COMMAND:e[i]=k;window=true;dxpart=true;break;
				case DOCK_WITH_UPPER_BORDER_OF_COMMAND:e[i]=k;window=true;dxpart=true;break;
				case DOCK_CENTER_INSIDE_OF_COMMAND:e[i]=k;window=true;dxpart=true;break;
				case DOCK_LEFT_INSIDE_OF_COMMAND:e[i]=k;window=true;break;
				case DOCK_RIGHT_INSIDE_OF_COMMAND:e[i]=k;window=true;break;
				case DOCK_TOP_INSIDE_OF_COMMAND:e[i]=k;window=true;dxpart=true;break;
				case DOCK_BOTTOM_INSIDE_OF_COMMAND:e[i]=k;window=true;dxpart=true;break;
				default:e[i]=k;break;			
			};
			
		}
	};
	rect=wxRect(x,y,dx,dy);
};

void UI_Theme::loadDataFiles()
{
	const int MAX_PARAMETERS = 50;
	char line[1024],old[1024];
    FILE* pFile;
    char* buffer;
    char p[MAX_PARAMETERS][1024];
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
	
	char dataFile[12];
	
	sprintf(dataFile, "data.txt"); //TODO

 	for(int i=MAX_PARAMETERS;i--;)
	{
		strcpy(p[i],"");
		v[i]=0;
	};
                                                                                                                                                           
    if((pFile = fopen (dataFile,"r"))==NULL)
    {
        debug.toLog(0,"ERROR: (UI_Theme::loadDataFiles) %s: Could not open file!",dataFile);
        return;
    }
                                                                                                                                                            
    while(fgets(line,sizeof(line),pFile)!=NULL)
    {
        ++ln;
        line[strlen(line)-1]='\0';
        if((line[0]=='#')||(line[0]=='\0')||(line=="")) continue;
		for(int i=MAX_PARAMETERS;i--;)
		{
			strcpy(p[i],"");
			v[i]=0;
		};
		char* line2=line;		
		while(((*line2)==32)||((*line2)==9))
			line2++;
		if((*line2)=='\0')
			continue;
		
        strcpy(old,line2);
		
        if((buffer=strtok(line2,",\0"))!=NULL) 
			strcpy(p[0],buffer);
		int k=1;
		
		while(((buffer=strtok(NULL,",\0"))!=NULL)&&(k<MAX_PARAMETERS))
		{
	        while(((*buffer)==32)||((*buffer)==9))
    	        buffer++;
			strcpy(p[k],buffer);
			k++;
		};
        if((buffer=strtok(NULL,",\0"))!=NULL)
        {
            debug.toLog(0,"WARNING: (UI_Theme::loadDataFiles) %s: Line %d [%s]: Too many parameters.",dataFile,ln,old);
            continue;
        }
		for(int j=0;j<MAX_PARAMETERS;j++)
			v[j]=atoi(p[j]);
                                                                                                                                                            
/*        if((value1<0)||(value2<0)||(value3<0)||(value4<0)||(value5<0))
        {
            debug.toLog(0,"WARNING: (UI_Theme::loadDataFiles) %s: Line %d [%s]: Value below zero.",dataFile,ln,old);
            continue;
        }*/

		// mode	
        if(mode==ZERO_DATA_TYPE)
        {
			mode=getDataType(p[0]);
			if(mode==ZERO_DATA_TYPE)
            {
                if(!strcmp(p[0],"@END"))
                    debug.toLog(0,"WARNING: (UI_Theme::loadDataFiles) %s: Line %d [%s]: Lonely @END.",dataFile,ln,old);
                else
                    debug.toLog(0,"WARNING: (UI_Theme::loadDataFiles) %s: Line %d [%s]: Line is outside a block but is not marked as comment.",dataFile,ln,old);
            }
			sub_mode=getSubDataType(mode);
			sub_sub_mode=getSubSubDataType(mode);
                                                                                                                                                            
        }
        else if(mode!=ZERO_DATA_TYPE)
		{
            if(!strcmp(p[0],"@END"))
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
			        case STRING_DATA_TYPE:stringList[current_language][current_line]=new wxString(p[1]);break;
		    	    case COLOUR_DATA_TYPE:colourList[current_theme][current_line]=new wxColour(v[1],v[2],v[3]);break;
		        	case BRUSH_DATA_TYPE:brushList[current_theme][current_line]=new wxBrush(wxColour(v[1],v[2],v[3]),get_brush_style(p[4]));break;
					default:break;
				};
				current_line++;
			}
			// 0 ebenen -> buttons :)
			else if((sub_mode==ZERO_SUB_DATA_TYPE)&&(sub_sub_mode==ZERO_SUB_SUB_DATA_TYPE))
			{
				buttonAnimationList[current_line] = new ButtonAnimation;
				buttonAnimationList[current_line]->speed=v[1];
				buttonAnimationList[current_line]->type=(eButtonAnimationType)v[2];
				for(int i=0;i<MAX_BUTTON_ANIMATION_PHASES;i++)
				{
					buttonAnimationList[current_line]->startBrush[i]=(eBrush)(v[0*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->endBrush[i]=(eBrush)(v[1*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->startTextColour[i]=(eColour)(v[2*MAX_BUTTON_ANIMATION_PHASES+3+i]);
					buttonAnimationList[current_line]->endTextColour[i]=(eColour)(v[3*MAX_BUTTON_ANIMATION_PHASES+3+i]);
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
							fontList[current_resolution][current_language][current_line]=new 
								wxFont(v[1], get_font_style1(p[2]), get_font_style2(p[3]), get_font_style3(p[4]), false, _T(""), wxFONTENCODING_DEFAULT);
						};break;
			        case BITMAP_DATA_TYPE:
						{
							bitmapList[current_resolution][current_theme][current_line]=new wxBitmap();
							char t[30];
							sprintf(t,"bitmaps/%s.bmp",p[1]);
							bitmapList[current_resolution][current_theme][current_line]->LoadFile(t,wxBITMAP_TYPE_BMP);
						};break;
			        case PEN_DATA_TYPE:penList[current_resolution][current_theme][current_line]=new wxPen(wxColour(v[2],v[3],v[4]),v[1],get_pen_style(p[5]));break;
					case RECT_DATA_TYPE:
						{
							rectList[current_resolution][current_tab][parse_window(p[1])]=new wxRect();
							parse_complete_command(p, &(trectList[current_resolution][current_tab][parse_window(p[1])][0]), *(rectList[current_resolution][current_tab][parse_window(p[1])]));
						};break;
			        case MAX_RECT_DATA_TYPE:
						{
							maxRectList[current_resolution][current_tab][parse_window(p[1])]=new wxRect();
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
					int oldx=rectList[i][j][k]->GetX();
					int oldy=rectList[i][j][k]->GetY();
					for(int l=1;l<MAX_PARAMETERS;l++)
						switch(trectList[i][j][k][l])
						{
							case NO_COMMAND:break;
				            case DOCK_WITH_LEFT_BORDER_OF_COMMAND:
								rectList[i][j][k]->SetX(-5+rectList[i][j][trectList[i][j][k][l+1]]->GetX()-rectList[i][j][k]->GetWidth());l++;break;
							case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:
								rectList[i][j][k]->SetX(5+rectList[i][j][trectList[i][j][k][l+1]]->GetX()+rectList[i][j][trectList[i][j][k][l+1]]->GetWidth());l++;break;
							case DOCK_WITH_LOWER_BORDER_OF_COMMAND:
                                rectList[i][j][k]->SetY(5+rectList[i][j][trectList[i][j][k][l+1]]->GetY()+rectList[i][j][trectList[i][j][k][l+1]]->GetHeight());l++;break;
				            case DOCK_WITH_UPPER_BORDER_OF_COMMAND:
								rectList[i][j][k]->SetY(-5+rectList[i][j][trectList[i][j][k][l+1]]->GetX()-rectList[i][j][k]->GetHeight());l++;break;
								
				            case DOCK_CENTER_INSIDE_OF_COMMAND:
								rectList[i][j][k]->SetX((rectList[i][j][trectList[i][j][k][l+1]]->GetX()+rectList[i][j][trectList[i][j][k][l+1]]->GetWidth()-rectList[i][j][k]->GetWidth())/2);
								rectList[i][j][k]->SetY((rectList[i][j][trectList[i][j][k][l+1]]->GetY()+rectList[i][j][trectList[i][j][k][l+1]]->GetHeight()-rectList[i][j][k]->GetHeight())/2);l++;break;
				            case DOCK_LEFT_INSIDE_OF_COMMAND:
								rectList[i][j][k]->SetX(10+rectList[i][j][trectList[i][j][k][l+1]]->GetX());l++;break;
				            case DOCK_RIGHT_INSIDE_OF_COMMAND:
								rectList[i][j][k]->SetX(-10+rectList[i][j][trectList[i][j][k][l+1]]->GetX()+rectList[i][j][trectList[i][j][k][l+1]]->GetWidth()-rectList[i][j][k]->GetWidth());l++;break;
				            case DOCK_TOP_INSIDE_OF_COMMAND:;
								rectList[i][j][k]->SetY(10+rectList[i][j][trectList[i][j][k][l+1]]->GetY());l++;break;
				            case DOCK_BOTTOM_INSIDE_OF_COMMAND:
								rectList[i][j][k]->SetY(-10+rectList[i][j][trectList[i][j][k][l+1]]->GetY()+rectList[i][j][trectList[i][j][k][l+1]]->GetHeight()-rectList[i][j][k]->GetHeight());l++;break;
							default:debug.toLog(0,"error... same as above oder so");break; // TODO ERROR
						}
					if((rectList[i][j][k]->GetX()!=oldx)||(rectList[i][j][k]->GetY()!=oldy))
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
                    int oldx=maxRectList[i][j][k]->GetX();
                    int oldy=maxRectList[i][j][k]->GetY();
                    for(int l=1;l<MAX_PARAMETERS;l++)
                        switch(tmaxRectList[i][j][k][l])
                        {
                            case NO_COMMAND:break;
                            case DOCK_WITH_LEFT_BORDER_OF_COMMAND:
                                maxRectList[i][j][k]->SetX(5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetX()-maxRectList[i][j][k]->GetWidth());l++;break;
                            case DOCK_WITH_RIGHT_BORDER_OF_COMMAND:
                                maxRectList[i][j][k]->SetX(-5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetX()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetWidth());l++;break;
                            case DOCK_WITH_LOWER_BORDER_OF_COMMAND:
                                maxRectList[i][j][k]->SetY(5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetY()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetHeight());l++;break;
                            case DOCK_WITH_UPPER_BORDER_OF_COMMAND:
                                maxRectList[i][j][k]->SetY(-5+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetX()-maxRectList[i][j][k]->GetHeight());l++;break;
                                                                                                                                                            
                            case DOCK_CENTER_INSIDE_OF_COMMAND:                                 
								maxRectList[i][j][k]->SetX(maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetX()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetWidth()-maxRectList[i][j][k]->GetWidth());
                                maxRectList[i][j][k]->SetY(maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetY()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetHeight()-maxRectList[i][j][k]->GetHeight());l++;break;
                            case DOCK_LEFT_INSIDE_OF_COMMAND:
                                maxRectList[i][j][k]->SetX(10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetX());l++;break;
                            case DOCK_RIGHT_INSIDE_OF_COMMAND:
                                maxRectList[i][j][k]->SetX(-10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetX()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetWidth()-maxRectList[i][j][k]->GetWidth());l++;break;
                            case DOCK_TOP_INSIDE_OF_COMMAND:;
                                maxRectList[i][j][k]->SetY(10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetY());l++;break;
                            case DOCK_BOTTOM_INSIDE_OF_COMMAND:
                                maxRectList[i][j][k]->SetY(-10+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetY()+maxRectList[i][j][tmaxRectList[i][j][k][l+1]]->GetHeight()-maxRectList[i][j][k]->GetHeight());l++;break;
							case CALCULATE_MAXHEIGHT_COMMAND:maxRectList[i][j][k]->SetPosition(rectList[i][j][k]->GetPosition());
															 maxRectList[i][j][k]->SetSize(wxSize(rectList[i][j][k]->GetWidth(), rectList[i][j][MAIN_WINDOW]->GetHeight() + rectList[i][j][MAIN_WINDOW]->GetY() - rectList[i][j][k]->GetY()));break;
															 // main window mal hernehmen... TODO andere Fenster miteinberechnen!!
							
							case CALCULATE_MAXSIZE_COMMAND:
							case CALCULATE_MAXWIDTH_COMMAND: //TODO
							case SAME_AS_ABOVE_COMMAND:
								{
									maxRectList[i][j][k]->SetPosition(rectList[i][j][k]->GetPosition());
									maxRectList[i][j][k]->SetSize(rectList[i][j][k]->GetSize());
								};break;
                            default:debug.toLog(0,"max error... same as above oder so");break; // TODO ERROR
                        }
                    if((maxRectList[i][j][k]->GetX()!=oldx)||(maxRectList[i][j][k]->GetY()!=oldy))
                        change=1;
                }
}


/*    for(int i=0;i<MAX_RESOLUTIONS;i++)
        for(int j=0;j<MAX_TABS;j++)
            for(int k=0;k<MAX_WINDOWS;k++)
                if(rectList[i][j][k])
                {
					debug.toLog(0,"[%i/%i/%i] x: %4i, y: %4i, dx: %4i, dy: %4i [max: x: %4i, y: %4i, dx: %4i, dy: %4i]", i, j, k, rectList[i][j][k]->GetX(), rectList[i][j][k]->GetY(), rectList[i][j][k]->GetWidth(), rectList[i][j][k]->GetHeight(), maxRectList[i][j][k]->GetX(), maxRectList[i][j][k]->GetY(), maxRectList[i][j][k]->GetWidth(), maxRectList[i][j][k]->GetHeight());
				}*/

					
//...



};


wxColour UI_Theme::mixColour(wxColour* id1, wxColour* id2)
{
	return(wxColour(id1->Red()  +id2->Red(), 
					id1->Green()+id2->Green(), 
					id1->Blue() +id2->Blue()));
					
};

wxColour UI_Theme::mixColour(wxColour* id1, wxColour* id2, int gradient)
{
	return(wxColour((id1->Red()*gradient  +id2->Red()*(100-gradient))/100, 
					(id1->Green()*gradient+id2->Green()*(100-gradient))/100, 
					(id1->Blue()*gradient +id2->Blue()*(100-gradient))/100));
					
};

wxColour UI_Theme::brightenColour(wxColour* id, int brightness)
{
    return(wxColour(id->Red()  +brightness,
					id->Green()+brightness,
					id->Blue() +brightness));
};

wxColour UI_Theme::lookUpMixedColour(eColour id1, eColour id2)
{
	return(mixColour(lookUpColour(id1), lookUpColour(id2)));
};

wxColour UI_Theme::lookUpMixedColour(eColour id1, eColour id2, int gradient)
{
	return(mixColour(lookUpColour(id1), lookUpColour(id2), gradient));
};

wxColour UI_Theme::lookUpBrightenedColour(eColour id, int brightness)
{
	return(brightenColour(lookUpColour(id), brightness));
};

wxBrush UI_Theme::lookUpBrightenedBrush(eBrush id, int brightness)
{
    return(wxBrush(brightenColour(&(lookUpBrush(id)->GetColour()), brightness), lookUpBrush(id)->GetStyle()));
};
	
wxBrush UI_Theme::lookUpMixedBrush(eBrush id1, eColour id2)
{
    return(wxBrush(mixColour(&(lookUpBrush(id1)->GetColour()), lookUpColour(id2)),lookUpBrush(id1)->GetStyle()));
};

wxBrush UI_Theme::lookUpMixedBrush(eBrush id1, eColour id2, int gradient)
{
    return(wxBrush(mixColour(&(lookUpBrush(id1)->GetColour()), lookUpColour(id2), gradient),lookUpBrush(id1)->GetStyle()));
};

wxBrush UI_Theme::lookUpMixedBrush(eBrush id1, eBrush id2)
{
    return(wxBrush(mixColour(&(lookUpBrush(id1)->GetColour()), &(lookUpBrush(id2)->GetColour())),lookUpBrush(id1)->GetStyle()));
};

wxBrush UI_Theme::lookUpMixedBrush(eBrush id1, eBrush id2, int gradient)
{
    return(wxBrush(mixColour(&(lookUpBrush(id1)->GetColour()), &(lookUpBrush(id2)->GetColour()), gradient),lookUpBrush(id1)->GetStyle()));
};

wxPen UI_Theme::lookUpBrightenedPen(ePen id, int brightness)
{
	    return(wxPen(brightenColour(&(lookUpPen(id)->GetColour()), brightness), lookUpPen(id)->GetWidth(), lookUpPen(id)->GetStyle()));
};
                                                                                                                  
wxPen UI_Theme::lookUpMixedPen(ePen id1, ePen id2)
{
	    return(wxPen(mixColour(&(lookUpPen(id1)->GetColour()), &(lookUpPen(id2)->GetColour())), lookUpPen(id1)->GetWidth(), lookUpPen(id1)->GetStyle()));
};
                                                                                                                  
wxPen UI_Theme::lookUpMixedPen(ePen id1, ePen id2, int gradient)
{
	    return(wxPen(mixColour(&(lookUpPen(id1)->GetColour()), &(lookUpPen(id2)->GetColour()), gradient), lookUpPen(id1)->GetWidth(), lookUpPen(id1)->GetStyle()));
};

wxPen UI_Theme::lookUpMixedPen(ePen id1, eColour id2)
{
	    return(wxPen(mixColour(&(lookUpPen(id1)->GetColour()), lookUpColour(id2)), lookUpPen(id1)->GetWidth(), lookUpPen(id1)->GetStyle()));
};
                                                                                                                  
wxPen UI_Theme::lookUpMixedPen(ePen id1, eColour id2, int gradient)
{
	    return(wxPen(mixColour(&(lookUpPen(id1)->GetColour()), lookUpColour(id2), gradient), lookUpPen(id1)->GetWidth(), lookUpPen(id1)->GetStyle()));
};


