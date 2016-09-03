#ifndef _CORE_ANARACE_HPP
#define _CORE_ANARACE_HPP

#include "prerace.hpp"

struct STATISTICS
{
	unsigned int needSupply;		// supply that is used up by all units
	unsigned int haveSupply;		// total supply of supply buildings (overlord, supply depot, command center, ...)
	unsigned int minerals;			// minerals at that time
	unsigned int gas;			// gas at that time
	unsigned int fitness;	//fitness at that time
};

struct PROGRAM
{
	bool built;			// was this order successfully built?
	unsigned int time;			// at which time this order was started
	unsigned int location;		// at which location was this unit built
	unsigned int successType;	// type of error
	unsigned int successUnit;	// unit number
//	int isGoal;			// is this unit part of the goal list? DOES NOT WORK YET :)
	unsigned int forceCount[UNIT_TYPE_COUNT];
	unsigned int availibleCount[UNIT_TYPE_COUNT];
	unsigned int facility; 	// where it was produced
	unsigned int BT;		//real buildtime, incl. moving scv etc.
//	int isConstant; //this build order may not be moved. In addition no order may be inserted before the order, if the previous order is also constant.
// TODO: Zeitfenster machen!
};

class ANARACE: public PRERACE
{
	private:
		unsigned int unitsTotal; // total number of all unit types at the end
		unsigned int unitsTotalMax; // maximum number of one unit type at the end
		unsigned int unchangedGenerations;
		unsigned int currentRun;
		bool optimizing;
		bool active;
		unsigned int generation;
		unsigned int maxpFitness;
		unsigned int maxsFitness;
		unsigned int maxtFitness;
		unsigned int timePercentage;
		unsigned int goalPercentage;
		unsigned int currentpFitness;
		unsigned int averageLength;
		unsigned int phaenoCode[MAX_LENGTH];		// the final build order: an array of unit numbers (as defined in main.h)

		PROGRAM program[MAX_LENGTH];
		STATISTICS statistics[MAX_TIME];
	
		static unsigned int successType; //type of error
		static unsigned int successUnit; //unit number

//		const unsigned int needTime(const unsigned int unit) const;
//		const unsigned int maximum(const unsigned int unit) const;
		void setCurrentpFitness(unsigned int num);
		
		const bool buildGene(const unsigned int build_unit);
	public:
		unsigned int fitnessAverage;
       	unsigned int fitnessVariance;
		unsigned int positiveCrossover;
		unsigned int wayneCrossover;
		unsigned int negativeCrossover;

		const unsigned int getUnitsTotalMax() const;
		const unsigned int getUnitsTotal() const;
		
		const unsigned int getPhaenoCode(const unsigned int IP) const;
		void setPhaenoCode(const unsigned int IP, const unsigned int num);		

		void countUnitsTotal(); // to set maxUnitForce and totalUnitForce
		const unsigned int getCurrentpFitness() const;		

		const bool isOptimizing() const;
		void setOptimizing(const bool optimizing=true);

		const bool isActive() const;
		void setActive(const bool active=true);

		const unsigned int getAverageLength() const;
			
//		unsigned int fitnessCode[MAX_GOALS]; ???

// external data output


		const GOAL_TREE getGoalTree(const unsigned currentGoalUnit=0) const;
		
//		const bool getProgramIsConstant(const unsigned int IP) const;

		const unsigned int getTimePercentage() const;
		const unsigned int getGoalPercentage() const;
		const unsigned int getProgramSuccessType(const unsigned int IP) const;	// determines the type of the last error before the item was built at that IP
		const unsigned int getProgramSuccessUnit(const unsigned int IP) const;	// what unit was missing? (connected to successtype)
		
		const unsigned int getStatisticsNeedSupply(const unsigned int IP) const;	// supply that is used up by all units
		const unsigned int getStatisticsHaveSupply(const unsigned int IP) const;	// total supply of supply buildings (overlord, supply depot, command center, ...)
		const unsigned int getStatisticsFitness(const unsigned int IP) const;
		const unsigned int getStatisticsHaveMinerals(const unsigned int IP) const; // minerals at that time
		const unsigned int getStatisticsHaveGas(const unsigned int IP) const;		// gas at that time

