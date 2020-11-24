/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu so scannerom
//Matej Hornik, xhorni20
//Filip Brna, xbrnaf00
*/


#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>

#include "str.h"

enum keyword
{
	//NECESSARY
	KWORD_FUNC,				//, cislo = 0
	KWORD_PACKAGE,			//, cislo = 1
	KWORD_MAIN,				//, cislo = 2
	KWORD_RETURN,			//, cislo = 3

	
	//DATA_TYPES
	KWORD_FLOAT64,			//, cislo = 4
	KWORD_INT,				//, cislo = 5
	KWORD_STRING,			//, cislo = 6


	//COMMANDS
	KWORD_IF,				//, cislo = 7
	KWORD_ELSE,				//, cislo = 8
	KWORD_FOR,				//, cislo = 9
};

enum t_type
{
	T_TYPE_EOF, //end of file			 , cislo = 0
	T_TYPE_EOL, //end of line	 		 , cislo = 1
	T_TYPE_IDENTIFIER, //identifier		 , cislo = 2
	T_TYPE_KEYWORD, //keyword		 	, cislo = 3


	//DATA_TYPES
	T_TYPE_INTEGER, //intiger literal 	, cislo = 4
	T_TYPE_DOUBLE, //double literal	 	, cislo = 5
	T_TYPE_STRING, //string			 	, cislo = 6


	//COMPARISONS AND DEFINITION
	T_TYPE_VARIABLE_DEFINITION, //:=, cislo = 7
	T_TYPE_COMPARISON, //==		 	, cislo = 8
	T_TYPE_NOT_EQUAL, //!=		 	, cislo = 9
	T_TYPE_LESS_EQUAL, //<= 		, cislo = 10
	T_TYPE_LESS, //<		 		, cislo = 11
	T_TYPE_MORE_EQUAL, //>=		 	, cislo = 12
	T_TYPE_MORE, //>		 		, cislo = 13


	//OPERATORS
	T_TYPE_ASSIGN, //=				, cislo = 14
	T_TYPE_PLUS, //+		 		, cislo = 15
	T_TYPE_MINUS, //-		 		, cislo = 16
	T_TYPE_MUL, //*		 			, cislo = 17
	T_TYPE_DIV, ///		 			, cislo = 18


	//BRACKETS
	T_TYPE_LEFT_BRACKET, //(		 , cislo = 19
	T_TYPE_RIGHT_BRACKET, //)		 , cislo = 20
	T_TYPE_LEFT_VINCULUM, //{		 , cislo = 21
	T_TYPE_RIGHT_VINCULUM, //}		 , cislo = 22


	//SEPARATOR
	T_TYPE_COMMA, //,		 		 , cislo = 23
	T_TYPE_SEMICOLON, //;			 , cislo = 24
	T_TYPE_EMPTY //prazdnota         , cislo = 25
};



struct t_attribute
{
	struct str_struct *string; 
	int int_literal; 
	enum keyword keyword; 
	double double_literal; 
};


struct token{
	enum t_type type; 
	struct t_attribute attribute; 
};

int cleaner(int exit_code, struct str_struct *s);

void setSourceFile(FILE *f);

int get_token(struct token *token);


#endif
