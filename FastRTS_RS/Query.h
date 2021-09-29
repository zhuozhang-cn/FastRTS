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
	double p_cnt; // unchange until global rebuil
    double tau;// unchange until global rebuild
    double threshold; // unchange until new epoch
    double thresholdOnSite; 
    int threshold_index;
    long long startIndex; // unchange until new epoch
    double evlCnt; // init when start a new epoch
    double roundCnt; // init when start a new round
    static MyVector<int> nodeIndexList;


    Query(int id,int*range, int threshold);
    void ReleaseSpace();
    void ComputeThresholdOnSite();
    int ReceiveNotification(void * quadTree_log,double ptCntIncrease);
    double CollectInfoFromSites(void * quadTree_log);
	double CollectPt(void* quadTree_log); 
    void EnterNextRound(void* quadTree_log);
    void EnterNextEpoch(void* quadTree_log,bool needFind);
    void DeletePreviousBucketElement(void* quadTree_log,int preThreshold_index,bool needFind);
    void BroadCastThresholdOnSite(void* quadTree_log) ;
	void BroadCastThresholdOnSite_Epoch(void* quadTree_log);
    void GlobalRebuild(void* quadTree_log);
};


#endif //QUADTREE_QUERY_H
