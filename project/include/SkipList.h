#ifndef _SKIP_LIST_
#define _SKIP_LIST_

#include "common.h"
#include <vector>
#include <time.h>
//#include <stdlib.h>
//#include <boost/shared_ptr.hpp>
#include <iostream>
using namespace std;

#define MAX_LEVEL 16
/*
template<class T> bool NodeComp(T a, T b)
{
	return a < b;
}

//--------------------------- class NodeOp ---------------------------
class NodeOp
{
public:
	virtual bool operator < (NodeOp second);
};
*/

//--------------------------- class SkipListNode ---------------------------
template<typename T>
class SkipListNode
{
public:
	typedef typename std::vector< SkipListNode<T> * > LevelType;
	typedef typename LevelType::iterator LevelType_iter;
public:
	SkipListNode(USHORT levelnum);

	void InitLevelInfo();
	
	bool operator < ( SkipListNode<T> &);

	bool operator == ( SkipListNode<T> &);

	LevelType& GetLevelInfo();

	void SetValue(T value){ m_value = value; }
	T GetValue(){ return m_value;}

	USHORT GetLevel(){ return (USHORT)(m_levels.size());}

	//bool NodeCompSmall(T a, T b){ return a < b; }

	//bool NodeCompEqual(T a, T b){}
private:
	LevelType	m_levels;// level info
	T			m_value;
};

template<typename T>
SkipListNode<T>::SkipListNode(USHORT levelnum):
m_levels(levelnum)
{
	
}

template<typename T>
typename SkipListNode<T>::LevelType& SkipListNode<T>::GetLevelInfo()
{
	return m_levels;
}

template<typename T>
void SkipListNode<T>::InitLevelInfo()
{
	//for (LevelType::iterator it = m_levels.begin(); it != m_levels.end(); ++it)
	//for (std::vector< SkipListNode<T> *>::iterator it = m_levels.begin(); it != m_levels.end(); ++it)
	#ifdef DEBUG
	USHORT i = 0;
	#endif 
	for (LevelType_iter it = m_levels.begin(); it != m_levels.end(); ++it)
	{
		#ifdef DEBUG
		cout << "----" << i << "----" << endl;
		i++;
		#endif
		*it = NULL;
	}
}

template<typename T>
bool SkipListNode<T>::operator < ( SkipListNode<T> & node)
{
	//return NodeComp(m_value, node.m_value);
	#ifdef DEBUG
		cout << "m_value:"<< m_value << " node.m_value:"<< node.m_value<<endl;
	#endif 
	return m_value < node.m_value;
}

template<typename T>
bool SkipListNode<T>::operator == ( SkipListNode<T> & node)
{
	#ifdef DEBUG
		cout << "m_value:"<< m_value << " node.m_value:"<< node.m_value<<endl;
	#endif 
	return m_value == node.m_value;
}

//--------------------------- class SkipList ---------------------------
template<typename T>
class SkipList
{
public:
	void SetLevel(USHORT level);

	USHORT GetSize(){return m_size;}
	
	bool Insert(T value);

	bool Delete(T value);

	bool Find(T value);

	void Print();
	
	void PrintStruct();
public:
	static USHORT RandLevelNum();

	static SkipList *  GetInstance();

private:
	USHORT						m_level;
	SkipListNode<T> *			m_head;
	USHORT						m_size;

private:
	SkipList();
	bool SetHead(SkipListNode<T> * head);
};

template<typename T>
SkipList<T>::SkipList():
m_level(MAX_LEVEL),
m_size(0)
{
}

template<typename T>
SkipList<T> * SkipList<T>::GetInstance()
{
	SkipList<T> * p_sl = new SkipList<T>;
	if ( NULL == p_sl )
		return NULL;
	
	//p_sl->SetLevel(0);

	SkipListNode<T> * p_node = new SkipListNode<T>(MAX_LEVEL);
	p_node->InitLevelInfo();

	if ( !p_sl->SetHead(p_node))
		return false;

	return p_sl;
}

template<typename T>
void SkipList<T>::SetLevel(USHORT level)
{
	this->m_level = level;
}

template<typename T>
bool SkipList<T>::SetHead(SkipListNode<T> * head)
{
	if ( NULL == head )
		return false;
	this->m_head = head;
	return true;
}

