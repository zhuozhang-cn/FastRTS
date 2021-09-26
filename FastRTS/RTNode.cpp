#include "RTNode.h"
#include <cassert>

MyVector<IndexablePoint*> RTNode::emptyVec;
double RTNode::duration;

RTNode::RTNode(){
	child[0]=NULL;
	child[1]=NULL;
	node_cnt=0;
	weight_sum=0;
	dim_lev=-1;
	range[0]=-1;
	range[1]=-1;
	subtree=NULL;
}
RTNode::RTNode(int d){
	child[0]=NULL;
	child[1]=NULL;
	node_cnt=0;
	weight_sum=0;
	dim_lev=d;
	range[0]=-1;
	range[1]=-1;
	subtree=NULL;
}

void RTNode::insertElement(IndexablePoint* ele){
	int d_v=ele->coords[dim_lev];
	//down
	if (node_cnt==0){
		//a newly build leaf node, specify ele
		node_cnt++;
		range[0]=d_v;
		range[1]=d_v;
		weight_sum=ele->weight;
		child[0]=ele;
	}else if (node_cnt==1){
		//so no child until now, spilt
		IndexablePoint* ele2=(IndexablePoint*) (child[0]);
		for (int i=0;i<2;i++){
			child[i]=new RTNode(dim_lev);
		}
		node_cnt++;
		weight_sum+=ele->weight;
		
		if (ele2->smaller(dim_lev,ele)){
			range[1]=d_v;
			((RTNode*)(child[0]))->insertElement(ele2);
			((RTNode*)(child[1]))->insertElement(ele);
		}else{
			range[0]=d_v;
			((RTNode*)(child[1]))->insertElement(ele2);
			((RTNode*)(child[0]))->insertElement(ele);
		}
	}else{
		int chd=insert_index(d_v);
		if (check_rebuild(chd)){
			static int rebuild_n=0;
			double start=getCurrentTime();
			rebuild(ele);
			duration+=getCurrentTime()-start;
			return;
		}
		if (child[chd]==NULL){
			printf("Something wrong in insert node!!\n");
			exit(12);
			child[chd]= new RTNode(dim_lev);
		}
		node_cnt++;
		weight_sum+=ele->weight;
		range[0]=min(range[0],d_v);
		range[1]=max(range[1],d_v);
		((RTNode*)(child[chd]))->insertElement(ele);
	}
	if (dim_lev<DIM_K-1){
		if (subtree==NULL){
			subtree=new RTNode(dim_lev+1);
		}
		((RTNode*)subtree)->insertElement(ele);
	}
}
#define MIN_REBUILD 16
#define W_BAL 4
bool RTNode::check_rebuild(int chd){
	if (node_cnt<=MIN_REBUILD){
		return false;
	}
	static int cnts[2];
	for(int i=0;i<2;i++){
		cnts[i]=((RTNode*)(child[i]))->node_cnt;
	}
	cnts[chd]+=1;
	if (cnts[0]>cnts[1]*W_BAL or cnts[1]>cnts[0]*W_BAL){
		return true;
	}
	return false;
}

void RTNode::rebuild(IndexablePoint* ele){
	MyVector<IndexablePoint*> *sorted_list;
	sorted_list=(MyVector<IndexablePoint*>*)malloc(sizeof(MyVector<IndexablePoint*>)*(DIM_K-dim_lev));
	RTNode* curd_top=this;
	for (int i=0;i<DIM_K-dim_lev;i++){
		sorted_list[i]=emptyVec;
		sorted_list[i].reserve(node_cnt+1);
		IndexablePoint* tmpe=ele;
		if (curd_top->range[0]>ele->coords[i+this->dim_lev]){
			sorted_list[i].push_back(ele);
			tmpe=NULL;
		}else if (curd_top->range[1]<=ele->coords[i+this->dim_lev]){
			tmpe=NULL;
		}
		curd_top->down_collectp(sorted_list+i,tmpe);
		if (curd_top->range[1]<=ele->coords[i+this->dim_lev]){
			sorted_list[i].push_back(ele);
		}
		if(sorted_list[i].size()!=(this->node_cnt+1)){
			printf("something wrong in rtnode rebuild!!\n");
		}
		curd_top=curd_top->subtree;
	}
	this->ReleaseSpace();
	
	subtree=NULL;
	child[0]=NULL;
	child[1]=NULL;
	node_cnt=0;
	weight_sum=0;
	range[0]=-1;
	range[1]=-1;
	
	batch_insert(sorted_list);
	for(int i=0;i<DIM_K-dim_lev;i++){
		sorted_list[i].release_space();
	}
	delete sorted_list;
}

