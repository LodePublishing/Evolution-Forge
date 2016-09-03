#include "bodiapoint.hpp"

BoDiagramPoint::BoDiagramPoint():
	initialized(false)
{
	current_y1.assign(MAX_ENTRIES, 0);
	current_y2.assign(MAX_ENTRIES, 0);
	start_y1.assign(MAX_ENTRIES, 0);
	start_y2.assign(MAX_ENTRIES, 0);
	target_y1.assign(MAX_ENTRIES, 0);
	target_y2.assign(MAX_ENTRIES, 0);
	highlight.assign(MAX_ENTRIES, 0);
}

BoDiagramPoint::~BoDiagramPoint()
{}

void BoDiagramPoint::setTargetY(unsigned int stat_num, unsigned int number_y1, unsigned int number_y2)
{
	if((number_y1 != target_y1[stat_num]) || ( number_y2 != target_y2[stat_num]))
		highlight[stat_num] = 150;
	
	if(!initialized)
	{
		current_y1[stat_num] = start_y1[stat_num] = target_y1[stat_num] = number_y1;
		current_y2[stat_num] = start_y2[stat_num] = target_y2[stat_num] = number_y2;
	} else
	{
		if(number_y1 != target_y1[stat_num])
		{
			start_y1[stat_num] = current_y1[stat_num];
			target_y1[stat_num] = number_y1;
		}
		if(number_y2 != target_y2[stat_num])
		{
			start_y2[stat_num] = current_y2[stat_num];
			target_y2[stat_num] = number_y2;
		}
	}
}

void BoDiagramPoint::setTargetX(unsigned int number_x)
{
	/*if(number_x != target_x)
	{
		for(unsigned int i = TOTAL_STATS; i--;)
			highlight[i] = 150;
	}*/
	if(true)//!initialized)
	{
		current_x = start_x = target_x = number_x;
	}
	else
	if(number_x != target_x)
	{
		start_x = current_x;
		target_x = number_x;
	}
}

std::vector<unsigned int> BoDiagramPoint::max;
unsigned int BoDiagramPoint::MAX_ENTRIES = 0;

