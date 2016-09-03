#include "files.hpp"
#include "../ui/object.hpp"
#include <sstream>
#include <fstream>
// ---------------------------
// ------- FILE LOADING ------
// ---------------------------


const unsigned int parseRace(const std::string& race_name)
{
	unsigned int race = GAME::MAX_RACES;
	for(unsigned int j = GAME::MAX_RACES; j--;)
		if(race_name == GAME::race[j].raceString)
			race = j;
	if(race == GAME::MAX_RACES)
	{
		std::ostringstream os;
		os << "ERROR (parseRace()): Invalid race entry (" + race_name + " [";
		if(GAME::MAX_RACES>0)
			os << GAME::race[0].raceString;
		for(unsigned int j = 1; j < GAME::MAX_RACES; ++j)
			os << " | " << GAME::race[j].raceString;
		os << "]).";
		toErrorLog(os.str());
		return(GAME::MAX_RACES);
	}
	return(race);
}

GOAL_ENTRY* parseGoalBlock(std::map<std::string, std::list<std::string> > block, const std::string& file_name)
{
	std::map<std::string, std::list<std::string> >::iterator i;

	unsigned int goal_race;
	std::string goal_name;
	
	if((i=block.find("Name"))!=block.end())
	{
		i->second.pop_front();
		goal_name = i->second.front();
		block.erase(i);		
	} else
	{
		toErrorLog("ERROR (parseGoalBlock()): Field name 'Name' not found.");
		return(NULL);
	}

	if((i=block.find("Race"))!=block.end())
	{
		i->second.pop_front();
	
		std::string estr = i->second.front();
		goal_race = parseRace(estr);
		if(goal_race == GAME::MAX_RACES)
			return(NULL);
		block.erase(i);
	} else
	{
		toErrorLog("ERROR (parseGoalBlock()): Field name 'Race' not found.");
		return(NULL);
	}
	
	GOAL_ENTRY* goal = new GOAL_ENTRY(goal_name, goal_race);

	bool found_at_least_one = false;
	i = block.begin();
	unsigned int k = 0;
	while(i!=block.end())
	{
		bool found = false;
		
		for(unsigned int language = MAX_LANGUAGES; (!found)&&(language--);)
			if(GAME::race[goal->getRace()].languageInitializedForUnitsStrings[language])
				for(unsigned int unit = RACE::UNIT_TYPE_COUNT; (!found)&&(unit--);)
					if(*(i->second.begin()) == GAME::race[goal->getRace()].unitsStringList[language][unit])
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
							toErrorLog("WARNING (parseGoalBlock()): Incorrect number of parameters for entry in goal list (\'" + *(i->second.begin()) + "\'): 3 parameters expected => line will be ignored in file " + file_name +".");
							block.erase(i);
							i = block.begin();
							found = true;
						}
					} else 
					{
						std::string bla1 = *(i->second.begin());
						std::string bla2 = GAME::race[goal->getRace()].unitsStringList[language][unit];
//						toErrorLog(bla1 + " =/= " + bla2);
//						std::ostringstream os;
//						os << language << " " << unit << " " << k;
//						toErrorLog(os.str());
					}
					if(!found)
					{
						toErrorLog("WARNING (parseGoalBlock()): Unknown entry in goal list (\'" + *(i->second.begin()) + "\') => line will be ignored in file "+ file_name +".");
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

BO_HEADER* parseBoHeaderBlock(std::map<std::string, std::list<std::string> > block, const std::string& file_name)
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
		toErrorLog("ERROR (parseBoHeaderBlock()): Field name 'Name' not found in file "+ file_name +".");
		delete bo_header;
		return(NULL);
	}

	if((i=block.find("Race"))!=block.end()){
		i->second.pop_front();
	
		std::string estr = i->second.front();
		unsigned int bo_race = parseRace(estr);
		if(bo_race == GAME::MAX_RACES)
			return(NULL);
		bo_header->setRace(bo_race);
		block.erase(i);
	} else
	{
		toErrorLog("ERROR (parseBoHeaderBlock()): Field name 'Race' not found in file "+ file_name +".");
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
		toErrorLog("ERROR (parseBoHeaderBlock()): Field name 'Time' not found in file "+ file_name +".");
		delete bo_header;
		return(NULL);
	}
	return(bo_header);
}

