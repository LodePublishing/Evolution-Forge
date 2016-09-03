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
		const unsigned int getProgramAvailibleCount(const unsigned int ip, const unsigned int unit_type) const;	 // how many units of the type at phaenoCode[s] do exist at that time?

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

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

/*void ANARACE::setFixed(const bool* fixed_list)
{
	fixed = fixed_list;
}

const bool* ANARACE::getFixed() const
{
	return(fixed);
}*/

inline const unsigned int ANARACE::getAverageLength() const
{
	return(averageLength);
}

inline const unsigned int ANARACE::getUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(unitsTotalMax>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotalMax);
}

inline const unsigned int ANARACE::getNonGoalsUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(nonGoalsUnitsTotalMax>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getNonGoalsUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(nonGoalsUnitsTotalMax);
}

inline const unsigned int ANARACE::getUnitsTotal() const
{
#ifdef _SCC_DEBUG
	if(unitsTotal>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotal): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotal);
}

inline const bool ANARACE::isActive() const
{
	return(active);
}

inline void ANARACE::setActive(const bool set_active)
{
	active=set_active;
}

inline const bool ANARACE::isOptimizing() const
{
	return(optimizing);
}

inline void ANARACE::setOptimizing(const bool set_optimizing)
{
	optimizing=set_optimizing;
}

inline const unsigned int ANARACE::getProgramFacility(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Value ip out of range.");return(0);
	}
	if(program[ip].facility >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Variable not initialized.");return(0);
	}
#endif
//	if((getRace()==PROTOSS)&&(program[ip].facility==PROBE))
//		while(true);

	return(program[ip].facility);
}

inline const unsigned int ANARACE::getProgramBT(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramBT): Value ip out of range.");return(0);
	}
	if(program[ip].BT >= MAX_TIME) {
		toLog("DEBUG: (ANARACE::getProgramBT): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].BT);
}																	  

inline const unsigned int ANARACE::getProgramSuccessType(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Value ip out of range.");return(0);
	}
	if(program[ip].successType >= ERROR_MESSAGES) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].successType);
}

inline const unsigned int ANARACE::getProgramSuccessUnit(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Value ip out of range.");return(0);
	}
	if(program[ip].successUnit >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].successUnit);
}

/*inline void ANARACE::setProgramIsConstant(const unsigned int ip, const bool isConstant)
{
		if((ip >= MAX_LENGTH)) {
				toLog("DEBUG: (ANARACE::setProgramIsConstant): Value ip out of range.");return(0);
		}
		program[ip].isConstant = isConstant;
};*/

inline void ANARACE::setProgramAvailibleCount(const unsigned int ip, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value ip out of range.");return;
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value unit_type out of range.");return;
	}
	if(unit_count >= MAX_TOTAL_UNITS) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value unit_count out of range.");return;
	}
#endif
	program[ip].availibleCount[unit_type] = unit_count;
}

inline void ANARACE::setProgramTotalCount(const unsigned int ip, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value ip out of range.");return;
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value unit_type out of range.");return;
	}
	if(unit_count >= MAX_TOTAL_UNITS) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value unit_count out of range.");return;
	}
#endif
	program[ip].forceCount[unit_type] = unit_count;
}

// first time is 0!
inline void ANARACE::setProgramTime(const unsigned int ip, const unsigned int program_time)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value ip out of range.");return;
	}
	if(program_time > configuration.getMaxTime()) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value program_time out of range.");return;
	}
#endif
	program[ip].time = program_time;
}

inline void ANARACE::setProgramBT(const unsigned int ip, const unsigned int program_bt)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value ip out of range.");return;
	}
	if(program_bt >= MAX_TIME) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value program_bt out of range.");return;
	}
#endif
	program[ip].BT = program_bt;
}

inline void ANARACE::setProgramIsBuilt(const unsigned int ip, const bool program_is_built)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramIsBuilt): Value ip out of range.");return;
	}
#endif
	program[ip].built = program_is_built;
}

inline void ANARACE::setProgramLocation(const unsigned int ip, const unsigned int program_location)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value ip out of range.");return;
	}
	if(program_location >= MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value program_location out of range.");return;
	}
#endif
	program[ip].location = program_location;
}
// ------ STATISTICS SET FUNCTIONS ------
inline void ANARACE::setIPStatisticsNeedSupply(const unsigned int ip, const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsNeedSupply): Value ip out of range.");return;
    }
	if(need_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setIPStatisticsNeedSupply): Value need_supply out of range.");return;
	}
#endif
	ipStatistics[ip].needSupply = need_supply;
}

