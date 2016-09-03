#include "main.hpp"
#include "SDL/SDL_gfxPrimitives.h"

int SCREEN_X;
int SCREEN_Y;
//int gizmor;
//int grey; //0-100%


void Main::resetData()
{
	refresh=0;
	endrun=0;
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

	if(update==2)
	{
		for(int i=settings.getMap(0)->getMaxPlayer();i--;)
			if(player[i]->isShown())
				player[i]->CheckOrders();
/*		ANARACE** temp;
//TODO: nach Ende eines Durchlaufs ist anarace 0, aber viele anderen Teile des Codes greifen noch drauf zu!!
		if((temp=settings.newGeneration(anarace)))
		{
			for(int i=settings.getMap(0)->getMaxPlayer();i--;)
			{
				anarace[i]=temp[i];
//			  if(anarace[i]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO
			}
		};*/
	}
	update=1;
	mainWindow->process();
	if(mainWindow->tabWasChanged())
	{
		gizmo=false;
		switch(mainWindow->getCurrentTab())
		{
			case BASIC_TAB:
				msgWindow->Show();
				theCore->Hide();
				tutorialWindow->Hide();
				gizmo=true;
			break;
			case ADVANCED_TAB: //1 player
				msgWindow->Show();
				theCore->Hide();
				tutorialWindow->Hide();
				gizmo=true;
			break;
			case EXPERT_TAB: //2 player rushversuche
				msgWindow->Show();
				theCore->Hide();
				tutorialWindow->Hide();
			break;
			case GOSU_TAB: // 2 player - Spieler spielt
				msgWindow->Show();
				theCore->Hide();
				tutorialWindow->Hide();
			break;
			case TRANSCENDEND_TAB: // 2 player - 2 Computer
				msgWindow->Show();
				theCore->Hide();
				tutorialWindow->Hide();
			break;
			case TUTORIAL_TAB:
				msgWindow->Hide();
				theCore->Hide();			
				tutorialWindow->Show();
			break;
			default:break;		
		} // end switch getCurrentTabs
		for(int i=0;i<2;i++)
			player[i]->setMode(mainWindow->getCurrentTab(), i);
//		player[0]->CheckOrders();
//		player[1]->CheckOrders();

		UI_Object::theme.setTab(mainWindow->getCurrentTab());
		
		mainWindow->updateRectangles(0);
		msgWindow->updateRectangles(0);
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
		os << "Final time round" << anarace[0]->getRun() << ": ["  
			<< setw(2) << (ga->maxTime-anarace[0]->getTimer())/60 << ":" 
			<< setw(2) << (ga->maxTime-anarace[0]->getTimer())%60 << "]";
		msgWindow->addMessage(os.str());
		resetData();
	}
}

void Main::stopOptimizing()
{
	for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		   if(player[i]->isShown())
			player[i]->setOptimizing(false);
}

void Main::startOptimizing()
{
	for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		   if(player[i]->isShown())
				player[i]->setOptimizing(true);
}

const int Main::isOptimizing()
{
	for(int i=settings.getMap(0)->getMaxPlayer();i--;)
		if(player[i]->isOptimizing())
			return(1);
	return(0);
}

void Main::helper(DC* dc, int i, int &dx, int &dy, const string& str) const
{
	dc->SetTextForeground(toSDL_Color(
				(Uint8)((0==ani%(20+i))*200+((0==ani%(19+i))+(0==ani%(21+i)))*50),
				(Uint8)((0==ani%(20+i))*200+((0==ani%(19+i))+(0==ani%(21+i)))*50),
				(Uint8)((0==ani%(20+i))*200+((0==ani%(19+i))+(0==ani%(21+i)))*100+50)));
	dc->DrawText(str.substr(str.size()-1, str.size()), mainWindow->getAbsoluteClientRectPosition()+Point(20+dx,20));
	dc->GetTextExtent(str.c_str(),&dx,&dy);
}

void Main::drawGizmo(DC* dc) const
{
	dc->SetFont(UI_Object::theme.lookUpFont(HUGE_DEFAULT_BOLD_FONT));
	int dx=0;int dy=0;
	string str="Evolution";
	for(unsigned int i=0;i<str.size();i++)
		helper(dc, i, dx, dy, str.substr(0,i+1));

	dc->SetTextForeground(toSDL_Color(25,25,85));
	dc->DrawText("Forge",mainWindow->getAbsoluteClientRectPosition()+Point(50,60));
	dc->SetTextForeground(toSDL_Color(0,0,85));
	dc->DrawText("v1.21",mainWindow->getAbsoluteClientRectPosition()+Point(158,98));
	dc->SetTextForeground(toSDL_Color(50,50,85));
	dc->DrawText("v1.21",mainWindow->getAbsoluteClientRectPosition()+Point(155,95));
}

