#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../parser.h"
#include "../scanner.h"
#include "../symtable.h"
#include "../str.h"
#include "../error.h"
#include "../expression.h"


#define FILE_COUNT_CORRECT 17
#define FILE_COUNT_INCORRECT 135
#define FILE_COUNT_UJEBANE 68

void reset() {
  printf("\033[0m");
}

void blue_color() {
  printf("\033[0;34m");
}

void red_color() {
  printf("\033[0;31m");
}

void print_file(FILE *f){
    int c = fgetc(f); 
    printf("zaciatok suboru\n");
    printf("-------------------------------------------------------\n");
    while (c != EOF) 
    { 
        printf ("%c", c); 
        c = fgetc(f); 
    } 
    printf("\n-------------------------------------------------------\n");
    printf("koniec suboru\n");
    
}

void test_parser(char *filename){
    //subor vypis
    FILE *f_print;
    f_print = fopen(filename, "r"); 
    if (f_print == NULL) { 
        //red_color();
        printf("Nepodarilo sa otvorit subor: \"%s\"\n", filename); 
        //reset();
        printf("errno: %d\n",errno);
        
        return;
    }
    print_file(f_print);
    fclose(f_print);

    FILE *f;
    f = fopen(filename, "r"); 
    if (f == NULL) { 
        //red_color();
        printf("Nepodarilo sa otvorit subor: \"%s\"\n", filename); 
        //reset();
        printf("errno: %d\n",errno);
        
        return;
    }
    setSourceFile(f);
    int tmp = parse();

    printf("EXIT CODE JE : %d\n", tmp);
   
    fclose(f);
}

