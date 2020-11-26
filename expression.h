/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu s vyrazmi
//Matej Hornik, xhorni20
*/

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "parser.h"
#include "symtable.h"



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
	PLUS,			// +                                                c = 0
	MINUS,			// -                                                c = 1
	MUL,			// *                                                c = 2
    DIV,            // /                                                c = 3
	EQ,				// ==                                               c = 4
	NEQ,			// !=                                               c = 5
	LEQ,			// <=                                               c = 6
	L,			    // <                                                c = 7
	MEQ,			// >=                                               c = 8
	M,			    // >                                                c = 9
	LEFT_BRACKET,	// (                                                c = 10
	RIGHT_BRACKET,	// )                                                c = 11
	IDENTIFIER,		// ID                                               c = 12
	INT_NUMBER,		// int                                              c = 13
	DOUBLE_NUMBER,	// double                                           c = 14
	STRING,			// string                                           c = 15
	DOLLAR,			// $                                                c = 16
	STOP,			// symbol ktory pouzijem pri redukcii( < )          c = 17
	NON_TERM		// neterminal(E)                                    c = 18
} Prec_table_symbol;


int expression(struct parser_data* data, bool *nondetermism);


#endif