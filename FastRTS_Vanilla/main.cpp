#include <iostream>
#include "GloVars.h"
#include "QuadTreeLog.h"
#include <random>
#include "LogGen_IO.h"
#include <unistd.h>

static normal_distribution<double> uniform(U/2.0,U/2.0*0.15);
static default_random_engine engine=default_random_engine(123);
int* random_range(){
    int* coord=(int*)malloc(DIM_K*2*sizeof(int));
    for(int i=0;i<DIM_K;++i){
        int mid,l1,l2;
        int l=U*0.1;
        do {
            mid = (int)uniform(engine);
            l1=mid-l/2;
            l2=l1+l;
        }while(l1<0 or l2>U);
        coord[2*i]=l1;
        coord[2*i+1]=l2;
    }
    return coord;
}
void random_point(int* coord){
    for(int i=0;i<DIM_K;++i){
        coord[i]=random()%U;
    }
}
int get_memory_by_pid(pid_t p)
{
    FILE *fd;
    char name[32], line_buff[256] = {0}, file[64] = {0};
    int i, vmrss = 0;


    sprintf(file, "/proc/%d/status", p);
    fd = fopen(file, "r");
    if(fd==NULL)
    {
        return -1;
    }

    for (i = 0; i < 40; i++)
    {
        if (fgets(line_buff, sizeof(line_buff), fd) == NULL)
        {
            break;
        }
        if (strstr(line_buff, "VmRSS:") != NULL)
        {
            sscanf(line_buff, "%s %d", name, &vmrss);
            break;
        }
    }

    fclose(fd);

    return vmrss;
}


int main(int nargs, char **args) {
    const double PER_M = 1024.0 * 1024;
    const double PER_K = 1024.0;
    MyVector<Query> queryList;
    MyHugeVector<long long> opList;
    FILE* ResultFile = fopen(args[2],"wt");
    printf("Start to read from log.\n");

	if (nargs==4){
		sscanf(args[3],"%lf",&GAMMA);
		printf("Gamma specified: %lf!\n",GAMMA);
	}
    fflush(stdout);
	file=fopen(args[1],"rt");	
    if (file == NULL) {
        printf(
                "Error in function ReadLogFromFile:\nFile open failed! Please check your file path!\n");
        exit(0);
    }
    fscanf(file, "%d\t%d\t%lld\t%d\t%d\t%d\n", &DIM_K, &qNumStart, &ptInsNum, &qInsNum,
			   &qDelNum,&U);

    QSN = qNumStart;

    PIN = ptInsNum;
    QIN = qInsNum;
    QDN = qDelNum;
	int increase=1000000;

    LogGen_IO::ReadLogFromFile( queryList);
	LogGen_IO::ReadOpFromFile(increase,opList);
    printf("Start to construct the structure.\n");
    printf("QueryList Size:%d\n",queryList.size());
    fflush(stdout);
	rho_epoch=(double*)malloc(sizeof(double)*EPO);
	double trho=RHO_S;
	printf("rho:");
	for (int x=EPO-1;x>=0;x--){
		printf("%lf,",trho);
		rho_epoch[x]=trho;
		trho=pow(trho,RHO_E);
	}
	printf("\n");
	rho_epoch[EPO-1]=RHO;
	int max_mem =0;
    // Construct the whole structure.
    double start = getCurrentTime();

    QuadTreeLog* quadTree = new QuadTreeLog();
	quadTree->init(queryList);

    double end = getCurrentTime();
    double duration = end - start;
    double constructionTime = end - start;

    printf("Construction done.\n");
    printf("Construction time = %lf seconds.\n", constructionTime);
    fflush(stdout);


	printf("tree node number:%d\n",quadTree->_treeNodeList.size());
    MyVector<int> alertIDList;
    int* ptCoords= new int[DIM_K];
    double pinTime=0;
    double qinTime=0;
    double qdnTime=0;
    int ptInsNum=0;
    int step=PIN/100;
    long long spaceCons;
	int piiid=getpid();
	bool no_end=true;
	while(no_end){
		printf("%lld\n",opList.size());
    long long opLength = opList.size();
    for (long long i = 0; i < opLength; ++i) {
		if (no_end==false){
			break;
		}
        if (opList[i] == -1) {
            // The end of the log resultFile.
            printf("log over\n");
			no_end=false;
            break;
        }
        else if (opList[i] == 1) {
            // Insertion of a point.
            long long pid = opList[++i];

            for (int j = 0; j < DIM_K; ++j) {
                ptCoords[j] = opList[++i];
            }
            int ptWeight = opList[++i];

            start = getCurrentTime();
            quadTree->InsertPoint(ptCoords, ptWeight, alertIDList);
            end = getCurrentTime();
            duration += end - start;
            pinTime += end - start;

            ptInsNum++;
            if(alertIDList.size()>0) {
                LogGen_IO::WriteResultLineToFile(ResultFile, pid, alertIDList);
                alertIDList.clear();
            }

            if (ptInsNum % step == 0) {
                printf("%d %%\t point insertions done.  Running time = %lf.\t \n", ptInsNum / step,  duration);
                printf("pinTime = %lf\n", pinTime);
                printf("qinTime = %lf\n", qinTime);
                printf("qdnTime = %lf\n", qdnTime);
				max_mem=max(max_mem,get_memory_by_pid(piiid));
				printf("memory: %d id:%d max:%d\n",get_memory_by_pid(piiid),piiid,max_mem);
                fflush(stdout);
            }
            if (quadTree->aliveNum == 0) {
                // All the queries have matured.
                // Output the space consumption to space file.
                printf("%d %%\t point insertions done.  Running time = %lf.\t \n", ptInsNum / step,  duration);
                fflush(stdout);
				no_end=true;
                break;
            }
        }
        else if (opList[i] == 2) {
            // Insertion of a query.
            int qid = opList[++i];
            int* range=new int[2*DIM_K];
            for (int j = 0; j < 2 * DIM_K; ++j) {
                range[j] = opList[++i];
            }
            int threshold = opList[++i];

            Query q(qid, range, threshold);
            start = getCurrentTime();
            quadTree->InsertQuery(q);
            end = getCurrentTime();
            duration += end - start;
            qinTime += end - start;
        }
        else if (opList[i] == 3) {
            // Deletion of a query.
            int qid = opList[++i];
            start = getCurrentTime();
            if(quadTree->DeleteQuery(qid)==false){
			}
            end = getCurrentTime();
            duration += end - start;
            qdnTime += end - start;
        }
        else {
            printf("opList[%lld] = %lld\n", i, opList[i]);
        }
    }
	if (no_end==false){
		break;
	}
	LogGen_IO::ReadOpFromFile(increase,opList);
	}
    LogGen_IO::WriteResultLineToFile(ResultFile, -1, alertIDList);
    fflush(ResultFile);
    fclose(ResultFile);
    FILE* final_file=fopen("./Result.txt","a");
    fprintf(final_file,"d\t%d\tqsn\t%d\tqin\t%d\tpin\t%d\ttau\t%d\trho\t%lf\tmax_d\t%d\tgamma\t%.3lf\tmem\t%d\ttt\t%lf\tpint\t%lf\tqint\t%lf\tqdnt\t%lf\tfile\t%s\n",DIM_K,QSN,QIN,PIN,0,RHO,MAX_D,GAMMA,max_mem,duration,pinTime,qinTime,qdnTime,args[1]);
}
