//
// Created by 张卓 on 2021/2/26.
//

#ifndef QUADTREE_MYVECTOR_H
#define QUADTREE_MYVECTOR_H


#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "GloVars.h"



template<class T>
class MyVector {
public:
    MyVector();
    ~MyVector();

    /*
     *  Add the given element to the back.
     */
    void push_back(T const& _val);

    /*
     *  Remove the last element from the array.
     */
    void pop_back();

    /*
     *  Allocate memory to the array with specified length.
     */
    void reserve(unsigned int len);

    /*
     *	Resize the array to specified length.
     *	All the new elements are initialized by the given value.
     */
    void resize(unsigned int targetSize, const T& _val);

    /*
     *	Resize the array to specified length.
     */
    void resize(unsigned int targetSize);

    /*
     *  Shrink the array to fit the occupied size.
     */
    void shrink_to_fit();

    /*
     *  Swap the content with given vector.
     */
    void swap(MyVector<T>& _list);

    /*
     *  Clear elements in the array but do not modify its capacity.
     */
    void clear();

    /*
     *  Release the space of the array, namely the array will be deleted.
     */
    void release_space();

    /*
     *  Return the size of the array.
     */
    unsigned int size() const;

    /*
     *  Return the capacity of the array.
     */
    unsigned int capacity() const;

    /*
     *  Return the element list.
     */
    T* get_list() const;

    /*
     *  Return the element at the given index.
     */
    T& operator[](unsigned int i) const {
        if (i < this->elementNum)
            return this->elementList[i];
        else {
            printf(
                    "Error in MyVector accessing array element: Out of boundary! Accessing [%d] while size = %d and length = %d!\n",
                    i, elementNum, length);
            int* a=NULL;
			*a=3;
			
        }
    }

//protected:
public:
    /*
     *  The available length of this vector.
     */
    unsigned int length;

    /*
     *  The current number of elements.
     */
    unsigned int elementNum;

    /*
     *  The array of elements.
     */
    T* elementList;

    /*
     *  Enlarge the length of the array by a factor of 2.
     */
    void enlarge();
};

template<class T>
MyVector<T>::MyVector() {
    // TODO Auto-generated constructor stub
    this->elementList = NULL;
    this->elementNum = 0;
    this->length = 0;
}

template<class T>
MyVector<T>::~MyVector() {
    // TODO Auto-generated destructor stub
    // Important Note:
    // In this destructor, we do not release the elementList.
    // Please explicitly call the release_space function after you use.
}

template<class T>
T* MyVector<T>::get_list() const {
    return this->elementList;
}

template<class T>
unsigned int MyVector<T>::size() const {
    return this->elementNum;
}

template<class T>
unsigned int MyVector<T>::capacity() const {
    return this->length;
}

template<class T>
void MyVector<T>::push_back(T const& _val) {
    if (this->elementNum < this->length) {
        this->elementList[elementNum] = _val;
        elementNum++;
    } else {
        // Need to enlarge the length of the array.
        this->enlarge();
        this->elementList[elementNum] = _val;
        elementNum++;

    }
}

template<class T>
void MyVector<T>::pop_back() {
    if (this->elementNum > 0) {
        this->elementNum--;
    } else {
        printf("Error in MyVector<T>::pop_back: Out of boundary!\n");
        exit(0);
    }
}

template<class T>
void MyVector<T>::reserve(unsigned int len) {
    if (len > this->length) {
        T* temp = this->elementList;

        this->elementList = (T*) realloc(temp, sizeof(T) * len);
        if (this->elementList == NULL) {
            printf("Error in reserve: The reallocation is failed!\n");
            exit(0);
        }
        this->length = len;

    }
}

template<class T>
void MyVector<T>::resize(unsigned int targetSize, const T& _val) {
    if (this->elementNum >= targetSize) {
        // Shrink to targetSize.
        this->elementNum = targetSize;
    } else {
        if (this->length < targetSize) {
            // Need to enlarge the length of the array.
            this->reserve(targetSize);
        }
        unsigned int temp = this->elementNum;
        for (unsigned int i = temp; i < targetSize; ++i) {
            this->elementList[i] = _val;
            elementNum++;
        }
    }
}

template<class T>
void MyVector<T>::resize(unsigned int targetSize) {
    if (this->elementNum >= targetSize) {
        // Shrink to targetSize.
        this->elementNum = targetSize;
    } else {
        if (this->length < targetSize) {
            // Need to enlarge the length of the array.
            this->reserve(targetSize);
        }
        this->elementNum = targetSize;
    }
}

template<class T>
void MyVector<T>::shrink_to_fit() {
    if (this->elementNum == 0) {
        this->release_space();
        return;
    }

    if (this->elementNum < this->length) {
        T* temp = this->elementList;
        this->elementList = (T*) realloc(temp, sizeof(T) * this->elementNum);

        if (this->elementList == NULL) {
            // realloc is failed.
            printf("Error in shrink_to_fit: the reallocation is failed!\n");
            exit(0);
        }

        this->length = this->elementNum;
    }
}

template<class T>
void MyVector<T>::swap(MyVector<T>& _list) {
    unsigned int temp = this->elementNum;
    this->elementNum = _list.elementNum;
    _list.elementNum = temp;

    temp = this->length;
    this->length = _list.length;
    _list.length = temp;

    T* ptr = this->elementList;
    this->elementList = _list.elementList;
    _list.elementList = ptr;
}

template<class T>
void MyVector<T>::clear() {
    this->elementNum = 0;
}

template<class T>
void MyVector<T>::release_space() {
    this->elementNum = 0;
    this->length = 0;
    if (this->elementList != NULL) {
        free(this->elementList);
        this->elementList = NULL;
    }
}

template<class T>
void MyVector<T>::enlarge() {
    if (this->length == 0) {
        this->elementList = (T*) malloc(sizeof(T) * 2);
        this->length = 2;
    } else {
		if (this->length<=2147483648){
			this->reserve(this->length * 2);
		}else{
			printf("Too large to enlarge in MyVector!!!\n");
			exit(1);
		}
    }
}



#endif //QUADTREE_MYVECTOR_H
