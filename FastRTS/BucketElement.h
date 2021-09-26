#ifndef QUADTREE_BUCKETELEMENT_H
#define QUADTREE_BUCKETELEMENT_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
class BucketElement {
public:
    /*
	 *  Here we use 14 bytes to store all the data member of HeapElement.
	 *  In the first 2 bytes, we store indexInQuery.
	 *  In the second 8 bytes, we store queryStartIndex.
	 *  In the last 4 bytes, we store emitCnt.
	 */
    char data[14];


    inline void setIndexInQuery(short indexInQuery) {
        *((short*) (data)) = indexInQuery;
    }

    inline int getIndexInQuery() {
        return *((short*) (data));

    }

    inline void setQueryStartIndex(long long queryStartIndex) {
        *((long long*) (data + 2)) = queryStartIndex;
    }

    inline long long getQueryStartIndex() {
        return *((long long *) (data + 2));
    }

    inline void setLastCnt(int emitCnt) {
        *((int*) (data + 10)) = emitCnt;
    }

    inline int getLastCnt() {
        return *((int*) (data + 10));
    }

    inline void ValueCopy(BucketElement& e) {
        memcpy(data, e.data, 14);
    }

    inline void swap(BucketElement& e) {
        char temp[14];
        memcpy(temp, this->data, 14);
        memcpy(this->data, e.data, 14);
        memcpy(e.data, temp, 14);


    }
};


#endif //QUADTREE_BUCKETELEMENT_H
