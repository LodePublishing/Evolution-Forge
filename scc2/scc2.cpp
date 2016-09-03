#include "../scc2dll/settings.h"
#include "../scc2dll/location.h"
#include "scc2.h"
#include "../scc2dll/prerace.h"
#include "../scc2dll/race.h"
#include "../scc2dll/anarace.h"
#include "math.h"
#include "graphics.h"

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(EF_Open,  MyFrame::OnOpen)
	EVT_MENU(EF_Start,  MyFrame::OnStart)
	EVT_MENU(EF_Stop,  MyFrame::OnStop)
	EVT_MENU(EF_Quit,  MyFrame::OnQuit)
	EVT_MENU(EF_About, MyFrame::OnAbout)
	EVT_MENU(EF_GeneralSettings, MyFrame::OnGeneralSettings)
	EVT_MENU(EF_GoalCreate,  MyFrame::OnGoalCreate)
	EVT_MENU(EF_GoalImport,  MyFrame::OnGoalImport)
	EVT_MENU(EF_MapCreate,  MyFrame::OnMapCreate)
	EVT_MENU(EF_MapImport, MyFrame::OnMapImport)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyDCWindow, wxWindow)
	EVT_ERASE_BACKGROUND(MyDCWindow::OnEraseBackground)
	EVT_IDLE(MyDCWindow::OnIdle)
	EVT_PAINT(MyDCWindow::OnPaint)
	EVT_MOTION(MyDCWindow::OnMouseMove)
	EVT_LEFT_UP(MyDCWindow::OnMouseLeftUp)
	EVT_LEFT_DOWN(MyDCWindow::OnMouseLeftDown)
	EVT_MOUSEWHEEL(MyDCWindow::OnMouseScroll)
	EVT_TIMER(-1,MyDCWindow::OnTimer)
END_EVENT_TABLE()




const wxColour BOcolor[11]={wxColour(0,0,0),wxColour(25,25,25),wxColour(0,50,0),wxColour(50,0,0),wxColour(50,0,50),wxColour(0,0,50),wxColour(0,50,50),wxColour(50,50,0),wxColour(0,0,25),wxColour(25,0,0),wxColour(0,25,0)};

const wxString BOnames[11]={_T("rest"),_T("SCV"),_T("Gas SCV"),_T("Combat units..."),_T("Support units..."),_T("Supply/Refinery..."),_T("Producing buildings..."),_T("Research buildings..."),_T("Building addons..."),_T("Researchs..."),_T("Special...")};


GraphixScrollWindow* boWindow;
GraphixScrollWindow* goalForceWindow;
GraphixScrollWindow* forceWindow;
GraphixScrollWindow* mainWindow;
GraphixScrollWindow* statisticsWindow;
GraphixScrollWindow* timerWindow;
GraphixScrollWindow* boDiagramWindow;
GraphixScrollWindow* boGraphWindow;
GraphixScrollWindow* msgWindow;
GraphixScrollWindow* haxor;
GraphixScrollWindow* theCore;
GraphixScrollWindow* infoWindow;
GraphixScrollWindow* tutorialWindow;

int SCREEN_X;
int SCREEN_Y;
int oldy;
int buttonType[3*UNIT_TYPE_COUNT]; //button ID -> unit
int addGoalButton;
int goalListOpened;

int addUnitButton[UNIT_TYPE_COUNT];

int gizmor;
const int COLOR1R=100;
const int COLOR1G=100;
const int COLOR1B=100;

const int COLOR2R=150;
const int COLOR2G=150;
const int COLOR2B=150;

const int PEN1R=20;
const int PEN1G=20;
const int PEN1B=20;

const int TEXT1R=0;
const int TEXT1G=0;
const int TEXT1B=0;

const int INFOWINDOWPENR=40;
const int INFOWINDOWPENG=150;
const int INFOWINDOWPENB=20;

const int INFOWINDOWR=10;
const int INFOWINDOWG=50;
const int INFOWINDOWB=0;

const int INFOWINDOWTEXTR=125;
const int INFOWINDOWTEXTG=125;
const int INFOWINDOWTEXTB=250;

const int BASIC=0;
const int ADVANCED=1;
const int EXPERT=2;
const int GOSU=3;
const int TRANSCENDEND=4;
const int TUTORIAL=5;

int grey; //0-100%



void addMessage(wxString bla)
{
	msgWindow->setScrollY(0);
	message[msgCount].col=155;
	message[msgCount].type=1;
	message[msgCount].string=bla;
	msgCount++;
}

wxColour convertColour(int r, int g, int b)
{
	int col=(r+g+b)/3;
//	return(wxColour(r,g,b));
	return(wxColour((int)(col+(r-col)/**grey/100*/),(int)(col+(g-col)/**grey/100*/),(int)(col+(b-col)/**grey/100*/)));
}

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
//	static const int max = 7;
//	int progress=0;
//	wxSplashScreen* splash=NULL;
//	wxBitmap* bitmap=new wxBitmap("scc.bmp", wxBITMAP_TYPE_BMP);
//	if(bitmap)
//		splash = new wxSplashScreen(*bitmap, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, -1, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxSTAY_ON_TOP);

/*	wxProgressDialog dialog(_T("Progress of initializing"), _T("Be patient"), max, NULL, wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);

	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Initializing main window"),progress*100/max)));progress++;*/
	SCREEN_X=1024-4;SCREEN_Y=768-55;
	MyFrame *frame = new MyFrame(_T("Evolution Forge v1.10"), wxPoint(0, 0), wxSize(SCREEN_X,SCREEN_Y+37));
/*	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting map and goals"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up tables"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up status bar"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Initializing EF core"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Loading data files"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Starting main application"),(progress+1)*100/max)));progress++;*/

	frame->Show(TRUE);
//	if(bitmap) delete(bitmap);
//	if(splash) delete(splash);
	return TRUE;
}


void MyDCWindow::resetData()
{
	int i,k;
	maxpFitness=100;
	maxsFitness=100;
	maxtFitness=100;
	mintFitness=999;
	maxForce=1;
	maxUnitForce=1;

	currentForce=0;
																			    
	for(i=0;i<200;i++)
	{
		pFitness[i]=0;
		harvestedMins[i]=0;
		harvestedGas[i]=0;
		tFitness[i]=0;
		aFitness[i]=0;
		vFitness[i]=0; //~~
		length[i]=0; //ok
		time[i]=0;
		force[i]=5; //5 units at the beginning!
	};
																			    
	for(k=0;k<20;k++)
	{
		oldTimeCounter[k]=0;
		oldTime[k]=ga->maxTime;
		oldForceCounter[k]=0;
		oldForce[k]=0;
		oldGasCounter[k]=0;
		oldGas[k]=0;
		oldMinsCounter[k]=0;
		oldMins[k]=0;
	}
																			    
	for(k=0;k<FORCE_LIST_NUMBER;k++)
		oldForceList[k]=0;
		
	orderList.Clear();															    
	infoWindowNumber=0;
	endrun=0;
	gizmor=rand()%GIZMO_NUMBER;
	anarace=NULL;
	update=0;mouseLeft=0;
	userIsNewbie=1;
	for(k=0;k<UNIT_TYPE_COUNT*3;k++)
		buttonType[k]=99999;
	for(k=0;k<UNIT_TYPE_COUNT;k++)
		addUnitButton[k]=99999;
	for(k=0;k<MAX_LENGTH;k++)
		selection[k]=1;
	addGoalButton=99999;
//	addBoGoalButton=99999;
	goalListOpened=0;
	boInsertPoint=-1;
	boEndPoint=-1;
	boGoalListOpened=0;
	continueButton=9999;
	tutorialChapter=120;
}



MyDCWindow::MyDCWindow(wxFrame *parent)
	:wxWindow(parent, -1)
{
//	bmpBack2.LoadFile("back2.bmp",wxBITMAP_TYPE_BMP);
	dt1=wxDateTime::UNow();
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

// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	settings.loadHarvestFile("mining.txt");
	settings.loadSettingsFile("settings.txt");
// Map in "map.txt" is now map[0]
	settings.loadMapFile("map.txt");
// choose the first map we loaded (map[0])
	settings.setCurrentMap(0);
// Goal in "goal.txt" is now goal[0]
	settings.loadGoalFile("goal.txt");
// assign goal 0 to all players
	int i,k;
	for(i=1;i<settings.getMap(0)->getMaxPlayer();i++)
		settings.setGoal(0,i);
	update=0;
// initialize the soup, set the parameters, load the players etc.
	settings.initSoup();
	ga=settings.getGa();
	run=0;
//	for(k=0;k<MAX_LENGTH*50;k++)
//		oldOrder=new ORDER;
	oldrun=0;
	grey=0;
	resetData();
	dt2=wxDateTime::UNow();
	averagecounter=0;
	for(i=0;i<100;i++)
		average[i]=0;
	msgCount=0;
	for(k=0;k<1000;k++)
	{
		message[k].type=0;
		message[k].col=0;
		message[k].string=_T("");
	}
	addMessage(_T("Welcome to Evolution Forge!"));
	addMessage(_T("Click above to add new goals."));

	bitmap.LoadFile("clawsoftware.bmp", wxBITMAP_TYPE_BMP);
	bitmap2.LoadFile("clemens.bmp", wxBITMAP_TYPE_BMP);
	hintBitmap.LoadFile("hint.bmp", wxBITMAP_TYPE_BMP);
	bmpCancel.LoadFile("cancel.bmp", wxBITMAP_TYPE_BMP);
	bmpAdd.LoadFile("add.bmp",wxBITMAP_TYPE_BMP);
	bmpSub.LoadFile("sub.bmp",wxBITMAP_TYPE_BMP);
	bmpNumbers.LoadFile("numbers.bmp",wxBITMAP_TYPE_BMP);
	bmpAlpha.LoadFile("alpha.bmp",wxBITMAP_TYPE_BMP);
	bmpTreppe.LoadFile("treppe.bmp",wxBITMAP_TYPE_BMP);
	bmpCloning.LoadFile("cloning.bmp",wxBITMAP_TYPE_BMP);
	bmpFitness.LoadFile("fitness.bmp",wxBITMAP_TYPE_BMP);
																			    
	bmpRad[0].LoadFile("rad1.bmp",wxBITMAP_TYPE_BMP);
	bmpRad[1].LoadFile("rad2.bmp",wxBITMAP_TYPE_BMP);
	bmpRad[2].LoadFile("rad3.bmp",wxBITMAP_TYPE_BMP);
	bmpRad[3].LoadFile("rad4.bmp",wxBITMAP_TYPE_BMP);
	bmpRad[4].LoadFile("rad5.bmp",wxBITMAP_TYPE_BMP);
																			    
	bmpHeart[0].LoadFile("heart1.bmp",wxBITMAP_TYPE_BMP);
	bmpHeart[1].LoadFile("heart2.bmp",wxBITMAP_TYPE_BMP);
	bmpHeart[2].LoadFile("heart3.bmp",wxBITMAP_TYPE_BMP);
	bmpHeart[3].LoadFile("heart4.bmp",wxBITMAP_TYPE_BMP);
	bmpHeart[4].LoadFile("heart5.bmp",wxBITMAP_TYPE_BMP);
																			    
	bmpArrowLeft.LoadFile("arrow_left.bmp",wxBITMAP_TYPE_BMP);
	bmpArrowUp.LoadFile("arrow_up.bmp",wxBITMAP_TYPE_BMP);
	bmpArrowRight.LoadFile("arrow_right.bmp",wxBITMAP_TYPE_BMP);
	bmpArrowDown.LoadFile("arrow_down.bmp",wxBITMAP_TYPE_BMP);

/*	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+440,theCore->getInnerUpperBound()+50,8,8),bmpAdd,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+440,theCore->getInnerUpperBound()+60,8,8),bmpSub,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+440,theCore->getInnerUpperBound()+70,8,8),bmpCancel,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+50,8,8),bmpAdd,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+60,8,8),bmpSub,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+70,8,8),bmpCancel,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+150,8,8),bmpAdd,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+160,8,8),bmpSub,1);
	theCore->addBitmapButton(wxRect(theCore->getInnerLeftBound()+40,theCore->getInnerUpperBound()+170,8,8),bmpCancel,1);*/

	boWindow->addButton(wxRect(boWindow->getInnerLeftBound()+boWindow->getWidth()-48,boWindow->getInnerUpperBound()-30,12,12),1);
	boWindow->addButton(wxRect(boWindow->getInnerLeftBound()+boWindow->getWidth()-48,boWindow->getInnerUpperBound()-15,12,12),1);

	optimizeMode=0;
	wxtimer.SetOwner(this);
	wxtimer.Start(40);
};

MyDCWindow::~MyDCWindow()
{
	wxtimer.Stop();
};

