#ifndef _CORE_RACE_HPP
#define _CORE_RACE_HPP

#include "prerace.hpp"

class RACE: public PRERACE
{
	private:
       	int mutationRate;
       	const bool buildGene(const int unit);
		int pFitness;
		int sFitness;
		int tFitness;
	public:
//Output:
		void setpFitness(const int pFitness);
		void setsFitness(const int sFitness);
		void settFitness(const int tFitness);

		const int calculateSecondaryFitness() const; //?

		const int getpFitness() const;
		const int getsFitness() const;
		const int gettFitness() const;
//Controls:
		const bool calculateStep(); 
//		void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
		void prepareForNewGeneration(); //resets all data to standard values
//		void crossOver(RACE* parent2, RACE* child1, RACE* child2);
		RACE();
		~RACE();
};

#endif //_CORE_RACE_HPP
