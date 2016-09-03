#ifndef __LIST_H
#define __LIST_H

#include "defs.h"

struct Order
{
	int blend;
	int blendTarget;
	int blendStart;
//build order list
	Rect rect;
	Rect brect;
	Rect target;
	Rect btarget;
	Rect start;
	Rect bstart;
//build order graph
	int unit;
	int checked;
//	int mins,gas,time,location,needSupply,haveSupply,forceFacilityCount,availibleFacilityCount,successType,successUnit,facility,code,forceCount;
	int bonew;
	int IP;
	int row;
	long marker;
//	int mins, color  etc.
};

//evtl nochmal ne ebene 'player' in mydcwindow und da dann das ganze zeugs unterbringen, v.a. orderlist etc.!

#endif

