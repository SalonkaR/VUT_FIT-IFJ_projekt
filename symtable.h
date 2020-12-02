/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu s binarnym strom
//Matej Hornik, xhorni20
*/

#include <stdbool.h>
#include <string.h>

#include "id_queue.h"
#include "str.h"

#define MODULO_NUM 12007 //musi byt prvocislo, pocet unikatnych slov v harry potter knihach * 2

#ifndef _SYMTABLE_
#define _SYMTABLE_


typedef char * hKey_t; 


// TIETO DATA ESTE MOZU BYT INAK
enum data_types
{
	TYPE_UNDEFINED,	// Data type undefined	//cislo=0
	TYPE_INT,		// Integer data type	//cislo=1
	TYPE_DOUBLE,	// Double data type		//cislo=2
	TYPE_STRING,	// String data type		//cislo=3
	TYPE_BOOL		// Bool data type 		//cislo=4 (kvoli vyrazom v ife alebo fore)
    //este mozno funkcia tu nvm
};

// TIETO DATA ESTE MOZU BYT INAK
typedef struct data
{
    enum data_types type;
	tID_queue func_params;
    hKey_t identifier;	 
	bool defined;
    struct data *nextptr;
} Data_t;

typedef struct Node {
	size_t hashKey;			                             //klíč, hash z identifikatoru
	Data_t *data;                                        /* užitečný obsah uzlu */
	struct Node * LPtr;                                  /* levý podstrom */
	struct Node * RPtr;                                  /* pravý podstrom */
}tNode;

//v parseri bude globalna premena tejto strukturu ktora sa bude pouzivat
typedef struct BT {
    bool definded;
    tNode *root_ptr;
}tBT;


//(http://www.cse.yorku.ca/~oz/hash.html - varianta sdbm):
size_t BT_hash_fun ( hKey_t key );
//TODO -> komentare

//vracia "true" ak sa podarila inicializacia
//vracia "false" ak sa nepodarilo
bool BT_init   (tBT *bt_ptr);

//vracia NULL ak nebolo najdene key a boolovska hodnota false
//vracia ukazatel na strukturu Data_t ak boli najdene data s klucom key
//vracia interal_error=true ak nastala interna chyba
Data_t *BT_search  (tBT *bt_ptr, hKey_t key, bool *internal_error);

//vracia NULL ak key uz je v binarnom strome a boolovska hodnota false
//vracia ukazatel na strukturu Data_t ak sa podaril insert a boolovska hodnota false
//vracia interal_error=true ak nastala interna chyba
Data_t *BT_insert (tBT *bt_ptr, hKey_t key, bool *internal_error);

//vracia true ak vymazalo prvok z binarneho stromu
//vracia false a ak internal error true tak nastala chyba pri mazani
//vracia false a ak internal error false tak dany prvok neni v binarnom strome
bool BT_delete (tBT *bt_ptr, hKey_t key, bool *internal_error);

//vymaze tabulku
//vracia true ak sa podarilo vymazat strom inac false
//po pouziti strom bude nedefinovany
bool BT_dispose (tBT *bt_ptr);


#endif