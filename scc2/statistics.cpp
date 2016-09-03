#include "statistics.h"
#include "math.h"

void StatisticsWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
}

StatisticsWindow::StatisticsWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(0,rahmen,maxSize,NOT_SCROLLED)
{
	resetData();
};
                                                                                                                                                            
void StatisticsWindow::showGraph(wxDC* dc, int* data,int max,wxColour col)
{
        int i,j,k;
        dc->SetPen(wxPen(col,2,wxSOLID));
        wxPoint blub[200];
        j=1;
        k=0;
        for(i=1;i<199;i++)
        {
                if((data[i]!=data[i+1])||(k>9))
                {
                        k=0;
                //TODO Hoehe 100 in clientarea.height aendern
                        blub[j]=wxPoint(i+getInnerLeftBound(),getInnerUpperBound()+100-data[i]/(max+1));
                        j++;
                } else k++;
        }
        blub[0]=wxPoint(getInnerLeftBound(),getInnerUpperBound()+100-data[0]/(max+1));
        blub[j]=wxPoint(199+getInnerLeftBound(),getInnerUpperBound()+100-data[199]/(max+1));
        dc->DrawSpline(j+1,blub);
};

void StatisticsWindow::resetData()
{
        int i;
        averagecounter=0;
        for(i=0;i<100;i++)
                average[i]=0;
        currentForce=0;
        maxForce=1;
                                                                                                                                                            
        maxpFitness=100;
        maxsFitness=100;
        maxtFitness=100;
        mintFitness=999;
        for(i=0;i<200;i++)
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
                                                                                                                                                            
        for(i=0;i<20;i++)
        {
                oldForceCounter[i]=0;
                oldForce[i]=0;
                oldGasCounter[i]=0;
                oldGas[i]=0;
                oldMinsCounter[i]=0;
                oldMins[i]=0;
        }
                                                                                                                                                            
};

void StatisticsWindow::drawStatistics(wxDC* dc)
{
        int i,j,k;
        for(i=0;i<20;i++)
                for(j=0;j<i;j++)
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
                                                                                                                                                            
        wxPoint Legend2=wxPoint(210+getInnerLeftBound(),getInnerUpperBound());
        wxPoint Legend3=wxPoint(180,FONT_SIZE+4);
                                                                                                                                                            
        dc->SetBrush(wxBrush(wxColour(40,40,40),wxSOLID));
        dc->SetPen(wxPen(wxColour(80,80,80),1,wxSOLID));
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y,Legend3.x,Legend3.y,4);
        for(k=0;k<20;k++)
                if(oldForceCounter[k]>0)
                {
                        dc->SetTextForeground(wxColour(40,200-oldForceCounter[k]*8,200-oldForceCounter[k]*8));
                        wxString bla=_T(wxString::Format(wxT("Force : %i"),force[199]/100));
                        int dx,dy;
                        dc->GetTextExtent(bla,&dx,&dy);
                                                                                                                                                            
                        if(oldForce[k]>0)
                                dc->DrawText(_T(wxString::Format(wxT("+%i"),oldForce[k])),Legend2.x+dx+oldForceCounter[k],Legend2.y);
                        else dc->DrawText(_T(wxString::Format(wxT("%i"),oldForce[k])),Legend2.x+dx+oldForceCounter[k],Legend2.y);
                        oldForceCounter[k]+=2;
                        if(oldForceCounter[k]>20)
                        {
                                oldForceCounter[k]=0;
                                oldForce[k]=0;
                        }
                }
                                                                                                                                                            
                                                                                                                                                            
        dc->SetTextForeground(wxColour(40,255,255));
        dc->DrawText(_T(wxString::Format(wxT("Force : %i"),force[199]/100)),Legend2.x,Legend2.y);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(255,255,40));
        dc->DrawText(_T(wxString::Format(wxT("Time : %.2i:%.2i"),time[199]/6000,(time[199]/100)%60)),Legend2.x,Legend2.y+12);
                                                                                                                                                            