void Main::draw(DC* dc) const
{
//	SDL_BlitSurface(*UI_Object::theme.lookUpBitmap(BACKGROUND_SPACE_BITMAP) , 0, dc->GetSurface(), 0 );
	if(mainWindow->isShown())
	{
		mainWindow->draw(dc);
		if(gizmo)
			drawGizmo(dc);
	}

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
// TODO evtl player in mainwindow als UI_Object einhaengen
}

										
//settings: log level (none, debug only, +final result, +result of each run, +snapshot every X generations, +snapshot every generation, EVERYTHING (~2MB/generation!)

void Main::OnIdle()
{
	if((endrun==0)&&(!refresh)&&(isOptimizing()))
	{
		update=2;
		ANARACE** temp;
//TODO: nach Ende eines Durchlaufs ist anarace 0, aber viele anderen Teile des Codes greifen noch drauf zu!!
		if((temp=settings.newGeneration(anarace)))
		{
			for(int i=settings.getMap(0)->getMaxPlayer();i--;)
			{
				anarace[i]=temp[i];
//				if(anarace[i]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO
			}
		};
	};
//	if(update==1)
//	{
//		update=2;
//	}

	refresh++;
	if(refresh>settings.getSpeed())
		refresh=0;		
}


void Main::Init(DC* dc)
{
	UI_Object::theme.loadDataFiles("settings/ui/default.ui","data/bitmaps","data/fonts",dc);
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	settings.loadHarvestFile("settings/harvest/default.hvt");
	settings.loadSettingsFile("settings/default.cfg");

	settings.loadMapFile("settings/maps/lt42.map");
	settings.loadStartconditionFile("settings/start/default_terra.start");
	settings.loadStartconditionFile("settings/start/default_protoss.start");
	settings.loadStartconditionFile("settings/start/default_zerg.start");
	settings.loadGoalFile("settings/goals/terra.gol");
	settings.loadGoalFile("settings/goals/protoss.gol");
	settings.loadGoalFile("settings/goals/zerg.gol");
// goal beschreibt Rasse, Ziele und Modus
	
// Map in "map.txt" is now map[0]
// choose the first map we loaded (map[0])
	settings.assignMap(0); // first map (lt) and ums = false
	settings.setMode(0); // TODO
	settings.setHarvestSpeed(TERRA, 0);
	settings.setHarvestSpeed(PROTOSS, 1);
	settings.setHarvestSpeed(ZERG, 2);

	settings.setStartRace(1, TERRA);
	settings.setStartRace(2, TERRA);

	settings.setStartcondition(1, TERRA);
	settings.setStartcondition(2, TERRA);

	settings.setStartPosition(1, 1);
	settings.setStartPosition(2, 7);
	settings.fillGroups();

	settings.setGoal(1, TERRA);
	settings.setGoal(2, TERRA); //~~
	// TODO FEHLERMELDUNG FALLS FALSCHES GOAL ZU FALSCHER RASSE

//	settings.assignStartconditionHarvestSpeed();
		
	settings.assignRunParametersToSoup(); // assign START and GA of settings to soup
	// initializes players, initializes Map
	
	update=0;
// initialize the soup, set the parameters, load the players etc.
	ga=settings.getGa();
	oldrun=0;
//	grey=0;
	resetData();
	for(int i=MAX_PLAYER;i--;)
		anarace[i]=0;
	if(ANARACE** temp=settings.newGeneration(anarace))
	{
		for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
			anarace[i]=temp[i];
//		update=2;
//		if(anarace[0]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO?
	};

	anarace[0]->setMaxpFitness(0);

	UI_Object::assignStartTime();
	
	mainWindow=new MainWindow();
	ostringstream os;
	//os << "[GUI: " << setprecision(2) << GUI_VERSION << "] [CORE: " << setprecision(2) << CORE_VERSION << "]";
	//mainWindow->setTitleParameter(os.str());
	
	msgWindow=new MessageWindow(mainWindow);
	theCore=new CoreWindow(mainWindow);
	tutorialWindow=new TutorialWindow(mainWindow);

//TODO: scc2 player und scc2dll player Zusammenhang nachschaun! loadPlayer wird net aufgerufen... goals ueberschneiden etc...

	for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		player[i]=new Player(mainWindow, &(anarace[i]), i);

//TODO grey wieder rein... evtl bei draw

	mainWindow->Show(1);
	msgWindow->Show(0);
	theCore->Show(0);
	tutorialWindow->Show(0);

	player[0]->Show(1);
	player[1]->Show(0);

	msgWindow->addMessage(*(UI_Object::theme.lookUpString(WELCOME_MSG1_STRING)));
	msgWindow->addMessage(*(UI_Object::theme.lookUpString(WELCOME_MSG2_STRING)));
	msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(PLAYERS_LOADED_STRING, settings.getMap(0)->getMaxPlayer()));
}

