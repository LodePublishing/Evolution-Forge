#include "../scc2dll/settings.h"
#include "io.h"

// just a small startup program
// generally all functions return 0 when there was an error and 1 (or a pointer or whatever) if everything went fine
// a message in the error.log is dispatched and you can get the error message (and later the error code) to determine the error
// all in all the error system is only for debugging, I am still working on the function that checks all of the variables

int main()
{
	SETTINGS settings;
	ANARACE* anarace;
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
	settings.loadHarvestFile("mining.txt");
	settings.loadSettingsFile("settings.txt");
// Map in "map.txt" is now map[0]
	settings.loadMapFile("map.txt");
// choose the first map we loaded (map[0])
	settings.setCurrentMap(0);
// Goal in "goal.txt" is now goal[0]
	settings.loadGoalFile("goal.txt");
// assign goal 0 to player 1
	settings.setGoal(0,1);
// assign goal 0 also to player 2
	settings.setGoal(0,2);
// initialize the soup, set the parameters, load the players etc.
	settings.initSoup();

// --- the current text user interface ---

// assign the ga parameters
	IO io(settings.getGa());
// main loop, newGeneration returns a pointer to the data (and 0 when the run is complete or an error occured)
	while (anarace=settings.newGeneration())
// print it out
		io.screen(anarace);
	return(0);
};

// What still remains are functions to create and save goals/maps/settings etc.

