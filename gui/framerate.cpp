#include "framerate.hpp"
#include "configuration.hpp"

UI_FrameRateControl::UI_FrameRateControl():
	fps(new FPS()),
	currentTicks(0), //lol, muss 0 sein
	framesPerSecond(10),
	totalTicks(500),
	tickIntervall(0),
	framesPerGeneration(0),
	refresh(0),
	generationsPerSecond(0)
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

UI_FrameRateControl::~UI_FrameRateControl()
{
	delete fps;
}

#include <sstream>
const bool UI_FrameRateControl::updateDesiredValues()
{
	bool has_changed = false;
	if( fps->setDesiredFramerate(uiConfiguration.getDesiredFramerate()) )
		has_changed = true;
	if( fps->setDesiredCPU(uiConfiguration.getDesiredCPU()) )
		has_changed = true;
	return has_changed;

}

void UI_FrameRateControl::poll(const eTicks etick)
{
	ticks[etick][tickIntervall] = SDL_GetTicks() - currentTicks;
	currentTicks = SDL_GetTicks();
}

void UI_FrameRateControl::calculateFramerate()
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
	
	framesPerSecond = 0;
	for(unsigned int j = TICK_INTERVALL;j--;)
		framesPerSecond += frames_count[j];
	framesPerSecond /= TICK_INTERVALL;
}

void UI_FrameRateControl::delay(const bool optimizing)
{
	calculateFramerate();
	framesPerGeneration = fps->getFramesPerGeneration();
	frames_count[tickIntervall] = fps->getCurrentFramerate();
	fps->setAdaptFramesPerGeneration(optimizing);
	updateDesiredValues();
	fps->setTotalTicks(1+totalTicks/TICK_INTERVALL);
	fps->delay();
	refresh += 100;
}

const bool UI_FrameRateControl::allowCalculation()
{
	bool allow = (refresh >= framesPerGeneration);
	if(framesPerGeneration > refresh)
		refresh = 0;
	else
		refresh -= framesPerGeneration;
	generationsPerSecond++;
	return(allow);
}

void UI_FrameRateControl::updateConfiguration()
{
	uiConfiguration.setCurrentFramerate(framesPerSecond);
	uiConfiguration.setCurrentFramesPerGeneration(framesPerGeneration);
}

const std::list<unsigned int> UI_FrameRateControl::getPercentList() const
{
	std::list<unsigned int> t;
	for(unsigned int i = 0; i < MAX_TICK_TYPES; i++)
		t.push_back(percent[i]);
	return(t);
}

const unsigned int UI_FrameRateControl::getFramesPerSecond() const
{
	return(framesPerSecond);
}

const unsigned int UI_FrameRateControl::getGenerationsPerSecond() const
{
	return(generationsPerSecond);
}

