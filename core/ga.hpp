#ifndef _CORE_GA_HPP
#define _CORE_GA_HPP


// TODO irgendwohinverfrachten :/
#include "main.hpp"

class EXPORT GA
{
// This is still a public class though access _SHOULD_ be made through the corresponding functions of settings
// TODO: make it private
private:
	int crossOver;			// percentage of population that should exchange their genetic informations with each other
	int breedFactor;		// percentage of population that should copy 1:1 their genetic informations 
	int mutationFactor;
public:
	int goalCount;			// number of total goals currently loaded
	int startconditionCount;
	int mapCount;			// number of total maps currently loaded
	int maxTime;			// border of global searchspace in seconds (the lower the faster but needs longer to get a valid solution at the first place)
	int maxTimeOut;			// border of local searchspace in seconds (how long the program should be allowed to wait until prerequisites of an item are fulfilled)
	int maxLength;			// max length of individual (as maxTime)
	int maxRuns;			// how many different solutions should be searched
	int maxGenerations;		// how many unchanged generations shall occur until the run is terminated
	int preprocessBuildOrder; // should the program create a basic valid solution? TODO: IMPLEMENT IT!
	int currentMap;			// number of current map
	int mode;				// mode, only mode 0 works as battle calculator is not yet implemented
	int noise; //in %
	int allowGoalAdaption;

	const int getMutationFactor() const;
	const int getCrossOver() const;
	const int getBreedFactor() const;

	void setMutationFactor(const int num);
	void setCrossOver(const int num);
	void setBreedFactor(const int num);
//constructor
	GA();
	~GA();
};

#endif // __GA_H



