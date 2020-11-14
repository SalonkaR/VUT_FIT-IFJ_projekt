//testovacie skripty pre lexikalny analyzator 

#include <stdio.h>
#include <stdlib.h>
#include "../error.h"
#include "../scanner.h"
#include "../str.h"

#define FILE_COUNT_CORRECT 18
#define FILE_COUNT_INCORRECT 1

void reset () {
  printf("\033[0m");
}

void blue_color() {
  printf("\033[0;34m");
}

void red_color() {
  printf("\033[0;31m");
}

void print_str(struct str_struct *p){
    printf("Hodnota stringu: %s, dlzka stringu: %d, Velkost al. miesta: %d\n", p->str, p->length, p->alloc_size);

}

void str_pridanie_znaku(struct str_struct *p, char c){
    bool result = str_add_char(p, c);
    if (result == false){
        printf("Interna chyba pri pridavani znaku\n");
        return;
    }

}

void str_pridanie_stringu(struct str_struct *p, const char *str){
    bool result = str_add_const_str(p, str);
    if (result == false){
        printf("Interna chyba pri pridavani znaku\n");
        return;
    }

}

void kontrola_str(){
    
    struct str_struct *str_test;
    str_test = malloc (sizeof(struct str_struct));
    if ( str_test == NULL){
        printf("Interna chyba pri inicializaci struktury str");
        return;
    }
    bool result_init = str_init(str_test);
    if ( result_init == false){
        printf("Interna chyba pri inicializaci struktury str");
        return;
    }
    blue_color();
    printf("\n[TEST01]\n");
    printf("Pridavanie znaku do stringu - zavolanie funkcie 4x\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_pridanie_znaku(str_test, 'a');
    print_str(str_test);
    str_pridanie_znaku(str_test, 'b');
    print_str(str_test);
    str_pridanie_znaku(str_test, 'c');
    print_str(str_test);
    str_pridanie_znaku(str_test, 'd');
    print_str(str_test);

    blue_color();
    printf("\n[TEST02]\n");
    printf("Vynulovanie stringu\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_clear(str_test);
    print_str(str_test);

    blue_color();
    printf("\n[TEST03]\n");
    printf("Pridanie stringu do stringu - pridanie stringu: \"jebo\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_pridanie_stringu(str_test, "jebo");
    print_str(str_test);

    blue_color();
    printf("\n[TEST04]\n");
    printf("Pridanie stringu do stringu - pridanie stringu: \"boje\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_pridanie_stringu(str_test, "boje");
    print_str(str_test);

    blue_color();
    printf("\n[TEST05]\n");
    printf("Pridanie stringu do stringu - pridanie stringu: \" prvajemedzera\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_pridanie_stringu(str_test, " prvajemedzera");
    print_str(str_test);

    blue_color();
    printf("\n[TEST06]\n");
    printf("Porovnanie stringov - porovnavam \"jebo\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_clear(str_test);
    str_pridanie_stringu(str_test, "jebo");
    print_str(str_test);
    int result = str_cmp_const_str(str_test, "jebo");
    printf("Ak su rovnake ma vratit 0, vysledok porovnavania = \"%d\"\n", result);

    blue_color();
    printf("\n[TEST07]\n");
    printf("Porovnanie stringov - porovnavam \"jeboo\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_clear(str_test);
    str_pridanie_stringu(str_test, "jebo");
    print_str(str_test);
    result = str_cmp_const_str(str_test, "jeboo");
    printf("Ak su rovnake ma vratit 0, vysledok porovnavania = \"%d\"\n", result);

    blue_color();
    printf("\n[TEST08]\n");
    printf("Kopirovanie stringu z jednej str sttruktury do druhej\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    reset();
    str_clear(str_test);
    str_pridanie_stringu(str_test, "jebo");
    printf("Struktrua 1 = ");
    print_str(str_test);
    struct str_struct *str_test2;
    str_test2 = malloc (sizeof(struct str_struct));
    if ( str_test2 == NULL){
        printf("Interna chyba pri inicializaci struktury str");
        return;
    }
    str_init(str_test2);
    if ( result_init == false){
        printf("Interna chyba pri inicializaci struktury str");
        return;
    }
    str_pridanie_stringu(str_test2, "rytmo");
    printf("Struktrua 2 = ");
    print_str(str_test2);
    printf("src bude struktura 1 a dst bude 2\n");
    bool result_copy = str_copy(str_test, str_test2);
    if (result_copy == false){
        printf("Interna chyba kopirovani stringu\n");
        return;
    }
    print_str(str_test2);

    str_free(str_test);
    str_free(str_test2);
    free(str_test);
    free(str_test2);
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

void test_scanner(char *filename){
    //pomocne premmene
    int result;
    int no_token = 0;
    struct token test;
    test.attribute.string = malloc(sizeof(struct str_struct));
    //kontroly
    if (test.attribute.string == NULL){
        printf("Chyba pri alokovani pamate\n");
        return;
    }
    bool init_result = str_init(test.attribute.string);
    //kontroly
    if (init_result == false){
        printf("Chyba pri inicializacii string struktury \n");
        free(test.attribute.string);
        return;
    }

    FILE *f_print;
    //subor vypis
    f_print = fopen(filename, "r"); 
    if (f_print == NULL) { 
        printf("Nepodarilo sa otvorit subor: \"%s\"\n", filename); 
        reset();
        str_free(test.attribute.string);
        free(test.attribute.string);
        fclose(f_print);
        return;
    }
    print_file(f_print);
    printf("Volam funkciu get_token az dokym mi nepride token \"T_TYPE_EOF\"\n");
    fclose(f_print);
    //koniec vypisu suboru
    FILE *f;
    f = fopen(filename, "r"); 
    if (f == NULL) { 
        printf("Nepodarilo sa otvorit subor: \"%s\"\n", filename); 
        reset();
        str_free(test.attribute.string);
        free(test.attribute.string);
        fclose(f);
        return;
    }
    setSourceFile(f);
    result = get_token(&test);
    no_token++;
    //natvrdo nastavim type na T_TYPE_EOL
    test.type = T_TYPE_EOL;
    
    while (test.type != T_TYPE_EOF && result == 0){
        print_token(&test, result, no_token);
        str_free(test.attribute.string);
        free(test.attribute.string);
        no_token++;
        test.attribute.string = malloc(sizeof(struct str_struct));
        //kontroly
        if (test.attribute.string == NULL){
            printf("Chyba pri alokovani pamate\n");
            fclose(f);
            return;
        }
        //kontroly
        bool init_result2 = str_init(test.attribute.string);
        if (init_result2 == false){
            printf("Chyba pri inicializacii string struktury \n");
            str_free(test.attribute.string);
            free(test.attribute.string);
            fclose(f);
            return;
        }
        result = get_token(&test);
        //kontroly
        if (result == ERROR_INTERNAL){
            red_color();
            printf("Interna chyba vo funkcii get_token\n");
            reset();
            str_free(test.attribute.string);
            free(test.attribute.string);
            fclose(f);
            return;
        }
    }
    print_token(&test, result, no_token);
    str_free(test.attribute.string);
    free(test.attribute.string);
    fclose(f);
}

void kontrola_scanner(){
    //TODO -> SUBORY NA KTORE SCANNER VYHODI CHYBU
    int test_count = 9;

    // KONTROLA SUBOROV KTORE BY MALI MAT SPRAVNE ZAPSANE TOKENY
    
    char *filenames[FILE_COUNT_CORRECT] =  
                            {"tests/test_files_correct/token_compar.test",
                             "tests/test_files_correct/token_brackets.test",
                             "tests/test_files_correct/token_comma_semmicolon.test",
                             "tests/test_files_correct/token_double.test",
                             "tests/test_files_correct/token_eof.test",
                             "tests/test_files_correct/token_eol.test",
                             "tests/test_files_correct/token_identifier.test",
                             "tests/test_files_correct/token_integer.test",
                             "tests/test_files_correct/token_keyword.test",
                             "tests/test_files_correct/token_less_equal.test",
                             "tests/test_files_correct/token_more_equal.test",
                             "tests/test_files_correct/token_not_equal.test",
                             "tests/test_files_correct/token_operations.test",
                             "tests/test_files_correct/token_string.test",
                             "tests/test_files_correct/token_comment1.test",
                             "tests/test_files_correct/token_comment2.test",
                             "tests/test_files_correct/token_enumber.test",
                             "tests/test_files_correct/token_escape_sek.test"};

    


    for (int i = 0; i < FILE_COUNT_CORRECT; i++){
        blue_color();
        printf("\n[TEST%d]\n", test_count);
        printf("Testovanie scanneru na subore: \"%s\"\n",filenames[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        reset();

        test_scanner(filenames[i]);
        test_count++;
    }

    // KONTROLA SUBOROV NA KTORYCH BY SA MAL SCANNER ZASAKNUT

    char *filenames_incorrect[FILE_COUNT_INCORRECT] =  
                            {"tests/test_files_incorrect/token_escape_sek.test",};

    for (int i = 0; i < FILE_COUNT_INCORRECT; i++){
        blue_color();
        printf("\n[TEST%d]\n", test_count);
        printf("Testovanie scanneru na subore: \"%s\"\n",filenames_incorrect[i]);
        printf("~~~~~~~~~~~~~~~~~~~~\n");
        reset();

        test_scanner(filenames_incorrect[i]);
        test_count++;
    }

    
}


int main(){
    printf("Testy pre lexikalnu analyzu\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\nTesty funkcii v str.c \n");
    printf("==================================\n");

    kontrola_str();
    printf("\nKoniec testov funkcii v str.c \n");
    printf("==================================\n");
    printf("==================================\n");

    printf("\nTesty funkcii v scanner.c \n");
    printf("==================================\n");

    kontrola_scanner();

    printf("\nKoniec testov funkcii v scanner.c \n");
    printf("==================================\n");
    printf("==================================\n");

    return 0;
}