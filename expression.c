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
	Data_t* symbol; //will be used

	switch (data->token.type){
        case T_TYPE_IDENTIFIER:
        //TODO
            // symbol = BT_search(&data->local_table, token->attribute.string->str);
            // if (symbol == NULL)
            //     return TYPE_UNDEFINED;
            // return symbol->type;
            return TYPE_UNDEFINED;

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

    //semanticka kontrola aritmetickych operacii
    //TODO
    switch(rule){
        case E_EQ_E:
        case E_NEQ_E:
        case E_LEQ_E:
        case E_L_E:
        case E_MEQ_E:
        case E_M_E:
            break;
        case E_PLUS_E:
            break;
        case E_MINUS_E:
            break;
        case E_MUL_E:
            break;
        case E_DIV_E:
            break;
        case E_BRACKETS:
            break;
        case OPERAND:
            break;

        default:
            break;
    }
    return SYN_OK;
}

static void print_stack(){
    tStack_item *tmp = stack_top(&stack);
    printf("TOP->");
    while (tmp != NULL) {
        printf(" %d,",tmp->symbol);
        tmp = tmp->next;
    }
    printf("\n");

}

static int reduce(){
	int result;

	tStack_item* op1 = NULL;
	tStack_item* op2 = NULL;
	tStack_item* op3 = NULL;
	enum data_types type_after_reduce = TYPE_INT; //ZMENIT NA NEDEFINOIVANE //TODO
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
        //printf("TU SOM V EXPRESSION4\n");
		return SYN_ERR;
    }

	if (rule == NOT_A_RULE){
        //printf("TU SOM V EXPRESSION5\n");
		return SYN_ERR;
	}
	else{
        // //skontrolujem semantiku vyrazu
		// if ((result = check_sem(rule, op1, op2, op3, &type_after_reduce))){
		// 	return result;
        // }

		if (rule == E_PLUS_E && type_after_reduce == TYPE_STRING){
			//GENERATE_CODE(generate_concat_stack_strings);
		}
		else {
            //GENERATE_CODE(generate_stack_operation, rule);
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




int expression(struct parser_data* data){

    int result = SYN_ERR;

    stack_init(&stack);

    if (!stack_push(&stack, DOLLAR, TYPE_UNDEFINED)){
        free_resources();
        return ERROR_INTERNAL;
    }

    tStack_item* top_stack_terminal;
	Prec_table_symbol actual_symbol;

	bool success = false;

    do
	{
		actual_symbol = get_symbol_from_token(&data->token);
		top_stack_terminal = stack_top_term(&stack);

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

                if(!stack_push(&stack, actual_symbol, get_data_type(data))){
                    free_resources();
                    return ERROR_INTERNAL;
                }

                if ((result = get_token(&data->token))){
                    free_resources();
                    return result;
                }
                break;

            case E:
                stack_push(&stack, actual_symbol, get_data_type(data));

                if ((result = get_token(&data->token))){
                    free_resources();
                    return result;
                }
                break;

            case R:
                if ((result = reduce())){
                    free_resources();
                    return result;
                }
                break;

            case N:
                if (actual_symbol == DOLLAR && top_stack_terminal->symbol == DOLLAR){
                    success = true;
                }
                else{
                    free_resources();
                    //printf("TU SOM V EXPRESSION1\n");
                    return SYN_ERR;
                }
                break;
		}
	} while (!success);

    tStack_item *expression_result= stack_top(&stack);
	if (expression_result == NULL){
        free_resources();
        //printf("TU SOM V EXPRESSION2\n");
        return SYN_ERR;
    }
    //na vrchole sa musi nachadzat vysledok vyrazu(posledny nonterm(E))
	if (expression_result->symbol != NON_TERM){
        free_resources();
        //printf("TU SOM V EXPRESSION3\n");
        return SYN_ERR;
    }

    free_resources();
    return SYN_OK;
}