#include"generatePCode.h"

const string LIT = "LIT";
const string LOD = "LOD";
const string STO = "STO";
const string CAL = "CAL";
const string INT = "INT";
const string JMP = "JMP";
const string JPC = "JPC";
const string OPR = "OPR";
const string ARR = "ARR";
const string LODA = "LODA";
const string STOA = "STOA";

vector<vector<string> > pcode;
int cur_level = 0;

void GENCODE::handle_id_varpart(NODE* tree){
    
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "["){
        handle_expr_list(tree->child[1]);
        int expr_num = tree->child[1]->expr_num;
        
        data_space[dataspace_len][0] = expr_num;
        data_space[dataspace_len][1] = _INTEGER;
        
        tt_code[0] = ARR;
        tt_code[1] = "0";
        tt_code[2] = to_string(dataspace_len++);
        
        pcode.push_back(tt_code);
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "2";
        pcode.push_back(tt_code);
        
    }
    else if(tree->child[0]->_name == "."){
        int _pos = tree->child[1]->pos;
        int ppos = uuMark[_pos].pos;
        
        int num = mmMark[ppos].deviation - 1;
//        cout<<"=========="<<num<<" "<<mmMark[ppos].value<<endl;
        data_space[dataspace_len][0] = num;
        data_space[dataspace_len][1] = _INTEGER;
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(dataspace_len++);
        pcode.push_back(tt_code);
        
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "2";
        pcode.push_back(tt_code);
        
        if(mmMark[ppos].type->type == "array"){
            for(int i = 1; i < mmMark[ppos].period_num; i++){
                int len = 0;
                if(mmMark[ppos].period[i].kind == 1){
                    len = mmMark[ppos].period[i].i_end - mmMark[ppos].period[i].i_start + 1;
                }
                else if(mmMark[ppos].period[i].kind == 2){
                    len = mmMark[ppos].period[i].c_end - mmMark[ppos].period[i].c_start + 1;
                }
                else if(mmMark[ppos].period[i].kind == 3){
                    len = mmMark[ppos].period[i].b_end - mmMark[ppos].period[i].b_start + 1;
                }
                
                data_space[dataspace_len][0] = len;
                data_space[dataspace_len][1] = _INTEGER;
                
                tt_code[0] = LIT;
                tt_code[1] = "0";
                tt_code[2] = to_string(dataspace_len++);
                pcode.push_back(tt_code);
                
            }
            int arr_len = mmMark[ppos].array_lenth;
         //   cout<<"========="<<arr_len<<" "<<mmMark[ppos].value<<endl;
            data_space[dataspace_len][0] = arr_len;
            data_space[dataspace_len][1] = _INTEGER;
            
            tt_code[0] = LIT;
            tt_code[1] = "0";
            tt_code[2] = to_string(dataspace_len++);
            pcode.push_back(tt_code);
            return ;
        }
        
        
    }
}

void GENCODE:: handle_id_varparts(NODE* tree){
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    if((int)tree->child.size() == 0){
        return ;
    }
    if((int)tree->child.size() == 2){
        handle_id_varparts(tree->child[0]);
        handle_id_varpart(tree->child[1]);
        
        return ;
    }
}

void GENCODE:: handle_variable(NODE* tree){
    int _pos = tree->child[0]->pos;
    int ppos = uuMark[_pos].pos;
//    cout<<mmMark[ppos].value<<endl;
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    int addr = mmMark[ppos].data_space_ptr;
    data_space[dataspace_len][0] =  addr;
    data_space[dataspace_len][1] = _INTEGER;
    data_space[dataspace_len][2] = 1;
    tt_code[0] = LIT;
    tt_code[1] = "0";
    tt_code[2] = to_string(dataspace_len++);
    pcode.push_back(tt_code);
    
    if(mmMark[ppos].type->type == "array"){
        for(int i = 1; i < tree->child[0]->period_num; i++){
            int len = 0;
            if(tree->child[0]->period[i].kind == 1){
                len = tree->child[0]->period[i].i_end - tree->child[0]->period[i].i_start + 1;
            }
            else if(tree->child[0]->period[i].kind == 2){
                len = tree->child[0]->period[i].c_end - tree->child[0]->period[i].c_start + 1;
            }
            else if(tree->child[0]->period[i].kind == 3){
                len = tree->child[0]->period[i].b_end - tree->child[0]->period[i].b_start + 1;
            }
            
            data_space[dataspace_len][0] = len;
            data_space[dataspace_len][1] = _INTEGER;
            
            tt_code[0] = LIT;
            tt_code[1] = "0";
            tt_code[2] = to_string(dataspace_len++);
            pcode.push_back(tt_code);
            
        }
        int arr_len = mmMark[ppos].array_lenth;
        
        data_space[dataspace_len][0] = arr_len;
        data_space[dataspace_len][1] = _INTEGER;
        
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(dataspace_len++);
        pcode.push_back(tt_code);
    }
    
    handle_id_varparts(tree->child[1]);
    
    tt_code[0] = "LODA";
    tt_code[1] = "0";
    tt_code[2] = "1";
    tt_code[3] = "";
    pcode.push_back(tt_code);
    
}

