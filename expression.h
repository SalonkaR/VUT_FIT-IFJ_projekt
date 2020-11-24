/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu s vyrazmi
//Matej Hornik, xhorni20
*/

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "symtable.h"
#include "parser.h"


typedef enum{
	E_EQ_E,		    // E -> E == E
	E_NEQ_E,		// E -> E != E
	E_LEQ_E,		// E -> E <= E
	E_L_E,	        // E -> E < E
	E_MEQ_E,		// E -> E => E
	E_M_E,		    // E -> E > E
	E_PLUS_E,		// E -> E + E
	E_MINUS_E,	    // E -> E - E
	E_MUL_E,		// E -> E * E
	E_DIV_E,		// E -> E / E
	E_BRACKETS,		// E -> (E)
	OPERAND,		// E -> i
	NOT_A_RULE		// pravidlo neexistuje
} Prec_rules;



typedef enum{
	PLUS,			// +
	MINUS,			// -
	MUL,			// *
    DIV,            // /
	EQ,				// ==
	NEQ,			// !=
	LEQ,			// <=
	L,			    // <
	MEQ,			// >=
	M,			    // >
	LEFT_BRACKET,	// (
	RIGHT_BRACKET,	// )
	IDENTIFIER,		// ID
	INT_NUMBER,		// int
	DOUBLE_NUMBER,	// double
	STRING,			// string
	DOLLAR,			// $
	STOP,			// symbol ktory pouzijem pri redukcii( < )
	NON_TERM		// neterminal(E)
} Prec_table_symbol;


int expression(struct parser_data* data);


#endif