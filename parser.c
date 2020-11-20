#include <stdio.h>
#include "str.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

int token;              // globalni promenna, ve ktere bude ulozen aktualni token
struct str_struct attr; // globalni promenna, ve ktere bude ulozen atribut tokenu

int counterVar = 1;
int result;

void check_token(struct parser_data *data)
{
    if ((result = get_token(&data->token)) != LEX_TOKEN_OK)
    {
    	return LEX_ERR;
    }
    return result;
}

void check_type(struct parser_data *data, enum t_type *param)
{
    if (data->token.type != param)
    {
        return SYN_ERR;
    } 
}

void check_rule(struct parser_data *data, int rule())
{
    if ((result = rule(data)))
    {
        return result;
    }
}

void check_keyword(struct parser_data *data, enum keyword *kword)
{
    if (data->token.type != T_TYPE_KEYWORD || data->token.attribute.keyword != kword)
    {
        return SYN_ERR;
    }
}

int start(struct parser_data *data)
{
  // pravidlo <start> -> package main EOL <eol> <prog> EOF
  result = 0;
  if (data->token.type == T_TYPE_KEYWORD && data->token.attribute.keyword == KWORD_PACKAGE)
  {
    check_token(data); //vypytam si dalsi token, ocakavam main
    check_keyword(data, KWORD_MAIN);
    check_token(data); //vypytam si dalsi token, ocakavam EOL
    check_type(data, T_TYPE_EOL);
    check_token(data);
    if (eol(data) != SYN_OK)
    { 
      return SYN_ERR;
    }
    check_token(data);
    if (prog(data) != SYN_OK)
    {
      return SYN_ERR;
    }
    check_token(data);
    check_type(data, T_TYPE_EOF);
    return SYN_OK;
  }
  return SYN_ERR;
}

int eol()
{
  // pravidlo <eol> -> EOL <eol>
  
  
  // pravidlo <eol> -> epsilon

  
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

/*

int prog()
{
  // pravidlo <prog> -> func ID (<params>) <raturn_value> { <eol> <body> } EOL <eol> <prog>
  // pravidlo <prog> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int body()
{
  // pravidlo <body> -> if <expression> { EOL <body>} else { EOL <body>} EOL <body>
  // pravidlo <body> -> for <definition> ; <expression> ; <assignment> { EOL <body> } EOL  <body>
  // pravidlo <body> -> ID := <expression> EOL <eol> <body>
  // pravidlo <body> -> <ids> = ID(<argument>) EOL <eol> <body>
  // pravidlo <body> -> <ids> = <expression> EOL <eol> <body>
  // pravidlo <body> ->  return <list_of_return_values> EOL <eol> <body>
  // pravidlo <body> -> ID ( <argument> ) EOL <eol> <body>
  // pravidlo <body> -> "SETZERO" "ID" ";"

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int definition()
{
  // pravidlo <definition> -> ID := <value>
  // pravidlo <definition> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int assignment()
{
  // pravidlo <assignment> -> ID = <expression>
  // pravidlo <assignment> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int ids()
{
  // pravidlo <ids> -> ID <id_n>

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int id_n()
{
  // pravidlo <id_n> -> , ID <id_n>
  // pravidlo <id_n> -> epsilon
  int result;
  switch (token)
  {
  case T_TYPE_COMMA:
    // pouze pozadame o dalsi token a vratime uspech
      if ((token = getNextToken(&attr) == LEX_ERR) return LEX_ERR;
      return SYNTAX_OK;
    break;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int list_of_return_values()
{
  // pravidlo <list_of_return_values> -> epsilon
  // pravidlo <list_of_return_values> -> <values>

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int values()
{
  // pravidlo <values> -> <expression> <values_n>

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int values_n()
{
  // pravidlo <values_n> -> ,<expression> <values_n>
  // pravidlo <values_n> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int params()
{
  // pravidlo <params> -> ID <type> <params_n>
  // pravidlo <params> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int params_n()
{
  // pravidlo <params_n> -> , ID <type> <params_n>
  // pravidlo <params_n> ->epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int return_value()
{
  // pravidlo <return_value> -> ( <type> <return_value_n> )
  // pravidlo <return_value> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int return_value_n()
{
  // pravidlo <return_value_n> -> , <type> <return_value_n>
  // pravidlo <return_value_n> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int type()
{
  // pravidlo <type> -> int
  // pravidlo <type> -> float
  // pravidlo <type> -> string

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int argument()
{
  // pravidlo <argument> -> <value> <argument_n>
  // pravidlo <argument> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int argument_n()
{
  // pravidlo <argument_n> -> ,<value> <argument_n>
  // pravidlo <argument_n> -> epsilon

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}

int value()
{
  // pravidlo <value> -> INT_VALUE
  // pravidlo <value> -> FLOAT_VALUE
  // pravidlo <value> -> STRING_VALUE

  int result;
  switch (token)
  {
  case:
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYNTAX_ERROR;
}
bool init_variables(struct parser_data* data)
{
	sym_table_init(&data->global_table);
	sym_table_init(&data->local_table);
    
	data->current_id = NULL;
	data->lhs_id = NULL;
	data->rhs_id = NULL;

	data->param_index = 0;
	data->label_index = 0;
	data->label_deep = -1;

	data->in_function = false;
	data->in_declaration = false;
	data->in_while_or_if = false;
	data->non_declared_function = false;

	// init default functions

	bool internal_error;
	struct parser_data* id;

	// Length(s As String) As Integer
	id = sym_table_add_symbol(&data->global_table, "length", &internal_error);
	if (internal_error) return false;

	id->defined = true;
	id->type = T_TYPE_INTEGER;
	if (!sym_table_add_param(id, T_TYPE_STRING)) return false;

	// SubStr(s As String, i As Integer, n As Integer) As String
	id = sym_table_add_symbol(&data->global_table, "substr", &internal_error);
	if (internal_error) return false;

	id->defined = true;
	id->type = T_TYPE_STRING;
	if (!sym_table_add_param(id, T_TYPE_STRING)) return false;
	if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;
	if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;

	// Asc(s As String, i As Integer) As Integer
	id = sym_table_add_symbol(&data->global_table, "asc", &internal_error);
	if (internal_error) return false;

	id->defined = true;
	id->type = T_TYPE_INTEGER;
	if (!sym_table_add_param(id, T_TYPE_STRING)) return false;
	if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;

	// Chr(i As Integer) As String
	id = sym_table_add_symbol(&data->global_table, "chr", &internal_error);
	if (internal_error) return false;

	id->defined = true;
	id->type = T_TYPE_STRING;
	if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;

	// Global variable %exp_result for storing result of expression.
	id = sym_table_add_symbol(&data->global_table, "%exp_result", &internal_error);
	if (internal_error) return false;
	id->defined = true;
	id->type = T_type_UNDEF;
	id->global = true;

	return true;
}

void free_variables(struct parser_data* data){

      sym_table_free(&data->global_table);
      sym_table_free(&data->local_table);
}





int parse()
{
  int result;

  struct str_struct str;

  if (str_init(&str) == NULL){
    return ERROR_INTERNAL;
  }
  set_dynamic_string(&str);
  struct parser_data data;

  if(init_variables(&data) == NULL){
    str_free(&str);
    return ERROR_INTERNAL;
  }

  if((result = get_token(&data.token)) == LEX_TOKEN_OK)
  {
    if(code_generator() == NULL){
      str_free(&str);
      free_variables(&data);
      return ERROR_INTERNAL;
    }

    result = start();
  }

  str_free(&str);
  free_variables(&data);
  return result;
} */
//} 

