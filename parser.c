/*
//IFJ20 - projekt(varianta I.)
//parser.c
//Matus Tvarozny, xtvaro00
//Filip Brna, dopis si login autik
*/

#include <stdio.h>
#include <stdlib.h>

#include "str.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "expression.h"
#include "bt_stack.h"

struct parser_data data;

struct str_struct *temp_string;

struct str_struct *func_id;

bool internal_error = false;
bool non_det = false;
bool bad_returns = false;

int result;


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

int check_token()
{
    //printf("mam token\n");

    //printf("%s", data.token.attribute.string->str);
    if ((result = get_token(&data.token)) != LEX_TOKEN_OK)
    {
      
    	return LEX_ERR;
    }
    //printf("tu som uz pici\n");
    return result;
}

int check_type(enum t_type param)
{
    if (data.token.type != param)
    {
        return SYN_ERR;
    }
    return SYN_OK;
}

int check_keyword(enum keyword kword)
{ 
    //printf("----------------CHECK KEYWORD %d = %d -------------\n",data.token.attribute.keyword, kword);
    if (data.token.type != T_TYPE_KEYWORD || data.token.attribute.keyword != kword)
    {
        
        return SYN_ERR;
    }
    return SYN_OK;
}

/* vykresli sktrukturu binarniho stromu */
void Print_tree2(tNode *TempTree, char* sufix, char fromdir){   
     if (TempTree != NULL)
     {
        char* suf2 = (char*) malloc(strlen(sufix) + 4);
        strcpy(suf2, sufix);
        if (fromdir == 'L')
	    {
            suf2 = strcat(suf2, "  |");
            printf("%s\n", suf2);
	    }
	    else
	        suf2 = strcat(suf2, "   ");
	    Print_tree2(TempTree->RPtr, suf2, 'R');
        //LL -> vypisovanie linked listu
        //printf("%s  +-[%c,%d]\n", sufix,  TempTree->hashKey, 9);
        printf("%s  +-[%lu; ", sufix, TempTree->hashKey);
        Data_t *tmp = TempTree->data;
        while (tmp != NULL){
            printf("'%s',", tmp->identifier);
            tmp = tmp->nextptr;
        }
        printf("]\n");


	    strcpy(suf2, sufix);
        if (fromdir == 'R')
	        suf2 = strcat(suf2, "  |");	
	    else
	        suf2 = strcat(suf2, "   ");
	    Print_tree2(TempTree->LPtr, suf2, 'L');
	    if (fromdir == 'R') printf("%s\n", suf2);
	    free(suf2);
    }
}

void Print_tree(tNode *TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
} 

int start()
{
  // pravidlo <start> -> <eol> package main EOL <eol> <prog> EOF
    if (check_type(T_TYPE_EOL) != SYN_ERR)
    {
        eol();
    }
    //printf("----------------0. TOKEN START TYPE = %d -------------\n",data.token.type);
    if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_PACKAGE)
    {
        if (check_token() == LEX_ERR) return LEX_ERR;
        // za keywordom package, musi pokracovat keyword main, je ale kontrolovany rucne, kvoli identifikatoru jednej z funkcii,
        // ktora musim byt main, ale typ daneho tokenu musi byt identifier nie keyword
        if (str_cmp_const_str(data.token.attribute.string, "main")) return SYN_ERR;
        if (check_token() == LEX_ERR) return LEX_ERR;
        if (check_type(T_TYPE_EOL) == SYN_ERR ) return SYN_ERR;
        int exit_eol = eol();
        if (exit_eol != SYN_OK) return exit_eol;
        int exit_prog = prog();
        if (exit_prog != SYN_OK) return exit_prog;
        if (check_token() == LEX_ERR) return LEX_ERR;
        if (check_type(T_TYPE_EOF) == SYN_ERR ) return SYN_ERR;
        return SYN_OK;
    }
    return SYN_ERR;
}

int eol()
{
    // pravidlo <eol> -> EOL <eol>
    //printf("----------------0. TOKEN EOL TYPE = %d -------------\n",data.token.type);
    if (check_type(T_TYPE_EOL) != SYN_ERR)
    {  
        if (check_token() == LEX_ERR) return LEX_ERR;
        return eol();
    }
    // pravidlo <eol> -> epsilon
    else
    {  
      //printf("----------------LAST TOKEN EOL TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
    }
}

