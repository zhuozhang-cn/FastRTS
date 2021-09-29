#include "QuadTreeLog.h"
TreeNode QuadTreeLog::emptyTreeNode;
MyVector<int> QuadTreeLog::newEpochList;
QuadTreeLog::QuadTreeLog() {
	int dim=DIM_K>MAX_D?MAX_D:DIM_K;
    int* u=new int[3*dim];
    for(int i=0;i<dim;++i){
        u[i*3]=0;
        u[i*3+1]=U/2;
        u[i*3+2]=U;
    }
    QuadTreeLog::emptyTreeNode.init(0,u,dim,0);
    _treeNodeList.push_back(QuadTreeLog::emptyTreeNode);

    _bucketList.push_back(TreeNode::emptyBuckets);
    aliveNum = 0;
    lastQNum=0;
    delNum=0;
    insNum=0;
    alertNum=0;
}

QuadTreeLog::~QuadTreeLog() {

}

void QuadTreeLog::init(MyVector<Query> &queryList) {
    int dim=DIM_K>MAX_D?MAX_D:DIM_K;
	int* u=new int[3*dim];
    for(int i=0;i<dim;++i){
        u[i*3]=0;
        u[i*3+1]=U/2;
        u[i*3+2]=U;
    }


    emptyTreeNode.init(0,u,dim,0);
    _treeNodeList.push_back(QuadTreeLog::emptyTreeNode);
    _bucketList.push_back(TreeNode::emptyBuckets);
    this->aliveNum=(int)queryList.size();
    this->_queryList.swap(queryList);
    MyVector<int> queryIndexList;
    queryIndexList.reserve(this->aliveNum);
    for(int i=0;i<aliveNum;++i){
        queryIndexList.push_back(i);
        id_index_map[_queryList[i].id]=i;
    }


    this->getRoot()->InsertQueryList(queryIndexList,this);

    queryIndexList.release_space();
    _treeNodeList.shrink_to_fit();
    _bucketList.shrink_to_fit();
    _indexInBucketList.shrink_to_fit();
    lastQNum=aliveNum;
    delNum=0;
    insNum=0;
    alertNum=0;
}

bool QuadTreeLog::DeleteQuery(int queryID) {
    auto it = id_index_map.find(queryID);
    if(it!=id_index_map.end()){
        int index=it->second;
        if (_queryList[index].threshold!=-1){
            /* Here we delete the query lazily.
             * When we scan its bucket, we delete its bucket element.
             * */
            _queryList[index].ReleaseSpace();
            aliveNum--;
            id_index_map.erase(it);
            delNum++;
            return true;
        }
        id_index_map.erase(it);
        return false;
    }
    return false;
}

int QuadTreeLog::InsertPoint(int *coords,int weight, MyVector<int> &alertIDList,
		MyVector<int>&nEL,TreeNode* root,int d){
	int alert=0;
	int code;
	int index=root->structureIndex;
	TreeNode* node=root;
	while(true){
		if (node->dim_lev+d<DIM_K){
			if (node->secStructureIndex>0){
				alert+=InsertPoint(coords,weight,alertIDList,newEpochList,
						&(this->_treeNodeList[node->secStructureIndex]),d+node->dim_lev);
			}
			
		}else{
			alert+=node->InsertPoint(weight,alertIDList,nEL,this);
		}
		node=&(this->_treeNodeList[index]);
		code=node->stabChildIndex(coords,d);
		index=node->childIndexList[code];
		if(index<=0){
				break;
		}
		node=&(this->_treeNodeList[index]);
	}
	return alert;

}



int QuadTreeLog::InsertPoint(int *coords, int weight, MyVector<int> &alertIDList) {
    int alert=0;
    int code;
	QuadTreeLog::newEpochList.clear();
    TreeNode* node=this->getRoot();
    alert=InsertPoint(coords,weight,alertIDList,newEpochList,node,0);

	for(int i=0;i<newEpochList.size();++i){
		Query& query=this->_queryList[newEpochList[i]];
		query.EnterNextEpoch(this,true);
		if (query.tau<0){
			alertIDList.push_back(query.id);
			query.ReleaseSpace();
			alert++;
		}
	}
	
    this->aliveNum-=alert;
    this->alertNum+=alert;
    if(CheckRebuild()){
        int x=rebuild(alertIDList);
		this->alertNum+=x;
		alert+=x;
    }
    return alert;
}

void QuadTreeLog::InsertQuery(Query &query) {
	id_index_map[query.id]=this->_queryList.size();
    	this->_queryList.push_back(query);
    this->getRoot()->InsertQuery(this->_queryList.size()-1,this);
    insNum++;
    aliveNum++;
}

int QuadTreeLog::rebuild(MyVector<int> &alertIDList) {
	int res=0;
    printf("start rebuild!!!\n");
    MyVector<Query> newQueryList;
    for(int i=0;i<this->_queryList.size();i++){
        if(_queryList[i].threshold>0){
            _queryList[i].GlobalRebuild(this);
			if (_queryList[i].tau>0){
				newQueryList.push_back(_queryList[i]);
			}else{
				alertIDList.push_back(_queryList[i].id);
				_queryList[i].ReleaseSpace();
				res++;
			}
        }
    }
    for(int i=0;i<this->_treeNodeList.size();i++){
        this->_treeNodeList[i].ReleaseSpace();
        this->_bucketList[i].ReleaseSpace();
    }
    this->_treeNodeList.clear();
    this->_bucketList.clear();
    this->_indexInBucketList.clear();
    id_index_map.clear();
	int dim=DIM_K>MAX_D?MAX_D:DIM_K;
	int* u=new int[3*dim];
    for(int i=0;i<dim;++i){
        u[i*3]=0;
        u[i*3+1]=U/2;
        u[i*3+2]=U;
    }
    emptyTreeNode.init(0,u,dim,0);
    _treeNodeList.push_back(QuadTreeLog::emptyTreeNode);
    _bucketList.push_back(TreeNode::emptyBuckets);
   
    this->_queryList.swap(newQueryList);
    newQueryList.release_space();
    MyVector<int> queryIndexList;
    this->aliveNum=this->_queryList.size();
    queryIndexList.reserve(this->aliveNum);
    for(int i=0;i<aliveNum;++i){
        queryIndexList.push_back(i);
        id_index_map[_queryList[i].id]=i;
    }
    this->getRoot()->InsertQueryList(queryIndexList,this);
    queryIndexList.release_space();
    _treeNodeList.shrink_to_fit();
    _bucketList.shrink_to_fit();
    _indexInBucketList.shrink_to_fit();
    this->lastQNum=this->aliveNum;
    this->delNum=0;
    this->insNum=0;
    this->alertNum=0;
	return res;
}

int QuadTreeLog::buildRoot(int lev,int start) {
	int* u=new int[3*lev];
    for(int i=0;i<lev;++i){
        u[i*3]=0;
        u[i*3+1]=U/2;
        u[i*3+2]=U;
    }
	int id=this->_treeNodeList.size();
    QuadTreeLog::emptyTreeNode.init(id,u,lev,start);
	this->_treeNodeList.push_back(QuadTreeLog::emptyTreeNode);
	this->_bucketList.push_back(TreeNode::emptyBuckets);
	return id;
}

