/*
//IFJ20 - projekt(varianta I.)
//gen_code.h -> hlavickovy subor pre generovanie kodu
//Ivo Prochazka, xproch0h
//Matej Hornik, xhorni20
*/

#ifndef gen_code_h

#define gen_code_h

#include <stdio.h>

#include "symtable.h"
#include "scanner.h"
#include "expression.h"

void gen_code_start();

void main_func();

void func_beg(char func_name[]);

void func_fin(char func_name[]);

void call_func(char func_name[]);

void before_call_func_params(struct token *token);

void after_func_beg_params(char value[]);

void func_retval(char value[], int index);

void retval_to_y(int index);

void make_var(char var_name[], char loc[]);

void gen_label(char func_name[], int index, char ee[]);

void gen_if_start();

void gen_if(char func_name[], int index);

void gen_if_else(char func_name[], int index);

void gen_if_end(char func_name[], int index);

void gen_for_start();

void gen_for(char func_name[], int index, int depth, char ee[]);

void gen_for_end(char func_name[], int index, int depth);

void gen_arithmetic(Prec_rules rule);

void gen_value(struct token *token, struct str_struct *to_which);

void concat_strings();

void print_ifjcode20();

void pop_value(char var_name[]);

void main_func_end();

void push_value(struct token *token);

void push_confirm();

void save_params_to_stack(char value[]);

void init_params();

void copy_ids_on_stack(tID_queue* src);

void pop_returned_values();

void print_gencode(struct token *token);

#endif 