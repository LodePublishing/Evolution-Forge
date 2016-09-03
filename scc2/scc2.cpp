#include "../scc2dll/location.h"
#include "scc2.h"
#include "../scc2dll/race.h"
#include "../scc2dll/anarace.h"
#include "math.h"

IMPLEMENT_APP(MyApp)

/*BEGIN_EVENT_TABLE(MyFrame, wxFrame)
//	EVT_MENU(EF_Open,  MyFrame::OnOpen)
	EVT_MENU(EF_Start,  MyFrame::OnStart)
	EVT_MENU(EF_Stop,  MyFrame::OnStop)
	EVT_MENU(EF_Quit,  MyFrame::OnQuit)
	EVT_MENU(EF_About, MyFrame::OnAbout)
	EVT_MENU(EF_GeneralSettings, MyFrame::OnGeneralSettings)
	EVT_MENU(EF_GoalCreate,  MyFrame::OnGoalCreate)
	EVT_MENU(EF_GoalImport,  MyFrame::OnGoalImport)
	EVT_MENU(EF_MapCreate,  MyFrame::OnMapCreate)
	EVT_MENU(EF_MapImport, MyFrame::OnMapImport)
END_EVENT_TABLE()*/

BEGIN_EVENT_TABLE(MyDCWindow, wxWindow)
	EVT_ERASE_BACKGROUND(MyDCWindow::OnEraseBackground)
	EVT_IDLE(MyDCWindow::OnIdle)
	EVT_PAINT(MyDCWindow::OnPaint)
	EVT_MOTION(MyDCWindow::OnMouseMove)
	EVT_LEFT_UP(MyDCWindow::OnMouseLeftUp)
	EVT_LEFT_DOWN(MyDCWindow::OnMouseLeftDown)
	EVT_RIGHT_UP(MyDCWindow::OnMouseRightUp)
	EVT_RIGHT_DOWN(MyDCWindow::OnMouseRightDown)
	EVT_MOUSEWHEEL(MyDCWindow::OnMouseScroll)
	EVT_KEY_DOWN(MyDCWindow::OnKeyDown)
	EVT_KEY_UP(MyDCWindow::OnKeyUp)
	EVT_TIMER(-1,MyDCWindow::OnTimer)
END_EVENT_TABLE()


int SCREEN_X;
int SCREEN_Y;
int oldy;

int gizmor;

const int BASIC=0;
const int ADVANCED=1;
const int EXPERT=2;
const int GOSU=3;
const int TRANSCENDEND=4;
const int MAP=5;
const int SETTINGS=6;
const int TUTORIAL=7;

int grey; //0-100%

int animationNumbers;

void MyDCWindow::resetData()
{
	refresh=0;
	endrun=0;
	gizmor=rand()%GIZMO_NUMBER;
	for(int i=0;i<MAX_PLAYER;i++)
		anarace[i]=NULL;
	update=0;mouseLeft=0;
	userIsNewbie=1;
	tutorialChapter=120;
}