int prog()
{
  // pravidlo <prog> -> func ID (<params>) <raturn_value> { EOL <eol> <body> } ???EOF??? EOL <eol> <prog>

  //printf("----------------VSTUPUJEM DO PROGU-------------\n");

   //vypytam si dalsi token, ocakavam func
  //printf("----------------0. TOKEN PROG MAM - TYPE = %d -------------\n",data.token.type);

  if (check_keyword(KWORD_FUNC) != SYN_ERR ){

      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("----------------1. TOKEN PROG -TYPE = %d -------------\n",data.token.type);
      //printf("----------------2. TOKEN PROG MAM IF-TYPE = %d -------------\n",check_type(T_TYPE_IDENTIFIER));
      if (check_type(T_TYPE_IDENTIFIER) == SYN_ERR) return SYN_ERR;
      str_copy(data.token.attribute.string, func_id);
      
      BT_insert(&data.BT_global, func_id->str, &internal_error);
      bt_stack_push(&data.BT_stack);

      if (internal_error == true) return ERROR_INTERNAL;
      
      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("----------------2. TOKEN PROG TYPE = %d -------------\n",data.token.type);

      if (check_type(T_TYPE_LEFT_BRACKET) == SYN_ERR ){
        return SYN_ERR;
      }
      
      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("----------------3. TOKEN PROG TYPE = %d -------------\n",data.token.type);

      //main nemoze mat ziadne vstupne parametre
      if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR && str_cmp_const_str(func_id, "main") == 0) return SEM_ERR_NO_PARAMS;

    	if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
        //printf("VOLAM PARAMS -----------\n");
        int exit_params = params();
        if (exit_params != SYN_OK){ 
          return exit_params;
        }
      }

      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }

      //main nemoze mat ziadne vystupne parametre
      if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR && str_cmp_const_str(func_id, "main") == 0) return SEM_ERR_NO_PARAMS;

      //printf("----------------4.5. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
    	if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
        //printf("----------------VOLAM RV-------------\n");
        int exit_return_value = return_value();
        if (exit_return_value != SYN_OK){ 
          return exit_return_value;
        }
      }

      if (check_token() == LEX_ERR){
      return LEX_ERR;
      }
      //printf("----------------5. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
      if (check_type( T_TYPE_EOL) == SYN_ERR){
        return SYN_ERR;
      }      
    	if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
        //printf("----------------VOLAM EOl- TYPE = %d -------------\n",data.token.type);
        int exit_eol = eol();
        if (exit_eol != SYN_OK){ 
          return exit_eol;
        }
        //printf("-------------------------------5.5. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
          //printf("---------------- 1 VOLAM BODY - TYPE = %d -------------\n",data.token.type);
          int exit_body = body();
          if (exit_body != SYN_OK){ 
            return exit_body;
          }
          //printf("---------------- 1.5 VOLAM BODY - TYPE = %d -------------\n",data.token.type);
          if ( check_type( T_TYPE_RIGHT_VINCULUM) == SYN_OK ){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
            //printf("----------------EOF- TYPE = %d -------------\n",data.token.type);
            if (check_type( T_TYPE_EOF) == SYN_OK){
              return SYN_OK;
            }
          } 
          else{          
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
          }    
          //printf("---------------- 2 VOLAM BODY - TYPE = %d -------------\n",data.token.type);        
          if (check_type( T_TYPE_EOL) == SYN_ERR){
            return SYN_ERR;
          }
          //printf("---------------- 3 VOLAM BODY - TYPE = %d -------------\n",data.token.type);

          if ( check_type( T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
            //printf("----------------VOLAM EOL- TYPE = %d -------------\n",data.token.type);
            int exit_eol2 = eol();
            if (exit_eol2 != SYN_OK){ 
              return exit_eol2;
            }
            //printf("----------------VOLAM PROG- TYPE = %d -------------\n",data.token.type);
            int exit_prog = prog();
            if (exit_prog != SYN_OK){ 
                return exit_prog;
            }
            return SYN_OK;
          }

          return SYN_ERR;
        }
      }
      //printf("-------------------------------5.5 TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
      if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
        return SYN_ERR;
      }

      bt_stack_pop(&data.BT_stack);

      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("-------------------------------6. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);

      if (check_type( T_TYPE_EOL) == SYN_ERR ){
        return SYN_ERR;
      }
      //printf("----------------VOLAM EOL- TYPE = %d -------------\n",data.token.type);
      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      int exit_eol3 = eol();
      if (exit_eol3  != SYN_OK){ 
          return exit_eol3 ;
      }
      //printf("----------------VOLAM PROG- TYPE = %d -------------\n",data.token.type);
      int exit_prog2 = prog();
      if (exit_prog2 != SYN_ERR){ 
          return exit_prog2;
      }
  }
  //printf("----------------ELSE VETVA PROG- TYPE = %d ------------\n",data.token.type);

  // pravidlo <prog> -> epsilon
  return SYN_OK;
}

