//str.h
//hlavickovy subor pre pracu so stringami


#ifndef _STR_H
#define _STR_H


#include <stdbool.h>

//struktura reprezentujuca 
struct str_struct
{
	char *str; // string 
	unsigned int length; //dlzka stringu
	unsigned int alloc_size; //pocet znakov allokovanych pre string
};


//inicializacia stringu
bool dynamic_string_init(struct str_struct *s);

//uvolnenie pamate 
void dynamic_string_free(struct str_struct *s);

//vynulovanie stringu, dlzka = 0
void dynamic_string_clear(struct str_struct *s);

//pridanie charu  nakoniec stringu
bool dynamic_string_add_char(struct str_struct *s, char c);

//pridanie stringu  nakoniec stringu
bool dynamic_string_add_const_str(struct str_struct, const char *const_string);

//porovnanie stringov
int dynamic_string_cmp_const_str(struct str_struct *s, const char *const_string);

//kopirovanie stringu z src do dst
bool dynamic_string_copy(struct str_struct *src, struct str_struct *dst);


#endif 