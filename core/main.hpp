//TODO: Problem: auf goals muessen sowohl ueber jeweiligen Ort (also gGoal[MAX_LOCATIONS][MAX_GOALS]), als auch einzelne Goals, wenn z.B. nach 5 und 10 Minuten 3 und 5 Marines an einem Ort sein sollen...

#ifndef _CORE_MAIN_HPP
#define _CORE_MAIN_HPP

#include <string>
using namespace std;

#ifdef BUILD_DLL
#define EXPORT __declspec(dllexport)
#elif IMPORT_DLL
#define EXPORT __declspec(dllimport)
#else
#define EXPORT
#endif

const float CORE_VERSION=0.50;

const int MAX_LOCATIONS = 9;
const int MIN_LOCATIONS = 0; //this does not mean that maps with 0 locations can exist....

const int MAX_PLAYER = 3;
const int MIN_PLAYER = 1;

const int MAX_GOAL_ENTRIES = 25;
const int MIN_GOAL_ENTRIES = 0;

const int MAX_MINERALS = 5000000;
const int MAX_GAS = 5000000;

const int MAX_MAPS = 25;
const int MIN_MAPS = 0;

const int MAX_PROGRAMS = 128; //must be multiplier of (16*player)
const int LARVA_MAX = 200;
const int UNIT_TYPE_COUNT = 101;
const int MAX_GOALS = 100; // count of possible different goals
const int MAX_HARVEST_SPEEDS = 100;
const int MAX_SUPPLY = 200;

const int MAX_GENERATIONS = 10000;
const int MIN_GENERATIONS = 100;

const int MAX_BREED_FACTOR = 40;
const int MIN_BREED_FACTOR = 0;

const int MAX_MODE = 2;
const int MIN_MODE = 0;

const int MAX_CROSSOVER = 40;
const int MIN_CROSSOVER = 0;

const int MAX_TIME = 3600;
const int MIN_TIME = 300;

const int MAX_TIMEOUT = 266;
const int MIN_TIMEOUT = 40;

const int MAX_LENGTH = 96;
const int MIN_LENGTH = 32;

const int MAX_RUNS = 10;
const int MIN_RUNS = 1;

const int MAX_PREPROCESS_BUILDORDER = 1;
const int MIN_PREPROCESS_BUILDORDER = 0;

const int MAX_TFITNESS = 99999;

const int MAX_START_CONDITIONS = 99; // ~~

#ifdef _SCC_DEBUG
#include <fstream>
void toLog(const string& msg);
#endif

struct LAST
{
	int unit;
	int location;
	int count;
};


struct GOAL
{
	int unit;//!
	int time;
	int count;
	int location;
};


enum eErrorMessages
{
	NO_ERROR,
	OK,
	ENOUGH_MINERALS_WERE_GATHERED,
	ENOUGH_GAS_WAS_GATHERED,
	SUPPLY_WAS_SATISFIED,
	PREREQUISITE_WAS_FULFILLED,
	FACILITY_BECAME_AVAILIBLE,
	TIMEOUT_ERROR,
	UNKNOWN_ERROR,
	ERROR_MESSAGES
};

enum eRace
{
	TERRA,
	PROTOSS,
	ZERG
};

#define MAX_RACES (ZERG+1)

enum eTerraUnits
{
	NONE,
	SCV,
	MARINE,
	GHOST,
	VULTURE,
	GOLIATH,
	SIEGE_TANK,
	FIREBAT,
	MEDIC,
	WRAITH,
	SCIENCE_VESSEL,
	DROPSHIP,
	BATTLE_CRUISER,
	VALKYRIE,
	NUCLEAR_WARHEAD,
	SUPPLY_DEPOT,
	BARRACKS,
	ACADEMY,
	FACTORY,
	COMMAND_CENTER,
	STARPORT,
	SCIENCE_FACILITY,
	ENGINEERING_BAY,
	ARMORY,
	MISSILE_TURRET,
	BUNKER,
	COMSAT_STATION,
	NUCLEAR_SILO,
	CONTROL_TOWER,
	COVERT_OPS,
	PHYSICS_LAB,
	MACHINE_SHOP,
	COMMAND_CENTER_CS,
	COMMAND_CENTER_NS,
	STARPORT_CT,
	SCIENCE_FACILITY_CO,
	SCIENCE_FACILITY_PL,
	FACTORY_MS,
	STIM_PACKS,
	LOCKDOWN,
	EMP_SHOCKWAVE,
	SPIDER_MINES,
	TANK_SIEGE_MODE,
	IRRADIATE,
	YAMATO_GUN,
	CLOAKING_FIELD,
	PERSONNEL_CLOAKING,
	RESTORATION,
	OPTICAL_FLARE,
	U238_SHELLS,
	ION_THRUSTERS,
	TITAN_REACTOR,
	OCULAR_IMPLANTS,
	MOEBIUS_REACTOR,
	APOLLO_REACTOR,
	COLOSSUS_REACTOR,
	CADUCEUS_REACTOR,
	CHARON_BOOSTER,
	INFANTRY_ARMOR,
	INFANTRY_WEAPONS,
	VEHICLE_PLATING,
	VEHICLE_WEAPONS,
	SHIP_PLATING,
	SHIP_WEAPONS,
	REFINERY, // <- must be set constant to 64
	GAS_SCV,
	
	MOVE_ONE_3_FORWARD,
	MOVE_ONE_1_FORWARD,
	MOVE_ONE_1_BACKWARD,
	ROTATE,
	FOLLOW,
	RESET,

	VESPENE_GEYSIR,
	MINERAL_PATCH,

