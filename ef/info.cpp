#include "info.hpp"

#include <sstream>

InfoWindow& InfoWindow::operator=(const InfoWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
	unit = object.unit;
	delete text;
	text = new UI_StaticText(*object.text);
	IP = object.IP;
	anarace = object.anarace;
	bo = object.bo;
	bg = object.bg;
	return(*this);
}

InfoWindow::InfoWindow(const InfoWindow& object) :
	UI_Window((UI_Window)object),
	unit(object.unit),
	text(new UI_StaticText(*object.text)),
	IP(object.IP),
	anarace(object.anarace),
	bo(object.bo),
	bg(object.bg)
{ }

InfoWindow::InfoWindow(UI_Object* info_parent, ANARACE* info_anarace, const unsigned int info_window_number):
	UI_Window(info_parent, INFO_WINDOW_TITLE_STRING, INFO_WINDOW, info_window_number, NOT_SCROLLED),
	unit(0),
	text(new UI_StaticText(this, "nothing", getRelativeClientRect(), BRIGHT_TEXT_COLOR, SMALL_MIDDLE_NORMAL_FONT, FORMATTED_NON_BLOCK_TEXT_MODE)),
	IP(0),
	anarace(info_anarace),
	bo(NULL),
	bg(NULL)
{ }

InfoWindow::~InfoWindow()
{
	delete text;
}

void InfoWindow::assignAnarace(ANARACE* info_anarace)
{
	anarace = info_anarace;
}

void InfoWindow::assignBg(const BoGraphEntry* info_bg)
{
	bg = info_bg;
}

const unsigned int InfoWindow::getUnit() const {
	return unit;
}

void InfoWindow::setIP(const unsigned int ip) {
	IP = ip;
}

void InfoWindow::setUnit(const unsigned int unit_type) {
	unit = unit_type;
}

void InfoWindow::resetData()
{
	unit = 0;
	IP = 0;
	bo = NULL;
	bg = NULL;
}

const unsigned int InfoWindow::getIP() const {
	return(IP);
}

void InfoWindow::assignBo(const BoEntry* info_bo)
{
	bo = info_bo;
}

void InfoWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();
	std::ostringstream os;
//	os << "$aaaaa$#";
	os << *UI_Object::theme.lookUpString(INFO_BUILD_STRING) << " $" 
	   << *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+unit+UNIT_NULL_STRING)) << "$ "
	   << *UI_Object::theme.lookUpString(INFO_AS_SOON_AS_STRING) << " $&"
	   << *UI_Object::theme.lookUpString((eString)(anarace->getProgramSuccessType(IP)+SUCCESS_OK_STRING)) << "&$ ";
	if(anarace->getProgramSuccessUnit(IP))
		os << "&$" << *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+anarace->getProgramSuccessUnit(IP)+UNIT_NULL_STRING)) << "$& " 
  		   << *UI_Object::theme.lookUpString(INFO_BECOMES_AVAILIBLE_STRING) << " ";
	// TODO
	os << *UI_Object::theme.lookUpString(INFO_AT_STRING) << " $"
		<< (*anarace->getMap())->getLocation(anarace->getProgramLocation(IP))->getName() << "$ "
		<< *UI_Object::theme.lookUpString(INFO_WHEN_STRING) << " "
		<< *UI_Object::theme.lookUpString(INFO_HAVING_STRING) << " &$" 
		<< (int)(anarace->getIPStatisticsHaveMinerals(IP*2+1)/100) << "$ " 
		<< *UI_Object::theme.lookUpString(INFO_MINERALS_STRING) << ",& &$" 
		<< (int)(anarace->getIPStatisticsHaveGas(IP*2+1)/100) << "$ "
		<< *UI_Object::theme.lookUpString(INFO_GAS_STRING) << ",& &$" 
		<< anarace->getIPStatisticsNeedSupply(IP*2+1) << "$/$" 
		<< anarace->getIPStatisticsHaveSupply(IP*2+1) << "$ "
		<< *UI_Object::theme.lookUpString(INFO_SUPPLY_STRING) << "& &["
		<< *UI_Object::theme.lookUpString(INFO_TIME_STRING) << ": $" 
		<< anarace->getRealProgramTime(IP)/60 << "$:$";
	  if((anarace->getRealProgramTime(IP)%60)<9) os << "0";
	  	os << anarace->getRealProgramTime(IP)%60 << "$]& #";
	text->updateText(os.str());
}

void InfoWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	if(bo)
	{
		dc->SetPen(Pen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN)->GetColor(), 2, SOLID_PEN_STYLE));
		dc->DrawLine(getAbsolutePosition() + Size(getWidth() / 2, 0) - Size(0, 3), bo->getAbsolutePosition() + Size(0, bo->getHeight() / 2) - Size(2, 0));
		dc->SetPen(Pen(*theme.lookUpPen(BIG_BUTTONS_PEN)->GetColor(), 1, SOLID_PEN_STYLE));
		dc->DrawLine(getAbsolutePosition() + Size(getWidth() / 2+2, 0) - Size(0, 3), bo->getAbsolutePosition() + Size(0, bo->getHeight() / 2+2) - Size(2, 0));
		dc->DrawLine(getAbsolutePosition() + Size(getWidth() / 2-1, 0) - Size(0, 3), bo->getAbsolutePosition() + Size(0, bo->getHeight() / 2) - Size(2, 1));
	}
	UI_Window::draw(dc);
}

