// -----------------------------------------------------------------------------------------
// DATABASE holds all map, harvest, goal and start-condition date for other classes to read
// In addition it saves and loads these data from/to files.
// -----------------------------------------------------------------------------------------

#include "database.hpp"

#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

DATABASE::DATABASE():
	loadedMap(),
	goalDataInitialized(false),
	buildOrderDataInitialized(true), // we don't need bos...
	startConditionDataInitialized(false),
	mapDataInitialized(false),
	harvestDataInitialized(false)
{}

DATABASE::~DATABASE()
{
	for(std::vector<BASIC_MAP*>::iterator i = loadedMap.begin(); i!=loadedMap.end(); ++i)
		delete *i;
	loadedMap.clear();
	for(unsigned int j = 0; j < MAX_RACES; ++j)
	{
		for(std::vector<START_CONDITION*>::iterator i = loadedStartCondition[j].begin(); i!=loadedStartCondition[j].end(); ++i)
			delete *i;
		for(std::vector<GOAL_ENTRY*>::iterator i = loadedGoal[j].begin(); i!=loadedGoal[j].end(); ++i)
			delete *i;
		for(std::vector<BUILD_ORDER*>::iterator i = loadedBuildOrder[j].begin(); i!=loadedBuildOrder[j].end(); ++i)
			delete *i;
	
		loadedStartCondition[j].clear();
		loadedGoal[j].clear();
		loadedBuildOrder[j].clear();
	}
}




void DATABASE::addDefaultGoal(const eRace race)
{
	GOAL_ENTRY* goal = new GOAL_ENTRY;
	goal->setName("Clear List");
	goal->setRace(race);
	loadedGoal[goal->getRace()].push_back(goal);
	goalDataInitialized = true;
}

// ---------------------------
// ------- FILE LOADING ------
// ---------------------------

GOAL_ENTRY* parseGoalBlock(std::map<std::string, std::list<std::string> > block)
{
	GOAL_ENTRY* goal = new GOAL_ENTRY;
	std::map<std::string, std::list<std::string> >::iterator i;
	
	if((i=block.find("Name"))!=block.end())
	{
		i->second.pop_front();
	  	goal->setName(i->second.front());
		block.erase(i);		
	} else
	{
		toErrorLog("ERROR (parseGoalBlock()): Field name 'Name' not found.");
		delete goal;
		return(NULL);
	}

	if((i=block.find("Race"))!=block.end()){
		i->second.pop_front();
	
		std::string estr = i->second.front();
		eRace goal_race = TERRA;
		if(estr == raceString[TERRA]) goal_race=TERRA;
		else if(estr == raceString[PROTOSS]) goal_race=PROTOSS;
		else if(estr == raceString[ZERG]) goal_race=ZERG;
		else {
			toErrorLog("ERROR (parseGoalBlock()): Invalid race entry (" + estr + " [" + raceString[TERRA] + "|" + raceString[PROTOSS] + "|" + raceString[ZERG] + "]).");
			delete goal;
			return(NULL);
		}
		goal->setRace(goal_race);
		block.erase(i);
	} else
	{
		toErrorLog("ERROR (parseGoalBlock()): Field name 'Race' not found.");
		delete goal;
		return(NULL);
	}

	bool found_at_least_one = false;
	i = block.begin();
	while(i!=block.end())
	{
		bool found = false;
		for(unsigned int unit=LAST_UNIT; (unit--) && (i!=block.end() ); )
			if(*(i->second.begin()) == stats[goal->getRace()][unit].name)
			{
				std::list<std::string>::iterator l=i->second.begin();
				if(l->size()>=3)
				{
					++l;int count=atoi(l->c_str());
					++l;int location=atoi(l->c_str());
					++l;int time=atoi(l->c_str());
					goal->addNewGoalToList(unit, time, location, count);
					found_at_least_one = true;
//					rueckgabewert TODO
					block.erase(i);
					i = block.begin();
					found = true;
				} else
				{
					toErrorLog("WARNING (parseGoalBlock()): Incorrect number of parameters for entry in goal list (\'" + *(i->second.begin()) + "\'): 3 parameters expected => line will be ignored.");
					block.erase(i);
					i = block.begin();
					found = true;
				}
			}
		if(!found)
		{
			toErrorLog("WARNING (parseGoalBlock()): Unknown entry in goal list (\'" + *(i->second.begin()) + "\') => line will be ignored.");
			i++;
		}
	}
	if(!found_at_least_one)
	{
		toErrorLog("ERROR (parseGoalBlock()): Empty goal list => file will be ignored.");
		delete goal;
		return(NULL);
	} else
		return(goal);
}

