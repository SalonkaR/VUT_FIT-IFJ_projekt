/*
//IFJ20 - projekt(varianta I.)
//stack.c
//Matej Hornik, xhorni20
*/


#include <stdlib.h>

#include "symtable.h"
#include "stack.h"


void stack_init(Symbol_stack* stack){
	stack->top = NULL;
}


bool stack_push(Symbol_stack* stack, Prec_table_symbol symbol, enum data_types type){

	Symbol_stack_item* new_item = malloc(sizeof(struct stack_item));

	if (new_item == NULL)
		return false;

	new_item->symbol = symbol;
	new_item->data_type = type;
	new_item->next = stack->top;

	stack->top = new_item;

	return true;
}


bool stack_pop(Symbol_stack* stack){
	
    if (stack->top != NULL){
		Symbol_stack_item* tmp = stack->top;
		stack->top = tmp->next;
		free(tmp);

		return true;
	}
	return false;
}




Symbol_stack_item* stack_top_terminal(Symbol_stack* stack){

	for (Symbol_stack_item* tmp = stack->top; tmp != NULL; tmp = tmp->next){
		if (tmp->symbol < STOP)
			return tmp;
	}

	return NULL;
}


bool stack_insert_after_top_terminal(Symbol_stack* stack, Prec_table_symbol symbol, enum data_types type){
	Symbol_stack_item* prev = NULL;

	for (Symbol_stack_item* tmp = stack->top; tmp != NULL; tmp = tmp->next){
		if (tmp->symbol < STOP){
			Symbol_stack_item* new_item = malloc(sizeof(struct stack_item));

			if (new_item == NULL)
				return false;

			new_item->symbol = symbol;
			new_item->data_type = type;

			if (prev == NULL){
				new_item->next = stack->top;
				stack->top = new_item;
			}
			else {
				new_item->next = prev->next;
				prev->next = new_item;
			}

			return true;
		}
		prev = tmp;
	}

	return false;
}


Symbol_stack_item* stack_top(Symbol_stack* stack){
	return stack->top;
}

void stack_free(Symbol_stack* stack){
	while (symbol_stack_pop(stack));
}