int body()
{
  //printf("----------------0. TOKEN BODY TYPE = %d -------------\n",data.token.type);
  if(data.token.type == T_TYPE_KEYWORD){
      //printf("----------------. TOKEN BODY MAM CASE kword-TYPE = %d -------------\n",data.token.type);
      //printf("----------------. TOKEN BODY MAM CASE kword JE-TYPE = %d -------------\n",check_keyword(KWORD_RETURN));
      // pravidlo <body> -> if <expression> { EOL <eol>  <body>} else { EOL <eol>  <body>} EOL <eol> <body>
      if (check_keyword(KWORD_IF) != SYN_ERR ){
          //printf("----------------1. TOKEN BODY MAM IF IF-TYPE = %d -------------\n",data.token.type);
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          int result_exp_if = expression(&data,&non_det);
          //printf("VYSLEDOK EXPRESSION VO <BODY> - IF = \"%d\"\n", result_exp_if);
          if (result_exp_if != SYN_OK){ 
            return result_exp_if;
          }

          //printf("----------------2. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }

          bt_stack_push(&data.BT_stack);

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------3. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }

          if (check_type(T_TYPE_EOL) == SYN_OK ){
            int exit_eol = eol();
            if (exit_eol != SYN_OK){ 
              return exit_eol;
            }
          }
  
          //printf("----------------4. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------VOLAM BODY po 4- TYPE = %d -------------\n",data.token.type);
            int exit_body = body();
            if (exit_body != SYN_OK){ 
              return exit_body;
            }
              
          }
          //printf("----------------5. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
          }

          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------5.25 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
            return SYN_ERR;
          }
          
          bt_stack_pop(&data.BT_stack);

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------5.5 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);

          if (check_keyword(KWORD_ELSE) == SYN_ERR ){
            return SYN_ERR;
          }

          bt_stack_push(&data.BT_stack);

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------6. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------7. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }

          if (check_type(T_TYPE_EOL) == SYN_OK ){
            int exit_eol2 = eol();
            if (exit_eol2 != SYN_OK){ 
              return exit_eol2;
            }
          }       
          //printf("----------------8. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------VOLAM BODY - TYPE = %d -------------\n",data.token.type);
            int exit_body2 = body();
            if (exit_body2 != SYN_OK){ 
              return exit_body2;
            }
          }
          //printf("----------------8.2 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
          }
          //printf("----------------8.5 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
                    
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }
          
          bt_stack_pop(&data.BT_stack);

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------9. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }

          if (check_type(T_TYPE_EOL) == SYN_OK ){
            int exit_eol3 = eol();
            if (exit_eol3 != SYN_OK){ 
              return exit_eol3;
            }
          }
          int exit_body3 = body();
          if (exit_body3 != SYN_OK){ 
            return exit_body3;
          }
          //printf("-------------------------------VYS TYPE = %d -------------\n",data.token.type);                   
          return SYN_OK;
      }

      // pravidlo <body> -> for <definition> ; <expression> ; <assignment> { EOL <eol>  <body> } EOL <eol>  <body>
      else if (check_keyword(KWORD_FOR) != SYN_ERR ){
          //printf("---------------- FOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR = %d -------------\n",data.token.type);
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }

          bt_stack_push(&data.BT_stack);

          //printf("----------------1 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
          int exit_definition = definition();
          if (exit_definition != SYN_OK){ 
              return exit_definition;
          }
          //printf("----------------1.5 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_OK ){
            return SYN_ERR;    
          }
          if (check_type(T_TYPE_SEMICOLON) == SYN_ERR ){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
          }
          
          //printf("----------------2 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_SEMICOLON) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------3 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
          int result_exp_for = expression(&data,&non_det);
          //printf("VYSLEDOK EXPRESSION VO <BODY> - FOR - EXPRESSION = \"%d\"\n", result_exp_for);
          if (result_exp_for != SYN_OK){ 
            return result_exp_for;
          }
          //printf("----------------5 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_SEMICOLON) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
            //printf("----------------5.5 VOLAM BODY - TYPE = %d -------------\n",data.token.type);
            int result_assignment = assignment();
            if (result_assignment != SYN_OK){ 
              return result_assignment;
            }
          }       
          //printf("----------------6. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }

          bt_stack_push(&data.BT_stack);

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------7. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }

          if (check_type(T_TYPE_EOL) == SYN_OK ){
            int exit_eol = eol();
            if (exit_eol != SYN_OK){ 
              return exit_eol;
            }
          }       
          //printf("----------------8. TOKEN body MAM FOR if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------VOLAM BODY - TYPE = %d -------------\n",data.token.type);
            int exit_body = body();
            if (exit_body != SYN_OK){ 
              return exit_body;
            }
          }
          //printf("----------------8.2 TOKEN body MAM FOR if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
          }
          //printf("----------------8.5 TOKEN body MAM FOR if-TYPE = %d -------------\n",data.token.type);
                    
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }
          
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------9. TOKEN body MAM FOR if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }

          if (check_type(T_TYPE_EOL) == SYN_OK ){
            int exit_eol2 = eol();
            if (exit_eol2 != SYN_OK){ 
              return exit_eol2;
            }
          }

          bt_stack_pop(&data.BT_stack);
          bt_stack_pop(&data.BT_stack);

          int exit_body2 = body();
          if (exit_body2 != SYN_OK){ 
            return exit_body2;
          }
          //printf("-------------------------------VYS TYPE = %d -------------\n",data.token.type);          
          return SYN_OK;
      }
      // pravidlo <body> ->  return <list_of_return_values> EOL <eol> <body>
      else if (check_keyword(KWORD_RETURN) != SYN_ERR ){
        //printf("----------------0. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (check_token() == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------1. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        int exit_list_of_return_values = list_of_return_values();
        //printf("----------------exit lorv = %d -------------\n",exit_list_of_return_values);
        if (exit_list_of_return_values != SYN_OK){ 
          return exit_list_of_return_values;
        } 
        //printf("----------------2. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        int exit_eol = eol();
        if (exit_eol != SYN_OK){ 
          return exit_eol;
        }  
        //printf("----------------3. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        int exit_body3 = body();
        if (exit_body3 != SYN_OK){ 
          return exit_body3;
        }
        
        //printf("----------------else. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
        return SYN_OK;
      }
  }

  else if(check_type(T_TYPE_IDENTIFIER) == SYN_OK){

      str_copy(data.token.attribute.string, temp_string);

      //printf("---------------- TOKEN IDENTIFIER TYPE = %d -------------\n",data.token.type);
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN IDENTIFIER TYPE = %d -------------\n",data.token.type);  
      int exit_ids = ids();
      if (check_type(T_TYPE_LEFT_BRACKET) != SYN_ERR ){
        // pravidlo <body> -> ID ( <argument> ) EOL <eol> <body>

        //funkcia main nemoze byt volana
        if(str_cmp_const_str(temp_string, "main") == 0) return SEM_ERR_OTHER;

        bool result_internal_error = false;
        Data_t *result_bt_search = BT_search(&data.BT_global, temp_string->str, &result_internal_error);
        if(result_internal_error == true){
            return ERROR_INTERNAL;
        }
        if(result_bt_search == NULL)
        {
            return SEM_ERR_UNDEFINED_VAR;
        }
        

        //printf("----------------0. TOKEN BODY MAM ID-TYPE = %d -------------\n",data.token.type);
        if (check_token() == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
          int exit_argument = argument();
          if (exit_argument != SYN_OK){ 
            return exit_argument;
          }
        }
        //printf("----------------2. TOKEN BODY MAM IF ID ( TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
          return SYN_ERR;
        }
        if (check_token() == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------3. TOKEN BODY MAM IF ID ( TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        int exit_eol = eol();
        if (exit_eol != SYN_OK){ 
          return exit_eol;
        }
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){

          //printf("----------------3. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          int exit_body = body();
          if (exit_body != SYN_OK){ 
            //printf("----------------4. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
            return exit_body;
          }
        }
        //printf("----------------else. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
        return SYN_OK;
        
      }
      else if (check_type(T_TYPE_VARIABLE_DEFINITION) != SYN_ERR ){
        // pravidlo <body> -> ID := <expression> EOL <eol> <body>
        
        tBT_stack_item* top_of_the_stack = bt_stack_top(&data.BT_stack);
        BT_insert(&top_of_the_stack->local_bt, temp_string->str, &internal_error);

        //kontrolne printiky
        printf("%s\n", temp_string->str);
        Print_tree(top_of_the_stack->local_bt.root_ptr);

        if (check_token() == LEX_ERR){
          return LEX_ERR;
        }          
          
        int result_exp_if = expression(&data,&non_det);
        //printf("VYSLEDOK EXPRESSION VO <BODY> - IF = \"%d\"\n", result_exp_if);
        if (result_exp_if != SYN_OK){ 
            return result_exp_if;
        }
        //printf("----------------1. VARIABLE TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
        }
        //printf("----------------2. VARIABLE TYPE  = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        //printf("----------------3. VARIABLE TYPE  = %d -------------\n",data.token.type);
        int exit_eol = eol();
        if (exit_eol != SYN_OK){ 
          return exit_eol;
        }
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
          //printf("----------------4. VARIABLE TYPE  = %d -------------\n",data.token.type);
          int exit_body = body();
          if (exit_body != SYN_OK){ 
            //printf("----------------5. VARIABLE TYPE  = %d -------------\n",data.token.type);        
            return exit_body;
          }
        }
        //printf("----------------else. VARIABLE TYPE  = %d -------------\n",data.token.type);        
        return SYN_OK;
      }
      // pravidlo <body> -> <ids> = ID(<argument>) EOL <eol> <body>
      // pravidlo <body> -> <ids> = <values> EOL <eol> <body>
      else if (exit_ids == SYN_OK){                                                            //////////////////////////////////////////////////////////////
        //printf("----------------0. BODY = TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_ASSIGN) == SYN_ERR){
          return SYN_ERR;
        }
        if (check_token() == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_EOL) == SYN_OK){
          return SYN_ERR;
        }
        //printf("----------------1. BODY = TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_IDENTIFIER) == SYN_OK){
          // pravidlo <body> -> <ids> = ID(<argument>) EOL <eol> <body>
          //printf("----------------1.5 BODY = TYPE = %d -------------\n",data.token.type);
          
          //main nemoze byt volany a nema ziadne navratove hodnoty, cize nemoze byt priradzovane do ziadneho ID
          //TODO
          
          non_det = true;
          int result_expr = expression(&data,&non_det);
          //printf("EXPR RESULT = TYPE = %d -------------\n",result_expr);
          if (non_det == false){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
            //printf("----------------2 BODY = TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK){
              
              if (check_token() == LEX_ERR){
                return LEX_ERR;
              }
              //printf("----------------2.5 BODY = TYPE = %d -------------\n",data.token.type);
              if (check_type(T_TYPE_EOL) == SYN_ERR ){
                  return SYN_ERR;
              }
              int exit_eol = eol();
              if (exit_eol != SYN_OK){ 
                return exit_eol;
              }
              //printf("----------------3. BODY = TYPE = %d -------------\n",data.token.type);
              if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
                //printf("----------------4.  BODY = TYPE = TYPE = %d -------------\n",data.token.type);
                int exit_body = body();
                if (exit_body != SYN_OK){ 
                  //printf("----------------5.  BODY = TYPE = TYPE = %d -------------\n",data.token.type);        
                  return exit_body;
                }
              }
              //printf("----------------6. BODY = TYPE = %d -------------\n",data.token.type);
              return SYN_OK;
            }
            //printf("----------------7. BODY = TYPE = %d -------------\n",data.token.type);
            int exit_argument = argument();
            if( exit_argument != SYN_OK){ 
              return exit_argument;
            }

            //printf("----------------8. BODY = TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
              return SYN_ERR;
            }          
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
            //printf("----------------9. BODY = TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_EOL) == SYN_ERR ){
              return SYN_ERR;
            }
            int exit_eol2 = eol();
            if (exit_eol2 != SYN_OK){ 
              return exit_eol2;
            }
            //printf("----------------9.5 BODY = TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_EOF) == SYN_OK ){
              return SYN_OK;
            }
            //printf("----------------10. BODY = TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
              //printf("---------------11. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
              int exit_body2 = body();
              if (exit_body2 != SYN_OK){ 
                //printf("----------------12. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
                return exit_body2;
              }
            }
            //printf("----------------70. BODY = TYPE = %d -------------\n",data.token.type);
            return SYN_OK;          
          }
          // pravidlo <body> -> <ids> = <expression>,<values_n> EOL <eol> <body>
          else{
            non_det = false;
            //printf("----------------0. TOKEN EXPRESSION,VALUE NEXT TYPE = %d -------------\n",data.token.type);
            if (result_expr != SYN_OK){
              return result_expr;
            }
            //printf("----------------1. TOKEN EXPRESSION,VALUE NEXT TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_EOL) == SYN_OK ){
              return SYN_OK;
            } 
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
            //printf("----------------2. TOKEN EXPRESSION,VALUE NEXT TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_COMMA) == SYN_ERR ){ 
              return SYN_ERR;
            }
            int exit_values_n = values_n();
            if (exit_values_n != SYN_OK){
              return exit_values_n;
            }
            //printf("----------3. TOKEN EXPRESSION,VALUE NEXTTYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_EOL) == SYN_ERR ){
              return SYN_ERR;
            }
            int exit_eol = eol();
            if (exit_eol != SYN_OK){ 
              return exit_eol;
            }  
            //printf("-------4. TOKEN EXPRESSION,VALUE NEXTTYPE = %d -------------\n",data.token.type);
            int exit_body = body();
            if (exit_body != SYN_OK){ 
              //printf("----------------4. TOKEN EXPRESSION,VALUE NEXTTYPE = %d -------------\n",data.token.type);        
              return exit_body;
            }            
            return SYN_OK;
          }

          //printf("expression = TYPE = %d -------------\n",data.token.type);
          
        }
        else{
          // pravidlo <body> -> <ids> = <expression>,<values_n> EOL <eol> <body> ???????
          //printf("---------------- -0. else if = TYPE = %d -------------\n",data.token.type);
          int tmp_vys = values();
          //printf("---------------- -0. TYPE = %d -------------\n",tmp_vys);
          if (tmp_vys == SYN_OK ){
            //printf("---------------- -0. else if = TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_EOL) == SYN_ERR ){
              return SYN_ERR;
            }
            //printf("----------------1. else if = TYPE = %d -------------\n",data.token.type);
            int exit_eol = eol();
            if (exit_eol != SYN_OK){ 
              return exit_eol;
            }
            //printf("----------------1.5 else if = TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
              //printf("---------------2. else if-TYPE = %d -------------\n",data.token.type);
              int exit_body = body();
              if (exit_body != SYN_OK){ 
                //printf("----------------3. else if-TYPE = %d -------------\n",data.token.type);        
                return exit_body;
              }
              
              if (check_type(T_TYPE_EOF) == SYN_OK ){
                //printf("----------------3.5 else if-TYPE = %d -------------\n",data.token.type);
                return SYN_OK;
              }
              //printf("----------------4. else if-TYPE = %d -------------\n",data.token.type); 
              return SYN_OK;
            }     
          
          }
          else if(tmp_vys != SYN_OK){
            return tmp_vys;
          }
        }
        
        
        //printf("----------------SYN OK-TYPE = %d -------------\n",data.token.type);
      } 
      else if (exit_ids != SYN_OK){
        return exit_ids;
      }
      return SYN_OK;    
  }

 
  else{
    //printf("KONIEC\n");
    return SYN_OK;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
  return SYN_OK;
}

