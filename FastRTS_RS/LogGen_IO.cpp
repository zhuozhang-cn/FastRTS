#include <fstream>
#include "LogGen_IO.h"
#include "stdio.h"


void LogGen_IO::ReadLogFromFile(MyVector<Query> &queryList) {
    
    queryList.clear();
    queryList.reserve(qNumStart);

    int query_id;
    int* range;
    int tempInt = 0;
    int tmpThreshold;
	int dim=DIM_K;

    for (int i = 0; i < qNumStart; ++i) {
        fscanf(file, "%d\t", &query_id);

        range = new int[2 * dim];

        for (int j = 0; j < 2 * dim; ++j) {
            fscanf(file, "%d\t", &tempInt);
            range[j] = tempInt;
        }
        fscanf(file, "%d\n", &tmpThreshold);

        queryList.push_back(Query(query_id,range,tmpThreshold));
    }

}

void LogGen_IO::ReadOpFromFile(int increase, MyHugeVector<long long>& opList){
	long long tempInt=-1;
	opList.clear();
	for (int i=0;i<increase;i++){
		fscanf(file,"%lld",&tempInt);
		if (tempInt==-1){
			opList.push_back(tempInt);
			return;
		}
		opList.push_back(tempInt);
		if (tempInt==1){
			for (int j=0;j<DIM_K+2;j++){
				fscanf(file,"%lld",&tempInt);
				opList.push_back(tempInt);
			}
		}else if (tempInt==2){
			for (int j=0;j<DIM_K*2+2;j++){
				fscanf(file,"%lld",&tempInt);
				opList.push_back(tempInt);
			}
		}else if (tempInt==3){
				fscanf(file,"%lld",&tempInt);
				opList.push_back(tempInt);
		}
	}

} 


void LogGen_IO::WriteResultLineToFile(FILE *file, long long pID, MyVector<int> &alertIDList) {
    if (file == NULL) {
        printf("Error in function WriteResultLineToFile:\nFile handle is NULL!\n");
    }
    if (pID==-1){
        fprintf(file,"-1\n");
        return;
    }

    // 0 means the start of a new line.
    fprintf(file, "0\t");
    fprintf(file, "%lld", pID);

    int idNum = (int)alertIDList.size();

    // sort the id's.
    sort(alertIDList.get_list(), alertIDList.get_list() + idNum);

    for (int i = 0; i < idNum; ++i) {
        fprintf(file, "\t%d", alertIDList[i]);
    }
    fprintf(file, "\n");
}


