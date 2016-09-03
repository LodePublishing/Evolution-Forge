#ifndef _CORE_BUILDORDER_HPP
#define _CORE_BUILDORDER_HPP

#include "prebuildorder.hpp"

class BUILDORDER : public PREBUILDORDER
{
	private:
	       	unsigned int mutationRate;
       		const bool buildGene(const unsigned int build_unit);
		unsigned int pFitness;
		unsigned int sFitness;
		unsigned int tFitness;
		
		const bool buildIt(const unsigned int build_unit);
	public:
		BUILDORDER();
		~BUILDORDER();
		BUILDORDER& operator=(const BUILDORDER& object);
		BUILDORDER(const BUILDORDER& object);
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
//		void crossOver(BUILDORDER* parent2, BUILDORDER* child1, BUILDORDER* child2);
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
	if(s_fitness > MAX_MINERALS+MAX_GAS) {
		toErrorLog("DEBUG: (BUILDORDER::setsFitness): Value s_fitness out of range.");return;
	}
#endif
	sFitness = s_fitness;
}

inline void BUILDORDER::settFitness(const unsigned int t_fitness)
{
#ifdef _SCC_DEBUG
        if(t_fitness > MAX_TFITNESS) {
                toErrorLog("DEBUG: (BUILDORDER::settFitness): Value t_fitness out of range.");return;
        }
#endif
        tFitness = t_fitness;
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
	if(sFitness>MAX_MINERALS+MAX_GAS) {
		toErrorLog("DEBUG: (BUILDORDER::getsFitness): Variable sFitness not initialized.");return(0);
	}
#endif
	return(sFitness);
}

inline const unsigned int BUILDORDER::gettFitness() const
{
#ifdef _SCC_DEBUG
        if(tFitness>MAX_TFITNESS) {
                toErrorLog("DEBUG: (BUILDORDER::gettFitness): Variable tFitness not initialized.");return(0);
        }
#endif
        return(tFitness);
}


#endif //_CORE_BUILDORDER_HPP
