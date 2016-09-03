#ifndef _CORE_ANARACE_HPP
#define _CORE_ANARACE_HPP

#include "prerace.hpp"

struct STATISTICS
{
	int needSupply;		// supply that is used up by all units
	int haveSupply;		// total supply of supply buildings (overlord, supply depot, command center, ...)
	int minerals;			// minerals at that time
	int gas;			// gas at that time
	int fitness;	//fitness at that time
};


struct PROGRAM
{
	int built;			// was this order successfully built?
	int time;			// at which time this order was started
	int temp;			// unused
	int location;		// at which location was this unit built
	int successType;	// type of error
	int successUnit;	// unit number
//	int isGoal;			// is this unit part of the goal list? DOES NOT WORK YET :)
	int forceCount[UNIT_TYPE_COUNT];
	int availibleCount[UNIT_TYPE_COUNT];
	int facility; 	// where it was produced
	int BT;		//real buildtime, incl. moving scv etc.
//	int isConstant; //this build order may not be moved. In addition no order may be inserted before the order, if the previous order is also constant.
// TODO: Zeitfenster machen!
}; 

EXPORT class ANARACE: public PRERACE
{
	private:
		int unitsTotal; // total number of all unit types at the end
		int unitsTotalMax; // maximum number of one unit type at the end
		void countUnitsTotal(); // to set maxUnitForce and totalUnitForce
		
		const bool buildGene(const int unit);
		int unchangedGenerations;
		int currentRun;
		bool optimizing;
		bool active;
		int generation;
		int maxpFitness;
		int maxsFitness;
		int maxtFitness;
		PROGRAM program[MAX_LENGTH];
		STATISTICS statistics[MAX_TIME];
		static int successType; //type of error
		static int successUnit; //unit number
//		static MAP_LOCATION backupLoc[MAX_PLAYER][MAX_LOCATIONS];
		const int needTime(const int unit) const;
		const int maximum(const int unit) const;
		int timePercentage;
		int goalPercentage;
		void setCurrentpFitness(int num);
		int currentpFitness;
		int phaenoCode[MAX_LENGTH];		// the final build order: an array of unit numbers (as defined in main.h)
	public:
		const int getUnitsTotalMax() const;
		const int getUnitsTotal() const;
		
		const int getPhaenoCode(const int IP) const;
		void setPhaenoCode(const int IP, const int num);		

		const int getCurrentpFitness() const;		

		const bool isOptimizing() const;
		void setOptimizing(const bool optimizing=true);

		const bool isActive() const;
		void setActive(const bool active=true);

		int fitnessCode[MAX_GOALS];

		int fitnessAverage;
       	int fitnessVariance;
		int positiveCrossover;
		int wayneCrossover;
		int negativeCrossover;
// external data output

		const int getMarker(const int IP) const;


//		const bool getProgramIsConstant(const int IP) const;

		const int getTimePercentage() const;
		const int getGoalPercentage() const;
		const int getProgramCode(const int IP) const;
		const int getProgramSuccessType(const int IP) const;	// determines the type of the last error before the item was built at that IP
		const int getProgramSuccessUnit(const int IP) const;	// what unit was missing? (connected to successtype)
		
		const int getStatisticsNeedSupply(const int time) const;	// supply that is used up by all units
		const int getStatisticsHaveSupply(const int time) const;	// total supply of supply buildings (overlord, supply depot, command center, ...)
		const int getStatisticsFitness(const int time) const;
		const int getStatisticsHaveMinerals(const int time) const; // minerals at that time
		const int getStatisticsHaveGas(const int time) const;		// gas at that time

		const bool getProgramIsBuilt(const int IP) const;		// was this order successfully built?

		const int getProgramTime(const int IP) const;			// at which time this order was started
		const int getProgramLocation(const int IP) const;		// at which location was this unit built
		const int getProgramTemp(const int IP) const;			// unused
//		const bool getProgramIsGoal(const int IP) const;		// is this unit part of the goal list? NOT YET WORKING!
		const int getProgramTotalCount(const int IP, const int unit) const;	// how many units of the type at phaenoCode[s] do exist at that time?
		const int getProgramAvailibleCount(const int IP, const int unit) const;     // how many units of the type at phaenoCode[s] do exist at that time?

		const int getProgramFacility(const int IP) const;
		const int getProgramBT(const int IP) const;

		const int getUnchangedGenerations() const;	// gets number of generations where no change in fitness took place
		const int getRun() const;					// gets number of runs (one run is complete when no <unchangedGenerations> > <maxGenerations>)
		const int getGeneration() const;			// gets number of total generations of this run
		const int getMaxpFitness() const;			// gets primary fitness (goal and time)
		const int getMaxsFitness() const;			// gets secondary fitness (ressources)
		const int getMaxtFitness() const;			// gets tertiary fitness (build order appearance, some heuristics)

//		void insertOrder(int unit, int position);
//		void removeOrder(int IP);


// internal control structures, do not touch ;-)
		void setUnchangedGenerations(const int unchangedGenerations); 
		void setRun(const int run);
		void setGeneration(const int generations);
		void setMaxpFitness(const int maxpFitness);
		void setMaxsFitness(const int maxsFitness);
		void setMaxtFitness(const int maxtFitness);

		void setStatisticsNeedSupply(const int time, const int supply);
		void setStatisticsHaveSupply(const int time, const int supply);
		void setStatisticsFitness(const int time, const int fitness);
		void setStatisticsHaveMinerals(const int time, const int minerals);
		void setStatisticsHaveGas(const int time, const int gas);
	

//		void setProgramIsConstant(const int IP, const bool isConstant);
		void setProgramFacility(const int IP, const int num);
		void setProgramBT(const int IP, const int num);
		void setProgramSuccessType(const int IP, const int num);
		void setProgramSuccessUnit(const int IP, const int num);
		void setProgramIsBuilt(const int IP, const bool isBuilt);
		void setProgramLocation(const int IP, const int location);
		void setProgramTime(const int IP, const int time);
//		void setProgramIsGoal(const int IP, const bool isGoal);	
		void setProgramTotalCount(const int IP, const int unit, const int count);	
		void setProgramAvailibleCount(const int IP, const int unit, const int count);

		const bool calculateStep();			// calculates another time step of current generation
		void resetData();				// resets all data to standard values
		void newRun();				// resets all data to standard values
//		void analyzeBuildOrder();		// sets the isGoal of program
// Constructor
		ANARACE();
		~ANARACE();
};

//int EXPORT getProgramSuccessType(int IP);

#endif //_CORE_ANARACE_HPP