MyDCWindow::MyDCWindow(wxFrame *parent)
	:wxWindow(parent, -1)
{
#ifdef __WIN32__
	GraphixScrollWindow::font=wxFont(FONT_SIZE,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
	GraphixScrollWindow::font2=wxFont(FONT_SIZE,wxDEFAULT,wxFONTSTYLE_ITALIC,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
#else
	GraphixScrollWindow::font=wxFont(FONT_SIZE,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
	GraphixScrollWindow::font2=wxFont(FONT_SIZE,wxDECORATIVE,wxFONTSTYLE_ITALIC,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
#endif
	GraphixScrollWindow::font4=wxFont(18,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
	GraphixScrollWindow::font3=wxFont(32,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);

	GraphixScrollWindow::font5=wxFont(24,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
	GraphixScrollWindow::font6=wxFont(12,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
	GraphixScrollWindow::font7=wxFont(16,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
		
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	GraphixScrollWindow::settings.loadHarvestFile("mining.txt");
	GraphixScrollWindow::settings.loadSettingsFile("settings.txt");
// Map in "map.txt" is now map[0]

	GraphixScrollWindow::settings.loadMapFile("LostTemple",USE_MAP_SETTINGS);
	GraphixScrollWindow::settings.loadMapFile("LostTemple",TOP_VS_BOTTOM);
// choose the first map we loaded (map[0])
	GraphixScrollWindow::settings.setCurrentMap(0);

// MAPS: Startforce
// Goals: 
	
// Goal in "goal.txt" is now goal[0]
	GraphixScrollWindow::settings.loadGoalFile("goalp1.txt");
	GraphixScrollWindow::settings.loadGoalFile("goalp0.txt");
	GraphixScrollWindow::settings.loadGoalFile("goalp0.txt");
	GraphixScrollWindow::settings.loadGoalFile("goalp0.txt");
	GraphixScrollWindow::settings.loadGoalFile("goalp0.txt");
	GraphixScrollWindow::settings.loadGoalFile("goalp0.txt");
// assign goal 0 to all players
	for(int i=1;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer();i++)
		GraphixScrollWindow::settings.setGoal(0,i);
	update=0;
// initialize the soup, set the parameters, load the players etc.
	GraphixScrollWindow::settings.initSoup();
	ga=GraphixScrollWindow::settings.getGa();
	oldrun=0;
	grey=0;
	resetData();
	if(ANARACE** temp=GraphixScrollWindow::settings.newGeneration(anarace))
	{
		for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
			anarace[i]=temp[i];
		update=2;
//		if(anarace[0]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO?
	};

	mainWindow=new GraphixScrollWindow(1,wxRect(0,0,SCREEN_X,SCREEN_Y),wxRect(0,0,SCREEN_X,SCREEN_Y),NOT_SCROLLED,0,TABBED);
	mainWindow->setTitle(SCREEN_X/2,_T(wxString::Format(wxT("Evolution Forge v.%i "),EF_Version)));
	mainWindow->addTab(10,_T("Basic"));
	mainWindow->addTab(130,_T("Advanced"));
	mainWindow->addTab(250,_T("Expert"));
	mainWindow->addTab(370,_T("Gosu >_<"));
	mainWindow->addTab(490,_T("Transcend"));
    mainWindow->addTab(610,_T("Map&Goals"));
	mainWindow->addTab(730,_T("Settings"));
	mainWindow->addTab(890,_T("Tutorial"));

	msgWindow=new MessageWindow(wxRect(mainWindow->getInnerPosition()+wxPoint(0,mainWindow->getInnerHeight()-125),wxSize(FIRST_COLOUMN,120)),wxRect(mainWindow->getInnerLeftBound(),mainWindow->getInnerLeftBound()+mainWindow->getInnerHeight()-125,FIRST_COLOUMN,120));
msgWindow->setTitle(0,"Messages");

	theCore=new GraphixScrollWindow(0,wxRect(FIRST_COLOUMN+5,mainWindow->getInnerUpperBound()+SECOND_ROW,SECOND_COLOUMN,220),wxRect(FIRST_COLOUMN+5,mainWindow->getInnerUpperBound()+SECOND_ROW,SECOND_COLOUMN,220),NOT_SCROLLED);
	theCore->setTitle(0,"The Core of Evolution Forge");

	tutorialWindow=new GraphixScrollWindow(0,wxRect(mainWindow->getInnerWidth()-theCore->getWidth(),SECOND_ROW,theCore->getWidth(),500),wxRect(mainWindow->getInnerLeftBound(),mainWindow->getInnerUpperBound(),mainWindow->getInnerWidth(),500),NOT_SCROLLED,0,TABBED);
	tutorialWindow->setTitle(0,"Evolution Forge Tutorial");

	Player::InitPositions(mainWindow);

//TODO: scc2 player und scc2dll player Zusammenhang nachschaun! loadPlayer wird net aufgerufen... goals ueberschneiden etc...

	for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
		player[i]=new Player(&(anarace[i]),i);

//	haxor=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,550,100),0,boGraphWindow->getLeftBound(),boGraphWindow->getLowerBound()+10,0);
//	haxor->setTitle(0,"H4Xx0r 57uff! :D");

//TODO grey wieder rein... evtl bei draw

	mainWindow->Show(1);
	msgWindow->Show(0);
	theCore->Show(0);
	tutorialWindow->Show(0);
	mainWindow->currentTab=0;

	player[0]->Show(1);
	player[1]->Show(0);

	msgWindow->addMessage(_T("Welcome to Evolution Forge!"));
	msgWindow->addMessage(_T("Click above to add new goals."));
	msgWindow->addMessage(_T(wxString::Format(wxT("%i players loaded..."),GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1))),

	GraphixScrollWindow::bmpClawsoftware.LoadFile("clawsoftware.bmp", wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpClemens.LoadFile("clemens.bmp", wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpCancel.LoadFile("cancel.bmp", wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpAdd.LoadFile("add.bmp",wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpSub.LoadFile("sub.bmp",wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpArrowLeft.LoadFile("arrow_left.bmp",wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpArrowUp.LoadFile("arrow_up.bmp",wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpArrowRight.LoadFile("arrow_right.bmp",wxBITMAP_TYPE_BMP);
	GraphixScrollWindow::bmpArrowDown.LoadFile("arrow_down.bmp",wxBITMAP_TYPE_BMP);

/*	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+440,theCore->getInnerUpperBound()+50,8,8),bmpAdd,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+440,theCore->getInnerUpperBound()+60,8,8),bmpSub,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+440,theCore->getInnerUpperBound()+70,8,8),bmpCancel,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+50,8,8),bmpAdd,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+60,8,8),bmpSub,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+70,8,8),bmpCancel,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+150,8,8),bmpAdd,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+160,8,8),bmpSub,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+170,8,8),bmpCancel,1);*/

//	wxtimer.SetOwner(this);
//	wxtimer.Start(200);
	dc=0;
};

MyDCWindow::~MyDCWindow()
{
//	wxtimer.Stop();
};

void MyDCWindow::OnEraseBackground(wxEraseEvent& event)
{
};

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{
//	msgBox=NULL;
	SetIcon(wxICON(icon));

#if wxUSE_MENUS
//	menuFile=new wxMenu;
/*	menuHelp=new wxMenu;	
	menuSettings=new wxMenu;
	menuGoal=new wxMenu;
	menuMap=new wxMenu;
	menuAdvanced=new wxMenu;
	menuHelp->Append(EF_About, _T("&About...\tF1"), _T("Show about dialog"));
	menuFile->Append(EF_Open, _T("&Open\tAlt-O"), _T("Load Build Order"));
//evtl nen strich mit save/load
/*      menuFile->Append(EF_Open, _T("&Open\tAlt-O"), _T("Load Population"));
	menuFile->Append(EF_Open, _T("&Open\tAlt-O"), _T("Save Build Order"));
	menuFile->Append(EF_Open, _T("&Open\tAlt-O"), _T("Save Population"));*/
//	menuFile->Append(EF_Start, _T("&Start\tAlt-S"), _T("Start/Continue calculation"));
//	menuFile->Append(EF_Stop, _T("S&top\tAlt-T"), _T("Stop calculation"));
//	menuFile->Append(EF_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
/*	menuSettings->Append(EF_GeneralSettings, _T("&General\tAlt-G"), _T("General settings"));
	menuGoal->Append(EF_GoalCreate, _T("C&reate\tAlt-R"),_T("Create new goal"));
//	menuGoal->Append(EF_GoalCreate, _T("C&reate\tAlt-R"),_T("Edit existing goal"));
	menuGoal->Append(EF_GoalImport, _T("&Import\tAlt-I"),_T("Import goal file"));
	menuMap->Append(EF_MapCreate, _T("&Create\tAlt-C"),_T("Create new map"));
//	menuGoal->Append(EF_GoalCreate, _T("C&reate\tAlt-R"),_T("Edit existing map"));
	menuMap->Append(EF_MapImport, _T("I&mport\tAlt-M"),_T("Import map file"));*/
//	wxMenuBar *menuBar = new wxMenuBar();
//	menuBar->Append(menuFile, _T("&File"));
/*	menuBar->Append(menuSettings, _T("&Settings"));
	menuBar->Append(menuGoal, _T("G&oals"));
	menuBar->Append(menuMap, _T("&Maps"));
	menuBar->Append(menuAdvanced, _T("&Advanced"));
	menuBar->Append(menuHelp, _T("&Help"));*/
//	SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
//	CreateStatusBar(2);
//	SetStatusText(_T("Click on File/Start to start the calculation!"));
#endif // wxUSE_STATUSBAR

//	ShowFullScreen(true,wxFULLSCREEN_NOBORDER);
	
	child=new MyDCWindow(this);
}

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	SCREEN_X=1024-4;SCREEN_Y=768-55;
	MyFrame *frame = new MyFrame(_T("Evolution Forge v1.11"), wxPoint(0, 0), wxSize(SCREEN_X,SCREEN_Y));
	frame->Show(TRUE);
	return TRUE;
}



void MyDCWindow::showToolTip()
{
/*	int what=0;
	if(boWindow->getButtonRect(0).Inside(mouseX,mouseY))
		what=1;
	else if(boWindow->getButtonRect(1).Inside(mouseX,mouseY))
		what=2;
	dc->SetFont(GraphixScrollWindow::font2);
	dc->SetTextForeground(wxColour(255,40,40));
	dc->DrawText(_T("Fitness"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+8);
	switch(what)
	{
		case 1:dc->DrawText(_T("Mutation will affect every part of the build order"),mouseX,mouseY);break; //~~
		case 2:dc->DrawText(_T("Mutation will affect only selected parts"),mouseX,mouseY);break;
		default:break;
	};*/
}


void MyDCWindow::processButtons()
{
	mainWindow->checkButtons();
	for(int i=0;i<TUTORIAL+1;i++) //always show main window
		if(mainWindow->isActivated(i))
		{
			update=2;
			player[0]->resetData();
			player[1]->resetData();
			mainWindow->currentTab=i;
			mainWindow->clearButtons();
			switch(mainWindow->currentTab)
			{
				case BASIC:
					player[0]->Show(1);
					player[1]->Show(0);
					 //1 player
					msgWindow->Show(0);
					theCore->Show(0);
					tutorialWindow->Show(0);
					GraphixScrollWindow::settings.setGoal(1,1); // share same goal with advanced
					GraphixScrollWindow::settings.setGoal(0,2); //zero goal!
				break;
				case ADVANCED: //1 player
					player[0]->Show(2);
					player[1]->Show(0);
					msgWindow->Show(0);
					theCore->Show(1);
					tutorialWindow->Show(0);
                                        GraphixScrollWindow::settings.setGoal(1,1); //share same goal with basic
                                        GraphixScrollWindow::settings.setGoal(0,2); //zero goal!
				break;
				case EXPERT: //2 player rushversuche
					player[0]->Show(3);
					player[1]->Show(4);
					msgWindow->Show(0);
					theCore->Show(0);
					tutorialWindow->Show(0);
                                        GraphixScrollWindow::settings.setGoal(1,1); // share same goal as advanced
                                        GraphixScrollWindow::settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				case GOSU: // 2 player - Spieler spielt
					player[0]->Show(5);
					player[1]->Show(6);
					msgWindow->Show(0);
					theCore->Show(0);
					tutorialWindow->Show(0);
// TODO wenn zero goal -> absturz beim aendern
                                        GraphixScrollWindow::settings.setGoal(0,1); // zero goal! -> human
                                        GraphixScrollWindow::settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				case TRANSCENDEND: // 2 player - 2 Computer
					player[0]->Show(7); //~~
					player[1]->Show(6);
					msgWindow->Show(0);
					theCore->Show(0);
					tutorialWindow->Show(0);
                                        GraphixScrollWindow::settings.setGoal(0,1); // zero goal!  -> computer!
                                        GraphixScrollWindow::settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				case TUTORIAL:
					player[0]->Show(1);
					player[1]->Show(0);
					player[0]->boWindow->Show(0);
					player[0]->forceWindow->Show(0);
					player[0]->timerWindow->Show(0);
						
					msgWindow->Show(0);
					theCore->Show(0);			
					tutorialWindow->Show(1);
                                        GraphixScrollWindow::settings.setGoal(1,1); // same goal as basic
                                        GraphixScrollWindow::settings.setGoal(0,2); // zero goal!  -> computer!
				break;
				default:break;		
			} // end switch

			player[0]->update();
			player[1]->update();

/*			GraphixScrollWindow::settings.initSoup();
		        resetData();
		        if(ANARACE** temp=GraphixScrollWindow::settings.newGeneration(anarace))
		        {                 for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
                			        anarace[i]=temp[i];
		                update=2;
//              if(anarace[0]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO?
		        };
		        for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
                		player[i]->assignAnarace(&(anarace[i]));*/
	
//			GraphixScrollWindow::settings.checkForChange();
		} // end mainwindow is activated

	GraphixScrollWindow::settings.checkForChange();
	for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
		if(player[i]->isShown())
		{
			player[i]->processButtons();
//			if(player[i]->hasChanged())
			{
				if(!isOptimizing())
					update=2;
				player[i]->changeAccepted(); //~~
			}
		}

//TODO scroll miteinbeziehen
/*
	if(theCore->isShown())
	{
		if(theCore->isPressed(0))
		{ 
			int old=ga->getMutationFactor();
			if(ga->getMutationFactor()<10) 
				ga->setMutationFactor(ga->getMutationFactor()+1);
			else if(ga->getMutationFactor()<30)
				ga->setMutationFactor(ga->getMutationFactor()+5);
			else if(ga->getMutationFactor()<100)
				ga->setMutationFactor(ga->getMutationFactor()+10);
			else if(ga->getMutationFactor()<200)
				ga->setMutationFactor(ga->getMutationFactor()+20);
			if(old!=ga->getMutationFactor())
				msgWindow->addMessage(_T(wxString::Format(wxT("Increased mutation factor to %i%%."),ga->getMutationFactor())));
			else if(old==200)
			{
				msgWindow->addMessage(_T("Mutation factor is already at maximum (200)."));
				msgWindow->addMessage(_T("Take a look in the ""help"" menu (settings / mutation factor)."));
			}
		}
		if(theCore->isPressed(1))
		{
			int old=ga->getMutationFactor();
			if(ga->getMutationFactor()>100)
				ga->setMutationFactor(ga->getMutationFactor()-20);
			else if(ga->getMutationFactor()>30)
				ga->setMutationFactor(ga->getMutationFactor()-10);
		       	else if(ga->getMutationFactor()>10)
				ga->setMutationFactor(ga->getMutationFactor()-5);
			else if(ga->getMutationFactor()>0)
				ga->setMutationFactor(ga->getMutationFactor()-1);
			if(old!=ga->getMutationFactor())
			{
				if(ga->getMutationFactor()==0)
				{
					if(ga->getCrossOver()==0)
		       				msgWindow->addMessage(_T("Deactivated mutations! Evolution will no longer occur."));
					else
						msgWindow->addMessage(_T("Deactivated mutations. From now on evolution will only occur through cross over."));
				}
				else
					msgWindow->addMessage(_T(wxString::Format(wxT("Decreased mutation factor to %i%%."),ga->getMutationFactor())));
			} else if(old==0)
				msgWindow->addMessage(_T("Mutation factor is already at minimum (0%%)."));
		}

		if(theCore->isPressed(2))
		{
			if(ga->getMutationFactor()>0)
			{
				ga->setMutationFactor(0);
				if(ga->getCrossOver()==0)
					msgWindow->addMessage(_T("Deactivated mutations! Evolution will no longer occur."));
				else
					msgWindow->addMessage(_T("Deactivated mutations. From now on evolution will only occur through cross over."));
			}
		}

		if(theCore->isPressed(3))
		{
			int old=ga->getBreedFactor();
			if(ga->getBreedFactor()<10)
				ga->setBreedFactor(ga->getBreedFactor()+1);
			else if(ga->getBreedFactor()<30)
				ga->setBreedFactor(ga->getBreedFactor()+5);
			else if(ga->getBreedFactor()<100)
				ga->setBreedFactor(ga->getBreedFactor()+10);
			if(old!=ga->getBreedFactor())
				msgWindow->addMessage(_T(wxString::Format(wxT("Increased breed factor to %i%%."),ga->getMutationFactor())));
			else if(old==100)
			{
				msgWindow->addMessage(_T("Breed factor is already at maximum (100%%)."));
				msgWindow->addMessage(_T("Take a look in the ""help"" menu (settings / breed factor)."));
			}
		}
																			    
		if(theCore->isPressed(4))
		{
			int old=ga->getBreedFactor();
			if(ga->getBreedFactor()>30)
				ga->setBreedFactor(ga->getBreedFactor()-10);
			else if(ga->getBreedFactor()>10)
				ga->setBreedFactor(ga->getBreedFactor()-5);
			else if(ga->getBreedFactor()>0)
				ga->setBreedFactor(ga->getBreedFactor()-1);
			if(old!=ga->getBreedFactor())
			{
				if(ga->getBreedFactor()==0)
				{
		//TODO hyperlinks in die Hilfe!
					if(ga->getCrossOver()==0)
						msgWindow->addMessage(_T("Deactivated breeding! Evolution will no longer occur."));
					else
						msgWindow->addMessage(_T("Deactivated breeding. From now on evolution will only occur through cross over."));
				}
				else
					msgWindow->addMessage(_T(wxString::Format(wxT("Decreased breed factor to %i%%."),ga->getMutationFactor())));
			} else if(old==0)
				msgWindow->addMessage(_T("Breed factor is already at minimum (0%%)."));
		}
																			   
		if(theCore->isPressed(5))
		{
			if(ga->getCrossOver()==0)
			msgWindow->addMessage(_T("Deactivated breeding! Evolution will no longer occur."));
				else
			msgWindow->addMessage(_T("Deactivated breeding. From now on evolution will only occur through cross over."));
			ga->setBreedFactor(0);
		}
	

		if(theCore->isPressed(6))
		{
			int old=ga->getCrossOver();
			if(ga->getCrossOver()<10)
				ga->setCrossOver(ga->getCrossOver()+1);
			else if(ga->getCrossOver()<30)
				ga->setCrossOver(ga->getCrossOver()+5);
			else if(ga->getCrossOver()<100)
				ga->setCrossOver(ga->getCrossOver()+10);
			if(old!=ga->getCrossOver())
				msgWindow->addMessage(_T(wxString::Format(wxT("Increased cross over to %i%%."),ga->getCrossOver())));
			else if(old==100)
			{
				msgWindow->addMessage(_T("Cross over is already at maximum (100%%)."));
				msgWindow->addMessage(_T("Take a look in the ""help"" menu (settings / cross over)."));
			}
		}
																			    
		if(theCore->isPressed(7))
		{
			int old=ga->getCrossOver();
			if(ga->getCrossOver()>30)
				ga->setCrossOver(ga->getCrossOver()-10);
			else if(ga->getCrossOver()>10)
				ga->setCrossOver(ga->getCrossOver()-5);
			else if(ga->getCrossOver()>0)
				ga->setCrossOver(ga->getCrossOver()-1);
			if(old!=ga->getCrossOver())
			{
				if(ga->getCrossOver()==0)
				{
					if((ga->getBreedFactor()==0)||(ga->getMutationFactor()==0))
					msgWindow->addMessage(_T("Deactivated cross over! Evolution will no longer occur."));
						else
					msgWindow->addMessage(_T("Deactivated crossover. From now on evolution will only occur through breeding and mutations."));
				}
				else msgWindow->addMessage(_T(wxString::Format(wxT("Decreased cross over to %i%%."),ga->getCrossOver())));
			}
			else if(old==0)
				msgWindow->addMessage(_T("Cross over is already at minimum (0%%)."));
		}
																			    
		if(theCore->isPressed(8))
		{
			if((ga->getBreedFactor()==0)||(ga->getMutationFactor()==0))
			msgWindow->addMessage(_T("Deactivated cross over! Evolution will no longer occur."));
				else
			msgWindow->addMessage(_T("Deactivated crossover. From now on evolution will only occur through breeding and mutations."));
			ga->setCrossOver(0);
		}
		theCore->clearButtons();
	}*/
	
};


void MyDCWindow::stopOptimizing()
{
	for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
	       if(player[i]->isShown())
			player[i]->setOptimizing(0);
};

int MyDCWindow::isOptimizing()
{
	for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
 	       if(player[i]->isOptimizing())
			return(1);
	return(0);
};

void MyDCWindow::drawGizmo()
{
	dc->SetFont(GraphixScrollWindow::font3);
	if(isOptimizing()) ani++;
	else ani=1;
	if(ani>30) ani=1;
	int dx=0;int dy=0;
	dc->SetTextForeground(wxColour((0==ani%20)*200+((0==ani%19)+(0==ani%21))*50,(0==ani%20)*200+((0==ani%19)+(0==ani%21))*50,(0==ani%20)*200+((0==ani%19)+(0==ani%21))*100+50));
	dc->DrawText(_T("E"),mainWindow->getInnerLeftBound()+10,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("E",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%21)*200+((0==ani%20)+(0==ani%22))*50,(0==ani%21)*200+((0==ani%20)+(0==ani%22))*50,(0==ani%21)*200+((0==ani%20)+(0==ani%22))*100+50));
	dc->DrawText(_T("v"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("Ev",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%22)*200+((0==ani%21)+(0==ani%23))*50,(0==ani%22)*200+((0==ani%21)+(0==ani%23))*50,(0==ani%22)*200+((0==ani%21)+(0==ani%23))*100+50));
	dc->DrawText(_T("o"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("Evo",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%23)*200+((0==ani%22)+(0==ani%24))*50,(0==ani%23)*200+((0==ani%22)+(0==ani%24))*50,(0==ani%23)*200+((0==ani%22)+(0==ani%24))*100+50));
	dc->DrawText(_T("l"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("Evol",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%24)*200+((0==ani%23)+(0==ani%25))*50,(0==ani%24)*200+((0==ani%23)+(0==ani%25))*50,(0==ani%24)*200+((0==ani%23)+(0==ani%25))*100+50));
	dc->DrawText(_T("u"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("Evolu",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%25)*200+((0==ani%24)+(0==ani%26))*50,(0==ani%25)*200+((0==ani%24)+(0==ani%26))*50,(0==ani%25)*200+((0==ani%24)+(0==ani%26))*100+50));
	dc->DrawText(_T("t"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("Evolut",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%26)*200+((0==ani%25)+(0==ani%27))*50,(0==ani%26)*200+((0==ani%25)+(0==ani%27))*50,(0==ani%26)*200+((0==ani%25)+(0==ani%27))*100+50));
	dc->DrawText(_T("i"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("Evoluti",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%27)*200+((0==ani%26)+(0==ani%28))*50,(0==ani%27)*200+((0==ani%26)+(0==ani%28))*50,(0==ani%27)*200+((0==ani%26)+(0==ani%28))*100+50));
	dc->DrawText(_T("o"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);
	dc->GetTextExtent("Evolutio",&dx,&dy);

	dc->SetTextForeground(wxColour((0==ani%28)*200+((0==ani%27)+(0==ani%29))*50,(0==ani%28)*200+((0==ani%27)+(0==ani%29))*50,(0==ani%28)*200+((0==ani%27)+(0==ani%29))*100+50));
	dc->DrawText(_T("n"),mainWindow->getInnerLeftBound()+10+dx,mainWindow->getInnerUpperBound()+5);

	dc->SetTextForeground(wxColour(25,25,85));
	dc->DrawText(_T("Forge"),mainWindow->getInnerLeftBound()+50,mainWindow->getInnerUpperBound()+45);
	dc->SetTextForeground(wxColour(0,0,85));
	dc->SetFont(GraphixScrollWindow::font3);
	dc->DrawText(_T("v1.11"),mainWindow->getInnerLeftBound()+154,mainWindow->getInnerUpperBound()+84);
	dc->SetTextForeground(wxColour(50,50,85));
	dc->SetFont(GraphixScrollWindow::font3);
	dc->DrawText(_T("v1.11"),mainWindow->getInnerLeftBound()+150,mainWindow->getInnerUpperBound()+80);
};


void MyDCWindow::showCoreAnimation()
{
}

wxColour chooseColour(int num,int row)
{
	if((num<50+row*50)&&(num>=row*50))
		return(wxColour(200-(num%50)*2,200-(num%50)*2,250-num%50));
	else
		return(wxColour(50,50,100));
};

void MyDCWindow::OnPaint(wxPaintEvent& event)
{
	if(dc) delete(dc);
	dc=new wxMemoryDC();
	dc->SelectObject(wxBitmap(SCREEN_X,SCREEN_Y));
	dc->SetBrush(*wxBLACK_BRUSH);
	animationNumbers++;
	if(animationNumbers>150)
	animationNumbers=1;


	if(mainWindow->isShown())
	{
		mainWindow->Draw(dc);
		drawGizmo();
	}

	if(userIsNewbie)
	{
		dc->SetBrush(*wxBLACK_BRUSH);
		dc->SetPen(*wxTRANSPARENT_PEN);
		dc->DrawBitmap(GraphixScrollWindow::bmpArrowRight,mainWindow->getLeftBound()+mainWindow->getWidth()-130-animationNumbers%4,mainWindow->getUpperBound()-5);
	}

//	if(update==2)
//	{
		for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
			if(player[i]->isShown())
				player[i]->update();
//	}

	update=1;

	if(tutorialWindow->isShown())
	{
		tutorialWindow->DrawButtons(dc);	
		userIsNewbie=0;
		int t=0;
		for(t=0;t<8;t++)
		if(tutorialWindow->isActivated(t))
		{
			tutorialAnimation=1;
			if(tutorialChapter==0)
				tutorialChapter=100+t*100;
			else if(tutorialChapter%100==0)
			{
				if(t==0)
					tutorialChapter=0;
				else
					tutorialChapter+=t*10;
			}
			else if(tutorialChapter%10==0)
			{
				if(t==0)
					tutorialChapter-=tutorialChapter%100;
			}
		}
		tutorialWindow->clearButtons();
		tutorialAnimation++;
		if((tutorialAnimation==400)&&(!isOptimizing()))
			tutorialAnimation=399;

		tutorialWindow->Draw(dc);
		wxRect edge;
		switch(tutorialChapter)
		{
			case 0:
				{
		//TODO info window wenn Maus drueber :o
		// evtl auch Mauszeiger aendern
					tutorialWindow->writeHeader(_T("Tutorial"),dc,0);
					tutorialWindow->textButton(_T("Chapter 1: Introduction"),dc);
					tutorialWindow->textButton(_T("Chapter 2: Detailed description"),dc);
					tutorialWindow->textButton(_T("Chapter 3: How to use the results"),dc);
					tutorialWindow->textButton(_T("Chapter 4: Advanced & Expert Mode"),dc);
					tutorialWindow->textButton(_T("Chapter 5: Gosu & Transcendend Mode"),dc);
					tutorialWindow->textButton(_T("Chapter 6: The core"),dc);
					tutorialWindow->textButton(_T("Chapter 7: The web"),dc);
					tutorialWindow->textButton(_T("Chapter 8: Development of EF"),dc);
				};break;
			case 100:
				{
					tutorialWindow->writeHeader(_T("1. Introduction"),dc,1);
					tutorialWindow->textButton(_T("1.1: What does this program?"),dc);
					tutorialWindow->textButton(_T("1.2: Quickstart"),dc);
					tutorialWindow->textButton(_T("1.3: Overview of the modes"),dc);
					tutorialWindow->textButton(_T("1.4: Is it useful?"),dc);
					tutorialWindow->textButton(_T("1.5: Where are the limits?"),dc);
					tutorialWindow->textButton(_T("1.6: Is it cheating?"),dc);
				}
				break; // Introduction
			case 110:
				{
					tutorialWindow->writeHeader(_T("1.1: What does this program?"),dc,2);
					tutorialWindow->writeLongText(_T("The program simulates an abstract $StarCraft:Broodwar$ environment and calculates the time a certain build order needs.#By comparing different build orders, randomly changing them and taking the best build order for the next generation, the build order is improved step by step.#$StarCraft$ build orders fit perfectly for this algorithm called $HillClimbing$.#"),dc);
				};break;
			case 120:
				{
				
					tutorialWindow->writeHeader(_T("1.2: Quickstart"),dc,2);
					chooseColour(tutorialAnimation/2,0);
					tutorialWindow->writeLongText(_T("It's easy:#"),dc);
					tutorialWindow->writeLine(_T(" - Click on the left side in the goal list window $'Click here to add goal'$, choose a category and pick your goals.#"),dc,chooseColour(tutorialAnimation/4,0));
					tutorialWindow->writeLine(_T("  - Click on the top right on the flashing $'Click to continue'$ button and sit back to watch the progress.#"),dc,chooseColour(tutorialAnimation/4,1));
					tutorialWindow->writeLine(_T("  - You will see items moving around in window named $'Build Order'$ on the bottom right. The program now will optimize the build order by rearranging the items.#"),dc,chooseColour(tutorialAnimation/4,2));
					tutorialWindow->writeLine(_T("  - On the top right you will see the current best time in the $'Timer window'$. If you read there a $'Searching'$ it means, the program is still searching for a valid solution which will need a moment depending how many goals you have entered while $'Optimizing'$ means that a valid solution is found and the program tries to optimize it.#"),dc,chooseColour(tutorialAnimation/4,3));
					tutorialWindow->writeLine(_T("  - On the top you will see the $'Statistical Data'$ window which displays changes in time, fitness and ressources of the best program over the last few seconds. It is interesting for the advanced user, this window is availible on $'Advanced'$ mode or higher.#"),dc,chooseColour(tutorialAnimation/4,4));
					
					tutorialWindow->writeLine(_T("  - Evolution forge also allows you to see a graphical illustration (through the $'Graphical illustration of the build order'$ window) of the build order to see what buildings are built at the same time, how long they need and when to build them. In addition there is the $'Overview of the build order'$ window where you can see how much minerals, gas and supply you have at a certain time if you play this build order.#"),dc,chooseColour(tutorialAnimation/4,5));
					tutorialWindow->writeLine(_T("  - After a while you will notice that there are no more changes on the screen. Probably the best build order is found. Press $ALT+T$ to stop and let program print out the build order via the File menu.#"),dc,chooseColour(tutorialAnimation/4,6));
					if(tutorialAnimation>1450)
					{
						tutorialAnimation=1;
						tutorialWindow->adjustWindow(wxRect(mainWindow->getInnerLeftBound()+mainWindow->getInnerWidth()-(tutorialWindow->getTargetWidth()),mainWindow->getInnerUpperBound()+SECOND_ROW,tutorialWindow->getTargetWidth(),400));
						stopOptimizing();
					}
			
					if((tutorialAnimation<400)&&(isOptimizing()))
						tutorialAnimation=400;
					

					if(tutorialAnimation<200)
						player[0]->forceWindow->Show(1);
					else
					if(tutorialAnimation<400)
						player[0]->timerWindow->Show(1);
					else
					if(tutorialAnimation<600)
					{
// TODO: hoehe vom tutwindow passt net irgendwie... guten moregn ;)
						tutorialWindow->adjustWindow(wxRect(theCore->getLeftBound(),mainWindow->getInnerUpperBound()+125,tutorialWindow->getTargetWidth(),500));
						player[0]->timerWindow->Show(0);
						player[0]->boWindow->Show(1);
					}
					else
					if(tutorialAnimation<800)
						player[0]->timerWindow->Show(1);
					else if(tutorialAnimation<1000)
					{
						player[0]->statisticsWindow->Show(1);
						tutorialWindow->adjustWindow(wxRect(theCore->getLeftBound(),200,tutorialWindow->getTargetWidth(),500));
					}
					else if(tutorialAnimation<1200)
					{
						player[0]->statisticsWindow->Show(0);
// Problem: da das hier ja oefters aufgerufen wird, wuerde jede Veraenderung durch andere Teile des Programms der Hoehe und Breite hier wieder zu nichte gemacht werden!						boDiagramWindow->adjustWindow(wxRect(boDiagramWindow->getLeftBound(),theCore->getLowerBound()+5+150,boDiagramWindow->getWidth(),boDiagramWindow->getHeight					
						tutorialWindow->adjustWindow(wxRect(theCore->getLeftBound(),mainWindow->getInnerUpperBound(),tutorialWindow->getTargetWidth(),500));
					}
				};break;
			case 130:
				{
					tutorialWindow->writeHeader(_T("1.3: Overview of the modes"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 140:
				{
					tutorialWindow->writeHeader(_T("1.4: Is it useful?"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 150:
				{
					tutorialWindow->writeHeader(_T("1.5: Where are the limits?"),dc,2);
					tutorialWindow->writeLongText(_T(" - It only calculates a general build order. Depending on the map, the latency, your mousespeed and many other small factors there might be a better build order.# - Goals consisting of many units will result in exponential growth of time needed for the calculation.# - The program is based on one-second steps. This might lead to some inaccurate calculation.# - In most cases the build orders cannot be applied directly 1:1 in the game but you have to exercise them and adapt them to your needs.#"),dc);
				};break;
			case 160:
				{
					tutorialWindow->writeHeader(_T("1.6: Is it cheating?"),dc,2);
					tutorialWindow->writeLongText(_T("Well... in a way... yes.#If you use the program correctly you might get an advantage over your enemy: You learn to get a better starting in the game so that you can concentrate on the main thing, strategies and tactics!#While you can get the same information by watching replays, reading strategy articles or just playing the game it is much more fun to interactivly trying out combinations to compare them.#"),dc);
				};break;

			case 200:				
				{
					tutorialWindow->writeHeader(_T("2. Detailed Description"),dc,1);
					tutorialWindow->textButton(_T("2.1: The goal window"),dc);
					tutorialWindow->textButton(_T("2.2: The build order window"),dc);
					tutorialWindow->textButton(_T("2.3: The statistics window"),dc);
					tutorialWindow->textButton(_T("2.4: The overview window"),dc);
					tutorialWindow->textButton(_T("2.5: The graphical window"),dc);
					tutorialWindow->textButton(_T("2.6: The timer window"),dc);
				}
				break; // Detail
			case 210:
				{
					tutorialWindow->writeHeader(_T("2.1: The goal window"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 220:
				{
					tutorialWindow->writeHeader(_T("2.2: The build order window"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 230:
				{
					tutorialWindow->writeHeader(_T("2.3: The statistics window"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 240:
				{
					tutorialWindow->writeHeader(_T("2.4: The overview window"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 250:
				{
					tutorialWindow->writeHeader(_T("2.5: The graphical window"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 260:
				{
					tutorialWindow->writeHeader(_T("2.6: The timer window"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;

			case 300:
				{
					tutorialWindow->writeHeader(_T("3. How to use the results"),dc,1);
					tutorialWindow->textButton(_T("3.1: About time and numbers"),dc);
					tutorialWindow->textButton(_T("3.2: HTML output"),dc);
					tutorialWindow->textButton(_T("3.3: Printing output"),dc);
					tutorialWindow->textButton(_T("3.4: During a game"),dc);
					tutorialWindow->textButton(_T("3.5: Using the database"),dc);
					tutorialWindow->textButton(_T("3.6: Publishing your database"),dc);
				}
				break; // I/O
			case 310:
				{
					tutorialWindow->writeHeader(_T("3.1: About time and numbers"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 320:
				{
					tutorialWindow->writeHeader(_T("3.2: HTML output"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 330:
				{
					tutorialWindow->writeHeader(_T("3.3: Printing output"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 340:
				{
					tutorialWindow->writeHeader(_T("3.4: During a game"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 350:
				{
					tutorialWindow->writeHeader(_T("3.5: Using the database"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 360:
				{
					tutorialWindow->writeHeader(_T("3.6: Publishing your database"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;

			case 400:
				{
					tutorialWindow->writeHeader(_T("4. Advanced & Expert Mode"),dc,1);
					tutorialWindow->textButton(_T("4.1: Testing the bo in Advanced mode"),dc);
					tutorialWindow->textButton(_T("4.2: New settings in Advanced mode"),dc);
					tutorialWindow->textButton(_T("4.3: New settings in Expert mode"),dc);
					tutorialWindow->textButton(_T("4.4: A guide to finetune your bo I"),dc);
					tutorialWindow->textButton(_T("4.5: A guide to finetune your bo II"),dc);
					tutorialWindow->textButton(_T("4.6: A guide to finetune your bo III"),dc);
				}
				break; // Expert & advanced mode
			case 410:
				{
					tutorialWindow->writeHeader(_T("4.1: Testing the bo in Advanced Mode"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 420:
				{
					tutorialWindow->writeHeader(_T("4.2: New settings in Advanced mode"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 430:
				{
					tutorialWindow->writeHeader(_T("4.3: New settings in Expert mode"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 440:
				{
					tutorialWindow->writeHeader(_T("4.4: A guide to finetune your bo I"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 450:
				{
					tutorialWindow->writeHeader(_T("4.5: A guide to finetune your bo II"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 460:
				{
					tutorialWindow->writeHeader(_T("4.6: A guide to finetune your bo III"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;

			case 500:
				{
					tutorialWindow->writeHeader(_T("5. Gosu & Transcendend Mode"),dc,1);
					tutorialWindow->textButton(_T("5.1: Gosu mode - Can you beat it?"),dc);
					tutorialWindow->textButton(_T("5.2: Strategies, hints, tricks"),dc);
					tutorialWindow->textButton(_T("5.3: What to learn from such tournaments"),dc);
					tutorialWindow->textButton(_T("5.4: Transcendend mode - silicium alife!"),dc);
					tutorialWindow->textButton(_T("5.5: What to learn from the computer"),dc);
					tutorialWindow->textButton(_T("5.6: Short analysis of such tournaments"),dc);
				}
				break; // Gosu & trancendent mode
			case 510:
				{
					tutorialWindow->writeHeader(_T("5.1: Gosu mode - Can you beat it?"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 520:
				{
					tutorialWindow->writeHeader(_T("5.2: Strategies, hints, tricks"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 530:
				{
					tutorialWindow->writeHeader(_T("5.3: What to learn from such tournaments"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 540:
				{
					tutorialWindow->writeHeader(_T("5.4: Transcendend mode - silicium alife!"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 550:
				{
					tutorialWindow->writeHeader(_T("5.5: What to learn from the computer"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 560:
				{
					tutorialWindow->writeHeader(_T("5.6: Short analysis of such tournaments"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;

			case 600:
				{
					tutorialWindow->writeHeader(_T("6. The Core"),dc,1);
					tutorialWindow->textButton(_T("6.1: Basic underlying algorithm"),dc);
					tutorialWindow->textButton(_T("6.2: Internal representation of an order list"),dc);
					tutorialWindow->textButton(_T("6.3: Flexibility/Scalability of the core"),dc);
					tutorialWindow->textButton(_T("6.4: A chapter about Genetic Algorithms"),dc);
					tutorialWindow->textButton(_T("6.5: Comparison with other algorithms"),dc);
					tutorialWindow->textButton(_T("6.6: Problems of crossing over"),dc);
				}
				break; // The Core
			case 610:
				{
					tutorialWindow->writeHeader(_T("6.1: Basic underlying algorithm"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 620:
				{
					tutorialWindow->writeHeader(_T("6.2: Internal representation of an order list"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 630:
				{
					tutorialWindow->writeHeader(_T("6.3: Flexibility/Scalability of the core"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 640:
				{
					tutorialWindow->writeHeader(_T("6.4: A chapter about Genetic Algorithms"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 650:
				{
					tutorialWindow->writeHeader(_T("6.5: Comparison with other algorithms"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 660:
				{
					tutorialWindow->writeHeader(_T("6.6: Problems of crossing over"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;

			case 700:				
				{
					tutorialWindow->writeHeader(_T("7. The web"),dc,1);
					tutorialWindow->textButton(_T("7.1: www.clawsoftware.de"),dc);
					tutorialWindow->textButton(_T("7.2: www.nix-step.com"),dc);
					tutorialWindow->textButton(_T("7.3: www.my-friendly-publisher.com"),dc);
					tutorialWindow->textButton(_T("7.4: Read news about EF"),dc);
					tutorialWindow->textButton(_T("7.4: Browse the database"),dc);
					tutorialWindow->textButton(_T("7.6: Check for updates"),dc);
				}
				break; // The web
			case 710:
				{
					tutorialWindow->writeHeader(_T("7.1: www.clawsoftware.de"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 720:
				{
					tutorialWindow->writeHeader(_T("7.2: www.nix-step.com"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 730:
				{
					tutorialWindow->writeHeader(_T("7.3: www.my-friendly-publishers.com"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 740:
				{
					tutorialWindow->writeHeader(_T("7.4: Read news about EF"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 750:
				{
					tutorialWindow->writeHeader(_T("7.5: Browse the database"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 760:
				{
					tutorialWindow->writeHeader(_T("7.6: Check for updates"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;

			case 800:				
				{
					tutorialWindow->writeHeader(_T("8. Development of EF"),dc,1);
					tutorialWindow->textButton(_T("8.1: Things 'todo'"),dc);
					tutorialWindow->textButton(_T("8.2: Things that will not be implemented"),dc);
					tutorialWindow->textButton(_T("8.3: About the past"),dc);
					tutorialWindow->textButton(_T("8.4: About the future"),dc);
					tutorialWindow->textButton(_T("8.5: About the programmer"),dc);
					tutorialWindow->textButton(_T("8.6: Join the crew - join the development"),dc);
				}
				break; // Development
			case 810:
				{
					tutorialWindow->writeHeader(_T("8.1: Things 'todo'"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 820:
				{
					tutorialWindow->writeHeader(_T("8.2: Things that will not be implemented"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 830:
				{
					tutorialWindow->writeHeader(_T("8.3: About the past"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 840:
				{
					tutorialWindow->writeHeader(_T("8.4: About the future"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 850:
				{
					tutorialWindow->writeHeader(_T("8.5: About the programmer"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;
			case 860:
				{
					tutorialWindow->writeHeader(_T("8.6: Join the crew - join the development"),dc,2);
					tutorialWindow->writeLongText(_T(""),dc);
				};break;

			default:break;
		}
	}

	if(theCore->isShown())
	{
		theCore->Draw(dc);
//		showCoreAnimation();
//		drawGeneString();
		theCore->DrawButtons(dc);
	}

	if(msgWindow->isShown())
	{
		msgWindow->Draw(dc);
		msgWindow->drawMessages(dc);
	}

	if(endrun)
	{
	       msgWindow->addMessage(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace[0]->getRun(),(ga->maxTime-anarace[0]->getTimer())/60,(ga->maxTime-anarace[0]->getTimer())%60)));
		resetData();
	}
/*		dc->SetFont(font3);
		dc->SetTextForeground(wxColour(155*grey*(100-grey)/2500,0,0));
		dc->DrawText(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace[0]->getRun(),(ga->maxTime-anarace[0]->getTimer())/60,(ga->maxTime-anarace[0]->getTimer())%60)),27,203);
																			    
		dc->SetTextForeground(wxColour(255*grey*(100-grey)/2500,0,0));
		dc->DrawText(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace[0]->getRun(),(ga->maxTime-anarace[0]->getTimer())/60,(ga->maxTime-anarace[0]->getTimer())%60)),25,200);
		if(grey==100)
		grey-=2;
		if(grey<=0) resetData();
	} else
	if(grey<100)
	{
		dc->SetTextForeground(wxColour(130*grey*(100-grey)/2500,0,0));
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("Round %i"),anarace[0]->getRun()+1)),msgWindow->getInnerLeftBound()+18,msgWindow->getInnerUpperBound()+13);
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("%s"),gizmo[gizmor])),msgWindow->getInnerLeftBound()+18,msgWindow->getInnerUpperBound()+63);
																			    
		dc->SetTextForeground(wxColour(230*grey*(100-grey)/2500,0,0));
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("Round %i"),anarace[0]->getRun()+1)),msgWindow->getInnerLeftBound()+15,msgWindow->getInnerUpperBound()+10);
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("%s"),gizmo[gizmor])),msgWindow->getInnerLeftBound()+15,msgWindow->getInnerUpperBound()+60);
																			    
		grey+=2;
	}*/


	for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
		if(player[i]->isShown())
			player[i]->DrawMe(dc);

//	if(theCore->isShown())
//		player[0]->drawGeneString(dc,wxRect(theCore->getPosition()+wxPoint(0,10),theCore->getSize()+wxSize(0,-4)));
	wxPaintDC dcp(this);

	processButtons();
//	wxScreenDC* dcp2=new wxScreenDC();
//	dcp2->StartDrawingOnTop();
	dcp.Blit(0, 0, SCREEN_X, SCREEN_Y, dc, 0, 0);
//	dcp2->EndDrawingOnTop();
//	delete dcp2;
}
										
//settings: log level (none, debug only, +final result, +result of each run, +snapshot every X generations, +snapshot every generation, EVERYTHING (~2MB/generation!)
/*
void MyFrame::OnSettingsDialogApply()
{
	child->GraphixScrollWindow::settings.setMaxTime(spin1->GetValue());
	child->GraphixScrollWindow::settings.setMaxTimeOut(spin2->GetValue());
	child->GraphixScrollWindow::settings.setMaxLength(spin3->GetValue());
	child->GraphixScrollWindow::settings.setMaxRuns(spin4->GetValue());
	child->GraphixScrollWindow::settings.setMaxGenerations(spin5->GetValue());
	child->GraphixScrollWindow::settings.setBreedFactor(spin6->GetValue());
	child->GraphixScrollWindow::settings.setCrossOver(spin7->GetValue());
	if(check1->GetValue())
		child->GraphixScrollWindow::settings.setPreprocessBuildOrder(1);
	else child->GraphixScrollWindow::settings.setPreprocessBuildOrder(0);

	child->GraphixScrollWindow::settings.setCurrentMap(lb1->GetSelection());
	child->GraphixScrollWindow::settings.setGoal(lb2->GetSelection(),1);
	child->GraphixScrollWindow::settings.setGoal(lb2->GetSelection(),2);
}
*/

void MyDCWindow::OnIdle(wxIdleEvent& WXUNUSED(event))
{
	if((endrun==0)&&(!refresh)&&(isOptimizing()))
	{
		update=2;
		ANARACE** temp;
//TODO: nach Ende eines Durchlaufs ist anarace 0, aber viele anderen Teile des Codes greifen noch drauf zu!!
		if(temp=GraphixScrollWindow::settings.newGeneration(anarace))
		{
			for(int i=0;i<GraphixScrollWindow::settings.getMap(0)->getMaxPlayer()-1;i++)
			{
				anarace[i]=temp[i];
//				if(anarace[i]->getRun()!=oldrun) {oldrun=anarace[0]->getRun();endrun=1;} TODO
			}
//			Refresh(false);
		};
	};
//	if(update==1)
//	{
//		update=2;
//		Refresh(false);
//	}

	refresh++;
	if(refresh>2)
		refresh=0;		
	Refresh(false);
}

void MyDCWindow::OnTimer(wxTimerEvent& WXUNUSED(event))
{
//	Refresh(false);
}

void MyDCWindow::OnKeyDown(wxKeyEvent& event)
{
//	if(event.ShiftDown())
		mainWindow->controls.setShiftPressed(1);
};

void MyDCWindow::OnKeyUp(wxKeyEvent& event)
{
//	if(!event.ShiftDown())
		mainWindow->controls.setShiftPressed(0);
// TODO, wenn event verloren geht... :/
};


void MyDCWindow::OnMouseScroll(wxMouseEvent& event)
{
	mainWindow->controls.scrollMouse(event.GetWheelRotation());
//	boWindow->OnScrollMouse(msy);
//	goalForceWindow->OnScrollMouse(msy);
//	forceWindow->OnScrollMouse(msy);
//	msgWindow->OnScrollMouse(msy);
}

void MyDCWindow::OnMouseMove(wxMouseEvent& event)
{
	wxPoint position;
	event.GetPosition(&position.x,&position.y);
	mainWindow->controls.setMouse(position);
	//TODO irgendwie ein player -> update oder so
}

void MyDCWindow::OnMouseRightDown(wxMouseEvent& event)
{
	mainWindow->controls.rightDown();
};

void MyDCWindow::OnMouseRightUp(wxMouseEvent& event)
{
	mainWindow->controls.rightUp();
};

void MyDCWindow::OnMouseLeftDown(wxMouseEvent& event)
{
	mainWindow->controls.leftDown();
}

void MyDCWindow::OnMouseLeftUp(wxMouseEvent& event)
{
	mainWindow->controls.leftUp();
}
/*
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}*/

#if 0													    
void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog fileDialog(this, wxT("Select a file"));
while(!(fileDialog.ShowModal()==wxID_OK));
}

void MyFrame::OnGoalCreate(wxCommandEvent& event)
{
	menuGoal->Enable(EF_GoalCreate,false);
	menuGoal->Enable(EF_GoalCreate,true);
};
void MyFrame::OnMapImport(wxCommandEvent& event)
{
	menuMap->Enable(EF_MapImport,false);
	wxFileDialog fileDialog(this, wxT("Select a Map file"));
	if(run==1) run=2;
	while(true)
	{
		int error;
		int temp=fileDialog.ShowModal();
		switch(temp)
		{
			case wxID_OK:
				{
					wxString path=fileDialog.GetPath();
					if(!(error=(child->GraphixScrollWindow::settings.loadMapFile(path))))
					{
						wxMessageDialog bla(this,path/*_T(wxString::Format(wxT("Error importing goal file!\n %s : Error Code %i."),path,error))*/,_T("Message"),wxOK,wxDefaultPosition);
						while(bla.ShowModal()!=wxID_OK);
					}
					else
					{
						//loaded...
					};
				}
				break;
			case wxID_CANCEL:break;
			default:break;
		}
		if((temp==wxID_OK)||(temp==wxID_CANCEL))
			break;
	}
	if(run==2) run=1;
	menuGoal->Enable(EF_MapImport,true);
};

void MyFrame::OnMapCreate(wxCommandEvent& event)
{
	menuMap->Enable(EF_MapCreate,false);
	menuMap->Enable(EF_MapCreate,true);
};
void MyFrame::OnGoalImport(wxCommandEvent& event)
{
	menuGoal->Enable(EF_GoalImport,false);
	if(run==1) run=2;
	wxFileDialog fileDialog(this, wxT("Select a Goal file"));
	while(true)
	{
		int error;
		int temp=fileDialog.ShowModal();
		switch(temp)
		{
			case wxID_OK:
				{
					wxString path=fileDialog.GetPath();
					if(!(error=(child->GraphixScrollWindow::settings.loadGoalFile(path))))
					{
						wxMessageDialog bla(this,path/*_T(wxString::Format(wxT("Error importing goal file!\n %s : Error Code %i."),path,error))*/,_T("Message"),wxOK,wxDefaultPosition);
						while(bla.ShowModal()!=wxID_OK);
					}
					else
					{
						//loaded...
					};
				}
				break;
			case wxID_CANCEL:break;
			default:break;
		}
		if((temp==wxID_OK)||(temp==wxID_CANCEL))
			break;
	}
	if(run==2) run=1;
	menuMap->Enable(EF_GoalImport,true);
};

void MyFrame::OnGeneralSettings(wxCommandEvent& WXUNUSED(event))
{
	wxStaticText *text1,*text2,*text3,*text4,*text5,*text7,*text8,*text9,*text10;
	menuSettings->Enable(EF_GeneralSettings,false);
	if(run==1) run=2;
	dia=new wxDialog(this,EF_SettingsDialog,_T("General settings"),wxPoint(100,100),wxSize(450,260),wxDEFAULT_DIALOG_STYLE,_T("lala"));
	text1=new wxStaticText(dia,-1,_T("MaxTime"),wxPoint(10,10),wxDefaultSize,0,_T("Max Time of Calculation"));
	text2=new wxStaticText(dia,-1,_T("MaxTimeOut"),wxPoint(10,30),wxDefaultSize,0,_T("Max Timeout"));
	text3=new wxStaticText(dia,-1,_T("MaxLength"),wxPoint(10,50),wxDefaultSize,0,_T("Max Length"));
	text4=new wxStaticText(dia,-1,_T("MaxRuns"),wxPoint(10,70),wxDefaultSize,0,_T("Max Runs"));
	text5=new wxStaticText(dia,-1,_T("MaxGenerations"),wxPoint(10,90),wxDefaultSize,0,_T("Max Generations"));
	text7=new wxStaticText(dia,-1,_T("BreedFactor"),wxPoint(10,110),wxDefaultSize,0,_T("BreedFactor percentage"));
	text8=new wxStaticText(dia,-1,_T("CrossOver"),wxPoint(10,130),wxDefaultSize,0,_T("CrossOver percentage"));

	spin1=new wxSpinCtrl(dia,EF_SpinMaxTime,wxEmptyString,wxPoint(100,10),wxSize(60,18),wxSP_ARROW_KEYS,child->GraphixScrollWindow::settings.getMINMaxTime(),child->settings.getMAXMaxTime(),child->settings.getMaxTime(),_T("MaxTimeSpin"));
	spin2=new wxSpinCtrl(dia,EF_SpinMaxTimeOut,wxEmptyString,wxPoint(100,30),wxSize(60,18),wxSP_ARROW_KEYS,child->GraphixScrollWindow::settings.getMINMaxTimeOut(),child->settings.getMAXMaxTimeOut(),child->settings.getMaxTimeOut(),_T("MaxTimeOutSpin"));
	spin3=new wxSpinCtrl(dia,EF_SpinMaxLength,wxEmptyString,wxPoint(100,50),wxSize(60,18),wxSP_ARROW_KEYS,child->GraphixScrollWindow::settings.getMINMaxLength(),child->settings.getMAXMaxLength(),child->settings.getMaxLength(),_T("MaxLengthSpin"));
	spin4=new wxSpinCtrl(dia,EF_SpinMaxRuns,wxEmptyString,wxPoint(100,70),wxSize(60,18),wxSP_ARROW_KEYS,child->GraphixScrollWindow::settings.getMINMaxRuns(),child->settings.getMAXMaxRuns(),child->settings.getMaxRuns(),_T("MaxRunsSpin"));
	spin5=new wxSpinCtrl(dia,EF_SpinMaxGenerations,wxEmptyString,wxPoint(100,90),wxSize(60,18),wxSP_ARROW_KEYS,child->GraphixScrollWindow::settings.getMINMaxGenerations(),child->settings.getMAXMaxGenerations(),child->settings.getMaxGenerations(),_T("MaxGenerationsSpin"));
	spin6=new wxSpinCtrl(dia,EF_SpinBreedFactor,wxEmptyString,wxPoint(100,110),wxSize(60,18),wxSP_ARROW_KEYS,child->GraphixScrollWindow::settings.getMINBreedFactor(),child->settings.getMAXBreedFactor(),child->settings.getBreedFactor(),_T("BreedFactorSpin"));
	spin7=new wxSpinCtrl(dia,EF_SpinCrossOver,wxEmptyString,wxPoint(100,130),wxSize(60,18),wxSP_ARROW_KEYS,child->GraphixScrollWindow::settings.getMINCrossOver(),child->settings.getMAXCrossOver(),child->settings.getCrossOver(),_T("CrossOverSpin"));

	check1=new wxCheckBox(dia,EF_CheckPreprocess,_T("Preprocess Buildorder"),wxPoint(10,150),wxDefaultSize,-1,wxDefaultValidator,_T("preprocess"));
	if(child->GraphixScrollWindow::settings.getPreprocessBuildOrder())
		check1->SetValue(TRUE);
	else check1->SetValue(FALSE);

	but1=new wxButton(dia,wxID_OK,_T("OK"),wxPoint(20,200),wxSize(65,20));
	but2=new wxButton(dia,wxID_APPLY,_T("Apply"),wxPoint(90,200),wxSize(65,20));
	but3=new wxButton(dia,wxID_RESET,_T("Defaults"),wxPoint(160,200),wxSize(65,20));
	but4=new wxButton(dia,wxID_CANCEL,_T("Cancel"),wxPoint(230,200),wxSize(65,20));

	wxString bla[MAX_MAPS];
	for(int i=0;i<child->GraphixScrollWindow::settings.getMapCount();i++)
		bla[i]=_T(wxString::Format(wxT("%s"),child->GraphixScrollWindow::settings.getMap(i)->getName()));
	text9=new wxStaticText(dia,-1,_T("SelectMap"),wxPoint(200,10),wxDefaultSize,0,_T("Select Map"));
	lb1=new wxListBox(dia,-1,wxPoint(200,30),wxSize(80,150),child->GraphixScrollWindow::settings.getMapCount(),bla,0,wxDefaultValidator,_T("listBox"));
	lb1->SetSelection(child->GraphixScrollWindow::settings.getCurrentMap());

	wxString bla2[MAX_MAPS];
	for(int i=0;i<child->GraphixScrollWindow::settings.getGoalCount();i++)
		bla2[i]=_T(wxString::Format(wxT("%s"),child->GraphixScrollWindow::settings.getGoal(i)->getName()));
	text10=new wxStaticText(dia,-1,_T("SelectGoal"),wxPoint(300,10),wxDefaultSize,0,_T("Select Goal"));
	lb2=new wxListBox(dia,-1,wxPoint(300,30),wxSize(80,150),child->GraphixScrollWindow::settings.getGoalCount(),bla2,0,wxDefaultValidator,_T("listBox"));
	//lb2->SetSelection(GraphixScrollWindow::settings.getCurrentGoal()); ~~~ multiple players...

	int temp;
	while(true)
	{
		temp=dia->ShowModal();
		switch(temp)
		{
			case wxID_CANCEL:break;
			case wxID_APPLY:
			case wxID_OK:OnSettingsDialogApply();break;
			case wxID_RESET:child->GraphixScrollWindow::settings.loadDefaults();break;
		}
		if((temp==wxID_CANCEL)||(temp==wxID_OK)) break;
	}
	if(run==2) run=1;
	menuSettings->Enable(EF_GeneralSettings,true);
}
#endif
/*
void MyFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
	if(run==0) run=1;
#if wxUSE_STATUSBAR
//	SetStatusText(_T("Calculation in progress :-)")); 
#endif // wxUSE_STATUSBAR
	menuFile->Enable(EF_Start,false);
}

void MyFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
	if(run==1) run=0;
#if wxUSE_STATUSBAR
//	SetStatusText(_T("Click on File/Start to start the calculation!")); 
#endif // wxUSE_STATUSBAR

	menuFile->Enable(EF_Start,true);
}
*/
#if 0																			    
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxStaticText* text1;
	menuSettings->Enable(EF_About,false);
	if(run==1) run=2;
	wxString msg;
	msg.Printf(_T("- Evolution Forge v%i\n- Current source code size: 400kb\n- GUI and library core programmed by Clemens Lode\n- Copyright by Clemens Lode\n- GUI created with %s library\n- Written with VIM - Vi IMproved version 6.2.120\n- MS Windows version compiled with MinGW / GCC 3.3.2\n- Windows executable/dll size: 396kb/344kb\n- Linux (GTK) version compiled with GCC 3.3.2\n- Linux binary/lib size: 180kb/248kb\n- Tested with winex3.2, MS Windows ME and Linux (2.4.22)\n- Work was done with Linux Fedora Core 1 (Redhat 10)\n\nThis is ONLY a demonstration program which may not be sold or copied without permission.\nThere is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\nGreetings, ghoul@clawsoftware.de"), EF_Version,wxVERSION_STRING);
	msgBox=new wxDialog(this,-1,_T("About EF"),wxPoint(100,100),wxSize(600,300),wxDEFAULT_FRAME_STYLE,_T("lala"));
	text1=new wxStaticText(msgBox,-1,msg,wxPoint(10,10),wxDefaultSize,0,_T("bla"));
//homepage, updates
//	but1=new wxButton(msgBox,wxID_OK,_T("OK"),wxPoint(20,275),wxSize(65,20));
	while(msgBox->ShowModal()!=wxID_CANCEL);
	delete(msgBox);
	if(run==2) run=1;
	menuSettings->Enable(EF_About,true);
}
#endif


