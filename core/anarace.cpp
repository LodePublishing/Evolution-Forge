#include "anarace.hpp"

/* ANARACE.CPP - last changed: 6/23/04                              *
 * Author: Clemens Lode                                             *
 * Copyright: Clemens Lode                                          *
 *                                                                  *
 *  PURPPOSE OF THIS FILE:                                          *
 *      This is the core of the core, where the main calculations   *
 *      are made. Also this class is used as an interface to the    *
 *      core from outside the library. In addition some data is     *
        preformatted/precalculated -> statistics.				    */

// TODO: reimplement/recheck the speed of the units
ANARACE::ANARACE(): 
	unitsTotal(4),
	unitsTotalMax(4),
	unchangedGenerations(0),
	currentRun(0),
	optimizing(false),
	active(true),
	totalGeneration(0),
	maxpFitness(0),
	maxsFitness(0),
	maxtFitness(MAX_TFITNESS),
	timePercentage(0),
	goalPercentage(0),
	currentpFitness(0),
	averageLength(0),
	fitnessAverage(0),
	fitnessVariance(0),
	positiveCrossover(0),
	wayneCrossover(0),
	negativeCrossover(0)
{
    resetSpecial();
}

ANARACE::~ANARACE()
{ }

ANARACE::ANARACE(const ANARACE& object) :
	PRERACE((PRERACE)object),
	unitsTotal( object.unitsTotal ),
	unitsTotalMax( object.unitsTotalMax ),
	unchangedGenerations( object.unchangedGenerations ),
	currentRun( object.currentRun ),
	optimizing( object.optimizing ),
	active( object.active ),
	totalGeneration( object.totalGeneration ),
	maxpFitness( object.maxpFitness ),
	maxsFitness( object.maxsFitness ),
	maxtFitness( object.maxtFitness ),
	timePercentage( object.timePercentage ),
	goalPercentage( object.goalPercentage ),
	currentpFitness( object.currentpFitness ),
	averageLength( object.averageLength ),
	fitnessAverage( object.fitnessAverage ),
	fitnessVariance( object.fitnessVariance ),
	positiveCrossover( object.positiveCrossover ),
	wayneCrossover( object.wayneCrossover ), 
	negativeCrossover( object.negativeCrossover )
{
	for(int i = MAX_LENGTH;i--;)
	{
		phaenoCode[i] = object.phaenoCode[i];
		program[i] = object.program[i];
		statistics[i] = object.statistics[i];
	}		
}

ANARACE& ANARACE::operator=(const ANARACE& object)
{
	(PRERACE)(*this) = (PRERACE)object;
    unitsTotal = object.unitsTotal;
    unitsTotalMax = object.unitsTotalMax;
    unchangedGenerations = object.unchangedGenerations;
    currentRun = object.currentRun;
    optimizing = object.optimizing;
    active = object.active;
    totalGeneration = object.totalGeneration;
    maxpFitness = object.maxpFitness;
    maxsFitness = object.maxsFitness;
    maxtFitness = object.maxtFitness;
    timePercentage = object.timePercentage;
    goalPercentage = object.goalPercentage;
    currentpFitness = object.currentpFitness;
    averageLength = object.averageLength;
    fitnessAverage = object.fitnessAverage;
    fitnessVariance = object.fitnessVariance;
    positiveCrossover = object.positiveCrossover;
    wayneCrossover = object.wayneCrossover;
    negativeCrossover = object.negativeCrossover;
    for(int i = MAX_LENGTH;i--;)
    {
        phaenoCode[i] = object.phaenoCode[i];
        program[i] = object.program[i];
        statistics[i] = object.statistics[i];
    }
	return(*this);
}



void ANARACE::resetData()
{
	resetSpecial();
	setTotalGeneration(0);
	setMaxpFitness(0);
	setMaxsFitness(0);
	setMaxtFitness(MAX_TFITNESS);
	setUnchangedGenerations(0);
	unitsTotal=4;
	unitsTotalMax=4;
	timePercentage=0;
	goalPercentage=0;
	averageLength=0;
	setTimer(0);
}


// Reset all ongoing data (between two runs)
void ANARACE::prepareForNewGeneration() // resets all data to standard starting values
{
	PRERACE::prepareForNewGeneration();
	setCurrentpFitness(0);

	for(int i=MAX_LENGTH;i--;)
	{
		setStatisticsHaveSupply(i, 0);
		setStatisticsHaveMinerals(i, 0);
		setStatisticsHaveGas(i, 0);
		setStatisticsNeedSupply(i, 0);
		setStatisticsFitness(i, 0);
	}

	for(int i=MAX_LENGTH;i--;)//ANA~
	{
		setProgramSuccessType(i, 0);
		setProgramSuccessUnit(i, 0);
		setProgramIsBuilt(i, false);
		setProgramTime(i, 0);
		for(int j=UNIT_TYPE_COUNT;j--;)
		{
			setProgramAvailibleCount(i, j, 0);
			setProgramTotalCount(i, j, 0);
		}
		//program[i].temp=0;
		setProgramLocation(i,0);
//		setProgramIsGoal(i,false);
		setProgramFacility(i,0);
		setProgramBT(i,0);
		phaenoCode[i]=0;
	}
}

// ----------------------------------------
// ------ CORE OF THE CORE FUNCTIONS ------
// ----------------------------------------

