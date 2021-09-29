#ifndef QUADTREE_TREENODE_H
#define QUADTREE_TREENODE_H

#include "MyVector.h"
#include "Query.h"
#include "GloVars.h"
#include "Buckets.h"


class TreeNode {
public:
    TreeNode();
    TreeNode(int id,int* coord,int lev, int start);
    ~TreeNode();
    int structureIndex;
    int secStructureIndex;
	int* childIndexList; //size: CHD_N
    /* For convenience, we take the following form in range
     * [a,b]*[c,d]
     * range: a,(a+b)/2,b,c,(c+d)/2,d
     * */
    int* range; //size of 3*dim
    int ptCnt;
	long long total_ptCnt;
	int dim_lev; //set to 1 for segment tree. 
	int dim_start; //from 0 to DIM_K-1-dim_lev	

    static BucketElement emptyBucketElement;
    static Buckets emptyBuckets;

    void ReleaseSpace();
    void init(int id, int* coord,int lev,int start);
    void InsertQueryList(MyVector<int>& queryIndexList, void * quadTree_Log);
    void InsertQuery(int queryIndex, void* quadTree_Log);
    int InsertPoint( int weight, MyVector<int>& alertIDList,MyVector<int>&newEpochList, void * quadTree_Log);
    
	double FindCanonicalNode_Epoch(Query& query,void* quadTree_log,MyVector<int>& nodeIndexList,double err_epo,double err_max);

	double CanonicalNodeCounter(Query& query,void* quadTree_log, double err_max);

	void FindCanonicalNode_Space(Query& query,void* quadTree_log,MyVector<int>& nodeIndexList,double err_max);

    int stabChildIndex(const int* coords,int d) const;

    int* ChildRange(int code,int* thisRange,int dim_l) {
        int* coord=new int[3*dim_l];
        for(int i=dim_l-1;i>=0;--i){
            if(code%2==0){
                coord[i*3]=thisRange[i*3];
                coord[i*3+2]=thisRange[i*3+1];
                coord[i*3+1]=(coord[i*3]+coord[i*3+2])/2;
            }else{
                coord[i*3]=thisRange[i*3+1];
                coord[i*3+2]=thisRange[i*3+2];
                coord[i*3+1]=(coord[i*3]+coord[i*3+2])/2;
            }
            code=code/2;
        }
        return coord;
    }

};


#endif //QUADTREE_TREENODE_H
