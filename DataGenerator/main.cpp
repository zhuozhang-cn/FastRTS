#include <cstring>
#include <algorithm>
#include <iostream>
#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <cstdint>
#include <string>
#include "MyHugeVector.h"
#include <cmath>
#define MAX_PATH_LEN 256

#ifdef WIN32
#define ACCESS(fileName,accessMode) _access(fileName,accessMode)
#define MKDIR(path) _mkdir(path)
#else
#define ACCESS(fileName,accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif
#include "ToolFunction.h"
#include <vector>
#include <random>
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
using namespace std;
int main(int nargs, char** args) {

    //The number of queries registered at the beginning.
    int qsn=0;
    //The number of queries to insert.
    int qin=0;
    //The number of points to insert.
    long long pin=-1;
    //The probability of whether to insert a query in any time step.
    double prob_in=-1;
    //Dimension
    int dim_k=-1;
    //Upper bound of data
    int U=10000000;
    //ratio of query range
    double scale=0.1;
    //Threshold
    int threshold=-1;
    //Weight parameter
    int w_mean=-1;
    double w_var;
    //File folder
    char file_folder[300]={0};
	//How many query keeps alive until it matures
	double dead_p=-1;
	
	int real_data=0;
	char real_file[300]={0};


    /*!
     * start reading input parameter
     */
    printf("The input parameters are:\n\n");
    bool failed=false;
    char* arg;
    int cnt=1;
    int index;
    char para[10];

    while(cnt<nargs && !failed){
        arg=args[cnt++];
        if (cnt==nargs){
            failed=true;
            break;
        }
        index=getNextChar(arg);
        if (arg[index]!='-'){
            failed=true;
            break;
        }
        getNextWord(arg+index+1,para);
        printf("%s\t",para);
        arg=args[cnt++];
        if (strcmp(para,"qsn")==0){
            qsn=atoi(arg);
            if (qsn<0){
                failed =true;
                break;
            }
            printf("%d\n",qsn);
        }else if (strcmp(para,"qin")==0){
            qin=atoi(arg);
            if (qin<0){
                failed =true;
                break;
            }
            printf("%d\n",qin);
        }else if (strcmp(para,"pin")==0){
            pin=atoi(arg);
            if(pin<=0){
                failed=true;
                break;
            }
            printf("%lld\n",pin);
        }else if (strcmp(para,"d")==0){
            dim_k=atoi(arg);
            if (dim_k<=0){
                failed =true;
                break;
            }
            printf("%d\n",dim_k);
        }else if (strcmp(para,"U")==0){
            U=atoi(arg);
            if (U<=0){
                failed =true;
                break;
            }
            printf("%d\n",U);
        } else if (strcmp(para,"t")==0){
            threshold=atoi(arg);
            if (threshold<=0){
                failed=true;
                break;
            }
            printf("%d\n",threshold);
		} else if (strcmp(para,"real")==0){
			real_data=atoi(arg);
			printf("%d\n",real_data);
		} else if (strcmp(para,"w")==0){
            w_mean=atoi(arg);
            if (w_mean<=0){
                failed=true;
                break;
            }
            printf("%d\n",w_mean);
        } else if (strcmp(para,"dp")==0){
            dead_p=atof(arg);
            if (dead_p<0 or dead_p>1){
                failed=true;
                break;
            }
            printf("%lf\n",dead_p);
        } else if (strcmp(para,"prob")==0){
            prob_in=atof(arg);
            if (prob_in<=0){
                failed=true;
                break;
            }
            printf("%lf\n",prob_in);
        }else if (strcmp(para,"s")==0){
            scale=atof(arg);
            if (scale<=0){
                failed=true;
                break;
            }
            printf("%lf\n",scale);
        }else if (strcmp(para, "lf") == 0) {
            getNextWord(arg, file_folder);
            printf("%s\n", file_folder);
        }else if (strcmp(para,"rf")==0){
			getNextWord(arg, real_file);
			printf("%s\n",real_file);
		} else {
            failed = true;
            printf("Unknown option -%s!\n\n", para);
        }
    }



		int line_num=0;
		float max_x=0,min_x=1000000;
		float max_y=0,min_y=1000000;
		float max_z=0,min_z=1000000;
		float x_mean=0;
		float y_mean=0;
	FILE* realdata_file=NULL;
	if (real_data>0){
		realdata_file=fopen(real_file,"r");
		if (realdata_file==NULL){
			printf("DATA FILE %s OPEN FILED!!\n",real_file);
			exit(1);
		}
		int tn;
		float tx,ty,tz;
		double sum_x=0;
		while(fscanf(realdata_file,"%d\t%f\t%f\t%f\t%f\t%f",&tn,&tx,&ty,&tz,&x_mean,&y_mean)>=0){
			max_x=max(max_x,tx);
			max_y=max(max_y,ty);
			max_z=max(max_z,tz);
			min_x=min(min_x,tx);
			min_y=min(min_y,ty);
			min_z=min(min_z,tz);
			sum_x+=tx;
			line_num++;
			if (line_num%100000==0){
				printf("%d %lf %lf %lf %lf %lf\n",line_num,sum_x/line_num,max_y,max_z,min_y,min_z);
			}
		}
		w_mean=int(sum_x/line_num);
		if (dim_k==1){
			U=(int)((max_y-min_y)*10+1);
		}
		else if (dim_k==2){
			U=(int)(max((max_y-min_y),(max_z-min_z))*10+1);
		}
		rewind(realdata_file);
	}

	if (prob_in<0){
		if(threshold<0 or w_mean<0 or scale<0){
			failed=true;
		}
		double exp_life=1.0*threshold/w_mean/scale;
		prob_in=1.0/exp_life*qin;	
	}
	else if (threshold<0){
		if (prob_in<0 or w_mean<0 or scale<0){
			failed=true;
		}
		double exp_life=1.0/prob_in*qsn;
		threshold=(int)(exp_life*w_mean*scale);
		
	}

	if (pin<0){
		pin=(long long)(1.0*threshold/w_mean/scale+1.0*qin/prob_in);
		printf("pin:%lld\n",pin);
	}
		if (real_data>0 and line_num<pin){
			printf("not enough point");
			exit(1);
		}


    if (failed) {
        //usage();
        printf("\n==================================================================\n");
        printf("The input parameters are incorrect! Please check them again!");
        printf("\n==================================================================\n");
        return 0;
    }
    printf("\n==================================================================\n");

    fflush(stdout);


    double length=U*pow(scale,1.0/dim_k);
    w_var=0.15*w_mean;
    char file_name[1000]={0};
    if (ACCESS(file_folder, 0) == -1){
        MKDIR(file_folder);
    }
    sprintf(file_name,"%s//d=%d_qsn=%d_qin=%d_pin=%lld_pro=%.2lf_tau=%d_w=%d_dp=%.2f_U=%d.log",
            file_folder,dim_k,qsn,qin,pin,prob_in,threshold,w_mean,dead_p,U);
    FILE* file=fopen(file_name,"wt");
    if(file==NULL){
        printf("FILE %s OPEN FILED!!\n",file_name);
        exit(1);
    }
    int* square=new int[2*dim_k];
    int* point=new int[dim_k];
    fprintf(file,"%d\t%d\t%lld\t%d\t%d\t%d\n",dim_k,qsn,pin,qin,0,U);

    MyHugeVector<DelEntry> delList;
    delList.reserve(qsn);
    double p=1-pow(dead_p,1.0/(1.0*threshold/w_mean/scale));//only keep xxx% query alive until it matures
	printf("p:%lf\n",p);
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::geometric_distribution<int> distribution(p);


    int delTimeStamp=0;
    Query qb;
    qb.range=square;

	int cent_n=1;
	int vvvar=0;
	int* cent=new int[dim_k*cent_n];
	for(int i=0;i<dim_k*cent_n;++i){
		cent[i]=U/2;
	}
	if (real_data>0){
		cent[0]=(int)((x_mean-min_y)*1.0/(max_y-min_y+1)*U);
		if (dim_k>1){
			cent[1]=(int)((y_mean-min_z)*1.0/(max_z-min_z+1)*U);
		}
	}
    for (int i = 0; i < qsn; ++i) {
		vvvar=(int)(length*0.15/pow(dim_k,0.5));
        GenGaussianSquare(dim_k, square, length, cent, vvvar, 1, U);

        qb.id = i + 1;
        qb.threshold = threshold;
        WriteQueryToFile(file, &qb,dim_k);
		if (dead_p>0.9999999){
			continue;
		}

		delTimeStamp = distribution(generator) + 1;
        DelEntry entry(delTimeStamp, i + 1);
        delList.push_back(entry);
    }

    long long num=pin+qin;
    long long ptID = 1;
    int qID = qsn + 1;

    MyHugeVector<int> opList;
    opList.reserve(num);

    long long i = 0;

    int insqnum=0;
    for (; i < pin; ++i) {
        // Insert a point.
        opList.push_back(1);
		int qin_num;
		if (prob_in>10){
			qin_num=(int)(prob_in);
		}else{
			qin_num=possion(prob_in);
		}
		if (i%100000==0){
			printf("%lld %lld %d\n",i,pin,qin_num);
		}

		while (qin_num>0 && insqnum<qin) {
            // Generate a query insertion.
            insqnum++;
            opList.push_back(2);
			if (dead_p>0.9999999){
				qID++;
				qin_num--;
				continue;
			}

			delTimeStamp = distribution(generator) + 1;
			delTimeStamp+=1;
            DelEntry entry(delTimeStamp, qID);
            delList.push_back(entry);
            qID++;
			qin_num--;
        }
    }


    sort(delList.elementList, delList.elementList+delList.elementNum);

    int w = 0;
    int t = 0;
    long long curPos = 0;

    qID = qsn + 1;
    num = opList.size();
    for (i = 0; i < num; ++i) {
        if (opList[i] == 1) {
			if (real_data<=0){
				GenUniformPoint(dim_k, point, 1, U);
				w = GenGaussianRandInt(w_mean, w_var);
			}else {
				int nn;
				float xx,yy,zz;
				if (fscanf(realdata_file,"%d\t%f\t%f\t%f\t%f\t%f",&nn,&xx,&yy,&zz,&x_mean,&y_mean)<0){
					printf("not enough point!!");
					exit(1);
				}
				w=int(xx);
				if (dim_k==1){
					point[0]=(int)((yy-min_y)*1.0*U/(max_y-min_y+1));
				}else if (dim_k==2){
					point[0]=(int)((yy-min_y)*U*1.0/(max_y-min_y+1));
					point[1]=(int)((zz-min_z)*U*1.0/(max_z-min_z+1));
				}

				cent[0]=(int)((x_mean-min_y)*1.0/(max_y-min_y+1)*U);
				if (dim_k>1){
					cent[1]=(int)((y_mean-min_z)*1.0/(max_z-min_z+1)*U);
				}
			}
            fprintf(file, "1\t%lld\t", ptID);
            for (int j = 0; j < dim_k; ++j) {
                fprintf(file, "%d\t", point[j]);
            }
            fprintf(file, "%d\n", w);

            while (curPos < delList.size() && ptID == delList[curPos].time) {
                fprintf(file, "3\t%d\n", delList[curPos].delID);
                curPos++;
            }

            ptID++;

        } else if (opList[i] == 2) {
			vvvar=(int)(length*0.15/pow(dim_k,0.5));
            GenGaussianSquare(dim_k, square, length, cent, vvvar, 1,
                              U);

            t = threshold;

            fprintf(file, "2\t");

            qb.id = qID;
            qb.threshold = t;

            WriteQueryToFile(file, &qb, dim_k);
            qID++;

        }
    }


    fprintf(file, "-1\n");

    if (square != NULL) {
        delete[] square;
        square = NULL;
    }

    if (point != NULL) {
        delete[] point;
        point = NULL;
    }

    //delList.release_space();

    fclose(file);
    printf("Log generated successfully:%s",file_name);

}
