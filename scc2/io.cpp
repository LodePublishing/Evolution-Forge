#include <stdio.h>
#include "../scc2dll/main.h"
#include "io.h"
#include "../scc2dll/text.h"

void setColor(int c)
{
	#ifdef WIN32
// windows console buffer
	SetConsoleTextAttribute(scr,colorsWin32[c-31]);
	#elif __linux__
// ANSI :-)
		printf("\033[0;%d;40m",c);
	#endif
};

void setAt(int cnt)
{
	if(cnt<4) setColor(37);			//White
	else if(cnt<8) setColor(36);	//Cyan
	else if(cnt<16) setColor(32);	//Green
	else if(cnt<32) setColor(33);	//Orange
	else if(cnt<64) setColor(31);	//Red
	else setColor(34);				//Blue
};

#ifdef WIN32
	HANDLE scr;
	void print(const char* x) {DWORD num; WriteConsole(scr,x,strlen(x),&num,0); }
#endif


IO::IO(const GA* ga)
{
    pFitnessGraphicsCounter=0;
    pFitnessDifference=0;
    sFitnessGraphicsCounter=0;
    sFitnessDifference=0;
	tFitnessGraphicsCounter=0;
	tFitnessDifference=0;
    oldsFitness=0;
    oldpFitness=0;
	oldtFitness=0;
	calc=0;
	this->ga=ga;
#ifdef WIN32 // in windows we have create ...
	scr=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CONSOLE_TEXTMODE_BUFFER, 0);
	SetConsoleActiveScreenBuffer(scr);
#endif
	clrscr();
}

IO::~IO()
{
	#ifdef WIN32 // ... and close a console handle
		CloseHandle(scr);
	#endif
}


