#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "gen_code.h"
#include "id_queue.h"
#include "scanner.h"
#include "str.h"



#define LENGTH 50


struct str_struct code20;
struct str_struct tmp;
tID_queue parameter_stack;

tID_queue_item *id_stack_push(tID_queue* queue){
    
	tID_queue_item* new_item = malloc(sizeof(struct id_queue_item));
	
	if (new_item == NULL){
		return NULL;
	}
	new_item->next = queue->top;
	bool str_init_res = str_init(&new_item->id);
	if (str_init_res == false){
		free(new_item);
		return NULL;
	}
	queue->top = new_item;
	
	return new_item;
}

void gen_code_start()
{
	str_init(&code20);
	str_init(&tmp);
	id_queue_init(&parameter_stack);

	str_add_const_str(&code20, "#Zacatek programu\n");
	str_add_const_str(&code20, ".IFJcode20\n");
	str_add_const_str(&code20, "DEFVAR GF@res\n");
	str_add_const_str(&code20, "DEFVAR GF@strA\n");
    str_add_const_str(&code20, "DEFVAR GF@strB\n");
    str_add_const_str(&code20, "DEFVAR GF@strC\n");
	str_add_const_str(&code20, "JUMP main\n");

}


void gen_value(struct token *token, struct str_struct *to_which)
{
	char act_value[LENGTH];

	struct str_struct h_str;
	str_init(&h_str);	

	switch(token->type){
		case T_TYPE_INTEGER:
			str_add_const_str(to_which, "int@");
			sprintf(act_value, "%d", token->attribute.int_literal);
			str_add_const_str(to_which, act_value);
			break;
		case T_TYPE_DOUBLE:
			str_add_const_str(to_which, "float@");
			sprintf(act_value, "%g", token->attribute.double_literal);
			str_add_const_str(to_which, act_value);
			break;
		case T_TYPE_STRING:
			for(long unsigned int i = 0; i <= strlen(token->attribute.string->str);i++){
				if(token->attribute.string->str[i] == '#' || token->attribute.string->str[i] == '\\' || token->attribute.string->str[i] <= 32){
					str_add_char(&h_str, '\\');
					sprintf(h_str.str, "%03d", token->attribute.string->str[i]);
					str_add_const_str(to_which, h_str.str);
				} else{
					str_add_const_str(to_which, "string@");
					str_add_const_str(to_which, h_str.str);
				}
			}
			break;
		case T_TYPE_IDENTIFIER:
			str_add_const_str(to_which, "TF@");			
			str_add_const_str(to_which, token->attribute.string->str);
			break;
		default:
			break;
		}
	str_free(&h_str);
}


void pop_value(char var_name[])
{
	str_add_const_str(&code20, "POPS GF@res\n");	

	str_add_const_str(&code20, "MOVE TF@");
	str_add_const_str(&code20, var_name);
	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "GF@res\n");	
}

void main_func()
{
	str_add_const_str(&code20, "#Hlavni funcke main\n");
	str_add_const_str(&code20, "LABEL main\n");
	str_add_const_str(&code20, "CREATEFRAME\n");
}

void main_func_end()
{
	str_add_const_str(&code20, "#Konec hlavni funcke main\n");
	str_add_const_str(&code20, "CLEARS\n");
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


void before_call_func_params(struct token *token)
{
	str_add_const_str(&code20, "PUSHS TF@");
	gen_value(token, &code20);
	str_add_const_str(&code20, "\n");
}


void after_func_beg_params(char value[])
{	
	str_add_const_str(&code20, "DEFVAR LF@");
	str_add_const_str(&code20, value);	
	str_add_const_str(&code20, "\n");	

	str_add_const_str(&code20, "POPS LF@");
	str_add_const_str(&code20, value);		
	str_add_const_str(&code20, "\n");	
}


void func_retval(char value[], int index)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "DEFVAR LF@%retval");
	str_add_const_str(&code20, tmp);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "MOVE LF@%retval");
	str_add_const_str(&code20, tmp);
	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, value);
	str_add_const_str(&code20, "\n");	
}


void retval_to_y(int index)
{
	printf("MOVE GF@y TF@%cretval%d\n", '%', index);	
}

//loc = TF nebo LF
void make_var(char var_name[], char loc[])
{
	str_add_const_str(&code20, "DEFVAR");
	str_add_const_str(&code20, " ");	
	str_add_const_str(&code20, loc);
	str_add_const_str(&code20, "@");
	str_add_const_str(&code20, var_name);
	str_add_const_str(&code20, "\n");	
}