inline void ANARACE::setIPStatisticsHaveSupply(const unsigned int ip, const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsHaveSupply): Value ip out of range.");return;
    }
	if(have_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setIPStatisticsHaveSupply): Value have_supply out of range.");return;
	}
#endif
	ipStatistics[ip].haveSupply = have_supply;
}

inline void ANARACE::setIPStatisticsHaveMinerals(const unsigned int ip, const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsHaveMinerals): Value ip out of range.");return;
    }
	if(have_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::setIPStatisticsHaveMinerals): Value have_minerals out of range.");return;
	}
#endif
	ipStatistics[ip].minerals = have_minerals;
}
																				
inline void ANARACE::setIPStatisticsHaveGas(const unsigned int ip, const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsHaveGas): Value ip out of range.");return;
    }
	if(have_gas >= MAX_GAS) {
		toLog("DEBUG: (ANARACE::setIPStatisticsHaveGas): Value have_gas out of range.");return;
	}
#endif
	ipStatistics[ip].gas = have_gas;
}

inline void ANARACE::setIPStatisticsFitness(const unsigned int ip, const unsigned int fitness)
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setIPStatisticsFitness): Value ip out of range.");return;
    }
	if(fitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setIPStatisticsFitness): Value fitness out of range.");return;
	}
#endif
	ipStatistics[ip].fitness = fitness;
}


inline void ANARACE::setTimeStatisticsNeedSupply(const unsigned int time, const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsNeedSupply): Value time out of range.");return;
    }
	if(need_supply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsNeedSupply): Value need_supply out of range.");return;
	}
#endif
	timeStatistics[time].needSupply = need_supply;
}

inline void ANARACE::setTimeStatisticsHaveSupply(const unsigned int time, const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsHaveSupply): Value time out of range.");return;
    }
	if(have_supply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsHaveSupply): Value have_supply out of range.");return;
	}
#endif
	timeStatistics[time].haveSupply = have_supply;
}

inline void ANARACE::setTimeStatisticsHaveMinerals(const unsigned int time, const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
    if((time >= MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsHaveMinerals): Value time out of range.");return;
    }
	if(have_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsHaveMinerals): Value have_minerals out of range.");return;
	}
#endif
	timeStatistics[time].minerals = have_minerals;
}
																				
inline void ANARACE::setTimeStatisticsHaveGas(const unsigned int time, const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
    if((time >= MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsHaveGas): Value time out of range.");return;
    }
	if(have_gas >= MAX_GAS) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsHaveGas): Value have_gas out of range.");return;
	}
#endif
	timeStatistics[time].gas = have_gas;
}

inline void ANARACE::setTimeStatisticsFitness(const unsigned int time, const unsigned int fitness)
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::setTimeStatisticsFitness): Value time out of range.");return;
    }
	if(fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setTimeStatisticsFitness): Value fitness out of range.");return;
	}
#endif
	timeStatistics[time].fitness=fitness;
}
// ------ END OF STATISTICS SET FUNCTIONS ------


inline const unsigned int ANARACE::getProgramTotalCount(const unsigned int ip, const unsigned int unit_type) const	   // how many units of the type at phaenoCode[s] do exist at that time?
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value ip out of range.");return(0);
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value unit_type out of range.");return(0);
	}
#endif
	return(program[ip].forceCount[unit_type]);
}

// how many units of the type at phaenoCode[s] do exist at that time?
inline const unsigned int ANARACE::getProgramAvailibleCount(const unsigned int ip, const unsigned int unit_type) const	   
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value ip out of range.");return(0);
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value unit_type out of range.");return(0);
	}
#endif
	return(program[ip].availibleCount[unit_type]);
}

inline void ANARACE::setProgramFacility(const unsigned int ip, const unsigned int facility)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value ip out of range.");return;
	}
	if(facility>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value facility out of range.");return;
	}
#endif
	program[ip].facility = facility;
}

inline void ANARACE::setProgramSuccessType(const unsigned int ip, const unsigned int type)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value ip out of range.");return;
	}
	if(type >= ERROR_MESSAGES) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value type out of range.");return;
	}
#endif
	program[ip].successType = type;
}

inline void ANARACE::setProgramSuccessUnit(const unsigned int ip, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value ip out of range.");return;
	}
	if(unit_type >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value unit_type out of range.");return;
	}
#endif
	program[ip].successUnit = unit_type;
}

// ------ STATISTICS GET FUNCTIONS ------
inline const unsigned int ANARACE::getIPStatisticsNeedSupply(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsNeedSupply): Value ip out of range.");return(0);
    }
	if(ipStatistics[ip].needSupply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getIPStatisticsNeedSupply): Variable ipStatistics.needSupply not initialized.");	return(0);
	}
