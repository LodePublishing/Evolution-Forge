#include "../scc2dll/settings.h"
#include "../scc2dll/location.h"
#include "scc2.h"
#include "../scc2dll/text.h"
#include <wx/tipdlg.h>
#include "wx/progdlg.h"
#include <wx/splash.h>
// just a small startup program
// generally all functions return 0 when there was an error and 1 (or a pointer or whatever) if everything went fine
// a message in the error.log is dispatched and you can get the error message (and later the error code) to determine the error
// all in all the error system is only for debugging, I am still working on the function that checks all of the variables

// What still remains are functions to create and save goals/maps/settings etc.

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------
                                                                                                                                                            
// IDs for the controls and the menu commands
enum
{
    // menu items
	Minimal_Open=1,
	Minimal_Start=2,
	Minimal_Stop=3,
    	Minimal_Quit = 4,
	BUILD_ORDER_GRID=202,
	FORCE_GRID=203,
                                                                                                                                                            
    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Minimal_About = wxID_ABOUT
};
                                                                                                                                                            
// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------
                                                                                                                                                            
// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(Minimal_Open,  MyFrame::OnOpen)
        EVT_MENU(Minimal_Start,  MyFrame::OnStart)
        EVT_MENU(Minimal_Stop,  MyFrame::OnStop)
        EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
	EVT_MENU(Minimal_About, MyFrame::OnAbout)
	EVT_IDLE(MyFrame::OnIdle)   
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)
                                                                                                                                                            
// ============================================================================
// implementation
// ============================================================================




// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window

    MyFrame *frame = new MyFrame(_T("Starcraft Calculator v1.04a"),
                                 wxPoint(0, 0), wxSize(800, 800));
    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);
    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------
                                                                                                                                                            
// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{
	// set the frame icon
	SetIcon(wxICON(mondrian));

  wxBitmap bitmap;
  wxSplashScreen* splash=NULL;
  if (bitmap.LoadFile("scc.bmp", wxBITMAP_TYPE_BMP))
  {
      splash = new wxSplashScreen(bitmap,
          wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT,
          -1, NULL, -1, wxDefaultPosition, wxDefaultSize,
          wxSIMPLE_BORDER|wxSTAY_ON_TOP);
  }
//  wxYield();
#if wxUSE_MENUS
	// create a menu bar
	wxMenu *menuFile = new wxMenu;
	// the "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));
        menuFile->Append(Minimal_Open, _T("O&pen\tAlt-O"), _T("Open a file"));
	menuFile->Append(Minimal_Start, _T("S&tart\tAlt-S"), _T("Start/Continue calculation"));
	menuFile->Append(Minimal_Stop, _T("St&op\tAlt-T"), _T("Stop calculation"));
        menuFile->Append(Minimal_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, _T("&File"));
	menuBar->Append(helpMenu, _T("&Help"));
// ... and attach this menu bar to the frame
        SetMenuBar(menuBar);
	Refresh();
#endif // wxUSE_MENUS
        //wxTipProvider *tipProvider = wxCreateFileTipProvider("settings.txt", 0);
        //wxShowTip(this, tipProvider);
        //delete tipProvider;
	static const int max = 7;
	int progress=0;
	
	wxProgressDialog dialog(_T("Progress of initializing"),
                            _T("Be patient"),
                            max,    // range
                            this,   // parent
                           // wxPD_CAN_ABORT |
                            wxPD_APP_MODAL |
                            // wxPD_AUTO_HIDE | -- try this as well
                            wxPD_ELAPSED_TIME |
                            wxPD_ESTIMATED_TIME |
                            wxPD_REMAINING_TIME);
	dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Initializing main window"),progress*100/max)));progress++;

//	wxgrid=new wxGrid(this,BUILD_ORDER_GRID,wxDefaultPosition,wxDefaultSize, wxWANTS_CHARS, "Build order");
//	wxgrid->CreateGrid(MAX_LENGTH,10, (wxGrid::wxGridSelectionModes) 0);

        //wxgrid2=new wxGrid(this,FORCE_GRID,wxPoint( 700, 0 ),wxDefaultSize, wxWANTS_CHARS, "Force");
//        wxgrid2->CreateGrid(MAX_LENGTH,1, (wxGrid::wxGridSelectionModes) 0);

        dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up tables"),(progress+1)*100/max)));progress++;

