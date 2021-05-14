#ifndef DEF_H
#define DEF_H

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

#define _INTEGER 0
#define _CHAR 1
#define _REAL 2
#define _BOOLEAN 3

#define VALUE 0
#define ADDRESS 1

struct _peroid{
    int kind; // 1 integer, 2 char, 3 bool
    int i_start, i_end;
    char c_start, c_end;
    bool b_start, b_end;
};

struct marktable{
    char value[30];
    int pos; //指向后合并符号表的位置,pos为-1表示不用定义的东西，例如write
    int depth; //第几层
    int deviation; //偏移，该区域的第几个id
    int Belong;
    int buffer;
    _peroid period[100]; // 数组范围
    int period_num;
    int size;
    int value_or_var;
    int hav[60]; //包含的id
    int belongPos; //id属于的program、function、procedure、record
    int addr; // 入口指令地址
    int para_cnt;
    int data_space_ptr; // data_space地址
    int data_space_temp_ptr; // 临时地址
    int alloc_flag; // 已经分配地址
    int array_lenth; // 一组长度
    int var_part_num; // varpart数量
};

extern marktable mmMark[5500];
extern vector<string> I; // 指令寄存器
extern int P; // 程序计数器
extern int B; // 基地址寄存器
extern int T; // 栈指针寄存器

extern vector<vector<string> > pcode;
extern double data_space[100000][3];
extern int dataspace_len;

extern vector<int> calc_stack;
extern vector<int> runtime_stack;

extern int cur_level;

extern vector<double> que;
extern vector<double> que1;

#endif