void MyDCWindow::OnEraseBackground(wxEraseEvent& event)
{
};

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{
	msgBox=NULL;
	SetIcon(wxICON(icon));

#if wxUSE_MENUS
	menuFile=new wxMenu;
	menuHelp=new wxMenu;	
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
	menuFile->Append(EF_Start, _T("&Start\tAlt-S"), _T("Start/Continue calculation"));
	menuFile->Append(EF_Stop, _T("S&top\tAlt-T"), _T("Stop calculation"));
	menuFile->Append(EF_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
	menuSettings->Append(EF_GeneralSettings, _T("&General\tAlt-G"), _T("General settings"));
	menuGoal->Append(EF_GoalCreate, _T("C&reate\tAlt-R"),_T("Create new goal"));
//	menuGoal->Append(EF_GoalCreate, _T("C&reate\tAlt-R"),_T("Edit existing goal"));
	menuGoal->Append(EF_GoalImport, _T("&Import\tAlt-I"),_T("Import goal file"));
	menuMap->Append(EF_MapCreate, _T("&Create\tAlt-C"),_T("Create new map"));
//	menuGoal->Append(EF_GoalCreate, _T("C&reate\tAlt-R"),_T("Edit existing map"));
	menuMap->Append(EF_MapImport, _T("I&mport\tAlt-M"),_T("Import map file"));
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, _T("&File"));
	menuBar->Append(menuSettings, _T("&Settings"));
	menuBar->Append(menuGoal, _T("G&oals"));
	menuBar->Append(menuMap, _T("&Maps"));
	menuBar->Append(menuAdvanced, _T("&Advanced"));
	menuBar->Append(menuHelp, _T("&Help"));
	SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
	CreateStatusBar(2);
	SetStatusText(_T("Click on File/Start to start the calculation!"));
#endif // wxUSE_STATUSBAR

	mainWindow=new GraphixScrollWindow(1,wxRect(0,0,SCREEN_X,SCREEN_Y),wxRect(0,0,SCREEN_X,SCREEN_Y),0,0,NOT_SCROLLED,TABBED);
	mainWindow->addTitle(SCREEN_X/2,_T(wxString::Format(wxT("Evolution Forge v.%i "),EF_Version)));
	mainWindow->addTab(10,_T("Basic"));
	mainWindow->addTab(152,_T("Advanced"));
	mainWindow->addTab(294,_T("Expert"));
	mainWindow->addTab(436,_T("Gosu -_-"));
	mainWindow->addTab(578,_T("Transcend"));
	mainWindow->addTab(SCREEN_X-160,_T("Tutorial"));

	msgWindow=new GraphixScrollWindow(0,wxRect(0,0,FIRST_COLOUMN,120),wxRect(mainWindow->getInnerLeftBound(),mainWindow->getInnerLeftBound()+mainWindow->getInnerHeight()-125,FIRST_COLOUMN,120),mainWindow->getInnerLeftBound(),mainWindow->getInnerUpperBound()+mainWindow->getInnerHeight()-125,SCROLLED);
msgWindow->addTitle(0,"Messages");


	boWindow=new GraphixScrollWindow(0,wxRect(0,0,THIRD_COLOUMN,6*(FONT_SIZE+5)+3),wxRect(mainWindow->getInnerLeftBound()+mainWindow->getInnerWidth()-200,mainWindow->getInnerUpperBound()+SECOND_ROW,THIRD_COLOUMN,mainWindow->getInnerHeight()-30-120),mainWindow->getInnerLeftBound()+mainWindow->getInnerWidth()-200,mainWindow->getInnerUpperBound()+SECOND_ROW,SCROLLED,NOT_TABBED,wxRect(0,50,1000,1000));
boWindow->addTitle(0,"Build Order");
//      boWindow->addDescription(0,"Name");boWindow->addDescription(110,"Mins");boWindow->addDescription(150,"Gas");boWindow->addDescription(190,"Supply");boWindow->addDescription(243,"Time");

	goalForceWindow=new GraphixScrollWindow(0,wxRect(0,0,FIRST_COLOUMN,7*(FONT_SIZE+5)+8),wxRect(mainWindow->getInnerLeftBound(),mainWindow->getInnerUpperBound()+SECOND_ROW,FIRST_COLOUMN,mainWindow->getInnerHeight()-(mainWindow->getInnerUpperBound()+100)-10*(FONT_SIZE+5)+3-msgWindow->getHeight()-10),mainWindow->getInnerLeftBound(),mainWindow->getInnerUpperBound()+SECOND_ROW,SCROLLED);
goalForceWindow->addTitle(0,"Goal list");

	forceWindow=new GraphixScrollWindow(0,wxRect(0,0,FIRST_COLOUMN,4*(FONT_SIZE+5)+3),wxRect(goalForceWindow->getLeftBound(),goalForceWindow->getLowerBound()+5,FIRST_COLOUMN,mainWindow->getInnerHeight()-goalForceWindow->getLowerBound()-msgWindow->getHeight()-30),goalForceWindow->getLeftBound(),goalForceWindow->getLowerBound());forceWindow->addTitle(0,"Auxiliary units");

	timerWindow=new GraphixScrollWindow(0,wxRect(0,0,THIRD_COLOUMN,120),wxRect(mainWindow->getInnerLeftBound()+mainWindow->getInnerWidth()-200,mainWindow->getInnerUpperBound(),THIRD_COLOUMN,120),mainWindow->getInnerLeftBound()+mainWindow->getInnerWidth()-200,mainWindow->getInnerUpperBound(),NOT_SCROLLED);
timerWindow->addTitle(0,"Best time until now");

	statisticsWindow=new GraphixScrollWindow(0,wxRect(0,0,SECOND_COLOUMN,120),wxRect(timerWindow->getLeftBound()-505,mainWindow->getInnerUpperBound(),SECOND_COLOUMN,120),timerWindow->getLeftBound()-505,mainWindow->getInnerUpperBound(),NOT_SCROLLED);
	statisticsWindow->addTitle(0,"Statistical Data");

//	int y1;
//	if(statisticsWindow->getLowerBound()>forceWindow->getLowerBound()) y1=statisticsWindow->getLowerBound();
//		else y1=forceWindow->getLowerBound();

	theCore=new GraphixScrollWindow(0,wxRect(0,0,SECOND_COLOUMN,220),wxRect(statisticsWindow->getLeftBound(),mainWindow->getInnerUpperBound()+SECOND_ROW,SECOND_COLOUMN,220),statisticsWindow->getLeftBound(),mainWindow->getInnerUpperBound()+SECOND_ROW,NOT_SCROLLED);
	theCore->addTitle(0,"The Core of Evolution Forge");

	boDiagramWindow=new GraphixScrollWindow(0,wxRect(0,0,SECOND_COLOUMN,110),wxRect(statisticsWindow->getLeftBound(),theCore->getLowerBound()+5,SECOND_COLOUMN,260),statisticsWindow->getLeftBound(),theCore->getLowerBound()+5,NOT_SCROLLED);
	boDiagramWindow->addTitle(0,"Overview of best build order");

	boGraphWindow=new GraphixScrollWindow(0,wxRect(0,0,SECOND_COLOUMN,40),wxRect(statisticsWindow->getLeftBound(),boDiagramWindow->getLowerBound()+5,SECOND_COLOUMN,380),statisticsWindow->getLeftBound(),boDiagramWindow->getLowerBound()+5,NOT_SCROLLED); 
	boGraphWindow->addTitle(0,"Graphical illustration of the build order");

	infoWindow=new GraphixScrollWindow(2,wxRect(0,0,200,5+9*(FONT_SIZE+5)),wxRect(theCore->getLeftBound()+theCore->getWidth()-200,boWindow->getUpperBound(),200,theCore->getLowerBound()-boWindow->getUpperBound()),boWindow->getLeftBound()-200,boWindow->getUpperBound(),NOT_SCROLLED);
	infoWindow->addTitle(0,"Order description");

	tutorialWindow=new GraphixScrollWindow(0,wxRect(0,0,theCore->getWidth(),theCore->getHeight()+statisticsWindow->getHeight()+150),wxRect(mainWindow->getInnerLeftBound(),mainWindow->getInnerUpperBound(),mainWindow->getInnerWidth(),mainWindow->getInnerHeight()),mainWindow->getInnerLeftBound()+mainWindow->getInnerWidth()-theCore->getWidth(),mainWindow->getInnerUpperBound()+SECOND_ROW,NOT_SCROLLED,TABBED);
	tutorialWindow->addTitle(0,"Evolution Forge Tutorial");


//	haxor=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,550,100),0,boGraphWindow->getLeftBound(),boGraphWindow->getLowerBound()+10,0);
//	haxor->addTitle(0,"H4Xx0r 57uff! :D");

//TODO grey wieder rein... evtl bei draw

	boWindow->Show(1);
	goalForceWindow->Show(1);
	mainWindow->Show(1);
	msgWindow->Show(1);
	infoWindow->Show(1);
//	theCore->Show(1);
	boDiagramWindow->Show(1);
	boGraphWindow->Show(1);
	timerWindow->Show(1);
	forceWindow->Show(1);
//	statisticsWindow->Show(1);
	tutorialWindow->Show(0);
//	ShowFullScreen(true,wxFULLSCREEN_NOBORDER);
	
	child=new MyDCWindow(this);
}

void MyDCWindow::showGraph(int* data,int max,wxColour col)
{
	int i,j,k;
	dc->SetPen(wxPen(col,2,wxSOLID));
	wxPoint blub[200];
	j=1;
	k=0;
	for(i=1;i<199;i++)
	{
		if((data[i]!=data[i+1])||(k>9))
		{
			k=0;
		//TODO Hoehe 100 in clientarea.height aendern
			blub[j]=wxPoint(i+statisticsWindow->getInnerLeftBound(),statisticsWindow->getInnerUpperBound()+100-data[i]/(max+1));
			j++;
		} else k++;
	}
	blub[0]=wxPoint(statisticsWindow->getInnerLeftBound(),statisticsWindow->getInnerUpperBound()+100-data[0]/(max+1));
	blub[j]=wxPoint(199+statisticsWindow->getInnerLeftBound(),statisticsWindow->getInnerUpperBound()+100-data[199]/(max+1));
	dc->DrawSpline(j+1,blub);
};

void MyDCWindow::showTimer()
{
	int i,j,k;
//	dc->DrawBitmap(bmpTimer,THIRD_COLOUMN-10,0,false);
	for(i=0;i<20;i++)
		for(j=0;j<i;j++)
			if(oldTimeCounter[j]<oldTimeCounter[i])
			{
				int tee=oldTimeCounter[i];
				oldTimeCounter[i]=oldTimeCounter[j];
				oldTimeCounter[j]=tee;
				tee=oldTime[i];
				oldTime[i]=oldTime[j];
				oldTime[j]=tee;
			}
												       
	for(k=0;k<20;k++)
		if(oldTimeCounter[k]>0)
		{
			dc->SetTextForeground(convertColour(160-oldTimeCounter[k]*8,0,0));
			dc->SetFont(GraphixScrollWindow::font4);
//wxFont(30,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));

			wxString bla=_T(wxString::Format(wxT("[%.2i:%.2i]"),oldTime[k]/60,oldTime[k]%60));
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
												       
			dc->DrawText(bla,timerWindow->getInnerLeftBound()+(timerWindow->getInnerWidth()-dx)/2+oldTimeCounter[k],timerWindow->getInnerUpperBound()+(timerWindow->getInnerHeight()-dy)/2-oldTimeCounter[k]/2);
												       
			oldTimeCounter[k]++;
			if(oldTimeCounter[k]>20)
			{
				oldTimeCounter[k]=0;
				oldTime[k]=0;
			}
		}
	wxString bla;
	wxString bla2;
	dc->SetFont(GraphixScrollWindow::font5);
//wxFont(24,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
	dc->SetTextForeground(convertColour(200,0,0));
	if(anarace->getTimer()==0)
	{
		if(!run) bla=_T("(paused)");
		else
			bla=_T("Searching...");
		int dx,dy;
		dc->GetTextExtent(bla,&dx,&dy);
		dc->DrawText(bla,timerWindow->getInnerLeftBound()+(timerWindow->getInnerWidth()-dx)/2,timerWindow->getInnerUpperBound()+(timerWindow->getInnerHeight()-dy)/2);
	}
	else 
	{
		dc->SetFont(GraphixScrollWindow::font6);
//wxFont(12,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		dc->DrawText(_T(wxString::Format(wxT("%i%% theoretical optimum"),anarace->getPercentage())),timerWindow->getInnerLeftBound(),timerWindow->getInnerUpperBound());
		dc->SetFont(GraphixScrollWindow::font5);
//wxFont(24,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
	   bla=_T(wxString::Format(wxT("[%.2i:%.2i]"),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60));
		if(run)
			bla2=_T("Optimizing...");
		else bla2=_T("(paused)");
	   int dx,dy,yy;
	   dc->GetTextExtent(bla,&dx,&dy);
		yy=(timerWindow->getInnerHeight()-dy)/3;
	   dc->DrawText(bla,timerWindow->getInnerLeftBound()+(timerWindow->getInnerWidth()-dx)/2,timerWindow->getInnerUpperBound()+yy);
	   dc->GetTextExtent(bla2,&dx,&dy);
	   dc->DrawText(bla2,timerWindow->getInnerLeftBound()+(timerWindow->getInnerWidth()-dx)/2,timerWindow->getInnerUpperBound()+2*yy);
	}
}

