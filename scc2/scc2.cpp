#include "../scc2dll/settings.h"
#include "../scc2dll/location.h"
#include "scc2.h"
#include "../scc2dll/prerace.h"
#include "../scc2dll/race.h"
#include "../scc2dll/anarace.h"
//#include <wx/tipdlg.h>
//#include "wx/progdlg.h"
//#include <wx/splash.h>
#include "math.h"

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(EC_Open,  MyFrame::OnOpen)
	EVT_MENU(EC_Start,  MyFrame::OnStart)
	EVT_MENU(EC_Stop,  MyFrame::OnStop)
	EVT_MENU(EC_Quit,  MyFrame::OnQuit)
	EVT_MENU(EC_About, MyFrame::OnAbout)
	EVT_MENU(EC_GeneralSettings, MyFrame::OnGeneralSettings)
	EVT_MENU(EC_GoalCreate,  MyFrame::OnGoalCreate)
	EVT_MENU(EC_GoalImport,  MyFrame::OnGoalImport)
	EVT_MENU(EC_MapCreate,  MyFrame::OnMapCreate)
	EVT_MENU(EC_MapImport, MyFrame::OnMapImport)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyDCWindow, wxScrolledWindow)
	EVT_ERASE_BACKGROUND(MyDCWindow::OnEraseBackground)
	EVT_IDLE(MyDCWindow::OnIdle)
	EVT_PAINT(MyDCWindow::OnPaint)
	EVT_MOTION(MyDCWindow::OnMouseMove)
END_EVENT_TABLE()

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
	MyFrame *frame = new MyFrame(_T("Evolution Chamber v1.07"), wxPoint(0, 0), wxSize(800, 600));
/*	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting map and goals"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up tables"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up status bar"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Initializing EC core"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Loading data files"),(progress+1)*100/max)));progress++;
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Starting main application"),(progress+1)*100/max)));progress++;*/

	frame->Show(TRUE);
//	if(bitmap) delete(bitmap);
//	if(splash) delete(splash);
	return TRUE;
}

MyDCWindow::MyDCWindow(wxFrame *parent) 
//	   : wxWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T("BLA"))
	:wxScrolledWindow(parent)
{
//	SetScrollbars(10,10,40,100,0,0);
	
//	bmpBack2.LoadFile("back2.bmp",wxBITMAP_TYPE_BMP);

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
	int i;
	for(i=1;i<settings.getMap(0)->getMaxPlayer();i++)
		settings.setGoal(0,i);
	update=0;
// initialize the soup, set the parameters, load the players etc.
	settings.initSoup();
	ga=settings.getGa();
	run=0;
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

	int k;
	for(k=0;k<20;k++)
	{
		oldTimeCounter[k]=0;
		oldTime[k]=ga->maxTime;
	}
	
	for(k=0;k<FORCE_LIST_NUMBER;k++)
		oldForceList[k]=0;

	for(k=0;k<MAX_LENGTH*50;k++)
	{
                oldOrder[k]=new OLDORDER;
		oldOrder[k]->blend=0;
		oldOrder[k]->blendTarget=0;
		oldOrder[k]->x=0;oldOrder[k]->y=0;
		oldOrder[k]->targetx=0;oldOrder[k]->targety=0;
		oldOrder[k]->dx=0;oldOrder[k]->dy=0;
		oldOrder[k]->unit=0;oldOrder[k]->marker=0;
		oldOrder[k]->forceCount=0;oldOrder[k]->successType=0;oldOrder[k]->successUnit=0;oldOrder[k]->location=0;
		oldOrder[k]->facility=0;oldOrder[k]->availibleFacilityCount=0;oldOrder[k]->forceFacilityCount=0;
		oldOrder[k]->code=0;
	
		oldOrder[k]->bx=0;oldOrder[k]->by=0;
		oldOrder[k]->targetbx=0;oldOrder[k]->targetby=0;

		oldOrder[k]->height=0;oldOrder[k]->width=0;
		oldOrder[k]->targetheight=0;oldOrder[k]->targetwidth=0;
	
		oldOrder[k]->bonew=0;
	}
	infoWindow=0;infoWindowX=0;infoWindowY=0;
}

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
	menuHelp->Append(EC_About, _T("&About...\tF1"), _T("Show about dialog"));
	menuFile->Append(EC_Open, _T("&Open\tAlt-O"), _T("Load Build Order"));
//evtl nen strich mit save/load
/*      menuFile->Append(EC_Open, _T("&Open\tAlt-O"), _T("Load Population"));
        menuFile->Append(EC_Open, _T("&Open\tAlt-O"), _T("Save Build Order"));
        menuFile->Append(EC_Open, _T("&Open\tAlt-O"), _T("Save Population"));*/
	menuFile->Append(EC_Start, _T("&Start\tAlt-S"), _T("Start/Continue calculation"));
	menuFile->Append(EC_Stop, _T("S&top\tAlt-T"), _T("Stop calculation"));
	menuFile->Append(EC_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
	menuSettings->Append(EC_GeneralSettings, _T("&General\tAlt-G"), _T("General settings"));
	menuGoal->Append(EC_GoalCreate, _T("C&reate\tAlt-R"),_T("Create new goal"));
//        menuGoal->Append(EC_GoalCreate, _T("C&reate\tAlt-R"),_T("Edit existing goal"));
	menuGoal->Append(EC_GoalImport, _T("&Import\tAlt-I"),_T("Import goal file"));
	menuMap->Append(EC_MapCreate, _T("&Create\tAlt-C"),_T("Create new map"));
//        menuGoal->Append(EC_GoalCreate, _T("C&reate\tAlt-R"),_T("Edit existing map"));
	menuMap->Append(EC_MapImport, _T("I&mport\tAlt-M"),_T("Import map file"));
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

	child=new MyDCWindow(this);
}

void MyDCWindow::showGraph(int* data,int max,wxColour col)
{
	int i,j;
	dc->SetPen(wxPen(col,1,wxSOLID));
	wxPoint blub[200];
	j=1;
	for(i=1;i<199;i++)
	{
		if(data[i]!=data[i+1])
		{
			blub[j]=wxPoint(i+THIRD_COLOUMN-5,190-data[i]/(max+1));
			j++;
		}
	}
	blub[0]=wxPoint(THIRD_COLOUMN-5,190-data[0]/(max+1));
	blub[j]=wxPoint(199+THIRD_COLOUMN-5,190-data[199]/(max+1));
	dc->DrawSpline(j+1,blub);
};