	R_STIM_PACKS,
	R_LOCKDOWN,
	R_EMP_SHOCKWAVE,
	R_SPIDER_MINES,
	R_TANK_SIEGE_MODE,
	R_IRRADIATE,
	R_YAMATO_GUN,
	R_CLOAKING_FIELD,
	R_PERSONNEL_CLOAKING,
	R_RESTORATION,
	R_OPTICAL_FLARE,
	R_U238_SHELLS,
	R_ION_THRUSTERS,
	R_TITAN_REACTOR,
	R_OCULAR_IMPLANTS,
	R_MOEBIUS_REACTOR,
	R_APOLLO_REACTOR,
	R_COLOSSUS_REACTOR,
	R_CADUCEUS_REACTOR,
	R_CHARON_BOOSTER,
	R_INFANTRY_ARMOR,
	R_INFANTRY_WEAPONS,
	R_VEHICLE_PLATING,
	R_VEHICLE_WEAPONS,
	R_SHIP_PLATING,
	R_SHIP_WEAPONS,
	INTRON,
//}; // end eTerranUnits
//
//enum eProtossUnits
//{
	PROBE = 1,
	DARK_TEMPLAR,
	DARK_ARCHON,
	ZEALOT,
	DRAGOON,
	HIGH_TEMPLAR,
	ARCHON,
	REAVER,
	HALF_SCARAB,
	FULL_SCARAB,
	CORSAIR,
	SHUTTLE,
	SCOUT,
	ARBITER,
	CARRIER,
	HALF_INTERCEPTOR,
	FULL_INTERCEPTOR,
	OBSERVER,
	NEXUS,
	ROBOTICS_FACILITY,
	PYLON,
	OBSERVATORY,
	GATEWAY,
	PHOTON_CANNON,
	CYBERNETICS_CORE,
	CITADEL_OF_ADUN,
	TEMPLAR_ARCHIVES,
	FORGE,
	STARGATE,
	FLEET_BEACON,
	ARBITER_TRIBUNAL,
	ROBOTICS_SUPPORT_BAY,
	SHIELD_BATTERY,
	PSIONIC_STORM,
	HALLUCINATION,
	RECALL,
	STASIS_FIELD,
	DISRUPTION_WEB,
	MIND_CONTROL,
	MAELSTROM,
	SINGULARITY_CHARGE,
	LEG_ENHANCEMENTS,
	SCARAB_DAMAGE,
	REAVER_CAPACITY,
	GRAVITIC_DRIVE,
	SENSOR_ARRAY,
	GRAVITIC_BOOSTERS,
	KHAYDARIN_AMULET,
	APIAL_SENSORS,
	GRAVITIC_THRUSTERS,
	CARRIER_CAPACITY,
	KHAYDARIN_CORE,
	ARGUS_JEWEL,
	ARGUS_TALISMAN,
	ARMOR,
	PLATING,
	GROUND_WEAPONS,
	AIR_WEAPONS,
	PLASMA_SHIELDS,
	ASSIMILATOR = 64,
	GAS_PROBE,

	R_PSIONIC_STORM = 74,
	R_HALLUCINATION,
	R_RECALL,
	R_STASIS_FIELD,
	R_DISRUPTION_WEB,
	R_MIND_CONTROL,
	R_MAELSTROM,
	R_SINGULARITY_CHARGE,
	R_LEG_ENHANCEMENTS,
	R_SCARAB_DAMAGE,
	R_REAVER_CAPACITY,
	R_GRAVITIC_DRIVE,
	R_SENSOR_ARRAY,
	R_GRAVITIC_BOOSTERS,
	R_KHAYDARIN_AMULET,
	R_APIAL_SENSORS,
	R_GRAVITIC_THRUSTERS,
	R_CARRIER_CAPACITY,
	R_KHAYDARIN_CORE,
	R_ARGUS_JEWEL,
	R_ARGUS_TALISMAN,
	R_ARMOR,
	R_PLATING,
	R_GROUND_WEAPONS,
	R_AIR_WEAPONS,
	R_PLASMA_SHIELDS,
//};
//
//enum eZergUnits
//{
	DRONE = 1,
	LARVA,
	ZERGLING,
	HYDRALISK,
	ULTRALISK,
	DEFILER,
	LURKER,
	OVERLORD,
	MUTALISK,
	GUARDIEN,
	QUEEN,
	SCOURGE,
	DEVOURER,
	LAIR,
	HIVE,
	NYDUS_CANAL,
	HYDRALISK_DEN,
	DEFILER_MOUND,
	HATCHERY,
	GREATER_SPIRE,
	QUEENS_NEST,
	EVOLUTION_CHAMBER,
	ULTRALISK_CAVERN,
	SPIRE,
	SPAWNING_POOL,
	CREEP_COLONY,
	SPORE_COLONY,
	SUNKEN_COLONY,
	VENTRAL_SACKS,
	ANTENNAE,
	PNEUMATIZED_CARAPACE,
	METABOLIC_BOOST,
	ADRENAL_GLANDS,
	MUSCULAR_AUGMENTS,
	GROOVED_SPINES,
	GAMETE_MEIOSIS,
	METASYNAPTIC_NODE,
	CHITINOUS_PLATING,
	ANABOLIC_SYNTHESIS,
	BURROWING,
	SPAWN_BROODLING,
	PLAGUE,
	CONSUME,
	ENSNARE,
	LURKER_ASPECT,
	CARAPACE,
	FLYER_CARAPACE,
	MELEE_ATTACKS,
	MISSILE_ATTACKS,
	FLYER_ATTACKS,
//	CREEP_COLONY,
	EXTRACTOR = 64,
	GAS_DRONE,

