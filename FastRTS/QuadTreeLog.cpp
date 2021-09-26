#include "QuadTreeLog.h"
#include "RangeTree.h"
TreeNode QuadTreeLog::emptyTreeNode;
MyVector<int> QuadTreeLog::newEpochList;
IndexablePoint QuadTreeLog::emptyPoint;

#define REBUILD_EFF 0.1

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
	eleNum=0;
	Q_bound=(int*)malloc(DIM_K*2*sizeof(int));
	next_max_ele=100;
	_eleList.reserve(next_max_ele);
}

QuadTreeLog::~QuadTreeLog() {

}

void QuadTreeLog::init_Qb(){
	for(int i=0;i<DIM_K;i++){
		Q_bound[i*2]=U;
		Q_bound[i*2+1]=0;
	}
}

void QuadTreeLog::update_Qb(Query* query){
	for(int i=0;i<DIM_K;i++){
		Q_bound[2*i]=min(Q_bound[2*i],query->range[2*i]);
		Q_bound[2*i+1]=max(Q_bound[2*i+1],query->range[2*i+1]);
	}

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
		update_Qb(&(this->_queryList[i]));
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
		if (1+d<DIM_K){
			if (node->secStructureIndex>0){
				alert+=InsertPoint(coords,weight,alertIDList,newEpochList,
						&(this->_treeNodeList[node->secStructureIndex]),d+1);
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
	if (!useful_point(coords)){
		return 0;
	}
    int alert=0;
    int code;

	emptyPoint.id=eleNum;
	eleNum++;
	emptyPoint.coords=(int*)malloc(DIM_K*sizeof(int));
	for(int i=0;i<DIM_K;i++){
		emptyPoint.coords[i]=coords[i];
		emptyPoint.weight=weight;
	}
	_eleList.push_back(emptyPoint);
	aux_RT.insertElement(&(_eleList[eleNum-1]));


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
    if (eleNum>=next_max_ele){
		printf("rebuild range tree:%d\n",eleNum);
		fflush(stdout);
		double aaaaa=getCurrentTime();
	init_Qb();
		for(int i=0;i<this->_queryList.size();i++){
			if (_queryList[i].threshold>0){
				update_Qb(&(_queryList[i]));
				_queryList[i].ReceiveExact(getExt(&(this->_queryList[i])));
				if (_queryList[i].tau<0){
					alertIDList.push_back(_queryList[i].id);
					_queryList[i].ReleaseSpace();
					alert++;
					this->aliveNum--;
					this->alertNum++;
				}
			}
		}
		eleNum=0;
		aux_RT.releaseSpace();
		for(int i=0;i<this->_eleList.size();i++){
			this->_eleList.release_space();
		}
		next_max_ele=max(5000,this->aliveNum*REBUILD_EFF);
		this->_eleList.clear();
		this->_eleList.reserve(next_max_ele);
		printf("rebuild range tree time:%lf\n",getCurrentTime()-aaaaa);
		fflush(stdout);

	}
	return alert;
}

void QuadTreeLog::InsertQuery(Query &query) {
	id_index_map[query.id]=this->_queryList.size();
    	this->_queryList.push_back(query);
	update_Qb(&(this->_queryList[this->_queryList.size()-1]));
    this->getRoot()->InsertQuery(this->_queryList.size()-1,this);
    insNum++;
    aliveNum++;
}

int QuadTreeLog::rebuild(MyVector<int> &alertIDList) {
	int res=0;
    printf("start rebuild!!!\n");
	init_Qb();
    MyVector<Query> newQueryList;
    for(int i=0;i<this->_queryList.size();i++){
        if(_queryList[i].threshold>0){
            _queryList[i].GlobalRebuild(this);
			if (_queryList[i].tau>0){
				newQueryList.push_back(_queryList[i]);
				update_Qb(&(_queryList[i]));
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
	for(int i=0;i<this->_eleList.size();i++){
		this->_eleList[i].releaseSpace();
	}
	eleNum=0;
	aux_RT.releaseSpace();
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
	next_max_ele=max(5000,this->aliveNum*REBUILD_EFF);
	this->_eleList.clear();
	this->_eleList.reserve(next_max_ele);
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


bool QuadTreeLog::useful_point(int* coords){
	for(int i=0;i<DIM_K;i++){
		if(coords[i]>=Q_bound[2*i+1] or coords[i]<Q_bound[2*i]){
			return false;
		}
	}
	return true;

}

int QuadTreeLog::getExt(Query* query){
	return aux_RT.rangeCntQuery(query->range);
}

