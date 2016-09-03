#include "main.hpp"

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

	mainWindow->process();
	if(mainWindow->tabWasChanged())
	{
//		player[0]->resetData();
//		player[1]->resetData();
			switch(mainWindow->getCurrentTab())
			{
				case BASIC_TAB:
					player[0]->Show(); // 1
					player[1]->Hide(); // 0
					 //1 player
					msgWindow->Show();
					theCore->Hide();
					tutorialWindow->Hide();
//					settings.setGoal(1,1); // share same goal with advanced
//					settings.setGoal(0,2); //zero goal!
				break;
				case ADVANCED_TAB: //1 player
					player[0]->Show(); // 2
					player[1]->Hide(); // 0
					msgWindow->Show();
					theCore->Show();
					tutorialWindow->Hide();
//					settings.setGoal(1,1); //share same goal with basic
//					settings.setGoal(0,2); //zero goal!
				break;
				case EXPERT_TAB: //2 player rushversuche
					player[0]->Show(); // 3
					player[1]->Show(); // 4
					msgWindow->Show();
					theCore->Show();
					tutorialWindow->Hide();
//					settings.setGoal(1,1); // share same goal as advanced
//					settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				case GOSU_TAB: // 2 player - Spieler spielt
					player[0]->Show(); // 5
					player[1]->Show(); // 6 
					msgWindow->Show();
					theCore->Show();
					tutorialWindow->Hide();
// TODO wenn zero goal -> absturz beim aendern
//					settings.setGoal(0,1); // zero goal! -> human
//					settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				case TRANSCENDEND_TAB: // 2 player - 2 Computer
					player[0]->Show(); // 7 ~~
					player[1]->Show(); // 6
					msgWindow->Show();
					theCore->Show();
					tutorialWindow->Hide();
//					settings.setGoal(0,1); // zero goal!  -> computer!
//					settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				case TUTORIAL_TAB:
					player[0]->Show(); // 1  // TODO evtl eigenen tutorial player machen
					player[1]->Hide(); // 0
					msgWindow->Hide();
					theCore->Hide();			
					tutorialWindow->Show();
//					settings.setGoal(1,1); // same goal as basic
//					settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				default:break;		
			} // end switch
			player[0]->CheckOrders();
			player[1]->CheckOrders();

			UI_Object::theme.setTab(mainWindow->getCurrentTab());
			
			mainWindow->updateRectangles();
			msgWindow->updateRectangles();
			theCore->updateRectangles();
			tutorialWindow->updateRectangles();
			player[0]->updateRectangles();
			player[1]->updateRectangles();
			
/*			settings.initSoup();
				resetData();
				if(ANARACE** temp=settings.newGeneration(anarace))
				{				 for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
									anarace[i]=temp[i];
						update=2;
//			  if(anarace[0]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO?
				};
				for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
						player[i]->assignAnarace(&(anarace[i]));*/
	
//			settings.checkForChange();
		} // end mainwindow is activated

	settings.checkForChange();
	if(update==2)
	{
		for(int i=settings.getMap(0)->getMaxPlayer();i--;)
			if(player[i]->isShown())
				player[i]->CheckOrders();
	}
	update=1;

	if(endrun)
	{
		ostringstream os;
		os << "Final time round" << anarace[0]->getRun() << ": [" << setw(2) << (ga->maxTime-anarace[0]->getTimer())/60 << ":" << setw(2) << (ga->maxTime-anarace[0]->getTimer())%60 << "]";
		msgWindow->addMessage(os.str());
		resetData();
	}

};

void Main::stopOptimizing()
{
	for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		   if(player[i]->isShown())
			player[i]->setOptimizing(false);
};

void Main::startOptimizing()
{
	for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		   if(player[i]->isShown())
				player[i]->setOptimizing(true);
};

const int Main::isOptimizing()
{
	for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
 		   if(player[i]->isOptimizing())
			return(1);
	return(0);
};