//      dc->DrawRoundedRectangle(Legend2.x,Legend2.y+12,Legend3.x,Legend3.y,4);
//      dc->SetTextForeground(wxColour(255,40,255));
//      dc->DrawText(_T(wxString::Format(wxT("Tertiary fitness : %i"),tFitness[199]/100)),Legend2.x,Legend2.y+12);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+24,Legend3.x,Legend3.y,4);
        for(k=0;k<20;k++)
                if(oldMinsCounter[k]>0)
                {
                        dc->SetTextForeground(wxColour(40,100-oldMinsCounter[k]*3,200-oldMinsCounter[k]*8));
                        wxString bla=_T(wxString::Format(wxT("Minerals: %i"),harvestedMins[199]/10000));
                        int dx,dy;
                        dc->GetTextExtent(bla,&dx,&dy);
                                                                                                                                                            
                        if(oldMins[k]>0)
                                dc->DrawText(_T(wxString::Format(wxT("+%i"),oldMins[k]/100)),Legend2.x+dx+oldMinsCounter[k],Legend2.y+24);
                        else dc->DrawText(_T(wxString::Format(wxT("%i"),oldMins[k]/100)),Legend2.x+dx+oldMinsCounter[k],Legend2.y+24);
                                                                                                                                                            
                        oldMinsCounter[k]+=2;
                        if(oldMinsCounter[k]>20)
                        {
                                oldMinsCounter[k]=0;
                                oldMins[k]=0;
                        }
                }
                                                                                                                                                            
        dc->SetTextForeground(wxColour(40,100,255));
        dc->DrawText(_T(wxString::Format(wxT("Minerals: %i"),harvestedMins[199]/10000)),Legend2.x,Legend2.y+24);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+36,Legend3.x,Legend3.y,4);
        for(k=0;k<20;k++)
                if(oldGasCounter[k]>0)
                {
                        dc->SetTextForeground(wxColour(40,200-oldGasCounter[k]*8,40));
                        wxString bla=_T(wxString::Format(wxT("Gas: %i"),harvestedGas[199]/10000));
                        int dx,dy;
                        dc->GetTextExtent(bla,&dx,&dy);
                        if(oldGas[k]>0)
                                dc->DrawText(_T(wxString::Format(wxT("+%i"),oldGas[k]/100)),Legend2.x+dx+oldGasCounter[k],Legend2.y+36);
                        else dc->DrawText(_T(wxString::Format(wxT("%i"),oldGas[k]/100)),Legend2.x+dx+oldGasCounter[k],Legend2.y+36);
                                                                                                                                                            
                        oldGasCounter[k]+=2;
                        if(oldGasCounter[k]>20)
                        {
                                oldGasCounter[k]=0;
                                oldGas[k]=0;
                        }
                }
                                                                                                                                                            
        dc->SetTextForeground(wxColour(40,255,40));
        dc->DrawText(_T(wxString::Format(wxT("Gas: %i"),harvestedGas[199]/10000)),Legend2.x,Legend2.y+36);
                                                                                                                                                            
