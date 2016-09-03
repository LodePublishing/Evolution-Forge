#include "guimain.hpp"

#include <dirent.h>
#include <sys/types.h>

#ifdef __linux__
#else 
#include <windows.h>
#endif

//int gizmor;
//int grey; //0-100%

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
	if(isOptimizing()) 
	{
		if(ani2>1)
		{
			ani++;
			ani2=0;
		} else ani2++;
	}
	else ani=1;
	if(ani>30) ani=1;


//	if(update==2)
	{
		for(int i=settings.getMap(0)->getMaxPlayer();i--;)
			if(player[i]->isShown())
				player[i]->CheckOrders();
	}
	update=1;
	UI_Object::editTextFeld = NULL;
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
			
			case COMPARE_TAB: // 2 player - 2 Computer
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
		os << "Run " << anarace[0]->getRun() << "finished, final time:  [" << setw(2) << (anarace[0]->getRealTimer())/60 << ":";
		if(anarace[0]->getRealTimer()<10)
			os << "0";
		os << setw(2) << (anarace[0]->getRealTimer())%60 << "]";
		msgWindow->addMessage(os.str());
        settings.saveBuildOrder(anarace[0]); // TODO, evtl alle saven...
        msgWindow->addMessage("Saved build order...");
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
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*15+20),
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*15+20),
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*30+60)));
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
	dc->DrawText("v1.59 BetaTest",mainWindow->getAbsoluteClientRectPosition()+Point(78,98));
	dc->SetTextForeground(toSDL_Color(50,50,85));
	dc->DrawText("v1.59 BetaTest",mainWindow->getAbsoluteClientRectPosition()+Point(75,95));

/*		dc->SetFont(font3);
		dc->SetTextForeground(toSDL_Color(155*grey*(100-grey)/2500,0,0));
		dc->DrawText(String::Format(T("Final time round %i: [%.2i:%.2i]"),anarace[0]->getRun(),(settings->getMaxTime()-anarace[0]->getTimer())/60,(settings->getMaxTime()-anarace[0]->getTimer())%60)),27,203);
																				
		dc->SetTextForeground(toSDL_Color(255*grey*(100-grey)/2500,0,0));
		dc->DrawText(String::Format(T("Final time round %i: [%.2i:%.2i]"),anarace[0]->getRun(),(settings->getMaxTime()-anarace[0]->getTimer())/60,(settings->getMaxTime()-anarace[0]->getTimer())%60)),25,200);
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
			player[0]->geneAnimation++;
			player[1]->geneAnimation++;
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
	if(refresh>=1)//configuration.getSpeed())
		refresh=0;		
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
	update(0),
	refresh(0),
	ani(1),
	ani2(0)
{
	toLog("Loading ui settings, bitmaps and fonts (settings/ui/default.ui, data/bitmaps, data/fonts...");
	UI_Object::theme.loadDataFiles("settings/ui/default.ui","data/bitmaps","data/fonts",dc);
	bar->draw(dc, 20, "Data files loaded");
	
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	toLog("Loading harvest data file (settings/harvest/default.hvt)...");
	settings.loadHarvestFile("settings/harvest/default.hvt");
	bar->draw(dc, 2, "Harvest files loaded");
	
//	bar->draw(dc, 2, "Core settings loaded");
	
	toLog("Loading maps (settings/maps)...");
	settings.loadMapFile("settings/maps/lt42.map");
	bar->draw(dc, 2, "Maps loaded");
	
	toLog("Loading start conditions (settings/start)...");

	settings.loadStartconditionFile("settings/start/default_terra.start");
	settings.loadStartconditionFile("settings/start/default_protoss.start");
	settings.loadStartconditionFile("settings/start/default_zerg.start");
	bar->draw(dc, 4, "Startconditions loaded");
	
	toLog("Loading goals (settings/goals)...");
#ifdef __linux__
	for(int i = 0; i < MAX_RACES; i++)
	{
		DIR *dir;
		struct dirent *entry;
		ostringstream os;
		os << "settings/goals/" << raceString[i];
		if ((dir = opendir(os.str().c_str())) == NULL)
		{
			os.str("");
			os << "ERROR Loading " << raceString[i] << "  goals... Directory settings/goals/" << raceString[i] << " not found...";
			toLog(os.str());
		}
		else {
			while ((entry = readdir(dir)) != NULL)
			{
				ostringstream oss;
				oss << os.str() << "/" << entry->d_name;
				settings.loadGoalFile(oss.str());
			}
			closedir(dir);
		}
	}
#else
	settings.loadGoalFile("settings\\goals\\Terra\\zzz.gol");
	settings.loadGoalFile("settings\\goals\\Protoss\\reaverdrop.gol");
	settings.loadGoalFile("settings\\goals\\Zerg\\lurker.gol"); // TODO
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
				toLog(os.str());
            } while(FindNextFile(fhandle,&dir));
	}
   FindClose(fhandle);
}*/

#endif
	bar->draw(dc, 12, "Goals loaded");
// goal beschreibt Rasse, Ziele und Modus
	
	toLog("Assigning and analyzing data...");
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
		
	toLog("Preparing soup for first run...");
	settings.assignRunParametersToSoup(); // assign START and GA of settings to soup
	// initializes players, initializes Map
	bar->draw(dc, 2, "Soup for first generation prepared");
	
// initialize the soup, set the parameters, load the players etc.
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
	toLog("Initializing GUI...");
	mainWindow=new MainWindow();
	ostringstream os;
	//os << "[GUI: " << setprecision(2) << GUI_VERSION << "] [CORE: " << setprecision(2) << CORE_VERSION << "]";
	//mainWindow->setTitleParameter(os.str());
	
	theCore=new CoreWindow(mainWindow);
	tutorialWindow=new TutorialWindow(mainWindow);
	settingsWindow = new SettingsWindow(mainWindow);
	msgWindow=new MessageWindow(mainWindow);

//TODO: scc2 player und scc2dll player Zusammenhang nachschaun! loadPlayer wird net aufgerufen... goals ueberschneiden etc...

	for(unsigned int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		player[i]=new Player(mainWindow, &(anarace[i]), msgWindow, i);
	for(unsigned int i = settings.getMap(0)->getMaxPlayer();i<MAX_PLAYER;i++)
		player[i]=0;

	msgWindow->setParent(mainWindow); // process AFTER player


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
	toLog("Main initialization complete.");
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

ProgressBar* Main::bar;