void MyDCWindow::showProgramGraph()
{
	int s,y1;
	wxPoint mins[MAX_TIME];
	wxPoint gas[MAX_TIME];
//	wxPoint fitness[MAX_LENGTH];
	wxPoint supply[MAX_TIME];
	int time;
	if(ga->maxTime-anarace->getTimer()<2) return;
	int count=0;
	if(infoWindowNumber)
		dc->SetBrush(wxBrush(wxColour(30,30,30),wxCROSS_HATCH));
	else
		dc->SetBrush(wxBrush(wxColour(50,50,50),wxCROSS_HATCH));
	dc->DrawRectangle(boDiagramWindow->getInnerLeftBound(),boDiagramWindow->getInnerUpperBound()+10,boDiagramWindow->getInnerWidth(),boDiagramWindow->getInnerHeight()-10);

	if(anarace->getTimer()==ga->maxTime) time=0;
		else time=anarace->getTimer()+1;
	s=ga->maxTime-1;
	while(s>=time)
	{
		if(anarace->getStatisticsHaveMinerals(s)>75000) y1=75; else y1=anarace->getStatisticsHaveMinerals(s)/1000;
		mins[count]=wxPoint(boDiagramWindow->getInnerLeftBound()+3+((count*(boDiagramWindow->getInnerWidth()-6))/(ga->maxTime-time)),boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-y1);

		if(anarace->getStatisticsHaveGas(s)>75000) y1=75; else y1=anarace->getStatisticsHaveGas(s)/1000;
		gas[count]=wxPoint(boDiagramWindow->getInnerLeftBound()+3+((count*(boDiagramWindow->getInnerWidth()-6))/(ga->maxTime-time)),boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-y1);
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!

/*		y1=anarace->getStatisticsFitness(s)*75/(anarace->getMaxpFitness()-anarace->getTimer());
		fitness[count]=wxPoint(boDiagramWindow->getInnerLeftBound()+3+((s*(boDiagramWindow->getInnerWidth()-6))/(ga->maxTime-anarace->getTimer())),boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-y1);*/

		if(anarace->getStatisticsHaveSupply(s)-anarace->getStatisticsNeedSupply(s)>15) y1=75; else 
		if(anarace->getStatisticsHaveSupply(s)<anarace->getStatisticsNeedSupply(s)) y1=0; else
		y1=(anarace->getStatisticsHaveSupply(s)-anarace->getStatisticsNeedSupply(s))*5;

		supply[count]=wxPoint(boDiagramWindow->getInnerLeftBound()+3+((count*(boDiagramWindow->getInnerWidth()-6))/(ga->maxTime-time)),boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-y1);
		count++;
		s--;
	}
												       
	if(count>0)
	{
		dc->SetFont(GraphixScrollWindow::font2);
		if(infoWindowNumber)
		{
			dc->SetPen(wxPen(convertColour(80,80,80),2,wxSOLID));
			dc->DrawSpline(count,supply);
			dc->SetPen(wxPen(convertColour(60,60,200),2,wxSOLID));
			dc->DrawSpline(count,mins);
			dc->SetPen(wxPen(convertColour(20,160,20),2,wxSOLID));
			dc->DrawSpline(count,gas);
//			dc->SetPen(wxPen(convertColour(255,40,40),2,wxSOLID));
//			dc->DrawSpline(count,fitness);
//			dc->SetTextForeground(convertColour(255,40,40));	 
//			dc->DrawText(_T("Fitness"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+8);
			dc->SetTextForeground(convertColour(60,60,200));	 
			dc->DrawText(_T("Minerals"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+10);
			dc->SetTextForeground(convertColour(20,200,20));
			dc->DrawText(_T("Gas"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+21);
			dc->SetTextForeground(convertColour(120,120,120));
			dc->DrawText(_T("Supply"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+32); 
		} else
		{
			dc->SetPen(wxPen(convertColour(120,120,120),2,wxSOLID));
			dc->DrawSpline(count,supply);
			dc->SetPen(wxPen(convertColour(80,80,255),2,wxSOLID));
			dc->DrawSpline(count,mins);
			dc->SetPen(wxPen(convertColour(40,255,40),2,wxSOLID));
			dc->DrawSpline(count,gas);
//			dc->SetPen(wxPen(convertColour(255,40,40),2,wxSOLID));
//			dc->DrawSpline(count,fitness);
//			dc->SetTextForeground(convertColour(255,40,40));	 
//			dc->DrawText(_T("Fitness"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+8);
			dc->SetTextForeground(convertColour(80,80,255));	 
			dc->DrawText(_T("Minerals"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+10);
			dc->SetTextForeground(convertColour(40,255,40));
			dc->DrawText(_T("Gas"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+21);
			dc->SetTextForeground(convertColour(160,160,160));
			dc->DrawText(_T("Supply"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+32); 
		}
		dc->SetPen(wxPen(convertColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
	}
	
	if(infoWindowNumber)
	{
		ORDER* order=orderList.Find(infoWindowNumber-1)->GetData();
		dc->SetPen(wxPen(wxColour(150,0,0),2,wxSHORT_DASH));
		dc->DrawLine(boDiagramWindow->getInnerLeftBound()+order->bx,boDiagramWindow->getInnerUpperBound()+10,boDiagramWindow->getInnerLeftBound()+order->bx,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight());
		dc->DrawLine(boDiagramWindow->getInnerLeftBound()+order->bx+order->bwidth,boDiagramWindow->getInnerUpperBound()+10,boDiagramWindow->getInnerLeftBound()+order->bx+order->bwidth,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight());
		if(stats[anarace->getPlayer()->getRace()][order->unit].mins)
		{
			dc->SetPen(wxPen(wxColour(120,120,255),2,wxSHORT_DASH));
			dc->DrawLine(boDiagramWindow->getInnerLeftBound()+order->bx+1,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].mins/1000,boDiagramWindow->getInnerLeftBound()+order->bx+order->bwidth-1,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].mins/1000);

		}
		if(stats[anarace->getPlayer()->getRace()][order->unit].gas)
		{
			dc->SetPen(wxPen(wxColour(80,255,80),2,wxSHORT_DASH));
			dc->DrawLine(boDiagramWindow->getInnerLeftBound()+order->bx+1,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].gas/1000,boDiagramWindow->getInnerLeftBound()+order->bx+order->bwidth-1,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].gas/1000); 
		}
		if(stats[anarace->getPlayer()->getRace()][order->unit].supply)
		{
			dc->SetPen(wxPen(wxColour(160,160,160),2,wxSHORT_DASH));
			dc->DrawLine(boDiagramWindow->getInnerLeftBound()+order->bx+1,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].supply*5,boDiagramWindow->getInnerLeftBound()+order->bx+order->bwidth-1,boDiagramWindow->getInnerUpperBound()+boDiagramWindow->getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].supply*5);
		}
	}
}

void MyDCWindow::showToolTip()
{
	int what=0;
	if(boWindow->getButtonRect(0).Inside(mouseX,mouseY))
		what=1;
	else if(boWindow->getButtonRect(1).Inside(mouseX,mouseY))
		what=2;
	dc->SetFont(GraphixScrollWindow::font2);
	dc->SetTextForeground(convertColour(255,40,40));
	dc->DrawText(_T("Fitness"),boDiagramWindow->getInnerLeftBound()+1,boDiagramWindow->getInnerUpperBound()+8);
	switch(what)
	{
		case 1:dc->DrawText(_T("Mutation will affect every part of the build order"),mouseX,mouseY);break; //~~
		case 2:dc->DrawText(_T("Mutation will affect only selected parts"),mouseX,mouseY);break;
		default:break;
	};
}

void MyDCWindow::analyzeData()
{
	int i,t;
	t=0;
	maxUnitForce=1;
	for(i=0;i<=GAS_SCV;i++)
	{
		if(anarace->getLocationForce(0,i)>maxUnitForce)
			maxUnitForce=anarace->getLocationForce(0,i);
		if(anarace->getPlayer()->goal->allGoal[i]>maxUnitForce)
			maxUnitForce=anarace->getPlayer()->goal->allGoal[i];
		t+=anarace->getLocationForce(0,i);;
	}
	if(t>maxForce) maxForce=t;
	currentForce=t;
};

void MyDCWindow::ProgressGraph()
{
	int i,k;
		for(i=0;i<199;i++)
		{
			pFitness[i]=pFitness[i+1];
			harvestedMins[i]=harvestedMins[i+1];
			harvestedGas[i]=harvestedGas[i+1];
			tFitness[i]=tFitness[i+1];
			aFitness[i]=aFitness[i+1];
			vFitness[i]=vFitness[i+1];
			length[i]=length[i+1];
			time[i]=time[i+1];
			force[i]=force[i+1];
		};
	
		if(anarace->getMaxpFitness()>maxpFitness) maxpFitness=anarace->getMaxpFitness();
		if(anarace->getMaxsFitness()>maxsFitness) maxsFitness=anarace->getMaxsFitness();
		if(anarace->getMaxtFitness()>maxtFitness) maxtFitness=anarace->getMaxtFitness();
		if(anarace->getMaxtFitness()<mintFitness) mintFitness=anarace->getMaxtFitness();

		pFitness[199]=anarace->getMaxpFitness()*100;
		harvestedMins[199]=anarace->getHarvestedMins()*100;
		harvestedGas[199]=anarace->getHarvestedGas()*100;
		tFitness[199]=(anarace->getMaxtFitness()-mintFitness)*100;
		aFitness[199]=anarace->fitnessAverage*100;
		vFitness[199]=(int)sqrt((double)anarace->fitnessVariance)*100;
		length[199]=anarace->getLength()*100;
		time[199]=(ga->maxTime-anarace->getTimer())*100;
		force[199]=currentForce*100;
	
		if(time[199]<time[198])
		{
			k=0;
			while(oldTimeCounter[k]>0)
			{
				k++;
				if(k>19) k=0;
			}
			oldTimeCounter[k]=1;
			oldTime[k]=time[198]/100;
		}


		if(force[199]!=force[198])
		{
			k=0;
			while(oldForceCounter[k]>0)
			{
				k++;
				if(k>19) k=0;
			}
			oldForceCounter[k]=1;
			oldForce[k]=(force[199]-force[198])/100;
		}
		if(harvestedGas[199]!=harvestedGas[198])
		{
			k=0;
			while(oldGasCounter[k]>0)
			{
				k++;
				if(k>19) k=0;
			}
			oldGasCounter[k]=1;
			oldGas[k]=(harvestedGas[199]-harvestedGas[198])/100;
		}
		if(harvestedMins[199]!=harvestedMins[198])
		{
			k=0;
			while(oldMinsCounter[k]>0)
			{
				k++;
				if(k>19) k=0;
			}
			oldMinsCounter[k]=1;
			oldMins[k]=(harvestedMins[199]-harvestedMins[198])/100;
		}
};

void MyDCWindow::CheckOldOrders()
{
	int s;
//TODO mal pruefen ob Marker wirklich eindeutig sind! <- jo sindse, wenn man kein crossover macht
		int k=0;
		for(s=MAX_LENGTH;s--;)
		if(anarace->getProgramIsBuilt(s)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
		{
			if(NODE* node=orderList.Find(anarace->getMarker(s)))
			{
				ORDER* oldOrder=node->GetData();
				oldOrder->targetx=0;
				oldOrder->targety=(k+1)*(FONT_SIZE+5);
				oldOrder->row=k+1;
				oldOrder->blendTarget=50;
				oldOrder->unit=anarace->phaenoCode[s];
				oldOrder->IP=s;
				oldOrder->targetwidth=SECOND_COLOUMN-8;
				oldOrder->checked=1;
			} // => aktualisieren
			else // => neues erstellen
			{
				//TODO: testen ob anderes item da ist, das aber die gleiche Unit besitzt + an die gleiche Position kommt
				int found=0;
				NODE *node=orderList.GetFirst();
				while(node&&(!found))
				{
					if((node->GetData()->unit!=anarace->phaenoCode[s])||(node->GetData()->row!=k+1)||(node->GetData()->targetx>0))
						node=node->GetNext();
					else //=> ueberschreiben
					{
						ORDER* oldOrder=node->GetData();
						oldOrder->blendTarget=50;
						//oldOrder->dx=0;oldOrder->dy=0;
						//oldOrder->y=(k+1)*(FONT_SIZE+5);
						oldOrder->row=k+1; //?
						oldOrder->targety=(k+1)*(FONT_SIZE+5); //?
						oldOrder->targetx=0;
						oldOrder->bonew=1;
						oldOrder->IP=s;
						oldOrder->unit=anarace->phaenoCode[s];
						oldOrder->targetwidth=SECOND_COLOUMN-8;
						oldOrder->checked=1;
						found=1;
					}
				}
				if(!found)
				{
					ORDER* oldOrder=new ORDER;
					oldOrder->x=170;oldOrder->y=(k+1)*(FONT_SIZE+5);
					oldOrder->dx=0;oldOrder->dy=0;
					oldOrder->blend=1;oldOrder->blendTarget=50;
					oldOrder->targetx=0;oldOrder->targety=oldOrder->y;
					oldOrder->bonew=1;
					oldOrder->targetwidth=SECOND_COLOUMN-8;
					oldOrder->unit=anarace->phaenoCode[s];
					oldOrder->row=k+1;
					oldOrder->IP=s;
					oldOrder->checked=1;
					orderList.Append((long)anarace->getMarker(s),oldOrder);
				}
			}
			k++;
		}


		NODE* node=orderList.GetFirst();

                while(node)
		{
			if(!node->GetData()->checked)
			{
				NODE* temp=node->GetNext();
				orderList.DeleteNode(node);
				node=temp;
			}
			else
			{
				node->GetData()->checked=0;
				node=node->GetNext();
			}
		}

};

void move(int& pos, int target)
{
	if(target>pos)
		pos+=1+(target-pos)/2;
	else if(target<pos)
		pos-=1+(pos-target)/2;
};

void MyDCWindow::MoveOldOrders()
{
	NODE *node=orderList.GetFirst();
	while(node)
	{
		ORDER* oldOrder=node->GetData();
		oldOrder->dx+=(oldOrder->targetx-oldOrder->x)/30;
		oldOrder->x+=(oldOrder->targetx-oldOrder->x)/3+oldOrder->dx+(oldOrder->targetx>oldOrder->x)-(oldOrder->targetx<oldOrder->x);
		oldOrder->dy+=(oldOrder->targety-oldOrder->y-oldOrder->dy)/30;
		oldOrder->y+=(oldOrder->targety-oldOrder->y)/3+oldOrder->dy+(oldOrder->targety>oldOrder->y)-(oldOrder->targety<oldOrder->y);

		if(abs(oldOrder->x-oldOrder->targetx)<7) {oldOrder->x=oldOrder->targetx;oldOrder->dx=0;}
		if(abs(oldOrder->y-oldOrder->targety)<7) {oldOrder->y=oldOrder->targety;oldOrder->dy=0;}

		move((oldOrder->bx),oldOrder->targetbx);
		move((oldOrder->by),oldOrder->targetby);
		move((oldOrder->width),oldOrder->targetwidth);
		move((oldOrder->bwidth),oldOrder->targetbwidth);
		move((oldOrder->bheight),oldOrder->targetbheight);
		move((oldOrder->blend),oldOrder->blendTarget);
		node=node->GetNext();
	} 
};

int MyDCWindow::CheckForInfoWindow()
{
	NODE *node=orderList.GetFirst();
	while(node)
	{
		ORDER* oldOrder=node->GetData();
		int row=((boInsertPoint>-1)&&(oldOrder->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
		wxRect edge2(oldOrder->bx,oldOrder->by/*-boGraphWindow->getScrollY()*/,oldOrder->bwidth,oldOrder->bheight);
		wxRect edge(oldOrder->x,oldOrder->y-boWindow->getScrollY()+row*(FONT_SIZE+5)-1,oldOrder->width,FONT_SIZE+6);
		if((boWindow->fitToClientArea(edge)&&edge.Inside(mouseX-boWindow->getInnerLeftBound(),mouseY-boWindow->getInnerUpperBound())))
		{
			infoWindow->adjustWindow(wxRect(infoWindow->getLeftBound(),edge.y+boWindow->getInnerUpperBound()-50,infoWindow->getWidth(),infoWindow->getHeight()));
			return(node->GetKey()+1);
		}
		else if((boGraphWindow->fitToClientArea(edge2)&&edge2.Inside(mouseX-boGraphWindow->getInnerLeftBound(),mouseY-boGraphWindow->getInnerUpperBound())))
		{
			infoWindow->adjustWindow(wxRect(infoWindow->getLeftBound(),boDiagramWindow->getUpperBound()-infoWindow->getHeight(),infoWindow->getWidth(),infoWindow->getHeight()));
			return(node->GetKey()+1);
		}
		node=node->GetNext();
	}
	return(0);
};


void MyDCWindow::showBoGraph()
{
	int s,k,j,i,t;
	struct BOGRAPH
	{
		int type;
		int position;
		int height;
		int lines;
	} bograph[20];
	int fac[20];
	int unitCounter[UNIT_TYPE_COUNT][MAX_LENGTH];
	int height[UNIT_TYPE_COUNT];
	int lines[UNIT_TYPE_COUNT];
	int faccount=0;


	for(k=0;k<UNIT_TYPE_COUNT;k++)
	{
		for(j=0;j<MAX_LENGTH;j++)
			unitCounter[k][j]=0;
		height[k]=0;
		lines[k]=0;
	}
	for(k=0;k<20;k++)
	{
		bograph[k].type=0;
		bograph[k].position=0;
		bograph[k].height=0;
		bograph[k].lines=0;
		fac[k]=0;
	}

//calculate maximum height per facility
	NODE* node=orderList.GetFirst();
	while(node)
	{
		int IP=node->GetData()->IP;
		if(anarace->getProgramFacility(IP)&&(anarace->getProgramForceCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP))>height[anarace->getProgramFacility(IP)]))
			 height[anarace->getProgramFacility(IP)]=anarace->getProgramForceCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP));
		node=node->GetNext();
	}
																		 
//calculate number of lines per facility and adjust the height
	for(s=UNIT_TYPE_COUNT;s--;)
	{
		while(height[s]>MIN_HEIGHT)
		{
			height[s]-=MIN_HEIGHT;
			lines[s]++;
		}
		if(height[s]>0) lines[s]++;
		if(lines[s]>1)
			height[s]=MIN_HEIGHT;
	}
																			    
//make a list of facilities that are needed...
	node=orderList.GetFirst();
	faccount=1;
	while(node)
	{
		if(anarace->getProgramFacility(node->GetData()->IP))
		{
			for(i=1;i<faccount;i++)
				if(fac[i]==anarace->getProgramFacility(node->GetData()->IP)) break;
			if(i==faccount)
			{
				fac[i]=anarace->getProgramFacility(node->GetData()->IP);
				faccount++;
			}
		}
		node=node->GetNext();
	}

// ...and sort them
	for(i=0;i<20;i++)
		if(fac[i])
			for(j=0;j<i;j++)
// scvs last	
				if(fac[i]>fac[j])
				{
					int temp=fac[i];fac[i]=fac[j];fac[j]=temp;
				}
																			    
// now put all together
	int position=0;
	for(i=0;i<20;i++)
	{
		bograph[position].type=fac[i];
		bograph[position].lines=lines[fac[i]];
		bograph[position].height=height[fac[i]];
		position+=lines[fac[i]];
	}
//sort the oldOrder after IPs
	orderList.Sort();

// now calculate the rectangles :)
	int hoehe=0;
	dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));

	node=orderList.GetFirst();
	while(node)
	{
		ORDER* oldOrder=node->GetData();
		if(anarace->getProgramFacility(oldOrder->IP))
			for(i=0;i<20;i++)
				if(bograph[i].type==anarace->getProgramFacility(oldOrder->IP))
				{
//oldOrder->time muesste vorsortiert sein
					for(j=0;j<MAX_LENGTH;j++)
       						if(unitCounter[bograph[i].type][j]<=anarace->getProgramTime(oldOrder->IP))
						{
							unitCounter[bograph[i].type][j]=anarace->getProgramTime(oldOrder->IP)+stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].BT;
							hoehe=j;
       							j=MAX_LENGTH;
		 			       }
					oldOrder->targetbx=((anarace->getProgramTime(oldOrder->IP)*boGraphWindow->getInnerWidth())/(ga->maxTime-anarace->getTimer()));
					oldOrder->targetby=(i+hoehe/MIN_HEIGHT)*(FONT_SIZE+5)+(hoehe%MIN_HEIGHT)*(FONT_SIZE+4)/bograph[i].height;
																			    
					oldOrder->targetbwidth=((stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].BT/*anarace->getProgramBT(s)*/*boGraphWindow->getInnerWidth())/(ga->maxTime-anarace->getTimer()));
					if(oldOrder->targetbwidth<6) oldOrder->targetbwidth=6;
					oldOrder->targetbheight=(FONT_SIZE+4)/(bograph[i].height);
					if(oldOrder->bonew)
					{
						oldOrder->bonew=0;
						oldOrder->by=oldOrder->targetby;
						oldOrder->bx=oldOrder->targetbx;
						oldOrder->bwidth=oldOrder->targetbwidth;
						oldOrder->bheight=oldOrder->targetbheight;
					}
				}
		node=node->GetNext();
	}

// and the time steps on the top
	dc->SetTextForeground(convertColour(50,200,50));
	dc->SetPen(wxPen(convertColour(200,0,0),1,wxSOLID));
	int timesteps=((ga->maxTime-anarace->getTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
	for(i=0;i<(ga->maxTime-anarace->getTimer())/30;i++)
		if(i%timesteps==0)
		{
//			dc->DrawLine(boGraphWindow->getInnerLeftBound()+(i+timesteps)*(boGraphWindow->getInnerWidth()/((ga->maxTime-anarace->getTimer())/30)),boGraphWindow->getInnerUpperBound(),
//				boGraphWindow->getInnerLeftBound()+(i+timesteps)*(boGraphWindow->getInnerWidth()/((ga->maxTime-anarace->getTimer())/30)),boGraphWindow->getInnerUpperBound()+FONT_SIZE+4);
			dc->DrawText(_T(wxString::Format(wxT("%i:%i0"),i/2,3*(i%2))),boGraphWindow->getInnerLeftBound()+5+i*((boGraphWindow->getInnerWidth()-20)/((ga->maxTime-anarace->getTimer())/30)),boGraphWindow->getInnerUpperBound());
		}

	for(i=0;i<(ga->maxTime-anarace->getTimer())/30;i++)
		if(i%timesteps==0)
		{
  //		      dc->DrawLine(boDiagramWindow->getInnerLeftBound()+(i+timesteps)*(boDiagramWindow->getInnerWidth()/((ga->maxTime-anarace->getTimer())/30)),boDiagramWindow->getInnerUpperBound(),
    //			    boDiagramWindow->getInnerLeftBound()+(i+timesteps)*(boDiagramWindow->getInnerWidth()/((ga->maxTime-anarace->getTimer())/30)),boDiagramWindow->getInnerUpperBound()+FONT_SIZE+4);
			dc->DrawText(_T(wxString::Format(wxT("%i:%i0"),i/2,3*(i%2))),boDiagramWindow->getInnerLeftBound()+5+i*((boDiagramWindow->getInnerWidth()-20)/((ga->maxTime-anarace->getTimer())/30)),boDiagramWindow->getInnerUpperBound());
		}
//TODO nochmal timesteps checken

// --------------------------------- END BUILD ORDER GRAPH ------------------------------


// now print the rectangles :)
	dc->SetPen(wxPen(convertColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
	wxRect edge;
	edge.height=FONT_SIZE+4;
	edge.width=10000;//boGraphWindow->getInnerWidth();

	int lastbograph=-1;
	for(i=0;i<20;i++)
	if(bograph[i].type>0)
		for(j=i;j<i+bograph[i].lines;j++)
		{
			edge.x=0;
			edge.y=j*(FONT_SIZE+5);
			if(edge.y>lastbograph)
				lastbograph=edge.y;

			if(boGraphWindow->fitToClientArea(edge))
			{
				edge.x+=boGraphWindow->getInnerLeftBound();
				edge.y+=boGraphWindow->getInnerUpperBound();
				
				if(j%2==0)
					dc->SetBrush(wxBrush(convertColour(COLOR1G,COLOR1G,COLOR1B),wxSOLID));
				else dc->SetBrush(wxBrush(convertColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
	    		      	dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
			}
		}

	boGraphWindow->adjustWindow(wxRect(boGraphWindow->getLeftBound(),boGraphWindow->getUpperBound(),boGraphWindow->getWidth(),lastbograph+40));

	node=orderList.GetFirst();
	while(node)
	{
		ORDER* oldOrder=node->GetData();
		if(anarace->getProgramFacility(oldOrder->IP))
		{
			edge.x=oldOrder->bx;
			edge.y=oldOrder->by/*+t*(FONT_SIZE+5)*/;
			edge.width=oldOrder->bwidth;
			edge.height=oldOrder->bheight;
			if(boGraphWindow->fitToClientArea(edge))
			{
				edge.x+=boGraphWindow->getInnerLeftBound();
				edge.y+=boGraphWindow->getInnerUpperBound();

dc->SetBrush(wxBrush(convertColour(
(100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+COLOR1R+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].type].Red())*oldOrder->blend/50,
(100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+(optimizeMode*selection[orderLength])*255+(1-(optimizeMode*selection[orderLength]))*(COLOR1G+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].type].Green())*oldOrder->blend/50),
(100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+COLOR1B+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].type].Blue())*oldOrder->blend/50),wxSOLID));


				dc->SetBrush(wxBrush(convertColour(
		((anarace->Code[anarace->getProgramDominant(oldOrder->IP)][oldOrder->IP]+1)*155/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%156+100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1),
		((anarace->Code[anarace->getProgramDominant(oldOrder->IP)][oldOrder->IP]+1)*155/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%156+100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1),
		((anarace->Code[anarace->getProgramDominant(oldOrder->IP)][oldOrder->IP]+1)*155/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%156+100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)),wxSOLID));
				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
			}
		}
		node=node->GetNext();
	}



	//finally print the legend
        dc->SetTextForeground(convertColour(0,0,0));
	for(i=0;i<20;i++)
		if(bograph[i].type>0)
			dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][bograph[i].type].name)),boGraphWindow->getInnerLeftBound(),boGraphWindow->getInnerUpperBound()+(i)*(FONT_SIZE+5));



};