const bool ANARACE::calculateStep()
{
//ZERG: CREEP!
//PROTOSS: Bauen: Hin und rueckfahren! PYLON!
	//TODO: Fehler hier, getHaveSupply - getNeedSupply kann -1 werden!
	// needSupply war 11, maxneedSupply war 10 :/

	if((!getTimer())||(ready=calculateReady())||(!getIP()))
	{
		setLength(configuration.getMaxLength()-getIP());
		if(!ready) 
			setTimer(0);
		while(!buildingQueue.empty())
            buildingQueue.pop();

		for(int i=MAX_LENGTH;i--;)
			phaenoCode[i]=getpGoal()->toPhaeno(getCode(i));
	//	if(getpGoal()->getMode()==0)
			setCurrentpFitness(calculatePrimaryFitness(ready));
		
// ------ ANARACE SPECIFIC ------
		countUnitsTotal();
		int maxPoints=getpGoal()->countGoals();
		if(maxPoints>0)
			goalPercentage = 100 * currentpFitness / maxPoints;
		else goalPercentage = 0;
// ------ END ANARACE SPECIFIC ------
		
		return(true);
	}

	bool ok=true;
	while((ok)&&(getIP()))
	{
// set needed_ to maximum to determine the minimum of minerals/gas our jobs need (needed_ is set in buildGene)
		neededMinerals=MAX_MINERALS;
		neededGas=MAX_GAS;
		setStatisticsNeedSupply(getIP(), getNeedSupply());
		setStatisticsHaveSupply(getIP(), getHaveSupply());
		setStatisticsHaveMinerals(getIP(), getMinerals());
		setStatisticsHaveGas(getIP(), getGas());
		setStatisticsFitness(getIP(), calculatePrimaryFitness(ready));
	
		ok=buildGene(getpGoal()->toPhaeno(getCurrentCode()));
		
		if(successType>0)
		{
			setProgramSuccessType(getIP(), successType);
			setProgramSuccessUnit(getIP(), successUnit);
		}
		
//TODO PROBLEM: wenn Einheit gebaut wurde (also in die build list incl. IP eingefuegt wurde) aber gleichzeitig ein timeout war, wird die Einheit als TIMEOUT markiert aber trotzdem gebaut
// Problemloesung: Ueberpruefen, dass utnen auch wirklich das Mininimum gewaehlt fuer t gewaehlt wird! 

		if((ok)||(!getTimeOut())) {
			if(ok) {
				setProgramTime(getIP(), getTimer());
			} else {
				setProgramTime(getIP(),configuration.getMaxTime());
				setProgramSuccessType(getIP(), TIMEOUT_ERROR);
				setProgramSuccessUnit(getIP(), 0);
//				setProgramSuccessLocation(0);
			}
			for(int i=UNIT_TYPE_COUNT;i--;)	{
				setProgramTotalCount(getIP(), i, getLocationTotal(GLOBAL, i));
				setProgramAvailibleCount(getIP(),i,getLocationAvailible(GLOBAL, i));
			}
	
			setTimeOut(configuration.getMaxTimeOut());
			setIP(getIP()-1);
		}
	}

//  ------ LEAP FORWARD IN TIME ------
	int t=calculateIdleTime();
//    int oldMinerals = getMinerals();
  //  int oldGas = getGas();

	setMinerals(getMinerals()+harvestMinerals()*t);
	setHarvestedMinerals(getHarvestedMinerals()+harvestMinerals()*t);
//    setWastedMinerals(getWastedMinerals() + oldMinerals*t + (getMinerals() - oldMinerals) * t / 2);
	
	setGas(getGas()+harvestGas()*t);
	setHarvestedGas(getHarvestedGas()+harvestGas()*t);
  //  setWastedGas(getWastedGas() + oldGas*t + (getGas() - oldGas) * t / 2);

	setTimeOut(getTimeOut()-t);
	setTimer(getTimer()-t);
//  ------ END LEAP FORWARD IN TIME ------
	
	bool foundAnother=true;
    while((!buildingQueue.empty())&&(foundAnother==true))
    {
        if((buildingQueue.top().canBeCompleted()) && (buildingQueue.top().getBuildFinishedTime() == getTimer()))
        {
			foundAnother=true;
	       	const Building build = buildingQueue.top();
			const UNIT_STATISTICS* stat=&((*pStats)[build.getType()]);

// ------ ACTUAL BUILDING ------
			adjustLocationUnitsAfterCompletion(build.getLocation(), stat->facilityType, build.getFacility(), stat->facility2);			
			setHaveSupply(getHaveSupply()+stat->haveSupply);
			addLocationTotal(build.getLocation(),build.getType(), build.getUnitCount());
			addLocationAvailible(build.getLocation(),build.getType(),build.getUnitCount());
// ------ END OF ACTUAL BUILDING ------

			
// ------ SPECIAL RULES ------
			if(build.getType()==REFINERY) {
				removeOneMapLocationTotal(GLOBAL, build.getLocation(), VESPENE_GEYSIR);
				adjustGasHarvest(build.getLocation());
			} else
			if((build.getType()==COMMAND_CENTER)&&(!getLocationTotal(build.getLocation(),COMMAND_CENTER))) {
				adjustMineralHarvest(build.getLocation());
				adjustGasHarvest(build.getLocation());
			} else 
// RACE SPECIFIC!
			if((build.getType()==LARVA)&&(getpGoal()->getRace()==ZERG)&&(!buildGene(LARVA))) {
				removeLarvaFromQueue(build.getLocation());
			}
// ------ END SPECIAL RULES ------

			
// ------ CHECK WHETHER WE ARE READY ------
			getpGoal()->calculateFinalTimes(build.getLocation(), build.getType(), getLocationTotal(build.getLocation(), build.getType()), getRealTimer());
			ready=calculateReady();
// ------ END CHECK ------

			
// ------ ENQUEUE THE LAST ITEM SO IT CAN BE ACCESSED BY THE MOVING ROUTINES ------
			last[lastcounter].unit=build.getType();
			last[lastcounter].count=build.getUnitCount();
			last[lastcounter].location=build.getLocation();

			if((stat->create)&&(!build.getOnTheRun())) //one additional unit (zerglings, scourge, comsat, etc.)
			{ //here no unitCount! ~~~
				addOneLocationTotal(build.getLocation(),stat->create);
				addOneLocationAvailible(build.getLocation(),stat->create);
				if( last[lastcounter].unit == stat->create ) 
					last[lastcounter].count++; //TODO ???
				// ~~~~ Ja... geht schon... aber kann ja auch mal was anderes sein...
			}
					//evtl noch location==0 als 'egal wo' einfuehren
			lastcounter++;
// ------ END OF LAST ITEM ------
			
// ------ RECORD THE BUILDING ------
			setProgramFacility(build.getIP(), build.getFacility());
			setProgramIsBuilt(build.getIP(),true);
			setProgramLocation(build.getIP(),build.getLocation());
			setProgramBT(build.getIP(),build.getTotalBuildTime()); //~~~
// ------ END OF RECORD -------
			
			buildingQueue.pop();
		} // end while(getremainingbuildorder) ~|~
		else foundAnother=false;
	} //end while
//TODO: Alles rausschmeissen, was schon von race berechnet wurde!
	
	return(false);

	//TODO: Auch voruebergehende Ziele miteinberechnen (Bewegungen!)
	//Also quasi eine zweite Goalreihe rein um scvs/Einheiten zu belohnen die bestimmte Orte besetzen... erstmal nur scvs... also z.B. int tempGoal...
	//mmmh... aber was reinschreiben? baue barracks bei Ort bla => belohne EINMAL ein scv bei ort bla
}

