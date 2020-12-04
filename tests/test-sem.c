#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../parser.h"
#include "../scanner.h"
#include "../symtable.h"
#include "../str.h"
#include "../error.h"
#include "../expression.h"
#include "../bt_stack.h"


#define FILE_COUNT_CORRECT 5
#define FILE_COUNT_INCORRECT 24
#define FILE_COUNT_UJEBANE 0//139

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

void test_sem(char *filename){
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

    printf("EXIT CODE JE : --------------------------------------------------------------------%d\n", tmp);
   
    fclose(f);
}

void kontrola_sem(){

    // CORRECT
    
    char *filenames[FILE_COUNT_CORRECT] =  
                            {"tests/test_files_sem_correct/sem_main.test",
                             "tests/test_files_sem_correct/sem_func_change_ID_values.test",
                             "tests/test_files_sem_correct/sem_func_change_ID_value.test",
                             "tests/test_files_sem_correct/sem_func_call_defined_func.test",
                             "tests/test_files_sem_correct/sem_builtin_func.test",};

    for (int i = 0; i < FILE_COUNT_CORRECT; i++){
        //blue_color();
        printf("\n[TEST%d]\n", i+1);
        printf("Testovanie semantiky parseru na subore: \"%s\"\n",filenames[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        //reset();

        test_sem(filenames[i]);
    }

    // INCORRECT

    char *filenames_incorrect[FILE_COUNT_INCORRECT] =  
                            {"tests/test_files_sem_incorrect/sem_func_call_nondefined_func.test",
                            "tests/test_files_sem_incorrect/sem_no_main.test",
                            "tests/test_files_sem_incorrect/sem_func_use_nonexist_ID_value.test",
                            "tests/test_files_sem_incorrect/sem_func_use_ID_from_other_func.test",
                            "tests/test_files_sem_incorrect/sem_func_change_toomany_ID_values.test",
                            "tests/test_files_sem_incorrect/sem_func_change_nondef_ID_value.test",
                            "tests/test_files_sem_incorrect/sem_redef_var.test",
                            "tests/test_files_sem_incorrect/sem_exp1.test",
                            "tests/test_files_sem_incorrect/sem_exp_mul_ass1.test",
                            "tests/test_files_sem_incorrect/sem_moreright1.test",
                            "tests/test_files_sem_incorrect/sem_zero_div1.test",
                            "tests/test_files_sem_incorrect/sem_zero_div2.test",
                            "tests/test_files_sem_incorrect/sem_zero_div3.test",
                            "tests/test_files_sem_incorrect/sem_zero_div4.test",
                            "tests/test_files_sem_incorrect/sem_return1.test",
                            "tests/test_files_sem_incorrect/sem_nols_funcall1.test",
                            "tests/test_files_sem_incorrect/sem_lsns1.test",
                            "tests/test_files_sem_incorrect/sem_lsns2.test",
                            "tests/test_files_sem_incorrect/sem_lsns3.test",
                            "tests/test_files_sem_incorrect/sem_wrong_returns1.test",
                            "tests/test_files_sem_incorrect/sem_wrong_returns2.test",
                            "tests/test_files_sem_incorrect/sem_wrong_returns3.test",
                            "tests/test_files_sem_incorrect/sem_wrong_returns4.test",
                            "tests/test_files_sem_incorrect/sem_main_returnvalues.test",};
    
    printf("\n\n TESTY NA SUBOROCH KTORE BY MALI VRACAT SYN_ERR\n");
    for (int i = 0; i < FILE_COUNT_INCORRECT; i++){
        //blue_color();
        printf("\n[TEST%d]\n", i+1);
        printf("Testovanie semantiky parseru na subore: \"%s\"\n",filenames_incorrect[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        //reset();

        test_sem(filenames_incorrect[i]);
    }
    // UJEBANE

    char *filenames_ujebane[FILE_COUNT_UJEBANE] =  
                            {"sem/builtin/cast_1.go",
                            "sem/builtin/cast_2.go",
                            "sem/builtin/cast_3.go",
                            "sem/builtin/cast_4.go",
                            "sem/builtin/cast_1.go",
                            "sem/builtin/chr_1.go",
                            "sem/builtin/chr_2.go",
                            "sem/builtin/inputf_1.go",
                            "sem/builtin/inputf_2.go",
                            "sem/builtin/inputi_1.go",
                            "sem/builtin/inputi_2.go",
                            "sem/builtin/inputs_1.go",
                            "sem/builtin/inputs_2.go",
                            "sem/builtin/len_1.go",
                            "sem/builtin/len_2.go",
                            "sem/builtin/ord_1.go",
                            "sem/builtin/ord_2.go",
                            "sem/builtin/ord_3.go",
                            "sem/builtin/print_all.go",
                            "sem/builtin/print_bad.go",
                            "sem/builtin/print_empty.go",
                            "sem/builtin/print_float.go",
                            "sem/builtin/print_int.go",
                            "sem/builtin/print_string.go",
                            "sem/builtin/substr_1.go",
                            "sem/builtin/substr_2.go",
                            "sem/builtin/substr_3.go",
                            "sem/declaration/multiple_functions.go",
                            "sem/declaration/multiple_vars_1.go",
                            "sem/declaration/multiple_vars_2.go",
                            "sem/declaration/multiple_vars_3.go",
                            "sem/declaration/multiple_vars_4.go",
                            "sem/declaration/multiple_vars_5.go",
                            "sem/declaration/nondefined_var_1.go",
                            "sem/declaration/nondefined_var_2.go",
                            "sem/declaration/nondefined_var_3.go",
                            "sem/declaration/nondefined_var_4.go",
                            "sem/expresion/add_1.go",
                            "sem/expresion/add_2.go",
                            "sem/expresion/add_3.go",
                            "sem/expresion/add_4.go",
                            "sem/expresion/add_5.go",
                            "sem/expresion/add_6.go",
                            "sem/expresion/add_7.go",
                            "sem/expresion/add_8.go",
                            "sem/expresion/add_9.go",
                            "sem/expresion/div_1.go",
                            "sem/expresion/div_2.go",
                            "sem/expresion/div_3.go",
                            "sem/expresion/div_4.go",
                            "sem/expresion/div_5.go",
                            "sem/expresion/div_6.go",
                            "sem/expresion/div_7.go",
                            "sem/expresion/div_zero.go",
                            "sem/expresion/div_zero_complex.go",
                            "sem/expresion/eq_1.go",
                            "sem/expresion/eq_2.go",
                            "sem/expresion/eq_3.go",
                            "sem/expresion/eq_4.go",
                            "sem/expresion/eq_5.go",
                            "sem/expresion/eq_6.go",
                            "sem/expresion/eq_7.go",
                            "sem/expresion/gt_1.go",
                            "sem/expresion/gt_2.go",
                            "sem/expresion/gt_3.go",
                            "sem/expresion/gt_4.go",
                            "sem/expresion/gt_5.go",
                            "sem/expresion/gt_6.go",
                            "sem/expresion/gt_7.go",
                            "sem/expresion/gte_1.go",
                            "sem/expresion/gte_2.go",
                            "sem/expresion/gte_3.go",
                            "sem/expresion/gte_4.go",
                            "sem/expresion/gte_5.go",
                            "sem/expresion/gte_6.go",
                            "sem/expresion/gte_7.go",
                            "sem/expresion/lt_1.go",
                            "sem/expresion/lt_2.go",
                            "sem/expresion/lt_3.go",
                            "sem/expresion/lt_4.go",
                            "sem/expresion/lt_5.go",
                            "sem/expresion/lt_6.go",
                            "sem/expresion/lt_7.go",
                            "sem/expresion/lte_1.go",
                            "sem/expresion/lte_2.go",
                            "sem/expresion/lte_3.go",
                            "sem/expresion/lte_4.go",
                            "sem/expresion/lte_5.go",
                            "sem/expresion/lte_6.go",
                            "sem/expresion/lte_7.go",
                            "sem/expresion/mul_1.go",
                            "sem/expresion/mul_2.go",
                            "sem/expresion/mul_3.go",
                            "sem/expresion/mul_4.go",
                            "sem/expresion/mul_5.go",
                            "sem/expresion/mul_6.go",
                            "sem/expresion/mul_7.go",
                            "sem/expresion/neq_1.go",
                            "sem/expresion/neq_2.go",
                            "sem/expresion/neq_3.go",
                            "sem/expresion/neq_4.go",
                            "sem/expresion/neq_5.go",
                            "sem/expresion/neq_6.go",
                            "sem/expresion/neq_7.go",
                            "sem/expresion/sub_1.go",
                            "sem/expresion/sub_2.go",
                            "sem/expresion/sub_3.go",
                            "sem/expresion/sub_4.go",
                            "sem/expresion/sub_5.go",
                            "sem/expresion/sub_6.go",
                            "sem/expresion/sub_7.go",
                            "sem/other/bad_prolog_id.go",
                            "sem/other/case_sensitive.go",
                            "sem/other/main_params.go",
                            "sem/other/main_params_returns.go",
                            "sem/other/main_returns.go",
                            "sem/other/no_main.go",
                            "sem/statement/assign_dont_match_1.go",
                            "sem/statement/assign_dont_match_2.go",
                            "sem/statement/assign_dont_match_3.go",
                            "sem/statement/bad_return_1.go",
                            "sem/statement/bad_return_2.go",
                            "sem/statement/bad_return_3.go",
                            "sem/statement/bad_return_4.go",
                            "sem/statement/call_bad_params_1.go",
                            "sem/statement/call_bad_params_2.go",
                            "sem/statement/call_bad_return_1.go",
                            "sem/statement/call_bad_return_2.go",
                            "sem/statement/call_bad_return_3.go",
                            "sem/statement/call_undefined.go",
                            "sem/statement/for_nonbool.go",
                            "sem/statement/for_scope_1.go",
                            "sem/statement/for_scope_2.go",
                            "sem/statement/if_nonbool.go",
                            "sem/statement/no_return.go",
                            "sem/statement/underscore_usage_1.go",
                            "sem/statement/underscore_usage_2.go",
                            "sem/statement/underscore_usage_3.go",
                            "sem/statement/underscore_usage_4.go",
                            };
    
    printf("\n\n TESTY NA SUBOROCH KTORE sme ujebali \n");
    for (int i = 0; i < FILE_COUNT_UJEBANE; i++){
        //blue_color();
        printf("\n[TEST%d]\n", i+1);
        printf("Testovanie semantiky parseru na subore: \"%s\"\n",filenames_ujebane[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        //reset();

        test_sem(filenames_ujebane[i]);
    }
}

int main()
{
    printf("Testy pre parser\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\nTesty funkcii v parser.c \n");
    printf("==================================\n");

    kontrola_sem();
    printf("\nKoniec testov funkcii v parser.c \n");
    printf("==================================\n");
    printf("==================================\n");

    return 0;
}