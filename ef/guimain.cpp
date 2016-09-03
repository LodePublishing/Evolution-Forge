#include "guimain.hpp"
#include "../ui/editfield.hpp"
#include "../ui/endrundialog.hpp"
#include "../ui/tooltip.hpp"


//int gizmor;
//int grey; //0-100%

Main::Main(DC* dc):
	mainWindow(NULL),
	msgWindow(NULL),
	infoWindow(NULL),
	tutorialWindow(NULL),
	theCore(NULL),
	settingsWindow(NULL),
	maus(),
//	buttonPressed(false),
//	hasAlreadyLeft(false),
//	button(NULL),
	oldrun(0),
	endrun(false),
	gizmo(true),
	update(0),
	refresh(0),
	ani(1),
	ani2(0),
	drawing(true)
{
	UI_Object::theme.setTab(BASIC_TAB);
	resetData(); // TODO 
	for(int i=0;i<MAX_PLAYER;i++)
		player[i]=NULL;
	toLog(*UI_Object::theme.lookUpString(START_LOAD_UI_BITMAPS_FONTS_STRING));
#ifdef __linux__
	UI_Object::theme.loadDataFiles("settings/ui/default.ui","data/bitmaps/","data/fonts/",dc);
	bar->draw(dc, 20, START_UI_BITMAPS_FONTS_LOADED_STRING);
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	bar->draw(dc, 2, START_LOAD_HARVEST_STRING);
	settings.loadHarvestFile("settings/harvest/default.hvt");
	
	bar->draw(dc, 2, START_LOAD_MAPS_STRING);
	settings.loadMapFile("settings/maps/lt42.map");
	bar->draw(dc, 4, START_LOAD_STARTCONDITIONS_STRING);
	settings.assignMap(0); // first map (lt) and ums = false
	settings.loadStartconditionFile("settings/start/default_terra.start");
	settings.loadStartconditionFile("settings/start/default_protoss.start");
	settings.loadStartconditionFile("settings/start/default_zerg.start");
#elif __WIN32__
	UI_Object::theme.loadDataFiles("settings\\ui\\default.ui","data\\bitmaps\\","data\\fonts\\",dc);
	bar->draw(dc, 20, START_UI_BITMAPS_FONTS_LOADED_STRING);
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	bar->draw(dc, 2, START_LOAD_HARVEST_STRING);
	settings.loadHarvestFile("settings\\harvest\\default.hvt");
	
	bar->draw(dc, 2, START_LOAD_MAPS_STRING);
	settings.loadMapFile("settings\\maps\\lt42.map");
	bar->draw(dc, 4, START_LOAD_STARTCONDITIONS_STRING);
	settings.assignMap(0); // first map (lt) and ums = false
	settings.loadStartconditionFile("settings\\start\\default_terra.start");
	settings.loadStartconditionFile("settings\\start\\default_protoss.start");
	settings.loadStartconditionFile("settings\\start\\default_zerg.start");
#endif
	

	bar->draw(dc, 12, START_LOAD_GOALS_STRING);
	loadGoals();

// goal beschreibt Rasse, Ziele und Modus
	
	bar->draw(dc, 8, START_ASSIGN_AND_ANALYZE_STRING);
// Map in "map.txt" is now map[0]
// choose the first map we loaded (map[0])
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
	// TODO FEHLERMELDUNG FALLS FALSCHES GOAL ZU FALSCHER RASSE
//	settings.assignStartconditionHarvestSpeed();
	bar->draw(dc, 2, START_PREPARE_FIRST_RUN_STRING);
	settings.assignRunParametersToSoup(); // assign START and GA of settings to soup
	// initializes players, initializes Map
// initialize the soup, set the parameters, load the players etc.
//	grey=0;
	if(ANARACE** temp=settings.newGeneration(anarace))
	{
		for(unsigned int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
			anarace[i]=temp[i];
	}

	anarace[0]->setMaxpFitness(0);

	UI_Object::assignStartTime();
	
	bar->draw(dc, 2, START_INIT_GUI_STRING);
	mainWindow = new MainWindow();

	theCore = new CoreWindow(mainWindow);
	tutorialWindow = new TutorialWindow(mainWindow);
	settingsWindow = new SettingsWindow(mainWindow);
	msgWindow = new MessageWindow(mainWindow);

//TODO: scc2 player und scc2dll player Zusammenhang nachschaun! loadPlayer wird net aufgerufen... goals ueberschneiden etc...

/*	for(unsigned int i=0;i<settings.getMap(0)->getMaxPlayer();i++)
	{
		player[i] = new Player(mainWindow, &(anarace[i]), msgWindow, i);
	}
	for(unsigned int i = settings.getMap(0)->getMaxPlayer();i<MAX_PLAYER;i++)
		player[i]=0;*/

	player[0] = new Player(mainWindow, &(anarace[0]), msgWindow, 0);
	player[1] = new Player(mainWindow, &(anarace[1]), msgWindow, 1);
	player[2] = 0;

	msgWindow->setParent(mainWindow); // process AFTER player
	
//TODO grey wieder rein... evtl bei draw

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
	bar->draw(dc, 8, START_MAIN_INIT_COMPLETE_STRING);
//	cursor=init_system_cursor(arrow);
//	SDL_ShowCursor(SDL_DISABLE);
//	cursor = (UI_Object::theme.lookUpBitmap(MAUS_BITMAP))->getSurface();

//  SDL_SetColorKey(cursor, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(cursor->format, 255, 255, 255));
			
//	cursor_save = *UI_Object::theme.lookUpBitmap(MAUS_BITMAP);
	
//	get_bg(dc, cursor_save, 0, 0);
//    SDL_SetAlpha(cursor, SDL_SRCALPHA, 127);
	mainWindow->forcePressTab(BASIC_TAB); // !!
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

void Main::resetData()
{
	refresh=0;
	endrun=false;
//	gizmor=rand()%GIZMO_NUMBER;
	for(int i=MAX_PLAYER;i--;)
		anarace[i]=NULL;
	update=0;
	drawing=true;
}

void Main::noticeFullscreen()
{
	settingsWindow->updateItems();
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
	if((configuration.isTooltips())&&(UI_Object::tooltip))
		UI_Object::tooltip->process();
	mainWindow->process();
	if(settingsWindow->hasLanguageChanged())
	{
		player[0]->reloadStrings();
		player[1]->reloadStrings();
		mainWindow->reloadStrings();
		settingsWindow->reloadStrings();
		msgWindow->addMessage(*UI_Object::theme.lookUpString(LANGUAGE_HAS_CHANGED_STRING));
	}

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
				settingsWindow->updateItems();
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
		
/*		settings.initSoup();
		resetData();*/
		update=2;
	} // end tabwasChanged

	for(int i=0;i<2;i++)
		player[i]->checkForChange();
	
	settings.checkForChange();

	if(endrun) // TODO!
	{
		if(configuration.isAutoSaveRuns())
		{
			ostringstream os;
			os << anarace[0]->getRun() << *UI_Object::theme.lookUpString(ENDRUN_FINISHED_STRING) << " [" << setw(2) << (anarace[0]->getRealTimer())/60 << ":";
			if(anarace[0]->getRealTimer()<10)
				os << "0";
			os << setw(2) << (anarace[0]->getRealTimer())%60 << "]";
			msgWindow->addMessage(os.str());
	        settings.saveBuildOrder(UI_EndRunDialog::getLastString(), anarace[0]); // TODO, evtl alle saven...
			// TODO Name muss mitangegeben werden...
			// Dann ist auch die Frage wie die Ergebnisse gespeichert werden sollen, klar, nach Rasse sortiert 
			// Voreinstellung fuer Editfeld ist letzter benutzter Name  (oder 'leer') + fortlaufende Zahl (Reset bei neuem Namen)
			// Spaeter: ueberschreiben pruefen
			// In DateiName evtl auch Zeit einbauen
			msgWindow->addMessage(*UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
//			resetData();

//TODO timer window updaten
			endrun=false;
		} else
		{
			if(UI_Object::editTextField==NULL)
				UI_Object::editTextField = new UI_EndRunDialog(mainWindow, "BLA");
				// Dialog aufmachen: Weiterrechnen oder neubeginnen? (checkbox: nicht mehr fragen) Neuanfang kann u.U. einen neuen Weg ermoeglichen und u.U. ein besseres Ergebnis bringen... Abspeichern Checkbox, EXIT, CONTINUE, NEW ROUND...
		// you may want to use 'comparison' (not availible yet) to show all results 
			else
			{
				if((UI_Object::editTextField->isDone())&&(UI_Object::editTextField->getCaller()==NULL))
				{
					drawing=true;
					if(UI_Object::editTextField->getString().length()>0)
					{
			            ostringstream os;
    	    		    os << anarace[0]->getRun() << *UI_Object::theme.lookUpString(ENDRUN_FINISHED_STRING) << " [" << setw(2) << (anarace[0]->getRealTimer())/60 << ":";
			            if(anarace[0]->getRealTimer()<10)
        			        os << "0";
		        	    os << setw(2) << (anarace[0]->getRealTimer())%60 << "]";
        		    	msgWindow->addMessage(os.str());
			            settings.saveBuildOrder(UI_Object::editTextField->getString(), anarace[0]); // TODO, evtl alle saven...
    	    		    msgWindow->addMessage(*UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
			//          resetData();
					}
        		    endrun=false;
					delete UI_Object::editTextField;
					UI_Object::resetButton();
					UI_Object::editTextField=NULL;
				} else
				if((UI_Object::editTextField->isCanceled())&&(UI_Object::editTextField->getCaller()==NULL))
				{
					drawing=true;
					delete UI_Object::editTextField;
					UI_Object::resetButton();
					UI_Object::editTextField=NULL;
					endrun=false;
					//resetData();??
				}
			}
		}
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
		if((player[i]->isShown())&&(player[i]->isOptimizing()))
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
		s = helper(dc, s.GetWidth(), i, str.substr(0, i+1));

	dc->SetTextForeground(toSDL_Color(25, 25, 85));
	dc->DrawText("Forge", mainWindow->getAbsoluteClientRectPosition() + Point(50, 58));
	dc->SetTextForeground(toSDL_Color(0,0,85));
	ostringstream os;
	os << "v1." << CORE_VERSION << " beta test";
	dc->DrawText(os.str(), mainWindow->getAbsoluteClientRectPosition()+Point(78, 98));
	dc->SetTextForeground(toSDL_Color(50, 50, 85));
	dc->DrawText(os.str(), mainWindow->getAbsoluteClientRectPosition()+Point(75, 95));
}

void Main::draw(DC* dc) const
{
	if(mainWindow->isShown())
	{
		mainWindow->draw(dc);
// ------ MOUSE DRAWING ------
/*		if(configuration.isSoftwareMouse())
		{
			SDL_ShowCursor(SDL_DISABLE);
			Point p = UI_Object::mouse - Size(20,10);//Point(90, 140);
			dc->SetFont(UI_Object::theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
			switch(UI_Object::mouseType)
			{
				case 0:dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_NONE), p);
					break;
				case 1:
					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_LEFT), p);
					dc->SetTextForeground(toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
				break;
				case 2:
					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_RIGHT), p);
					dc->SetTextForeground(toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				case 3:
					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_BOTH), p);
					dc->SetTextForeground(toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
					dc->SetTextForeground(toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
			}
		}
		else
			SDL_ShowCursor(SDL_ENABLE);*/
// ------ END MOUSE DRAWING ------
	
		if(UI_Object::tooltip)
			UI_Object::tooltip->draw(dc);
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
	if((!UI_Object::editTextField/*endrun*/)&&(isOptimizing()))
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

}

void Main::loadGoals()
{
	for(int i = 0; i < MAX_RACES; i++)
	{
		list<string> goalFiles = settings.findFiles("settings", "goals", raceString[i]);
		for(list<string>::iterator j = goalFiles.begin(); j!=goalFiles.end(); j++)
			settings.loadGoalFile(*j);
	}
//TODO flag setzen ob was geladen wurde
}

void Main::leftDown()
{
	if(UI_Object::currentButton)
	{
		UI_Object::currentButton->mouseLeftButtonPressed();
		UI_Object::currentButtonPressed=true;
	}
}

void Main::leftUp(const Point p)
{
	if((UI_Object::currentButton)&&(UI_Object::currentButtonPressed))
		UI_Object::currentButton->mouseLeftButtonReleased();
	UI_Object::currentButtonPressed=false;
	UI_Object::currentButtonHasAlreadyLeft=false;
	setMouse(p);
}

void Main::rightDown()
{
	if(UI_Object::currentButton)
	{
		UI_Object::currentButton->mouseRightButtonPressed();
		UI_Object::currentButtonPressed=true;
	}
}

void Main::rightUp(const Point p)
{
	if((UI_Object::currentButton)&&(UI_Object::currentButtonPressed))
		UI_Object::currentButton->mouseRightButtonReleased();
	UI_Object::currentButtonPressed=false;
	UI_Object::currentButtonHasAlreadyLeft=false;
	setMouse(p);
}

void Main::setMouse(const Point p)
{
//	if(p == UI_Object::mouse)
//		return;
	maus=p;
	UI_Object::mouse=p;
	((BoGraphWindow*)(player[0]->window[BO_GRAPH_WINDOW]))->mouseHasMoved();
//	else if(player[1]->window[BO_GRAPH_WINDOW]->Inside(p))
//		(BoGraphWindow*)(player[1]->window[BO_GRAPH_WINDOW])->mouseHasMoved();
	
	if(UI_Object::currentButtonHasAlreadyLeft)
	{
		if((UI_Object::currentButton)&&(UI_Object::currentButton->getAbsoluteRect().Inside(p)))
		{
			UI_Object::currentButton->mouseHasEnteredArea();
			UI_Object::currentButtonHasAlreadyLeft=false;
		}
		else return;
	}
	if((UI_Object::currentButton)&&(!UI_Object::currentButton->getAbsoluteRect().Inside(p)))
	{
		UI_Object::currentButton->mouseHasLeftArea();
		if(!UI_Object::currentButtonPressed)
			UI_Object::currentButton=0;
		else
			UI_Object::currentButtonHasAlreadyLeft=true;
	}
// ignore mousemove if button is still pressed		
	if(UI_Object::currentButton)
	{
		if(UI_Object::currentButtonPressed)
		{
			UI_Object::currentButton->forceHighlighted();
//			UI_Object::currentButton->mouseHasMoved();
		}
		return;
	}
	UI_Object::currentButton=NULL;
	if(UI_Object::editTextField==NULL)
	{
		UI_Object::currentButton = (UI_Button*) (player[0]->checkHighlight());
		if(!UI_Object::currentButton)
			UI_Object::currentButton = (UI_Button*) (player[1]->checkHighlight());
		if(!UI_Object::currentButton)
			UI_Object::currentButton = (UI_Button*) (mainWindow->checkHighlight());
		if(!UI_Object::currentButton)
			UI_Object::currentButton = (UI_Button*) (settingsWindow->checkHighlight());
	} else
	if((!UI_Object::currentButton)&&(UI_Object::editTextField!=NULL))
		UI_Object::currentButton = (UI_Button*) (UI_Object::editTextField->checkHighlight());
	if(UI_Object::currentButton)
	{
		UI_Object::currentButton->mouseHasEnteredArea();
		UI_Object::currentButtonHasAlreadyLeft=false;
	}
	if(configuration.isTooltips())
	{
//		UI_Object* temp=UI_Object::toolTipParent;
		UI_Object* temp2;
		UI_Object::toolTipParent=NULL;
        temp2 = player[0]->checkTooltip();
        if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
            UI_Object::toolTipParent = temp2;
		temp2=NULL;
		if(UI_Object::toolTipParent==NULL)
        	temp2 = player[1]->checkTooltip();
        if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
            UI_Object::toolTipParent = temp2;
		temp2=NULL;
		if(UI_Object::toolTipParent==NULL)
        	temp2 = mainWindow->checkTooltip();
        if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
            UI_Object::toolTipParent = temp2;
		temp2=NULL;
		if(UI_Object::toolTipParent==NULL)
        	temp2 = settingsWindow->checkTooltip();
        if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
            UI_Object::toolTipParent = temp2;
//    	if((UI_Object::toolTipParent!=temp)//||(UI_Object::tooltip==NULL))
// TODO
	    {
			delete UI_Object::tooltip;
			if(/*(temp!=NULL)&&*/(UI_Object::toolTipParent==NULL))
				UI_Object::tooltip=NULL;
			else
				UI_Object::tooltip=new UI_Tooltip(NULL/*UI_Object::toolTipParent*/, (UI_Object::toolTipParent)->getToolTipString());
		}
    } else if(UI_Object::tooltip)
	{
		delete UI_Object::tooltip;
		UI_Object::tooltip=NULL;
	}
}


ProgressBar* Main::bar;

