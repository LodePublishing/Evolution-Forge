#include "blist.hpp"

BNODE::BNODE(BNODE* prev)
{
	SetData(0);
	SetPrev(prev);
	if(GetPrev())
	{
		if(prev->GetNext())
		{
			SetNext(prev->GetNext());
			prev->GetNext()->SetPrev(this);
		}
		else
			SetNext(0);
		prev->SetNext(this);
	} else SetNext(0);
};

BNODE::~BNODE()
{
	if(GetData())
	{
		delete data;
		data=0;
	}
	if(GetPrev())
	{
		if(GetNext())
			GetPrev()->SetNext(GetNext());
		else
			GetPrev()->SetNext(0);
	}
	if(GetNext())
	{
		if(GetPrev())
			GetNext()->SetPrev(GetPrev());
		else
			GetNext()->SetPrev(0);
	}
};

Building* BNODE::GetData() const
{
	return data;
};

BNODE* BNODE::GetNext() const
{
	return next;
};

BNODE* BNODE::GetPrev() const
{
	return prev;
};

void BNODE::SetData(Building* data)
{
	if(((int)data>999)||((int)data==0))
		this->data=data;
// else?
};

void BNODE::SetNext(BNODE* next)
{
	this->next=next;
};

void BNODE::SetPrev(BNODE* prev)
{
	this->prev=prev;
};

BuildingList::BuildingList()
{
	count=0;
	head=0;
	tail=0;
};

BuildingList::~BuildingList()
{
	Clear();
	head=0;
	tail=0;
	count=0;
};

void BuildingList::Append(Building* object)
{
 	BNODE* node=new BNODE(tail);
	node->SetData(object);
	tail=node;
	if(head==0)
		head=node;
	count++;
};

void BuildingList::Insert(BNODE* next, Building* object)
{
	if((!tail)||(!next)) // kein Tail? => Liste ist leer! => hinten anfuegen!
		Append(object);
	else if(next==head) // aha, am Anfang einfuegen
	{
		BNODE* node=new BNODE(0);
		head=node;
		next->SetPrev(node);
		node->SetNext(next);
		node->SetData(object);
		count++;
	} else //=> irgendwo dazwischen
	{
		BNODE* node=new BNODE(next->GetPrev());
		node->SetData(object);
		count++;
	}
};

void BuildingList::Clear()
{
	while(tail)
	{
		BNODE* temp=tail->GetPrev();
		delete tail;
		tail=temp;
	}
	count=0;
	head=0;
	tail=0;
};

void BuildingList::DeleteNode(BNODE* node)
{
	if(node->GetNext()&&node->GetPrev()) //irgendwo dazwischen
	{
		node->GetNext()->SetPrev(node->GetPrev());
		node->GetPrev()->SetNext(node->GetNext());
	} else if(node->GetNext()) //am Anfang
	{
		head=node->GetNext();
		head->SetPrev(0);
	} else if(node->GetPrev()) // Am Schluss
	{
		tail=node->GetPrev();
		tail->SetNext(0);
	} else //letztes Glied
	{
		tail=0;
		head=0;
	}
	delete node;
	node=0;
	count--;
};

const bool BuildingList::isEmpty() const
{
	if(count==0)
		return(true);
	else return(false);
}

const int BuildingList::GetCount() const
{
	return(count);
};

BNODE* BuildingList::GetHead() const
{
	return(head);
};

BNODE* BuildingList::GetTail() const
{
	return(tail);
};