void MyDCWindow::showForceListBack()
{
	int j;

//      dc->DrawBitmap(bmpBack2,SECOND_COLOUMN-10,BUILD_ORDER_GRAPH_LENGTH,true);

        dc->SetPen(wxPen(wxColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
        dc->SetBrush(wxBrush(wxColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));
        dc->DrawRectangle(SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+10,500,53);

        dc->SetPen(wxPen(wxColour(INFOWINDOWPENR*3/4,INFOWINDOWPENG*3/4,INFOWINDOWPENB*3/4),1,wxSOLID));
	
	dc->DrawLine(SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+37,SECOND_COLOUMN+500,BUILD_ORDER_GRAPH_LENGTH+37);

        dc->DrawLine(SECOND_COLOUMN+125,BUILD_ORDER_GRAPH_LENGTH+10,SECOND_COLOUMN+125,BUILD_ORDER_GRAPH_LENGTH+63);
        dc->DrawLine(SECOND_COLOUMN+250,BUILD_ORDER_GRAPH_LENGTH+10,SECOND_COLOUMN+250,BUILD_ORDER_GRAPH_LENGTH+63);
        dc->DrawLine(SECOND_COLOUMN+375,BUILD_ORDER_GRAPH_LENGTH+10,SECOND_COLOUMN+375,BUILD_ORDER_GRAPH_LENGTH+63);


        dc->SetFont(font2);
        dc->SetTextForeground(wxColour(255,40,40));
	dc->DrawText(_T("Fitness"),SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+10);
        dc->SetTextForeground(wxColour(80,80,255));
        dc->DrawText(_T("Minerals"),SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+22);
        dc->SetTextForeground(wxColour(40,255,40));
        dc->DrawText(_T("Gas"),SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+34);
        dc->SetTextForeground(wxColour(255,0,0));

        dc->SetPen(wxPen(wxColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
        wxRect edge=wxRect();
        edge.x=0;
        edge.height=FONT_SIZE+4;
        edge.width=SECOND_COLOUMN-10-edge.x;
/*      for(j=0;j<BUILD_ORDER_NUMBER;j++)
        {
                edge.y=j*(FONT_SIZE+5)+3;
                if(j%2==0)
                        dc->SetBrush(b1);
                else dc->SetBrush(b2);
                dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
        };*/

/*        edge.height=FONT_SIZE+4;
        edge.x=SECOND_COLOUMN;
        edge.width=500;
                                                                                                       
        for(j=1;j<BUILD_ORDER_GRAPH_NUMBER;j++)
        {
                edge.y=j*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;
                if(j%2==0)
                        dc->SetBrush(wxBrush(wxColour(COLOR1G,COLOR1G,COLOR1B),wxSOLID));
                else dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
              dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
        }*/

}

void MyDCWindow::showTimer()
{
	int i,j,k;
        dc->DrawBitmap(bmpTimer,THIRD_COLOUMN-10,0,false);
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
                        dc->SetTextForeground(wxColour(160-oldTimeCounter[k]*8,0,0));
                        dc->SetFont(wxFont(30,wxMODERN,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                                                                                                       
                        dc->DrawText(_T(wxString::Format(wxT("[%.2i:%.2i]"),oldTime[k]/60,oldTime[k]%60)),THIRD_COLOUMN+5+oldTimeCounter[k],20-oldTimeCounter[k]/2);
                                                                                                       
                        oldTimeCounter[k]++;
                        if(oldTimeCounter[k]>20)
                        {
                                oldTimeCounter[k]=0;
                                oldTime[k]=0;
                        }
                }
                                                                                                       
        dc->SetFont(wxFont(30,wxMODERN,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
        dc->SetTextForeground(wxColour(255,0,0));
        dc->DrawText(_T(wxString::Format(wxT("[%.2i:%.2i]"),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60)),THIRD_COLOUMN,20);
}

void MyDCWindow::showProgramGraph()
{
	int s,y1;
        wxPoint mins[MAX_LENGTH];
        wxPoint gas[MAX_LENGTH];
        wxPoint fitness[MAX_LENGTH];
        int count=1;
                                                                                                       
        for(s=MAX_LENGTH;s--;) //~~ Max length
                if((anarace->getProgramIsBuilt(s)==1)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
                {
                        if(anarace->getProgramHaveMinerals(s)>50000) y1=50; else y1=anarace->getProgramHaveMinerals(s)/1000;
                        mins[count]=wxPoint(SECOND_COLOUMN+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer())),BUILD_ORDER_GRAPH_LENGTH+61-y1);
                        if(anarace->getProgramHaveGas(s)>50000) y1=50; else y1=anarace->getProgramHaveGas(s)/1000;
                        gas[count]=wxPoint(SECOND_COLOUMN+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer())),BUILD_ORDER_GRAPH_LENGTH+61-y1);
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!
                        y1=anarace->getProgramFitness(s)*50/(anarace->getMaxpFitness()-anarace->getTimer());
                        fitness[count]=wxPoint(SECOND_COLOUMN+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer())),BUILD_ORDER_GRAPH_LENGTH+61-y1);
                        count++;
                }
                                                                                                       
//TODO: Wenn !ready werden 30 Minuten dargestellt, aber im letzten Teil haben wir keine Aufzeichnungen ueber Mineralien etc. mehr :/
                                                                                                       
        mins[0]=wxPoint(SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+61-5);
        gas[0]=wxPoint(SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+61);
        fitness[0]=wxPoint(SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+61);
                                                                                                       
        if(anarace->getMins()>50000) y1=50; else y1=anarace->getMins()/1000;
        mins[count]=wxPoint(SECOND_COLOUMN+500,BUILD_ORDER_GRAPH_LENGTH+61-y1);
                                                                                                       
        if(anarace->getGas()>50000) y1=50; else y1=anarace->getGas()/1000;
        gas[count]=wxPoint(SECOND_COLOUMN+500,BUILD_ORDER_GRAPH_LENGTH+61-y1);
                                                                                                       
        //Problem: Falls Fitness nicht ganz erfuellt, ist es trotzdem ein schraeger Strich... impliziert eine Entwicklung O_o
        fitness[count]=wxPoint(SECOND_COLOUMN+500,BUILD_ORDER_GRAPH_LENGTH+61-50);
        count++;
                                                                                                       
        if(count>0)
        {
                dc->SetPen(wxPen(wxColour(80,80,255),2,wxSOLID));
                dc->DrawSpline(count,mins); //~~ Start End Punkt?
                dc->SetPen(wxPen(wxColour(40,255,40),2,wxSOLID));
                dc->DrawSpline(count,gas);
                dc->SetPen(wxPen(wxColour(255,40,40),2,wxSOLID));
                dc->DrawSpline(count,fitness);
                dc->SetPen(wxPen(wxColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
        }
}

void MyDCWindow::OnPaint(wxPaintEvent& event)
{
	int i,j,k,s,t,x1,x2,y1,y2;
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
	dc->SelectObject(wxBitmap(800,600));
	dc->SetBrush(wxBrush(wxColour(0,0,30),wxSOLID));
	dc->DrawRectangle(0,0,800,600);

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

//        	dc->SetTextForeground(wxColour(255,0,0));
	        for(s=0;s<MAX_LENGTH*50;s++)
//              if((oldOrder[s]->moveCounter)||(oldOrder[s]->blendCounter))
        	        if(oldOrder[s]->blendTarget==50)
                	{
	                        int notthere=1;
        	                for(t=0;(t<MAX_LENGTH)&&(notthere);t++)
                	                if(anarace->getProgramIsBuilt(t))
                        	                notthere&=(anarace->Marker[anarace->getProgramDominant(t)][t]!=oldOrder[s]->marker);
	                        if(notthere) //loeschen
        	                {
                	                oldOrder[s]->blendTarget=0;
                        	        oldOrder[s]->targetx=-300;
					oldOrder[s]->targetbx=900;
        	                }
                	}
                                                                                                                                                            
		k=0;
	        for(s=MAX_LENGTH;s--;)
        	        if(anarace->getProgramIsBuilt(s)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
                	{
	                        int notthere=1;
        	                for(t=0;(t<MAX_LENGTH*50)&&(notthere);t++)
                                	notthere&=(oldOrder[t]->marker!=anarace->Marker[anarace->getProgramDominant(s)][s]);
	                        if(notthere) //neues erstellen
        	                {
                	                t=0;
					while(oldOrder[t]->marker) t++;
	                                oldOrder[t]->x=-300;oldOrder[t]->y=(k+1)*(FONT_SIZE+5)+3;
        	                        oldOrder[t]->dx=0;oldOrder[t]->dy=0;
                	                oldOrder[t]->blend=1;
                        	        oldOrder[t]->unit=anarace->phaenoCode[s];
                                	oldOrder[t]->marker=anarace->Marker[anarace->getProgramDominant(s)][s];
					oldOrder[t]->bonew=1;
        	                } else //altes anpassen
                		        t=t-1;
/*                                if((oldOrder[t]->targety!=(k+1)*(FONT_SIZE+5)+3)&&(oldOrder[t]->x==oldOrder[t]->targetx))
                                {
                                        if(oldOrder[t]->dx<(oldOrder[t]->targety-oldOrder[t]->y)/10)  oldOrder[t]->dx+=(oldOrder[t]->targety-oldOrder[t]->y)/10;
                                        else oldOrder[t]->dx=(oldOrder[t]->targety-oldOrder[t]->y)/5;
                                }*/
                                oldOrder[t]->targetx=5;oldOrder[t]->targety=(k+1)*(FONT_SIZE+5)+3;
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
                                                                                                                                                            
	        for(i=0;i<MAX_LENGTH*50;i++)
        	        if(oldOrder[i]->marker)
	                {
        	                oldOrder[i]->dx+=(oldOrder[i]->targetx-oldOrder[i]->x-oldOrder[i]->dx)/30;oldOrder[i]->x+=(oldOrder[i]->targetx-oldOrder[i]->x)/10+oldOrder[i]->dx;
                	        oldOrder[i]->dy+=(oldOrder[i]->targety-oldOrder[i]->y-oldOrder[i]->dy)/30;oldOrder[i]->y+=(oldOrder[i]->targety-oldOrder[i]->y)/10+oldOrder[i]->dy;
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
	
        	                if((oldOrder[i]->targetheight-oldOrder[i]->height)/5==0)
                	        {
                        	        if(oldOrder[i]->targetheight>oldOrder[i]->height)
                                	        oldOrder[i]->height++;
	                                else if(oldOrder[i]->targetheight<oldOrder[i]->height)
        	                                oldOrder[i]->height--;
                	        }
				else
		                        oldOrder[i]->height+=(oldOrder[i]->targetheight-oldOrder[i]->height)/5;

                	        oldOrder[i]->blend+=(oldOrder[i]->blendTarget-oldOrder[i]->blend)/10;
	//                      oldOrder[i]->blend=oldOrder[i]->blendTarget;oldOrder[i]->x=oldOrder[i]->targetx;oldOrder[i]->y=oldOrder[i]->targety;
        	                if((oldOrder[i]->blendTarget==0)&&(oldOrder[i]->blend<10)) oldOrder[i]->blend=0; //~~
				if((oldOrder[i]->blendTarget==50)&&(oldOrder[i]->blend>40)) oldOrder[i]->blend=50;
			} 
	}//update==2 end
	update=1;

	showTimer();

//	dc->SetFont(font);
//	dc->DrawBitmap(bmpGraph,THIRD_COLOUMN-10,100,true);

	dc->SetPen(wxPen(wxColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
	dc->SetBrush(wxBrush(wxColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));
	dc->DrawRectangle(THIRD_COLOUMN-5,90,200,100);

	dc->DrawLine(THIRD_COLOUMN-5,123,THIRD_COLOUMN+195,123);
        dc->DrawLine(THIRD_COLOUMN-5,157,THIRD_COLOUMN+195,157);

        dc->DrawLine(THIRD_COLOUMN+62,90,THIRD_COLOUMN+62,190);
        dc->DrawLine(THIRD_COLOUMN+129,90,THIRD_COLOUMN+129,190);

        dc->SetPen(wxPen(wxColour(INFOWINDOWPENR*3/4,INFOWINDOWPENG*3/4,INFOWINDOWPENB*3/4),1,wxSOLID));


	showGraph(tFitness,maxtFitness-mintFitness,wxColour(255,40,255));
	showGraph(sFitness,maxsFitness,wxColour(40,255,40));
	showGraph(force,maxForce,wxColour(40,255,255));
	showGraph(length,MAX_LENGTH,wxColour(100,150,200));
	showGraph(time,ga->maxTime,wxColour(255,255,40));
	showGraph(aFitness,maxpFitness,wxColour(200,200,200));
	showGraph(vFitness,maxpFitness,wxColour(100,100,100));
	showGraph(pFitness,maxpFitness,wxColour(255,40,40));

	showForceListBack();

        dc->SetFont(font2);
	dc->DrawText(_T("Name"),0,3);
	dc->DrawText(_T("Mins"),110,3);
	dc->DrawText(_T("Gas"),150,3);
	dc->DrawText(_T("Supply"),190,3);
	dc->DrawText(_T("Time"),240,3);

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
	dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
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
                        	        oldOrder[s]->targetbx=SECOND_COLOUMN+((oldOrder[s]->time*500)/(ga->maxTime-anarace->getTimer()));
	                                oldOrder[s]->targetby=(i+1+hoehe/*[bograph[i].type]*//MIN_HEIGHT)*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH+(hoehe/*[bograph[i].type]*/%MIN_HEIGHT)*(FONT_SIZE+4)/bograph[i].height;
                                                                                                                                                            
        	                        oldOrder[s]->targetwidth=((stats[anarace->getPlayer()->getRace()][oldOrder[s]->unit].BT/*anarace->getProgramBT(s)*/*500)/(ga->maxTime-anarace->getTimer()));
                	                if(oldOrder[s]->targetwidth<6) oldOrder[s]->targetwidth=6;
                        	        oldOrder[s]->targetheight=(FONT_SIZE+4)/(bograph[i].height);
                                	if(oldOrder[s]->bonew)
	                                {
        	                                oldOrder[s]->bonew=0;
                	                        oldOrder[s]->by=oldOrder[s]->targetby;
                        	                oldOrder[s]->bx=850;
                                	        oldOrder[s]->width=oldOrder[s]->targetwidth;
                                        	oldOrder[s]->height=oldOrder[s]->targetheight;
		                        }
                	        }
                                                                                                                                                            
// --------------------- BUILD ORDER GRAPH START ----------------------
/*
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
for(s=MAX_LENGTH;s--;)
	if(anarace->getProgramIsBuilt(s)&&anarace->getProgramFacility(s)&&(anarace->getProgramForceCount(s,anarace->getProgramFacility(s))-anarace->getProgramAvailibleCount(s,anarace->getProgramFacility(s))>height[anarace->getProgramFacility(s)]))
		height[anarace->getProgramFacility(s)]=anarace->getProgramForceCount(s,anarace->getProgramFacility(s))-anarace->getProgramAvailibleCount(s,anarace->getProgramFacility(s));
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
for(s=MAX_LENGTH;s--;)
	if(anarace->getProgramIsBuilt(s)&&anarace->getProgramFacility(s))
	{
		for(i=1;i<faccount;i++)
			if(fac[i]==anarace->getProgramFacility(s)) break;
		if(i==faccount)
		{
			fac[i]=anarace->getProgramFacility(s);
			faccount++;
		}
	}
// ...and sort them
for(i=0;i<20;i++)
	if(fac[i])
		for(j=0;j<i;j++)
			if(fac[i]<fac[j])
			{
				int temp=fac[i];fac[i]=fac[j];fac[j]=temp;
			}
// now put all together
position=0;
for(i=0;i<20;i++)
{
	bograph[position].type=fac[i];
	bograph[position].lines=lines[fac[i]];
	bograph[position].height=height[fac[i]];
	position+=lines[fac[i]];
}

// now print the rectangles :)
/*hoehe=0;
dc->SetTextForeground(wxColour(255,0,0));
for(s=MAX_LENGTH;s--;)
	if(anarace->getProgramIsBuilt(s)&&(anarace->getProgramFacility(s)))
		for(i=0;i<20;i++)
			if(bograph[i].type==anarace->getProgramFacility(s))
			{
                                for(j=0;j<MAX_LENGTH;j++)
                                        if(unitCounter[bograph[i].type][j]<=anarace->getProgramTime(s))
                                        {
                                                unitCounter[bograph[i].type][j]=anarace->getProgramTime(s)+stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[s]].BT;
//                                                hoehe[bograph[i].type]=j;
						hoehe=j;
                                                j=MAX_LENGTH;
                                        }
				x1=SECOND_COLOUMN+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer()));
				y1=(i+1+hoehe/MIN_HEIGHT)*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH+(hoehe%MIN_HEIGHT)*(FONT_SIZE+4)/bograph[i].height;

				x2=((stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[s]].BT*500)/(ga->maxTime-anarace->getTimer()));
				if(x2<6) x2=6;
				y2=(FONT_SIZE+4)/(bograph[i].height);
				dc->SetBrush(wxBrush(wxColour(anarace->getProgramCode(s)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,((anarace->getProgramCode(s)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,((anarace->getProgramCode(s)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256)),wxSOLID));
				dc->DrawRoundedRectangle(x1,y1,x2,y2,4);				

			}
//finally print the legend
for(i=0;i<20;i++)
	if(bograph[i].type>0)
		dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][bograph[i].type].name)),SECOND_COLOUMN,(1+i)*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH);*/

// and the time steps on the top
        dc->SetTextForeground(wxColour(255,0,0));
	dc->SetPen(wxPen(wxColour(200,0,0),1,wxSOLID));
        int timesteps=((ga->maxTime-anarace->getTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
        for(i=0;i<(ga->maxTime-anarace->getTimer())/30;i++)
                if(i%timesteps==0)
                {
                        dc->DrawLine(SECOND_COLOUMN+(i+timesteps)*(500/((ga->maxTime-anarace->getTimer())/30)),FORCE_LIST_LENGTH+3,
                                SECOND_COLOUMN+(i+timesteps)*(500/((ga->maxTime-anarace->getTimer())/30)),FORCE_LIST_LENGTH+3+FONT_SIZE+4);
                        dc->DrawText(_T(wxString::Format(wxT("%i:%i0"),i/2,3*(i%2))),SECOND_COLOUMN+5+i*(500/((ga->maxTime-anarace->getTimer())/30)),FORCE_LIST_LENGTH+3);
                }

// --------------------------------- END BUILD ORDER GRAPH ------------------------------

// now print the rectangles :)
	dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
        dc->SetPen(wxPen(wxColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));

	wxRect edge=wxRect();

        edge.height=FONT_SIZE+4;
        edge.x=SECOND_COLOUMN;
        edge.width=500;

        for(i=0;i<20;i++)
	if(bograph[i].type>0)
		for(j=i;j<i+bograph[i].lines;j++)
		{
	                edge.y=(j+1)*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;
        	        if(j%2==0)
                	        dc->SetBrush(wxBrush(wxColour(COLOR1G,COLOR1G,COLOR1B),wxSOLID));
	                else dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
        	      dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
		}

	for(i=0;i<MAX_LENGTH*50;i++)
		if(oldOrder[i]->blend)
		{
			if(oldOrder[i]->facility)
                        {
				if(i!=infoWindow)
				{
					dc->SetBrush(wxBrush(wxColour(
	((oldOrder[i]->code+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
	((oldOrder[i]->code+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
	((oldOrder[i]->code+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));	
	                                dc->DrawRoundedRectangle(oldOrder[i]->bx,oldOrder[i]->by,oldOrder[i]->width,oldOrder[i]->height,4);
				}
                        }

		        edge.y=oldOrder[i]->y;
			if(edge.y<=BUILD_ORDER_NUMBER*(FONT_SIZE+5)+3)
			{
				if(i!=infoWindow)
				{
		                        edge.x=oldOrder[i]->x;
        		                edge.height=FONT_SIZE+4;
                		        edge.width=SECOND_COLOUMN-10;
					if(((oldOrder[i]->targety-3)/(FONT_SIZE+5))%2)
dc->SetBrush(wxBrush(wxColour(
COLOR1R*oldOrder[i]->blend/50,
COLOR1G*oldOrder[i]->blend/50,
COLOR1B*oldOrder[i]->blend/50),wxSOLID));
else dc->SetBrush(wxBrush(wxColour(
COLOR2R*oldOrder[i]->blend/50,
COLOR2G*oldOrder[i]->blend/50,
COLOR2B*oldOrder[i]->blend/50),wxSOLID));

		        		dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
					dc->DrawText(_T(wxString::Format(wxT("%i.%s"),oldOrder[i]->forceCount+1,stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].name)),oldOrder[i]->x,oldOrder[i]->y);
		                        dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->mins)),110+oldOrder[i]->x,oldOrder[i]->y);
			                dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->gas)),150+oldOrder[i]->x,oldOrder[i]->y);
        		        	dc->DrawText(_T(wxString::Format(wxT("%i/%i"),oldOrder[i]->needSupply,oldOrder[i]->haveSupply)),190+oldOrder[i]->x,oldOrder[i]->y);
					dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),oldOrder[i]->time/60,oldOrder[i]->time%60)),240+oldOrder[i]->x,oldOrder[i]->y);
	//      	        	dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->successType)),190+oldOrder[i]->x,oldOrder[i]->y);
				}
			}
		}
		if((infoWindow)&&(oldOrder[infoWindow]->blend))
		{
			i=infoWindow;
                                        dc->SetPen(wxPen(wxColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
                                        dc->SetBrush(wxBrush(wxColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));
                 	                dc->DrawRoundedRectangle(oldOrder[i]->bx-1,oldOrder[i]->by-1,oldOrder[i]->width+2,oldOrder[i]->height+2,4);

                                        dc->SetTextForeground(wxColour(INFOWINDOWTEXTR,INFOWINDOWTEXTG,INFOWINDOWTEXTB));
                        wxRect edge=wxRect();
                                                                                                                                                            
                        edge.y=oldOrder[i]->y;
                        if(edge.y<=BUILD_ORDER_NUMBER*(FONT_SIZE+5)+3)
			{
                                                                                                                            
                                        edge.y-=1;
                                        edge.x=oldOrder[i]->x-1;
                                        edge.height=FONT_SIZE+6;
                                        edge.width=SECOND_COLOUMN-8;
                                        dc->SetPen(wxPen(wxColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
                                        dc->SetBrush(wxBrush(wxColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));
                                        dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
                                                                                                                                                            
                                        dc->DrawText(_T(wxString::Format(wxT("%i.%s"),oldOrder[i]->forceCount+1,stats[anarace->getPlayer()->getRace()][oldOrder[i]->unit].name)),edge.x,edge.y);
                                        dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->mins)),110+edge.x+1,edge.y+1);
                                        dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->gas)),150+edge.x+1,edge.y+1);
                                        dc->DrawText(_T(wxString::Format(wxT("%i/%i"),oldOrder[i]->needSupply,oldOrder[i]->haveSupply)),190+edge.x+1,edge.y+1);
                                        dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),oldOrder[i]->time/60,oldOrder[i]->time%60)),240+edge.x+1,edge.y+1);
                //                      dc->DrawText(_T(wxString::Format(wxT("%i"),oldOrder[i]->successType)),190+oldOrder[i]->x,oldOrder[i]->y);
			}
		}
                dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));

