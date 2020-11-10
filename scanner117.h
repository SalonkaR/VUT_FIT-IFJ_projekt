#ifndef HEADER_FILE
#define HEADER_FILE

#define <stdio.h>

#include "str.h"

enum keyword
{
	//NECESSARY
	KWORD_FUNC,
	KWORD_MAIN,
	KWORD_PACKAGE,
	KWORD_RETURN,

	
	//DATA_TYPES
	KWORD_FLOAT64,
	KWORD_INT,
	KWORD_STRING,


	//COMMANDS
	KWORD_IF,
	KWORD_ELSE,
	KWORD_FOR,


	//BUILT-IN_FUNCTIONS
	KWORD_FUNC_INPUTS,
	KWORD_FUNC_INPUTI,
	KWORD_FUNC_INPUTF,
	KWORD_FUNC_PRINT,
	KWORD_FUNC_INT2FLOAT,
	KWORD_FUNC_FLOAT2INT,
	KWORD_FUNC_LEN,
	KWORD_FUNC_SUBSTR,
	KWORD_FUNC_ORD,
	KWORD_FUNC_CHR,	
}

enum t_type
{
	T_TYPE_EOF, //end of file
	T_TYPE_EOL, //end of line	 
	T_TYPE_IDENTIFIER, //identifier
	T_TYPE_KEYWORD, //keyword


	//DATA_TYPES
	T_TYPE_INTIGER, //intiger literal 
	T_TYPE_DOUBLE, //double literal
	T_TYPE_STRING, //string	


	//COMPARISONS AND DEFINITION
	T_TYPE_VARIABLE_DEFINITION, //:=	
	T_TYPE_COMPARISON, //==
	T_TYPE_NOT_EQUAL, //!=
	T_TYPE_LESS_EQUAL, //<= 
	T_TYPE_LESS, //<
	T_TYPE_MORE_EQUAL, //>=
	T_TYPE_MORE, //>


	//OPERATORS
	T_TYPE_ASSIGN, //=
	T_TYPE_PLUS, //+
	T_TYPE_MINUS, //-
	T_TYPE_MUL, //*
	T_TYPE_DIV, ///


	//BRACKETS
	T_TYPE_LEFT_BRACKET, //(
	T_TYPE_RIGHT_BRACKET, //)
	T_TYPE_LEFT_VINCULUM, //{
	T_TYPE_RIGHT_VINCULUM, //}


	//SEPARATOR
	T_TYPE_COMMA, //,
	T_TYPE_SEMICOLON, //;	
}



union t_attribute
{
	struct str_struct *string; 
	int int_literal; 
	enum keyword keyword; 
	double dec_literal; 
}


struct token{
	enum t_type type; 
	union t_attribute attribute; 
}

void set_content_file(FILE *f);

void str(Dynamic_string *string);

int get_token(Token *token);


#endif