BO_HEADER* parseBoHeaderBlock(std::map<std::string, std::list<std::string> > block)
{
	BO_HEADER* bo_header = new BO_HEADER;
	std::map<std::string, std::list<std::string> >::iterator i;
	
	if((i=block.find("Name"))!=block.end())
	{
		i->second.pop_front();
	  	bo_header->setName(i->second.front());
		block.erase(i);		
	} else
	{
		toErrorLog("ERROR (parseBoHeaderBlock()): Field name 'Name' not found.");
		delete bo_header;
		return(NULL);
	}

	if((i=block.find("Race"))!=block.end()){
		i->second.pop_front();
	
		std::string estr = i->second.front();
		eRace bo_race = TERRA;
		if(estr == raceString[TERRA]) bo_race=TERRA;
		else if(estr == raceString[PROTOSS]) bo_race=PROTOSS;
		else if(estr == raceString[ZERG]) bo_race=ZERG;
		else {
			toErrorLog("ERROR (parseBoHeaderBlock()): Invalid race entry (" + estr + " [" + raceString[TERRA] + "|" + raceString[PROTOSS] + "|" + raceString[ZERG] + "]).");
			delete bo_header;
			return(NULL);
		}
		bo_header->setRace(bo_race);
		block.erase(i);
	} else
	{
		toErrorLog("ERROR (parseBoHeaderBlock()): Field name 'Race' not found.");
		delete bo_header;
		return(NULL);
	}
	
	if((i=block.find("Time"))!=block.end())
	{
		i->second.pop_front();
	  	bo_header->setTime(atoi(i->second.front().c_str()));
		block.erase(i);
	} else
	{
		toErrorLog("ERROR (parseBoHeaderBlock()): Field name 'Time' not found.");
		delete bo_header;
		return(NULL);
	}
	return(bo_header);
}

std::list<PROGRAM>* parseBuildOrderBlock(const eRace bo_race, std::list<std::list<std::string> > block)
{
	bool found_at_least_one = false;
	std::list<PROGRAM>* program_list = new std::list<PROGRAM>;
	for(std::list<std::list<std::string> >::iterator i = block.begin(); i!=block.end(); ++i)
	{
		bool found = false;
		for(unsigned int unit=LAST_UNIT; unit--;)
			if(*(i->begin())==stats[bo_race][unit].name)
			{
				PROGRAM p;
				p.setUnit(unit);
				program_list->push_back(p);
				found = true;
				found_at_least_one = true;
				break;
			}
		if(!found)
			toErrorLog("WARNING (parseBuildOrderBlock()): Unkown entry in build order list (\'" + *(i->begin()) + "\') => ignoring entry.");
	}
	
	if(!found_at_least_one)
	{
		toErrorLog("ERROR (parseBuildOrderBlock()): Empty build order list => file will be ignored.");
		delete program_list;
		return(NULL);
	} else
		return(program_list);
}


const bool DATABASE::loadGoalFile(const std::string& goal_file)
{
	if((goal_file.compare(goal_file.size()-4, goal_file.size(), ".gol")==1))
		return(true);

	std::ifstream pFile(goal_file.c_str());
	if(!checkStreamIsOpen(pFile, "DATABASE::loadGoalFile", goal_file))
		return(false);
	
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "DATABASE::loadGoalFile", goal_file))
			return(false);

		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start == std::string::npos) || (text[0] == '#') || (text[0] == '\0'))
			continue; // ignore line
		size_t stop = text.find_first_of("\t ", start);
		if(stop == std::string::npos) 
			stop = text.size();
		std::string index = text.substr(start, stop);
		if(index=="@GOAL")
		{
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (DATABASE::loadGoalFile()): No concluding @END for @GOAL block was found in file " + goal_file + " => trying to parse what we have so far.");
			}
			GOAL_ENTRY* my_goal = parseGoalBlock(block);
			if(my_goal == NULL)
			{
				toErrorLog("ERROR (DATABASE::loadGoalFile()): Error parsing " + goal_file + ".");
				return(false);
			}
			else 
			{
				loadedGoal[my_goal->getRace()].push_back(my_goal);
				goalDataInitialized = true;
				return(true);
			}
		} // end index == GOAL
	} // end while
	toErrorLog("ERROR (DATABASE::loadGoalFile()): No @GOAL block was found in file " + goal_file + ".");
	return(false);	
} // schoen :) naja :o

