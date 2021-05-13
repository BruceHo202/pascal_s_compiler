#include "analysis.h"



double data_space[100000][3];
int dataspace_len;

Z_marktable mMark[5500], uMark[5500];
marktable mmMark[5500], uuMark[5500];

int give_tree_type(NODE* tree, typeNode* type){
    int ans = 0;
    if((int)tree->child.size() == 0){
        tree->type = type;
        
        if(tree->_name == "id"){
            int _pos = tree->pos;
            uuMark[_pos].type = type;
            int ppos = uuMark[_pos].pos;
            mmMark[ppos].type = type;
            ans = 1;
        }
        
        return ans;
    }
    for(int i=0;i<(int)tree->child.size();i++){
        ans += give_tree_type(tree->child[i], type);
    }
    tree->type = type;
    return ans;
}

void give_id_buffer(NODE* tree, int buffer, int& num, int period_num, _peroid* period){
    if((int)tree->child.size() == 0){
        
        if(tree->_name == "id"){
            int _pos = tree->pos;
            int ppos = uuMark[_pos].pos;
            mmMark[ppos].buffer = buffer;
            num += 1;
            mmMark[ppos].period_num = period_num;
            for(int i = 0; i < period_num; i++){
                mmMark[ppos].period[i] = period[i];
            }
            
        }
            
        return ;
    }
    for(int i=0;i<(int)tree->child.size();i++){
        give_id_buffer(tree->child[i], buffer, num, period_num, period);
    }
    
}

bool comparetree(typeNode* t1, typeNode* t2){
    if(t1 && t2){
        if(t1->children.size() != t2->children.size()) return false;
        if(!(t1->type == t2->type)) return false;
        for(int i = 0; i < t1->children.size(); i++){
            if(!comparetree(t1->children[i], t2->children[i])) return false;
            
        }
    }
    else return false;
    return true;
}

void add_var_or_value_flag(NODE* tree, int flag, int _cnt){
    if((int)tree->child.size() == 0){
        
        if(tree->_name == "id"){
            int _pos = tree->pos;
            int ppos = uuMark[_pos].pos;
            mmMark[ppos].value_or_var = flag;
            _cnt++;
        }
            
        return ;
    }
    for(int i=0;i<(int)tree->child.size();i++){
        add_var_or_value_flag(tree->child[i], flag, _cnt);
    }
}

