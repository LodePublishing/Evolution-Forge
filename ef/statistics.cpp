#include "statistics.hpp"
#include "../core/settings.hpp"
#include <sys/time.h>

StatisticsWindow::StatisticsWindow(UI_Object* parent, const ANARACE* anarace, const int windowNumber):UI_Window(parent, STATISTICS_WINDOW_TITLE_STRING, STATISTICS_WINDOW, windowNumber, NOT_SCROLLED)
{
	this->anarace=anarace;
	resetData();
    timeval tim;
    gettimeofday(&tim, NULL);
    start_time=tim.tv_sec*1000000+tim.tv_usec;
	av=0;
}

StatisticsWindow::~StatisticsWindow()
{
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
			blub[j]=Point(i+getAbsoluteClientRectLeftBound(),getAbsoluteClientRectUpperBound()+100-100*(data[i]-min)/(max+1));
			j++;
		} else k++;
	}
	blub[0]=Point(getAbsoluteClientRectPosition()+Point(0,100-100*(data[0]-min)/(max+1)));
blub[j]=Point(getAbsoluteClientRectPosition()+Point(199,100-100*(data[199]-min)/(max+1)));
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
		length[i]=0; //ok
		time[i]=0;
		force[i]=5; //5 units at the beginning!
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
																				
	Point Legend2=Point(getAbsoluteClientRectPosition()+Point(210,0));
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
			dc->SetTextForeground(//*theme.lookUpColor(WINDOW_TEXT_COLOUR));
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
	

	dc->SetTextForeground(*theme.lookUpColor(GAS_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(GAS_STRING, harvestedGas[199]/100),Legend2.x,Legend2.y+36);
//	addButton(Rect(Legend2.x, Legend2.y+36, Legend3.x, Legend3.y), "total harvested gass at the end"); TODO
																				
//	  dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
//	  dc->SetTextForeground(dc->doColor(255,40,40));
//	  dc->DrawText(_T(String::Format(T("Primary fitness : %i"),pFitness[199])),Legend2.x,Legend2.y+48);
																				
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(*theme.lookUpColor(FITNESS_AVERAGE_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(FITNESS_AVERAGE_STRING, aFitness[199], anarace->getMaxpFitness()),Legend2.x,Legend2.y+48);
//	addButton(Rect(Legend2.x, Legend2.y+48, Legend3.x, Legend3.y), "average fitness including mutations"); TODO
																				
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+60,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(*theme.lookUpColor(FITNESS_VARIANCE_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(FITNESS_VARIANCE_STRING, vFitness[199]),Legend2.x,Legend2.y+60);
//	addButton(Rect(Legend2.x, Legend2.y+60, Legend3.x, Legend3.y), "average fitness difference to average fitness"); TODO
																				
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+72,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(*theme.lookUpColor(BOLENGTH_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(BUILD_ORDER_LENGTH_STRING, length[199]),Legend2.x,Legend2.y+72);
//	addButton(Rect(Legend2.x, Legend2.y+72, Legend3.x, Legend3.y), "all single build orders summed together"); TODO
																				
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+84,Legend3.x,Legend3.y,4);
	dc->SetBrush(*theme.lookUpBrush(GENERATIONS_LEFT_BRUSH));
	dc->DrawRoundedRectangle(Legend2.x+Legend3.x-(anarace->ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/anarace->ga->maxGenerations,Legend2.y+84,(anarace->ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/anarace->ga->maxGenerations ,Legend3.y,4);
	dc->SetTextForeground(*theme.lookUpColor(GENERATIONS_LEFT_TEXT_COLOUR));
	dc->DrawText(theme.lookUpFormattedString(GENERATIONS_LEFT_STRING, anarace->ga->maxGenerations-anarace->getUnchangedGenerations()),Legend2.x,Legend2.y+84);
//	addButton(Rect(Legend2.x, Legend2.y+84, Legend3.x, Legend3.y), "minimum expected number of generations until final solution"); TODO
//	  int tlength=force[199]/100-5;

	int dx,dy;
	dc->SetTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOUR));

	ostringstream os;
	os << av << "ms/frame";
	dc->GetTextExtent(os.str(),&dx,&dy);
	dc->DrawText(os.str(),getAbsoluteClientRectLeftBound()+getClientRectWidth()-dx-5,getAbsoluteClientRectUpperBound()+getClientRectHeight()-3*dy-5);

	os.str("");
	os << 1000.0/((float)(av)) << " fps";
	dc->GetTextExtent(os.str(),&dx,&dy);
	dc->DrawText(os.str(),getAbsoluteClientRectLeftBound()+getClientRectWidth()-dx-5,getAbsoluteClientRectUpperBound()+getClientRectHeight()-2*dy-5);
	if(anarace->isOptimizing())
	{
		os.str("");
		os << 1000.0*(MAX_PROGRAMS+(*anarace->getMap())->getMaxPlayer()-1)/((float)(av*(1+settings.getSpeed()))) << " bops";
		dc->GetTextExtent(os.str(),&dx,&dy);
		dc->DrawText(os.str(),getAbsoluteClientRectLeftBound()+getClientRectWidth()-dx-5,getAbsoluteClientRectUpperBound()+getClientRectHeight()-dy-5);
	}
																				
//	dc->DrawText(_T(String::Format(T("%i of %.0f billion possible build orders checked [%.6f%%]."),anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1),pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/1000000000.0,(anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1))/pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes()))),haxor->getAbsoluteClientRectLeftBound(),haxor->getAbsoluteClientRectUpperBound());

/*	if(!run)
		dc->DrawText(_T(String::Format(T("%i ms/frame (%.2f fps)."),av,1000.0/((float)(av)))),haxor->getAbsoluteClientRectLeftBound(),haxor->getAbsoluteClientRectUpperBound()+20);
	else
	{
		dc->DrawText(_T(String::Format(T("%i ms/frame (%.2f fps) [%.2f bops]."),av,1000.0/((float)(av)),1000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)/((float)(av)))),haxor->getAbsoluteClientRectLeftBound(),haxor->getAbsoluteClientRectUpperBound()+20);
																				
		dc->DrawText(_T(String::Format(T("Estimated time using stupid brute-force algorithm: %.5f billion years."),
		pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/(1000000000.0*365.0*24.0*3600000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)/((float)(av))))),haxor->getAbsoluteClientRectLeftBound(),haxor->getAbsoluteClientRectUpperBound()+40);
		if(anarace[0]->getTimer()!=anarace->ga->maxTime)
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
																				
	dc->DrawRectangle(getAbsoluteClientRectPosition(),Size(200,100));
//	  showGraph(dc,tFitness,mintFitness,maxtFitness,Color(255,40,255));
	showGraph(dc,harvestedMinerals,0,maxHarvestedRessources,*theme.lookUpColor(MINERALS_TEXT_COLOUR));
	showGraph(dc,harvestedGas,0,maxHarvestedRessources,*theme.lookUpColor(GAS_TEXT_COLOUR));
	showGraph(dc,force,0,maxForce/*anarace->getUnitsTotal()*/,*theme.lookUpColor(STATISTICS_FORCE_TEXT_COLOUR));
	showGraph(dc,length,0,MAX_LENGTH,*theme.lookUpColor(BOLENGTH_TEXT_COLOUR));
	showGraph(dc,time,0,anarace->ga->maxTime,*theme.lookUpColor(TIME_TEXT_COLOUR));
	showGraph(dc,aFitness,0,maxaFitness,*theme.lookUpColor(FITNESS_AVERAGE_TEXT_COLOUR));
	showGraph(dc,vFitness,0,maxvFitness,*theme.lookUpColor(FITNESS_VARIANCE_TEXT_COLOUR));
//	  showGraph(dc,pFitness,maxpFitness,dc->doColor(255,40,40));

}

// TODO ueberpruefen ob is shown
void StatisticsWindow::process()
{
	if(!shown)
		return;
	
	
	UI_Window::process();
	for(int i=0;i<199;i++)
	{
		pFitness[i]=pFitness[i+1];
		harvestedMinerals[i]=harvestedMinerals[i+1];
		harvestedGas[i]=harvestedGas[i+1];
		tFitness[i]=tFitness[i+1];
		aFitness[i]=aFitness[i+1];
		vFitness[i]=vFitness[i+1];
		length[i]=length[i+1];
		time[i]=time[i+1];
		force[i]=force[i+1];
	};
																			
																			
	pFitness[199]=anarace->getMaxpFitness();
	harvestedMinerals[199]=anarace->getHarvestedMinerals();
	harvestedGas[199]=anarace->getHarvestedGas();
	tFitness[199]=anarace->getMaxtFitness();
	aFitness[199]=anarace->fitnessAverage;
	vFitness[199]=(int)sqrt((double)anarace->fitnessVariance);
	length[199]=anarace->getLength();
	time[199]=(anarace->ga->maxTime-anarace->getTimer());
	force[199]=anarace->getUnitsTotal();

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
																				
	for(int i=0;i<averagecounter;i++)
		av+=average[i];
	av/=averagecounter;

}

