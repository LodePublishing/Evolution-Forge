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

BEGIN_EVENT_TABLE(MyDCWindow, wxScrolledWindow)
	EVT_ERASE_BACKGROUND(MyDCWindow::OnEraseBackground)
	EVT_IDLE(MyDCWindow::OnIdle)
	EVT_PAINT(MyDCWindow::OnPaint)
	EVT_MOTION(MyDCWindow::OnMouseMove)
	EVT_LEFT_UP(MyDCWindow::OnMouseLeftUp)
	EVT_LEFT_DOWN(MyDCWindow::OnMouseLeftDown)
	EVT_MOUSEWHEEL(MyDCWindow::OnMouseScroll)
END_EVENT_TABLE()

const wxColour BOcolor[11]={wxColour(0,0,0),wxColour(25,25,25),wxColour(0,50,0),wxColour(50,0,0),wxColour(50,0,50),wxColour(0,0,50),wxColour(0,50,50),wxColour(50,50,0),wxColour(0,0,25),wxColour(25,0,0),wxColour(0,25,0)};

GraphixScrollWindow* boWindow;
GraphixScrollWindow* goalForceWindow;
GraphixScrollWindow* forceWindow;
GraphixScrollWindow* mainWindow;
GraphixScrollWindow* graphWindow;
GraphixScrollWindow* timerWindow;
GraphixScrollWindow* diagramWindow;
GraphixScrollWindow* boGraphWindow;
GraphixScrollWindow* adWindow;
GraphixScrollWindow* haxor;

int SCREEN_X;
int SCREEN_Y;

int gizmor;
const int COLOR1R=150;
const int COLOR1G=150;
const int COLOR1B=150;

const int COLOR2R=200;
const int COLOR2G=200;
const int COLOR2B=200;

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

const int INFOWINDOWTEXTR=50;
const int INFOWINDOWTEXTG=200;
const int INFOWINDOWTEXTB=50;

int grey; //0-100%



wxColour convertColour(int r, int g, int b)
{
	int col=(r+g+b)/3;
//	return(wxColour(r,g,b));
	return(wxColour((int)(col+(r-col)*grey/100),(int)(col+(g-col)*grey/100),(int)(col+(b-col)*grey/100)));
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
        SCREEN_X=1020;SCREEN_Y=713;
	MyFrame *frame = new MyFrame(_T("Evolution Forge v1.08"), wxPoint(0, 0), wxSize(1024,768));
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
        bitmap.LoadFile("clawsoftware.bmp", wxBITMAP_TYPE_BMP);
	bitmap2.LoadFile("clemens.bmp", wxBITMAP_TYPE_BMP);
	int i,k;
	maxpFitness=100;
	maxsFitness=100;
	maxtFitness=100;
	mintFitness=999;
	maxForce=1;
	maxUnitForce=1;
																			    
	for(i=0;i<200;i++)
	{
		pFitness[i]=0;
		sFitness[i]=0;
		tFitness[i]=0;
		aFitness[i]=0;
		vFitness[i]=0;
		length[i]=0;
		time[i]=ga->maxTime*100;
		force[i]=0;
	};
																			    
	for(k=0;k<20;k++)
	{
		oldTimeCounter[k]=0;
		oldTime[k]=ga->maxTime;
	}
																			    
	for(k=0;k<FORCE_LIST_NUMBER;k++)
		oldForceList[k]=0;
																			    
	for(k=0;k<MAX_LENGTH*50;k++)
	{
		oldOrder[k]->blend=0;
		oldOrder[k]->blendTarget=0;
		oldOrder[k]->x=0;oldOrder[k]->y=0;
		oldOrder[k]->targetx=0;oldOrder[k]->targety=0;
		oldOrder[k]->targetwidth=0;
		oldOrder[k]->dx=0;oldOrder[k]->dy=0;
		oldOrder[k]->unit=0;oldOrder[k]->marker=0;
		oldOrder[k]->forceCount=0;oldOrder[k]->successType=0;oldOrder[k]->successUnit=0;oldOrder[k]->location=0;
		oldOrder[k]->facility=0;oldOrder[k]->availibleFacilityCount=0;oldOrder[k]->forceFacilityCount=0;
		oldOrder[k]->code=0;
																			    
		oldOrder[k]->bx=0;oldOrder[k]->by=0;
		oldOrder[k]->targetbx=0;oldOrder[k]->targetby=0;
																			    
		oldOrder[k]->bheight=0;oldOrder[k]->bwidth=0;
		oldOrder[k]->targetbheight=0;oldOrder[k]->targetbwidth=0;
																			    
		oldOrder[k]->bonew=0;
	}
	infoWindow=0;infoWindowX=0;infoWindowY=0;
	endrun=0;
	gizmor=rand()%GIZMO_NUMBER;
	anarace=NULL;
	update=0;mouseLeft=0;
}

MyDCWindow::MyDCWindow(wxFrame *parent) 
	:wxScrolledWindow(parent)
{
//	SetScrollbars(10,10,40,100,0,0);
//	bmpBack2.LoadFile("back2.bmp",wxBITMAP_TYPE_BMP);
        dt1=wxDateTime::UNow();
#ifdef __WIN32__	
	font=wxFont(FONT_SIZE,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
	font2=wxFont(FONT_SIZE,wxDEFAULT,wxFONTSTYLE_ITALIC,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
#else
	font=wxFont(FONT_SIZE,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
	font2=wxFont(FONT_SIZE,wxDECORATIVE,wxFONTSTYLE_ITALIC,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
#endif
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
	for(k=0;k<MAX_LENGTH*50;k++)
		oldOrder[k]=new OLDORDER;
	oldrun=0;
	grey=0;
	resetData();
	dt2=wxDateTime::UNow();
	averagecounter=0;
	for(i=0;i<100;i++)
		average[i]=0;
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

	boWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,SECOND_COLOUMN+40,(BUILD_ORDER_NUMBER+10)*(FONT_SIZE+5)+3),1,5,10);
	boWindow->addTitle(0,"Build Order");
      boWindow->addDescription(0,"Name");boWindow->addDescription(110,"Mins");boWindow->addDescription(150,"Gas");boWindow->addDescription(190,"Supply");boWindow->addDescription(243,"Time");

	goalForceWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,SECOND_COLOUMN_WIDTH,11*(FONT_SIZE+5)+8),0,boWindow->getRightBound()+10,10);
	goalForceWindow->addTitle(0,"Number of units part of the goal list");
        forceWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,SECOND_COLOUMN_WIDTH,8*(FONT_SIZE+5)+3),0,goalForceWindow->getLeftBound(),goalForceWindow->getLowerBound()+10);
	forceWindow->addTitle(0,"Number of units not in the goal list");

	mainWindow=new GraphixScrollWindow(wxColour(100,100,255), wxColour(5,25,80), wxColour(40,150,20), wxRect(0,0,SCREEN_X,SCREEN_Y),1,0,0,0);
	mainWindow->addTitle(SCREEN_X/2,_T(wxString::Format(wxT("Evolution Forge v.%i "),EF_Version)));

        timerWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,200,100),1,forceWindow->getRightBound()+10,10,0);
        timerWindow->addTitle(0,"Best time until now");

	graphWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,410,140),1,timerWindow->getLeftBound(),timerWindow->getLowerBound()+10,0);
	graphWindow->addTitle(0,"Statistical Data");

	int y1;
	if(graphWindow->getLowerBound()>forceWindow->getLowerBound()) y1=graphWindow->getLowerBound();
		else y1=forceWindow->getLowerBound();

        diagramWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,550,100),0,forceWindow->getLeftBound(),y1+10,0);
        diagramWindow->addTitle(0,"Overview of best build order");


	boGraphWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,550,220),1,forceWindow->getLeftBound(),diagramWindow->getLowerBound()+10,0); 
        boGraphWindow->addTitle(0,"Graphical illustration of the build order");

	adWindow=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,320,120),0,mainWindow->getInnerLeftBound(),mainWindow->getInnerUpperBound()+mainWindow->getInnerHeight()-130,0);
	adWindow->addTitle(0,"Spam }8-) ");

	haxor=new GraphixScrollWindow(wxColour(255,0,0), wxColour(5,25,0), wxColour(40,150,20), wxRect(0,0,550,100),0,boGraphWindow->getLeftBound(),boGraphWindow->getLowerBound()+10,0);
	haxor->addTitle(0,"H4Xx0r 57uff! :D");

