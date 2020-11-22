//testy pre binarny strom
//Matej Hornik, xhorni20

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../symtable.h"
#include "../scanner.h"
#include "../str.h"
#include "../error.h"

//globalna premena pre binarny strom
tBT binary_tree;
tNode *TempTree;


/* vykresli sktrukturu binarniho stromu */
void Print_tree2(tNode *TempTree, char* sufix, char fromdir){   
     if (TempTree != NULL)
     {
        char* suf2 = (char*) malloc(strlen(sufix) + 4);
        strcpy(suf2, sufix);
        if (fromdir == 'L')
	    {
            suf2 = strcat(suf2, "  |");
            printf("%s\n", suf2);
	    }
	    else
	        suf2 = strcat(suf2, "   ");
	    Print_tree2(TempTree->RPtr, suf2, 'R');
        //LL -> vypisovanie linked listu
        //printf("%s  +-[%c,%d]\n", sufix,  TempTree->hashKey, 9);
        printf("%s  +-[%lu; ", sufix, TempTree->hashKey);
        Data_t *tmp = TempTree->data;
        while (tmp != NULL){
            printf("'%s',", tmp->identifier);
            tmp = tmp->nextptr;
        }
        printf("]\n");


	    strcpy(suf2, sufix);
        if (fromdir == 'R')
	        suf2 = strcat(suf2, "  |");	
	    else
	        suf2 = strcat(suf2, "   ");
	    Print_tree2(TempTree->LPtr, suf2, 'L');
	    if (fromdir == 'R') printf("%s\n", suf2);
	    free(suf2);
    }
}

void Print_tree(tNode *TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
} 

void test_BS_init(tBT *binary_tree){
    if (binary_tree == NULL){
        printf("Neplatny ukazatel na binarny strom\n");
        exit(1);

    }
    bool result = BT_init(binary_tree);
    if (result == false){
        printf("Nepodarila sa inicializacia binarneho stromu\n");
        exit(1);
    }
    Print_tree(binary_tree->root_ptr);

}

void test_BSTSearch(tBT *binary_tree, hKey_t key){
    if (binary_tree == NULL){
        printf("Neplatny ukazatel na binarny strom\n");
        exit(1);

    }

    size_t hask_k = BT_hash_fun(key) % MODULO_NUM;

    bool internal_error;
    Data_t *searched = BT_search(binary_tree, key, &internal_error);
    if (internal_error == true){
        printf("Nastala interna chyba pri hladani '%lu'\n", hask_k);
        exit(1);
    }
    if (searched == NULL && internal_error == false){
        printf("Polozka (key: '%s' hash:'%lu') nebola najdena !\n", key, hask_k);
        return;
    }
    printf("Polozka (key: '%s' hash:'%lu') bola najdena !\n", key, hask_k);
    printf("Polozka obsahuje data_type:'%d', id:'%s', defined: '%d', global:'%d'\n", 
            searched->type, searched->identifier, searched->defined, searched->global);
    Print_tree(binary_tree->root_ptr);
}

void test_BSTInsert(tBT *binary_tree, hKey_t key){
    if (binary_tree == NULL){
        printf("Neplatny ukazatel na binarny strom\n");
        exit(1);

    }

    size_t hask_k = BT_hash_fun(key) % MODULO_NUM;

    bool internal_error;
    Data_t *searched = BT_insert(binary_tree, key, &internal_error);
    if (internal_error == true){
        printf("Nastala interna chyba pri vkladani prvku (key: '%s' hash:'%lu')\n", key, hask_k);
        exit(1);
    }
    if (searched == NULL && internal_error == false){
        printf("Polozka (key: '%s' hash:'%lu') sa uz nachadza v binarnom strome !\n", key, hask_k);
        return;
    }
    printf("Polozka (key: '%s' hash:'%lu') bola uspesne pridana!\n", key, hask_k);

    Print_tree(binary_tree->root_ptr);
}


void test_BSTDelete(tBT *binary_tree, hKey_t key){
    if (binary_tree == NULL){
        printf("Neplatny ukazatel na binarny strom\n");
        exit(1);

    }

    size_t hask_k = BT_hash_fun(key) % MODULO_NUM;

    bool internal_error;
    bool result = BT_delete(binary_tree, key, &internal_error);
    if (internal_error == true){
        printf("Nastala interna chyba pri vkladani prvku (key: '%s' hash:'%lu')\n", key, hask_k);
        exit(1);
    }
    if (result == false && internal_error == false){
        printf("Polozka (key: '%s' hash:'%lu') sa nenachadza v binarnom strome!\n", key, hask_k);
        return;
    }
    printf("Polozka (key: '%s' hash:'%lu') bola uspesne zmazana!\n", key, hask_k);

    Print_tree(binary_tree->root_ptr);
}

void test_BSTDispose(tBT *binary_tree){
    if (binary_tree == NULL){
        printf("Neplatny ukazatel na binarny strom\n");
        exit(1);
    }
    bool result = BT_dispose(binary_tree);

    if (result == false){
        printf("Nastala interna chyba pri vymazavani binarenho stromu\n");
        exit(1);
    }
    printf("Uspesne vymazani binarny strom !\n");

    Print_tree(binary_tree->root_ptr);
}

