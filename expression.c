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
#include "bt_stack.h"
#include "parser.h"


#define TABLE_SIZE 17

tStack stack; //globalna premena pre zasobnik

typedef enum{
	S,      //  <  SHIFT
	E,      //  =  EQUAL
	R,      //  >  REDUCE
	N       // ' ' ERROR
} Prec_table_sign;


//precedencna tabulka
int prec_table[TABLE_SIZE][TABLE_SIZE] = {
//	| + | - | * | / | == | != | <= | < | >= | > | ( | ) | ID | i | d | s | $ |
	{ R , R , S , S ,  R ,  R ,  R,  R,  R,   R,  S,  R,  S,   S,  S,  S,  R}, // +
	{ R , R , S , S ,  R ,  R ,  R,  R,  R,   R,  S,  R,  S,   S,  S,  S,  R}, // -
	{ R , R , R , R ,  R ,  R ,  R,  R,  R,   R,  S,  R,  S,   S,  S,  S,  R}, // *
	{ R , R , R , R ,  R ,  R ,  R,  R,  R,   R,  S,  R,  S,   S,  S,  S,  R}, // /
	{ S , S , S , S ,  N ,  N ,  N,  N,  N,   N,  S,  R,  S,   S,  S,  S,  R}, // ==
	{ S , S , S , S ,  N ,  N ,  N,  N,  N,   N,  S,  R,  S,   S,  S,  S,  R}, // !=
	{ S , S , S , S ,  N ,  N ,  N,  N,  N,   N,  S,  R,  S,   S,  S,  S,  R}, // <=
    { S , S , S , S ,  N ,  N ,  N,  N,  N,   N,  S,  R,  S,   S,  S,  S,  R}, // <
	{ S , S , S , S ,  N ,  N ,  N,  N,  N,   N,  S,  R,  S,   S,  S,  S,  R}, // >=
	{ S , S , S , S ,  N ,  N ,  N,  N,  N,   N,  S,  R,  S,   S,  S,  S,  R}, // >
	{ S , S , S , S ,  S ,  S ,  S,  S,  S,   S,  S,  E,  S,   S,  S,  S,  N}, // (
	{ R , R , R , R ,  R ,  R ,  R,  R,  R,   R,  N,  R,  N,   N,  N,  N,  R}, // )
	{ R , R , R , R ,  R ,  R ,  R,  R,  R,   R,  N,  R,  N,   N,  N,  N,  R}, // ID
	{ R , R , R , R ,  R ,  R ,  R,  R,  R,   R,  N,  R,  N,   N,  N,  N,  R}, // i
    { R , R , R , R ,  R ,  R ,  R,  R,  R,   R,  N,  R,  N,   N,  N,  N,  R}, // d
	{ R , R , R , R ,  R ,  R ,  R,  R,  R,   R,  N,  R,  N,   N,  N,  N,  R}, // s
	{ S , S , S , S ,  S ,  S ,  S,  S,  S,   S,  S,  N,  S,   S,  S,  S,  N}  // $
};

// //TODO VYMAZAT DO PICE
// void print_token_exp(struct token *t, int ret_val, int no_token){
//     printf("%d token ->Prijaty token je typu: \"%d\"\n",no_token, t->type);
//     if (t->type == T_TYPE_INTEGER){
//         printf("%d token ->Atributy token: int= \"%d\"\n",no_token, t->attribute.int_literal);
//     }
//     else if (t->type == T_TYPE_DOUBLE){
//         printf("%d token ->Atributy token: double= \"%lf\"\n",no_token, t->attribute.double_literal);
//     }
//     else if (t->type == T_TYPE_IDENTIFIER){
//         printf("%d token ->Atributy token: string= \"%s\"\n",no_token, t->attribute.string->str);
//     }
//     else if (t->type == T_TYPE_STRING){
//         printf("%d token ->Atributy token: string= \"%s\"\n",no_token, t->attribute.string->str);
//     }

//     printf("%d token ->Prijaty token vracia: \"%d\"\n",no_token, ret_val);
// }

