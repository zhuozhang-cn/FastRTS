#include "TreeNode.h"
#include "QuadTreeLog.h"
#include "Buckets.h"
#include <cmath>
BucketElement TreeNode::emptyBucketElement;
Buckets TreeNode::emptyBuckets;
TreeNode::TreeNode() {
    this->structureIndex=-1;
    this->range= nullptr;
    this->childIndexList= nullptr;
    this->ptCnt=0;
	this->secStructureIndex-=1;
	this->dim_start=0;
}

void TreeNode::InsertQueryList(MyVector<int> &queryIndexList, void *quadTree_Log) {
    QuadTreeLog* log=(QuadTreeLog*)quadTree_Log;
    int queryNum=(int)queryIndexList.size();
    log->_treeNodeList.shrink_to_fit();
    MyVector<Buckets>& bucketList=log->_bucketList;

    for(int i=0;i<queryNum;++i){
        int query_index=queryIndexList[i];
        Query& query=log->_queryList[query_index];
        Query::nodeIndexList.clear();
		int query_length=query.range[1]-query.range[0];
		double e1=rho_epoch[query.epoch_n]*query_length;

		log->getRoot()->FindCanonicalNode_Epoch(query,log,Query::nodeIndexList,e1);
		query.p_cnt=log->getExt(&query);
		query.siteNum=(int)Query::nodeIndexList.size();

        if (query.siteNum<=0){
            printf("queryid %d\n",query.id);
        }
        query.evlCnt=0;
		query.roundCnt=0;
		query.ComputeThresholdOnSite();
        query.startIndex=(long long)log->_indexInBucketList.size();
        log->_indexInBucketList.push_back(query_index);
		int qti=query.threshold_index;
        for(int j=0;j<query.siteNum;++j){
            TreeNode& node=log->_treeNodeList[Query::nodeIndexList[j]];
            Buckets& buck=bucketList[node.structureIndex];
            TreeNode::emptyBucketElement.setIndexInQuery((short)j);
            TreeNode::emptyBucketElement.setQueryStartIndex(query.startIndex);
            TreeNode::emptyBucketElement.setLastCnt(node.ptCnt);
            log->_indexInBucketList.push_back(buck.sizeByIndex(qti));
			buck.InsertNewELement(qti,TreeNode::emptyBucketElement);
        }
    }
    log->_bucketList.shrink_to_fit();
    log->_indexInBucketList.shrink_to_fit();
}

int TreeNode::InsertPoint(int weight, MyVector<int> &alertIDList,MyVector<int> &newEpochList, void *quadTree_Log) {
    //Ensure that this point stab the range of this node.
    ptCnt=ptCnt+weight;
    int alertCnt = 0;
    QuadTreeLog* log=(QuadTreeLog*) quadTree_Log;
    Buckets& buck=log->_bucketList[this->structureIndex];
    for(int i=0;i<buck.listSize();++i){
        if(buck.CheckEmptyByIndex(i)){
            continue;
        }
        if(ptCnt/pow_2[i]-(ptCnt-weight)/pow_2[i]==0){
            break;
        }
        MyVector<BucketElement>& buckLists=buck.buckList[i];
        int buckListSize=buckLists.size();
        for(int j=0;j<buckListSize;++j){
			int queryInd=log->_indexInBucketList[buckLists[j].getQueryStartIndex()];
            Query& query=log->_queryList[queryInd];
            if(query.threshold<0){
                buck.DeleteElement(i,j,quadTree_Log);
                buckListSize--;
                j--;
                continue;
            }
            int ptCntIncrease=ptCnt-buckLists[j].getLastCnt();
            buckLists[j].setLastCnt(ptCnt);
            int temp=query.ReceiveNotification(quadTree_Log,ptCntIncrease);

            if(temp==1){
                buck.DeleteElement(i,j,quadTree_Log);
                buckListSize--;
                j--;
                query.ReleaseSpace();
                alertCnt++;
                alertIDList.push_back(query.id);
            }else if(temp==2){
                j--;
                buckListSize--;
            }else if (temp==3){
				newEpochList.push_back(queryInd);
			}
        }
    }
    return alertCnt;

}