std::list<PROGRAM>* parseBuildOrderBlock(const unsigned int bo_race, std::list<std::list<std::string> > block, const std::string& file_name)
{
	// TODO language!!
	bool found_at_least_one = false;
	std::list<PROGRAM>* program_list = new std::list<PROGRAM>;
	for(std::list<std::list<std::string> >::iterator i = block.begin(); i!=block.end(); ++i)
	{
		bool found = false;
		for(unsigned int language = MAX_LANGUAGES; language--;)
			if(GAME::race[bo_race].languageInitializedForUnitsStrings[language])
		for(unsigned int unit = RACE::UNIT_TYPE_COUNT; unit--;)
			if(*(i->begin()) == GAME::race[bo_race].unitsStringList[language][unit])
			{
				PROGRAM p;
				p.setUnit(unit);
				program_list->push_back(p);
				found = true;
				found_at_least_one = true;
				break;
			}
		if(!found)
			toErrorLog("WARNING (parseBuildOrderBlock()): Unkown entry in build order list (\'" + *(i->begin()) + "\') => ignoring entry in file " + file_name +".");
	}
	
	if(!found_at_least_one)
	{
		toErrorLog("ERROR (parseBuildOrderBlock()): Empty build order list => file will be ignored.");
		delete program_list;
		return(NULL);
	} else
		return(program_list);
}

