#include "statistics.hpp"
#include "../core/settings.hpp"
#include <sys/time.h>

enum eStatEntry
{
	FORCE_STAT_ENTRY,
	TIME_STAT_ENTRY,
	MINERALS_STAT_ENTRY,
	GAS_STAT_ENTRY,
	FITNESS_AVERAGE_STAT_ENTRY,
	FITNESS_VARIANCE_STAT_ENTRY,
	AVERAGE_BO_LENGTH_STAT_ENTRY,
	GENERATIONS_LEFT_STAT_ENTRY,
	MAX_STAT_ENTRY
};


StatisticsWindow::StatisticsWindow(UI_Object* parent, const ANARACE* anarace, const int windowNumber):UI_Window(parent, STATISTICS_WINDOW_TITLE_STRING, STATISTICS_WINDOW, windowNumber, NOT_SCROLLED)
{
	this->anarace=anarace;
	resetData();
    timeval tim;
    gettimeofday(&tim, NULL);
    start_time=tim.tv_sec*1000000+tim.tv_usec;

	Point l1=Point(getRelativeClientRectRightBound() - 190, getRelativeClientRectUpperBound());
    Size l2=Size(180,FONT_SIZE+4);
		
	for(int i=0;i<MAX_STAT_ENTRY;i++)
		statEntry[i] = new UI_Button(this, Rect(l1+Point(0,i*12), l2), Rect(l1+Point(0,i*12), l2), (eString)(FORCE_STAT_STRING+i), (eString)(FORCE_STAT_STRING+i), (eButton)(FORCE_STAT_BUTTON+i), NO_TEXT_MODE, NO_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE);

	statEntry[FORCE_STAT_ENTRY]->updateToolTip(*theme.lookUpString(FORCE_STAT_TOOLTIP_STRING));
	statEntry[TIME_STAT_ENTRY]->updateToolTip(*theme.lookUpString(TIME_STAT_TOOLTIP_STRING));
	statEntry[MINERALS_STAT_ENTRY]->updateToolTip(*theme.lookUpString(MINERALS_STAT_TOOLTIP_STRING));
	statEntry[GAS_STAT_ENTRY]->updateToolTip(*theme.lookUpString(GAS_STAT_TOOLTIP_STRING));
	statEntry[FITNESS_AVERAGE_STAT_ENTRY]->updateToolTip(*theme.lookUpString(FITNESS_AVERAGE_STAT_TOOLTIP_STRING));
	statEntry[FITNESS_VARIANCE_STAT_ENTRY]->updateToolTip(*theme.lookUpString(FITNESS_VARIANCE_STAT_TOOLTIP_STRING));
	statEntry[AVERAGE_BO_LENGTH_STAT_ENTRY]->updateToolTip(*theme.lookUpString(AVERAGE_BO_LENGTH_STAT_TOOLTIP_STRING));
	statEntry[GENERATIONS_LEFT_STAT_ENTRY]->updateToolTip(*theme.lookUpString(GENERATIONS_LEFT_STAT_TOOLTIP_STRING));

	mspf = new UI_StaticText(this, Rect(20, 40, /*getRelativeClientRectLeftBound()+getClientRectWidth()-60, getRelativeClientRectUpperBound()+getClientRectHeight()-35,*/ 0, 0), NO_TEXT_MODE, BRIGHT_TEXT_COLOUR);
	fps = new UI_StaticText(this,  Rect(20, 50, /*getRelativeClientRectLeftBound()+getClientRectWidth()-60, getRelativeClientRectUpperBound()+getClientRectHeight()-25,*/ 0, 0), NO_TEXT_MODE, BRIGHT_TEXT_COLOUR);
	bps = new UI_StaticText(this,  Rect(20, 60, /*getRelativeClientRectLeftBound()+getClientRectWidth()-60, getRelativeClientRectUpperBound()+getClientRectHeight()-15,*/ 0, 0), NO_TEXT_MODE, BRIGHT_TEXT_COLOUR);
}