#endif
	return(ipStatistics[ip].needSupply);
}

inline const unsigned int ANARACE::getIPStatisticsHaveSupply(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsHaveSupply): Value ip out of range.");return(0);
    }
	if(ipStatistics[ip].haveSupply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getIPStatisticsHaveSupply): Variable ipStatistics.haveSupply not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].haveSupply);
}

inline const unsigned int ANARACE::getIPStatisticsHaveMinerals(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
	        toLog("DEBUG: (ANARACE::getIPStatisticsHaveMinerals): Value ip out of range.");return(0);
	}
	if(ipStatistics[ip].minerals > MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::getIPStatisticsHaveMinerals): Variable ipStatistics.minerals not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].minerals);
}

inline const unsigned int ANARACE::getIPStatisticsHaveGas(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsHaveGas): Value ip out of range.");return(0);
    }
	if(ipStatistics[ip].gas > MAX_GAS) {
		toLog("DEBUG: (ANARACE::getIPStatisticsHaveGas): Variable ipStatistics.gas not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].gas);
}

inline const unsigned int ANARACE::getIPStatisticsFitness(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
    if((ip >= 2*MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getIPStatisticsFitness): Value ip out of range.");return(0);
    }
	// TODO max pfitness 
	if(ipStatistics[ip].fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getIPStatisticsFitness): Variable ipStatistics.fitness not initialized.");return(0);
	}
#endif
	return(ipStatistics[ip].fitness);
}


inline const unsigned int ANARACE::getTimeStatisticsNeedSupply(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::getTimeStatisticsNeedSupply): Value time out of range.");return(0);
    }
	if(timeStatistics[time].needSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsNeedSupply): Variable timeStatistics.needSupply not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].needSupply);
}

inline const unsigned int ANARACE::getTimeStatisticsHaveSupply(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
        toLog("DEBUG: (ANARACE::getTimeStatisticsHaveSupply): Value time out of range.");return(0);
    }
	if(timeStatistics[time].haveSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsHaveSupply): Variable timeStatistics.haveSupply not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].haveSupply);
}

inline const unsigned int ANARACE::getTimeStatisticsHaveMinerals(const unsigned int time) const
{
#ifdef _SCC_DEBUG
    if((time>=MAX_TIME)) {
	        toLog("DEBUG: (ANARACE::getTimeStatisticsHaveMinerals): Value time out of range.");return(0);
	}
	if(timeStatistics[time].minerals>MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsHaveMinerals): Variable timeStatistics.minerals not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].minerals);
}
inline const unsigned int ANARACE::getTimeStatisticsHaveGas(const unsigned int time) const
{
#ifdef _SCC_DEBUG
	if((time>=MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsHaveGas): Value time out of range.");return(0);
	}
	if(timeStatistics[time].gas>MAX_GAS) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsHaveGas): Variable timeStatistics.gas not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].gas);
}

inline const unsigned int ANARACE::getTimeStatisticsFitness(const unsigned int time) const
{
#ifdef _SCC_DEBUG
	if((time>=MAX_TIME)) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsFitness): Value time out of range.");return(0);
	}
	// TODO max pfitness 
	if(timeStatistics[time].fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getTimeStatisticsFitness): Variable timeStatistics.fitness not initialized.");return(0);
	}
#endif
	return(timeStatistics[time].fitness);
}
// ------ END OF STATISTICS GET FUNCTIONS ------

inline const bool ANARACE::getProgramIsBuilt(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsBuilt): Value ip out of range.");return(0);
	}
#endif
	return(program[ip].built);
}


/*const bool ANARACE::getProgramIsConstant(const unsigned int ip)
{
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsConstant): Value ip [%i] out of range.",IP);return(false);
	}
	return(program[ip].isConstant);
};*/

// first program time is
inline const unsigned int ANARACE::getProgramTime(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTime): Value ip out of range.");return(0);
	}
	if(program[ip].time > configuration.getMaxTime()) {
		toLog("DEBUG: (ANARACE::getProgramTime): Variable not initialized.");return(0);
	}
#endif
	return(program[ip].time);
}

inline const unsigned int ANARACE::getRealProgramTime(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Value ip out of range.");return(0);
	}
#endif
	return(configuration.getMaxTime() - getProgramTime(ip));
}


inline const unsigned int ANARACE::getProgramLocation(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Value ip out of range.");return(0);
	}
	if(program[ip].location > MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Variable program.location not initialized.");return(0);
	}
#endif
	return(program[ip].location);
}

inline const unsigned int ANARACE::getTimePercentage() const
{
	return(timePercentage);
}

