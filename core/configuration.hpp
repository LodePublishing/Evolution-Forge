#ifndef _CORE_CONFIGURATION_HPP
#define _CORE_CONFIGURATION_HPP

#include "defs.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <deque>

using namespace std;

class Configuration
{
// This is still a public class though access _SHOULD_ be made through the corresponding functions of settings
private:
	unsigned int crossingOver;			// percentage of population that should exchange their genetic informations with each other
	unsigned int breedFactor;		// percentage of population that should copy 1:1 their genetic informations 
	unsigned int mutationFactor;
	unsigned int maxTime;			// border of global searchspace in seconds (the lower the faster but needs longer to get a valid solution at the first place)
	unsigned int maxTimeOut;			// border of local searchspace in seconds (how long the program should be allowed to wait until prerequisites of an item are fulfilled)
	unsigned int maxLength;			// max length of individual (as maxTime)
	unsigned int maxRuns;			// how many different solutions should be searched
	unsigned int maxGenerations;		// how many unchanged generations shall occur until the run is terminated
	unsigned int noise; //in %
	unsigned int dynamicFramerate; // X Frames per generation
	unsigned int staticFramerate; // hold X FPS

	bool preprocessBuildOrder; // should the program create a basic valid solution? TODO: IMPLEMENT IT!
	bool allowGoalAdaption;
	bool softwareMouse;
	bool fullScreen;
	bool backgroundBitmap;
	bool allowStaticFramerate;
	bool glowingButtons;
	bool tooltips;
	bool transparency;
	bool smoothMovements;

	string configurationFile;
	
public:
	Configuration();
	~Configuration();

	void initDefaults();
	
	const unsigned int getCrossingOver() const;
	const unsigned int getBreedFactor() const;
	const unsigned int getMutationFactor() const;
	const unsigned int getMaxTime() const;
	const unsigned int getMaxTimeOut() const;
	const unsigned int getMaxLength() const;
	const unsigned int getMaxRuns() const;
	const unsigned int getMaxGenerations() const;
	const unsigned int getNoise() const;
	const unsigned int getStaticFramerate() const;
    const unsigned int getDynamicFramerate() const;
	
	const bool isPreprocessBuildOrder() const;
	const bool isAllowGoalAdaption() const;
	const bool isSoftwareMouse() const;
	const bool isFullScreen() const;
	const bool isBackgroundBitmap() const;
	const bool isAllowStaticFramerate() const;
	const bool isGlowingButtons() const;
	const bool isTooltips() const;
	const bool isTransparency() const;
	const bool isSmoothMovements() const;
	
	void setCrossingOver(const unsigned int crossing_over);
	void setBreedFactor(const unsigned int breed_factor);
	void setMutationFactor(const unsigned int mutation_factor);
	void setMaxTime(const unsigned int max_time);
	void setMaxTimeOut(const unsigned int time_out);
	void setMaxLength(const unsigned int max_length);
	void setMaxRuns(const unsigned int max_runs);
	void setMaxGenerations(const unsigned int max_generations);
	void setNoise(const unsigned int noise);
    void setDynamicFramerate(const unsigned int dynamic_framerate);
	void setStaticFramerate(const unsigned int static_frame_rate);
	void setPreprocessBuildOrder(const bool preprocess_build_order);
	void setAllowGoalAdaption(const bool allow_goal_adaption);
	void setSoftwareMouse(const bool software_mouse);
	void setFullScreen(const bool full_screen);
	void setBackgroundBitmap(const bool background_bitmap);
	void setAllowStaticFramerate(const bool allow_static_framerate);
	void setGlowingButtons(const bool glowing_buttons);
	void setTooltips(const bool tool_tips);
	void setTransparency(const bool trans_parency);
	void setSmoothMovements(const bool smooth_movements);
    
	void setConfigurationFile(const string& configurationFile);
	void loadConfigurationFile();
	void saveToFile() const;
};

extern Configuration configuration;

extern void parse_2nd_block(ifstream& stream, map<string, map<string, list<string> > >& block);
extern void parse_block(ifstream& stream, map<string, list<string> >& block);
extern void parse_line(string& text, list<string>& words);


#endif // _CORE_CONFIGURATION_HPP



