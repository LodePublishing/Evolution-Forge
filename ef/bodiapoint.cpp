#include "bodiapoint.hpp"

BoDiagramPoint::BoDiagramPoint():
	initialized(false)
{
	for(unsigned int i = TOTAL_STATS; i--;)
		highlight[i] = 0;
}

BoDiagramPoint::~BoDiagramPoint()
{}

void BoDiagramPoint::setTargetY(eHaveStats stat_num, unsigned int number_y1, unsigned int number_y2)
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

unsigned int BoDiagramPoint::max[TOTAL_STATS];