//finally print the legend
	for(i=0;i<20;i++)
        	if(bograph[i].type>0)
                	dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][bograph[i].type].name)),SECOND_COLOUMN,(1+i)*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH);


/*        dc->SetTextForeground(wxColour(255,0,0));

	t=0;
	for(s=MAX_LENGTH;s--;)
		if((anarace->getProgramIsBuilt(s)==1)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*//*) //TODO Zeit rausnehmen?
		{
			if(bolog[s].order==anarace->phaenoCode[s])
			{
				if(bolog[s].count<160)
					bolog[s].count+=3;
				else bolog[s].count=160;
			} else
			{
				bolog[s].count=0;
				bolog[s].order=anarace->phaenoCode[s];
			}
//			dc->SetTextForeground(convertColor(bolog[s].count));
//                        dc->SetTextForeground(wxColour((anarace->Marker[anarace->getProgramDominant(s)][s]/65536)%256,(anarace->Marker[anarace->getProgramDominant(s)][s]/256)%256,anarace->Marker[anarace->getProgramDominant(s)][s]%256));

			int h=(t+1)*(FONT_SIZE+5)+3;
			dc->DrawText(_T(wxString::Format(wxT("%i.%s"),anarace->getProgramForceCount(s,anarace->phaenoCode[s])+1,stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[s]].name)),5,h);
//			dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramHaveMinerals(s)/100)),110,h);
//			dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramFacility(s))),110,h);
//			dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramHaveGas(s)/100)),150,h);
                        dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->Marker[anarace->getProgramDominant(s)][s])),240,h);

//			dc->DrawText(_T(wxString::Format(wxT("%i/%i"),anarace->getProgramNeedSupply(s),anarace->getProgramHaveSupply(s))),190,h);
//			dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),anarace->getProgramTime(s)/60,anarace->getProgramTime(s)%60)),240,h);
		/*dc->DrawText(_T(wxString::Format(wxT("%s"),anarace->getMap()->location[anarace->getProgramLocation(s)].getName())),310,h);*/
