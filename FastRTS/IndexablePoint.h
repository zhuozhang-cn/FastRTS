#ifndef INDEXABLEPOINT_H_
#define INDEXABLEPOINT_H_

/*
 *  A base class for the indexable points such as in R-tree and other data structures.
 */
class IndexablePoint {
public:
	int id;
	/*
	 *  The coordinates of this point.
	 */
	int* coords;
	int weight;
public:
	IndexablePoint();
	IndexablePoint(int _id, int* _coords, int _weight);
	IndexablePoint(int _id, int* _coords );
	~IndexablePoint();
	void releaseSpace();

	double computeSqrDist(int* _coords, int _dim);
	double computeDist(int* _coords, int _dim);
	long long computeDotProduct(int* _coords, int _dim);
	long long computeDotProduct(long long* _coords, int _dim);
	
	bool smaller(int dim,IndexablePoint* e2){
		if (this->coords[dim]<e2->coords[dim]){
			return true;
		}
		if (this->coords[dim]>e2->coords[dim]){
			return false;
		}
		return this->id<e2->id;
	}

	void showCoords(int _dim);
};

/*
 *  The tool for sorting points by coordinates.
 */
struct IndexablePointCoordsSorter {
	int dim;
	IndexablePointCoordsSorter(int _dim) :
			dim(_dim) {
	}
	bool operator()(const IndexablePoint* p1, const IndexablePoint* p2) {
		for (int i = 0; i < dim; ++i) {
			if (p1->coords[i] < p2->coords[i]) {
				return true;
			} else if (p1->coords[i] > p2->coords[i]) {
				return false;
			}
		}
		// p1 and p2 have the same coordinates.
		return p1->id < p2->id;
	}
};

/*
 *  The tool for sorting points by the specified dimension coordinates.
 */
struct PointDimSorter {
	int dim;
	int dimIndex; // in [0, dim-1]
	PointDimSorter(int _dim, int _dimLev) :
			dim(_dim), dimIndex(_dimLev - 1) {
	}
	bool operator()(const IndexablePoint* p1, const IndexablePoint* p2) {
		return p1->coords[dimIndex] < p2->coords[dimIndex];
	}
};

/*
 *  Check whether the coordinates of the two given points are the same.
 */
extern bool IndexablePointCoordsEqual(const IndexablePoint* p1, const IndexablePoint* p2, int dim);

#endif /* INDEXABLEPOINT_H_ */
