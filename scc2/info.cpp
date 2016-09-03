#include "info.h"

void InfoWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
}

InfoWindow::InfoWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(2,rahmen,maxSize,NOT_SCROLLED)
{
        resetData();
};
                                                                                                                                                            
void InfoWindow::resetData()
{
//      infoWindowNumber=NULL;
};
                                                                                                                                                            
void InfoWindow::showInfoWindow(wxDC* dc)
{
        //TODO: Daten direkt in infowindow abspeichern... bei check for infowindow
//      if(!infoWindowNumber)
//              return;
//      NODE* node=orderList.Find(infoWindowNumber-1);
//      if(!node)
//              return;
                                                                                                                                                            
//      ORDER* order=infoWindowNumber;//node->GetData();
                                                                                                                                                            
//      int t=0;
        Draw(dc);
                                                                                                                                                            
/*      dc->SetTextForeground(wxColour(INFOWINDOWTEXTR,INFOWINDOWTEXTG,INFOWINDOWTEXTB));
        dc->DrawText(_T(wxString::Format(wxT("Build %i. %s"),anarace->getProgramForceCount(order->IP,order->unit)+1,stats[anarace->getPlayer()->getRace()][order->unit].name)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
dc->DrawText(_T(wxString::Format(wxT("at %s"),anarace->getMap()->location[anarace->getProgramLocation(order->IP)].getName())),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;         dc->DrawText(_T(wxString::Format(wxT("having %i minerals"),anarace->getStatisticsHaveMinerals(anarace->getProgramTime(order->IP))/100)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
        dc->DrawText(_T(wxString::Format(wxT("and %i gas"),anarace->getStatisticsHaveGas(anarace->getProgramTime(order->IP))/100)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;         dc->DrawText(_T(wxString::Format(wxT("and %i of %i supply"),anarace->getStatisticsNeedSupply(anarace->getProgramTime(order->IP)),anarace->getStatisticsHaveSupply(anarace->getProgramTime(order->IP)))),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
        dc->DrawText(_T(wxString::Format(wxT("as soon as %s"),error_message[anarace->getProgramSuccessType(order->IP)])),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
        if(anarace->getProgramSuccessUnit(order->IP)>0) {                 dc->DrawText(_T(wxString::Format(wxT("%s becomes availible."),stats[anarace->getPlayer()->getRace()][anarace->getProgramSuccessUnit(order->IP)].name)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;}
        dc->DrawText(_T(wxString::Format(wxT("(time %.2i:%.2i)"),anarace->getProgramTime(order->IP)/60,anarace->getProgramTime(order->IP)%60)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));*/
                                                                                                                                                            
//TODO Daten selber in infowindow schreioben
};

