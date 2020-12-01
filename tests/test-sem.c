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
#define FILE_COUNT_INCORRECT 6

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
                            "tests/test_files_sem_incorrect/sem_func_change_nondef_ID_value.test",};
    
    printf("\n\n TESTY NA SUBOROCH KTORE BY MALI VRACAT SYN_ERR\n");
    for (int i = 0; i < FILE_COUNT_INCORRECT; i++){
        //blue_color();
        printf("\n[TEST%d]\n", i+1);
        printf("Testovanie semantiky parseru na subore: \"%s\"\n",filenames_incorrect[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        //reset();

        test_sem(filenames_incorrect[i]);
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