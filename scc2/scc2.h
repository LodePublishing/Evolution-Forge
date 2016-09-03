#ifndef __EC2_H
#define __EC2_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
                                                                                                                                                            
// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "/home/clawg/work/sc1037/sc/icon.xpm"
//    #include "scc.png"
#endif

#include <wx/spinctrl.h>
#include <wx/button.h>
enum
{
        EC_Version=107,
    // menu items
        EC_Open=1,
        EC_Start=2,
        EC_Stop=3,
        EC_Quit = 4,
        EC_GeneralSettings = 5,
        EC_SettingsDialog=101,
                                                                                
        EC_SpinMaxTime=301,
        EC_SpinMaxTimeOut=302,
        EC_SpinMaxLength=303,
        EC_SpinMaxRuns=304,
        EC_SpinMaxGenerations=305,
        EC_SpinBreedFactor=306,
        EC_SpinCrossOver=307,
        EC_CheckPreprocess=308,
                                                                                
        EC_GoalCreate=401,
        EC_GoalImport=402,
                                                                                
        EC_MapCreate=501,
        EC_MapImport=502,
                                                                                
    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    EC_About = wxID_ABOUT
};

#ifdef __WIN32__                                                                                
const int FONT_SIZE=8;
#else
const int FONT_SIZE=9;
#endif


// this is mainly for for io to display some of the output
const char error_message[ERROR_MESSAGES][25]=
{
        "possible.", //= OK
        "enough minerals.", 
        "enough gas.",
        "supply is satisfied.",
        "prerequisite",
        "facility",
        "hell freezes.", //timeout
        "SC2 comes out." //unknown
};
/*char error_small_message[ERROR_MESSAGES]=
{
        'O','M','G','S','P','F','T','U'
};*/
                                                                                    


const int SECOND_COLOUMN=290;
const int SECOND_COLOUMN_WIDTH=250;
const int THIRD_COLOUMN=570;
const int BUILD_ORDER_NUMBER=37;
const int FORCE_LIST_NUMBER=23;

const int BUILD_ORDER_GRAPH_LENGTH=FORCE_LIST_NUMBER*(FONT_SIZE+5)+3;//BUILD_ORDER_GRAPH_NUMBER*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;

const int FORCE_LIST_LENGTH=60+3+BUILD_ORDER_GRAPH_LENGTH;//FORCE_LIST_NUMBER*(FONT_SIZE+5)+3;
const int BUILD_ORDER_GRAPH_NUMBER=10; 
//const int BUILD_ORDER_GRAPH_LENGTH=BUILD_ORDER_GRAPH_NUMBER*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;
const int MIN_HEIGHT=2;

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------
                                                                                                                                                            
    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};
                                                                                                                                                           
struct BOLOG
{
        int count;
        int order;
};  //this is for logging the build order



struct OLDORDER
{
	
	int blend;
	int blendTarget;

//build order list
	int x,y; //current x,y
	int targetx,targety;
	int dx,dy;

//build order graph
	int bx,by;
	int targetbx,targetby;

	int height,width;
	int targetheight,targetwidth;
	
	int unit,mins,gas,time,location,needSupply,haveSupply,forceFacilityCount,availibleFacilityCount,successType,successUnit,facility,code,forceCount;
	int marker,bonew;
//	int mins, color  etc.
};

class MyDCWindow : 
//public wxWindow
public wxScrolledWindow
{
public:
        MyDCWindow(wxFrame *parent);
        void OnEraseBackground(wxEraseEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnIdle(wxIdleEvent& WXUNUSED(event));
	void OnMouseMove(wxMouseEvent& event);
        SETTINGS settings;
	int run;
private:
        const GA* ga;
        BOLOG bolog[MAX_LENGTH];
        BOLOG globalForcelog[UNIT_TYPE_COUNT];
        wxBitmap bmpGraph,bmpTimer,bmpBack,bmpBack2;
        void showGraph(int* data,int max,wxColour col);
        void showForceListBack();
	void showTimer();
	void showProgramGraph();
	wxMemoryDC* dc;
        ANARACE* anarace;
        
	int infoWindow,infoWindowX,infoWindowY;
	int update;
        int sFitness[200];
        int pFitness[200];
        int tFitness[200];
        int aFitness[200];
	int vFitness[200];
	int length[200];
        int time[200];
        int force[200];

        int oldTimeCounter[20],oldTime[20];
	
	OLDORDER* oldOrder[MAX_LENGTH*50];
//	int oldMarker[MAX_LENGTH];

//	int oldBuildOrders[BUILD_ORDER_NUMBER];
	int oldForceList[FORCE_LIST_NUMBER];
	//int oldData[8];

        int maxsFitness;
        int maxpFitness;
        int maxtFitness;
	int mintFitness;
        int maxForce; //all units
        int maxUnitForce; //single unit
        wxFont font,font2;
        DECLARE_EVENT_TABLE()
};
                                                                                


// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
            long style = wxDEFAULT_FRAME_STYLE);

// event handlers (these functions should _not_ be virtual)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);
	void OnStart(wxCommandEvent& event);
	void OnStop(wxCommandEvent& event);
	void OnGeneralSettings(wxCommandEvent& event);

	
        void OnSettingsDialogApply();

	void OnGoalCreate(wxCommandEvent& event);
        void OnGoalImport(wxCommandEvent& event);
        void OnMapCreate(wxCommandEvent& event);
        void OnMapImport(wxCommandEvent& event);
private:
	MyDCWindow* child;
	wxBitmap bitmap;

        wxMenu* menuFile;
        wxMenu* menuHelp;
	wxMenu* menuSettings;
	wxMenu* menuGoal;
	wxMenu* menuMap;
	wxMenu* menuAdvanced;

	wxDialog* dia;

	wxSpinCtrl* spin1;
	wxSpinCtrl* spin2;
	wxSpinCtrl* spin3;
	wxSpinCtrl* spin4;
	wxSpinCtrl* spin5;
	wxSpinCtrl* spin6;
	wxSpinCtrl* spin7;
	wxCheckBox* check1;
	
	wxButton* but1;
	wxButton* but2;
	wxButton* but3;
	wxButton* but4;

	wxListBox* lb1;
	wxListBox* lb2;

	wxDialog* msgBox;

    // any class wishing to process wxWindows events must use this macro
        DECLARE_EVENT_TABLE()
};

#endif