const bool DATABASE::loadBuildOrderFile(const std::string& build_order_file)
{
// just ignore the file if it's no .txt file... 
	if((build_order_file.compare(build_order_file.size()-4, build_order_file.size(), ".txt")==1))
		return(true);

	std::ifstream pFile(build_order_file.c_str());
	if(!checkStreamIsOpen(pFile, "DATABASE::loadBuildOrderFile", build_order_file))
		return(false);
	
	bool found_goal_block = false;
	bool found_bo_block = false;
	bool found_bo_header_block = false;
	
	BO_HEADER* bo_header = NULL;
	GOAL_ENTRY* bo_goal = NULL;
	std::list<PROGRAM>* program_list = NULL;
	
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "DATABASE::loadBuildOrderFile", build_order_file))
			return(false);
	
		std::string text = line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		size_t stop_position = text.find_first_of("\t ", start_position);
		if(stop_position == std::string::npos) 
			stop_position = text.size();
		std::string index = text.substr(start_position, stop_position);

		if(index=="@BO_HEADER")
		{
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (DATABASE::loadBuildOrderFile()): No concluding @END for @BO_HEADER block was found in file " + build_order_file + " => trying to parse what we have.");
			}
			if(found_bo_header_block)
			{
				toErrorLog("WARNING (DATABASE::loadBuildOrderFile()): Too many @BO_HEADER blocks defined in file " + build_order_file + " => ignoring new block.");
			}
			else
			{
				bo_header = parseBoHeaderBlock(block);
				if(bo_header == NULL)
				{
					toErrorLog("ERROR (DATABASE::loadBuildOrderFile()): Error parsing build order header in file " + build_order_file + ".");
					return(false);
				}
				found_bo_header_block = true;		
			}
		} else
		if(index=="@GOAL")
		{
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING: (DATABASE::loadBuildOrderFile) No concluding @END for @GOAL block was found in file " + build_order_file + " => trying to parse what we have.");
			}
			if(found_goal_block)
			{
				toErrorLog("WARNING (DATABASE::loadBuildOrderFile()): Too many @GOAL blocks defined in file " + build_order_file + " => ignoring new block.");
			}
			else
			{
				if(!found_bo_header_block)
				{
					toErrorLog("ERROR (DATABASE::loadBuildOrderFile()): [Until I fixed it] the order in build order file " +  build_order_file + " has to be @BO_HEADER, @GOAL, @BUILDORDER.");
					return(false);
				}
				bo_goal = parseGoalBlock(block);
				if(bo_goal == NULL)
				{
					toErrorLog("ERROR: DATABASE::loadBuildOrderFile(): Error parsing goal in file " + build_order_file + ".");
					delete bo_header;
					return(false);
				}
				found_goal_block = true;
			}
		} // end index == GOAL
		else if(index=="@BUILDORDER")
		{
			std::list<std::list<std::string> > block;
			if(!parse_block_list(pFile, block))
			{
				toErrorLog("WARNING (DATABASE::loadBuildOrderFile()): No concluding @END for @BUILDORDER block was found in file " + build_order_file + " => trying to parse what we have.");
			}
			if(found_bo_block)
			{
				toErrorLog("WARNING (DATABASE::loadBuildOrderFile()): Too many @BUILDORDER blocks defined in file " + build_order_file + " => ignoring new block.");
			}
			else
			{
				if(!found_goal_block)
				{
					toErrorLog("ERROR (DATABASE::loadBuildOrderFile()): [Until I fixed it] the order in build order file " +  build_order_file + " has to be @BO_HEADER, @GOAL, @BUILDORDER.");
					if(found_bo_header_block)
						delete bo_header;
					return(false);
				}
				program_list = parseBuildOrderBlock(bo_header->getRace(), block);
				if(program_list == NULL)
				{
					toErrorLog("ERROR (DATABASE::loadBuildOrderFile()): Error parsing build order in file " + build_order_file + ".");
					delete bo_header;
					delete bo_goal;
					return(false);
				}
				found_bo_block = true;
			}		
		} // end index == BUILDORDER
	} // end while
	
	if(found_bo_header_block && found_goal_block && found_bo_block)
	{	
		BUILD_ORDER* build_order = new BUILD_ORDER(*bo_header, *bo_goal, *program_list);
		delete bo_header;
		delete bo_goal;
		delete program_list;
		loadedBuildOrder[build_order->getRace()].push_back(build_order);
		buildOrderDataInitialized = true;
		return(true);
	}
	else
	{
		toErrorLog("ERROR (DATABASE::loadBuildOrderFile()): End of file in file " + build_order_file + " before all blocks (@BO_HEADER, @GOAL and @BUILDORDER) were defined.");
		delete bo_header;
		delete bo_goal;
		delete program_list;
		return(false);
	}
} // schoen :)