void MyDCWindow::processButtons()
{
	int i,j;
	for(i=0;i<TUTORIAL+1;i++) //always show main window
		if(mainWindow->isPressed(i))
		{
			mainWindow->currentTab=i;
			mainWindow->clearButtons();
			switch(mainWindow->currentTab)
			{
				case BASIC:
					boWindow->Show(1);
					goalForceWindow->Show(1);
					msgWindow->Show(1);
					infoWindow->Show(0);
					theCore->Show(0);
					boDiagramWindow->Show(1);
					boGraphWindow->Show(1);
					timerWindow->Show(1);
					forceWindow->Show(1);
					statisticsWindow->Show(0);
					tutorialWindow->Show(0);
					break;
			
		case ADVANCED:
			boWindow->Show(1);
			goalForceWindow->Show(1);
			msgWindow->Show(1);
			infoWindow->Show(0);
			theCore->Show(1);
			boDiagramWindow->Show(1);
			boGraphWindow->Show(1);
			timerWindow->Show(1);
			forceWindow->Show(1);
			statisticsWindow->Show(1);
			tutorialWindow->Show(0);
			break;
		case EXPERT:
			boWindow->Show(1);
			goalForceWindow->Show(1);
			msgWindow->Show(1);
			infoWindow->Show(1);
			theCore->Show(0);
			boDiagramWindow->Show(1);
			boGraphWindow->Show(1);
			timerWindow->Show(1);
			forceWindow->Show(1);
			statisticsWindow->Show(0);
			tutorialWindow->Show(0);
			break;
		case GOSU:
			boWindow->Show(1);
			goalForceWindow->Show(1);
			msgWindow->Show(1);
			infoWindow->Show(1);
			theCore->Show(1);
			boDiagramWindow->Show(1);
			boGraphWindow->Show(1);
			timerWindow->Show(1);
			forceWindow->Show(1);
			statisticsWindow->Show(1);
			tutorialWindow->Show(0);
			break;
		case TRANSCENDEND:
                        boWindow->Show(1);
                        goalForceWindow->Show(1);
                        msgWindow->Show(1);
                        infoWindow->Show(1);
                        theCore->Show(1);
                        boDiagramWindow->Show(1);
                        boGraphWindow->Show(1);
                        timerWindow->Show(1);
                        forceWindow->Show(1);
                        statisticsWindow->Show(1);
                        tutorialWindow->Show(0);
                        break;
		case TUTORIAL:
			boWindow->Show(0);
			goalForceWindow->Show(0);
			msgWindow->Show(0);
			infoWindow->Show(0);
			theCore->Show(0);
			boDiagramWindow->Show(0);
			boGraphWindow->Show(0);
			timerWindow->Show(0);
			forceWindow->Show(0);
			statisticsWindow->Show(0);
			tutorialWindow->Show(1);
			break;
		default:break;		
	}

		}


//TODO scroll miteinbeziehen
	if(boWindow->isShown())
	{
		for(i=0;i<MAX_LENGTH;i++)
		{
			if(boWindow->isPressed(optButton[i]))
			{
				if((boInsertPoint==-1)||(i<boInsertPoint))
					selection[i]=!selection[i];
				else if(i>boEndPoint)
 				       selection[boInsertPoint-boEndPoint+i]=!selection[boInsertPoint-boEndPoint+i];
			}
			if(boWindow->isPressed(unitButton[i]))
			{
   				addMessage(_T(wxString::Format(wxT("Button %i pressed"),i)));
				if(boInsertPoint==-1) //all closed
				{
					addMessage(_T(wxString::Format(wxT("Opening menu at %i"),i)));
					boInsertPoint=i+1;
					run=0;
					boGoalListOpened=1; //scheint zu funzen
				}
				else if(i<boInsertPoint)
				{
     			 		addMessage(_T(wxString::Format(wxT("Closing menu at %i and opening menu at %i"),boInsertPoint,i)));
					boInsertPoint=i+1;
					run=0;
					boGoalListOpened=1;
				} 
				else if(i>=boEndPoint)
				{
					addMessage(_T(wxString::Format(wxT("Closing menu at %i and opening menu at %i"),boInsertPoint,boInsertPoint-boEndPoint+i)));
					boInsertPoint=boInsertPoint-boEndPoint+i+1;
					run=0;
					boGoalListOpened=1;
				} else
				{
					if(/*(i+1==boEndPoint)||*/(i==boInsertPoint)) //oeffne/schliesse Knopf <- aha...
					{
						if(boGoalListOpened==1) //=> schliesse
						{
							addMessage(_T(wxString::Format(wxT("Closing menu at %i"),boInsertPoint)));
							boInsertPoint=-1;
							boEndPoint=0;
							boGoalListOpened=0;
						} // => schliessen
						else if(boGoalListOpened>1)
						{
							addMessage(_T(wxString::Format(wxT("Returning to 1. menu at %i"),boInsertPoint)));
							boGoalListOpened=1;
						} // => schliessen
					} else // aha ein menueeintrag
					{
//						addMessage(_T(wxString::Format(wxT("%i %i %i"),boInsertPoint,boEndPoint,i)));
						if(boGoalListOpened==1) // => 1. Untermenue
						{
							if(i==boInsertPoint+1)
							{
								anarace->insertOrder(SCV,boInsertPoint);
								addMessage(_T(wxString::Format(wxT("Item 1 was chosen")))); // => scv
								update=2;
							}
							else if(i==boInsertPoint+2)
							{
								anarace->insertOrder(GAS_SCV,boInsertPoint);
								addMessage(_T(wxString::Format(wxT("Item 2 was chosen")))); // => gas scv
								update=2;
							}
							else 
							{
								addMessage(_T(wxString::Format(wxT("Opening 2. menu [%i] at %i"),i-boInsertPoint-1,boInsertPoint)));
								boGoalListOpened=i-boInsertPoint-1;
							}
						} else if(boGoalListOpened>1)
						{
							int l=0;
							for(j=0;(j<UNIT_TYPE_COUNT)&&(l<=i-boInsertPoint);j++)
								if(stats[anarace->getPlayer()->getRace()][j].type==boGoalListOpened)
									l++;
							anarace->insertOrder(j,i-boInsertPoint);
							addMessage(_T(wxString::Format(wxT("Item %i was chosen"),i-boInsertPoint-1)));
							update=2;
							//actually add an entry to the build order
						}
					}
					run=0;
				}
			}
		}

		if((optimizeMode)&&(boWindow->isPressed(0))) // Modus: alle
		{
			boWindow->transformClientWidth(24);
			optimizeMode=0;
			addMessage(_T("Mutations will now affect the whole build order."));
		} else if((!optimizeMode)&&(boWindow->isPressed(1)))
		{
			boWindow->transformClientWidth(-24);
			optimizeMode=1;
			addMessage(_T("Mutations will now affect only the selected parts."));
			addMessage(_T("Press continue as soon as you are done selecting."));
		}

		boWindow->clearButtons();

		for(i=0;i<MAX_LENGTH;i++)
		{
			optButton[i]=9999;
			unitButton[i]=9999;
		}
	}

	if(timerWindow->isShown())
	{
		if(timerWindow->isPressed(continueButton))
		{
			run=1;
			boInsertPoint=-1;
			boEndPoint=0;
			boGoalListOpened=0;
		}
		timerWindow->clearButtons();
	}

	if(goalForceWindow->isShown())
	{
		for(i=0;i<UNIT_TYPE_COUNT;i++)
		{
			if(goalForceWindow->isPressed(buttonType[i*3]))
			{
				anarace->getPlayer()->goal->addGoal(i,1,0,0);
				addMessage(_T(wxString::Format(wxT("Increased number of goals of %s."),stats[anarace->getPlayer()->getRace()][i].name)));
				update=2;
			}
			if(goalForceWindow->isPressed(buttonType[i*3+1]))
			{
				anarace->getPlayer()->goal->addGoal(i,-1,0,0);
				addMessage(_T(wxString::Format(wxT("Decreased number of goals of %s."),stats[anarace->getPlayer()->getRace()][i].name)));
				update=2;
			}
			if(goalForceWindow->isPressed(buttonType[i*3+2]))
			{
				anarace->getPlayer()->goal->addGoal(i,-anarace->getPlayer()->goal->globalGoal[0][i],0,0);
				addMessage(_T(wxString::Format(wxT("Removed %s from the goal list."),stats[anarace->getPlayer()->getRace()][i].name)));
				update=2;
			}
		}

		if(!goalForceWindow->insideClientArea(mouseX,mouseY))
			goalListOpened=0;

		if(goalForceWindow->isPressed(addGoalButton))
		{
			if(!goalListOpened)
				goalListOpened=1;
			else if(goalListOpened==1)
				goalListOpened=0;
			else if(goalListOpened>1)
				goalListOpened=1;
		} else
		for(i=0;i<UNIT_TYPE_COUNT;i++)
			if(goalForceWindow->isPressed(addUnitButton[i]))
			{
				if((goalListOpened==1)&&(i==0)) //scv
				{
					anarace->getPlayer()->goal->addGoal(SCV,1,0,0);
					addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][SCV].name)));
					update=2;
				}
				else
				if((goalListOpened==1)&&(i==1)) //gasscv
				{
					anarace->getPlayer()->goal->addGoal(GAS_SCV,1,0,0);
					addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][GAS_SCV].name)));
					update=2;
				}
				else if(goalListOpened==1)
					goalListOpened=i;
				else if(goalListOpened)
				{
					int l=0;
					for(j=0;j<UNIT_TYPE_COUNT;j++)
					{
						if(stats[anarace->getPlayer()->getRace()][j].type==goalListOpened+1)
						{
							if(i==l)
							{
								anarace->getPlayer()->goal->addGoal(j,1,0,0);
								update=2;
								addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][j].name)));
//								anarace->getPlayer()->goal->adjustGoals(0); //~~
								j=UNIT_TYPE_COUNT;
							}
							l++;
						}	
					}
				}
			}
		}
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
				addMessage(_T(wxString::Format(wxT("Increased mutation factor to %i%%."),ga->getMutationFactor())));
			else if(old==200)
			{
				addMessage(_T("Mutation factor is already at maximum (200)."));
				addMessage(_T("Take a look in the ""help"" menu (settings / mutation factor)."));
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
		       				addMessage(_T("Deactivated mutations! Evolution will no longer occur."));
					else
						addMessage(_T("Deactivated mutations. From now on evolution will only occur through cross over."));
				}
				else
					addMessage(_T(wxString::Format(wxT("Decreased mutation factor to %i%%."),ga->getMutationFactor())));
			} else if(old==0)
				addMessage(_T("Mutation factor is already at minimum (0%%)."));
		}

		if(theCore->isPressed(2))
		{
			if(ga->getMutationFactor()>0)
			{
				ga->setMutationFactor(0);
				if(ga->getCrossOver()==0)
					addMessage(_T("Deactivated mutations! Evolution will no longer occur."));
				else
					addMessage(_T("Deactivated mutations. From now on evolution will only occur through cross over."));
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
				addMessage(_T(wxString::Format(wxT("Increased breed factor to %i%%."),ga->getMutationFactor())));
			else if(old==100)
			{
				addMessage(_T("Breed factor is already at maximum (100%%)."));
				addMessage(_T("Take a look in the ""help"" menu (settings / breed factor)."));
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
						addMessage(_T("Deactivated breeding! Evolution will no longer occur."));
					else
						addMessage(_T("Deactivated breeding. From now on evolution will only occur through cross over."));
				}
				else
					addMessage(_T(wxString::Format(wxT("Decreased breed factor to %i%%."),ga->getMutationFactor())));
			} else if(old==0)
				addMessage(_T("Breed factor is already at minimum (0%%)."));
		}
																			   
		if(theCore->isPressed(5))
		{
			if(ga->getCrossOver()==0)
			addMessage(_T("Deactivated breeding! Evolution will no longer occur."));
				else
			addMessage(_T("Deactivated breeding. From now on evolution will only occur through cross over."));
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
				addMessage(_T(wxString::Format(wxT("Increased cross over to %i%%."),ga->getCrossOver())));
			else if(old==100)
			{
				addMessage(_T("Cross over is already at maximum (100%%)."));
				addMessage(_T("Take a look in the ""help"" menu (settings / cross over)."));
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
					addMessage(_T("Deactivated cross over! Evolution will no longer occur."));
						else
					addMessage(_T("Deactivated crossover. From now on evolution will only occur through breeding and mutations."));
				}
				else addMessage(_T(wxString::Format(wxT("Decreased cross over to %i%%."),ga->getCrossOver())));
			}
			else if(old==0)
				addMessage(_T("Cross over is already at minimum (0%%)."));
		}
																			    
		if(theCore->isPressed(8))
		{
			if((ga->getBreedFactor()==0)||(ga->getMutationFactor()==0))
			addMessage(_T("Deactivated cross over! Evolution will no longer occur."));
				else
			addMessage(_T("Deactivated crossover. From now on evolution will only occur through breeding and mutations."));
			ga->setCrossOver(0);
		}
		theCore->clearButtons();
	}
};

void MyDCWindow::drawStatistics()
{
	int i,j,k;
	for(i=0;i<20;i++)
		for(j=0;j<i;j++)
		{
			if(oldForceCounter[j]<oldForceCounter[i])
			{
				int tee=oldForceCounter[i];
				oldForceCounter[i]=oldForceCounter[j];
				oldForceCounter[j]=tee;
				tee=oldForce[i];
				oldForce[i]=oldForce[j];
				oldForce[j]=tee;
			}
			if(oldGasCounter[j]<oldGasCounter[i])
			{
				int tee=oldGasCounter[i];
				oldGasCounter[i]=oldGasCounter[j];
				oldGasCounter[j]=tee;
				tee=oldGas[i];
				oldGas[i]=oldGas[j];
				oldGas[j]=tee;
			}
																			    
			if(oldMinsCounter[j]<oldMinsCounter[i])
			{
				int tee=oldMinsCounter[i];
				oldMinsCounter[i]=oldMinsCounter[j];
				oldMinsCounter[j]=tee;
				tee=oldMins[i];
				oldMins[i]=oldMins[j];
				oldMins[j]=tee;
			}
		}

	wxPoint Legend2=wxPoint(210+statisticsWindow->getInnerLeftBound(),statisticsWindow->getInnerUpperBound());
	wxPoint Legend3=wxPoint(180,FONT_SIZE+4);

	dc->SetBrush(wxBrush(convertColour(40,40,40),wxSOLID));
	dc->SetPen(wxPen(convertColour(80,80,80),1,wxSOLID));
															    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y,Legend3.x,Legend3.y,4);
	for(k=0;k<20;k++)
		if(oldForceCounter[k]>0)
		{
			dc->SetTextForeground(wxColour(40,200-oldForceCounter[k]*8,200-oldForceCounter[k]*8));
			wxString bla=_T(wxString::Format(wxT("Force : %i"),force[199]/100));
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);

			if(oldForce[k]>0)
				dc->DrawText(_T(wxString::Format(wxT("+%i"),oldForce[k])),Legend2.x+dx+oldForceCounter[k],Legend2.y);
			else dc->DrawText(_T(wxString::Format(wxT("%i"),oldForce[k])),Legend2.x+dx+oldForceCounter[k],Legend2.y);
																						    oldForceCounter[k]+=2;
			if(oldForceCounter[k]>20)
			{
				oldForceCounter[k]=0;
				oldForce[k]=0;
			}
		}


	dc->SetTextForeground(convertColour(40,255,255));
	dc->DrawText(_T(wxString::Format(wxT("Force : %i"),force[199]/100)),Legend2.x,Legend2.y);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(255,255,40));
	dc->DrawText(_T(wxString::Format(wxT("Time : %.2i:%.2i"),time[199]/6000,(time[199]/100)%60)),Legend2.x,Legend2.y+12);
  
