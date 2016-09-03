#include "statistics.hpp"
#include "../core/configuration.hpp"

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
	FPS_STAT_ENTRY,
	MAX_STAT_ENTRY
};


StatisticsWindow::StatisticsWindow(UI_Object* stat_parent, const ANARACE* stat_anarace, const unsigned int stat_window_number):
	UI_Window(stat_parent, STATISTICS_WINDOW_TITLE_STRING, STATISTICS_WINDOW, stat_window_number, NOT_SCROLLED),
//	fps(new UI_StaticText(this,  Rect(getRelativeClientRectPosition() + Point(332, 102), Size(0, 0)), NO_TEXT_MODE, BRIGHT_TEXT_COLOR)),
//	bps(new UI_StaticText(this,  Rect(getRelativeClientRectPosition() + Point(302, 112), Size(0, 0)), NO_TEXT_MODE, BRIGHT_TEXT_COLOR)),
	start_time(0),
	anarace(stat_anarace),
	maxpFitness(100),
	maxsFitness(100),
	maxtFitness(100),
	mintFitness(99999),
	maxaFitness(0),
	maxvFitness(0),
//	maxForce(0),
	maxHarvestedRessources(1000),
	maxfps(0),
	averagecounter(0)
//    speed(new NumberField(this, Rect(getRelativeClientRectPosition() + Point(210, 111), Size(100, 100)), 0, 100, 1, 25, SPEED_STRING, SETTING_FRAMERATE_TOOLTIP_STRING))
{
	ani=0;
	resetData();
    start_time = SDL_GetTicks();

	Point l1=Point(getRelativeClientRectPosition()+Point(210,0));
    Size l2=Size(180, FONT_SIZE+5);
		
	for(int i=0;i<MAX_STAT_ENTRY;i++)
		statEntry[i] = new UI_Button(this, Rect(l1+Point(0,i*12), l2), Rect(l1+Point(0,i*12), l2), (eString)(FORCE_STAT_STRING+i), (eString)(FORCE_STAT_STRING+i), (eButton)(FORCE_STAT_BUTTON+i), NO_TEXT_MODE, NO_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE);

	statEntry[FORCE_STAT_ENTRY]->updateToolTip(FORCE_STAT_TOOLTIP_STRING);
	statEntry[TIME_STAT_ENTRY]->updateToolTip(TIME_STAT_TOOLTIP_STRING);
	statEntry[MINERALS_STAT_ENTRY]->updateToolTip(MINERALS_STAT_TOOLTIP_STRING);
	statEntry[GAS_STAT_ENTRY]->updateToolTip(GAS_STAT_TOOLTIP_STRING);
	statEntry[FITNESS_AVERAGE_STAT_ENTRY]->updateToolTip(FITNESS_AVERAGE_STAT_TOOLTIP_STRING);
	statEntry[FITNESS_VARIANCE_STAT_ENTRY]->updateToolTip(FITNESS_VARIANCE_STAT_TOOLTIP_STRING);
	statEntry[AVERAGE_BO_LENGTH_STAT_ENTRY]->updateToolTip(AVERAGE_BO_LENGTH_STAT_TOOLTIP_STRING);
	statEntry[GENERATIONS_LEFT_STAT_ENTRY]->updateToolTip(GENERATIONS_LEFT_STAT_TOOLTIP_STRING);
}

StatisticsWindow::~StatisticsWindow()
{
	for(int i=0;i<MAX_STAT_ENTRY;i++)
		delete statEntry[i];
//	delete speed;
}
																				
