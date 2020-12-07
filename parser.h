/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu s parserom
//Matus Tvarozny, xtvaro00
//Filip Brna, xbrnaf00
//Matej Hornik, xhorni20
*/
#ifndef PARSER_H
#define PARSER_H


#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"
#include "bt_stack.h"
#include "id_queue.h"

typedef struct func_calls{
    struct str_struct called_func;
    tID_queue rs;       //prava strana, cize vstupne parametry(typy)
    tID_queue ls;       //lava strana, cize vystupne hodnoty funkie(typy)
    struct func_calls *next;
}tFunc_calls;

struct parser_data
{
    tBT BT_global;
    tBT_stack BT_stack;
    struct token token;

    bool in_if_for;
    bool set_type_id;
    bool check_type;
    bool check_returns;

    int checked_returns; //pomocne pocitadlo pre expression pri checkovani return values
    tID_queue ID_queue;
    unsigned int argument_counter;
    unsigned int no_ifs;
    Data_t *actual_func;

    tFunc_calls *check_func_calls; //pre konotrlovanie volani funckii, linked list
};

int parse();

//int parse(tSymbolTable *ST, tListOfInstr *instrList)
#endif