int definition()
{
  // pravidlo <definition> -> ID := <expression>

  //printf("----------------0. TOKEN DEFINITION TYPE = %d -------------\n",data.token.type);
  if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
      
      tBT_stack_item* top_of_the_stack = bt_stack_top(&data.BT_stack);
      BT_insert(&top_of_the_stack->local_bt, data.token.attribute.string->str, &internal_error);
    

      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN DEFINITION TYPE = %d -------------\n",data.token.type);
      if (check_type(T_TYPE_VARIABLE_DEFINITION) == SYN_ERR ){
        return SYN_ERR;
      }
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------2. TOKEN DEFINITION TYPE = %d -------------\n",data.token.type);
      
      int result_exp = expression(&data,&non_det);
      //printf("VYSLEDOK EXPRESSION VO <VALUES_N> = %d\n", result_exp);
      if( result_exp != SYN_OK ){
        return result_exp;
      }
      //printf("----------------VYS DEFINITION TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
  }
  
  // pravidlo <definition> -> epsilon
  else{
    //printf("----------------VYS DEFINITION TYPE = %d -------------\n",data.token.type);
    return SYN_OK;
  }

}

int assignment()
{
  // pravidlo <assignment> -> ID = <expression>

//intf("----------------0. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
  if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
    if (check_token() == LEX_ERR){
      return LEX_ERR;
    }
    //printf("----------------1. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
    if (check_type(T_TYPE_ASSIGN) == SYN_ERR ){
        return SYN_ERR;
    }
    if (check_token() == LEX_ERR){
      return LEX_ERR;
    }
    //printf("----------------2. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
    int result_assignment = expression(&data,&non_det);
    //printf("VYSLEDOK EXPRESSION VO <ASSIGNMENT> = \"%d\"\n", result_assignment);
    if (result_assignment != SYN_OK){ 
         return result_assignment;
    }
    //printf("----------------3. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
    return SYN_OK;
  }
  else{
    // pravidlo <assignment> -> epsilon
    return SYN_OK;
  }

}