//	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
//	dc->SetTextForeground(convertColour(255,40,255));
//	dc->DrawText(_T(wxString::Format(wxT("Tertiary fitness : %i"),tFitness[199]/100)),Legend2.x,Legend2.y+12);
 
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+24,Legend3.x,Legend3.y,4);
	for(k=0;k<20;k++)
		if(oldMinsCounter[k]>0)
		{
			dc->SetTextForeground(wxColour(40,100-oldMinsCounter[k]*3,200-oldMinsCounter[k]*8));
			wxString bla=_T(wxString::Format(wxT("Minerals: %i"),harvestedMins[199]/10000));
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
																			    
			if(oldMins[k]>0)
				dc->DrawText(_T(wxString::Format(wxT("+%i"),oldMins[k]/100)),Legend2.x+dx+oldMinsCounter[k],Legend2.y+24);
			else dc->DrawText(_T(wxString::Format(wxT("%i"),oldMins[k]/100)),Legend2.x+dx+oldMinsCounter[k],Legend2.y+24);
																			    
			oldMinsCounter[k]+=2;
			if(oldMinsCounter[k]>20)
			{
				oldMinsCounter[k]=0;
				oldMins[k]=0;
			}
		}

	dc->SetTextForeground(convertColour(40,100,255));
	dc->DrawText(_T(wxString::Format(wxT("Minerals: %i"),harvestedMins[199]/10000)),Legend2.x,Legend2.y+24);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+36,Legend3.x,Legend3.y,4);
	for(k=0;k<20;k++)
		if(oldGasCounter[k]>0)
		{
			dc->SetTextForeground(wxColour(40,200-oldGasCounter[k]*8,40));
			wxString bla=_T(wxString::Format(wxT("Gas: %i"),harvestedGas[199]/10000));
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
																			    
			if(oldGas[k]>0)
				dc->DrawText(_T(wxString::Format(wxT("+%i"),oldGas[k]/100)),Legend2.x+dx+oldGasCounter[k],Legend2.y+36);
			else dc->DrawText(_T(wxString::Format(wxT("%i"),oldGas[k]/100)),Legend2.x+dx+oldGasCounter[k],Legend2.y+36);
																			    
			oldGasCounter[k]+=2;
			if(oldGasCounter[k]>20)
			{
				oldGasCounter[k]=0;
				oldGas[k]=0;
			}
		}

	dc->SetTextForeground(convertColour(40,255,40));
	dc->DrawText(_T(wxString::Format(wxT("Gas: %i"),harvestedGas[199]/10000)),Legend2.x,Legend2.y+36);
 	 
//	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
//	dc->SetTextForeground(convertColour(255,40,40));
//	dc->DrawText(_T(wxString::Format(wxT("Primary fitness : %i"),pFitness[199]/100)),Legend2.x,Legend2.y+48);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(200,200,200));
	dc->DrawText(_T(wxString::Format(wxT("Fitness average : %i"),aFitness[199]/100)),Legend2.x,Legend2.y+48);
	
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+60,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(100,100,100));
	dc->DrawText(_T(wxString::Format(wxT("Fitness variance : %i"),vFitness[199]/100)),Legend2.x,Legend2.y+60);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+72,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(100,150,200));
	dc->DrawText(_T(wxString::Format(wxT("BuildOrder Length: %i"),length[199]/100)),Legend2.x,Legend2.y+72);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+84,Legend3.x,Legend3.y,4);
	dc->SetBrush(wxBrush(convertColour(40,40,100),wxSOLID));
	dc->DrawRoundedRectangle(Legend2.x+Legend3.x-(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations,Legend2.y+84,(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations ,Legend3.y,4);
	dc->SetTextForeground(convertColour(100,100,255));
	dc->DrawText(_T(wxString::Format(wxT("%i+ generations left"),(ga->maxGenerations-anarace->getUnchangedGenerations()))),Legend2.x,Legend2.y+84);

	dt1=wxDateTime::UNow();
	wxTimeSpan ts=dt1.Subtract(dt2);
	dt2=dt1;

	if(averagecounter<100)
		averagecounter++;
	for(i=averagecounter-1;i--;)
		average[i+1]=average[i];		
	average[0]=ts.GetMilliseconds().ToLong();

	int av=0;
	for(i=0;i<averagecounter;i++)
		av+=average[i];
	av/=averagecounter;
//	int tlength=force[199]/100-5;

//	haxor->Draw(dc);
/*
	dc->SetTextForeground(convertColour(100,100,255));

	dc->DrawText(_T(wxString::Format(wxT("%i of %.0f billion possible build orders checked [%.6f%%]."),anarace->getGeneration()*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1),pow(tlength,anarace->getPlayer()->goal->getMaxBuildTypes())/1000000000.0,(anarace->getGeneration()*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1))/pow(tlength,anarace->getPlayer()->goal->getMaxBuildTypes()))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound());

	if(!run)
	dc->DrawText(_T(wxString::Format(wxT("%i ms per frame (%.2f fps)."),av,1000.0/((float)(av)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+20);
	else
	{
		dc->DrawText(_T(wxString::Format(wxT("%i ms per frame (%.2f fps) [%.2f bops]."),av,1000.0/((float)(av)),1000.0*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1)/((float)(av)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+20);

		dc->DrawText(_T(wxString::Format(wxT("Estimated time using stupid brute-force algorithm: %.5f billion years."),
		pow(tlength,anarace->getPlayer()->goal->getMaxBuildTypes())/(1000000000.0*365.0*24.0*3600000.0*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1)/((float)(av))))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+40);
		if(anarace->getTimer()!=ga->maxTime)
		{
			float totaln=1;
			for(i=0;i<=GAS_SCV;i++)
				if(anarace->getLocationForce(0,i))
		//todo startforce beruecksichtigen... aber erstmal map location[0] fuellen... TODO
				{
					if(i==SCV)
					for(j=1;j<anarace->getLocationForce(0,i)-4;j++)
						totaln*=j;
					else if(i==COMMAND_CENTER)
					for(j=1;j<anarace->getLocationForce(0,i)-1;j++)
						totaln*=j;		
					else
					for(j=1;j<anarace->getLocationForce(0,i);j++)
						totaln*=j;
				}
			float totalz=1;
			for(i=1;i<force[199]/100-5;i++)
				totalz*=i;
			dc->DrawText(_T(wxString::Format(wxT("Estimated time using intelligent brute-force algorithm: %.2f hours."), (totalz*((float)(av)))/(totaln*3600000.0*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+60);
			
		}
	}*/
		dc->SetPen(wxPen(convertColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
		dc->SetBrush(wxBrush(convertColour(INFOWINDOWPENR/2,INFOWINDOWPENG/2,INFOWINDOWPENB/2),wxCROSS_HATCH));

		dc->DrawRectangle(statisticsWindow->getInnerLeftBound(),statisticsWindow->getInnerUpperBound(),200,100);
	//	showGraph(tFitness,maxtFitness-mintFitness,convertColour(255,40,255));
		showGraph(harvestedMins,maxsFitness,convertColour(40,100,255));
		showGraph(harvestedGas,maxsFitness,convertColour(40,255,40));
		showGraph(force,maxForce,convertColour(40,255,255));
		showGraph(length,MAX_LENGTH,convertColour(100,150,200));
		showGraph(time,ga->maxTime,convertColour(255,255,40));
		showGraph(aFitness,maxpFitness,convertColour(200,200,200));
		showGraph(vFitness,maxpFitness,convertColour(100,100,100));
	//	showGraph(pFitness,maxpFitness,convertColour(255,40,40));
};


void MyDCWindow::drawGoalList()
{
	int t,k,i;
	t=1;
	dc->SetTextForeground(convertColour(255,0,0));
	dc->SetPen(wxPen(convertColour(0,0,0),1,wxSOLID));
//	dc->SetFont(font2);
//fuer jeden Balken ne Variable

	dc->SetFont(GraphixScrollWindow::font);
	dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));

	int percent[UNIT_TYPE_COUNT];
	int counter[UNIT_TYPE_COUNT]; //number of different goals
	for(k=0;k<UNIT_TYPE_COUNT;k++) {percent[k]=0;counter[k]=0;}
	for(k=0;k<MAX_GOALS;k++)
		{
			percent[anarace->getPlayer()->goal->goal[k].unit]+=anarace->fitnessCode[k];
			counter[anarace->getPlayer()->goal->goal[k].unit]++;
		}

// goallist

int worstTime[UNIT_TYPE_COUNT];
for(i=0;i<MAX_GOALS;i++)
	worstTime[i]=0;
for(i=0;i<MAX_GOALS;i++)
	if(anarace->getPlayer()->goal->goal[i].count>0)
		if(worstTime[anarace->getPlayer()->goal->goal[i].unit]<anarace->getFinalTime(i))
			worstTime[anarace->getPlayer()->goal->goal[i].unit]=anarace->getFinalTime(i);
	t=1;
	goalForceWindow->clearButtons();
	int lastgoal=-1;
	wxRect edge;
			edge.x=0;
			edge.width=250+20;
			edge.height=FONT_SIZE+5;
			edge.y=t*(FONT_SIZE+5)-goalForceWindow->getScrollY();
			lastgoal=edge.y;
			if(goalForceWindow->fitToClientArea(edge))
			{
				edge.x+=goalForceWindow->getInnerLeftBound();
				edge.y+=goalForceWindow->getInnerUpperBound();
				//if(t%2==0)
					dc->SetBrush(wxBrush(wxColour(50,50,50),wxSOLID));
				//else
				//dc->SetBrush(wxBrush(convertColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
																			    
				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
																			    
				dc->SetTextForeground(convertColour(0,200,0));
				if(!goalListOpened)
				{
					dc->DrawBitmap(bmpArrowDown,edge.x+4,edge.y+2);
					dc->DrawText(_T("Click here to add a goal"),edge.x+14,edge.y);
				}
				else if(goalListOpened==1)
				{
					dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
					dc->DrawText(_T("(Click here to close)"),edge.x+14,edge.y);
				}
				else
				{
					dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
					dc->DrawText(_T("(Click here to go back)"),edge.x+14,edge.y);
				}
				addGoalButton=goalForceWindow->addButton(edge);
		      }
	t++;

	if(goalListOpened==1)
	{
	int l=0;
		for(i=1;i<11;i++)
		{
			edge.x=10;
			edge.width=270;
			edge.height=FONT_SIZE+5;
			edge.y=t*(FONT_SIZE+5)-goalForceWindow->getScrollY();
			lastgoal=edge.y;
			if(goalForceWindow->fitToClientArea(edge))
			{
				edge.x+=goalForceWindow->getInnerLeftBound();
				edge.y+=goalForceWindow->getInnerUpperBound();
																			    
				dc->SetBrush(wxBrush(convertColour(
(COLOR1R+2*BOcolor[i].Red()),
(COLOR1G+2*BOcolor[i].Green()),
(COLOR1B+2*BOcolor[i].Blue())),wxSOLID));
																			    
				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
				dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
				dc->DrawText(BOnames[i],edge.x+10,edge.y);
				addUnitButton[l]=goalForceWindow->addButton(edge);
				l++;
				t++;
			}
																			    
		}
	}
	else if(goalListOpened>1)
	{
		int l=0;
		int type=goalListOpened+1;
		for(i=0;i<UNIT_TYPE_COUNT;i++)
		{
			if(stats[anarace->getPlayer()->getRace()][i].type==type)
			{
				edge.x=10;
				edge.width=270;
				edge.height=FONT_SIZE+5;
				edge.y=t*(FONT_SIZE+5)-goalForceWindow->getScrollY();
				lastgoal=edge.y;
				if(goalForceWindow->fitToClientArea(edge))
				{
					edge.x+=goalForceWindow->getInnerLeftBound();
					edge.y+=goalForceWindow->getInnerUpperBound();
					dc->SetBrush(wxBrush(convertColour(
(COLOR1R+2*BOcolor[type].Red()),
(COLOR1G+2*BOcolor[type].Green()),
(COLOR1B+2*BOcolor[type].Blue())),wxSOLID));
																			    
					dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
					dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
					dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.x+10,edge.y);
					addUnitButton[l]=goalForceWindow->addButton(edge);
					l++;
					t++;
				}
			}
		}
	}
/*	edge.x=20;
	edge.y=t*(FONT_SIZE+5)+9-goalForceWindow->getScrollY();
	edge.width=goalForceWindow->getInnerWidth()-40;
	edge.height=2;
	if(goalForceWindow->fitToClientArea(edge))
	{
		dc->SetPen(wxPen(convertColour(200,0,0),2,wxSOLID));
		dc->DrawLine(edge.x+goalForceWindow->getInnerLeftBound(),edge.y+goalForceWindow->getInnerUpperBound(),edge.x+edge.width+goalForceWindow->getInnerLeftBound(),edge.y+goalForceWindow->getInnerUpperBound());
		dc->SetPen(wxPen(convertColour(0,0,0),1,wxSOLID));
	}*/


t++;
// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for(i=0;i<=GAS_SCV;i++)
		if(anarace->getPlayer()->goal->allGoal[i]>0)
		{
			if(globalForcelog[i].order==anarace->getLocationForce(0,i))
			{
				if(globalForcelog[i].count<160)
					globalForcelog[i].count+=3;
			}
			else
			{
				globalForcelog[i].count=0;
				globalForcelog[i].order=anarace->getLocationForce(0,i);
			}
			int h=t*(FONT_SIZE+5);
			edge.x=0;
			edge.width=250+20;
			edge.height=FONT_SIZE+5;
			edge.y=h-goalForceWindow->getScrollY();
			lastgoal=edge.y;

			 if(oldForceList[t]<anarace->getLocationForce(0,i)*100/maxUnitForce)
					 oldForceList[t]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[t])/5+1;
			 else if(oldForceList[t]>anarace->getLocationForce(0,i)*100/maxUnitForce)
					 oldForceList[t]-=(oldForceList[t]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;
			if(goalForceWindow->fitToClientArea(edge))
			{
				edge.x+=goalForceWindow->getInnerLeftBound();
				edge.y+=goalForceWindow->getInnerUpperBound();
				if(t%2==0)
					dc->SetBrush(wxBrush(convertColour(COLOR1R,COLOR1G,COLOR1B),wxSOLID));
				else dc->SetBrush(wxBrush(convertColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));

				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
	
				dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
				dc->DrawText(_T(wxString::Format(wxT("%s [%.2i:%.2i]"),stats[anarace->getPlayer()->getRace()][i].name,worstTime[i]/60,worstTime[i]%60)),edge.x+10,edge.y);
			   	

				if(oldForceList[t]>0)
				{
dc->SetBrush(wxBrush(convertColour(((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));

					dc->DrawRoundedRectangle(edge.x+edge.width-40-1-oldForceList[t],edge.y,oldForceList[t]+1,FONT_SIZE+4,4);
				}
				dc->SetBrush(wxBrush(wxColour(200,200,200),wxTRANSPARENT)); //TODO gruen rot  evtl

				dc->DrawRoundedRectangle(edge.x+edge.width-40-1-anarace->getPlayer()->goal->allGoal[i]*100/maxUnitForce,edge.y,anarace->getPlayer()->goal->allGoal[i]*100/maxUnitForce+1,FONT_SIZE+4,4);
				
				if((percent[i]/counter[i]<100)&&(percent[i]/counter[i]>0))
				{
				      dc->DrawText(_T(wxString::Format(wxT("%2i [%i]"),anarace->getLocationForce(0,i),anarace->getPlayer()->goal->allGoal[i])),edge.x+edge.width-40-40-oldForceList[t],edge.y);
					dc->DrawText(_T(wxString::Format(wxT("%i%%"),percent[i]/counter[i])),edge.x+edge.width-40-30,edge.y);
				}
				else
					dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i))),edge.x+edge.width-40-40-oldForceList[t],edge.y);


				buttonType[i*3]=goalForceWindow->addBitmapButton(wxRect(edge.x+edge.width-36,edge.y+2,8,8),bmpAdd);
				buttonType[i*3+1]=goalForceWindow->addBitmapButton(wxRect(edge.x+edge.width-26,edge.y+2,8,8),bmpSub);
				buttonType[i*3+2]=goalForceWindow->addBitmapButton(wxRect(edge.x+edge.width-16,edge.y+2,8,8),bmpCancel);

			}
		t++;
	}

			goalForceWindow->DrawButtons(dc);

	int lastgoal2=-1;
