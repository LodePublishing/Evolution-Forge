#include "configuration.hpp"
#include "defs.hpp"

Configuration::Configuration(): 
	crossingOver(MIN_CROSSING_OVER),
	breedFactor(20),
	mutationFactor(200),
	maxTime(MAX_TIME),
	maxTimeOut(MAX_TIMEOUT),
	maxLength(MAX_LENGTH),
	maxRuns(MAX_RUNS),
	maxGenerations(200),
	noise(0),
	dynamicFramerate(25),
	staticFramerate(25),
	currentFramerate(1),
	currentFramesPerGeneration(1),

	language(ZERO_LANGUAGE),

	autoSaveRuns(false),
	restrictSC(true),
	facilityMode(true),
	preprocessBuildOrder(false),
	allowGoalAdaption(true),
	softwareMouse(false),
	fullScreen(false),
	backgroundBitmap(false),
	allowStaticFramerate(false),
	glowingButtons(true),
	dnaSpiral(true),
	tooltips(true),
	transparency(false),
	smoothMovements(true),
	configurationFile("settings/default.cfg")
{ }

Configuration::~Configuration()
{ }

void Configuration::initDefaults()
{
	setMaxTime(MAX_TIME-1);
	setMaxTimeOut(MAX_TIMEOUT-1);
	setMaxLength(MAX_LENGTH-1);
	setMaxRuns(MAX_RUNS-1);
	setMaxGenerations(MAX_GENERATIONS-1);
	setDynamicFramerate(25);
	setStaticFramerate(25);
	setCurrentFramerate(1);
	setCrossingOver(MIN_CROSSING_OVER);
	setBreedFactor(20);

	setLanguage(ENGLISH_LANGUAGE);
	
	setAutoSaveRuns(false);
	setRestrictSC(false);
	setFacilityMode(true);
	setPreprocessBuildOrder(false);
	setAllowGoalAdaption(true);
	setSoftwareMouse(false);
	setFullScreen(false);
	setBackgroundBitmap(false);
	setAllowStaticFramerate(false);
	setGlowingButtons(true);
	setDnaSpiral(true),
	setTooltips(true);
	setTransparency(false);
	setSmoothMovements(true);
	configurationFile = "settings/default.cfg";
}


// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------


void parse_line(string& text, list<string>& words)
{
	size_t n = text.length();
	size_t stop;
	bool inParantheses=false;
	// " gefunden? ignoriere alle Sonderzeichen bis naechstes "
	size_t start = text.find_first_not_of("\t ");
	while (start!=string::npos) 
	{
		if(text[start]=='\"')
		{
			inParantheses=true;
			start++;
		}
	
		if(inParantheses)
			stop = text.find_first_of("\"", start);
		else 
			stop = text.find_first_of(",\t\" =", start);
		inParantheses=false;
		if(stop==string::npos)
			stop = n;
		string bla=text.substr(start, stop - start);
		words.push_back(bla);
	   
		start = text.find_first_not_of(",\t =", stop+1);
	}
}

void parse_block(ifstream& stream, map<string, list<string> >& block)
{
	char line[1024];
	string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~ios::failbit);
		text=line;
		if(text.find("@END")!=string::npos) return;
		size_t start=text.find_first_not_of("\t\" ");
		if((start==string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		
		list<string> words;
		parse_line(text, words);
		string bla=words.front();
//		string bla=text.substr(start);
		block.insert(pair<string, list<string> >(bla, words));
	}
}

