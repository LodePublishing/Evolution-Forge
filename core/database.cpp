// -----------------------------------------------------------------------------------------
// DATABASE holds all map, harvest, goal and start-condition date for other classes to read
// In addition it saves and loads these data from/to files.
// -----------------------------------------------------------------------------------------

#include "database.hpp"

#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>

#ifdef __linux__
#else 
#include <windows.h>
#endif

DATABASE::DATABASE():
	loadedMap()
{
	srand(time(NULL));
}

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

// TODO evtl in misc.cpp rein
std::list<std::string> DATABASE::findFiles(const std::string& directory1, const std::string& directory2, const std::string& directory3)
{
	std::list<std::string> fileList;
	std::ostringstream os;
	os.str("");
#ifdef __linux__
	DIR *dir;
	struct dirent *entry;
	if(directory3!="")
		os << directory1 << "/" << directory2 << "/" << directory3;
	else if(directory2!="")
		os << directory1 << "/" << directory2;
	else if(directory1!="")
		os << directory1;

	if ((dir = opendir(os.str().c_str())) == NULL)
		toLog("ERROR: (DATABASE::findFiles) Cannot open directory " + os.str());
	else 
	{
		while ((entry = readdir(dir)) != NULL)
			fileList.push_back(os.str() + "/" + entry->d_name);
		closedir(dir);
	}
#elif __WIN32__
	WIN32_FIND_DATA dir;
	HANDLE fhandle;
	os << directory1 << "\\" << directory2 << "\\" << directory3 << "\\" << "*.*";
	if ((fhandle=FindFirstFile(os.str().c_str(), &dir)) !=INVALID_HANDLE_VALUE)
	{
		do
			fileList.push_back(directory1 + "\\" + directory2 + "\\" + directory3 + "\\" + dir.cFileName);
		while(FindNextFile(fhandle, &dir));
	} else
		toLog("ERROR: (DATABASE::findFiles) Cannot load " + directory1 + "\\" + directory2 + "\\" + directory3 + ".");
	FindClose(fhandle);
#endif
	return fileList;
}




void DATABASE::addDefaultGoal(const eRace race)
{
	GOAL_ENTRY* goal = new GOAL_ENTRY;
	goal->setName("Clear List");
	goal->setRace(race);
	loadedGoal[goal->getRace()].push_back(goal);
}

// ---------------------------
// ------- FILE LOADING ------
// ---------------------------

GOAL_ENTRY* parseGoalBlock(std::list< std::list<std::string> >::iterator& i, std::list< std::list<std::string> >& block)
{
	GOAL_ENTRY* goal = new GOAL_ENTRY;
	bool race_initialized = false;

	for(; i!=block.end(); ++i)
	{
		if(*(i->begin()) == "Name")
		{
			i->pop_front();
			goal->setName(*(i->begin()));
		} else if(*(i->begin()) == "Race")
		{
			i->pop_front();
			std::string estr = i->front();
			eRace goal_race = TERRA;
			if(estr == raceString[TERRA]) goal_race=TERRA;
			else if(estr == raceString[PROTOSS]) goal_race=PROTOSS;
			else if(estr == raceString[ZERG]) goal_race=ZERG;
#ifdef _SCC_DEBUG
			else {
				toLog("ERROR: parseGoalBlock(): Wrong race entry (" + estr + " [" + raceString[TERRA] + "|" + raceString[PROTOSS] + "|" + raceString[ZERG] + "]).");delete goal;return(NULL);
			}
			race_initialized = true;
#endif
			goal->setRace(goal_race);
		} else if(*(i->begin()) == "@END")
		{
			return(goal);
		} else if(race_initialized)
		{
			for(unsigned int unit=UNIT_TYPE_COUNT;unit--;)
			{
				if(*(i->begin()) == stats[goal->getRace()][unit].name)
				{
					std::list<std::string>::iterator l=i->begin();
					if(l->size()>=3)
					{
						++l;int count=atoi(l->c_str());
						++l;int location=atoi(l->c_str());
						++l;int time=atoi(l->c_str());
						goal->addNewGoalToList(unit, time, location, count);
					}
				}
			}
		}
	}
	toLog("ERROR: parseGoalBlock(): No @END for goal block found");
	return(NULL);
}