// falschen Standort ueber distances abrechnen! (100-distance oder so... je nach dem wieviele am falschen Ort sind!)

const bool ANARACE::buildGene(const unsigned int build_unit)
{
	const UNIT_STATISTICS* stat=&((*pStats)[build_unit]);
	bool ok=false;

	successType=OK;
	successUnit=0;

	if(build_unit<=REFINERY+1)
	{
		if((stat->prerequisite[0]>0)&&(getLocationTotal(GLOBAL,stat->prerequisite[0])==0)) {
			successUnit=stat->prerequisite[0];
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else		
		if((stat->prerequisite[1]>0)&&(getLocationTotal(GLOBAL,stat->prerequisite[1])==0)) {
			successUnit=stat->prerequisite[1];
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else 
		if((stat->prerequisite[2]>0)&&(getLocationTotal(GLOBAL,stat->prerequisite[2])==0)) {
			successUnit=stat->prerequisite[2];
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else
		if //ANA~
//TODO, < ((stat->facilityType==IS_LOST)&&(stat->facility[fac]==stat->facility2)) einfuegen....
			( ((stat->facility[0]==0)||(getLocationAvailible(GLOBAL, stat->facility[0])==0))&&
			  ((stat->facility[1]==0)||(getLocationAvailible(GLOBAL, stat->facility[1])==0))&&
			  ((stat->facility[2]==0)||(getLocationAvailible(GLOBAL, stat->facility[2])==0))&&
			  ((stat->facility[0]>0)||(stat->facility[1]>0)||(stat->facility[2]>0))
			)
 //TODO: availible/force[0] auf 100 setzen... naj ama guggn
		{
			if(stat->facility[0]>0)
				successUnit=stat->facility[0];
			else if(stat->facility[1]>0)
				successUnit=stat->facility[1];
			else if(stat->facility[2]>0)
				successUnit=stat->facility[2];
			successType=FACILITY_BECAME_AVAILIBLE;
		}
		else
		if((stat->facility2>0)&&(getLocationAvailible(GLOBAL, stat->facility2)==0))
		{
			successUnit=stat->facility2;
			successType=FACILITY_BECAME_AVAILIBLE;
		}
//TODO: evtl success 2 Schritte zurueckverfolgen...
		else
		if(((getHaveSupply()<getNeedSupply()+stat->needSupply-stat->haveSupply) ||
		    (stat->needSupply+getNeedSupply()>MAX_SUPPLY)) &&
			(stat->needSupply!=0)) {
			successUnit=0;
			successType=SUPPLY_WAS_SATISFIED;
		}
		else
		if(getMinerals()<stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit))
		{
			successUnit=0;
			successType=ENOUGH_MINERALS_WERE_GATHERED;
			if(neededMinerals>stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getMinerals())
				neededMinerals=stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getMinerals();
//Fehler is wahrscheinlich hier irgendwoe...

		}
		else
		if(getGas()<stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit))
		{
			successUnit=0;
			successType=ENOUGH_GAS_WAS_GATHERED;
			if(neededGas>stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getGas())
				neededGas=stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)-getGas();
		}
		else
		if((stat->upgrade[0]>0)&&(getLocationTotal(GLOBAL, build_unit)>=2) && (!getLocationTotal(GLOBAL, stat->upgrade[0])))
		{
            successUnit=stat->upgrade[0];
			successType=PREREQUISITE_WAS_FULFILLED;
		}
		else
		if((stat->upgrade[1]>0)&&(getLocationTotal(GLOBAL, build_unit)>=3) && (!getLocationTotal(GLOBAL, stat->upgrade[1])))
		{
            successUnit=stat->upgrade[1];
            successType=PREREQUISITE_WAS_FULFILLED;
		}
		else	
		{
//Zuerst: availible pruefen ob am Ort gebaut werden kann
//Wenn nicht => +/- absteigen bis alle locations durch sind
			int fac=0;
			int tloc=1;
			int ttloc=0;
			int j=0;
		
			if(lastcounter>0)
			{
				lastcounter--;
				tloc=last[lastcounter].location;
			}
// TODO!
//			if((stat->facility2==0)||(getLocationAvailible(tloc,stat->facility2)>0))
//			for(fac=3;fac--;)
			for(fac=0;fac<3; fac++)
				if( ((stat->facility[fac]>0)&&(getLocationAvailible(tloc, stat->facility[fac]))&&
				((getLocationAvailible(ttloc, stat->facility[fac])>1)||(stat->facilityType!=IS_LOST)||(stat->facility[fac]==stat->facility2)))  )
	//		||	((stat->facility[fac]==0)&&(fac==0))) 
				{
					ok=true;
					break;
				}
		
			j=1;
			if(!ok)
				while(j<MAX_LOCATIONS)
				{
					ttloc=(*pMap)->getLocation(tloc)->getNearest(j);
//					if((stat->facility2==0)||(getLocationAvailible(ttloc,stat->facility2)>0))
//					{
//					for(fac=3;fac--;)
					for(fac=0;fac<3; fac++)
						if(((stat->facility[fac]>0)&&(getLocationAvailible(ttloc, stat->facility[fac]))&&
						// special rules for morphing units
						((getLocationAvailible(ttloc, stat->facility[fac])>1)||(stat->facilityType!=IS_LOST)||(stat->facility[fac]!=stat->facility2)) ))
	//					|| ((stat->facility[fac]==0)&&(fac==0))) //~~
						{
							tloc=ttloc;
							ok=true;
							break;
						}
					break;
//					}
					j++;
				}	
//				bewegliche Sachen ueberdenken...
//					evtl zusaetzliche Eigenschaft 'speed' einbauen (muss sowieso noch...)... bei speed>0 ... mmmh... trifft aber auch nur auf scvs zu ... weil bringt ja wenig erst mit der hydra rumzulaufen und dann zum lurker... mmmh... aber waere trotzdem zu ueberlegen...
//					auch noch ueberlegen, wenn z.B. mit scv ohne kommandozentrale woanders gesammelt wird...
//		Phagen ueber Phagen...

			if((ok)&&(build_unit==REFINERY))
			{
				if(getMapLocationAvailible(NEUTRAL_PLAYER, tloc, VESPENE_GEYSIR)<=0)
					ok=false;
				else
					removeOneMapLocationAvailible(NEUTRAL_PLAYER, tloc, VESPENE_GEYSIR);
			}

			if(ok)
			{
					if(getpGoal()->getRace()==ZERG)
					{
						if((*pStats)[build_unit].facility[0]==LARVA)
						{
							if(
									(((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE))*3 >
										  (larvaInProduction[tloc]+getLocationTotal(tloc, LARVA)))&& // zuviel Larven da?
									 ((getLocationTotal(tloc, HATCHERY)+getLocationTotal(tloc, LAIR)+getLocationTotal(tloc, HIVE) > 
									 	   larvaInProduction[tloc])))) // zuwenig Larven da?
							{
								if(buildGene(LARVA))
									addLarvaToQueue(tloc);
							}
						}
					}
					
				Building build;
				build.setOnTheRun(false);
				build.setFacility(stat->facility[fac]);
				build.setLocation(tloc);
				build.setUnitCount(1);
				build.setBuildFinishedTime(getTimer()-stat->BT/*+3200*(stat->facility2==build_unit)*/); //~~ hack :/ TODO SINN???????
				build.setTotalBuildTime(stat->BT);
				build.setType(build_unit);
				build.setIP(getIP());
					
				setMinerals(getMinerals()-(stat->minerals+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)));
				setGas(getGas()-(stat->gas+stat->upgrade_cost*getLocationTotal(GLOBAL, build_unit)));
				setNeedSupply(getNeedSupply()+stat->needSupply);					 
//				setHaveSupply(getHaveSupply()-stat->needSupply);
//				if((stat->needSupply>0)||(((*pStats)[stat->facility[0]].needSupply<0)&&(stat->facilityType==IS_LOST))) 
//					setNeedSupply(getNeedSupply()-stat->needSupply);TODO
				adjustAvailibility(tloc,fac,stat);

				buildingQueue.push(build);
			} // end if (ok)
		} // if resources prere etc ok
	} // end build_unit <= REFINERY+1