void GENCODE::handle_call_procedure_stat(NODE *tree){
    vector<string> tt_code;
    
    for(int i=0;i<4;i++){
        tt_code.push_back("");
    }
    
    if(strcmp(uuMark[tree->child[0]->pos].value, "write") == 0){
        if((int)tree->child.size() == 1){
            return ;
        }
        handle_expr_list(tree->child[2]);
        if((int)tree->child.size() == 1){
            return ;
        }
        int expr_num = tree->child[2]->expr_num;
        data_space[dataspace_len][0] = expr_num;
        data_space[dataspace_len][1] = _INTEGER;
        
        for(int i = 0; i < expr_num; i++){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "14";
            pcode.push_back(tt_code);
        }
        tt_code[0] = "QUE";
        tt_code[1] = "1";
        tt_code[2] = to_string(dataspace_len++);
        tt_code[3] = "";
        pcode.push_back(tt_code);
        return ;
    }
    else if(strcmp(uuMark[tree->child[0]->pos].value, "writeln") == 0){
        if((int)tree->child.size() == 1){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "15";
            pcode.push_back(tt_code);
            return ;
        }
        handle_expr_list(tree->child[2]);
        int expr_num = tree->child[2]->expr_num;
        
        data_space[dataspace_len][0] = expr_num;
        data_space[dataspace_len][1] = _INTEGER;
        for(int i = 0; i < expr_num; i++){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "14";
            pcode.push_back(tt_code);
            
        }
        
        tt_code[0] = "QUE";
        tt_code[1] = "1";
        tt_code[2] = to_string(dataspace_len++);
        tt_code[3] = "";
        pcode.push_back(tt_code);
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "15";
        pcode.push_back(tt_code);
        
        return ;
    }
    else if(strcmp(uuMark[tree->child[0]->pos].value, "read") == 0 ||
            strcmp(uuMark[tree->child[0]->pos].value, "readln") == 0){
  //      cout<<"nihaonihaonihaonihaonihoanihao"<<endl;
        handle_expr_list(tree->child[2]);
        int expr_num = 0;
        if((int)tree->child.size() == 1) return ;
        expr_num = tree->child[2]->expr_num;
        data_space[dataspace_len][0] = expr_num;
        data_space[dataspace_len][1] = _INTEGER;
        
        for(int i = 0; i < expr_num; i++){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "16";
            pcode.push_back(tt_code);
            
//            tt_code[0] = STOA;
//            tt_code[1] = "0";
//            tt_code[2] = "1";
//            pcode.push_back(tt_code);
            
        }
        tt_code[0] = "QUE";
        tt_code[1]= "0";
        tt_code[2] = to_string(dataspace_len++);
        tt_code[3] = "";
        pcode.push_back(tt_code);
        return ;
    }
    
    int _pos = tree->child[0]->pos;
    int ppos = uuMark[_pos].pos;
    
    int expr_num;
    if((int)tree->child.size() == 1)
        expr_num = 0;
    else
        expr_num = tree->child[2]->expr_num;
    
    int buf[expr_num + 2][2];
    
    if((int)tree->child.size() == 4){
        
        
        
        for(int i = 1; i <= expr_num; i++){
            int p_ppos = mmMark[ppos].hav[i];
            int val_ptr = mmMark[p_ppos].data_space_ptr;
            double val = data_space[val_ptr][0];
            int val_type = data_space[val_ptr][1];
            data_space[dataspace_len][0] = val;
            data_space[dataspace_len][1] = val_type;
            buf[i][0] = dataspace_len++;
            
            buf[i][1] = mmMark[p_ppos].value_or_var;
            if(buf[i][1] == VALUE_CALL){
                tt_code[0] = LODA;
                tt_code[1] = "1";
                tt_code[2] = to_string(p_ppos);
                pcode.push_back(tt_code);
            }
        }
        handle_expr_list(tree->child[2]);
        
        for(int i = expr_num; i >= 1; i--){
            tt_code[0] = "STO";
            int p_ppos = mmMark[ppos].hav[i];
//            int dep = -mmMark[p_ppos].depth + cur_level + 1;
//            int dev = mmMark[p_ppos].deviation;
            tt_code[1] = "0";
            tt_code[2] = to_string(p_ppos);
            if(mmMark[p_ppos].value_or_var == VAR_CALL){
                tt_code[3] = to_string(VAR_CALL);
            }
            else{
                tt_code[3] = to_string(VALUE_CALL);
            }
            pcode.push_back(tt_code);
//            tt_code[0] = "OPR";
//            tt_code[1] = "0";
//            tt_code[2] = "22";
//            tt_code[3] = "";
//            pcode.push_back(tt_code);
        }
        
    }
//    int _pos = tree->child[0]->pos;
//    int ppos = uuMark[_pos].pos;
    
    int level = mmMark[ppos].depth;
    tt_code[0] = CAL;
    tt_code[1] = to_string(level - cur_level);
//    cout<<"--------"<<cur_level<<" "<<level<<" "<<mmMark[ppos].value<<endl;
    int addr = mmMark[ppos].addr;
    tt_code[2] = to_string(addr);
    pcode.push_back(tt_code);
    
    for(int i = expr_num; i >= 1; i--){
        if(buf[i][1] == VALUE_CALL){
//            tt_code[0] = LIT;
//            tt_code[1] = "0";
//            tt_code[2] = to_string(buf[i][0]);
//            tt_code[3] = "";
//            pcode.push_back(tt_code);

            tt_code[0] = STO;
            tt_code[1] = "0";
            tt_code[2] = to_string(mmMark[ppos].hav[i]);
            tt_code[3] = "0";
            pcode.push_back(tt_code);
        }
    }
    
//    tt_code[0] = LIT;
//    tt_code[1] = "0";
//    tt_code[2] = to_string(mmMark[ppos].data_space_ptr);
//    tt_code[3] = "";
//    pcode.push_back(tt_code);
    
}

