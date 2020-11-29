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

struct parser_data
{
    tBT BT_global;
    tBT_stack BT_stack;
    struct token token;
    bool in_if_for;
};

int parse();

//int parse(tSymbolTable *ST, tListOfInstr *instrList)
#endif
