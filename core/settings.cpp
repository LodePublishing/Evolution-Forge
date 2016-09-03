#include "settings.hpp"
#include "../ui/object.hpp"

#include <dirent.h>
#include <sys/types.h>

#ifdef __linux__
#else 
#include <windows.h>
#endif

// TODO in theme rein oder so

SETTINGS::SETTINGS():
	loadedMap(),
	soup(),
	start()
{
	srand(time(NULL));
//	initDefaults();
}

SETTINGS::~SETTINGS()
{
	for(std::vector<BASIC_MAP*>::iterator i = loadedMap.begin(); i!=loadedMap.end(); i++)
		delete *i;
	loadedMap.clear();
	for(unsigned int j=0;j<MAX_RACES;j++)
	{
		for(std::vector<START_CONDITION*>::iterator i = loadedStartcondition[j].begin(); i!=loadedStartcondition[j].end(); i++)
			delete *i;
		for(std::vector<GOAL_ENTRY*>::iterator i = loadedGoal[j].begin(); i!=loadedGoal[j].end(); i++)
			delete *i;
		loadedStartcondition[j].clear();
		loadedGoal[j].clear();
	}
}


list<string> SETTINGS::findFiles(const string& directory1, const string& directory2, const string& directory3) const
{
	list<string> fileList;
#ifdef __linux__
	DIR *dir;
	struct dirent *entry;
	ostringstream os;
	os << directory1 << "/" << directory2 << "/" << directory3;
	if ((dir = opendir(os.str().c_str())) == NULL)
		toLog("ERROR opening directory " + os.str());
	else 
	{
		while ((entry = readdir(dir)) != NULL)
			fileList.push_back(os.str() + "/" + entry->d_name);
		closedir(dir);
	}
#elif __WIN32__
	WIN32_FIND_DATA dir;
	HANDLE fhandle;
	ostringstream os;
	os << directory1 << "\\" << directory2 << "\\" << directory3 << "\\" << "*.*";
	if ((fhandle=FindFirstFile(os.str().c_str(), &dir)) !=INVALID_HANDLE_VALUE)
	{
		do 
			fileList.push_back(directory1 + "\\" + directory2 + "\\" + directory3 + "\\" + dir.cFileName);
		while(FindNextFile(fhandle, &dir));
	} else
		toLog("ERROR Loading " + directory1 + "\\" + directory2 + "\\" + directory3 + ".");
	FindClose(fhandle);
#endif
	return fileList;
}



SETTINGS settings;

// -------------------------------
// ------ CONTROL FUNCTIONS ------
// -------------------------------

void SETTINGS::assignRunParametersToSoup()
{
//	soup.initializeMap(start.getMap()); //???? TODO
	// set GA and START on prerace and soup
	soup.setParameters(&start);
	// allocate memory for players ~~
}

ANARACE** SETTINGS::newGeneration(ANARACE* oldAnarace[MAX_PLAYER])
{
	return(soup.newGeneration(oldAnarace));
}

void SETTINGS::calculateAnaplayer()
{
	soup.calculateAnaplayer();
}

void SETTINGS::fillGroups()
{
	start.fillGroups();
}

void SETTINGS::checkForChange() const
{
	soup.checkForChange();	
}

// ---------------------------
// ------- FILE LOADING ------
// ---------------------------

void SETTINGS::loadGoalFile(const string& goalFile)
{
	if((goalFile.compare(goalFile.size()-4, goalFile.size(), ".gol")==1))
		return;

	ifstream pFile(goalFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadGoalFile): File " + goalFile + " not found.");
		return;
	}
	toLog(goalFile + " loaded.");
	
	char line[1024];
	string text;
	GOAL_ENTRY* goal = new GOAL_ENTRY;
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~ios::failbit);
		text=line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		size_t stop_position = text.find_first_of("\t ",start_position);
		if(stop_position == string::npos) stop_position = text.size();
		string index=text.substr(start_position, stop_position);
		map<string, list<string> >::iterator i;

		if(index=="@GOAL")
		{
			map<string, list<string> > block;
			parse_block(pFile, block);
			if((i=block.find("Name"))!=block.end()){
				i->second.pop_front();
				goal->setName(i->second.front());
			}
			if((i=block.find("Race"))!=block.end()) 
			{
				eRace race=TERRA;
				i->second.pop_front();
				string estr=i->second.front();
				if(i->second.front()=="Terra") race=TERRA;
				else if(i->second.front()=="Protoss") race=PROTOSS;
				else if(i->second.front()=="Zerg") race=ZERG;
#ifdef _SCC_DEBUG
				else {
					toLog("ERROR: (loadSettingsFile): Wrong race entry.");return;
				}
#endif
				goal->setRace(race);
			}
			map<string, list<string> >::iterator k;
			for(unsigned int unit=UNIT_TYPE_COUNT;unit--;)
			{
				if((k=block.find(stats[goal->getRace()][unit].name))!=block.end())
				{
					list<string>::iterator l=k->second.begin();
					if(l->size()>=3)
					{
						l++;int count=atoi(l->c_str());
						l++;int location=atoi(l->c_str());
						l++;int time=atoi(l->c_str());
						goal->addGoal(unit, count, time, location);
					}
				}
			}
		} // end index == GOAL
	}

	loadedGoal[goal->getRace()].push_back(goal);
	
