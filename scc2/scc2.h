#ifndef __SCC2_H
#define __SCC2_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
                                                                                                                                                            
// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mondrian.xpm"
    #include "scc.png"
#endif

#include <wx/spinctrl.h>
#include <wx/button.h>                                                                                                                                                            
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
        void OnIdle(wxIdleEvent& WXUNUSED(event));
	void OnPaint(wxPaintEvent& event);
	void OnGeneralSettings(wxCommandEvent& event);
	
        void OnSettingsDialogApply();

	void OnGoalCreate(wxCommandEvent& event);
        void OnGoalImport(wxCommandEvent& event);
        void OnMapCreate(wxCommandEvent& event);
        void OnMapImport(wxCommandEvent& event);

private:
        SETTINGS settings;
        ANARACE* anarace;
        int run,update;
        int sFitness[200];
        int pFitness[200];
        int tFitness[200];
	int aFitness[200];
	int time[200];
	int force[200];
	int oldTimeCounter[10],oldTime[10];
	int maxsFitness;
	int maxpFitness;
	int maxtFitness;
	int maxForce; //all units
	int maxUnitForce; //single unit

	wxFont font;	

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

	const GA* ga;
        BOLOG bolog[MAX_LENGTH];
        BOLOG globalForcelog[UNIT_TYPE_COUNT];
	wxBitmap bmpGraph,bmpTimer,bmpBack,bmpBack2,bitmap;
	void showGraph(wxDC* dec,int* data,int max,wxColour col);
	
    // any class wishing to process wxWindows events must use this macro
        DECLARE_EVENT_TABLE()
};


#endif

