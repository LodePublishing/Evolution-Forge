#ifndef _CORE_BLIST_HPP
#define _CORE_BLIST_HPP

#include "building.hpp"

class BNODE
{
	public:
		BNODE(BNODE* prev);
		~BNODE();
		Building* GetData() const;
		BNODE* GetNext() const;
		BNODE* GetPrev() const;
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
		const bool isEmpty() const;
		const int GetCount() const;
		BNODE* GetHead() const;
		BNODE* GetTail() const;
	private:
		int count;
		BNODE* head;
		BNODE* tail;
};

#endif // _CORE_BLIST_HPP

