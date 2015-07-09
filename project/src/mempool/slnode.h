#ifndef _NODE_
#define _NODE_
#include "SkipList.h"

class Node
{
public:
	bool operator < ( Node &);
	bool operator == ( Node &);
	friend ostream & operator << ( ostream & , const Node &);
	USHORT k;
	USHORT v;

	void Set(USHORT k, USHORT v){ this->k = k, this->v = v;}
};

bool Node::operator < ( Node &  node)
{
	if (k == node.k)
		return v < node.v;
	else
		return k < node.k;
}

bool Node::operator == ( Node & node )
{
	if ( k != node.k)
		return false;
	else 
		return v == node.v;
}
ostream & operator << ( ostream & output, const Node & node)
{
	output<< "(K:" << node.k << "  V:"<< node.v<< ") ";
	return output;
}

#endif