int ids()
{
  // pravidlo <ids> -> ID <id_n>
  //printf("----------------0. IDS TYPE = %d -------------\n",data.token.type);
  if (check_type(T_TYPE_COMMA) == SYN_OK ){
    int exit_id_n = id_n();
    if (exit_id_n != SYN_OK){ 
      //printf("----------------2. IDS TYPE = %d -------------\n",data.token.type);
      return exit_id_n;
    }
    //printf("----------------3. IDS TYPE = %d -------------\n",data.token.type);
    return SYN_OK;
  }
  //printf("----------------4. IDS TYPE = %d -------------\n",data.token.type);
  return SYN_OK;
}

int id_n()
{
  // pravidlo <id_n> -> , ID <id_n>
  //printf("----------------0. IDS NEXT TYPE = %d -------------\n",data.token.type);
  if( check_type(T_TYPE_COMMA) != SYN_ERR ) {
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. IDS NEXT TYPE = %d -------------\n",data.token.type);
      if (check_type(T_TYPE_IDENTIFIER) == SYN_ERR ){
        return SYN_ERR;
      }
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------2. IDS NEXT TYPE = %d -------------\n",data.token.type);
      if (check_type(T_TYPE_ASSIGN) == SYN_OK ){
        return SYN_OK;
      }
      if (check_type(T_TYPE_COMMA) == SYN_OK ){
        int exit_id_n =id_n();
        if (exit_id_n == SYN_OK){ 
          //printf("----------------3. IDS NEXT TYPE = %d -------------\n",data.token.type);
          return id_n();
        }
        else{
          return exit_id_n;
        }
      }
  }

  // pravidlo <id_n> -> epsilon
  else{
    //printf("----------------4. IDS NEXT TYPE = %d -------------\n",data.token.type);
    return SYN_OK;
  }
  return SYN_OK;
}

