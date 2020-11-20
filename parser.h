#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"

struct parser_data
{
    tBT BT_global;
    tBT BT_local;
    struct token token;
};

int eol(struct parser_data *data);
int prog(struct parser_data *data);
int body(struct parser_data *data);
int definition(struct parser_data *data);
int assignment(struct parser_data *data);
int ids(struct parser_data *data);
int id_n(struct parser_data *data);
int list_of_return_values(struct parser_data *data);
int values(struct parser_data *data);
int values_n(struct parser_data *data);
int params(struct parser_data *data);
int params_n(struct parser_data *data);
int return_value(struct parser_data *data);
int return_value_n(struct parser_data *data);
int type(struct parser_data *data);
int argument(struct parser_data *data);
int argument_n(struct parser_data *data);
int value(struct parser_data *data);

int parse();

//int parse(tSymbolTable *ST, tListOfInstr *instrList)
