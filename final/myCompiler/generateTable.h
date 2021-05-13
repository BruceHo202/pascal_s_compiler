#ifndef GENERATETABLE_H
#define GENERATETABLE_H

#include"def.h"




string my_tostring(int k); //整数转成对应的字符串
string read_a_word(string s, int pos);
bool is_end(string s);
int word_to_index(string s);
string index_to_word(int k);
int _locate(string s, int format);
void init_ch();
void init_endch();
void input_lge();
int which_lge(string s);
vector<string> _merge(vector<string>a, vector<string> b);
vector<string> first(string s);
void expand(string expand_ch, vector<string>& I, string add);
int check_i_exist(vector<string> cur, vector<vector<string> > kernel);
void generate_an_i(vector<string> cur, vector<vector<string> >& I, vector<vector<string> >& kernel);
void generateTable();



#endif /* generateTable_h */