//TODO grey wieder rein... evtl bei draw

	child=new MyDCWindow(this);
}

void MyDCWindow::showGraph(int* data,int max,wxColour col)
{
	int i,j;
	dc->SetPen(wxPen(col,2,wxSOLID));
	wxPoint blub[200];
	j=1;
	for(i=1;i<199;i++)
	{
		if(data[i]!=data[i+1])
		{
			blub[j]=wxPoint(i+graphWindow->getInnerLeftBound(),graphWindow->getInnerUpperBound()+100-data[i]/(max+1));
			j++;
		}
	}
	blub[0]=wxPoint(graphWindow->getInnerLeftBound(),graphWindow->getInnerUpperBound()+100-data[0]/(max+1));
	blub[j]=wxPoint(199+graphWindow->getInnerLeftBound(),graphWindow->getInnerUpperBound()+100-data[199]/(max+1));
	dc->DrawSpline(j+1,blub);
};

void MyDCWindow::showForceListBack()
{
//      dc->DrawBitmap(bmpBack2,SECOND_COLOUMN-10,BUILD_ORDER_GRAPH_LENGTH,true);
/*

	dc->SetBrush(wxBrush(convertColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));
	dc->DrawRectangle(SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+10,500,53);

	dc->SetPen(wxPen(convertColour(INFOWINDOWPENR*3/4,INFOWINDOWPENG*3/4,INFOWINDOWPENB*3/4),1,wxSOLID));
	
	dc->DrawLine(SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+37,SECOND_COLOUMN+500,BUILD_ORDER_GRAPH_LENGTH+37);

	dc->DrawLine(SECOND_COLOUMN+125,BUILD_ORDER_GRAPH_LENGTH+10,SECOND_COLOUMN+125,BUILD_ORDER_GRAPH_LENGTH+63);
	dc->DrawLine(SECOND_COLOUMN+250,BUILD_ORDER_GRAPH_LENGTH+10,SECOND_COLOUMN+250,BUILD_ORDER_GRAPH_LENGTH+63);
	dc->DrawLine(SECOND_COLOUMN+375,BUILD_ORDER_GRAPH_LENGTH+10,SECOND_COLOUMN+375,BUILD_ORDER_GRAPH_LENGTH+63);

	dc->SetPen(wxPen(convertColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
	wxRect edge=wxRect();
	edge.x=0;
	edge.height=FONT_SIZE+4;
	edge.width=SECOND_COLOUMN-10-edge.x;*/
}

