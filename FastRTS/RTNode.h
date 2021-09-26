#ifndef RTNODE_H_
#define RTNODE_H_


#include "IndexablePoint.h"
#include "MyVector.h"
class RTNode{
public:
	void* child[2]; // if node_cnt==1, a leaf node and no child, so pointer to element in child[0]
	int range[2]; //[x,y] range of nodes in dim_lev
	int node_cnt;
	int weight_sum;
	int dim_lev; //from 0 to DIM_K-1
	RTNode* subtree;
	RTNode(); 
	~RTNode();
	RTNode(int d);
	static double duration;
	void insertElement(IndexablePoint* ele);	

	static MyVector<IndexablePoint*> emptyVec;

	bool check_rebuild(int chd);
	void rebuild(IndexablePoint* ele);
	int insert_index(int d_v);
	void batch_insert(MyVector<IndexablePoint*> * eles); //with sorted list for dim_lev to DIM_K-1, first bottom up build tree in this dim level, then top down split dim_lev+1 to DIM-1 level 
	
	void down_collectp(MyVector<IndexablePoint*> *eles,IndexablePoint* ele);// only goes down, gives sorted list for this->dim with ele
	void ReleaseSpace();//delete child node from up to down..
	
	int rangeCntQuery(int* q);
	
	inline bool disjoint(int *q){
		return (this->range[1]<q[dim_lev*2] or this->range[0]>=q[dim_lev*2+1]);
	}
	inline bool inside(int *q){
		return (this->range[0]>=q[dim_lev*2] and this->range[1]<q[dim_lev*2+1]);
	}
};
#endif /* RTNODE_H_ */
