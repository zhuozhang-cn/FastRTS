#include "Query.h"
#include <cmath>
#include "GloVars.h"
#include "QuadTreeLog.h"

MyVector<int> Query::nodeIndexList;

void Query::ComputeThresholdOnSite() {
    int rest = this->threshold - this->evlCnt;
    this->thresholdOnSite = rest / (2* this->siteNum);
    threshold_index=int(log(thresholdOnSite)/log(2));
    if (this->threshold_index < 0) {
        this->threshold_index=0;
    }
}

int Query::ReceiveNotification(void *quadTree_log, int ptCntIncrease) {
	this->evlCnt+=ptCntIncrease;
    if(this->evlCnt>=this->tau){
		if (this->epoch_n==EPO-1){
			this->ReleaseSpace();
			return 1;
		}else{
			return 3;
		}
    }
    this->roundCnt+=ptCntIncrease;
    if (this->thresholdOnSite>1 and this->evlCnt+this->roundCnt>=this->threshold){
        this->evlCnt=this->evlCnt+this->CollectInfoFromSites(quadTree_log);
        if (this->evlCnt>=this->tau){
            if (this->epoch_n==EPO-1){
				this->ReleaseSpace();
				return 1;
			}else{
				return 3;
			}
        }
        this->EnterNextRound(quadTree_log);
        return 2;
    }
    return 0;

}

int Query::CollectPt(void *quadTree_log) {
    auto* log=(QuadTreeLog*) quadTree_log;
	double e=rho_epoch[epoch_n]*(range[1]-range[0]);
	return log->getRoot()->CanonicalNodeCounter(*this,quadTree_log,e);
}


int Query::CollectInfoFromSites(void *quadTree_log) {
    auto* log=(QuadTreeLog*) quadTree_log;
    int ptCnt = 0;
	nodeIndexList.clear();
    nodeIndexList.reserve(this->siteNum);
	double e = rho_epoch[epoch_n]*(range[1]-range[0]);
    log->getRoot()->FindCanonicalNode_Space(*this,log,Query::nodeIndexList,e);
    for (int i = 0; i < siteNum; ++i) {
        TreeNode& node=log->_treeNodeList[Query::nodeIndexList[i]];
		ptCnt=ptCnt+node.ptCnt-
                log->_bucketList[node.structureIndex].
				buckList[this->threshold_index][log->_indexInBucketList[startIndex+i+1]].getLastCnt();
    }
    return ptCnt;
}

void Query::EnterNextRound(void *quadTree_log) {
    this->roundCnt = 0;
    int preThreshold_index=this->threshold_index;
    this->ComputeThresholdOnSite();
    this->DeletePreviousBucketElement(quadTree_log,preThreshold_index,false);
    this->BroadCastThresholdOnSite(quadTree_log);
}

void Query::ReceiveExact(int x){
	this->p_cnt-=x;
	if(this->tau-(0-this->p_cnt)<0){
		tau=-1;
	}
}

void Query::EnterNextEpoch(void *quadTree_log,bool needFind) {
	auto* log=(QuadTreeLog*) quadTree_log;
	int preThreshold_index=this->threshold_index;
	this->DeletePreviousBucketElement(quadTree_log,preThreshold_index,needFind);
	this->roundCnt=0;
	this->epoch_n++;
	if (this->epoch_n==EPO-1){
		if (this->threshold>=EXA_THE){
			this->epoch_n--;
		}
	}
	nodeIndexList.clear();
	int length=range[1]-range[0];
	double e1=rho_epoch[epoch_n]*length;
	int pre=this->evlCnt;
	int p_n=this->siteNum;
	this->evlCnt=log->getRoot()->FindCanonicalNode_Epoch(*this,log,Query::nodeIndexList,e1);
	this->evlCnt=log->getExt(this);
	this->threshold=this->threshold-(this->evlCnt-this->p_cnt);
	this->tau=this->tau-(this->evlCnt-this->p_cnt);
	this->p_cnt=this->evlCnt;
	this->siteNum=Query::nodeIndexList.size();
	this->evlCnt=0;
	this->ComputeThresholdOnSite();
	this->BroadCastThresholdOnSite_Epoch(quadTree_log);
}

