#include "RangeTree.h"

RangeTree::RangeTree(){
	root=NULL;
}

void RangeTree::insertElement(IndexablePoint* e){
	if (root==NULL){
		root=new RTNode(0);
	}
	root->insertElement(e);
}

int RangeTree::rangeCntQuery(int *q){
	if (root==NULL){
		return 0;
	}
	return root->rangeCntQuery(q);
}

void RangeTree::releaseSpace(){
	if (root!=NULL){
		//root->ReleaseSpace();
		delete root;
		root=NULL;
	}
}