void GENCODE::handle_expression(NODE *tree){
    vector<string> tt_code;
    for(int i=0;i<4;i++){
        tt_code.push_back("");
    }
    
    if((int)tree->child.size() == 3){
        handle_simple_expr(tree->child[0]);
        handle_simple_expr(tree->child[2]);
        char relop_kind[30];
        int _pos = tree->child[1]->pos;
        memcpy(relop_kind, uuMark[_pos].value, sizeof(uuMark[_pos].value));
        
        if(strcmp(relop_kind, "=") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "8";
        }
        else if(strcmp(relop_kind, "<>") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "9";
        }
        else if(strcmp(relop_kind, "<") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "10";
        }
        else if(strcmp(relop_kind, "<=") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "13";
        }
        else if(strcmp(relop_kind, ">") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "12";
        }
        else if(strcmp(relop_kind, ">=") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "11";
        }
        pcode.push_back(tt_code);
    }
    else if((int)tree->child.size() == 1){
        handle_simple_expr(tree->child[0]);
        
    }
}

void GENCODE::handle_expr_list(NODE* tree){
    vector<string> tt_code;
    for(int i=0;i<4;i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "expression_list"){
        handle_expr_list(tree->child[0]);
        handle_expression(tree->child[2]);
        
    }
    else if(tree->child[0]->_name == "expression"){
        handle_expression(tree->child[0]);
    }
}

void GENCODE::handle_simple_expr(NODE *tree){
    vector<string> tt_code;
    for(int i=0;i<4;i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "term"){
        handle_term(tree->child[0]);
        
    }
    else if(tree->child[0]->_name == "+"){
        handle_term(tree->child[1]);
        
    }
    else if(tree->child[0]->_name == "-"){
        handle_term(tree->child[1]);
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "1";
        pcode.push_back(tt_code);
    }
    else if(tree->child[0]->_name == "simple_expression"){
        handle_simple_expr(tree->child[0]);
        handle_term(tree->child[2]);
        
        char addop_kind[30];
        int _pos = tree->child[1]->pos;
        memcpy(addop_kind, uuMark[_pos].value, sizeof(uuMark[_pos].value));
        
        if(strcmp(addop_kind, "+") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "2";
        }
        else if(strcmp(addop_kind, "-") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "3";
        }
        else if(strcmp(addop_kind, "or") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "21";
        }
        pcode.push_back(tt_code);
    }
        
}

void GENCODE::handle_term(NODE *tree){
    vector<string> tt_code;
    for(int i=0;i<4;i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "term"){
        handle_term(tree->child[0]);
        handle_factor(tree->child[2]);
//        tt_code[0] = LIT;
//        tt_code[1] = "0";
//
//        if(tree->child[0]->type->type == "integer"){
//            tt_code[2] = to_string(tree->child[0]->intValue);
//
//        }
//        else if(tree->child[0]->type->type == "real"){
//            tt_code[2] = to_string(tree->child[0]->realValue);
//        }
//        pcode.push_back(tt_code);
        
        char mulop_kind[30];
        int _pos = tree->child[1]->pos;
        memcpy(mulop_kind, uuMark[_pos].value, sizeof(uuMark[_pos].value));
        
        if(strcmp(mulop_kind, "*") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "4";
        }
        else if(strcmp(mulop_kind, "/") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "3";
        }
        else if(strcmp(mulop_kind, "div") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "17";
        }
        else if(strcmp(mulop_kind, "mod") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "18";
        }
        else if(strcmp(mulop_kind, "and") == 0){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "19";
        }
        
        pcode.push_back(tt_code);
        
    }
    else if(tree->child[0]->_name == "factor"){
        handle_factor(tree->child[0]);
    }
    
}