const bool FILES::loadStartConditionFile(const std::string& start_condition_file)
{
	if((start_condition_file.compare(start_condition_file.size()-6, start_condition_file.size(), ".start")==1))
		return(true);

	std::ifstream pFile(start_condition_file.c_str());
	if(!checkStreamIsOpen(pFile, "FILES::loadStartConditionFile", start_condition_file))
		return(false);
	
	START_CONDITION* startcondition = new START_CONDITION;
	
	bool found_start_conditions_block = false;
	bool found_location_block[MAX_LOCATIONS];
	for(unsigned int i = MAX_LOCATIONS; i--;)
		found_location_block[i] = false;

	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "FILES::loadStartConditionFile", start_condition_file))
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
				toErrorLog("WARNING (FILES::loadStartConditionFile()): No concluding @END for @STARTCONDITIONS block was found in file " + start_condition_file + " => trying to parse what we have so far.");
			}

			if(found_start_conditions_block)
			{
				toErrorLog("WARNING (FILES::loadStartConditionFile()): @STARTCONDITIONS block was already defined in file " + start_condition_file + " => ignoring new block.");
			} else
			{
				std::map<std::string, std::list<std::string> >::iterator i;
	

				if((i=block.find("Race"))!=block.end())
				{
					i->second.pop_front();
					std::string estr = i->second.front();
					unsigned int start_race = parseRace(estr);
					if(start_race == GAME::MAX_RACES)
					{
						delete startcondition;
						return(false);
					}
					startcondition->assignRace(start_race);
					block.erase(i);
				} else
				{
					toErrorLog("ERROR (FILES::loadStartConditionFile()): Field name 'Race' not found in file " + start_condition_file + ".");
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
			
				for(unsigned int res = RACE::MAX_RESOURCE_TYPES; res--;)
				{
					bool res_found = false;
					for(unsigned int language = MAX_LANGUAGES; language--;)
						if(GAME::languageInitializedForGameStrings[language])
							if((i=block.find(GAME::gameStringList[language][GAME::FIRST_RESOURCE_STRING + res]))!=block.end()) 
							{
								i->second.pop_front();
								startcondition->setResource(res, 100*atoi(i->second.front().c_str()));
								block.erase(i);
								res_found = true;
								break;
							}
					if(!res_found)
					{
						toErrorLog("ERROR (loadStartConditionFile()): Field name 'Minerals' not found in file " + start_condition_file + "."); // TODO
						delete startcondition;
						return(false);
					}
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
				toErrorLog("ERROR (FILES::loadStartConditionFile()): @LOCATION entry needs a following number (e.g. '@LOCATION \"2\"') in file " + start_condition_file + ".");
				delete startcondition;
				return(false);
			}

			signed int location = atoi(j->c_str()) - 1;
			std::list<std::list<std::string> > block;
			if(!parse_block_list(pFile, block))
			{
				toErrorLog("WARNING (FILES::loadStartConditionFile()): No concluding @END for @LOCATION " + *j + " block was found in file " + start_condition_file + " => trying to parse what we have so far.");
			}

			if(location < 0)
			{
				toErrorLog("WARNING (FILES::loadStartConditionFile()): Number for @LOCATION block " + *j + " is invalid in file " + start_condition_file + " => ignoring block.");
			} else if(found_location_block[location])
			{
				toErrorLog("WARNING (FILES::loadStartConditionFile()): @LOCATION block " + *j + " block was already defined in file " + start_condition_file + " => ignoring new block.");
			} else if(!found_start_conditions_block)
			{
				toErrorLog("WARNING (FILES::loadStartConditionFile()): @STARTCONDITIONS block was not yet defined prior to @LOCATION block " + *j + " block in file " + start_condition_file + " => ignoring block.");
				
			} else
			{
				bool found_at_least_one = false;
				std::list<std::list<std::string> >::iterator i = block.begin();
				while(i!=block.end())
				{
					bool found = false;
		for(unsigned int language = MAX_LANGUAGES; (!found)&&(language--);)
			if(GAME::race[startcondition->getRace()].languageInitializedForUnitsStrings[language])
				for(unsigned int unit = RACE::UNIT_TYPE_COUNT; (!found)&&(unit--)&&(i!=block.end());)
				{
					if(*(i->begin()) == GAME::race[startcondition->getRace()].unitsStringList[language][unit])
					{
						i->pop_front();

						if(i->size() != 1)
						{
							toErrorLog("WARNING (FILES::loadStartConditionFile()): Incorrect number of parameters for entry \'" + *(i->begin()) + "\' in file " + start_condition_file + ": 1 parameter expected => line will be ignored.");
						} else
						{
							unsigned int count = atoi(i->front().c_str());
							if(count > 0) {
								startcondition->setLocationTotal(location, unit, count);
								startcondition->setLocationAvailible(location, unit, count);
								found_at_least_one = true;
							} else
							{
								toErrorLog("WARNING (FILES::loadStartConditionFile()): Invalid parameter for entry \'" + *(i->begin()) + "\' in file " + start_condition_file + " => line will be ignored.");
							}
						}
						block.erase(i);
						i = block.begin();
						found = true;
					}
				}
		
					if(!found)
					{
						toErrorLog("WARNING (FILES::loadStartConditionFile()): Unknown entry \'" + *(i->begin()) + "\' in file " + start_condition_file + " => line will be ignored.");
						i++;
					}
				} // end while
		
				if(!found_at_least_one)
				{
					toErrorLog("ERROR (FILES::loadStartConditionFile()): Empty @LOCATION " + *j + " block in file " + start_condition_file + " => location will be ignored.");
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
		database.addStartCondition(startcondition);

	return(true);
} // schoen :)


const bool FILES::loadMapFile(const std::string& map_file)
{
	if((map_file.size()<20)||(map_file.compare(map_file.size()-4, map_file.size(), ".map")==1))
		return(true);
	
	std::ifstream pFile(map_file.c_str());
	if(!checkStreamIsOpen(pFile, "FILES::loadMapFile", map_file))
		return(false);

	bool found_map_block = false;
	bool found_location_block[MAX_LOCATIONS];
	for(unsigned int i = MAX_LOCATIONS; i--;)
		found_location_block[i] = false;
	
	BASIC_MAP* basic_map = new BASIC_MAP; // TODO mehrere maps verhindern?
	
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "FILES::loadMapFile", map_file))
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
				toErrorLog("WARNING (FILES::loadMapFile()): No concluding @END for @MAP block was found in file " + map_file + " => trying to parse what we have so far.");
			}
			if(found_map_block)
			{
				toErrorLog("WARNING (FILES::loadMapFile()): Too many @MAP blocks defined in file " + map_file + " => ignoring new block.");
				
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
				   	if(!basic_map->setMaxPlayer(atoi(i->second.front().c_str())))
					{
						delete basic_map;
						toErrorLog("ERROR (loadMapFile()): Field name 'Max player' out of range in file " + map_file + " => ignoring file.");
						return(false);
					}
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
				toErrorLog("ERROR (FILES::loadMapFile()): @LOCATION entry needs a following number (e.g. '@LOCATION \"2\"') in file " + map_file + ".");
				delete basic_map;
				return(false);
			}
			int location = atoi(j->c_str())-1;
			std::map<std::string, std::list<std::string> > block;
			if(!parse_block_map(pFile, block))
			{
				toErrorLog("WARNING (FILES::loadMapFile()): No concluding @END for @LOCATION block " + *j + " block was found in file " + map_file + " => trying to parse what we have so far.");
			}
			if(location<0)
			{
				toErrorLog("WARNING (FILES::loadMapFile()): Number for @LOCATION block " + *j + " is invalid in file " + map_file + " => ignoring block.");
			} else if(!found_map_block)
			{
				toErrorLog("WARNING (FILES::loadMapFile()): @MAP block was not yet defined prior to @LOCATION block " + *j + " block in file " + map_file + " => ignoring block.");
			} else if(location > basic_map->getMaxLocations())
			{
				toErrorLog("WARNING (FILES::loadMapFile()): Number for @LOCATION block " + *j + " is bigger than what was defined in 'Max locations'  in the @MAP block in file " + map_file + " => ignoring block.");
			} else
			if(found_location_block[location])
			{
				toErrorLog("WARNING (FILES::loadMapFile()): @LOCATION block " + *j + " block was already defined in file " + map_file + " => ignoring new block.");
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
			
				/*if((i=block.find("Mineral distance"))!=block.end()) 
				{
					i->second.pop_front();
					basic_map->setLocationMineralDistance(location, atoi(i->second.front().c_str()));
				} else
				{
					toErrorLog("ERROR (loadMapFile()): Field name 'Mineral distance' not found within @LOCATION block " + *j + " block in file " + map_file + ".");
					delete basic_map;
					return(false);				
				}*/ // TODO
				
				if((i=block.find("Distance to"))!=block.end()) 
				{
					i->second.pop_front();
					int target = atoi(i->second.front().c_str())-1;
					i->second.pop_front();
					basic_map->setLocationDistance(location, target, atoi(i->second.front().c_str()));
					std::ostringstream os;
					os << location << "->" << target-1 << " : " << atoi(i->second.front().c_str());
					toErrorLog(os.str());
				} else
				{
/*					toErrorLog("ERROR (loadMapFile()): Field name 'Distance to' not found within @LOCATION block " + *j + " block in file " + map_file + ".");
					delete basic_map;
					return(false);*/
					// don't care, distance to is not essential (although it's important...)
				}

//				- MINERAL_PATCH etc. nur einmal
//				- Alle Einheiten nur einmal, eine lange Liste
//				-> problem: unit_type_count
//				- Optimierung: Liste pro Location noch machen => schnellerer Durchlauf!
//				- availibleGoals machen -> alle goals die auf Basis der Starteinheiten gebaut werden koennen
//				- haveable wieder rein (start einheiten!)

				
				for(unsigned int k = 0; k < RACE::MAX_RESOURCE_TYPES; ++k)
				{
					bool found = false;
					for(unsigned int l = 1; l < MAX_LANGUAGES; ++l)
					{
						if(GAME::race[0].languageInitializedForUnitsStrings[l])
						{
							unsigned int item;
							if(GAME::race[0].stats[GAME::race[0].resource[k].provider].facility2 > 0) // TODO... Refinery <> Vespene Geysir...
								item = GAME::race[0].stats[GAME::race[0].resource[k].provider].facility2;
							else item = GAME::race[0].resource[k].provider;
							if((i = block.find(GAME::race[0].unitsStringList[l][item])) != block.end())
							{
								i->second.pop_front();
								basic_map->setLocationUnitTotal(location, item, atoi(i->second.front().c_str())); // TODO
								found = true;
								break;
							}
						}
					}
					if(!found)
					{
						toErrorLog("ERROR (loadMapFile()): Field name for resource not found within @LOCATION block " + *j + " block in file " + map_file + ".");
						delete basic_map;
						return(false);				
					}
				}
				
				found_location_block[location] = true;
			}
			
		}
	}// END while
	
	if(!found_map_block)
	{
		toErrorLog("ERROR (FILES::loadMapFile()): @MAP block was not defined in file " + map_file + ".");
		delete basic_map;
		return(false);
	}
		
	for(unsigned int i = 0; i < basic_map->getMaxLocations(); ++i)
		if(!found_location_block[i])
		{
			std::ostringstream os;
			os << (i+1);
			toErrorLog("ERROR (FILES::loadMapFile()): @LOCATION block " + os.str() + " was not defined in file " + map_file + ".");
			delete basic_map;
			return(false);
		}
	
	std::ostringstream os;
	os << "--------\n";
	for(unsigned int i = 0; i < basic_map->getMaxLocations(); ++i)
	{	
		os << "Location " << i << " ";
		for(unsigned int j = 0; j < basic_map->getMaxLocations(); ++j)
			os << basic_map->getLocation(i)->getDistance(j) << " ";
		os << "\n";
	}
	basic_map->calculateLocationDistances();
	for(unsigned int i = 0; i < basic_map->getMaxLocations(); ++i)
	{	
		os << "Location " << i << " ";
		for(unsigned int j = 0; j < basic_map->getMaxLocations(); ++j)
			os << basic_map->getLocation(i)->getDistance(j) << " ";
		os << "\n";
	}
	os << "----------\n";
	toErrorLog(os.str());
	
	database.addMap(basic_map);
	return(true);
	// TODO
} // schoen :)

