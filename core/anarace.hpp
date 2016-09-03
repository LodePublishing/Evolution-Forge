#ifndef _CORE_ANARACE_HPP
#define _CORE_ANARACE_HPP

#include "prerace.hpp"

class STATISTICS
{
	public:
		STATISTICS();
		~STATISTICS();
		void resetData();
		unsigned int needSupply;		// supply that is used up by all units
		unsigned int haveSupply;		// total supply of supply buildings (overlord, supply depot, command center, ...)
		unsigned int minerals;			// minerals at that time
		unsigned int gas;			// gas at that time
		unsigned int fitness;	//fitness at that time
};

struct PROGRAM
{
	public:
		PROGRAM();
		~PROGRAM();
		void resetData();		
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
//		int isConstant; //this build order may not be moved. In addition no order may be inserted before the order, if the previous order is also constant.
// TODO: Zeitfenster machen!
};

class ANARACE: public PRERACE
{
	private:
		unsigned int unitsTotal; // total number of all unit types at the end
		unsigned int unitsTotalMax; // maximum number of one unit type at the end
		unsigned int nonGoalsUnitsTotalMax; // maximum number of one NON GOAL unit type at the end
		unsigned int unchangedGenerations;
		unsigned int currentRun;
		bool optimizing;
		bool active;
		unsigned int totalGeneration;
		unsigned int maxpFitness;
		unsigned int maxsFitness;
		unsigned int maxtFitness;
		unsigned int timePercentage;
		unsigned int goalPercentage;
		unsigned int currentpFitness;
		unsigned int averageLength;
		unsigned int phaenoCode[MAX_LENGTH];		// the final build order: an array of unit numbers (as defined in main.h)

		PROGRAM program[MAX_LENGTH];
		STATISTICS ipStatistics[2*MAX_LENGTH]; // before and after command!
		STATISTICS timeStatistics[MAX_TIME];
	
//		const bool* fixed;
		
		static unsigned int successType; //type of error
		static unsigned int successUnit; //unit number

		void setCurrentpFitness(unsigned int current_pfitness);
		
		const bool buildGene(const unsigned int build_unit);

		void setIPStatisticsNeedSupply(const unsigned int ip, const unsigned int need_supply);
		void setIPStatisticsHaveSupply(const unsigned int ip, const unsigned int have_supply);
		void setIPStatisticsFitness(const unsigned int ip, const unsigned int fitness);
		void setIPStatisticsHaveMinerals(const unsigned int ip, const unsigned int have_minerals);
		void setIPStatisticsHaveGas(const unsigned int ip, const unsigned int have_gas);

		void setTimeStatisticsNeedSupply(const unsigned int time, const unsigned int need_supply);
		void setTimeStatisticsHaveSupply(const unsigned int time, const unsigned int have_supply);
		void setTimeStatisticsFitness(const unsigned int time, const unsigned int fitness);
		void setTimeStatisticsHaveMinerals(const unsigned int time, const unsigned int have_minerals);
		void setTimeStatisticsHaveGas(const unsigned int time, const unsigned int have_gas);
		
	public:
		ANARACE();
		~ANARACE();
		ANARACE& operator=(const ANARACE& anarace);
		ANARACE(const ANARACE& anarace);
	
		unsigned int fitnessAverage;
		unsigned int fitnessVariance;
		unsigned int positiveCrossover;
		unsigned int wayneCrossover;
		unsigned int negativeCrossover;

		const unsigned int getUnitsTotalMax() const;
		const unsigned int getNonGoalsUnitsTotalMax() const;
		const unsigned int getUnitsTotal() const;
		
		const unsigned int getPhaenoCode(const unsigned int ip) const;
		void setPhaenoCode(const unsigned int ip, const unsigned int phaeno_code);		

		void countUnitsTotal(); // to set maxUnitForce and totalUnitForce
		const unsigned int getCurrentpFitness() const;		

		const bool isOptimizing() const;
		void setOptimizing(const bool optimizing=true);

		const bool isActive() const;
		void setActive(const bool active=true);

		const unsigned int getAverageLength() const;
//		void setFixed(const bool* fixed_list);
//		const bool* getFixed() const;
			
//		unsigned int fitnessCode[MAX_GOALS]; ???

// external data output

		const unsigned int getFastestGoalTime() const;

		const GOAL_TREE getGoalTree(const unsigned currentGoalUnit=0) const;
		
//		const bool getProgramIsConstant(const unsigned int ip) const;

		const unsigned int getTimePercentage() const;
		const unsigned int getGoalPercentage() const;
		const unsigned int getProgramSuccessType(const unsigned int ip) const;	// determines the type of the last error before the item was built at that IP
		const unsigned int getProgramSuccessUnit(const unsigned int ip) const;	// what unit was missing? (connected to successtype)
		