//	goalForceWindow->setMaxScrollY((t+1)*(FONT_SIZE+5));
	
	dc->SetFont(GraphixScrollWindow::font);
	dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
	t=1;
	for(i=0;i<=GAS_SCV;i++)
		if((anarace->getPlayer()->goal->allGoal[i]==0)&&(anarace->getLocationForce(0,i)))
		{
			if(globalForcelog[i].order==anarace->getLocationForce(0,i))
			{
			       if(globalForcelog[i].count<160)
				       globalForcelog[i].count+=5;
			}
			else
			{
				globalForcelog[i].count=0;
				globalForcelog[i].order=anarace->getLocationForce(0,i);
			}
			int h=t*(FONT_SIZE+5)+3;

			edge.x=0;
			edge.width=250+20;
			edge.height=FONT_SIZE+5;
			edge.y=h-forceWindow->getScrollY();
			lastgoal2=edge.y;
			       if(oldForceList[t]<anarace->getLocationForce(0,i)*100/maxUnitForce)
					oldForceList[t]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[t])/5+1;
				else if(oldForceList[t]>anarace->getLocationForce(0,i)*100/maxUnitForce)
					oldForceList[t]-=(oldForceList[t]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;
			if(forceWindow->fitToClientArea(edge))
			{
				edge.x+=forceWindow->getInnerLeftBound();
				edge.y+=forceWindow->getInnerUpperBound();
				if(t%2==0)
					dc->SetBrush(wxBrush(convertColour(COLOR1R,COLOR1G,COLOR1B),wxSOLID));
				else dc->SetBrush(wxBrush(convertColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);

				dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.x+10,edge.y);
				dc->SetBrush(wxBrush(convertColour(   ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
						((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
						((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));
				dc->DrawRoundedRectangle(edge.x+edge.width-1-oldForceList[t],edge.y,oldForceList[t]+1,FONT_SIZE+4,4);
				dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i))),edge.x+edge.width-30-oldForceList[t],edge.y);
			}
			t++;
		}

                if((lastgoal>=0)&&(lastgoal2>=0))
                {
                        goalForceWindow->adjustWindow(wxRect(goalForceWindow->getLeftBound(),goalForceWindow->getUpperBound(),goalForceWindow->getWidth(),lastgoal+50));
                        forceWindow->adjustWindow(wxRect(forceWindow->getLeftBound(),lastgoal+goalForceWindow->getUpperBound()+50,forceWindow->getWidth(),lastgoal2+50));
                } else if(lastgoal>=0)
		{
                        goalForceWindow->adjustWindow(wxRect(goalForceWindow->getLeftBound(),goalForceWindow->getUpperBound(),goalForceWindow->getWidth(),lastgoal+45));
                        forceWindow->adjustWindow(wxRect(forceWindow->getLeftBound(),lastgoal+goalForceWindow->getUpperBound()+50,forceWindow->getWidth(),forceWindow->getTargetHeight()));
		}


//	forceWindow->setMaxScrollY(t*(FONT_SIZE+5));


};

void MyDCWindow::drawGizmo()
{
		dc->SetFont(GraphixScrollWindow::font3);
		if(run) ani++; else ani=1;
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
		dc->DrawText(_T("v1.10"),mainWindow->getInnerLeftBound()+154,mainWindow->getInnerUpperBound()+84);
		dc->SetTextForeground(wxColour(50,50,85));
		dc->SetFont(GraphixScrollWindow::font3);
		dc->DrawText(_T("v1.10"),mainWindow->getInnerLeftBound()+150,mainWindow->getInnerUpperBound()+80);


};


void MyDCWindow::drawMessages()
{
	int i,t;
	t=0;
	dc->SetFont(GraphixScrollWindow::font2);

	for(i=msgCount;i--;)
		if(message[i].type>0)
		{
			dc->SetTextForeground(convertColour(100+message[i].col,100+message[i].col,255));
			if(message[i].col>5) message[i].col-=message[i].col/5+1;
			else message[i].col=0;
			wxRect edge;
			edge.x=5;
			edge.width=msgWindow->getInnerWidth();
			edge.height=FONT_SIZE+5;
			edge.y=t*(FONT_SIZE+5)-msgWindow->getScrollY()+5;
			if(msgWindow->fitToClientArea(edge))
			{
				edge.x+=msgWindow->getInnerLeftBound();
				edge.y+=msgWindow->getInnerUpperBound();
//				dc->DrawBitmap(hintBitmap,edge.x,edge.y);
				dc->SetPen(wxPen(wxColour(20,20,20),1,wxSOLID));
				dc->SetBrush(wxBrush(wxColour(30,30,30),wxSOLID));
				dc->DrawRoundedRectangle(edge,3);
				dc->DrawText(message[i].string,edge.x,edge.y);
			}
			t++;
		}
//	msgWindow->setMaxScrollY(t*(FONT_SIZE+5));
};

void MyDCWindow::drawSelectionStuff()
{
	int i;	
//	boanzahl=orderList.GetCount();

if(optimizeMode)
{
	int start=0;
	int inrow=0;
	int l=0;
	for(i=0;i<orderLength;i++)
	{
		if((!inrow)&&(selection[i])&&(i!=boInsertPoint))
		{
			inrow=1;
			start=l;
		} else if((inrow)&&((!selection[i])||(i==boInsertPoint)))
		{
			inrow=0;
			wxPoint pointsKlammer[9];
			int h1=start*(FONT_SIZE+5)-boWindow->getScrollY()+11+boWindow->getInnerUpperBound();
			int h2=(l+1)*(FONT_SIZE+5)-boWindow->getScrollY()-2+boWindow->getInnerUpperBound();
			
			if((h1<boWindow->getInnerUpperBound())&&(h2>boWindow->getInnerUpperBound()))
				h1=boWindow->getInnerUpperBound(); //~~
			if((h1<boWindow->getInnerUpperBound()+boWindow->getInnerHeight())&&(h2>boWindow->getInnerUpperBound()+boWindow->getInnerHeight()))
				h2=boWindow->getInnerUpperBound()+boWindow->getInnerHeight();

			if((h2<boWindow->getInnerUpperBound())||(h1>boWindow->getInnerUpperBound()+boWindow->getInnerHeight()))
				i=orderLength;
			else
			{

			int x1=boWindow->getInnerLeftBound()+boWindow->getInnerWidth();

				pointsKlammer[0].x=x1+2;
				pointsKlammer[8].x=x1+2;

				pointsKlammer[1].x=x1+4;
				pointsKlammer[7].x=x1+4;

				pointsKlammer[2].x=x1+5;
				pointsKlammer[6].x=x1+5;

				pointsKlammer[3].x=x1+6;
				pointsKlammer[5].x=x1+6;

				pointsKlammer[4].x=x1+8;


				pointsKlammer[0].y=h1;
				pointsKlammer[8].y=h2;
																			    
				pointsKlammer[1].y=h1+2;
				pointsKlammer[7].y=h2-2;
																			    
				pointsKlammer[2].y=h1+6;
				pointsKlammer[6].y=h2-6;
																			    
				pointsKlammer[3].y=(h1+h2)/2-3;
				pointsKlammer[5].y=(h1+h2)/2+3;
																			    
				pointsKlammer[4].y=(h1+h2)/2;
//TODO gruen/rot
				dc->SetPen(wxPen(wxColour(50,200,50),1,wxSOLID));
				dc->DrawSpline(9,pointsKlammer);
			}
			}
			l++;
			if(i==boInsertPoint)
				l=boEndPoint;
		}
}


};

void MyDCWindow::drawGeneString()
{
	int i,k,j;
	int stringheight=0;
	int currentType=0;
	k=0;

	wxPoint points1[200];
	wxPoint points2[200];
	wxPoint points3[200];
	wxPoint points4[200];

	for(i=0;i<200;i++)
	{
		points1[i].x=0;points1[i].y=0;
		points2[i].x=0;points2[i].y=0;
		points3[i].x=0;points3[i].y=0;
		points4[i].x=0;points4[i].y=0;
	}

	int boanzahl=orderList.GetCount();
	if(run)
	{
		NODE* node=orderList.GetFirst();
		while(node)
		{
			ORDER* oldOrder=node->GetData();
			if(oldOrder->blend)
			{
				k=1;
				while((node)&&((!(oldOrder=node->GetData())->blend)||((stats[anarace->getPlayer()->getRace()][oldOrder->unit].type)&&(stats[anarace->getPlayer()->getRace()][oldOrder->unit].type==currentType))))
				{
					node=node->GetNext();
					if(!oldOrder->blend) continue;
					k++;	
				}
				currentType=stats[anarace->getPlayer()->getRace()][oldOrder->unit].type;

				for(j=0;j<k+1;j++)
				{
					points1[j].x=10+(stringheight+j)*(theCore->getInnerWidth()-20)/(boanzahl)+theCore->getInnerLeftBound()-1;
					points2[j].x=10+(stringheight+j)*(theCore->getInnerWidth()-20)/(boanzahl)+theCore->getInnerLeftBound()-1;
					points1[j].y=(int)(cos((float)((stringheight+j)+anarace->getGeneration())*10.0*3.1416/200.0)*0.9*theCore->getInnerHeight()/2)+theCore->getInnerUpperBound()+theCore->getInnerHeight()/2.1;
					points2[j].y=(int)(sin(((float)((stringheight+j)+anarace->getGeneration())+13.0)*10.0*3.1416/200.0)*0.9*theCore->getInnerHeight()/2)+theCore->getInnerUpperBound()+theCore->getInnerHeight()/2.1;

					points3[j].x=10+(stringheight+j)*(theCore->getInnerWidth()-20)/(boanzahl)+theCore->getInnerLeftBound()-1;
					points4[j].x=10+(stringheight+j)*(theCore->getInnerWidth()-20)/(boanzahl)+theCore->getInnerLeftBound()-1;
					points3[j].y=(int)(cos(((float)((stringheight+j)+anarace->getGeneration())+26.0)*10.0*3.1416/200.0)*0.9*theCore->getInnerHeight()/2)+theCore->getInnerUpperBound()+theCore->getInnerHeight()/2.1;
					points4[j].y=(int)(sin(((float)((stringheight+j)+anarace->getGeneration())+39.0)*10.0*3.1416/200.0)*0.9*theCore->getInnerHeight()/2)+theCore->getInnerUpperBound()+theCore->getInnerHeight()/2.1;
				} //end for(j=0;j<k;j++)
				stringheight+=k;
//				points1[stringheight].x=boDiagramWindow->getRightBound()+60+(int)(cos((float)((stringheight+j)+anarace->getGeneration())*5.0*3.1416/200.0)*50.0);
				k++;
				if(k>=1)
				{
				wxPen bla1=wxPen(convertColour(
3*(COLOR1R+2*BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Red())/10,
3*(COLOR1G+2*BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Green())/10,
3*(COLOR1B+2*BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Blue())/10),3,wxSOLID);
				wxPen bla2=wxPen(convertColour(
				3*(COLOR1R/2+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Red())/10,
				3*(COLOR1G/2+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Green())/10,
				3*(COLOR1B/2+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Blue())/10),1,wxSOLID);

				dc->SetPen(bla1);
				dc->DrawSpline(k,points1);
				for(j=0;j<k;j++) points1[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points1);
				for(j=0;j<k;j++) points1[j].y+=4;
				dc->DrawSpline(k,points1);

				dc->SetPen(bla1);
				dc->DrawSpline(k,points2);
				for(j=0;j<k;j++) points2[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points2);
				for(j=0;j<k;j++) points2[j].y+=4;
				dc->DrawSpline(k,points2);

				dc->SetPen(bla1);
				dc->DrawSpline(k,points3);
				for(j=0;j<k;j++) points3[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points3);
				for(j=0;j<k;j++) points3[j].y+=4;
				dc->DrawSpline(k,points3);

				dc->SetPen(bla1);
				dc->DrawSpline(k,points4);
				for(j=0;j<k;j++) points4[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points4);
				for(j=0;j<k;j++) points4[j].y+=4;
				dc->DrawSpline(k,points4);
				}
			} //end blend
			if(node)
				node=node->GetNext();
			
		} //end while(node)
	} //end if(run)
};

void MyDCWindow::drawBuildOrder()
{
	int t=0;
	int i;
	int lastOrder=-1;
	wxRect edge;
	boEndPoint=0;
	dc->SetPen(wxPen(convertColour(0,0,0),1,wxSOLID));
	if(boInsertPoint>-1)
	{
		t=boInsertPoint;
			edge.x=0;
			edge.width=250+20;
			edge.height=FONT_SIZE+4;
			edge.y=t*(FONT_SIZE+5)-boWindow->getScrollY();
			if(edge.y>lastOrder)
				lastOrder=edge.y;
			if(boWindow->fitToClientArea(edge))
			{
				edge.x+=boWindow->getInnerLeftBound();
				edge.y+=boWindow->getInnerUpperBound();
				//if(t%2==0)
					dc->SetBrush(wxBrush(wxColour(50,50,50),wxSOLID));
				//else
				//dc->SetBrush(wxBrush(convertColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
																			    
				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
																			    
				dc->SetTextForeground(convertColour(0,200,0));
				if(boGoalListOpened==1)
				{
					dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
					dc->DrawText(_T("(Click here to close)"),edge.x+14,edge.y);
				}
				else
				{
					dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
					dc->DrawText(_T("(Click here to go back)"),edge.x+14,edge.y);
				}
				unitButton[t]=boWindow->addButton(edge);
		      }
					t++;
		if(boGoalListOpened==1)
		{
			for(i=1;i<11;i++)
			{
				edge.x=10;
				edge.width=270;
				edge.height=FONT_SIZE+4;
				edge.y=t*(FONT_SIZE+5)-boWindow->getScrollY();
				lastBogoal=edge.y;
				if(boWindow->fitToClientArea(edge))
				{
					edge.x+=boWindow->getInnerLeftBound();
					edge.y+=boWindow->getInnerUpperBound();
																			    
					dc->SetBrush(wxBrush(convertColour(
(COLOR1R+2*BOcolor[i].Red()),
(COLOR1G+2*BOcolor[i].Green()),
(COLOR1B+2*BOcolor[i].Blue())),wxSOLID));
																								    dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
					dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
					dc->DrawText(BOnames[i],edge.x+10,edge.y);
					unitButton[t]=boWindow->addButton(edge);
				}
				t++;
			}
		}
		else if(boGoalListOpened>1)
		{
			int type=boGoalListOpened+1;
			for(i=0;i<UNIT_TYPE_COUNT;i++)
			{
				if(stats[anarace->getPlayer()->getRace()][i].type==type)
				{
					edge.x=20;				 
					edge.width=270;
					edge.height=FONT_SIZE+4;
					edge.y=t*(FONT_SIZE+5)-boWindow->getScrollY();
					lastBogoal=edge.y;
					if(boWindow->fitToClientArea(edge))
					{
						edge.x+=boWindow->getInnerLeftBound();
						edge.y+=boWindow->getInnerUpperBound();
						dc->SetBrush(wxBrush(convertColour(
(COLOR1R+2*BOcolor[type].Red()),
(COLOR1G+2*BOcolor[type].Green()),
(COLOR1B+2*BOcolor[type].Blue())),wxSOLID));
		 				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
						dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
						dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.x+10,edge.y);
						unitButton[t]=boWindow->addButton(edge);
					}
					t++;
				}
			}
		}
/*		edge.x=20;
		edge.y=t*(FONT_SIZE+5)+9-boWindow->getScrollY();
		edge.width=boWindow->getInnerWidth()-40;
		edge.height=2;
		if(boWindow->fitToClientArea(edge))
		{
			dc->SetPen(wxPen(convertColour(200,0,0),2,wxSOLID));
			dc->DrawLine(edge.x+boWindow->getInnerLeftBound(),edge.y+boWindow->getInnerUpperBound(),edge.x+edge.width+boWindow->getInnerLeftBound(),edge.y+boWindow->getInnerUpperBound());
			dc->SetPen(wxPen(convertColour(0,0,0),1,wxSOLID));
		}*/
		boEndPoint=t+1;
		//end
	}
	

	orderLength=0;
	t=0;
        NODE* node=orderList.GetFirst();
	while(node)
	{
		ORDER* oldOrder=node->GetData();

		edge.x=oldOrder->x;
		edge.height=FONT_SIZE+4;
		edge.width=oldOrder->width;
		int row=((boInsertPoint>-1)&&(oldOrder->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
		edge.y=row*(FONT_SIZE+5)+oldOrder->y-boWindow->getScrollY();
		if(edge.y>lastOrder) 
			lastOrder=edge.y;
		if(boWindow->fitToClientArea(edge))
		{
			edge.x+=boWindow->getInnerLeftBound();
			edge.y+=boWindow->getInnerUpperBound();
			{
//					if(((oldOrder->targety-3)/(FONT_SIZE+5))%2)
				dc->SetBrush(wxBrush(convertColour(
(100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+COLOR1R+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].type].Red())*oldOrder->blend/50,
(100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+(optimizeMode*selection[orderLength])*255+(1-(optimizeMode*selection[orderLength]))*(COLOR1G+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].type].Green())*oldOrder->blend/50),
(100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+COLOR1B+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].type].Blue())*oldOrder->blend/50),wxSOLID));

