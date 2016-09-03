#include "info.hpp"

#include <sstream>

InfoWindow& InfoWindow::operator=(const InfoWindow& object)
{
    ((UI_Window)(*this)) = ((UI_Window)object);
    bx = object.bx;
    bwidth = object.bwidth;
    unit = object.unit;
    key = object.key;
	delete text;
    text = new UI_StaticText(*object.text);
    setup = object.setup;
    IP = object.IP;
    anarace = object.anarace;
	return(*this);
}

InfoWindow::InfoWindow(const InfoWindow& object) :
    UI_Window((UI_Window)object),
    bx(object.bx),
    bwidth(object.bwidth),
    unit(object.unit),
    key(object.key),
    text(new UI_StaticText(*object.text)),
    setup(object.setup),
    IP(object.IP),
    anarace(object.anarace)
{ }

InfoWindow::InfoWindow(UI_Object* info_parent, ANARACE* info_anarace, const unsigned int info_window_number):
	UI_Window(info_parent, INFO_WINDOW_TITLE_STRING, INFO_WINDOW, info_window_number, NOT_SCROLLED),
	bx(0),
	bwidth(0),
	unit(0),
	key(0),
	text(new UI_StaticText(this, "nothing", getRelativeClientRect(), FORMATTED_TEXT_MODE, BRIGHT_TEXT_COLOR, SMALL_MIDDLE_NORMAL_FONT)),
	setup(0),
	IP(0),
	anarace(info_anarace)
{
//	resetData(); // TODO raus...
}

InfoWindow::~InfoWindow()
{
	delete text;
}

const signed int InfoWindow::getBx() const
{
	return bx;
}

const unsigned int InfoWindow::getBWidth() const
{
	return bwidth;
}

const unsigned int InfoWindow::getUnit() const
{
	return unit;
}

void InfoWindow::setBx(const signed int bx)
{
	this->bx=bx;
}

void InfoWindow::setBWidth(const unsigned int bwidth)
{
	this->bwidth=bwidth;
}

void InfoWindow::setIP(const unsigned int IP)
{
	this->IP=IP;
}

void InfoWindow::setUnit(const unsigned int unit)
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

const unsigned int InfoWindow::isSet() const
{
	return(setup);
}

void InfoWindow::setupOk(const unsigned int ok)
{
	setup=ok;
}

const unsigned int InfoWindow::getIP() const
{
	return(IP);
}

void InfoWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();
	std::ostringstream os;
	os << "Build $" << stats[anarace->getRace()][unit].name << 
		  "$ as soon as $" << error_message[anarace->getProgramSuccessType(IP)] <<
		  "$ at $" << (*anarace->getMap())->getLocation(anarace->getProgramLocation(IP))->getName() << "$ when";
	if(anarace->getProgramSuccessUnit(IP))
		os << "$" << stats[anarace->getRace()][anarace->getProgramSuccessUnit(IP)].name << "$ becomes availible and";
	os << " having $" << (int)(anarace->getStatisticsHaveMinerals(IP)/100) << 
	      "$ minerals, $" << (int)(anarace->getStatisticsHaveGas(IP)/100) << 
		  "$ gas,$" << anarace->getStatisticsNeedSupply(IP) << 
		  "$/$" << anarace->getStatisticsHaveSupply(IP) <<
		  "$ supply [time: $" << anarace->getProgramTime(IP)/60 << 
		  "$:$" << anarace->getProgramTime(IP)%60 << "$]#";
	text->updateText(os.str());
}

void InfoWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
//	dc->DrawText(os.str(), getAbsoluteClientRectPosition());
	
	
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