void MyDCWindow::showTimer()
{
	int i,j,k;
//	dc->DrawBitmap(bmpTimer,THIRD_COLOUMN-10,0,false);
	timerWindow->Draw(dc);
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
			dc->SetFont(wxFont(30,wxMODERN,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));

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
        dc->SetTextForeground(convertColour(255,0,0));
	if(anarace->getTimer()==0)
	{
		dc->SetFont(wxFont(24,wxMODERN,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		bla=_T("None yet 8[");
	}
	else 
	{
	        dc->SetFont(wxFont(30,wxMODERN,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		bla=_T(wxString::Format(wxT("[%.2i:%.2i]"),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60));
	}

           int dx,dy;
           dc->GetTextExtent(bla,&dx,&dy);

           dc->DrawText(bla,timerWindow->getInnerLeftBound()+(timerWindow->getInnerWidth()-dx)/2,timerWindow->getInnerUpperBound()+(timerWindow->getInnerHeight()-dy)/2);
}

void MyDCWindow::showProgramGraph()
{
	int s,y1;
	wxPoint mins[MAX_LENGTH];
	wxPoint gas[MAX_LENGTH];
	wxPoint fitness[MAX_LENGTH];
	wxPoint supply[MAX_LENGTH];
	int count=1;

	dc->SetBrush(wxBrush(wxColour(50,50,50),wxCROSS_HATCH));
	dc->DrawRectangle(diagramWindow->getInnerLeftBound(),diagramWindow->getInnerUpperBound(),diagramWindow->getInnerWidth(),diagramWindow->getInnerHeight());
												       
	for(s=MAX_LENGTH;s--;) //~~ Max length
		if((anarace->getProgramIsBuilt(s)==1)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
		{
			if(anarace->getProgramHaveMinerals(s)>50000) y1=50; else y1=anarace->getProgramHaveMinerals(s)/1000;
			mins[count]=wxPoint(diagramWindow->getInnerLeftBound()+3+((anarace->getProgramTime(s)*(diagramWindow->getInnerWidth()-6))/(ga->maxTime-anarace->getTimer())),diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);

			if(anarace->getProgramHaveGas(s)>50000) y1=50; else y1=anarace->getProgramHaveGas(s)/1000;
			gas[count]=wxPoint(diagramWindow->getInnerLeftBound()+3+((anarace->getProgramTime(s)*(diagramWindow->getInnerWidth()-6))/(ga->maxTime-anarace->getTimer())),diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!

			y1=anarace->getProgramFitness(s)*50/(anarace->getMaxpFitness()-anarace->getTimer());
			fitness[count]=wxPoint(diagramWindow->getInnerLeftBound()+3+((anarace->getProgramTime(s)*(diagramWindow->getInnerWidth()-6))/(ga->maxTime-anarace->getTimer())),diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);

			if(anarace->getProgramHaveSupply(s)-anarace->getProgramNeedSupply(s)>16) y1=50; else 
			if(anarace->getProgramHaveSupply(s)<anarace->getProgramNeedSupply(s)) y1=0; else
			y1=(anarace->getProgramHaveSupply(s)-anarace->getProgramNeedSupply(s))*3;
			supply[count]=wxPoint(diagramWindow->getInnerLeftBound()+3+((anarace->getProgramTime(s)*(diagramWindow->getInnerWidth()-6))/(ga->maxTime-anarace->getTimer())),diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);
			count++;
		}
												       
//TODO: Wenn !ready werden 30 Minuten dargestellt, aber im letzten Teil haben wir keine Aufzeichnungen ueber Mineralien etc. mehr :/
												       
	if(anarace->getPlayer()->getMins()>50000) y1=50; else y1=anarace->getPlayer()->getMins()/1000;
	mins[0]=wxPoint(diagramWindow->getInnerLeftBound()+2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);
	if(anarace->getPlayer()->getGas()>50000) y1=50; else y1=anarace->getPlayer()->getGas()/1000;
	gas[0]=wxPoint(diagramWindow->getInnerLeftBound()+2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);
	fitness[0]=wxPoint(diagramWindow->getInnerLeftBound()+2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight());

	if(anarace->getPlayer()->getMaxSupply()-anarace->getPlayer()->getSupply()>16) y1=50; else 
	if(anarace->getPlayer()->getMaxSupply()<anarace->getPlayer()->getSupply()) y1=0; else
	y1=(anarace->getPlayer()->getMaxSupply()-anarace->getPlayer()->getSupply())*3;
	supply[0]=wxPoint(diagramWindow->getInnerLeftBound()+2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);
												       
	if(anarace->getMins()>50000) y1=50; else y1=anarace->getMins()/1000;
	mins[count]=wxPoint(diagramWindow->getInnerLeftBound()+diagramWindow->getInnerWidth()-2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);
												       
	if(anarace->getGas()>50000) y1=50; else y1=anarace->getGas()/1000;
	gas[count]=wxPoint(diagramWindow->getInnerLeftBound()+diagramWindow->getInnerWidth()-2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);
												       
	//Problem: Falls Fitness nicht ganz erfuellt, ist es trotzdem ein schraeger Strich... impliziert eine Entwicklung O_o
	fitness[count]=wxPoint(diagramWindow->getInnerLeftBound()+diagramWindow->getInnerWidth()-2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-50);

	if(anarace->getMaxSupply()-anarace->getSupply()>16) y1=50; else 
	if(anarace->getMaxSupply()<anarace->getSupply()) y1=0; else
	y1=(anarace->getMaxSupply()-anarace->getSupply())*3;
	supply[count]=wxPoint(diagramWindow->getInnerLeftBound()+diagramWindow->getInnerWidth()-2,diagramWindow->getInnerUpperBound()+diagramWindow->getInnerHeight()-y1);

	count++;
												       
	if(count>0)
	{
		dc->SetPen(wxPen(convertColour(120,120,120),2,wxSOLID));
		dc->DrawSpline(count,supply);
		dc->SetPen(wxPen(convertColour(80,80,255),2,wxSOLID));
		dc->DrawSpline(count,mins);
		dc->SetPen(wxPen(convertColour(40,255,40),2,wxSOLID));
		dc->DrawSpline(count,gas);
		dc->SetPen(wxPen(convertColour(255,40,40),2,wxSOLID));
		dc->DrawSpline(count,fitness);
		dc->SetPen(wxPen(convertColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
	}
	dc->SetFont(font2);
	dc->SetTextForeground(convertColour(255,40,40));	 
	dc->DrawText(_T("Fitness"),diagramWindow->getInnerLeftBound()+1,diagramWindow->getInnerUpperBound()+8);
	dc->SetTextForeground(convertColour(80,80,255));	 
	dc->DrawText(_T("Minerals"),diagramWindow->getInnerLeftBound()+1,diagramWindow->getInnerUpperBound()+19);
	dc->SetTextForeground(convertColour(40,255,40));
	dc->DrawText(_T("Gas"),diagramWindow->getInnerLeftBound()+1,diagramWindow->getInnerUpperBound()+30);
	dc->SetTextForeground(convertColour(160,160,160));
	dc->DrawText(_T("Supply"),diagramWindow->getInnerLeftBound()+1,diagramWindow->getInnerUpperBound()+41); 
}

void MyDCWindow::OnPaint(wxPaintEvent& event)
{
	int i,j,k,s,t;
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
	wxPaintDC dcp(this);
	if(update==0) return; //to avoid segfault before anarace is initialized in onIdle
	if(dc) delete(dc);
	dc=new wxMemoryDC();
	dc->SelectObject(wxBitmap(SCREEN_X,SCREEN_Y));

        dc->SetFont(font2);
	mainWindow->Draw(dc);	

	if(update==2)
	{
		for(i=0;i<199;i++)
		{
			pFitness[i]=pFitness[i+1];
			sFitness[i]=sFitness[i+1];
			tFitness[i]=tFitness[i+1];
			aFitness[i]=aFitness[i+1];
			vFitness[i]=vFitness[i+1];
			length[i]=length[i+1];
			time[i]=time[i+1];
			force[i]=force[i+1];
		};
		t=0;
		for(i=0;i<GAS_SCV;i++)
		{
			if(anarace->getLocationForce(0,i)>maxUnitForce)
				maxUnitForce=anarace->getLocationForce(0,i);
			t+=anarace->getLocationForce(0,i);;
		}
		if(t>maxForce) maxForce=t;
		
		if(anarace->getMaxpFitness()>maxpFitness) maxpFitness=anarace->getMaxpFitness();
		if(anarace->getMaxsFitness()>maxsFitness) maxsFitness=anarace->getMaxsFitness();
		if(anarace->getMaxtFitness()>maxtFitness) maxtFitness=anarace->getMaxtFitness();
		if(anarace->getMaxtFitness()<mintFitness) mintFitness=anarace->getMaxtFitness();

		pFitness[199]=anarace->getMaxpFitness()*100;
		sFitness[199]=anarace->getMaxsFitness()*100;
		tFitness[199]=(anarace->getMaxtFitness()-mintFitness)*100;
		aFitness[199]=anarace->fitnessAverage*100;
		vFitness[199]=(int)sqrt((double)anarace->fitnessVariance)*100;
		length[199]=anarace->getLength()*100;
		time[199]=(ga->maxTime-anarace->getTimer())*100;
		force[199]=t*100;
	
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

		for(s=0;s<MAX_LENGTH*50;s++)
			if(oldOrder[s]->blendTarget==50)
			{
				int notthere=1;
				for(t=0;(t<MAX_LENGTH)&&(notthere);t++)
					if(anarace->getProgramIsBuilt(t))
						notthere&=(anarace->Marker[anarace->getProgramDominant(t)][t]+1!=oldOrder[s]->marker);
				if(notthere) //loeschen
				{
					oldOrder[s]->blendTarget=0;
//					oldOrder[s]->targetx=-300;
//					oldOrder[s]->targetbx=900;
					oldOrder[s]->targetwidth=0;
				}
			}
																			    
		k=0;
		for(s=MAX_LENGTH;s--;)
			if(anarace->getProgramIsBuilt(s)&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer()))
			{
				int notthere=1;
				

				for(t=0;(t<MAX_LENGTH*50)&&(notthere);t++)
					notthere&=(oldOrder[t]->marker!=anarace->Marker[anarace->getProgramDominant(s)][s]+1);
				if(notthere) //neues erstellen
				{
					t=0;
					while(oldOrder[t]->marker) t++;
					oldOrder[t]->x=-300;oldOrder[t]->y=(k+1)*(FONT_SIZE+5);
					oldOrder[t]->dx=0;oldOrder[t]->dy=0;
					oldOrder[t]->blend=1;
					oldOrder[t]->unit=anarace->phaenoCode[s];
					oldOrder[t]->marker=anarace->Marker[anarace->getProgramDominant(s)][s]+1;
					oldOrder[t]->bonew=1;
				} else //altes anpassen
					t=t-1;
/*				if((oldOrder[t]->targety!=(k+1)*(FONT_SIZE+5)+3)&&(oldOrder[t]->x==oldOrder[t]->targetx))
				{
					if(oldOrder[t]->dx<(oldOrder[t]->targety-oldOrder[t]->y)/10)  oldOrder[t]->dx+=(oldOrder[t]->targety-oldOrder[t]->y)/10;
					else oldOrder[t]->dx=(oldOrder[t]->targety-oldOrder[t]->y)/5;
				}*/
				oldOrder[t]->targetwidth=SECOND_COLOUMN-8;
				oldOrder[t]->targetx=0;oldOrder[t]->targety=(k+1)*(FONT_SIZE+5);
				oldOrder[t]->blendTarget=50;
//hier auch hochzaehlen ...mmmh... TODO
				oldOrder[t]->time=anarace->getProgramTime(s);
				oldOrder[t]->mins=anarace->getProgramHaveMinerals(s)/100;
				oldOrder[t]->gas=anarace->getProgramHaveGas(s)/100;
				oldOrder[t]->needSupply=anarace->getProgramNeedSupply(s);
				oldOrder[t]->haveSupply=anarace->getProgramHaveSupply(s);
				oldOrder[t]->forceCount=anarace->getProgramForceCount(s,anarace->phaenoCode[s]);
				oldOrder[t]->forceFacilityCount=anarace->getProgramForceCount(s,anarace->getProgramFacility(s));
				oldOrder[t]->successType=anarace->getProgramSuccessType(s);
				oldOrder[t]->successUnit=anarace->getProgramSuccessUnit(s);
				oldOrder[t]->location=anarace->getProgramLocation(s);
				oldOrder[t]->facility=anarace->getProgramFacility(s);
				oldOrder[t]->availibleFacilityCount=anarace->getProgramAvailibleCount(s,anarace->getProgramFacility(s));
				oldOrder[t]->code=anarace->getProgramCode(s);
				k++;
			}

		boanzahl=k;
																			    
		for(i=0;i<MAX_LENGTH*50;i++)
			if(oldOrder[i]->marker)
			{
				oldOrder[i]->dx+=(oldOrder[i]->targetx-oldOrder[i]->x-oldOrder[i]->dx)/30;
				oldOrder[i]->x+=(oldOrder[i]->targetx-oldOrder[i]->x)/10+oldOrder[i]->dx;
				oldOrder[i]->dy+=(oldOrder[i]->targety-oldOrder[i]->y-oldOrder[i]->dy)/30;
				oldOrder[i]->y+=(oldOrder[i]->targety-oldOrder[i]->y)/10+oldOrder[i]->dy;
				if(abs(oldOrder[i]->x-oldOrder[i]->targetx)<10) {oldOrder[i]->x=oldOrder[i]->targetx;oldOrder[i]->dx=0;}
				if(abs(oldOrder[i]->y-oldOrder[i]->targety)<10) {oldOrder[i]->y=oldOrder[i]->targety;oldOrder[i]->dy=0;}
				oldOrder[i]->bx+=(oldOrder[i]->targetbx-oldOrder[i]->bx)/5;
				oldOrder[i]->by+=(oldOrder[i]->targetby-oldOrder[i]->by)/5;

				if(abs(oldOrder[i]->bx-oldOrder[i]->targetbx)<5) oldOrder[i]->bx=oldOrder[i]->targetbx;
				if(abs(oldOrder[i]->by-oldOrder[i]->targetby)<5) oldOrder[i]->by=oldOrder[i]->targetby;
	
                                if((oldOrder[i]->targetwidth-oldOrder[i]->width)/5==0)
                                {
                                        if(oldOrder[i]->targetwidth>oldOrder[i]->width)
                                                oldOrder[i]->width++;
                                        else if(oldOrder[i]->targetwidth<oldOrder[i]->width)
                                                oldOrder[i]->width--;
                                }
                                else
                                        oldOrder[i]->width+=(oldOrder[i]->targetwidth-oldOrder[i]->width)/5;


				if((oldOrder[i]->targetbwidth-oldOrder[i]->bwidth)/5==0)
				{
					if(oldOrder[i]->targetbwidth>oldOrder[i]->bwidth)
						oldOrder[i]->bwidth++;
					else if(oldOrder[i]->targetbwidth<oldOrder[i]->bwidth)
						oldOrder[i]->bwidth--;
				}
				else 
					oldOrder[i]->bwidth+=(oldOrder[i]->targetbwidth-oldOrder[i]->bwidth)/5;
	
				if((oldOrder[i]->targetbheight-oldOrder[i]->bheight)/5==0)
				{
					if(oldOrder[i]->targetbheight>oldOrder[i]->bheight)
						oldOrder[i]->bheight++;
					else if(oldOrder[i]->targetbheight<oldOrder[i]->bheight)
						oldOrder[i]->bheight--;
				}
				else
					oldOrder[i]->bheight+=(oldOrder[i]->targetbheight-oldOrder[i]->bheight)/5;

				oldOrder[i]->blend+=(oldOrder[i]->blendTarget-oldOrder[i]->blend)/10;
				if((oldOrder[i]->blendTarget==0)&&(oldOrder[i]->blend<10)) oldOrder[i]->blend=0; //~~
				if((oldOrder[i]->blendTarget==50)&&(oldOrder[i]->blend>40)) oldOrder[i]->blend=50;
			} 
	}//update==2 end
	update=1;

	infoWindow=0;
	int fenster=1;
	for(i=0;(i<MAX_LENGTH*50)&&(fenster);i++)
		if(oldOrder[i]->blend)
		{	
			wxRect edge(oldOrder[i]->x+boWindow->getInnerLeftBound(),oldOrder[i]->y+boWindow->getInnerUpperBound()-boWindow->getScrollY(),oldOrder[i]->width,FONT_SIZE+4);
			wxRect edge2(oldOrder[i]->bx+boGraphWindow->getInnerLeftBound(),oldOrder[i]->by+boGraphWindow->getInnerUpperBound()/*-boGraphWindow->getScrollY()*/,oldOrder[i]->bwidth,oldOrder[i]->bheight);
/*		        for(i=0;i<20;i++)         
				if(bograph[i].type>0)
			                for(j=i;j<i+bograph[i].lines;j++)
			                {*/
//			                        edge.y=(j+1)*(FONT_SIZE+5)+boGraphWindow->getInnerUpperBound();
			                        if((boWindow->fitToClientArea(edge)&&edge.Inside(mouseX,mouseY))||
						    (boGraphWindow->fitToClientArea(edge2)&&edge2.Inside(mouseX,mouseY)))
						{
							infoWindow=i+1;
							infoWindowX=mouseX+10;infoWindowY=mouseY;
							fenster=0;
						}
//					}
		}


	if(fenster)
	{
//	      if((x<SECOND_COLOUMN-10)&&(x>=0)&&()
		//position pruefen ob im Bereich der bos, rechte Maustaste => einfuegen
	}


	showTimer();
	dc->SetFont(font2);
	graphWindow->Draw(dc);

	dc->SetPen(wxPen(convertColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
	dc->SetBrush(wxBrush(convertColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),wxCROSS_HATCH));

	dc->DrawRectangle(graphWindow->getInnerLeftBound(),graphWindow->getInnerUpperBound(),200,100);

//	dc->DrawLine(graphWindow->getInnerLeftBound(),graphWindow->getInnerUpperBound()+33,graphWindow->getInnerLeftBound()+200,graphWindow->getInnerUpperBound()+33);
//	dc->DrawLine(graphWindow->getInnerLeftBound(),graphWindow->getInnerUpperBound()+66,graphWindow->getInnerLeftBound()+200,graphWindow->getInnerUpperBound()+66);

//	dc->DrawLine(graphWindow->getInnerLeftBound()+67,90,graphWindow->getInnerLeftBound()+67,190);
//	dc->DrawLine(graphWindow->getInnerLeftBound()+134,90,graphWindow->getInnerLeftBound()+124,190);

//	dc->SetPen(wxPen(convertColour(INFOWINDOWPENR*3/4,INFOWINDOWPENG*3/4,INFOWINDOWPENB*3/4),1,wxSOLID)); ??

	showGraph(tFitness,maxtFitness-mintFitness,convertColour(255,40,255));
	showGraph(sFitness,maxsFitness,convertColour(40,255,40));
	showGraph(force,maxForce,convertColour(40,255,255));
	showGraph(length,MAX_LENGTH,convertColour(100,150,200));
	showGraph(time,ga->maxTime,convertColour(255,255,40));
	showGraph(aFitness,maxpFitness,convertColour(200,200,200));
	showGraph(vFitness,maxpFitness,convertColour(100,100,100));
	showGraph(pFitness,maxpFitness,convertColour(255,40,40));

//	showForceListBack();

	dc->SetTextForeground(convertColour(255,0,0));
	dc->SetFont(font2);

        boWindow->Draw(dc);
	
	diagramWindow->Draw(dc);
	boGraphWindow->Draw(dc);
	
	adWindow->Draw(dc);

	showProgramGraph();


//calculate maximum height per facility
	for(s=0;s<MAX_LENGTH*50;s++)
		if(oldOrder[s]->blendTarget&&oldOrder[s]->facility&&(oldOrder[s]->forceFacilityCount-oldOrder[s]->availibleFacilityCount>height[oldOrder[s]->facility]))
			 height[oldOrder[s]->facility]=oldOrder[s]->forceFacilityCount-oldOrder[s]->availibleFacilityCount;
																			    
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
	faccount=1;
	for(s=0;s<MAX_LENGTH*50;s++)
		if(oldOrder[s]->blendTarget&&oldOrder[s]->facility)
		{
			for(i=1;i<faccount;i++)
				if(fac[i]==oldOrder[s]->facility) break;
			if(i==faccount)
			{
				fac[i]=oldOrder[s]->facility;
				faccount++;
			}
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
//sort the oldOrder after time
	for(s=0;s<MAX_LENGTH*50;s++)
		if(oldOrder[s]->marker)
			for(i=0;i<s;i++)
				if(oldOrder[i]->marker)
					if(oldOrder[s]->time<oldOrder[i]->time)
					{
						OLDORDER* temp=oldOrder[s];
						oldOrder[s]=oldOrder[i];
						oldOrder[i]=temp;
					}

// now calculate the rectangles :)
	int hoehe=0;
	dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
	for(s=0;s<MAX_LENGTH*50;s++)
		if(oldOrder[s]->blendTarget&&oldOrder[s]->facility)
			for(i=0;i<20;i++)
				if(bograph[i].type==oldOrder[s]->facility)
				{
//oldOrder[s]->time muesste vorsortiert sein
					for(j=0;j<MAX_LENGTH;j++)
       						if(unitCounter[bograph[i].type][j]<=oldOrder[s]->time)
						{
							unitCounter[bograph[i].type][j]=oldOrder[s]->time+stats[anarace->getPlayer()->getRace()][oldOrder[s]->unit].BT;
							hoehe=j;
       							j=MAX_LENGTH;
		 			       }
					oldOrder[s]->targetbx=((oldOrder[s]->time*boGraphWindow->getInnerWidth())/(ga->maxTime-anarace->getTimer()));
					oldOrder[s]->targetby=(i+1+hoehe/MIN_HEIGHT)*(FONT_SIZE+5)+(hoehe%MIN_HEIGHT)*(FONT_SIZE+4)/bograph[i].height;
																			    
					oldOrder[s]->targetbwidth=((stats[anarace->getPlayer()->getRace()][oldOrder[s]->unit].BT/*anarace->getProgramBT(s)*/*boGraphWindow->getInnerWidth())/(ga->maxTime-anarace->getTimer()));
					if(oldOrder[s]->targetbwidth<6) oldOrder[s]->targetbwidth=6;
					oldOrder[s]->targetbheight=(FONT_SIZE+4)/(bograph[i].height);
					if(oldOrder[s]->bonew)
					{
						oldOrder[s]->bonew=0;
						oldOrder[s]->by=oldOrder[s]->targetby;
						oldOrder[s]->bx=oldOrder[s]->targetbx;
						oldOrder[s]->bwidth=oldOrder[s]->targetbwidth;
						oldOrder[s]->bheight=oldOrder[s]->targetbheight;
					}
				}
																			    
// and the time steps on the top
	dc->SetTextForeground(convertColour(255,0,0));
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
  //                      dc->DrawLine(diagramWindow->getInnerLeftBound()+(i+timesteps)*(diagramWindow->getInnerWidth()/((ga->maxTime-anarace->getTimer())/30)),diagramWindow->getInnerUpperBound(),
    //                            diagramWindow->getInnerLeftBound()+(i+timesteps)*(diagramWindow->getInnerWidth()/((ga->maxTime-anarace->getTimer())/30)),diagramWindow->getInnerUpperBound()+FONT_SIZE+4);
                        dc->DrawText(_T(wxString::Format(wxT("%i:%i0"),i/2,3*(i%2))),diagramWindow->getInnerLeftBound()+5+i*((diagramWindow->getInnerWidth()-20)/((ga->maxTime-anarace->getTimer())/30)),diagramWindow->getInnerUpperBound());
                }
//TODO nochmal timesteps checken

// --------------------------------- END BUILD ORDER GRAPH ------------------------------

// now print the rectangles :)
	dc->SetPen(wxPen(convertColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));

	wxRect edge=wxRect();

	edge.height=FONT_SIZE+4;
	edge.x=boGraphWindow->getInnerLeftBound();
	edge.width=10000;//boGraphWindow->getInnerWidth();

	for(i=0;i<20;i++)
	if(bograph[i].type>0)
		for(j=i;j<i+bograph[i].lines;j++)
		{
			edge.y=(j+1)*(FONT_SIZE+5)+boGraphWindow->getInnerUpperBound();
			if(boGraphWindow->fitToClientArea(edge))
			{
				if(j%2==0)
					dc->SetBrush(wxBrush(convertColour(COLOR1G,COLOR1G,COLOR1B),wxSOLID));
				else dc->SetBrush(wxBrush(convertColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
	    		      	dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
			}
		}
	orderLength=0;
	for(i=0;i<MAX_LENGTH*50;i++)
		if(oldOrder[i]->blend)
		{
			if(oldOrder[i]->facility)
			{
				if((i+1)!=infoWindow)
				{
					edge.x=oldOrder[i]->bx+boGraphWindow->getInnerLeftBound();edge.y=oldOrder[i]->by+boGraphWindow->getInnerUpperBound();
					edge.width=oldOrder[i]->bwidth;edge.height=oldOrder[i]->bheight;
					if(boGraphWindow->fitToClientArea(edge))
					{
						dc->SetBrush(wxBrush(convertColour(
		((oldOrder[i]->code+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
		((oldOrder[i]->code+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
		((oldOrder[i]->code+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));	
						dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
					}
				}
			}

			edge.x=oldOrder[i]->x+boWindow->getInnerLeftBound();
                        edge.height=FONT_SIZE+4;
                        edge.width=oldOrder[i]->width;
			edge.y=oldOrder[i]->y-boWindow->getScrollY()+boWindow->getInnerUpperBound();
			if(boWindow->fitToClientArea(edge))
			{
				if((i+1)!=infoWindow)
				{
//					if(((oldOrder[i]->targety-3)/(FONT_SIZE+5))%2)
dc->SetBrush(wxBrush(convertColour(
(COLOR1R+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Red())*oldOrder[i]->blend/50,
(COLOR1G+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Green())*oldOrder[i]->blend/50,
(COLOR1B+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Blue())*oldOrder[i]->blend/50),wxSOLID));

//else dc->SetBrush(wxBrush(convertColour(
//(COLOR2R+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Red())*oldOrder[i]->blend/50,
//(COLOR2G+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Green())*oldOrder[i]->blend/50,
//(COLOR2B+BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Blue())*oldOrder[i]->blend/50),wxSOLID));

					dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
				        dc->SetTextForeground(BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type]);

					if(edge.width>=110)
					dc->DrawText(_T(wxString::Format(wxT("%i.%s"),oldOrder[i]->forceCount+1,stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].name)),edge.x,edge.y);
					if(edge.width>=140)
						dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->mins)),110+edge.x,edge.y);
                                        if(edge.width>=180)
						dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->gas)),150+edge.x,edge.y);
                                        if(edge.width>=230)
						dc->DrawText(_T(wxString::Format(wxT("%i/%i"),oldOrder[i]->needSupply,oldOrder[i]->haveSupply)),190+edge.x,edge.y);
                                        if(edge.width>=260)
						dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),oldOrder[i]->time/60,oldOrder[i]->time%60)),240+edge.x,edge.y);
	//      			dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->successType)),190+oldOrder[i]->x,oldOrder[i]->y);
//					}
				}
			}
			orderLength++;
		}
		if((infoWindow)&&(oldOrder[infoWindow-1]->blend))
		{
			wxRect edge=wxRect();
			i=infoWindow-1;
			edge.x=oldOrder[i]->bx+boGraphWindow->getInnerLeftBound();
			edge.y=oldOrder[i]->by+boGraphWindow->getInnerUpperBound();
			edge.height=oldOrder[i]->bheight;
			edge.width=oldOrder[i]->bwidth;
			if(boGraphWindow->fitToClientArea(edge))
			{
				dc->SetPen(wxPen(convertColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
				dc->SetBrush(wxBrush(convertColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));
	 			dc->DrawRoundedRectangle(edge.x-1,edge.y-1,edge.width+2,edge.height+2,4);
			}

			dc->SetTextForeground(convertColour(INFOWINDOWTEXTR,INFOWINDOWTEXTG,INFOWINDOWTEXTB));

                        edge.x=oldOrder[i]->x+boWindow->getInnerLeftBound();
                        edge.height=FONT_SIZE+4;
                        edge.width=oldOrder[i]->width;
                        edge.y=oldOrder[i]->y-boWindow->getScrollY()+boWindow->getInnerUpperBound();
                        if(boWindow->fitToClientArea(edge))
			{
				edge.y-=1;
				edge.x-=1;
				edge.height+=2;
				edge.width+=2;
				dc->SetPen(wxPen(convertColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
				dc->SetBrush(wxBrush(convertColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));
				dc->DrawRoundedRectangle(edge,4);
																		    
				dc->DrawText(_T(wxString::Format(wxT("%i.%s"),oldOrder[i]->forceCount+1,stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].name)),edge.x,edge.y);
				if(edge.width>=SECOND_COLOUMN-8)
				{
					dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->mins)),110+edge.x+1,edge.y+1);
					dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->gas)),150+edge.x+1,edge.y+1);
					dc->DrawText(_T(wxString::Format(wxT("%i/%i"),oldOrder[i]->needSupply,oldOrder[i]->haveSupply)),190+edge.x+1,edge.y+1);
					dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),oldOrder[i]->time/60,oldOrder[i]->time%60)),240+edge.x+1,edge.y+1);
				}
			}
			orderLength++;
		}
                boWindow->setMaxScrollY(orderLength*(FONT_SIZE+5));