	BREAK_UP_BUILDING = 74,
	R_VENTRAL_SACKS,
	R_ANTENNAE,
	R_PNEUMATIZED_CARAPACE,
	R_METABOLIC_BOOST,
	R_ADRENAL_GLANDS,
	R_MUSCULAR_AUGMENTS,
	R_GROOVED_SPINES,
	R_GAMETE_MEIOSIS,
	R_METASYNAPTIC_NODE,
	R_CHITINOUS_PLATING,
	R_ANABOLIC_SYNTHESIS,
	R_BURROWING,
	R_SPAWN_BROODLING,
	R_PLAGUE,
	R_CONSUME,
	R_ENSNARE,
	R_LURKER_ASPECT,
	R_CARAPACE,
	R_FLYER_CARAPACE,
	R_MELEE_ATTACKS,
	R_MISSILE_ATTACKS,
	R_FLYER_ATTACKS
}; // end eZergUnits


enum eFacilityType
{
// these are the possible values for facility_type
// they describe what happens to the units while constructing / when the building is complete
	// error when this comes up!
	NO_FACILITY_BEHAVIOUR_DEFINED,
// needs no facility rules
	NO_FACILITY,
	IS_LOST,
// facilities are lost (templars/archons, drones/buildings, command center/comsat, ...), 
	NEEDED_ONCE,
// facility is needed at this place, only once (probes!)
	NEEDED_UNTIL_COMPLETE, // facility is needed until the item is complete (Scvs,ldings in general)
	NEEDED_ONCE_IS_LOST, // Special, probes in facility0!
	NEEDED_UNTIL_COMPLETE_IS_LOST, // Special, scvs in facility0!
	NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, //availible returns to 1 but one item is lost... kinda hack for upgrades :)
	NEEDED_ALWAYS, //nuke silos/nukes, reaver/scarabs, carrier/interceptors, ...
	FACILITY_TYPES
};

// units can optionally be ordered by area (combat, support, research etc.) instead by their facility:
enum eUnitType
{
	REMAINING_UNIT_TYPE,
	WORKER_UNIT_TYPE,
	GAS_MINER_UNIT_TYPE,
	COMBAT_UNIT_TYPE,
	SUPPORT_UNIT_TYPE,
	SUPPLY_UNIT_TYPE,
	PRODUCE_UNIT_TYPE,
	RESEARCH_FACILITY_UNIT_TYPE,
	ADD_ON_UNIT_TYPE,
	RESEARCH_UNIT_TYPE,
	SPECIAL_UNIT_TYPE,
	UNIT_TYPE_TYPES
};	

struct UNIT_STATISTICS
{
	string name;
	int BT;
	int minerals,gas;
	int needSupply;
	int haveSupply;
	int upgrade_cost;	 // upgrade costs in minerals and gas (50, 75, ...)
	int upgrade[2];		 // what building is needed to go upgrade level 2 and level 3 (templar archives, science facility, fleet beacon, lair, hive)
	int prerequisite[3]; // these prerequisites buildings need to be anywhere on the map, so that this unit can be build
	int facility[3];	 // where _can_ the unit be produced? primariliy for zerg and terra
			             // for upgrades: fac[2] and fac[3] are places to hold prequerisities for additional upgrading beyond level 1~~
	int facility2;		 // additional facilities, primarily for drones or morphing templars
	eFacilityType facilityType;	 // see above
						 
	int create;			 // Additional building created when this item is completed (only for add-ons)
	int speed;			 // speed of units, not yet implemented
	eUnitType unitType;		
};


