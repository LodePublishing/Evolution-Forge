//TODO: Problem: auf goals muessen sowohl ueber jeweiligen Ort (also gGoal[MAX_LOCATIONS][MAX_GOALS]), als auch einzelne Goals, wenn z.B. nach 5 und 10 Minuten 3 und 5 Marines an einem Ort sein sollen...

#ifndef __MAIN_H
#define __MAIN_H

#ifdef BUILD_DLL
#define EXPORT __declspec(dllexport)
#elif IMPORT_DLL
#define EXPORT __declspec(dllimport)
#else
#define EXPORT
#endif


#define MAX_RACE 2
#define MIN_RACE 0

#define MAX_LOCATIONS 10
#define MIN_LOCATIONS 0 //this does not mean that maps with 0 locations can exist....

#define MAX_PLAYER 3
#define MIN_PLAYER 1

#define MAX_GOAL_ENTRIES 25
#define MIN_GOAL_ENTRIES 0

#define MAX_MINS 5000000
#define MAX_GAS 5000000

#define MAX_MAPS 25
#define MIN_MAPS 0

#define MAX_PROGRAMS 128 //must be multiplier of (16*player)
#define LARVA_MAX 200
#define MAX_BUILDINGS 12 // How many buildings can you built simultaneously?
#define UNIT_TYPE_COUNT 101
#define MAX_GOALS 100 // count of possible different goals
#define MAX_SUPPLY 200

#define MAX_GENERATIONS 10000
#define MIN_GENERATIONS 100

#define MAX_BREED_FACTOR 40
#define MIN_BREED_FACTOR 0

#define MAX_MODE 2
#define MIN_MODE 0

#define MAX_CROSSOVER 40
#define MIN_CROSSOVER 0

#define MAX_TIME 3600
#define MIN_TIME 300

#define MAX_TIMEOUT 266
#define MIN_TIMEOUT 40

#define MAX_LENGTH 96
#define MIN_LENGTH 32

#define MAX_RUNS 10
#define MIN_RUNS 1

#define MAX_PREPROCESS_BUILDORDER 1
#define MIN_PREPROCESS_BUILDORDER 0

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

#define ERROR_MESSAGES 8

#define OK 0
#define ENOUGH_MINERALS 1
#define ENOUGH_GAS 2
#define SUPPLY_SATISFIED 3
#define PREREQUISITE_FAILED 4
#define FACILITY_FAILED 5
#define TIMEOUT 6
#define UNKNOWN 7

#define NAME_LENGTH 21

#define TERRA 0
#define PROTOSS 1
#define ZERG 2

#define RACES 3

#define SCV 				1
#define MARINE 				2
#define GHOST 				3
#define VULTURE 			4
#define GOLIATH 			5
#define SIEGE_TANK 			6
#define FIREBAT 			7
#define MEDIC  				8
#define WRAITH 				9
#define SCIENCE_VESSEL 			10
#define DROPSHIP 			11
#define BATTLE_CRUISER 			12
#define VALKYRIE 			13
#define NUCLEAR_WARHEAD 		14
#define COMMAND_CENTER 			15
#define SUPPLY_DEPOT 			16
#define BARRACKS 			17
#define ACADEMY 			18
#define FACTORY 			19
#define STARPORT 			20
#define SCIENCE_FACILITY 		21
#define ENGINEERING_BAY 		22
#define ARMORY 				23
#define MISSILE_TURRET 			24
#define BUNKER 				25
#define COMSAT_STATION 			26
#define NUCLEAR_SILO 			27
#define CONTROL_TOWER 			28
#define COVERT_OPS 			29
#define PHYSICS_LAB 			30
#define MACHINE_SHOP 			31
#define COMMAND_CENTER_CS 		32
#define COMMAND_CENTER_NS 		33
#define STARPORT_CT 			34
#define SCIENCE_FACILITY_CO 		35
#define SCIENCE_FACILITY_PL 		36 
#define FACTORY_MS 			37
#define STIM_PACKS 			38
#define LOCKDOWN 			39
#define EMP_SHOCKWAVE 			40
#define SPIDER_MINES 			41
#define TANK_SIEGE_MODE 		42
#define IRRADIATE 			43
#define YAMATO_GUN 			44
#define CLOAKING_FIELD 			45
#define PERSONNEL_CLOAKING 		46
#define RESTORATION 			47
#define OPTICAL_FLARE 			48
#define U238_SHELLS 			49
#define ION_THRUSTERS 			50
#define TITAN_REACTOR 			51
#define OCULAR_IMPLANTS 		52
#define MOEBIUS_REACTOR 		53
#define APOLLO_REACTOR 			54
#define COLOSSUS_REACTOR 		55
#define CADUCEUS_REACTOR 		56
#define CHARON_BOOSTER 			57
#define INFANTRY_ARMOR 			58
#define INFANTRY_WEAPONS 		59
#define VEHICLE_PLATING 		60
#define VEHICLE_WEAPONS 		61
#define SHIP_PLATING 			62
#define SHIP_WEAPONS 			63
#define REFINERY 			64 // <- must be set constant to 64
#define GAS_SCV				65

#define MOVE_ONE_3_FORWARD		66
#define MOVE_ONE_1_FORWARD		67
#define MOVE_ONE_1_BACKWARD		68

#define ROTATE		69 //~~
#define FOLLOW		70 //~~
#define RESET		71 //~~