const bool DATABASE::loadHarvestFile(const std::string& harvest_file)
{
	if((harvest_file.compare(harvest_file.size()-4, harvest_file.size(), ".hvt")==1))
		return(true);

	std::ifstream pFile(harvest_file.c_str());
	
	if(!checkStreamIsOpen(pFile, "DATABASE::loadHarvestFile", harvest_file))
		return(false);
	
	bool harvest_mode = false;
	unsigned int current_race = MAX_RACES;
	const std::string race_names[MAX_RACES] = {"@TERRA", "@PROTOSS", "@ZERG"};
	HARVEST_SPEED* harvest[MAX_RACES] = {NULL, NULL, NULL};
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "DATABASE::loadHarvestFile", harvest_file))
			return(false);
		
		std::string text = line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		
		size_t stop_position = text.find_first_of("\t ", start_position);
		if(stop_position == std::string::npos) stop_position = text.size();
		std::string index = text.substr(start_position, stop_position);
		
		if(!harvest_mode)
		{
			if(index == "@HARVESTDATA")
				harvest_mode = true;
			else
				toErrorLog("WARNING (DATABASE::loadHarvestFile()): Line '" + index + "' is outside of @HARVESTDATA block in file " + harvest_file + " => line will be ignored.");
		} else
		{
			if(current_race == MAX_RACES)
			{
				if(index == "@END")
					harvest_mode = false;
				else 
				{
					bool race_found = false;
					for(unsigned int i = MAX_RACES; (i--)&&(!race_found);)
						if(index == race_names[i])
						{
							current_race = i;
							race_found = true;
						}
					if(!race_found)
						toErrorLog("WARNING (DATABASE::loadHarvestFile()): Line '" + index + "' is outside of any race block (@TERRA, @PROTOSS or @ZERG) in file " + harvest_file + " => line will be ignored.");
				}
			} else
			{
				std::map<std::string, std::list<std::string> > block;
				pFile.seekg(old_pos);
				if(!parse_block_map(pFile, block))
				{
					toErrorLog("WARNING (DATABASE::loadHarvestFile()): No concluding @END for " + race_names[current_race] + " block was found in file " + harvest_file + " => trying to parse what we have so far.");
				}
			
				if(harvest[current_race])
				{
					toErrorLog("WARNING (DATABASE::loadHarvestFile()): " + race_names[current_race] + " block was already defined in current file " + harvest_file + " => ignoring new block.");
				}
				else
				{
					std::map<std::string, std::list<std::string> >::iterator item;
					for(unsigned int i = MAX_RACES; i--;)
						if((item = block.find(race_names[i])) != block.end())
						{
							toErrorLog("WARNING (DATABASE::loadHarvestFile()): Block " + race_names[i] + " was found in file " + race_names[current_race] + " block in file " + harvest_file + " => ignoring entry and trying to continue.");
							block.erase(item);
						}

					harvest[current_race] = new HARVEST_SPEED;
					if((item = block.find("Mineral harvest")) != block.end())
					{
						unsigned int j = 0;
						for(std::list<std::string>::const_iterator i = item->second.begin(); i != item->second.end(); ++i)
						{
							if(i!= item->second.begin()) // TODO, warum ist item->second.begin() == 'Mineral harvest'?
								harvest[current_race]->setHarvestMineralSpeed(j++, atoi(i->c_str()));
						}
						block.erase(item);
					} else 
					{
						toErrorLog("ERROR (DATABASE::loadHarvestFile()): Field name 'Mineral harvest' not found in file " + race_names[current_race] + " block in file " + harvest_file + ".");
						for(unsigned int i = MAX_RACES; i--;)
							delete harvest[i];
						return(false);
					}
	
					if((item = block.find("Gas harvest")) != block.end())
					{
						unsigned int j = 0;
						for(std::list<std::string>::const_iterator i = item->second.begin(); i != item->second.end(); ++i)
						{
							if(i!= item->second.begin()) // TODO, warum ist item->second.begin() == 'Gas harvest'?
								harvest[current_race]->setHarvestGasSpeed(j++, atoi(i->c_str()));
						}
						block.erase(item);
					} else 
					{
						toErrorLog("ERROR (DATABASE::loadHarvestFile()): Field name 'Gas harvest' not found in file " + race_names[current_race] + " block in file " + harvest_file + ".");
						for(unsigned int i = MAX_RACES; i--;)
							delete harvest[i];
						return(false);
					}
					if(block.size() > 0)
						toErrorLog("WARNING (DATABASE::loadHarvestFile()): Other unknown entries were found in file " + race_names[current_race] + " block in file " + harvest_file + " => ignoring entries.");
				}
				current_race = MAX_RACES;
			} // end current_race != MAX_RACES
		} // end harvest_mode
		old_pos = pFile.tellg();
	} // end while
	
	for(unsigned int i = MAX_RACES; i--;)
		if(!harvest[i])
		{
			toErrorLog("ERROR (loadHarvestFile()): Race block " + race_names[i] + " is missing in file " + harvest_file + ".");
			for(unsigned int j = MAX_RACES; j--;)
				delete harvest[j];
			return(false);
		}
	for(unsigned int i = MAX_RACES; i--;)
		loadedHarvestSpeed[i].push_back(harvest[i]);
	harvestDataInitialized = true;
	return(true);
}
// TODO Vor (zuvielen) uebriggebliebenen Eintraege warnen

