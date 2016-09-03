#include "statistics.h"
#include "math.h"

StatisticsWindow::StatisticsWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace):UI_Window(parent, STATISTICS_WINDOW_TITLE_STRING,rahmen,maxSize,NOT_SCROLLED)
{
	this->anarace=anarace;
	resetData();
};

StatisticsWindow::~StatisticsWindow()
{
};
																			    
void StatisticsWindow::showGraph(wxDC* dc, int* data, int min, int max, wxColour col)
{
	int j,k;
	dc->SetPen(wxPen(col,2,wxSOLID));
	wxPoint blub[200];
	j=1;
	k=0;
	for(int i=1;i<199;i++)
	{
		if((data[i]!=data[i+1])||(k>9))
		{
			k=0;
		//TODO Hoehe 100 in clientarea.height aendern
			blub[j]=wxPoint(i+getClientRectLeftBound(),getClientRectUpperBound()+100-100*(data[i]-min)/(max+1));
			j++;
		} else k++;
	}
	blub[0]=wxPoint(getClientRectLeftBound(),getClientRectUpperBound()+100-100*(data[0]-min)/(max+1));
	blub[j]=wxPoint(199+getClientRectLeftBound(),getClientRectUpperBound()+100-100*(data[199]-min)/(max+1));
	dc->DrawSpline(j+1,blub);
};

void StatisticsWindow::resetData()
{
//        dt1=wxDateTime::UNow();
  //      dt2=wxDateTime::UNow();
	averagecounter=0;
	for(int i=0;i<100;i++)
		average[i]=0;
																			    
	maxpFitness=100;
	maxsFitness=100;
	maxtFitness=100;
	maxHarvestedRessources=1000;
	mintFitness=99999;
	for(int i=0;i<200;i++)
	{
		pFitness[i]=0;
		harvestedMins[i]=0;
		harvestedGas[i]=0;
		tFitness[i]=0;
		aFitness[i]=0;
		vFitness[i]=0; //~~
		length[i]=0; //ok
		time[i]=0;
		force[i]=5; //5 units at the beginning!
	};
																			    
	for(int i=0;i<20;i++)
	{
		oldForceCounter[i]=0;
		oldForce[i]=0;
		oldGasCounter[i]=0;
		oldGas[i]=0;
		oldMinsCounter[i]=0;
		oldMins[i]=0;
	}
																			    
};

void StatisticsWindow::draw(wxDC* dc)
{
	UI_Window::draw(dc);
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
																			    
			if(oldMinsCounter[j]<oldMinsCounter[i])
			{
				int tee=oldMinsCounter[i];
				oldMinsCounter[i]=oldMinsCounter[j];
				oldMinsCounter[j]=tee;
				tee=oldMins[i];
				oldMins[i]=oldMins[j];
				oldMins[j]=tee;
			}
		}
																			    
	wxPoint Legend2=wxPoint(210+getClientRectLeftBound(),getClientRectUpperBound());
	wxPoint Legend3=wxPoint(180,FONT_SIZE+4);
																			    
	dc->SetBrush(wxBrush(wxColour(40,40,40),wxSOLID));
	dc->SetPen(wxPen(wxColour(80,80,80),1,wxSOLID));
																			    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y,Legend3.x,Legend3.y,4);


	for(int k=0;k<20;k++)
		if(oldForceCounter[k]>0)
		{
			wxString bla=theme.lookUpFormattedString(FORCE_STRING, force[199]);
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
			dc->SetTextForeground(wxColour(40,200-oldForceCounter[k]*8,200-oldForceCounter[k]*8));
			if(oldForce[k]>0)
				dc->DrawText(_T(wxString::Format(wxT("+%i"),oldForce[k])),Legend2.x+dx+oldForceCounter[k],Legend2.y);
			else dc->DrawText(_T(wxString::Format(wxT("%i"),oldForce[k])),Legend2.x+dx+oldForceCounter[k],Legend2.y);
		}
																			
																			    
	dc->SetTextForeground(wxColour(40,255,255));
	dc->DrawText(theme.lookUpFormattedString(FORCE_STRING, force[199]),Legend2.x,Legend2.y);
//    addButton(wxRect(Legend2.x, Legend2.y, Legend3.x, Legend3.y), "total number of units/buildings at the end"); TODO
																			    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(255,255,40));
	dc->DrawText(theme.lookUpFormattedString(TIME_STRING, time[199]/60, time[199]%60),Legend2.x,Legend2.y+12);