//TODO: error wenn facilities net gefunden...
	else // build_unit > REFINERY+1
	{
	/*	int count=0;
		switch(build_unit)
		{
			   case MOVE_ONE_1_FORWARD:count=1;break;
			   case MOVE_ONE_3_FORWARD:count=3;break;
			   case MOVE_ONE_1_BACKWARD:count=-1;break;
			   default:count=0;break;
		}
		   if(count>0)
		{
			if((lastcounter>0)&&(getLocationAvailible(last[lastcounter-1].location,last[lastcounter-1].unit)>0)&&((*pStats)[last[lastcounter-1].unit].speed>0))
				{
					lastcounter--;
					int nr=0;
					while((nr<MAX_BUILDINGS-1)&&(getRemainingBuildTime(nr)))
							nr++;
					//TODO: Fehler wenn nicht genug buildings
					if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>last[lastcounter].count)
						setUnitCount(nr,last[lastcounter].count);
					else setUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
					setFacility(nr,0);
					if(last[lastcounter].location+count<1)
						count=MAX_LOCATIONS+count;
					else if(last[lastcounter].location+count>=MAX_LOCATIONS)
						count=1+last[lastcounter].location+count-MAX_LOCATIONS;
					else count=last[lastcounter].location+count;

					setLocation(nr,count);
					setType(nr,last[lastcounter].unit);
					setRemainingBuildTime(nr,pMap->getDistance(last[lastcounter].location,count)*100/(*pStats)[last[lastcounter].unit].speed);
					setOnTheRun(nr,1);
					setIP(nr,getIP()); // ~ANA
						// 2x Unit => send 8/All instead of just one unit there
			
					if((getIP()>1)&&((Code[0][getIP()-1]==build_unit)||(Code[1][getIP()-1]==build_unit)))
					{
										if(getLocationAvailible(last[lastcounter].location,last[lastcounter].unit)>=6)
									setUnitCount(nr,6);
								else setUnitCount(nr,getLocationAvailible(last[lastcounter].location,last[lastcounter].unit));
								setProgramIsBuilt(getIP(),1);
	
								setIP(getIP()-1);
								//TODO dominance... und 8 checken... evtl weniger
					   }*/
/*					setProgramIsBuilt(getIP(),1);

					addLocationAvailible(last[lastcounter].location,getType(nr),-getUnitCount(nr));
					addLocationTotal(last[lastcounter].location,getType(nr),-getUnitCount(nr));
						ok=1;
				}
		}*/
	}
					
