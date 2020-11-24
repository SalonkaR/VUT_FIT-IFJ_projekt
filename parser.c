/*
//IFJ20 - projekt(varianta I.)
//subor pre pracu s parserom
//Filip Brna, xbrnaf00
*/



#include <stdio.h>
#include <stdlib.h>

#include "str.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

//int token;              // globalni promenna, ve ktere bude ulozen aktualni token
struct str_struct str;

struct parser_data data;

int counterVar = 1;
int result;

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
    //printf("type ok\n");
    return SYN_OK;
}

int check_rule(int rule())
{
    if ((result = rule(data)))
    {
        return result;
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



int start()
{
  // pravidlo <start> -> package main EOL <eol> <prog> EOF

  result = 0;
  if (data.token.type == T_TYPE_KEYWORD && data.token.attribute.keyword == KWORD_PACKAGE)
  {
    if (check_token() == LEX_ERR){
    	return LEX_ERR;                   //vypytam si dalsi token, ocakavam main
    } 
    if (check_keyword(KWORD_MAIN) == SYN_ERR ){
      return SYN_ERR;
    }
    if (check_token() == LEX_ERR){
    	return LEX_ERR;
    }
    if (check_type(T_TYPE_EOL) == SYN_ERR ){
      return SYN_ERR;
    }

    if (eol() == SYN_ERR)
    { 
      return SYN_ERR;
    }
    //if (check_token() == LEX_ERR){
    // 	return LEX_ERR;
    //}
    
    if (prog() == SYN_ERR)
    {
      return SYN_ERR;
    }
    if (check_token() == LEX_ERR){
    	return LEX_ERR;
    }
    
    if (check_type(T_TYPE_EOF) == SYN_ERR ){
      return SYN_ERR;
    }
    return SYN_OK;
  }
  return SYN_ERR;
}

int eol()
{
  // pravidlo <eol> -> EOL <eol>
  //hlavickovy subor pre tabulku symbolov
  //printf("-------------------------------1. TOKEN EOL MAM IF-TYPE = %d -------------\n",data.token.type);
  if ( check_type(T_TYPE_EOL) != SYN_ERR ){  //vypytam si dalsi token, ocakavam EOL
    if (check_token() == LEX_ERR){
  	  return LEX_ERR;
    }
    eol();
  }
  else{   // pravidlo <eol> -> epsilon
    return SYN_OK;
  }
}



int prog()
{
  // pravidlo <prog> -> func ID (<params>) <raturn_value> { <eol> <body> } EOL <eol> <prog>

  //printf("----------------VSTUPUJEM DO PROGU-------------\n");

   //vypytam si dalsi token, ocakavam func
  //printf("----------------1. TOKEN PROG MAM - TYPE = %d -------------\n",data.token.type);

  if ( check_keyword(KWORD_FUNC) != SYN_ERR ){

      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("----------------2. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
      //printf("----------------2. TOKEN PROG MAM IF-TYPE = %d -------------\n",check_type(T_TYPE_IDENTIFIER));
      
      if (check_type(T_TYPE_IDENTIFIER) == SYN_ERR && check_keyword(KWORD_MAIN) == SYN_ERR ){
        //printf("----------------2.5 TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
        return SYN_ERR;
      }

      //bool internal_error;
      ///BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
      //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
      //if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
      //  return SEM_ERR_UNDEFINED_VAR;  
      //}
      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("----------------3. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);

      if (check_type(T_TYPE_LEFT_BRACKET) == SYN_ERR ){
        return SYN_ERR;
      }
      
       if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("----------------4. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);


    	if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
        //printf("VOLAM PARAMS -----------\n");
        if (params() != SYN_OK){ 
          return SYN_ERR;
        }
      }


      if (check_token() == LEX_ERR){
    	  return LEX_ERR;
      }
      //printf("----------------4.5. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
    	if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
        printf("----------------VOLAM RV-------------\n");
        if (return_value() != SYN_OK){ 
          return SYN_ERR;
        }
      }

      if (check_token() == LEX_ERR){
      return LEX_ERR;
      }
      printf("----------------5. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
    	if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
        //printf("----------------VOLAM EOl- TYPE = %d -------------\n",data.token.type);
        if (eol() != SYN_OK){ 
          return SYN_ERR;
        }
        printf("-------------------------------5.5. TOKEN PROG MAM IF-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
         // printf("---------------- 1 VOLAM BODY - TYPE = %d -------------\n",data.token.type);
          if (body() != SYN_OK){ 
            return SYN_ERR;
          }
          if (check_token() == LEX_ERR){
            return LEX_ERR;
          }
          
          //printf("---------------- 2 VOLAM BODY - TYPE = %d -------------\n",data.token.type);
          if (check_type( T_TYPE_EOL) == SYN_ERR || check_type( T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            if (check_token() == LEX_ERR){
              return LEX_ERR;
            }
            //printf("----------------VOLAM EOL- TYPE = %d -------------\n",data.token.type);
            if (eol() != SYN_OK){ 
                return SYN_ERR;
            }
            //printf("----------------VOLAM PROG- TYPE = %d -------------\n",data.token.type);
            if (prog() != SYN_ERR){ 
                prog();
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
      if (eol() != SYN_OK){ 
          return SYN_ERR;
      }
      //printf("----------------VOLAM PROG- TYPE = %d -------------\n",data.token.type);
      if (prog() != SYN_ERR){ 
          prog();
      }
  }
  //printf("----------------ELSE VETVA PROG-------------\n");
  
  // pravidlo <prog> -> epsilon
  return SYN_OK;
}

int body()
{
  if(data.token.type == T_TYPE_KEYWORD){
      //printf("----------------. TOKEN BODY MAM CASE kword-TYPE = %d -------------\n",data.token.type);
      //printf("----------------. TOKEN BODY MAM CASE kword JE-TYPE = %d -------------\n",check_keyword(KWORD_RETURN));
     // pravidlo <body> -> if <expression> { EOL <body>} else { EOL <body>} EOL <body>
      if (check_keyword(KWORD_IF) != SYN_ERR ){
          //printf("----------------1. TOKEN BODY MAM IF IF-TYPE = %d -------------\n",data.token.type);
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          if (expression() != SYN_OK){ 
            return SYN_ERR;
          }
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------2. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------3. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------4. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------VOLAM BODY po 4- TYPE = %d -------------\n",data.token.type);
            if (body() != SYN_OK){ 
              return SYN_ERR;
            }
              
          }
          //printf("----------------4.5 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          //printf("----------------5. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            if (check_token(data) == LEX_ERR){
              return LEX_ERR;
            }
          }

          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------5.25 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
            return SYN_ERR;
          }
          

          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------5.5 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);

          if (check_keyword(KWORD_ELSE) == SYN_ERR ){
            return SYN_ERR;
          }  
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------6. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------7. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------8. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------VOLAM BODY - TYPE = %d -------------\n",data.token.type);
            if (body() != SYN_OK){ 
              return SYN_ERR;
            }
          }
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------8.5 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
                    
          if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
          }
          
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          //printf("----------------9. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }
          if (body(data) != SYN_OK){ 
            return SYN_ERR;
          }
          //printf("-------------------------------VYS TYPE = %d -------------\n",data.token.type);          

          //if (check_token(data) == LEX_ERR){
            //return LEX_ERR;
         // }
          //printf("-------------------------------VYS TYPE = %d -------------\n",data.token.type);          
          return SYN_OK;
      }

      // pravidlo <body> -> for <definition> ; <expression> ; <assignment> { EOL <body> } EOL  <body>
      else if (check_keyword(KWORD_FOR) != SYN_ERR ){
        //printf("---------------- TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------1.5 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
        if (definition() != SYN_OK){ 
            return SYN_ERR;
        }
        if (check_type(T_TYPE_SEMICOLON) == SYN_ERR ){
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
        }
        
        //printf("----------------2 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_SEMICOLON) == SYN_ERR ){
          return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------2.5 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
        if (expression() != SYN_OK){ 
          return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------3 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_SEMICOLON) == SYN_ERR ){
          return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
          printf("----------------VOLAM BODY - TYPE = %d -------------\n",data.token.type);
          if (assignment() != SYN_OK){ 
            return SYN_ERR;
          }
        }
        printf("----------------3.5 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
        

        //printf("----------------4 TOKEN BODY MAM IF FOR-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
            return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------7. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------8. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
            //printf("----------------VOLAM BODY - TYPE = %d -------------\n",data.token.type);
          if (body() != SYN_OK){ 
              return SYN_ERR;
          }
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------8.5 TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
                    
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
          return SYN_ERR;
        }
          
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------9. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        if (body(data) != SYN_OK){ 
          return SYN_ERR;
        }
        //printf("-------------------------------VYS FOR TYPE = %d -------------\n",data.token.type);          
          return SYN_OK;
      }
      // pravidlo <body> ->  return <list_of_return_values> EOL <eol> <body>
      else if (check_keyword(KWORD_RETURN) != SYN_ERR ){
        //printf("----------------0. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------1. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (list_of_return_values(data) != SYN_OK){ 
          return SYN_ERR;
        } 
        //printf("----------------2. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        if (eol(data) != SYN_OK){ 
          return SYN_ERR;
        }  
        //printf("----------------3. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
        if (body(data) != SYN_OK){ 
          //printf("----------------4. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
          return SYN_ERR;
        }
        //printf("----------------else. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
        return SYN_OK;
      }

  }

  else if(check_type(T_TYPE_IDENTIFIER) == SYN_OK){

      //printf("---------------- TOKEN IDENTIFIER TYPE = %d -------------\n",data.token.type);
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN IDENTIFIER TYPE = %d -------------\n",data.token.type);  
      if (check_type(T_TYPE_LEFT_BRACKET) != SYN_ERR ){
        // pravidlo <body> -> ID ( <argument> ) EOL <eol> <body>
        //printf("----------------0. TOKEN BODY MAM ID-TYPE = %d -------------\n",data.token.type);
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
          if (argument(data) != SYN_OK){ 
            return SYN_ERR;
          }
        }
        //printf("----------------2. TOKEN BODY MAM IF ID ( TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
          return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        //printf("----------------3. TOKEN BODY MAM IF ID ( TYPE = %d -------------\n",data.token.type);
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        if (eol(data) != SYN_OK){ 
          return SYN_ERR;
        }
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){

          //printf("----------------3. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (body(data) != SYN_OK){ 
            //printf("----------------4. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
            return SYN_ERR;
          }
        }
        //printf("----------------else. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
        return SYN_OK;
        
      }
      else if (check_type(T_TYPE_VARIABLE_DEFINITION) != SYN_ERR ){\
          // pravidlo <body> -> ID := <expression> EOL <eol> <body>


          //bool internal_error;
          //BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
          //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
          //if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
          //  return SEM_ERR_UNDEFINED_VAR;  
          //}
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }          
          
        if (expression() != SYN_OK){ 
          return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_EOL) == SYN_ERR ){
          return SYN_ERR;
        }
        if (eol(data) != SYN_OK){ 
          return SYN_ERR;
        }
        if (check_type(T_TYPE_RIGHT_VINCULUM) == SYN_ERR ){
          //printf("----------------3. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);
          if (body(data) != SYN_OK){ 
            //printf("----------------4. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
            return SYN_ERR;
          }
        }
        //printf("----------------else. TOKEN body MAM IF if-TYPE = %d -------------\n",data.token.type);        
        return SYN_OK;
      }
      // pravidlo <body> -> <ids> = ID(<argument>) EOL <eol> <body>
      // pravidlo <body> -> <ids> = <values> EOL <eol> <body>
      else if (check_type(T_TYPE_ASSIGN) != SYN_ERR ){ 
    
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_ASSIGN) == SYN_ERR ){
          return SYN_ERR;
        }
        if (check_token(data) == LEX_ERR){
          return LEX_ERR;
        }
        if (check_type(T_TYPE_IDENTIFIER) == SYN_OK ){
          //bool internal_error;
          //BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
          //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
          //if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
          //  return SEM_ERR_UNDEFINED_VAR;  
          //}
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          if ( check_type(T_TYPE_LEFT_BRACKET) != SYN_ERR ){
            return SYN_ERR;
          }

          if (argument() != SYN_OK){ 
          return SYN_ERR;
          } 

          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          if (check_type(T_TYPE_RIGHT_BRACKET) == SYN_ERR ){
            return SYN_ERR;
          }
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }
          if (eol(data) != SYN_OK){ 
            return SYN_ERR;
          }
          if (body(data) != SYN_OK){ 
            return SYN_ERR;
          }
          return SYN_OK;
        }
        else{ // EXPRESSION TO DO
          //if (expression(data) != SYN_OK){ 
          //  return SYN_ERR;
          //}
          if (check_token(data) == LEX_ERR){
            return LEX_ERR;
          }
          if (check_type(T_TYPE_EOL) == SYN_ERR ){
            return SYN_ERR;
          }
          if (eol(data) != SYN_OK){ 
            return SYN_ERR;
          }
          if (body(data) != SYN_OK){ 
            return SYN_ERR;
          }
          return SYN_OK;
        }
      
      }

      return SYN_OK;
      
      
  }
 
  else{
    //printf("KONIEC\n");
    return SYN_OK;
  }
  // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
}

