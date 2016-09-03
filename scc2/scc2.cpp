#include "../scc2dll/settings.h"
#include "../scc2dll/location.h"
#include "scc2.h"
#include "../scc2dll/text.h"
#include <wx/tipdlg.h>
#include "wx/progdlg.h"
#include <wx/splash.h>
#include "math.h"

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
        EVT_MENU(SCC_Open,  MyFrame::OnOpen)
        EVT_MENU(SCC_Start,  MyFrame::OnStart)
        EVT_MENU(SCC_Stop,  MyFrame::OnStop)
        EVT_MENU(SCC_Quit,  MyFrame::OnQuit)
        EVT_MENU(SCC_About, MyFrame::OnAbout)
        EVT_MENU(SCC_GeneralSettings, MyFrame::OnGeneralSettings)
        EVT_MENU(SCC_GoalCreate,  MyFrame::OnGoalCreate)
        EVT_MENU(SCC_GoalImport,  MyFrame::OnGoalImport)
        EVT_MENU(SCC_MapCreate,  MyFrame::OnMapCreate)
        EVT_MENU(SCC_MapImport, MyFrame::OnMapImport)
END_EVENT_TABLE()



BEGIN_EVENT_TABLE(MyDCWindow, wxScrolledWindow)
        EVT_ERASE_BACKGROUND(MyDCWindow::OnEraseBackground)
        EVT_IDLE(MyDCWindow::OnIdle)
        EVT_PAINT(MyDCWindow::OnPaint)
END_EVENT_TABLE()


// ============================================================================
// implementation
// ============================================================================




// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	static const int max = 7;
	int progress=0;
        wxSplashScreen* splash=NULL;
	wxBitmap* bitmap=new wxBitmap("scc.bmp", wxBITMAP_TYPE_BMP);
	if(bitmap)
                splash = new wxSplashScreen(*bitmap, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, -1, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxSTAY_ON_TOP);

	wxProgressDialog dialog(_T("Progress of initializing"), _T("Be patient"), max, NULL, wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);

	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Initializing main window"),progress*100/max)));progress++;
	MyFrame *frame = new MyFrame(_T("Starcraft Calculator v1.04a"), wxPoint(0, 0), wxSize(800, 600));

	
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting map and goals"),(progress+1)*100/max)));progress++;

	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up tables"),(progress+1)*100/max)));progress++;
                                                                                                                                                           
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up status bar"),(progress+1)*100/max)));progress++;
	
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Initializing SCC core"),(progress+1)*100/max)));progress++;

	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Loading data files"),(progress+1)*100/max)));progress++;

	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Starting main application"),(progress+1)*100/max)));progress++;


	frame->Show(TRUE);
	if(bitmap) delete(bitmap);
	if(splash) delete(splash);
	return TRUE;
}