static void free_resources(){
    stack_free(&stack);
}


static Prec_table_symbol get_symbol_from_token(struct token* token){
	switch (token->type){
        case T_TYPE_PLUS:
            return PLUS;
        case T_TYPE_MINUS:
            return MINUS;
        case T_TYPE_MUL:
            return MUL;
        case T_TYPE_DIV:
            return DIV;
        case T_TYPE_COMPARISON:
            return EQ;
        case T_TYPE_NOT_EQUAL:
            return NEQ;
        case T_TYPE_LESS_EQUAL:
            return LEQ;
        case T_TYPE_LESS:
            return L;
        case T_TYPE_MORE_EQUAL:
            return MEQ;
        case T_TYPE_MORE:
            return M;
        case T_TYPE_LEFT_BRACKET:
            return LEFT_BRACKET;
        case T_TYPE_RIGHT_BRACKET:
            return RIGHT_BRACKET;
        case T_TYPE_IDENTIFIER:
            return IDENTIFIER;
        case T_TYPE_INTEGER:
            return INT_NUMBER;
        case T_TYPE_DOUBLE:
            return DOUBLE_NUMBER;
        case T_TYPE_STRING:
            return STRING;
        default:
            return DOLLAR;
	}
}

static enum data_types get_data_type(struct parser_data *data){
	Data_t* symbol;
    bool internal_err;
	switch (data->token.type){
        case T_TYPE_IDENTIFIER:
            symbol = bt_stack_search(&data->BT_stack, data->token.attribute.string->str, &internal_err);
            if (symbol == NULL)
                return TYPE_UNDEFINED;
            return symbol->type;

        case T_TYPE_INTEGER:
            return TYPE_INT;

        case T_TYPE_DOUBLE:
            return TYPE_DOUBLE;

        case T_TYPE_STRING:
            return TYPE_STRING;
        default:
            return TYPE_UNDEFINED;
	}
}

static Prec_rules test_rule(int op_count, tStack_item* op1, tStack_item* op2, tStack_item* op3){
	switch (op_count){
        case 1:
            // E -> i
            if (op1->symbol == IDENTIFIER || op1->symbol == INT_NUMBER || op1->symbol == DOUBLE_NUMBER || op1->symbol == STRING){
                return OPERAND;
            }

            return NOT_A_RULE;

        case 3:
            // E -> (E)
            if (op1->symbol == LEFT_BRACKET && op2->symbol == NON_TERM && op3->symbol == RIGHT_BRACKET){
                return E_BRACKETS;
            }

            if (op1->symbol == NON_TERM && op3->symbol == NON_TERM){
                switch (op2->symbol){
                    // E -> E + E
                    case PLUS:
                        return E_PLUS_E;

                    // E -> E - E
                    case MINUS:
                        return E_MINUS_E;

                    // E -> E * E
                    case MUL:
                        return E_MUL_E;

                    // E -> E / E
                    case DIV:
                        return E_DIV_E;

                    // E -> E == E
                    case EQ:
                        return E_EQ_E;

                    // E -> E != E
                    case NEQ:
                        return E_NEQ_E;

                    // E -> E <= E
                    case LEQ:
                        return E_LEQ_E;

                    // E -> E < E
                    case L:
                        return E_L_E;

                    // E -> E >= E
                    case MEQ:
                        return E_MEQ_E;

                    // E -> E > E
                    case M:
                        return E_M_E;

                    // zly operator
                    default:
                        return NOT_A_RULE;
                }
            }
            return NOT_A_RULE;
	}
	return NOT_A_RULE;
}


