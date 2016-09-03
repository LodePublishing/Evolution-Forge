#include "race.hpp"
#include <math.h>
#include "../stl/misc.hpp"

UNIT_STATISTICS::UNIT_STATISTICS():
	BT(0),
	needSupply(0),
	haveSupply(0),
	facility2(0),
	facilityType(NO_FACILITY_BEHAVIOUR_DEFINED),
	create(0),
	unitType(INVALID_UNIT_TYPE),
	hp(0),
	xp(0),
	affectsResource(0)
{
	resource[0] = 0;
	resource[1] = 0;
	prerequisite[0] = 0;
	prerequisite[1] = 0;
	prerequisite[2] = 0;
	facility[0] = 0;
	facility[1] = 0;
	facility[2] = 0;
}

UNIT_STATISTICS::UNIT_STATISTICS(unsigned int bt, unsigned int res0, unsigned int res1, unsigned int need_supply, unsigned int have_supply, unsigned int prereq0, unsigned int prereq1, unsigned int prereq2, unsigned int fac0, unsigned int fac1, unsigned int fac2, unsigned int f2, eFacilityType facility_type, unsigned int unit_create, eUnitType unit_type, unsigned int affects_resource) :
	BT(bt),
	needSupply(need_supply),
	haveSupply(have_supply),
	facility2(f2),
	facilityType(facility_type),
	create(unit_create),
	unitType(unit_type),
	hp(0),
	xp(0),
	affectsResource(affects_resource)
{
	resource[0] = res0;
	resource[1] = res1;
	prerequisite[0] = prereq0;
	prerequisite[1] = prereq1;
	prerequisite[2] = prereq2;
	facility[0] = fac0;
	facility[1] = fac1;
	facility[2] = fac2;
}

UNIT_STATISTICS::UNIT_STATISTICS(unsigned int bt, unsigned int res0, unsigned int res1, unsigned int need_supply, unsigned int have_supply, unsigned int prereq0, unsigned int prereq1, unsigned int prereq2, unsigned int fac0, unsigned int fac1, unsigned int fac2, unsigned int f2, eFacilityType facility_type, unsigned int unit_create, eUnitType unit_type, unsigned int unit_hp, unsigned int unit_xp, unsigned int affects_resource) :
	BT(bt),
	needSupply(need_supply),
	haveSupply(have_supply),
	facility2(f2),
	facilityType(facility_type),
	create(unit_create),
	unitType(unit_type),
	hp(unit_hp),
	xp(unit_xp),
	affectsResource(affects_resource)
{
	resource[0] = res0;
	resource[1] = res1;
	prerequisite[0] = prereq0;
	prerequisite[1] = prereq1;
	prerequisite[2] = prereq2;
	facility[0] = fac0;
	facility[1] = fac1;
	facility[2] = fac2;
}

UNIT_STATISTICS::~UNIT_STATISTICS()
{}
#include <sstream>

void RESOURCE::calculateHarvestSpeed()
{
	resourceHarvestPerFrame.resize(MAX_RESOURCE_DISTANCE);
	for(unsigned int distance_time = MAX_RESOURCE_DISTANCE; distance_time--;)
	{
		resourceHarvestPerFrame[distance_time].resize(MAX_RESOURCE_PROVIDER_PER_LOCATION);
//		toErrorLog("--------------------");
//		toErrorLog(distance_time);
//		toErrorLog("--------------------");
		for(unsigned int provider = MAX_RESOURCE_PROVIDER_PER_LOCATION; provider--;)
		{
			resourceHarvestPerFrame[distance_time][provider].resize(MAX_RESOURCE_WORKERS);
			unsigned int old_value = 9999999;
//			bool changed = true;
			std::ostringstream os;
			unsigned int max = provider * gatherAmount / harvestTime;
//			os << provider << "x provider: [MAX: " << max << "]:";


			unsigned int a1 = provider * ( harvestTime + 30 * distance_time) - harvestTime;
			unsigned int a2 = provider * ( harvestTime + 30 * distance_time);
			
			for(unsigned int worker = MAX_RESOURCE_WORKERS; (worker--)/*&&(changed)*/;)
			{

		                unsigned int harvest = 0;
                		float wait_time = 0;
		                float test = 0;

                		for(unsigned int j = 0; j < 10; j++)
		                {
                		        test = 0.0;
		                        for(unsigned int k = 1; k < worker; k++)
                		                test += powf(1.0 - powf( ((float)a1 + (float)provider * wait_time) / ((float)a2 + (float)provider * wait_time), worker - 1), (float)k) * ((float)harvestTime * (float)k /*- harvestTime*/ / 2.0);
		                        if(test > wait_time)
                		                wait_time += (test - wait_time) / 2.0;
		                        else if(test < wait_time)
                		                wait_time -= (wait_time - test) / 2.0;
		                }

// TODO worker speed mit rein in distance
				unsigned int value = 0;
		//                if(worker <= provider)
		//     	                value = worker * gatherAmount / (harvestTime + 2.0 * distance_time);
		//		else
		       	                value = (float)worker * (float)gatherAmount / ((float)harvestTime + 30.0 * (float)distance_time + wait_time);
					if(value > max)
						value = max;

//				        value = worker * gatherAmount / ( harvestTime + 2.0 * distance_time + (0.5 * (sqrt(2.0 * distance_time + harvestTime + 4 * (worker-1) * harvestTime ) - 2.0 * distance_time - harvestTime)));
		//		if(value == old_value)
		//		{
		//			changed = false;
		//			break;
		//		}
		//		old_value = value;
				resourceHarvestPerFrame[distance_time][provider][worker] = value;
	//			os << value << " ";
			}
	//		toErrorLog(os.str());
		}
	}
// nach naehestem command_center suchen TODO
// nach naehesten Mineralien suchen TODO
//TODO: Wenn 2 SPieler an einem sammeln, beide einberechnen!
}

