#include "main.hpp"

#include "../sdl/framerate.hpp"
#include "progressbar.hpp"

#include <dirent.h>
#include <sys/types.h>

#ifdef __linux__
#else 
#include <windows.h>
#endif

#include <fstream>

ProgressBar* bar;

unsigned int SCREEN_X;
unsigned int SCREEN_Y;
//int gizmor;
//int grey; //0-100%

void get_bg(DC* dc, SDL_Surface *surface, int x, int y) {
    /* Blits a surface sized chunk of background to that surface */
    SDL_Rect src;
    SDL_Rect dst;
                                                                                
    src.x = x;
    src.y = y;
    src.w = dc->GetSurface()->w;
    src.h = dc->GetSurface()->h;
                                                                                
    dst.x = 0;
    dst.y = 0;
    dst.w = dc->GetSurface()->w;
    dst.h = dc->GetSurface()->h;
    SDL_BlitSurface(dc->GetSurface(), &src, surface, &dst);
}

void RS_Blit(DC* dc, SDL_Surface *bitmap, Sint16 x, Sint16 y) {
    SDL_Rect dest;
                                                                                
    dest.x = x;
    dest.y = y;
    dest.w = bitmap->w;
    dest.h = bitmap->h;
    SDL_BlitSurface(bitmap, NULL, dc->GetSurface(), &dest);
}

/*static SDL_Cursor *init_system_cursor(const char *image[])
{
  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  int hot_x, hot_y;

  i = -1;
  for ( row=0; row<32; ++row ) {
    for ( col=0; col<32; ++col ) {
      if ( col % 8 ) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i] = mask[i] = 0;
      }
      switch (image[4+row][col]) {
        case 'X':
          data[i] |= 0x01;
          mask[i] |= 0x01;
          break;
        case '.':
          mask[i] |= 0x01;
          break;
        case ' ':
          break;
      }
    }
  }
  sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
  return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}*/

void Main::resetData()
{
	refresh=0;
	endrun=false;
//	gizmor=rand()%GIZMO_NUMBER;
	for(int i=MAX_PLAYER;i--;)
		anarace[i]=NULL;
	update=0;
}

void Main::process()
{
	if(isOptimizing()) ani++;
	else ani=1;
	if(ani>30) ani=1;


//	if(update==2)
	{
		for(int i=settings.getMap(0)->getMaxPlayer();i--;)
			if(player[i]->isShown())
				player[i]->CheckOrders();
	}
	update=1;
	mainWindow->process();
	if(mainWindow->tabWasChanged())
	{
		gizmo=false;
		eTab ctab = mainWindow->getCurrentTab();
		switch(ctab)
		{
			case BASIC_TAB:
				msgWindow->Show();
				settingsWindow->Hide();
				theCore->Hide();
				tutorialWindow->Hide();
				gizmo=true;
			break;
			case ADVANCED_TAB: //1 player
				msgWindow->Show();
				settingsWindow->Hide();
				theCore->Hide();
				tutorialWindow->Hide();
				gizmo=true;
			break;
			case EXPERT_TAB: //2 player rushversuche
				msgWindow->Show();
				settingsWindow->Hide();
				theCore->Hide();
				tutorialWindow->Hide();
			break;
			case GOSU_TAB: // 2 player - Spieler spielt
				msgWindow->Show();
				settingsWindow->Hide();
				theCore->Hide();
				tutorialWindow->Hide();
			break;
			case TRANSCENDEND_TAB: // 2 player - 2 Computer
				msgWindow->Show();
				settingsWindow->Hide();
				theCore->Hide();
				tutorialWindow->Hide();
			break;
			case MAP_TAB:
                msgWindow->Show();
                settingsWindow->Hide();
                theCore->Hide();
                tutorialWindow->Hide();
            break;
			case SETTINGS_TAB:
				msgWindow->Hide();
				settingsWindow->Show();
				theCore->Hide();
				tutorialWindow->Hide();
			break;
			case TUTORIAL_TAB:
				msgWindow->Hide();
				settingsWindow->Hide();
				theCore->Hide();			
				tutorialWindow->Show();
			break;
			default:break;		
		} // end switch getCurrentTabs
		for(int i=0;i<2;i++)
			player[i]->setMode(ctab, i);
//		player[0]->CheckOrders();
//		player[1]->CheckOrders();

		UI_Object::theme.setTab(ctab);
		
		mainWindow->updateRectangles(0);
		msgWindow->updateRectangles(0);
		settingsWindow->updateRectangles(0);
		theCore->updateRectangles(0);
		tutorialWindow->updateRectangles(0);
		int maxPlayer=0;
		if(player[0]->isShown()) 
			maxPlayer++;
		if(player[1]->isShown()) 
			maxPlayer++;
		if(player[0]->isShown())
			player[0]->updateRectangles(maxPlayer-1);
		if(player[1]->isShown()) 
			player[1]->updateRectangles(maxPlayer-1);
		
/*			settings.initSoup();
			resetData();*/
		update=2;
	} // end tabwasChanged

	for(int i=0;i<2;i++)
		player[i]->checkForChange();
	
	settings.checkForChange();


	if(endrun)
	{
		ostringstream os;
		os << "Final time round " << anarace[0]->getRun() << " : ["  
			<< setw(2) << (ga->maxTime-anarace[0]->getTimer())/60 << ":" 
			<< setw(2) << (ga->maxTime-anarace[0]->getTimer())%60 << "]";
		msgWindow->addMessage(os.str());
		endrun=false;
//		resetData();
	}
}