/*	  else
	  if((gRace==ZERG)&&(build_unit==BREAK_UP_BUILDING)&&(BuildingRunning>0)) // lieber eine variable mit last_gebaeude oder so machen und da die Daten speichern, anstatt Programm oder buildings durchzulaufen...
			{
				int min=5000;
				int n=0;
				for(i=0;i<MAX_BUILDINGS;i++)
					if(building[i].RB>0)
					{
						if((stats[2][building[i].type].type==2) && (stats[2][building[i].type].BT-building[i].RB<min))
		// type == 2 because this makes only sense with buildings
		// Sure... but unit about Lurker, Guardien, Sunken, ...  ?
		// mmmh... on the other hand this really makes no sense :}
						{
							min=stats[2][building[i].type].BT-building[i].RB;
							n=i;
						}
					}

					if(min<5000)
					{
						peonminerals++;
						minerals+=stats[2][building[n].type].minerals*0.75;
						gas+=stats[2][building[n].type].gas*0.75;
						Supply--;
						force[DRONE]++;
						if(building[n].type==REFINERY)
						{
							Vespene_Extractors--;
							Vespene_Av++;
						}
						building[n].type=255;
						building[n].RB=0;
//						program[IP].built=1;
//						program[building[n].IP].built=1;
					}*/
//TODO: Verwandtschaftsgrad fuer crossing over feststellen!	
//	if(ok) setProgramIsBuilt(getIP(),1); //??? TODO ???
//	else setProgramIsBuilt(getIP(),0);
	return(ok);
}



// -------------------------------------------
// ------ END CORE OF THE CORE FUNCTIONS -----
// -------------------------------------------


// -------------------------------
// ------ CONTROL FUNCTIONS ------
// -------------------------------

/*
void ANARACE::removeOrder(const unsigned int IP)
{
		for(int j=IP;j--;)
		{
				Code[j+1]=Code[j];
				Marker[j+1]=Marker[j];
				program[j+1].built=program[j].built;
		}
	for(int j=0;j<MAX_LENGTH;j++)
		phaenoCode[j]=getpGoal()->toPhaeno(Code[j]);
	(*pStartCondition)->wasChanged(); // to allow update of time etc. of anarace
};


void ANARACE::insertOrder(int unit, int position)
{
	int l=0;
	int i;

	for(i=MAX_LENGTH;(l<=position)&&(i--);)
		if(getProgramIsBuilt(i))
			l++;
	i++;
	if(i==0)
		i=MAX_LENGTH-1;
	for(int j=0;j<i;j++)
	{
		Code[j]=Code[j+1];
		Marker[j]=Marker[j+1];
		program[j].built=program[j+1].built;
	}
	
	if(getpGoal()->allGoal[unit]==0)
	{
		getpGoal()->addGoal(unit,1,0,0);
		(*pStartCondition)->changeAccepted();
	}
	replaceCode(i,getpGoal()->toGeno(unit));
	
	program[i].built=1;

	for(int j=0;j<MAX_LENGTH;j++)
		phaenoCode[j]=getpGoal()->toPhaeno(Code[j]); 
	(*pStartCondition)->wasChanged(); // to allow update of time etc. of anarace
}
*/
//void ANARACE::backupMap()
//{
/*	for(int i=0;i<pMap->getMaxPlayer()-1;i++)
		for(int j=0;j<MAX_LOCATIONS;j++)
			for(int k=0;k<UNIT_TYPE_COUNT;k++)
			{
				backupLoc[i][j].availible[k]=getMapLocationAvailible(i,j,k);
				backupLoc[i][j].force[k]=getMapLocationTotal(i,j,k);
			}*/
//	return(1);
//}

//void ANARACE::restoreMap()
//{
/*	for(int i=0;i<pMap->getMaxPlayer()-1;i++)
		for(int j=0;j<MAX_LOCATIONS;j++)
			for(int k=0;k<UNIT_TYPE_COUNT;k++)
			{
				setMapLocationAvailible(i,j,k,backupLoc[i][j].availible[k]);
				setMapLocationTotal(i,j,k,backupLoc[i][j].force[k]);
			}*/
//};

// -----------------------------------
// ------ END CONTROL FUNCTIONS ------
// -----------------------------------

// ----------------------------
// ------ MISC FUNCTIONS ------
// ----------------------------

