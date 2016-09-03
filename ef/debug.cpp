#include "debug.hpp"
#include <sstream>
#include "configuration.hpp"

DebugWindow::DebugWindow( UI_Object* parent_window ) :
	UI_Window( parent_window, DEBUG_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(DEBUG_WINDOW), theme.lookUpGlobalMaxHeight(DEBUG_WINDOW), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 10, 1280, 1024) ),
	fps(new FPS()),
	currentTicks(0), //lol, muss 0 sein
	framesPerSecond(10),
	totalTicks(500),
	tickIntervall(0),
	framesPerGeneration(0),
	refresh(0),

	tickGroup(new UI_Group(this, Rect(Point(5, 10), Size(100,0)), Size(0,10), ONE_COLOUMN_GROUP, false, TOP_LEFT, NULL_STRING)),

	fpsText(new UI_StaticText(this, UI_Object::theme.lookUpString(FPS_TEXT_STRING), Rect(Point(0, 5), Size(0,0)), Size(10, 0), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, TOP_RIGHT)),
	gpsText(new UI_StaticText(this, UI_Object::theme.lookUpString(GPS_TEXT_STRING), Rect(Point(0, 0), Size(0,0)), Size(10, 5), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, BOTTOM_RIGHT))
{
	for(unsigned int i = MAX_TICK_TYPES;i--;)
		for(unsigned int j = TICK_INTERVALL;j--;)
			ticks[i][j] = 0;
	for(unsigned int j = TICK_INTERVALL;j--;)
		frames_count[j]=0;
	for(unsigned int i = MAX_TICK_TYPES; i--;)
	{
		percent[i] = 0;
		tickText[i] = new UI_StaticText(tickGroup, UI_Object::theme.lookUpString((eString)(DRAW_TICKS_STRING+i)), Rect(Point(0, 0), Size(0,0)), Size(0,0), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST);
	}
	
	updateDesiredValues();
		
	refresh = fps->getFramesPerGeneration();
}

DebugWindow::~DebugWindow()
{
	delete fps;
	for(unsigned int i = MAX_TICK_TYPES;i--;)
		delete tickText[i];
	delete tickGroup;
	delete fpsText;
	delete gpsText;
}

void DebugWindow::updateDesiredValues()
{
	if( fps->setDesiredFramerate(efConfiguration.getDesiredFramerate()) )
	{
		std::ostringstream os;
		os << efConfiguration.getDesiredFramerate();
		toInitLog("* " + UI_Object::theme.lookUpString(START_SET_DESIRED_FRAMERATE_STRING) + " " + os.str());
	}
	
	if( fps->setDesiredCPU(efConfiguration.getDesiredCPU()) )
	{
		std::ostringstream os;
		os << efConfiguration.getDesiredCPU();
		toInitLog("* " + UI_Object::theme.lookUpString(START_SET_DESIRED_CPU_STRING) + " " + os.str() + "%");
	}
}

void DebugWindow::poll(const eTicks etick)
{
	ticks[etick][tickIntervall] = SDL_GetTicks() - currentTicks;
	currentTicks = SDL_GetTicks();
}