int definition()
{
  // pravidlo <definition> -> ID := <value>

  //printf("----------------0. TOKEN DEFINITION TYPE = %d -------------\n",data.token.type);
  if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
      //bool internal_error;
      ///BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
      //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
      //if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
      //  return SEM_ERR_UNDEFINED_VAR;  
      //}
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN DEFINITION TYPE = %d -------------\n",data.token.type);
      if (check_type(T_TYPE_VARIABLE_DEFINITION) == SYN_ERR ){
        return SYN_ERR;
      }
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------2. TOKEN DEFINITION TYPE = %d -------------\n",data.token.type);
      
      if (value(data) != SYN_OK){ 
          return SYN_ERR;
      }
      //printf("----------------3. TOKEN DEFINITION TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
  }
  
  // pravidlo <definition> -> epsilon
  else{
    return SYN_OK;
  }

}

int assignment()
{
  // pravidlo <assignment> -> ID = <expression>

  printf("----------------0. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
  if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
    //bool internal_error;
    //BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
    //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
    //if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
    //  return SEM_ERR_UNDEFINED_VAR;  
    //}
    if (check_token(data) == LEX_ERR){
      return LEX_ERR;
    }
    printf("----------------1. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
    if (check_type(T_TYPE_ASSIGN) == SYN_ERR ){
        return SYN_ERR;
    }
    if (check_token(data) == LEX_ERR){
      return LEX_ERR;
    }
    printf("----------------2. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
    if (expression(data) != SYN_OK){ 
         return SYN_ERR;
    }
    printf("----------------3. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
    if (check_token(data) == LEX_ERR){
      return LEX_ERR;
    }
    printf("----------------4. TOKEN ASSIGNMENT TYPE = %d -------------\n",data.token.type);
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

  if (check_token(data) == LEX_ERR){
    return LEX_ERR;
  }
  if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
    bool internal_error;
    BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
    //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
    if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
      return SEM_ERR_UNDEFINED_VAR;  
    }

    if (id_n(data) != SYN_OK){ 
      return SYN_ERR;
    }
    return SYN_OK;
  }
  return SYN_ERR;
}

int id_n()
{
  // pravidlo <id_n> -> , ID <id_n>

  if (check_token(data) == LEX_ERR){
    return LEX_ERR;
  }
  if( check_type(T_TYPE_COMMA) != SYN_ERR ) {
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
        bool internal_error;
        BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
        //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
        if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
          return SEM_ERR_UNDEFINED_VAR;  
        }
        if (id_n(data) != SYN_ERR){ 
            id_n(data);
        }
      }
  }

  // pravidlo <id_n> -> epsilon

  return SYN_OK;
}

