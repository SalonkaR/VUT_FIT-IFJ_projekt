/*//IFJ20 - projekt(varianta I.)
//symtable.c
//Matej Hornik, xhorni20*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "symtable.h"
#include "str.h"
#include "id_queue.h"

//definicie funkcii pre symtable
static Data_t *search_rec(tNode *root_ptr, size_t hash_k, hKey_t key);
static Data_t *insert_rec (tBT *bt_ptr, tNode *parent_ptr, tNode* root_ptr,size_t hash_k, hKey_t key, bool *internal_error );
static void delete_rec(tBT *bt_ptr, tNode *parent_ptr, tNode *RootPtr, size_t hash_k, hKey_t key);
static void ReplaceByRightmost (tBT *bt_ptr,tNode *parent_ptr, tNode *PtrReplaced, tNode *RootPtr, size_t hash_k);
static void dispose_rec(tNode *root_ptr);

int counter = 1;


size_t BT_hash_fun(hKey_t str){
    //djb2
    uint32_t  h = 5381;
    int c;
    while (c = *str++)
        h = ((h << 5) + h) + c; /* hash * 33 + c */

    return h;
}

bool BT_init   (tBT *bt_ptr){
    if (bt_ptr->definded == true){
        fprintf(stderr, "Binarny strom je uz inicializovany(symtable.c/BT_init)\n");
        return false;
    }
    bt_ptr->definded = true;
    bt_ptr->root_ptr = NULL;
    return true;
}


//treba pouzit pomocny bool premennu do ktorej sa ulozi ci nastala interna chyba alebo nie

Data_t *BT_search  (tBT *bt_ptr, hKey_t key, bool *internal_error){
    //skontrolujem ci je definovany binarny strom
    if (bt_ptr->definded != true){
        fprintf(stderr, "V nedefinovanom strome sa neda hladat(symtable.c/iBT_search)\n");
        *(internal_error) = true;
        return NULL;
    }

    size_t key_hash = BT_hash_fun(key) % MODULO_NUM;
    
    //zavolam rekurzivnu funckiu nad binarnym stromom
    Data_t *tmp = search_rec(bt_ptr->root_ptr, key_hash, key);
    if (tmp == NULL){
        //printf("Data neboli najdene\n");
        *(internal_error) = false;
        return NULL;
    }
    *(internal_error) = false;
    return tmp;
}


static Data_t *search_rec(tNode *root_ptr, size_t hash_k, hKey_t key){
	//ak bude ukaztel na koren NULL, vratime FALSE
	if (root_ptr == NULL){
		return NULL;
	}
	else {
		//ak sme nasli uzel s klicem tak vratime TRUE
		if (root_ptr->hashKey == hash_k){
			//TODO *Content = RootPtr->BSTNodeCont
            //hladam v zretazenom zozname
            Data_t *tmp = NULL;
            Data_t *ptr_data = root_ptr->data;
            while (ptr_data != NULL){
                if (strcmp(key, ptr_data->identifier) == 0){
                    tmp = ptr_data;
                    break;
                }
                ptr_data = root_ptr->data->nextptr;
            }

			return tmp;
		}
		//hladame hlbsie v strome 
		else{
			//hladany kluc je mensi nez hladany tak volam search funkciou nad lavou castou stromu
			if (root_ptr->hashKey > hash_k){
				return search_rec(root_ptr->LPtr,hash_k, key);
			}
			//hladame nad pravou stranou stromu
			else{
				return search_rec(root_ptr->RPtr, hash_k, key);
			}
		}
	}
}

Data_t *BT_insert (tBT *bt_ptr, hKey_t key, bool *internal_error){
    //skontrolujem ci je definovany binarny strom
    if (bt_ptr->definded != true){
        fprintf(stderr, "V nedefinovanom strome sa neda hladat(symtable.c/BT_insert)\n");
        *(internal_error) = true;
        return NULL;
    }

    size_t key_hash = BT_hash_fun(key) % MODULO_NUM;
    Data_t *found = search_rec(bt_ptr->root_ptr, key_hash, key);
    //ak uz dany kluc je v binarnom strome
    if (found != NULL){
        *(internal_error) = false;
        return NULL;
    }
    Data_t *new_insert = insert_rec(bt_ptr, NULL, bt_ptr->root_ptr, key_hash, key, internal_error);
    
    return new_insert;
}

