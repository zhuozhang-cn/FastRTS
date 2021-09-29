#ifndef QUADTREE_BUCKETS_H
#define QUADTREE_BUCKETS_H
#include "BucketElement.h"
#include "MyVector.h"


class Buckets {
public:
	
	MyVector<MyVector<BucketElement>> buckList;
	
    Buckets();
    
	static MyVector<BucketElement> EmptyBucket;

	int listSize();
	int sizeByIndex(int i);
	
	void extendListSize(int size);

	void shrinkToFit();

	int InsertNewELement(int i,BucketElement& e);
	bool CheckEmptyByIndex(int index);
    void DeleteElement(int bucketIndex,int elementIndex,void *segTree_Log);
    ~Buckets();
    void ReleaseSpace();

};


#endif //QUADTREE_BUCKETS_H
