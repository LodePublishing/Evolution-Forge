#include "start.h"

START::START()
{
};

START::~START()
{
};

const BASIC_MAP** START::getMap() const
{
	return(&tmpmap);
};

const HARVEST** START::getHarvest(const int player) const
{
	return(&(harvest[player]));
};

const PLAYER* START::getStartPlayer(const int player) const
{
	return(&startPlayer[player]);
};

void START::copyStartForce(void* target)
{
	memcpy(target, &(startForce[0][0]), sizeof(startForce));
};

const START_CONDITION* START::getStartcondition(const int player) const
{
	return(startcondition[player]);
};

void EXPORT START::setPlayerRace(const int player, const eRace race)
{
	startPlayer[player].setRace(race); // TODO
};

void EXPORT START::assignGoal(const GOAL_ENTRY* goal, const int player)
{
	tmpgoal[player]=goal;
};

void EXPORT START::assignStartcondition(const int player, const START_CONDITION* startcondition)
{
	this->startcondition[player]=startcondition;
};

void EXPORT START::assignMap(const BASIC_MAP* map)
{
	this->tmpmap=map;
	// initialize Map???
	// player 0 ?
};

