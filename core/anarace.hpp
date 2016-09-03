#ifndef _CORE_ANARACE_HPP
#define _CORE_ANARACE_HPP

#include "prerace.hpp"
#include "program.hpp"
#include "statistics.hpp"

class ANARACE: public PRERACE
{
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
		
		void countUnitsTotal(); // to set maxUnitForce and totalUnitForce
		const unsigned int getCurrentpFitness() const;		

		const bool isOptimizing() const;
		void setOptimizing(const bool optimizing=true);

		const bool isActive() const;
		void setActive(const bool active=true);

		std::list<PROGRAM>& getProgramList();
		STATISTICS* getTimeStatistics();

		const unsigned int getAverageLength() const;
//		void setFixed(const bool* fixed_list);
//		const bool* getFixed() const;
			
//		unsigned int fitnessCode[MAX_GOALS]; ???

// external data output

		const unsigned int getFastestGoalTime() const;

		const GOAL_TREE getGoalTree(const unsigned currentGoalUnit=0) const;
		

		const unsigned int getTimePercentage() const;
		const unsigned int getGoalPercentage() const;
				

		const unsigned int getUnchangedGenerations() const;	// gets number of generations where no change in fitness took place
		const unsigned int getRun() const;					// gets number of runs (one run is complete when no <unchangedGenerations> > <maxGenerations>)
		const unsigned int getTotalGeneration() const;			// gets number of total generations of this run
		const unsigned int getMaxpFitness() const;			// gets primary fitness (goal and time)
		const unsigned int getMaxsFitness() const;			// gets secondary fitness (ressources)
		const unsigned int getMaxtFitness() const;
		
//		void insertOrder(int unit, int position);
//		void removeOrder(int IP);


// internal control structures, do not touch ;-)
		void setUnchangedGenerations(const unsigned int unchanged_generations); 
		void setRun(const unsigned int current_run);
		void setTotalGeneration(const unsigned int total_generations);
		void setMaxpFitness(const unsigned int max_pfitness);
		void setMaxsFitness(const unsigned int max_sfitness);
		void setMaxtFitness(const unsigned int max_tfitness);

		const bool calculateStep();			// calculates another time step of current generation
		void resetData();				// resets all data to standard values
		void restartData();				// resets all data to standard values
		static void resetStaticData();
		void prepareForNewGeneration();				// resets all data to standard values
//		void analyzeBuildOrder();		// sets the isGoal of program
	
		std::list<PROGRAM> programList; // TODO private machen
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
//		unsigned int phaenoCode[MAX_LENGTH];		// the final build order: an array of unit numbers (as defined in main.h)

		STATISTICS timeStatistics[MAX_TIME];

//		const bool* fixed;
		
		static unsigned int successType; //type of error
		static unsigned int successUnit; //unit number

		void setCurrentpFitness(unsigned int current_pfitness);
		
		const bool buildGene(const unsigned int build_unit);
		const bool buildIt(const unsigned int build_unit);
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

inline std::list<PROGRAM>& ANARACE::getProgramList() {
	return(programList);
}

inline STATISTICS* ANARACE::getTimeStatistics() {
	return(timeStatistics);
}

inline const unsigned int ANARACE::getAverageLength() const {
	return(averageLength);
}

inline const unsigned int ANARACE::getUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(unitsTotalMax > MAX_TOTAL_UNITS*UNIT_TYPE_COUNT * MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotalMax);
}

inline const unsigned int ANARACE::getNonGoalsUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(nonGoalsUnitsTotalMax>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT * MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getNonGoalsUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(nonGoalsUnitsTotalMax);
}

inline const unsigned int ANARACE::getUnitsTotal() const
{
#ifdef _SCC_DEBUG
	if(unitsTotal>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT * MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotal): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotal);
}

inline const bool ANARACE::isActive() const {
	return(active);
}

inline void ANARACE::setActive(const bool set_active) {
	active = set_active;
}

inline const bool ANARACE::isOptimizing() const {
	return(optimizing);
}

inline void ANARACE::setOptimizing(const bool set_optimizing) {
	optimizing = set_optimizing;
}


// ------ END OF STATISTICS GET FUNCTIONS ------

inline const unsigned int ANARACE::getTimePercentage() const {
	return(timePercentage);
}

inline const unsigned int ANARACE::getFastestGoalTime() const {
	return(getpGoal()->calculateFastestBO((*pStartCondition)->getUnit(GLOBAL)));
}

inline const GOAL_TREE ANARACE::getGoalTree(const unsigned int currentGoalUnit) const {
	return(getpGoal()->getGoalTree((*pStartCondition)->getUnit(GLOBAL), currentGoalUnit));
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

inline void ANARACE::setTotalGeneration(const unsigned int total_generation) {
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

#endif //_CORE_ANARACE_HPP