MyDCWindow::MyDCWindow(wxFrame *parent) 
//	   : wxWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER, _T("BLA"))
	:wxScrolledWindow(parent)
{
//	SetScrollbars(10,10,40,100,0,0);
        
        bmpGraph.LoadFile("r2.bmp",wxBITMAP_TYPE_BMP);
        bmpTimer.LoadFile("r3.bmp",wxBITMAP_TYPE_BMP);
        bmpBack.LoadFile("back.bmp",wxBITMAP_TYPE_BMP);
        bmpBack2.LoadFile("back2.bmp",wxBITMAP_TYPE_BMP);

#ifdef __WIN32__	
	font=wxFont(FONT_SIZE,wxDEFAULT,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
#else
	font=wxFont(FONT_SIZE,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT);
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
// assign goal 0 to player 1
        settings.setGoal(0,1);
// assign goal 0 also to player 2
        settings.setGoal(0,2);
        update=0;
// initialize the soup, set the parameters, load the players etc.
        settings.initSoup();
        ga=settings.getGa();
        run=0;
        maxpFitness=100;
        maxsFitness=100;
        maxtFitness=100;
        maxForce=0;
        maxUnitForce=0;
        int k;
        for(k=0;k<10;k++)
        {
                oldTimeCounter[k]=0;
                oldTime[k]=ga->maxTime;
        }

}

void MyDCWindow::OnEraseBackground(wxEraseEvent& event)
{
/*    wxDC& dct = *event.GetDC();
    dct.SetPen(*wxGREEN_PEN);
                                                                                
    // this line is needed, otherwise the junk is left on win the background
    dct.Clear();
                                                                                
    wxSize size = GetClientSize();
    for ( int x = 0; x < size.x; x+= 10 )
    {
        dct.DrawLine(x, 0, x, size.y);
    }
                                                                                
    for ( int y = 0; y < size.y; y+= 10 )
    {
        dct.DrawLine(0, y, size.x, y);
    }
                                                                                
    dct.SetTextForeground(*wxRED);
    dct.DrawText(_T("This text is drawn from OnEraseBackground"), 60, 60);

	//event.GetDC()->DrawBitmap(bmpGraph,THIRD_COLOUMN-10,100,true);
//	event.m_dc->SetTextBackground(wxColour(255,0,0));*/
//event.Skip();
//blub
};

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{
	msgBox=NULL;
	SetIcon(wxICON(mondrian));

#if wxUSE_MENUS
	menuFile=new wxMenu;
	menuHelp=new wxMenu;	
	menuSettings=new wxMenu;
	menuGoal=new wxMenu;
	menuMap=new wxMenu;
	menuAdvanced=new wxMenu;
	menuHelp->Append(SCC_About, _T("&About...\tF1"), _T("Show about dialog"));
	menuFile->Append(SCC_Open, _T("&Open\tAlt-O"), _T("Open a file"));
	menuFile->Append(SCC_Start, _T("&Start\tAlt-S"), _T("Start/Continue calculation"));
	menuFile->Append(SCC_Stop, _T("S&top\tAlt-T"), _T("Stop calculation"));
	menuFile->Append(SCC_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
	
	menuSettings->Append(SCC_GeneralSettings, _T("&General\tAlt-G"), _T("General settings"));

	menuGoal->Append(SCC_GoalCreate, _T("C&reate\tAlt-R"),_T("Create new goal"));
	menuGoal->Append(SCC_GoalImport, _T("&Import\tAlt-I"),_T("Import goal file"));

        menuMap->Append(SCC_MapCreate, _T("&Create\tAlt-C"),_T("Create new map"));
        menuMap->Append(SCC_MapImport, _T("I&mport\tAlt-M"),_T("Import map file"));

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
// create a status bar just for fun (by default with 1 pane only)
// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(2);
	SetStatusText(_T("Click on File/Start to start the calculation!"));
#endif // wxUSE_STATUSBAR

	child=new MyDCWindow(this);
}

void MyDCWindow::showGraph(wxDC* dec,int* data,int max,wxColour col)
{
	int i,j;
        dec->SetPen(wxPen(col,1,wxSOLID));
	wxPoint blub[200];
	j=1;
	for(i=1;i<199;i++) 
	{
		if(data[i]!=data[i+1])
		{
			blub[j]=wxPoint(i+THIRD_COLOUMN,210-data[i]/max);
			j++;
		}
	}
	blub[0]=wxPoint(THIRD_COLOUMN,210-data[0]/max);
	blub[j]=wxPoint(199+THIRD_COLOUMN,210-data[199]/max);
	dec->DrawSpline(j+1,blub);
};

void MyDCWindow::OnPaint(wxPaintEvent& event)
{
        int i,j,k,s,t;
	int availible[MAX_LENGTH];	
	int avcount=1;
	int h2=0;
	for(i=0;i<MAX_LENGTH;i++) availible[i]=0;

	wxPaintDC dcp(this);
        if(update==0) return;
	if(dc) delete(dc);
	dc=new wxMemoryDC();
        dc->SelectObject(wxBitmap(800,600));
	dc->SetTextBackground(wxColour(255,0,0));
	if(update==2)
	{
	for(i=0;i<199;i++)
        {
                pFitness[i]=pFitness[i+1];
                sFitness[i]=sFitness[i+1];
                tFitness[i]=tFitness[i+1];
		aFitness[i]=aFitness[i+1];
		time[i]=time[i+1];
		force[i]=force[i+1];
        };
        t=0;
	for(i=0;i<GAS_SCV;i++)
	{
		if(anarace->location[0].force[i]>maxUnitForce)
			maxUnitForce=anarace->location[0].force[i];
		t+=anarace->location[0].force[i];
	}
	if(t>maxForce)
		maxForce=t;
		
	if(anarace->getMaxpFitness()>maxpFitness) maxpFitness=anarace->getMaxpFitness();
        if(anarace->getMaxsFitness()>maxsFitness) maxsFitness=anarace->getMaxsFitness();
        if(anarace->getMaxtFitness()>maxtFitness) maxtFitness=anarace->getMaxtFitness();

        pFitness[199]=anarace->getMaxpFitness()*100;
        sFitness[199]=anarace->getMaxsFitness()*100;
        tFitness[199]=anarace->getMaxtFitness()*100;
	aFitness[199]=anarace->fitnessAverage*100;
	time[199]=(ga->maxTime-anarace->getTimer())*100;
	force[199]=t*100;
        
	if(time[199]<time[198])
	{
		k=0;
		while(oldTimeCounter[k]>0)
		{
			k++;
			if(k>9) k=0;
		}
		oldTimeCounter[k]=1;
		oldTime[k]=time[198]/100;
	}
	}
	update=1;
        dc->DrawBitmap(bmpTimer,THIRD_COLOUMN-10,0,false);
	
	for(k=0;k<10;k++)
	if(oldTimeCounter[k]>0)
	{
	        dc->SetFont(wxFont(30-oldTimeCounter[k]*2,wxMODERN,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
        	dc->SetTextForeground(wxColour(180-oldTimeCounter[k]*18,0,0));
        	dc->DrawText(_T(wxString::Format(wxT("[%.2i:%.2i]"),oldTime[k]/60,oldTime[k]%60)),THIRD_COLOUMN+10+oldTimeCounter[k]*2,40-oldTimeCounter[k]);
       		
		oldTimeCounter[k]++;
		if(oldTimeCounter[k]>10)
			oldTimeCounter[k]=0;
	}

        dc->SetFont(wxFont(30,wxMODERN,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
        dc->SetTextForeground(wxColour(200,0,0));
        dc->DrawText(_T(wxString::Format(wxT("[%.2i:%.2i]"),(ga->maxTime-anarace->getTimer())/60,(ga->maxTime-anarace->getTimer())%60)),THIRD_COLOUMN+10,40);
	dc->SetFont(font);

	dc->DrawBitmap(bmpGraph,THIRD_COLOUMN-10,100,true);
	
	showGraph(dc,tFitness,maxtFitness,wxColour(255,40,255));
        showGraph(dc,sFitness,maxsFitness,wxColour(40,255,40));
	showGraph(dc,force,maxForce,wxColour(40,255,255));
        showGraph(dc,time,ga->maxTime,wxColour(255,255,40));
        showGraph(dc,aFitness,maxpFitness,wxColour(200,200,200));
	showGraph(dc,pFitness,maxpFitness,wxColour(255,40,40));
	
 	
//TODO Bitmap Brush fuer Rectangles
	
	dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
	wxRect edge=wxRect();
	edge.x=0;
	edge.height=FONT_SIZE+4;
	edge.width=SECOND_COLOUMN-10-edge.x;
	wxBrush b1=wxBrush(wxColour(230,230,230),wxSOLID);
	wxBrush b2=wxBrush(wxColour(190,190,190),wxSOLID);
	for(j=0;j<BUILD_ORDER_NUMBER;j++)
	{
		edge.y=j*(FONT_SIZE+5)+3;
		if(j%2==0)
			dc->SetBrush(b1);
		else dc->SetBrush(b2);
		dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
	};
	edge.x=SECOND_COLOUMN;
        edge.width=SECOND_COLOUMN_WIDTH;
        for(j=0;j<FORCE_LIST_NUMBER;j++)
        {
                edge.y=j*(FONT_SIZE+5)+3;
                if(j%2==0)
                        dc->SetBrush(b1);
                else dc->SetBrush(b2);
                dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
        };

	edge.x=SECOND_COLOUMN;
	edge.width=500;

	for(j=0;j<BUILD_ORDER_GRAPH_NUMBER;j++)
	{
		edge.y=j*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;
		if(j%2==0)
			dc->SetBrush(b1);
		else dc->SetBrush(b2);
              dc->DrawRoundedRectangle(edge.x,edge.y,edge.width,edge.height,4);
	}


        dc->DrawBitmap(bmpBack2,SECOND_COLOUMN-10,BUILD_ORDER_GRAPH_LENGTH,true);

        dc->SetTextForeground(wxColour(255,40,40));
	dc->DrawText(_T("Fitness"),SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+10);
        dc->SetTextForeground(wxColour(40,40,255));
	dc->DrawText(_T("Minerals"),SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+22);
        dc->SetTextForeground(wxColour(40,255,40));
	dc->DrawText(_T("Gas"),SECOND_COLOUMN,BUILD_ORDER_GRAPH_LENGTH+34);
        dc->SetTextForeground(wxColour(0,0,0));
	
//        dc->SetPen(wxPen(wxColour(40,255,40),1,wxSOLID));
//	dc->DrawRectangle(330,55+13*(FONT_SIZE+5)+3+30*(FONT_SIZE+5)+3,500,50); //gas


//       dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramForceCount(rand()%MAX_LENGTH))),0,h);
       dc->DrawText(_T("Name"),0,3);
       dc->DrawText(_T("Mins"),110,3);
       dc->DrawText(_T("Gas"),150,3);
       dc->DrawText(_T("Supply"),190,3);
       dc->DrawText(_T("Time"),240,3);
  

        dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));

	t=1;
	int y1;

	wxPoint mins[MAX_LENGTH];
	wxPoint gas[MAX_LENGTH];
	wxPoint fitness[MAX_LENGTH];
	int count=0;

	for(s=MAX_LENGTH;s--;) //~~ Max length
//		program[0] ist irgendwie kaputt...
		if(anarace->getProgramIsBuilt(s)&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer()))
		{
//			who=s-1;
//			while((who>=0)&&((!anarace->getProgramIsBuilt(who))||(anarace->getProgramTime(who)>ga->maxTime-anarace->getTimer()))) who--;
			
//			if(who>=0)
//			{
			if(anarace->getProgramHaveGas(s)>50000) y1=50; else y1=anarace->getProgramHaveGas(s)/1000;
mins[count]=wxPoint(SECOND_COLOUMN+10+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer()))  ,BUILD_ORDER_GRAPH_LENGTH+61-y1);
       	                 if(anarace->getProgramHaveMinerals(s)>50000) y1=50; else y1=anarace->getProgramHaveMinerals(s)/1000;
gas[count]=wxPoint(SECOND_COLOUMN+10+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer())),BUILD_ORDER_GRAPH_LENGTH+61-y1);
			 y1=anarace->getProgramFitness(s)*50/(anarace->getMaxpFitness()-anarace->getTimer());
fitness[count]=wxPoint(SECOND_COLOUMN+10+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer())),BUILD_ORDER_GRAPH_LENGTH+61-y1);
			count++;
		}

        dc->SetPen(wxPen(wxColour(40,255,40),1,wxSOLID));
	dc->DrawSpline(count,mins); //~~ Start End Punkt?
        dc->SetPen(wxPen(wxColour(40,40,255),1,wxSOLID));
	dc->DrawSpline(count,gas);
        dc->SetPen(wxPen(wxColour(255,40,40),1,wxSOLID));
	dc->DrawSpline(count,fitness);
        dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));

	for(s=MAX_LENGTH;s--;)
        if((anarace->getProgramIsBuilt(s))&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer()))

	{

			if(bolog[s].order==anarace->phaenoCode[s])
			{
				if(bolog[s].count<160)
					bolog[s].count+=3;
			} else
			{
				bolog[s].count=0;
				bolog[s].order=anarace->phaenoCode[s];
			}
			dc->SetTextForeground(wxColour(bolog[s].count,bolog[s].count/2,bolog[s].count/3));
//			if(bolog[s].count<=5)
                         int h=t*(FONT_SIZE+5)+3;

 if((anarace->getProgramFacility(s)>0)&&(stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[s]].BT>0))
			{
				dc->SetBrush(wxBrush(wxColour(   ((anarace->getProgramCode(s)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
								((anarace->getProgramCode(s)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
								((anarace->getProgramCode(s)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),
						wxSOLID));
				for(i=1;i<avcount;i++)
					if(availible[i]==anarace->getProgramFacility(s))	break;
				if(i==avcount) {availible[i]=anarace->getProgramFacility(s);avcount++;}
				h2=i*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;  //~~~
	                        
//paar ragen drueber hinaus, weil sie angefangen wurden, das Programm aber vorher schon fertig war
dc->DrawRoundedRectangle(SECOND_COLOUMN+((anarace->getProgramTime(s)*500)/(ga->maxTime-anarace->getTimer())),h2,
                       ((stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[s]].BT/*anarace->getProgramBT(s)*/*500)/(ga->maxTime-anarace->getTimer())),FONT_SIZE+4,4);
 //~~ tatsaechliche BT... ~~anzahl facilities mit einbeziehen! hoehe des Balkens etc. und durchreihen
			}
				for(i=0;i<(ga->maxTime-anarace->getTimer())/60;i++)
				{
					dc->DrawLine(SECOND_COLOUMN+(i+1)*(500/((ga->maxTime-anarace->getTimer())/60)),FORCE_LIST_LENGTH+3,
						SECOND_COLOUMN+(i+1)*(500/((ga->maxTime-anarace->getTimer())/60)),FORCE_LIST_LENGTH+3+FONT_SIZE+4);
	                                dc->DrawText(_T(wxString::Format(wxT("%i:00"),i)),SECOND_COLOUMN+5+i*(500/((ga->maxTime-anarace->getTimer())/60)),FORCE_LIST_LENGTH+3);
				}
				
                                dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramForceCount(rand()%MAX_LENGTH))),0,h);
				dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[s]].name)),0,h);
				dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramHaveMinerals(s)/100)),110,h);
				dc->DrawText(_T(wxString::Format(wxT("%i"),anarace->getProgramHaveGas(s)/100)),150,h);
				dc->DrawText(_T(wxString::Format(wxT("%i/%i"),anarace->getProgramNeedSupply(s),anarace->getProgramHaveSupply(s))),190,h);
				dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),anarace->getProgramTime(s)/60,anarace->getProgramTime(s)%60)),240,h);
				//dc->DrawText(_T(wxString::Format(wxT("%s"),anarace->getMap()->location[anarace->getProgramLocation(s)].getName())),310,h);
				//dc->DrawText(_T(wxString::Format(wxT("%s"),error_message[anarace->getProgramSuccessType(s)])),440,h);
				//dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->getProgramSuccessUnit(s)].name)),530,h);
				t++;
			}
        dc->SetTextForeground(wxColour(0,0,0));
        for(i=1;i<avcount;i++)
	        dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][availible[i]].name)),835,i*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH);

	wxPoint Legend2=wxPoint(THIRD_COLOUMN,225);
	wxPoint Legend3=wxPoint(160,FONT_SIZE+4);

	dc->SetBrush(wxBrush(wxColour(50,50,50),wxSOLID));
        dc->SetPen(wxPen(wxColour(255,255,255),1,wxSOLID));
                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(40,255,255));
	dc->DrawText(_T(wxString::Format(wxT("Force            : %i"),force[199]/100)),Legend2.x,Legend2.y);
         
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+15,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(255,255,40));
        dc->DrawText(_T(wxString::Format(wxT("Time             : %i"),time[199]/100)),Legend2.x,Legend2.y+15);
  
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+30,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(255,40,255));
        dc->DrawText(_T(wxString::Format(wxT("Tertiary fitness : %i"),tFitness[199]/100)),Legend2.x,Legend2.y+30);
 
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+45,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(40,255,40));
	dc->DrawText(_T(wxString::Format(wxT("Secondary fitness: %i"),sFitness[199]/10000)),Legend2.x,Legend2.y+45);
  
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+60,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(255,40,40));
        dc->DrawText(_T(wxString::Format(wxT("Primary fitness  : %i"),pFitness[199]/100)),Legend2.x,Legend2.y+60);

        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+75,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(200,200,200));
        dc->DrawText(_T(wxString::Format(wxT("Fitness average  : %i"),aFitness[199]/100)),Legend2.x,Legend2.y+75);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+90,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(100,100,100));
        dc->DrawText(_T(wxString::Format(wxT("Fitness variance : %i"),(int)sqrt((double)anarace->fitnessVariance))),Legend2.x,Legend2.y+90);


	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+115,Legend3.x,Legend3.y,4);
        dc->SetBrush(wxBrush(wxColour(40,40,100),wxSOLID));
	dc->DrawRoundedRectangle(Legend2.x+Legend3.x-(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations,Legend2.y+115,(ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/ga->maxGenerations ,Legend3.y,4);
	dc->SetTextForeground(wxColour(100,100,255));
        dc->DrawText(_T(wxString::Format(wxT("%i+ generations left"),(ga->maxGenerations-anarace->getUnchangedGenerations()))),Legend2.x,Legend2.y+115);

	t=1;

        dc->DrawText(_T("Units part of the goal list"),SECOND_COLOUMN+SECOND_COLOUMN_WIDTH/2,3);
 // all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for(i=0;i<=GAS_SCV;i++)
		if(anarace->getPlayer()->goal->allGoal[i]>0)
		{
			if(globalForcelog[i].order==anarace->location[0].force[i])
			{
				if(globalForcelog[i].count<160)
					globalForcelog[i].count+=3;
			}
			else
			{
				globalForcelog[i].count=0;
				globalForcelog[i].order=anarace->location[0].force[i];
			}
			int h=t*(FONT_SIZE+5)+3;
			dc->SetTextForeground(wxColour(0,0,0));
//((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
  //                            ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
    //                          ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256));

                        dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),SECOND_COLOUMN,h);
			//max force einfuehren
//			dc->SetBrush(wxBrush(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count),wxSOLID));
                   dc->SetBrush(wxBrush(wxColour(((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
                                                ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
                                                ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));

			dc->DrawRoundedRectangle(SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-anarace->location[0].force[i]*100/maxUnitForce-1,h,anarace->location[0].force[i]*100/maxUnitForce+1,FONT_SIZE+4,4);
		        dc->SetTextForeground(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count));
			dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->location[0].force[i])),SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-30-anarace->location[0].force[i]*100/maxUnitForce,h);
			t++;
		}
	t++;
        dc->DrawText(_T("Units not on the goal list"),SECOND_COLOUMN+SECOND_COLOUMN_WIDTH/2,t*(FONT_SIZE+5)+3);
	t++;
	for(i=0;i<=GAS_SCV;i++)
		if((anarace->getPlayer()->goal->allGoal[i]==0)&&(anarace->location[0].force[i]))
		{
			if(globalForcelog[i].order==anarace->location[0].force[i])
			{
			       if(globalForcelog[i].count<160)
				       globalForcelog[i].count+=5;
			}
			else
			{
				globalForcelog[i].count=0;
				globalForcelog[i].order=anarace->location[0].force[i];
			}
			int h=t*(FONT_SIZE+5)+3;

dc->SetTextForeground(wxColour(0,0,0));
/*((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
                              ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
                              ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256));*/

			dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),SECOND_COLOUMN,h);
                        //max force einfuehren