const bool DATABASE::loadMapFile(const std::string& map_file)
{
	if((map_file.size()<20)||(map_file.compare(map_file.size()-4, map_file.size(), ".map")==1))
		return(true);
	
	std::ifstream pFile(map_file.c_str());
	if(!checkStreamIsOpen(pFile, "DATABASE::loadMapFile", map_file))
		return(false);

	bool found_map_block = false;
	bool found_location_block[MAX_LOCATIONS];
	for(unsigned int i = MAX_LOCATIONS; i--;)
		found_location_block[i] = false;
	
	BASIC_MAP* basic_map = new BASIC_MAP; // TODO mehrere maps verhindern?
	
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "DATABASE::loadMapFile", map_file))
		{
			delete basic_map;
			return(false);
		}
		
		std::string text = line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		std::list<std::string> words;
		parse_line(text, words);
		if(words.empty())  // ? TODO
			continue; 
		std::list<std::string>::iterator j=words.begin();
		std::string index=*j;++j;
		std::map<std::string, std::list<std::string> >::iterator i;
		
		if(index=="@MAP")
		{
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (DATABASE::loadMapFile()): No concluding @END for @MAP block was found in file " + map_file + " => trying to parse what we have so far.");
			}
			if(found_map_block)
			{
				toErrorLog("WARNING (DATABASE::loadMapFile()): Too many @MAP blocks defined in file " + map_file + " => ignoring new block.");
				
			} else
			{
				if((i=block.find("Name"))!=block.end()){
					i->second.pop_front();
				  	basic_map->setName(i->second.front());
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Name' not found within @MAP block in file " + map_file + ".");
					delete basic_map;
					return(false);				
				}
			
				if((i=block.find("Max locations"))!=block.end()){
					i->second.pop_front();
				   	basic_map->setMaxLocations(atoi(i->second.front().c_str()));
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Max locations' not found within @MAP block in file " + map_file + ".");
					delete basic_map;
					return(false);
				}
			
				if((i=block.find("Max player"))!=block.end()){
					i->second.pop_front();
				   	basic_map->setMaxPlayer(atoi(i->second.front().c_str()));
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Max player' not found within @MAP block in file " + map_file + ".");
					delete basic_map;
					return(false);				
				}
				found_map_block = true;
			}
		}
		else if(index=="@LOCATION")
		{
			if(j==words.end())
			{
				toErrorLog("ERROR (DATABASE::loadMapFile()): @LOCATION entry needs a following number (e.g. '@LOCATION \"2\"') in file " + map_file + ".");
				delete basic_map;
				return(false);
			}
			int location = atoi(j->c_str())-1;
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (DATABASE::loadMapFile()): No concluding @END for @LOCATION block " + *j + " block was found in file " + map_file + " => trying to parse what we have so far.");
			}
			if(location<0)
			{
				toErrorLog("WARNING (DATABASE::loadMapFile()): Number for @LOCATION block " + *j + " is invalid in file " + map_file + " => ignoring block.");
			} else if(!found_map_block)
			{
				toErrorLog("WARNING (DATABASE::loadMapFile()): @MAP block was not yet defined prior to @LOCATION block " + *j + " block in file " + map_file + " => ignoring block.");
			} else if(location > basic_map->getMaxLocations())
			{
				toErrorLog("WARNING (DATABASE::loadMapFile()): Number for @LOCATION block " + *j + " is bigger than what was defined in 'Max locations'  in the @MAP block in file " + map_file + " => ignoring block.");
			} else
			if(found_location_block[location])
			{
				toErrorLog("WARNING (DATABASE::loadMapFile()): @LOCATION block " + *j + " block was already defined in file " + map_file + " => ignoring new block.");
			} else
			{
				if((i=block.find("Name"))!=block.end()) 
				{
					i->second.pop_front();
					basic_map->setLocationName(location, i->second.front().c_str());
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Name' not found within @LOCATION block " + *j + " block in file " + map_file + ".");
					delete basic_map;
					return(false);				
				}
			
				if((i=block.find("Mineral distance"))!=block.end()) 
				{
					i->second.pop_front();
					basic_map->setLocationMineralDistance(location, atoi(i->second.front().c_str()));
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Mineral distance' not found within @LOCATION block " + *j + " block in file " + map_file + ".");
					delete basic_map;
					return(false);				
				}
				
				if((i=block.find("Distance to"))!=block.end()) 
				{
					i->second.pop_front();
					int target = atoi(i->second.front().c_str())-1;
					i->second.pop_front();
					basic_map->setLocationDistance(location, target, atoi(i->second.front().c_str()));
//					cout << location << "->" << target-1 << " : " << atoi(i->second.front().c_str()) << std::endl;
				} else
				{
/*					toErrorLog("ERROR (loadMapFile()): Field name 'Distance to' not found within @LOCATION block " + *j + " block in file " + map_file + ".");
					delete basic_map;
					return(false);*/
					// don't care, distance to is not essential (although it's important...)
				}
				
				if((i=block.find("Minerals"))!=block.end())
				{
					i->second.pop_front();
					basic_map->setLocationMineralPatches(location, atoi(i->second.front().c_str()));
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Minerals' not found within @LOCATION block " + *j + " block in file " + map_file + ".");
					delete basic_map;
					return(false);				
				}
				
				if((i=block.find("Geysirs"))!=block.end())
				{
					i->second.pop_front();
					basic_map->setLocationVespeneGeysirs(location, atoi(i->second.front().c_str()));
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Geysirs' not found within @LOCATION block " + *j + " block in file " + map_file + ".");
					delete basic_map;
					return(false);				
				}
				
				found_location_block[location] = true;
			}
			
		}
	}// END while
	
	if(!found_map_block)
	{
		toErrorLog("ERROR (DATABASE::loadMapFile()): @MAP block was not defined in file " + map_file + ".");
		delete basic_map;
		return(false);
	}
		
	for(unsigned int i = 0; i < basic_map->getMaxLocations(); ++i)
		if(!found_location_block[i])
		{
			std::ostringstream os;
			os << (i+1);
			toErrorLog("ERROR (DATABASE::loadMapFile()): @LOCATION block " + os.str() + " was not defined in file " + map_file + ".");
			delete basic_map;
			return(false);
		}
/*DEBUG	for(unsigned int i = 1; i < basic_map->getMaxLocations(); ++i)
	{	
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < basic_map->getMaxLocations(); ++j)
			std::cout << basic_map->getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}

	basic_map->calculateLocationsDistances();

	for(unsigned int i = 1; i < basic_map->getMaxLocations(); ++i)
	{	
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < basic_map->getMaxLocations(); ++j)
			std::cout << basic_map->getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}*/
	
	loadedMap.push_back(basic_map);
	mapDataInitialized = true;
	return(true);
	// TODO
} // schoen :)