StatisticsWindow::~StatisticsWindow()
{
	for(int i=0;i<MAX_STAT_ENTRY;i++)
		delete statEntry[i];
	delete mspf;
	delete fps;
	delete bps;
}
																


void StatisticsWindow::showGraph(DC* dc, const int* data, const int min, const int max, const Color col) const
{
	int j,k;
	dc->SetPen(Pen(col,1,SOLID_PEN_STYLE));
	Point blub[200];
	j=1;
	k=0;
	for(int i=1;i<199;i++)
	{
		if((data[i]!=data[i+1])||(k>9))
		{
			k=0;
		//TODO Hoehe 100 in clientarea.height aendern
			blub[j]=Point(i+getAbsoluteClientRectLeftBound()+5,getAbsoluteClientRectUpperBound()+100-100*(data[i]-min)/(max+1));
			j++;
		} else k++;
	}
	blub[0]=Point(getAbsoluteClientRectPosition()+Point(5,100-100*(data[0]-min)/(max+1)));
	blub[j]=Point(getAbsoluteClientRectPosition()+Point(204,100-100*(data[199]-min)/(max+1)));
	dc->DrawSpline(j+1,blub);
}

void StatisticsWindow::resetData()
{
	averagecounter=0;
	for(int i=100;i--;)
		average[i]=0;
																				
	maxpFitness=100;
	maxsFitness=100;
	maxtFitness=100;
	maxHarvestedRessources=1000;
	mintFitness=99999;
	for(int i=200;i--;)
	{
		pFitness[i]=0;
		harvestedMinerals[i]=0;
		harvestedGas[i]=0;
		tFitness[i]=0;
		aFitness[i]=0;
		vFitness[i]=0; //~~
		alength[i]=0; //ok
		time[i]=0;
		force[i]=5; //5 units at the beginning!
		generation[i]=settings.getGa()->maxGenerations;
	};
																				
	for(int i=20;i--;)
	{
		oldForceCounter[i]=0;
		oldForce[i]=0;
		oldGasCounter[i]=0;
		oldGas[i]=0;
		oldMineralsCounter[i]=0;
		oldMinerals[i]=0;
	}
}

