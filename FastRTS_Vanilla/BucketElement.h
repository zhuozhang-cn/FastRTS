#ifndef QUADTREE_BUCKETELEMENT_H
#define QUADTREE_BUCKETELEMENT_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
class BucketElement {
public:
    /*
	 *  Here we use 26 bytes to store all the data member of HeapElement.
	 *  In the first 2 bytes, we store indexInQuery.
	 *  In the second 8 bytes, we store queryStartIndex.
	 *  In the last 8 bytes, we store emitCnt.
	 */
    char data[18];


    inline void setIndexInQuery(short indexInQuery) {
//		memcpy(data + 3, (char*) &indexInQuery, 3);
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
        memcpy(data, e.data, 18);
    }

    /*
     *  Swap the data of this heap element with that of the given heap element.
     */
    inline void swap(BucketElement& e) {
        char temp[18];
        memcpy(temp, this->data, 18);
        memcpy(this->data, e.data, 18);
        memcpy(e.data, temp, 18);


    }
};


#endif //QUADTREE_BUCKETELEMENT_H