bool inline ifContain(const int *x, const int *y){
    return (x[0]<=y[0]) and (x[1]>=y[1]);
}

bool inline ifDisjoint(const int* x, const int *y){
    return (x[0]>=y[1]) or (x[1]<=y[0]);
}

int TreeNode::CanonicalNodeCounter(Query &query, void *quadTree_log,double err_max) {
    QuadTreeLog* log=(QuadTreeLog*) quadTree_log;
	int* tmpChildIndexList=this->childIndexList;
	int* tmpRange=this->range;
	int thisId=this->structureIndex;
	int d=this->dim_start;
	int result=0;
	bool *disjoint=(bool*)malloc(sizeof(bool)*2);
	bool *contain=(bool*)malloc(sizeof(bool)*2);
	disjoint[0]=ifDisjoint(query.range+2*d,tmpRange);
	contain[0]=ifContain(query.range+2*d,tmpRange);
	disjoint[1]=ifDisjoint(query.range+2*d,tmpRange+1);
	contain[1]=ifContain(query.range+2*d,tmpRange+1);
	int child_length=tmpRange[1]-tmpRange[0];

	bool small= (tmpRange[1]-tmpRange[0]<=err_max);

	for(int child_i=0;child_i<2;child_i++){
		int code=child_i;
		bool dis=false;
		bool c=true;
		int offset=code-code/2*2;
		dis=dis or disjoint[offset];
		c=c and contain[offset];
		code=code/2;
		if(dis){
			continue;
		}
		c=c or small;
		int childId=tmpChildIndexList[child_i];
		if(childId<=0){
			//no need to build new node 
			//continue;
			childId=(int)log->_treeNodeList.size();
			QuadTreeLog::emptyTreeNode.init(childId,ChildRange(child_i,tmpRange,1),1,d);
			tmpChildIndexList[child_i]=childId;
			log->_treeNodeList.push_back(QuadTreeLog::emptyTreeNode);
			log->_bucketList.push_back(TreeNode::emptyBuckets);
		}
		TreeNode* child=&(log->_treeNodeList[childId]);
		if(!c){
			result=result+child->CanonicalNodeCounter(query,quadTree_log,err_max);
		}else{
			if (d+1==DIM_K){
				result=result+child->ptCnt;
			}else{
				int rest_dim=DIM_K-d-1>MAX_D?MAX_D:DIM_K-d-1;
				if (child->secStructureIndex==-1){
					int sec_index=log->buildRoot(rest_dim,d+1);
					child=&(log->_treeNodeList[childId]);
					child->secStructureIndex=sec_index;
				}
				TreeNode* secRoot=&(log->_treeNodeList[child->secStructureIndex]);
				result=result+secRoot->CanonicalNodeCounter(query,quadTree_log,err_max);
			}
		}
	}	
	free(contain);
	free(disjoint);
	return result;
}

