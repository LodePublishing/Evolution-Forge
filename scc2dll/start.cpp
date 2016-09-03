#include "start.h"

START::START()
{
};

START::~START()
{
};

MAP* START::getMap()
{
	return(&map);
};

void EXPORT START::adjustGoals(int player)
{
	map.adjustGoals(player);
};

void EXPORT START::setPlayerRace(int player, int race)
{
	map.setStartPlayerRace(player, race);
};

void EXPORT START::assignMap(MAP* map, DEFAULT* defaults)
{
	//TODO wenn map settings so sind, dass auch Einheiten ausserhalb der Startposition liegen, gehts hier net...
	if(defaults)
	{
		this->map.copyBasic(map); // just copy the basics, not the units! (except player 0 and resetting all locations)
		// goals are copied together with player in map
		this->map.copy(defaults);
		// CALL adjustGoals afterwards! (after race is set)
	}
	else
	{
		this->map.copy(map); //copy everything...
		for(int i=1;i<map->getMaxPlayer();i++)
			adjustGoals(i); // and adjust the goals (only here because the players' races are fixed
	}
};