#define VESPENE_GEYSIR 			72 
#define MINERALS			73
#define R_STIM_PACKS 			74
#define R_LOCKDOWN 			75
#define R_EMP_SHOCKWAVE 		76
#define R_SPIDER_MINES 			77
#define R_TANK_SIEGE_MODE 		78
#define R_IRRADIATE 			79 
#define R_YAMATO_GUN 			80
#define R_CLOAKING_FIELD 		81
#define R_PERSONNEL_CLOAKING 		82
#define R_RESTORATION 			83
#define R_OPTICAL_FLARE 		84
#define R_U238_SHELLS 			85
#define R_ION_THRUSTERS 		86
#define R_TITAN_REACTOR 		87
#define R_OCULAR_IMPLANTS 		88
#define R_MOEBIUS_REACTOR 		89
#define R_APOLLO_REACTOR 		90
#define R_COLOSSUS_REACTOR 		91
#define R_CADUCEUS_REACTOR 		92
#define R_CHARON_BOOSTER 		93
#define R_INFANTRY_ARMOR 		94
#define R_INFANTRY_WEAPONS 		95
#define R_VEHICLE_PLATING 		96
#define R_VEHICLE_WEAPONS 		97
#define R_SHIP_PLATING 			98 
#define R_SHIP_WEAPONS			99 

#define PROBE 				1
#define DARK_TEMPLAR 			2
#define DARK_ARCHON 			3
#define ZEALOT 				4
#define DRAGOON 			5
#define HIGH_TEMPLAR 			6
#define ARCHON 				7
#define REAVER 				8
#define HALF_SCARAB 			9
#define FULL_SCARAB 			10 
#define CORSAIR 			11
#define SHUTTLE 			12
#define SCOUT 				13
#define ARBITER 			14
#define CARRIER 			15
#define HALF_INTERCEPTOR 		16
#define FULL_INTERCEPTOR 		17
#define OBSERVER 			18
#define NEXUS 				19
#define ROBOTICS_FACILITY 		20
#define PYLON 				21
#define OBSERVATORY 			22
#define GATEWAY 			23
#define PHOTON_CANNON 			24
#define CYBERNETICS_CORE 		25
#define CITADEL_OF_ADUN 		26
#define TEMPLAR_ARCHIVES 		27
#define FORGE 				28
#define STARGATE 			29
#define FLEET_BEACON 			30
#define ARBITER_TRIBUNAL 		31
#define ROBOTICS_SUPPORT_BAY 		32
#define SHIELD_BATTERY 			33
#define PSIONIC_STORM 			34
#define HALLUCINATION 			35
#define RECALL 				36
#define STASIS_FIELD 			37
#define DISRUPTION_WEB 			38
#define MIND_CONTROL 			39
#define MAELSTROM 			40
#define SINGULARITY_CHARGE 		41
#define LEG_ENHANCEMENTS 		42
#define SCARAB_DAMAGE 			43
#define REAVER_CAPACITY 		44
#define GRAVITIC_DRIVE 			45
#define SENSOR_ARRAY 			46
#define GRAVITIC_BOOSTERS 		47
#define KHAYDARIN_AMULET 		48
#define APIAL_SENSORS 			49
#define GRAVITIC_THRUSTERS 		50
#define CARRIER_CAPACITY 		51
#define KHAYDARIN_CORE 			52
#define ARGUS_JEWEL 			53
#define ARGUS_TALISMAN 			54
#define ARMOR 				55
#define PLATING 			56
#define GROUND_WEAPONS 			57
#define AIR_WEAPONS 			58
#define PLASMA_SHIELDS 			59
//#define NULL				60
//#define NULL				61
//#define NULL				62
//#define NULL				63
#define ASSIMILATOR 			64  // <- must be set constant to 64
#define GAS_PROBE			65

//#define MOVE_ONE_3_FORWARD              66
//#define MOVE_ONE_1_FORWARD              67
//#define MOVE_ONE_1_BACKWARD             68
//#define ROTATE          69 //~~
//#define FOLLOW          70 //~~
//#define RESET           71 //~~

//#define VESPENE_GEYSIR 		72  <- already defined in terra unit list
//#define MINERALS			73
#define R_PSIONIC_STORM                 74
#define R_HALLUCINATION                 75
#define R_RECALL                        76
#define R_STASIS_FIELD                  77
#define R_DISRUPTION_WEB                78
#define R_MIND_CONTROL                  79
#define R_MAELSTROM                     80
#define R_SINGULARITY_CHARGE            81
#define R_LEG_ENHANCEMENTS              82
#define R_SCARAB_DAMAGE                 83
#define R_REAVER_CAPACITY               84
#define R_GRAVITIC_DRIVE                85
#define R_SENSOR_ARRAY                  86
#define R_GRAVITIC_BOOSTERS             87
#define R_KHAYDARIN_AMULET              88
#define R_APIAL_SENSORS                 89
#define R_GRAVITIC_THRUSTERS            90
#define R_CARRIER_CAPACITY              91
#define R_KHAYDARIN_CORE                92
#define R_ARGUS_JEWEL                   93
#define R_ARGUS_TALISMAN                94
#define R_ARMOR                         95
#define R_PLATING                       96
#define R_GROUND_WEAPONS                97
#define R_AIR_WEAPONS                   98
#define R_PLASMA_SHIELDS                99