void kontrola_parser(){
    int test_count = 1;

    // CORRECT
    
    char *filenames[FILE_COUNT_CORRECT] =  
                            {"tests/test_files_parser/parser_nondet1.test",
                            "tests/test_files_parser/parser_nondet2.test",
                            "tests/test_files_parser/parser_nondet3.test",
                            "tests/test_files_parser/parser_nondet4.test",
                            "tests/test_files_parser/parser_nondet5.test",
                            "tests/test_files_parser/parser_nondet6.test",
                            "tests/test_files_parser/parser_nondet7.test",
                            "tests/test_files_parser/parser_nondet8.test",
                            "tests/test_files_parser/parser_package_main.test",
                             "tests/test_files_parser/parser_func_main.test",
                             "tests/test_files_parser/parser_func_for.test",
                             "tests/test_files_parser/parser_func_if.test",
                             "tests/test_files_parser/parser_func_return_type.test",
                             "tests/test_files_parser/parser_func_call_of_func.test",
                             "tests/test_files_parser/parser_func_:=.test",
                             "tests/test_files_parser/parser_func_ids.test",
                             "tests/test_files_parser/parser_func_returns.test",};

    for (int i = 0; i < FILE_COUNT_CORRECT; i++){
        //blue_color();
        printf("\n[TEST%d]\n", i+1);
        printf("Testovanie parseru na subore: \"%s\"\n",filenames[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        //reset();

        test_parser(filenames[i]);
    }

    // INCORRECT

    char *filenames_incorrect[FILE_COUNT_INCORRECT] =  
                            {"tests/test_files_parser/parser_nondet_incorrect1.test",
                            "tests/test_files_parser/parser_nondet_incorrect2.test",
                            "tests/test_files_parser/parser_nondet_incorrect3.test",
                            "tests/test_files_parser/parser_nondet_incorrect4.test",
                            "tests/test_files_parser/parser_nondet_incorrect5.test",
                            "tests/test_files_parser/parser_nondet_incorrect6.test",
                            "tests/test_files_parser/parser_nondet_incorrect7.test",
                            "tests/test_files_parser/parser_nondet_incorrect8.test",
                            "tests/test_files_parser/parser_nondet_incorrect9.test",
                            "tests/test_files_parser/parser_nondet_incorrect10.test",
                            "tests/test_files_parser/parser_nondet_incorrect11.test",
                            "tests/test_files_parser/parser_nondet_incorrect12.test",
                            "tests/test_files_parser/parser_nondet_incorrect13.test",
                            "tests/test_files_parser/parser_nondet_incorrect14.test",
                             "tests/test_files_parser/parser_package_main_incorrect.test",
                             "tests/test_files_parser/parser_func_main_incorrect.test",
                             "tests/test_files_parser/parser_func_for_if_incorrect_eol_1.test",
                             "tests/test_files_parser/parser_func_for_if_incorrect_eol_2.test",
                             "tests/test_files_parser/parser_func_for_if_incorrect_eol_3.test",
                             "tests/test_files_parser/parser_declaration_eol1.test",
                             "tests/test_files_parser/parser_declaration_eol2.test",
                             "tests/test_files_parser/parser_declaration_eol3.test",
                             "tests/test_files_parser/parser_eol1.test",
                             "tests/test_files_parser/parser_eol2.test",
                             "tests/test_files_parser/parser_eol3.test",
                             "tests/test_files_parser/parser_eol4.test",
                             "tests/test_files_parser/parser_eol5.test",
                             "tests/test_files_parser/parser_eol6.test",
                             "tests/test_files_parser/parser_eol7.test",
                             "tests/test_files_parser/parser_eol8.test",
                             "tests/test_files_parser/parser_eol9.test",
                             "tests/test_files_parser/parser_eol10.test",
                             "tests/test_files_parser/parser_eol11.test",
                             "tests/test_files_parser/parser_eol12.test",
                             "tests/test_files_parser/parser_eol13.test",
                             "tests/test_files_parser/parser_eol14.test",
                             "tests/test_files_parser/parser_eol15.test",
                             "tests/test_files_parser/parser_eol16.test",
                             "tests/test_files_parser/parser_eol17.test",
                             "tests/test_files_parser/parser_eol18.test",
                             "tests/test_files_parser/parser_fcall_eol1.test",
                             "tests/test_files_parser/parser_fcall_eol2.test",
                             "tests/test_files_parser/parser_fcall_eol3.test",
                             "tests/test_files_parser/parser_fcall_eol4.test",
                             "tests/test_files_parser/parser_fcall_eol5.test",
                             "tests/test_files_parser/parser_fcall_eol6.test",
                             "tests/test_files_parser/parser_fcall_eol7.test",
                             "tests/test_files_parser/parser_for_eol1.test",
                             "tests/test_files_parser/parser_for_eol2.test",
                             "tests/test_files_parser/parser_for_eol3.test",
                             "tests/test_files_parser/parser_for_eol4.test",
                             "tests/test_files_parser/parser_for_eol5.test",
                             "tests/test_files_parser/parser_for_eol6.test",
                             "tests/test_files_parser/parser_for_eol7.test",
                             "tests/test_files_parser/parser_for_eol8.test",
                             "tests/test_files_parser/parser_for_eol9.test",
                             "tests/test_files_parser/parser_for_eol10.test",
                             "tests/test_files_parser/parser_for_eol11.test",
                             "tests/test_files_parser/parser_for_eol12.test",
                             "tests/test_files_parser/parser_for_eol13.test",
                             "tests/test_files_parser/parser_for_eol14.test",
                             "tests/test_files_parser/parser_for_eol15.test",
                             "tests/test_files_parser/parser_for_eol16.test",
                             "tests/test_files_parser/parser_for_eol17.test",
                             "tests/test_files_parser/parser_for_eol18.test",
                             "tests/test_files_parser/parser_for_eol19.test",
                             "tests/test_files_parser/parser_for_eol20.test",
                             "tests/test_files_parser/parser_for_eol21.test",
                             "tests/test_files_parser/parser_for_eol22.test",
                             "tests/test_files_parser/parser_ids_eol1.test",
                             "tests/test_files_parser/parser_ids_eol2.test",
                             "tests/test_files_parser/parser_ids_eol3.test",
                             "tests/test_files_parser/parser_ids_eol4.test",
                             "tests/test_files_parser/parser_ids_eol5.test",
                             "tests/test_files_parser/parser_ids_eol6.test",
                             "tests/test_files_parser/parser_ids_eol7.test",
                             "tests/test_files_parser/parser_ids_eol8.test",
                             "tests/test_files_parser/parser_ids_eol9.test",
                             "tests/test_files_parser/parser_ids_eol10.test",
                             "tests/test_files_parser/parser_ids_eol11.test",
                             "tests/test_files_parser/parser_ids_eol12.test",
                             "tests/test_files_parser/parser_ids_eol13.test",
                             "tests/test_files_parser/parser_ids_eol14.test",
                             "tests/test_files_parser/parser_if_eol1.test",
                             "tests/test_files_parser/parser_if_eol2.test",
                             "tests/test_files_parser/parser_if_eol3.test",
                             "tests/test_files_parser/parser_if_eol4.test",
                             "tests/test_files_parser/parser_if_eol5.test",
                             "tests/test_files_parser/parser_if_eol6.test",
                             "tests/test_files_parser/parser_if_eol7.test",
                             "tests/test_files_parser/parser_if_eol8.test",
                             "tests/test_files_parser/parser_if_eol9.test",
                             "tests/test_files_parser/parser_if_eol10.test",
                             "tests/test_files_parser/parser_if_eol11.test",
                             "tests/test_files_parser/parser_if_eol12.test",
                             "tests/test_files_parser/parser_if_eol13.test",
                             "tests/test_files_parser/parser_if_eol14.test",
                             "tests/test_files_parser/parser_if_eol15.test",
                             "tests/test_files_parser/parser_if_eol16.test",
                             "tests/test_files_parser/parser_if_eol17.test",
                             "tests/test_files_parser/parser_if_eol18.test",
                             "tests/test_files_parser/parser_incorrect_1.test",
                             "tests/test_files_parser/parser_incorrect_2.test",
                             "tests/test_files_parser/parser_incorrect_3.test",
                             "tests/test_files_parser/parser_incorrect_4.test",
                             "tests/test_files_parser/parser_incorrect_5.test",
                             "tests/test_files_parser/parser_incorrect_6.test",
                             "tests/test_files_parser/parser_incorrect_7.test",
                             "tests/test_files_parser/parser_incorrect_8.test",
                             "tests/test_files_parser/parser_incorrect_9.test",
                             "tests/test_files_parser/parser_incorrect_10.test",
                             "tests/test_files_parser/parser_incorrect_11.test",
                             "tests/test_files_parser/parser_incorrect_12.test",
                             "tests/test_files_parser/parser_incorrect_13.test",
                             "tests/test_files_parser/parser_incorrect_14.test",
                             "tests/test_files_parser/parser_incorrect_15.test",
                             "tests/test_files_parser/parser_incorrect_16.test",
                             "tests/test_files_parser/parser_incorrect_17.test",
                             "tests/test_files_parser/parser_incorrect_18.test",
                             "tests/test_files_parser/parser_incorrect_19.test",
                             "tests/test_files_parser/parser_incorrect_20.test",
                             "tests/test_files_parser/parser_incorrect_21.test",
                             "tests/test_files_parser/parser_incorrect_22.test",
                             "tests/test_files_parser/parser_incorrect_23.test",
                             "tests/test_files_parser/parser_incorrect_24.test",
                             "tests/test_files_parser/parser_incorrect_25.test",
                             "tests/test_files_parser/parser_incorrect_26.test",
                             "tests/test_files_parser/parser_incorrect_27.test",
                             "tests/test_files_parser/parser_incorrect_28.test",
                             "tests/test_files_parser/parser_incorrect_29.test",
                             "tests/test_files_parser/parser_incorrect_30.test",
                             "tests/test_files_parser/parser_return_eol1.test",
                             "tests/test_files_parser/parser_return_eol2.test",
                             "tests/test_files_parser/parser_return_eol3.test",
                             "tests/test_files_parser/parser_return_eol4.test"};
    
    printf("\n\n TESTY NA SUBOROCH KTORE BY MALI VRACAT SYN_ERR\n");
    for (int i = 0; i < FILE_COUNT_INCORRECT; i++){
        //blue_color();
        printf("\n[TEST%d]\n", i+1);
        printf("Testovanie parseru na subore: \"%s\"\n",filenames_incorrect[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        //reset();

        test_parser(filenames_incorrect[i]);
    }

    // UJEBANE
    
    char *filenames[FILE_COUNT_UJEBANE] =  
                            {"syn/function/bad_function_kw.go",
                             "syn/function/double_function_id.go",
                             "syn/function/empty_returns.go",
                             "syn/function/eol_in_function_1.go",
                             "syn/function/eol_in_function_2.go",
                             "syn/function/function_missing_spaces.go",
                             "syn/function/function_without_body.go",
                             "syn/function/multiple_blocks.go",
                             "syn/function/nested_functions.go",
                             "syn/function/no_param_list.go",
                             "syn/function/no_params_returns.go",
                             "syn/function/params_returns.go",
                             "syn/function/params_with_value.go",
                             "syn/function/params_without_comma.go",
                             "syn/function/params_without_id.go",
                             "syn/function/params_without_type.go",
                             "syn/function/params.go",
                             "syn/function/returns_with_id.go",
                             "syn/function/returns_without_comma.go",
                             "syn/function/returns.go",
                             "syn/function/switched_params_returns.go",
                             "syn/function/unending_function.go",
                             "syn/statement/assign_multiple.go",
                             "syn/statement/assign_without_comma.go",
                             "syn/statement/assign_without_expr.go",
                             "syn/statement/assign_without_id.go",
                             "syn/statement/assign.go",
                             "syn/statement/bad_call_assign.go",
                             "syn/statement/bad_call.go",
                             "syn/statement/call_assign_multiple.go",
                             "syn/statement/call_assign.go",
                             "syn/statement/call_no_term.go",
                             "syn/statement/call_params.go",
                             "syn/statement/call_with_types.go",
                             "syn/statement/call.go",
                             "syn/statement/def_without_expr.go",
                             "syn/statement/def_without_id.go",
                             "syn/statement/def.go",
                             "syn/statement/for_assign.go",
                             "syn/statement/for_bad_assign.go",
                             "syn/statement/for_bad_def.go",
                             "syn/statement/for_def_assign.go",
                             "syn/statement/for_def.go",
                             "syn/statement/for_no_semicolons.go",
                             "syn/statement/for_with_eol.go",
                             "syn/statement/for_without_eol.go",
                             "syn/statement/for_without_expr.go",
                             "syn/statement/for.go",
                             "syn/statement/if_else_with_expr.go",
                             "syn/statement/if_with_eol.go",
                             "syn/statement/if_without_else.go",
                             "syn/statement/if_without_eol.go",
                             "syn/statement/if_without_expr.go",
                             "syn/statement/if_without_then.go",
                             "syn/statement/if.go",
                             "syn/statement/multiple_eols.go",
                             "syn/statement/no_eols_statements.go",
                             "syn/statement/return_value.go",
                             "syn/statement/return.go",
                             "syn/top/bad_header.go",
                             "syn/top/case_sensitive_kw.go",
                             "syn/top/ending_eol.go",
                             "syn/top/functions.go",
                             "syn/top/gloabl_variable.go",
                             "syn/top/no_eol.go",
                             "syn/top/no_function.go",
                             "syn/top/no_header.go",
                             "syn/top/starting_eol.go"};

    printf("\n\n TESTY NA SUBOROCH KTORE SME UJEBALI A NEVIEME CO MAJU VRACAT\n");
    for (int i = 0; i < FILE_COUNT_CORRECT; i++){
        //blue_color();
        printf("\n[TEST%d]\n", i+1);
        printf("Testovanie parseru na subore: \"%s\"\n",filenames[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        //reset();

        test_parser(filenames[i]);
    }
}

int main()
{
    printf("Testy pre parser\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\nTesty funkcii v parser.c \n");
    printf("==================================\n");

    kontrola_parser();
    printf("\nKoniec testov funkcii v parser.c \n");
    printf("==================================\n");
    printf("==================================\n");

    return 0;
}