const bool FILES::loadBuildOrderFile(const std::string& build_order_file)
{
// just ignore the file if it's no .txt file... 
	if((build_order_file.compare(build_order_file.size()-4, build_order_file.size(), ".txt")==1))
		return(true);

	std::ifstream pFile(build_order_file.c_str());
	if(!checkStreamIsOpen(pFile, "FILES::loadBuildOrderFile", build_order_file))
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
		if(!checkStreamForFailure(pFile, "FILES::loadBuildOrderFile", build_order_file))
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
				toErrorLog("WARNING (FILES::loadBuildOrderFile()): No concluding @END for @BO_HEADER block was found in file " + build_order_file + " => trying to parse what we have.");
			}
			if(found_bo_header_block)
			{
				toErrorLog("WARNING (FILES::loadBuildOrderFile()): Too many @BO_HEADER blocks defined in file " + build_order_file + " => ignoring new block.");
			}
			else
			{
				bo_header = parseBoHeaderBlock(block, build_order_file);
				if(bo_header == NULL)
				{
					toErrorLog("ERROR (FILES::loadBuildOrderFile()): Error parsing build order header in file " + build_order_file + ".");
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
				toErrorLog("WARNING: (FILES::loadBuildOrderFile) No concluding @END for @GOAL block was found in file " + build_order_file + " => trying to parse what we have.");
			}
			if(found_goal_block)
			{
				toErrorLog("WARNING (FILES::loadBuildOrderFile()): Too many @GOAL blocks defined in file " + build_order_file + " => ignoring new block.");
			}
			else
			{
				if(!found_bo_header_block)
				{
					toErrorLog("ERROR (FILES::loadBuildOrderFile()): [Until I fixed it] the order in build order file " +  build_order_file + " has to be @BO_HEADER, @GOAL, @BUILDORDER.");
					return(false);
				}
				bo_goal = parseGoalBlock(block, build_order_file);
				if(bo_goal == NULL)
				{
					toErrorLog("ERROR: FILES::loadBuildOrderFile(): Error parsing goal in file " + build_order_file + ".");
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
				toErrorLog("WARNING (FILES::loadBuildOrderFile()): No concluding @END for @BUILDORDER block was found in file " + build_order_file + " => trying to parse what we have.");
			}
			if(found_bo_block)
			{
				toErrorLog("WARNING (FILES::loadBuildOrderFile()): Too many @BUILDORDER blocks defined in file " + build_order_file + " => ignoring new block.");
			}
			else
			{
				if(!found_goal_block)
				{
					toErrorLog("ERROR (FILES::loadBuildOrderFile()): [Until I fixed it] the order in build order file " +  build_order_file + " has to be @BO_HEADER, @GOAL, @BUILDORDER.");
					if(found_bo_header_block)
						delete bo_header;
					return(false);
				}
				program_list = parseBuildOrderBlock(bo_header->getRace(), block, build_order_file);
				if(program_list == NULL)
				{
					toErrorLog("ERROR (FILES::loadBuildOrderFile()): Error parsing build order in file " + build_order_file + ".");
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
		database.addBuildOrder(build_order);
		return(true);
	}
	else
	{
		toErrorLog("ERROR (FILES::loadBuildOrderFile()): End of file in file " + build_order_file + " before all blocks (@BO_HEADER, @GOAL and @BUILDORDER) were defined.");
		delete bo_header;
		delete bo_goal;
		delete program_list;
		return(false);
	}
} // schoen :)

const bool FILES::loadGoalFile(const std::string& goal_file)
{
	if((goal_file.compare(goal_file.size()-4, goal_file.size(), ".gol")==1))
		return(true);

	std::ifstream pFile(goal_file.c_str());
	if(!checkStreamIsOpen(pFile, "FILES::loadGoalFile", goal_file))
		return(false);
	
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "FILES::loadGoalFile", goal_file))
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
				toErrorLog("WARNING (FILES::loadGoalFile()): No concluding @END for @GOAL block was found in file " + goal_file + " => trying to parse what we have so far.");
			}
			GOAL_ENTRY* my_goal = parseGoalBlock(block, goal_file);
			if(my_goal == NULL)
			{
				toErrorLog("ERROR (FILES::loadGoalFile()): Error parsing " + goal_file + ".");
				return(false);
			}
			else 
			{
				database.addGoal(my_goal);
				return(true);
			}
		} // end index == GOAL
	} // end while
	toErrorLog("ERROR (FILES::loadGoalFile()): No @GOAL block was found in file " + goal_file + ".");
	return(false);	
} // schoen :) naja :o