void Query::DeletePreviousBucketElement(void *quadTree_log, int preThreshold_index, bool needFind) {
    auto* log=(QuadTreeLog*) quadTree_log;
	if(needFind){
		nodeIndexList.clear();
		nodeIndexList.reserve(this->siteNum);
		double e=rho_epoch[epoch_n]*(range[1]-range[0]);
		log->getRoot()->FindCanonicalNode_Space(*this,log,Query::nodeIndexList,e);
	}
    for(int i=0;i<this->siteNum;i++){
        TreeNode& node=log->_treeNodeList[Query::nodeIndexList[i]];
        Buckets& buck=log->_bucketList[node.structureIndex];
		buck.DeleteElement(preThreshold_index,log->_indexInBucketList[startIndex+i+1],quadTree_log);
    }
}

void Query::BroadCastThresholdOnSite_Epoch(void* quadTree_log){
	auto *log=(QuadTreeLog*) quadTree_log;
	int queryIndex=log->_indexInBucketList[this->startIndex]; 
	this->startIndex=(long long) log->_indexInBucketList.size();
	log->_indexInBucketList.push_back(queryIndex);
	
	for(int i=0;i<this->siteNum;++i){
		TreeNode& node=log->_treeNodeList[Query::nodeIndexList[i]];
		Buckets* buck=&(log->_bucketList[node.structureIndex]);
        TreeNode::emptyBucketElement.setQueryStartIndex(this->startIndex);
        TreeNode::emptyBucketElement.setIndexInQuery((short)i);
        TreeNode::emptyBucketElement.setLastCnt(node.ptCnt);
		log->_indexInBucketList.push_back(buck->sizeByIndex(threshold_index));
		buck->InsertNewELement(threshold_index,TreeNode::emptyBucketElement);
	}
}

void Query::BroadCastThresholdOnSite(void *quadTree_log) {
    auto* log = (QuadTreeLog*) quadTree_log;
    for (int i = 0; i < this->siteNum; ++i) {
        // Here we do not need to compute nodeIndexList, because we have computed it
        // in function CollectInfoFromSites_Space.
        TreeNode& node=log->_treeNodeList[Query::nodeIndexList[i]];
        Buckets& buck=log->_bucketList[node.structureIndex];
        TreeNode::emptyBucketElement.setQueryStartIndex(this->startIndex);
        TreeNode::emptyBucketElement.setIndexInQuery((short)i);
        TreeNode::emptyBucketElement.setLastCnt(node.ptCnt);
		buck.InsertNewELement(threshold_index,TreeNode::emptyBucketElement);
        log->_indexInBucketList[startIndex+i+1]=buck.sizeByIndex(threshold_index)-1;
    }
}

void Query::ReleaseSpace() {
    //lazy release
    if (this->range!= nullptr){
        delete[] range;
        this->range = nullptr;
    }
    this->threshold=-1;
}



Query::Query(int id, int *range, int threshold) {
    this->id=id;
    this->range=range;
    this->threshold=threshold;
    this->tau=(threshold*(1-GAMMA))+1;
	this->startIndex=-1;
    this->siteNum=-1;
    this->evlCnt=0;
    this->roundCnt=0;
    this->thresholdOnSite=0;
	this->threshold_index=-1;
	this->epoch_n=0;
	this->p_cnt=0;
}

void Query::GlobalRebuild(void *quadTree_log) {
    this->evlCnt=((QuadTreeLog*)quadTree_log)->getExt(this)-p_cnt;
    this->tau-=this->evlCnt;
    this->threshold-=this->evlCnt;
    this->evlCnt=0;
    this->roundCnt=0;
    this->thresholdOnSite=0;
	this->threshold_index=-1;
	this->p_cnt=0;
}