int list_of_return_values()
{
  // pravidlo <list_of_return_values> -> <values>
  //printf("----------------LORV TYPE = %d -------------\n",data.token.type);
  int exit_values = values();
  if( exit_values != SYN_OK ){
    //printf("----------------SYN ERR LORV TYPE = %d -------------\n",exit_values);
    // to do tu sa to jebe  pri rekurzii 
    return exit_values;
  }

  // pravidlo <list_of_return_values> -> epsilon
  //printf("----------------SYN OK LORV TYPE = %d -------------\n",data.token.type);
  return SYN_OK;
}
int values()
{
  // pravidlo <values> -> <expression> <values_n>
  //printf("----------------0. VALUES TYPE = %d -------------\n",data.token.type);
  if( check_type(T_TYPE_EOL) != SYN_ERR ) {
      return SYN_OK;
  }
  int result_exp = expression(&data,&non_det);
  //printf("VYSLEDOK EXPRESSION VO <VALUES> = %d\n", result_exp);
  //printf("TOKEN Z EXPRESSION TYPE = %d -------------\n",data.token.type);
  if( result_exp != SYN_ERR ){
    //printf("----------------0.7 VALUES TYPE = %d -------------\n",data.token.type);
    if( check_type(T_TYPE_RIGHT_VINCULUM) == SYN_OK ) {
      return SYN_ERR;
    }


    if( check_type(T_TYPE_EOL) != SYN_ERR ) {
      return SYN_OK;
    }

    //printf("----------------1. VALUES TYPE = %d -------------\n",data.token.type);
    int vys_values_n = values_n();
    //printf("----------------VYS VN VALUES TYPE = %d -------------\n",vys_values_n);
    if ( vys_values_n != SYN_OK){ 
      //printf("----------------1.5 VALUES TYPE = %d -------------\n",vys_values_n);
      //bad_returns = false;
      return vys_values_n;
    }
    //printf("----------------exit VALUES TYPE = %d -------------\n",data.token.type);
    if (bad_returns == false){
      return SYN_OK;
    }
    
  }
  else{
    //printf("--------------RESULT VALUES TYPE = %d -------------\n",data.token.type);
    return result_exp;
  }
  //printf("--------------syn err VALUES TYPE = %d -------------\n",data.token.type);
  return SYN_ERR;
}

int values_n()
{
    // pravidlo <values_n> -> ,<expression> <values_n>
    if (bad_returns == true){
      return SYN_ERR;
    }
    //printf("----------------0 VALUES NEXT TYPE = %d -------------\n",data.token.type);
    //int tmp;
    if (check_type(T_TYPE_COMMA) != SYN_ERR ){
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1 VALUES NEXT TYPE = %d -------------\n",data.token.type);
      int result_exp = expression(&data,&non_det);
      //printf("VYSLEDOK EXPRESSION VO <VALUES_N> = %d\n", result_exp);
      if( result_exp != SYN_OK ){
        //printf("---------------- BAD RETURNS = %d -------------\n",result_exp);
        if (result_exp == SYN_ERR){
          bad_returns = true;
        }
        return result_exp;
      }
      //printf("----------------1.5 VALUES NEXT TYPE = %d -------------\n",data.token.type);
      if (check_type(T_TYPE_EOL) == SYN_OK ){
        //printf("----------------2 VALUES NEXT TYPE = %d -------------\n",data.token.type);
        return SYN_OK;
      }

      if (check_type(T_TYPE_COMMA) == SYN_OK ){
          if (bad_returns == false){ 
            //printf("----------------VOLAAAAAAAAAAAAAAAAAAAAAAAA VALUES NEXT TYPE = %d -------------\n",data.token.type);
            values_n();
          }
          else{
            return SYN_ERR;
          }
      }
      //printf("----------------BAD RETURNS TYPE = %d -------------\n",bad_returns);
      if (bad_returns == true){
        return SYN_ERR;
      }    
      //printf("----------------EXIT VALUES NEXT TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
    }

    else if (check_type(T_TYPE_EOL) == SYN_OK ){
      return SYN_OK;
      //printf("----------------ELSE IF VALUES NEXT TYPE = %d -------------\n",data.token.type);
    }
    else{
      //printf("----------------ELSE VALUES NEXT TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
    }

}

