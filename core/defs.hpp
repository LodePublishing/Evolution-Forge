//TODO: Problem: auf goals muessen sowohl ueber jeweiligen Ort (also gGoal[MAX_LOCATIONS][MAX_GOALS]), als auch einzelne Goals, wenn z.B. nach 5 und 10 Minuten 3 und 5 Marines an einem Ort sein sollen...

#ifndef _CORE_DEFS_HPP
#define _CORE_DEFS_HPP

#include <string>

#define MAX_RACES 3
#define UNIT_TYPE_COUNT 101
#define MAX_LOCATIONS 9
#define MAX_PLAYER 3

#define GLOBAL 0
extern const float CORE_VERSION;

//extern const unsigned int MAX_LOCATIONS;
extern const unsigned int MIN_LOCATIONS; //this does not mean that maps with 0 locations can exist....

//extern const unsigned int MAX_PLAYER;
extern const unsigned int MIN_PLAYER;

extern const unsigned int MAX_MINERALS;
extern const unsigned int MAX_GAS;

#define MAX_PROGRAMS 128
//extern const unsigned int MAX_PROGRAMS; //must be multiplier of (16*player)
extern const unsigned int LARVA_MAX;
//extern const unsigned int UNIT_TYPE_COUNT;
#define MAX_HARVEST_SPEEDS 100
//extern const unsigned int MAX_HARVEST_SPEEDS;
extern const unsigned int MAX_SUPPLY;

extern const unsigned int MAX_TOTAL_UNITS;

extern const unsigned int MAX_GENERATIONS;
extern const unsigned int MIN_GENERATIONS;

extern const unsigned int MAX_BREED_FACTOR;
extern const unsigned int MIN_BREED_FACTOR;

extern const unsigned int MAX_MODE;
extern const unsigned int MIN_MODE;

extern const unsigned int MAX_CROSSOVER;
extern const unsigned int MIN_CROSSOVER;
#define MAX_TIME 3601
//extern const unsigned int MAX_TIME;
extern const unsigned int MIN_TIME;

extern const unsigned int MAX_TIMEOUT;
extern const unsigned int MIN_TIMEOUT;
#define MAX_LENGTH 96
//extern const unsigned int MAX_LENGTH;
extern const unsigned int MIN_LENGTH;

#define MAX_RUNS 10
//extern const unsigned int MAX_RUNS;
extern const unsigned int MIN_RUNS;

extern const unsigned int MAX_PREPROCESS_BUILDORDER;
extern const unsigned int MIN_PREPROCESS_BUILDORDER;


extern const unsigned int MAX_TFITNESS;
extern const unsigned int MAX_PFITNESS;

//extern const unsigned int MAX_RACES;

extern void toLog(const std::string& msg);

#define NEUTRAL_PLAYER 0

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
	REFINERY, // <- must be set extern constant to 64
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
//} // end eTerranUnits
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
//}
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
// they describe what happens to the units while extern constructing / when the building is complete
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

struct LAST
{
	unsigned int unit;
	unsigned int location;
	unsigned int count;
};

struct GOAL
{
	unsigned int unit;//!
	unsigned int time;
	unsigned int count;
	unsigned int location;

	unsigned int finalTime; // temporary!
};

struct UNIT_STATISTICS
{
	std::string name;
	unsigned int BT;
	unsigned int minerals,gas;
	unsigned int needSupply;
	unsigned int haveSupply;
	unsigned int upgrade_cost;	 // upgrade costs in minerals and gas (50, 75, ...)
	unsigned int upgrade[2];		 // what building is needed to go upgrade level 2 and level 3 (templar archives, science facility, fleet beacon, lair, hive)
	unsigned int prerequisite[3]; // these prerequisites buildings need to be anywhere on the map, so that this unit can be build
	unsigned int facility[3];	 // where _can_ the unit be produced? primariliy for zerg and terra
			             // for upgrades: fac[2] and fac[3] are places to hold prequerisities for additional upgrading beyond level 1~~
	unsigned int facility2;		 // additional facilities, primarily for drones or morphing templars
	eFacilityType facilityType;	 // see above
						 
	unsigned int create;			 // Additional building created when this item is completed (only for add-ons)
	unsigned int speed;			 // speed of units, not yet implemented
	eUnitType unitType;		
};

extern const UNIT_STATISTICS stats[MAX_RACES][UNIT_TYPE_COUNT];

// TODO: automatic building units/buildings for spells and larvaes

#endif // _CORE_DEFS_HPP