const bool DATABASE::loadStartConditionFile(const std::string& start_condition_file)
{
	if((start_condition_file.compare(start_condition_file.size()-6, start_condition_file.size(), ".start")==1))
		return(true);

	std::ifstream pFile(start_condition_file.c_str());
	if(!checkStreamIsOpen(pFile, "DATABASE::loadStartConditionFile", start_condition_file))
		return(false);
	
	START_CONDITION* startcondition = new START_CONDITION;
	
	bool found_start_conditions_block = false;
	bool found_location_block[MAX_LOCATIONS];
	for(unsigned int i = MAX_LOCATIONS; i--;)
		found_location_block[i] = false;

	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "DATABASE::loadStartConditionFile", start_condition_file))
			return(false);
		std::string text = line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		std::list<std::string> words;
		parse_line(text, words);

		std::list<std::string>::iterator j=words.begin();
		std::string index=*j;++j;
		if(index=="@STARTCONDITIONS")
		{
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (DATABASE::loadStartConditionFile()): No concluding @END for @STARTCONDITIONS block was found in file " + start_condition_file + " => trying to parse what we have so far.");
			}

			if(found_start_conditions_block)
			{
				toErrorLog("WARNING (DATABASE::loadStartConditionFile()): @STARTCONDITIONS block was already defined in file " + start_condition_file + " => ignoring new block.");
			} else
			{
				std::map<std::string, std::list<std::string> >::iterator i;
	
				if((i=block.find("Race"))!=block.end())
				{
					i->second.pop_front();
					std::string estr = i->second.front();
					if(estr == raceString[TERRA]) startcondition->assignRace(TERRA);
					else if(estr == raceString[PROTOSS]) startcondition->assignRace(PROTOSS);
					else if(estr == raceString[ZERG]) startcondition->assignRace(ZERG);
					else {
						toErrorLog("ERROR (DATABASE::loadStartConditionFile()): Invalid race entry (" + estr + " [" + raceString[TERRA] + "|" + raceString[PROTOSS] + "|" + raceString[ZERG] + "]) in file " + start_condition_file + ".");
						delete startcondition;
						return(false);
					}
					block.erase(i);
				} else
				{
					toErrorLog("ERROR (DATABASE::loadStartConditionFile()): Field name 'Race' not found in file " + start_condition_file + ".");
					delete startcondition;
					return(false);
				}
// TODO evtl ueberpruefen ob gueltige Werte...
				if((i=block.find("Name"))!=block.end()) 
				{
					i->second.pop_front();
					startcondition->setName(i->second.front());
					block.erase(i);
				} else
				{
					toErrorLog("ERROR (loadStartConditionFile()): Field name 'Name' not found in file " + start_condition_file + ".");
					delete startcondition;
					return(false);
				}
			
				if((i=block.find("Minerals"))!=block.end()) 
				{
					i->second.pop_front();
					startcondition->setMinerals(100*atoi(i->second.front().c_str()));
					block.erase(i);
				} else
				{
					toErrorLog("ERROR (loadStartConditionFile()): Field name 'Minerals' not found in file " + start_condition_file + ".");
					delete startcondition;
					return(false);
				}
			
				if((i=block.find("Gas"))!=block.end()) 
				{
					i->second.pop_front();
					startcondition->setGas(100*atoi(i->second.front().c_str()));
					block.erase(i);
				} else
				{
					toErrorLog("ERROR (loadStartConditionFile()): Field name 'Gas' not found in file " + start_condition_file + ".");
					delete startcondition;
					return(false);
				}
			
				if((i=block.find("Time"))!=block.end()) 
				{
					i->second.pop_front();
					startcondition->setStartTime(atoi(i->second.front().c_str()));
					block.erase(i);
				} else
				{
					toErrorLog("ERROR (loadStartConditionFile()): Field name 'Time' not found in file " + start_condition_file + ".");
					delete startcondition;
					return(false);
				}
				found_start_conditions_block = true;
			}
		}
		else if(index=="@LOCATION")
		{
			if(j==words.end())
			{
				toErrorLog("ERROR (DATABASE::loadStartConditionFile()): @LOCATION entry needs a following number (e.g. '@LOCATION \"2\"') in file " + start_condition_file + ".");
				delete startcondition;
				return(false);
			}

			signed int location = atoi(j->c_str()) - 1;
			std::list<std::list<std::string> > block;
			if(!parse_block_list(pFile, block))
			{
				toErrorLog("WARNING (DATABASE::loadStartConditionFile()): No concluding @END for @LOCATION " + *j + " block was found in file " + start_condition_file + " => trying to parse what we have so far.");
			}

			if(location < 0)
			{
				toErrorLog("WARNING (DATABASE::loadStartConditionFile()): Number for @LOCATION block " + *j + " is invalid in file " + start_condition_file + " => ignoring block.");
			} else if(found_location_block[location])
			{
				toErrorLog("WARNING (DATABASE::loadStartConditionFile()): @LOCATION block " + *j + " block was already defined in file " + start_condition_file + " => ignoring new block.");
			} else if(!found_start_conditions_block)
			{
				toErrorLog("WARNING (DATABASE::loadStartConditionFile()): @STARTCONDITIONS block was not yet defined prior to @LOCATION block " + *j + " block in file " + start_condition_file + " => ignoring block.");
				
			} else
			{
				bool found_at_least_one = false;
				std::list<std::list<std::string> >::iterator i = block.begin();
				while(i!=block.end())
				{
					bool found = false;
					for(unsigned int unit=LAST_UNIT; (unit--)&&(i!=block.end()) ;)
						if(*(i->begin()) == stats[startcondition->getRace()][unit].name)
						{
							i->pop_front();

							if(i->size() != 1)
							{
								toErrorLog("WARNING (DATABASE::loadStartConditionFile()): Incorrect number of parameters for entry \'" + *(i->begin()) + "\' in file " + start_condition_file + ": 1 parameter expected => line will be ignored.");
							} else
							{
								unsigned int count = atoi(i->front().c_str());
								if(count > 0) {
									startcondition->setLocationTotal(location, unit, count);
									startcondition->setLocationAvailible(location, unit, count);
									found_at_least_one = true;
								} else
								{
									toErrorLog("WARNING (DATABASE::loadStartConditionFile()): Invalid parameter for entry \'" + *(i->begin()) + "\' in file " + start_condition_file + " => line will be ignored.");
								}
							}
							block.erase(i);
							i = block.begin();
							found = true;
						}
					if(!found)
					{
						toErrorLog("WARNING (DATABASE::loadStartConditionFile()): Unknown entry \'" + *(i->begin()) + "\' in file " + start_condition_file + " => line will be ignored.");
						i++;
					}
				} // end while
		
				if(!found_at_least_one)
				{
					toErrorLog("ERROR (DATABASE::loadStartConditionFile()): Empty @LOCATION " + *j + " block in file " + start_condition_file + " => location will be ignored.");
				} else
					found_location_block[location] = true;
			}
		} // end index == @LOCATION
	} // end while

	if(!found_start_conditions_block)
	{
		toErrorLog("WTF?");
		delete startcondition;
		return(false);
	}
	else 
	{
		startcondition->adjustResearches();
		startcondition->adjustSupply();
		loadedStartCondition[startcondition->getRace()].push_back(startcondition);
		startConditionDataInitialized = true;
		for(unsigned int i = MAX_RACES; i--;)
			if(loadedStartCondition[i].size() == 0)
			{
				startConditionDataInitialized = false;
				break;
			}		
	}
	return(true);
} // schoen :)