int main(/*int argc, char **argv*/)
{
	SDL_Event event;
	DC* screen;

	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) < 0 )
	{
		cout << "Unable to init SDL: " << SDL_GetError() << endl;
		return(1);
	}
	/* start SDL_ttf */
	if(TTF_Init()==-1)
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		return 2;
	}
	atexit(TTF_Quit); /* remember to quit SDL_ttf */
	atexit(SDL_Quit);
	SDL_WM_SetCaption("EVOLUTION FORGE (C) CLEMENS LODE, 2004 www.clawsoftware.de","");
	int vx=1280;
	int vy=1024;
	int FULLSCREEN=0;
 	SDL_Rect c;c.x=0;c.y=0;c.w=vx;c.h=vy;
	if (FULLSCREEN==1)
		screen=new DC(SDL_SetVideoMode(c.w, c.h, 32, SDL_HWSURFACE|SDL_FULLSCREEN));
	else
		screen=new DC(SDL_SetVideoMode(c.w, c.h, 32, SDL_HWSURFACE));
	if ( screen == NULL )
	{
		printf("error\n");
		return(1);
	}

	Main m;
	m.Init(screen);

	FPSmanager* fpsmanager=new FPSmanager;
	SDL_initFramerate(fpsmanager);
	// cap framerate
	SDL_setFramerate(fpsmanager, 50);
	bool fullupdate=false;
	int screenshot=100;
	while(true)
	{
		UI_Object::rectnumber=0;
		m.process();
		m.OnIdle();
		m.draw(screen);
		
//		SDL_UpdateRects(screen->GetSurface(), UI_Object::rectnumber, &(UI_Object::rectlist[0]));
		SDL_UpdateRect(screen->GetSurface(),0,0,0,0);
	    SDL_SetClipRect(screen->GetSurface(), &c);
//        SDL_Flip(screen->GetSurface());
		
		SDL_framerateDelay(fpsmanager);

		if(fullupdate)
		{
			ostringstream os;os << "shot" << screenshot << ".bmp";
			SDL_SaveBMP(screen->GetSurface() , os.str().c_str());os.str("");screenshot++;
		}

		//SDL_SetClipRect(screen->GetSurface(), &c);	
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:return(0);break;
				case SDL_KEYDOWN:
							  switch(event.key.keysym.sym)
							  {
									case SDLK_ESCAPE:return(0);break;
									case SDLK_SPACE:
													 fullupdate=!fullupdate;break;/*
									 {
										if(m.isOptimizing())
											m.stopOptimizing();
										else m.startOptimizing();
										};break;*/
									case SDLK_RETURN:
									 if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
											SDL_WM_ToggleFullScreen(screen->GetSurface());
									 break;
									case SDLK_UP:if(c.y>vy/10) c.y-=vy/10;else c.y=0;break;
									case SDLK_DOWN:if(c.y+c.h<vy-vy/10) c.y+=vy/10;else c.y=vy-c.h;break;
									case SDLK_LEFT:if(c.x>vx/10) c.x-=vx/10;else c.x=0;break;
									case SDLK_RIGHT:if(c.x+c.w<vx-vx/10) c.x+=vx/10;else c.x=vx-c.w;break;
									case SDLK_F1:m.mainWindow->forcePressTab(BASIC_TAB);break;
									case SDLK_F2:m.mainWindow->forcePressTab(ADVANCED_TAB);break;
									case SDLK_F3:m.mainWindow->forcePressTab(EXPERT_TAB);break;
									case SDLK_F4:m.mainWindow->forcePressTab(GOSU_TAB);break;
									case SDLK_F5:m.mainWindow->forcePressTab(TRANSCENDEND_TAB);break;
									case SDLK_F6:m.mainWindow->forcePressTab(MAP_TAB);break;
									case SDLK_F7:m.mainWindow->forcePressTab(SETTINGS_TAB);break;
									case SDLK_F8:m.mainWindow->forcePressTab(TUTORIAL_TAB);break;
									default:break;
							  };
					
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.button == SDL_BUTTON_LEFT)
						controls.leftDown();
					else if(event.button.button == SDL_BUTTON_RIGHT)
						controls.rightDown();
					break;
				case SDL_MOUSEBUTTONUP:
					if(event.button.button == SDL_BUTTON_LEFT)
						controls.leftUp();
					else if(event.button.button == SDL_BUTTON_RIGHT)
						controls.rightUp();
					break;
				case SDL_MOUSEMOTION:
					controls.setMouse(Point(event.motion.x, event.motion.y));break;
				default:break;
			}
		}
	}
	delete fpsmanager;
}
					
/*					
void Main::OnMouseWheelScroll(MouseEvent& event)
{
	controls.scrollMouseWheel(event.GetWheelRotation());
}*/
