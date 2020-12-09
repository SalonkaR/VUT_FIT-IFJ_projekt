/*
//IFJ20 - projekt(varianta I.)
//Hlavny subor prekladaca, ifj20proj.c
//Matej Hornik, xhorni20
*/


#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "gen_code.h"

int main (){

    FILE *source_file;

    source_file = stdin;

    setSourceFile(source_file);

    int error_code;
    if ((error_code = parse())){
		return error_code;
	}

    print_ifjcode20();

    return 0;
}