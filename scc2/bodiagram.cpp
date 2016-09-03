#include "bodiagram.h"

BoDiagramWindow::BoDiagramWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(0,rahmen,maxSize,NOT_SCROLLED)
{
};

void BoDiagramWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
}
                                                                                                                                                            
void BoDiagramWindow::showProgramGraph(wxDC* dc)
{
        int s,y1;
        wxPoint mins[MAX_TIME];
        wxPoint gas[MAX_TIME];
//      wxPoint fitness[MAX_LENGTH];
        wxPoint supply[MAX_TIME];
        int time;
        if(anarace->ga->maxTime-anarace->getTimer()<2) return;
        int count=0;
//      if(infoWindowNumber)
//              dc->SetBrush(wxBrush(wxColour(30,30,30),wxCROSS_HATCH));
//      else
                dc->SetBrush(wxBrush(wxColour(50,50,50),wxCROSS_HATCH));
        dc->DrawRectangle(getInnerLeftBound(),getInnerUpperBound()+10,getInnerWidth(),getInnerHeight()-10);
                                                                                                                                                            
        if(anarace->getTimer()==anarace->ga->maxTime) time=0;
                else time=anarace->getTimer()+1;
        s=anarace->ga->maxTime-1;
        while(s>=time)
        {
                if(anarace->getStatisticsHaveMinerals(s)>75000) y1=75; else y1=anarace->getStatisticsHaveMinerals(s)/1000;
                mins[count]=wxPoint(getInnerLeftBound()+3+((count*(getInnerWidth()-6))/(anarace->ga->maxTime-time)),getInnerUpperBound()+getInnerHeight()-y1);
                                                                                                                                                            
                if(anarace->getStatisticsHaveGas(s)>75000) y1=75; else y1=anarace->getStatisticsHaveGas(s)/1000;
                gas[count]=wxPoint(getInnerLeftBound()+3+((count*(getInnerWidth()-6))/(anarace->ga->maxTime-time)),getInnerUpperBound()+getInnerHeight()-y1);
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!
                                                                                                                                                            
/*              y1=anarace->getStatisticsFitness(s)*75/(anarace->getMaxpFitness()-anarace->getTimer());
                fitness[count]=wxPoint(getInnerLeftBound()+3+((s*(getInnerWidth()-6))/(anarace->ga->maxTime-anarace->getTimer())),getInnerUpperBound()+getInnerHeight()-y1);*/
                                                                                                                                                            
                if(anarace->getStatisticsHaveSupply(s)-anarace->getStatisticsNeedSupply(s)>15) y1=75; else
                if(anarace->getStatisticsHaveSupply(s)<anarace->getStatisticsNeedSupply(s)) y1=0; else
                y1=(anarace->getStatisticsHaveSupply(s)-anarace->getStatisticsNeedSupply(s))*5;
                                                                                                                                                            
                supply[count]=wxPoint(getInnerLeftBound()+3+((count*(getInnerWidth()-6))/(anarace->ga->maxTime-time)),getInnerUpperBound()+getInnerHeight()-y1);
                count++;
                s--;
        }
                                                                                                                                                            
        if(count>0)
        {
                dc->SetFont(GraphixScrollWindow::font2);
/*              if(infoWindowNumber)
                {
                        dc->SetPen(wxPen(wxColour(80,80,80),2,wxSOLID));
                        dc->DrawSpline(count,supply);
                        dc->SetPen(wxPen(wxColour(60,60,200),2,wxSOLID));
                        dc->DrawSpline(count,mins);
                        dc->SetPen(wxPen(wxColour(20,160,20),2,wxSOLID));
                        dc->DrawSpline(count,gas);
//                      dc->SetPen(wxPen(wxColour(255,40,40),2,wxSOLID));
//                      dc->DrawSpline(count,fitness);
//                      dc->SetTextForeground(wxColour(255,40,40));
//                      dc->DrawText(_T("Fitness"),getInnerLeftBound()+1,getInnerUpperBound()+8);
                        dc->SetTextForeground(wxColour(60,60,200));
                        dc->DrawText(_T("Minerals"),getInnerLeftBound()+1,getInnerUpperBound()+10);
                        dc->SetTextForeground(wxColour(20,200,20));
                        dc->DrawText(_T("Gas"),getInnerLeftBound()+1,getInnerUpperBound()+21);
                        dc->SetTextForeground(wxColour(120,120,120));
                        dc->DrawText(_T("Supply"),getInnerLeftBound()+1,getInnerUpperBound()+32);
                } else*/
                {
                        dc->SetPen(wxPen(wxColour(120,120,120),2,wxSOLID));
                        dc->DrawSpline(count,supply);
                        dc->SetPen(wxPen(wxColour(80,80,255),2,wxSOLID));
                        dc->DrawSpline(count,mins);
                        dc->SetPen(wxPen(wxColour(40,255,40),2,wxSOLID));
                        dc->DrawSpline(count,gas);
//                      dc->SetPen(wxPen(wxColour(255,40,40),2,wxSOLID));
//                      dc->DrawSpline(count,fitness);
//                      dc->SetTextForeground(wxColour(255,40,40));
//                      dc->DrawText(_T("Fitness"),getInnerLeftBound()+1,getInnerUpperBound()+8);
                        dc->SetTextForeground(wxColour(80,80,255));
                        dc->DrawText(_T("Minerals"),getInnerLeftBound()+1,getInnerUpperBound()+10);
                        dc->SetTextForeground(wxColour(40,255,40));
                        dc->DrawText(_T("Gas"),getInnerLeftBound()+1,getInnerUpperBound()+21);
                        dc->SetTextForeground(wxColour(160,160,160));
                        dc->DrawText(_T("Supply"),getInnerLeftBound()+1,getInnerUpperBound()+32);
                }
                dc->SetPen(wxPen(wxColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
        }
                                                                                                                                                            
/*      if(infoWindowNumber)
        {
                ORDER* order=orderList.Find(infoWindowNumber-1)->GetData();
                dc->SetPen(wxPen(wxColour(150,0,0),2,wxSHORT_DASH));
                dc->DrawLine(getInnerLeftBound()+order->bx,getInnerUpperBound()+10,getInnerLeftBound()+order->bx,getInnerUpperBound()+getInnerHeight());
                dc->DrawLine(getInnerLeftBound()+order->bx+order->bwidth,getInnerUpperBound()+10,getInnerLeftBound()+order->bx+order->bwidth,getInnerUpperBound()+getInnerHeight());
                if(stats[anarace->getPlayer()->getRace()][order->unit].mins)
                {
                        dc->SetPen(wxPen(wxColour(120,120,255),2,wxSHORT_DASH));
                        dc->DrawLine(getInnerLeftBound()+order->bx+1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].mins/1000,getInnerLeftBound()+order->bx+order->bwidth-1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].mins/1000);
                                                                                                                                                            
                }
                if(stats[anarace->getPlayer()->getRace()][order->unit].gas)
                {
                        dc->SetPen(wxPen(wxColour(80,255,80),2,wxSHORT_DASH));
                        dc->DrawLine(getInnerLeftBound()+order->bx+1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].gas/1000,getInnerLeftBound()+order->bx+order->bwidth-1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].gas/1000);
                }
                if(stats[anarace->getPlayer()->getRace()][order->unit].supply)
                {
                        dc->SetPen(wxPen(wxColour(160,160,160),2,wxSHORT_DASH));
                        dc->DrawLine(getInnerLeftBound()+order->bx+1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].supply*5,getInnerLeftBound()+order->bx+order->bwidth-1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][order->unit].supply*5);
                }
        }*/
}

