#ifndef QUADTREE_QUERY_H
#define QUADTREE_QUERY_H
#include "MyVector.h"
#include "GloVars.h"

class Query {
public:
    int *range;
    int id;
    int siteNum;
	int epoch_n; // ++when start a new epoch
	int p_cnt; // unchange until global rebuil
    int tau;// unchange until global rebuild
    int threshold; // unchange until new epoch
    int thresholdOnSite; 
    int threshold_index;
    long long startIndex; // unchange until new epoch
    int evlCnt; // init when start a new epoch
    int roundCnt; // init when start a new round
    static MyVector<int> nodeIndexList;

    Query(int id,int*range, int threshold);
    void ReleaseSpace();
    void ComputeThresholdOnSite();
    int ReceiveNotification(void * quadTree_log,int ptCntIncrease);
    int CollectInfoFromSites(void * quadTree_log);
	int CollectPt(void* quadTree_log); 
    void EnterNextRound(void* quadTree_log);
    void EnterNextEpoch(void* quadTree_log,bool needFind);
    void DeletePreviousBucketElement(void* quadTree_log,int preThreshold_index,bool needFind);
    void BroadCastThresholdOnSite(void* quadTree_log) ;
	void BroadCastThresholdOnSite_Epoch(void* quadTree_log);
    void GlobalRebuild(void* quadTree_log);
	void ReceiveExact(int x);
};


#endif //QUADTREE_QUERY_H