void IO::screen(ANARACE* anaplayer)
{
	int s,t;
	char tmp[255];

	gotoxy(0,0);setColor(37);
	print("Calculating Status:");

// increase the counters that will determine the color of the fitness display
	if(pFitnessGraphicsCounter<160) pFitnessGraphicsCounter++;
	if(sFitnessGraphicsCounter<160) sFitnessGraphicsCounter++;
	if(tFitnessGraphicsCounter<160) tFitnessGraphicsCounter++;

// the new anaplayer has an improved fitness? Update it and reset the counter
	if(anaplayer->getMaxtFitness()>oldtFitness)
	{
		tFitnessGraphicsCounter=0;
		tFitnessDifference=anaplayer->getMaxtFitness()-oldtFitness;
		oldtFitness=anaplayer->getMaxtFitness();
	}
	if(anaplayer->getMaxsFitness()>oldsFitness)
	{
		sFitnessGraphicsCounter=0;
		sFitnessDifference=anaplayer->getMaxsFitness()-oldsFitness;
		oldsFitness=anaplayer->getMaxsFitness();
	}

// the new anaplayer has a found a valid solution?
	if(anaplayer->getTimer()<ga->maxTime)
	{
		// ... and improved the primary fitness
		if(anaplayer->getMaxpFitness()>oldpFitness)
		{
			pFitnessGraphicsCounter=0;
			pFitnessDifference=anaplayer->getMaxpFitness()-oldpFitness;
			oldpFitness=anaplayer->getMaxpFitness();
		}
	}
	else // no valid solution ...
	{
// ... but perhaps a new goal was found? (this is hacked and probably wrong if two goals have improved but not both are completed)
		if((oldpFitness/100)<(anaplayer->getMaxpFitness()/100))
       {
                  pFitnessGraphicsCounter=0;
                  pFitnessDifference=anaplayer->getMaxpFitness()-oldpFitness;
                  oldpFitness=anaplayer->getMaxpFitness();
          }
	}

// primary fitness has priority (the lower the counter, the 'fresher' were the fitness change)
// set the appropriate color with setAt
	if((pFitnessGraphicsCounter<sFitnessGraphicsCounter)&&(pFitnessGraphicsCounter<tFitnessGraphicsCounter))
		setAt(pFitnessGraphicsCounter);
	else if(sFitnessGraphicsCounter<tFitnessGraphicsCounter)
		setAt(sFitnessGraphicsCounter);
	else setAt(tFitnessGraphicsCounter);

// valid solution?
	if(anaplayer->getTimer()<ga->maxTime)
	{
		// primary fitness counter very fresh?
		if(pFitnessGraphicsCounter<=16)
		{
			sprintf(tmp," got faster solution! [-%2i]   ",pFitnessDifference);
			print(tmp);
		}
		else
		// secondary fitness counter very fresh?
		if(sFitnessGraphicsCounter<=16)
		{
			sprintf(tmp," improved resources! [+%4i]   ",sFitnessDifference/100);
			print(tmp);
		}
		else
		// idle message
			print(" optimizing resources . . .    ");
	} // invalid solution ...
	else
	{
		// primary fitness counter very fresh?
		if(pFitnessGraphicsCounter<=16)
		{
			sprintf(tmp," another goal complete! [+%3i]",pFitnessDifference);
			print(tmp);
		}
		else
		// secondary fitness counter very fresh?
		if(sFitnessGraphicsCounter<=16)
		{
			sprintf(tmp," improved resources! [+%4i]  ",sFitnessDifference/100);
			print(tmp);
		}
		else
		// idle message
			print(" searching . . .              ");
	}

// print the generation counter
	setColor(37);
	gotoxy(1,2);sprintf(tmp,"%i runs and %i+ generations remaining. [Total %i generations]      ",ga->maxRuns-anaplayer->getRun(),ga->maxGenerations-anaplayer->getUnchangedGenerations(),anaplayer->getGeneration());
	print(tmp);

// print the fitness counters
	gotoxy(0,4);
	print("[res]  [time]  [nice]");
	gotoxy(0,5);
	print("-------------------------");
	setAt(sFitnessGraphicsCounter);
	gotoxy(0,6);sprintf(tmp,"%5.4i",anaplayer->getMaxsFitness()/100);print(tmp);
	setAt(pFitnessGraphicsCounter);
	gotoxy(8,6);
	if(anaplayer->getTimer()<ga->maxTime) 
	{ 
		sprintf(tmp,"%.2i:%.2i",(ga->maxTime-anaplayer->getTimer())/60,(ga->maxTime-anaplayer->getTimer())%60);
		print(tmp);
	} 
	else 
	{ 
		sprintf(tmp," %4i",anaplayer->getMaxpFitness());
		print(tmp);
	};
	setAt(tFitnessGraphicsCounter);
	gotoxy(16,6);sprintf(tmp,"%3i",anaplayer->getMaxtFitness());print(tmp);

// print the SCC 103 animation
	setColor(COLOR_1);
if((calc%80==0)||(calc%80==1)) setColor(COLOR_3); else if((calc%80==78)||(calc%80==79)||(calc%80==2)||(calc%80==3)) setColor(COLOR_2);else setColor(COLOR_1);
	gotoxy(21,3);print("     ***    ***   ***   *      **     ***");
if((calc%80==2)||(calc%80==3)) setColor(COLOR_3); else if((calc%80==0)||(calc%80==1)||(calc%80==4)||(calc%80==5)) setColor(COLOR_2);else setColor(COLOR_1);
	gotoxy(21,4);print("   *      *     *      *     *  *       *");
if((calc%80==4)||(calc%80==5)) setColor(COLOR_3); else if((calc%80==2)||(calc%80==3)||(calc%80==6)||(calc%80==7)) setColor(COLOR_2); else setColor(COLOR_1);
	gotoxy(21,5);print("    *    *     *      *     *  *      *  ");
if((calc%80==6)||(calc%80==7)) setColor(COLOR_3); else if((calc%80==4)||(calc%80==5)||(calc%80==8)||(calc%80==9)) setColor(COLOR_2); else setColor(COLOR_1);
	gotoxy(21,6);print("    *   *     *      *     *  *       * ");
if((calc%80==8)||(calc%80==9)) setColor(COLOR_3); else if((calc%80==6)||(calc%80==7)||(calc%80==10)||(calc%80==11)) setColor(COLOR_2);else setColor(COLOR_1);
	gotoxy(21,7);print("***     ***   ***   *  *   **     ***  ");
	
// ---- here starts the build order analyzation
	

		//bolog logs how long which build order stands there (you know, the gColors ;-)
		//TODO: recognize moved blocks to stress real Mutations

		setColor(37);
//The Build order List

			t=0;
			for(s=MAX_LENGTH;s--;)
			{
				gotoxy((t/HEIGHT)*WIDTH,9+t%HEIGHT);
				t++;
				setColor(37);
				if(anaplayer->getProgramIsBuilt(s))
				{
// bolog is for logging the time a build order entry remains unchanged on the same place
// There is still some work to do in the core to track movements of whole blocks etc.

					if(bolog[s].order==anaplayer->phaenoCode[s])
					{
						if(bolog[s].count<160)
							bolog[s].count++;
					} else
					{
						bolog[s].count=0;
						bolog[s].order=anaplayer->phaenoCode[s];
					}
		
// Is this entry part of the goal list?
					if(anaplayer->getProgramIsGoal(s))
					{
						setColor(34);print("@");
					} else print(" ");

// print the last error _before_ the item was built (i.e. what was the reason why this item could not be built immediately?)
					setColor(35);
					sprintf(tmp,"%c",error_small_message[anaplayer->getProgramSuccessType(s)]);
					print(tmp);

// print the short name of the build order item
					setAt(bolog[s].count);
					sprintf(tmp,"%s",short_name[anaplayer->getPlayer()->getRace()][anaplayer->phaenoCode[s]].value);
					print(tmp);

// print the supply situation when this unit was built
					setColor(37);
					if(anaplayer->getProgramNeedSupply(s)<100)
						sprintf(tmp," %.2i",anaplayer->getProgramNeedSupply(s));
					else sprintf(tmp,"%3i",anaplayer->getProgramNeedSupply(s));
					print(tmp);

				}
// built==0? => invalid entry
				else print(" ------    "); 
			}

// print out the result of the build order (the unit force) that were there at the end of the build order
		setColor(37);
		t=4;
		gotoxy(66,3);print("Force:");
 // all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
		for(s=0;s<=GAS_SCV;s++)
			if(anaplayer->getPlayer()->goal->allGoal[s]>0)
			{
				if(globalForcelog[s].order==anaplayer->location[0].force[s])
				{
				      if(globalForcelog[s].count<160)
							globalForcelog[s].count++;
				}
				else
				{
					globalForcelog[s].count=0;
					globalForcelog[s].order=anaplayer->location[0].force[s];
				}
				setAt(globalForcelog[s].count);
				gotoxy(70,t);
				sprintf(tmp,"%s:%2i ",short_name[anaplayer->getPlayer()->getRace()][s].value,anaplayer->location[0].force[s]);
				print(tmp);
				t++;
				if(t>23) break;
				//Well... console can't print out more than ~20 goals :/
			}
		if(t<=23) 
		{
			gotoxy(66,t);
			setColor(37);
			print("non-goals:");
			t++;
			for(s=0;s<=GAS_SCV;s++)
				if((anaplayer->getPlayer()->goal->allGoal[s]==0)&&(anaplayer->location[0].force[s]))
				{
					if(globalForcelog[s].order==anaplayer->location[0].force[s])
					{
						if(globalForcelog[s].count<160)
							globalForcelog[s].count++;
					}
					else
					{
						globalForcelog[s].count=0;
						globalForcelog[s].order=anaplayer->location[0].force[s];
					}
					setAt(globalForcelog[s].count);
					gotoxy(70,t);
					sprintf(tmp,"%s:%2i ",short_name[anaplayer->getPlayer()->getRace()][s].value,anaplayer->location[0].force[s]);
					print(tmp);
					t++;
					if(t>23) break;
				}
		}
		calc++; //Graphics counter
};

