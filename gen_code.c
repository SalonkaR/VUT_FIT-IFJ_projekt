#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "gen_code.h"
#include "scanner.h"
#include "str.h"

struct str_struct code20;

static void gen_code_start()
{
	str_add_const_str(&code20, "#Zacatek programu\n");
	str_add_const_str(&code20, ".IFJcode20\n");
	str_add_const_str(&code20, "DEFVAR GF@y");
	str_add_const_str(&code20, "JUMP main\n");	
}


void main_func()
{
	str_add_const_str(&code20, "#Hlavni funcke main\n");
	str_add_const_str(&code20, "LABEL main\n");
	str_add_const_str(&code20, "CREATEFRAME\n");		
}


void func_beg(char func_name[])
{
	str_add_const_str(&code20, "#Zacatek funkce ");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "LABEL ");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "PUSHFRAME\n");	
}


void func_fin(char func_name[])
{
	str_add_const_str(&code20, "#Konec funkce ");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "LABEL fin_");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "POPFRAME\n");

	str_add_const_str(&code20, "RETURN\n");	
}


void call_func(char func_name[])
{
	str_add_const_str(&code20, "CALL ");	
	str_add_const_str(&code20, func_name);	
	str_add_const_str(&code20, "\n");	
}


void before_call_func_params(char value[], int index)
{
	str_add_const_str(&code20, "DEFVAR TF@%");
	str_add_const_str(&code20, index);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "MOVE TF@%");
	str_add_const_str(&code20, index);
	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, value);
	str_add_const_str(&code20, "\n");	
}


void after_func_beg_params(int index)
{
	str_add_const_str(&code20, "DEFVAR LF@param");
	str_add_const_str(&code20, index);
	str_add_const_str(&code20, "\n");	

	str_add_const_str(&code20, "MOVE LF@param");
	str_add_const_str(&code20, index);
	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "LF@%");
	str_add_const_str(&code20, index);
	str_add_const_str(&code20, "\n");	
}


void func_retval(char value[], int index)
{
	str_add_const_str(&code20, "DEFVAR LF@%retval");
	str_add_const_str(&code20, index);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "MOVE LF@%retval");
	str_add_const_str(&code20, index);
	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, value);
	str_add_const_str(&code20, "\n");	
}


void retval_to_y(int index)
{
	printf("MOVE GF@y TF@%cretval%d\n", '%', index);	
}

//loc = TF nebo LF
void make_var(char var_name[], char loc[], int depth)
{
	printf("DEFVAR %s@%s$%d\n", loc, var_name, depth);
}


void gen_label(char func_name[], int index, int depth, char ee[])
{
	str_add_const_str(&code20, "");
	printf("LABEL $%s$%d$%d$%s\n", func_name, index, depth, ee);
}


void gen_if_start()
{
	str_add_const_str(&code20, "#Tady zacina if\n");	
}


void gen_if(char func_name[], int index, int depth)
{
	str_add_const_str(&code20, "JUMPIFNEQ $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, index);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, depth);

	str_add_const_str(&code20, "$else\n");	
}


void gen_if_else(char func_name[], int index, int depth)
{
	str_add_const_str(&code20, "JUMP $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, index);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, depth);

	str_add_const_str(&code20, "$end\n");

	str_add_const_str(&code20, "#Else cast\n");
	
	gen_label(func_name, index, depth, "else");
}


void gen_if_end(char func_name[], int index, int depth)
{	
	gen_label(func_name, index, depth, "if_end");
}


void gen_for_start()
{
	str_add_const_str(&code20, "#Tady zacina for\n");	
}


void gen_for(char func_name[], int index, int depth, char ee[])
{
	str_add_const_str(&code20, "JUMPIFNEQ $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, index);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, depth);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, ee);

	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "TF@RES$");
	str_add_const_str(&code20, index);

	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "bool@true\n");	
}


void gen_for_end(char func_name[], int index, int depth)
{
	str_add_const_str(&code20, "JUMP $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, index);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, depth);

	str_add_const_str(&code20, "$FOR\n");
	
	gen_label(func_name, index, depth, "for_end");	
}


void gen_arithmetic(symb)
{
	switch(symb){
		case plus:
			str_add_const_str(&code20, "ADDS\n");
			break;
		case minus:
			str_add_const_str(&code20, "SUBS\n");
			break;
		case multi:
			str_add_const_str(&code20, "MULS\n");
			break;
		case divide:
			str_add_const_str(&code20, "DIVS\n");
			break;
	}
}