const bool FILES::loadHarvestFile(const std::string& harvest_file)
{
	if((harvest_file.compare(harvest_file.size()-4, harvest_file.size(), ".hvt")==1))
		return(true);

	std::ifstream pFile(harvest_file.c_str());
	
	if(!checkStreamIsOpen(pFile, "FILES::loadHarvestFile", harvest_file))
		return(false);
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024]; // evtl erhoehen
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "FILES::loadHarvestFile", harvest_file))
			return(false);
		
		std::string text = line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		
		std::list<std::string> words;

		size_t stop_position = text.find_first_of("\t ", start_position);
		if(stop_position == std::string::npos) stop_position = text.size();
		std::string index = text.substr(start_position, stop_position);

		if(index == "@HARVESTDATA")
		{
			std::list<std::list<std::string> > block;
			if(!parse_block_list(pFile, block))
			{
				toErrorLog("WARNING (FILES::loadHarvestFile()): No concluding @END was found in file " + harvest_file + " => trying to parse what we have so far.");
			}
			std::list<std::list<std::string> >::iterator i = block.begin();
			for(unsigned int j = 0; j < GAME::MAX_RACES; ++j)
				for(unsigned int k = 0; k < RACE::MAX_RESOURCE_TYPES; ++k)
				{
					GAME::race[j].resource[k].resourceHarvestPerFrame.resize(RESOURCE::MAX_RESOURCE_DISTANCE);
					for(unsigned int l = 0; l < RESOURCE::MAX_RESOURCE_DISTANCE; ++l)
					{
						GAME::race[j].resource[k].resourceHarvestPerFrame[l].resize(RESOURCE::MAX_RESOURCE_PROVIDER_PER_LOCATION);
						for(unsigned int m = 0; m < RESOURCE::MAX_RESOURCE_PROVIDER_PER_LOCATION; ++m, ++i)
						{
							std::list<std::string>::const_iterator o = i->begin();
							GAME::race[j].resource[k].resourceHarvestPerFrame[l][m].resize(RESOURCE::MAX_RESOURCE_WORKERS);
							for(unsigned int n = 0; n < RESOURCE::MAX_RESOURCE_WORKERS; ++n, ++o)
								GAME::race[j].resource[k].resourceHarvestPerFrame[l][m][n] = atoi(o->c_str());
						}
					}
				}
		}
	}
	return(true);
}