//		dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramSuccessType(s))),240,h);
/*		dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->getProgramSuccessUnit(s)].name)),530,h);*/
//			t++;
//		}

	wxPoint Legend2=wxPoint(THIRD_COLOUMN-10,195);
	wxPoint Legend3=wxPoint(180,FONT_SIZE+4);

	dc->SetBrush(wxBrush(wxColour(40,40,40),wxSOLID));
	dc->SetPen(wxPen(wxColour(80,80,80),1,wxSOLID));
															    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(40,255,255));
	dc->DrawText(_T(wxString::Format(wxT("Force : %i"),force[199]/100)),Legend2.x,Legend2.y);
	 
//	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+15,Legend3.x,Legend3.y,4);
//	dc->SetTextForeground(wxColour(255,255,40));
//	dc->DrawText(_T(wxString::Format(wxT("Time : %i"),time[199]/100)),Legend2.x,Legend2.y+15);
  
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+15,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(255,40,255));
	dc->DrawText(_T(wxString::Format(wxT("Tertiary fitness : %i"),tFitness[199]/100)),Legend2.x,Legend2.y+15);
 
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+30,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(40,255,40));
	dc->DrawText(_T(wxString::Format(wxT("Secondary fitness: %i"),sFitness[199]/10000)),Legend2.x,Legend2.y+30);
 	 
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+45,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(255,40,40));
	dc->DrawText(_T(wxString::Format(wxT("Primary fitness : %i"),pFitness[199]/100)),Legend2.x,Legend2.y+45);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+60,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(200,200,200));
	dc->DrawText(_T(wxString::Format(wxT("Fitness average : %i"),aFitness[199]/100)),Legend2.x,Legend2.y+60);
	
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+75,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(100,100,100));
	dc->DrawText(_T(wxString::Format(wxT("Fitness variance : %i"),vFitness[199]/100)),Legend2.x,Legend2.y+75);