void analyze(int lge_no, NODE* father, vector<NODE*> children){
    switch(lge_no){
        default:
        {
            return ;
        }
        case 0:
        {
            break;
        }
        case 1:
        {
            break;
        }
        case 2: // program_head->program id (identifier_list);
        {
            
            break;
        }
        case 3:
        {
            break;
        }
        case 4: // identifier_list->identifier_list,id
        {
            
            break;
        }
        case 5: // identifier_list->id
        {
            
            break;
        }
        case 6:
        {
            break;
        }
        case 7:
        {
            break;
        }
        case 8:
        {
            break;
        }
        case 9:
        {
            break;
        }
        case 10: // const_variable->+id
        {
            typeNode* tt = new typeNode;
            int _pos = children[1]->pos;
            int ppos = uuMark[_pos].pos;
            tt = mmMark[ppos].type;
            if(tt->type == "function" || tt->type == "procedure"){
                if(tt->children[1]->type != "integer" && tt->children[1]->type != "real"){
                    puts("const leixing cuowu error!!!!!!");
                    exit(0);
                }
                else{
                    tt= tt->children[1];
                }
            }
            else if(tt->type != "integer" && tt->type != "real"){
                puts("const leixing cuowu error!!!!!!");
                exit(0);
            }
            father->type = tt;
            
            string value;
            _pos = children[1]->pos;
            ppos = uuMark[_pos].pos;
            value = mmMark[ppos].value;
            if(tt->type == "real"){
                father->realValue = atof(value.c_str());
            }
            if(tt->type == "integer"){
                father->intValue = atoi(value.c_str());
            }
            
            break;
        }
        case 11: // const_variable->-id
        {
            typeNode* tt = new typeNode;
            int _pos = children[1]->pos;
            int ppos = uuMark[_pos].pos;
            tt = mmMark[ppos].type;
            if(tt->type == "function" || tt->type == "procedure"){
                if(tt->children[1]->type != "integer" && tt->children[1]->type != "real"){
                    puts("const leixing cuowu error!!!!!!");
                    exit(0);
                }
                else{
                    tt = tt->children[1];
                }
            }
            else if(tt->type != "integer" && tt->type != "real"){
                puts("const leixing cuowu error!!!!!!");
                exit(0);
            }
            father->type = tt;
            
            string value;
            _pos = children[1]->pos;
            ppos = uuMark[_pos].pos;
            value = mmMark[ppos].value;
            if(tt->type == "real"){
                father->realValue = -atof(value.c_str());
            }
            if(tt->type == "integer"){
                father->intValue = -atoi(value.c_str());
            }
            
            break;
        }
        case 12: // const_variable->id
        {
            typeNode* tt = new typeNode;
            int _pos = children[0]->pos;
            int ppos = uuMark[_pos].pos;
            tt = mmMark[ppos].type;
            father->type = tt;
            
            string value;
            _pos = children[0]->pos;
            ppos = uuMark[_pos].pos;
            value = mmMark[ppos].value;
            if(tt->type == "real"){
                father->realValue = atof(value.c_str());
            }
            if(tt->type == "integer"){
                father->intValue = atoi(value.c_str());
            }
            if(tt->type == "char"){
                father->charValue = value[0];
            }
            if(tt->type == "Boolean"){
                if(value == "true" || value == "True")
                    father->boolValue = true;
                else if(value == "false" || value =="False")
                    father->boolValue = false;
                else{
                    puts("type err1\n");
                    exit(1);
                }
            }
            
            break;
        }
        case 13: // const_variable->+num
        {
            typeNode* newTypeNode = new typeNode;
            int _pos = children[1]->pos;
            char numch[30];
            memcpy(numch, uuMark[_pos].value, sizeof(numch));
            bool tag = false;
            for(int i = 0; i < strlen(numch); i++){
                if(numch[i] == '.' || numch[i] == 'E'){
                    tag = true;
                    break;
                }
            }
            if(tag){
                newTypeNode->type = "real";
                father->realValue = atof(numch);
            }
            else {
                newTypeNode->type = "integer";
                father->intValue = atoi(numch);
            }
            father->type = newTypeNode;
            
            break;
        }
        case 14: // const_variable->-num
        {
            typeNode* newTypeNode = new typeNode;
            int _pos = children[1]->pos;
            char numch[30];
            memcpy(numch, uuMark[_pos].value, sizeof(numch));
            bool tag = false;
            for(int i = 0; i < strlen(numch); i++){
                if(numch[i] == '.' || numch[i] == 'E'){
                    tag = true;
                    break;
                }
            }
            if(tag){
                newTypeNode->type = "real";
                father->realValue = -atof(numch);
            }
            else {
                newTypeNode->type = "integer";
                father->intValue = -atoi(numch);
            }
            father->type = newTypeNode;
            break;
        }
        case 15: // const_variable->num
        {
            typeNode* newTypeNode = new typeNode;
            int _pos = children[0]->pos;
            char numch[30];
            memcpy(numch, uuMark[_pos].value, sizeof(numch));
            bool tag = false;
            for(int i = 0; i < strlen(numch); i++){
                if(numch[i] == '.' || numch[i] == 'E'){
                    tag = true;
                    break;
                }
            }
            if(tag){
                newTypeNode->type = "real";
                father->realValue = atof(numch);
            }
            else {
                newTypeNode->type = "integer";
                father->intValue = atoi(numch);
            }
            father->type = newTypeNode;
            break;
        }
        case 16: // const_variable->'letter'
        {
            typeNode* newTypeNode = new typeNode;
            newTypeNode->type = "char";
            father->type = newTypeNode;
            int _pos = children[1]->pos;
            father->charValue = uuMark[_pos].value[0];
            break;
        }
        case 17:
        {
            break;
        }
        case 18:
        {
            break;
        }
        case 19: // type_declaration->type_declaration;id=type0
        {
            typeNode* tt = children[0]->type;
            tt->children.push_back(children[4]->type);
//            children[2]->type = children[4]->type;
//            //********************//
//            tt = children[2]->type;
//            while(tt->children.size() != 0){
//                cout<<tt->type<<" ";
//                tt = tt->children[0];
//            }
            break;
        }
        case 20: // type_declaration->id=type0
        {
            typeNode* tt = new typeNode;
            tt->type = "NULL";
            tt->children.push_back(children[2]->type);
            father->type = tt;
            children[0]->type = children[2]->type;
            break;
        }
        case 21: // type0->standard_type
        {
            father->type = children[0]->type;
            father->buffer = children[0]->buffer;
            break;
        }
        case 22: // type0->record record_body end
        {
            father->type = children[1]->type;
            father->buffer = children[1]->buffer;
            break;
        }
        case 23: // type0->array[periods] of type0
        {
            typeNode* tt =new typeNode;
            tt->type = "array";
            tt->children.push_back(children[5]->type);
            father->type = tt;
            
            memcpy(father->period, children[2]->period, sizeof(children[2]->period));
            father->period_num = children[2]->period_num;
            
            int temp = 1;
            for(int i = father->period_num-1; i >= 0;i--){
                if(father->period[i].kind == 1){
                    temp *= (father->period[i].i_end - father->period[i].i_start + 1);
                    
                }
                if(father->period[i].kind == 2){
                    temp *= (father->period[i].c_end - father->period[i].c_start + 1);
                    
                }
                if(father->period[i].kind == 3){
                    temp *= (father->period[i].b_end - father->period[i].b_start + 1);
                    
                }
            }
            father->buffer = temp * children[5]->buffer;
            break;
        }
        case 24: // standard_type->integer
        {
            typeNode* newTypeNode = new typeNode;
            newTypeNode->type = "integer";
            children[0]->type = newTypeNode;
            father->type = children[0]->type;
            father->buffer = 1;
            break;
        }
        case 25: // standard_type->real
        {
            typeNode* newTypeNode = new typeNode;
            newTypeNode->type = "real";
            children[0]->type = newTypeNode;
            father->type = children[0]->type;
            father->buffer = 1;
            break;
        }
        case 26: // standard_type->Boolean
        {
            typeNode* newTypeNode = new typeNode;
            newTypeNode->type = "Boolean";
            children[0]->type = newTypeNode;
            father->type = children[0]->type;
            father->buffer = 1;
            break;
        }
        case 27: // standard_type->char
        {
            typeNode* newTypeNode = new typeNode;
            newTypeNode->type = "char";
            children[0]->type = newTypeNode;
            father->type = children[0]->type;
            father->buffer = 1;
            break;
        }
        case 28: // record_body->var_declaration
        {
            typeNode* tt = children[0]->type;
            tt->type = "record";
            father->type = tt;
            
            father->buffer = children[0]->buffer;
            
            break;
        }
        case 29: // record_body->
        {
            typeNode* tt = new typeNode;
            tt->type = "record";
            father->type = tt;
            father->buffer = 0;
            break;
        }
        case 30: // periods->periods,period
        {
            father->period_num = children[0]->period_num + children[2]->period_num;
            memcpy(father->period, children[0]->period, sizeof(children[0]->period));
            int p_num = father->period_num;
            father->period[p_num-1] = children[2]->period[0];
            break;
        }
        case 31: // periods->period
        {
            father->type = children[0]->type;
            father->intValue = children[0]->intValue;
            father->charValue = children[0]->charValue;
            father->boolValue = children[0]->boolValue;
            father->realValue = children[0]->realValue;
            memcpy(father->period, children[0]->period, sizeof(children[0]->period));
            father->period_num = children[0]->period_num;
            break;
        }
        case 32: // period->const_variable..const_variable
        {
            if(children[0]->type->type != children[3]->type->type){
                puts("type err2\n");
                exit(1);
            }
            if(children[0]->type->type != "integer"
               && children[0]->type->type != "char"
               && children[0]->type->type != "Boolean"){
                puts("type err3\n");
                exit(1);
            }
            if(children[3]->type->type != "integer"
               && children[3]->type->type != "char"
               && children[3]->type->type != "Boolean"){
                puts("type err4\n");
                exit(1);
            }
            
            typeNode* tt = new typeNode;
            
            
            if(children[0]->type->type == "integer"){
                father->period_num = 1;
                tt->type = "integer";
                father->type = tt;
                father->period[0].i_start = children[0]->intValue;
                father->period[0].i_end = children[3]->intValue;
                father->period[0].kind = 1;
                if(children[0]->intValue > children[3]->intValue){
                    puts("array declaration err!!!!!\n");
                    exit(1);
                }
            }
            if(children[0]->type->type == "char"){
                father->period_num = 1;
                tt->type = "char";
                father->type = tt;
                father->period[0].c_start = children[0]->charValue;
                father->period[0].c_end = children[3]->charValue;
                father->period[0].kind = 2;
                if(children[0]->charValue > children[3]->charValue){
                    puts("array declaration err!!!!!\n");
                    exit(1);
                }
            }
            if(children[0]->type->type == "Boolean"){
                father->period_num = 1;
                tt->type = "Boolean";
                father->type = tt;
                father->period[0].b_start = children[0]->boolValue;
                father->period[0].b_end = children[3]->boolValue;
                father->period[0].kind = 3;
                if(children[0]->boolValue > children[3]->boolValue){
                    puts("array declaration err!!!!!\n");
                    exit(1);
                }
            }
            
            
            break;
        }
        case 33: // var_declarations->var var_declaration;
        {
            
            break;
        }
        case 34:
        {
            break;
        }
        case 35: // var_declaration->var_declaration;identifier_list:type0
        {
            typeNode* tt = children[0]->type;
            tt->children.push_back(children[4]->type);
            father->type = tt;
            
            give_tree_type(children[2], children[4]->type);
            //********************//
//            tt = children[2]->type;
//            while(tt->children.size() != 0){
//                cout<<tt->type<<" ";
//                tt = tt->children[0];
//            }
            
            int nnnum = 0;
            give_id_buffer(children[2], children[4]->buffer, nnnum, children[4]->period_num, children[4]->period);
            father->buffer = children[0]->buffer + nnnum * children[4]->buffer;
            
            break;
        }
        case 36: // var_declaration->identifier_list:type0
        {
            typeNode* tt = new typeNode;
            tt->type = "NULL";
            tt->children.push_back(children[2]->type);
            father->type = tt;
            
            give_tree_type(children[0], children[2]->type);
            //********************//
//            tt = children[0]->type;
//            while(tt->children.size() != 0){
//                cout<<tt->type<<" ";
//                tt = tt->children[0];
//            }
            
            int nnnum = 0;
            give_id_buffer(children[0], children[2]->buffer, nnnum, children[2]->period_num, children[2]->period);
            father->buffer = nnnum * children[2]->buffer;
            
            break;
        }
        case 37:
        {
            break;
        }
        case 38:
        {
            break;
        }
        case 39:
        {
            break;
        }
        case 40: // subprogram_head->function id formal_parameter:standard_type;
        {
            typeNode* tt = new typeNode;
            tt->type = "function";
            tt->children.push_back(children[2]->type);
            tt->children.push_back(children[4]->type);
            children[1]->type = tt;
            father->type = tt;
            
            int _pos = children[1]->pos;
            uuMark[_pos].type = children[1]->type;
            int ppos = uuMark[_pos].pos;
            mmMark[ppos].type = children[1]->type;
            
            mmMark[ppos].para_cnt = children[2]->para_cnt;
            break;
        }
        case 41: // subprogram_head->procedure id formal_parameter;
        {
            typeNode* tt = new typeNode;
            tt->type = "procedure";
            tt->children.push_back(children[2]->type);
            typeNode* ttt = new typeNode;
            ttt->type = "NULL";
            tt->children.push_back(ttt);
            children[1]->type = tt;
            father->type = tt;
            
            int _pos = children[1]->pos;
            uuMark[_pos].type = children[1]->type;
            int ppos = uuMark[_pos].pos;
            mmMark[ppos].type = children[1]->type;
            
            mmMark[ppos].para_cnt = children[2]->para_cnt;
            break;
        }
        case 42: // formal_parameter->(parameter_lists)
        {
            father->type = children[1]->type;
            father->para_cnt = children[1]->para_cnt;
            break;
        }
        case 43: // formal_parameter->
        {
            typeNode* tt = new typeNode;
            tt->type = "NULL";
            father->type = tt;
            
            father->para_cnt = 0;
            break;
        }
        case 44: // parameter_lists->parameter_lists;parameter_list
        {
            typeNode* tt = children[0]->type;
            for(int i = 0; i <(int)children[2]->type->children.size(); i++){
                tt->children.push_back(children[2]->type->children[i]);
            }
            father->type = tt;
            
            father->para_cnt = children[0]->para_cnt + children[2]->para_cnt;
            break;
        }
        case 45: // parameter_lists->parameter_list
        {
//            typeNode* tt = new typeNode;
//            tt->type = "NULL";
//            tt->children.push_back(children[0]->type);
            father->type = children[0]->type;
            
            father->para_cnt = children[0]->para_cnt;
            break;
        }
        case 46: // parameter_list->var_parameter
        {
            father->type = children[0]->type;
            int _cnt = 0;
            add_var_or_value_flag(children[0], VAR_CALL, _cnt);
            father->para_cnt = _cnt;
            break;
        }
        case 47: // parameter_list->value_parameter
        {
            int _cnt = 0;
            father->type = children[0]->type;
            add_var_or_value_flag(children[0], VALUE_CALL, _cnt);
            father->para_cnt = _cnt;
            break;
        }
        case 48: // var_parameter->var value_parameter
        {
            father->type = children[1]->type;
            
            break;
        }
        case 49: // value_parameter->identifier_list:standard_type
        {
//            father->type = children[2]->type;
            typeNode* tt = new typeNode;
            tt->type = "NULL";
            
            int times = give_tree_type(children[0], children[2]->type);
            for(int i = 0; i < times; i++){
                tt->children.push_back(children[2]->type);
            }
            father->type = tt;
            int nn = 0;
            give_id_buffer(children[0], children[2]->buffer, nn, children[2]->period_num, children[2]->period);
            
            break;
        }
        case 50:
        {
            break;
        }
        case 51:
        {
            break;
        }
        case 52:
        {
            break;
        }
        case 53: // statement->variable assignop expression
        {
            if(children[0]->type->type != children[2]->type->type){
                if(children[0]->type->type == "real"&& children[2]->type->type == "integer"){
                    break;
                }
                puts("assignop fuzhi error!!!!!!!");
                exit(0);
            }
            break;
        }
        case 54: // statement->call_procedure_statement
        {
            
            break;
        }
        case 55: // statement->compound_statement
        {
            break;
        }
        case 56: // statement->if expression then statement else_part
        {
            if(children[1]->type->type != "Boolean"){
                puts("if expression type error!!!!!!!");
                exit(0);
            }
            break;
        }
        case 57: // statement->case expression of case_body end
        {
            if(children[3]->type->type == "GOOD"){
                break;
            }
            else{
                if(children[1]->type->type != children[3]->type->type){
                    puts("case leixing buyizhi error!!!!!!");
                    exit(0);
                }
            }
            break;
        }
        case 58: // statement->while expression do statement
        {
            if(children[1]->type->type != "Boolean"){
                puts("while expression type error!!!!!!!");
                exit(0);
            }
            break;
        }
        case 59: // statement->repeat statement_list until expression
        {
            if(children[3]->type->type != "Boolean"){
                puts("while expression type error!!!!!!!");
                exit(0);
            }
            break;
        }
        case 60: // statement->for id assignop expression updown expression do statement
        {
            int _pos = children[1]->pos;
            int ppos = uuMark[_pos].pos;
            string idtype;
            idtype = mmMark[ppos].type->type;
            if(idtype != children[3]->type->type){
                puts(" for assignop fuzhi error!!!!!!!");
                exit(0);
            }
            if(children[3]->type->type != children[5]->type->type){
                puts("for expression error!!!!!!!");
                exit(0);
            }
            break;
        }
        case 61:
        {
            break;
        }
        case 62: // variable->id id_varparts
        {
             
            typeNode* _type;
            int _pos = children[0]->pos;
            int ppos = uuMark[_pos].pos;
            _type = mmMark[ppos].type;
            if(children[1]->type->type == "NULL" ){
                children[0]->type = _type;
                if(_type->type == "function" || _type->type == "procedure" ){
                    father->type = _type->children[1];
                }
                else{
                    father->type = _type;
                }
                
            }
            else if(children[1]->type->type == "array"){
                children[0]->type = _type->children[0];
                father->type = _type->children[0];
            }
            else{
                father->type = children[1]->type;
            }
            
            father->var_part_num = children[1]->var_part_num;
            mmMark[ppos].var_part_num = father->var_part_num;
            break;
        }
        case 63: // id_varparts->id_varparts id_varpart
        {
            father->type = children[1]->type;
            
            father->var_part_num = children[0]->var_part_num + children[1]->var_part_num;
            break;
        }
        case 64: // id_varparts->
        {
            typeNode* tt = new typeNode;
            tt->type = "NULL";
            father->type = tt;
            father->var_part_num = 0;
            break;
        }
        case 65: // id_varpart->[expression_list]
        {
            typeNode* tt = new typeNode;
            tt->type = "array";
            father->type = tt;
            
            father->var_part_num = 1;
            
            break;
        }
        case 66: // id_varpart->.id
        {
            typeNode* _type;
            int _pos = children[1]->pos;
            int ppos = uuMark[_pos].pos;
            _type = mmMark[ppos].type;
            children[1]->type = _type;
            father->type = _type;
            
            father->var_part_num = 1;
            break;
        }
        case 67:
        {
            break;
        }
        case 68:
        {
            break;
        }
        case 69: // case_body->branch_list
        {
            father->type = children[0]->type;
            break;
        }
        case 70: // case_body->
        {
            typeNode* tt = new typeNode;
            tt->type = "GOOD";
            father->type = tt;
            break;
        }
        case 71: // branch_list->branch_list;branch
        {
            if(children[0]->type->type != children[2]->type->type){
                puts("case branch leixing buyizhi error!!!!!!");
                exit(0);
            }
            else{
                father->type = children[0]->type;
            }
            break;
        }
        case 72: // branch_list->branch
        {
            father->type = children[0]->type;
            break;
        }
        case 73: // branch->const_list:statement
        {
            father->type= children[0]->type;
            break;
        }
        case 74: // const_list->const_list,const_variable
        {
            if(children[0]->type->type != children[2]->type->type){
                puts("case list leixing buyizhi error!!!!!!");
                exit(0);
            }
            else{
                father->type = children[0]->type;
            }
            break;
        }
        case 75: // const_list->const_variable
        {
            father->type = children[0]->type;
            break;
        }
        case 76: // updown->to
        {
            break;
        }
        case 77: // updown->downto
        {
            
            break;
        }
        case 78: // call_procedure_statement->id
        {
            typeNode* tt = new typeNode;
            int _pos = children[0]->pos;
            char idname[30];
            memcpy(idname, uuMark[_pos].value, sizeof(idname));
            if(strcmp(idname, "write") == 0 ||
               strcmp(idname, "writeln") == 0 ||
               strcmp(idname, "read") == 0 ||
               strcmp(idname, "readln") == 0){
                tt->type = "NULL";
                father->type = tt;
                break;
            }
            int ppos = uuMark[_pos].pos;
            tt =  mmMark[ppos].type->children[1];
            father->type = tt;
            break;
        }
        case 79: // call_procedure_statement->id(expression_list)
        {
            typeNode* tt = new typeNode;
            int _pos = children[0]->pos;
            char idname[30];
            memcpy(idname, uuMark[_pos].value, sizeof(idname));
            if(strcmp(idname, "write") == 0 || strcmp(idname, "writeln") == 0 || strcmp(idname, "read") == 0 ||
               strcmp(idname, "readln") == 0){
                tt->type = "NULL";
                father->type = tt;
                break;
            }
            int ppos = uuMark[_pos].pos;
            tt = mmMark[ppos].type;
            if(!(comparetree(tt->children[0], children[2]->type))){
                puts("hanshu canshu error!!!!!!");
                exit(0);
            }
            father->type = tt->children[1];
            break;
        }
        case 80: // expression_list->expression_list,expression
        {
            typeNode* tt = children[0]->type;
            tt->children.push_back(children[2]->type);
            father->type = tt;
            father->expr_num = children[0]->expr_num + 1;
            break;
        }
        case 81: // expression_list->expression
        {
            typeNode* tt = new typeNode;
            tt->type = "NULL";
            tt->children.push_back(children[0]->type);
            father->type = tt;
            father->expr_num = 1;
            break;
        }
        case 82: // expression->simple_expression relop simple_expression
        {
            typeNode* tt = new typeNode;
            tt->type = "Boolean";
            father->type = tt;
            string s1 = children[0]->type->type;
            string s2 = children[2]->type->type;
            if(s1 != s2){
                if(!((s1 == "integer" && s2 == "real") || (s1 == "real" && s2 == "integer"))){
                    puts("relop liangce leixing bufu :error!!!!");
                    exit(0);
                }
            }
            break;
        }
        case 83: // expression->simple_expression
        {
            father->type = children[0]->type;
            break;
        }
        case 84: // simple_expression->term
        {
            father->type = children[0]->type;
            break;
        }
        case 85: // simple_expression->+term
        {
            father->type = children[1]->type;
            break;
        }
        case 86: // simple_expression->-term
        {
            father->type = children[1]->type;
            break;
        }
        case 87: // simple_expression->simple_expression addop term
        {
            char ch[30];
            int _pos = children[1]->pos;
            memcpy(ch, uuMark[_pos].value, sizeof(ch));
//            puts(ch);
            if(strcmp(ch,"+") ==0  || strcmp(ch,"-") == 0){
                if(children[0]->type->type != "integer" && children[0]->type->type != "real") {
                    puts("error1!!!!!!");
                    exit(0);
                }
                if(children[2]->type->type != "integer" && children[2]->type->type != "real") {
                    puts("error2!!!!!!");
                    exit(0);
                }
                if(children[0]->type->type == "integer" && children[2]->type->type == "integer"){
                    typeNode* tt = new typeNode;
                    tt->type = "integer";
                    father->type = tt;
                }
                else{
                    typeNode* tt = new typeNode;
                    tt->type = "real";
                    father->type = tt;
                }
            }
            else{
                if(children[0]->type->type == "integer" && children[2]->type->type == "integer"){
                    typeNode* tt = new typeNode;
                    tt->type = "integer";
                    father->type = tt;
                }
                else if(children[0]->type->type == "Boolean" && children[2]->type->type == "Boolean"){
                    typeNode* tt = new typeNode;
                    tt->type = "Boolean";
                    father->type = tt;
                }
                else{
                    puts("error3!!!!!!");
                    exit(0);
                }
            }
            break;
        }
        case 88: //term->term mulop factor
        {
            char ch[30];
            int _pos = children[1]->pos;
            memcpy(ch, uuMark[_pos].value, sizeof(ch));
//            puts(ch);
            if(strcmp(ch,"*") ==0  || strcmp(ch,"/") == 0){
                if(children[0]->type->type != "integer" && children[0]->type->type != "real") {
                    puts("error4!!!!!!");
                    exit(0);
                }
                if(children[2]->type->type != "integer" && children[2]->type->type != "real") {
                    puts("error5!!!!!!");
                    exit(0);
                }
                if(children[0]->type->type == "integer" && children[2]->type->type == "integer"){
                    typeNode* tt = new typeNode;
                    tt->type = "integer";
                    father->type = tt;
                }
                else{
                    typeNode* tt = new typeNode;
                    tt->type = "real";
                    father->type = tt;
                }
            }
            else if(strcmp(ch,"div") == 0 || strcmp(ch,"mod") == 0){
                if(children[0]->type->type == "integer" && children[2]->type->type == "integer"){
                    typeNode* tt = new typeNode;
                    tt->type = "integer";
                    father->type = tt;
                }
                else{
                    puts("error6!!!!!!");
                    exit(0);
                }
            }
            else{
                if(children[0]->type->type == "integer" && children[2]->type->type == "integer"){
                    typeNode* tt = new typeNode;
                    tt->type = "integer";
                    father->type = tt;
                }
                else if(children[0]->type->type == "Boolean" && children[2]->type->type == "Boolean"){
                    typeNode* tt = new typeNode;
                    tt->type = "Boolean";
                    father->type = tt;
                }
                else{
                    puts("error7!!!!!!");
                    exit(0);
                }
            }
            break;
        }
        case 89: // term->factor
        {
            father->type = children[0]->type;
            break;
        }
        case 90: //factor->unsign_const_variable
        {
            father->type = children[0]->type;
            break;
        }
        case 91: //factor->variable
        {
            
            father->type = children[0]->type;
            break;
        }
        case 92: //factor->id(expression_list)
        {
            typeNode* _type;
            int _pos = children[0]->pos;
            int ppos = uuMark[_pos].pos;
            _type = mmMark[ppos].type;
            children[0]->type = _type;
            father->type = _type->children[1];
            if(!(comparetree(children[0]->type->children[0], children[2]->type))) {printf("error8!!!!!!"); exit(0);}
            break;
        }
        case 93: //factor->(expression)
        {
            father->type = children[1]->type;
            break;
        }
        case 94: // factor->not factor
        {
            if(children[1]->type->type != "integer" &&
               children[1]->type->type != "Boolean"){
                printf("type error!!!!!!\n");
                exit(0);
            }
            father->type = children[1]->type;
            
            break;
        }
        case 95: // unsign_const_variable->num
        {
            typeNode* newTypeNode = new typeNode;
            int _pos = children[0]->pos;
            char numch[30];
            memcpy(numch, uuMark[_pos].value, sizeof(numch));
            bool tag = false;
            for(int i = 0; i < strlen(numch); i++){
                if(numch[i] == '.' || numch[i] == 'E'){
                    tag = true;
                    break;
                }
            }
            if(tag){
                newTypeNode->type = "real";
                father->realValue = atof(uuMark[_pos].value);
            }
            else {
                newTypeNode->type = "integer";
                father->intValue = atoi(uuMark[_pos].value);
            }
            father->type = newTypeNode;
            break;
        }
        case 96: // unsign_const_variable->'letter'
        {
            typeNode* newTypeNode = new typeNode;
            newTypeNode->type = "char";
            father->type = newTypeNode;
            
            int _pos = children[1]->pos;
            father->charValue = uuMark[_pos].value[0];
            break;
        }
    }
}