const bool FILES::saveHarvestFile(const std::string& harvest_file)
{
	std::ofstream pFile(harvest_file.c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!checkStreamIsOpen(pFile, "FILES::saveHarvestFile", harvest_file))
		return(false);
	pFile << "@HARVESTDATA" << std::endl;
	for(unsigned int j = 0; j < GAME::MAX_RACES; ++j)
		for(unsigned int k = 0; k < RACE::MAX_RESOURCE_TYPES; ++k)
			for(unsigned int l = 0; l < RESOURCE::MAX_RESOURCE_DISTANCE; ++l)
				for(unsigned int m = 0; m < RESOURCE::MAX_RESOURCE_PROVIDER_PER_LOCATION; ++m)
				{
					std::ostringstream os;
					for(unsigned int n = 0; n < RESOURCE::MAX_RESOURCE_WORKERS; ++n)
						pFile << GAME::race[j].resource[k].resourceHarvestPerFrame[l][m][n] << " ";
					pFile << std::endl;
				}
	pFile << "@END" << std::endl;
	
	if(!loadHarvestFile(harvest_file))
	{
		toErrorLog("ERROR (FILES::saveHarvestFile()): Could not reload file " + harvest_file + ". Harvest data is probably lost. This is either a BUG or the OS messed something up.");
		return(false);
	}
	return(true);
}

