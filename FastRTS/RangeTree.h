#ifndef RANGETREE_H_
#define RANGETREE_H_
#include "RTNode.h"

class RangeTree{
public:
	RTNode* root;
	RangeTree();
	void insertElement(IndexablePoint* e);
	int rangeCntQuery(int* q);
	void releaseSpace();

};
#endif /* RANGETREE_H_ */