int TreeNode::FindCanonicalNode_Epoch(Query &query, void *quadTree_log, MyVector<int> &nodeIndexList,double err_epo) {
    QuadTreeLog* log=(QuadTreeLog*) quadTree_log;
	int* tmpChildIndexList=this->childIndexList;
	int* tmpRange=this->range;
	int thisId=this->structureIndex;
	int d=this->dim_start;
	int result=0;
	
	bool *disjoint=(bool*)malloc(sizeof(bool)*2);
	bool *contain=(bool*)malloc(sizeof(bool)*2);
	disjoint[0]=ifDisjoint(query.range+2*d,tmpRange);
	contain[0]=ifContain(query.range+2*d,tmpRange);
	disjoint[1]=ifDisjoint(query.range+2*d,tmpRange+1);
	contain[1]=ifContain(query.range+2*d,tmpRange+1);
	int child_length=tmpRange[1]-tmpRange[0];

	bool small= (tmpRange[1]-tmpRange[0]<=err_epo);

	for(int child_i=0;child_i<2;child_i++){
		int code=child_i;
		bool dis=false;
		bool c=true;
		int offset=code-code/2*2;
		dis=dis or disjoint[offset];
		c=c and contain[offset];
		code=code/2;
		if(dis){
			continue;
		}
		int childId=tmpChildIndexList[child_i];
		TreeNode* child=NULL;

		if (c or small){
			if(childId<=0){
				childId=(int)log->_treeNodeList.size();
				QuadTreeLog::emptyTreeNode.init(childId,ChildRange(child_i,tmpRange,1),1,d);
				tmpChildIndexList[child_i]=childId;
				log->_treeNodeList.push_back(QuadTreeLog::emptyTreeNode);
				log->_bucketList.push_back(TreeNode::emptyBuckets);
			}
			child=&(log->_treeNodeList[childId]);
			if (d+1==DIM_K){
				result=result+child->ptCnt;
				nodeIndexList.push_back(child->structureIndex);
			}else{
				int rest_dim=DIM_K-d-1>MAX_D?MAX_D:DIM_K-d-1;
				if (child->secStructureIndex==-1){
					int sec_index=log->buildRoot(rest_dim,d+1);
					child=&(log->_treeNodeList[childId]);
					child->secStructureIndex=sec_index;
				}
				TreeNode* secRoot=&(log->_treeNodeList[child->secStructureIndex]);
				result=result+secRoot->FindCanonicalNode_Epoch(query,quadTree_log,nodeIndexList,err_epo);
			}
			
		}
		else{
			if(childId<=0){
				childId=(int)log->_treeNodeList.size();
				QuadTreeLog::emptyTreeNode.init(childId,ChildRange(child_i,tmpRange,1),1,d);
				tmpChildIndexList[child_i]=childId;
				log->_treeNodeList.push_back(QuadTreeLog::emptyTreeNode);
				log->_bucketList.push_back(TreeNode::emptyBuckets);
			}
			child=&(log->_treeNodeList[childId]);
			result=result+child->FindCanonicalNode_Epoch(query,quadTree_log,nodeIndexList,err_epo);
		}
	}	
	free(contain);
	free(disjoint);
	return result;
}

void TreeNode::FindCanonicalNode_Space(Query &query, void *quadTree_log, MyVector<int> &nodeIndexList, double err_max) {
    QuadTreeLog* log=(QuadTreeLog*) quadTree_log;
	int* tmpChildIndexList=this->childIndexList;
	int* tmpRange=this->range;
	int thisId=this->structureIndex;
	int d=this->dim_start;
	
	bool *disjoint=(bool*)malloc(sizeof(bool)*2);
	bool *contain=(bool*)malloc(sizeof(bool)*2);
		disjoint[0]=ifDisjoint(query.range+2*d,tmpRange);
		contain[0]=ifContain(query.range+2*d,tmpRange);
		disjoint[1]=ifDisjoint(query.range+2*d,tmpRange+1);
		contain[1]=ifContain(query.range+2*d,tmpRange+1);
	int child_length=tmpRange[1]-tmpRange[0];

	bool small= (tmpRange[1]-tmpRange[0]<=err_max);

	for(int child_i=0;child_i<2;child_i++){
		int code=child_i;
		bool dis=false;
		bool c=true;
		int offset=code-code/2*2;
		dis=dis or disjoint[offset];
		c=c and contain[offset];
		code=code/2;
		if(dis){
			continue;
		}
		c=c or small;
		int childId=tmpChildIndexList[child_i];
		if(childId<=0){
			childId=(int)log->_treeNodeList.size();
			QuadTreeLog::emptyTreeNode.init(childId,ChildRange(child_i,tmpRange,1),1,d);
			tmpChildIndexList[child_i]=childId;
			log->_treeNodeList.push_back(QuadTreeLog::emptyTreeNode);
			log->_bucketList.push_back(TreeNode::emptyBuckets);
		}
		TreeNode* child=&(log->_treeNodeList[childId]);
		if(!c){
			child->FindCanonicalNode_Space(query,quadTree_log,nodeIndexList,err_max);
		}else{
			if (d+1==DIM_K){
				nodeIndexList.push_back(child->structureIndex);
			}else{
				int rest_dim=DIM_K-d-1>MAX_D?MAX_D:DIM_K-d-1;
				if (child->secStructureIndex==-1){
					int sec_index=log->buildRoot(rest_dim,d+1);
					child=&(log->_treeNodeList[childId]);
					child->secStructureIndex=sec_index;
				}
				TreeNode* secRoot=&(log->_treeNodeList[child->secStructureIndex]);
				secRoot->FindCanonicalNode_Space(query,quadTree_log,nodeIndexList,err_max);
			}
		}
		
	}	
	free(contain);
	free(disjoint);
}

