#ifndef __EF2_H
#define __EF2_H

#include "wx/wxprec.h"
#ifndef WX_PREFOMP
    #include "wx/wx.h"
#endif

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "/home/clawg/work/sc1041/sc/icon.xpm"
//    #include "scc.png"
#endif

//#include <wx/spinctrl.h>
//#include <wx/button.h>
#include <wx/datetime.h>

#include "graphics.h"
#include "list.h"
#include "player.h"
#include "message.h"

enum
{
        EF_Version=108,
    // menu items
        EF_Open=1,
        EF_Start=2,
        EF_Stop=3,
        EF_Quit = 4,
        EF_GeneralSettings = 5,
        EF_SettingsDialog=101,
                                                                                
        EF_SpinMaxTime=301,
        EF_SpinMaxTimeOut=302,
        EF_SpinMaxLength=303,
        EF_SpinMaxRuns=304,
        EF_SpinMaxGenerations=305,
        EF_SpinBreedFactor=306,
        EF_SpinCrossOver=307,
        EF_CheckPreprocess=308,
                                                                                
        EF_GoalCreate=401,
        EF_GoalImport=402,
                                                                                
        EF_MapCreate=501,
        EF_MapImport=502,
                                                                                
    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    EF_About = wxID_ABOUT
};

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

const int GIZMO_NUMBER=13;

const char gizmo[GIZMO_NUMBER][40]=
{
	"Perfection is the key",
	"Look at la Luna",
	"It is a good day to optimize",
	"The early ling catches the worm",
	"Build orders are best served optimized",
	"Pool Pool Pool Pool Dead - A newbie",
	"Good... Bad... I'm the guy with the gum",
	"LOOOOOOOOOOOOOOOOOOOOOOOOOL",
	"ALT+F4 - Zergling has left the game :/",
	"WHY ARE YOUR ZEALOTS INVISIBLE!?",
	"!!!!! 3VS3 BGH NO NOOBS !!!!",
	"<ý¦<¬ ;) ýJ<¼ýD Ìýt<Üý >_<",
	"60 Minutes no Rush!"
};
                                                                                    



//const int BUILD_ORDER_NUMBER=36;

//const int BUILD_ORDER_GRAPH_LENGTH=FORCE_LIST_NUMBER*(FONT_SIZE+5)+3;//BUILD_ORDER_GRAPH_NUMBER*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;

//const int FORCE_LIST_LENGTH=60+3+BUILD_ORDER_GRAPH_LENGTH;//FORCE_LIST_NUMBER*(FONT_SIZE+5)+3;
//const int BUILD_ORDER_GRAPH_NUMBER=10; 
//const int BUILD_ORDER_GRAPH_LENGTH=BUILD_ORDER_GRAPH_NUMBER*(FONT_SIZE+5)+3+FORCE_LIST_LENGTH;

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
                                                                                                                                                           

class MyDCWindow:public wxWindow
{
public:
        MyDCWindow(wxFrame *parent);
	~MyDCWindow();
        void OnEraseBackground(wxEraseEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnIdle(wxIdleEvent& WXUNUSED(event));
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
        void OnMouseScroll(wxMouseEvent& event);
	void OnTimer(wxTimerEvent& event);
        SETTINGS settings;
	void resetData();
	void showToolTip();
	GraphixScrollWindow* mainWindow;
	MessageWindow* msgWindow;
	GraphixScrollWindow* haxor;
	GraphixScrollWindow* infoWindow;
	GraphixScrollWindow* tutorialWindow;
	Player* player[MAX_PLAYER];
	GraphixScrollWindow* theCore;
	int isOptimizing();
	void stopOptimizing();
private:
	wxTimer wxtimer;
	int userIsNewbie;
	
	int mouseLeft;
	int oldrun,endrun;
        GA* ga;
        BOLOG bolog[MAX_LENGTH];
        wxBitmap bmpGraph,bmpTimer,bmpBack,bmpBack2,bmpCancel,bmpAdd,bmpSub;
	wxBitmap bmpArrowLeft,bmpArrowUp,bmpArrowRight,bmpArrowDown;
	void processButtons();
	void drawGizmo();
	void showCoreAnimation();
	wxMemoryDC* dc;
        ANARACE* anarace[MAX_PLAYER];
        


	int animationNumbers;
	int update;

//	wxDateTime dt1;
//	wxDateTime dt2;
	int refresh;

	int tutorialChapter;

	
wxBitmap bitmap,bitmap2,bitmap3,hintBitmap,bmpNumbers,bmpRadiation,bmpAlpha,bmpTreppe,bmpCloning,bmpFitness;
	wxBitmap bmpRad[5];
	wxBitmap bmpHeart[5];
	int ani;
	int tutorialAnimation;
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
//        void OnQuit(wxCommandEvent& event);
/*        void OnAbout(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);*/
//	void OnStart(wxCommandEvent& event);
//	void OnStop(wxCommandEvent& event);
//	void OnGeneralSettings(wxCommandEvent& event);

	
/*        void OnSettingsDialogApply();

	void OnGoalCreate(wxCommandEvent& event);
        void OnGoalImport(wxCommandEvent& event);
        void OnMapCreate(wxCommandEvent& event);
        void OnMapImport(wxCommandEvent& event);*/
private:
	MyDCWindow* child;
//	wxBitmap bitmap;

/*      wxMenu* menuFile;
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

	wxDialog* msgBox;*/

    // any class wishing to process wxWindows events must use this macro
//        DECLARE_EVENT_TABLE()
};

#endif

