#ifndef __LIST_H
#define __LIST_H

struct ORDER
{
        int blend;
        int blendTarget;
                                                                               
//build order list
        int x,y; //current x,y
        int targetx,targety;
        int targetwidth,width;
        int dx,dy;
                                                                               
//build order graph
        int bx,by;
        int targetbx,targetby;
                                                                               
        int bheight,bwidth;
        int checked;
        int targetbheight,targetbwidth;
            
	int unit;
                                                                   
//      int mins,gas,time,location,needSupply,haveSupply,forceFacilityCount,availibleFacilityCount,successType,successUnit,facility,code,forceCount;
        int /*marker,*/bonew;
                                                                               
        int IP;
        int row;
//      int mins, color  etc.
};


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
		void DeleteNode(NODE* node);
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