int RTNode::insert_index(int d_v){
	//ensure that this node has two child nodes, not a leaf node!!
	int leftmax=((RTNode*)(child[0]))->range[1];
	if (d_v<leftmax){
		return 0;
	}
	return 1; //when inserting one element, we only insert it from small id to large id. the same value in this dim means it is a larger one. so insert it into the right one.
	/*
	int rightmin=((RTNode*)(child[1]))->range[0];
	if (d_v>rightmin){
		return 1;
	}
	if (((RTNode*)(child[1]))->node_cnt<((RTNode*)(child[0]))->node_cnt){
		return 1;
	}else {
		return 0;
	}
	*/
}

void RTNode::down_collectp(MyVector<IndexablePoint*> *eles, IndexablePoint* ele){
	if (this->node_cnt==1){
		eles->push_back((IndexablePoint*)(child[0]));
		return;
	}
	RTNode* lc=(RTNode*)(this->child[0]);
	RTNode* rc=(RTNode*)(this->child[1]);
	if (ele!=NULL and 
			lc->range[1]<=ele->coords[dim_lev] and rc->range[0]>ele->coords[dim_lev]){
		lc->down_collectp(eles,NULL);
		eles->push_back(ele);
		rc->down_collectp(eles,NULL);
	}else {
		lc->down_collectp(eles,ele);
		rc->down_collectp(eles,ele);
	}
}

RTNode::~RTNode(){
	this->ReleaseSpace();
}

void RTNode::ReleaseSpace(){
	if (subtree!=NULL){
		delete subtree;
	}
	if (node_cnt>=2){
		delete ((RTNode*)(child[0]));
		delete ((RTNode*)(child[1]));
	}
}

int largest_power(unsigned int N)
{
    // assume int is 32bit
    N = N | (N>>1);
    N = N | (N>>2);
    N = N | (N>>4);
    N = N | (N>>8);
    N = N | (N>>16);
    return (N + 1) >> 1;
}

void RTNode::batch_insert(MyVector<IndexablePoint*> *eles){
	if (eles[0].size()==1){
		IndexablePoint* tmp=eles[0][0];
		this->range[0]=tmp->coords[this->dim_lev];
		this->range[1]=this->range[0];
		this->node_cnt=1;
		this->weight_sum=tmp->weight;
		this->child[0]=tmp;
	}else{
		MyVector<IndexablePoint*>* l_sortedlist=(MyVector<IndexablePoint*>*)malloc(sizeof(MyVector<IndexablePoint*>)*(DIM_K-dim_lev));
		MyVector<IndexablePoint*>* r_sortedlist=(MyVector<IndexablePoint*>*)malloc(sizeof(MyVector<IndexablePoint*>)*(DIM_K-dim_lev));
		this->node_cnt=eles[0].size();
		this->range[0]=eles[0][0]->coords[this->dim_lev];
		this->range[1]=eles[0][this->node_cnt-1]->coords[this->dim_lev];
		//build child, down, make aux sorted list
		RTNode* lchild=new RTNode(dim_lev);
		RTNode* rchild=new RTNode(dim_lev);
		IndexablePoint* pivot=eles[0][(this->node_cnt+1)/2];
		for(int i=0;i<DIM_K-dim_lev;i++){
			l_sortedlist[i]=emptyVec;
			r_sortedlist[i]=emptyVec;

			l_sortedlist[i].reserve((this->node_cnt+1)/2);
			r_sortedlist[i].reserve(this->node_cnt-(this->node_cnt+1)/2);
			for(int j=0;j<this->node_cnt;j++){
				if (eles[i][j]->smaller(dim_lev,pivot)){
					l_sortedlist[i].push_back(eles[i][j]);
				}else{
					r_sortedlist[i].push_back(eles[i][j]);
				}
			}
		//	printf("%d %d %d %d %d\n", 
		//			this->node_cnt,(this->node_cnt+1)/2,this->node_cnt-(this->node_cnt+1)/2,
		//			l_sortedlist[0].size(),r_sortedlist[0].size());
		}
		lchild->batch_insert(l_sortedlist);
		rchild->batch_insert(r_sortedlist);
		this->weight_sum=lchild->weight_sum+rchild->weight_sum;
		child[0]=lchild;
		child[1]=rchild;

		for(int i=0;i<DIM_K-dim_lev;i++){
			l_sortedlist[i].release_space();
			r_sortedlist[i].release_space();
		}
		delete l_sortedlist;
		delete r_sortedlist;
	}
	if (dim_lev<DIM_K-1){
		subtree=new RTNode(dim_lev+1);
		subtree->batch_insert(eles+1);
	}

}

int RTNode::rangeCntQuery(int *q){
	if (disjoint(q)){
		return 0;
	}
	else if (inside(q)){
		if (this->dim_lev==DIM_K-1){
			return this->weight_sum;
		}
		return this->subtree->rangeCntQuery(q);
	}else{
		int ret=0;
		for(int i=0;i<2;i++){
			ret=ret+((RTNode*)(this->child[i]))->rangeCntQuery(q);
		}
		return ret;
	}


}

