#include "ToolFunction.h"
#include <iostream>
#include <cmath>
int possion(double lambda){
	int k=0;
	long double p=1;
	long double L=exp(-1*lambda);
	while(p>=L){
		double u=GenUniformRandom(0,1);
		p*=u;
		k++;
	}
	return k-1;
}

void getNextWord(char *str, char *word) {
    // Jump over all blanks
    while (*str == ' ') {
        str++;
    }

    while (*str != ' ' && *str != '\0') {
        *word = *str;
        str++;
        word++;
    }

    *word = '\0';
}
int getNextChar(char *str) {
    int rtn = 0;

    // Jump over all blanks
    while (str[rtn] == ' ') {
        rtn++;
    }

    return rtn;
}
void WriteQueryToFile(FILE* file, Query* query, int dim) {
    if (file == NULL) {
        printf("Error in function WriteQueryToFile:\nFile handle is NULL!\n");
        exit(1);
    }

    // Output qid.
    fprintf(file, "%d", query->id);

    // Output the query range.
    for (int i = 0; i < 2 * dim; i++) {
        fprintf(file, "\t%d", query->range[i]);
    }
    // Output threshold.
    fprintf(file, "\t%d", query->threshold);

    fprintf(file, "\n");
}
double GenUniformRandom(double rangeStart, double rangeEnd) {
    if (rangeStart > rangeEnd) {
        printf(
                "Error in genUniformRandom: the rangeEnd is < the rangeStart!\n");
        exit(1);
    }
    double r;
    r = rangeStart
        + ((rangeEnd - rangeStart) * (double) random() / (double) RAND_MAX);
    if (r < rangeStart || r > rangeEnd) {
        printf("Error in genUniformRandom: the number generated is not in the given range!\n");
        exit(1);
    }
    return r;
}

double GenGaussianRandom() {
    // Use Box-Muller transform to generate a point from normal
    // distribution.
    double x1, x2;
    do {
        x1 = GenUniformRandom(0.0, 1.0);
    } while (x1 == 0); // cannot take log of 0.
    x2 = GenUniformRandom(0.0, 1.0);
    double z;
    z = sqrt(-2.0 * log(x1)) * cos(2.0 * M_PI * x2);
    return z;
}


double GenGaussianRandom(double u, double sigma) {
    // Generate a real from N(0, 1).
    double x = GenGaussianRandom();
    x *= sigma;
    x += u;
    return x;
}

int GenGaussianRandInt(int u, int sigma) {
    double x = GenGaussianRandom(u, sigma);
    return (int) (x + 0.5);
}


void GenGaussianSquare(int dim, int* targetPlace, int len, int* u, int sigma,
                       int rangeS, int rangeE) {


    int off=dim*(GenRandInt(0,10)%10);
	off=0;

	int coords[dim];
    int qLen_Half = len / 2;
    for (int j = 0; j < dim; ++j) {

        coords[j] = GenGaussianRandInt(u[j+off], sigma);
        while (coords[j] - qLen_Half < rangeS
               || coords[j] + qLen_Half > rangeE) {
            coords[j] = GenGaussianRandInt(u[j+off], sigma);
        }

        targetPlace[2 * j] = coords[j] - qLen_Half;
        targetPlace[2 * j + 1] = coords[j] + qLen_Half;
    }



}

void GenGaussianSquare(int dim, int* targetPlace, int len, int u, int sigma,
                       int rangeS, int rangeE) {

    int coords[dim];
    int qLen_Half = len / 2;
    for (int j = 0; j < dim; ++j) {


        coords[j] = GenGaussianRandInt(u, sigma);
        while (coords[j] - qLen_Half < rangeS
               || coords[j] + qLen_Half > rangeE) {
            coords[j] = GenGaussianRandInt(u, sigma);
        }

        targetPlace[2 * j] = coords[j] - qLen_Half;
        targetPlace[2 * j + 1] = coords[j] + qLen_Half;
    }

}
int GenRandInt(int rangeS, int rangeE) {
    return (int) (rangeS + ((double) rand() / RAND_MAX) * (rangeE - rangeS));
}
void GenUniformPoint(int dim, int* targetPlace, int rangeS, int rangeE) {

    for (int i = 0; i < dim; ++i) {
        targetPlace[i] = GenRandInt(rangeS, rangeE);
        while (targetPlace[i] < rangeS || targetPlace[i] > rangeE) {
            targetPlace[i] = GenRandInt(rangeS, rangeE);
        }
    }

}