void StatisticsWindow::showGraph(DC* dc, const unsigned int* data, const unsigned int min, const unsigned int max, const Color col) const
{
	int j,k;
	if(getAbsoluteClientRectPosition().y <0)
		return;
	dc->SetPen(Pen(col, 1, SOLID_PEN_STYLE));
	Point blub[200];
	j=1;
	k=0;
	for(int i=1;i<199;i++)
	{
		if((data[i]!=data[i+1])||(k>9))
		{
			k=0;
		//TODO Hoehe 100 in clientarea.height aendern
			blub[j]=Point(i+getAbsoluteClientRectLeftBound(), getAbsoluteClientRectLowerBound() - 2 - (getClientRectHeight()-4)*(data[i]-min)/(max+1));
			j++;
		} else k++;
	}
	blub[0]=Point(getAbsoluteClientRectPosition() + Point(2, getClientRectHeight() - 3 - (getClientRectHeight()-4)*(data[0]-min)/(max+1)));
	blub[j]=Point(getAbsoluteClientRectPosition() + Point(201, getClientRectHeight() - 3 - (getClientRectHeight()-4)*(data[199]-min)/(max+1)));
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
		generation[i]=configuration.getMaxGenerations();
	};
																				
	for(int i=20;i--;)
	{
//		oldForceCounter[i]=0;
//		oldForce[i]=0;
		oldGasCounter[i]=0;
		oldGas[i]=0;
		oldMineralsCounter[i]=0;
		oldMinerals[i]=0;
//		oldBoLengthCounter[i]=0;
//		oldBoLength[i]=0;
	}
}

void StatisticsWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);

	dc->SetBrush(*theme.lookUpBrush( WINDOW_BACKGROUND_BRUSH ));
	dc->SetPen(*theme.lookUpPen( INNER_BORDER_PEN ));
	dc->DrawRectangle(getAbsoluteClientRectPosition(), Size(202, getClientRectHeight()));

// ----- MINERALS ------
    if(statEntry[MINERALS_STAT_ENTRY]->isCurrentlyHighlighted())
		showGraph(dc,harvestedMinerals,0,maxHarvestedRessources, dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(MINERALS_TEXT_COLOR)));
	else
	    showGraph(dc,harvestedMinerals,0,maxHarvestedRessources,*theme.lookUpColor(MINERALS_TEXT_COLOR));
 
    for(int k=0;k<20;k++)
        if(oldMineralsCounter[k]>0)
        {
			dc->SetTextForeground(dc->darkenColor(theme.lookUpColor(MINERALS_TEXT_COLOR),  80-oldMineralsCounter[k]*4));
            ostringstream os;
            if(oldMinerals[k]>0) 
				os << "+";
            os << oldMinerals[k];
            dc->DrawText(os.str(), statEntry[MINERALS_STAT_ENTRY]->getAbsolutePosition() + Point(statEntry[MINERALS_STAT_ENTRY]->getTextWidth() + oldMineralsCounter[k]-5,3));
        }
// ------ END MINERALS ------

// ------ VESPINE GAS ------
    if(statEntry[GAS_STAT_ENTRY]->isCurrentlyHighlighted())
		showGraph(dc,harvestedGas,0,maxHarvestedRessources, dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(GAS_TEXT_COLOR)));
	else
	    showGraph(dc,harvestedGas,0,maxHarvestedRessources,*theme.lookUpColor(GAS_TEXT_COLOR));

	for(int k=0;k<20;k++)
        if(oldGasCounter[k]>0)
        {
			dc->SetTextForeground(dc->darkenColor(theme.lookUpColor(GAS_TEXT_COLOR),  80-oldGasCounter[k]*4));
            ostringstream os;
            if(oldGas[k]>0) 
				os << "+";
            os << oldGas[k];
            dc->DrawText(os.str(), statEntry[GAS_STAT_ENTRY]->getAbsolutePosition() + Point(statEntry[GAS_STAT_ENTRY]->getTextWidth() + oldGasCounter[k]-5,3));
        }
// ------ END VESPINE GAS ------


// ------ GENERATIONS ------
    if(statEntry[GENERATIONS_LEFT_STAT_ENTRY]->isCurrentlyHighlighted())
		showGraph(dc,generation,0,configuration.getMaxGenerations(), dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(IMPORTANT_COLOR)));
	else
	    showGraph(dc,generation,0,configuration.getMaxGenerations(), *theme.lookUpColor(IMPORTANT_COLOR));
// ------ END GENERATIONS ------


