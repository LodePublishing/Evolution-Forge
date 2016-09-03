#include "list.h"

NODE::NODE(NODE* prev, int key)
{
	SetData(0);
	SetPrev(prev);
	SetKey(key);
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

NODE::~NODE()
{
	if(GetData())
	{
		delete data;
		data=0;
	}
/*	if(GetPrev())
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
	}*/
};

ORDER* NODE::GetData()
{
	return data;
};

NODE* NODE::GetNext()
{
	return next;
};

NODE* NODE::GetPrev()
{
	return prev;
};

int NODE::GetKey()
{
	return key;
};

void NODE::SetData(ORDER* data)
{
	this->data=data;
};

void NODE::SetNext(NODE* next)
{
	this->next=next;
};

void NODE::SetPrev(NODE* prev)
{
	this->prev=prev;
};

void NODE::SetKey(int key)
{
	this->key=key;
};


OrderList::OrderList()
{
	head=0;
	tail=0;
};

OrderList::~OrderList()
{
	Clear();
	head=0;
	tail=0;
};

void OrderList::Append(int key, ORDER* object)
{
	NODE* node=new NODE(tail, key);
	node->SetData(object);
	tail=node;
	if(head==0)
		head=node;
};

void OrderList::Clear()
{
	while(tail)
	{
		NODE* temp=tail->GetPrev();
		delete tail;
		tail=temp;
	}
	head=0;
	tail=0;
};

NODE* OrderList::DeleteNode(NODE* node)
{
	NODE* temp=0;	
	if(node->GetNext()&&node->GetPrev())
	{
		node->GetNext()->SetPrev(node->GetPrev());
		node->GetPrev()->SetNext(node->GetNext());
		temp=node->GetNext();
	} else if(node->GetNext())
	{
		head=node->GetNext();
		head->SetPrev(0);
		temp=node->GetNext();
	} else if(node->GetPrev())
	{
		tail=node->GetPrev();
		tail->SetNext(0);
	} else
	{
		tail=0;
		head=0;
	}
	delete node;
	return(temp);
};

NODE* OrderList::Find(int key)
{
	NODE* temp=head;
	while(temp)
	{
		if(temp->GetKey()==key)
			return(temp);
		temp=temp->GetNext();
	}
	return(0);
};

int OrderList::GetCount()
{
	NODE* temp=head;
	int count=0;
	while(temp)
	{
		count++;
		temp=temp->GetNext();
	}
	return count;
};

NODE* OrderList::GetFirst()
{
	return(head);
};

NODE* OrderList::GetTail()
{
	return(tail);
};

void OrderList::Sort()
{
	NODE* temp=head;
	while(temp)
	{
		NODE* temp2=head;
		while(temp2&&temp2!=temp)
		{
			if(temp->GetData()->IP>temp2->GetData()->IP)
			{
				ORDER* temp3=temp->GetData();
				temp->SetData(temp2->GetData());
				temp2->SetData(temp3);
				int temp4=temp->GetKey();
				temp->SetKey(temp2->GetKey());
				temp2->SetKey(temp4);			
			};
			temp2=temp2->GetNext();
		};
		temp=temp->GetNext();
	};
};