static Data_t *insert_rec (tBT *bt_ptr, tNode *parent_ptr, tNode* root_ptr,size_t hash_k, hKey_t key, bool *internal_error ){
	if (root_ptr == NULL){
		//vlozime polozku

        //alokovanie novej nody
		tNode *newPtr = malloc(sizeof(struct Node));
		if (newPtr == NULL){
            fprintf(stderr, "Neuspech mallocu v insert_rec, allocovanie listu(symtable.c/insert_rec)\n");
            *(internal_error) = true;
			return NULL;
		}
        //alokovanie dat
        Data_t *newPtr_data = malloc(sizeof(struct data));
		if (newPtr_data == NULL){
            free(newPtr);
            fprintf(stderr, "Neuspech mallocu v insert_rec, allocovanie dat(symtable.c/insert_rec)\n");
            *(internal_error) = true;
			return NULL;
		}

        //allokovanie identifikatoru
        newPtr_data->identifier = (char *)malloc((strlen(key) + 1) * sizeof(char));
        if (newPtr_data->identifier == NULL){
            free(newPtr_data);
            free(newPtr);
            fprintf(stderr, "Neuspech mallocu v insert_rec, allocovanie identifikatoru(symtable.c/insert_rec)\n");
            *(internal_error) = true;
			return NULL;
        }
        //nakopirovanie indentifikatoru do dat
        id_queue_init(&newPtr_data->func_params);
        newPtr_data->no_ret_values =0;
        strcpy(newPtr_data->identifier, key);
        //data
        newPtr_data->nextptr = NULL;
        newPtr_data->defined = false;
        newPtr_data->type = TYPE_UNDEFINED;
        //node
        newPtr->hashKey = hash_k;
        newPtr->data = newPtr_data;
		newPtr->LPtr = newPtr->RPtr = NULL;

        
		root_ptr = newPtr;
        root_ptr->LPtr = NULL;
        root_ptr->RPtr = NULL;

        *(internal_error) = false;
        // ak to je uplne prvy prvok tak ho musim priradit do struktury
        if (bt_ptr->root_ptr == NULL){
            bt_ptr->root_ptr = root_ptr;
        }
        else{
            if (hash_k < (parent_ptr->hashKey)){
                parent_ptr->LPtr = newPtr;
            }
            else if (hash_k > (parent_ptr->hashKey)){
                parent_ptr->RPtr = newPtr;
            }
        }

		return root_ptr->data;
	}
	else{//ideme do laveho podstromu
		if (hash_k < (root_ptr->hashKey)){
            return insert_rec(bt_ptr, root_ptr, root_ptr->LPtr, hash_k, key, internal_error);
		}
		else{//ideme do praveho podstromu
			if (hash_k > (root_ptr->hashKey)){
				return insert_rec(bt_ptr, root_ptr, root_ptr->RPtr, hash_k, key, internal_error);
			}//rovnaky hash ale iny indentifikator
			else{
                //alokovanie dat
                Data_t *newPtr_data = malloc(sizeof(struct data));
                if (newPtr_data == NULL){
                    fprintf(stderr, "Neuspech mallocu v insert_rec, allocovanie dat(symtable.c/insert_rec)\n");
                    *(internal_error) = true;
                    return NULL;
                }

                //allokovanie identifikatoru
                newPtr_data->identifier = (char *)malloc((strlen(key) + 1) * sizeof(char));
                if (newPtr_data->identifier == NULL){
                    free(newPtr_data);
                    fprintf(stderr, "Neuspech mallocu v insert_rec, allocovanie identifikatoru(symtable.c/insert_rec)\n");
                    *(internal_error) = true;
                    return NULL;
                }
                //nakopirovanie indentifikatoru do dat
                strcpy(newPtr_data->identifier, key);

                //data
                Data_t *tmp = root_ptr->data->nextptr;
                id_queue_init(&newPtr_data->func_params);
                newPtr_data->no_ret_values =0;
                newPtr_data->nextptr = tmp;
                newPtr_data->defined = false;
                newPtr_data->type = TYPE_UNDEFINED;

                root_ptr->data = newPtr_data;

                *(internal_error) = false;
                return root_ptr->data;

			}
		}
	}
}

//zmaze prvok z binarneho stromu
bool BT_delete (tBT *bt_ptr, hKey_t key, bool *internal_error){
    //skontrolujem ci je definovany binarny strom
    if (bt_ptr->definded != true){
        fprintf(stderr, "V nedefinovanom strome sa neda hladat(symtable.c/BT_delete)\n");
        *(internal_error) = true;
        return false;
    }

    size_t key_hash = BT_hash_fun(key) % MODULO_NUM;
    Data_t *found = search_rec(bt_ptr->root_ptr, key_hash, key);
    //ak dany kluc nieje v binarnom strome
    if (found == NULL){
        *(internal_error) = false;
        return false;
    }

    delete_rec(bt_ptr, NULL, bt_ptr->root_ptr, key_hash, key);
    *(internal_error) = false;
    
    return true;
}