void Main::stopOptimizing()
{
	for(unsigned int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		   if(player[i]->isShown())
			player[i]->setOptimizing(false);
}

void Main::startOptimizing()
{
	for(unsigned int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		   if(player[i]->isShown())
				player[i]->setOptimizing(true);
}

const bool Main::isOptimizing() const
{
	for(unsigned int i=settings.getMap(0)->getMaxPlayer();i--;)
		if(player[i]->isOptimizing())
			return(true);
	return(false);
}

const Size Main::helper(DC* dc, const unsigned int dx, const int i, const string& str) const
{
	dc->SetTextForeground(toSDL_Color(
				(Uint8)((0==ani%(20+i))*200+((0==ani%(19+i))+(0==ani%(21+i)))*50),
				(Uint8)((0==ani%(20+i))*200+((0==ani%(19+i))+(0==ani%(21+i)))*50),
				(Uint8)((0==ani%(20+i))*200+((0==ani%(19+i))+(0==ani%(21+i)))*100+50)));
	dc->DrawText(str.substr(str.size()-1, str.size()), mainWindow->getAbsoluteClientRectPosition()+Point(20+dx,20));
	return(dc->GetTextExtent(str.c_str()));
}

void Main::drawGizmo(DC* dc) const
{
	dc->SetFont(UI_Object::theme.lookUpFont(HUGE_DEFAULT_BOLD_FONT));
	string str="Evolution";
	Size s;
	for(unsigned int i=0;i<str.size();i++)
		s = helper(dc, s.GetWidth(), i, str.substr(0,i+1));

	dc->SetTextForeground(toSDL_Color(25,25,85));
	dc->DrawText("Forge",mainWindow->getAbsoluteClientRectPosition()+Point(50,60));
	dc->SetTextForeground(toSDL_Color(0,0,85));
	dc->DrawText("v1.25 BetaTest",mainWindow->getAbsoluteClientRectPosition()+Point(78,98));
	dc->SetTextForeground(toSDL_Color(50,50,85));
	dc->DrawText("v1.25 BetaTest",mainWindow->getAbsoluteClientRectPosition()+Point(75,95));

/*		dc->SetFont(font3);
		dc->SetTextForeground(toSDL_Color(155*grey*(100-grey)/2500,0,0));
		dc->DrawText(String::Format(T("Final time round %i: [%.2i:%.2i]"),anarace[0]->getRun(),(ga->maxTime-anarace[0]->getTimer())/60,(ga->maxTime-anarace[0]->getTimer())%60)),27,203);
																				
		dc->SetTextForeground(toSDL_Color(255*grey*(100-grey)/2500,0,0));
		dc->DrawText(String::Format(T("Final time round %i: [%.2i:%.2i]"),anarace[0]->getRun(),(ga->maxTime-anarace[0]->getTimer())/60,(ga->maxTime-anarace[0]->getTimer())%60)),25,200);
		if(grey==100)
		grey-=2;
		if(grey<=0) resetData();
	} else
	if(grey<100)
	{
		dc->SetTextForeground(toSDL_Color(130*grey*(100-grey)/2500,0,0));
		dc->SetFont(font3);
		dc->DrawText(String::Format(T("Round %i"),anarace[0]->getRun()+1)),msgWindow->getAbsoluteClientRectLeftBound()+18,msgWindow->getClientRectUpperBound()+13);
		dc->SetFont(font3);
		dc->DrawText(String::Format(T("%s"),gizmo[gizmor])),msgWindow->getAbsoluteClientRectLeftBound()+18,msgWindow->getClientRectUpperBound()+63);
																				
		dc->SetTextForeground(toSDL_Color(230*grey*(100-grey)/2500,0,0));
		dc->SetFont(font3);
		dc->DrawText(String::Format(T("Round %i"),anarace[0]->getRun()+1)),msgWindow->getAbsoluteClientRectLeftBound()+15,msgWindow->getClientRectUpperBound()+10);
		dc->SetFont(font3);
		dc->DrawText(String::Format(T("%s"),gizmo[gizmor])),msgWindow->getAbsoluteClientRectLeftBound()+15,msgWindow->getClientRectUpperBound()+60);
																				
		grey+=2;
	}*/
	
}

void Main::draw(DC* dc) const
{
	if(mainWindow->isShown())
	{
		mainWindow->draw(dc);
		if(gizmo)
			drawGizmo(dc);
	}
	SDL_Rect c;
	c.x=maus.x;
	c.y=maus.y;
	c.w=32;
	c.h=32;	
//	get_bg(dc, cursor_save, maus.x, maus.y);
//	SDL_BlitSurface(*UI_Object::theme.lookUpBitmap(MAUS_BITMAP) , 0, dc->GetSurface(), &c );
//	RS_Blit(dc, cursor, maus.x, maus.y);
	//RS_Blit(dc, cursor_save, maus.x, maus.y);
}

										
//settings: log level (none, debug only, +final result, +result of each run, +snapshot every X generations, +snapshot every generation, EVERYTHING (~2MB/generation!)

void Main::OnIdle()
{
	if((!endrun)&&(!refresh)&&(isOptimizing()))
	{
		update=2;
		ANARACE** temp;
//TODO: nach Ende eines Durchlaufs ist anarace 0, aber viele anderen Teile des Codes greifen noch drauf zu!!
		if((temp=settings.newGeneration(anarace)))
		{
			for(int i=settings.getMap(0)->getMaxPlayer();i--;)
			{
				anarace[i]=temp[i];
			}
			if(anarace[0]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=true;}
		}
	}
	if(settings.getIsNewRun())
	{
		player[0]->resetData();
		player[1]->resetData();
	}
//	if(update==1)
//	{
//		update=2;
//	}

	refresh++;
	if(refresh>settings.getSpeed())
		refresh=0;		
}

void toConsole(const string& str)
{
    ofstream pFile("error.log", ios_base::app);
    pFile << str << endl;
		
//	cout << str << endl;
}


Main::Main(DC* dc):
	mainWindow(NULL),
	msgWindow(NULL),
	infoWindow(NULL),
	tutorialWindow(NULL),
	theCore(NULL),
	settingsWindow(NULL),
	maus(),
	cursor_save(NULL),
	cursor(NULL),
	buttonPressed(false),
	hasAlreadyLeft(false),
	button(NULL),
	oldrun(0),
	endrun(false),
	gizmo(true),
	ga(NULL),
	update(0),
	refresh(0),
	ani(0)	
{
	toConsole("Loading ui settings, bitmaps and fonts (settings/ui/default.ui, data/bitmaps, data/fonts...");
	UI_Object::theme.loadDataFiles("settings/ui/default.ui","data/bitmaps","data/fonts",dc);
	bar->draw(dc, 20, "Data files loaded");
	
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	toConsole("Loading harvest data file (settings/harvest/default.hvt)...");
	settings.loadHarvestFile("settings/harvest/default.hvt");
	bar->draw(dc, 2, "Harvest files loaded");
	
	toConsole("Loading core settings (settings/default.cfg)...");
	settings.loadSettingsFile("settings/default.cfg");
	bar->draw(dc, 2, "Core settings loaded");
	
	toConsole("Loading maps (settings/maps)...");
	settings.loadMapFile("settings/maps/lt42.map");
	bar->draw(dc, 2, "Maps loaded");
	
	toConsole("Loading start conditions (settings/start)...");
	settings.loadStartconditionFile("settings/start/default_terra.start");
	settings.loadStartconditionFile("settings/start/default_protoss.start");
	settings.loadStartconditionFile("settings/start/default_zerg.start");
	bar->draw(dc, 4, "Startconditions loaded");
	
	toConsole("Loading goals (settings/goals)...");
#ifdef __linux__
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir("settings/goals/terra")) == NULL)
		toConsole("ERROR Loading terra goals... Directory settings/goals/terra not found...");
	else {
	while ((entry = readdir(dir)) != NULL)
	{
		ostringstream os;
		os << "settings/goals/terra/" << entry->d_name;
		settings.loadGoalFile(os.str());
	}
	closedir(dir);
	}
#else
	settings.loadGoalFile("settings\\goals\\terra\\zzz.gol");
/*				
{
   WIN32_FIND_DATA dir;
   HANDLE fhandle;
   if ((fhandle=FindFirstFile("settings\\goals\\terra",&dir)) !=INVALID_HANDLE_VALUE)
      {
         do {
		        ostringstream os;
		        os << "settings\\goals\\terra\\" << dir.cFileName;
		        settings.loadGoalFile(os.str());
				toConsole(os.str());
            } while(FindNextFile(fhandle,&dir));
	}
   FindClose(fhandle);
}*/

#endif
	
	bar->draw(dc, 4, "Terra goals loaded");
#ifdef __linux__
	if ((dir = opendir("settings/goals/protoss")) == NULL)
		toConsole("ERROR Loading terra goals... Directory settings/goals/protoss not found...");
	else {
	while ((entry = readdir(dir)) != NULL)
	{
		ostringstream os;
		os << "settings/goals/protoss/" << entry->d_name;
		settings.loadGoalFile(os.str());
	}
	closedir(dir);
	}
#else
	settings.loadGoalFile("settings\\goals\\protoss\\reaverdrop.gol");
/*{
   WIN32_FIND_DATA dir;
   HANDLE fhandle;
   if ((fhandle=FindFirstFile("settings/goals/protoss/",&dir)) !=INVALID_HANDLE_VALUE)
      {
         do {
		        ostringstream os;
		        os << "settings/goals/protoss/" << dir.cFileName;
		        settings.loadGoalFile(os.str());
				toConsole(os.str());
            } while(FindNextFile(fhandle,&dir));
	}
   FindClose(fhandle);
}*/
// windows
#endif
	bar->draw(dc, 4, "Protoss goals loaded");

#ifdef __linux__
	if ((dir = opendir("settings/goals/zerg")) == NULL)
		toConsole("ERROR Loading zerg goals... Directory settings/goals/zerg not found...");
	else {
	while ((entry = readdir(dir)) != NULL)
	{
		ostringstream os;
		os << "settings/goals/zerg/" << entry->d_name;
		settings.loadGoalFile(os.str());
	}
	closedir(dir);
	}
#else
	settings.loadGoalFile("settings\\goals\\zerg\\lurker.gol");
/*{
   WIN32_FIND_DATA dir;
   HANDLE fhandle;
   if ((fhandle=FindFirstFile("settings/goals/zerg",&dir)) !=INVALID_HANDLE_VALUE)
      {
         do {
		        ostringstream os;
		        os << "settings/goals/zerg/" << dir.cFileName;
		        settings.loadGoalFile(os.str());
				toConsole(os.str());
            } while(FindNextFile(fhandle,&dir));
	}
   FindClose(fhandle);
}*/
// windows
#endif

	bar->draw(dc, 4, "Zerg goals loaded");
// goal beschreibt Rasse, Ziele und Modus
	
	toConsole("Assigning and analyzing data...");
// Map in "map.txt" is now map[0]
// choose the first map we loaded (map[0])
	settings.assignMap(0); // first map (lt) and ums = false
//	settings.setMode(0); // TODO
	settings.setHarvestSpeed(TERRA, 0);
	settings.setHarvestSpeed(PROTOSS, 1);
	settings.setHarvestSpeed(ZERG, 2);

	settings.assignStartRace(1, TERRA);
	settings.assignStartRace(2, TERRA);

	settings.assignStartcondition(1, 0);
	settings.assignStartcondition(2, 0);

	settings.setStartPosition(1, 1);
	settings.setStartPosition(2, 7);
	settings.fillGroups();

	settings.assignGoal(1, 0);
	settings.assignGoal(2, 0); //~~
	bar->draw(dc, 8, "Data initialized");
	// TODO FEHLERMELDUNG FALLS FALSCHES GOAL ZU FALSCHER RASSE

//	settings.assignStartconditionHarvestSpeed();
		
	toConsole("Preparing soup for first run...");
	settings.assignRunParametersToSoup(); // assign START and GA of settings to soup
	// initializes players, initializes Map
	bar->draw(dc, 2, "Soup for first generation prepared");
	
// initialize the soup, set the parameters, load the players etc.
	ga=settings.getGa();
//	grey=0;
	resetData(); // TODO 
	if(ANARACE** temp=settings.newGeneration(anarace))
	{
		for(unsigned int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
			anarace[i]=temp[i];
	}

	anarace[0]->setMaxpFitness(0);

	UI_Object::assignStartTime();
	
	bar->draw(dc, 2, "First generation executed");
	toConsole("Initializing GUI...");
	mainWindow=new MainWindow();
	ostringstream os;
	//os << "[GUI: " << setprecision(2) << GUI_VERSION << "] [CORE: " << setprecision(2) << CORE_VERSION << "]";
	//mainWindow->setTitleParameter(os.str());
	
	msgWindow=new MessageWindow(mainWindow);
	theCore=new CoreWindow(mainWindow);
	tutorialWindow=new TutorialWindow(mainWindow);
	settingsWindow = new SettingsWindow(mainWindow);

//TODO: scc2 player und scc2dll player Zusammenhang nachschaun! loadPlayer wird net aufgerufen... goals ueberschneiden etc...

	for(unsigned int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		player[i]=new Player(mainWindow, &(anarace[i]), msgWindow, i);
	for(unsigned int i = settings.getMap(0)->getMaxPlayer();i<MAX_PLAYER;i++)
		player[i]=0;

//TODO grey wieder rein... evtl bei draw

	bar->draw(dc, 4, "GUI Windows initialized");
	mainWindow->Show();
	msgWindow->Show();
	theCore->Hide();
	tutorialWindow->Hide();
	settingsWindow->Hide();

	player[0]->Show();
	player[1]->Hide();

	
	msgWindow->addMessage(*(UI_Object::theme.lookUpString(WELCOME_MSG1_STRING)));
	msgWindow->addMessage(*(UI_Object::theme.lookUpString(WELCOME_MSG2_STRING)));
	msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(PLAYERS_LOADED_STRING, settings.getMap(0)->getMaxPlayer()));
	toConsole("Main initialization complete.");
	bar->draw(dc, 4, "Main initialization complete");
//	cursor=init_system_cursor(arrow);
//	SDL_ShowCursor(SDL_DISABLE);
//	cursor = (UI_Object::theme.lookUpBitmap(MAUS_BITMAP))->getSurface();

//  SDL_SetColorKey(cursor, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(cursor->format, 255, 255, 255));
			
//	cursor_save = *UI_Object::theme.lookUpBitmap(MAUS_BITMAP);
	
//	get_bg(dc, cursor_save, 0, 0);
//    SDL_SetAlpha(cursor, SDL_SRCALPHA, 127);
}



Main::~Main()
{
    delete mainWindow;
    delete msgWindow;
    delete theCore;
    delete tutorialWindow;
    delete settingsWindow;
    for(int i=0;i<MAX_PLAYER;i++)
        delete player[i];
}

void Main::leftDown()
{
	if(button)
	{
		button->mouseLeftButtonPressed();
		buttonPressed=true;
	}
		
}

void Main::leftUp(const Point p)
{
	if((button)&&(buttonPressed))
		button->mouseLeftButtonReleased();
	buttonPressed=false;
	hasAlreadyLeft=false;
	setMouse(p);
}

void Main::rightDown()
{
	if(button)
	{
		button->mouseRightButtonPressed();
		buttonPressed=true;
	}
		
}

void Main::rightUp(const Point p)
{
	if((button)&&(buttonPressed))
		button->mouseRightButtonReleased();
	buttonPressed=false;
	hasAlreadyLeft=false;
	setMouse(p);
}

void Main::setMouse(const Point p)
{
	if(p == UI_Object::mouse)
		return;
	maus=p;
	UI_Object::mouse=p;
	((BoGraphWindow*)(player[0]->window[BO_GRAPH_WINDOW]))->mouseHasMoved();
//	else if(player[1]->window[BO_GRAPH_WINDOW]->Inside(p))
//		(BoGraphWindow*)(player[1]->window[BO_GRAPH_WINDOW])->mouseHasMoved();
	
	if(hasAlreadyLeft)
	{
		if((button)&&(button->getAbsoluteRect().Inside(p)))
		{
			button->mouseHasEnteredArea();
			hasAlreadyLeft=false;
		}
		else return;
	}
	if((button)&&(!button->getAbsoluteRect().Inside(p)))
	{
		button->mouseHasLeftArea();
		if(!buttonPressed)
			button=0;
		else
			hasAlreadyLeft=true;
	}
// ignore mousemove if button is still pressed		
	if(button)
	{
		if(buttonPressed)
		{
			button->forceHighlighted();
//			button->mouseHasMoved();
		}
		return;
	}
	button = (UI_Button*) (player[0]->checkHighlight());
	if(!button)
		button = (UI_Button*) (player[1]->checkHighlight());
	if(!button)
		button = (UI_Button*) (mainWindow->checkHighlight());
	if(!button)
		button = (UI_Button*) (settingsWindow->checkHighlight());
	if(button)
	{
		button->mouseHasEnteredArea();
		hasAlreadyLeft=false;
	}
}


int main(int argc, char *argv[])
{
	toConsole("Starting ... ");
	long unsigned int ss = SDL_GetTicks();
	SDL_Event event;
	DC* screen;
	toConsole("Initializing SDL ... ");
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0 )
	{
		ostringstream os;
		os << "Unable to init SDL: " << SDL_GetError();
		toConsole(os.str());
		return(1);
	}
	bool FULLSCREEN=false;
 	SDL_Rect c;c.x=0;c.y=0;c.w=1024;c.h=768;
	toConsole("Initializing videomode...");
	
	if (FULLSCREEN)
	{
		toConsole("Setting fullscreen...");
		screen=new DC(SDL_SetVideoMode(c.w, c.h, 32, SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT|SDL_FULLSCREEN));
	}
	else
	{
		toConsole("Setting window mode...");
		screen=new DC(SDL_SetVideoMode(c.w, c.h, 32, SDL_SWSURFACE|SDL_DOUBLEBUF|SDL_ANYFORMAT));
	}
	if ( screen == NULL )
	{
		toConsole("Error setting video mode");
		return(1);
	}
	screen->setResolution(c.w, c.h);
	UI_Object::max_x=c.w;
	UI_Object::max_y=c.h;
	c.x = 0;
	c.y = 0;

	Bitmap progress("data/bitmaps/back2.bmp");
	Bitmap claw("data/bitmaps/clawsoftware.bmp");

	screen->DrawBitmap(progress, (c.w - progress->w)/2, (c.h - progress->h)/2);
	screen->DrawBitmap(claw, c.w - claw->w, c.h - claw->h);
	screen->SetPen(Pen(Color(screen->GetSurface(), 255, 255, 255), 1, SOLID_PEN_STYLE));
	screen->SetBrush(Brush(Color(screen->GetSurface(), 100, 150, 255), SOLID_BRUSH_STYLE));
	
	bar = new ProgressBar(Rect((c.w-progress->w)/2 + 10, (c.h - progress->h)/2 + 10, progress->w - 20, progress->h - 20));
	bar->draw(screen, 5);

	SDL_WM_SetCaption("EVOLUTION FORGE (C) CLEMENS LODE, 2004 www.clawsoftware.de","");

	toConsole("Initializing SDL truetype fonts ... ");
	/* start SDL_ttf */
	if(TTF_Init()==-1)
	{
		ostringstream os;
		os << "TTF_Init: " << TTF_GetError();
		toConsole(os.str());		
		return 2;
	}
	atexit(TTF_Quit); /* remember to quit SDL_ttf */
	atexit(SDL_Quit);
	bar->draw(screen, 5);

	toConsole("Initializing graphic engine and core ...");
	Main m(screen);

// ------ CAP FRAMERATE ------
	FPS* fps=new FPS();
	fps->setFramerate(99);
// ------ END CAP FRAMERATE

	bar->draw(screen, 100, "Graphic engine and core initialized");

	bool fullupdate=false;
	unsigned int screenshot=100;
	Rect text;
//	UI_StaticText* t=new UI_StaticText(NULL, "Software", Rect(-200, 0, 0, 0), NO_TEXT_MODE, FITNESS_TEXT_COLOR, VERY_LARGE_NORMAL_BOLD_FONT);
//	t->adjustRelativeRect(Rect(800, 700, 0, 0));
	toConsole("System ready ...");
	ostringstream oss;
	oss << ((float)(ss - SDL_GetTicks()))/1000000000.0;
	toConsole("Initialization time: "+oss.str());
	
	while(true)
	{
		m.process();
		m.OnIdle();
//		SDL_BlitSurface(*UI_Object::theme.lookUpBitmap(BACKGROUND_SPACE_BITMAP) , 0, screen->GetSurface(), &c );
	    screen->SetPen(Pen(Color(screen->GetSurface(), 0, 0, 0), 1, TRANSPARENT_PEN_STYLE));
		screen->SetBrush(Brush(Color(screen->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
		screen->DrawRectangle(Rect(c.x, c.y, c.w-1, c.h-1));
																						
		m.draw(screen);
// ------ SCREENCAPTURE ------		
		if(fullupdate) {
			ostringstream os;os << "shot" << screenshot << ".bmp";
			SDL_SaveBMP(screen->GetSurface() , os.str().c_str());os.str("");screenshot++;
			os << " saved (" << (c.w * c.h)/256 << "kb) [" << (screenshot * c.w * c.h)/(1024*256)<<"MB total]";
			screen->DrawText(os.str(), 50, 600);
		}
// ------ END SCREENCAPTURE -----
/*		ostringstream os;
		os << m.maus.x << ":" << m.maus.y << " " << m.buttonPressed << " " << m.hasAlreadyLeft << " | ";
		if(m.button)
			os << "button: " << m.button->getAbsoluteLeftBound() << ":" << m.button->getAbsoluteUpperBound() << ":" << m.button->getAbsoluteRightBound() << ":" << m.button->getAbsoluteLowerBound();
		screen->SetBrush(*UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		screen->DrawRectangle(600,500,200,20);
		screen->SetFont(UI_Object::theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
		screen->SetTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
		screen->DrawText(os.str(), 605, 505);*/
		

//    if ( (screen->GetSurface()->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF ) {
    //    SDL_Flip(screen->GetSurface());
  //  } else {
//		SDL_UpdateRects(screen->GetSurface(), UI_Object::rectnumber, &(UI_Object::rectlist[0]));
//	}
//		SDL_SetClipRect(screen->GetSurface(), &c);

		screen->updateScreen();
		fps->delay();

		while (SDL_PollEvent(&event))
		{
			
		switch (event.type)
		{
			case SDL_QUIT:
				return(0);break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:delete fps;return(0);break;
					case SDLK_SPACE:
						fullupdate=!fullupdate;break;
/*						{
							if(m.isOptimizing())
								m.stopOptimizing();
							else m.startOptimizing();
						};break;*/
					case SDLK_RETURN:
						if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
							SDL_WM_ToggleFullScreen(screen->GetSurface());
					break;
					case SDLK_PLUS:
							if(UI_Object::theme.getResolution()<RESOLUTION_1280x1024)
							{
								UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() + 1));
							}
							break;
					case SDLK_MINUS:
							if(UI_Object::theme.getResolution()>RESOLUTION_800x600)
							{
								UI_Object::theme.setResolution((eResolution)(UI_Object::theme.getResolution() - 1));
							}
							break;
				
					case SDLK_F1:m.mainWindow->forcePressTab(BASIC_TAB);break;
					case SDLK_F2:m.mainWindow->forcePressTab(ADVANCED_TAB);break;
					case SDLK_F3:m.mainWindow->forcePressTab(EXPERT_TAB);break;
					case SDLK_F4:m.mainWindow->forcePressTab(GOSU_TAB);break;
					case SDLK_F5:m.mainWindow->forcePressTab(TRANSCENDEND_TAB);break;
					case SDLK_F6:m.mainWindow->forcePressTab(MAP_TAB);break;
					case SDLK_F7:m.mainWindow->forcePressTab(SETTINGS_TAB);break;
					case SDLK_F8:m.mainWindow->forcePressTab(TUTORIAL_TAB);break;
					default:break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT)
					m.leftDown();
				else if(event.button.button == SDL_BUTTON_RIGHT)
					m.rightDown();
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT)
					m.leftUp(Point(event.motion.x, event.motion.y));
				else if(event.button.button == SDL_BUTTON_RIGHT)
					m.rightUp(Point(event.motion.x, event.motion.y));
				break;
			case SDL_MOUSEMOTION:
				m.setMouse(Point(event.motion.x, event.motion.y));break;
			default:break;
	/*					
void Main::OnMouseWheelScroll(MouseEvent& event)
{
	controls.scrollMouseWheel(event.GetWheelRotation());
}*/				
		}
	}


		
	}
}
					

