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

const unsigned int Configuration::getCrossingOver() const {
#ifdef _SCC_DEBUG
    if((crossingOver<MIN_CROSSING_OVER)||(crossingOver>MAX_CROSSING_OVER)) {
        toLog("WARNING: (Configuration::getCrossingOver): Value out of range.");return(MIN_CROSSING_OVER);
    }
#endif
	return(crossingOver);
}

const unsigned int Configuration::getBreedFactor() const {
#ifdef _SCC_DEBUG
    if((breedFactor<MIN_BREED_FACTOR)||(breedFactor>MAX_BREED_FACTOR)) {
        toLog("WARNING: (Configuration::getBreedFactor): Value out of range.");return(MIN_BREED_FACTOR);
    }
#endif
	return(breedFactor);
}

const unsigned int Configuration::getMutationFactor() const {
#ifdef _SCC_DEBUG
    if((mutationFactor<MIN_MUTATION_FACTOR)||(mutationFactor>MAX_MUTATION_FACTOR)) {
        toLog("WARNING: (Configuration::getMutationFactor): Value out of range.");return(MIN_MUTATION_FACTOR);
    }
#endif
	return(mutationFactor);
}
const unsigned int Configuration::getMaxTime() const {
#ifdef _SCC_DEBUG
    if((maxTime<MIN_TIME)||(maxTime>MAX_TIME)) {
        toLog("WARNING: (Configuration::getMaxTime): Value out of range.");return(MIN_TIME);
    }
#endif
	return(maxTime);
}
const unsigned int Configuration::getMaxTimeOut() const {
#ifdef _SCC_DEBUG
    if((maxTimeOut<MIN_TIMEOUT)||(maxTimeOut>MAX_TIMEOUT)) {
        toLog("WARNING: (Configuration::getMaxTimeOut): Value out of range.");return(MIN_TIMEOUT);
    }
#endif
	return(maxTimeOut);
}

const unsigned int Configuration::getMaxLength() const {
#ifdef _SCC_DEBUG
    if((maxLength<MIN_LENGTH)||(maxLength>MAX_LENGTH)) {
        toLog("WARNING: (Configuration::getMaxLength): Value out of range.");return(MIN_LENGTH);
    }
#endif
	return(maxLength);
}
const unsigned int Configuration::getMaxRuns() const 
{
#ifdef _SCC_DEBUG
    if((maxRuns<MIN_RUNS)||(maxRuns>MAX_RUNS)) {
        toLog("WARNING: (Configuration::getMaxRuns): Value out of range.");return(MIN_RUNS);
    }
#endif
	return(maxRuns);
}
const unsigned int Configuration::getMaxGenerations() const 
{
#ifdef _SCC_DEBUG
    if((maxGenerations<MIN_GENERATIONS)||(maxGenerations>MAX_GENERATIONS)) {
        toLog("WARNING: (Configuration::getMaxGenerations): Value out of range.");return(MIN_NOISE);
    }
#endif
	return(maxGenerations);
}
const unsigned int Configuration::getNoise() const {
#ifdef _SCC_DEBUG
    if((noise < MIN_NOISE) || (noise > MAX_NOISE)) {
        toLog("WARNING: (Configuration::getNoise): Value out of range.");return(MIN_NOISE);
    }
#endif
	return(noise);
}

const unsigned int Configuration::getStaticFramerate() const
{
#ifdef _SCC_DEBUG
    if((staticFramerate<MIN_STATIC_FRAMERATE)||(staticFramerate>MAX_STATIC_FRAMERATE)) {
        toLog("WARNING: (Configuration::getStaticFramerate): Value out of range.");return(MIN_STATIC_FRAMERATE);
    }
#endif
    return(staticFramerate);
}

