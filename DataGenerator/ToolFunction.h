#ifndef DATAGENERATOR_TOOLFUNCTION_H
#define DATAGENERATOR_TOOLFUNCTION_H
#include <cstdio>
struct Query{
    int id;
    int threshold;
    int* range;

};


struct DelEntry {
    int time;
    int delID;

    DelEntry() {
        this->time = 0;
        this->delID = 0;
    }

    DelEntry(int time, int delID) {
        this->time = time;
        this->delID = delID;
    }

    bool operator <(const DelEntry& e2) const {
        return this->time < e2.time;
    }
};
int possion(double lambda);
void getNextWord(char *str, char *word);
int GenRandInt(int rangeS, int rangeE);
int getNextChar(char *str);
void WriteQueryToFile(FILE* file,Query* query,int dim);
void GenGaussianSquare(int dim, int* targetPlace, int len, int* u , int sigma,
                       int rangeS, int rangeE);
void GenGaussianSquare(int dim, int* targetPlace, int len, int u, int sigma,
                       int rangeS, int rangeE);
double GenUniformRandom(double rangeStart, double rangeEnd);
void GenUniformPoint(int dim, int* targetPlace, int rangeS, int rangeE);
int GenGaussianRandInt(int u, int sigma);

#endif //DATAGENERATOR_TOOLFUNCTION_H
