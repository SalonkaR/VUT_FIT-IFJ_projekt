/*
//IFJ20 - projekt(varianta I.)
//bt_stack.c
//Matej Hornik, xhorni20
*/


#include <stdlib.h>

#include "symtable.h"
#include "bt_stack.h"


void bt_stack_init(tBT_stack* stack){
	stack->top = NULL;
	return;
}


tBT_stack_item* bt_stack_top(tBT_stack* stack){
	return stack->top;
}


bool bt_stack_push(tBT_stack* stack){

	tBT_stack_item* new_item = malloc(sizeof(struct bt_stack_item));

	if (new_item == NULL){
		return false;
	}
    //inicializujem strom
	new_item->local_bt.definded = false;
    bool init_result = BT_init(&new_item->local_bt);
    if (init_result == false){
        return false;
    }
	new_item->next = stack->top;

	stack->top = new_item;

	return true;
}


Data_t *bt_stack_search(tBT_stack* stack, hKey_t key, bool *internal_error){
    
    for (tBT_stack_item* tmp = stack->top; tmp != NULL; tmp = tmp->next){
		Data_t *result = BT_search(&tmp->local_bt, key, internal_error);
        
        if (*internal_error == true){
            return NULL;
        }

        if (result != NULL){
            return result;
        } 
	}
    return NULL;
}


bool bt_stack_pop(tBT_stack* stack){
	
    if (stack->top != NULL){
		tBT_stack_item* tmp = stack->top;
		stack->top = tmp->next;
        BT_dispose(&tmp->local_bt);
		free(tmp);

		return true;
	}
	return false;
}


void bt_stack_free(tBT_stack* stack){
	while (bt_stack_pop(stack));
	return;
}