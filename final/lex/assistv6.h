//
//  assistv6.h
//  lex
//
//  Created by Bruce Ho on 2021/5/15.
//

#ifndef assistv6_h
#define assistv6_h

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<string>


#define Z_ERROR 40
#define Z_NUM 11
#define Z_SPECIAL 12
#define Z_WORD 13
#define Z_others 14
#define Z_ID 15
#define Z_KEY 16

#define X_WORD 13
#define X_KEYWORD 130
#define X_ERROR_CHAR 131
#define X_ERROR_LENGTH 132
#define X_OUT 133


//夏

#define num_keyword 31          //关键字个数
const char * keyword[num_keyword];//更改
char   word[15];                //存放当前正在识别的单词字符串,长度10完全够用吧
int is_keyword();//判断当前单词是否是关键字
int store_in_buffer();  //把C中字符连接在word字符串后面
int analysis_word(); //处理字符
void X_set_keyword()            //初始化关键字表
{
    keyword[ 0] = "and";
    keyword[ 1] = "mod";
    keyword[ 2] = "div";

    keyword[ 3] = "or";

    keyword[ 4] = "not";
    keyword[ 5] = "array";
    keyword[ 6] = "begin";
    keyword[ 7] = "Boolean";
    keyword[ 8] = "case";
    keyword[ 9] = "const";
    keyword[10] = "do";
    keyword[11] = "downto";
    keyword[12] = "else";
    keyword[13] = "end";
    keyword[14] = "for";
    keyword[15] = "function";
    keyword[16] = "if";
    keyword[17] = "integer";
    keyword[18] = "of";
    keyword[19] = "procedure";
    keyword[20] = "program";
    keyword[21] = "real";
    keyword[22] = "record";
    keyword[23] = "repeat";
    keyword[24] = "then";
    keyword[25] = "to";
    keyword[26] = "type";
    keyword[27] = "until";
    keyword[28] = "var";
    keyword[29] = "while";
    keyword[30] = "char";
}

//张
#define Z_NUM_BUFFER 50
#define Z_SP_ERROR 30
#define Z_SP_OUT 31

int wordtype_num=0;
int wordtype_sp=0;
int wordtype_id=0;
int wordtype_key=0;
int wordnum=0;
int if_note=0;

typedef struct Z_marktable{
    int valuetype;
    char mark[30];
    char value[30];
    
}Z_MarkT;
// Z_MarkT * Ztemp;
Z_MarkT *tmark;

int is_letter=0;
int is_equal=0;
int is_end=0;
int is_const_or_type=0;
char C;//读取到的字符
int charnum;//读取到的字符数量
int state;//定义状态
char sp_array[18]={'+','-','*','/','(',')','<','=','>',';',':',',','[',']','{','}','.','\''};

char NUM_buffer[Z_NUM_BUFFER];//数字长度
char Z_lable[15];//二元组标签
int buffer_len=0;

void analysis();//词法分析主程序

void get_char();//读取字符
int Z_store_in_buffer(char ch);//储存字符串到缓冲区||addword()改名
void retract();//回退字符
int Z_MarkTable();//保存到文件

int Z_analy_num();//分析常量。下为子程序
int Z_special();//分析特殊符号
int Z_other_ch();//分析空格

int error();//错误
int Z_check_type();//检查类型

bool letter(char ch);
bool digit(char ch);
bool special(char ch);
bool others(char ch);
void change_state(char ch);//变更状态

void Initialize()
{
    return ;
}


#endif /* assistv6_h */