// ---------------------------------
// ------ END OF FILE LOADING ------
// ---------------------------------

// FILE SAVING

const bool DATABASE::saveGoal(const std::string& goal_name, GOAL_ENTRY* goalentry)
{
	std::ostringstream goal_file;
	goal_file.str("");
#ifdef __linux__
	goal_file << "settings/goals/";
	goal_file << raceString[goalentry->getRace()] << "/" << goal_name << ".gol";// TODO!
#elif __WIN32__
	goal_file << "settings\\goals\\";
	goal_file << raceString[goalentry->getRace()] << "\\" << goal_name << ".gol";// TODO!
#endif 
	std::ofstream pFile(goal_file.str().c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!checkStreamIsOpen(pFile, "DATABASE::saveGoal", goal_file.str()))
		return(false);

	goalentry->setName(goal_name);

	pFile << "@GOAL" << std::endl;
	pFile << "		\"Name\" \"" << goal_name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << raceString[goalentry->getRace()] << "\"" << std::endl;

	for(std::list<GOAL>::const_iterator i = goalentry->goal.begin(); i!=goalentry->goal.end(); ++i)
		pFile << "		\"" << stats[goalentry->getRace()][i->getUnit()].name << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	pFile << "@END" << std::endl;

	if(!loadGoalFile(goal_file.str()))
	{
		toErrorLog("ERROR (DATABASE::saveGoal()): Could not reload file " + goal_file.str() + ". Goal is probably lost. This is either a BUG or the OS messed something up.");
		return(false);
	}
	return(true);
}

