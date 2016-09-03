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
        	int buildGene(int unit);
       		static void generateBasicBuildOrder(); //pre-process goals to create a b
		int pFitness;
		int sFitness;
		int tFitness;
	public:
//Output:
		int setpFitness(int num);
		int setsFitness(int num);
		int settFitness(int num);

		void calculateFitness(int ready);

		int getpFitness();
		int getsFitness();
		int gettFitness();
//Controls:
		void eraseIllegalCode();
		void mutateGeneCode();
		int calculateStep();
		void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
		void resetData(); //resets all data to standard values
		void crossOver(RACE* parent2, RACE* child1, RACE* child2);
		RACE();
		~RACE();
};

#endif //__RACE_H
