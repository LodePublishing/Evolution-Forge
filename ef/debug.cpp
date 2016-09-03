#include "debug.hpp"
#include <sstream>

DebugWindow::DebugWindow( UI_Object* parent_window ) :
	UI_Window( parent_window, DEBUG_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(DEBUG_WINDOW), theme.lookUpGlobalMaxHeight(DEBUG_WINDOW), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 10, 1280, 1024) ),
	framesPerSecond(10),
	generationsPerSecond(0),
	percentList(),
	tickGroup(new UI_Group(this, Rect(Point(5, 10), Size(100,0)), Size(0,10), ONE_COLOUMN_GROUP, false, TOP_LEFT, NULL_STRING)),
	fpsText(new UI_StaticText(this, UI_Object::theme.lookUpString(FPS_TEXT_STRING), Rect(Point(0, 5), Size(0,0)), Size(10, 0), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, TOP_RIGHT)),
	gpsText(new UI_StaticText(this, UI_Object::theme.lookUpString(GPS_TEXT_STRING), Rect(Point(50, 0), Size(0,0)), Size(10, 5), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST /*BOTTOM_RIGHT*/))
{
	for(unsigned int i = 0; i < MAX_TICK_TYPES; i++)
		tickText[i] = new UI_StaticText(tickGroup, UI_Object::theme.lookUpString((eString)(DRAW_TICKS_STRING+i)), Rect(Point(0, 0), Size(0,0)), Size(0,0), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST);
}

DebugWindow::~DebugWindow()
{
	for(unsigned int i = MAX_TICK_TYPES;i--;)
		delete tickText[i];
	delete tickGroup;
	delete fpsText;
	delete gpsText;
}

void DebugWindow::updateData(const std::list<unsigned int> percent_list, const unsigned int fps, const unsigned int gps)
{
#ifdef _SCC_DEBUG
	if(percent_list.size() > MAX_TICK_TYPES) {
		toErrorLog("DEBUG (DebugWindow::updateData()): percent_list too large.");return;
	}
		
#endif 
	percentList = percent_list;
	framesPerSecond = fps;
	generationsPerSecond = gps;
}

void DebugWindow::process()
{
	UI_Window::process();

	unsigned int n = 0;
	for(std::list<unsigned int>::iterator i = percentList.begin(); i != percentList.end(); ++i, ++n)
	{
		std::ostringstream os; os.str("");
		os << UI_Object::theme.lookUpString((eString)(DRAW_TICKS_STRING+n)) << " " << *i << "%";
		tickText[n]->updateText(os.str());
	}
	std::ostringstream os; os.str("");
	os << framesPerSecond << " fps";
	fpsText->updateText(os.str());

	os.str("");
	os << generationsPerSecond << "gps";
	gpsText->updateText(os.str());
}

void DebugWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(DEBUG_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(DEBUG_WINDOW));
	toErrorLog("reload original size debug window");
	UI_Window::reloadOriginalSize();
}

const eBrush tickBrush[MAX_TICK_TYPES] = {BRIGHT_UNIT_TYPE_1_BRUSH, BRIGHT_UNIT_TYPE_2_BRUSH, BRIGHT_UNIT_TYPE_4_BRUSH, BRIGHT_UNIT_TYPE_5_BRUSH, BRIGHT_UNIT_TYPE_6_BRUSH, BRIGHT_UNIT_TYPE_7_BRUSH};

void DebugWindow::draw() const
{
	UI_Window::drawWindow();

/*	unsigned int n = 0;
	for(std::list<unsigned int>::const_iterator i = percentList.begin(); i != percentList.end(); ++i, ++n)
	{
		dc->setPen(*UI_Object::theme.lookUpPen((ePen)(tickBrush[n] - BRIGHT_UNIT_TYPE_0_BRUSH + UNIT_TYPE_0_PEN)));
		dc->setBrush(*UI_Object::theme.lookUpBrush(tickBrush[n]));
		dc->DrawRectangle(Rect(tickText[n]->getRelativePosition(), Size(getClientRectWidth() * (*i)/100, 10)));
	}*/ // TODO
	UI_Object::draw();
}

