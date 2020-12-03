/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu s frontou pre ukladanie id
//Matej Hornik, xhorni20
*/


#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>

#include "str.h"


typedef struct id_queue_item{
    struct str_struct id;
	struct id_queue_item *next; // ukazatel na dalsi prvok()
} tID_queue_item;


typedef struct id_queue{
	tID_queue_item *top; //ukazatel na zaciatok fronty
} tID_queue;


void id_queue_init(tID_queue* queue);

tID_queue_item* id_queue_top(tID_queue* queue);

tID_queue_item *id_queue_push(tID_queue* queue);

bool id_queue_pop(tID_queue* queue);

void id_queue_free(tID_queue* queue);

tID_queue_item *n_item(tID_queue* queue, int n);

void print_queue(tID_queue* queue);

#endif