void parse_2nd_block(ifstream& stream, map<string, map<string, list<string> > >& block)
{
	char line[1024];
	string text;
	while(stream.getline(line, sizeof line))
	{
		if(stream.fail())
			stream.clear(stream.rdstate() & ~ios::failbit);
		text=line;
		if(text.find("@END")!=string::npos) return;
		size_t start=text.find_first_not_of("\t ");
		if((start==string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		map<string, list<string> > words;
		parse_block(stream, words);
		block.insert(pair<string, map<string, list<string> > > (text.substr(start), words));
	}
}

// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------


void Configuration::setConfigurationFile(const string& configuration_file)
{
	configurationFile = configuration_file;
}

void Configuration::saveToFile() const
{
	ofstream pFile(configurationFile.c_str(), ios_base::out | ios_base::trunc);
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadConfigurationFile): File could not be opened.");
		return;
	}
	pFile << "@SETTINGS" << endl;
	pFile << "# 1 = english, 2 = german" << endl;
	pFile << "    \"Language\" = \"" << (int)getLanguage() << "\"" << endl;
	pFile << "# max time in seconds" << endl;
	pFile << "    \"Max Time\" = \"" << getMaxTime() << "\"" << endl;
	pFile << "# Do autosave at the end of a run or ask for it?" << endl;
	pFile << "    \"Autosave runs\" = \"" << (int)isAutoSaveRuns() << "\"" << endl;
	pFile << "# Preprocessing not implemented yet" << endl;
	pFile << "    \"Preprocess Buildorder\" = \"" << (int)isPreprocessBuildOrder() << "\"" << endl;
	pFile << "    \"Max unchanged Generations\" = \"" << getMaxGenerations() << "\"" << endl;
	pFile << "    \"Max Length\" = \"" << getMaxLength() << "\"" << endl;
	pFile << "    \"Max Runs\" = \"" << getMaxRuns() << "\"" << endl;
	pFile << "# max timeout for each order in seconds" << endl;
	pFile << "    \"Max Timeout\" = \"" << getMaxTimeOut() << "\"" << endl;
	pFile << "# Allow to ignore unnecessary goals (mainly for using for example only \"lair\" instead of both \"lair\" and \"hatchery\")" << endl;
	pFile << "    \"Allow goal adaption\" = \"" << (int)isAllowGoalAdaption() << "\"" << endl;
	pFile << "# Breed Factor:" << endl;
	pFile << "# This parameter determines how many % of the players will multiply by 1:1 copy" << endl;
	pFile << "    \"Breed Factor\" = \"" << getBreedFactor() << "\"" << endl;
	pFile << "# Crossing Over:" << endl;
	pFile << "# This parameter determines how many % of the players will multiply by crossover# CURRENTLY NOT IMPLEMENTED!" << endl;
	pFile << "    \"Crossing Over\" = \"" << getCrossingOver() << "\"" << endl;
	pFile << "# 10 means that programs will be sorted into 10-program groups, the best 2 are" << endl;
	pFile << "# taken for reproduction and the worst two are replaced by their children" << endl;
	pFile << "" << endl;                                                       
	pFile << "# GUI SETTINGS:" << endl;
	pFile << "" << endl;            
	pFile << "# set this to 1 to set all details to zero, fastest output" << endl;
	pFile << "# Real minimalists should take a look at the command line options where SDL can be deactivated completely! 8-DD" << endl;
	pFile << "# CURRENTLY NOT IMPLEMENTED" << endl;
	pFile << "    \"Minimalist\" = \"0\"" << endl; // TODO
	pFile << "" << endl;
	pFile << "# how a frame update is weighted compared to a core update (e.g. 4 means that a new generation is calculated every 4 frames)" << endl;
	pFile << "    \"Allow static framerate\" = \"" << (int)isAllowStaticFramerate() << "\"" << endl;
	pFile << "# allow the program to dynamicly reduce the speed of the core to reach this frame rate. 0 = offline." << endl;
	pFile << "    \"Static framerate\" = \"" << getStaticFramerate() << "\"" << endl;
	pFile << "# Draw X frames per new generation" << endl;
	pFile << "    \"Dynamic framerate\" = \"" << getDynamicFramerate() << "\"" << endl;
	pFile << "" << endl;                                                                                
	pFile << "# Restrict unit menus to StarCraft (TM) units?" << endl;
	pFile << "    \"Restrict units\" = \"" << (int)isRestrictSC() << "\"" << endl;
	pFile << "# Order entries in the unitmenu by area or by facility?" << endl;
	pFile << "    \"Facility mode\" = \"" << (int)isFacilityMode() << "\"" << endl;
	pFile << "# glowing effects" << endl;
	pFile << "    \"Glowing buttons\" = \"" << (int)isGlowingButtons() << "\"" << endl;
	pFile << "# Show nice DNA spiral?" << endl;
	pFile << "    \"DNA Spiral\" = \"" << (int)isDnaSpiral() << "\"" << endl;
	pFile << "# moving rectangles, 2 = all objects move smoothly, 1 = some objects move smoothly, 0 = all objects jump directly to their destination" << endl;
	pFile << "    \"Smooth movements\" = \"" << (int)isSmoothMovements() << "\"" << endl;
	pFile << "# Rounded rectangles, saves a little cpu power if deactivated (the computer loves simple object :-D" << endl;
	pFile << "# CURRENTLY NOT IMPLEMENTED" << endl;
	pFile << "    \"Rounded rectangles\" = \"1\"" << endl; // TODO
	pFile << "# use background bitmap, saves some cpu power if deactivated" << endl;
	pFile << "    \"Background bitmap\" = \"" << (int)isBackgroundBitmap() << "\"" << endl;
	pFile << "# Transparency, let's burn some cpu power" << endl;
	pFile << "    \"Transparency\" = \"" << (int)isTransparency() << "\"" << endl;
	pFile << endl;                                                                                
	pFile << "    \"Tooltips\" = \"" << (int)isTooltips() << "\"" << endl;
	pFile << "    \"Fullscreen\" = \"" << (int)isFullScreen() << "\"" << endl;
	pFile << "    \"Software mouse\" = \"" << (int)isSoftwareMouse() << "\"" << endl;
	pFile << "@END" << endl;
}