//pomocna funkca pre delete
static void ReplaceByRightmost (tBT *bt_ptr, tNode *parent_ptr, tNode *PtrReplaced, tNode *RootPtr, size_t hash_k) {
	if (RootPtr->RPtr == NULL){
        //vymazem data
        Data_t *dataToDelete =  PtrReplaced->data;
        id_queue_free(&dataToDelete->func_params);
        free(dataToDelete->identifier);
        free(dataToDelete);
        PtrReplaced->data = NULL;
		PtrReplaced->data = RootPtr->data;
        PtrReplaced->hashKey = RootPtr->hashKey;


        //uvolnenie uzlu
        tNode *itemToDelete = RootPtr;
        RootPtr = RootPtr->LPtr;
        //parenta musim nastavit 
        if (counter == 1){
            PtrReplaced->LPtr = PtrReplaced->LPtr->LPtr;

        }
        else{
            parent_ptr->RPtr = parent_ptr->RPtr->LPtr;

            counter = 1;
        }

        //printf("ADRESSA RODICA:%p, ADRESA REPLACED: %p\n", (void *) &parent_ptr, (void *) &PtrReplaced);
        free(itemToDelete);
	}
	else {
        counter++;
		ReplaceByRightmost(bt_ptr, RootPtr, PtrReplaced, RootPtr->RPtr, hash_k);
	}

	return;
}