/*
        if(mouseLeft)
        {
                mouseLeft=0;
                if(infoWindow)
                {
                        oldOrder[infoWindow]->blendTarget=0;
                        oldOrder[infoWindow]->targetx=-300;
                        oldOrder[infoWindow]->targetbx=900;
                        int there=1;
                        for(t=0;(t<MAX_LENGTH)&&there;t++)
                                if(anarace->getProgramIsBuilt(t))
                                        there&=(anarace->Marker[anarace->getProgramDominant(t)][t]+1!=oldOrder[infoWindow]->marker);
                        if(t<MAX_LENGTH)
                        {
                                for(i=t;i<MAX_LENGTH-1;i++)
                                {
                                        anarace->Code[0][i]=anarace->Code[0][i+1];
                                        anarace->Code[1][i]=anarace->Code[1][i+1];
                                        anarace->Marker[0][i]=anarace->Marker[0][i+1];
                                        anarace->Marker[1][i]=anarace->Marker[1][i+1];
                                }
                                                t=rand()%anarace->getPlayer()->goal->getMaxBuildTypes();
                                                PRERACE::markerCounter++;anarace->Marker[0][MAX_LENGTH-1]=PRERACE::markerCounter;
                                                PRERACE::markerCounter++;anarace->Marker[1][MAX_LENGTH-1]=PRERACE::markerCounter;
                                                anarace->Code[0][MAX_LENGTH-1]=t;
                                                t=rand()%anarace->getPlayer()->goal->getMaxBuildTypes();
                                                anarace->Code[1][MAX_LENGTH-1]=t;
                        }
                }

	}*/

		dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));