const bool DATABASE::saveBuildOrder(const std::string& build_order_name, BUILD_ORDER& build_order)
{
	std::ostringstream build_order_file;
	build_order_file.str("");
#ifdef __linux__
	build_order_file << "output/bos/";
	build_order_file << raceString[build_order.getRace()] << "/" << build_order_name << ".txt";
#elif __WIN32__
	build_order_file << "output\\bos\\";
	build_order_file << raceString[build_order.getRace()] << "\\" << build_order_name << ".txt";
#endif 
	std::ofstream pFile(build_order_file.str().c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!checkStreamIsOpen(pFile, "DATABASE::saveBuildOrder", build_order_file.str()))
		return(false);

	pFile << "@BO_HEADER" << std::endl;
	pFile << "		\"Name\" \"" << build_order_name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << raceString[build_order.getRace()] << "\"" << std::endl;
	pFile << "		\"Time\" \"" << build_order.getTime() << "\"" << std::endl;
	pFile << "@END" << std::endl;
	
	
	pFile << "@GOAL" << std::endl;
	pFile << "		\"Name\" \"" << build_order_name << "\"" << std::endl; // TODO! Name muesste eigentlich der echte Goal Name sein!
	pFile << "		\"Race\" \"" << raceString[build_order.getGoal().getRace()] << "\"" << std::endl;

	for(std::list<GOAL>::const_iterator i = build_order.getGoal().goal.begin(); i!= build_order.getGoal().goal.end(); ++i)
		pFile << "		\"" << stats[build_order.getGoal().getRace()][i->getUnit()].name << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	pFile << "@END" << std::endl;


	pFile << "@BUILDORDER" << std::endl;
	for(std::list<PROGRAM>::iterator i = build_order.getProgramList().begin(); i!=build_order.getProgramList().end(); ++i)
		pFile << "		\"" << stats[build_order.getRace()][i->getUnit()].name << "\""/* \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\""*/ << std::endl;
	pFile << "@END" << std::endl;
	
	if(!loadBuildOrderFile(build_order_file.str()))
	{
		toErrorLog("ERROR (DATABASE::saveBuildOrder()): Could not reload file " + build_order_file.str() + ". Build order is probably lost. This is either a BUG or the OS messed something up.");
		return(false);
	}
	return(true);

/*	std::ostringstream os;
	os.str("");
#ifdef __linux__
	os << "output/bos/";
	os << raceString[build_order.getRace()] << "/" << name << ".html";
#elif __WIN32__
	os << "output\\bos\\";
	os << raceString[build_order.getRace()] << "\\" << name << ".html";
#endif
	std::ofstream pFile(os.str().c_str(), std::ios_base::out | std::ios_base::trunc);
	
	if(!pFile.is_open())
	{
		toErrorLog("ERROR: (DATABASE::saveBuildOrder) Could not create file " + os.str() + " (write protection? disk space?)");
		return;
	}
	
	pFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << std::endl;
	pFile << "<html>" << std::endl;
	pFile << "<head>" << std::endl;
	pFile << "  <meta content=\"text/html; charset=ISO-8859-1\"" << std::endl;
	pFile << " http-equiv=\"content-type\">" << std::endl;
	pFile << "  <title>Build order list</title>" << std::endl;
	pFile << "</head>" << std::endl;
	pFile << "<body alink=\"#000099\" vlink=\"#990099\" link=\"#000099\" style=\"color: rgb("<< (int)UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR)->r() << ", " << (int)UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR)->g() << ", " << (int)UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR)->b() << "); background-color: rgb(" << (int)UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()->r() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()->g() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()->b() << ");\">" << std::endl;
	pFile << "<div style=\"text-align: center;\"><big style=\"font-weight: bold;\"><big>Evolution Forge " << CORE_VERSION << "</big></big><br><br>" << std::endl;
	pFile << "<big>Buildorder list " << name << "</big><br>" << std::endl;
	pFile << "</div>" << std::endl;
	pFile << "<br>" << std::endl;
	pFile << "<table style=\"background-color: rgb(" << (int)UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH)->GetColor()->r() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH)->GetColor()->g() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH)->GetColor()->b() << "); text-align: center; vertical-align: middle; width: 600px; margin-left: auto; margin-right: auto;\""<< std::endl;
	pFile << " border=\"1\" cellspacing=\"0\" cellpadding=\"1\">" << std::endl;
	pFile << "  <tbody>" << std::endl;
	pFile << "	<tr>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 200px;\">" << UI_Object::theme.lookUpString(OUTPUT_UNITNAME_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << UI_Object::theme.lookUpString(OUTPUT_SUPPLY_STRING) << "</td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << UI_Object::theme.lookUpString(OUTPUT_MINERALS_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << UI_Object::theme.lookUpString(OUTPUT_GAS_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
//	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 100px;\">" << UI_Object::theme.lookUpString(OUTPUT_LOCATION_STRING) << "<br>" << std::endl;
//	pFile << "	  </td>" << std::endl; TODO
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << UI_Object::theme.lookUpString(OUTPUT_TIME_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	</tr>" << std::endl;

	for(std::list<PROGRAM>::const_iterator order = build_order.getProgramList().begin(); order != build_order.getProgramList().end(); ++order)
	{
		pFile << "	<tr style=\"text-align: center; vertical-align: middle; background-color: rgb(" << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[build_order.getRace()][order->getUnit()].unitType))->GetColor()->r() << ", " << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[build_order.getRace()][order->getUnit()].unitType))->GetColor()->g() << ", " << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[build_order.getRace()][order->getUnit()].unitType))->GetColor()->b() << ");\">" << std::endl;
		pFile << "	  <td style=\"\">" << UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*build_order.getRace() + order->getUnit() + UNIT_NULL_STRING)) << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;
		pFile << "	  <td style=\"\">" << order->getStatisticsBefore().getNeedSupply() << "/" << order->getStatisticsBefore().getHaveSupply() << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;

		pFile << "	  <td style=\"\">" << order->getStatisticsBefore().getHaveMinerals()/100 << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;

		pFile << "	  <td style=\"\">" << order->getStatisticsBefore().getHaveGas()/100 << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;

//		pFile << "	  <td style=\"\">" << (*build_order.getMap())->getLocation(order->getLocation())->getName() << "<br>" << std::endl;
//		pFile << "	  </td>" << std::endl; TODO

		pFile << "	  <td style=\"\">" << formatTime(order->getRealTime()) << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;
																								   
		pFile << "	</tr>" << std::endl;
	}

	pFile << "  </tbody>" << std::endl;
	pFile << "</table>" << std::endl;
	pFile << "<br>" << std::endl;
	pFile << "<b><a href=\"http://www.clawsoftware.de\">www.clawsoftware.de</a></b>\n";
	pFile << "</body>\n";
	pFile << "</html>" << std::endl;*/
}

const unsigned int DATABASE::getBuildOrderCount(const eRace race, const GOAL_ENTRY* goal) const 
{
	unsigned int count = 0;
	for(unsigned int i = loadedBuildOrder[race].size(); i--;)
	{
		if(loadedBuildOrder[race][i]->getGoal() == *goal)
			count++;
	}
	return(count);
}


BUILD_ORDER* DATABASE::getBuildOrder(const eRace race, const GOAL_ENTRY* goal, const unsigned int build_order) const
{
#ifdef _SCC_DEBUG
	if(build_order >= getBuildOrderCount(race, goal)) {
		toErrorLog("DEBUG WARNING (DATABASE::getBuildOrder()): Value 'build_order' out of range.");return(NULL);
	}
#endif
	unsigned int count = 0;
	for(unsigned int i = 0; i < loadedBuildOrder[race].size(); i++)
	{
		if(!(loadedBuildOrder[race][i]->getGoal() == *goal))
			continue;
		if(count == build_order)
			return(loadedBuildOrder[race][i]);
		count++;
	}
	return(NULL);
}


// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

DATABASE database;