void GENCODE::handle_factor(NODE* tree){
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "unsign_const_variable"){
        
        handle_unsign_constant(tree->child[0]);
        
//        tree->intValue = tree->child[0]->intValue;
//        tree->charValue = tree->child[0]->charValue;
//        tree->realValue = tree->child[0]->realValue;
        
//        tt_code.push_back(LIT);
//        tt_code.push_back("0");
//        if(tree->type->type == "integer"){
//            tt_code.push_back(to_string(tree->intValue));
//        }
//        else if(tree->type->type == "real"){
//            tt_code.push_back(to_string(tree->realValue));
//        }
//        else if(tree->type->type == "char"){
//            string ss = "";
//            ss.push_back(tree->charValue);
//            tt_code.push_back(ss);
//        }
//        tt_code.push_back("");
//        pcode.push_back(tt_code);
//
    }
    else if(tree->child[0]->_name == "variable"){
        
        handle_variable(tree->child[0]);
        
//        tree->intValue = tree->child[0]->intValue;
//        tree->charValue = tree->child[0]->charValue;
//        tree->realValue = tree->child[0]->realValue;
//        tree->boolValue = tree->child[0]->boolValue;
            
//        tt_code.push_back(LIT);
//        tt_code.push_back("0");
//        if(tree->type->type == "integer"){
//            tt_code.push_back(to_string(tree->intValue));
//        }
//        else if(tree->type->type == "real"){
//            tt_code.push_back(to_string(tree->realValue));
//        }
//        else if(tree->type->type == "char"){
//            string ss = "";
//            ss.push_back(tree->charValue);
//            tt_code.push_back(ss);
//        }
//        else if(tree->type->type == "Boolean"){
//            string ss = "";
//            if(tree->boolValue == true)
//                tt_code.push_back("1");
//            else
//                tt_code.push_back("0");
//        }
//        tt_code.push_back("");
//        pcode.push_back(tt_code);
    }
    else if(tree->child[0]->_name == "id"){
        
        if(strcmp(uuMark[tree->child[0]->pos].value, "write") == 0){
            if((int)tree->child.size() == 1){
                return ;
            }
            handle_expr_list(tree->child[2]);
            int expr_num = tree->child[2]->expr_num;
            data_space[dataspace_len][0] = expr_num;
            data_space[dataspace_len][1] = _INTEGER;
            for(int i = 0; i < expr_num; i++){
                tt_code[0] = OPR;
                tt_code[1] = "0";
                tt_code[2] = "14";
                pcode.push_back(tt_code);
            }
            tt_code[0] = "QUE";
            tt_code[1] = "1";
            tt_code[2] = to_string(dataspace_len++);
            tt_code[3] = "";
            pcode.push_back(tt_code);
            return ;
        }
        else if(strcmp(uuMark[tree->child[0]->pos].value, "writeln") == 0){
            if((int)tree->child.size() == 1){
                tt_code[0] = OPR;
                tt_code[1] = "0";
                tt_code[2] = "15";
                pcode.push_back(tt_code);
                return ;
            }
            handle_expr_list(tree->child[2]);
            int expr_num = tree->child[2]->expr_num;
            data_space[dataspace_len][0] = expr_num;
            data_space[dataspace_len][1] = _INTEGER;
            
            for(int i = 0; i < expr_num; i++){
                tt_code[0] = OPR;
                tt_code[1] = "0";
                tt_code[2] = "14";
                pcode.push_back(tt_code);
                
            }
            tt_code[0] = "QUE";
            tt_code[1] = "1";
            tt_code[2] = to_string(dataspace_len++);
            tt_code[3] = "";
            pcode.push_back(tt_code);
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "15";
            pcode.push_back(tt_code);
            return ;
        }
        else if(strcmp(uuMark[tree->child[0]->pos].value, "read") == 0 ||
                strcmp(uuMark[tree->child[0]->pos].value, "readln") == 0){
            
            handle_expr_list(tree->child[2]);
//            cout<<"hahhahahaahahahahhahahahahahhahahahahahahahaahah"<<endl;
            
            if((int)tree->child.size() == 1){
                return ;
            }
            int expr_num = tree->child[2]->expr_num;
            data_space[dataspace_len][0] = expr_num;
            data_space[dataspace_len][1] = _INTEGER;
            
            for(int i = 0; i < expr_num; i++){
                tt_code[0] = OPR;
                tt_code[1] = "0";
                tt_code[2] = "16";
                pcode.push_back(tt_code);
//                tt_code[0] = STOA;
//                tt_code[1] = "0";
//                tt_code[2] = "1";
//                pcode.push_back(tt_code);
            }
            tt_code[0] = "QUE";
            tt_code[1] = "0";
            tt_code[2] = to_string(dataspace_len++);
            tt_code[3] = "";
            pcode.push_back(tt_code);
            
            return ;
        }
        
        
        int _pos = tree->child[0]->pos;
        int ppos = uuMark[_pos].pos;
        
        int level = mmMark[ppos].depth;
        
        
        int expr_num = tree->child[2]->expr_num;
        
        int buf[expr_num + 2][2];
        for(int i = 1; i <= expr_num; i++){
            int p_ppos = mmMark[ppos].hav[i];
            int val_ptr = mmMark[p_ppos].data_space_ptr;
            double val = data_space[val_ptr][0];
            int val_type = data_space[val_ptr][1];
            data_space[dataspace_len][0] = val;
            data_space[dataspace_len][1] = val_type;
            buf[i][0] = dataspace_len++;
            
            buf[i][1] = mmMark[p_ppos].value_or_var;
            if(buf[i][1] == VALUE_CALL){
                tt_code[0] = LODA;
                tt_code[1] = "1";
                tt_code[2] = to_string(p_ppos);
                pcode.push_back(tt_code);
            }
        }
        handle_expr_list(tree->child[2]);
        
        for(int i = expr_num; i >= 1; i--){
            tt_code[0] = "STO";
            int p_ppos = mmMark[ppos].hav[i];
//            int dep = cur_level - mmMark[p_ppos].depth + 1;
//            int dev = mmMark[p_ppos].deviation;
            tt_code[1] = "0";
            tt_code[2] = to_string(p_ppos);
            if(mmMark[p_ppos].value_or_var == VAR_CALL){
                tt_code[3] = to_string(VAR_CALL);
            }
            else{
                tt_code[3] = to_string(VALUE_CALL);
            }
            pcode.push_back(tt_code);
//            tt_code[0] = "OPR";
//            tt_code[1] = "0";
//            tt_code[2] = "22";
//            tt_code[3] = "";
//            pcode.push_back(tt_code);
        }
        
        tt_code[0] = CAL;
        tt_code[1] = to_string(level - cur_level);
        int addr = mmMark[ppos].addr;
        tt_code[2] = to_string(addr);
        tt_code[3] = "";
        pcode.push_back(tt_code);
        
        for(int i = expr_num; i >=1; i--){
            if(buf[i][1] == VALUE_CALL){
//                tt_code[0] = LIT;
//                tt_code[1] = "0";
//                tt_code[2] = to_string(buf[i][0]);
//                tt_code[3] = "";
//                pcode.push_back(tt_code);
                
                tt_code[0] = STO;
                tt_code[1] = "0";
                tt_code[2] = to_string(mmMark[ppos].hav[i]);
                tt_code[3] = "0";
                pcode.push_back(tt_code);
            }
        }
        
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(mmMark[ppos].data_space_ptr);
        tt_code[3] = "";
        pcode.push_back(tt_code);
//        tt_code[0] = LIT;
//        tt_code[1] = "0";
//
//        string _type = tree->child[0]->type->type;
//
//        if(_type == "integer"){
//            int value = tree->child[0]->intValue;
//            tt_code[2] = to_string(value);
//            tree->intValue = tree->child[0]->intValue;
//        }
//        else if(_type == "real"){
//            tt_code[2] = to_string(tree->child[0]->realValue);
//            tree->realValue = tree->child[0]->realValue;
//        }
//        else if(_type == "char"){
//            string ss = "";
//            ss.push_back(tree->child[0]->charValue);
//            tt_code[2] = ss;
//            tree->charValue = tree->child[0]->charValue;
//        }
//        else if(_type == "Boolean"){
//            if(tree->child[0]->boolValue){
//                tt_code[2] = "1";
//                tree->boolValue = true;
//            }
//            else{
//                tt_code[2] = "0";
//                tree->boolValue = false;
//            }
//        }
//        pcode.push_back(tt_code);
        
    }
    else if(tree->child[0]->_name == "("){

        handle_expression(tree->child[1]);
        
//        tt_code.push_back(LIT);
//        tt_code.push_back("0");
//        if(tree->child[1]->type->type == "integer"){
//            tt_code.push_back(to_string(tree->child[1]->intValue));
//            tree->intValue = tree->child[1]->intValue;
//        }
//        else if(tree->child[1]->type->type == "real"){
//            tt_code.push_back(to_string(tree->child[1]->realValue));
//            tree->realValue = tree->child[1]->realValue;
//        }
//        else if(tree->child[1]->type->type == "char"){
//            string ss = "";
//            ss.push_back(tree->child[1]->charValue);
//            tt_code[2] = ss;
//            tree->charValue = tree->child[1]->charValue;
//        }
//        else if(tree->child[1]->type->type == "Boolean"){
//            if(tree->child[1]->boolValue){
//                tt_code[2] = "1";
//                tree->boolValue = true;
//            }
//            else{
//                tt_code[2] = "0";
//                tree->boolValue = false;
//            }
//        }
//        tt_code.push_back("");
        
    }
    else if(tree->child[0]->_name == "not"){
        handle_factor(tree->child[1]);
        
//        if(tree->type->type == "integer"){
//            tree->intValue = -tree->child[1]->intValue - 1;
//        }
//        else if(tree->type->type == "Boolean"){
//            if(tree->child[1]->boolValue == true)
//                tree->boolValue = false;
//            else
//                tree->boolValue = true;
//        }
        
        tt_code[0] = "OPR";
        tt_code[1] = "0";
        tt_code[2] = "20";
        pcode.push_back(tt_code);
    }
}