int main (){
    

    printf("Binarni vyhledavaci strom\n");
	printf("=========================\n");
	
	printf("[TEST01]\n");
	printf("Test inicializace....\n");
	test_BS_init(&binary_tree);


    hKey_t id_kluc1 = "restt";
    printf("[TEST02]\n");
	printf("Pokusime se vyhledat polozku (key:%s, hash:%lu) -- nenalezne se\n", id_kluc1, BT_hash_fun(id_kluc1) % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTSearch(&binary_tree, id_kluc1);


    hKey_t id_kluc2 = "kolesa";
    printf("[TEST03]\n");
	printf("Vlozime prvni prvek (key:%s, hash:%lu)\n", id_kluc2, BT_hash_fun(id_kluc2) % MODULO_NUM );
	test_BSTInsert(&binary_tree, id_kluc2);


    printf("[TEST04]\n");
	printf("Pokusime se vyhledat polozku (key:%s, hash:%lu)\n", id_kluc2, BT_hash_fun(id_kluc2) % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTSearch(&binary_tree, id_kluc2);


    hKey_t id_kluc3 = "piko";
    printf("[TEST05]\n");
	printf("Vlozime prvek (key:%s, hash:%lu)\n", id_kluc3, BT_hash_fun(id_kluc3) % MODULO_NUM );
	test_BSTInsert(&binary_tree, id_kluc3);


    hKey_t id_kluc4 = "RYTMO";
    printf("[TEST06]\n");
	printf("Vlozime prvek (key:%s, hash:%lu)\n", id_kluc4, BT_hash_fun(id_kluc4) % MODULO_NUM );
	test_BSTInsert(&binary_tree, id_kluc4);


    hKey_t id_kluc5 = "pomocna_premenaaaaa10";
    printf("[TEST07]\n");
	printf("Vlozime prvek (key:%s, hash:%lu)\n", id_kluc5, BT_hash_fun(id_kluc5) % MODULO_NUM );
	test_BSTInsert(&binary_tree, id_kluc5);


    printf("[TEST08]\n");
	printf("Pokusime se vyhledat polozku (key:%s, hash:%lu)\n", id_kluc3, BT_hash_fun(id_kluc3) % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTSearch(&binary_tree, id_kluc3);


    printf("[TEST09]\n");
	printf("Pokusime se vyhledat polozku (key:%s, hash:%lu)\n", id_kluc5, BT_hash_fun(id_kluc5) % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTSearch(&binary_tree, id_kluc5);


    printf("[TEST010]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "piko", BT_hash_fun("piko") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "piko");


    printf("[TEST11]\n");
	printf("Vymazem cely strom\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDispose(&binary_tree);



    printf("[TEST12]\n");
	printf("Vlozime dostatocne mnozstvo prvkov do stromu\n");
    test_BS_init(&binary_tree);

    test_BSTInsert(&binary_tree, "q");
	test_BSTInsert(&binary_tree, "w");
    test_BSTInsert(&binary_tree, "e");
    test_BSTInsert(&binary_tree, "r");
    test_BSTInsert(&binary_tree, "t");
    test_BSTInsert(&binary_tree, "y");
    test_BSTInsert(&binary_tree, "u");
    test_BSTInsert(&binary_tree, "i");
    test_BSTInsert(&binary_tree, "o");
    test_BSTInsert(&binary_tree, "p");
    test_BSTInsert(&binary_tree, "a");
    test_BSTInsert(&binary_tree, "s");
    test_BSTInsert(&binary_tree, "d");
    test_BSTInsert(&binary_tree, "f");
    test_BSTInsert(&binary_tree, "g");
    test_BSTInsert(&binary_tree, "h");
    test_BSTInsert(&binary_tree, "j");
    test_BSTInsert(&binary_tree, "k");
    test_BSTInsert(&binary_tree, "l");
    test_BSTInsert(&binary_tree, "z");
    test_BSTInsert(&binary_tree, "x");
    test_BSTInsert(&binary_tree, "c");
    test_BSTInsert(&binary_tree, "v");
    test_BSTInsert(&binary_tree, "b");
    test_BSTInsert(&binary_tree, "n");
    test_BSTInsert(&binary_tree, "m");
    test_BSTInsert(&binary_tree, "aa");
    test_BSTInsert(&binary_tree, "aaaaa");
    test_BSTInsert(&binary_tree, "aaaaaa");
    test_BSTInsert(&binary_tree, "aaaaaaa");


    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "y", BT_hash_fun("y") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "y");


    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "f", BT_hash_fun("f") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "f");


    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "g", BT_hash_fun("g") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "g");


    //tusa to jebe lebo mazem strom z dvoma listami
    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "w", BT_hash_fun("w") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "w");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "o", BT_hash_fun("o") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "o");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "e", BT_hash_fun("e") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "e");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "a", BT_hash_fun("a") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "a");


    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "t", BT_hash_fun("t") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "t");


    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "z", BT_hash_fun("z") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "z");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "x", BT_hash_fun("x") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "x");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "q", BT_hash_fun("q") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "q");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "n", BT_hash_fun("n") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "n");


    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "l", BT_hash_fun("l") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "l");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "k", BT_hash_fun("k") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "k");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "j", BT_hash_fun("j") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "j");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "m", BT_hash_fun("m") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "m");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "p", BT_hash_fun("p") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "p");

        printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "aaaaa", BT_hash_fun("aaaaa") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "aaaaa");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "v", BT_hash_fun("v") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "v");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "r", BT_hash_fun("r") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "r");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "s", BT_hash_fun("s") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "s");


    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "u", BT_hash_fun("u") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "u");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "i", BT_hash_fun("i") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "i");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "h", BT_hash_fun("h") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "h");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "d", BT_hash_fun("d") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "d");

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "c", BT_hash_fun("c") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "c");



    printf("[TEST13]\n");
	printf("Vymazem cely strom\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDispose(&binary_tree);

    printf("[TEST12.1]\n");
	printf("Zrusime polozku (key:%s, hash:%lu)\n", "c", BT_hash_fun("c") % MODULO_NUM );
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	test_BSTDelete(&binary_tree, "c");



    return 0;
}