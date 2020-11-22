//str.c


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "str.h"


#define STRING_LENGHT 20 //pociatocna dlzka retazca


void str_clear(struct str_struct *s){
	s->length = 0;
	s->str[s->length] = '\0';
}


bool str_init(struct str_struct *s){
	s->str = malloc(STRING_LENGHT);
    if (s->str == NULL){
		return false;
	}

	str_clear(s);
	s->alloc_size = STRING_LENGHT;

	return true;
}


void str_free(struct str_struct *s){
	free(s->str);
}


bool str_add_char(struct str_struct *s, char c){
	if (s->length + 1 >= s->alloc_size){
		int new_size = s->length + STRING_LENGHT;
        s->str = (char *) realloc(s->str, new_size);
		if (s->str == NULL){
			return false;
		}
		s->alloc_size = new_size;
	}

	s->str[s->length] = c;
    s->length++;
	s->str[s->length] = '\0';

	return true;
}


bool str_add_const_str(struct str_struct *s, const char *const_string){
	unsigned int const_str_length = (unsigned int) strlen(const_string);

	if (s->length + const_str_length + 1 >= s->alloc_size){
		unsigned int new_size = s->length + const_str_length + 1;
        s->str = (char *) realloc(s->str, new_size);
		if (s->str == NULL){
			return false;
		}
		s->alloc_size = new_size;
	}

	s->length += const_str_length;
	strcat(s->str, const_string);
	s->str[s->length] = '\0';

	return true;
}


int str_cmp_const_str(struct str_struct *str, const char *const_string){
    //vracia 0 ak sa rovnaju
	return strcmp(str->str, const_string);
}


bool str_copy(struct str_struct *src, struct str_struct *dst){
	
	unsigned int new_length = src->length;
	if (new_length >= dst->alloc_size){
        
		dst->str = (char *) realloc(dst->str, new_length + 1);
		if (dst->str == NULL){
			return false;
		}
		dst->alloc_size = new_length + 1;
	}
	strcpy(dst->str, src->str);
	dst->length = new_length;

	return true;
}