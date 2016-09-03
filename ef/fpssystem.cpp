#include "fpssystem.hpp"
#include "configuration.hpp"
#include "../stl/misc.hpp"
#include <sstream>

FPS_SYSTEM::FPS_SYSTEM():
	fps(new FPS()),
	current_ticks(500),
	frames_per_second(10),
	total_ticks(500),
	tick_intervall(0),
	frames_per_generation(0),
	refresh(0)
{
	for(unsigned int i = MAX_TICK_TYPES;i--;)
		for(unsigned int j = TICK_INTERVALL;j--;)
			ticks[i][j] = 10;
	for(unsigned int j = TICK_INTERVALL;j--;)
		frames_count[j]=5;
	for(unsigned int i = MAX_TICK_TYPES; i--;)
		percent[i] = 10;
		
	{
		std::ostringstream os;
		os << efConfiguration.getDesiredFramerate();
		toLog("* Setting desired framerate to " + os.str());
	}
	fps->setDesiredFramerate(efConfiguration.getDesiredFramerate());
	{
	//	std::ostringstream os;
	//	os << efConfiguration.getDesiredCPU();
	//	toLog("* Setting desired CPU usage to " + os.str() + "%");
	}
	fps->setDesiredCPU(efConfiguration.getDesiredCPU());
	refresh = fps->getFramesPerGeneration();
}

FPS_SYSTEM::~FPS_SYSTEM()
{
	delete fps;
}


void FPS_SYSTEM::poll(const eTicks etick)
{
	ticks[etick][tick_intervall] = SDL_GetTicks() - current_ticks;
	current_ticks = SDL_GetTicks();
}


void FPS_SYSTEM::process()
{
	tick_intervall++;
	if(tick_intervall >= TICK_INTERVALL)
		tick_intervall = 0;
	
	total_ticks = 0;
	unsigned int total_this_ticks[MAX_TICK_TYPES];
	for(unsigned int i = MAX_TICK_TYPES;i--;)
	{
		total_this_ticks[i]=0;
		for(unsigned int j = TICK_INTERVALL;j--;)
			total_this_ticks[i] += ticks[i][j];
		total_ticks+=total_this_ticks[i];
		
	}
	total_ticks+=1;

	unsigned int total_frames = 0;
	for(unsigned int j = TICK_INTERVALL;j--;)
		total_frames += frames_count[j];
	total_frames /= TICK_INTERVALL;

	if(frames_per_second > total_frames)
		frames_per_second -= (frames_per_second - total_frames)/5 + 1;
	else
	if(frames_per_second < total_frames)
		frames_per_second += (total_frames - frames_per_second)/5 + 1;
	if(frames_per_second < 0) 
		frames_per_second = 0;
		
	for(unsigned int i = MAX_TICK_TYPES;i--;)
	{
		if(percent[i] > total_this_ticks[i] * 100 / total_ticks)
			percent[i] -= (percent[i] - total_this_ticks[i] * 100 / total_ticks)/5 + 1;
		else
		if(percent[i] < total_this_ticks[i] * 100 / total_ticks)
			percent[i] += (total_this_ticks[i] * 100 / total_ticks - percent[i])/5 + 1;
		if(percent[i] < 0) percent[i]=0;
		if(percent[i] > 100) percent[i]=100;
	}
}

const eBrush tickBrush[MAX_TICK_TYPES] = {BRIGHT_UNIT_TYPE_1_BRUSH, BRIGHT_UNIT_TYPE_2_BRUSH, BRIGHT_UNIT_TYPE_4_BRUSH, BRIGHT_UNIT_TYPE_5_BRUSH, BRIGHT_UNIT_TYPE_6_BRUSH, BRIGHT_UNIT_TYPE_7_BRUSH};
const std::string tickName[MAX_TICK_TYPES] = {"process()", "draw()", "sound()", "newGeneration()", "message()", "idle()"};

void FPS_SYSTEM::draw(DC* dc) const
{
	Point p = Point(15, UI_Object::max_y - MAX_TICK_TYPES*10 - 25);
	
	dc->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->SetBrush(Brush(Color(dc->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
	dc->DrawEdgedRoundedRectangle(Rect(p, Size(150, MAX_TICK_TYPES*10+10)), 4);
	dc->SetPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
	dc->SetBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->DrawEdgedRoundedRectangle(Rect(p-Size(3,3), Size(156, MAX_TICK_TYPES*10+16)), 4);
	DC::addRectangle(Rect(p-Size(3,3), Size(156, MAX_TICK_TYPES*10+16)));

	dc->SetTextForeground(DC::toSDL_Color(255, 20, 20));
	//dc->SetTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOR));
	dc->SetFont(UI_Object::theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
	for(unsigned int i=MAX_TICK_TYPES;i--;)
	{
		std::ostringstream os; os.str("");
		dc->SetPen(*UI_Object::theme.lookUpPen((ePen)(tickBrush[i]-BRIGHT_UNIT_TYPE_0_BRUSH+UNIT_TYPE_0_PEN)));
		dc->SetBrush(*UI_Object::theme.lookUpBrush(tickBrush[i]));
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
}

void FPS_SYSTEM::delay(const bool optimizing)
{
	frames_per_generation = fps->getFramesPerGeneration();
	frames_count[tick_intervall] = fps->getCurrentFramerate();
	fps->setAdaptFramesPerGeneration(optimizing);
	fps->setDesiredFramerate(efConfiguration.getDesiredFramerate());
	fps->setDesiredCPU(efConfiguration.getDesiredCPU());
	fps->setTotalTicks(1+total_ticks/TICK_INTERVALL);
	fps->delay();
	refresh += 100;
}

const bool FPS_SYSTEM::allowCalculation()
{
	bool allow = (refresh > frames_per_generation);
	if(frames_per_generation > refresh)
		refresh = 0;
	else
		refresh -= frames_per_generation;
	return(allow);
}

void FPS_SYSTEM::updateConfiguration()
{
	efConfiguration.setCurrentFramerate(frames_per_second);
	efConfiguration.setCurrentFramesPerGeneration(frames_per_generation);
}


