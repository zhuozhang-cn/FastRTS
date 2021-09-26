#ifndef QUADTREE_LOGGEN_IO_H
#define QUADTREE_LOGGEN_IO_H
#include "QuadTreeLog.h"
#include "GloVars.h"
#include <algorithm>
class LogGen_IO {
public:
    static void ReadLogFromFile(MyVector<Query>& queryList);
	static void ReadOpFromFile(int increase, MyHugeVector<long long>& opList);
    static void WriteResultLineToFile(FILE* file, long long pID, MyVector<int>& alertIDList);

};


#endif //QUADTREE_LOGGEN_IO_H