void DATABASE::loadGoalFile(const std::string& goal_file)
{
	if((goal_file.compare(goal_file.size()-4, goal_file.size(), ".gol")==1))
		return;

	std::ifstream pFile(goal_file.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (DATABASE::loadGoalFile): File " + goal_file + " not found.");
		return;
	}
//	toLog(goal_file + " loaded.");
	std::list<std::list<std::string> > block;
	parse_block(pFile, block);

	for(std::list<std::list<std::string> >::iterator i = block.begin(); i!=block.end(); ++i)
	{

		if(*(i->begin())=="@GOAL")
		{
			parse_block(pFile, block);
			GOAL_ENTRY* my_goal = parseGoalBlock(i, block);
			if(my_goal == NULL)
			{
				toLog("ERROR: DATABASE::loadGoalFile(): Error parsing " + goal_file + ".");
				return;
			}
			else 
			{
				loadedGoal[my_goal->getRace()].push_back(my_goal);
				return;
			}
		} // end index == GOAL
	}
} // schoen :) naja :o

void DATABASE::loadBuildOrderFile(const std::string& build_order_file)
{
	if((build_order_file.compare(build_order_file.size()-4, build_order_file.size(), ".txt")==1))
		return;

	std::ifstream pFile(build_order_file.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (DATABASE::loadBuildOrderFile): File " + build_order_file + " not found.");
		return;
	}
//	toLog(build_order_file + " loaded.");
	
	
	std::list<std::list<std::string> > block;
	std::list<PROGRAM> program_list;
	parse_block(pFile, block);
	bool bo_mode = false;
	eRace bo_race = TERRA;
	std::string bo_name;
	GOAL_ENTRY* bo_goal;
	unsigned int bo_time = 0;
	for(std::list<std::list<std::string> >::iterator i = block.begin(); i!=block.end(); ++i)
	{
		if(*(i->begin())=="@GOAL")
		{
			parse_block(pFile, block);
			bo_goal = parseGoalBlock(i, block);
			if(bo_goal==NULL) {
				toLog("ERROR: DATABASE::loadBuildOrderFile(): Error parsing " +  build_order_file + ".");return;
			}
		} else	
		if(*(i->begin()) == "@BUILDORDER")
			bo_mode = true;
		else if(*(i->begin()) == "@END")
			bo_mode = false;
		else if(bo_mode==true)
		{
			if(*(i->begin()) == "Name")
			{
				i->pop_front();
				bo_name = i->front();
			} else 
			if(*(i->begin()) == "Race")
			{
				i->pop_front();
				std::string estr = i->front();
				if(estr == raceString[TERRA]) bo_race=TERRA;
				else if(estr == raceString[PROTOSS]) bo_race=PROTOSS;
				else if(estr == raceString[ZERG]) bo_race=ZERG;
#ifdef _SCC_DEBUG
				else {
					toLog("ERROR: (DATABASE::loadBuildOrderFile [" + build_order_file + "]): Wrong race entry (" + estr + " [" + raceString[TERRA] + "|" + raceString[PROTOSS] + "|" + raceString[ZERG] + "]).");return;
				}
#endif
			} else 
			if(*(i->begin()) == "Time")
			{
				i->pop_front();
				bo_time = atoi(i->begin()->c_str());
			} else
			{
				
				for(unsigned int unit=UNIT_TYPE_COUNT;unit--;)
				{
					if(*(i->begin())==stats[bo_race][unit].name)
					{
						PROGRAM p;
						p.setUnit(unit);
						program_list.push_back(p);
						break;
					}
				}
			}
		} // end index == BUILDORDER
	}
	BUILD_ORDER* build_order = new BUILD_ORDER(bo_race, *bo_goal, bo_name, bo_time, program_list);
	delete bo_goal;
	loadedBuildOrder[build_order->getRace()].push_back(build_order);

} // schoen :)