//    addButton(wxRect(Legend2.x, Legend2.y+12, Legend3.x, Legend3.y), "time of the best build order"); TODO
	
																			    
//      dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
//      dc->SetTextForeground(wxColour(255,40,255));
//      dc->DrawText(_T(wxString::Format(wxT("Tertiary fitness : %i"),tFitness[199])),Legend2.x,Legend2.y+12);
																			    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+24,Legend3.x,Legend3.y,4);
	for(int k=0;k<20;k++)
		if(oldMinsCounter[k]>0)
		{
			dc->SetTextForeground(wxColour(40,100-oldMinsCounter[k]*3,200-oldMinsCounter[k]*8));
			wxString bla=theme.lookUpFormattedString(MINERALS_STRING, harvestedMins[199]/100);
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
																		
			if(oldMins[k]>0)
				dc->DrawText(_T(wxString::Format(wxT("+%i"),oldMins[k]/100)),Legend2.x+dx+oldMinsCounter[k],Legend2.y+24);
			else dc->DrawText(_T(wxString::Format(wxT("%i"),oldMins[k]/100)),Legend2.x+dx+oldMinsCounter[k],Legend2.y+24);
		}
																			    
	dc->SetTextForeground(wxColour(40,100,255));
	dc->DrawText(theme.lookUpFormattedString(MINERALS_STRING, harvestedMins[199]/100),Legend2.x,Legend2.y+24);
//    addButton(wxRect(Legend2.x, Legend2.y+24, Legend3.x, Legend3.y), "total harvested minerals at the end"); TODO
	
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+36,Legend3.x,Legend3.y,4);
	for(int k=0;k<20;k++)
		if(oldGasCounter[k]>0)
		{
			dc->SetTextForeground(wxColour(40,200-oldGasCounter[k]*8,40));
			wxString bla=theme.lookUpFormattedString(GAS_STRING, harvestedGas[199]/100);
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
			if(oldGas[k]>0)
				dc->DrawText(_T(wxString::Format(wxT("+%i"),oldGas[k]/100)),Legend2.x+dx+oldGasCounter[k],Legend2.y+36);
			else dc->DrawText(_T(wxString::Format(wxT("%i"),oldGas[k]/100)),Legend2.x+dx+oldGasCounter[k],Legend2.y+36);
		}
	

	dc->SetTextForeground(wxColour(40,255,40));
	dc->DrawText(theme.lookUpFormattedString(GAS_STRING, harvestedGas[199]/100),Legend2.x,Legend2.y+36);
//    addButton(wxRect(Legend2.x, Legend2.y+36, Legend3.x, Legend3.y), "total harvested gass at the end"); TODO
																			    
//      dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
//      dc->SetTextForeground(wxColour(255,40,40));
//      dc->DrawText(_T(wxString::Format(wxT("Primary fitness : %i"),pFitness[199])),Legend2.x,Legend2.y+48);
																			    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(200,200,200));
	dc->DrawText(theme.lookUpFormattedString(FITNESS_AVERAGE_STRING, aFitness[199]),Legend2.x,Legend2.y+48);
//	addButton(wxRect(Legend2.x, Legend2.y+48, Legend3.x, Legend3.y), "average fitness including mutations"); TODO
																			    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+60,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(100,100,100));
	dc->DrawText(theme.lookUpFormattedString(FITNESS_VARIANCE_STRING, vFitness[199]),Legend2.x,Legend2.y+60);
//	addButton(wxRect(Legend2.x, Legend2.y+60, Legend3.x, Legend3.y), "average fitness difference to average fitness"); TODO
																			    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+72,Legend3.x,Legend3.y,4);
	dc->SetTextForeground(wxColour(100,150,200));
	dc->DrawText(theme.lookUpFormattedString(BUILD_ORDER_LENGTH_STRING, length[199]),Legend2.x,Legend2.y+72);
