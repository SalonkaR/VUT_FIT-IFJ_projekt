//testovacie skripty pre lexikalny analyzator 

#include <stdio.h>
#include <stdlib.h>
#include "../error.h"
#include "../scanner.h"
#include "../str.h"

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
    printf("\n[TEST01]\n");
    printf("Pridavanie znaku do stringu - zavolanie funkcie 4x\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    str_pridanie_znaku(str_test, 'a');
    print_str(str_test);
    str_pridanie_znaku(str_test, 'b');
    print_str(str_test);
    str_pridanie_znaku(str_test, 'c');
    print_str(str_test);
    str_pridanie_znaku(str_test, 'd');
    print_str(str_test);

    printf("\n[TEST02]\n");
    printf("Vynulovanie stringu\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    str_clear(str_test);
    print_str(str_test);

    printf("\n[TEST03]\n");
    printf("Pridanie stringu do stringu - pridanie stringu: \"jebo\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    str_pridanie_stringu(str_test, "jebo");
    print_str(str_test);

    printf("\n[TEST04]\n");
    printf("Pridanie stringu do stringu - pridanie stringu: \"boje\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    str_pridanie_stringu(str_test, "boje");
    print_str(str_test);

    printf("\n[TEST05]\n");
    printf("Pridanie stringu do stringu - pridanie stringu: \" prvajemedzera\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    str_pridanie_stringu(str_test, " prvajemedzera");
    print_str(str_test);

    printf("\n[TEST06]\n");
    printf("Porovnanie stringov - porovnavam \"jebo\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    str_clear(str_test);
    str_pridanie_stringu(str_test, "jebo");
    print_str(str_test);
    int result = str_cmp_const_str(str_test, "jebo");
    printf("Ak su rovnake ma vratit 0, vysledok porovnavania = \"%d\"\n", result);


    printf("\n[TEST07]\n");
    printf("Porovnanie stringov - porovnavam \"jeboo\"\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
    str_clear(str_test);
    str_pridanie_stringu(str_test, "jebo");
    print_str(str_test);
    result = str_cmp_const_str(str_test, "jeboo");
    printf("Ak su rovnake ma vratit 0, vysledok porovnavania = \"%d\"\n", result);


    printf("\n[TEST08]\n");
    printf("Kopirovanie stringu z jednej str sttruktury do druhej\n");
    printf("~~~~~~~~~~~~~~~~~~~~\n");
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


int main(){
    printf("Testy pre lexikalnu analyzu\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("\nTesty funkcii v str.c \n");
    printf("==================================\n");

    kontrola_str();
    printf("\nKoniec testov funkcii v str.c \n");
    printf("==================================\n");

    printf("\nTesty funkcii v scanner.c \n");

    return 0;
}