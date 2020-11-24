#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../symtable.h"
#include "../scanner.h"
#include "../str.h"
#include "../error.h"
#include "../parser.h"

#define FILE_COUNT_CORRECT 8
#define FILE_COUNT_INCORRECT 2

void reset () {
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

void print_token(struct token *t, int ret_val, int no_token){
    printf("%d token ->Prijaty token je typu: \"%d\"\n",no_token, t->type);
    if (t->type == T_TYPE_INTEGER){
        printf("%d token ->Atributy token: int= \"%d\"\n",no_token, t->attribute.int_literal);
    }
    else if (t->type == T_TYPE_DOUBLE){
        printf("%d token ->Atributy token: double= \"%lf\"\n",no_token, t->attribute.double_literal);
    }
    else if (t->type == T_TYPE_IDENTIFIER){
        printf("%d token ->Atributy token: string= \"%s\"\n",no_token, t->attribute.string->str);
    }
    else if (t->type == T_TYPE_STRING){
        printf("%d token ->Atributy token: string= \"%s\"\n",no_token, t->attribute.string->str);
    }

    printf("%d token ->Prijaty token vracia: \"%d\"\n",no_token, ret_val);
}

void test_parser(char *filename){
    //pomocne premmene
    //int result;
    //int no_token = 0;
    
    //subor vypis
    FILE *f_print;
    f_print = fopen(filename, "r"); 
    if (f_print == NULL) { 
        red_color();
        printf("Nepodarilo sa otvorit subor: \"%s\"\n", filename); 
        reset();
        printf("errno: %d\n",errno);
        
        return;
    }
    print_file(f_print);
    fclose(f_print);

    FILE *f;
    f = fopen(filename, "r"); 
    if (f == NULL) { 
        red_color();
        printf("Nepodarilo sa otvorit subor: \"%s\"\n", filename); 
        reset();
        printf("errno: %d\n",errno);
        
        return;
    }
    setSourceFile(f);
    int tmp = parse();
    
    printf("EXIT CODE JE : %d\n", tmp);
    if (tmp == 0)
    {
        
    }
    fclose(f);
}

void kontrola_parser(){
    //TODO -> SUBORY NA KTORE PARSER VYHODI CHYBU
    int test_count = 1;

    // KONTROLA SUBOROV KTORE BY MALI MAT SPRAVNE ZAPSANE TOKENY
    
    char *filenames[FILE_COUNT_CORRECT] =  
                            {"tests/test_files_parser/parser_package_main.test",
                             "tests/test_files_parser/parser_func_main.test",
                             "tests/test_files_parser/parser_func_returns.test",
                             "tests/test_files_parser/parser_func_for.test",
                             "tests/test_files_parser/parser_func_if.test",
                             "tests/test_files_parser/parser_func_return_type.test",
                             "tests/test_files_parser/parser_func_call_of_func.test",
                             "tests/test_files_parser/parser_func_:=.test",};

    for (int i = 0; i < FILE_COUNT_CORRECT; i++){
        blue_color();
        printf("\n[TEST%d]\n", test_count);
        printf("Testovanie parseru na subore: \"%s\"\n",filenames[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        reset();

        test_parser(filenames[i]);
        test_count++;
    }

    // KONTROLA SUBOROV NA KTORYCH BY SA MAL SCANNER ZASAKNUT

    char *filenames_incorrect[FILE_COUNT_INCORRECT] =  
                            {"tests/test_files_parser/parser_package_main_incorrect.test",
                             "tests/test_files_parser/parser_func_main_incorrect.test"};
    
    printf("\n\n TESTY NA SUBOROCH KTORE BY MALI VRACAT SYN_ERR\n");
    for (int i = 0; i < FILE_COUNT_INCORRECT; i++){
        blue_color();
        printf("\n[TEST%d]\n", test_count);
        printf("Testovanie parseru na subore: \"%s\"\n",filenames_incorrect[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        reset();

        test_parser(filenames_incorrect[i]);
        test_count++;
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