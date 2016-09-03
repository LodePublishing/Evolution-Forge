#ifndef __LIST_H
#define __LIST_H

#include "wx/wx.h"

struct ORDER
{
        int blend;
        int blendTarget;
	int blendStart;
                                                                               
//build order list
	wxRect rect;
	wxRect target;
	wxRect start;
                                                                               
//build order graph
            
	int unit;
        int checked;
//      int mins,gas,time,location,needSupply,haveSupply,forceFacilityCount,availibleFacilityCount,successType,successUnit,facility,code,forceCount;
        int /*marker,*/bonew;
                                                                               
        int IP;
        int row;
//      int mins, color  etc.
};

//evtl nochmal ne ebene 'player' in mydcwindow und da dann das ganze zeugs unterbringen, v.a. orderlist etc.!


class NODE
{
	public:
		NODE(NODE* prev, int key);
		~NODE();
		ORDER* GetData();
		NODE* GetNext();
		NODE* GetPrev();
		int GetKey();
		void SetData(ORDER* data);
		void SetNext(NODE* next);
		void SetPrev(NODE* prev);
		void SetKey(int key);
	private:
		ORDER* data;
		NODE* next;
		NODE* prev;
		int key;
};


class OrderList
{
	public:
		OrderList();
		~OrderList();
		void Append(int key, ORDER* object);
		void Clear();
		NODE* DeleteNode(NODE* node);
		NODE* Find(int key);
		int GetCount();
		NODE* GetFirst();
		NODE* GetTail();
		void Sort();
	private:
		NODE* head;
		NODE* tail;
};

#endif