static int check_sem(Prec_rules rule, tStack_item* op1, tStack_item* op2, tStack_item* op3, enum data_types *type_after_reduce){
    
    //semanticke kontroly pre nedefinovane premenne
    if (rule == OPERAND){
        if (op1->data_type == TYPE_UNDEFINED){
            return SEM_ERR_UNDEFINED_VAR;
        }
    }

    if (rule == E_BRACKETS){
        if (op2->data_type == TYPE_UNDEFINED){
            return SEM_ERR_UNDEFINED_VAR;
        }
    }

    if (rule != OPERAND && rule != E_BRACKETS){
        if (op1->data_type == TYPE_UNDEFINED || op3->data_type == TYPE_UNDEFINED){
            return SEM_ERR_UNDEFINED_VAR;
        }
    }

    switch(rule){
        case E_EQ_E:
        case E_NEQ_E:
        case E_LEQ_E:
        case E_L_E:
        case E_MEQ_E:
        case E_M_E:

            *type_after_reduce = TYPE_BOOL;
            if ((op1->data_type == TYPE_STRING) || (op3->data_type == TYPE_STRING) ){
                return SEM_ERR_EXP;
            }
            if ((op1->data_type == TYPE_INT) && (op3->data_type == TYPE_DOUBLE) ){
                return SEM_ERR_EXP;
            }
            if ((op1->data_type == TYPE_DOUBLE) && (op3->data_type == TYPE_INT) ){
                return SEM_ERR_EXP;
            }
            break;
        case E_PLUS_E:
        case E_MINUS_E:
        case E_MUL_E:
        case E_DIV_E:
            if ((op1->data_type == TYPE_STRING) && (op3->data_type == TYPE_STRING) && (rule == E_PLUS_E) ){
                *type_after_reduce = TYPE_STRING;
                break;
            }
            
            if ((op1->data_type == TYPE_STRING) || (op3->data_type == TYPE_STRING) ){
                return SEM_ERR_EXP;
            }

            if ((op1->data_type == TYPE_INT) && (op3->data_type == TYPE_INT) ){
                *type_after_reduce = TYPE_INT;
                break;
            }
            else if ((op1->data_type == TYPE_DOUBLE) && (op3->data_type == TYPE_DOUBLE) ){
                *type_after_reduce = TYPE_DOUBLE;
                break;
            }
            else {
                return SEM_ERR_EXP;
            }
            break;
        case E_BRACKETS:
            *type_after_reduce = op2->data_type;
            break;
        case OPERAND:
            *type_after_reduce = op1->data_type;
            break;

        default:
            break;
    }
    return SYN_OK;
}

// static void print_stack(){
//     tStack_item *tmp = stack_top(&stack);
//     printf("TOP->");
//     while (tmp != NULL) {
//         printf(" %d,",tmp->symbol);
//         tmp = tmp->next;
//     }
//     printf("\n");

// }

static int reduce(){
	int result;

	tStack_item* op1 = NULL;
	tStack_item* op2 = NULL;
	tStack_item* op3 = NULL;
	enum data_types type_after_reduce = TYPE_UNDEFINED;
	Prec_rules rule;
	bool found = false;

    int count = 0;

    //spocitam pocet symbolom az po stop
    tStack_item *i = stack_top(&stack);
    for (; i != NULL; i = i->next){
        if (i->symbol == STOP){
            found = true;
            break;
        }
        count++;
    }
    // printf("STACK BEFORE REDUCE\n");
    // print_stack();

	if (count == 1 && found){
		op1 = stack.top;
		rule = test_rule(count, op1, NULL, NULL);
	}
	else if (count == 3 && found){
		op1 = stack.top->next->next;
		op2 = stack.top->next;
		op3 = stack.top;
		rule = test_rule(count, op1, op2, op3);
	}
	else{
		return SYN_ERR;
    }

	if (rule == NOT_A_RULE){
		return SYN_ERR;
	}
	else{
        //skontrolujem semantiku vyrazu
		if ((result = check_sem(rule, op1, op2, op3, &type_after_reduce))){
			return result;
        }

        //generovanie kodu
        if (rule == E_PLUS_E && type_after_reduce == TYPE_STRING){
            void concat_strings();
		}
		else {
            gen_arithmetic(rule);
        }

        //popnem count+1 znakov
        for (int i = 0; i < count + 1; i++){
            stack_pop(&stack);
        }
		stack_push(&stack, NON_TERM, type_after_reduce);
	}

    // printf("STACK AFTER REDUCE\n");
    // print_stack();

	return SYN_OK;
}

