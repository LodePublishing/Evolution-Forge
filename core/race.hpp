#ifndef _CORE_RACE_HPP
#define _CORE_RACE_HPP

#include "prerace.hpp"

class RACE : public PRERACE
{
	private:
	       	unsigned int mutationRate;
       		const bool buildGene(const unsigned int build_unit);
		unsigned int pFitness;
		unsigned int sFitness;
		unsigned int tFitness;
		
		const bool buildIt(const unsigned int build_unit);
	public:
		RACE();
		~RACE();
		RACE& operator=(const RACE& object);
		RACE(const RACE& object);
		void resetData();
//Output:
		void setpFitness(const unsigned int p_fitness);
		void setsFitness(const unsigned int s_fitness);
		void settFitness(const unsigned int t_fitness);

		const unsigned int calculateSecondaryFitness() const; //?

		const unsigned int getpFitness() const;
		const unsigned int getsFitness() const;
		const unsigned int gettFitness() const;
//Controls:
		const bool calculateStep(); 
//		void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
		void prepareForNewGeneration(); //resets all data to standard values
//		void crossOver(RACE* parent2, RACE* child1, RACE* child2);
};

// ----------------------------------------------------
// ------ PRETTY UNINTERESTING SET/GET FUNCTIONS ------
// ----------------------------------------------------

inline void RACE::setpFitness(const unsigned int p_fitness) 
{
#ifdef _SCC_DEBUG
	if(p_fitness > MAX_PFITNESS) {
		toLog("DEBUG: (RACE::setpFitness): Value p_fitness out of range.");return;
	}
#endif
	pFitness = p_fitness;
}

inline void RACE::setsFitness(const unsigned int s_fitness)
{
#ifdef _SCC_DEBUG
	if(s_fitness > MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (RACE::setsFitness): Value s_fitness out of range.");return;
	}
#endif
	sFitness = s_fitness;
}

inline void RACE::settFitness(const unsigned int t_fitness)
{
#ifdef _SCC_DEBUG
        if(t_fitness > MAX_TFITNESS) {
                toLog("DEBUG: (RACE::settFitness): Value t_fitness out of range.");return;
        }
#endif
        tFitness = t_fitness;
}


inline const unsigned int RACE::getpFitness() const
{
#ifdef _SCC_DEBUG
	if(pFitness > MAX_PFITNESS) {
		toLog("DEBUG: (RACE::getpFitness): Variable pFitness not initialized.");return(0);
	}
#endif
	return(pFitness);
}

inline const unsigned int RACE::getsFitness() const
{
#ifdef _SCC_DEBUG
	if(sFitness>MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (RACE::getsFitness): Variable sFitness not initialized.");return(0);
	}
#endif
	return(sFitness);
}

inline const unsigned int RACE::gettFitness() const
{
#ifdef _SCC_DEBUG
        if(tFitness>MAX_TFITNESS) {
                toLog("DEBUG: (RACE::gettFitness): Variable tFitness not initialized.");return(0);
        }
#endif
        return(tFitness);
}


#endif //_CORE_RACE_HPP