//                        dc->SetBrush(wxBrush(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count),wxSOLID));
                dc->SetBrush(wxBrush(wxColour(   ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
                                                ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
                                                ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));
			dc->DrawRoundedRectangle(SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-anarace->location[0].force[i]*100/maxUnitForce-1,h,anarace->location[0].force[i]*100/maxUnitForce+1,FONT_SIZE+4,4);
                        dc->SetTextForeground(wxColour(globalForcelog[i].count/2,globalForcelog[i].count/4,globalForcelog[i].count));
                        dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->location[0].force[i])),SECOND_COLOUMN+SECOND_COLOUMN_WIDTH-30-anarace->location[0].force[i]*100/maxUnitForce,h);
                        t++;
		}
	dcp.Blit(0, 0, 800, 600, dc, 0, 0);
}
										


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
	menuGoal->Enable(SCC_GoalCreate,false);
	menuGoal->Enable(SCC_GoalCreate,true);
};
void MyFrame::OnGoalImport(wxCommandEvent& event)
{
        menuMap->Enable(SCC_GoalImport,false);
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
        menuGoal->Enable(SCC_GoalImport,true);
};

void MyFrame::OnMapCreate(wxCommandEvent& event)
{
        menuMap->Enable(SCC_MapCreate,false);
        menuMap->Enable(SCC_MapCreate,true);
};
void MyFrame::OnMapImport(wxCommandEvent& event)
{
        menuGoal->Enable(SCC_MapImport,false);
	if(child->run==1) child->run=2;
        wxFileDialog fileDialog(this, wxT("Select a file"));
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
        menuMap->Enable(SCC_MapImport,true);
};
                                                                                                                                                           


