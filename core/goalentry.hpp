#ifndef _CORE_GOALENTRY_HPP
#define _CORE_GOALENTRY_HPP

#include "location.hpp"
#include "unit.hpp"
#include "goal.hpp"

struct GOAL_TREE
{
	bool checked[UNIT_TYPE_COUNT];
	unsigned int level[UNIT_TYPE_COUNT];
	unsigned int coloumn[UNIT_TYPE_COUNT];
	unsigned int width[10];
	std::list<unsigned int> con[UNIT_TYPE_COUNT];
	std::list<unsigned int> unit[10];
};

struct ALLOW
{
        list<unsigned int> facility;
        list<unsigned int> facility2;
        list<unsigned int> prerequisite;
};

struct NEED
{
	public:
		const bool facilityIsDone();
		const bool facility2IsDone();
		const bool prerequisiteIsDone();
		NEED& operator=(const NEED& object);
	
        	bool facility;
	        bool facility2;
        	unsigned int prerequisite; // we need all... no double?
};


class GOAL_ENTRY
{
	private:
		unsigned int number; // to look up in the goal name list //???? TODO
		std::string name;
		eRace race;
		unsigned int maxBuildTypes;
		const UNIT_STATISTICS* pStats;
//		bool initialized;
		unsigned int phaenoToGenotype[UNIT_TYPE_COUNT];
		unsigned int genoToPhaenotype[UNIT_TYPE_COUNT];
//		int mode;

		unsigned int allGoal[UNIT_TYPE_COUNT];
		unsigned int globalGoal[MAX_LOCATIONS][UNIT_TYPE_COUNT];
		bool changed;
		bool raceInitialized;
		bool isBuildable[UNIT_TYPE_COUNT];
		bool isStatic[UNIT_TYPE_COUNT];	
		bool isHaveable[UNIT_TYPE_COUNT]; // all units that are goals, can be build or are build by the bo (larva etc.)

	public:
		std::list<GOAL> goal; // private?
		NEED need[UNIT_TYPE_COUNT];
		ALLOW allow[UNIT_TYPE_COUNT];
		
		GOAL_ENTRY();
		GOAL_ENTRY(const GOAL_ENTRY& object);
		~GOAL_ENTRY();
		GOAL_ENTRY& operator=(const GOAL_ENTRY& object);

 
//		const unsigned int getMode() const; // 0: normal, 1: based on success of enemy
//		void setMode(int mode);

		const std::list<unsigned int> allowDeletion(const unsigned int unitType);
		const std::list<unsigned int> allowAddition(const unsigned int unitType);

		const unsigned int countGoals() const;
		void calculateFinalTimes(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time);
		const bool calculateReady(const UNIT* units) const;
		const unsigned int getAllGoal(const unsigned int unit) const;
		const unsigned int getGlobalGoal(const unsigned int location, const unsigned int unit) const;
		const bool getIsBuildable(const unsigned int unit) const;
		const bool getIsHaveable(const unsigned int unit) const;
		const bool getIsStatic(const unsigned int unit) const;

		void resetData();

//		const GOAL& getGoal(const int goal_number) const;
//		const bool getNextGoal(std::list<GOAL>::const_iterator& current, const bool first) const;
		void addGoal(const unsigned int unit, const signed int count, const unsigned int time, const unsigned int location);
		void adjustGoals(const bool allowGoalAdaption, const UNIT* unit=0);
		const unsigned int calculateFastestBO(const UNIT* startForce) const;
		const GOAL_TREE getGoalTree(const UNIT* startForce, const unsigned int currentGoalUnit) const;

		const bool isChanged() const;
		void changeAccepted();
		const std::string& getName() const;
		const UNIT_STATISTICS* getpStats() const;
		const unsigned int toGeno(const unsigned int phaeno) const;
		const unsigned int toPhaeno(const unsigned int geno) const;
		const eRace getRace() const;
		const unsigned int getMaxBuildTypes() const;
		const bool isRaceInitialized() const;
		void setName(const std::string& goal_name);
		void setRace(const eRace goal_race);
		const bool isGoal(const unsigned int unit) const;
		const bool getInitialized() const;

};

#endif // _CORE_GOALENTRY_HPP