static int circle_number(int cislo){
    if (cislo < 0){
        if (cislo == -1){
            return 3;
        }
        if (cislo == -2){
            return 2;
        }
        if (cislo == -3){
            return 1;
        }
    }
    return cislo;
}


int expression(struct parser_data* data, bool *nondetermism){

    int result_exp = SYN_ERR;

    stack_init(&stack);

    if (!stack_push(&stack, DOLLAR, TYPE_UNDEFINED)){
        free_resources();
        return ERROR_INTERNAL;
    }

    tStack_item* top_stack_terminal;
	Prec_table_symbol actual_symbol;
    bool resolved_d = false;

	bool success = false;

    Prec_table_symbol last4[4] = {NON_TERM, NON_TERM, NON_TERM, NON_TERM};
    int last4_c = 1;
    last4[0] = get_symbol_from_token(&data->token);

    do
	{
		actual_symbol = get_symbol_from_token(&data->token);
		top_stack_terminal = stack_top_term(&stack);

        //last4[last4_c] = actual_symbol;

        if ((data->token.type == T_TYPE_INTEGER) && (data->token.attribute.int_literal == 0)){
            last4[last4_c] = ZERO_NUMBER;
        }
        if ((data->token.type == T_TYPE_DOUBLE) && (data->token.attribute.double_literal == 0)){
            last4[last4_c] = ZERO_NUMBER;
        }


        if ((last4[circle_number(last4_c - 1)] == DIV) && (last4[last4_c] == ZERO_NUMBER)){
            free_resources();
            return SEM_ERR_ZERO_DIV;
        }


        if ((last4[circle_number(last4_c - 3)] == DIV) 
            && (last4[circle_number(last4_c - 2)] == LEFT_BRACKET) 
            && (last4[circle_number(last4_c - 1)] == ZERO_NUMBER) 
            && (last4[last4_c] == RIGHT_BRACKET)){
            free_resources();
            return SEM_ERR_ZERO_DIV;
        }



		if (top_stack_terminal == NULL){
			free_resources();
            return ERROR_INTERNAL;
        }
		switch (prec_table[top_stack_terminal->symbol][actual_symbol]){
            case S:
                if (!stack_push_after_top_term(&stack, STOP, TYPE_UNDEFINED)){
                    free_resources();
                    return ERROR_INTERNAL;
                }
                //printf("SHIFTUJEM NA STACK->typ=%d\n", get_data_type(data));
                if(!stack_push(&stack, actual_symbol, get_data_type(data))){
                    free_resources();
                    return ERROR_INTERNAL;
                }

                if (actual_symbol == IDENTIFIER || actual_symbol == INT_NUMBER 
                    || actual_symbol == DOUBLE_NUMBER || actual_symbol == STRING){
                    
                    push_value(&data->token);
                }


                if (last4_c == 3){
                    last4_c = 0;
                }
                else {
                    last4_c++;
                }
                //TU POSLAT INFO PARSERU 
                result_exp = get_token(&data->token);
                last4[last4_c] = get_symbol_from_token(&data->token);
                if ((*nondetermism == true) && (resolved_d == false)){
                    if (data->token.type == T_TYPE_LEFT_BRACKET){
                        *nondetermism = false;
                        resolved_d = true;
                        free_resources();
                        return SYN_OK;
                    }
                }

                
                if ((result_exp)){
                    free_resources();
                    return result_exp;
                }
                break;

            case E:
                stack_push(&stack, actual_symbol, get_data_type(data));

                if (last4_c == 3){
                    last4_c = 0;
                }
                else {
                    last4_c++;
                }

                result_exp = get_token(&data->token);
                last4[last4_c] = get_symbol_from_token(&data->token);
                if ((result_exp)){
                    free_resources();
                    return result_exp;
                }

                break;

            case R:
                result_exp = reduce();
                
                if (result_exp != SYN_OK){
                    free_resources();
                    return result_exp;
                }
                break;

            case N:
                if (actual_symbol == DOLLAR && top_stack_terminal->symbol == DOLLAR){
                    success = true;
                }
                else{
                    free_resources();
                    return SYN_ERR;
                }
                break;
		}
	} while (!success);

    tStack_item *expression_result= stack_top(&stack);
	if (expression_result == NULL){
        free_resources();
        return SYN_ERR;
    }
    //na vrchole sa musi nachadzat vysledok vyrazu(posledny nonterm(E))
	if (expression_result->symbol != NON_TERM){
        free_resources();
        return SYN_ERR;
    }

    //TODO
    //nastavenie TYPU idcka pri definicii
    if (data->set_type_id == true){
        if (expression_result->data_type == TYPE_BOOL){
            free_resources();
            return SEM_ERR_OTHER;
        }



        tID_queue_item *top_queue = id_queue_top(&data->ID_queue);
        //pri definicii nemoze byt _
        hKey_t tmp = "_";
        if (str_cmp_const_str(&top_queue->id, tmp) == 0){
            return SEM_ERR_OTHER;
        }

        tBT_stack_item *top_bt_stack = bt_stack_top(&data->BT_stack);
        bool internal_err;
        Data_t *search_found = BT_search(&top_bt_stack->local_bt, top_queue->id.str, &internal_err);
        if (internal_err == true){
            free_resources();
            return ERROR_INTERNAL;
        }
        search_found->type = expression_result->data_type;
    }
    //ked sme v ife alebo vo fore v podmienke, vysledok musi btr typpu bool
    else if (data->in_if_for == true){
        if (expression_result->data_type != TYPE_BOOL){
            free_resources();
            return SEM_ERR_OTHER;
        }
    }
    //pri assigne musia sa zhodovat typy
    else if (data->check_type == true && data->check_returns == false){

        bool internal_err;
        tID_queue_item *top_queue_def = id_queue_top(&data->ID_queue);
        //viac vyrazov na pravej strane ako idciek na lavej
        if (top_queue_def == NULL){
            free_resources();
            return SEM_ERR_OTHER;
        }
        hKey_t tmp = "_";
        if (str_cmp_const_str(&top_queue_def->id, tmp) == 0){
            return SYN_OK;
        }

        Data_t *search_found = bt_stack_search(&data->BT_stack, top_queue_def->id.str, &internal_err);
        if (internal_err == true){
            free_resources();
            return ERROR_INTERNAL;
        }
        if (search_found == NULL){
            free_resources();
            return SEM_ERR_UNDEFINED_VAR;
        }

        if (search_found->type != expression_result->data_type){
            free_resources();
            return SEM_ERR_OTHER;
        }
    }
    else if (data->check_returns == true){
        if (expression_result->data_type == TYPE_BOOL){
            free_resources();
            return SEM_ERR_OTHER;
        }

        tID_queue_item *n_id = n_item(&data->actual_func->func_params, data->checked_returns);
        if (n_id == NULL){
            free_resources();
            return SEM_ERR_NO_PARAMS;
        }
        if (expression_result->data_type == TYPE_INT){
            hKey_t tmp = "int";
            if (str_cmp_const_str(&n_id->id, tmp) != 0){
                free_resources();
                return SEM_ERR_NO_PARAMS;
            }
        }
        else if (expression_result->data_type == TYPE_DOUBLE){
            hKey_t tmp = "double";
            if (str_cmp_const_str(&n_id->id, tmp) != 0){
                free_resources();
                return SEM_ERR_NO_PARAMS;
            }

        }
        else if (expression_result->data_type == TYPE_STRING){
            hKey_t tmp = "string";
            if (str_cmp_const_str(&n_id->id, tmp) != 0){
                free_resources();
                return SEM_ERR_NO_PARAMS;
            }
        }
        else if (expression_result->data_type == TYPE_UNDEFINED){
            free_resources();
            return SEM_ERR_OTHER;
        }

    }


    free_resources();
    return SYN_OK;
}