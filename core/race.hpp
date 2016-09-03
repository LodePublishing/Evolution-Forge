#ifndef _CORE_RACE_HPP
#define _CORE_RACE_HPP

#include "prerace.hpp"

class RACE: public PRERACE
{
	private:
       	unsigned int mutationRate;
       	const bool buildGene(const unsigned int build_unit);
		unsigned int pFitness;
		unsigned int sFitness;
		unsigned int tFitness;
	public:
//Output:
		void setpFitness(const unsigned int pFitness);
		void setsFitness(const unsigned int sFitness);
		void settFitness(const unsigned int tFitness);

		const unsigned int calculateSecondaryFitness() const; //?

		const unsigned int getpFitness() const;
		const unsigned int getsFitness() const;
		const unsigned int gettFitness() const;
//Controls:
		const bool calculateStep(); 
//		void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
		void prepareForNewGeneration(); //resets all data to standard values
//		void crossOver(RACE* parent2, RACE* child1, RACE* child2);

		const RACE& operator=(const RACE& race);
		RACE(const RACE& race);
		RACE();
		~RACE();
};

#endif //_CORE_RACE_HPP