// ------ FORCE ------
//    if(statEntry[FORCE_STAT_ENTRY]->isCurrentlyHighlighted())
//		showGraph(dc,force,0,maxForce/*anarace->getUnitsTotal()*/,dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(STATISTICS_FORCE_TEXT_COLOR)));
//	else
//	    showGraph(dc,force,0,maxForce/*anarace->getUnitsTotal()*/,*theme.lookUpColor(STATISTICS_FORCE_TEXT_COLOR));
/*    for(int k=0;k<20;k++)
        if(oldForceCounter[k]>0)
        {
			dc->SetTextForeground(dc->darkenColor(theme.lookUpColor(FORCE_TEXT_COLOR),  40-oldForceCounter[k]*2));
            ostringstream os;
            if(oldForce[k]>0) 
				os << "+";
            os << oldForce[k];
            dc->DrawText(os.str(), statEntry[FORCE_STAT_ENTRY]->getAbsolutePosition() + Point(statEntry[FORCE_STAT_ETNRY]->getTextWidth() + oldForceCounter[k],0));
        }*/

// ------ END FORCE ------


// ------ BOLENGTH ------
    if(statEntry[AVERAGE_BO_LENGTH_STAT_ENTRY]->isCurrentlyHighlighted())
		showGraph(dc,alength,0,MAX_LENGTH, dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(BOLENGTH_TEXT_COLOR)));
	else
	    showGraph(dc,alength,0,MAX_LENGTH,*theme.lookUpColor(BOLENGTH_TEXT_COLOR));
// ------ END BOLENGTH ------

// ------ TIME ------
    if(statEntry[TIME_STAT_ENTRY]->isCurrentlyHighlighted())
		showGraph(dc, time, 0, configuration.getMaxTime(), dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(TIME_TEXT_COLOR)));
	else
	    showGraph(dc, time, 0, configuration.getMaxTime(), *theme.lookUpColor(TIME_TEXT_COLOR));
// ------ END TIME ------

// ------ AVERAGE FITNESS ------
    if(statEntry[FITNESS_AVERAGE_STAT_ENTRY]->isCurrentlyHighlighted())
		showGraph(dc,aFitness,0,maxaFitness, dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(FITNESS_AVERAGE_TEXT_COLOR)));
	else
	    showGraph(dc,aFitness,0,maxaFitness,*theme.lookUpColor(FITNESS_AVERAGE_TEXT_COLOR));
// ------ END AVERAGE FITNESS ------

// ------ FITNESS VARIANCE ------
    if(statEntry[FITNESS_VARIANCE_STAT_ENTRY]->isCurrentlyHighlighted())
		showGraph(dc,vFitness,0,maxvFitness, dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(FITNESS_VARIANCE_TEXT_COLOR)));
	else
		showGraph(dc,vFitness,0,maxvFitness,*theme.lookUpColor(FITNESS_VARIANCE_TEXT_COLOR));
// ------ END FITNESS VARIANCE ------

	showGraph(dc,fps,0,maxfps,*theme.lookUpColor(FPS_TEXT_COLOR));
}

// TODO ueberpruefen ob is shown
void StatisticsWindow::process()
{
	if(!isShown())
		return;
	
	UI_Window::process();
//	configuration.setSpeed(speed->getNumber());

	if(!anarace->isOptimizing()) //~~ fehlt noch ein run am Anfang
		return;

	long int difference = SDL_GetTicks() - start_time;
	start_time += difference;

	if(averagecounter<100)
		averagecounter++;
	for(int i=averagecounter-1;i--;)
		average[i+1]=average[i];
	average[0]=difference;
	
	int av=0;
	for(unsigned int i=0;i<averagecounter;i++)
		av+=average[i];
	av/=averagecounter;

	ani++;
	if(ani>5)
	{
	ani=0;
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
		fps[i]=fps[i+1];
	}
																			
	pFitness[199]=anarace->getMaxpFitness();
	harvestedMinerals[199]=anarace->getHarvestedMinerals();
	harvestedGas[199]=anarace->getHarvestedGas();
	tFitness[199]=anarace->getMaxtFitness();
	aFitness[199]=anarace->fitnessAverage;
	vFitness[199]=(int)sqrt((double)anarace->fitnessVariance);
//	alength[199]=anarace->getAverageLength();
	alength[199]=anarace->getLength();
	time[199]=anarace->getRealTimer();
//	force[199]=anarace->getUnitsTotal();
	generation[199]=configuration.getMaxGenerations() - anarace->getUnchangedGenerations();
	fps[199]=1000/av;

	maxpFitness=0;
	maxsFitness=0;
	maxtFitness=0;
	maxvFitness=0;
	maxaFitness=0;
	mintFitness=99999;
	maxfps=0;
	maxHarvestedRessources=0;
//	maxForce=0;

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
//		if(force[i]>maxForce)
//			maxForce=force[i];
		if(fps[i]>maxfps)
			maxfps=fps[i];
	}

	int l=0;
