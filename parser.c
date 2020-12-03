/*
//IFJ20 - projekt(varianta I.)
//parser.c
//Matus Tvarozny, xtvaro00
//Filip Brna, xbrnaf00
//Matej Hornik, xhorni20
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

struct str_struct called_func;

struct str_struct *func_id;

bool internal_error = false;
bool non_det = false;
bool bad_returns = false;
bool saving_return_types = false;
bool return_included = false;
bool set_param_type = false;
bool saving_arguments = false;

Data_t *actual_parameter = NULL;


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
      
      Data_t *func_insert_global = BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
      data.actual_func = func_insert_global;
      if (internal_error == true) return ERROR_INTERNAL;
      if (func_insert_global == NULL){
        return SEM_ERR_UNDEFINED_VAR;
      }

      bt_stack_push(&data.BT_stack);
      
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
        set_param_type = true;
        int exit_params = params();
        set_param_type = false;
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
        saving_return_types = true;
        int exit_return_value = return_value();
        saving_return_types = false;
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
            //funkcia, ktora ma return values, musi obsahovat return
            if((data.actual_func->func_params.top) != NULL && return_included == false) return SEM_ERR_OTHER;
            return_included = false;

            bt_stack_pop(&data.BT_stack);
            data.actual_func = NULL;
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
      //funkcia, ktora ma return values, musi obsahovat return
      if((data.actual_func->func_params.top) != NULL && return_included == false) return SEM_ERR_OTHER;


      return_included = false;

      //konci funkcia tak popnem stack frame
      bt_stack_pop(&data.BT_stack);
      data.actual_func = NULL;

      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("-------------------------------6. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
      
      if (check_type( T_TYPE_EOF) == SYN_OK ){
        return SYN_OK;
      }


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

          //pushnem novy frame pre premmenne
          bt_stack_push(&data.BT_stack);


          data.in_if_for = true;
          int result_exp_if = expression(&data,&non_det);
          //printf("VYSLEDOK EXPRESSION VO <BODY> - IF = \"%d\"\n", result_exp_if);
          if (result_exp_if != SYN_OK){ 
            return result_exp_if;
          }
          data.in_if_for = false;

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

          //vychadzam z ifu tak popnem jeden frame zo stacku pre premenne
          bt_stack_pop(&data.BT_stack);
          
          bt_stack_pop(&data.BT_stack);

          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------5.5 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);

          if (check_keyword(KWORD_ELSE) == SYN_ERR ){
            return SYN_ERR;
          }  

          //vchadzam do elsu tak pushnem novy stack na premenne
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

          //vychadzam z elsu tak popnem stack pre premmenne
          bt_stack_pop(&data.BT_stack);
          
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

          //vytvorim si temporary frame pre for defiuniton(1. vo fore)
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

          data.in_if_for = true;

          //printf("----------------3 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
          int result_exp_for = expression(&data,&non_det);
          //printf("VYSLEDOK EXPRESSION VO <BODY> - FOR - EXPRESSION = \"%d\"\n", result_exp_for);
          if (result_exp_for != SYN_OK){ 
            return result_exp_for;
          }

          data.in_if_for = false;

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

          //som vo fore pushnem si novy stack frame (2. vo fore)
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
            
            // //konci for tak popnem 2 stack framy
            // bt_stack_pop(&data.BT_stack);
            // bt_stack_pop(&data.BT_stack);

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

          //vychadzam z foru tak popnem 2 stack framy
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
        //return je obsiahnuty
        return_included = true;
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

      //pushnueme si do queue id
      tID_queue_item *top_queue = id_queue_push(&data.ID_queue);
      //vyplnim to id
      str_copy(data.token.attribute.string, &top_queue->id);


      //printf("---------------- TOKEN IDENTIFIER TYPE = %d -------------\n",data.token.type);
      if (check_token() == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN IDENTIFIER TYPE = %d -------------\n",data.token.type);  
      int exit_ids;
      if (check_type(T_TYPE_LEFT_BRACKET) != SYN_ERR ){
        // pravidlo <body> -> ID ( <argument> ) EOL <eol> <body>

        //funkcia main nemoze byt volana
        if(str_cmp_const_str(&top_queue->id, "main") == 0) return SEM_ERR_OTHER;

		//ak je definovana premena nad tym
		Data_t *found = bt_stack_search(&data.BT_stack, top_queue->id.str, &internal_error);
		if (found != NULL){
			return SEM_ERR_OTHER;
		}

        
		//ulozim si toto volanie funkcie
		tFunc_calls *first_in_ll = data.check_func_calls;
		tFunc_calls *new_item_ll = malloc(sizeof(struct func_calls));
		if (new_item_ll == NULL){
			return ERROR_INTERNAL;
		}
		id_queue_init(&new_item_ll->ls);
		id_queue_init(&new_item_ll->rs);
		str_init(&new_item_ll->called_func);
		//nakopirujeme nazov volanej funkcie
		tID_queue_item *top_queue = id_queue_top(&data.ID_queue);
      	str_copy(&top_queue->id, &new_item_ll->called_func);
		//pripojim novu polozku k linked listu
		new_item_ll->next = first_in_ll;
		data.check_func_calls = new_item_ll;





        //printf("----------------0. TOKEN BODY MAM ID-TYPE = %d -------------\n",data.token.type);
        
        //popnem z queue pretoze tento id je volanie funkcie
        id_queue_pop(&data.ID_queue);

        if (check_token() == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
			saving_arguments = true;
			int exit_argument = argument();
			saving_arguments = false;
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



        //checknem ci este id neni definovane
        tBT_stack_item *top_bt_stack = bt_stack_top(&data.BT_stack);
        tID_queue_item *top_queue_def = id_queue_top(&data.ID_queue);
        hKey_t tmp = "_";
        if (str_cmp_const_str(&top_queue_def->id, tmp) == 0){
            return SEM_ERR_OTHER;
        }
        Data_t *search_found = BT_search(&top_bt_stack->local_bt, top_queue_def->id.str, &internal_error);
        if (internal_error == true){
          return ERROR_INTERNAL;
        }
        if (search_found != NULL){
          return SEM_ERR_UNDEFINED_VAR;
        }

        BT_insert(&top_bt_stack->local_bt, top_queue_def->id.str, &internal_error);
        if (internal_error == true){
          return ERROR_INTERNAL;
        }
        

        if (check_token() == LEX_ERR){
          return LEX_ERR;
        }          
        
        data.set_type_id = true;
        int result_exp_if = expression(&data,&non_det);
        //printf("VYSLEDOK EXPRESSION VO <BODY> - IF = \"%d\"\n", result_exp_if);
        if (result_exp_if != SYN_OK){ 
            return result_exp_if;
        }
        data.set_type_id = false;

        //popnem id z queue pretoze uz som nastavil jeho typ v expression
        id_queue_pop(&data.ID_queue);

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
      else if( (exit_ids = ids()) == SYN_OK){                                                  
        //////////////////////////////////////////////////////////////
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

		  //ulozim si nazov funckie
		  str_clear(&called_func);
		  str_add_const_str(&called_func, data.token.attribute.string->str);
          
          non_det = true;

          data.check_type = true;
          int result_expr = expression(&data,&non_det);
          data.check_type = false;

          //printf("EXPR RESULT = TYPE = %d -------------\n",result_expr);
          if (non_det == false){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }


			//funckia main nemoze byt volana
			if(str_cmp_const_str(&called_func, "main") == 0) return SEM_ERR_OTHER;

			//nemoze byt uz definovana dana premena s rovnakym id
			Data_t *found = bt_stack_search(&data.BT_stack, called_func.str, &internal_error);
			if (found != NULL){
				return SEM_ERR_OTHER;
			}

            //tu treba si poznacit pri volani danej funkcie ci vracia dane typy ake su na lavej strane
			//ulozim si toto volanie funkcie
			tFunc_calls *first_in_ll = data.check_func_calls;
			tFunc_calls *new_item_ll = malloc(sizeof(struct func_calls));
			if (new_item_ll == NULL){
				return ERROR_INTERNAL;
			}
			id_queue_init(&new_item_ll->ls);
			id_queue_init(&new_item_ll->rs);
			str_init(&new_item_ll->called_func);
			//nakopirujeme nazov volanej funkcie
			str_copy(&called_func, &new_item_ll->called_func);
			//pripojim novu polozku k linked listu
			new_item_ll->next = first_in_ll;
			data.check_func_calls = new_item_ll;
			//nakopirujem lavu stranu(jej typy)
			tID_queue_item *tmp = data.ID_queue.top;
			while (tmp != NULL){
				tID_queue_item *new_item = id_queue_push(&new_item_ll->ls);

				if (str_cmp_const_str(&tmp->id, "_") == 0){
					str_add_const_str(&new_item->id, "_");
					break;
				}

				Data_t *found = bt_stack_search(&data.BT_stack, tmp->id.str, &internal_error);
				if (found == NULL){
					return SEM_ERR_UNDEFINED_VAR;
				}
				if (found->type == TYPE_INT){
					hKey_t tmp = "int";
					str_add_const_str(&new_item->id, tmp);
				}
				else if (found->type == TYPE_DOUBLE){
					hKey_t tmp = "double";
					str_add_const_str(&new_item->id, tmp);
				}
				else if (found->type == TYPE_STRING){
					hKey_t tmp = "string";
					str_add_const_str(&new_item->id, tmp);
				}
				tmp = tmp->next;
			}

            id_queue_free(&data.ID_queue);
            id_queue_init(&data.ID_queue);


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
			saving_arguments = true;
            int exit_argument = argument();
			saving_arguments = false;
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

            //popnem uz jedno id z queue ktore bolo skontrolovane
            id_queue_pop(&data.ID_queue);


            //printf("----------------0. TOKEN EXPRESSION,VALUE NEXT TYPE = %d -------------\n",data.token.type);
            if (result_expr != SYN_OK){
              return result_expr;
            }
            //printf("----------------1. TOKEN EXPRESSION,VALUE NEXT TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_EOL) == SYN_OK ){
              return SYN_OK;
            } 
            //if (check_token() == LEX_ERR){
            //  return LEX_ERR; //////////////////////////toto bolo zakomentovane za ucelom fixu 10. test-sem
            //}
            //printf("----------------2. TOKEN EXPRESSION,VALUE NEXT TYPE = %d -------------\n",data.token.type);
            if (check_type(T_TYPE_COMMA) == SYN_ERR ){ 
              return SYN_ERR;
            }
            int exit_values_n = values_n();
            //printf("----------------2. TOKEN EXPRESSION,VALUE NEXT TYPE = %d -------------\n",exit_values_n);
            if (exit_values_n != SYN_OK){
              return exit_values_n;
            }


            //ak fronta nieje prazdna, musi byt na lavej strane viac idciek ako vyrazov na pravej
            if (id_queue_top(&data.ID_queue) != NULL){
              return SEM_ERR_OTHER;
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
          //return SYN_OK;
          //printf("expression = TYPE = %d -------------\n",data.token.type);
          
        }
        else{
          // pravidlo <body> -> <ids> = <expression>,<values_n> EOL <eol> <body> ???????
          //printf("---------------- -0. else if = TYPE = %d -------------\n",data.token.type);
          int tmp_vys = values();
          //printf("---------------- -0. TYPE = %d -------------\n",tmp_vys);

          //ak fronta nieje prazdna, musi byt na lavej strane viac idciek ako vyrazov na pravej
          if (id_queue_top(&data.ID_queue) != NULL){
            return SEM_ERR_OTHER;
          }

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

      hKey_t tmp = "_";
      if (str_cmp_const_str(data.token.attribute.string, tmp) == 0){
          return SEM_ERR_OTHER;
      }
      
      tID_queue_item *top_queue = id_queue_push(&data.ID_queue);

      str_copy(data.token.attribute.string, &top_queue->id);

      tBT_stack_item *top_bt_stack = bt_stack_top(&data.BT_stack);
      Data_t *search_found = BT_insert(&top_bt_stack->local_bt, top_queue->id.str, &internal_error);
      if (internal_error == true){
        return ERROR_INTERNAL;
      }

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
      
      data.set_type_id = true;
      int result_exp = expression(&data,&non_det);
      data.set_type_id = false;
      
      //printf("VYSLEDOK EXPRESSION VO <VALUES_N> = %d\n", result_exp);
      if( result_exp != SYN_OK ){
        return result_exp;
      }

      id_queue_pop(&data.ID_queue);

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

    tID_queue_item *top_queue = id_queue_push(&data.ID_queue);

    str_copy(data.token.attribute.string, &top_queue->id);


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

    data.check_type = true;
    int result_assignment = expression(&data,&non_det);
    data.check_type = false;
    
    //printf("VYSLEDOK EXPRESSION VO <ASSIGNMENT> = \"%d\"\n", result_assignment);
    if (result_assignment != SYN_OK){ 
         return result_assignment;
    }

    id_queue_pop(&data.ID_queue);

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

      //pushujem si idcka do fronty
      tID_queue_item *top_queue = id_queue_push(&data.ID_queue);
      str_copy(data.token.attribute.string, &top_queue->id); 

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

  data.check_returns = true;
  int exit_values = values();
  data.check_returns = false;

  //ak bolo malo vyrazov v returne ako je navreatovych hodnot funkcie vratim error
  //nastavim spat pocitadlo checknutych returnov
  
  if( exit_values != SYN_OK ){
    //printf("----------------SYN ERR LORV TYPE = %d -------------\n",exit_values);
    // to do tu sa to jebe  pri rekurzii 
    return exit_values;
  }

  if (data.checked_returns != data.actual_func->no_ret_values){
    return SEM_ERR_NO_PARAMS;
  }
  data.checked_returns = 0;

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

  data.check_type = true;
  
  int result_exp = expression(&data,&non_det);
  data.check_type = false;

  if (data.check_returns == true){
    data.checked_returns++;
  }

  //popnem uz 1 expression checknuty
  id_queue_pop(&data.ID_queue);


  //printf("VYSLEDOK EXPRESSION VO <VALUES> = %d\n", result_exp);
  //printf("TOKEN Z EXPRESSION TYPE = %d -------------\n",data.token.type);
  if( result_exp == SYN_OK){
    //printf("----------------0.7 VALUES TYPE = %d -------------\n",data.token.type);
    if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_OK ) {
      return SYN_ERR;
    }


    if( check_type(T_TYPE_EOL) != SYN_ERR ) {
      return SYN_OK;
    }

    //printf("----------------1. VALUES TYPE = %d -------------\n",result_exp);
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
    int exit_values_n;
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

      data.check_type = true;
    
      int result_exp = expression(&data,&non_det);
      data.check_type = true;

    if (data.check_returns == true){
        data.checked_returns++;
    }

      id_queue_pop(&data.ID_queue);

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
            exit_values_n = values_n();
          }
          else{
            return SYN_ERR;
          }

          if ( exit_values_n != SYN_OK){
            return exit_values_n;
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
    hKey_t tmp = "_";
    if (str_cmp_const_str(data.token.attribute.string, tmp) == 0){
        return SEM_ERR_OTHER;
    }
    actual_parameter = BT_insert(&top_of_the_stack->local_bt, data.token.attribute.string->str, &internal_error);
    
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

    hKey_t tmp = "_";
    if (str_cmp_const_str(data.token.attribute.string, tmp) == 0){
        return SEM_ERR_OTHER;
    }

    tBT_stack_item* top_of_the_stack = bt_stack_top(&data.BT_stack);
    //kontrola ci IDcka vstupnych parametrov nie su rovnake
    actual_parameter = BT_insert(&top_of_the_stack->local_bt, data.token.attribute.string->str, &internal_error);
    if (actual_parameter == NULL) return SEM_ERR_UNDEFINED_VAR;

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
      
      //anastavim si typy vstupnych parametrov v aktualnom stacku
      //tu si ukladam do fronty k aktualnej funkcii, typy parametrov danej funkcie
      if (set_param_type == true){
        if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_INT){

            tID_queue_item *pushnuta = id_queue_push(&data.actual_func->input_params);
          
            hKey_t tmp = "int";
            str_add_const_str(&pushnuta->id, tmp);

            actual_parameter->type = TYPE_INT;
        }
        else if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_FLOAT64){
            tID_queue_item *pushnuta = id_queue_push(&data.actual_func->input_params);
          
            hKey_t tmp = "double";
            str_add_const_str(&pushnuta->id, tmp);

            actual_parameter->type = TYPE_DOUBLE;
        } 
        else if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_STRING){

            tID_queue_item *pushnuta = id_queue_push(&data.actual_func->input_params);
          
            hKey_t tmp = "string";
            str_add_const_str(&pushnuta->id, tmp);

            actual_parameter->type = TYPE_STRING;
        }
      }

      //tu si ukladam do fronty k aktualnej funkcii, navrtove hodnoty danej funkcie
      if (saving_return_types == true){
        if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_INT){

          tID_queue_item *pushnuta = id_queue_push(&data.actual_func->func_params);
          
          hKey_t tmp = "int";
          str_add_const_str(&pushnuta->id, tmp);
        }
        else if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_FLOAT64){

          tID_queue_item *pushnuta = id_queue_push(&data.actual_func->func_params);
          hKey_t tmp = "double";
          str_add_const_str(&pushnuta->id, tmp);
        }
        else if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_STRING){

          tID_queue_item *pushnuta= id_queue_push(&data.actual_func->func_params);
          hKey_t string = "string";
          str_add_const_str(&pushnuta->id, string);
        }
        data.actual_func->no_ret_values++;
      }
      
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

		if (saving_arguments == true){
			if (data.token.type == T_TYPE_INTEGER){
				tID_queue_item *pushnuta = id_queue_push(&data.check_func_calls->rs);
          
				hKey_t tmp = "int";
				str_add_const_str(&pushnuta->id, tmp);
			}
			else if (data.token.type == T_TYPE_DOUBLE){
				tID_queue_item *pushnuta = id_queue_push(&data.check_func_calls->rs);
          
				hKey_t tmp = "double";
				str_add_const_str(&pushnuta->id, tmp);
			}
			else if (data.token.type == T_TYPE_STRING){
				tID_queue_item *pushnuta = id_queue_push(&data.check_func_calls->rs);
          
				hKey_t tmp = "string";
				str_add_const_str(&pushnuta->id, tmp);
			}
			else if (data.token.type == T_TYPE_IDENTIFIER){

				Data_t *found = bt_stack_search(&data.BT_stack, data.token.attribute.string->str, &internal_error);
				if (found == NULL){
					return SEM_ERR_UNDEFINED_VAR;
				}

				tID_queue_item *pushnuta = id_queue_push(&data.check_func_calls->rs);

				if (found->type == TYPE_INT){
					hKey_t tmp = "int";
					str_add_const_str(&pushnuta->id, tmp);
				}
				else if (found->type == TYPE_DOUBLE){
					hKey_t tmp = "double";
					str_add_const_str(&pushnuta->id, tmp);
				}
				else if (found->type == TYPE_STRING){
					hKey_t tmp = "string";
					str_add_const_str(&pushnuta->id, tmp);
				}
			}
		}

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
	str_init(&called_func);
    data.BT_global.definded = false;  //som retard a musim to spravit
    if(BT_init(&data.BT_global) == false) return false;

    //func inputs() (string,int)
    Data_t *temp = BT_insert(&data.BT_global, "inputs", &internal_error);
    tID_queue_item *temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "string");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    //func inputi() (int,int)
    temp = BT_insert(&data.BT_global, "inputi", &internal_error);
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    //func inputf() (float64,int)
    temp = BT_insert(&data.BT_global, "inputf", &internal_error);
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "double");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    //func print ( term1 , term2 , …, term𝑛 )
    temp = BT_insert(&data.BT_global, "print", &internal_error);
    //func int2float(i int) (float64)
    temp = BT_insert(&data.BT_global, "int2float", &internal_error);
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "int");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "double");
    //func float2int(f float64) (int)
    temp = BT_insert(&data.BT_global, "float2int", &internal_error);
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "double");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    //func len(𝑠 string) (int)
    temp = BT_insert(&data.BT_global, "len", &internal_error);
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "string");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    //func substr(s string, i int, n int) (string, int)
    temp = BT_insert(&data.BT_global, "substr", &internal_error);
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "string");
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "int");
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "int");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "string");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    //func ord(s string, i int) (int, int)
    temp = BT_insert(&data.BT_global, "ord", &internal_error);
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "string");
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "int");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");
    //func chr(i int) (string, int)
    temp = BT_insert(&data.BT_global, "chr", &internal_error);
    temp2 = id_queue_push(&temp->input_params);
    str_add_const_str(&temp2->id, "int");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "string");
    temp2 = id_queue_push(&temp->func_params);
    str_add_const_str(&temp2->id, "int");

    //inicializacia boolovsky hodnot v parser_data
    data.in_if_for = false;
    data.set_type_id = false;
    data.check_type = false;
    data.check_returns = false;
    data.checked_returns = 0;
    data.actual_func = NULL;
    data.check_func_calls = NULL;

    bt_stack_init(&data.BT_stack);

    id_queue_init(&data.ID_queue);
    
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
    //vymazanie linked listu pre kontrolovanie volani funkcii
	str_free(&called_func);
	tFunc_calls *Element = data.check_func_calls;
    tFunc_calls *tmp;

    while  (Element != NULL){
        tmp = Element;
        Element = Element->next;
        //UVOLNENIE 
        id_queue_free(&tmp->ls);
        id_queue_free(&tmp->rs);
        str_free(&tmp->called_func);
        free(tmp);
    }
   	data.check_func_calls = NULL;
	//koniec

    BT_dispose(&data.BT_global);
    bt_stack_free(&data.BT_stack);
    id_queue_free(&data.ID_queue);

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
        //Print_tree(data.BT_global.root_ptr);
        //funkcia s ID main musi byt obsiahnuta
		if (result == SYN_OK){
        	if(BT_search(&data.BT_global, "main", &internal_error) == NULL) result = SEM_ERR_UNDEFINED_VAR;
		}
    }

	if (result == SYN_OK){
		//skontrolujem volania funckii
		tFunc_calls *tmp = data.check_func_calls;
		while (tmp != NULL){

			//TODO print - special case
			if ((str_cmp_const_str(&tmp->called_func, "print")) == 0){
				if (tmp->ls.top != NULL){
					result = SEM_ERR_NO_PARAMS;
					goto error_func_call;
				}

				break;
			}
			//najdem si funkciu ktoru chcem skontrolovat
			Data_t *func_check = BT_search(&data.BT_global, tmp->called_func.str, &internal_error);
			if (func_check == NULL){
				result = SEM_ERR_UNDEFINED_VAR;
				goto error_func_call;
			}

			//LAVA STRANA
			int ls_counter = 0;
			
			tID_queue_item *ls_btglobal = func_check->func_params.top;
			tID_queue_item *ls_tocheck = tmp->ls.top;
			while (ls_btglobal != NULL && ls_tocheck != NULL){
				
				ls_btglobal = n_item(&func_check->func_params, ls_counter);
				ls_tocheck = n_item(&tmp->ls, ls_counter);

				if (ls_btglobal == NULL || ls_tocheck == NULL){
					break;
				}

				if ((str_cmp_const_str(&ls_btglobal->id, ls_tocheck->id.str)) != 0 ){
					if (str_cmp_const_str(&ls_tocheck->id, "_") != 0){
						result = SEM_ERR_NO_PARAMS;
						goto error_func_call;
					}
				}

				ls_counter++;
			}
			//kontrola poctu
			if (ls_btglobal != NULL && ls_tocheck == NULL){
				result = SEM_ERR_NO_PARAMS;
				goto error_func_call;
			}
			if (ls_btglobal == NULL && ls_tocheck != NULL){
				result = SEM_ERR_NO_PARAMS;
				goto error_func_call;
			}
			//PRAVA STRANA
			int rs_counter = 0;
			tID_queue_item *rs_btglobal = func_check->input_params.top;
			tID_queue_item *rs_tocheck = tmp->rs.top;
			while (rs_btglobal != NULL && rs_tocheck != NULL){
				
				rs_btglobal = n_item(&func_check->input_params, rs_counter);
				rs_tocheck = n_item(&tmp->rs, rs_counter);

				if (rs_btglobal == NULL || rs_tocheck == NULL){
					break;
				}
				if ((str_cmp_const_str(&rs_btglobal->id, rs_tocheck->id.str)) != 0){
					result = SEM_ERR_NO_PARAMS;
					goto error_func_call;
				}

				rs_counter++;
			}
			//kontrola poctu
			if (rs_btglobal != NULL && rs_tocheck == NULL){
				result = SEM_ERR_NO_PARAMS;
				goto error_func_call;
			}
			if (rs_btglobal == NULL && rs_tocheck != NULL){
				result = SEM_ERR_NO_PARAMS;
				goto error_func_call;
			}

			tmp = tmp->next;
		}
	}
	error_func_call:


    //uvolnenie pamate 
    free_variables();
    //vratenie exit code
    return result;
} 