const UNIT_STATISTICS stats[MAX_RACES][UNIT_TYPE_COUNT]=
{
{//n                          t     m     g  ns hs uc up      prere       fac     fac2    facility_type         create speed  unit type
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 100, REMAINING_UNIT_TYPE},
{"SCV"	,					 20, 5000,    0,  1, 0, 0, {0, 0}, {0 ,0, 0}, {COMMAND_CENTER, COMMAND_CENTER_CS, COMMAND_CENTER_NS}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, WORKER_UNIT_TYPE},
{"Marine", 			 		 24, 5000,    0,  1, 0, 0, {0, 0}, {0 ,0, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Ghost",					 50, 2500, 7500,  1, 0, 0, {0, 0}, {ACADEMY ,COVERT_OPS, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Vulture",					 30, 7500,    0,  2, 0, 0, {0, 0}, {FACTORY ,0, 0}, {FACTORY, FACTORY_MS, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Goliath",					 40,10000, 5000,  2, 0, 0, {0, 0}, {ARMORY, 0, 0}, {FACTORY, FACTORY_MS, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Siege Tank", 				 50,15000,10000,  2, 0, 0, {0, 0}, {0 ,0, 0}, {FACTORY_MS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Firebat",					 24, 5000, 2500,  1, 0, 0, {0, 0}, {ACADEMY ,0, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Medic", 					 30, 5000, 2500,  1, 0, 0, {0, 0}, {ACADEMY ,0, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Wraith", 					 60,15000,10000,  2, 0, 0, {0, 0}, {0 ,0, 0}, {STARPORT, STARPORT_CT, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Science Vessel", 			 80,10000,22500,  2, 0, 0, {0, 0}, {SCIENCE_FACILITY, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Dropship", 				 50,10000,10000,  2, 0, 0, {0, 0}, {0, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Battle Cruiser",			133,40000,30000,  6, 0, 0, {0, 0}, {PHYSICS_LAB, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Valkyrie", 				 50,25000,12500,  3, 0, 0, {0, 0}, {ARMORY, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Nuclear Warhead",			100,20000,20000,  8, 0, 0, {0, 0}, {COMMAND_CENTER_NS, 0, 0}, {NUCLEAR_SILO, 0, 0}, 0, NEEDED_ALWAYS, 0, 0, SUPPORT_UNIT_TYPE},
{"Supply Depot", 			 40,10000,    0,  0, 8, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0, SUPPLY_UNIT_TYPE},
{"Barracks", 				 80,15000,    0,  0, 0, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10, PRODUCE_UNIT_TYPE},
{"Academy", 				 80,15000,    0,  0, 0, 0, {0, 0}, {BARRACKS ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Factory", 				 80,20000,10000,  0, 0, 0, {0, 0}, {BARRACKS ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10, PRODUCE_UNIT_TYPE},
{"Command Center",			120,40000,    0,  0,10, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10, SUPPLY_UNIT_TYPE},
{"Starport", 				 70,15000,10000,  0, 0, 0, {0, 0}, {FACTORY ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10, PRODUCE_UNIT_TYPE},
{"Science Facility", 		 60,10000,15000,  0, 0, 0, {0, 0}, {STARPORT ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10, RESEARCH_FACILITY_UNIT_TYPE},
{"Engineering Bay", 		 60,12500,    0,  0, 0, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10, RESEARCH_FACILITY_UNIT_TYPE},
{"Armory", 					 80,10000, 5000,  0, 0, 0, {0, 0}, {FACTORY, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Missile Turret", 			 30, 7500,    0,  0, 0, 0, {0, 0}, {ENGINEERING_BAY, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0, SUPPORT_UNIT_TYPE},
{"Bunker", 					 30,10000,    0,  0, 0, 0, {0, 0}, {BARRACKS, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0, SUPPORT_UNIT_TYPE},
{"Comsat Station",  		  0,    0,    0,  0, 0, 0, {0, 0}, {COMSAT_STATION, COMMAND_CENTER_CS, 0}, {0, 0 ,0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE}, 
{"Nuclear Silo",  			  0,    0,    0,  0, 0, 0, {0, 0}, {NUCLEAR_SILO, COMMAND_CENTER_NS, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"Control Tower",  			  0,    0,    0,  0, 0, 0, {0, 0}, {CONTROL_TOWER, STARPORT_CT, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"Covert Ops",  			  0,    0,    0,  0, 0, 0, {0, 0}, {COVERT_OPS, SCIENCE_FACILITY_CO, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"Physics Lab",  			  0,    0,    0,  0, 0, 0, {0, 0}, {PHYSICS_LAB, SCIENCE_FACILITY_PL, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"Machine Shop",  			  0,    0,    0,  0, 0, 0, {0, 0}, {MACHINE_SHOP, FACTORY_MS, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"Command Center[CS]", 		 40, 5000, 5000,  0,10, 0, {0, 0}, {ACADEMY, 0, 0}, {COMMAND_CENTER, 0, 0}, 0, IS_LOST, COMSAT_STATION, 0, ADD_ON_UNIT_TYPE}, //TODO: Beim Abheben zerstoeren...
{"Command Center[NS]", 		 40,10000,10000,  0,10, 0, {0, 0}, {COVERT_OPS, 0, 0}, {COMMAND_CENTER, 0, 0}, 0, IS_LOST, NUCLEAR_SILO, 0, ADD_ON_UNIT_TYPE},
{"Starport[CT]", 			 80, 5000, 5000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {STARPORT, 0, 0}, 0, IS_LOST, CONTROL_TOWER, 0, ADD_ON_UNIT_TYPE},
{"Science Facility[CO]", 	 40, 5000, 5000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, 0, 0}, 0, IS_LOST, COVERT_OPS, 0, ADD_ON_UNIT_TYPE},
{"Science Facility[PL]", 	 40, 5000, 5000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, 0, 0}, 0, IS_LOST, PHYSICS_LAB, 0, ADD_ON_UNIT_TYPE},
{"Factory[MS]", 			 40, 5000, 5000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {FACTORY, 0, 0}, 0, IS_LOST, MACHINE_SHOP, 0, ADD_ON_UNIT_TYPE},
{"Stim Packs", 				 80,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_STIM_PACKS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Lockdown",				100,20000,20000,  0, 0, 0, {0, 0}, {SCIENCE_FACILITY_CO, 0, 0}, {COVERT_OPS, 0, 0}, R_LOCKDOWN, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"EMP Shockwave",			120,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, SCIENCE_FACILITY_CO, SCIENCE_FACILITY_PL}, R_EMP_SHOCKWAVE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Spider Mines", 			 80,10000,10000,  0, 0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_SPIDER_MINES, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Siege Mode", 				 80,15000,15000,  0, 0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_TANK_SIEGE_MODE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Irradiate", 				 80,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, SCIENCE_FACILITY_CO, SCIENCE_FACILITY_PL}, R_IRRADIATE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Yamato Gun",				120,10000,10000,  0, 0, 0, {0, 0}, {SCIENCE_FACILITY_PL ,0, 0}, {PHYSICS_LAB, 0, 0}, R_YAMATO_GUN, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Cloaking Field",			100,15000,15000,  0, 0, 0, {0, 0}, {STARPORT_CT ,0, 0}, {CONTROL_TOWER, 0, 0}, R_CLOAKING_FIELD, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Personnel Cloaking", 		 80,10000,10000,  0, 0, 0, {0, 0}, {SCIENCE_FACILITY_CO ,0, 0}, {COVERT_OPS, 0, 0}, R_PERSONNEL_CLOAKING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Restoration", 			 80,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_RESTORATION, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Optical Flare",			120,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_OPTICAL_FLARE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"U-238 Shells",			100,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_U238_SHELLS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Ion Thrusters",			100,10000,10000,  0, 0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_ION_THRUSTERS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Titan Reactor",			166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, SCIENCE_FACILITY_CO, SCIENCE_FACILITY_PL}, R_TITAN_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Ocular Implants",			166,10000,10000,  0, 0, 0, {0, 0}, {SCIENCE_FACILITY_CO ,0, 0}, {COVERT_OPS, 0, 0}, R_OCULAR_IMPLANTS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Moebius Reactor",			166,20000,20000,  0, 0, 0, {0, 0}, {SCIENCE_FACILITY_CO ,0, 0}, {COVERT_OPS, 0, 0}, R_MOEBIUS_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Apollo Reactor",			166,20000,20000,  0, 0, 0, {0, 0}, {STARPORT_CT ,0, 0}, {CONTROL_TOWER, 0, 0}, R_APOLLO_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Colossus Reactor",		166,15000,15000,  0, 0, 0, {0, 0}, {SCIENCE_FACILITY_PL ,0, 0}, {PHYSICS_LAB, 0, 0}, R_COLOSSUS_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Caduceus Reactor",		166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_CADUCEUS_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Charon Booster",			133,10000,10000,  0, 0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_CHARON_BOOSTER, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Infantry Armor",			266,10000,10000,  0, 0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ENGINEERING_BAY, 0, 0}, R_INFANTRY_ARMOR, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Infantry Weapons",		266,10000,10000,  0, 0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ENGINEERING_BAY, 0, 0}, R_INFANTRY_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Vehicle Plating",			266,10000,10000,  0, 0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_VEHICLE_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Vehicle Weapons",			266,10000,10000,  0, 0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_VEHICLE_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Ship Plating",			266,15000,15000,  0, 0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_SHIP_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Ship Weapons",			266,10000,10000,  0, 0, 5000, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_SHIP_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Refinery", 				 40,10000,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {SCV, 0, 0}, 0/*VESPENE_GEYSIR*/, NEEDED_UNTIL_COMPLETE_IS_LOST/*_IS_LOST*/, GAS_SCV, 0, SUPPLY_UNIT_TYPE}, //~~~ueberlegen...
{"Gas SCV",					  3,    0,    0,  1, 0, 0, {0, 0}, {REFINERY ,0, 0}, {SCV/*REFINERY*/, 0, 0}, /*SCV*/0, /*NEEDED_ONCE_*/IS_LOST, 0, 0, GAS_MINER_UNIT_TYPE},	//~~~~  warum 3,0,0,1?
{"Move 3 Forward",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move 1 Forward",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move 1 Backward",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Window move prev",		  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move from here",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move to here",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},

{"Vespene Geysir",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"Minerals",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Stim Packs",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Lockdown",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_EMP Shockwave",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Spider Mines",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Siege Mode",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Irradiate",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Yamato Gun",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Cloaking Field",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Personnel Cloaking",	  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Restoration",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Optical Flare",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_U-238 Shells",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Ion Thrusters",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Titan Reactor",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Ocular Implants",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Moebius Reactor",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Apollo Reactor",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Colossus Reactor",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Caduceus Reactor",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Charon Booster",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Infantry Armor",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Infantry Weapons",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Vehicle Plating",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Vehicle Weapons",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Ship Plating",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Ship Weapons",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"INTRON",					  0,    0,    0,  0, 0, 0, {0, 0}, {0, 0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE}
},

 //for colors... 0= rest, 1= scv, 2= gas scv, 3= combat units, 4=support, 5=supply/refinery, 6=produce buildings, 7=research buildigns, 8= addons, 9=researchs, 10= special


{
{"NULL",  					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"Probe",					 20, 5000,    0,  1, 0, 0, {0, 0}, {0 ,0, 0}, {NEXUS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, WORKER_UNIT_TYPE},
{"Dark Templar",			 50,12500,10000,  2, 0, 0, {0, 0}, {TEMPLAR_ARCHIVES ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Dark Archon",				 20,    0,    0,  2, 0, 0, {0, 0}, {0 ,0, 0}, {DARK_TEMPLAR, 0, 0}, DARK_TEMPLAR, IS_LOST, 0, 100, COMBAT_UNIT_TYPE},
{"Zealot",					 40,10000,    0,  2, 0, 0, {0, 0}, {0 ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Dragoon",					 50,12500, 5000,  2, 0, 0, {0, 0}, {CYBERNETICS_CORE ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"High Templar",			 50, 5000,15000,  2, 0, 0, {0, 0}, {TEMPLAR_ARCHIVES ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Archon",					 20,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {HIGH_TEMPLAR, 0, 0}, HIGH_TEMPLAR, IS_LOST, 0, 100, COMBAT_UNIT_TYPE},
{"Reaver",					 70,20000,10000,  4, 0, 0, {0, 0}, {ROBOTICS_SUPPORT_BAY ,0, 0}, {ROBOTICS_FACILITY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE}, 
{"5xScarab",				 35, 7500,    0,  0, 0, 0, {0, 0}, {0, 0, 0}, {REAVER, 0, 0}, 0, NEEDED_ALWAYS, 0, 0, SPECIAL_UNIT_TYPE},
{"10xScarab",				 35, 7500,    0,  0, 0, 0, {0, 0}, {REAVER_CAPACITY, 0, 0}, {HALF_SCARAB, 0, 0}, 0, IS_LOST, 0, 0, SPECIAL_UNIT_TYPE}, //~~ special rule... reaver is no longer availible :/
{"Corsair",					 40,15000,10000,  2, 0, 0, {0, 0}, {0 ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Shuttle",					 60,20000,    0,  2, 0, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_FACILITY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Scout",					 80,27500,12500,  3, 0, 0, {0, 0}, {0 ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"Arbiter",					160,10000,35000,  4, 0, 0, {0, 0}, {ARBITER_TRIBUNAL ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Carrier",					140,35000,25000,  6, 0, 0, {0, 0}, {FLEET_BEACON ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, COMBAT_UNIT_TYPE},
{"5xInterceptor",			100,25000,    0,  0, 0, 0, {0, 0}, {0,0, REMAINING_UNIT_TYPE}, {CARRIER, 0, 0}, 0, NEEDED_ALWAYS, 0, 0, SPECIAL_UNIT_TYPE},
{"8xInterceptor",			 60,15000,    0,  0, 0, 0, {0, 0}, {CARRIER_CAPACITY,0, REMAINING_UNIT_TYPE}, {HALF_INTERCEPTOR, 0, 0}, 0, IS_LOST, 0, 0, SPECIAL_UNIT_TYPE}, //~~~ Special rule... carrier is no longer availible :/
{"Observer",				 40, 2500, 7500,  1, 0, 0, {0, 0}, {OBSERVATORY ,0, 0}, {ROBOTICS_FACILITY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100, SUPPORT_UNIT_TYPE},
{"Nexus",					120,40000,    0,  0, 9, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, SUPPLY_UNIT_TYPE},
{"Robotics Facility",		 80,20000,20000,  0, 0, 0, {0, 0}, {CYBERNETICS_CORE ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, PRODUCE_UNIT_TYPE},
{"Pylon",					 30,10000,    0,  0, 8, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, SUPPLY_UNIT_TYPE},
{"Observatory",				 30, 5000,10000,  0, 0, 0, {0, 0}, {ROBOTICS_FACILITY ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Gateway",					 60,15000,    0,  0, 0, 0, {0, 0}, {PYLON ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, PRODUCE_UNIT_TYPE},
{"Photon Cannon",			 50,15000,    0,  0, 0, 0, {0, 0}, {FORGE ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, SUPPORT_UNIT_TYPE},
{"Cybernetics Core",		 60,20000,    0,  0, 0, 0, {0, 0}, {GATEWAY ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Citadel Of Adun",			 60,15000,10000,  0, 0, 0, {0, 0}, {CYBERNETICS_CORE ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Templar Archives",		 60,15000,20000,  0, 0, 0, {0, 0}, {CITADEL_OF_ADUN ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Forge",					 40,15000,    0,  0, 0, 0, {0, 0}, {PYLON ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Stargate",				 70,15000,15000,  0, 0, 0, {0, 0}, {CYBERNETICS_CORE ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, PRODUCE_UNIT_TYPE},
{"Fleet Beacon",			 60,30000,20000,  0, 0, 0, {0, 0}, {STARGATE ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Arbiter Tribunal",		 60,20000,15000,  0, 0, 0, {0, 0}, {TEMPLAR_ARCHIVES ,STARGATE, PYLON}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Robotics Support Bay",	 30,15000,10000,  0, 0, 0, {0, 0}, {ROBOTICS_FACILITY ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Shield Battery",			 30,10000,    0,  0, 0, 0, {0, 0}, {GATEWAY ,PYLON, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0, SUPPORT_UNIT_TYPE},
{"Psionic Storm",			120,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_PSIONIC_STORM, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Hallucination",			 80,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_HALLUCINATION, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Recall",					120,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ARBITER_TRIBUNAL, 0, 0}, R_RECALL, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Stasis Field",			100,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ARBITER_TRIBUNAL, 0, 0}, R_STASIS_FIELD, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Disruption Web",			 80,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_DISRUPTION_WEB, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Mind Control",			120,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_MIND_CONTROL, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Maelstrom",				100,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_MAELSTROM, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Singularity Charge",		166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {CYBERNETICS_CORE, 0, 0}, R_SINGULARITY_CHARGE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Leg Enhancements",		133,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {CITADEL_OF_ADUN, 0, 0}, R_LEG_ENHANCEMENTS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Scarab Damage",			166,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_SUPPORT_BAY, 0, 0}, R_SCARAB_DAMAGE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Reaver Capacity",			166,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_SUPPORT_BAY, 0, 0}, R_REAVER_CAPACITY, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Gravitic Drive",			166,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_SUPPORT_BAY, 0, 0}, R_GRAVITIC_DRIVE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Sensor Array",			133,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {OBSERVATORY, 0, 0}, R_SENSOR_ARRAY, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Gravitic Boosters",		133,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {OBSERVATORY, 0, 0}, R_GRAVITIC_BOOSTERS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Khaydarin Amulet",		166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_KHAYDARIN_AMULET, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Apial Sensors",			166,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_APIAL_SENSORS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Gravitic Thrusters",		166,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_GRAVITIC_THRUSTERS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Carrier Capacity",		100,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_CARRIER_CAPACITY, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Khaydarin Core",			166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ARBITER_TRIBUNAL, 0, 0}, R_KHAYDARIN_CORE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Argus Jewel",				166,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_ARGUS_JEWEL, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Argus Talisman",			166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_ARGUS_TALISMAN, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Armor",					266,10000,10000,  0, 0, 7500, {TEMPLAR_ARCHIVES, TEMPLAR_ARCHIVES}, {0 ,0, 0}, {FORGE, 0, 0}, R_ARMOR, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Plating",					266,15000,15000,  0, 0, 7500, {FLEET_BEACON, FLEET_BEACON}, {0 ,0, 0}, {CYBERNETICS_CORE, 0, 0}, R_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Ground Weapons",			266,10000,10000,  0, 0, 5000, {TEMPLAR_ARCHIVES, TEMPLAR_ARCHIVES}, {0 ,0, 0}, {FORGE, 0, 0}, R_GROUND_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Air Weapons",				266,10000,10000,  0, 0, 7500, {FLEET_BEACON, FLEET_BEACON}, {0 ,0, 0}, {CYBERNETICS_CORE, 0, 0}, R_AIR_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Plasma Shields",			266,20000,20000,  0, 0, 10000, {0, 0}, {0 ,0, 0}, {FORGE, 0, 0}, R_PLASMA_SHIELDS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"Assimilator",				 40,10000,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, /*VESPENE_GEYSIR*/0, NEEDED_ONCE/*_IS_LOST*/, 0, 0, SUPPLY_UNIT_TYPE},
{"Gas Probe",				  3,    0,    0,  1, 0, 0, {0, 0}, {ASSIMILATOR ,0, 0}, {PROBE/*ASSIMILATOR*/, 0, 0}, 0/*PROBE*/, /*NEEDED_ONCE_*/IS_LOST, 0, 0, GAS_MINER_UNIT_TYPE},	//Needed once? ~~~ wenn hier ne Aenderung, Facility2 in race und anarace wieder auf location availible checken !TODO
{"Window move+++",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Window move+",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Window move-",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Window move prev",		  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move from here",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move to here",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},

{"Vespene Geysir",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"Minerals",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Psionic Storm",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Hallucination",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Recall",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Stasis Field",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Disruption Web",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Mind Control",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Maelstrom",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Singularity Charge",	  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Leg Enhancements",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Scarab Damage",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Reaver Capacity",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Gravitic Drive",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Sensor Array",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Gravitic Boosters",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Khaydarin Amulet",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Apial Sensors",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Gravitic Thrusters",	  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Carrier Capacity",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Khaydarin Core",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Argus Jewel",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Argus Talisman",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Armor",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Plating",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Ground Weapons",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Air Weapons",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Plasma Shields",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"INTRON",					  0,    0,    0,  0, 0, 0, {0, 0}, {0, 0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE}
},
 //for colors... 0= rest, 1= scv, 2= gas scv, 3= combat units, 4=support, 5=supply/refinery, 6=produce buildings, 7=research buildigns, 8= addons, 9=researchs, 10= special
{
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0, 0, 0}, {0 ,0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"Drone",					 23, 5000,    0,  1, 0, 0, {0, 0}, {0 ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, WORKER_UNIT_TYPE},
{"Larva",					 20,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NEEDED_ONCE, 0, 0, SPECIAL_UNIT_TYPE},
{"Zergling",				 28, 5000,    0,  1, 0, 0, {0, 0}, {SPAWNING_POOL ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, COMBAT_UNIT_TYPE},
{"Hydralisk",				 28, 7500, 2500,  1, 0, 0, {0, 0}, {HYDRALISK_DEN ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, COMBAT_UNIT_TYPE}, //Hier stand ein Zergling als facility2!?
{"Ultralisk",				 60,20000,20000,  6, 0, 0, {0, 0}, {ULTRALISK_CAVERN ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, COMBAT_UNIT_TYPE},
{"Defiler",					 50, 5000,15000,  2, 0, 0, {0, 0}, {DEFILER_MOUND ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, SUPPORT_UNIT_TYPE},
{"Lurker",					 40, 5000,10000,  2, 0, 0, {0, 0}, {HYDRALISK_DEN ,0, 0}, {HYDRALISK, 0, 0}, 0, IS_LOST, 0, 100, COMBAT_UNIT_TYPE},
{"Overlord",				 40,10000,    0,  0, 8, 0, {0, 0}, {0 ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, SUPPLY_UNIT_TYPE},
{"Mutalisk",				 40,10000,10000,  2, 0, 0, {0, 0}, {SPIRE ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, COMBAT_UNIT_TYPE},
{"Guardien",				 40, 5000,10000,  2, 0, 0, {0, 0}, {GREATER_SPIRE ,0, 0}, {MUTALISK, 0, 0}, 0, IS_LOST, 0, 100, COMBAT_UNIT_TYPE}, 
{"Queen",					 50,10000,10000,  2, 0, 0, {0, 0}, {QUEENS_NEST ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100, SUPPORT_UNIT_TYPE},
{"Scourge",					 30, 2500, 7500,  1, 0, 0, {0, 0}, {SPIRE ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, SCOURGE, 100, COMBAT_UNIT_TYPE},
{"Devourer",				 40,10000, 5000,  2, 0, 0, {0, 0}, {GREATER_SPIRE ,0, 0}, {MUTALISK, 0, 0}, 0, IS_LOST, 0, 100, COMBAT_UNIT_TYPE}, 
{"Lair",					100,15000,10000,  0, 1, 0, {0, 0}, {SPAWNING_POOL ,0, 0}, {HATCHERY, 0, 0}, 0, IS_LOST, 0, 0, ADD_ON_UNIT_TYPE},
{"Hive",					120,20000,15000,  0, 1, 0, {0, 0}, {QUEENS_NEST ,0, 0}, {LAIR, 0, 0}, 0, IS_LOST, 0, 0, ADD_ON_UNIT_TYPE},
{"Nydus Canal",				 40,15000,    0,  0, 0, 0, {0, 0}, {HIVE ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, SUPPORT_UNIT_TYPE},
{"Hydralisk den",			 40,10000, 5000,  0, 0, 0, {0, 0}, {SPAWNING_POOL ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Defiler mound",			 60,10000,10000,  0, 0, 0, {0, 0}, {HIVE ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Hatchery",				120,30000,    0,  0, 1, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, LARVA, 0, PRODUCE_UNIT_TYPE}, //create 1 Larva when done :-) TODO: add another "create"field for "CREEP"
{"Greater Spire",			120,10000,15000,  0, 0, 0, {0, 0}, {HIVE ,0, 0}, {SPIRE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Queen's Nest",			 60,15000,10000,  0, 0, 0, {0, 0}, {LAIR ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Evolution Chamber",		 40, 7500,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Ultralisk Cavern",		 80,15000,20000,  0, 0, 0, {0, 0}, {HIVE ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Spire",					120,20000,15000,  0, 0, 0, {0, 0}, {LAIR ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Spawning Pool",			 80,20000,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, RESEARCH_FACILITY_UNIT_TYPE},
{"Creep Colony",			 20, 7500,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0, SUPPORT_UNIT_TYPE},
{"Spore Colony",			 20, 5000,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {CREEP_COLONY, 0, 0}, 0, IS_LOST, 0, 0, ADD_ON_UNIT_TYPE},
{"Sunken Colony",			 20, 5000,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {CREEP_COLONY, 0, 0}, 0, IS_LOST, 0, 0, ADD_ON_UNIT_TYPE},
{"Ventral Sacs",			160,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {LAIR, HIVE, 0}, R_VENTRAL_SACKS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE}, 
{"Antennae",				133,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {LAIR, HIVE, 0}, R_ANTENNAE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Pneumatized Carapace",	133,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {LAIR, HIVE, 0}, R_PNEUMATIZED_CARAPACE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Metabolic Boost",			100,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {SPAWNING_POOL, 0, 0}, R_METABOLIC_BOOST, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Adrenal Glands",			100,20000,20000,  0, 0, 0, {0, 0}, {HIVE ,0, 0}, {SPAWNING_POOL, 0, 0}, R_ADRENAL_GLANDS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Muscular Augments",		100,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {HYDRALISK_DEN, 0, 0}, R_MUSCULAR_AUGMENTS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Grooved Spines",			100,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {HYDRALISK_DEN, 0, 0}, R_GROOVED_SPINES, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Gamete Meiosis",			166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {QUEENS_NEST, 0, 0}, R_GAMETE_MEIOSIS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Metasynaptic Node",		166,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {DEFILER_MOUND, 0, 0}, R_METASYNAPTIC_NODE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Chitinous Plating",		133,15000,15000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ULTRALISK_CAVERN, 0, 0}, R_CHITINOUS_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Anabolic Synthesis",		133,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {ULTRALISK_CAVERN, 0, 0}, R_ANABOLIC_SYNTHESIS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Burrowing",				 80,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {HATCHERY, LAIR, HIVE}, R_BURROWING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Spawn Broodling",			 80,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {QUEENS_NEST, 0, 0}, R_SPAWN_BROODLING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Plague",					100,20000,20000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {DEFILER_MOUND, 0, 0}, R_PLAGUE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Consume",					100,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {DEFILER_MOUND, 0, 0}, R_CONSUME, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Ensnare",					 80,10000,10000,  0, 0, 0, {0, 0}, {0 ,0, 0}, {QUEENS_NEST, 0, 0}, R_ENSNARE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE},
{"Lurker Aspect",			120,20000,20000,  0, 0, 0, {0, 0}, {LAIR ,0, 0}, {HYDRALISK_DEN, 0, 0}, R_LURKER_ASPECT, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0, RESEARCH_UNIT_TYPE}, 
{"Carapace",				266,15000,15000,  0, 0, 7500, {LAIR, HIVE}, {0 ,0, 0}, {EVOLUTION_CHAMBER, 0, 0}, R_CARAPACE, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Flyer Carapace",			266,15000,15000,  0, 0, 7500, {LAIR, HIVE}, {0 ,0, 0}, {SPIRE, GREATER_SPIRE, 0}, R_FLYER_CARAPACE, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Melee Attacks",			266,10000,10000,  0, 0, 5000, {LAIR, HIVE}, {0 ,0, 0}, {EVOLUTION_CHAMBER, 0, 0}, R_MELEE_ATTACKS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Missile Attacks",			266,10000,10000,  0, 0, 5000, {LAIR, HIVE}, {0 ,0, 0}, {EVOLUTION_CHAMBER, 0, 0}, R_MISSILE_ATTACKS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"Flyer Attacks",			266,10000,10000,  0, 0, 7500, {LAIR, HIVE}, {0 ,0, 0}, {SPIRE, GREATER_SPIRE, 0}, R_FLYER_ATTACKS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0, RESEARCH_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"Extractor",				 40, 5000,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {/*VESPENE_GEYSIR*/DRONE, 0, 0}, 0/*DRONE*/, IS_LOST, 0, 0, SUPPLY_UNIT_TYPE}, //~~ //TODO HAE?
{"Gas Drone",				  3,    0,    0,  1, 0, 0, {0, 0}, {EXTRACTOR ,0, 0}, {/*EXTRACTOR*/DRONE, 0, 0}, /*DRONE*/0, /*NEEDED_ONCE_*/IS_LOST, 0, 0, GAS_MINER_UNIT_TYPE}, //~~~~

{"Window move+++",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Window move+",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Window move-",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Window move prev",		  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move from here",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
{"Move to here",			  3,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, SPECIAL_UNIT_TYPE},
	
{"Vespene Geysir",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"Minerals",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"Break up Building",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE},
{"R_Ventral Sacs",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Antennae",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Pneumati. Carapace",	  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Metabolic Boost",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Adrenal Glands",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Muscular Augments",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Grooved Spines",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Gamete Meiosis",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Metasynaptic Node",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Chitinous Plating",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Anabolic Synthesis",	  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Burrowing",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Spawn Broodling",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Plague",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Consume",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Ensnare",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Lurker Aspect",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Carapace",				  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Flyer Carapace",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Melee Attacks",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Missile Attacks",		  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"R_Flyer Attacks",			  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"NULL",					  0,    0,    0,  0, 0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, NO_FACILITY_BEHAVIOUR_DEFINED, 0, 0, REMAINING_UNIT_TYPE},
{"INTRON",					  0,    0 ,   0,  0, 0, 0, {0, 0}, {0, 0, 0}, {0, 0, 0}, 0, NO_FACILITY, 0, 0, REMAINING_UNIT_TYPE}
}
};

// TODO: automatic building units/buildings for spells and larvaes

#endif // _CORE_MAIN_HPP