void GENCODE::handle_unsign_constant(NODE *tree){
    
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "num"){
        if(tree->type->type == "integer"){
            data_space[dataspace_len][0] = tree->intValue;
            data_space[dataspace_len++][1] = _INTEGER;
            
        }
        else if(tree->type->type == "real"){
            data_space[dataspace_len][0] = tree->realValue;
            data_space[dataspace_len++][1] = _REAL;
        }
    }
    else if(tree->child[0]->_name == "'"){
        data_space[dataspace_len][0] = tree->charValue;
        data_space[dataspace_len++][1] = _CHAR;
    }
    
    tt_code[0] = "LIT";
    tt_code[1] = "0";
    tt_code[2] = to_string(dataspace_len - 1);
    pcode.push_back(tt_code);
}

void GENCODE::handle_else_part(NODE *tree, int &if_backpatch_num){
    if((int)tree->child.size() == 0){
        if_backpatch_num = (int)pcode.size() - 1;
        return ;
    }
    else if((int)tree->child.size() == 2){
        if_backpatch_num = (int)pcode.size() - 1;
        handle_stat(tree->child[1], if_backpatch_num);
    }
    
}

void GENCODE::handle_stat(NODE* tree, int& backpatch_num){
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    if((int)tree->child.size() == 0)
        return ;
    if(tree->child[0]->_name == "variable"){
        handle_variable(tree->child[0]);
        handle_expression(tree->child[2]);
        
        tt_code[0] = "STOA";
        tt_code[1] = "0";
        tt_code[2] = "1";
        pcode.push_back(tt_code);
    }
    else if(tree->child[0]->_name == "call_procedure_statement"){
        handle_call_procedure_stat(tree->child[0]);
    }
    else if(tree->child[0]->_name == "compound_statement"){
        handle_compound_stat(tree->child[0], backpatch_num);
    }
    else if(tree->child[0]->_name == "if"){
        handle_expression(tree->child[1]);
        
        int if_backpatch_num;
        tt_code[0] = JPC;
        tt_code[1] = "0";
        
        pcode.push_back(tt_code);
        int cur_len = (int)pcode.size() - 1;
        
        handle_stat(tree->child[3], if_backpatch_num);
        int cur_line2 = (int)pcode.size();
        tt_code[0] = JMP;
        tt_code[1] = "0";
        tt_code[2] = "";
        tt_code[2] = "";
        pcode.push_back(tt_code);
        
        pcode[cur_len][2] = to_string((int)pcode.size());
        handle_else_part(tree->child[4], if_backpatch_num);
        pcode[cur_line2][2] = to_string((int)pcode.size());
    }
    else if(tree->child[0]->_name == "case"){
        handle_expression(tree->child[1]);
        tt_code[0] = STOA;
        tt_code[1] = "1";
        tt_code[2] = to_string(dataspace_len++);
        pcode.push_back(tt_code);
        int num = dataspace_len - 1;
        handle_case_body(tree->child[3], num);
    }
    else if(tree->child[0]->_name == "while"){
        int cur_line = (int)pcode.size();
        handle_expression(tree->child[1]);
        
        int while_backpatch_num;
        tt_code[0] = JPC;
        tt_code[1] = "0";
        
        
        pcode.push_back(tt_code);
        int cur_len = (int)pcode.size() - 1;
        
        handle_stat(tree->child[3], while_backpatch_num);
        
        while_backpatch_num = (int)pcode.size() + 1;
        pcode[cur_len][2] = to_string(while_backpatch_num);
        tt_code[0] = JMP;
        tt_code[1] = "0";
        tt_code[2] = to_string(cur_line);
        pcode.push_back(tt_code);
        
    }
    else if(tree->child[0]->_name == "repeat"){
        int cur_line = (int)pcode.size();
        handle_stat_list(tree->child[1], backpatch_num);
        handle_expression(tree->child[3]);
//        tt_code[0] = "OPR";
//        tt_code[1] = "0";
//        tt_code[2] = "1";
//        pcode.push_back(tt_code);
        tt_code[0] = "JPC";
        tt_code[1] = "0";
        tt_code[2] = to_string(cur_line);
        pcode.push_back(tt_code);
    }
    else if(tree->child[0]->_name == "for"){
        handle_expression(tree->child[3]);
        int _pos = tree->child[1]->pos;
        int ppos = uuMark[_pos].pos;
//        int level = mmMark[ppos].depth;
//        int dev = mmMark[ppos].deviation;
        tt_code[0] = STO;
        tt_code[1] = "0";
        tt_code[2] = to_string(ppos);
        pcode.push_back(tt_code);
        
        int cur_len = (int)pcode.size();
        tt_code[0] = LOD;
        tt_code[1] = "0";
        tt_code[2] = to_string(ppos);
        pcode.push_back(tt_code);
        handle_expression(tree->child[5]);
        
        
        handle_updown(tree->child[4]);
        tt_code[0] = JPC;
        tt_code[1] = "0";
        tt_code[2] = to_string((int)pcode.size() + 2);
        pcode.push_back(tt_code);
        
        tt_code[0] = JMP;
        tt_code[1] = "0";
        tt_code[2] = "";
        pcode.push_back(tt_code);
        int _thisline = (int)pcode.size() - 1;
        
        handle_stat(tree->child[7], backpatch_num);
        
        tt_code[0] = LOD;
        tt_code[1] = "0";
        tt_code[2] = to_string(ppos);
        pcode.push_back(tt_code);
        data_space[dataspace_len][0] = 1;
        data_space[dataspace_len][1] = _INTEGER;
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(dataspace_len++);
        tt_code[3] = "";
        pcode.push_back(tt_code);
        if(tree->child[4]->child[0]->_name == "to"){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "2";
            pcode.push_back(tt_code);
        }
        else if(tree->child[4]->child[0]->_name == "downto"){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "3";
            pcode.push_back(tt_code);
        }
        tt_code[0] = STO;
        tt_code[1] = "0";
        tt_code[2] = to_string(ppos);
        pcode.push_back(tt_code);
        
        
        
        tt_code[0] = JMP;
        tt_code[1] = "0";
        tt_code[2] = to_string(cur_len);
        pcode.push_back(tt_code);
        
        pcode[_thisline][2] = to_string((int)pcode.size());
        tt_code[0] = LOD;
        tt_code[1] = "0";
        tt_code[2] = to_string(ppos);
        pcode.push_back(tt_code);
        data_space[dataspace_len][0] = 1;
        data_space[dataspace_len][1] = _INTEGER;
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(dataspace_len++);
        tt_code[3] = "";
        pcode.push_back(tt_code);
        if(tree->child[4]->child[0]->_name == "to"){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "3";
            pcode.push_back(tt_code);
        }
        else if(tree->child[4]->child[0]->_name == "downto"){
            tt_code[0] = OPR;
            tt_code[1] = "0";
            tt_code[2] = "2";
            pcode.push_back(tt_code);
        }
        tt_code[0] = STO;
        tt_code[1] = "0";
        tt_code[2] = to_string(ppos);
        pcode.push_back(tt_code);
        
    }
    else if(tree->child[0]->_name == ""){
        return ;
    }
}

