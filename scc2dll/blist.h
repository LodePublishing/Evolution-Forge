#ifndef __BLIST_H
#define __BLIST_H

#include "building.h"

class BNODE
{
	public:
		BNODE(BNODE* prev);
		~BNODE();
		Building* GetData();
		BNODE* GetNext();
		BNODE* GetPrev();
		void SetData(Building* data);
		void SetNext(BNODE* next);
		void SetPrev(BNODE* prev);
	private:
		Building* data;
		BNODE* next;
		BNODE* prev;
};


class BuildingList
{
	public:
		BuildingList();
		~BuildingList();
		void Append(Building* object);
		void Insert(BNODE* next, Building* object);
		void Clear();
		void DeleteNode(BNODE* node);
		int GetCount();
		BNODE* GetFirst();
		BNODE* GetTail();
	private:
		BNODE* head;
		BNODE* tail;
};

#endif