//else dc->SetBrush(wxBrush(convertColour(
//(COLOR2R+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Red())*oldOrder->blend/50,
//(COLOR2G+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Green())*oldOrder->blend/50,
//(COLOR2B+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder->unit].type].Blue())*oldOrder->blend/50),wxSOLID));

					dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
					dc->SetTextForeground(BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].type]);

					if(edge.width>=110)
					{
						unitButton[row+t]=boWindow->addButton(wxRect(edge.x+edge.width-12,edge.y+1,8,8));
						dc->DrawText(_T(wxString::Format(wxT("%i."),anarace->getProgramForceCount(oldOrder->IP,anarace->phaenoCode[oldOrder->IP])+1)),edge.x+5,edge.y);
						dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[oldOrder->IP]].name)),edge.x+20,edge.y);
						if(row+t+1==boEndPoint)
							dc->DrawBitmap(bmpArrowDown,edge.x+edge.width-12,edge.y+1);
						else
							dc->DrawBitmap(bmpArrowUp,edge.x+edge.width-12,edge.y+1);
						if(optimizeMode)
							optButton[row+t]=boWindow->addButton(wxRect(boWindow->getInnerLeftBound()+edge.width,edge.y,boWindow->getWidth()-edge.width,FONT_SIZE+5));
					}
/*					if(edge.width>=140)
						dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder->mins)),110+edge.x,edge.y);
					if(edge.width>=180)
						dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder->gas)),150+edge.x,edge.y);
					if(edge.width>=230)
						dc->DrawText(_T(wxString::Format(wxT("%i/%i"),oldOrder->needSupply,oldOrder->haveSupply)),190+edge.x,edge.y);
					if(edge.width>=260)
						dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),oldOrder->time/60,oldOrder->time%60)),240+edge.x,edge.y);
	//      			dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder->successType)),190+oldOrder->x,oldOrder->y);*/
//					}
			}
		}
		t++;
		orderLength++;
		node=node->GetNext();
	} //order blend>0
	
		if(lastOrder>=0)
			boWindow->adjustWindow(wxRect(boWindow->getLeftBound(),boWindow->getUpperBound(),boWindow->getWidth(),lastOrder+boWindow->getRelativeInnerUpperBound()+40));

}

void MyDCWindow::showInfoWindow()
{
	if(!infoWindowNumber)
		return;
	NODE* node=orderList.Find(infoWindowNumber-1);
	if(!node)
		return;

	ORDER* order=node->GetData();

	int t=0;
	infoWindow->Draw(dc);

	dc->SetTextForeground(wxColour(INFOWINDOWTEXTR,INFOWINDOWTEXTG,INFOWINDOWTEXTB));
	dc->DrawText(_T(wxString::Format(wxT("Build %i. %s"),anarace->getProgramForceCount(order->IP,order->unit)+1,stats[anarace->getPlayer()->getRace()][order->unit].name)),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;		 
dc->DrawText(_T(wxString::Format(wxT("at %s"),anarace->getMap()->location[anarace->getProgramLocation(order->IP)].getName())),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
	dc->DrawText(_T(wxString::Format(wxT("having %i minerals"),anarace->getStatisticsHaveMinerals(anarace->getProgramTime(order->IP))/100)),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
	dc->DrawText(_T(wxString::Format(wxT("and %i gas"),anarace->getStatisticsHaveGas(anarace->getProgramTime(order->IP))/100)),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
	dc->DrawText(_T(wxString::Format(wxT("and %i of %i supply"),anarace->getStatisticsNeedSupply(anarace->getProgramTime(order->IP)),anarace->getStatisticsHaveSupply(anarace->getProgramTime(order->IP)))),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
	dc->DrawText(_T(wxString::Format(wxT("as soon as %s"),error_message[anarace->getProgramSuccessType(order->IP)])),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;	       
	if(anarace->getProgramSuccessUnit(order->IP)>0) {
		dc->DrawText(_T(wxString::Format(wxT("%s becomes availible."),stats[anarace->getPlayer()->getRace()][anarace->getProgramSuccessUnit(order->IP)].name)),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;}
	dc->DrawText(_T(wxString::Format(wxT("(time %.2i:%.2i)"),anarace->getProgramTime(order->IP)/60,anarace->getProgramTime(order->IP)%60)),infoWindow->getInnerLeftBound()+5,infoWindow->getInnerUpperBound()+5+t*(FONT_SIZE+5));
};

void MyDCWindow::showCoreAnimation()
{
//	int bla=(sin((animationNumbers%10)*3.141*0.2)+1.0)*2.5;
//	if(ga->getMutationFactor()==0)
//		bla=4;
//(animationNumbers%10>4)*4+(animationNumbers%10>4)*(-2)*(animationNumbers%5)+(animationNumbers%5);

	if(animationNumbers%11==0) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,470,60,0,20);

	dc->DrawBitmap(bmpRad[4],theCore->getInnerLeftBound()+400/*+(39-bmpRad[bla].GetWidth())/2*/,theCore->getInnerUpperBound()+50/*+(39-bmpRad[bla].GetHeight())/2*/);
	dc->DrawText(_T(wxString::Format(wxT("%i%%"),ga->getMutationFactor())),theCore->getInnerLeftBound()+405/*+(39-bmpRad[bla].GetWidth())/2*/,theCore->getInnerUpperBound()+55/*+(39-bmpRad[bla].GetHeight())/2*/);
	dc->DrawText(_T("Mutations"),theCore->getInnerLeftBound()+400,theCore->getInnerUpperBound()+30);

	if(animationNumbers%11==1) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,370,60,0,20);

// Now some parts of some build orders of the current population of build orders are randomly changed. This is also known as Mutation which will broaden the search.

	dc->DrawBitmap(bmpFitness,theCore->getInnerLeftBound()+300,theCore->getInnerUpperBound()+50);
	dc->DrawText(_T("Fitness Test"),theCore->getInnerLeftBound()+300,theCore->getInnerUpperBound()+30);

	if(animationNumbers%11==2) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,270,60,0,20);

// The new creations are then tested on their fitness. That means how fast they are able to fulfill the goals and how much resources they gather.

	wxMemoryDC temp_dc;
	temp_dc.SelectObject(wxBitmap(32,16));
	temp_dc.DrawBitmap(bmpNumbers,-animationNumbers,0);
	dc->Blit(theCore->getInnerLeftBound()+200,theCore->getInnerUpperBound()+50,32,16,&temp_dc,0,0);
	dc->DrawText(_T("Quicksort"),theCore->getInnerLeftBound()+200,theCore->getInnerUpperBound()+30);


// All possible solutions are now sorted after their fitness values

	dc->DrawBitmap(bmpTreppe,theCore->getInnerLeftBound()+100,theCore->getInnerUpperBound()+50);
	dc->DrawText(_T("Ranking"),theCore->getInnerLeftBound()+100,theCore->getInnerUpperBound()+30);

	if(animationNumbers%11==4) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,70,60,0,20);
	if(animationNumbers%11==3) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,170,60,0,20);

// And put together in a ranking. Pretty bad build orders will be deleted.

	dc->DrawBitmap(bmpCloning,theCore->getInnerLeftBound(),theCore->getInnerUpperBound()+50);
	dc->DrawText(_T(wxString::Format(wxT("%i%%"),ga->getBreedFactor())),theCore->getInnerLeftBound()+5,theCore->getInnerUpperBound()+55);
	dc->DrawText(_T("Cloning"),theCore->getInnerLeftBound(),theCore->getInnerUpperBound()+30);

	if(animationNumbers%11==5) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,40,110,1,20);


// A certain percentage of the build orders which were ranked best in the last step are now "cloned" to deepen the search for the optimal solution into that direction.

//	bla=(sin(((animationNumbers+3)%10)*3.141*0.2)+1.0)*2.5;
  //      if(ga->getCrossOver()==0)
    //	    bla=0;

	dc->DrawBitmap(bmpHeart[0],theCore->getInnerLeftBound(),theCore->getInnerUpperBound()+150);
	dc->DrawText(_T(wxString::Format(wxT("%i%%"),ga->getCrossOver())),theCore->getInnerLeftBound()+5,theCore->getInnerUpperBound()+155);
	dc->DrawText(_T("Crossing Over"),theCore->getInnerLeftBound(),theCore->getInnerUpperBound()+130);

	if(animationNumbers%11==6) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,50,160,2,20);


// In addition several build orders are choosen and merged together, like the chromosoms in nature. This is more or less another elaborate mutation .

	dc->DrawBitmap(bmpFitness,theCore->getInnerLeftBound()+100,theCore->getInnerUpperBound()+150);
	dc->DrawText(_T("Fitness Test"),theCore->getInnerLeftBound()+100,theCore->getInnerUpperBound()+130);

	if(animationNumbers%11==7) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,150,160,2,20);

// Through cloning and crossing over the build orders have changed. So some of them get under another fitness test.
																			    
	temp_dc.DrawBitmap(bmpNumbers,-animationNumbers-32,0);
	dc->Blit(theCore->getInnerLeftBound()+200,theCore->getInnerUpperBound()+150,32,16,&temp_dc,0,0);
	dc->DrawText(_T("Quicksort"),theCore->getInnerLeftBound()+200,theCore->getInnerUpperBound()+130);

	if(animationNumbers%11==8) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,250,160,2,20);


// The final population are sorted again after their fitness values
																			    
	dc->DrawBitmap(bmpTreppe,theCore->getInnerLeftBound()+300,theCore->getInnerUpperBound()+150);
	dc->DrawText(_T("Ranking"),theCore->getInnerLeftBound()+300,theCore->getInnerUpperBound()+130);

	if(animationNumbers%11==9) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,350,160,2,20);


// And put into a final ranking of this generation of build orders.

	dc->DrawBitmap(bmpAlpha,theCore->getInnerLeftBound()+400,theCore->getInnerUpperBound()+150);
	dc->DrawText(_T("Alpha program"),theCore->getInnerLeftBound()+400,theCore->getInnerUpperBound()+130);

	if(animationNumbers%11==10) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,450,160,2,20);

	if(animationNumbers%11==10) dc->SetPen(wxPen(wxColour(100,255,80),3,wxSOLID));else dc->SetPen(wxPen(wxColour(40,150,20),2,wxSOLID));
	theCore->drawArrow(dc,420,110,3,20);


// The best program, so-called "Alpha program" is chosen and gets under further analysis and tests to make the data user readable and ready to print it on the screen

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
	dc->SetBrush(wxBrush(wxColour(0,0,0),wxSOLID));
	animationNumbers++;
	if(animationNumbers>150)
	animationNumbers=1;

	processButtons();

	if(mainWindow->isShown())
	{
		mainWindow->Draw(dc);
		drawGizmo();
	}

	if(userIsNewbie)
	{
		dc->SetBrush(wxBrush(wxColour(0,0,0),wxSOLID));
		dc->SetPen(wxPen(wxColour(0,0,0),0,wxTRANSPARENT));
		dc->SetFont(GraphixScrollWindow::font);
//wxFont(FONT_SIZE,wxDECORATIVE,wxFONTSTYLE_ITALIC,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		wxString bla=_T("Click");
		int dx,dy;
		dc->GetTextExtent(bla,&dx,&dy);
		wxRect edget=wxRect(mainWindow->getLeftBound()+mainWindow->getWidth()-dx-190-3,mainWindow->getUpperBound()-dy-3,dx+26,dy+6);
		dc->DrawRectangle(edget);
		dc->DrawText(bla,mainWindow->getLeftBound()+mainWindow->getWidth()-dx-190,mainWindow->getUpperBound()-dy);
		dc->DrawBitmap(bmpArrowRight,mainWindow->getLeftBound()+mainWindow->getWidth()-180-animationNumbers%4,mainWindow->getUpperBound()-dy);
	}
		

	if(update==0) 
	{
		//return; //to avoid segfault before anarace is initialized in onIdle
		ANARACE* temp;
		if((temp=settings.newGeneration(anarace)))
		{
			anarace=temp;
			update=2;
			if(anarace->getRun()!=oldrun) {oldrun=anarace->getRun();endrun=1;}
			Refresh(false);
		};
		run=0;
	};


	if(update==2)
	{
		analyzeData();
		if(statisticsWindow->isShown()) //they are sorted there, otherwise we get an infinite loop
			ProgressGraph();
		CheckOldOrders();
		infoWindowNumber=0;
		if(infoWindow->isShown())
			infoWindowNumber=CheckForInfoWindow();
	}

	update=1;


	if(tutorialWindow->isShown())
	{
		tutorialWindow->DrawButtons(dc);	
		userIsNewbie=0;
		int t=0;
		for(t=0;t<8;t++)
		if(tutorialWindow->isPressed(t))
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
		if((tutorialAnimation==400)&&(!run))
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
                                                tutorialWindow->adjustWindow(wxRect(mainWindow->getInnerLeftBound()+mainWindow->getInnerWidth()-(SECOND_COLOUMN-30),mainWindow->getInnerUpperBound()+SECOND_ROW,tutorialWindow->getWidth(),tutorialWindow->getHeight()));
						boWindow->Show(0);
						forceWindow->Show(0);
						goalForceWindow->Show(0);
						timerWindow->Show(0);
						statisticsWindow->Show(0);
						boDiagramWindow->Show(0);
						boGraphWindow->Show(0);
						run=0;
					}
			
					if((tutorialAnimation<400)&&(run))
						tutorialAnimation=400;
					

					if(tutorialAnimation<200)
					{
						forceWindow->Show(1);
						goalForceWindow->Show(1);
					}
					else
					if(tutorialAnimation<400)
						timerWindow->Show(1);
					else
					if(tutorialAnimation<600)
					{
						tutorialWindow->adjustWindow(wxRect(theCore->getLeftBound(),theCore->getUpperBound(),tutorialWindow->getWidth(),tutorialWindow->getHeight()));
						timerWindow->Show(0);
						boWindow->Show(1);
					}
					else
					if(tutorialAnimation<800)
						timerWindow->Show(1);
					else if(tutorialAnimation<1000)
						statisticsWindow->Show(1);
					else if(tutorialAnimation<1200)
					{
						statisticsWindow->Show(0);
                                                tutorialWindow->adjustWindow(wxRect(theCore->getLeftBound(),mainWindow->getInnerUpperBound(),tutorialWindow->getWidth(),tutorialWindow->getHeight()));
						boDiagramWindow->Show(1);
						boGraphWindow->Show(1);
// Problem: da das hier ja oefters aufgerufen wird, wuerde jede Veraenderung durch andere Teile des Programms der Hoehe und Breite hier wieder zu nichte gemacht werden!						boDiagramWindow->adjustWindow(wxRect(boDiagramWindow->getLeftBound(),theCore->getLowerBound()+5+150,boDiagramWindow->getWidth(),boDiagramWindow->getHeight					
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


	if(timerWindow->isShown())
	{
		timerWindow->Draw(dc);
		showTimer();
		if(!run)
		{
			wxRect edge=wxRect(timerWindow->getInnerLeftBound()+timerWindow->getInnerWidth()-180,timerWindow->getInnerUpperBound()+timerWindow->getInnerHeight()-20,160,FONT_SIZE+10);
			dc->SetPen(wxPen(wxColour(100,100,255),2,wxSOLID));
			dc->SetBrush(wxBrush(wxColour(100-(animationNumbers%25)*2,100-(animationNumbers%25)*2,200-(animationNumbers%50)*2),wxSOLID));
			dc->DrawRoundedRectangle(edge,3);
			dc->SetTextForeground(wxColour(255,255,255));
			dc->SetFont(GraphixScrollWindow::font2);
			dc->DrawText(_T("Click to continue"),edge.x+10,edge.y+3);
			continueButton=timerWindow->addButton(edge);
		}
	}

	if(statisticsWindow->isShown())
	{
		statisticsWindow->Draw(dc);
		drawStatistics();
	}

	if(boDiagramWindow->isShown())
	{
		boDiagramWindow->Draw(dc);
		showProgramGraph();
	}


	if(boGraphWindow->isShown())
	{
		boGraphWindow->Draw(dc);
		showBoGraph();
	}

	if(boWindow->isShown())
	{
		MoveOldOrders();
		dc->SetTextForeground(convertColour(255,0,0));
		boWindow->Draw(dc);
		drawBuildOrder();
		
		dc->SetPen(wxPen(convertColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
		dc->SetTextForeground(wxColour(100,100,255));
		dc->SetFont(GraphixScrollWindow::font2);
		dc->DrawText(_T("Optimize everything"),boWindow->getInnerLeftBound(),boWindow->getInnerUpperBound()-32);
		dc->DrawText(_T("Optimize selected"),boWindow->getInnerLeftBound(),boWindow->getInnerUpperBound()-18);
		dc->SetBrush(boWindow->getBackground());
		wxRect edge=wxRect(boWindow->getInnerLeftBound()+boWindow->getWidth()-48,boWindow->getInnerUpperBound()-32,12,12);
		wxRect edge2=wxRect(boWindow->getInnerLeftBound()+boWindow->getWidth()-48,boWindow->getInnerUpperBound()-18,12,12);

		dc->DrawRoundedRectangle(edge,3);
		dc->DrawRoundedRectangle(edge2,3);

		dc->SetTextForeground(wxColour(100,200,100));
		if(!optimizeMode) dc->DrawCheckMark(edge);
		else
		if(optimizeMode)
		{
			dc->DrawBitmap(bmpArrowDown,boWindow->getInnerLeftBound()+boWindow->getWidth()-46,boWindow->getInnerUpperBound()-4);
			dc->DrawCheckMark(edge2);
		}
		drawSelectionStuff();
	}




	if(goalForceWindow->isShown())
	{
		goalForceWindow->Draw(dc);
		forceWindow->Draw(dc);
		drawGoalList();
	}

	if(theCore->isShown())
	{
		theCore->Draw(dc);
//		showCoreAnimation();
		drawGeneString();
		theCore->DrawButtons(dc);
	}

	if(msgWindow->isShown())
	{
		msgWindow->Draw(dc);
		drawMessages();
	}

	if(endrun)
	{
	       addMessage(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace->getRun(),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60)));
		resetData();
	}
/*		dc->SetFont(font3);
		dc->SetTextForeground(wxColour(155*grey*(100-grey)/2500,0,0));
		dc->DrawText(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace->getRun(),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60)),27,203);
																			    
		dc->SetTextForeground(wxColour(255*grey*(100-grey)/2500,0,0));
		dc->DrawText(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace->getRun(),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60)),25,200);
		if(grey==100)
		grey-=2;
		if(grey<=0) resetData();
	} else
	if(grey<100)
	{
		dc->SetTextForeground(wxColour(130*grey*(100-grey)/2500,0,0));
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("Round %i"),anarace->getRun()+1)),msgWindow->getInnerLeftBound()+18,msgWindow->getInnerUpperBound()+13);
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("%s"),gizmo[gizmor])),msgWindow->getInnerLeftBound()+18,msgWindow->getInnerUpperBound()+63);
																			    
		dc->SetTextForeground(wxColour(230*grey*(100-grey)/2500,0,0));
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("Round %i"),anarace->getRun()+1)),msgWindow->getInnerLeftBound()+15,msgWindow->getInnerUpperBound()+10);
		dc->SetFont(font3);
		dc->DrawText(_T(wxString::Format(wxT("%s"),gizmo[gizmor])),msgWindow->getInnerLeftBound()+15,msgWindow->getInnerUpperBound()+60);
																			    
		grey+=2;
	}*/


	if(infoWindow->isShown())
	{
		if(infoWindowNumber) //infoWindow needs to be AFTER theCore
			showInfoWindow();
	}
	wxPaintDC dcp(this);

//	wxScreenDC* dcp2=new wxScreenDC();
//	dcp2->StartDrawingOnTop();
	dcp.Blit(0, 0, SCREEN_X, SCREEN_Y, dc, 0, 0);
//	dcp2->EndDrawingOnTop();
//	delete dcp2;
}
										