void ANARACE::countUnitsTotal()
{
	unitsTotal = 0;
	unitsTotalMax = 4;
	for (int i = GAS_SCV; i--;)
	{
		if (getLocationTotal(GLOBAL, i) > unitsTotalMax)
			unitsTotalMax = getLocationTotal(GLOBAL, i);
		if (getpGoal()->getAllGoal(i) > unitsTotalMax)
			unitsTotalMax = getpGoal()->getAllGoal(i);
		unitsTotal += getLocationTotal(GLOBAL, i);;
	}
}

// -----------------------------------
// ------ END OF MISC FUNCTIONS ------
// -----------------------------------

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const unsigned int ANARACE::getAverageLength() const
{
	return(averageLength);
}


const unsigned int ANARACE::getUnitsTotalMax() const
{
#ifdef _SCC_DEBUG
	if(unitsTotalMax>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotalMax): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotalMax);
}

const unsigned int ANARACE::getUnitsTotal() const
{
#ifdef _SCC_DEBUG
	if(unitsTotal>MAX_TOTAL_UNITS*UNIT_TYPE_COUNT*MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getUnitsTotal): Variable not initialized.");return(0);
	}
#endif
	return(unitsTotal);
}

const bool ANARACE::isActive() const
{
	return(active);
}

void ANARACE::setActive(const bool set_active)
{
	active=set_active;
}

const bool ANARACE::isOptimizing() const
{
	return(optimizing);
}

void ANARACE::setOptimizing(const bool set_optimizing)
{
	optimizing=set_optimizing;
}

const unsigned int ANARACE::getProgramFacility(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if(IP>=MAX_LENGTH) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Value IP out of range.");return(0);
	}
	if(program[IP].facility>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramFacility): Variable not initialized.");return(0);
	}
#endif
//	if((getRace()==PROTOSS)&&(program[IP].facility==PROBE))
//		while(true);

	return(program[IP].facility);
}

const unsigned int ANARACE::getProgramBT(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramBT): Value IP out of range.");return(0);
	}
	if(program[IP].BT>=MAX_TIME) {
		toLog("DEBUG: (ANARACE::getProgramBT): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].BT);
}																	  

const unsigned int ANARACE::getProgramSuccessType(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Value IP out of range.");return(0);
	}
	if(program[IP].successType>=ERROR_MESSAGES) {
		toLog("DEBUG: (ANARACE::getProgramSuccessType): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].successType);
}

const unsigned int ANARACE::getProgramSuccessUnit(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Value IP out of range.");return(0);
	}
	if(program[IP].successUnit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramSuccessUnit): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].successUnit);
}

/*void ANARACE::setProgramIsConstant(const unsigned int IP, const bool isConstant)
{
		if((IP>=MAX_LENGTH)) {
				toLog("DEBUG: (ANARACE::setProgramIsConstant): Value IP [%i] out of range.",IP);return(0);
		}
		program[IP].isConstant=isConstant;
};*/

void ANARACE::setProgramAvailibleCount(const unsigned int IP, const unsigned int unit, const unsigned int count)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value IP out of range.");return;
	}
	if(unit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value unit out of range.");return;
	}
	if(count>=MAX_TOTAL_UNITS) {
		toLog("DEBUG: (ANARACE::setProgramAvailibleCount): Value count out of range.");return;
	}
#endif
	program[IP].availibleCount[unit]=count;
}

void ANARACE::setProgramTotalCount(const unsigned int IP, const unsigned int unit, const unsigned int count)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value IP out of range.");return;
	}
	if(unit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value unit out of range.");return;
	}
	if(count>=MAX_TOTAL_UNITS) {
		toLog("DEBUG: (ANARACE::setProgramTotalCount): Value count out of range.");return;
	}
#endif
	program[IP].forceCount[unit]=count;
}

void ANARACE::setStatisticsNeedSupply(const unsigned int IP, const unsigned int needSupply)
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setStatisticsNeedSupply): Value IP out of range.");return;
    }
	if(needSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setStatisticsNeedSupply): Value needSupply out of range.");return;
	}
#endif
	statistics[IP].needSupply=needSupply;
}
void ANARACE::setStatisticsHaveSupply(const unsigned int IP, const unsigned int haveSupply)
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setStatisticsHaveSupply): Value IP out of range.");return;
    }
	if(haveSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveSupply): Value haveSupply out of range.");return;
	}
#endif
	statistics[IP].haveSupply=haveSupply;
}

// first time is 0!
void ANARACE::setProgramTime(const unsigned int IP, const unsigned int time)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value IP out of range.");return;
	}
	if(time>configuration.getMaxTime()) {
		toLog("DEBUG: (ANARACE::setProgramTime): Value time out of range.");return;
	}
#endif
	program[IP].time=time;
}

void ANARACE::setStatisticsHaveMinerals(const unsigned int IP, const unsigned int minerals)
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setStatisticsHaveMinerals): Value IP out of range.");return;
    }
	if(minerals>=MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveMinerals): Value minerals out of range.");return;
	}
#endif
	statistics[IP].minerals=minerals;
}
																				
void ANARACE::setStatisticsHaveGas(const unsigned int IP, const unsigned int gas)
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setStatisticsHaveGas): Value IP out of range.");return;
    }
	if(gas>=MAX_GAS) {
		toLog("DEBUG: (ANARACE::setStatisticsHaveGas): Value minerals out of range.");return;
	}
#endif
	statistics[IP].gas=gas;
}

void ANARACE::setProgramBT(const unsigned int IP, const unsigned int time)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value IP out of range.");return;
	}
	if(time>=MAX_TIME) {
		toLog("DEBUG: (ANARACE::setProgramBT): Value time out of range.");return;
	}