//	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+90,Legend3.x,Legend3.y,4);
//	dc->SetTextForeground(wxColour(100,150,200));
//	dc->DrawText(_T(wxString::Format(wxT("BuildOrder Length: %i"),length[199]/100)),Legend2.x,Legend2.y+90);

	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+90,Legend3.x,Legend3.y,4);
	dc->SetBrush(wxBrush(wxColour(40,40,100),wxSOLID));
	dc->DrawRoundedRectangle(Legend2.x+Legend3.x-(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations,Legend2.y+90,(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations ,Legend3.y,4);
	dc->SetTextForeground(wxColour(100,100,255));
	dc->DrawText(_T(wxString::Format(wxT("%i+ generations left"),(ga->maxGenerations-anarace->getUnchangedGenerations()))),Legend2.x,Legend2.y+90);

	t=1;
	dc->SetTextForeground(wxColour(255,0,0));
	dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
	dc->SetFont(font2);
//fuer jeden Balken ne Variable

        edge.x=SECOND_COLOUMN;
        edge.width=SECOND_COLOUMN_WIDTH;
	edge.height=FONT_SIZE+4;
	dc->DrawText(_T("Units part of the goal list"),SECOND_COLOUMN+10,3);
	dc->SetFont(font);
	dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));

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
			int h=t*(FONT_SIZE+5)+3;
			dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