		const unsigned int getIPStatisticsNeedSupply(const unsigned int ip) const;	// supply that is used up by all units
		const unsigned int getIPStatisticsHaveSupply(const unsigned int ip) const;	// total supply of supply buildings (overlord, supply depot, command center, ...)
		const unsigned int getIPStatisticsFitness(const unsigned int ip) const;
		const unsigned int getIPStatisticsHaveMinerals(const unsigned int ip) const; // minerals at that time
		const unsigned int getIPStatisticsHaveGas(const unsigned int ip) const;		// gas at that time

		const unsigned int getTimeStatisticsNeedSupply(const unsigned int time) const;	// supply that is used up by all units
		const unsigned int getTimeStatisticsHaveSupply(const unsigned int time) const;	// total supply of supply buildings (overlord, supply depot, command center, ...)
		const unsigned int getTimeStatisticsFitness(const unsigned int time) const;
		const unsigned int getTimeStatisticsHaveMinerals(const unsigned int time) const; // minerals at that time
		const unsigned int getTimeStatisticsHaveGas(const unsigned int time) const;		// gas at that time

		const bool getProgramIsBuilt(const unsigned int ip) const;		// was this order successfully built?

		const unsigned int getProgramTime(const unsigned int ip) const;			// at which time this order was started
		const unsigned int getRealProgramTime(const unsigned int ip) const;			// at which time this order was started
		const unsigned int getProgramLocation(const unsigned int ip) const;		// at which location was this unit built
//		const bool getProgramIsGoal(const unsigned int ip) const;		// is this unit part of the goal list? NOT YET WORKING!
		const unsigned int getProgramTotalCount(const unsigned int ip, const unsigned int unit_type) const;	// how many units of the type at phaenoCode[s] do exist at that time?
		const unsigned int getProgramAvailibleCount(const unsigned int ip, const unsigned int unit_type) const;     // how many units of the type at phaenoCode[s] do exist at that time?

		const unsigned int getProgramFacility(const unsigned int ip) const;
		const unsigned int getProgramBT(const unsigned int ip) const;

		const unsigned int getUnchangedGenerations() const;	// gets number of generations where no change in fitness took place
		const unsigned int getRun() const;					// gets number of runs (one run is complete when no <unchangedGenerations> > <maxGenerations>)
		const unsigned int getTotalGeneration() const;			// gets number of total generations of this run
		const unsigned int getMaxpFitness() const;			// gets primary fitness (goal and time)
		const unsigned int getMaxsFitness() const;			// gets secondary fitness (ressources)
		const unsigned int getMaxtFitness() const;			// gets tertiary fitness (build order appearance, some heuristics)

//		void insertOrder(int unit, int position);
//		void removeOrder(int IP);


// internal control structures, do not touch ;-)
		void setUnchangedGenerations(const unsigned int unchanged_generations); 
		void setRun(const unsigned int current_run);
		void setTotalGeneration(const unsigned int total_generations);
		void setMaxpFitness(const unsigned int max_pfitness);
		void setMaxsFitness(const unsigned int max_sfitness);
		void setMaxtFitness(const unsigned int max_tfitness);

//		void setProgramIsConstant(const unsigned int ip, const bool isConstant);
		void setProgramFacility(const unsigned int ip, const unsigned int program_facility);
		void setProgramBT(const unsigned int ip, const unsigned int program_bt);
		void setProgramSuccessType(const unsigned int ip, const unsigned int program_success_type);
		void setProgramSuccessUnit(const unsigned int ip, const unsigned int program_success_unit);
		void setProgramIsBuilt(const unsigned int ip, const bool program_is_built);
		void setProgramLocation(const unsigned int ip, const unsigned int program_location);
		void setProgramTime(const unsigned int ip, const unsigned int program_time);
//		void setProgramIsGoal(const unsigned int ip, const bool isGoal);	
		void setProgramTotalCount(const unsigned int ip, const unsigned int unit_type, const unsigned int unit_count);	
		void setProgramAvailibleCount(const unsigned int ip, const unsigned int unit_type, const unsigned int unit_count);

		const bool calculateStep();			// calculates another time step of current generation
		void resetData();				// resets all data to standard values
		void restartData();				// resets all data to standard values
		static void resetStaticData();
		void prepareForNewGeneration();				// resets all data to standard values
//		void analyzeBuildOrder();		// sets the isGoal of program
		
};

//int getProgramSuccessType(int IP);

#endif //_CORE_ANARACE_HPP