//	addButton(wxRect(Legend2.x, Legend2.y+72, Legend3.x, Legend3.y), "all single build orders summed together"); TODO
																			    
	dc->DrawRoundedRectangle(Legend2.x,Legend2.y+84,Legend3.x,Legend3.y,4);
	dc->SetBrush(wxBrush(wxColour(40,40,100),wxSOLID));
	dc->DrawRoundedRectangle(Legend2.x+Legend3.x-(anarace->ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/anarace->ga->maxGenerations,Legend2.y+84,(anarace->ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/anarace->ga->maxGenerations ,Legend3.y,4);
	dc->SetTextForeground(wxColour(100,100,255));
	dc->DrawText(theme.lookUpFormattedString(GENERATIONS_LEFT_STRING, anarace->ga->maxGenerations-anarace->getUnchangedGenerations()),Legend2.x,Legend2.y+84);
//	addButton(wxRect(Legend2.x, Legend2.y+84, Legend3.x, Legend3.y), "minimum expected number of generations until final solution"); TODO
	
#if 0	
        dt1=wxDateTime::UNow();
      wxTimeSpan ts=dt1.Subtract(dt2);
    dt2=dt1;
																			    
        if(averagecounter<100)
		averagecounter++;
	for(int i=averagecounter-1;i--;)
		average[i+1]=average[i];
	average[0]=ts.GetMilliseconds().ToLong();
																			    
	int av=0;
	for(int i=0;i<averagecounter;i++)
		av+=average[i];
	av/=averagecounter;
//      int tlength=force[199]/100-5;

//      haxor->Draw(dc);

	int dx,dy;
	wxString bla;
	dc->SetTextForeground(wxColour(100,100,255));

	bla=_T(wxString::Format(wxT("%i ms/frame"),av));dc->GetTextExtent(bla,&dx,&dy);
        dc->DrawText(bla,getClientRectLeftBound()+getClientRectWidth()-dx-5,getClientRectUpperBound()+getClientRectHeight()-3*dy-5);
	bla=_T(wxString::Format(wxT("%.2f fps"),1000.0/((float)(av))));dc->GetTextExtent(bla,&dx,&dy);
        dc->DrawText(bla,getClientRectLeftBound()+getClientRectWidth()-dx-5,getClientRectUpperBound()+getClientRectHeight()-2*dy-5);
	if(anarace->isOptimizing())
	{
		bla=_T(wxString::Format(wxT("%.2f bops"),1000.0*(MAX_PROGRAMS+anarace->getMap()->getMaxPlayer()-1)/((float)(av))));dc->GetTextExtent(bla,&dx,&dy);
        	dc->DrawText(bla,getClientRectLeftBound()+getClientRectWidth()-dx-5,getClientRectUpperBound()+getClientRectHeight()-dy-5);
	}
																			    
//	dc->DrawText(_T(wxString::Format(wxT("%i of %.0f billion possible build orders checked [%.6f%%]."),anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1),pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/1000000000.0,(anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1))/pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes()))),haxor->getClientRectLeftBound(),haxor->getClientRectUpperBound());

#endif																    
/*	if(!run)
		dc->DrawText(_T(wxString::Format(wxT("%i ms/frame (%.2f fps)."),av,1000.0/((float)(av)))),haxor->getClientRectLeftBound(),haxor->getClientRectUpperBound()+20);
	else
	{
		dc->DrawText(_T(wxString::Format(wxT("%i ms/frame (%.2f fps) [%.2f bops]."),av,1000.0/((float)(av)),1000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)/((float)(av)))),haxor->getClientRectLeftBound(),haxor->getClientRectUpperBound()+20);
																			    
/*		dc->DrawText(_T(wxString::Format(wxT("Estimated time using stupid brute-force algorithm: %.5f billion years."),
		pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/(1000000000.0*365.0*24.0*3600000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)/((float)(av))))),haxor->getClientRectLeftBound(),haxor->getClientRectUpperBound()+40);
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
			dc->DrawText(_T(wxString::Format(wxT("Estimated time using intelligent brute-force algorithm: %.2f hours."), (totalz*((float)(av)))/(totaln*3600000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)))),haxor->getClientRectLeftBound(),haxor->getClientRectUpperBound()+60);
																			    
		}*/
//	}

	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)); // TODO HATCH wieder machen
																			    
	dc->DrawRectangle(getClientRectLeftBound(),getClientRectUpperBound(),200,100);
//      showGraph(dc,tFitness,mintFitness,maxtFitness,wxColour(255,40,255));
	showGraph(dc,harvestedMins,0,maxHarvestedRessources,wxColour(40,100,255));
	showGraph(dc,harvestedGas,0,maxHarvestedRessources,wxColour(40,255,40));
	showGraph(dc,force,0,maxForce/*anarace->getTotalUnitForce()*/,wxColour(40,255,255));
	showGraph(dc,length,0,MAX_LENGTH,wxColour(100,150,200));
	showGraph(dc,time,0,anarace->ga->maxTime,wxColour(255,255,40));
	showGraph(dc,aFitness,0,maxaFitness,wxColour(200,200,200));
	showGraph(dc,vFitness,0,maxvFitness,wxColour(100,100,100));
//      showGraph(dc,pFitness,maxpFitness,wxColour(255,40,40));
};

// TODO ueberpruefen ob is shown
void StatisticsWindow::process()
{
	UI_Window::process();
	for(int i=0;i<199;i++)
	{
		pFitness[i]=pFitness[i+1];
		harvestedMins[i]=harvestedMins[i+1];
		harvestedGas[i]=harvestedGas[i+1];
		tFitness[i]=tFitness[i+1];
		aFitness[i]=aFitness[i+1];
		vFitness[i]=vFitness[i+1];
		length[i]=length[i+1];
		time[i]=time[i+1];
		force[i]=force[i+1];
	};
																		    
																		    
	pFitness[199]=anarace->getMaxpFitness();
	harvestedMins[199]=anarace->getHarvestedMins();
	harvestedGas[199]=anarace->getHarvestedGas();
	tFitness[199]=anarace->getMaxtFitness();
	aFitness[199]=anarace->fitnessAverage;
	vFitness[199]=(int)sqrt((double)anarace->fitnessVariance);
	length[199]=anarace->getLength();
	time[199]=(anarace->ga->maxTime-15);//anarace->getTimer());
	force[199]=anarace->getTotalUnitForce();

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
		if(harvestedMins[i]>maxHarvestedRessources)
			maxHarvestedRessources=harvestedMins[i];
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
			if(oldMinsCounter[k]>0)
			{
        	    oldMinsCounter[k]+=2;
            	if(oldMinsCounter[k]>20)
    	        {
	                oldMinsCounter[k]=0;
        	        oldMins[k]=0;
	            }
			}
			if(oldMinsCounter[k]==0)
				l=k;
		}
		if(harvestedMins[199]!=harvestedMins[198])
		{
			oldMinsCounter[l]=1;
			oldMins[l]=harvestedMins[199]-harvestedMins[198];
		}
};

