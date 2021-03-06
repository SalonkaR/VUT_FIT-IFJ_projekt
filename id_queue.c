/*
//IFJ20 - projekt(varianta I.)
//id_queue.c
//Matej Hornik, xhorni20
*/


#include <stdio.h>
#include <stdlib.h>

#include "id_queue.h"
#include "str.h"


void id_queue_init(tID_queue* queue){
    queue->top = NULL;
	return;
}


tID_queue_item* id_queue_top(tID_queue* queue){
    return queue->top;
}

tID_queue_item *id_queue_push(tID_queue* queue){
    
    //prvy prvok v queue
    if (queue->top == NULL){
        
        tID_queue_item* new_item = malloc(sizeof(struct id_queue_item));
        
        if (new_item == NULL){
            return NULL;
        }
        new_item->next = NULL;
        bool str_init_res = str_init(&new_item->id);
        if (str_init_res == false){
            free(new_item);
            return NULL;
        }
        queue->top = new_item;
        
        return new_item;
    }

    for (tID_queue_item *tmp = queue->top; tmp != NULL; tmp = tmp->next){
        //hladame posledny prvok voi fronte
        if (tmp->next == NULL){
            tID_queue_item* new_item = malloc(sizeof(struct id_queue_item));

            if (new_item == NULL){
                return NULL;
            }
            new_item->next = NULL;
            bool str_init_res = str_init(&new_item->id);
            if (str_init_res == false){
                free(new_item);
                return NULL;
            }
            tmp->next = new_item;
            return new_item;
        }
    } 
    return NULL;
}


bool id_queue_pop(tID_queue* queue){
    if (queue->top != NULL){
        tID_queue_item* tmp;
        tmp = queue->top;
        queue->top = tmp->next;
        str_clear(&tmp->id);
        str_free(&tmp->id);
        free(tmp);
        return true;
    }
    return false;
}


void id_queue_free(tID_queue* queue){
    
    tID_queue_item* tmp = queue->top;
	while (tmp != NULL){
        tID_queue_item *to_delete = tmp;
        tmp = tmp->next;
        str_clear(&to_delete->id);
        str_free(&to_delete->id);
        free(to_delete);
    }
	return;
}

tID_queue_item *n_item(tID_queue* queue, int n){
    int counter = 0;
    tID_queue_item *tmp = queue->top;
    for (; counter < n; counter++){
        if (tmp != NULL){

            tmp = tmp->next;
        }
        else {
            return NULL;
        }
    }
    return tmp;
}


void id_queue_copy(tID_queue* src, tID_queue* dst){
    id_queue_free(dst);
    id_queue_init(dst);
    
    tID_queue_item *tmp = src->top;
    while (tmp != NULL){
        tID_queue_item *new_item = id_queue_push(dst);
        str_copy(&tmp->id, &new_item->id);

        tmp = tmp->next;
    }
}

void id_queue_copy_reverse(tID_queue* src, tID_queue* dst){
    id_queue_free(dst);
    id_queue_init(dst);
    
    tID_queue_item *tmp = src->top;
    while (tmp != NULL){
        tID_queue_item *new_item = id_stack_push(dst);
        str_copy(&tmp->id, &new_item->id);

        tmp = tmp->next;
    }
}

tID_queue_item *id_stack_push(tID_queue* queue){
    
	tID_queue_item* new_item = malloc(sizeof(struct id_queue_item));
	
	if (new_item == NULL){
		return NULL;
	}
	new_item->next = queue->top;
	bool str_init_res = str_init(&new_item->id);
	if (str_init_res == false){
		free(new_item);
		return NULL;
	}
	queue->top = new_item;
	
	return new_item;
}



//used for debug
void print_queue(tID_queue* queue){
    printf("QUEUE-> ");
    tID_queue_item* tmp = queue->top;
	while (tmp != NULL){
        printf("%s, ",tmp->id.str);
        tmp = tmp->next;
    }
    printf("; KONIEC\n");
    return;
}
