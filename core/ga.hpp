#ifndef _CORE_GA_HPP
#define _CORE_GA_HPP


// TODO irgendwohinverfrachten :/
#include "defs.hpp"

class GA
{
// This is still a public class though access _SHOULD_ be made through the corresponding functions of settings
// TODO: make it private
private:
	unsigned int crossOver;			// percentage of population that should exchange their genetic informations with each other
	unsigned int breedFactor;		// percentage of population that should copy 1:1 their genetic informations 
	unsigned int mutationFactor;
public:
	bool preprocessBuildOrder; // should the program create a basic valid solution? TODO: IMPLEMENT IT!
	bool allowGoalAdaption;

	unsigned int maxTime;			// border of global searchspace in seconds (the lower the faster but needs longer to get a valid solution at the first place)
	unsigned int maxTimeOut;			// border of local searchspace in seconds (how long the program should be allowed to wait until prerequisites of an item are fulfilled)
	unsigned int maxLength;			// max length of individual (as maxTime)
	unsigned int maxRuns;			// how many different solutions should be searched
	unsigned int maxGenerations;		// how many unchanged generations shall occur until the run is terminated
	unsigned int currentMap;			// number of current map TODO
	unsigned int noise; //in %


	const unsigned int getMutationFactor() const;
	const unsigned int getCrossOver() const;
	const unsigned int getBreedFactor() const;

	void setMutationFactor(const unsigned int num);
	void setCrossOver(const unsigned int num);
	void setBreedFactor(const unsigned int num);
//constructor
	GA();
	~GA();
};

#endif // _CORE_GA_HPP