void StatisticsWindow::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Window::draw(dc);


	
/*	Point Legend2=Point(getAbsoluteClientRectPosition()+Point(210,0));
	Point Legend3=Point(180,FONT_SIZE+4);
																				
	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)); // TODO
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN)); // TODO
	
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y,Legend3.x,Legend3.y,4);


	for(int k=0;k<20;k++)
		if(oldForceCounter[k]>0)
		{
			string bla=theme.lookUpFormattedString(FORCE_STRING, force[199]);
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
			dc->SetTextForeground(/*theme.lookUpColor(WINDOW_TEXT_COLOUR));
					Color(dc->GetSurface(), 40,200-oldForceCounter[k]*8,200-oldForceCounter[k]*8));
			ostringstream os;
			if(oldForce[k]>0) os << "+";
			os << oldForce[k];
			dc->DrawText(os.str(),Legend2.x+dx+oldForceCounter[k],Legend2.y);
		}
																			
																				
	dc->SetTextForeground(*theme.lookUpColor(STATISTICS_FORCE_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(FORCE_STRING, force[199]),Legend2.x,Legend2.y);
//	addButton(Rect(Legend2.x, Legend2.y, Legend3.x, Legend3.y), "total number of units/buildings at the end"); TODO
																				
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(*theme.lookUpColor(TIME_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(TIME_STRING,time[199]%60,time[199]/60),Legend2.x, Legend2.y+12);
//	addButton(Rect(Legend2.x, Legend2.y+12, Legend3.x, Legend3.y), "time of the best build order"); TODO
	
																				
//	  dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
//	  dc->SetTextForeground(dc->doColor(255,40,255));
//	  dc->DrawText(_T(String::Format(T("Tertiary fitness : %i"),tFitness[199])),Legend2.x,Legend2.y+12);
																				
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+24,Legend3.x,Legend3.y,4);
	for(int k=0;k<20;k++)
		if(oldMineralsCounter[k]>0)
		{
//			dc->SetTextForeground(*theme.lookUpColor(WINDOW_TEXT_COLOUR));
			dc->SetTextForeground(Color(dc->GetSurface(), 40, 100-oldMineralsCounter[k]*3, 200-oldMineralsCounter[k]*8));
			string bla=theme.lookUpFormattedString(MINERALS_STRING, harvestedMinerals[199]/100);
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);

			ostringstream os;
			if(oldMinerals[k]>0) os << "+";
			os << oldMinerals[k]/100;
			dc->DrawText(os.str(),Legend2.x+dx+oldMineralsCounter[k],Legend2.y+24);
		}
																				
	dc->SetTextForeground(*theme.lookUpColor(MINERALS_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(MINERALS_STRING, harvestedMinerals[199]/100),Legend2.x,Legend2.y+24);
//	addButton(Rect(Legend2.x, Legend2.y+24, Legend3.x, Legend3.y), "total harvested minerals at the end"); TODO
	
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+36,Legend3.x,Legend3.y,4);
	for(int k=0;k<20;k++)
		if(oldGasCounter[k]>0)
		{
//			dc->SetTextForeground(*theme.lookUpColor(MINERALS_TEXT_COLOUR)); // TODO
			dc->SetTextForeground(Color(dc->GetSurface(), 40,200-oldGasCounter[k]*8,40));
			string bla=theme.lookUpFormattedString(GAS_STRING, harvestedGas[199]/100);
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
			ostringstream os;
			if(oldForce[k]>0) os << "+";
			os << oldGas[k]/100;
			dc->DrawText(os.str(),Legend2.x+dx+oldGasCounter[k],Legend2.y+36);
		}
	
*/
//	dc->DrawText(_T(String::Format(T("%i of %.0f billion possible build orders checked [%.6f%%]."),anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1),pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/1000000000.0,(anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1))/pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes()))),haxor->getAbsoluteClientRectLeftBound(),haxor->getAbsoluteClientRectUpperBound());

/*	
		dc->DrawText(_T(String::Format(T("Estimated time using stupid brute-force algorithm: %.5f billion years."),
		pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/(1000000000.0*365.0*24.0*3600000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)/((float)(av))))),haxor->getAbsoluteClientRectLeftBound(),haxor->getAbsoluteClientRectUpperBound()+40);
		if(anarace[0]->getTimer()!=settings.getGa()->maxTime)
		{
			float totaln=1;
			for(int i=0;i<=GAS_SCV;i++)
				if(anarace[0]->getLocationForce(0,i))
		//todo startforce beruecksichtigen... aber erstmal map location[0] fuellen... TODO
				{
					if(i==SCV)
					for(int j=1;j<anarace[0]->getLocationForce(0,i)-4;j++)
						totaln*=j;
					else if(i==COMMAND_CENTER)
					for(int j=1;j<anarace[0]->getLocationForce(0,i)-1;j++)
						totaln*=j;
					else
					for(int j=1;j<anarace[0]->getLocationForce(0,i);j++)
						totaln*=j;
				}
			float totalz=1;
			for(int i=1;i<force[199]-5;i++)
				totalz*=i;
			dc->DrawText(_T(String::Format(T("Estimated time using intelligent brute-force algorithm: %.2f hours."), (totalz*((float)(av)))/(totaln*3600000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)))),haxor->getAbsoluteClientRectLeftBound(),haxor->getAbsoluteClientRectUpperBound()+60);
																				
		}*/
//	}

	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)); // TODO HATCH wieder machen
	dc->DrawRectangle(getAbsoluteClientRectPosition(), Size(getClientRectWidth() - 200, getClientRectHeight()));