int params()
{
  // pravidlo <params> -> ID <type> <params_n>
  //printf("----------------0. TOKEN PARAMS TYPE = %d -------------\n",data.token.type);
  if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
    
    tBT_stack_item* top_of_the_stack = bt_stack_top(&data.BT_stack);
    BT_insert(&top_of_the_stack->local_bt, data.token.attribute.string->str, &internal_error);
    
    if (check_token() == LEX_ERR){
      return LEX_ERR;
    }
    //printf("----------------1. TOKEN PARAMS TYPE = %d -------------\n",data.token.type);
    int exit_type = type();
    if (exit_type != SYN_OK){ 
      return exit_type;
    }
    //printf("----------------2. TOKEN PARAMS TYPE = %d -------------\n",data.token.type);
    if (check_token() == LEX_ERR){
      return LEX_ERR;
    }
    //printf("----------------3. TOKEN PARAMS TYPE = %d -------------\n",data.token.type);
    int exit_params_n = params_n();
    if (exit_params_n != SYN_OK){ 
      return exit_params_n;
    }
    //printf("----------------4. TOKEN PARAMS TYPE = %d -------------\n",data.token.type);
    return SYN_OK;
  }
  return SYN_OK;

  // pravidlo <params> -> epsilon

}

int params_n()
{
  // pravidlo <params_n> -> , ID <type> <params_n>
  

  //printf("----------------0. TOKEN PARAMSNEXT TYPE = %d -------------\n",data.token.type);
  if ( check_type(T_TYPE_COMMA) != SYN_ERR ){
    if (check_token() == LEX_ERR){
      return LEX_ERR;
    }
    //printf("----------------1. TOKEN PARAMSNEXT TYPE = %d -------------\n",data.token.type);
    if (check_type(T_TYPE_IDENTIFIER) == SYN_ERR ){
      return SYN_ERR;
    }

    tBT_stack_item* top_of_the_stack = bt_stack_top(&data.BT_stack);
    //kontrola ci IDcka vstupnych parametrov nie su rovnake
    if (BT_insert(&top_of_the_stack->local_bt, data.token.attribute.string->str, &internal_error) == NULL) return SEM_ERR_UNDEFINED_VAR;

    if (check_token() == LEX_ERR){
      return LEX_ERR;
    }
    //printf("----------------2. TOKEN PARAMSNEXT TYPE = %d -------------\n",data.token.type);
    int exit_type = type();
    if (exit_type != SYN_OK){ 
      return exit_type;
    }
    if (check_token() == LEX_ERR){
      return LEX_ERR;
    }
    //printf("----------------3. TOKEN PARAMSNEXT TYPE = %d -------------\n",data.token.type);
    if ( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK ){
      //printf("  ----> SYN OK argn TYPE = %d  <---- \n",data.token.type);
      return SYN_OK;
    }
    if (check_type(T_TYPE_COMMA) == SYN_OK ){  
      int exit_params_n = params_n();  
      if (exit_params_n == SYN_OK){ 
        return params_n();
      }
      else{
        return exit_params_n; 
      }

    }
    //printf("----------------4. TOKEN PARAMSNEXT TYPE = %d -------------\n",data.token.type);
    
  }
  else{
    return SYN_OK;
  }
  // pravidlo <params_n> ->epsilon

}

int return_value()
{
  // pravidlo <return_value> -> ( <type> <return_value_n> )
  //printf("----------------0. TOKEN RV-TYPE = %d -------------\n",data.token.type);
  if ( check_type(T_TYPE_LEFT_BRACKET) != SYN_ERR ){
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN RV-TYPE = %d -------------\n",data.token.type);
      int exit_type = type();  
      if (exit_type != SYN_OK){ 
        return exit_type;
      }
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------2. TOKEN RV-TYPE = %d -------------\n",data.token.type);
      int exit_return_value_n = return_value_n();
      if (exit_return_value_n != SYN_OK){ 
        return exit_return_value_n;
      }
      //printf("----------------3. TOKEN RV-TYPE = %d -------------\n",data.token.type);
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------4. TOKEN RV-TYPE = %d -------------\n",data.token.type);   
      if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
        return SYN_ERR;
      }
      return SYN_OK;
  }
  return SYN_OK;
  
  // pravidlo <return_value> -> epsilon
}

int return_value_n()
{
  // pravidlo <return_value_n> -> , <type> <return_value_n>

  //printf("----------------0. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
  if (check_type(T_TYPE_COMMA) != SYN_ERR ){
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK ){
        //printf("  ----> %d  <---- \n",check_type(T_TYPE_RIGHT_BRACKET));
        return SYN_ERR;
      }
      int exit_type = type();
      if (exit_type != SYN_OK){ 
        return exit_type;
      }
      //printf("----------------2. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------3. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      int exit_return_value_n = return_value_n();
      if (exit_return_value_n == SYN_OK){ 
        //printf("----------------3.5 TOKEN RVN-TYPE = %d -------------\n",data.token.type);
        return_value_n();
      }
      else{
        return exit_return_value_n;
      }
      //printf("----------------4. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
  }
  
  // pravidlo <return_value_n> -> epsilon
  else if( check_type(T_TYPE_RIGHT_BRACKET) != SYN_ERR ){
    //printf("  ----> %d  <---- \n",check_type(T_TYPE_RIGHT_BRACKET));
    return SYN_OK;
  }
  return SYN_OK;

}

int type()
{
  // pravidlo <type> -> int
  // pravidlo <type> -> float
  // pravidlo <type> -> string

  if ( ( check_keyword(KWORD_INT) != SYN_ERR ) || 
       ( check_keyword(KWORD_FLOAT64) != SYN_ERR )  ||
       ( check_keyword(KWORD_STRING) != SYN_ERR )) {
      //printf("  ----> SOM V TYPE OK = %d  <---- \n",data.token.type);
      
      
      return SYN_OK;
  }
  //printf("  ----> SOM V TYPE ERR = %d  <---- \n",data.token.type);
  return SYN_ERR;
}