void GENCODE::handle_case_body(NODE *tree, int num){
    if((int)tree->child.size() == 1){
        int case_line[100];
        int case_num = 0;
        handle_branch_list(tree->child[0], num, case_line, case_num);
        int this_line = (int)pcode.size();
        for(int i = 0; i < case_num; i++){
            pcode[case_line[i]][2] = to_string(this_line);
        }
    }
}

void GENCODE::handle_branch_list(NODE *tree, int num, int* case_line, int& case_line_num){
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "branch"){
        handle_branch(tree->child[0], num, case_line, case_line_num);
        
    }
    else if(tree->child[0]->_name == "branch_list"){
        handle_branch_list(tree->child[0], num, case_line, case_line_num);
        handle_branch(tree->child[2], num, case_line, case_line_num);
    }
    
    
    
}

void GENCODE::handle_branch(NODE *tree, int num, int* case_line, int& case_line_num){
    int cur_line[100];
    int cur_line_num = 0;
    handle_const_list(tree->child[0], cur_line, cur_line_num, num);
    vector<string> tt_code;
    for(int i = 0; i <4; i++){
        tt_code.push_back("");
    }
    
    tt_code[0] = JMP;
    tt_code[1] = "0";
    int tmp = (int)pcode.size();
    pcode.push_back(tt_code);
    
    
    int this_line = (int)pcode.size();
    int for_no_use = 0;
    handle_stat(tree->child[2], for_no_use);
    tt_code[0] = JMP;
    tt_code[1] = "0";
    int _this_line = (int)pcode.size();
    pcode.push_back(tt_code);
    case_line[case_line_num++] = _this_line;
    for(int i = 0; i < cur_line_num; i++){
        pcode[cur_line[i]][2] = to_string(this_line);
    }
    this_line = (int)pcode.size();
    pcode[tmp][2] = to_string(this_line);
}


