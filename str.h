/*
//IFJ20 - projekt(varianta I.)
//hlavickovy subor pre pracu so stringami
//Matej Hornik, xhorni20
*/


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
bool str_init(struct str_struct *s);

//uvolnenie pamate 
void str_free(struct str_struct *s);

//vynulovanie stringu, dlzka = 0
void str_clear(struct str_struct *s);

//pridanie charu  nakoniec stringu
bool str_add_char(struct str_struct *s, char c);

//pridanie stringu  nakoniec stringu
bool str_add_const_str(struct str_struct *s, const char *const_string);

//porovnanie stringov
int str_cmp_const_str(struct str_struct *s, const char *const_string);

//kopirovanie stringu z src do dst
bool str_copy(struct str_struct *src, struct str_struct *dst);


#endif 