const unsigned int Configuration::getDynamicFramerate() const
{
#ifdef _SCC_DEBUG
    if((dynamicFramerate<MIN_DYNAMIC_FRAMERATE)||(dynamicFramerate>MAX_DYNAMIC_FRAMERATE)) {
        toLog("WARNING: (Configuration::getDynamicFramerate): Value out of range.");return(MIN_DYNAMIC_FRAMERATE);
    }
#endif
    return(dynamicFramerate);
}

const eLanguage Configuration::getLanguage() const {
	return(language);
}

const unsigned int Configuration::getCurrentFramerate() const {
	return(currentFramerate);
}

void Configuration::setCurrentFramerate(const unsigned int frame_rate) {
	currentFramerate = frame_rate;
}

void Configuration::setCurrentFramesPerGeneration(const unsigned int frames_per_generation) {
	currentFramesPerGeneration = frames_per_generation;
}

const unsigned int Configuration::getCurrentFramesPerGeneration() const {
	return(currentFramesPerGeneration);
}

const bool Configuration::isAutoSaveRuns() const {
	return(autoSaveRuns);
}

const bool Configuration::isFacilityMode() const {
	return(facilityMode);
}

const bool Configuration::isPreprocessBuildOrder() const {
	return(preprocessBuildOrder);
}

const bool Configuration::isAllowGoalAdaption() const {
	return(allowGoalAdaption);
}

const bool Configuration::isSoftwareMouse() const {
    return(softwareMouse);
}
                                                                                                                                                            
const bool Configuration::isFullScreen() const {
    return(fullScreen);
}
                                                                                                                                                            
const bool Configuration::isBackgroundBitmap() const {
    return(backgroundBitmap);
}
                                                                                                                                                            
const bool Configuration::isAllowStaticFramerate() const {
    return(allowStaticFramerate);
}
                                                                                                                                                            
const bool Configuration::isGlowingButtons() const {
    return(glowingButtons);
}

const bool Configuration::isDnaSpiral() const {
    return(dnaSpiral);
}
                                                                                                                                                         
const bool Configuration::isTooltips() const {
    return(tooltips);
}
                                                                                                                                                            
const bool Configuration::isTransparency() const {
    return(transparency);
}
                                                                                                                                                            
const bool Configuration::isSmoothMovements() const {
    return(smoothMovements);
}

void Configuration::setCrossingOver(const unsigned int crossing_over) 
{
#ifdef _SCC_DEBUG
    if((crossing_over<MIN_CROSSING_OVER)||(crossing_over>MAX_CROSSING_OVER)) {
        toLog("WARNING: (Configuration::setCrossingOver): Value out of range.");return;
    }
#endif
	crossingOver = crossing_over;
}

void Configuration::setBreedFactor(const unsigned int breed_factor) 
{
#ifdef _SCC_DEBUG
    if((breed_factor<MIN_BREED_FACTOR)||(breed_factor>MAX_BREED_FACTOR)) {
        toLog("WARNING: (Configuration::setBreedFactor): Value out of range.");return;
    }
#endif
	breedFactor = breed_factor;
}
void Configuration::setMutationFactor(const unsigned int mutation_factor) 
{
#ifdef _SCC_DEBUG
    if((mutation_factor<MIN_MUTATION_FACTOR)||(mutation_factor>MAX_MUTATION_FACTOR)) {
        toLog("WARNING: (Configuration::setMutationFactor): Value out of range.");return;
    }
#endif
	mutationFactor = mutation_factor;
}
void Configuration::setMaxTime(const unsigned int max_time) 
{
#ifdef _SCC_DEBUG
    if((max_time<MIN_TIME)||(max_time>MAX_TIME)) {
        toLog("WARNING: (Configuration::setMaxTime): Value out of range.");return;
    }
#endif
	maxTime = max_time;
}

void Configuration::setMaxTimeOut(const unsigned int time_out) 
{
#ifdef _SCC_DEBUG
    if((time_out<MIN_TIMEOUT)||(time_out>MAX_TIMEOUT)) {
        toLog("WARNING: (Configuration::setMaxTimeOut): Value out of range.");return;
    }
#endif
	maxTimeOut = time_out;
}