int list_of_return_values()
{
  // pravidlo <list_of_return_values> -> <values>

  if( values(data) != SYN_ERR ){
    return SYN_OK;
  }

  // pravidlo <list_of_return_values> -> epsilon
  return SYN_OK;
}

int values()
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

int values_n()
{
  // pravidlo <values_n> -> ,<expression> <values_n>

    if (check_token(data) == LEX_ERR){
      return LEX_ERR;
    }
    if (check_type(T_TYPE_COMMA) == SYN_ERR ){
        return SYN_ERR;
      }
    
    if (check_token(data) == LEX_ERR){
      return LEX_ERR;
    }
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

int params()
{
  // pravidlo <params> -> ID <type> <params_n>

  //if (check_token(data) == LEX_ERR){
  //  return LEX_ERR;
  //}
  if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
    bool internal_error;
    BT_insert(&data.BT_local, data.token.attribute.string->str, &internal_error);
  //int tmp = BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error);
    if ( BT_search(&data.BT_local, data.token.attribute.string->str, &internal_error) == NULL){
      return SEM_ERR_UNDEFINED_VAR;  
    }

    if (type(data) != SYN_OK){ 
      return SYN_ERR;
    }

    if (params_n(data) != SYN_OK){ 
      return SYN_ERR;
    }
  }

  // pravidlo <params> -> epsilon
  return SYN_OK;
}

