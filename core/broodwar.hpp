//TODO: Problem: auf goals muessen sowohl ueber jeweiligen Ort (also gGoal[MAX_LOCATIONS][MAX_GOALS]), als auch einzelne Goals, wenn z.B. nach 5 und 10 Minuten 3 und 5 Marines an einem Ort sein sollen...?

#ifndef _CORE_BROODWAR_HPP
#define _CORE_BROODWAR_HPP

#include "game.hpp"

class BROODWAR : public GAME
{
	public:
		BROODWAR();
		~BROODWAR();

		void init();
		void initStringIdentifier();

                enum eResource
                {
                        MINERALS,
                        VESPENE_GAS
                };

                enum eAffectsResource
                {
                        AFFECTS_MINERALS = 1,
                        AFFECTS_GAS = 2,
                        AFFECTS_MINERALS_AND_GAS = 3, // 2 + 1
                        AFFECTS_MINERALS_GLOBALLY = 129, // 128 + 1
                        AFFECTS_GAS_GLOBALLY = 130, // 128 + 2
                        AFFECTS_MINERALS_AND_GAS_GLOBALLY = 131 // 128 + 2 + 1
                };

		enum eRace
		{
			TERRA,
			PROTOSS,
			ZERG
		}; 

		enum eBroodwarUnits
		{
			NONE,
			
			MINERAL_PATCH,
			VESPENE_GEYSIR,

			SCV,
			GAS_SCV,
			SCV_FROM_GAS_TO_MINERALS,
			TERRA_WAIT,
			
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
			
			REFINERY,
			COMMAND_CENTER,
			SUPPLY_DEPOT,
			BARRACKS,
			ACADEMY,
			FACTORY,
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
			
                        // level 1 at first for 'adjustResearches' to work correctly
                        INFANTRY_ARMOR_1,
                        INFANTRY_WEAPONS_1,
                        VEHICLE_PLATING_1,
                        VEHICLE_WEAPONS_1,
                        SHIP_PLATING_1,
                        SHIP_WEAPONS_1,
                        INFANTRY_ARMOR_2,
                        INFANTRY_ARMOR_3,
                        INFANTRY_WEAPONS_2,
                        INFANTRY_WEAPONS_3,
                        VEHICLE_PLATING_2,
                        VEHICLE_PLATING_3,
                        VEHICLE_WEAPONS_2,
                        VEHICLE_WEAPONS_3,
                        SHIP_PLATING_2,
                        SHIP_PLATING_3,
                        SHIP_WEAPONS_2,
                        SHIP_WEAPONS_3,

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

			F_FACTORY_ADDON,
			F_STARPORT_ADDON,
			F_COMMAND_CENTER_ADDON,
			F_SCIENCE_FACILITY_ADDON,

			UNIT_TYPE_COUNT_TERRA,


		//} // end eTerranUnits
		//
		//enum eProtossUnits
		//{
			PROBE = 3,
			GAS_PROBE,
			PROBE_FROM_GAS_TO_MINERALS,
			
			PROTOSS_WAIT,

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
			
			ASSIMILATOR,
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
			
                        ARMOR_1,
                        PLATING_1,
                        GROUND_WEAPONS_1,
                        AIR_WEAPONS_1,
                        PLASMA_SHIELDS_1,
                        ARMOR_2,
                        ARMOR_3,
                        PLATING_2,
                        PLATING_3,
                        GROUND_WEAPONS_2,
                        GROUND_WEAPONS_3,
                        AIR_WEAPONS_2,
                        AIR_WEAPONS_3,
                        PLASMA_SHIELDS_2,
                        PLASMA_SHIELDS_3,
			
			R_PSIONIC_STORM,
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
			UNIT_TYPE_COUNT_PROTOSS,
		//}
		//
		//enum eZergUnits
		//{
			DRONE = 3,
			GAS_DRONE,
			DRONE_FROM_GAS_TO_MINERALS,
			
			ZERG_WAIT,
			BREAK_UP_BUILDING,

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
			
			EXTRACTOR,
			HATCHERY,
			LAIR,
			HIVE,
			NYDUS_CANAL,
			HYDRALISK_DEN,
			DEFILER_MOUND,
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

                        CARAPACE_1,
                        FLYER_CARAPACE_1,
                        MELEE_ATTACKS_1,
                        MISSILE_ATTACKS_1,
                        FLYER_ATTACKS_1,

                        CARAPACE_2,
//                      CARAPACE_2_T,
                        CARAPACE_3,
                        FLYER_CARAPACE_2,
//                      FLYER_CARAPACE_2_T,
                        FLYER_CARAPACE_3,
                        MELEE_ATTACKS_2,
//                      MELEE_ATTACKS_2_T,
                        MELEE_ATTACKS_3,
                        MISSILE_ATTACKS_2,
//                      MISSILE_ATTACKS_2_T,
                        MISSILE_ATTACKS_3,
                        FLYER_ATTACKS_2,
//                      FLYER_ATTACKS_2_T,
                        FLYER_ATTACKS_3,
			
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
			R_FLYER_ATTACKS,
			UNIT_TYPE_COUNT_ZERG
		}; // end eZergUnits
                
		enum eBroodwarStrings
                {
                        NULL_STRING,
                        TERRA_STRING,
                        PROTOSS_STRING,
                        ZERG_STRING,

                        MINERAL_STRING = 10,
                        VESPENE_GAS_STRING,
// force window:
                        UNIT_TYPE_0_STRING = 20,
                        UNIT_TYPE_1_STRING,
                        UNIT_TYPE_2_STRING,
                        UNIT_TYPE_3_STRING,
                        UNIT_TYPE_4_STRING,
                        UNIT_TYPE_5_STRING,
                        UNIT_TYPE_6_STRING,
                        UNIT_TYPE_7_STRING,
                        UNIT_TYPE_8_STRING,
                        UNIT_TYPE_9_STRING,
                        UNIT_TYPE_10_STRING,

                        GAME_SPEED_SLOWEST_STRING = 40,
                        GAME_SPEED_SLOWER_STRING, // 9
                        GAME_SPEED_SLOW_STRING, // 12
                        GAME_SPEED_NORMAL_STRING, // 15
                        GAME_SPEED_FAST_STRING, // 18
                        GAME_SPEED_FASTER_STRING, // 21
                        GAME_SPEED_FASTEST_STRING, // 24

			DIAGRAM_SUPPLY_STRING = 50,
			DIAGRAM_TIME_STRING,
			
                        MAX_BROODWAR_STRINGS
                };
		enum eGameSpeedFrames
		{
			GAME_SPEED_SLOWEST,
			GAME_SPEED_SLOWER,
			GAME_SPEED_SLOW,
			GAME_SPEED_NORMAL,
			GAME_SPEED_FAST,
			GAME_SPEED_FASTER,
			GAME_SPEED_FASTEST,
			MAX_GAME_SPEED
		};

	private:
};


#endif // _CORE_BROODWAR_HPP




