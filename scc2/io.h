#ifndef __IO_H
#define __IO_H

#include "../scc2dll/anarace.h" 
#include "../scc2dll/ga.h" 

#define HEIGHT 16	// mumber of entries (build order list)
#define WIDTH 11	// witdh of the entries in characters
#define COLOR_1 34  // colors of the SCC 103 animation
#define COLOR_2 36
#define COLOR_3 37

extern void setColor(int c); // sets current color
extern void setAt(int cnt);  // sets color in a logarithmic series

#ifdef WIN32
	#include <windows.h>
	extern HANDLE scr;
	//Translate Linux ANSI Colors to SetConsoleTextAttribute Colors
	const unsigned char colorsWin32[7]= 
	{
		FOREGROUND_RED,FOREGROUND_GREEN,FOREGROUND_RED|FOREGROUND_GREEN,FOREGROUND_BLUE,FOREGROUND_RED|FOREGROUND_BLUE,FOREGROUND_GREEN|FOREGROUND_BLUE,FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN
	};
	extern void print(const char* x); //prints a simple string (printf does not work with a windows console handle)
	#define clrscr() system("cls") //clear screen
	#define gotoxy(x,y) {COORD pos={(x),(y)}; SetConsoleCursorPosition(scr,pos); }

#elif __linux__ // ANSI codes, not supported by windows console
	#define clrscr() printf("\033[2J") //clear screen
	#define gotoxy(x,y) printf("\033[%d;%dH",(y),(x))
	#define print(x) printf((x))
#endif

struct BOLOG
{
	int count;
	int order;
};  //this is for logging the build order 

class IO
{
	private:
		//all these counter are to keep track when each entry was changed
		//they effect the line at the top (the text "new goal found", "searching", etc. and its color)
		int pFitnessGraphicsCounter;
        int pFitnessDifference;
        int sFitnessGraphicsCounter;
        int sFitnessDifference;
        int tFitnessGraphicsCounter;
        int tFitnessDifference;
        int oldsFitness;
        int oldpFitness;
		int oldtFitness;

		//just a temporary variable for the SCC 103 animation
        int calc;

		
		//bolog logs how long which build order stands there on the screen
		//the longer the entry is not changed, the darker is the color
		BOLOG bolog[MAX_LENGTH];

		//same as bolog, the force list on the right
		BOLOG globalForcelog[UNIT_TYPE_COUNT];

		//pointer to the basic 'genetic algorithms' settings
		const GA* ga;
	public:
		void screen(ANARACE* anaplayer);
		IO(const GA* ga);
		~IO();
};

#endif