#define DRONE 				1
#define LARVA                           2
#define ZERGLING 			3
#define HYDRALISK			4
#define ULTRALISK 			5
#define DEFILER 			5
#define LURKER 				7
#define OVERLORD 			8
#define MUTALISK 			9
#define GUARDIEN 			10
#define QUEEN 				11
#define SCOURGE 			12
#define DEVOURER 			13
#define HATCHERY 			14
#define LAIR 				15
#define HIVE 				16
#define NYDUS_CANAL 			17
#define HYDRALISK_DEN 			18
#define DEFILER_MOUND 			19
#define GREATER_SPIRE 			20
#define QUEENS_NEST 			21
#define EVOLUTION_CHAMBER 		22
#define ULTRALISK_CAVERN 		23
#define SPIRE 				24
#define SPAWNING_POOL 			25
#define CREEP_COLONY 			26
#define SPORE_COLONY 			27
#define SUNKEN_COLONY 			28
#define VENTRAL_SACKS 			29
#define ANTENNAE 			30
#define PNEUMATIZED_CARAPACE 		31
#define METABOLIC_BOOST 		32
#define ADRENAL_GLANDS 			33 
#define MUSCULAR_AUGMENTS 		34
#define GROOVED_SPINES 			35
#define GAMETE_MEIOSIS 			36
#define METASYNAPTIC_NODE 		37
#define CHITINOUS_PLATING 		38
#define ANABOLIC_SYNTHESIS 		39
#define BURROWING 			40
#define SPAWN_BROODLING 		41
#define PLAGUE 				42
#define CONSUME 			43
#define ENSNARE 			44
#define LURKER_ASPECT 			45
#define CARAPACE 			46
#define FLYER_CARAPACE 			47
#define MELEE_ATTACKS 			48
#define MISSILE_ATTACKS 		49
#define FLYER_ATTACKS 			50
//#define NULL                          51
//#define NULL                          52
//#define NULL                          53
//#define NULL                          54
//#define NULL                          55
//#define NULL                          56
//#define NULL                          57
//#define NULL                          58
//#define NULL                          59
//#define NULL                          60
//#define NULL                          61
//#define NULL                          62
//#define NULL				63
#define EXTRACTOR 			64 // <- must be set constant to 64
#define GAS_DRONE			65

//#define MOVE_ONE_3_FORWARD              66
//#define MOVE_ONE_1_FORWARD              67
//#define MOVE_ONE_1_BACKWARD             68
                                                                                                                                                            
//#define ROTATE          69 //~~
//#define FOLLOW          70 //~~
//#define RESET           71 //~~

//#define VESPENE_GEYSIR 		72 <- already defined in terra unit list
//#define MINERALS			73 <- already defined in terra unit list
#define BREAK_UP_BUILDING		74
#define R_VENTRAL_SACKS                 75
#define R_ANTENNAE                      76
#define R_PNEUMATIZED_CARAPACE          77
#define R_METABOLIC_BOOST               78
#define R_ADRENAL_GLANDS                79
#define R_MUSCULAR_AUGMENTS             80
#define R_GROOVED_SPINES                81
#define R_GAMETE_MEIOSIS                82
#define R_METASYNAPTIC_NODE             83
#define R_CHITINOUS_PLATING             84
#define R_ANABOLIC_SYNTHESIS            85
#define R_BURROWING                     86
#define R_SPAWN_BROODLING               87
#define R_PLAGUE                        88
#define R_CONSUME                       89
#define R_ENSNARE                       90
#define R_LURKER_ASPECT                 91
#define R_CARAPACE                      92
#define R_FLYER_CARAPACE                93
#define R_MELEE_ATTACKS                 94
#define R_MISSILE_ATTACKS               95
#define R_FLYER_ATTACKS                 96
//#define NULL                          97
//#define NULL                          98
//#define NULL                          99
#define INTRON				100

// these are the possible values for facility_type
// they describe what happens to the units while constructing / when the building is complete
#define IS_LOST 0		// 0: facilities are lost (templars/archons, drones/buildings, command center/comsat, ...), 
#define NEEDED_ONCE 1   // 1: facility is needed at this place, but only once (probes!)
#define NEEDED_UNTIL_COMPLETE 2 // 2: facility is needed until the item is complete (Scvs, buildings in general)
#define NEEDED_ONCE_IS_LOST 3 // Special, probes in facility0!
#define NEEDED_UNTIL_COMPLETE_IS_LOST 4 // Special, scvs in facility0!
#define NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE 5 //availible returns to 1 but one item is lost... kinda hack for upgrades :)
#define NEEDED_ALWAYS 6 //nuke silos/nukes, reaver/scarabs, carrier/interceptors, ...


struct UNIT_STATISTICS
{
	char name[NAME_LENGTH];
	int BT;
	int mins,gas;
	int supply;
	int upgrade_cost;	 // upgrade costs in minerals and gas (50, 75, ...)
	int upgrade[2];		 // what building is needed to go upgrade level 2 and level 3 (templar archives, science facility, fleet beacon, lair, hive)
	int prerequisite[3]; // these prerequisites buildings need to be anywhere on the map, so that this unit can be build
	int facility[3];	 // where _can_ the unit be produced? primariliy for zerg and terra
			             // for upgrades: fac[2] and fac[3] are places to hold prequerisities for additional upgrading beyond level 1~~
	int facility2;		 // additional facilities, primarily for drones or morphing templars
	int facility_type;	 // see above
						 
	int create;			 // Additional building created when this item is completed (only for add-ons)
	int speed;			 // speed of units, not yet implemented
};