//	  showGraph(dc,tFitness,mintFitness,maxtFitness,Color(255,40,255));
	showGraph(dc,harvestedMinerals,0,maxHarvestedRessources,*theme.lookUpColor(MINERALS_TEXT_COLOUR));
	showGraph(dc,harvestedGas,0,maxHarvestedRessources,*theme.lookUpColor(GAS_TEXT_COLOUR));
	showGraph(dc,generation,0,settings.getGa()->maxGenerations,*theme.lookUpColor(IMPORTANT_COLOUR));
	showGraph(dc,force,0,maxForce/*anarace->getUnitsTotal()*/,*theme.lookUpColor(STATISTICS_FORCE_TEXT_COLOUR));
	showGraph(dc,alength,0,MAX_LENGTH,*theme.lookUpColor(BOLENGTH_TEXT_COLOUR));
	showGraph(dc,time,0,settings.getGa()->maxTime,*theme.lookUpColor(TIME_TEXT_COLOUR));
	showGraph(dc,aFitness,0,maxaFitness,*theme.lookUpColor(FITNESS_AVERAGE_TEXT_COLOUR));
	showGraph(dc,vFitness,0,maxvFitness,*theme.lookUpColor(FITNESS_VARIANCE_TEXT_COLOUR));
//	  showGraph(dc,pFitness,maxpFitness,dc->doColor(255,40,40));

}