//  loadedGoal[getGoalCount()].adjustGoals(configuration.allowGoalAdaption);
} // schoen :)

void SETTINGS::loadHarvestFile(const string& harvestFile)
{
	ifstream pFile(harvestFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadHarvestFile): File not found.");
		return;
	}
	char line[1024];
	string text;
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~ios::failbit);
		text = line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == string::npos)||(text[0]=='#')||(text[0]=='\0'))
				continue; // ignore line
		size_t stop_position = text.find_first_of("\t ", start_position);
		if(stop_position == string::npos) stop_position = text.size();
		string index = text.substr(start_position, stop_position);
		map<string, map<string, list<string> > >::iterator value;
		map<string, list<string> >::iterator item;
		if(index == "@HARVESTDATA")
		{
				map<string, map<string, list<string> > > block;
				parse_2nd_block(pFile, block);
				map<string, list<string> > player;
				if((value = block.find("@TERRA")) != block.end())
				{
					// erstes Element falsch? TODO
					if((item = value->second.find("Mineral Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); // the expression 'mineral harvest' itself
						for(list<string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							loadedHarvestSpeed[TERRA].setHarvestMineralSpeed(j++, atoi(i->c_str()));
					}
					if((item = value->second.find("Gas Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); // the expression 'gas harvest' itself
						for(list<string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							loadedHarvestSpeed[TERRA].setHarvestGasSpeed(j++, atoi(i->c_str()));
					}
				}
				if((value=block.find("@PROTOSS"))!=block.end())
				{
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front();
						for(list<string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							loadedHarvestSpeed[PROTOSS].setHarvestMineralSpeed(j++, atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); 
						for(list<string>::const_iterator i = item->second.begin();i != item->second.end(); ++i)
							loadedHarvestSpeed[PROTOSS].setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
				}
				if((value=block.find("@ZERG"))!=block.end())
				{
					if((item = value->second.find("Mineral Harvest")) != value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); 
						for(list<string>::const_iterator i=item->second.begin();i != item->second.end(); ++i)
							loadedHarvestSpeed[ZERG].setHarvestMineralSpeed(j++, atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest")) != value->second.end())
					{
						unsigned int j = 0;
						item->second.pop_front(); 
						for(list<string>::const_iterator i = item->second.begin();i != item->second.end();++i)
							loadedHarvestSpeed[ZERG].setHarvestGasSpeed(j++, atoi(i->c_str()));
					}
				}
		}
	}// END while
} // schoen :)

void SETTINGS::loadMapFile(const string& mapFile)
{
	ifstream pFile(mapFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadMapFile): File not found.");
		return;
	}
	char line[1024];
	string text;
	BASIC_MAP* basicmap = new BASIC_MAP;
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~ios::failbit);
		text=line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		list<string> words;
		parse_line(text, words);
		if(words.empty()) continue;
		list<string>::iterator j=words.begin();
		string index=*j;j++;
		map<string, list<string> >::iterator i;
		if(index=="@MAP")
		{
				map<string, list<string> > block;
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
				toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
				return;
			}
			int location = atoi(j->c_str());
			map<string, list<string> > block;
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
	
/*DEBUG	for(unsigned int i = 1; i < basicmap->getMaxLocations(); i++)
	{	
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < basicmap->getMaxLocations(); j++)
			std::cout << basicmap->getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}

	basicmap->calculateLocationsDistances();

	for(unsigned int i = 1; i < basicmap->getMaxLocations(); i++)
	{	
		std::cout << "Location " << i << " ";
		for(unsigned int j = 1; j < basicmap->getMaxLocations(); j++)
			std::cout << basicmap->getLocation(i)->getDistance(j) << " ";
		std::cout << std::endl;
	}*/
	
	loadedMap.push_back(basicmap);
} // schoen :)

void SETTINGS::loadStartconditionFile(const string& startconditionFile)
{
	ifstream pFile(startconditionFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadStartconditionFile): File not found.");
		return;
	}
	START_CONDITION* startcondition = new START_CONDITION;

	char line[1024];
	string text;
	eRace race=TERRA; 
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~ios::failbit);
		text=line;
		size_t start_position = text.find_first_not_of("\t ");
		if((start_position == string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		list<string> words;
		parse_line(text, words);

		list<string>::iterator j=words.begin();
		string index=*j;j++;
		map<string, list<string> >::iterator i;
		if(index=="@STARTCONDITIONS")
		{
			if(j==words.end())
			{
				toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
				return;
			}

			if(*j=="Terra") race=TERRA;
			else if(*j=="Protoss") race=PROTOSS;
			else if(*j=="Zerg") race=ZERG;

			map<string, list<string> > block;
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
				toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
				return;
			}
			
			int location = atoi(j->c_str());
// TODO pruefen
			map<string, list<string> > block;
			parse_block(pFile, block);
			for(unsigned int k=UNIT_TYPE_COUNT;k--;)
			{
				string unit=stats[race][k].name;
				if((i=block.find(unit))!=block.end())
				{
					i->second.pop_front();int count=atoi(i->second.front().c_str());
						//TODO: values checken!
					startcondition->setLocationTotal(location-1, k, count);
					startcondition->setLocationAvailible(location-1, k, count);
				}
			}
		} // end if == LOCATION
	}// END while
	startcondition->assignRace(race);
	startcondition->adjustResearches();
	startcondition->adjustSupply();
	loadedStartcondition[race].push_back(startcondition); // beendet nicht richtig :/
} // schoen :)

// ---------------------------------
// ------ END OF FILE LOADING ------
// ---------------------------------

// FILE SAVING


void SETTINGS::saveBuildOrder(const string& name, const ANARACE* anarace) const
{
	ostringstream os;
#ifdef __linux__
	os << "output/bos/";
	os << raceString[anarace->getRace()] << "/" << name << ".html";
#elif __WIN32__
	os << "output\\bos\\";
	os << raceString[anarace->getRace()] << "\\" << name << ".html";
#endif
	ofstream pFile(os.str().c_str(), ios_base::out | ios_base::trunc);
	
	if(!pFile.is_open())
	{
		toLog("ERROR: Could not create file (write protection? disk space?)");
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
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 200px;\">" << *UI_Object::theme.lookUpString(OUTPUT_UNITNAME_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << *UI_Object::theme.lookUpString(OUTPUT_SUPPLY_STRING) << "</td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << *UI_Object::theme.lookUpString(OUTPUT_MINERALS_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << *UI_Object::theme.lookUpString(OUTPUT_GAS_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 100px;\">" << *UI_Object::theme.lookUpString(OUTPUT_LOCATION_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	  <td style=\"text-align: center; vertical-align: middle; width: 75px;\">" << *UI_Object::theme.lookUpString(OUTPUT_TIME_STRING) << "<br>" << std::endl;
	pFile << "	  </td>" << std::endl;
	pFile << "	</tr>" << std::endl;

	for(std::list<PROGRAM>::const_iterator order = anarace->programList.begin(); order != anarace->programList.end(); ++order)
	{
		pFile << "	<tr style=\"text-align: center; vertical-align: middle; background-color: rgb(" << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][order->getUnit()].unitType))->GetColor()->r() << ", " << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][order->getUnit()].unitType))->GetColor()->g() << ", " << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][order->getUnit()].unitType))->GetColor()->b() << ");\">" << std::endl;
		pFile << "	  <td style=\"\">" << *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace() + order->getUnit() + UNIT_NULL_STRING)) << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;
		pFile << "	  <td style=\"\">" << order->getStatisticsBefore().getNeedSupply() << "/" << order->getStatisticsBefore().getHaveSupply() << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;

		pFile << "	  <td style=\"\">" << order->getStatisticsBefore().getHaveMinerals()/100 << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;

		pFile << "	  <td style=\"\">" << order->getStatisticsBefore().getHaveGas()/100 << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;

		pFile << "	  <td style=\"\">" << (*anarace->getMap())->getLocation(order->getLocation())->getName() << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;

		pFile << "	  <td style=\"\">" << formatTime(order->getRealTime()) << "<br>" << std::endl;
		pFile << "	  </td>" << std::endl;
																								   
		pFile << "	</tr>" << std::endl;
	}

	pFile << "  </tbody>" << std::endl;
	pFile << "</table>" << std::endl;
	pFile << "<br>" << std::endl;
	pFile << "<b><a href=\"http://www.clawsoftware.de\">www.clawsoftware.de</a></b>\n";
	pFile << "</body>\n";
	pFile << "</html>" << std::endl;
}

void SETTINGS::saveGoal(const string& name, GOAL_ENTRY* goalentry)
{
	ostringstream os;
#ifdef __linux__
	os << "settings/goals/";
	os << raceString[goalentry->getRace()] << "/" << name << ".gol";// TODO!
#elif __WIN32__
	os << "settings\\goals\\";
	os << raceString[goalentry->getRace()] << "\\" << name << ".gol";// TODO!
#endif 
	ofstream pFile(os.str().c_str(), ios_base::out | ios_base::trunc);
	if(!pFile.is_open())
	{
			toLog("ERROR: Could not create file (write protection? disk space?)");
			return;
	}

	goalentry->setName(name);

	pFile << "@GOAL" << std::endl;
	pFile << "		\"Name\" \"" << name << "\"" << std::endl; // TODO
	pFile << "		\"Race\" \"" << raceString[goalentry->getRace()] << "\"" << std::endl;

	for(std::list<GOAL>::const_iterator i = goalentry->goal.begin(); i!=goalentry->goal.end(); i++)
		pFile << "		\"" << stats[goalentry->getRace()][i->getUnit()].name << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	
	pFile << "@END" << std::endl;
	loadGoalFile(os.str().c_str());
}


// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

