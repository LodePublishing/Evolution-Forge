#ifndef _CORE_ANABUILDORDER_HPP
#define _CORE_ANABUILDORDER_HPP

#include "prebuildorder.hpp"
#include "program.hpp"

class ANABUILDORDER : public PREBUILDORDER
{
	public:
		ANABUILDORDER(const unsigned int player_number, START* start, UNIT (&units)[MAX_PLAYER][MAX_LOCATIONS]);
		~ANABUILDORDER();
	
		const unsigned int getNonGoalsUnitsTotalMax() const;
		const unsigned int getUnitsTotal() const;
		
		void countUnitsTotal(); // to set maxUnitForce and totalUnitForce
		const unsigned int getCurrentpFitness() const;		

		const bool isOptimizing() const;
		void setOptimizing(const bool optimizing=true);

		const std::list<PROGRAM>& getProgramList() const;
		const std::list<STATISTICS>& getTimeStatisticsList() const;

// external data output

		const unsigned int getFastestGoalTime() const;

		const GOAL_TREE getGoalTree(const unsigned currentGoalUnit=0) const;
		

		const unsigned int getTimePercentage() const;
		const unsigned int getGoalPercentage() const;
				

		const unsigned int getMaxpFitness() const;			// gets primary fitness (goal and time)
		const unsigned int getMaxsFitness() const;			// gets secondary fitness (ressources)
		
// internal control structures, do not touch ;-)
		void setMaxpFitness(const unsigned int max_pfitness);
		void setMaxsFitness(const unsigned int max_sfitness);

		const bool calculateStep();			// calculates another time step of current generation
		void resetData();				// resets all data to standard values
		void restartData();				// resets all data to standard values
		void prepareForNewGeneration();				// resets all data to standard values
	
		const bool writeProgramBackToCode(const std::list<PROGRAM>& program_list);
		void copyProgramList(const std::list<PROGRAM>& program_list);

		void postProcessing(); // TODO nachschauen ob virtual private geht
	protected:
	//	ANABUILDORDER& operator=(const ANABUILDORDER& anarace);
	//	ANABUILDORDER(const ANABUILDORDER& anarace);
	private:
		const STATISTICS generateStatistics() const;
		std::list<PROGRAM> programList;
		unsigned int unitsTotal; // total number of all unit types at the end
		unsigned int unitsTotalMax; // maximum number of one unit type at the end
		unsigned int nonGoalsUnitsTotalMax; // maximum number of one NON GOAL unit type at the end
		bool optimizing; // mutate, sort, multiply etc.
		unsigned int maxpFitness;
		unsigned int maxsFitness;
		unsigned int timePercentage;
		unsigned int goalPercentage;
		unsigned int currentpFitness;
		std::list<STATISTICS> timeStatisticsList;
		void setCurrentpFitness(unsigned int current_pfitness);
};

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

inline const std::list<PROGRAM>& ANABUILDORDER::getProgramList() const {
	return(programList);
}

inline const std::list<STATISTICS>& ANABUILDORDER::getTimeStatisticsList() const {
	return(timeStatisticsList);
}

inline const unsigned int ANABUILDORDER::getNonGoalsUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(nonGoalsUnitsTotalMax > GAME::MAX_TOTAL_UNITS * RACE::UNIT_TYPE_COUNT * MAX_LOCATIONS) {
		toErrorLog("DEBUG: (ANABUILDORDER::getNonGoalsUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(nonGoalsUnitsTotalMax);
}

inline const unsigned int ANABUILDORDER::getUnitsTotal() const
{
#ifdef _SCC_DEBUG
	if(unitsTotal > GAME::MAX_TOTAL_UNITS * RACE::UNIT_TYPE_COUNT * MAX_LOCATIONS) {
		toErrorLog("DEBUG: (ANABUILDORDER::getUnitsTotal): Variable not initialized.");return(0);
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


// ------ END OF STATISTICS GET FUNCTIONS ------

inline const unsigned int ANABUILDORDER::getTimePercentage() const {
	return(timePercentage);
}

inline const unsigned int ANABUILDORDER::getFastestGoalTime() const {
	return(getGoal()->calculateFastestBO((*(getStartCondition()))->getUnit(GLOBAL)));
}

inline const GOAL_TREE ANABUILDORDER::getGoalTree(const unsigned int currentGoalUnit) const {
	return(getGoal()->getGoalTree((*(getStartCondition()))->getUnit(GLOBAL), currentGoalUnit));
}

inline const unsigned int ANABUILDORDER::getCurrentpFitness() const
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
	if(currentpFitness > MAX_PFITNESS) {
		toErrorLog("DEBUG: (ANABUILDORDER::getCurrentpFitness): Variable currentpFitness not initialized.");return(0);
	}
#endif
	return(currentpFitness);
}

inline void ANABUILDORDER::setCurrentpFitness(const unsigned int current_pfitness)
{
#ifdef _SCC_DEBUG
// TODO max pfitness?
	if(current_pfitness > MAX_PFITNESS) {
		toErrorLog("DEBUG: (ANABUILDORDER::setCurrentpFitness): Variable current_pfitness not initialized.");return;
	}
#endif
	currentpFitness = current_pfitness;
}


inline const unsigned int ANABUILDORDER::getMaxpFitness() const
{
#ifdef _SCC_DEBUG	
// TODO max pfitness?
	if(maxpFitness > MAX_PFITNESS) {
		toErrorLog("DEBUG: (ANABUILDORDER::getMaxpFitness): Variable maxpFitness not initialized.");return(0);
	}
#endif
	return(maxpFitness);
}

inline const unsigned int ANABUILDORDER::getMaxsFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxsFitness > 2 * GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG: (ANABUILDORDER::getMaxsFitness): Variable maxsFitness not initialize.");return(0);
	}
#endif
	return(maxsFitness);
}

inline void ANABUILDORDER::setMaxpFitness(const unsigned int max_pfitness) 
{
#ifdef _SCC_DEBUG
// TODO maxpitness
	if(max_pfitness > MAX_PFITNESS) {
		toErrorLog("DEBUG: (ANABUILDORDER::setMaxpFitness): Value out of range.");return;
	}
#endif
	maxpFitness = max_pfitness;
}

inline void ANABUILDORDER::setMaxsFitness(const unsigned int max_sfitness)
{
#ifdef _SCC_DEBUG
	if(max_sfitness > 2 * GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG: (ANABUILDORDER::setMaxsFitness): Value out of range.");return;
	}
#endif
	maxsFitness = max_sfitness;
}

#endif //_CORE_ANABUILDORDER_HPP