//((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
  //			    ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
    //			  ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->g  oal->getMaxBuildTypes()/8))%256));
//			if((worstTime[i]>0)&&(worstTime[i]<ga->maxTime))
        edge.x=SECOND_COLOUMN;
        edge.width=SECOND_COLOUMN_WIDTH;
        edge.y=h;
        if(t%2==0)
                dc->SetBrush(wxBrush(wxColour(COLOR1R,COLOR1G,COLOR1B),wxSOLID));
                else dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
        dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);

				dc->DrawText(_T(wxString::Format(wxT("%s [%.2i:%.2i]"),stats[anarace->getPlayer()->getRace()][i].name,worstTime[i]/60,worstTime[i]%60)),SECOND_COLOUMN+10,h);
//			dc->SetBrush(wxBrush(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count),wxSOLID));
	   		dc->SetBrush(wxBrush(wxColour(((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));

			if(oldForceList[t]<anarace->getLocationForce(0,i)*100/maxUnitForce) oldForceList[t]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[t])/5+1;
			else if(oldForceList[t]>anarace->getLocationForce(0,i)*100/maxUnitForce) oldForceList[t]-=(oldForceList[t]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;

			dc->DrawRoundedRectangle(SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-1-oldForceList[t],h,oldForceList[t]+1,FONT_SIZE+4,4);
			//dc->SetTextForeground(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count));
			dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i))),SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-40-oldForceList[t],h);
			if((percent[i]/counter[i]<100)&&(percent[i]/counter[i]>0))
			dc->DrawText(_T(wxString::Format(wxT("%i%%"),percent[i]/counter[i])),SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-30,h);

			t++;
		}
	t++;
	dc->SetTextForeground(wxColour(255,0,0));
	dc->SetFont(font2);
	dc->DrawText(_T("Units not on the goal list"),SECOND_COLOUMN+10,t*(FONT_SIZE+5)+3);
	dc->SetFont(font);
	dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
	t++;
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
	