void GENCODE::handle_const_list(NODE* tree, int* cur_line, int& cur_line_num, int num){
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "const_list"){
        handle_const_list(tree->child[0], cur_line, cur_line_num, num);
        handle_const_variable(tree->child[2], cur_line, cur_line_num);
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "9";
        pcode.push_back(tt_code);
        
        tt_code[0] = JPC;
        tt_code[1] = "0";
        tt_code[2] = "";
        int this_line = (int)pcode.size();
        cur_line[cur_line_num++] = this_line;
        pcode.push_back(tt_code);
        
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(num);
        pcode.push_back(tt_code);
        
    }
    else if(tree->child[0]->_name == "const_variable"){
        handle_const_variable(tree->child[0], cur_line, cur_line_num);
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "9";
        pcode.push_back(tt_code);
        
        tt_code[0] = JPC;
        tt_code[1] = "0";
        tt_code[2] = "";
        int this_line = (int)pcode.size();
        cur_line[cur_line_num++] = this_line;
        pcode.push_back(tt_code);
        
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(num);
        pcode.push_back(tt_code);
        
    }
}

void GENCODE::handle_const_variable(NODE *tree, int* cur_line, int& cur_line_num){
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "id" ||
            ((int)tree->child.size() >= 2 && tree->child[1]->_name == "id")){
        int _pos = tree->child[1]->pos;
        int ppos = uuMark[_pos].pos;
        int addr = mmMark[ppos].data_space_ptr;
        
        if(tree->type->type == "integer"){
            int val = (int)data_space[addr][0];
            
            data_space[dataspace_len][0] = val;
            data_space[dataspace_len][1] = _INTEGER;
            
            
            tt_code[0] = LIT;
            tt_code[1] = "0";
            tt_code[2] = to_string(dataspace_len++);
            pcode.push_back(tt_code);
        }
        else if(tree->type->type == "real"){
            double val = data_space[addr][0];
            data_space[dataspace_len][0] = val;
            data_space[dataspace_len][1] = _REAL;
            
            
            tt_code[0] = LIT;
            tt_code[1] = "0";
            tt_code[2] = to_string(dataspace_len++);
            pcode.push_back(tt_code);
        }
        
    }
    else if(tree->child[0]->_name == "num" ||
            ((int)tree->child.size() >= 2 && tree->child[1]->_name == "num")){
        
        
        if(tree->type->type == "integer"){
            int val = tree->intValue;
            
            data_space[dataspace_len][0] = val;
            data_space[dataspace_len][1] = _INTEGER;
            
            
            tt_code[0] = LIT;
            tt_code[1] = "0";
            tt_code[2] = to_string(dataspace_len++);
            pcode.push_back(tt_code);
        }
        else if(tree->type->type == "real"){
            double val = tree->realValue;
            data_space[dataspace_len][0] = val;
            data_space[dataspace_len][1] = _REAL;
            
            
            tt_code[0] = LIT;
            tt_code[1] = "0";
            tt_code[2] = to_string(dataspace_len++);
            pcode.push_back(tt_code);
        }
        
    }
    else if(tree->child[0]->_name == "'"){
        char val = tree->charValue;
        data_space[dataspace_len][0] = val;
        data_space[dataspace_len][1] = _CHAR;
        
        
        tt_code[0] = LIT;
        tt_code[1] = "0";
        tt_code[2] = to_string(dataspace_len++);
        pcode.push_back(tt_code);
    }
    if(tree->child[0]->_name == "-"){
        data_space[dataspace_len - 1][0] =  -data_space[dataspace_len - 1][0];
    }
}

