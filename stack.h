/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu zo zasobnikom pouziti pri praci s vyrazmi
//Matej Hornik, xhorni20
*/

#ifndef STACK_H
#define STACK_H


#include <stdbool.h>

#include "expression.h"
#include "symtable.h"



typedef struct stack_item
{
	Prec_table_symbol symbol; //symbol na stacku
	enum data_types data_type; //typ dat
	struct stack_item *next; // ukazatel na dalsi prvok()
} tStack_item;


typedef struct stack
{
	tStack_item *top; //ukazatel na vrchol stacku
} tStack;


void stack_init(tStack* stack);

bool stack_push(tStack* stack, Prec_table_symbol symbol, enum data_types type);

bool stack_pop(tStack* stack);

tStack_item* stack_top_term(tStack* stack);

bool stack_push_after_top_term(tStack* stack, Prec_table_symbol symbol, enum data_types type);

tStack_item* stack_top(tStack* stack);

void stack_free(tStack* stack);


#endif