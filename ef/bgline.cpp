#include "bgline.hpp"

BoGraphLine::BoGraphLine(UI_Object* bg_parent, const Rect& initial_rect, const eRace bg_race, const unsigned int bg_facility, const unsigned int bg_line_height, const unsigned int bg_lines) :
	UI_Object(bg_parent, initial_rect),
	boGraphList(),
	race(bg_race),
	facility(bg_facility),
	lineHeight(bg_line_height),
	lines(bg_lines),
	facilityChanged(false),
	facilityName(new UI_StaticText(this, stats[race][facility].name, Rect(2,2,0,0), Size(), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST))
{}

BoGraphLine::~BoGraphLine()
{
	delete facilityName;
	std::list<BoGraphEntry*>::iterator i = boGraphList.begin();
	while(i != boGraphList.end())
	{	
		delete(*i);
		i = boGraphList.erase(i);
	}
}

void BoGraphLine::resetData()
{
	facility = 0;
	lineHeight = 0;
	lines = 0;
	race = TERRA;
}

void BoGraphLine::process()
{
	if(facilityChanged)
	{
		facilityChanged = false;
		facilityName->updateText(stats[race][facility].name);
	}
	UI_Object::process();
}

void BoGraphLine::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(checkForNeedRedraw())
	{
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
		dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->DrawRectangle(getAbsoluteRect());
		facilityName->Hide();
	}
	UI_Object::draw(dc);
	if(!facilityName->isShown())
	{
		facilityName->Show();
		facilityName->draw(dc);
	}
}

void BoGraphLine::checkSelected(const unsigned int selected)
{
	for(std::list<BoGraphEntry*>::iterator entry = boGraphList.begin(); entry!=boGraphList.end(); ++entry)
		if((*entry)->getNumber() == selected)
		{
			(*entry)->resetGradient();
			return;
		}
}

/*	std::list<BoGraphEntry*>::iterator j = boGraphList.begin();
		while(j != boGraphList.end())
		{
			delete(*j);
			j = boGraphList.erase(j);
		}
*/
