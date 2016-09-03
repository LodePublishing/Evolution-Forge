#include "player.hpp"
#include "configuration.hpp"
#include <sstream>

Player::Player(UI_Object* player_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max) :
	UI_Object(player_parent, theme.lookUpPlayerRect(PLAYER_WINDOW, game_number, game_max, player_number, player_max), Size(0,0), DO_NOT_ADJUST),
	anarace(NULL),
	forceWindow(new ForceWindow(this, game_number, game_max, player_number, player_max)),
	boWindow(new BoWindow(this, game_number, game_max, player_number, player_max)),
	boGraphWindow(new BoGraphWindow(this, game_number, game_max, player_number, player_max)),
	boDiagramWindow(new BoDiagramWindow(this, game_number, game_max, player_number, player_max)),
	gameNumber(game_number),
	gameMax(game_max),
	playerNumber(player_number),
	playerMax(player_max)
	
{
	std::ostringstream os;
	os.str("");
	os << UI_Object::theme.lookUpString(PLAYER_WINDOW_TITLE_STRING) << " " << (player_number+1);
	forceWindow->setTitleParameter(os.str());
	boWindow->setTitleParameter(os.str());
	boGraphWindow->setTitleParameter(os.str());
	boDiagramWindow->setTitleParameter(os.str());

	Hide();
}
	

Player::~Player()
{ 
	delete forceWindow;
	delete boWindow;
	delete boGraphWindow;
	delete boDiagramWindow;
}


//TODO nach reset bograph und bodiagram window korrekt benachrichtigen...
void Player::assignAnarace(ANABUILDORDER* player_anarace)
{
	anarace = player_anarace;
	forceWindow->assignAnarace(anarace);
	boWindow->assignAnarace(anarace);
	boGraphWindow->assignAnarace(anarace);
	boDiagramWindow->assignAnarace(anarace);
}

void Player::reloadStrings() //TODO
{
	std::ostringstream os;
	os.str("");
	os << UI_Object::theme.lookUpString(PLAYER_WINDOW_TITLE_STRING) << " " << (playerNumber+1);
	forceWindow->setTitleParameter(os.str());
	boWindow->setTitleParameter(os.str());
	boGraphWindow->setTitleParameter(os.str());
	boDiagramWindow->setTitleParameter(os.str());

	UI_Object::reloadStrings(); 
}

void Player::draw(DC* dc) const
{
	if(!isShown())
		return;
/*	if(efConfiguration.isRaceSpecificTheme())
		switch(anarace->getRace())
		{
			case TERRA:UI_Object::theme.setColorTheme(DARK_BLUE_THEME);break;
			case PROTOSS:UI_Object::theme.setColorTheme(YELLOW_THEME);break;
			case ZERG:UI_Object::theme.setColorTheme(DARK_RED_THEME);break;
			default:break;
		}*/ // TODO

	// TODO
	UI_Object::draw(dc);
}

void Player::process()
{
	if(!isShown())
		return;
	UI_Object::process();
	if(checkForNeedRedraw())
		setNeedRedrawMoved();
	
// ------ COMMUNICATION BETWEEN THE WINDOWS ------
	if(boGraphWindow->getSelectedItems().size() > 0)
		boWindow->setSelected(boGraphWindow->getSelectedItems());
	else if(boWindow->getSelectedItems().size() > 0)
		boGraphWindow->setSelected(boWindow->getSelectedItems());
	else if(boDiagramWindow->getSelectedItems().size() > 0)
	{
		boWindow->setSelected(boDiagramWindow->getSelectedItems());
		boGraphWindow->setSelected(boDiagramWindow->getSelectedItems());
	}
// ------ END COMMUNICATION BETWEEN THE WINDOWS ------
}

const bool Player::openMenu(const ePlayerOrder order)
{
	switch(order)
	{
		case OPEN_BO_MENU:boWindow->openBoMenu();break;
		case OPEN_GOAL_MENU:forceWindow->openGoalMenu();break;
		case OPEN_UNIT_MENU:forceWindow->openUnitMenu();break;
		case SAVE_GOAL:forceWindow->saveGoal();break;
		case SAVE_BUILD_ORDER:boWindow->saveBuildOrder();break;
		case EDIT_FORCE_LIST:/*forceWindow->editForceList();*/break;
		default:return(false);
	}
	return(false);
}

void Player::compactDisplayModeHasChanged()
{
	boWindow->resetData();
}

void Player::setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max)
{
	if((game_number == gameNumber) && (game_max == gameMax) && (player_number == playerNumber) && (player_max == playerMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	playerNumber = player_number;
	playerMax = player_max;
	forceWindow->setMode(game_number, game_max, player_number, player_max);
	boWindow->setMode(game_number, game_max, player_number, player_max);
	boGraphWindow->setMode(game_number, game_max, player_number, player_max);
	boDiagramWindow->setMode(game_number, game_max, player_number, player_max);
	
	reloadOriginalSize();
}

void Player::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpPlayerRect(PLAYER_WINDOW, gameNumber, gameMax, playerNumber, playerMax));
//	setMaxHeight(UI_Object::theme.lookUpPlayerMaxHeight(PLAYER_WINDOW, gameNumber, gameMax, playerNumber, playerMax));


	UI_Object::reloadOriginalSize();
}

void Player::resetData()
{
	if(anarace == NULL)
		return;
	boWindow->resetData();
	forceWindow->resetData();
	boGraphWindow->resetData();
	boDiagramWindow->resetData();
}


void Player::recheckSomeDataAfterChange()
{
	boWindow->recheckSomeDataAfterChange();
	boGraphWindow->processList();  // ~
}

void Player::CheckOrders()
{
	boDiagramWindow->processList();
	boWindow->processList();
	boGraphWindow->processList();
	forceWindow->processList();
}

//virtual machen
//resetData, updateItems, assignAnarace, checkOrders

const bool Player::wasResetted() const 
{
	if(!isShown())
		return(false);
	return(boWindow->wasResetted());
}