static void delete_rec(tBT *bt_ptr, tNode *parent_ptr, tNode *RootPtr, size_t hash_k, hKey_t key){
	//osetrenie NULLovych ukazatelov
	if ( RootPtr != NULL) {
        if ( hash_k < RootPtr->hashKey ) {
            delete_rec(bt_ptr, RootPtr, RootPtr->LPtr, hash_k, key);
        }
        else if ( hash_k > RootPtr->hashKey) {
            delete_rec(bt_ptr, RootPtr, RootPtr->RPtr, hash_k, key);
        }
        else { //ak bol najdeny uzol s danym klucom
			//ak sa jedna o listovy uzol
            if ( (RootPtr->LPtr == NULL) && (RootPtr->RPtr == NULL) ) {
                //zistim kolko poloziek je v linked liste
                int count = 0;
                Data_t *tmp = RootPtr->data;
                while (tmp != NULL){
                    count ++;
                    tmp = tmp->nextptr;
                }
                //ked v linked liste sa nachadza len 1 prvok
                if (count ==1){
                    id_queue_free(&RootPtr->data->func_params);
                    free(RootPtr->data->identifier);
                    free(RootPtr->data);
                    free(RootPtr);
                    RootPtr = NULL;
                    if (parent_ptr == NULL){
                        bt_ptr->root_ptr = NULL;
                    }
                    else{
                        if (hash_k < (parent_ptr->hashKey)){
                            parent_ptr->LPtr = NULL;
                        }
                        else if (hash_k > (parent_ptr->hashKey)){
                            parent_ptr->RPtr = NULL;
                        }
                    }
                    return;
                }
                //v linked liste sa nachadza viac prvkov
                else if (count > 1){
                    //najdem si prvok ktory zmazat
                    Data_t *found = RootPtr->data;
                    while (found != NULL){
                        if (strcmp(found->nextptr->identifier, key) == 0){
                            Data_t *tmp_next = found->nextptr->nextptr;
                            id_queue_free(&found->nextptr->func_params);
                            free(found->nextptr->identifier);
                            free(found->nextptr);

                            found->nextptr = tmp_next;
                            return;
                        }
                        found = found->nextptr;
                    }
                }
            }
			//ak ma uzol len lavy podstrom
            else if ( (RootPtr->LPtr != NULL) && (RootPtr->RPtr == NULL) ) {
                //zistim kolko poloziek je v linked liste
                int count = 0;
                Data_t *tmp = RootPtr->data;
                while (tmp != NULL){
                    count ++;
                    tmp = tmp->nextptr;
                }
                //ked v linked liste sa nachadza len 1 prvok
                if (count == 1){
                    tNode *itemToDelete = RootPtr;

                    if (parent_ptr == NULL){
                        bt_ptr->root_ptr = bt_ptr->root_ptr->LPtr;
                    }
                    else{
                        if (hash_k < (parent_ptr->hashKey)){
                            parent_ptr->LPtr = RootPtr->LPtr;
                        }
                        else if (hash_k > (parent_ptr->hashKey)){
                            parent_ptr->RPtr = RootPtr->LPtr;
                        }
                    }
                    id_queue_free(&RootPtr->data->func_params);
                    free(RootPtr->data->identifier);
                    free(RootPtr->data);

                    RootPtr = RootPtr->LPtr;
                    free(itemToDelete);
                    return;
                }
                //v linked liste sa nachadza viac prvkov
                else if (count > 1){
                    //najdem si prvok ktory zmazat
                    Data_t *found = RootPtr->data;
                    while (found != NULL){
                        if (strcmp(found->nextptr->identifier, key) == 0){
                            Data_t *tmp_next = found->nextptr->nextptr;
                            id_queue_free(&found->nextptr->func_params);
                            free(found->nextptr->identifier);
                            free(found->nextptr);

                            found->nextptr = tmp_next;
                            return;
                        }
                        found = found->nextptr;
                    }
                }
            }
			//ak ma uzol len pravy podstrom
            else if ( (RootPtr->RPtr != NULL) && (RootPtr->LPtr == NULL) ) {
                //zistim kolko poloziek je v linked liste
                int count = 0;
                Data_t *tmp = RootPtr->data;
                while (tmp != NULL){
                    count ++;
                    tmp = tmp->nextptr;
                }
                //ked v linked liste sa nachadza len 1 prvok
                if (count == 1){
                    tNode *itemToDelete = RootPtr;

                    if (parent_ptr == NULL){
                        bt_ptr->root_ptr = bt_ptr->root_ptr->RPtr;
                    }
                    else{
                        //dead code ?!
                        if (hash_k < (parent_ptr->hashKey)){
                            parent_ptr->LPtr = RootPtr->RPtr;
                        }
                        else if (hash_k > (parent_ptr->hashKey)){
                            parent_ptr->RPtr = RootPtr->RPtr;
                        }
                    }
                    id_queue_free(&RootPtr->data->func_params);
                    free(RootPtr->data->identifier);
                    free(RootPtr->data);

                    RootPtr = RootPtr->RPtr;
                    free(itemToDelete);
                    return;
                }
                //v linked liste sa nachadza viac prvkov
                else if (count > 1){
                    //najdem si prvok ktory zmazat
                    Data_t *found = RootPtr->data;
                    while (found != NULL){
                        if (strcmp(found->nextptr->identifier, key) == 0){
                            Data_t *tmp_next = found->nextptr->nextptr;
                            id_queue_free(&found->nextptr->func_params);
                            free(found->nextptr->identifier);
                            free(found->nextptr);

                            found->nextptr = tmp_next;
                            return;
                        }
                        found = found->nextptr;
                    }
                }
            }
			// ak ma uzol oba podstromy
            else {
                //zistim kolko poloziek je v linked liste
                int count = 0;
                Data_t *tmp = RootPtr->data;
                while (tmp != NULL){
                    count ++;
                    tmp = tmp->nextptr;
                }
                //ked v linked liste sa nachadza len 1 prvok
                if (count == 1){
                    ReplaceByRightmost(bt_ptr, RootPtr->LPtr, RootPtr, RootPtr->LPtr, hash_k);
                }
                //v linked liste sa nachadza viac prvkov
                else if (count > 1){
                    //najdem si prvok ktory zmazat
                    Data_t *found = RootPtr->data;
                    while (found != NULL){
                        if (strcmp(found->nextptr->identifier, key) == 0){
                            Data_t *tmp_next = found->nextptr->nextptr;
                            id_queue_free(&found->nextptr->func_params);
                            free(found->nextptr->identifier);
                            free(found->nextptr);

                            found->nextptr = tmp_next;
                            return;
                        }
                        found = found->nextptr;
                    }
                }
            }
        }
    }
	return;
}


bool BT_dispose (tBT *bt_ptr){
    //skontrolujem ci je definovany binarny strom
    if (bt_ptr->definded != true){
        fprintf(stderr, "V nedefinovanom strome sa neda hladat(symtable.c/BT_dispose)\n");
        return false;
    }

    dispose_rec(bt_ptr->root_ptr);
    bt_ptr->root_ptr = NULL;
    bt_ptr->definded = false;
    
    return true;
}

static void dispose_rec(tNode *root_ptr){

    //podmienka na ukoncenie rekurzie
	if (root_ptr == NULL){
		return;
	}
	dispose_rec(root_ptr->LPtr);
	dispose_rec(root_ptr->RPtr);

	//uvolnenie pamate

    //UVOLNENIE LINKED LISTU
    //pomocne premenne aby som nestratil ukazetele
	Data_t *Element = root_ptr->data;
    Data_t *tmp;

    while  (Element != NULL){
        tmp = Element;
        Element = Element->nextptr;
        //UVOLNENIE 
        id_queue_free(&tmp->func_params);
        free(tmp->identifier);
        free(tmp);
    }
    root_ptr->data = NULL;


	free(root_ptr);
	root_ptr = NULL;
	return;
}