int params_n()
{
  // pravidlo <params_n> -> , ID <type> <params_n>
  

  if (check_token(data) == LEX_ERR){
    return LEX_ERR;
  }
  if ( check_type(T_TYPE_COMMA) != SYN_ERR ){
    if (check_token(data) == LEX_ERR){
      return LEX_ERR;
    }
    if (check_type(T_TYPE_IDENTIFIER) != SYN_ERR ){
      bool internal_error;
      BT_insert(&data.BT_global, data.token.attribute.string->str, &internal_error);
      //int tmp = BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error);
      if ( BT_search(&data.BT_global, data.token.attribute.string->str, &internal_error) == NULL){
        return SEM_ERR_UNDEFINED_VAR;  
      }
    }
    if (type(data) != SYN_OK){ 
      return SYN_ERR;
    }
    if (params_n(data) != SYN_ERR){ 
      params_n(data);
    }
  }

  // pravidlo <params_n> ->epsilon
  return SYN_OK;
}

int return_value()
{
  // pravidlo <return_value> -> ( <type> <return_value_n> )
  //printf("----------------0. TOKEN RV-TYPE = %d -------------\n",data.token.type);
  if ( check_type(T_TYPE_LEFT_BRACKET) != SYN_ERR ){
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN RV-TYPE = %d -------------\n",data.token.type);  
      if (type() != SYN_OK){ 
        return SYN_ERR;
      }
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------2. TOKEN RV-TYPE = %d -------------\n",data.token.type);
      if (return_value_n() == SYN_ERR){ 
        return SYN_ERR;
      }
      //printf("----------------3. TOKEN RV-TYPE = %d -------------\n",data.token.type);
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------4. TOKEN RV-TYPE = %d -------------\n",data.token.type);   
      if (check_type(T_TYPE_LEFT_VINCULUM) == SYN_ERR ){
        return SYN_ERR;
      }
      return SYN_OK;
  }
  
  // pravidlo <return_value> -> epsilon
     
  
}

