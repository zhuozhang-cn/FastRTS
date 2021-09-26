#ifndef QUADTREE_QUADTREELOG_H
#define QUADTREE_QUADTREELOG_H

#include "Query.h"
#include "MyHugeVector.h"
#include "MyVector.h"
#include "TreeNode.h"
#include "Buckets.h"
#include <map>
#include <cassert>
#include "IndexablePoint.h"
#include "RangeTree.h"

#define SMALL_SIZE 128

using namespace std;
class QuadTreeLog {
public:
    QuadTreeLog();
    ~QuadTreeLog();

    int aliveNum;
    int lastQNum;
    int delNum;
    int insNum;
    int alertNum;
	int next_max_ele;
	
	RangeTree aux_RT;
	int eleNum;
	int *Q_bound;	
	
	MyVector<IndexablePoint> _eleList;
    MyVector<Query> _queryList;
    MyVector<Buckets> _bucketList;
    MyVector<TreeNode> _treeNodeList;
    MyHugeVector<int> _indexInBucketList;
    map<int,int> id_index_map;

    static TreeNode emptyTreeNode;
	static MyVector<int> newEpochList;
	static IndexablePoint emptyPoint;

	void init(MyVector<Query>& queryList);

    bool DeleteQuery(int queryID);

    int InsertPoint(int *coords, int weight,MyVector<int>&alertIDList);
    
	int InsertPoint(int *coords, int weight,MyVector<int>&alertIDList, MyVector<int>&nEL,TreeNode* root, int d);

    void InsertQuery(Query& query);

    inline TreeNode* getRoot() const{
        return &(_treeNodeList[0]);
    }

    inline bool CheckRebuild(){
        bool rtn = (this->delNum + this->alertNum ) >= ((this->lastQNum + this->insNum )/ 2);

        rtn = rtn && ((this->lastQNum + this->insNum ) >= SMALL_SIZE);

        return rtn;
    }

    int rebuild(MyVector<int>& alertIDList);

    int buildRoot(int lev,int start);

	void init_Qb();
	bool useful_point(int* coords);
	int getExt(Query* query);
	void update_Qb(Query* query);
};


#endif //QUADTREE_QUADTREELOG_H