// TODO ueberpruefen ob is shown
void StatisticsWindow::process()
{
	if(!shown)
		return;
	
	for(int i=0;i<199;i++)
	{
		pFitness[i]=pFitness[i+1];
		harvestedMinerals[i]=harvestedMinerals[i+1];
		harvestedGas[i]=harvestedGas[i+1];
		tFitness[i]=tFitness[i+1];
		aFitness[i]=aFitness[i+1];
		vFitness[i]=vFitness[i+1];
		alength[i]=alength[i+1];
		time[i]=time[i+1];
		force[i]=force[i+1];
		generation[i]=generation[i+1];
	};
																			
																			
	pFitness[199]=anarace->getMaxpFitness();
	harvestedMinerals[199]=anarace->getHarvestedMinerals();
	harvestedGas[199]=anarace->getHarvestedGas();
	tFitness[199]=anarace->getMaxtFitness();
	aFitness[199]=anarace->fitnessAverage;
	vFitness[199]=(int)sqrt((double)anarace->fitnessVariance);
	alength[199]=anarace->getAverageLength();
	time[199]=anarace->getRealTimer();
	force[199]=anarace->getUnitsTotal();
	generation[199]=settings.getGa()->maxGenerations - anarace->getUnchangedGenerations();

	maxpFitness=0;
	maxsFitness=0;
	maxtFitness=0;
	maxvFitness=0;
	maxaFitness=0;
	mintFitness=99999;
	maxHarvestedRessources=0;
	maxForce=0;

	for(int i=0;i<200;i++)
	{
		if(harvestedMinerals[i]>maxHarvestedRessources)
			maxHarvestedRessources=harvestedMinerals[i];
		if(harvestedGas[i]>maxHarvestedRessources)
			maxHarvestedRessources=harvestedGas[i];
		if(pFitness[i]>maxpFitness)
			maxpFitness=pFitness[i];
//		if(sFitness[i]>maxsFitness)
//			maxsFitness=sFitness[i];

		if(aFitness[i]>maxaFitness)
			maxaFitness=aFitness[i];
		if(vFitness[i]>maxvFitness)
			maxvFitness=vFitness[i];
		if(tFitness[i]>maxtFitness)
			maxtFitness=tFitness[i];
	
		if(tFitness[i]<mintFitness)
			mintFitness=tFitness[i];
		if(force[i]>maxForce)
			maxForce=force[i];
	};

	int l=0;
	for(int k=0;k<20;k++)
	{
		if(oldForceCounter[k]>0)
		{
			oldForceCounter[k]+=2;
			if(oldForceCounter[k]>20)
			{
				oldForceCounter[k]=0;
				oldForce[k]=0;
			}
		}
		if(oldForceCounter[k]==0)
			l=k;
	}
	if(force[199]!=force[198])
	{
		oldForceCounter[l]=1;
		oldForce[l]=force[199]-force[198];
	}

	for(int k=0;k<20;k++)
	{
		if(oldGasCounter[k]>0)
		{
			oldGasCounter[k]+=2;
			if(oldGasCounter[k]>20)
			{
				oldGasCounter[k]=0;
				oldGas[k]=0;
			}
		}
		if(oldGasCounter[k]==0)
			l=k;
	}
	if(harvestedGas[199]!=harvestedGas[198])
	{
		oldGasCounter[l]=1;
		oldGas[l]=harvestedGas[199]-harvestedGas[198];
	}
	
	for(int k=0;k<20;k++)
	{
		if(oldMineralsCounter[k]>0)
		{
			oldMineralsCounter[k]+=2;
			if(oldMineralsCounter[k]>20)
			{
				oldMineralsCounter[k]=0;
				oldMinerals[k]=0;
			}
		}
		if(oldMineralsCounter[k]==0)
			l=k;
	}
	if(harvestedMinerals[199]!=harvestedMinerals[198])
	{
		oldMineralsCounter[l]=1;
		oldMinerals[l]=harvestedMinerals[199]-harvestedMinerals[198];
	}
	for(int i=0;i<20;i++)
		for(int j=0;j<i;j++)
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
			if(oldMineralsCounter[j]<oldMineralsCounter[i])
			{
				int tee=oldMineralsCounter[i];
				oldMineralsCounter[i]=oldMineralsCounter[j];
				oldMineralsCounter[j]=tee;
				tee=oldMinerals[i];
				oldMinerals[i]=oldMinerals[j];
				oldMinerals[j]=tee;
			}
		}
	timeval tim;
    gettimeofday(&tim, NULL);
	long int new_time=tim.tv_sec*1000000+tim.tv_usec;
	long int difference = (new_time - start_time)/1000;
	start_time=new_time;

	if(averagecounter<100)
		averagecounter++;
	for(int i=averagecounter-1;i--;)
		average[i+1]=average[i];
	average[0]=difference;
	
	int av=0;
	for(int i=0;i<averagecounter;i++)
		av+=average[i];
	av/=averagecounter;

	statEntry[FORCE_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(FORCE_STAT_STRING, force[199]));
	statEntry[TIME_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(TIME_STAT_STRING, time[199]%60, time[199]/60));
	statEntry[MINERALS_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(MINERALS_STAT_STRING, harvestedMinerals[199]/100));
	statEntry[GAS_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(GAS_STAT_STRING, harvestedGas[199]/100));
	statEntry[FITNESS_AVERAGE_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(FITNESS_AVERAGE_STAT_STRING, aFitness[199], anarace->getMaxpFitness()));
	statEntry[FITNESS_VARIANCE_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(FITNESS_VARIANCE_STAT_STRING, vFitness[199]));
	statEntry[AVERAGE_BO_LENGTH_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(AVERAGE_BO_LENGTH_STAT_STRING, alength[199]));
	statEntry[GENERATIONS_LEFT_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(GENERATIONS_LEFT_STAT_STRING, generation[199]));

	ostringstream os;
	os << av << "ms/frame";
	mspf->updateText(os.str());

	os.str("");
	os << 1000.0/((float)(av)) << " fps [" << UI_Object::rectnumber << " updates]";
	fps->updateText(os.str());
	if(anarace->isOptimizing())
	{
		bps->Show();
		os.str("");
		os << 1000.0*(MAX_PROGRAMS+(*anarace->getMap())->getMaxPlayer()-1)/((float)(av*(1+settings.getSpeed()))) << " bops";
		bps->updateText(os.str());
	} else bps->Hide();

	UI_Window::process();
}

