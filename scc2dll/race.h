#ifndef __RACE_H
#define __RACE_H

#include "main.h"
#include "prerace.h"

class RACE: public PRERACE
{
	private:
       	static int basicLength;
       	static int basicBuildOrder[2][MAX_LENGTH];
       	int mutationRate;
       	const int buildGene(const int unit);
 		static void generateBasicBuildOrder(); //pre-process goals to create a b
		int pFitness;
		int sFitness;
		int tFitness;
	public:
//Output:
		void setpFitness(const int num);
		void setsFitness(const int num);
		void settFitness(const int num);

		const int calculateSecondaryFitness() const; //?

		const int getpFitness() const;
		const int getsFitness() const;
		const int gettFitness() const;
//Controls:
		void eraseIllegalCode();
		void mutateGeneCode();
		const int calculateStep(); //?
		void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
		void resetData(); //resets all data to standard values
		void crossOver(RACE* parent2, RACE* child1, RACE* child2);
		RACE();
		~RACE();
};

#endif //__RACE_H