void DebugWindow::process()
{
	UI_Window::process();
	if(tickGroup->checkForNeedRedraw())
		setNeedRedrawNotMoved();
	tickIntervall++;
	if(tickIntervall >= TICK_INTERVALL)
		tickIntervall = 0;
	
	totalTicks = 0;
	unsigned int total_this_ticks[MAX_TICK_TYPES];
	for(unsigned int i = MAX_TICK_TYPES;i--;)
	{
		total_this_ticks[i]=0;
		for(unsigned int j = TICK_INTERVALL;j--;)
			total_this_ticks[i] += ticks[i][j];
		totalTicks+=total_this_ticks[i];
		
	}
	if(totalTicks == 0)
		totalTicks = 1;
	unsigned int total_frames = 0;
	for(unsigned int j = TICK_INTERVALL;j--;)
		total_frames += frames_count[j];
	total_frames /= TICK_INTERVALL;

	if(framesPerSecond > total_frames)
		framesPerSecond -= (framesPerSecond - total_frames)/5 + 1;
	else
	if(framesPerSecond < total_frames)
		framesPerSecond += (total_frames - framesPerSecond)/5 + 1;
	if(framesPerSecond < 0) 
		framesPerSecond = 0;
		
	for(unsigned int i = MAX_TICK_TYPES;i--;)
	{
		if(percent[i] > total_this_ticks[i] * 100 / totalTicks)
			percent[i] -= (percent[i] - total_this_ticks[i] * 100 / totalTicks)/5 + 1;
		else
		if(percent[i] < total_this_ticks[i] * 100 / totalTicks)
			percent[i] += (total_this_ticks[i] * 100 / totalTicks - percent[i])/5 + 1;
		if(percent[i] < 0) percent[i]=0;
		if(percent[i] > 100) percent[i]=100;

		std::ostringstream os; os.str("");
		os << UI_Object::theme.lookUpString((eString)(DRAW_TICKS_STRING+i)) << " " << percent[i] << "%";
		tickText[i]->updateText(os.str());
	}
	
	int t = 0;
	for(unsigned int j = TICK_INTERVALL;j--;)
		t+=frames_count[j];
	t=t/TICK_INTERVALL;
	std::ostringstream os; os.str("");
	os << t << "fps";
	fpsText->updateText(os.str());

	os.str("");
	/*
	if(framesPerGeneration>0)
	{
		t = t * 100 / framesPerGeneration;
		os << t << "gps";
	} else
	os << " ---gps";*/
	os << generationsPerSecond << "gps";
	gpsText->updateText(os.str());
	generationsPerSecond = 0;
}

void DebugWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(DEBUG_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(DEBUG_WINDOW));
	UI_Window::reloadOriginalSize();
}

const eBrush tickBrush[MAX_TICK_TYPES] = {BRIGHT_UNIT_TYPE_1_BRUSH, BRIGHT_UNIT_TYPE_2_BRUSH, BRIGHT_UNIT_TYPE_4_BRUSH, BRIGHT_UNIT_TYPE_5_BRUSH, BRIGHT_UNIT_TYPE_6_BRUSH, BRIGHT_UNIT_TYPE_7_BRUSH};

void DebugWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::drawWindow(dc);

	for(unsigned int i=MAX_TICK_TYPES;i--;)
	{
		dc->setPen(*UI_Object::theme.lookUpPen((ePen)(tickBrush[i] - BRIGHT_UNIT_TYPE_0_BRUSH + UNIT_TYPE_0_PEN)));
		dc->setBrush(*UI_Object::theme.lookUpBrush(tickBrush[i]));
		
		dc->DrawRectangle(Rect(tickText[i]->getAbsolutePosition(), Size(getClientRectWidth() * percent[i]/100, 10)));
	}
	UI_Object::draw(dc);
}

void DebugWindow::delay(const bool optimizing)
{
	framesPerGeneration = fps->getFramesPerGeneration();
	frames_count[tickIntervall] = fps->getCurrentFramerate();
	fps->setAdaptFramesPerGeneration(optimizing);
	updateDesiredValues();
	fps->setTotalTicks(1+totalTicks/TICK_INTERVALL);
	fps->delay();
	refresh += 100;
}

const bool DebugWindow::allowCalculation()
{
	bool allow = (refresh >= framesPerGeneration);
	if(framesPerGeneration > refresh)
		refresh = 0;
	else
		refresh -= framesPerGeneration;
	generationsPerSecond++;
	return(allow);
}

void DebugWindow::updateConfiguration()
{
	efConfiguration.setCurrentFramerate(framesPerSecond);
	efConfiguration.setCurrentFramesPerGeneration(framesPerGeneration);
}


