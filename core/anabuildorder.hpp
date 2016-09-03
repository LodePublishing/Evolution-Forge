#ifndef _CORE_ANABUILDORDER_HPP
#define _CORE_ANABUILDORDER_HPP

#include "prebuildorder.hpp"
#include "program.hpp"
#include "statistics.hpp"

class ANABUILDORDER: public PREBUILDORDER
{
	public:
		ANABUILDORDER();
		~ANABUILDORDER();
		ANABUILDORDER& operator=(const ANABUILDORDER& anarace);
		ANABUILDORDER(const ANABUILDORDER& anarace);
	
		unsigned int fitnessAverage;
		unsigned int fitnessVariance;
		unsigned int positiveCrossover;
		unsigned int wayneCrossover;
		unsigned int negativeCrossover;

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
				

		const unsigned int getMaxpFitness() const;			// gets primary fitness (goal and time)
		const unsigned int getMaxsFitness() const;			// gets secondary fitness (ressources)
		const unsigned int getMaxtFitness() const;
		
//		void insertOrder(int unit, int position);
//		void removeOrder(int IP);


// internal control structures, do not touch ;-)
		void setMaxpFitness(const unsigned int max_pfitness);
		void setMaxsFitness(const unsigned int max_sfitness);
		void setMaxtFitness(const unsigned int max_tfitness);

		const bool calculateStep();			// calculates another time step of current generation
		void resetData();				// resets all data to standard values
		void restartData();				// resets all data to standard values
		static void resetStaticData();
		void prepareForNewGeneration();				// resets all data to standard values
	
		const bool writeProgramBackToCode(std::list<PROGRAM>& program_list);
		void copyProgramList(std::list<PROGRAM>& program_list);

		std::list<PROGRAM> programList; // TODO private machen
	private:
		unsigned int unitsTotal; // total number of all unit types at the end
		unsigned int unitsTotalMax; // maximum number of one unit type at the end
		unsigned int nonGoalsUnitsTotalMax; // maximum number of one NON GOAL unit type at the end
		bool optimizing; // mutate, sort, multiply etc.
		bool active; // calculate
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
		void postProcessing();
};

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

/*void ANABUILDORDER::setFixed(const bool* fixed_list)
{
	fixed = fixed_list;
}

const bool* ANABUILDORDER::getFixed() const
{
	return(fixed);
}*/

inline std::list<PROGRAM>& ANABUILDORDER::getProgramList() {
	return(programList);
}

inline STATISTICS* ANABUILDORDER::getTimeStatistics() {
	return(timeStatistics);
}

inline const unsigned int ANABUILDORDER::getAverageLength() const {
	return(averageLength);
}

inline const unsigned int ANABUILDORDER::getNonGoalsUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(nonGoalsUnitsTotalMax>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT * MAX_LOCATIONS) {
		toLog("DEBUG: (ANABUILDORDER::getNonGoalsUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(nonGoalsUnitsTotalMax);
}

inline const unsigned int ANABUILDORDER::getUnitsTotal() const
{
#ifdef _SCC_DEBUG
	if(unitsTotal>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT * MAX_LOCATIONS) {
		toLog("DEBUG: (ANABUILDORDER::getUnitsTotal): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotal);
}

inline const bool ANABUILDORDER::isOptimizing() const {
	return(optimizing);
}

inline void ANABUILDORDER::setOptimizing(const bool set_optimizing) {
	optimizing = set_optimizing;
}

inline const bool ANABUILDORDER::isActive() const {
	return(active);
}

inline void ANABUILDORDER::setActive(const bool set_active) {
	active = set_active;
}


// ------ END OF STATISTICS GET FUNCTIONS ------

inline const unsigned int ANABUILDORDER::getTimePercentage() const {
	return(timePercentage);
}

inline const unsigned int ANABUILDORDER::getFastestGoalTime() const {
	return(getGoal()->calculateFastestBO((*(pStart->getStartCondition()))->getUnit(GLOBAL)));
}

inline const GOAL_TREE ANABUILDORDER::getGoalTree(const unsigned int currentGoalUnit) const {
	return(getGoal()->getGoalTree((*(pStart->getStartCondition()))->getUnit(GLOBAL), currentGoalUnit));
}

inline const unsigned int ANABUILDORDER::getCurrentpFitness() const
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
	if(currentpFitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANABUILDORDER::getCurrentpFitness): Variable currentpFitness not initialized.");return(0);
	}
#endif
	return(currentpFitness);
}

inline void ANABUILDORDER::setCurrentpFitness(const unsigned int current_pfitness)
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
	if(current_pfitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANABUILDORDER::setCurrentpFitness): Variable current_pfitness not initialized.");return;
	}
#endif
	currentpFitness = current_pfitness;
}


inline const unsigned int ANABUILDORDER::getMaxpFitness() const
{
#ifdef _SCC_DEBUG	
// TODO max pfitness?
	if(maxpFitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANABUILDORDER::getMaxpFitness): Variable maxpFitness not initialized.");return(0);
	}
#endif
	return(maxpFitness);
}

inline const unsigned int ANABUILDORDER::getMaxsFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxsFitness > MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANABUILDORDER::getMaxsFitness): Variable maxsFitness not initialize.");return(0);
	}
#endif
	return(maxsFitness);
}

inline const unsigned int ANABUILDORDER::getMaxtFitness() const
{
#ifdef _SCC_DEBUG       
        if(maxtFitness > MAX_TFITNESS) {
                toLog("DEBUG: (ANABUILDORDER::getMaxtFitness): Variable maxtFitness not initialized.");return(0);
        }
#endif
        return(maxtFitness);
}

inline void ANABUILDORDER::setMaxpFitness(const unsigned int max_pfitness) 
{
#ifdef _SCC_DEBUG
// TODO maxpitness
	if(max_pfitness > MAX_PFITNESS) {
		toLog("DEBUG: (ANABUILDORDER::setMaxpFitness): Value out of range.");return;
	}
#endif
	maxpFitness = max_pfitness;
}

inline void ANABUILDORDER::setMaxsFitness(const unsigned int max_sfitness)
{
#ifdef _SCC_DEBUG
	if(max_sfitness > MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANABUILDORDER::setMaxsFitness): Value out of range.");return;
	}
#endif
	maxsFitness = max_sfitness;
}

inline void ANABUILDORDER::setMaxtFitness(const unsigned int max_tfitness)
{
#ifdef _SCC_DEBUG
        if(max_tfitness > MAX_TFITNESS) {
                toLog("DEBUG: (ANABUILDORDER::setMaxtFitness): Value out of range.");return;
        }
#endif
        maxtFitness = max_tfitness;
}

#endif //_CORE_ANABUILDORDER_HPP