int return_value_n()
{
  // pravidlo <return_value_n> -> , <type> <return_value_n>

  //if (check_token(data) == LEX_ERR){
  //  return LEX_ERR;
  //}
  
  //int tmp = check_type(T_TYPE_COMMA);
  //printf("----------------0. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
  if ( check_type(T_TYPE_COMMA) != SYN_ERR ){
      

      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------1. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      if( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK ){
        //printf("  ----> %d  <---- \n",check_type(T_TYPE_RIGHT_BRACKET));
        return SYN_ERR;
      }

      if (type() != SYN_OK){ 
        return SYN_ERR;
      }
      //printf("----------------2. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      if (check_token(data) == LEX_ERR){
        return LEX_ERR;
      }
      //printf("----------------3. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      if (return_value_n() != SYN_ERR){ 
        return_value_n();
      }
      else{
        //printf("  ----> %d  <---- \n",data.token.type);
        return SYN_ERR;
      }
      //printf("----------------4. TOKEN RVN-TYPE = %d -------------\n",data.token.type);
      return SYN_OK;
  }
  
  // pravidlo <return_value_n> -> epsilon
  else if( check_type(T_TYPE_RIGHT_BRACKET) != SYN_ERR ){
    //printf("  ----> %d  <---- \n",check_type(T_TYPE_RIGHT_BRACKET));
    return SYN_OK;
  }

}

