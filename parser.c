#include <stdio.h>

#include "str.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

int token;              // globalni promenna, ve ktere bude ulozen aktualni token
struct str_struct attr; // globalni promenna, ve ktere bude ulozen atribut tokenu

struct parser_data data;

int counterVar = 1;
int result;

int check_token(struct parser_data *data)
{
    if ((result = get_token(&data->token)) != LEX_TOKEN_OK)
    {
    	return LEX_ERR;
    }
    return result;
}

int check_type(struct parser_data *data, enum t_type param)
{
    if (data->token.type != param)
    {
        return SYN_ERR;
    }
    return SYN_OK;
}

int check_rule(struct parser_data *data, int rule())
{
    if ((result = rule(data)))
    {
        return result;
    }
    return SYN_OK;
}

int check_keyword(struct parser_data *data, enum keyword kword)
{
    if (data->token.type != T_TYPE_KEYWORD || data->token.attribute.keyword != kword)
    {
        return SYN_ERR;
    }
    return SYN_OK;
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

    if (eol(data) != SYN_OK)
    { 
      return SYN_ERR;
    }
    
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

int eol(struct parser_data *data)
{
  // pravidlo <eol> -> EOL <eol>

  check_token(data); //vypytam si dalsi token, ocakavam EOL
  if ( check_type(data, T_TYPE_EOL) != SYN_ERR ){
    eol(data);
  }

  // pravidlo <eol> -> epsilon

  return SYN_OK;
}



int prog(struct parser_data *data)
{
  // pravidlo <prog> -> func ID (<params>) <raturn_value> { <eol> <body> } EOL <eol> <prog>

  check_token(data); //vypytam si dalsi token, ocakavam func
  if ( check_keyword(data, KWORD_FUNC) != SYN_ERR ){
      check_token(data);

      // TO DO --> ID

      check_token(data);
      check_type(data, T_TYPE_LEFT_BRACKET);

      if (params(data) != SYN_OK){ 
          return SYN_ERR;
      }

      check_token(data);
      check_type(data, T_TYPE_RIGHT_BRACKET);

      if (return_value(data) != SYN_OK){ 
          return SYN_ERR;
      }

      check_token(data);
      check_type(data, T_TYPE_LEFT_VINCULUM);

      if (eol(data) != SYN_OK){ 
          return SYN_ERR;
      }

      if (body(data) != SYN_OK){ 
          return SYN_ERR;
      }

      check_token(data);
      check_type(data, T_TYPE_RIGHT_VINCULUM);
      check_token(data);
      check_type(data, T_TYPE_EOL);

      if (eol(data) != SYN_OK){ 
          return SYN_ERR;
      }

      if (prog(data) != SYN_ERR){ 
          prog(data);
      }
  }
  // pravidlo <prog> -> epsilon
  return SYN_OK;
}


int body(struct parser_data *data)
{
  // pravidlo <body> -> if <expression> { EOL <body>} else { EOL <body>} EOL <body>
  // pravidlo <body> -> for <definition> ; <expression> ; <assignment> { EOL <body> } EOL  <body>
  // pravidlo <body> -> ID := <expression> EOL <eol> <body>
  // pravidlo <body> -> <ids> = ID(<argument>) EOL <eol> <body>
  // pravidlo <body> -> <ids> = <expression> EOL <eol> <body>
  // pravidlo <body> ->  return <list_of_return_values> EOL <eol> <body>
  // pravidlo <body> -> ID ( <argument> ) EOL <eol> <body>
  // pravidlo <body> -> "SETZERO" "ID" ";"

 
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYN_OK;
}

int definition(struct parser_data *data)
{
  // pravidlo <definition> -> ID := <value>

  check_token(data);
  // TO DO -->  if ID then 
  //       ||
  //       ||
  //       \/
  //check_token(data);
  //check_type(data, T_TYPE_VARIABLE_DEFINITION);

  //if (value(data) != SYN_OK){ 
  //        return SYN_ERR;
  //    }

  // pravidlo <definition> -> epsilon

  return SYN_OK;
}

int assignment(struct parser_data *data)
{
  // pravidlo <assignment> -> ID = <expression>

  check_token(data);
  // TO DO -->  if ID then 
  //       ||
  //       ||
  //       \/
 /*
  check_token(data);
  check_type(data, T_TYPE_ASSIGN);

  TO DO --> EXPRESSION
  
  if (expression(data) != SYN_OK){ 
          return SYN_ERR;
      } 
  */

  // pravidlo <assignment> -> epsilon

  return SYN_OK;
}

int ids(struct parser_data *data)
{
  // pravidlo <ids> -> ID <id_n>

  check_token(data);
  // TO DO --> ID
  //       ||
  //       ||
  //       \/
  check_token(data);
  if (id_n(data) != SYN_OK){ 
          return SYN_ERR;
      }

  return SYN_OK;
}

int id_n(struct parser_data *data)
{
  // pravidlo <id_n> -> , ID <id_n>

  check_token(data);
  if( check_type(data, T_TYPE_RIGHT_BRACKET) != SYN_ERR ) {
      // TO DO --> ID
      //       ||
      //       ||
      //       \/
      check_token(data);
      if (id_n(data) != SYN_ERR){ 
          id_n(data);
      }
  }

  // pravidlo <id_n> -> epsilon

  return SYN_OK;
}

int list_of_return_values(struct parser_data *data)
{
  // pravidlo <list_of_return_values> -> <values>

  check_token(data);
  if( values(data) != SYN_ERR ){
    return SYN_OK;
  }

  // pravidlo <list_of_return_values> -> epsilon
  return SYN_OK;
}

int values(struct parser_data *data)
{
  // pravidlo <values> -> <expression> <values_n>

  check_token(data);
  // TO DO --> EXPRESSION
  //       ||
  //       ||
  //       \/
  //if( expression(data) != SYN_ERR ){
  //  check_token(data);
  //  if (values_n(data) != SYN_OK){ 
  //    return SYN_ERR;
  //  }
  //  return SYN_OK;
  //}
  //else{
  //   return SYN_ERR;
  //}
  return SYN_OK;
}

int values_n(struct parser_data *data)
{
  // pravidlo <values_n> -> ,<expression> <values_n>

    check_token(data);
    check_type(data, T_TYPE_COMMA);
    check_token(data);
  // TO DO --> EXPRESSION
  //       ||
  //       ||
  //       \/
  //if( expression(data) != SYN_ERR ){
  //  check_token(data);
  //  if (values_n(data) != SYN_OK){ 
  //    return SYN_ERR;
  //  }
  //}

  return SYN_OK;
}

int params(struct parser_data *data)
{
  // pravidlo <params> -> ID <type> <params_n>

  check_token(data);
  // TO DO --> IF ID THEN
  //       ||
  //       ||
  //       \/
  //      if (type(data) != SYN_OK){ 
  //          return SYN_ERR;
  //      }
  //      if (params_n(data) != SYN_OK){ 
  //          return SYN_ERR;
  //      }

  // pravidlo <params> -> epsilon
  return SYN_OK;
}

int params_n(struct parser_data *data)
{
  // pravidlo <params_n> -> , ID <type> <params_n>
  

  check_token(data);
  if ( check_type(data, T_TYPE_COMMA) != SYN_ERR ){
  // TO DO --> ID 
  //       ||
  //       ||
  //       \/
  //      if (type(data) != SYN_OK){ 
  //          return SYN_ERR;
  //      }
  //      if (params_n(data) != SYN_ERR){ 
  //          params_n(data);
  }

  // pravidlo <params_n> ->epsilon
  return SYN_OK;
}

int return_value(struct parser_data *data)
{
  // pravidlo <return_value> -> ( <type> <return_value_n> )

  check_token(data);
  if ( check_type(data, T_TYPE_LEFT_BRACKET) != SYN_ERR ){
      if (type(data) != SYN_OK){ 
        return SYN_ERR;
      }
      if (return_value_n(data) != SYN_OK){ 
        return SYN_ERR;
      }
      check_token(data);
      check_type(data, T_TYPE_RIGHT_BRACKET);
  }

  // pravidlo <return_value> -> epsilon
     
  return SYN_OK;
}

int return_value_n(struct parser_data *data)
{
  // pravidlo <return_value_n> -> , <type> <return_value_n>

  check_token(data);
  if ( check_type(data, T_TYPE_COMMA) != SYN_ERR ){
      if (type(data) != SYN_OK){ 
        return SYN_ERR;
      }
      if (return_value_n(data) != SYN_ERR){ 
        return_value_n(data);
      }
  }

  // pravidlo <return_value_n> -> epsilon

  return SYN_OK;
}

int type(struct parser_data *data)
{
  // pravidlo <type> -> int
  // pravidlo <type> -> float
  // pravidlo <type> -> string

  check_token(data);
  if ( ( check_keyword(data, KWORD_INT) != SYN_ERR ) || 
       ( check_keyword(data, KWORD_FLOAT64) != SYN_ERR )  ||
       ( check_keyword(data, KWORD_STRING) != SYN_ERR )) {
      return SYN_OK;
  }
  return SYN_ERR;
}

int argument(struct parser_data *data)
{
  // pravidlo <argument> -> <value> <argument_n>
  
  check_token(data);
  if ( value(data) != SYN_ERR){ 
      if (argument_n(data) != SYN_ERR){ 
        return SYN_OK;;
      }
  }

  // pravidlo <argument> -> epsilon
  return SYN_OK;
}

int argument_n(struct parser_data *data)
{
  // pravidlo <argument_n> -> ,<value> <argument_n>

  check_token(data);
  if ( check_type(data, T_TYPE_COMMA) != SYN_ERR ){
      if (value(data) != SYN_OK){ 
        return SYN_ERR;
      }
      if (argument_n(data) != SYN_ERR){ 
        argument_n(data);
      }
  }

  // pravidlo <argument_n> -> epsilon

  return SYN_OK;
}

int value(struct parser_data *data)
{
  // pravidlo <value> -> INT_VALUE
  // pravidlo <value> -> FLOAT_VALUE
  // pravidlo <value> -> STRING_VALUE

  check_token(data);
  if ( ( check_type(data, T_TYPE_INTEGER) != SYN_ERR ) || 
       ( check_type(data, T_TYPE_DOUBLE) != SYN_ERR )  ||
       ( check_type(data, T_TYPE_STRING) != SYN_ERR )) {
      return SYN_OK;
  }
  return SYN_ERR;
}


bool init_variables(struct parser_data* data)
{
	BT_init(&data->BT_global);
	BT_init(&data->BT_local);
    
	//data->current_id = NULL;
	//data->lhs_id = NULL;
	//data->rhs_id = NULL;

	//data->param_index = 0;
	//data->label_index = 0;
	//data->label_deep = -1;

	//data->in_function = false;
	//data->in_declaration = false;
	//data->in_while_or_if = false;
	//data->non_declared_function = false;

	// init default functions

	//bool internal_error;
	//struct parser_data* id;

	// Length(s As String) As Integer
	//id = sym_table_add_symbol(&data->BT_global, "length", &internal_error);
	//if (internal_error) return false;

	//id->defined = true;
	//id->type = T_TYPE_INTEGER;
	//if (!sym_table_add_param(id, T_TYPE_STRING)) return false;

	// SubStr(s As String, i As Integer, n As Integer) As String
	//id = sym_table_add_symbol(&data->global_table, "substr", &internal_error);
	//if (internal_error) return false;

	//id->defined = true;
	//id->type = T_TYPE_STRING;
	//if (!sym_table_add_param(id, T_TYPE_STRING)) return false;
	//if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;
	//if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;

	// Asc(s As String, i As Integer) As Integer
	//id = sym_table_add_symbol(&data->global_table, "asc", &internal_error);
	//if (internal_error) return false;

	//id->defined = true;
	//id->type = T_TYPE_INTEGER;
	//if (!sym_table_add_param(id, T_TYPE_STRING)) return false;
	//if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;

	// Chr(i As Integer) As String
	//id = sym_table_add_symbol(&data->global_table, "chr", &internal_error);
	//if (internal_error) return false;

	//id->defined = true;
	//id->type = T_TYPE_STRING;
	//if (!sym_table_add_param(id, T_TYPE_INTEGER)) return false;

	// Global variable %exp_result for storing result of expression.
	//id = sym_table_add_symbol(&data->global_table, "%exp_result", &internal_error);
	//if (internal_error) return false;
	//id->defined = true;
	//id->type = T_type_UNDEF;
	//id->global = true;

	return true;
}

void free_variables(struct parser_data* data)
{
    BT_dispose(&data->BT_global);
    BT_dispose(&data->BT_local);
    //TODO
}

int parse()
{
    struct str_struct str;

    if (str_init(&str) == false){
        return ERROR_INTERNAL;
    }
  
    
    data.BT_global.definded = false;  //som retard a musim to spravit
    data.BT_local.definded = false;   //tu tiez to iste  

    if(init_variables(&data) == false){
        str_free(&str);
        return ERROR_INTERNAL;
    }

    if((result = get_token(&data.token)) == LEX_TOKEN_OK)
    {
        /*
        if(code_generator() == NULL){
            str_free(&str);
            free_variables(&data);
            return ERROR_INTERNAL;
        }
        */
        result = start(&data);
    }

    str_free(&str);
    free_variables(&data);
    return result;
} 


