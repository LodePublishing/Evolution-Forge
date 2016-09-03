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
		newTextPage();
		writeLongText(_T(" "), dc);
		if(!anarace->getProgramSuccessUnit(order->IP))
		{
				writeLongText(_T(wxString::Format(wxT("Build $%i$. $%s$ as soon as $%s$ at $%s$ when having $%i$ minerals, $%i$ gas, $%i$/$%i$ supply [time: %.2i:%.2i]#"),
				anarace->getProgramForceCount(order->IP,order->unit)+1, 
				stats[anarace->getPlayer()->getRace()][order->unit].name, 
				error_message[anarace->getProgramSuccessType(order->IP)], 
				anarace->getMap()->getLocationName(anarace->getProgramLocation(order->IP)), 
				anarace->getStatisticsHaveMinerals(anarace->ga->maxTime-anarace->getProgramTime(order->IP))/100, 
				anarace->getStatisticsHaveGas(anarace->ga->maxTime-anarace->getProgramTime(order->IP))/100, 
				anarace->getStatisticsNeedSupply(anarace->ga->maxTime-anarace->getProgramTime(order->IP)),
				anarace->getStatisticsHaveSupply(anarace->ga->maxTime-anarace->getProgramTime(order->IP)),
				anarace->getProgramTime(order->IP)/60,
				anarace->getProgramTime(order->IP)%60)),dc);
		}
		else
		{
                writeLongText(_T(wxString::Format(wxT("Build $%i$. $%s$ as soon as $%s$ at $%s$ when $%s$ becomes availible and having $%i$ minerals, $%i$ gas, $%i$/$%i$ supply [time: %.2i:%.2i]#"),
                anarace->getProgramForceCount(order->IP,order->unit)+1,
                stats[anarace->getPlayer()->getRace()][order->unit].name,
                error_message[anarace->getProgramSuccessType(order->IP)],
                anarace->getMap()->getLocationName(anarace->getProgramLocation(order->IP)),
				stats[anarace->getPlayer()->getRace()][anarace->getProgramSuccessUnit(order->IP)].name,
                anarace->getStatisticsHaveMinerals(anarace->ga->maxTime-anarace->getProgramTime(order->IP))/100,
                anarace->getStatisticsHaveGas(anarace->ga->maxTime-anarace->getProgramTime(order->IP))/100,
                anarace->getStatisticsNeedSupply(anarace->ga->maxTime-anarace->getProgramTime(order->IP)),
                anarace->getStatisticsHaveSupply(anarace->ga->maxTime-anarace->getProgramTime(order->IP)),
                anarace->getProgramTime(order->IP)/60,
                anarace->getProgramTime(order->IP)%60)),dc);
			
		}
};