#endif
	program[IP].BT=time;
}

void ANARACE::setProgramIsBuilt(const unsigned int IP, const bool isBuilt)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramIsBuilt): Value IP out of range.");return;
	}
#endif
	program[IP].built=isBuilt;
}

void ANARACE::setProgramLocation(const unsigned int IP, const unsigned int location)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value IP out of range.");return;
	}
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::setProgramLocation): Value location out of range.");return;
	}
#endif
	program[IP].location=location;
}

void ANARACE::setStatisticsFitness(const unsigned int IP, const unsigned int fitness)
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::setProgramLocation): Value IP out of range.");return;
    }
	if(fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setStatisticsFitness): Value fitness out of range.");return;
	}
#endif
	statistics[IP].fitness=fitness;
}

const unsigned int ANARACE::getProgramTotalCount(const unsigned int IP, const unsigned int unit) const	   // how many units of the type at phaenoCode[s] do exist at that time?
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value IP out of range.");return(0);
	}
	if(unit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramTotalCount): Value unit out of range.");return(0);
	}
#endif
	return(program[IP].forceCount[unit]);
}

// how many units of the type at phaenoCode[s] do exist at that time?
const unsigned int ANARACE::getProgramAvailibleCount(const unsigned int IP, const unsigned int unit) const	   
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value IP out of range.");return(0);
	}
	if(unit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::getProgramAvailibleCount): Value unit out of range.");return(0);
	}
#endif
	return(program[IP].availibleCount[unit]);
}

void ANARACE::setProgramFacility(const unsigned int IP, const unsigned int facility)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value IP out of range.");return;
	}
	if(facility>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramFacility): Value facility out of range.");return;
	}
#endif
	program[IP].facility=facility;
}

void ANARACE::setProgramSuccessType(const unsigned int IP, const unsigned int type)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value IP out of range.");return;
	}
	if(type>=ERROR_MESSAGES) {
		toLog("DEBUG: (ANARACE::setProgramSuccessType): Value type out of range.");return;
	}
#endif
	program[IP].successType=type;
}

void ANARACE::setProgramSuccessUnit(const unsigned int IP, const unsigned int unit)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value IP out of range.");return;
	}
	if(unit>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setProgramSuccessUnit): Value unit out of range.");return;
	}
#endif
	program[IP].successUnit=unit;
}

const unsigned int ANARACE::getStatisticsNeedSupply(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getStatisticsNeedSupply): Value IP out of range.");return(0);
    }
	if(statistics[IP].needSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getStatisticsNeedSupply): Variable not initialized [%i].");	return(0);
	}
#endif
	return(statistics[IP].needSupply);
}

const unsigned int ANARACE::getStatisticsHaveSupply(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getStatisticsHaveSupply): Value IP out of range.");return(0);
    }
	if(statistics[IP].haveSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveSupply): Variable not initialized.");return(0);
	}
#endif
	return(statistics[IP].haveSupply);
}
const bool ANARACE::getProgramIsBuilt(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsBuilt): Value IP out of range.");return(0);
	}
#endif
	return(program[IP].built);
}

/*const bool ANARACE::getProgramIsConstant(const unsigned int IP)
{
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramIsConstant): Value IP [%i] out of range.",IP);return(false);
	}
	return(program[IP].isConstant);
};*/

// first program time is
const unsigned int ANARACE::getProgramTime(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramTime): Value IP out of range.");return(0);
	}
	if(program[IP].time>configuration.getMaxTime()) {
		toLog("DEBUG: (ANARACE::getProgramTime): Variable not initialized.");return(0);
	}
#endif
	return(program[IP].time);
}

const unsigned int ANARACE::getRealProgramTime(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getProgramLocation): Value IP out of range.");return(0);
    }
#endif
	return(configuration.getMaxTime() - getProgramTime(IP));
}

const unsigned int ANARACE::getStatisticsHaveMinerals(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
	        toLog("DEBUG: (ANARACE::getStatisticsHaveMinerals): Value IP out of range.");return(0);
	}
	if(statistics[IP].minerals>MAX_MINERALS) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveMinerals): Variable not initialized.");return(0);
	}
#endif
	return(statistics[IP].minerals);
}

const unsigned int ANARACE::getStatisticsHaveGas(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getStatisticsHaveGas): Value IP out of range.");return(0);
    }
	if(statistics[IP].gas>MAX_GAS) {
		toLog("DEBUG: (ANARACE::getStatisticsHaveGas): Variable statistics.gas not initialized.");return(0);
	}
#endif
	return(statistics[IP].gas);
}

const unsigned int ANARACE::getProgramLocation(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Value IP out of range.");return(0);
	}
	if(program[IP].location>MAX_LOCATIONS) {
		toLog("DEBUG: (ANARACE::getProgramLocation): Variable program.location not initialized.");return(0);
	}
#endif
	return(program[IP].location);
}

const unsigned int ANARACE::getStatisticsFitness(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
    if((IP>=MAX_LENGTH)) {
        toLog("DEBUG: (ANARACE::getStatisticsFitness): Value IP out of range.");return(0);
    }
	// TODO max pfitness 
	if(statistics[IP].fitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getStatisticsFitness): Variable statistics.fitness not initialized.");return(0);
	}
#endif
	return(statistics[IP].fitness);
}
const unsigned int ANARACE::getTimePercentage() const
{
	return(timePercentage);
}

const unsigned int ANARACE::getGoalPercentage() const
{
	if(getTimer()==0)
		return(goalPercentage);
	else 
	{
		unsigned int optimalTime = getpGoal()->calculateFastestBO((*pStartCondition)->getUnit(GLOBAL));
		if(optimalTime==0)
			return(100);
		else
			return(100*optimalTime/getRealTimer());
	}
}