template<typename T>
bool SkipList<T>::Delete(T value)
{
	SkipListNode<T> * update[MAX_LEVEL] = {NULL};
	SkipListNode<T> * p = m_head;
	SkipListNode<T> * q = NULL;
	USHORT level = m_level;

	SkipListNode<T> tmp_node(0);
	tmp_node.SetValue(value);

	bool first = true;
	SkipListNode<T> * find = NULL;

	do{
		for(q = p->GetLevelInfo()[level-1]; q != NULL; )
		{
			if ( *q == tmp_node)
			{
				update[level-1] = p;
				--level;
				if (level == 0)
				{
					goto  SUCCESS;
				}
				if ( first )
				{
					first = false;
					find = q;
				}

				q = p->GetLevelInfo()[level-1];
			}
			else if ( *q < tmp_node)
			{
				p = q;
				q = p->GetLevelInfo()[level-1];
			}
			else if ( tmp_node < *q)
			{
				--level;
				if (level == 0)
				{
					cout << "--  Delete() hava fatal failed!! --" << endl;
					return false;
				}
				q = p->GetLevelInfo()[level-1];
			}
		}
	}while(--level>=1);
	
SUCCESS:
	if (NULL ==  find )
		return false;

	//cout << "value:" << find->GetValue() << " level:"<< find->GetLevel() <<"  addr:"<< find << endl;
	//cout << "-------------------------------------" << endl;
	for ( USHORT i = 0; i < find->GetLevel(); ++i )
	{
		p = update[i];
		if ( NULL == p)
		{
			cout << " Delete() ERROR!!" << endl;
			continue;
		}
		//cout << "value:" << p->GetLevelInfo()[i]->GetValue() << "  addr:"<< p->GetLevelInfo()[i]<<endl;
		p->GetLevelInfo()[i] = find->GetLevelInfo()[i];
		find->GetLevelInfo()[i] = NULL;
	}

	delete find;
	--m_size;

	return true;
}

template<typename T>
bool SkipList<T>::Insert(T value)
{
	SkipListNode<T> * update[MAX_LEVEL];
	SkipListNode<T> * p = m_head;
	SkipListNode<T> * q = NULL;
	USHORT level = m_level;
	
	SkipListNode<T>* p_insert_node = new SkipListNode<T>(SkipList<T>::RandLevelNum());
	if (NULL == p_insert_node)
		return false;
	p_insert_node->InitLevelInfo();
	p_insert_node->SetValue(value);
	do{
		for(q = p->GetLevelInfo()[level-1];
				(q != NULL) && ((*q) < (*p_insert_node)); )
		{
			p = q;
			q = p->GetLevelInfo()[level-1];
		}
		if ( q !=  NULL && (*q) == (*p_insert_node))
		{
			cout << "value:"<< p_insert_node->GetValue() << " have Exist!!" << endl;
			return false;
		}
		update[level-1] = p;
	}while(--level>=1);

	for ( USHORT i = 0; i < p_insert_node->GetLevel(); ++i)
	{
		p = update[i];
		p_insert_node->GetLevelInfo()[i] = p->GetLevelInfo()[i];
		p->GetLevelInfo()[i] = p_insert_node;
	}
	++m_size;
	return true;
}

template<typename T>
USHORT SkipList<T>::RandLevelNum()
{
	USHORT level = 1;
	//while( rand()%2 == 1 && level < MAX_LEVEL)
	//	++level;
	USHORT tmp = rand();
	level = level + tmp%MAX_LEVEL;
	#ifdef DEBUG
		cout << "tmp:" << tmp << "  level:" << level<< endl;
	#endif

	return level;
}

template<typename T>
void SkipList<T>::PrintStruct()
{
	SkipListNode<T> * p = m_head->GetLevelInfo()[0];
	if ( p == NULL )
	{
		cout << "Print() p is NULL" << endl;
		return ;
	}
	
	while(p)
	{
		for ( USHORT i = 0 ; i < p->GetLevel(); ++i)
		{
			cout<< p->GetValue()<< "  ";
		}
		cout << endl;
		p = p->GetLevelInfo()[0];
	}
}

template<typename T>
void SkipList<T>::Print()
{	
	SkipListNode<T> * p = m_head->GetLevelInfo()[0];
	if ( p == NULL )
	{
		cout << "Print() p is NULL" << endl;
		return ;
	}
	USHORT i = 0;
	while(p)
	{
		cout << "value["<< i++ <<"]:" << p->GetValue() << "   ";
		if ( i%8 == 0)
			cout << endl;
		p = p->GetLevelInfo()[0];
	}
}

template<typename T>
bool SkipList<T>::Find(T value)
{
	SkipListNode<T> * p = m_head;
	SkipListNode<T> * q = NULL;
	USHORT level = m_level;

	SkipListNode<T> tmp_node(0);
	tmp_node.SetValue(value);

	do{
		for (q = p->GetLevelInfo()[level-1]; q != NULL; )
		{
			if ( *q == tmp_node)
			{
				return true;
			}else if ( *q < tmp_node){
				p = q;
				q = p->GetLevelInfo()[level-1];
			}else if ( tmp_node < *q){
				--level;
				if (level == 0)
				{
					cout << "--  Find() hava fatal failed!! --" << endl;
					return false;
				}
				q = p->GetLevelInfo()[level-1];
			}
		}
	}while(--level>=1);

	return false;
}
#endif

