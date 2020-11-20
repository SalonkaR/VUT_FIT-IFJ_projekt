#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../symtable.h"
#include "../scanner.h"
#include "../str.h"
#include "../error.h"
#include "../parser.h"

#define FILE_COUNT_CORRECT 1
#define FILE_COUNT_INCORRECT 0

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

void test_parser(char *filename){
    
    parse();
}

void kontrola_parser(){
    //TODO -> SUBORY NA KTORE SCANNER VYHODI CHYBU
    int test_count = 0;

    // KONTROLA SUBOROV KTORE BY MALI MAT SPRAVNE ZAPSANE TOKENY
    
    char *filenames[FILE_COUNT_CORRECT] =  
                            {"tests/test_files_parser/parser_package_main.test"};

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
                            {};
    
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