/*	wxgrid->SetColLabelValue(0,wxT("Name"));
        wxgrid->SetColLabelValue(1,wxT("Mins"));
        wxgrid->SetColLabelValue(2,wxT("Gas"));
        wxgrid->SetColLabelValue(3,wxT("Supply"));
        wxgrid->SetColLabelValue(4,wxT("Time"));
        wxgrid->SetColLabelValue(5,wxT("Location"));
        wxgrid->SetColLabelValue(6,wxT("Success Type"));
        wxgrid->SetColLabelValue(7,wxT("Success Unit"));

        wxgrid->SetColLabelValue(8,wxT("Goal Name"));
        wxgrid->SetColLabelValue(9,wxT("Goal count"));
    
	wxgrid->EnableEditing(false);
	wxgrid->DisableDragGridSize();
	wxgrid->DisableDragRowSize();
	wxgrid->DisableDragColSize();

	wxFont wxfont=wxFont(10,wxDEFAULT,wxNORMAL,false);
	wxgrid->SetLabelFont(wxfont);
//	wxgrid2->SetLabelFont(wxfont);

	wxgrid->SetColSize(0,110);
        wxgrid->SetColSize(1,30);
        wxgrid->SetColSize(2,30);
        wxgrid->SetColSize(3,40);
        wxgrid->SetColSize(4,40);
        wxgrid->SetColSize(5,100);
    	wxgrid->SetColSize(6,80);
        wxgrid->SetColSize(7,100);
        wxgrid->SetColSize(8,100);
        wxgrid->SetColSize(9,40);
 
	wxgrid->SetRowLabelSize(25);
	wxgrid->SetGridLineColour(wxColour(200,200,200));

        wxgrid2->SetColSize(0,110);
        wxgrid2->SetRowLabelSize(100);
        wxgrid2->SetGridLineColour(wxColour(200,200,200));

	int i,j;
	for(j=0;j<MAX_LENGTH;j++)
	{
		for(i=0;i<8;i++)	
			wxgrid->SetCellBackgroundColour(j,i,wxColour( 230-40*(j%2), 230-40*(j%2), 230-40*(j%2)));
                wxgrid->SetCellBackgroundColour(j,8,wxColour( 230,230,230));
                wxgrid->SetCellBackgroundColour(j,9,wxColour( 230,230,230));

//		wxgrid2->SetCellBackgroundColour(j,0,wxColour( 230-40*(j%2), 230-40*(j%2), 230-40*(j%2)));
	}

//	wxgrid->SetLabelTextColour
//(wxGrid::wxGridSelectionModes)GetLong (wxT("selmode"),0));
	//SetupWindow(wxgrid);*/
	

        dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting up status bar"),(progress+1)*100/max)));progress++;
	
 
#if wxUSE_STATUSBAR
// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(2);
	SetStatusText(_T("Click on File/Start to start the calculation!"));
#endif // wxUSE_STATUSBAR

       dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Loading data files"),(progress+1)*100/max)));progress++;
                                                                                                                     
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
        settings.loadHarvestFile("mining.txt");
        settings.loadSettingsFile("settings.txt");
// Map in "map.txt" is now map[0]
        settings.loadMapFile("map.txt");

        dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Setting map and goals"),(progress+1)*100/max)));progress++;

// choose the first map we loaded (map[0])
        settings.setCurrentMap(0);
// Goal in "goal.txt" is now goal[0]
        settings.loadGoalFile("goal.txt");
// assign goal 0 to player 1
        settings.setGoal(0,1);
// assign goal 0 also to player 2
        settings.setGoal(0,2);

        dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Initializing SCC core"),(progress+1)*100/max)));progress++;
                                                                                                                        
// initialize the soup, set the parameters, load the players etc.
        settings.initSoup();
	ga=settings.getGa();
	run=0;
        dialog.Update(progress,_T(wxString::Format(wxT("%i%%: Starting main application"),(progress+1)*100/max)));progress++;
	if(splash)
	delete splash;
}
                                                                                                                                                           
