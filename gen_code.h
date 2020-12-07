#ifndef gen_code_h
#define gen_code_h

#include <stdio.h>

#include "symtable.h"
#include "scanner.h"
#include "expression.h"

static void gen_code_start();

void main_func();

void func_beg(char func_name[]);

void func_fin(char func_name[]);

void call_func(char func_name[]);

void before_call_func_params(struct token *token, int index);

void after_func_beg_params(int index);

void func_retval(char value[], int index);

void retval_to_y(int index);

void make_var(char var_name[], char loc[], int depth);

void gen_label(char func_name[], int index, int depth, char ee[]);

void gen_if_start();

void gen_if(char func_name[], int index, int depth);

void gen_if_else(char func_name[], int index, int depth);

void gen_if_end(char func_name[], int index, int depth);

void gen_for_start();

void gen_for(char func_name[], int index, int depth, char ee[]);

void gen_for_end(char func_name[], int index, int depth);

void gen_arithmetic(symb);

void push_value(struct token *token);

void concat_strings();

#endif 