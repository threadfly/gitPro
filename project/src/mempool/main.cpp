#include "common.h"
#include "a.h"
#include "MemoryPool.h"
#include <sys/time.h>
#include "SkipList.h"
#include "slnode.h"


//---------------------------- test skip list ---------------------------------
const USHORT TIMES = 1000;
const USHORT MAX_NUM = 1000;
int main()
{
	SkipList<Node> * pl = SkipList<Node>::GetInstance();
	if ( NULL == pl)
	{
		cout << "new pl error!!" << endl;
		return 1;
	}
	Node node;
	node.Set(2,3);
	pl->Insert(node);
	node.Set(2,4);
	pl->Insert(node);
	node.Set(1,3);
	pl->Insert(node);
	node.Set(1,3);
	pl->Insert(node);
	node.Set(3,0);
	pl->Insert(node);
	node.Set(3,3);
	pl->Insert(node);
	pl->Print();
	cout << endl;
	pl->PrintStruct();
	cout << "-------------" << endl;
	pl->Delete(node);
	pl->PrintStruct();
	/*
	SkipList<USHORT> * p_skip_list_1 = SkipList<USHORT>::GetInstance();
	if ( NULL == p_skip_list_1 )
	{
		cout << "new SkipList Error!! p_skip_list_1:" << p_skip_list_1 << endl;
		return 1;
	}
	USHORT i = 0;
	while ( i < TIMES )
	{
		p_skip_list_1->Insert((USHORT)rand()%MAX_NUM);
		++i;
	}
	p_skip_list_1->Insert(23);
	p_skip_list_1->Insert(14);
	p_skip_list_1->Insert(89);
	p_skip_list_1->Insert(100);
	p_skip_list_1->Insert(23);
	p_skip_list_1->Insert(21);
	p_skip_list_1->Print();
	//SkipList<USHORT> * p_skip_list_2 = new SkipList<USHORT>;
	//
	if (p_skip_list_1->Find(23))
	{
		cout << "find value!" << endl;
	}
	//cout <<"-----------------------"<< endl;
	//p_skip_list_1->PrintStruct();
	cout << " --- size:" << p_skip_list_1->GetSize() << " --- "<< endl;
	if (p_skip_list_1->Delete(14))
	{
		cout << " delete 23 success!!" << endl;
		p_skip_list_1->Print();
	}
	else{
		cout << " delete 23 error!! " << endl;
	}
	//cout <<"-----------------------"<< endl;
	//p_skip_list_1->PrintStruct();
	cout << " --- size:" << p_skip_list_1->GetSize() << " --- "<< endl;
	*/
	return 0;
}

//---------------------------- test memory pool ---------------------------------
/*
#define TEST_TIMES	8000
A* A_array[TEST_TIMES];
int main()
{
	timeval start,end;
    cout<<"sizeof(USHORT):"<<sizeof(USHORT)<<endl;

	cout<<"-------- Use Memory Pool --------"<<endl;
	//MemoryPool pool(sizeof(A));
	//MemoryPool pool(sizeof(A), 800000, 5000);
	gettimeofday(&start, NULL);
	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		//MemoryPool pool(2);
		//A * pA = new (pool.Alloc()) A;
		A * pA = new A;
		A_array[i] = pA;
		//pA->~A();
	//	pool.Free(pA);
	}
	gettimeofday(&end, NULL);

	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		//MemoryPool pool(2);
		//A_array[i]->~A();
		//pool.Free(A_array[i]);
		delete A_array[i];
	//	pool.Free(pA);
	}

	USHORT timeuser = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	cout<<"-- Use Memory Pool -- cost:"<< timeuser/1000 <<" microsecond"<<endl;

	cout<<"------- No Use Memory Pool -------"<<endl;
	gettimeofday(&start, NULL);
	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		A * pA = new A;
		A_array[i] = pA;
	}
	gettimeofday(&end, NULL);
	timeuser = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
	for (USHORT i = 0; i < TEST_TIMES; i++)
	{
		delete A_array[i];
	}
	cout<<"-- No Use Memory Pool -- cost:"<< timeuser/1000 <<" microsecond"<<endl;
	return 0;
}
*/