void Configuration::loadConfigurationFile()
{
	ifstream pFile(configurationFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadConfigurationFile): File not found.");
		return;
	}
	char line[1024];
	string text;
	while(pFile.getline(line, sizeof line))
	{
		if(pFile.fail())
			pFile.clear(pFile.rdstate() & ~ios::failbit);
		text=line;
		size_t start=text.find_first_not_of("\t ");
		if((start==string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		size_t stop=text.find_first_of("\t ", start);
		if(stop==string::npos) stop=text.size();
		string index=text.substr(start, stop);
		string value;
		map<string, list<string> >::iterator i;
		if(index=="@SETTINGS")
		{
			map<string, list<string> > block;
			parse_block(pFile, block);

			if((i=block.find("Autosave runs"))!=block.end()) {
				i->second.pop_front();
			   	setAutoSaveRuns(atoi(i->second.front().c_str()));
			}
		
			if((i=block.find("Allow goal adaption"))!=block.end()){
				i->second.pop_front();
			   	setAllowGoalAdaption(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Restrict units"))!=block.end()){
				i->second.pop_front();
				setRestrictSC(atoi(i->second.front().c_str()));
			}
			
			if((i=block.find("Facility mode"))!=block.end()){
				i->second.pop_front();
			   	setFacilityMode(atoi(i->second.front().c_str()));
			}
	
			if((i=block.find("Language"))!=block.end()){
				i->second.pop_front();
			   	setLanguage((eLanguage)(atoi(i->second.front().c_str())));
			}
			if((i=block.find("Max Time"))!=block.end()){
				i->second.pop_front();
			   	setMaxTime(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Timeout"))!=block.end()){
				i->second.pop_front();
			   	setMaxTimeOut(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Length"))!=block.end()){
				i->second.pop_front();
			   	setMaxLength(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Runs"))!=block.end()){
				i->second.pop_front();
			   	setMaxRuns(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Preprocess Buildorder"))!=block.end()){
				i->second.pop_front();
			   	setPreprocessBuildOrder(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Breed Factor"))!=block.end()){
				i->second.pop_front();
			   	setBreedFactor(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Crossing Over"))!=block.end()){ 
				i->second.pop_front();
				setCrossingOver(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max unchanged Generations"))!=block.end()){
				i->second.pop_front();
			   	setMaxGenerations(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Software mouse"))!=block.end()){
				i->second.pop_front();
			   	setSoftwareMouse(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Fullscreen"))!=block.end()){
				i->second.pop_front();
			   	setFullScreen(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Background bitmap"))!=block.end()){
				i->second.pop_front();
			   	setBackgroundBitmap(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Allow static framerate"))!=block.end()){
				i->second.pop_front();
			   	setAllowStaticFramerate(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Static framerate"))!=block.end()){
				i->second.pop_front();
			   	setStaticFramerate(atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Dynamic framerate"))!=block.end()){
				i->second.pop_front();
			   	setDynamicFramerate(atoi(i->second.front().c_str()));
			}		
		
			if((i=block.find("Glowing buttons"))!=block.end()){
				i->second.pop_front();
			   	setGlowingButtons(atoi(i->second.front().c_str()));
			}
			if((i=block.find("DNA Spiral"))!=block.end()){
				i->second.pop_front();
			   	setDnaSpiral(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Tooltips"))!=block.end()){
				i->second.pop_front();
			   	setTooltips(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Transparency"))!=block.end()){
				i->second.pop_front();
			   	setTransparency(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Smooth movements"))!=block.end()){
				i->second.pop_front();
			   	setSmoothMovements(atoi(i->second.front().c_str()));
			}

		}
	}// END while
} // schoen :)

	
Configuration configuration;

