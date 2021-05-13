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
#define INUM 1050

#define VALUE_CALL 0
#define VAR_CALL 1

#define _INTEGER 0
#define _CHAR 1
#define _REAL 2
#define _BOOLEAN 3


struct typeNode{
    string type;
    vector<typeNode*> children;
    int pos;
};

struct Z_marktable{
    int kind; //1、2、3、5、6、7，共6种
    char mark[30]; //mark
    char value[30]; //value
    int pos; //指向后合并符号表的位置,pos为-1表示不用定义的东西，例如write
    int depth; //第几层
    int deviation; //偏移，该区域的第几个id
    int Belong;
    int size;
    int hav[60]; //包含的id
    int belongPos; //id属于的program、function、procedure、record
};


struct _peroid{
    int kind; // 1 integer, 2 char, 3 bool
    int i_start, i_end;
    char c_start, c_end;
    bool b_start, b_end;
};

struct marktable{
    int kind; //1、2、3、5、6、7，共6种
    char mark[30]; //mark
    char value[30]; //value
    int pos; //指向后合并符号表的位置,pos为-1表示不用定义的东西，例如write
    int depth; //第几层
    int deviation; //偏移，该区域的第几个id
    int Belong;
    typeNode* type;
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

struct outMMark{
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


//originMark， mergedMark只合并id， updatedMark


struct NODE{
    string _name;
    vector<NODE*> child;
    /* */
    typeNode* type;
    int intValue;
    char charValue;
    bool boolValue;
    double realValue;
    int pos;
    _peroid period[100];
    int period_num;
    int buffer;
    NODE* father;
    int para_cnt;
    int expr_num;
    int var_part_num; // varpart数量
    /* */
    
};

extern string table[INUM][100]; // 分析表

extern int lge_cnt; // 文法数量
extern string lge[100]; // 文法

extern int ch_num; // 单词数量
extern int end_num; // 终结符数量

extern string word[100]; // 单词表
extern string end_word[100]; // 终结符表
extern string _word[100]; //索引对应的单词
extern map<string, int> ch; // 单词哈希表
extern map<string, int> end_ch; // 终结符哈希表
extern map<string, vector<string> > first_hash; // first集哈希表

extern Z_marktable mMark[5500], uMark[5500];
extern marktable mmMark[5500], uuMark[5500];
extern outMMark outmark[5500];

extern vector<vector<string> > pcode;

extern double data_space[100000][3];
extern int dataspace_len;

extern string lex_path;
extern string lge_path;
extern string end_path;
extern string table_dir;
extern string mergeMarktable_dir;
extern string updateMarktable_dir;

extern string pcode_dir;
extern string pcodedat_dir;
extern string data_space_dir;

#endif