int argument()
{
  // pravidlo <argument> -> <value> <argument_n>
  //printf("  ---->0. ARGUMENT TYPE = %d  <---- \n",data.token.type);
  int exit_value = value();
  if ( exit_value == SYN_OK){
    if (check_token() == LEX_ERR){
     return LEX_ERR;
    } 
    if ( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK ){
      //printf("  ----> SYN OK arg TYPE = %d  <---- \n",data.token.type);
      return SYN_OK;
    }
    //printf("  ---->1. ARGUMENT TYPE = %d  <---- \n",data.token.type);
    int exit_argument_n = argument_n();
    if (exit_argument_n != SYN_OK){ 
      //printf("  ---->1.5 ARGUMENT TYPE = %d  <---- \n",data.token.type);
      return exit_argument_n;
    }

    return SYN_OK;
  }
  else {
    return exit_value;
  }
  //printf("  ---->ziadny arg ARGUMENT TYPE = %d  <---- \n",data.token.type);
  // pravidlo <argument> -> epsilon
  return SYN_OK;
}

int argument_n()
{
  // pravidlo <argument_n> -> ,<value> <argument_n>
  //printf("  ---->0. ARGUMENT_N TYPE = %d  <---- \n",data.token.type);
  if ( check_type(T_TYPE_COMMA) != SYN_ERR ){
    if (check_token() == LEX_ERR){
     return LEX_ERR;
    } 
    //printf("  ---->1. ARGUMENT_N TYPE = %d  <---- \n",data.token.type);
    int exit_value = value();
    if (exit_value != SYN_OK){ 
      //printf("  ---->1.5 ARGUMENT_N TYPE = %d  <---- \n",data.token.type);
      return exit_value;
    }
    if (check_token() == LEX_ERR){
     return LEX_ERR;
    } 
    if ( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK ){
      //printf("  ----> SYN OK argn TYPE = %d  <---- \n",data.token.type);
      return SYN_OK;
    }

   // printf("  ---->2. ARGUMENT_N TYPE = %d  <---- \n",data.token.type);

    if ( check_type(T_TYPE_COMMA) == SYN_OK ){
      int exit_argument_n = argument_n();
      if (exit_argument_n == SYN_OK){ 
        return argument_n();
      }
      else{
        return exit_argument_n;
      }
    }    

  }
  else{
    // pravidlo <argument_n> -> epsilon
    //printf("  ---->3. ARGUMENT_N TYPE = %d  <---- \n",data.token.type);
    return SYN_OK;
  }
  return SYN_OK;
}

int value()
{
  // pravidlo <value> -> INT_VALUE
  // pravidlo <value> -> FLOAT_VALUE
  // pravidlo <value> -> STRING_VALUE
  // pravidlo <value> -> ID
  //printf("----------------0. TOKEN VALUE TYPE = %d -------------\n",data.token.type);
  if ( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK){
    return SYN_ERR;
  } 

  if ( ( check_type(T_TYPE_INTEGER) != SYN_ERR ) || 
       ( check_type(T_TYPE_DOUBLE) != SYN_ERR )  ||
       ( check_type(T_TYPE_STRING) != SYN_ERR )  ||
       ( check_type(T_TYPE_IDENTIFIER) != SYN_ERR )  ) {
      //printf("----------------1. TOKEN VALUE TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
  }
  else{ 
    //printf("----------------2. TOKEN VALUE TYPE = %d -------------\n",data.token.type);
    return SYN_ERR;
  }
}


bool init_variables()
{
    data.BT_global.definded = false;  //som retard a musim to spravit
    if(BT_init(&data.BT_global) == false) return false;

    bt_stack_init(&data.BT_stack);
    
    data.token.attribute.string = malloc(sizeof(struct str_struct));
    if(data.token.attribute.string == NULL) return ERROR_INTERNAL;
    if(str_init(data.token.attribute.string) == false) return false;
    
    temp_string = malloc(sizeof(struct str_struct));
    if(temp_string == NULL) return ERROR_INTERNAL;
    if(str_init(temp_string) == false) return false;

    func_id = malloc(sizeof(struct str_struct));
    if(func_id == NULL) return ERROR_INTERNAL;
    if(str_init(func_id) == false) return false;

    return true;
}

void free_variables()
{
    BT_dispose(&data.BT_global);
    bt_stack_free(&data.BT_stack);
    
    str_clear(data.token.attribute.string);
    str_free(data.token.attribute.string);
    if((data.token.attribute.string) != NULL) free(data.token.attribute.string);

    str_clear(temp_string);
    str_free(temp_string);
    if((temp_string) != NULL) free(temp_string);

    str_clear(func_id);
    str_free(func_id);
    if((func_id) != NULL) free(func_id);
}

int parse()
{
    if(init_variables() == false)
    {
        free_variables();
        return ERROR_INTERNAL;
    }
    
    if((result = get_token(&data.token)) == LEX_TOKEN_OK)
    {
        result = start(&data);
        bad_returns = false;
        Print_tree(data.BT_global.root_ptr);
        //funkcia s ID main musi byt obsiahnuta
        //if(BT_search(&data.BT_global, "main", &internal_error) == NULL) result = SEM_ERR_UNDEFINED_VAR;
    }
    //uvolnenie pamate 
    free_variables();
    //vratenie exit code
    return result;
} 


