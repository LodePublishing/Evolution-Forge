#include "fpssystem.hpp"
#include <sstream>
#include "configuration.hpp"


FPS_SYSTEM::FPS_SYSTEM():
	fps(new FPS()),
	currentTicks(0), //lol, muss 0 sein
	framesPerSecond(10),
	totalTicks(500),
	tickIntervall(0),
	framesPerGeneration(0),
	refresh(0)
{
	for(unsigned int i = MAX_TICK_TYPES;i--;)
		for(unsigned int j = TICK_INTERVALL;j--;)
			ticks[i][j] = 0;
	for(unsigned int j = TICK_INTERVALL;j--;)
		frames_count[j]=0;
	for(unsigned int i = MAX_TICK_TYPES; i--;)
		percent[i] = 0;

	updateDesiredValues();
		
	refresh = fps->getFramesPerGeneration();
}

FPS_SYSTEM::~FPS_SYSTEM()
{
	delete fps;
}

void FPS_SYSTEM::updateDesiredValues()
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

void FPS_SYSTEM::poll(const eTicks etick)
{
	ticks[etick][tickIntervall] = SDL_GetTicks() - currentTicks;
	currentTicks = SDL_GetTicks();
}


void FPS_SYSTEM::process()
{
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
	}
}

const eBrush tickBrush[MAX_TICK_TYPES] = {BRIGHT_UNIT_TYPE_1_BRUSH, BRIGHT_UNIT_TYPE_2_BRUSH, BRIGHT_UNIT_TYPE_4_BRUSH, BRIGHT_UNIT_TYPE_5_BRUSH, BRIGHT_UNIT_TYPE_6_BRUSH, BRIGHT_UNIT_TYPE_7_BRUSH};
const std::string tickName[MAX_TICK_TYPES] = {"draw()", "newGeneration()", "process()", "message()", "sound()", "idle()"};

void FPS_SYSTEM::draw(DC* dc) const
{
	Point p = Point(15, UI_Object::max_y - MAX_TICK_TYPES*10 - 25);
	
	dc->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->setBrush(Brush(Color(dc->getSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
	dc->DrawEdgedRoundedRectangle(Rect(p, Size(150, MAX_TICK_TYPES*10+10)), 4);
	dc->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
	dc->setBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->DrawEdgedRoundedRectangle(Rect(p-Size(3,3), Size(156, MAX_TICK_TYPES*10+16)), 4);
	DC::addRectangle(Rect(p-Size(3,3), Size(156, MAX_TICK_TYPES*10+16)));

	dc->setTextForeground(DC::toSDL_Color(255, 20, 20));
	//dc->setTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOR));
	dc->setFont(UI_Object::theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
	for(unsigned int i=MAX_TICK_TYPES;i--;)
	{
		std::ostringstream os; os.str("");
		dc->setPen(*UI_Object::theme.lookUpPen((ePen)(tickBrush[i]-BRIGHT_UNIT_TYPE_0_BRUSH+UNIT_TYPE_0_PEN)));
		dc->setBrush(*UI_Object::theme.lookUpBrush(tickBrush[i]));
		dc->DrawRectangle(Rect(p + Size(10, 5+i*10), Size(percent[i], 10)));
		os << tickName[i] << " " << percent[i] << "%";
		dc->DrawText(os.str(), p + Size(20, 7+i*10));
	}
	int t = 0;
	for(unsigned int j = TICK_INTERVALL;j--;)
		t+=frames_count[j];
	t=t/TICK_INTERVALL;
	std::ostringstream os; os.str("");
	os << t << "fps";
	dc->DrawText(os.str(), p + Size(115, 6));

	os.str("");
	if(framesPerGeneration>0)
	{
		t = t * 100 / framesPerGeneration;
		os << t << "gps";
	} else
	os << " ---gps";
	dc->DrawText(os.str(), p + Size(115,20));
}

void FPS_SYSTEM::delay(const bool optimizing)
{
	framesPerGeneration = fps->getFramesPerGeneration();
	frames_count[tickIntervall] = fps->getCurrentFramerate();
	fps->setAdaptFramesPerGeneration(optimizing);
	updateDesiredValues();
	fps->setTotalTicks(1+totalTicks/TICK_INTERVALL);
	fps->delay();
	refresh += 100;
}

const bool FPS_SYSTEM::allowCalculation()
{
	bool allow = (refresh >= framesPerGeneration);
	if(framesPerGeneration > refresh)
		refresh = 0;
	else
		refresh -= framesPerGeneration;
	return(allow);
}

void FPS_SYSTEM::updateConfiguration()
{
	efConfiguration.setCurrentFramerate(framesPerSecond);
	efConfiguration.setCurrentFramesPerGeneration(framesPerGeneration);
}