//finally print the legend
	for(i=0;i<20;i++)
		if(bograph[i].type>0)
			dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][bograph[i].type].name)),boGraphWindow->getInnerLeftBound(),boGraphWindow->getInnerUpperBound()+(1+i)*(FONT_SIZE+5));


	wxPoint Legend2=wxPoint(210+graphWindow->getInnerLeftBound(),graphWindow->getInnerUpperBound());
	wxPoint Legend3=wxPoint(180,FONT_SIZE+4);

	dc->SetBrush(wxBrush(convertColour(40,40,40),wxSOLID));
	dc->SetPen(wxPen(convertColour(80,80,80),1,wxSOLID));
															    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(40,255,255));
	dc->DrawText(_T(wxString::Format(wxT("Force : %i"),force[199]/100)),Legend2.x,Legend2.y);
	 
//	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+15,Legend3.x,Legend3.y,4);
//	dc->SetTextForeground(convertColour(255,255,40));
//	dc->DrawText(_T(wxString::Format(wxT("Time : %i"),time[199]/100)),Legend2.x,Legend2.y+15);
  
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+15,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(255,40,255));
	dc->DrawText(_T(wxString::Format(wxT("Tertiary fitness : %i"),tFitness[199]/100)),Legend2.x,Legend2.y+15);
 
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+30,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(40,255,40));
	dc->DrawText(_T(wxString::Format(wxT("Secondary fitness: %i"),sFitness[199]/10000)),Legend2.x,Legend2.y+30);
 	 
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+45,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(255,40,40));
	dc->DrawText(_T(wxString::Format(wxT("Primary fitness : %i"),pFitness[199]/100)),Legend2.x,Legend2.y+45);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+60,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(200,200,200));
	dc->DrawText(_T(wxString::Format(wxT("Fitness average : %i"),aFitness[199]/100)),Legend2.x,Legend2.y+60);
	
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+75,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(convertColour(100,100,100));
	dc->DrawText(_T(wxString::Format(wxT("Fitness variance : %i"),vFitness[199]/100)),Legend2.x,Legend2.y+75);