void handle_array_or_record(int& ds_cur_size, int cur_line){
    mmMark[cur_line].data_space_ptr = ds_cur_size;
    int temp_ds_cur_size = ds_cur_size;
    if(mmMark[cur_line].size == 0){
        ds_cur_size++;
        
    }
    else{
        for(int i=1;i<=mmMark[cur_line].size;i++){
            int temp = mmMark[cur_line].hav[i];
            if(mmMark[temp].type->type == "integer"){
                mmMark[temp].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _INTEGER;
                mmMark[temp].alloc_flag = 1;
            }
            else if(mmMark[temp].type->type == "real"){
                mmMark[temp].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _REAL;
                mmMark[temp].alloc_flag = 1;
            }
            else if(mmMark[temp].type->type == "char"){
                mmMark[temp].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _CHAR;
                mmMark[temp].alloc_flag = 1;
            }
            else if(mmMark[temp].type->type == "Boolean"){
                mmMark[temp].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _BOOLEAN;
                mmMark[temp].alloc_flag = 1;
            }
            else if(mmMark[temp].type->type == "array" ||
                    mmMark[temp].type->type == "record"){
                handle_array_or_record(ds_cur_size, temp);
            }
        }
    }
    
    mmMark[cur_line].array_lenth = ds_cur_size - temp_ds_cur_size;
    
    int times = mmMark[cur_line].buffer / mmMark[cur_line].array_lenth;
    for(int i = 0; i < times; i++){
        for(int j = 0; j < mmMark[cur_line].array_lenth ; j++){
            data_space[temp_ds_cur_size + i * mmMark[cur_line].array_lenth + j][1] = data_space[temp_ds_cur_size + j][1];
        }
    }
    
    ds_cur_size = mmMark[cur_line].buffer + temp_ds_cur_size;
    mmMark[cur_line].alloc_flag = 1;
    
    
}