void RACE::initResources()
{
	for(unsigned int i = MAX_RESOURCE_TYPES; i--;)
		resource[i].calculateHarvestSpeed();
}

void RACE::fillInfluenceList()
{
	allows.resize(UNIT_TYPE_COUNT);
	needs.resize(UNIT_TYPE_COUNT);
	stats.resize(UNIT_TYPE_COUNT);
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
	{
//		allows[i].facility.clear();
//		allows[i].facility2.clear();
//		allows[i].prerequisite.clear();
		needs[i].facility = false;
		needs[i].facility2 = false;
		needs[i].prerequisite = 0;;
	}
	
	for(unsigned int i = UNIT_TYPE_COUNT; i--;)
	{
		for(unsigned int j = 3; j--;)
		{
			if(stats[i].facility[j] )
				allows[stats[i].facility[j]].facility.push_back( i );
			if( stats[i].prerequisite[j] )
				allows[stats[i].prerequisite[j]].prerequisite.push_back( i );
		}
		if(stats[i].facility2)
			allows[stats[i].facility2].facility2.push_back( i );
		
		if( stats[i].facility[0] || stats[i].facility[1] || stats[i].facility[2] )
			needs[i].facility = true;
		
		for(unsigned int j = 3; j--;)
			if(stats[i].prerequisite[j])
				++needs[i].prerequisite;
		
		if(stats[i].facility2)
			needs[i].facility2 = true;
	}
}


void RACE::setResearchParameters(const unsigned int first, const unsigned int last, const unsigned int basic) 
{
	research.first = first;
	research.last = last;
	research.basic = basic;
}

void ALLOW::updateBuildableList(std::vector<NEED>& need, std::list<unsigned int>& new_buildable, const unsigned int unit)
{
	for(std::list<unsigned int>::const_iterator j = facility.begin(); j != facility.end(); ++j)
		if(need[*j].facilityIsDone())
			new_buildable.push_back(*j);
	for(std::list<unsigned int>::const_iterator j = facility2.begin(); j != facility2.end(); ++j)
		if(need[*j].facility2IsDone())
			new_buildable.push_back(*j);
	for(std::list<unsigned int>::const_iterator j = prerequisite.begin(); j != prerequisite.end(); ++j)
		if(need[*j].prerequisiteIsDone())
			new_buildable.push_back(*j);
}


const bool NEED::facilityIsDone()
{
	if(facility)
	{
		facility = false;
		if((!facility2)&&(prerequisite == 0))
			return(true);
	}
	return(false);
}

const bool NEED::facility2IsDone()
{
	if(facility2)
	{
		facility2 = false;
		if((!facility) && (prerequisite == 0))
			return(true);
	}
	return(false);
}

const bool NEED::prerequisiteIsDone()
{
	if(prerequisite>0)
	{
		--prerequisite;
		if((!facility) && (!facility2) && (prerequisite==0))
			return(true);
	}
	return(false);
}

NEED& NEED::operator=(const NEED& object)
{
	facility = object.facility;
	facility2 = object.facility2;
	prerequisite = object.prerequisite;
	return(*this);
}

unsigned int RACE::UNIT_TYPE_COUNT = 0;
unsigned int RACE::MAX_RESOURCE_TYPES = 0;

unsigned int RESOURCE::MAX_RESOURCE_WORKERS = 0;
unsigned int RESOURCE::MAX_RESOURCE_PROVIDER_PER_LOCATION = 0;
unsigned int RESOURCE::MAX_RESOURCE_DISTANCE = 0;