inline const unsigned int ANARACE::getFastestGoalTime() const
{
	return(getpGoal()->calculateFastestBO((*pStartCondition)->getUnit(GLOBAL)));
}
inline const GOAL_TREE ANARACE::getGoalTree(const unsigned int currentGoalUnit) const
{
	return(getpGoal()->getGoalTree((*pStartCondition)->getUnit(GLOBAL), currentGoalUnit));
}

inline void ANARACE::setPhaenoCode(const unsigned int ip, const unsigned int phaeno_code)
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value ip out of range.");return;
	}
	if(phaeno_code >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value phaeno_code out of range.");return;
	}
#endif
	phaenoCode[ip] = phaeno_code;
}

inline const unsigned int ANARACE::getPhaenoCode(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if((ip >= MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getPhaenoCode): Value ip out of range.");return(0);
	}
#endif
	return(phaenoCode[ip]);
}

inline const unsigned int ANARACE::getCurrentpFitness() const
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
	if(currentpFitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getCurrentpFitness): Variable currentpFitness not initialized.");return(0);
	}
#endif
	return(currentpFitness);
}

inline void ANARACE::setCurrentpFitness(const unsigned int current_pfitness)
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
	if(current_pfitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setCurrentpFitness): Variable current_pfitness not initialized.");return;
	}
#endif
	currentpFitness = current_pfitness;
}

inline const unsigned int ANARACE::getUnchangedGenerations() const
{
#ifdef _SCC_DEBUG	
	if(unchangedGenerations > configuration.getMaxGenerations()) {
		toLog("DEBUG: (ANARACE::getUnchangedGenerations): Variable unchangedGenerations not initialized.");return(0);
	}
#endif
	return(unchangedGenerations);
}

inline const unsigned int ANARACE::getRun() const
{
#ifdef _SCC_DEBUG	
	if(currentRun > configuration.getMaxRuns()) {
		toLog("DEBUG: (ANARACE::getRun): Variable currentRun not initialized.");return(0);
	}
#endif
	return(currentRun);
}

inline const unsigned int ANARACE::getTotalGeneration() const
{
	return(totalGeneration);
}

inline const unsigned int ANARACE::getMaxpFitness() const
{
#ifdef _SCC_DEBUG	
// TODO max pfitness?
	if(maxpFitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getMaxpFitness): Variable maxpFitness not initialized.");return(0);
	}
#endif
	return(maxpFitness);
}

inline const unsigned int ANARACE::getMaxsFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxsFitness > MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANARACE::getMaxsFitness): Variable maxsFitness not initialize.");return(0);
	}
#endif
	return(maxsFitness);
}

inline const unsigned int ANARACE::getMaxtFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxtFitness > MAX_TFITNESS) {
		toLog("DEBUG: (ANARACE::getMaxtFitness): Variable maxtFitness not initialized.");return(0);
	}
#endif
	return(maxtFitness);
}

inline void ANARACE::setUnchangedGenerations(const unsigned int unchanged_generations)
{
#ifdef _SCC_DEBUG	
	if(unchanged_generations > configuration.getMaxGenerations()) {
		toLog("DEBUG: (ANARACE::setUnchangedGenerations): Value out of range.");return;
	}
#endif
	unchangedGenerations = unchanged_generations;
}

inline void ANARACE::setRun(const unsigned int current_run)
{
#ifdef _SCC_DEBUG
	if(current_run > configuration.getMaxRuns()) {
		toLog("DEBUG: (ANARACE::setRun): Value out of range.");return;
	}
#endif
	currentRun = current_run;
}

inline void ANARACE::setTotalGeneration(const unsigned int total_generation)
{
	totalGeneration = total_generation;
}

inline void ANARACE::setMaxpFitness(const unsigned int max_pfitness) 
{
#ifdef _SCC_DEBUG
	// TODO maxpitness
	if(max_pfitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setMaxpFitness): Value out of range.");return;
	}
#endif
	maxpFitness = max_pfitness;
}

inline void ANARACE::setMaxsFitness(const unsigned int max_sfitness)
{
#ifdef _SCC_DEBUG
	if(max_sfitness > MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANARACE::setMaxsFitness): Value out of range.");return;
	}
#endif
	maxsFitness = max_sfitness;
}

inline void ANARACE::setMaxtFitness(const unsigned int max_tfitness)
{
#ifdef _SCC_DEBUG
	if(max_tfitness > MAX_TFITNESS) {
		toLog("DEBUG: (ANARACE::setMaxtFitness): Value out of range.");return;
	}
#endif
	maxtFitness = max_tfitness;
}
//int getProgramSuccessType(int IP);

#endif //_CORE_ANARACE_HPP

