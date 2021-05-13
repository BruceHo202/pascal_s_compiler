#ifndef ANALYSIS_H
#define ANALYSIS_H

#include"generateTable.h"
#include"def.h"

int give_tree_type(NODE* tree, typeNode* type);
void give_id_buffer(NODE* tree, int buffer, int& num, int period_num, _peroid* period);
bool comparetree(typeNode* t1, typeNode* t2);
void analyze(int lge_no, NODE* father, vector<NODE*> children);
void generateDataSpace();
void handle_array_or_record(int& ds_cur_size, int cur_line);

#endif
