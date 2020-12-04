#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TYPE_MAX_LENGTH 6


static void gen_code_start()
{
	printf("#Zacatek programu\n");
	printf(".IFJcode20\n");	

	printf("DEFVAR GF@y");

	printf("JUMP main\n");
}


void main_func()
{
	printf("#Hlavni funcke main\n");
	printf("LABEL main\n");
	printf("CREATEFRAME\n");	
}


void func_beg(char func_name[])
{
	printf("#Zacatek funkce %s\n", func_name);
	printf("LABEL %s\n", func_name);
	printf("PUSHFRAME\n");
}


void func_fin(char func_name[])
{
	printf("#Konec funkce %s\n", func_name);
	printf("LABEL fin_%s\n", func_name);
	printf("POPFRAME\n");
	printf("RETURN\n");
}


void call_func(char func_name[])
{
	printf("CALL %s\n", func_name);
}


//vytvori promenne pro predani hodnot parametru
void before_call_func_params(char value[], int index)
{
	printf("DEFVAR TF@%c%d\n", '%', index);
	printf("MOVE TF@%c%d %s\n", '%', index, value);
}


void after_func_beg_params(int index)
{
	printf("DEFVAR LF@param%d\n", index);
	printf("MOVE LF@param%d LF@%c%d\n", index, '%', index);
}


void func_retval(char value[], int index)
{
	printf("DEFVAR LF@%retval%d\n", index);
	printf("MOVE LF@%retval%d %s\n", index, value);
}


void retval_to_y(int index)
{
	printf("MOVE GF@y TF@%retval%d\n", index);	
}

//loc = TF nebo LF
void make_var(char var_name[], char loc[], int depth)
{
	printf("DEFVAR %s@%s$%d\n", loc, var_name, depth);
}


void gen_label(char func_name[], int index, int depth, char ee[])
{
	printf("LABEL $%s$%d$%d$%s\n", func_name, index, depth, ee);
}


void gen_if_start()
{
	printf("#Tady zacina if");
}


void gen_if(char func_name[], int index, int depth)
{
	printf("JUMPIFNEQ $%s$%d$%d$else\n", func_name, index, depth);
}


void gen_if_else(char func_name[], int index, int depth)
{
	printf("JUMP $%s$%d$%d$end\n", func_name, index, depth);
	printf("#Else cast");
	gen_label(func_name, index, depth, "else");
}


void gen_if_end(char func_name[], int index, int depth)
{
	printf("#Konec if");
	gen_label(func_name, index, depth, "if_end");
}


void gen_for_start()
{
	printf("#Tady zacina for");
}


void gen_for(char func_name[], int index, int depth, char ee[])
{
	printf("JUMPIFNEQ $%s$%d$%d$%s TF@RES$%d bool@true", func_name[], index, depth, ee, index);
}


void gen_for_end(char func_name[], int index, int depth, char ee[])
{
	printf("JUMP $%s$%d$%d$FOR", func_name[], index, depth, ee);
	gen_label(func_name, index, depth, "for_end");	
}

int main(){
	gen_code_start();
	return 0;
}