#if 0
const bool FILES::loadHarvestFile(const std::string& harvest_file)
{
	if((harvest_file.compare(harvest_file.size()-4, harvest_file.size(), ".hvt")==1))
		return(true);

	std::ifstream pFile(harvest_file.c_str());
	
	if(!checkStreamIsOpen(pFile, "FILES::loadHarvestFile", harvest_file))
		return(false);
	
	bool harvest_mode = false;
	unsigned int current_race = MAX_RACES;
	const std::string race_names[MAX_RACES] = {"@TERRA", "@PROTOSS", "@ZERG"};
	HARVEST_SPEED* harvest[MAX_RACES] = {NULL, NULL, NULL};
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		if(!checkStreamForFailure(pFile, "FILES::loadHarvestFile", harvest_file))
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
				toErrorLog("WARNING (FILES::loadHarvestFile()): Line '" + index + "' is outside of @HARVESTDATA block in file " + harvest_file + " => line will be ignored.");
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
						toErrorLog("WARNING (FILES::loadHarvestFile()): Line '" + index + "' is outside of any race block (@TERRA, @PROTOSS or @ZERG) in file " + harvest_file + " => line will be ignored.");
				}
			} else
			{
				std::map<std::string, std::list<std::string> > block;
				pFile.seekg(old_pos);
				if(!parse_block_map(pFile, block))
				{
					toErrorLog("WARNING (FILES::loadHarvestFile()): No concluding @END for " + race_names[current_race] + " block was found in file " + harvest_file + " => trying to parse what we have so far.");
				}
			
				if(harvest[current_race])
				{
					toErrorLog("WARNING (FILES::loadHarvestFile()): " + race_names[current_race] + " block was already defined in current file " + harvest_file + " => ignoring new block.");
				}
				else
				{
					std::map<std::string, std::list<std::string> >::iterator item;
					for(unsigned int i = MAX_RACES; i--;)
						if((item = block.find(race_names[i])) != block.end())
						{
							toErrorLog("WARNING (FILES::loadHarvestFile()): Block " + race_names[i] + " was found in file " + race_names[current_race] + " block in file " + harvest_file + " => ignoring entry and trying to continue.");
							block.erase(item);
						}

					harvest[current_race] = new HARVEST_SPEED;
					if((item = block.find("Mineral harvest")) != block.end())
					{
						unsigned int j = 0;
						for(std::list<std::string>::const_iterator i = item->second.begin(); i != item->second.end(); ++i)
						{
							if(i!= item->second.begin()) // TODO, warum ist item->second.begin() == 'Mineral harvest'?
								harvest[current_race]->setHarvestMineralSpeed(j++, atoi(i->c_str())/15);
						}
						block.erase(item);
					} else 
					{
						toErrorLog("ERROR (FILES::loadHarvestFile()): Field name 'Mineral harvest' not found in file " + race_names[current_race] + " block in file " + harvest_file + ".");
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
								harvest[current_race]->setHarvestGasSpeed(j++, atoi(i->c_str())/15);
						}
						block.erase(item);
					} else 
					{
						toErrorLog("ERROR (FILES::loadHarvestFile()): Field name 'Gas harvest' not found in file " + race_names[current_race] + " block in file " + harvest_file + ".");
						for(unsigned int i = MAX_RACES; i--;)
							delete harvest[i];
						return(false);
					}
					if(block.size() > 0)
						toErrorLog("WARNING (FILES::loadHarvestFile()): Other unknown entries were found in file " + race_names[current_race] + " block in file " + harvest_file + " => ignoring entries.");
				}
				current_race = MAX_RACES;
			} // end current_race != MAX_RACES