void GENCODE::handle_updown(NODE *tree){
    vector<string> tt_code;
    for(int i = 0; i < 4; i++){
        tt_code.push_back("");
    }
    
    if(tree->child[0]->_name == "to"){
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "12";
        pcode.push_back(tt_code);
    }
    else if(tree->child[0]->_name == "downto"){
        tt_code[0] = OPR;
        tt_code[1] = "0";
        tt_code[2] = "10";
        pcode.push_back(tt_code);
    }
}

//void GENCODE::where_to_go(NODE *tree, int &backpatch_num){
//    if(tree->child.size() == 0){
//        return ;
//    }
//    for(int i = 0; i < (int)tree->child.size(); i++){
//        if(tree->_name == "program_body"){
//            handle_p_body(tree, backpatch_num);
//        }
//        else if(tree->_name == "compound_statement"){
//            handle_compound_stat(tree, backpatch_num);
//        }
//        else if(tree->_name == "statement"){
//            handle_stat(tree, backpatch_num);
//        }
//    }
//}

void GENCODE::handle_stat_list(NODE* tree, int& backpatch_num){
    if(tree->child[0]->_name == "statement_list"){
        handle_stat_list(tree->child[0], backpatch_num);
        handle_stat(tree->child[2], backpatch_num);
    }
    else if(tree->child[0]->_name == "statement"){
        handle_stat(tree->child[0], backpatch_num);
    }
}

void GENCODE::handle_compound_stat(NODE* tree, int& backpatch_num){
    int cur_line = (int)pcode.size() - 1;

    handle_stat_list(tree->child[1], backpatch_num);

    backpatch_num = cur_line;

}

void GENCODE::handle_subprogram_declaration(NODE *tree, int& backpatch_num){
    int _pos = tree->child[0]->child[1]->pos;
    int ppos = uuMark[_pos].pos;
    int size = mmMark[ppos].size;
    handle_p_body(tree->child[1], backpatch_num, size);
    
}

void GENCODE::handle_subprogram_declarations(NODE *tree, int& backpatch_num){
    if((int)tree->child.size() == 3){
        handle_subprogram_declarations(tree->child[0], backpatch_num);
        handle_subprogram_declaration(tree->child[1], backpatch_num);
    }
    else if((int)tree->child.size() == 0){
        return ;
    }
       
}

void GENCODE::handle_program_head(NODE *tree){
    vector<string> this_code;
    for(int i = 0; i < 4; i++){
        this_code.push_back("");
    }
    int _pos = tree->child[1]->pos;
    int ppos = uuMark[_pos].pos;
    int size = mmMark[ppos].size;
    this_code[0] = INT;
    this_code[1] = "0";
    this_code[2] = to_string(size + 3);

    pcode.push_back(this_code);
}

void GENCODE::handle_p_body(NODE* tree, int& backpatch_num, int size){
    cur_level++;
    
    vector<string> this_code;
    this_code.push_back(JMP);
    this_code.push_back("0");
    this_code.push_back("");
    this_code.push_back("");

    pcode.push_back(this_code);
    int cur_line = (int)pcode.size() - 1;
    
    NODE* this_node = tree->father->child[0]->child[1];
    int ppos = uuMark[this_node->pos].pos;
    mmMark[ppos].addr = (int)pcode.size() - 1;
//    cout<<">>>>>>>>>>>"<<mmMark[ppos].addr<<" "<<mmMark[ppos].value<<endl;

    handle_subprogram_declarations(tree->child[3], backpatch_num);
    this_code[0] = INT;
    this_code[1] = "0";
    this_code[2] = to_string(size + 3);
    pcode.push_back(this_code);
    handle_compound_stat(tree->child[4], backpatch_num);

    pcode[cur_line][2] = to_string(backpatch_num);
    cur_level--;
    this_code[0] = OPR;
    this_code[1] = "0";
    this_code[2] = "0";
    pcode.push_back(this_code);
}

void GENCODE::handle_program(NODE *tree){
    int backpatch_num = 0;
    int _pos = tree->child[0]->child[1]->pos;
    int ppos = uuMark[_pos].pos;
    int size = mmMark[ppos].size;
    handle_p_body(tree->child[1], backpatch_num, size);
}