//settings: log level (none, debug only, +final result, +result of each run, +snapshot every X generations, +snapshot every generation, EVERYTHING (~2MB/generation!)

void MyFrame::OnSettingsDialogApply()
{
	child->settings.setMaxTime(spin1->GetValue());
	child->settings.setMaxTimeOut(spin2->GetValue());
	child->settings.setMaxLength(spin3->GetValue());
	child->settings.setMaxRuns(spin4->GetValue());
	child->settings.setMaxGenerations(spin5->GetValue());
	child->settings.setBreedFactor(spin6->GetValue());
	child->settings.setCrossOver(spin7->GetValue());
	if(check1->GetValue())
		child->settings.setPreprocessBuildOrder(1);
	else child->settings.setPreprocessBuildOrder(0);

	child->settings.setCurrentMap(lb1->GetSelection());
	child->settings.setGoal(lb2->GetSelection(),1);
	child->settings.setGoal(lb2->GetSelection(),2);
}


void MyDCWindow::OnIdle(wxIdleEvent& WXUNUSED(event))
{
	if(endrun==0)
	{
		ANARACE* temp;
		if((run==1)&&(temp=settings.newGeneration(anarace)))
		{
			anarace=temp;
			//SetStatusText(_T(wxString::Format(wxT("%i runs and %i+ generations remaining. [Total %i generations]"),ga->maxRuns-anarace->getRun(),ga->maxGenerations-anarace->getUnchangedGenerations(),anarace->getGeneration())));
			update=2;
			if(anarace->getRun()!=oldrun) {oldrun=anarace->getRun();endrun=1;}
//			Refresh(false);
		};
	};
//	if(update==1)
//	{
//		update=2;
//		Refresh(false);
//	}
}

void MyDCWindow::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	Refresh(false);
}

void MyDCWindow::OnMouseScroll(wxMouseEvent& event)
{
	int msy=event.GetWheelRotation();
	boWindow->OnScrollMouse(msy);
	goalForceWindow->OnScrollMouse(msy);
	forceWindow->OnScrollMouse(msy);
	msgWindow->OnScrollMouse(msy);
}

void MyDCWindow::OnMouseMove(wxMouseEvent& event)
{
	event.GetPosition(&mouseX,&mouseY);
	boWindow->setMouse(mouseX,mouseY);
	goalForceWindow->setMouse(mouseX,mouseY);
	forceWindow->setMouse(mouseX,mouseY);
	theCore->setMouse(mouseX,mouseY);
	msgWindow->setMouse(mouseX,mouseY);
	mainWindow->setMouse(mouseX,mouseY);
	boGraphWindow->setMouse(mouseX,mouseY);
	statisticsWindow->setMouse(mouseX,mouseY);
	boDiagramWindow->setMouse(mouseX,mouseY);
	timerWindow->setMouse(mouseX,mouseY);
	tutorialWindow->setMouse(mouseX,mouseY);
	infoWindowNumber=0;
	if(infoWindow->isShown())
		infoWindowNumber=CheckForInfoWindow();
}

void MyDCWindow::OnMouseLeftDown(wxMouseEvent& event)
{
	boWindow->OnMouseLeftDown();
	goalForceWindow->OnMouseLeftDown();
	forceWindow->OnMouseLeftDown();
	theCore->OnMouseLeftDown();
	msgWindow->OnMouseLeftDown();
	mainWindow->OnMouseLeftDown();
	timerWindow->OnMouseLeftDown();
	tutorialWindow->OnMouseLeftDown();
}

void MyDCWindow::OnMouseLeftUp(wxMouseEvent& event)
{
	boWindow->OnMouseLeftUp();
	goalForceWindow->OnMouseLeftUp();
	forceWindow->OnMouseLeftUp();
	msgWindow->OnMouseLeftUp();
	timerWindow->OnMouseLeftUp();
	mainWindow->OnMouseLeftUp();
	timerWindow->OnMouseLeftUp();
	tutorialWindow->OnMouseLeftUp();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}
																			    
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
	if(child->run==1) child->run=2;
	while(true)
	{
		int error;
		int temp=fileDialog.ShowModal();
		switch(temp)
		{
			case wxID_OK:
				{
					wxString path=fileDialog.GetPath();
					if(!(error=(child->settings.loadMapFile(path))))
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
	if(child->run==2) child->run=1;
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
	if(child->run==1) child->run=2;
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
					if(!(error=(child->settings.loadGoalFile(path))))
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
	if(child->run==2) child->run=1;
	menuMap->Enable(EF_GoalImport,true);
};

void MyFrame::OnGeneralSettings(wxCommandEvent& WXUNUSED(event))
{
	wxStaticText *text1,*text2,*text3,*text4,*text5,*text7,*text8,*text9,*text10;
	menuSettings->Enable(EF_GeneralSettings,false);
	if(child->run==1) child->run=2;
	dia=new wxDialog(this,EF_SettingsDialog,_T("General settings"),wxPoint(100,100),wxSize(450,260),wxDEFAULT_DIALOG_STYLE,_T("lala"));
	text1=new wxStaticText(dia,-1,_T("MaxTime"),wxPoint(10,10),wxDefaultSize,0,_T("Max Time of Calculation"));
	text2=new wxStaticText(dia,-1,_T("MaxTimeOut"),wxPoint(10,30),wxDefaultSize,0,_T("Max Timeout"));
	text3=new wxStaticText(dia,-1,_T("MaxLength"),wxPoint(10,50),wxDefaultSize,0,_T("Max Length"));
	text4=new wxStaticText(dia,-1,_T("MaxRuns"),wxPoint(10,70),wxDefaultSize,0,_T("Max Runs"));
	text5=new wxStaticText(dia,-1,_T("MaxGenerations"),wxPoint(10,90),wxDefaultSize,0,_T("Max Generations"));
	text7=new wxStaticText(dia,-1,_T("BreedFactor"),wxPoint(10,110),wxDefaultSize,0,_T("BreedFactor percentage"));
	text8=new wxStaticText(dia,-1,_T("CrossOver"),wxPoint(10,130),wxDefaultSize,0,_T("CrossOver percentage"));

	spin1=new wxSpinCtrl(dia,EF_SpinMaxTime,wxEmptyString,wxPoint(100,10),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxTime(),child->settings.getMAXMaxTime(),child->settings.getMaxTime(),_T("MaxTimeSpin"));
	spin2=new wxSpinCtrl(dia,EF_SpinMaxTimeOut,wxEmptyString,wxPoint(100,30),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxTimeOut(),child->settings.getMAXMaxTimeOut(),child->settings.getMaxTimeOut(),_T("MaxTimeOutSpin"));
	spin3=new wxSpinCtrl(dia,EF_SpinMaxLength,wxEmptyString,wxPoint(100,50),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxLength(),child->settings.getMAXMaxLength(),child->settings.getMaxLength(),_T("MaxLengthSpin"));
	spin4=new wxSpinCtrl(dia,EF_SpinMaxRuns,wxEmptyString,wxPoint(100,70),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxRuns(),child->settings.getMAXMaxRuns(),child->settings.getMaxRuns(),_T("MaxRunsSpin"));
	spin5=new wxSpinCtrl(dia,EF_SpinMaxGenerations,wxEmptyString,wxPoint(100,90),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxGenerations(),child->settings.getMAXMaxGenerations(),child->settings.getMaxGenerations(),_T("MaxGenerationsSpin"));
	spin6=new wxSpinCtrl(dia,EF_SpinBreedFactor,wxEmptyString,wxPoint(100,110),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINBreedFactor(),child->settings.getMAXBreedFactor(),child->settings.getBreedFactor(),_T("BreedFactorSpin"));
	spin7=new wxSpinCtrl(dia,EF_SpinCrossOver,wxEmptyString,wxPoint(100,130),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINCrossOver(),child->settings.getMAXCrossOver(),child->settings.getCrossOver(),_T("CrossOverSpin"));

	check1=new wxCheckBox(dia,EF_CheckPreprocess,_T("Preprocess Buildorder"),wxPoint(10,150),wxDefaultSize,-1,wxDefaultValidator,_T("preprocess"));
	if(child->settings.getPreprocessBuildOrder())
		check1->SetValue(TRUE);
	else check1->SetValue(FALSE);

	but1=new wxButton(dia,wxID_OK,_T("OK"),wxPoint(20,200),wxSize(65,20));
	but2=new wxButton(dia,wxID_APPLY,_T("Apply"),wxPoint(90,200),wxSize(65,20));
	but3=new wxButton(dia,wxID_RESET,_T("Defaults"),wxPoint(160,200),wxSize(65,20));
	but4=new wxButton(dia,wxID_CANCEL,_T("Cancel"),wxPoint(230,200),wxSize(65,20));

	wxString bla[MAX_MAPS];
	for(int i=0;i<child->settings.getMapCount();i++)
		bla[i]=_T(wxString::Format(wxT("%s"),child->settings.getMap(i)->getName()));
	text9=new wxStaticText(dia,-1,_T("SelectMap"),wxPoint(200,10),wxDefaultSize,0,_T("Select Map"));
	lb1=new wxListBox(dia,-1,wxPoint(200,30),wxSize(80,150),child->settings.getMapCount(),bla,0,wxDefaultValidator,_T("listBox"));
	lb1->SetSelection(child->settings.getCurrentMap());

	wxString bla2[MAX_MAPS];
	for(int i=0;i<child->settings.getGoalCount();i++)
		bla2[i]=_T(wxString::Format(wxT("%s"),child->settings.getGoal(i)->getName()));
	text10=new wxStaticText(dia,-1,_T("SelectGoal"),wxPoint(300,10),wxDefaultSize,0,_T("Select Goal"));
	lb2=new wxListBox(dia,-1,wxPoint(300,30),wxSize(80,150),child->settings.getGoalCount(),bla2,0,wxDefaultValidator,_T("listBox"));
	//lb2->SetSelection(settings.getCurrentGoal()); ~~~ multiple players...

	int temp;
	while(true)
	{
		temp=dia->ShowModal();
		switch(temp)
		{
			case wxID_CANCEL:break;
			case wxID_APPLY:
			case wxID_OK:OnSettingsDialogApply();break;
			case wxID_RESET:child->settings.loadDefaults();break;
		}
		if((temp==wxID_CANCEL)||(temp==wxID_OK)) break;
	}
	if(child->run==2) child->run=1;
	menuSettings->Enable(EF_GeneralSettings,true);
}

void MyFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
	if(child->run==0) child->run=1;
#if wxUSE_STATUSBAR
	SetStatusText(_T("Calculation in progress :-)")); 
#endif // wxUSE_STATUSBAR
	menuFile->Enable(EF_Start,false);
}

void MyFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
	if(child->run==1) child->run=0;
#if wxUSE_STATUSBAR
	SetStatusText(_T("Click on File/Start to start the calculation!")); 
#endif // wxUSE_STATUSBAR

	menuFile->Enable(EF_Start,true);
}

																			    
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxStaticText* text1;
	menuSettings->Enable(EF_About,false);
	if(child->run==1) child->run=2;
	wxString msg;
	msg.Printf(_T("- Evolution Forge v%i\n- Current source code size: 400kb\n- GUI and library core programmed by Clemens Lode\n- Copyright by Clemens Lode\n- GUI created with %s library\n- Written with VIM - Vi IMproved version 6.2.120\n- MS Windows version compiled with MinGW / GCC 3.3.2\n- Windows executable/dll size: 396kb/344kb\n- Linux (GTK) version compiled with GCC 3.3.2\n- Linux binary/lib size: 180kb/248kb\n- Tested with winex3.2, MS Windows ME and Linux (2.4.22)\n- Work was done with Linux Fedora Core 1 (Redhat 10)\n\nThis is ONLY a demonstration program which may not be sold or copied without permission.\nThere is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\nGreetings, ghoul@clawsoftware.de"), EF_Version,wxVERSION_STRING);
	msgBox=new wxDialog(this,-1,_T("About EF"),wxPoint(100,100),wxSize(600,300),wxDEFAULT_FRAME_STYLE,_T("lala"));
	text1=new wxStaticText(msgBox,-1,msg,wxPoint(10,10),wxDefaultSize,0,_T("bla"));
//homepage, updates
//	but1=new wxButton(msgBox,wxID_OK,_T("OK"),wxPoint(20,275),wxSize(65,20));
	while(msgBox->ShowModal()!=wxID_CANCEL);
	delete(msgBox);
	if(child->run==2) child->run=1;
	menuSettings->Enable(EF_About,true);
}