const GOAL_TREE ANARACE::getGoalTree(const unsigned int currentGoalUnit) const
{
	return(getpGoal()->getGoalTree((*pStartCondition)->getUnit(GLOBAL), currentGoalUnit));
}

void ANARACE::setPhaenoCode(const unsigned int IP, const unsigned int num)
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value IP out of range.");return;
	}
	if(num>=UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ANARACE::setPhaenoCode): Value num out of range.");return;
	}
#endif
	phaenoCode[IP]=num;
}

const unsigned int ANARACE::getPhaenoCode(const unsigned int IP) const
{
#ifdef _SCC_DEBUG
	if((IP>=MAX_LENGTH)) {
		toLog("DEBUG: (ANARACE::getPhaenoCode): Value IP out of range.");return(0);
	}
#endif
	return(phaenoCode[IP]);
}

const unsigned int ANARACE::getCurrentpFitness() const
{
	return(currentpFitness);
}

void ANARACE::setCurrentpFitness(const unsigned int num)
{
	currentpFitness=num;
}

const unsigned int ANARACE::getUnchangedGenerations() const
{
#ifdef _SCC_DEBUG	
	if(unchangedGenerations>configuration.getMaxGenerations()) {
		toLog("DEBUG: (ANARACE::getUnchangedGenerations): Variable unchangedGenerations not initialized.");return(0);
	}
#endif
	return(unchangedGenerations);
}

const unsigned int ANARACE::getRun() const
{
#ifdef _SCC_DEBUG	
	if(currentRun>configuration.getMaxRuns()) {
		toLog("DEBUG: (ANARACE::getRun): Variable currentRun not initialized.");return(0);
	}
#endif
	return(currentRun);
}

const unsigned int ANARACE::getTotalGeneration() const
{
	return(totalGeneration);
}

const unsigned int ANARACE::getMaxpFitness() const
{
#ifdef _SCC_DEBUG	
// TODO max pfitness?
	if(maxpFitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::getMaxpFitness): Variable maxpFitness not initialized.");return(0);
	}
#endif
	return(maxpFitness);
}

const unsigned int ANARACE::getMaxsFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxsFitness>MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANARACE::getMaxsFitness): Variable maxsFitness not initialize.");return(0);
	}
#endif
	return(maxsFitness);
}

const unsigned int ANARACE::getMaxtFitness() const
{
#ifdef _SCC_DEBUG	
	if(maxtFitness>MAX_TFITNESS) {
		toLog("DEBUG: (ANARACE::getMaxtFitness): Variable maxtFitness not initialized.");return(0);
	}
#endif
	return(maxtFitness);
}

void ANARACE::setUnchangedGenerations(const unsigned int unchangedGenerations)
{
#ifdef _SCC_DEBUG	
	if(unchangedGenerations>configuration.getMaxGenerations()) {
		toLog("DEBUG: (ANARACE::setUnchangedGenerations): Value num out of range.");return;
	}
#endif
	this->unchangedGenerations=unchangedGenerations;
}

void ANARACE::setRun(const unsigned int run)
{
#ifdef _SCC_DEBUG
	if(run>configuration.getMaxRuns()) {
		toLog("DEBUG: (ANARACE::setRun): Value run out of range.");return;
	}
#endif
	currentRun=run;
}

void ANARACE::setTotalGeneration(const unsigned int total_generation)
{
	totalGeneration=total_generation;
}

void ANARACE::setMaxpFitness(const unsigned int maxpFitness) 
{
#ifdef _SCC_DEBUG
	// TODO maxpitness
	if(maxpFitness>MAX_PFITNESS) {
		toLog("DEBUG: (ANARACE::setMaxpFitness): Value out of range.");return;
	}
#endif
	this->maxpFitness=maxpFitness;
}

void ANARACE::setMaxsFitness(const unsigned int maxsFitness)
{
#ifdef _SCC_DEBUG
	if(maxsFitness>MAX_MINERALS+MAX_GAS) {
		toLog("DEBUG: (ANARACE::setMaxsFitness): Value out of range.");return;
	}
#endif
	this->maxsFitness=maxsFitness;
}

void ANARACE::setMaxtFitness(const unsigned int maxtFitness)
{
#ifdef _SCC_DEBUG
	if(maxtFitness>MAX_TFITNESS) {
		toLog("DEBUG: (ANARACE::setMaxtFitness): Value out of range.");return;
	}
#endif
	this->maxtFitness=maxtFitness;
}

/*void ANARACE::analyzeBuildOrder()
{
//keeps track of the '@' symbol on the left of each build order entry
//if the goal is set to 10 marines, the eleventh won't be marked as a 'fulfilled goal' with a '@'
	int tGoal[UNIT_TYPE_COUNT];
// reset the tgGoals (to sign with '@' the units which are part of the goal list)
// and subtract the units that are already on the map
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
	{
		tGoal[i]=0;
		for(int j=1;j<MAX_LOCATIONS;j++)
			tGoal[i]+=getpGoal()->globalGoal[j][i]-pStartcondition->getLocationTotal(j,i);
	}

	for(int i=0;i<MAX_LENGTH;i++)
		if(tGoal[phaenoCode[i]]>0) //~~~~~~ location=0?
		{
			tGoal[phaenoCode[i]]--;
			setProgramIsGoal(i,true);
		}
	else setProgramIsGoal(i,false);
};*/


unsigned int ANARACE::successType;
unsigned int ANARACE::successUnit;

