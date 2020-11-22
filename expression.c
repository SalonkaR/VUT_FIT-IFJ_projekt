/*
//IFJ20 - projekt(varianta I.)
//expression.c
//Matej Hornik, xhorni20
*/

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "expression.h"
#include "stack.h"
#include "error.h"


#define TABLE_SIZE 7

tStack stack; //globalna premena pre zasobnik

typedef enum
{
	S,      //  <  SHIFT
	E,      //  =  EQUAL
	R,      //  >  REDUCE
	N       // ' ' ERROR
} Prec_table_sign;

typedef enum
{
	I_PLUS_MINUS,		// 0 +-
	I_MUL_DIV,			// 1 */
	I_REL_OP,			// 2 relacne operatory
	I_LEFT_BRACKET,		// 3 (
	I_RIGHT_BRACKET,	// 4 )
	I_DATA,				// 5 i
	I_DOLLAR			// 6 $
} Prec_table_index;

//precedencna tabulka
int prec_table[TABLE_SIZE][TABLE_SIZE] =
{
//	|+- | */| R | ( | ) | i | $ |
	{ R , S , R , S , R , S , R }, // +-
	{ R , R , R , S , R , S , R }, // */
	{ S , S , N , S , R , S , R }, // R (relacne operatory)
	{ S , S , S , S , E , S , N }, // (
	{ R , R , R , N , R , N , R }, // )
	{ R , R , R , N , R , N , R }, // i
	{ S , S , S , S , N , S , N }  // $
};

static void free_resources(){
    stack_free(&stack);
}









int expression(PData* data){

    int result = SYN_ERR;

    stack_init(&stack);

    if (!stack_push(&stack, DOLLAR, TYPE_UNDEFINED)){
        free_resources();
        return ERROR_INTERNAL;
    }

    tStack_item* top_stack_terminal;
	Prec_table_symbol actual_symbol;

	bool success = false;
    

}