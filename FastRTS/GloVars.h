#ifndef QUADTREE_GLOVARS_H
#define QUADTREE_GLOVARS_H

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
//using namespace std;
extern double RHO;
extern int EPO;
//extern int ERR;
extern int DIM_K;
//extern int CHD_N;
extern int U; //Define our question on [0,U]
extern int QSN;	//query number when start
extern int PIN;	//total element number
extern int QIN;	//insert query number
extern int QDN;	 
extern int MAX_D; //quad-tree structure
extern int qNumStart;
extern long long ptInsNum;
extern int qInsNum;
extern int qDelNum;

extern FILE* file;
extern double GAMMA; //eps in paper..

inline int min(int a,int b){
    return a>b?b:a;
}

inline int max(int a,int b){
    return a>b?a:b;
}

extern double* rho_epoch;

extern double RHO_E;
extern double RHO_S;
extern int EXA_THE;

static int pow_2[30]={1	,
                      2	,
                      4	,
                      8	,
                      16	,
                      32	,
                      64	,
                      128	,
                      256	,
                      512	,
                      1024	,
                      2048	,
                      4096	,
                      8192	,
                      16384	,
                      32768	,
                      65536	,
                      131072	,
                      262144	,
                      524288	,
                      1048576	,
                      2097152	,
                      4194304	,
                      8388608	,
                      16777216	,
                      33554432	,
                      67108864	,
                      134217728	,
                      268435456	,
                      536870912	};

double getCurrentTime();

#endif //QUADTREE_GLOVARS_H
