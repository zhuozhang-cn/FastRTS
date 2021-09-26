#include "IndexablePoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

IndexablePoint::IndexablePoint() {
	id = 0;
	coords = NULL;
	weight = 0;
}

IndexablePoint::IndexablePoint(int _id, int* _coords) {
	id = _id;
	coords = _coords;
}
IndexablePoint::IndexablePoint(int _id, int* _coords,int _weight) {
	id = _id;
	coords = _coords;
	weight=_weight;
}

IndexablePoint::~IndexablePoint() {
	// TODO Auto-generated destructor stub
//	releaseSpace();
}

void IndexablePoint::releaseSpace() {
	if (coords != NULL) {
		delete[] coords;
		coords = NULL;
	}
}

double IndexablePoint::computeSqrDist(int* _coords, int _dim) {
	double sqrDist = 0;
	long long temp = 0;
	for (int i = 0; i < _dim; ++i) {
		temp = coords[i];
		temp = temp - _coords[i];
		sqrDist += temp * temp;
	}
	return sqrDist;
}

double IndexablePoint::computeDist(int* _coords, int _dim) {
	return sqrt(computeSqrDist(_coords, _dim));
}

long long IndexablePoint::computeDotProduct(int* _coords, int _dim) {
	long long ans = 0;
	long long temp = 0;
	for (int i = 0; i < _dim; ++i) {
		temp = _coords[i];
		temp *= coords[i];
		ans += temp;
	}
	return ans;
}

long long IndexablePoint::computeDotProduct(long long* _coords, int _dim) {
	long long ans = 0;
	long long temp = 0;
	for (int i = 0; i < _dim; ++i) {
		temp = coords[i];
		ans += _coords[i] * temp;
	}
	return ans;
}

void IndexablePoint::showCoords(int _dim) {
	if (coords == NULL) {
		printf("Error in IndexablePoint::showCoords: coords == NULL!\n");
		return;
	}
	printf("%d", coords[0]);
	for (int j = 1; j < _dim; ++j) {
		printf(", %d", coords[j]);
	}
	printf("\n");
}

/*
 *  Check whether the coordinates of the two given points are the same.
 */
bool IndexablePointCoordsEqual(const IndexablePoint* p1, const IndexablePoint* p2, int dim) {
	for (int i = 0; i < dim; ++i) {
		if (p1->coords[i] != p2->coords[i])
			return false;
	}
	return true;
}