void generateDataSpace(){
    int ds_cur_size = 0;
    for(int i=2;i<=mMark[0].Belong;i++){
        if(mmMark[i].type == NULL){
            mmMark[i].data_space_ptr = ds_cur_size;
            data_space[ds_cur_size++][1] = _REAL;
            mmMark[i].alloc_flag = 1;
            continue;
        }
        if(mmMark[i].buffer == 0 && mmMark[i].type->type == "function" && mmMark[i].alloc_flag == 0){
            if(mmMark[i].type->children[1]->type == "integer"){
                mmMark[i].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _INTEGER;
                mmMark[i].alloc_flag = 1;
            }
            else if(mmMark[i].type->children[1]->type == "real"){
                mmMark[i].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _REAL;
                mmMark[i].alloc_flag = 1;
            }
            else if(mmMark[i].type->children[1]->type == "char"){
                mmMark[i].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _CHAR;
                mmMark[i].alloc_flag = 1;
            }
            else if(mmMark[i].type->children[1]->type == "Boolean"){
                mmMark[i].data_space_ptr = ds_cur_size;
                data_space[ds_cur_size++][1] = _BOOLEAN;
                mmMark[i].alloc_flag = 1;
            }
        }
        if(mmMark[i].buffer == 0 || mmMark[i].alloc_flag == 1)
            continue;
        if(mmMark[i].type->type == "integer"){
            mmMark[i].data_space_ptr = ds_cur_size;
            data_space[ds_cur_size++][1] = _INTEGER;
            mmMark[i].alloc_flag = 1;
        }
        else if(mmMark[i].type->type == "real"){
            mmMark[i].data_space_ptr = ds_cur_size;
            data_space[ds_cur_size++][1] = _REAL;
            mmMark[i].alloc_flag = 1;
        }
        else if(mmMark[i].type->type == "char"){
            mmMark[i].data_space_ptr = ds_cur_size;
            data_space[ds_cur_size++][1] = _CHAR;
            mmMark[i].alloc_flag = 1;
        }
        else if(mmMark[i].type->type == "Boolean"){
            mmMark[i].data_space_ptr = ds_cur_size;
            data_space[ds_cur_size++][1] = _BOOLEAN;
            mmMark[i].alloc_flag = 1;
        }
        else if(mmMark[i].type->type == "array" ||
                mmMark[i].type->type == "record"){
            handle_array_or_record(ds_cur_size, i);
        }
    }
    dataspace_len = ds_cur_size;
    
}
