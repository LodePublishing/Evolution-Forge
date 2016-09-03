#ifndef _CORE_BUILDORDER_HPP
#define _CORE_BUILDORDER_HPP

#include "prebuildorder.hpp"
#include "program.hpp"
#include "statistics.hpp"

class BUILDORDER : public PREBUILDORDER
{
	public:
		BUILDORDER(const unsigned int player_number, START* start, UNIT (&units)[MAX_PLAYER][MAX_LOCATIONS]);
		~BUILDORDER();
		void resetData();
//Output:
		void setpFitness(const unsigned int p_fitness);
		void setsFitness(const unsigned int s_fitness);

		const unsigned int calculateSecondaryFitness() const; //?

		const unsigned int getpFitness() const;
		const unsigned int getsFitness() const;
//Controls:
		const bool calculateStep(); 
		void prepareForNewGeneration(); //resets all data to standard values
		void postProcessing();
	protected:
	
	private:
	       	unsigned int mutationRate;
		unsigned int pFitness;
		unsigned int sFitness;
		
		BUILDORDER& operator=(const BUILDORDER& object);
		BUILDORDER(const BUILDORDER& object);
};

// ----------------------------------------------------
// ------ PRETTY UNINTERESTING SET/GET FUNCTIONS ------
// ----------------------------------------------------

inline void BUILDORDER::setpFitness(const unsigned int p_fitness) 
{
#ifdef _SCC_DEBUG
	if(p_fitness > MAX_PFITNESS) {
		toErrorLog("DEBUG: (BUILDORDER::setpFitness): Value p_fitness out of range.");return;
	}
#endif
	pFitness = p_fitness;
}

inline void BUILDORDER::setsFitness(const unsigned int s_fitness)
{
#ifdef _SCC_DEBUG
	if(s_fitness > 2*GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG: (BUILDORDER::setsFitness): Value s_fitness out of range.");return;
	}
#endif
	sFitness = s_fitness;
}

inline const unsigned int BUILDORDER::getpFitness() const
{
#ifdef _SCC_DEBUG
	if(pFitness > MAX_PFITNESS) {
		toErrorLog("DEBUG: (BUILDORDER::getpFitness): Variable pFitness not initialized.");return(0);
	}
#endif
	return(pFitness);
}

inline const unsigned int BUILDORDER::getsFitness() const
{
#ifdef _SCC_DEBUG
	if(sFitness > 2*GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG: (BUILDORDER::getsFitness): Variable sFitness not initialized.");return(0);
	}
#endif
	return(sFitness);
}

#endif //_CORE_BUILDORDER_HPP
