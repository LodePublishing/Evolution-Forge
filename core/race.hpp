#ifndef _CORE_RACE_HPP
#define _CORE_RACE_HPP

enum eFacilityType
{
// these are the possible values for facility_type
// they describe what happens to the units while extern constructing / when the building is complete
// error when this comes up!
	NO_FACILITY_BEHAVIOUR_DEFINED,
// needs no facility rules
	NO_FACILITY,
	IS_LOST,
	IS_MORPHING,
// facilities are lost (templars/archons, drones/buildings, command center/comsat, ...), 
	NEEDED_ONCE,
// facility is needed at this place, only once (probes!)
	NEEDED_UNTIL_COMPLETE, // facility is needed until the item is complete (Scvs,ldings in general)
	NEEDED_ONCE_IS_LOST, // Special, probes in facility0!
	NEEDED_UNTIL_COMPLETE_IS_LOST, // Special, scvs in facility0! ~~

	NEEDED_UNTIL_COMPLETE_IS_LOST_EVERYWHERE, // <- researchs!
	
	NEEDED_ALWAYS, //nuke silos/nukes, reaver/scarabs, carrier/interceptors, ...

	IS_LOST_GLOBALLY,
	NEEDED_ONCE_IS_LOST_GLOBALLY,
	NEEDED_UNTIL_COMPLETE_IS_LOST_GLOBALLY,
	
	FACILITY_TYPES
};

// units can optionally be ordered by area (combat, support, research etc.) instead by their facility:
enum eUnitType
{
	INVALID_UNIT_TYPE, // not possible to enqueue in the building queue (like R_*)
	
	WORKER_UNIT_TYPE,
	GAS_MINER_UNIT_TYPE,
	COMBAT_UNIT_TYPE,
	SUPPORT_UNIT_TYPE,
	SUPPLY_UNIT_TYPE,
	PRODUCE_UNIT_TYPE,
	RESEARCH_FACILITY_UNIT_TYPE,
	ADD_ON_UNIT_TYPE,
	RESEARCH_UNIT_TYPE, // also scarabs, interceptors

	ORDER_UNIT_TYPE, // move to gas, wait, etc. => cannot be goals
	SPECIAL_UNIT_TYPE, // possible to enqueue in the building queue but never part of the code (like Larva)

	UNIT_TYPE_TYPES
};


class UNIT_STATISTICS
{
	public:
		UNIT_STATISTICS();
		UNIT_STATISTICS(unsigned int bt, unsigned int res0, unsigned int res1, unsigned int need_supply, unsigned int have_supply, unsigned int prereq0, unsigned int prereq1, unsigned int prereq2, unsigned int fac0, unsigned int fac1, unsigned int fac2, unsigned int f2, eFacilityType facility_type, unsigned int unit_create, eUnitType unit_type, unsigned int affects_resource = 0);
		UNIT_STATISTICS(unsigned int bt, unsigned int res0, unsigned int res1, unsigned int need_supply, unsigned int have_supply, unsigned int prereq0, unsigned int prereq1, unsigned int prereq2, unsigned int fac0, unsigned int fac1, unsigned int fac2, unsigned int f2, eFacilityType facility_type, unsigned int unit_create, eUnitType unit_type, unsigned int hp, unsigned int xp, unsigned int affects_resource = 0);
		~UNIT_STATISTICS();
	
	unsigned int BT; // in frames (slowest: 6 fps, fastest: 24fps)
	unsigned int resource[2];
	unsigned int needSupply;
	unsigned int haveSupply;
	unsigned int prerequisite[3]; // these prerequisites buildings need to be anywhere on the map, so that this unit can be build
	unsigned int facility[3];	 // where _can_ the unit be produced? primariliy for zerg and terra
			             // for upgrades: fac[2] and fac[3] are places to hold prequerisities for additional upgrading beyond level 1~~
	unsigned int facility2;		 // additional facilities, primarily for drones or morphing templars
	eFacilityType facilityType;	 // see above
						 
	unsigned int create;			 // Additional building created when this item is completed (only for add-ons)
	eUnitType unitType;

	unsigned int hp;
	unsigned int xp;
	unsigned int affectsResource; // bitmask of resources this unit will affect, 128+ means that this affects the global harvesting and not just the local economy (for example because of a research that improves resource gathering)
};



struct NEED
{
	const bool facilityIsDone();
	const bool facility2IsDone();
	const bool prerequisiteIsDone();
	NEED& operator=(const NEED& object);
	
	bool facility;
	bool facility2;
	unsigned int prerequisite; // we need all... no double?
};

#include <list>
#include <vector>
struct ALLOW
{
	std::list<unsigned int> facility;
	std::list<unsigned int> facility2;
	std::list<unsigned int> prerequisite;
	void updateBuildableList(std::vector<NEED>& need, std::list<unsigned int>& new_buildable, const unsigned int unit);
};

struct RESEARCH
{
	unsigned int first;
	unsigned int last;
	unsigned int basic;
};

struct RESOURCE
{
	unsigned int provider;
	unsigned int gatherer; // spaeter aus beidem ne linked list machen :o
	std::list<unsigned int> accepter;

	unsigned int gatherAmount;
	unsigned int harvestTime;

	void calculateHarvestSpeed();
	std::vector< std::vector< std::vector< unsigned int > > > resourceHarvestPerFrame; // distance, provider-count, gatherer-count 


	static unsigned int MAX_RESOURCE_WORKERS;
	static unsigned int MAX_RESOURCE_PROVIDER_PER_LOCATION;
	static unsigned int MAX_RESOURCE_DISTANCE;
};

#include <string>
struct RACE
{
	public:
		static unsigned int UNIT_TYPE_COUNT;
		static unsigned int MAX_RESOURCE_TYPES;
		static const unsigned int FIRST_STRING;
		
		std::vector<UNIT_STATISTICS> stats;
		
		std::string raceString; // fuer interne Dinge, z.B. Verzeichnisnamen
		char hotkey; // for racemenu
// is filled externally
//		std::vector< std::vector<std::string> > stringList; //languages! 
//		std::vector<bool> languageInitialized; ?

		std::list<unsigned int> supply;
		std::vector<ALLOW> allows;
		std::vector<NEED> needs;
		RESEARCH research;
		std::vector<RESOURCE> resource;

		void fillInfluenceList();
		void setResearchParameters(const unsigned int first, const unsigned int last, const unsigned int basic);

		void initResources();
		
		std::vector<std::string> unitsStringIdentifier;
		std::vector< std::vector<std::string> > unitsStringList; // languages
		std::vector<bool> languageInitializedForUnitsStrings;
};


#endif