const UNIT_STATISTICS stats[RACES][UNIT_TYPE_COUNT]=
{
{
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 100},
{"SCV", 20, 5000,    0,  1, 0, {0, 0}, {0 ,0, 0}, {COMMAND_CENTER, COMMAND_CENTER_CS, COMMAND_CENTER_NS}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Marine", 24, 5000,    0,  1, 0, {0, 0}, {0 ,0, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Ghost", 50, 2500, 7500,  1, 0, {0, 0}, {ACADEMY ,COVERT_OPS, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Vulture", 30, 7500,    0,  2, 0, {0, 0}, {FACTORY ,0, 0}, {FACTORY, FACTORY_MS, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Goliath", 40,10000, 5000,  2, 0, {0, 0}, {ARMORY, 0, 0}, {FACTORY, FACTORY_MS, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Siege Tank", 50,15000,10000,  2, 0, {0, 0}, {0 ,0, 0}, {FACTORY_MS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Firebat", 24, 5000, 2500,  1, 0, {0, 0}, {ACADEMY ,0, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Medic", 30, 5000, 2500,  1, 0, {0, 0}, {ACADEMY ,0, 0}, {BARRACKS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Wraith", 60,15000,10000,  2, 0, {0, 0}, {0 ,0, 0}, {STARPORT, STARPORT_CT, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Science Vessel", 80,10000,22500,  2, 0, {0, 0}, {SCIENCE_FACILITY, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Dropship", 50,10000,10000,  2, 0, {0, 0}, {0, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Battle Cruiser",133,40000,30000,  6, 0, {0, 0}, {PHYSICS_LAB, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Valkyrie", 50,25000,12500,  3, 0, {0, 0}, {ARMORY, 0, 0}, {STARPORT_CT, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"Nuclear Warhead",100,20000,20000,  8, 0, {0, 0}, {COMMAND_CENTER_NS, 0, 0}, {NUCLEAR_SILO, 0, 0}, 0, NEEDED_ALWAYS, 0, 0},
{"Command Center",120,40000,    0,-10, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10},
{"Supply Depot", 40,10000,    0, -8, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0},
{"Barracks", 80,15000,    0,  0, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10},
{"Academy", 80,15000,    0,  0, 0, {0, 0}, {BARRACKS ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0},
{"Factory", 80,20000,10000,  0, 0, {0, 0}, {BARRACKS ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10},
{"Starport", 70,15000,10000,  0, 0, {0, 0}, {FACTORY ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10},
{"Science Facility", 60,10000,15000,  0, 0, {0, 0}, {STARPORT ,0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10},
{"Engineering Bay", 60,12500,    0,  0, 0, {0, 0}, {0, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 10},
{"Armory", 80,10000, 5000,  0, 0, {0, 0}, {FACTORY, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0},
{"Missile Turret", 30, 7500,    0,  0, 0, {0, 0}, {ENGINEERING_BAY, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0},
{"Bunker", 30,10000,    0,  0, 0, {0, 0}, {BARRACKS, 0, 0}, {SCV, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 0},
{"Comsat Station",  0,    0,    0,  0, 0, {0, 0}, {COMSAT_STATION, COMMAND_CENTER_CS, 0}, {0, 0 ,0}, 0, 0, 0, 0}, 
{"Nuclear Silo",  0,    0,    0,  0, 0, {0, 0}, {NUCLEAR_SILO, COMMAND_CENTER_NS, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"Control Tower",  0,    0,    0,  0, 0, {0, 0}, {CONTROL_TOWER, STARPORT_CT, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"Covert Ops",  0,    0,    0,  0, 0, {0, 0}, {COVERT_OPS, SCIENCE_FACILITY_CO, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"Physics Lab",  0,    0,    0,  0, 0, {0, 0}, {PHYSICS_LAB, SCIENCE_FACILITY_PL, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"Machine Shop",  0,    0,    0,  0, 0, {0, 0}, {MACHINE_SHOP, FACTORY_MS, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"Command Center[CS]", 40, 5000, 5000,-10, 0, {0, 0}, {ACADEMY, 0, 0}, {COMMAND_CENTER, 0, 0}, 0, IS_LOST, COMSAT_STATION, 0}, //TODO: Beim Abheben zerstoeren...
{"Command Center[NS]", 40,10000,10000,-10, 0, {0, 0}, {COVERT_OPS, 0, 0}, {COMMAND_CENTER, 0, 0}, 0, IS_LOST, NUCLEAR_SILO, 0},
{"Starport[CT]", 80, 5000, 5000,  0, 0, {0, 0}, {0 ,0, 0}, {STARPORT, 0, 0}, 0, IS_LOST, CONTROL_TOWER, 0},
{"Science Facility[CO]", 40, 5000, 5000,  0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, 0, 0}, 0, IS_LOST, COVERT_OPS, 0},
{"Science Facility[PL]", 40, 5000, 5000,  0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, 0, 0}, 0, IS_LOST, PHYSICS_LAB, 0},
{"Factory[MS]", 40, 5000, 5000,  0, 0, {0, 0}, {0 ,0, 0}, {FACTORY, 0, 0}, 0, IS_LOST, MACHINE_SHOP, 0},
{"Stim Packs", 80,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_STIM_PACKS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Lockdown",100,20000,20000,  0, 0, {0, 0}, {SCIENCE_FACILITY_CO, 0, 0}, {COVERT_OPS, 0, 0}, R_LOCKDOWN, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"EMP Shockwave",120,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, SCIENCE_FACILITY_CO, SCIENCE_FACILITY_PL}, R_EMP_SHOCKWAVE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Spider Mines", 80,10000,10000,  0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_SPIDER_MINES, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Siege Mode", 80,15000,15000,  0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_TANK_SIEGE_MODE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Irradiate", 80,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, SCIENCE_FACILITY_CO, SCIENCE_FACILITY_PL}, R_IRRADIATE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Yamato Gun",120,10000,10000,  0, 0, {0, 0}, {SCIENCE_FACILITY_PL ,0, 0}, {PHYSICS_LAB, 0, 0}, R_YAMATO_GUN, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Cloaking Field",100,15000,15000,  0, 0, {0, 0}, {STARPORT_CT ,0, 0}, {CONTROL_TOWER, 0, 0}, R_CLOAKING_FIELD, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Personnel Cloaking", 80,10000,10000,  0, 0, {0, 0}, {SCIENCE_FACILITY_CO ,0, 0}, {COVERT_OPS, 0, 0}, R_PERSONNEL_CLOAKING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Restoration", 80,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_RESTORATION, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Optical Flare",120,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_OPTICAL_FLARE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"U-238 Shells",100,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_U238_SHELLS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Ion Thrusters",100,10000,10000,  0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_ION_THRUSTERS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Titan Reactor",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {SCIENCE_FACILITY, SCIENCE_FACILITY_CO, SCIENCE_FACILITY_PL}, R_TITAN_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Ocular Implants",166,10000,10000,  0, 0, {0, 0}, {SCIENCE_FACILITY_CO ,0, 0}, {COVERT_OPS, 0, 0}, R_OCULAR_IMPLANTS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Moebius Reactor",166,20000,20000,  0, 0, {0, 0}, {SCIENCE_FACILITY_CO ,0, 0}, {COVERT_OPS, 0, 0}, R_MOEBIUS_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Apollo Reactor",166,20000,20000,  0, 0, {0, 0}, {STARPORT_CT ,0, 0}, {CONTROL_TOWER, 0, 0}, R_APOLLO_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Colossus Reactor",166,15000,15000,  0, 0, {0, 0}, {SCIENCE_FACILITY_PL ,0, 0}, {PHYSICS_LAB, 0, 0}, R_COLOSSUS_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Caduceus Reactor",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {ACADEMY, 0, 0}, R_CADUCEUS_REACTOR, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Charon Booster",133,10000,10000,  0, 0, {0, 0}, {FACTORY_MS ,0, 0}, {MACHINE_SHOP, 0, 0}, R_CHARON_BOOSTER, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Infantry Armor",266,10000,10000,  0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ENGINEERING_BAY, 0, 0}, R_INFANTRY_ARMOR, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"Infantry Weapons",266,10000,10000,  0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ENGINEERING_BAY, 0, 0}, R_INFANTRY_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"Vehicle Plating",266,10000,10000,  0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_VEHICLE_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"Vehicle Weapons",266,10000,10000,  0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_VEHICLE_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"Ship Plating",266,15000,15000,  0, 7500, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_SHIP_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"Ship Weapons",266,10000,10000,  0, 5000, {SCIENCE_FACILITY, SCIENCE_FACILITY}, {0 ,0, 0}, {ARMORY, 0, 0}, R_SHIP_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"Refinery", 40,10000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {SCV, 0, 0}, 0/*VESPENE_GEYSIR*/, NEEDED_UNTIL_COMPLETE/*_IS_LOST*/, 0, 0}, //~~~ueberlegen...
{"Gas SCV",  3,    0,    0,  1, 0, {0, 0}, {0 ,0, 0}, {REFINERY, 0, 0}, SCV, NEEDED_ONCE_IS_LOST, 0, 0},	//~~~~
{"      Window move+++",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Window move+",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Window move-",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    Window move prev",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      Move from here",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Move to here",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},

{"      Vespene Geysir",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"            Minerals",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        R_Stim Packs",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"          R_Lockdown",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_EMP Shockwave",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_Spider Mines",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        R_Siege Mode",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"         R_Irradiate",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        R_Yamato Gun",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Cloaking Field",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"R_Personnel Cloaking",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"       R_Restoration",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Optical Flare",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_U-238 Shells",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Ion Thrusters",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Titan Reactor",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Ocular Implants",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Moebius Reactor",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Apollo Reactor",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"  R_Colossus Reactor",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"  R_Caduceus Reactor",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Charon Booster",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Infantry Armor",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"  R_Infantry Weapons",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Vehicle Plating",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Vehicle Weapons",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_Ship Plating",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_Ship Weapons",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"INTRON",  0,   0,   0,  0, 0, {0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0}
},

{
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"               Probe", 20, 5000,    0,  1, 0, {0, 0}, {0 ,0, 0}, {NEXUS, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"        Dark Templar", 50,12500,10000,  2, 0, {0, 0}, {TEMPLAR_ARCHIVES ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"         Dark Archon", 20,    0,    0,  2, 0, {0, 0}, {0 ,0, 0}, {DARK_TEMPLAR, 0, 0}, DARK_TEMPLAR, IS_LOST, 0, 100},
{"              Zealot", 40,10000,    0,  2, 0, {0, 0}, {0 ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"             Dragoon", 50,12500, 5000,  2, 0, {0, 0}, {CYBERNETICS_CORE ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"        High Templar", 50, 5000,15000,  2, 0, {0, 0}, {TEMPLAR_ARCHIVES ,0, 0}, {GATEWAY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"              Archon", 20,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {HIGH_TEMPLAR, 0, 0}, HIGH_TEMPLAR, IS_LOST, 0, 100},
{"              Reaver", 70,20000,10000,  4, 0, {0, 0}, {ROBOTICS_SUPPORT_BAY ,0, 0}, {ROBOTICS_FACILITY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100}, 
{"            5xScarab", 35, 7500,    0,  0, 0, {0, 0}, {0, 0, 0}, {REAVER, 0, 0}, 0, NEEDED_ALWAYS, 0, 0},
{"           10xScarab", 35, 7500,    0,  0, 0, {0, 0}, {REAVER_CAPACITY, 0, 0}, {HALF_SCARAB, 0, 0}, 0, IS_LOST, 0, 0}, //~~ special rule... reaver is no longer availible :/
{"             Corsair", 40,15000,10000,  2, 0, {0, 0}, {0 ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"             Shuttle", 60,20000,    0,  2, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_FACILITY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"               Scout", 80,27500,12500,  3, 0, {0, 0}, {0 ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"             Arbiter",160,10000,35000,  4, 0, {0, 0}, {ARBITER_TRIBUNAL ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"             Carrier",140,35000,25000,  6, 0, {0, 0}, {FLEET_BEACON ,0, 0}, {STARGATE, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"       5xInterceptor",100,25000,    0,  0, 0, {0, 0}, {0,0,0}, {CARRIER, 0, 0}, 0, NEEDED_ALWAYS, 0, 0},
{"       8xInterceptor", 60,15000,    0,  0, 0, {0, 0}, {CARRIER_CAPACITY,0,0}, {HALF_INTERCEPTOR, 0, 0}, 0, IS_LOST, 0, 0}, //~~~ Special rule... carrier is no longer availible :/
{"            Observer", 40, 2500, 7500,  1, 0, {0, 0}, {OBSERVATORY ,0, 0}, {ROBOTICS_FACILITY, 0, 0}, 0, NEEDED_UNTIL_COMPLETE, 0, 100},
{"               Nexus",120,40000,    0, -9, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"   Robotics Facility", 80,20000,20000,  0, 0, {0, 0}, {CYBERNETICS_CORE ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"               Pylon", 30,10000,    0, -8, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"         Observatory", 30, 5000,10000,  0, 0, {0, 0}, {ROBOTICS_FACILITY ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"             Gateway", 60,15000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"       Photon Cannon", 50,15000,    0,  0, 0, {0, 0}, {FORGE ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"    Cybernetics Core", 60,20000,    0,  0, 0, {0, 0}, {GATEWAY ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"     Citadel Of Adun", 60,15000,10000,  0, 0, {0, 0}, {CYBERNETICS_CORE ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"    Templar Archives", 60,15000,20000,  0, 0, {0, 0}, {CITADEL_OF_ADUN ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"               Forge", 40,15000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"            Stargate", 70,15000,15000,  0, 0, {0, 0}, {CYBERNETICS_CORE ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"        Fleet Beacon", 60,30000,20000,  0, 0, {0, 0}, {STARGATE ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"    Arbiter Tribunal", 60,20000,15000,  0, 0, {0, 0}, {TEMPLAR_ARCHIVES ,STARGATE, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"Robotics Support Bay", 30,15000,10000,  0, 0, {0, 0}, {ROBOTICS_FACILITY ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"      Shield Battery", 30,10000,    0,  0, 0, {0, 0}, {GATEWAY ,0, 0}, {PROBE, 0, 0}, 0, NEEDED_ONCE, 0, 0},
{"       Psionic Storm",120,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_PSIONIC_STORM, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"       Hallucination", 80,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_HALLUCINATION, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"              Recall",120,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {ARBITER_TRIBUNAL, 0, 0}, R_RECALL, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"        Stasis Field",100,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {ARBITER_TRIBUNAL, 0, 0}, R_STASIS_FIELD, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"      Disruption Web", 80,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_DISRUPTION_WEB, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"        Mind Control",120,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_MIND_CONTROL, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"           Maelstrom",100,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_MAELSTROM, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"  Singularity Charge",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {CYBERNETICS_CORE, 0, 0}, R_SINGULARITY_CHARGE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"    Leg Enhancements",133,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {CITADEL_OF_ADUN, 0, 0}, R_LEG_ENHANCEMENTS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"       Scarab Damage",166,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_SUPPORT_BAY, 0, 0}, R_SCARAB_DAMAGE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"     Reaver Capacity",166,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_SUPPORT_BAY, 0, 0}, R_REAVER_CAPACITY, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"      Gravitic Drive",166,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {ROBOTICS_SUPPORT_BAY, 0, 0}, R_GRAVITIC_DRIVE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"        Sensor Array",133,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {OBSERVATORY, 0, 0}, R_SENSOR_ARRAY, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"   Gravitic Boosters",133,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {OBSERVATORY, 0, 0}, R_GRAVITIC_BOOSTERS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"    Khaydarin Amulet",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_KHAYDARIN_AMULET, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"       Apial Sensors",166,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_APIAL_SENSORS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"  Gravitic Thrusters",166,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_GRAVITIC_THRUSTERS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"    Carrier Capacity",100,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_CARRIER_CAPACITY, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"      Khaydarin Core",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {ARBITER_TRIBUNAL, 0, 0}, R_KHAYDARIN_CORE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"         Argus Jewel",166,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {FLEET_BEACON, 0, 0}, R_ARGUS_JEWEL, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"      Argus Talisman",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {TEMPLAR_ARCHIVES, 0, 0}, R_ARGUS_TALISMAN, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"               Armor",266,10000,10000,  0, 7500, {TEMPLAR_ARCHIVES, TEMPLAR_ARCHIVES}, {0 ,0, 0}, {FORGE, 0, 0}, R_ARMOR, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"             Plating",266,15000,15000,  0, 7500, {FLEET_BEACON, FLEET_BEACON}, {0 ,0, 0}, {CYBERNETICS_CORE, 0, 0}, R_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"      Ground Weapons",266,10000,10000,  0, 5000, {TEMPLAR_ARCHIVES, TEMPLAR_ARCHIVES}, {0 ,0, 0}, {FORGE, 0, 0}, R_GROUND_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"         Air Weapons",266,10000,10000,  0, 7500, {FLEET_BEACON, FLEET_BEACON}, {0 ,0, 0}, {CYBERNETICS_CORE, 0, 0}, R_AIR_WEAPONS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"      Plasma Shields",266,20000,20000,  0, 10000, {0, 0}, {0 ,0, 0}, {FORGE, 0, 0}, R_PLASMA_SHIELDS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"         Assimilator", 40,10000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {PROBE, 0, 0}, /*VESPENE_GEYSIR*/0, NEEDED_ONCE/*_IS_LOST*/, 0, 0},
{"           Gas Probe",  3,    0,    0,  1, 0, {0, 0}, {0 ,0, 0}, {ASSIMILATOR, 0, 0}, PROBE, NEEDED_ONCE_IS_LOST, 0, 0},	//Needed once? ~~~
{"      Window move+++",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Window move+",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Window move-",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    Window move prev",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      Move from here",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Move to here",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},

	
{"            Minerals",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      Vespene Geysir",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Psionic Storm",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Hallucination",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"            R_Recall",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_Stasis Field",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Disruption Web",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_Mind Control",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"         R_Maelstrom",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"R_Singularity Charge",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"  R_Leg Enhancements",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Scarab Damage",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Reaver Capacity",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Gravitic Drive",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_Sensor Array",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{" R_Gravitic Boosters",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"  R_Khaydarin Amulet",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Apial Sensors",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"R_Gravitic Thrusters",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"  R_Carrier Capacity",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Khaydarin Core",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"       R_Argus Jewel",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Argus Talisman",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"             R_Armor",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"           R_Plating",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Ground Weapons",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"       R_Air Weapons",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Plasma Shields",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"INTRON",  0,   0,   0,  0, 0, {0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0}
},

{
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0, 0, 0}, {0 ,0, 0}, 0, 0, 0, 0},
{"               Drone", 23, 5000,    0,  1, 0, {0, 0}, {0 ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100},
{"               Larva", 20,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"            Zergling", 28, 5000,    0,  1, 0, {0, 0}, {SPAWNING_POOL ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100},
{"           Hydralisk", 28, 7500, 2500,  1, 0, {0, 0}, {HYDRALISK_DEN ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, ZERGLING, 100},
{"           Ultralisk", 60,20000,20000,  6, 0, {0, 0}, {ULTRALISK_CAVERN ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100},
{"             Defiler", 50, 5000,15000,  2, 0, {0, 0}, {DEFILER_MOUND ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100},
{"              Lurker", 40, 5000,10000,  2, 0, {0, 0}, {HYDRALISK_DEN ,0, 0}, {HYDRALISK, 0, 0}, 0, IS_LOST, 0, 100},
{"            Overlord", 40,10000,    0, -8, 0, {0, 0}, {0 ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100},
{"            Mutalisk", 40,10000,10000,  2, 0, {0, 0}, {SPIRE ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100},
{"            Guardien", 40, 5000,10000,  2, 0, {0, 0}, {GREATER_SPIRE ,0, 0}, {MUTALISK, 0, 0}, 0, IS_LOST, 0, 100}, 
{"               Queen", 50,10000,10000,  2, 0, {0, 0}, {QUEENS_NEST ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, 0, 100},
{"             Scourge", 30, 2500, 7500,  1, 0, {0, 0}, {SPIRE ,0, 0}, {LARVA, 0, 0}, 0, IS_LOST, SCOURGE, 100},
{"            Devourer", 40,10000, 5000,  2, 0, {0, 0}, {GREATER_SPIRE ,0, 0}, {MUTALISK, 0, 0}, 0, IS_LOST, 0, 100}, 
{"            Hatchery",120,30000,    0, -1, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, LARVA, 0}, //create 1 Larva when done :-)
{"                Lair",100,15000,10000, -1, 0, {0, 0}, {SPAWNING_POOL ,0, 0}, {HATCHERY, 0, 0}, 0, IS_LOST, 0, 0},
{"                Hive",120,20000,15000, -1, 0, {0, 0}, {QUEENS_NEST ,0, 0}, {LAIR, 0, 0}, 0, IS_LOST, 0, 0},
{"         Nydus Canal", 40,15000,    0,  0, 0, {0, 0}, {HIVE ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"       Hydralisk den", 40,10000, 5000,  0, 0, {0, 0}, {SPAWNING_POOL ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"       Defiler mound", 60,10000,10000,  0, 0, {0, 0}, {HIVE ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"       Greater Spire",120,10000,15000,  0, 0, {0, 0}, {HIVE ,0, 0}, {SPIRE, 0, 0}, 0, IS_LOST, 0, 0},
{"        Queen's Nest", 60,15000,10000,  0, 0, {0, 0}, {LAIR ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"   Evolution Chamber", 40, 7500,    0,  0, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"    Ultralisk Cavern", 80,15000,20000,  0, 0, {0, 0}, {HIVE ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"               Spire",120,20000,15000,  0, 0, {0, 0}, {LAIR ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"       Spawning Pool", 80,20000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"        Creep Colony", 20, 7500,    0,  0, 0, {0, 0}, {0 ,0, 0}, {DRONE, 0, 0}, 0, IS_LOST, 0, 0},
{"        Spore Colony", 20, 5000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {CREEP_COLONY, 0, 0}, 0, IS_LOST, 0, 0},
{"       Sunken Colony", 20, 5000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {CREEP_COLONY, 0, 0}, 0, IS_LOST, 0, 0},
{"        Ventral Sacs",160,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {LAIR, HIVE, 0}, R_VENTRAL_SACKS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0}, 
{"            Antennae",133,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {LAIR, HIVE, 0}, R_ANTENNAE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"Pneumatized Carapace",133,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {LAIR, HIVE, 0}, R_PNEUMATIZED_CARAPACE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"     Metabolic Boost",100,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {SPAWNING_POOL, 0, 0}, R_METABOLIC_BOOST, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"      Adrenal Glands",100,20000,20000,  0, 0, {0, 0}, {HIVE ,0, 0}, {SPAWNING_POOL, 0, 0}, R_ADRENAL_GLANDS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"   Muscular Augments",100,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {HYDRALISK_DEN, 0, 0}, R_MUSCULAR_AUGMENTS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"      Grooved Spines",100,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {HYDRALISK_DEN, 0, 0}, R_GROOVED_SPINES, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"      Gamete Meiosis",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {QUEENS_NEST, 0, 0}, R_GAMETE_MEIOSIS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"   Metasynaptic Node",166,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {DEFILER_MOUND, 0, 0}, R_METASYNAPTIC_NODE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"   Chitinous Plating",133,15000,15000,  0, 0, {0, 0}, {0 ,0, 0}, {ULTRALISK_CAVERN, 0, 0}, R_CHITINOUS_PLATING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"  Anabolic Synthesis",133,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {ULTRALISK_CAVERN, 0, 0}, R_ANABOLIC_SYNTHESIS, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"           Burrowing", 80,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {HATCHERY, LAIR, HIVE}, R_BURROWING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"     Spawn Broodling", 80,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {QUEENS_NEST, 0, 0}, R_SPAWN_BROODLING, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"              Plague",100,20000,20000,  0, 0, {0, 0}, {0 ,0, 0}, {DEFILER_MOUND, 0, 0}, R_PLAGUE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"             Consume",100,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {DEFILER_MOUND, 0, 0}, R_CONSUME, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"             Ensnare", 80,10000,10000,  0, 0, {0, 0}, {0 ,0, 0}, {QUEENS_NEST, 0, 0}, R_ENSNARE, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0},
{"       Lurker Aspect",120,20000,20000,  0, 0, {0, 0}, {LAIR ,0, 0}, {HYDRALISK_DEN, 0, 0}, R_LURKER_ASPECT, NEEDED_UNTIL_COMPLETE_IS_LOST, 0, 0}, 
{"            Carapace",266,15000,15000,  0, 7500, {LAIR, HIVE}, {0 ,0, 0}, {EVOLUTION_CHAMBER, 0, 0}, R_CARAPACE, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"      Flyer Carapace",266,15000,15000,  0, 7500, {LAIR, HIVE}, {0 ,0, 0}, {SPIRE, GREATER_SPIRE, 0}, R_FLYER_CARAPACE, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"       Melee Attacks",266,10000,10000,  0, 5000, {LAIR, HIVE}, {0 ,0, 0}, {EVOLUTION_CHAMBER, 0, 0}, R_MELEE_ATTACKS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"     Missile Attacks",266,10000,10000,  0, 5000, {LAIR, HIVE}, {0 ,0, 0}, {EVOLUTION_CHAMBER, 0, 0}, R_MISSILE_ATTACKS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"       Flyer Attacks",266,10000,10000,  0, 7500, {LAIR, HIVE}, {0 ,0, 0}, {SPIRE, GREATER_SPIRE, 0}, R_FLYER_ATTACKS, NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"           Extractor", 40, 5000,    0,  0, 0, {0, 0}, {0 ,0, 0}, {/*VESPENE_GEYSIR*/DRONE, 0, 0}, 0/*DRONE*/, IS_LOST, 0, 0}, //~~
{"           Gas Drone",  3,    0,    0,  1, 0, {0, 0}, {0 ,0, 0}, {EXTRACTOR, 0, 0}, DRONE, NEEDED_ONCE_IS_LOST, 0, 0}, //~~~~

{"      Window move+++",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Window move+",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Window move-",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    Window move prev",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      Move from here",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"        Move to here",  3,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
	
{"            Minerals",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      Vespene Geysir",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   Break up Building",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"      R_Ventral Sacs",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"          R_Antennae",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"R_Pneumati. Carapace",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Metabolic Boost",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Adrenal Glands",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{" R_Muscular Augments",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Grooved Spines",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Gamete Meiosis",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{" R_Metasynaptic Node",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{" R_Chitinous Plating",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"R_Anabolic Synthesis",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"         R_Burrowing",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Spawn Broodling",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"            R_Plague",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"           R_Consume",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"           R_Ensnare",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Lurker Aspect",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"          R_Carapace",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"    R_Flyer Carapace",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Melee Attacks",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"   R_Missile Attacks",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"     R_Flyer Attacks",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"                NULL",  0,    0,    0,  0, 0, {0, 0}, {0 ,0, 0}, {0, 0, 0}, 0, 0, 0, 0},
{"INTRON",  0,   0,   0,  0, 0, {0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0}
}
};

// TODO: automatic building units/buildings for spells and larvaes

#endif // __MAIN_H