void MyFrame::OnIdle(wxIdleEvent& WXUNUSED(event))
{
	int s,t,k,i;
	if((run)&&(anarace=settings.newGeneration()))
	{
		SetStatusText(_T(wxString::Format(wxT("%i runs and %i+ generations remaining. [Total %i generations]\r\n"),ga->maxRuns-anarace->getRun(),ga->maxGenerations-anarace->getUnchangedGenerations(),anarace->getGeneration())));
    
                t=0;
                        for(s=MAX_LENGTH;s--;)
                        {
				t++;
				if(anarace->getProgramIsBuilt(s))
                                {
// bolog is for logging the time a build order entry remains unchanged on the same place
// There is still some work to do in the core to track movements of whole blocks etc.
                                                                                                                
                                        if(bolog[s].order==anarace->phaenoCode[s])
                                        {
                                                if(bolog[s].count<160)
                                                        bolog[s].count+=3;
                                        } else
                                        {
                                                bolog[s].count=0;
                                                bolog[s].order=anarace->phaenoCode[s];
                                        }
                                                                                                                
// Is this entry part of the goal list?
/*                                        if(anarace->getProgramIsGoal(s))
                                        {
						
                                                setColor(34);print("@");
                                        } else print(" ");*/
                                                                                                                
// print the last error _before_ the item was built (i.e. what was the reason why this item could not be built immediately?)
// print the short name of the build order item

//		for(k=0;k<8;k++)  
//			wxgrid->SetCellTextColour(MAX_LENGTH-s-1,k,wxColour(bolog[s].count,bolog[s].count,bolog[s].count));
		if(bolog[s].count<=5)
		{
//wxgrid->SetCellValue(MAX_LENGTH-s-1,0,_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[s]].name)));
/*wxgrid->SetCellValue(MAX_LENGTH-s-1,1,_T(wxString::Format(wxT("%i"),anarace->getProgramHaveMinerals(s)/100)));
wxgrid->SetCellValue(MAX_LENGTH-s-1,2,_T(wxString::Format(wxT("%i"),anarace->getProgramHaveGas(s)/100)));
wxgrid->SetCellValue(MAX_LENGTH-s-1,3,_T(wxString::Format(wxT("%i/%i"),anarace->getProgramNeedSupply(s),anarace->getProgramHaveSupply(s))));
wxgrid->SetCellValue(MAX_LENGTH-s-1,4,_T(wxString::Format(wxT("%.2i:%.2i"),anarace->getProgramTime(s)/60,anarace->getProgramTime(s)%60)));
wxgrid->SetCellValue(MAX_LENGTH-s-1,5,_T(wxString::Format(wxT("%s"),anarace->getMap()->location[anarace->getProgramLocation(s)].getName())));
wxgrid->SetCellValue(MAX_LENGTH-s-1,6,_T(wxString::Format(wxT("%s"),error_message[anarace->getProgramSuccessType(s)])));
wxgrid->SetCellValue(MAX_LENGTH-s-1,7,_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->getProgramSuccessUnit(s)].name)));*/
}
				}
// built==0? => invalid entry
			
				else 
		                	for(k=0;k<8;k++)
					{
//                		        	wxgrid->SetCellTextColour(MAX_LENGTH-s-1,k,wxColour(0,0,0));
//						wxgrid->SetCellValue(MAX_LENGTH-s-1,k, wxT(" "));
					}
		}
                t=0;
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
//                                wxgrid->SetCellTextColour(t,8,wxColour(globalForcelog[i].count,globalForcelog[i].count,globalForcelog[i].count));
  //                              wxgrid->SetCellTextColour(t,9,wxColour(globalForcelog[i].count,globalForcelog[i].count,globalForcelog[i].count));
    //                            wxgrid->SetCellValue(t,9,_T(wxString::Format(wxT("%2i"),anarace->location[0].force[i])));
//				wxgrid->SetCellValue(t,8,_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)));
                                t++;
                        }
                t++;
//	        wxgrid->SetCellValue(t,8,wxT("NON GOALS"));
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
//                                wxgrid->SetCellTextColour(t,8,wxColour(globalForcelog[i].count,globalForcelog[i].count,globalForcelog[i].count));
  //                              wxgrid->SetCellTextColour(t,9,wxColour(globalForcelog[i].count,globalForcelog[i].count,globalForcelog[i].count));
    //                            wxgrid->SetCellValue(t,9,_T(wxString::Format(wxT("%2i"),anarace->location[0].force[i])));
      //                          wxgrid->SetCellValue(t,8,_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)));
                                t++;			
			}
		for(i=0;i<=GAS_SCV;i++)
		{
//			wxgrid->SetCellTextColour(i,8,wxColour(0,0,0));wxgrid->SetCellValue(i,8, wxT(" "));
//			wxgrid->SetCellTextColour(i,9,wxColour(0,0,0));wxgrid->SetCellValue(i,9, wxT(" "));
		}
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
//        SetStatusText(_T(wxString::Format(wxT("%s [%i]\r\n"), replay.m_oHeader.getGameName(),replay.m_oActions.GetActionCount())));
}

void MyFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
	if(run==0) run=1;
}

void MyFrame::OnStop(wxCommandEvent& WXUNUSED(event))
{
	if(run==1) run=0;
}

                                                                                                                                                            
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the About dialog of the minimal sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);
                                                                                                                                                            
    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}



