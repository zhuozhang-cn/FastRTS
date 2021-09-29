#include "GloVars.h"
#include <cmath>


int DIM_K=2;
int Q_NUM=2000000;
int U=10000000;
int QSN=0;
int PIN=0;
int QIN=0;
int QDN=0;
int qNumStart=0;
long long ptInsNum=0;
int qInsNum=0;
int qDelNum=0;
int EPO=1;
int MAX_D=1;

double RHO=0;
double RHO_E=1;
double RHO_S=0;
double* rho_epoch=NULL;

FILE* file=NULL;
double GAMMA=0.05;
int EXA_THE=5000;

static struct timeval tim;
double getCurrentTime() {
    gettimeofday(&tim, nullptr);
    return tim.tv_sec + (tim.tv_usec / (double) CLOCKS_PER_SEC);
}
