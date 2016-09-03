#ifndef __TEXT_H
#define __TEXT_H

#include "main.h"

// this is mainly for for io to display some of the output

struct NAME
{
	char value[6];
};

const char error_message[ERROR_MESSAGES][21]=
{
	"OK",
	"enough Minerals",
	"enough Gas",
	"supply satisfied",
	"prerequisite built",
	"facility availible",
	"timeout",
	"unknown problem"
};


char error_small_message[ERROR_MESSAGES]=
{
	'O','M','G','S','P','F','T','U'
};

const NAME short_name[RACES][UNIT_TYPE_COUNT]=
{
	{
	{"NULL!"},{"SCV  "},{"Marin"},{"Ghost"},{"Vultu"},{"Golia"},{"STank"},{"FiBat"},{"Medic"},{"Wrait"},
	{"SciVe"},{"Drops"},{"BCrui"},{"Valky"},{"NucWa"},{"CommC"},{"Depot"},{"Barra"},{"Acade"},{"Facto"},
	{"StarP"},{"SciLa"},{"Engin"},{"Armor"},{"MisTu"},{"Bunke"},{"ComSa"},{"NucSi"},{"ConTo"},{"CovOp"},
	{"PhyLa"},{"MasSh"},{"CCCoS"},{"CCNuS"},{"StpCT"},{"ScFCO"},{"ScFPL"},{"FacMS"},{"StimP"},{"LockD"},
	{"EMPSw"},{"SMine"},{"SiegM"},{"Irrad"},{"YamaG"},{"Cloak"},{"PersC"},{"Resto"},{"OptcF"},{"U238S"},
	{"IonTh"},{"TitaR"},{"OculI"},{"MoebR"},{"ApolR"},{"ColoR"},{"CaduR"},{"CharB"},{"InfAr"},{"InfWe"},
	{"VehAr"},{"VehWe"},{"ShpAr"},{"ShpWe"},{"Refin"},{"VeSCV"},{"Move1"},{"Move3"},{"Mov-1"},{"NULL!"},
	{"NULL!"},{"NULL!"},{"VespG"},{"Miner"},{"StimP"},{"LockD"},{"EMPSw"},{"SMine"},{"SiegM"},{"Irrad"},
	{"YamaG"},{"Cloak"},{"PersC"},{"Resto"},{"OptcF"},{"U238S"},{"IonTh"},{"TitaR"},{"OculI"},{"MoebR"},
	{"ApolR"},{"ColoR"},{"CaduR"},{"CharB"},{"InfAr"},{"InfWe"},{"VehAr"},{"VehWe"},{"ShpAr"},{"ShpWe"}
	},

	{
	{"NULL!"},{"Probe"},{"DarkT"},{"DarkA"},{"Probe"},{"Zealo"},{"HighT"},{"Archo"},{"Reave"},{"5xSca"},
	{"10xSc"},{"Corsa"},{"Shutt"},{"Scout"},{"Arbit"},{"Carri"},{"5xInt"},{"8xInt"},{"Obser"},{"Nexus"},
	{"RobFa"},{"Pylon"},{"ObsTy"},{"GateW"},{"Photo"},{"Cyber"},{"Citad"},{"TempA"},{"Forge"},{"StarG"},
	{"Fleet"},{"ArbTr"},{"RobBy"},{"Shiel"},{"PsiSt"},{"Hallu"},{"Recal"},{"Stasi"},{"Disru"},{"MindC"},
	{"Maels"},{"Singu"},{"LegEn"},{"ScrbD"},{"ReavC"},{"GraDr"},{"Senso"},{"GraBo"},{"KhaAm"},{"ApiaS"},
	{"GraTh"},{"CarCa"},{"KhaCo"},{"ArgJe"},{"ArgTa"},{"Armor"},{"Plati"},{"Groun"},{"AirWe"},{"PShie"},
	{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},{"Assim"},{"VePrb"},{"Move1"},{"Move3"},{"Mov-1"},{"NULL!"},
	{"NULL!"},{"NULL!"},{"VespG"},{"Miner"},{"PsiSt"},{"Hallu"},{"Recal"},{"Stasi"},{"Disru"},{"MindC"},
	{"Maels"},{"Singu"},{"LegEn"},{"ScrbD"},{"ReavC"},{"GraDr"},{"Senso"},{"GraBo"},{"KhaAm"},{"ApiaS"},
	{"GraTh"},{"CarCa"},{"KhaCo"},{"ArgJe"},{"ArgTa"},{"Armor"},{"Plati"},{"Groun"},{"AirWe"},{"PShie"},
	},
		
	{
	{"NULL!"},{"Drone"},{"Larva"},{"ZLing"},{"Hydra"},{"Ultra"},{"Defil"},{"Lurkr"},{"Overl"},{"Mutal"},
	{"Guard"},{"Queen"},{"Scour"},{"Devou"},{"Hatch"},{"Lair "},{"Hive "},{"Nydus"},{"HyDen"},{"DeMou"},
	{"GSpir"},{"QNest"},{"EvoCh"},{"UCave"},{"Spire"},{"SPool"},{"Creep"},{"Spore"},{"Sunkn"},{"VSacs"},
	{"Anten"},{"Pneum"},{"Boost"},{"Adren"},{"Muscu"},{"Groov"},{"Gamet"},{"MNode"},{"Chiti"},{"Anabo"},
	{"Burro"},{"Spawn"},{"Plagu"},{"Consu"},{"Ensna"},{"LAspc"},{"Carap"},{"FCarp"},{"Melee"},{"Missl"},
	{"FAtta"},{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},
	{"NULL!"},{"NULL!"},{"NULL!"},{"NULL!"},{"Extra"},{"VeSCV"},{"Move1"},{"Move3"},{"Mov-1"},{"NULL!"},
	{"NULL!"},{"NULL!"},{"VespG"},{"Miner"},{"Cancl"},{"VSacs"},{"Anten"},{"Pneum"},{"Boost"},{"Adren"},
	{"Muscu"},{"Groov"},{"Gamet"},{"MNode"},{"Chiti"},{"Anabo"},{"Burro"},{"Spawn"},{"Plagu"},{"Consu"},
	{"Ensna"},{"LAspc"},{"Carap"},{"FCarp"},{"Melee"},{"Missl"},{"FAtta"},{"NULL!"},{"NULL!"},{"NULL!"}
	}
};	


#endif

