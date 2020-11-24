/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu s parserom
//Filip Brna, xbrnaf00
*/



#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"

struct parser_data
{
    tBT BT_global;
    tBT BT_local;
    struct token token;
    bool in_if_for;
};

//implicitna deklarakia funkcii

int eol();
int prog();
int body();
int definition();
int assignment();
int ids();
int id_n();
int list_of_return_values();
int values();
int values_n();
int params();
int params_n();
int return_value();
int return_value_n();
int type();
int argument();
int argument_n();
int value();
int expression();

int parse();


