#ifndef GENERATEPCODE_H
#define GENERATEPCODE_H

#include"def.h"

class GENCODE{
public:
    void handle_program(NODE* tree);
    void handle_p_body(NODE* tree, int& backpatch_num ,int size);
    void handle_compound_stat(NODE* tree, int& backpatch_num);
    void handle_stat_list(NODE* tree, int& backpatch_num);
    void handle_stat(NODE* tree, int& backpatch_num);
    void where_to_go(NODE* tree, int& backpatch_num);
    void handle_unsign_constant(NODE* tree);
    void handle_factor(NODE* tree);
    void handle_expression(NODE* tree);
    void handle_term(NODE* tree);
    void handle_simple_expr(NODE* tree);
    void handle_expr_list(NODE* tree);
    void handle_call_procedure_stat(NODE* tree);
    void handle_variable(NODE* tree);
    void handle_id_varparts(NODE* tree);
    void handle_id_varpart(NODE* tree);
    void handle_subprogram_declaration(NODE* tree, int& backpatch_num);
    void handle_subprogram_declarations(NODE* tree, int& backpatch_num);
    void handle_else_part(NODE* tree, int& if_backpatch_num);
    void handle_updown(NODE* tree);
    void handle_case_body(NODE* tree, int num);
    void handle_branch_list(NODE* tree, int num, int* case_line, int& case_line_num);
    void handle_branch(NODE* tree, int num, int* case_line, int& case_line_num);
    void handle_const_list(NODE* tree, int* cur_line, int& cur_line_num, int num);
    void handle_const_variable(NODE* tree, int* cur_line, int& cur_line_num);
    void handle_program_head(NODE* tree);
    void handle_subprogram_head(NODE* tree);
};

#endif