void MyFrame::OnGeneralSettings(wxCommandEvent& WXUNUSED(event))
{
	menuSettings->Enable(SCC_GeneralSettings,false);
	if(child->run==1) child->run=2;
	dia=new wxDialog(this,SCC_SettingsDialog,_T("General settings"),wxPoint(100,100),wxSize(450,260),wxDEFAULT_DIALOG_STYLE,_T("lala"));
	wxStaticText* text1=new wxStaticText(dia,-1,_T("MaxTime"),wxPoint(10,10),wxDefaultSize,0,_T("Max Time of Calculation"));
        wxStaticText* text2=new wxStaticText(dia,-1,_T("MaxTimeOut"),wxPoint(10,30),wxDefaultSize,0,_T("Max Timeout"));
        wxStaticText* text3=new wxStaticText(dia,-1,_T("MaxLength"),wxPoint(10,50),wxDefaultSize,0,_T("Max Length"));
        wxStaticText* text4=new wxStaticText(dia,-1,_T("MaxRuns"),wxPoint(10,70),wxDefaultSize,0,_T("Max Runs"));
        wxStaticText* text5=new wxStaticText(dia,-1,_T("MaxGenerations"),wxPoint(10,90),wxDefaultSize,0,_T("Max Generations"));
        wxStaticText* text7=new wxStaticText(dia,-1,_T("BreedFactor"),wxPoint(10,110),wxDefaultSize,0,_T("BreedFactor percentage"));
        wxStaticText* text8=new wxStaticText(dia,-1,_T("CrossOver"),wxPoint(10,130),wxDefaultSize,0,_T("CrossOver percentage"));

	spin1=new wxSpinCtrl(dia,SCC_SpinMaxTime,wxEmptyString,wxPoint(100,10),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxTime(),child->settings.getMAXMaxTime(),child->settings.getMaxTime(),_T("MaxTimeSpin"));
        spin2=new wxSpinCtrl(dia,SCC_SpinMaxTimeOut,wxEmptyString,wxPoint(100,30),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxTimeOut(),child->settings.getMAXMaxTimeOut(),child->settings.getMaxTimeOut(),_T("MaxTimeOutSpin"));
        spin3=new wxSpinCtrl(dia,SCC_SpinMaxLength,wxEmptyString,wxPoint(100,50),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxLength(),child->settings.getMAXMaxLength(),child->settings.getMaxLength(),_T("MaxLengthSpin"));
        spin4=new wxSpinCtrl(dia,SCC_SpinMaxRuns,wxEmptyString,wxPoint(100,70),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxRuns(),child->settings.getMAXMaxRuns(),child->settings.getMaxRuns(),_T("MaxRunsSpin"));
        spin5=new wxSpinCtrl(dia,SCC_SpinMaxGenerations,wxEmptyString,wxPoint(100,90),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINMaxGenerations(),child->settings.getMAXMaxGenerations(),child->settings.getMaxGenerations(),_T("MaxGenerationsSpin"));
        spin6=new wxSpinCtrl(dia,SCC_SpinBreedFactor,wxEmptyString,wxPoint(100,110),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINBreedFactor(),child->settings.getMAXBreedFactor(),child->settings.getBreedFactor(),_T("BreedFactorSpin"));
        spin7=new wxSpinCtrl(dia,SCC_SpinCrossOver,wxEmptyString,wxPoint(100,130),wxSize(60,18),wxSP_ARROW_KEYS,child->settings.getMINCrossOver(),child->settings.getMAXCrossOver(),child->settings.getCrossOver(),_T("CrossOverSpin"));

	check1=new wxCheckBox(dia,SCC_CheckPreprocess,_T("Preprocess Buildorder"),wxPoint(10,150),wxDefaultSize,-1,wxDefaultValidator,_T("preprocess"));
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
        menuSettings->Enable(SCC_GeneralSettings,true);
}

void MyFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
	if(child->run==0) child->run=1;
	menuFile->Enable(SCC_Start,false);
}

void MyFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
	if(child->run==1) child->run=0;
	menuFile->Enable(SCC_Start,true);
}

																			    
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
        menuSettings->Enable(SCC_About,false);
	if(child->run==1) child->run=2;
	wxString msg;
	msg.Printf(_T("StarCraft Calculator v%i\nGUI and Library core programmed by Clemens Lode\nCopyright by Clemens Lode\nGUI created with %s library\nWritten with VIM\nCompiled with MinGW / GCC 3.3.2\nWork was done with Fedora Core 1\n\nGreetings to all who helped me creating this software\n"), SCC_Version,wxVERSION_STRING);
	msgBox=new wxDialog(this,-1,_T("About SCC"),wxPoint(100,100),wxSize(300,200),wxDEFAULT_FRAME_STYLE,_T("lala"));
        wxStaticText* text1=new wxStaticText(msgBox,-1,msg,wxPoint(10,10),wxDefaultSize,0,_T("bla"));
//homepage, updates
        but1=new wxButton(msgBox,wxID_OK,_T("OK"),wxPoint(120,150),wxSize(65,20));
	while(msgBox->ShowModal()!=wxID_OK);
	delete(msgBox);
        if(child->run==2) child->run=1;
	menuSettings->Enable(SCC_About,true);
}



