/*
//IFJ20 - projekt(varianta I.)
//stack.c
//Matej Hornik, xhorni20
*/


#include <stdlib.h>

#include "symtable.h"
#include "stack.h"


void stack_init(tStack* stack){
	stack->top = NULL;
	return;
}


tStack_item* stack_top(tStack* stack){
	return stack->top;
}


bool stack_push(tStack* stack, Prec_table_symbol symbol, enum data_types type){

	tStack_item* new_item = malloc(sizeof(struct stack_item));

	if (new_item == NULL){
		return false;
	}

	new_item->symbol = symbol;
	new_item->data_type = type;
	new_item->next = stack->top;

	stack->top = new_item;

	return true;
}


bool stack_pop(tStack* stack){
	
    if (stack->top != NULL){
		tStack_item* tmp = stack->top;
		stack->top = tmp->next;
		free(tmp);

		return true;
	}
	return false;
}




tStack_item* stack_top_term(tStack* stack){

	for (tStack_item* tmp = stack->top; tmp != NULL; tmp = tmp->next){
		if (tmp->symbol < STOP){
			return tmp;
		}
	}

	return NULL;
}


bool stack_push_after_top_term(tStack* stack, Prec_table_symbol symbol, enum data_types type){
	tStack_item* tmp1 = NULL;

	for (tStack_item* tmp = stack->top; tmp != NULL; tmp = tmp->next){
		if (tmp->symbol < STOP){
			tStack_item* new_item = malloc(sizeof(struct stack_item));

			if (new_item == NULL){
				return false;
			}

			new_item->symbol = symbol;
			new_item->data_type = type;

			if (tmp1 == NULL){
				new_item->next = stack->top;
				stack->top = new_item;
			}
			else {
				new_item->next = tmp1->next;
				tmp1->next = new_item;
			}

			return true;
		}
		tmp1 = tmp;
	}

	return false;
}


void stack_free(tStack* stack){
	while (stack_pop(stack));
	return;
}