//			dc->SetTextForeground(wxColour(0,0,0));
/*((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
			      ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
			      ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256));*/
        edge.x=SECOND_COLOUMN;
        edge.width=SECOND_COLOUMN_WIDTH;
        edge.y=h;
        if(t%2==0)
                dc->SetBrush(wxBrush(wxColour(COLOR1R,COLOR1G,COLOR1B),wxSOLID));
                else dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
        dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);

			dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),SECOND_COLOUMN+10,h);
//			dc->SetBrush(wxBrush(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count),wxSOLID));
			dc->SetBrush(wxBrush(wxColour(   ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));
			if(oldForceList[t]<anarace->getLocationForce(0,i)*100/maxUnitForce) oldForceList[t]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[t])/5+1;
			else if(oldForceList[t]>anarace->getLocationForce(0,i)*100/maxUnitForce) oldForceList[t]-=(oldForceList[t]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;

			dc->DrawRoundedRectangle(SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-1-oldForceList[t],h,oldForceList[t]+1,FONT_SIZE+4,4);
//			dc->SetTextForeground(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count));
			dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i))),SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-30-oldForceList[t],h);
			t++;
		}

	if(infoWindow)
	{
	        dc->SetTextForeground(wxColour(INFOWINDOWTEXTR,INFOWINDOWTEXTG,INFOWINDOWTEXTB));

		t=1;
		dc->SetPen(wxPen(wxColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
		dc->SetBrush(wxBrush(wxColour(INFOWINDOWR,INFOWINDOWG,INFOWINDOWB),wxSOLID));	
		dc->DrawRoundedRectangle(infoWindowX,infoWindowY+(FONT_SIZE+5),250,5+8*(FONT_SIZE+5),4);
		
		dc->DrawText(_T(wxString::Format(wxT("Build %i. %s"),oldOrder[infoWindow]->forceCount+1,stats[anarace->getPlayer()->getRace()][oldOrder[infoWindow]->unit].name)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
		dc->DrawText(_T(wxString::Format(wxT("at %s"),anarace->getMap()->location[oldOrder[infoWindow]->location].getName())),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
                dc->DrawText(_T(wxString::Format(wxT("having %i minerals"),oldOrder[infoWindow]->mins)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
                      dc->DrawText(_T(wxString::Format(wxT("and %i gas"),oldOrder[infoWindow]->gas)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
                      dc->DrawText(_T(wxString::Format(wxT("and %i/%i supply"),oldOrder[infoWindow]->needSupply,oldOrder[infoWindow]->haveSupply)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
              dc->DrawText(_T(wxString::Format(wxT("as soon as %s"),error_message[oldOrder[infoWindow]->successType])),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
              if(oldOrder[infoWindow]->successUnit>0) {
		dc->DrawText(_T(wxString::Format(wxT("%s becomes availible."),stats[anarace->getPlayer()->getRace()][oldOrder[infoWindow]->successUnit].name)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;}


                      dc->DrawText(_T(wxString::Format(wxT("(time %.2i:%.2i)"),oldOrder[infoWindow]->time/60,oldOrder[infoWindow]->time%60)),infoWindowX+5,infoWindowY+5+t*(FONT_SIZE+5));t++;
	}
	dcp.Blit(0, 0, 800, 600, dc, 0, 0);
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
	ANARACE* temp;
	if((run==1)&&(temp=settings.newGeneration()))
	{
		anarace=temp;
		//SetStatusText(_T(wxString::Format(wxT("%i runs and %i+ generations remaining. [Total %i generations]"),ga->maxRuns-anarace->getRun(),ga->maxGenerations-anarace->getUnchangedGenerations(),anarace->getGeneration())));
		update=2;
		Refresh(false);
	}
//	Refresh(true);
}

void MyDCWindow::OnMouseMove(wxMouseEvent& event)
{
	int x,y,i;
	infoWindow=0;
	event.GetPosition(&x,&y);
	int fenster=1;
	for(i=0;(i<MAX_LENGTH*50)&&(fenster);i++)
		if(oldOrder[i]->blend) 
		{	
	                if(((x<=SECOND_COLOUMN-10)&&(x>=0)&&(y<=oldOrder[i]->y+(FONT_SIZE+5))&&(y>=oldOrder[i]->y))||((x>=oldOrder[i]->bx)&&(x<=oldOrder[i]->bx+oldOrder[i]->width)&&(y>=oldOrder[i]->by)&&(y<=oldOrder[i]->by+oldOrder[i]->height)))
			{
				infoWindow=i;
				infoWindowX=x+10;infoWindowY=y;
				fenster=0;
			}
		}
	if(fenster)
	{
//		if((x<SECOND_COLOUMN-10)&&(x>=0)&&()
		//position pruefen ob im Bereich der bos, rechte Maustaste => einfuegen
	}
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
//    wxString path;
//  if ( (fileDialog.ShowModal() == wxID_OK )&&(replay.Load(path=fileDialog.GetPath())))
//	SetStatusText(_T(wxString::Format(wxT("%s [%i]\r\n"), replay.m_oHeader.getGameName(),replay.m_oActions.GetActionCount())));
}

void MyFrame::OnGoalCreate(wxCommandEvent& event)
{
	menuGoal->Enable(EC_GoalCreate,false);
	menuGoal->Enable(EC_GoalCreate,true);
};
void MyFrame::OnMapImport(wxCommandEvent& event)
{
	menuMap->Enable(EC_MapImport,false);
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
	menuGoal->Enable(EC_MapImport,true);
};

void MyFrame::OnMapCreate(wxCommandEvent& event)
{
	menuMap->Enable(EC_MapCreate,false);
	menuMap->Enable(EC_MapCreate,true);
};
void MyFrame::OnGoalImport(wxCommandEvent& event)
{
	menuGoal->Enable(EC_GoalImport,false);
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
	menuMap->Enable(EC_GoalImport,true);
};

void MyFrame::OnGeneralSettings(wxCommandEvent& WXUNUSED(event))
{
	menuSettings->Enable(EC_GeneralSettings,false);
	if(child->run==1) child->run=2;
	dia=new wxDialog(this,EC_SettingsDialog,_T("General settings"),wxPoint(100,100),wxSize(450,260),wxDEFAULT_DIALOG_STYLE,_T("lala"));
	wxStaticText* text1=new wxStaticText(dia,-1,_T("MaxTime"),wxPoint(10,10),wxDefaultSize,0,_T("Max Time of Calculation"));
	wxStaticText* text2=new wxStaticText(dia,-1,_T("MaxTimeOut"),wxPoint(10,30),wxDefaultSize,0,_T("Max Timeout"));
	wxStaticText* text3=new wxStaticText(dia,-1,_T("MaxLength"),wxPoint(10,50),wxDefaultSize,0,_T("Max Length"));
	wxStaticText* text4=new wxStaticText(dia,-1,_T("MaxRuns"),wxPoint(10,70),wxDefaultSize,0,_T("Max Runs"));
	wxStaticText* text5=new wxStaticText(dia,-1,_T("MaxGenerations"),wxPoint(10,90),wxDefaultSize,0,_T("Max Generations"));
	wxStaticText* text7=new wxStaticText(dia,-1,_T("BreedFactor"),wxPoint(10,110),wxDefaultSize,0,_T("BreedFactor percentage"));
	wxStaticText* text8=new wxStaticText(dia,-1,_T("CrossOver"),wxPoint(10,130),wxDefaultSize,0,_T("CrossOver percentage"));

	spin1=new wxSpinCtrl(dia,EC_SpinMaxTime,wxEmptyString,wxPoint(100,10),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxTime(),child->settings.getMAXMaxTime(),child->settings.getMaxTime(),_T("MaxTimeSpin"));
	spin2=new wxSpinCtrl(dia,EC_SpinMaxTimeOut,wxEmptyString,wxPoint(100,30),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxTimeOut(),child->settings.getMAXMaxTimeOut(),child->settings.getMaxTimeOut(),_T("MaxTimeOutSpin"));
	spin3=new wxSpinCtrl(dia,EC_SpinMaxLength,wxEmptyString,wxPoint(100,50),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxLength(),child->settings.getMAXMaxLength(),child->settings.getMaxLength(),_T("MaxLengthSpin"));
	spin4=new wxSpinCtrl(dia,EC_SpinMaxRuns,wxEmptyString,wxPoint(100,70),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxRuns(),child->settings.getMAXMaxRuns(),child->settings.getMaxRuns(),_T("MaxRunsSpin"));
	spin5=new wxSpinCtrl(dia,EC_SpinMaxGenerations,wxEmptyString,wxPoint(100,90),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxGenerations(),child->settings.getMAXMaxGenerations(),child->settings.getMaxGenerations(),_T("MaxGenerationsSpin"));
	spin6=new wxSpinCtrl(dia,EC_SpinBreedFactor,wxEmptyString,wxPoint(100,110),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINBreedFactor(),child->settings.getMAXBreedFactor(),child->settings.getBreedFactor(),_T("BreedFactorSpin"));
	spin7=new wxSpinCtrl(dia,EC_SpinCrossOver,wxEmptyString,wxPoint(100,130),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINCrossOver(),child->settings.getMAXCrossOver(),child->settings.getCrossOver(),_T("CrossOverSpin"));

	check1=new wxCheckBox(dia,EC_CheckPreprocess,_T("Preprocess Buildorder"),wxPoint(10,150),wxDefaultSize,-1,wxDefaultValidator,_T("preprocess"));
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
	wxStaticText* text9=new wxStaticText(dia,-1,_T("SelectMap"),wxPoint(200,10),wxDefaultSize,0,_T("Select Map"));
	lb1=new wxListBox(dia,-1,wxPoint(200,30),wxSize(80,150),child->settings.getMapCount(),bla,0,wxDefaultValidator,_T("listBox"));
	lb1->SetSelection(child->settings.getCurrentMap());

	wxString bla2[MAX_MAPS];
	for(int i=0;i<child->settings.getGoalCount();i++)
		bla2[i]=_T(wxString::Format(wxT("%s"),child->settings.getGoal(i)->getName()));
	wxStaticText* text10=new wxStaticText(dia,-1,_T("SelectGoal"),wxPoint(300,10),wxDefaultSize,0,_T("Select Goal"));
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
	menuSettings->Enable(EC_GeneralSettings,true);
}

void MyFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
	if(child->run==0) child->run=1;
#if wxUSE_STATUSBAR
	SetStatusText(_T("Calculation in progress :-)")); 
#endif // wxUSE_STATUSBAR

	menuFile->Enable(EC_Start,false);
}

void MyFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
	if(child->run==1) child->run=0;
#if wxUSE_STATUSBAR
	SetStatusText(_T("Click on File/Start to start the calculation!")); 
#endif // wxUSE_STATUSBAR

	menuFile->Enable(EC_Start,true);
}

																			    
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	menuSettings->Enable(EC_About,false);
	if(child->run==1) child->run=2;
	wxString msg;
	msg.Printf(_T("- Evolution Chamber v%i\n- Current source code size: 400kb\n- GUI and library core programmed by Clemens Lode\n- Copyright by Clemens Lode\n- GUI created with %s library\n- Written with VIM - Vi IMproved version 6.2.120\n- MS Windows version compiled with MinGW / GCC 3.3.2\n- Windows executable/dll size: 396kb/344kb\n- Linux (GTK) version compiled with GCC 3.3.2\n- Linux binary/lib size: 180kb/248kb\n- Tested with winex3.2, MS Windows ME and Linux (2.4.22)\n- Work was done with Linux Fedora Core 1 (Redhat 10)\n\nThis is ONLY a demonstration program which may not be sold or copied without permission.\nThere is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\nGreetings, ghoul@clawsoftware.de"), EC_Version,wxVERSION_STRING);
	msgBox=new wxDialog(this,-1,_T("About EC"),wxPoint(100,100),wxSize(600,300),wxDEFAULT_FRAME_STYLE,_T("lala"));
	wxStaticText* text1=new wxStaticText(msgBox,-1,msg,wxPoint(10,10),wxDefaultSize,0,_T("bla"));
//homepage, updates
//	but1=new wxButton(msgBox,wxID_OK,_T("OK"),wxPoint(20,275),wxSize(65,20));
	while(msgBox->ShowModal()!=wxID_CANCEL);
	delete(msgBox);
	if(child->run==2) child->run=1;
	menuSettings->Enable(EC_About,true);
}



