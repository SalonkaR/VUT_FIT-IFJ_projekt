/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu zo zasobnikom pouziti pri praci z definiciami premmenych
//Matej Hornik, xhorni20
*/

#ifndef BT_STACK_H
#define BT_STACK_H

#include <stdio.h>
#include <stdbool.h>

#include "symtable.h"



typedef struct bt_stack_item{
    tBT local_bt;
	struct bt_stack_item *next; // ukazatel na dalsi prvok()
} tBT_stack_item;


typedef struct bt_stack{
	tBT_stack_item *top; //ukazatel na vrchol stacku
} tBT_stack;


void bt_stack_init(tBT_stack* stack);

tBT_stack_item* bt_stack_top(tBT_stack* stack);

bool bt_stack_push(tBT_stack* stack);

Data_t *bt_stack_search(tBT_stack* stack, hKey_t key, bool *internal_error);

bool bt_stack_pop(tBT_stack* stack);

void bt_stack_free(tBT_stack* stack);

#endif