//      dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
//      dc->SetTextForeground(wxColour(255,40,40));
//      dc->DrawText(_T(wxString::Format(wxT("Primary fitness : %i"),pFitness[199]/100)),Legend2.x,Legend2.y+48);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+48,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(200,200,200));
        dc->DrawText(_T(wxString::Format(wxT("Fitness average : %i"),aFitness[199]/100)),Legend2.x,Legend2.y+48);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+60,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(100,100,100));
        dc->DrawText(_T(wxString::Format(wxT("Fitness variance : %i"),vFitness[199]/100)),Legend2.x,Legend2.y+60);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+72,Legend3.x,Legend3.y,4);
        dc->SetTextForeground(wxColour(100,150,200));
        dc->DrawText(_T(wxString::Format(wxT("BuildOrder Length: %i"),length[199]/100)),Legend2.x,Legend2.y+72);
                                                                                                                                                            
        dc->DrawRoundedRectangle(Legend2.x,Legend2.y+84,Legend3.x,Legend3.y,4);
        dc->SetBrush(wxBrush(wxColour(40,40,100),wxSOLID));
        dc->DrawRoundedRectangle(Legend2.x+Legend3.x-(anarace->ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/anarace->ga->maxGenerations,Legend2.y+84,(anarace->ga->maxGenerations-anarace->getUnchangedGenerations())*Legend3.x/anarace->ga->maxGenerations ,Legend3.y,4);
        dc->SetTextForeground(wxColour(100,100,255));
        dc->DrawText(_T(wxString::Format(wxT("%i+ generations left"),(anarace->ga->maxGenerations-anarace->getUnchangedGenerations()))),Legend2.x,Legend2.y+84);
                                                                                                                                                            
//      dt1=wxDateTime::UNow();
//      wxTimeSpan ts=dt1.Subtract(dt2);
//      dt2=dt1;
                                                                                                                                                            
/*      if(averagecounter<100)
                averagecounter++;
        for(i=averagecounter-1;i--;)
                average[i+1]=average[i];
        average[0]=ts.GetMilliseconds().ToLong();
                                                                                                                                                            
        int av=0;
        for(i=0;i<averagecounter;i++)
                av+=average[i];
        av/=averagecounter;
//      int tlength=force[199]/100-5;

//      haxor->Draw(dc);
/*
        dc->SetTextForeground(wxColour(100,100,255));
                                                                                                                                                            
        dc->DrawText(_T(wxString::Format(wxT("%i of %.0f billion possible build orders checked [%.6f%%]."),anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1),pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/1000000000.0,(anarace[0]->getGeneration()*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1))/pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes()))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound());
                                                                                                                                                            
        if(!run)
        dc->DrawText(_T(wxString::Format(wxT("%i ms per frame (%.2f fps)."),av,1000.0/((float)(av)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+20);
        else
        {
                dc->DrawText(_T(wxString::Format(wxT("%i ms per frame (%.2f fps) [%.2f bops]."),av,1000.0/((float)(av)),1000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)/((float)(av)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+20);
                                                                                                                                                            
                dc->DrawText(_T(wxString::Format(wxT("Estimated time using stupid brute-force algorithm: %.5f billion years."),
                pow(tlength,anarace[0]->getPlayer()->goal->getMaxBuildTypes())/(1000000000.0*365.0*24.0*3600000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)/((float)(av))))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+40);
                if(anarace[0]->getTimer()!=anarace->ga->maxTime)
                {
                        float totaln=1;
                        for(i=0;i<=GAS_SCV;i++)
                                if(anarace[0]->getLocationForce(0,i))
                //todo startforce beruecksichtigen... aber erstmal map location[0] fuellen... TODO
                                {
                                        if(i==SCV)
                                        for(j=1;j<anarace[0]->getLocationForce(0,i)-4;j++)
                                                totaln*=j;
                                        else if(i==COMMAND_CENTER)
                                        for(j=1;j<anarace[0]->getLocationForce(0,i)-1;j++)
                                                totaln*=j;
                                        else
                                        for(j=1;j<anarace[0]->getLocationForce(0,i);j++)
                                                totaln*=j;
                                }
                        float totalz=1;
                        for(i=1;i<force[199]/100-5;i++)
                                totalz*=i;
                        dc->DrawText(_T(wxString::Format(wxT("Estimated time using intelligent brute-force algorithm: %.2f hours."), (totalz*((float)(av)))/(totaln*3600000.0*(MAX_PROGRAMS+anarace[0]->getMap()->getMaxPlayer()-1)))),haxor->getInnerLeftBound(),haxor->getInnerUpperBound()+60);
                                                                                                                                                            
                }
        }*/
                dc->SetPen(wxPen(wxColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
                dc->SetBrush(wxBrush(wxColour(INFOWINDOWPENR/2,INFOWINDOWPENG/2,INFOWINDOWPENB/2),wxCROSS_HATCH));
                                                                                                                                                            
                dc->DrawRectangle(getInnerLeftBound(),getInnerUpperBound(),200,100);
        //      showGraph(dc,tFitness,maxtFitness-mintFitness,wxColour(255,40,255));
                showGraph(dc,harvestedMins,maxsFitness,wxColour(40,100,255));
                showGraph(dc,harvestedGas,maxsFitness,wxColour(40,255,40));
                showGraph(dc,force,maxForce,wxColour(40,255,255));
                showGraph(dc,length,MAX_LENGTH,wxColour(100,150,200));
                showGraph(dc,time,anarace->ga->maxTime,wxColour(255,255,40));
                showGraph(dc,aFitness,maxpFitness,wxColour(200,200,200));
                showGraph(dc,vFitness,maxpFitness,wxColour(100,100,100));
        //      showGraph(dc,pFitness,maxpFitness,wxColour(255,40,40));
};

void StatisticsWindow::ProgressGraph()
{
        int i,k;
                for(i=0;i<199;i++)
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
                                                                                                                                                            
                if(anarace->getMaxpFitness()>maxpFitness) maxpFitness=anarace->getMaxpFitness();
                if(anarace->getMaxsFitness()>maxsFitness) maxsFitness=anarace->getMaxsFitness();
                if(anarace->getMaxtFitness()>maxtFitness) maxtFitness=anarace->getMaxtFitness();
                if(anarace->getMaxtFitness()<mintFitness) mintFitness=anarace->getMaxtFitness();
                                                                                                                                                            
                pFitness[199]=anarace->getMaxpFitness()*100;
                harvestedMins[199]=anarace->getHarvestedMins()*100;
                harvestedGas[199]=anarace->getHarvestedGas()*100;
                tFitness[199]=(anarace->getMaxtFitness()-mintFitness)*100;
                aFitness[199]=anarace->fitnessAverage*100;
                vFitness[199]=(int)sqrt((double)anarace->fitnessVariance)*100;
                length[199]=anarace->getLength()*100;
                time[199]=(anarace->ga->maxTime-anarace->getTimer())*100;
                force[199]=currentForce*100;
                if(force[199]!=force[198])
                {
                        k=0;
                        while(oldForceCounter[k]>0)
                        {
                                k++;
                                if(k>19) k=0;
                        }
                        oldForceCounter[k]=1;
                        oldForce[k]=(force[199]-force[198])/100;
                }
                if(harvestedGas[199]!=harvestedGas[198])
                {
                        k=0;
                        while(oldGasCounter[k]>0)
                        {
                                k++;
                                if(k>19) k=0;
                        }
                        oldGasCounter[k]=1;
                        oldGas[k]=(harvestedGas[199]-harvestedGas[198])/100;
                }
                if(harvestedMins[199]!=harvestedMins[198])
                {
                        k=0;
                        while(oldMinsCounter[k]>0)
                        {
                                k++;
                                if(k>19) k=0;
                        }
                        oldMinsCounter[k]=1;
                        oldMins[k]=(harvestedMins[199]-harvestedMins[198])/100;
                }
};
 