/*	for(int k=0;k<20;k++)
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
		oldForce[l]=((signed int)(force[199])-(signed int)(force[198]));
	}*/

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
		oldGas[l]=((signed int)(harvestedGas[199])-(signed int)(harvestedGas[198]))/100;
	}

// ------ MINERALS ------
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
		oldMinerals[l]=((signed int)(harvestedMinerals[199])-(signed int)(harvestedMinerals[198]))/100;
	}
// ------ END MINERALS ------	

/*	for(int k=0;k<20;k++)
	{
		if(oldBoLengthCounter[k]>0)
		{
			oldBoLengthCounter[k]+=2;
			if(oldBoLengthCounter[k]>20)
			{
				oldBoLengthCounter[k]=0;
				oldBoLength[k]=0;
			}
		}
		if(oldBoLengthCounter[k]==0)
			l=k;
	}
	if(bolength[199]!=bolength[198])
	{
		oldBoLengthCounter[l]=1;
		oldBoLength[l]=((signed int)(boLength[199])-(signed int)(boLength[198]));
	}*/



	
	for(int i=0;i<20;i++)
		for(int j=0;j<i;j++)
		{
/*			if(oldForceCounter[j]<oldForceCounter[i])
			{
				int tee=oldForceCounter[i];
				oldForceCounter[i]=oldForceCounter[j];
				oldForceCounter[j]=tee;
				tee=oldForce[i];
				oldForce[i]=oldForce[j];
				oldForce[j]=tee;
			}*/
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
/*			if(oldBoLengthCounter[j]<oldBoLengthCounter[i])
			{
				int tee=oldBoLengthCounter[i];
				oldBoLengthCounter[i]=oldBoLengthCounter[j];
				oldBoLengthCounter[j]=tee;
				tee=oldBoLength[i];
				oldBoLength[i]=oldBoLength[j];
				oldBoLength[j]=tee;
			}*/
		}

	statEntry[FORCE_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(FORCE_STAT_STRING, force[199]));
	statEntry[TIME_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(TIME_STAT_STRING, time[199]%60, time[199]/60));
	statEntry[MINERALS_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(MINERALS_STAT_STRING, harvestedMinerals[199]/100));
	statEntry[GAS_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(GAS_STAT_STRING, harvestedGas[199]/100));
	statEntry[FITNESS_AVERAGE_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(FITNESS_AVERAGE_STAT_STRING, aFitness[199], anarace->getMaxpFitness()));
	statEntry[FITNESS_VARIANCE_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(FITNESS_VARIANCE_STAT_STRING, vFitness[199]));
	statEntry[AVERAGE_BO_LENGTH_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(AVERAGE_BO_LENGTH_STAT_STRING, alength[199]));
	}
	statEntry[GENERATIONS_LEFT_STAT_ENTRY]->updateNormalText(theme.lookUpFormattedString(GENERATIONS_LEFT_STAT_STRING, configuration.getMaxGenerations() - anarace->getUnchangedGenerations(), anarace->getTotalGeneration()));

	ostringstream os;
	os << fps[199] << " fps ";/* [" << UI_Object::rectnumber << " updates]";*/
//	if(anarace->isOptimizing())
//		os << "[" << (int)(1000.0*(MAX_PROGRAMS+(*anarace->getMap())->getMaxPlayer()-1)/((float)(av*(1+configuration.getSpeed())))) << " BOps]";
//		TODO alles in eine FPS Klasse oder so
//	statEntry[FPS_STAT_ENTRY]->updateNormalText(os.str());

}