int TreeNode::stabChildIndex(const int *coords, int d) const {
    int code=0;
        if(coords[d]<this->range[1]){
             code=code*2;
        }else{
            code=code*2+1;
        }
    return code;
}

TreeNode::TreeNode(int id, int *coord,int lev,int start) {
    this->structureIndex=id;
    this->range=coord;
	this->dim_start=start;
    this->childIndexList=new int[pow_2[lev]];
    for(int i=0;i<pow_2[lev];++i){
        this->childIndexList[i]=-1;
    }
    this->ptCnt=0;
}

TreeNode::~TreeNode() {

}

void TreeNode::init(int id, int *coord,int lev,int start) {
	this->dim_start=start;
    this->structureIndex=id;
    this->range=coord;
	int child_num=pow_2[lev];
    this->childIndexList=new int[child_num];
    for(int i=0;i<child_num;++i){
        this->childIndexList[i]=-1;
    }
    this->ptCnt=0;
}

void TreeNode::InsertQuery(int queryIndex, void *quadTree_Log) {
    QuadTreeLog* log=(QuadTreeLog*)quadTree_Log;
    log->_treeNodeList.shrink_to_fit();
    MyVector<Buckets>& bucketList=log->_bucketList;
    Query& query=log->_queryList[queryIndex];
    Query::nodeIndexList.clear();
	int query_length=query.range[1]-query.range[0];
	double e1=rho_epoch[query.epoch_n]*query_length;

	query.p_cnt=log->getRoot()->FindCanonicalNode_Epoch(query,log,Query::nodeIndexList,e1);
	query.p_cnt=log->getExt(&query);
    query.siteNum=(int)Query::nodeIndexList.size();
    query.evlCnt=0;
    query.ComputeThresholdOnSite();
    query.startIndex=(long long)log->_indexInBucketList.size();
    log->_indexInBucketList.push_back(queryIndex);
	int qti=query.threshold_index;
    for(int j=0;j<query.siteNum;++j){
        TreeNode& node=log->_treeNodeList[Query::nodeIndexList[j]];
        Buckets& buck=bucketList[node.structureIndex];
        TreeNode::emptyBucketElement.setIndexInQuery((short)j);
        TreeNode::emptyBucketElement.setQueryStartIndex(query.startIndex);
        TreeNode::emptyBucketElement.setLastCnt(node.ptCnt);
        log->_indexInBucketList.push_back(buck.sizeByIndex(qti));
		buck.InsertNewELement(qti,TreeNode::emptyBucketElement);
    }
}

void TreeNode::ReleaseSpace() {
    if (this->range!= nullptr){
        delete[] range;
        this->range = nullptr;
    }
    if (this->childIndexList!= nullptr){
        delete[] childIndexList;
        this->childIndexList = nullptr;
    }
    structureIndex=-1;
}