void DATABASE::loadHarvestFile(const std::string& harvestFile)
{
	if((harvestFile.compare(harvestFile.size()-4, harvestFile.size(), ".hvt")==1))
		return;

	std::ifstream pFile(harvestFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (DATABASE::loadHarvestFile): File " + harvestFile + "not found.");
		return;
	}
	char line[1024];
	std::string text;
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		text = line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
				continue; // ignore line
		size_t stop_position = text.find_first_of("\t ", start_position);
		if(stop_position == std::string::npos) stop_position = text.size();
		std::string index = text.substr(start_position, stop_position);
		std::map<std::string, std::map<std::string, std::list<std::string> > >::iterator value;
		std::map<std::string, std::list<std::string> >::iterator item;
		if(index == "@HARVESTDATA")
		{
				std::map<std::string, std::map<std::string, std::list<std::string> > > block;
				parse_2nd_block(pFile, block);
				std::map<std::string, std::list<std::string> > player;
				if((value = block.find("@TERRA")) != block.end())
				{
					HARVEST_SPEED* harvest = new HARVEST_SPEED();
					// erstes Element falsch? TODO
					if((item = value->second.find("Mineral Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); // the expression 'mineral harvest' itself
						for(std::list<std::string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							harvest->setHarvestMineralSpeed(j++, atoi(i->c_str()));
					}
					if((item = value->second.find("Gas Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); // the expression 'gas harvest' itself
						for(std::list<std::string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							harvest->setHarvestGasSpeed(j++, atoi(i->c_str()));
					}
					loadedHarvestSpeed[TERRA].push_back(harvest);
				}
				if((value=block.find("@PROTOSS"))!=block.end())
				{
					HARVEST_SPEED* harvest = new HARVEST_SPEED();
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front();
						for(std::list<std::string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							harvest->setHarvestMineralSpeed(j++, atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); 
						for(std::list<std::string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							harvest->setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
					loadedHarvestSpeed[PROTOSS].push_back(harvest);
				}
				if((value=block.find("@ZERG"))!=block.end())
				{
					HARVEST_SPEED* harvest = new HARVEST_SPEED();
					if((item = value->second.find("Mineral Harvest")) != value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); 
						for(std::list<std::string>::const_iterator i=item->second.begin();i != item->second.end(); ++i)
							harvest->setHarvestMineralSpeed(j++, atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest")) != value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); 
						for(std::list<std::string>::const_iterator i = item->second.begin();i != item->second.end();++i)
							harvest->setHarvestGasSpeed(j++, atoi(i->c_str()));
					}
					loadedHarvestSpeed[ZERG].push_back(harvest);
				}
		}
	}// END while
} // schoen :)

void DATABASE::loadMapFile(const std::string& mapFile)
{
	if((mapFile.size()<20)||(mapFile.compare(mapFile.size()-4, mapFile.size(), ".map")==1))
		return;
	std::ifstream pFile(mapFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (DATABASE::loadMapFile): File " + mapFile + " not found.");
		return;
	}
	char line[1024];
	std::string text;
	BASIC_MAP* basicmap = new BASIC_MAP; // TODO mehrere maps verhindern?
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		text=line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		std::list<std::string> words;
		parse_line(text, words);
		if(words.empty()) continue;
		std::list<std::string>::iterator j=words.begin();
		std::string index=*j;++j;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@MAP")
		{
			std::map<std::string, std::list<std::string> > block;
			parse_block(pFile, block);
			if((i=block.find("Name"))!=block.end()){
				i->second.pop_front();
			  	basicmap->setName(i->second.front());
			}
			if((i=block.find("Max Locations"))!=block.end()){
				i->second.pop_front();
			   	basicmap->setMaxLocations(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Player"))!=block.end()){
				i->second.pop_front();
			   	basicmap->setMaxPlayer(atoi(i->second.front().c_str()));
			}
		}
		else if(index=="@LOCATION")
		{
			if(j==words.end())
			{
				toLog("ERROR: (DATABASE::loadMapFile [" + mapFile + "]): Every @LOCATION entry needs a number.");
				return;
			}
			int location = atoi(j->c_str());
			std::map<std::string, std::list<std::string> > block;
			parse_block(pFile, block);
			if((i=block.find("Name"))!=block.end()) 
			{
				i->second.pop_front();
				basicmap->setLocationName(location-1, i->second.front().c_str());
			}
			if((i=block.find("Mineral Distance"))!=block.end()) 
			{
				i->second.pop_front();
				basicmap->setLocationMineralDistance(location-1, atoi(i->second.front().c_str()));
			}
			if((i=block.find("Distance to"))!=block.end()) 
			{
				i->second.pop_front();
				int target = atoi(i->second.front().c_str());
				i->second.pop_front();
				basicmap->setLocationDistance(location-1, target-1, atoi(i->second.front().c_str()));
//				cout << location-1 << "->" << target-1 << " : " << atoi(i->second.front().c_str()) << std::endl;
			}
			if((i=block.find("Minerals"))!=block.end())
			{
				i->second.pop_front();
				basicmap->setLocationMineralPatches(location-1, atoi(i->second.front().c_str()));
			}
			if((i=block.find("Geysirs"))!=block.end())
			{
				i->second.pop_front();
				basicmap->setLocationVespeneGeysirs(location-1, atoi(i->second.front().c_str()));
			}
			
		}
	}// END while
	
/*DEBUG	for(unsigned int i = 1; i < basicmap->getMaxLocations(); ++i)
	{	
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < basicmap->getMaxLocations(); ++j)
			std::cout << basicmap->getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}

	basicmap->calculateLocationsDistances();

	for(unsigned int i = 1; i < basicmap->getMaxLocations(); ++i)
	{	
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < basicmap->getMaxLocations(); ++j)
			std::cout << basicmap->getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}*/
	
	loadedMap.push_back(basicmap);
} // schoen :)

void DATABASE::loadStartConditionFile(const std::string& startconditionFile)
{
	if((startconditionFile.compare(startconditionFile.size()-6, startconditionFile.size(), ".start")==1))
		return;

	std::ifstream pFile(startconditionFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (DATABASE::loadStartConditionFile): File " + startconditionFile + " not found.");
		return;
	}
	START_CONDITION* startcondition = new START_CONDITION;

	char line[1024];
	std::string text;
	eRace race=TERRA; 
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~std::ios::failbit);
		text=line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		std::list<std::string> words;
		parse_line(text, words);

		std::list<std::string>::iterator j=words.begin();
		std::string index=*j;++j;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@STARTCONDITIONS")
		{
			if(j==words.end())
			{
				toLog("ERROR: (DATABASE::loadMapFile [" + startconditionFile + "]): Every @LOCATION entry needs a number.");
				return;
			}

			if(*j == raceString[TERRA]) race=TERRA;
			else if(*j == raceString[PROTOSS]) race=PROTOSS;
			else if(*j == raceString[ZERG]) race=ZERG;

			std::map<std::string, std::list<std::string> > block;
			parse_block(pFile, block);

			if((i=block.find("Name"))!=block.end()) 
			{
				i->second.pop_front();
				startcondition->setName(i->second.front());
			}
			if((i=block.find("Minerals"))!=block.end()) 
			{
				i->second.pop_front();
				startcondition->setMinerals(100*atoi(i->second.front().c_str()));
			}
			if((i=block.find("Gas"))!=block.end()) 
			{
				i->second.pop_front();
				startcondition->setGas(100*atoi(i->second.front().c_str()));
			}
			if((i=block.find("Time"))!=block.end()) 
			{
				i->second.pop_front();
				startcondition->setStartTime(atoi(i->second.front().c_str()));
			}
		}
		else if(index=="@LOCATION")
		{
			if(j==words.end())
			{
				toLog("ERROR: (DATABASE::loadMapFile [" + startconditionFile + "]): Every @LOCATION entry needs a number.");
				return;
			}
			
			unsigned int location = atoi(j->c_str());
// TODO pruefen
			std::map<std::string, std::list<std::string> > block;
			parse_block(pFile, block);
			for(unsigned int k=UNIT_TYPE_COUNT;k--;)
			{
				std::string unit=stats[race][k].name;
				if((i=block.find(unit))!=block.end())
				{
					i->second.pop_front();
					unsigned int count=atoi(i->second.front().c_str());
//TODO: values checken!
					if(count > 0)
					{
						startcondition->setLocationTotal(location-1, k, count);
						startcondition->setLocationAvailible(location-1, k, count);
			//			std::ostringstream os;
			//			os << unit << " : " << count;
			//			toLog(os.str());
					}
				}
			}
		} // end if == LOCATION
	}// END while
	startcondition->assignRace(race);
	startcondition->adjustResearches();
	startcondition->adjustSupply();
	loadedStartCondition[race].push_back(startcondition); // beendet nicht richtig :/
		
/*	for(unsigned int i = MAX_LOCATIONS;i--;)
		for(unsigned int j=UNIT_TYPE_COUNT;j--;)
			if(startcondition->getLocationTotal(i, j))
			{
				std::ostringstream os;
				os << stats[startcondition->getRace()][j].name << " : " << startcondition->getLocationTotal(i, j);
				toLog(os.str());
			}*/
} // schoen :)

// ---------------------------------
// ------ END OF FILE LOADING ------
// ---------------------------------

// FILE SAVING

void DATABASE::saveGoal(const std::string& name, GOAL_ENTRY* goalentry)
{
	std::ostringstream os;
	os.str("");
#ifdef __linux__
	os << "settings/goals/";
	os << raceString[goalentry->getRace()] << "/" << name << ".gol";// TODO!
#elif __WIN32__
	os << "settings\\goals\\";
	os << raceString[goalentry->getRace()] << "\\" << name << ".gol";// TODO!
#endif 
	std::ofstream pFile(os.str().c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!pFile.is_open())
	{
		toLog("ERROR: (DATABASE::saveGoal) Could not create file " + os.str() + " (write protection? disk space?)");
		return;
	}

	goalentry->setName(name);

	pFile << "@GOAL" << std::endl;
	pFile << "		\"Name\" \"" << name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << raceString[goalentry->getRace()] << "\"" << std::endl;

	for(std::list<GOAL>::const_iterator i = goalentry->goal.begin(); i!=goalentry->goal.end(); ++i)
		pFile << "		\"" << stats[goalentry->getRace()][i->getUnit()].name << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	pFile << "@END" << std::endl;
	loadGoalFile(os.str().c_str());
}

void DATABASE::saveBuildOrder(const std::string& name, BUILD_ORDER& build_order)
{
	std::ostringstream os;
	os.str("");
#ifdef __linux__
	os << "output/bos/";
	os << raceString[build_order.getRace()] << "/" << name << ".txt";
#elif __WIN32__
	os << "output\\bos\\";
	os << raceString[build_order.getRace()] << "\\" << name << ".txt";
#endif 
	std::ofstream pFile(os.str().c_str(), std::ios_base::out | std::ios_base::trunc);
	if(!pFile.is_open())
	{
		toLog("ERROR: (DATABASE::saveBuildOrder) Could not create file " + os.str() + " (write protection? disk space?)");
		return;
	}

	pFile << "@GOAL" << std::endl;
	pFile << "		\"Name\" \"" << name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << raceString[build_order.getGoal().getRace()] << "\"" << std::endl;

	for(std::list<GOAL>::const_iterator i = build_order.getGoal().goal.begin(); i!= build_order.getGoal().goal.end(); ++i)
		pFile << "		\"" << stats[build_order.getGoal().getRace()][i->getUnit()].name << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	pFile << "@END" << std::endl;

	pFile << "@BUILDORDER" << std::endl;
	pFile << "		\"Name\" \"" << name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << raceString[build_order.getRace()] << "\"" << std::endl;
	pFile << "		\"Time\" \"" << build_order.getTime() << "\"" << std::endl;

	for(std::list<PROGRAM>::iterator i = build_order.getProgramList().begin(); i!=build_order.getProgramList().end(); ++i)
		pFile << "		\"" << stats[build_order.getRace()][i->getUnit()].name << "\""/* \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\""*/ << std::endl;
	pFile << "@END" << std::endl;
	loadBuildOrderFile(os.str().c_str());

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
		toLog("ERROR: (DATABASE::saveBuildOrder) Could not create file " + os.str() + " (write protection? disk space?)");
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
	if(build_order>=getBuildOrderCount(race, goal)) {
		toLog("WARNING: (DATABASE::getBuildOrder): Value out of range.");return(NULL);
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