void Main::drawGizmo(DC* dc) const
{
	dc->SetFont(UI_Object::theme.lookUpFont(HUGE_DEFAULT_BOLD_FONT));
	int dx=0;int dy=0;
	dc->SetTextForeground(toSDL_Color((0==ani%20)*200+((0==ani%19)+(0==ani%21))*50,(0==ani%20)*200+((0==ani%19)+(0==ani%21))*50,(0==ani%20)*200+((0==ani%19)+(0==ani%21))*100+50));
	dc->DrawText("E",mainWindow->getAbsoluteClientRectPosition()+Point(10,5));
	dc->GetTextExtent("E",&dx,&dy);

	dc->SetTextForeground(toSDL_Color((0==ani%21)*200+((0==ani%20)+(0==ani%22))*50,(0==ani%21)*200+((0==ani%20)+(0==ani%22))*50,(0==ani%21)*200+((0==ani%20)+(0==ani%22))*100+50));
	dc->DrawText("v",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));
	dc->GetTextExtent("Ev",&dx,&dy);

	dc->SetTextForeground(toSDL_Color((0==ani%22)*200+((0==ani%21)+(0==ani%23))*50,(0==ani%22)*200+((0==ani%21)+(0==ani%23))*50,(0==ani%22)*200+((0==ani%21)+(0==ani%23))*100+50));
	dc->DrawText("o",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));
	dc->GetTextExtent("Evo",&dx,&dy);

	dc->SetTextForeground(toSDL_Color((0==ani%23)*200+((0==ani%22)+(0==ani%24))*50,(0==ani%23)*200+((0==ani%22)+(0==ani%24))*50,(0==ani%23)*200+((0==ani%22)+(0==ani%24))*100+50));
	dc->DrawText("l",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));
	dc->GetTextExtent("Evol",&dx,&dy);

	dc->SetTextForeground(toSDL_Color((0==ani%24)*200+((0==ani%23)+(0==ani%25))*50,(0==ani%24)*200+((0==ani%23)+(0==ani%25))*50,(0==ani%24)*200+((0==ani%23)+(0==ani%25))*100+50));
	dc->DrawText("u",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));
	dc->GetTextExtent("Evolu",&dx,&dy);

	dc->SetTextForeground(toSDL_Color((0==ani%25)*200+((0==ani%24)+(0==ani%26))*50,(0==ani%25)*200+((0==ani%24)+(0==ani%26))*50,(0==ani%25)*200+((0==ani%24)+(0==ani%26))*100+50));
	dc->DrawText("t",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));
	dc->GetTextExtent("Evolut",&dx,&dy);

	dc->SetTextForeground(toSDL_Color((0==ani%26)*200+((0==ani%25)+(0==ani%27))*50,(0==ani%26)*200+((0==ani%25)+(0==ani%27))*50,(0==ani%26)*200+((0==ani%25)+(0==ani%27))*100+50));
	dc->DrawText("i",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));
	dc->GetTextExtent("Evoluti",&dx,&dy);

		dc->SetTextForeground(toSDL_Color((0==ani%27)*200+((0==ani%26)+(0==ani%28))*50,(0==ani%27)*200+((0==ani%26)+(0==ani%28))*50,(0==ani%27)*200+((0==ani%26)+(0==ani%28))*100+50));
	dc->DrawText("o",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));
	dc->GetTextExtent("Evolutio",&dx,&dy);

	dc->SetTextForeground(toSDL_Color((0==ani%28)*200+((0==ani%27)+(0==ani%29))*50,(0==ani%28)*200+((0==ani%27)+(0==ani%29))*50,(0==ani%28)*200+((0==ani%27)+(0==ani%29))*100+50));
	dc->DrawText("n",mainWindow->getAbsoluteClientRectPosition()+Point(10+dx,5));

	dc->SetTextForeground(toSDL_Color(25,25,85));
	dc->DrawText("Forge",mainWindow->getAbsoluteClientRectPosition()+Point(50,45));
	dc->SetTextForeground(toSDL_Color(0,0,85));
	dc->DrawText("v1.12",mainWindow->getAbsoluteClientRectPosition()+Point(154,84));
	dc->SetTextForeground(toSDL_Color(50,50,85));
	dc->DrawText("v1.12",mainWindow->getAbsoluteClientRectPosition()+Point(150,80));
};

void Main::draw(DC* dc) const
{
	SDL_BlitSurface(*UI_Object::theme.lookUpBitmap(BACKGROUND_BITMAP) , 0, dc->GetSurface(), 0 );


	if(mainWindow->isShown())
	{
		mainWindow->draw(dc);
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
};

										
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
	if(refresh>5)
		refresh=0;		
}