int type()
{
  // pravidlo <type> -> int
  // pravidlo <type> -> float
  // pravidlo <type> -> string

  //if (check_token(data) == LEX_ERR){
   // return LEX_ERR;
  //}
  //printf("  ----> SOM V TYPE 2 = %d  <---- \n",data.token.type);

  //if ( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK){
  //  return SYN_ERR;
  //} 

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
  if ( value() != SYN_ERR){
    if (check_token(data) == LEX_ERR){
     return LEX_ERR;
    } 
    if ( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK ){
      //printf("  ----> SYN OK arg TYPE = %d  <---- \n",data.token.type);
      return SYN_OK;
    }
    //printf("  ---->1. ARGUMENT TYPE = %d  <---- \n",data.token.type);
    if (argument_n(data) == SYN_ERR){ 
      //printf("  ---->1.5 ARGUMENT TYPE = %d  <---- \n",data.token.type);
      return SYN_ERR;
    }

    return SYN_OK;
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
    if (check_token(data) == LEX_ERR){
     return LEX_ERR;
    } 
    //printf("  ---->1. ARGUMENT_N TYPE = %d  <---- \n",data.token.type);
    if (value() == SYN_ERR){ 
      //printf("  ---->1.5 ARGUMENT_N TYPE = %d  <---- \n",data.token.type);
      return SYN_ERR;
    }
    if (check_token(data) == LEX_ERR){
     return LEX_ERR;
    } 
    if ( check_type(T_TYPE_RIGHT_BRACKET) == SYN_OK ){
      //printf("  ----> SYN OK argn TYPE = %d  <---- \n",data.token.type);
      return SYN_OK;
    }

   // printf("  ---->2. ARGUMENT_N TYPE = %d  <---- \n",data.token.type);

    if ( check_type(T_TYPE_COMMA) == SYN_OK ){
      if (argument_n() != SYN_ERR){ 
        argument_n();
      }
    }    

  }
  else{
    // pravidlo <argument_n> -> epsilon
    //printf("  ---->3. ARGUMENT_N TYPE = %d  <---- \n",data.token.type);
    return SYN_OK;
  }
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

int expression()
{
  //if (check_token(data) == LEX_ERR){
  //  return LEX_ERR;
  //}
  return SYN_OK;
}

bool init_variables()
{
    if(BT_init(&data.BT_global) == false) return false;
    if(BT_init(&data.BT_local) == false) return false;
    data.token.attribute.string = malloc(sizeof(struct str_struct));
    if(data.token.attribute.string == NULL) return ERROR_INTERNAL;
    if(str_init(data.token.attribute.string) == false) return false;
    
    return true;
}

void free_variables()
{
    BT_dispose(&data.BT_global);
    BT_dispose(&data.BT_local);
    str_clear(data.token.attribute.string);
    str_free(data.token.attribute.string);
    if((data.token.attribute.string) != NULL) free(data.token.attribute.string);
}

int parse()
{
    //struct str_struct str;
    //init_variables();
    if(init_variables() == false)
    {
        //str_free(&data.token.attribute.string);
        free_variables();
        return ERROR_INTERNAL;
    }

    //if (str_init(&str) == false){
    //    return ERROR_INTERNAL;
    //}

    //data.BT_global.definded = false;  //som retard a musim to spravit
    //data.BT_local.definded = false;   //tu tiez to iste  
    //data.token.attribute.string = malloc(sizeof(struct str_struct));
    //if(data.token.attribute.string == NULL){
    //   return ERROR_INTERNAL;
    //}

    //if (str_init(&data.token.attribute.string) == false){
    //    str_free(&data.token.attribute.string);
    //    return ERROR_INTERNAL;
    //}

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

    //str_free(&data.token.attribute.string);
    //free(&data.token.attribute.string);
    //str_free(&str);
    //free_variables(&data);
    free_variables();
    
    return result;
} 