		const bool getProgramIsBuilt(const unsigned int IP) const;		// was this order successfully built?

		const unsigned int getProgramTime(const unsigned int IP) const;			// at which time this order was started
		const unsigned int getRealProgramTime(const unsigned int IP) const;			// at which time this order was started
		const unsigned int getProgramLocation(const unsigned int IP) const;		// at which location was this unit built
//		const bool getProgramIsGoal(const unsigned int IP) const;		// is this unit part of the goal list? NOT YET WORKING!
		const unsigned int getProgramTotalCount(const unsigned int IP, const unsigned int unit) const;	// how many units of the type at phaenoCode[s] do exist at that time?
		const unsigned int getProgramAvailibleCount(const unsigned int IP, const unsigned int unit) const;     // how many units of the type at phaenoCode[s] do exist at that time?

		const unsigned int getProgramFacility(const unsigned int IP) const;
		const unsigned int getProgramBT(const unsigned int IP) const;

		const unsigned int getUnchangedGenerations() const;	// gets number of generations where no change in fitness took place
		const unsigned int getRun() const;					// gets number of runs (one run is complete when no <unchangedGenerations> > <maxGenerations>)
		const unsigned int getGeneration() const;			// gets number of total generations of this run
		const unsigned int getMaxpFitness() const;			// gets primary fitness (goal and time)
		const unsigned int getMaxsFitness() const;			// gets secondary fitness (ressources)
		const unsigned int getMaxtFitness() const;			// gets tertiary fitness (build order appearance, some heuristics)

//		void insertOrder(int unit, int position);
//		void removeOrder(int IP);


// internal control structures, do not touch ;-)
		void setUnchangedGenerations(const unsigned int unchangedGenerations); 
		void setRun(const unsigned int run);
		void setGeneration(const unsigned int generations);
		void setMaxpFitness(const unsigned int maxpFitness);
		void setMaxsFitness(const unsigned int maxsFitness);
		void setMaxtFitness(const unsigned int maxtFitness);

		void setStatisticsNeedSupply(const unsigned int IP, const unsigned int supply);
		void setStatisticsHaveSupply(const unsigned int IP, const unsigned int supply);
		void setStatisticsFitness(const unsigned int IP, const unsigned int fitness);
		void setStatisticsHaveMinerals(const unsigned int IP, const unsigned int minerals);
		void setStatisticsHaveGas(const unsigned int IP, const unsigned int gas);
	

//		void setProgramIsConstant(const unsigned int IP, const bool isConstant);
		void setProgramFacility(const unsigned int IP, const unsigned int num);
		void setProgramBT(const unsigned int IP, const unsigned int num);
		void setProgramSuccessType(const unsigned int IP, const unsigned int num);
		void setProgramSuccessUnit(const unsigned int IP, const unsigned int num);
		void setProgramIsBuilt(const unsigned int IP, const bool isBuilt);
		void setProgramLocation(const unsigned int IP, const unsigned int location);
		void setProgramTime(const unsigned int IP, const unsigned int time);
//		void setProgramIsGoal(const unsigned int IP, const bool isGoal);	
		void setProgramTotalCount(const unsigned int IP, const unsigned int unit, const unsigned int count);	
		void setProgramAvailibleCount(const unsigned int IP, const unsigned int unit, const unsigned int count);

		const bool calculateStep();			// calculates another time step of current generation
		void resetData();				// resets all data to standard values
		void prepareForNewGeneration();				// resets all data to standard values
//		void analyzeBuildOrder();		// sets the isGoal of program
// Constructor
		ANARACE& operator=(const ANARACE& anarace);
		ANARACE(const ANARACE& anarace);
		ANARACE();
		~ANARACE();
};

//int getProgramSuccessType(int IP);

#endif //_CORE_ANARACE_HPP

