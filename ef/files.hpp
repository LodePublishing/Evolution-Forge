#ifndef _GUI_FILES_HPP
#define _GUI_FILES_HPP

#include "../core/database.hpp"

class FILES
{
	public:
		static const bool loadHarvestFile(const std::string& harvest_file);
		static const bool loadGoalFile(const std::string& goal_file);
		static const bool loadBuildOrderFile(const std::string& build_order_file);
		static const bool loadStartConditionFile(const std::string& start_condition_file);
		static const bool loadMapFile(const std::string& mapFile);

		static const bool saveGoal(const std::string& goal_name, GOAL_ENTRY* goalentry);
		static const bool saveBuildOrder(const std::string& build_order_name, const BUILD_ORDER& build_order, ANABUILDORDER* bla);
		static const bool saveHarvestFile(const std::string& harvest_file);
	private:
		FILES();
		~FILES();
};


#endif