void Main::Init(DC* dc)
{
	UI_Object::theme.loadDataFiles("settings/ui/default.ui","data/bitmaps","data/fonts",dc);
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	settings.loadHarvestFile("settings/harvest/default.hvt");
	// harvest file VOR startcondition file!!!!!!!!!!----------------
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
//	settings.setStartRace( ...
	settings.setHarvestSpeed(TERRA, 0);
	settings.setHarvestSpeed(PROTOSS, 1);
	settings.setHarvestSpeed(ZERG, 2);

//	settings.assignHarvestSpeedToStartcondition();
	
	settings.setStartRace(1, PROTOSS);
	settings.setStartRace(2, PROTOSS);

	settings.setStartcondition(1, 1);
	settings.setStartcondition(2, 1);

	settings.setStartPosition(1, 1);
	settings.setStartPosition(2, 7);
	settings.fillGroups();

	settings.setGoal(1, 0);
	settings.setGoal(2, 1); //~~

//	settings.assignStartconditionHarvestSpeed();
		
	settings.initSoup(); // assign START of settings
	// initializes players, initializes Map
	
//	for(int i=1;i<settings.getMap(0)->getMaxPlayer();i++)
//		settings.setGoal(0, i);
	
	//setMap VOR initsoup um v.a. die Zahl der Spieler zu bestimmen

// MAPS: Startforce
// Goals: 
	
// Goal in "goal.txt" is now goal[0]
// assign goal 0 to all players
//GOALS are set by setMap, each startPlayer has now its own goal entry
	
	update=0;
// initialize the soup, set the parameters, load the players etc.
	ga=settings.getGa();
	oldrun=0;
//	grey=0;
	resetData();
	if(ANARACE** temp=settings.newGeneration(anarace))
	{
		for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
			anarace[i]=temp[i];
		update=2;
//		if(anarace[0]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO?
	};

	UI_Object::assignStartTime();
	
	mainWindow=new MainWindow();
	ostringstream os;
	os << "[GUI: " << setprecision(2) << GUI_VERSION << "] [CORE: " << setprecision(2) << CORE_VERSION << "]";
	 mainWindow->setTitleParameter(os.str());
	
	msgWindow=new MessageWindow(mainWindow);
	theCore=new CoreWindow(mainWindow);
	tutorialWindow=new TutorialWindow(mainWindow);

//TODO: scc2 player und scc2dll player Zusammenhang nachschaun! loadPlayer wird net aufgerufen... goals ueberschneiden etc...

	for(int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
		player[i]=new Player(mainWindow, &(anarace[i]), BASIC_PLAYER_MODE, i);

//	haxor=new UI_WindowtoSDL_Color(255,0,0), Color(5,25,0), Color(40,150,20), Rect(0,0,550,100),0,boGraphWindow->getRelativeLeftBound(),boGraphWindow->getLowerBound()+10,0);
//	haxor->setTitle(0,"H4Xx0r 57uff! :D");

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
};

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
 	SDL_Rect c;c.x=0;c.y=0;c.w=1280;c.h=1024;
    if (FULLSCREEN==1)
        screen=new DC(SDL_SetVideoMode(c.w, c.h, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN));
    else
        screen=new DC(SDL_SetVideoMode(c.w, c.h, 32, SDL_HWSURFACE|SDL_DOUBLEBUF));
    if ( screen == NULL )
    {
		printf("error\n");
        return(1);
    }

	Main m;
	m.Init(screen);

	while(true)
	{
	
		m.process();
		m.draw(screen);
		m.OnIdle();

		SDL_UpdateRect(screen->GetSurface(), 0, 0, 0, 0);
		SDL_SetClipRect(screen->GetSurface(), &c);	
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
	            case SDL_QUIT:return(0);break;
	            case SDL_KEYDOWN:
							  printf("key [%i, %i]\n", c.x, c.y);
							  switch(event.key.keysym.sym)
							  {
									case SDLK_ESCAPE:return(0);break;
									case SDLK_SPACE:
									 {
										if(m.isOptimizing())
											m.stopOptimizing();
										else m.startOptimizing();
										};break;
									case SDLK_RETURN:
									 if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))
											SDL_WM_ToggleFullScreen(screen->GetSurface());
									 break;
									case SDLK_UP:if(c.y>vy/10) c.y-=vy/10;else c.y=0;break;
									case SDLK_DOWN:if(c.y+c.h<vy-vy/10) c.y+=vy/10;else c.y=vy-c.h;break;
									case SDLK_LEFT:if(c.x>vx/10) c.x-=vx/10;else c.x=0;break;
									case SDLK_RIGHT:if(c.x+c.w<vx-vx/10) c.x+=vx/10;else c.x=vx-c.w;break;
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
};
					
/*					
void Main::OnMouseWheelScroll(MouseEvent& event)
{
    controls.scrollMouseWheel(event.GetWheelRotation());
}*/
                                                                                                                                                            