//	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+90,Legend3.x,Legend3.y,4);
//	dc->SetTextForeground(convertColour(100,150,200));
//	dc->DrawText(_T(wxString::Format(wxT("BuildOrder Length: %i"),length[199]/100)),Legend2.x,Legend2.y+90);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+90,Legend3.x,Legend3.y,4);
	dc->SetBrush(wxBrush(convertColour(40,40,100),wxSOLID));
	dc->DrawRoundedRectangle(Legend2.x+Legend3.x-(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations,Legend2.y+90,(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations ,Legend3.y,4);
	dc->SetTextForeground(convertColour(100,100,255));
	dc->DrawText(_T(wxString::Format(wxT("%i+ generations left"),(ga->maxGenerations-anarace->getUnchangedGenerations()))),Legend2.x,Legend2.y+90);

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
	int tlength=force[199]/100-5;

	haxor->Draw(dc);

        dc->SetTextForeground(convertColour(100,100,255));

	dc->DrawText(_T(wxString::Format(wxT("%i of %.0f billion possible build orders checked [%.6f%%]."),anarace->getGeneration()*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1),pow(tlength,anarace->getPlayer()->goal->getMaxBuildTypes())/1000000000.0,(anarace->getGeneration()*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1))/pow(tlength,anarace->getPlayer()->goal->getMaxBuildTypes()))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound());

	if(!run)
	dc->DrawText(_T(wxString::Format(wxT("%i ms per frame (%.2f fps)."),av,1000.0/((float)(av)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+20);
	else
	{
		dc->DrawText(_T(wxString::Format(wxT("%i ms per frame (%.2f fps) [%.2f bops]."),av,1000.0/((float)(av)),1000.0*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1)/((float)(av)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+20);

	        dc->DrawText(_T(wxString::Format(wxT("Estimated time using stupid brute-force algorithm: %.5f billion years."),/* (current time: %f [%f %% of total])."),*/
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
	}

	goalForceWindow->Draw(dc);
	forceWindow->Draw(dc);

	t=1;
	dc->SetTextForeground(convertColour(255,0,0));
	dc->SetPen(wxPen(convertColour(0,0,0),1,wxSOLID));
	dc->SetFont(font2);
//fuer jeden Balken ne Variable

//	dc->DrawText(_T("Units part of the goal list"),SECOND_COLOUMN+10,3);
	dc->SetFont(font);
	dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));

	int percent[UNIT_TYPE_COUNT];
	int counter[UNIT_TYPE_COUNT]; //number of different goals
	for(k=0;k<UNIT_TYPE_COUNT;k++) {percent[k]=0;counter[k]=0;}
	for(k=0;k<MAX_GOALS;k++)
		{
			percent[anarace->getPlayer()->goal->goal[k].unit]+=anarace->fitnessCode[k];
			counter[anarace->getPlayer()->goal->goal[k].unit]++;
		}
int worstTime[UNIT_TYPE_COUNT];
for(i=0;i<MAX_GOALS;i++)
	worstTime[i]=0;
for(i=0;i<MAX_GOALS;i++)
	if(anarace->getPlayer()->goal->goal[i].count>0)
		if(worstTime[anarace->getPlayer()->goal->goal[i].unit]<anarace->getFinalTime(i))
			worstTime[anarace->getPlayer()->goal->goal[i].unit]=anarace->getFinalTime(i);
	t=0;
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
			edge.x=goalForceWindow->getInnerLeftBound();;
			edge.width=SECOND_COLOUMN_WIDTH+20;
			edge.height=FONT_SIZE+5;
			edge.y=goalForceWindow->getInnerUpperBound()+h-goalForceWindow->getScrollY();
                         if(oldForceList[t]<anarace->getLocationForce(0,i)*100/maxUnitForce)
                                         oldForceList[t]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[t])/5+1;
                         else if(oldForceList[t]>anarace->getLocationForce(0,i)*100/maxUnitForce)
                                         oldForceList[t]-=(oldForceList[t]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;
			if(goalForceWindow->fitToClientArea(edge))
			{
	                        if(t%2==0)
        	                        dc->SetBrush(wxBrush(convertColour(COLOR1R,COLOR1G,COLOR1B),wxSOLID));
                	        else dc->SetBrush(wxBrush(convertColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));

				dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
	
	                        dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
				dc->DrawText(_T(wxString::Format(wxT("%s [%.2i:%.2i]"),stats[anarace->getPlayer()->getRace()][i].name,worstTime[i]/60,worstTime[i]%60)),edge.x+10,edge.y);
			   	dc->SetBrush(wxBrush(convertColour(((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));

				dc->DrawRoundedRectangle(edge.x+edge.width-1-oldForceList[t],edge.y,oldForceList[t]+1,FONT_SIZE+4,4);
				dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i))),edge.x+edge.width-40-oldForceList[t],edge.y);
				if((percent[i]/counter[i]<100)&&(percent[i]/counter[i]>0))
					dc->DrawText(_T(wxString::Format(wxT("%i%%"),percent[i]/counter[i])),edge.x+edge.width-30,edge.y);
			}
		t++;
	}
	goalForceWindow->setMaxScrollY(t*(FONT_SIZE+5));
	
//	dc->SetTextForeground(convertColour(255,0,0));
//	dc->SetFont(font2);
//	dc->DrawText(_T("Units not on the goal list"),SECOND_COLOUMN+10,t*(FONT_SIZE+5)+3);
	dc->SetFont(font);
	dc->SetTextForeground(convertColour(TEXT1R,TEXT1G,TEXT1B));
	t=0;
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

                        edge.x=forceWindow->getInnerLeftBound();;
                        edge.width=SECOND_COLOUMN_WIDTH+20;
                        edge.height=FONT_SIZE+5;
                        edge.y=forceWindow->getInnerUpperBound()+h-forceWindow->getScrollY();
                               if(oldForceList[t]<anarace->getLocationForce(0,i)*100/maxUnitForce)
                                        oldForceList[t]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[t])/5+1;
                                else if(oldForceList[t]>anarace->getLocationForce(0,i)*100/maxUnitForce)
                                        oldForceList[t]-=(oldForceList[t]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;
		        if(forceWindow->fitToClientArea(edge))
			{
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
        forceWindow->setMaxScrollY(t*(FONT_SIZE+5));


	if(infoWindow)
	{
		i=infoWindow-1;
		dc->SetTextForeground(convertColour(INFOWINDOWTEXTR,INFOWINDOWTEXTG,INFOWINDOWTEXTB));

		t=1;
		dc->SetPen(wxPen(convertColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
		dc->SetBrush(wxBrush(convertColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));	
		dc->DrawRoundedRectangle(infoWindowX,infoWindowY+(FONT_SIZE+5),250,5+8*(FONT_SIZE+5),4);
		
		dc->DrawText(_T(wxString::Format(wxT("Build %i. %s"),oldOrder[i]->forceCount+1,stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].name)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
		dc->DrawText(_T(wxString::Format(wxT("at %s"),anarace->getMap()->location[oldOrder[i]->location].getName())),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
		dc->DrawText(_T(wxString::Format(wxT("having %i minerals"),oldOrder[i]->mins)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
		      dc->DrawText(_T(wxString::Format(wxT("and %i gas"),oldOrder[i]->gas)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
		      dc->DrawText(_T(wxString::Format(wxT("and %i/%i supply"),oldOrder[i]->needSupply,oldOrder[i]->haveSupply)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
	      dc->DrawText(_T(wxString::Format(wxT("as soon as %s"),error_message[oldOrder[i]->successType])),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
	      if(oldOrder[i]->successUnit>0) {
		dc->DrawText(_T(wxString::Format(wxT("%s becomes availible."),stats[anarace->getPlayer()->getRace()][oldOrder[i]->successUnit].name)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;}


		      dc->DrawText(_T(wxString::Format(wxT("(time %.2i:%.2i)"),oldOrder[i]->time/60,oldOrder[i]->time%60)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
	}

                dc->SetFont(wxFont(32,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		if(run) ani++; else ani=1;
		if(ani>30) ani=1;
		int dx=0;int dy=0;
                dc->SetTextForeground(wxColour((0==ani%20)*200+((0==ani%19)+(0==ani%21))*50,(0==ani%20)*200+((0==ani%19)+(0==ani%21))*50,(0==ani%20)*200+((0==ani%19)+(0==ani%21))*100+50));
                dc->DrawText(_T("E"),timerWindow->getRightBound()+10,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("E",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%21)*200+((0==ani%20)+(0==ani%22))*50,(0==ani%21)*200+((0==ani%20)+(0==ani%22))*50,(0==ani%21)*200+((0==ani%20)+(0==ani%22))*100+50));
                dc->DrawText(_T("v"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("Ev",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%22)*200+((0==ani%21)+(0==ani%23))*50,(0==ani%22)*200+((0==ani%21)+(0==ani%23))*50,(0==ani%22)*200+((0==ani%21)+(0==ani%23))*100+50));
                dc->DrawText(_T("o"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("Evo",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%23)*200+((0==ani%22)+(0==ani%24))*50,(0==ani%23)*200+((0==ani%22)+(0==ani%24))*50,(0==ani%23)*200+((0==ani%22)+(0==ani%24))*100+50));
                dc->DrawText(_T("l"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("Evol",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%24)*200+((0==ani%23)+(0==ani%25))*50,(0==ani%24)*200+((0==ani%23)+(0==ani%25))*50,(0==ani%24)*200+((0==ani%23)+(0==ani%25))*100+50));
                dc->DrawText(_T("u"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("Evolu",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%25)*200+((0==ani%24)+(0==ani%26))*50,(0==ani%25)*200+((0==ani%24)+(0==ani%26))*50,(0==ani%25)*200+((0==ani%24)+(0==ani%26))*100+50));
                dc->DrawText(_T("t"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("Evolut",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%26)*200+((0==ani%25)+(0==ani%27))*50,(0==ani%26)*200+((0==ani%25)+(0==ani%27))*50,(0==ani%26)*200+((0==ani%25)+(0==ani%27))*100+50));
                dc->DrawText(_T("i"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("Evoluti",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%27)*200+((0==ani%26)+(0==ani%28))*50,(0==ani%27)*200+((0==ani%26)+(0==ani%28))*50,(0==ani%27)*200+((0==ani%26)+(0==ani%28))*100+50));
                dc->DrawText(_T("o"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);
                dc->GetTextExtent("Evolutio",&dx,&dy);

                dc->SetTextForeground(wxColour((0==ani%28)*200+((0==ani%27)+(0==ani%29))*50,(0==ani%28)*200+((0==ani%27)+(0==ani%29))*50,(0==ani%28)*200+((0==ani%27)+(0==ani%29))*100+50));
                dc->DrawText(_T("n"),timerWindow->getRightBound()+10+dx,mainWindow->getInnerUpperBound()+5);

                dc->SetTextForeground(wxColour(0,0,80));

                dc->DrawText(_T("Forge"),timerWindow->getRightBound()+50,mainWindow->getInnerUpperBound()+45);


	if(adWindow->insideClientArea(mouseX,mouseY))
	{
	        dc->SetTextForeground(wxColour(0,35,0));
	        dc->SetFont(wxFont(32,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
        	dc->DrawText(_T("Evolution"),adWindow->getInnerLeftBound()+80,adWindow->getInnerUpperBound()+15);
	        dc->DrawText(_T("Forge"),adWindow->getInnerLeftBound()+140,adWindow->getInnerUpperBound()+55);

		dc->DrawBitmap(bitmap2,adWindow->getInnerLeftBound(),adWindow->getInnerUpperBound()+3);
		dc->SetTextForeground(wxColour(230,0,0));
                dc->SetFont(wxFont(12,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		dc->DrawText(_T("The whole ClawSoftware Crew:"),adWindow->getInnerLeftBound()+80,adWindow->getInnerUpperBound()+15);
                dc->DrawText(_T("me. ;-)"),adWindow->getInnerLeftBound()+80,adWindow->getInnerUpperBound()+30);
		dc->DrawText(_T("visit www.clawsoftware.de 8-]"),adWindow->getInnerLeftBound()+110,adWindow->getInnerUpperBound()+80);
	}
	else
        dc->DrawBitmap(bitmap,adWindow->getInnerLeftBound(),adWindow->getInnerUpperBound()+3,true);

        if(endrun)
        {
                dc->SetFont(wxFont(32,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                                                                                                                                                            
                dc->SetTextForeground(wxColour(155*grey*(100-grey)/2500,0,0));
                dc->DrawText(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace->getRun(),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60)),27,203);
                                                                                                                                                            
                dc->SetTextForeground(wxColour(255*grey*(100-grey)/2500,0,0));
                dc->DrawText(_T(wxString::Format(wxT("Final time round %i: [%.2i:%.2i]"),anarace->getRun(),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60)),25,200);
                grey-=2;
                if(grey<=0) resetData();
        } else
        if(grey<100)
        {
                dc->SetTextForeground(wxColour(130*grey*(100-grey)/2500,0,0));
                dc->SetFont(wxFont(32,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                dc->DrawText(_T(wxString::Format(wxT("Round %i"),anarace->getRun()+1)),adWindow->getInnerLeftBound()+18,adWindow->getInnerUpperBound()+13);
                dc->SetFont(wxFont(12,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                dc->DrawText(_T(wxString::Format(wxT("%s"),gizmo[gizmor])),adWindow->getInnerLeftBound()+18,adWindow->getInnerUpperBound()+63);
                                                                                                                                                            
                dc->SetTextForeground(wxColour(230*grey*(100-grey)/2500,0,0));
                dc->SetFont(wxFont(32,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                dc->DrawText(_T(wxString::Format(wxT("Round %i"),anarace->getRun()+1)),adWindow->getInnerLeftBound()+15,adWindow->getInnerUpperBound()+10);
                dc->SetFont(wxFont(12,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                dc->DrawText(_T(wxString::Format(wxT("%s"),gizmo[gizmor])),adWindow->getInnerLeftBound()+15,adWindow->getInnerUpperBound()+60);
                                                                                                                                                            
                grey+=2;
        }



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

boanzahl=0;
for(i=0;i<MAX_LENGTH*50;i++)
	if(oldOrder[i]->blend)
		boanzahl++;

 for(i=0;i<MAX_LENGTH*50;i++)
                if(oldOrder[i]->blend)
                {
			k=2;
			while((i+1<MAX_LENGTH*50)&&((!oldOrder[i]->blend)||((stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type)&&(stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type==currentType))))
			{
				i++;
				if(oldOrder[i]->blend!=50) continue;
				k++;	
			}
			currentType=stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type;
			for(j=0;j<k;j++)
			{
	                	points1[j].y=(stringheight+j-1)*(mainWindow->getInnerHeight()+mainWindow->getInnerUpperBound()-graphWindow->getLowerBound()-20)/boanzahl+graphWindow->getLowerBound()+20;
        		        points2[j].y=(stringheight+j-1)*(mainWindow->getInnerHeight()+mainWindow->getInnerUpperBound()-graphWindow->getLowerBound()-20)/boanzahl+graphWindow->getLowerBound()+20;
        	        	points1[j].x=diagramWindow->getRightBound()+60+(int)(cos((float)((stringheight+j)+anarace->getGeneration())*5.0*3.1416/200.0)*50.0);
		                points2[j].x=diagramWindow->getRightBound()+60+(int)(sin(((float)((stringheight+j)+anarace->getGeneration())+13.0)*5.0*3.1416/200.0)*50.0);
                                points3[j].y=(stringheight+j-1)*(mainWindow->getInnerHeight()+mainWindow->getInnerUpperBound()-graphWindow->getLowerBound()-20)/boanzahl+graphWindow->getLowerBound()+20;
                                points4[j].y=(stringheight+j-1)*(mainWindow->getInnerHeight()+mainWindow->getInnerUpperBound()-graphWindow->getLowerBound()-20)/boanzahl+graphWindow->getLowerBound()+20;
                                points3[j].x=diagramWindow->getRightBound()+60+(int)(cos(((float)((stringheight+j)+anarace->getGeneration())+26.0)*5.0*3.1416/200.0)*50.0);
                                points4[j].x=diagramWindow->getRightBound()+60+(int)(sin(((float)((stringheight+j)+anarace->getGeneration())+39.0)*5.0*3.1416/200.0)*50.0);

				stringheight++;
			}
//			stringheight--;
			points1[stringheight].x=diagramWindow->getRightBound()+60+(int)(cos((float)((stringheight+j)+anarace->getGeneration())*5.0*3.1416/200.0)*50.0);
				
dc->SetPen(wxPen(convertColour(
(COLOR1R+2*BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Red())*oldOrder[i]->blend/50,
(COLOR1G+2*BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Green())*oldOrder[i]->blend/50,
(COLOR1B+2*BOcolor[stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].type].Blue())*oldOrder[i]->blend/50),3,wxSOLID));
			dc->DrawSpline(k,points1);
			dc->DrawSpline(k,points2);
                        dc->DrawSpline(k,points3);
                        dc->DrawSpline(k,points4);
		}

	dcp.Blit(0, 0, SCREEN_X, SCREEN_Y, dc, 0, 0);
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
			Refresh(false);
		};
	};
	Refresh(false);
}

void MyDCWindow::OnMouseScroll(wxMouseEvent& event)
{
	int msy=event.GetWheelRotation();
	boWindow->OnScrollMouse(msy);
	goalForceWindow->OnScrollMouse(msy);
	forceWindow->OnScrollMouse(msy);
}

void MyDCWindow::OnMouseMove(wxMouseEvent& event)
{
	event.GetPosition(&mouseX,&mouseY);
	boWindow->setMouse(mouseX,mouseY);
	goalForceWindow->setMouse(mouseX,mouseY);
	forceWindow->setMouse(mouseX,mouseY);
}

void MyDCWindow::OnMouseLeftDown(wxMouseEvent& event)
{
	boWindow->OnMouseLeftDown();
	goalForceWindow->OnMouseLeftDown();
	forceWindow->OnMouseLeftDown();
}

void MyDCWindow::OnMouseLeftUp(wxMouseEvent& event)
{
	boWindow->OnMouseLeftUp();
	goalForceWindow->OnMouseLeftUp();
	forceWindow->OnMouseLeftUp();
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