void Configuration::setMaxLength(const unsigned int max_length) 
{
#ifdef _SCC_DEBUG
    if((max_length<MIN_LENGTH)||(max_length>MAX_LENGTH)) {
        toLog("WARNING: (Configuration::setMaxLength): Value out of range.");return;
    }
#endif
	maxLength = max_length;
}

void Configuration::setMaxRuns(const unsigned int max_runs) 
{
#ifdef _SCC_DEBUG
    if((max_runs<MIN_RUNS)||(max_runs>MAX_RUNS)) {
        toLog("WARNING: (Configuration::setMaxRuns): Value out of range.");return;
    }
#endif
	maxRuns = max_runs;
}

void Configuration::setMaxGenerations(const unsigned int max_generations) 
{
#ifdef _SCC_DEBUG
    if((max_generations<MIN_GENERATIONS)||(max_generations>MAX_GENERATIONS)) {
        toLog("WARNING: (Configuration::setMaxGenerations): Value out of range.");return;
    }
#endif
	maxGenerations = max_generations;
}

void Configuration::setNoise(const unsigned int desired_noise)
{
#ifdef _SCC_DEBUG
    if((desired_noise < MIN_NOISE)||(desired_noise > MAX_NOISE)) {
        toLog("WARNING: (Configuration::setNoise): Value out of range.");return;
    }
#endif
	noise = desired_noise;
}


void Configuration::setDynamicFramerate(const unsigned int dynamic_framerate)
{
#ifdef _SCC_DEBUG
    if((dynamic_framerate<MIN_DYNAMIC_FRAMERATE)||(dynamic_framerate>MAX_DYNAMIC_FRAMERATE)) {
        toLog("WARNING: (Configuration::setDynamicFramerate): Value out of range.");return;
    }
#endif
	dynamicFramerate = dynamic_framerate;
}

void Configuration::setStaticFramerate(const unsigned int frame_rate)
{
#ifdef _SCC_DEBUG
    if((frame_rate<MIN_STATIC_FRAMERATE)||(frame_rate>MAX_STATIC_FRAMERATE)) {
        toLog("WARNING: (Configuration::setStaticFramerate): Value out of range.");return;
    }
#endif
    staticFramerate = frame_rate;
}

void Configuration::setLanguage(const eLanguage current_language) {
	language = current_language;
}

void Configuration::setAutoSaveRuns(const bool auto_save_runs) {
	autoSaveRuns = auto_save_runs;
}

void Configuration::setFacilityMode(const bool facility_mode) {
	facilityMode = facility_mode;
}

void Configuration::setPreprocessBuildOrder(const bool preprocess_build_order) {
	preprocessBuildOrder = preprocess_build_order;
}

void Configuration::setAllowGoalAdaption(const bool allow_goal_adaption) {
	allowGoalAdaption = allow_goal_adaption;
}

void Configuration::setSoftwareMouse(const bool software_mouse) {
	softwareMouse = software_mouse;
}

void Configuration::setFullScreen(const bool full_screen) {
	fullScreen = full_screen;
}

void Configuration::setBackgroundBitmap(const bool background_bitmap) {
	backgroundBitmap = background_bitmap;
}

void Configuration::setAllowStaticFramerate(const bool allow_static_framerate) {
	allowStaticFramerate = allow_static_framerate;
}

void Configuration::setGlowingButtons(const bool glowing_buttons) {
	glowingButtons = glowing_buttons;
}

void Configuration::setDnaSpiral(const bool dna_spiral) {
	dnaSpiral = dna_spiral;
}
void Configuration::setTooltips(const bool tool_tips) {
	tooltips = tool_tips;
}

void Configuration::setTransparency(const bool trans_parency) {
	transparency = trans_parency;
}

void Configuration::setSmoothMovements(const bool smooth_movements) {
	smoothMovements = smooth_movements;
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
pFile << "    \"Language\" = \"" << getLanguage() << "\"" << endl;
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