#endif
// ---------------------------------
// ------ END OF FILE LOADING ------
// ---------------------------------

// FILE SAVING

const bool FILES::saveGoal(const std::string& goal_name, GOAL_ENTRY* goalentry)
{
	std::ostringstream goal_file;
	goal_file.str("");
#ifdef __linux__
	goal_file << "settings/goals/";
	goal_file << GAME::race[goalentry->getRace()].raceString << "/" << goal_name << ".gol";// TODO!
#elif __WIN32__
	goal_file << "settings\\goals\\";
	goal_file << GAME::race[goalentry->getRace()].raceString << "\\" << goal_name << ".gol";// TODO!
#endif 
	std::ofstream pFile(goal_file.str().c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!checkStreamIsOpen(pFile, "FILES::saveGoal", goal_file.str()))
		return(false);

	goalentry->setName(goal_name);

	pFile << "@GOAL" << std::endl;
	pFile << "		\"Name\" \"" << goal_name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << GAME::race[goalentry->getRace()].raceString << "\"" << std::endl;

	for(std::list<GOAL>::const_iterator i = goalentry->goalList.begin(); i!=goalentry->goalList.end(); ++i)
		pFile << "		\"" << GAME::lookUpUnitString(goalentry->getRace(), i->getUnit()) << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	pFile << "@END" << std::endl;

	if(!loadGoalFile(goal_file.str()))
	{
		toErrorLog("ERROR (FILES::saveGoal()): Could not reload file " + goal_file.str() + ". Goal is probably lost. This is either a BUG or the OS messed something up.");
		return(false);
	}
	return(true);
}

const bool FILES::saveBuildOrder(const std::string& build_order_name, const BUILD_ORDER& build_order, ANABUILDORDER* bla)
{
	std::ostringstream build_order_file;
	build_order_file.str("");
#ifdef __linux__
	build_order_file << "output/bos/";
	build_order_file << GAME::race[build_order.getRace()].raceString << "/" << build_order_name << ".txt";
#elif __WIN32__
	build_order_file << "output\\bos\\";
	build_order_file << GAME::race[build_order.getRace()].raceString << "\\" << build_order_name << ".txt";
#endif 
	std::ofstream pFile(build_order_file.str().c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!checkStreamIsOpen(pFile, "FILES::saveBuildOrder", build_order_file.str()))
		return(false);

	pFile << "@BO_HEADER" << std::endl;
	pFile << "		\"Name\" \"" << build_order_name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << GAME::race[build_order.getRace()].raceString << "\"" << std::endl;
	pFile << "		\"Time\" \"" << build_order.getTime() << "\"" << std::endl;
	pFile << "@END" << std::endl;
	
	
	pFile << "@GOAL" << std::endl;
	pFile << "		\"Name\" \"" << build_order_name << "\"" << std::endl; // TODO! Name muesste eigentlich der echte Goal Name sein!
	pFile << "		\"Race\" \"" << GAME::race[build_order.getGoal().getRace()].raceString << "\"" << std::endl;

	for(std::list<GOAL>::const_iterator i = build_order.getGoal().goalList.begin(); i!= build_order.getGoal().goalList.end(); ++i)
		pFile << "		\"" << GAME::lookUpUnitString(build_order.getGoal().getRace(), i->getUnit()) << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	pFile << "@END" << std::endl;


	pFile << "@BUILDORDER" << std::endl;
	for(std::list<PROGRAM>::const_iterator i = build_order.getProgramList().begin(); i!=build_order.getProgramList().end(); ++i)
		pFile << "		\"" << GAME::lookUpUnitString(build_order.getRace(), i->getUnit()) << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;
	for(std::list<unsigned int>::const_iterator i = bla->getCode().begin(); i != bla->getCode().end(); ++i)
		pFile << build_order.getGoal().toPhaeno(*i) << std::endl;
	pFile << bla->getCode().size() << std::endl;
	pFile << "@END" << std::endl;
	
	if(!loadBuildOrderFile(build_order_file.str()))
	{
		toErrorLog("ERROR (FILES::saveBuildOrder()): Could not reload file " + build_order_file.str() + ". Build order is probably lost. This is either a BUG or the OS messed something up.");
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
		toErrorLog("ERROR: (FILES::saveBuildOrder) Could not create file " + os.str() + " (write protection? disk space?)");
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