void gen_label(char func_name[], int index, char ee[])
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "LABEL $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, ee);

	str_add_const_str(&code20, "\n");	
}


void gen_if_start()
{
	str_add_const_str(&code20, "#Tady zacina if\n");	
}


void gen_if(char func_name[], int index)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMPIFNEQ $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);	

	str_add_const_str(&code20, "$else\n");	
}


void gen_if_else(char func_name[], int index)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMP $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);	

	str_add_const_str(&code20, "$if_end\n");

	str_add_const_str(&code20, "#Else cast\n");
	
	gen_label(func_name, index, "else");
}


void gen_if_end(char func_name[], int index)
{	
	gen_label(func_name, index, "if_end");
}


void gen_for_start()
{
	str_add_const_str(&code20, "#Tady zacina for\n");	
}


void gen_for(char func_name[], int index, int depth, char ee[])
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMPIFNEQ $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, ee);

	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "TF@RES$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "bool@true\n");	
}


void gen_for_end(char func_name[], int index, int depth)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMP $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$FOR\n");
	
	gen_label(func_name, index, "for_end");	
}


void gen_arithmetic(Prec_rules symb)
{
	switch(symb){		
		case E_EQ_E:
			str_add_const_str(&code20, "EQS\n");
			break;
		case E_NEQ_E:
			str_add_const_str(&code20, "EQS\n");
			str_add_const_str(&code20, "NOTS\n");
			break;
		case E_MEQ_E:
			str_add_const_str(&code20, "POPS GF@strA\n");
			str_add_const_str(&code20, "POPS GF@strB\n");
			str_add_const_str(&code20, "PUSHS GF@strB\n");
			str_add_const_str(&code20, "PUSHS GF@strA\n");
			str_add_const_str(&code20, "GTS\n");
			str_add_const_str(&code20, "PUSHS GF@strB\n");
			str_add_const_str(&code20, "PUSHS GF@strA\n");
			str_add_const_str(&code20, "EQS\n");			
			str_add_const_str(&code20, "ORS\n");
			break;
		case E_M_E:
			str_add_const_str(&code20, "GTS\n");
			break;
		case E_LEQ_E:
			str_add_const_str(&code20, "POPS GF@strA\n");
			str_add_const_str(&code20, "POPS GF@strB\n");
			str_add_const_str(&code20, "PUSHS GF@strB\n");
			str_add_const_str(&code20, "PUSHS GF@strA\n");
			str_add_const_str(&code20, "LTS\n");
			str_add_const_str(&code20, "PUSHS GF@strB\n");
			str_add_const_str(&code20, "PUSHS GF@strA\n");
			str_add_const_str(&code20, "EQS\n");			
			str_add_const_str(&code20, "ORS\n");
			break;
		case E_L_E:
			str_add_const_str(&code20, "LTS\n");
			break;
		case E_PLUS_E:
			str_add_const_str(&code20, "ADDS\n");
			break;
		case E_MINUS_E:
			str_add_const_str(&code20, "SUBS\n");
			break;
		case E_MUL_E:
			str_add_const_str(&code20, "MULS\n");
			break;
		case E_DIV_E:
			str_add_const_str(&code20, "DIVS\n");
			break;
		default:
			break;
	}
}

void push_value(struct token *token)
{
    str_add_const_str(&tmp, "PUSHS");
    str_add_const_str(&tmp, " ");    
    gen_value(token, &tmp);
    str_add_const_str(&tmp, "\n");
}


void concat_strings()
{
	str_add_const_str(&code20, "POPS GF@strC\n");
	str_add_const_str(&code20, "POPS GF@strB\n");
	str_add_const_str(&code20, "CONCAT GF@strA GF@strB GF@strC\n");
	str_add_const_str(&code20, "PUSHS GF@strA\n");	
}


void print_ifjcode20(){
	printf("%s", code20.str);
	str_free(&tmp);
	str_free(&code20);
	id_queue_free(&parameter_stack);
}


void push_confirm(){
	 str_add_const_str(&code20, tmp.str);
	 str_clear(&tmp);
}


void save_params_to_stack(char value[]){
	tID_queue_item *new_item = id_stack_push(&parameter_stack);
	str_add_const_str(&new_item->id, value);	
}


void init_params(){
	//prejdeme zasobnik a inicializujeme
	tID_queue_item* tmp = id_queue_top(&parameter_stack);
	while (tmp != NULL){
        after_func_beg_params(tmp->id.str);
        tmp = tmp->next;
    }
}


