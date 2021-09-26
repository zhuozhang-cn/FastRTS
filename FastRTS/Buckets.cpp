#include "Buckets.h"
#include "QuadTreeLog.h"

MyVector<BucketElement> Buckets::EmptyBucket;

void Buckets::DeleteElement(int bucketIndex, int elementIndex,void *QuadTree_Log) {
    QuadTreeLog *log = (QuadTreeLog *) QuadTree_Log;
	BucketElement& old_e = buckList[bucketIndex][elementIndex];
    MyVector<BucketElement> &bList = this->buckList[bucketIndex];
    if (bList.size() == 1) {
        bList.pop_back();
        //this->shrinkToFit();
		return;
    }
    int old_size = bList.size();
    BucketElement &swap_e = bList[old_size - 1];
    log->_indexInBucketList[swap_e.getQueryStartIndex() + swap_e.getIndexInQuery() + 1] = elementIndex;
    old_e.ValueCopy(swap_e);
    bList.pop_back();
    if (bList.size() < bList.capacity() * 0.5) {
        bList.shrink_to_fit();
    }
}

Buckets::~Buckets() {
	for(int i=0;i<buckList.size();++i){
		buckList[i].release_space();	
	}
	buckList.release_space();

}

int Buckets::listSize(){
	return buckList.size();
}

void Buckets::extendListSize(int index){
	while(buckList.size()<index){
		buckList.push_back(EmptyBucket);
	}
	return ;
}

int Buckets::InsertNewELement(int i, BucketElement& e){
	this->extendListSize(i+1);
	buckList[i].push_back(e);
	return buckList[i].size();
}

bool Buckets::CheckEmptyByIndex(int index) {
    if (buckList.size()<index){
        return true;
    }else if (buckList[index].size()==0){
		return true;
	}
    return false;

}

int Buckets::sizeByIndex(int i){
	if (buckList.size()<=i){
		return 0;
	}
	return buckList[i].size();
}

void Buckets::shrinkToFit(){
	int i=buckList.size()-1;
	while(buckList[i].size()==0){
		buckList.pop_back();
		i--;
	}
	return;
}

Buckets::Buckets() {


}

void Buckets::ReleaseSpace() {
	for(int i=0;i<buckList.size();++i){
		buckList[i].release_space();	
	}
	buckList.release_space();
}
