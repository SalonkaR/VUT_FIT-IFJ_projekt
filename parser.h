/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu s parserom
//Matus Tvarozny, xtvaro00
//Filip Brna, xbrnaf00
*/
#ifndef PARSER_H
#define PARSER_H


#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"
#include "bt_stack.h"
#include "id_queue.h"

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
};

int parse();

//int parse(tSymbolTable *ST, tListOfInstr *instrList)
#endif
