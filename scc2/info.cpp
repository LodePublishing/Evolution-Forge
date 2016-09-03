#include "info.h"


int InfoWindow::getBx()
{
	return bx;
};

int InfoWindow::getBWidth()
{
	return bwidth;
};

int InfoWindow::getUnit()
{
	return unit;
};

int InfoWindow::getKey()
{
	return key;
};

void InfoWindow::setBx(int bx)
{
	this->bx=bx;
};

void InfoWindow::setBWidth(int bwidth)
{
	this->bwidth=bwidth;
};

void InfoWindow::setUnit(int unit)
{
	this->unit=unit;
};

void InfoWindow::setKey(int key)
{
	this->key=key;
};

void InfoWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
}

void InfoWindow::setOrder(ORDER* order)
{
	this->order=order;
};

InfoWindow::InfoWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(2,rahmen,maxSize,NOT_SCROLLED)
{
        resetData();
};
                                                                                                                                                           
void InfoWindow::resetData()
{
	bx=0;
	bwidth=0;
	unit=0;
	key=0;
	order=0;
	setup=0;
};

int InfoWindow::isSet()
{
	return(setup);
};

void InfoWindow::setupOk(int ok)
{
	setup=ok;
};

ORDER* InfoWindow::getOrder()
{
	return(order);
};
                                                                                                                                                            
void InfoWindow::drawInfo(wxDC* dc)
{
        if(!order)
                return;
	int t=0;
//      Draw(dc);

        dc->SetTextForeground(wxColour(INFOWINDOWTEXTR,INFOWINDOWTEXTG,INFOWINDOWTEXTB));
        dc->DrawText(_T(wxString::Format(wxT("Build %i. %s"),anarace->getProgramForceCount(order->IP,order->unit)+1,stats[anarace->getPlayer()->getRace()][order->unit].name)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
dc->DrawText(_T(wxString::Format(wxT("at %s"),anarace->getMap()->getLocationName(anarace->getProgramLocation(order->IP)))),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;         
	dc->DrawText(_T(wxString::Format(wxT("having %i minerals"),anarace->getStatisticsHaveMinerals(anarace->ga->maxTime-anarace->getProgramTime(order->IP))/100)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
        dc->DrawText(_T(wxString::Format(wxT("and %i gas"),anarace->getStatisticsHaveGas(anarace->ga->maxTime-anarace->getProgramTime(order->IP))/100)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;         dc->DrawText(_T(wxString::Format(wxT("and %i of %i supply"),anarace->getStatisticsNeedSupply(anarace->ga->maxTime-anarace->getProgramTime(order->IP)),anarace->getStatisticsHaveSupply(anarace->ga->maxTime-anarace->getProgramTime(order->IP)))),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
        dc->DrawText(_T(wxString::Format(wxT("as soon as %s"),error_message[anarace->getProgramSuccessType(order->IP)])),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
        if(anarace->getProgramSuccessUnit(order->IP)>0) 
	{
                 dc->DrawText(_T(wxString::Format(wxT("%s becomes availible."),stats[anarace->getPlayer()->getRace()][anarace->getProgramSuccessUnit(order->IP)].name)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));t++;
	}
        dc->DrawText(_T(wxString::Format(wxT("(time %.2i:%.2i)"),anarace->getProgramTime(order->IP)/60,anarace->getProgramTime(order->IP)%60)),getInnerLeftBound()+5,getInnerUpperBound()+5+t*(FONT_SIZE+5));
                                                                                                                                                            
//TODO Daten selber in infowindow schreioben
};

