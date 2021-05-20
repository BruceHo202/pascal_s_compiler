#ifndef MERGEASSIST_H
#define MERGEASSIST_H

#include<iostream>
#include<iomanip>
#include<vector>
#include<map>
#include<queue>
#include<string>
#include<string.h>
#include<algorithm>
#include<time.h>
#include<fstream>
using namespace std;

#define programType 1
#define functionType 2
#define procedureType 3
#define recordType 4
#define constType 5
#define typeType 6
#define varType 7
#define idType 8
#define beginType 9
#define endType 10
#define otherType 11
#define caseType 12

struct Z_marktable{
    int kind; //1、2、3、5、6、7，共6种
    char mark[30]; //mark
    char value[30]; //value
    int pos; //指向后合并符号表的位置,pos为-1表示不用定义的东西，例如write
    int depth; //第几层
    int deviation; //偏移，该区域的第几个id
    int Belong; //属于哪个定义,名字等价
    int size; //program、function专属，里面声明了多少变量，包含传参
    int hav[60]; //包含的id
    int belongPos; //id属于的program、function、procedure、record
}oMark[5500], mMark[5500], uMark[5500], mmMark[5500];
//originMark， mergedMark只合并id， updatedMark

int oMarkNum = 0; //originMark的数量
int mMarkNum = 0; //mergedMark的数量
int uMarkNum = 0; //updatedMark的数量

struct WorkSpace{
    int Begin, End; //这个工作区的符号表起始位置和结束位置
    int depth; //第几层
    int kind; //哪种类型的，program，function，procedure
    int root; //父层
    int id;//编号，无大用
    WorkSpace(){};
    WorkSpace(int bb, int ee, int dd, int kk, int rr, int ii){
        Begin = bb;
        End = ee;
        depth = dd;
        kind = kk;
        root = rr;
        id = ii;
    }
    bool operator < (const WorkSpace & a)const{
        if(depth != a.depth) return depth < a.depth;
        return Begin < a.Begin;
    }
}workSpace[55000];
int workSpaceNum;

int Belong[55000]; //每一行属于哪个代码段

void Inititalize(); //初始化

void Store(char*, int); //将读入存储到oMark中

void ReadMarkTable(); //从文件读入符号表

int GetType(char *ss, char * s);//类型判断,s :program 1，function 2，procedure 3 ... ss : id

void CodeLayer(); //代码分层

int FindPos(int x); //第x行是个id，找这个id第一次声明的位置，如果没有，返回-1

bool noDefId(char *s);//判断s是不是不需要定义的id，如write

void ReplaceConstTypeAndMerge(); //const、type替换，符号表合并

#endif
