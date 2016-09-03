#include "info.hpp"

InfoWindow::InfoWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber):UI_Window(parent, INFO_WINDOW_TITLE_STRING, INFO_WINDOW, windowNumber, NOT_SCROLLED)
{
	this->anarace=anarace;
	resetData();
}

InfoWindow::~InfoWindow()
{
}

const int InfoWindow::getBx() const
{
	return bx;
}

const int InfoWindow::getBWidth() const
{
	return bwidth;
}

const int InfoWindow::getUnit() const
{
	return unit;
}

void InfoWindow::setBx(int bx)
{
	this->bx=bx;
}

void InfoWindow::setBWidth(int bwidth)
{
	this->bwidth=bwidth;
}

void InfoWindow::setIP(int IP)
{
	this->IP=IP;
}

void InfoWindow::setUnit(int unit)
{
	this->unit=unit;
}

void InfoWindow::resetData()
{
	bx=0;
	bwidth=0;
	unit=0;
	IP=0;
	setup=0;
}

const int InfoWindow::isSet() const
{
	return(setup);
}

void InfoWindow::setupOk(int ok)
{
	setup=ok;
}

const int InfoWindow::getIP() const
{
	return(IP);
}

void InfoWindow::process()
{
	if(!shown) return;
	UI_Window::process();
}

void InfoWindow::draw(DC* dc) const
{
	if(!shown) return;
	UI_Window::draw(dc);
	stringstream os;
	os << stats[anarace->getRace()][unit].name << ": " << anarace->getProgramTime(IP);
	dc->DrawText(os.str(), getAbsoluteClientRectPosition());
	
/*	newTextPage();
		writeLongText(_T("#"), dc);
		if(!anarace->getProgramSuccessUnit(order->IP))
		{
				writeLongText(_T(String::Format(T("Build $%i$. $%s$ as soon as $%s$ at $%s$ when having $%i$ minerals, $%i$ gas, $%i$/$%i$ supply [time: %.2i:%.2i]"),
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
				writeLongText(_T(String::Format(T("Build $%i$. $%s$ as soon as $%s$ at $%s$ when $%s$ becomes availible and having $%i$ minerals, $%i$ gas, $%i$/$%i$ supply [time: %.2i:%.2i]"),
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
#endif*/
}

