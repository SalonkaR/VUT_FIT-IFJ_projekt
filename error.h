/*
//IFJ20 - projekt(varianta I.)
//definicia chyb
//Matej Hornik, xhorni20
//Filip Brna, xbrnaf00
*/

#ifndef ERROR_H
#define ERROR_H


#define LEX_TOKEN_OK		    	0 // Token je v poriadku
#define SYN_OK 				    	0 // Syntax is ok.
#define LEX_ERR             		1 // Lexikalna chyba
#define SYN_ERR					    2 // Chyba v syntaktickej analyze (neocakavane ordiadkovanie)
#define SEM_ERR_UNDEFINED_VAR		3 // Semanticka chyba - nedefinova funkcia/ premenna
#define SEM_ERR_TYPE		        4 // Semanticka chyba - pri odvodzovani datove typu pri deklarovani novej premmenej
#define SEM_ERR_EXP                 5 // Semanticka chyba - chyba pri aritm. operaciach
#define SEM_ERR_NO_PARAMS			6 // Semantic error - Zly pocet navratoch/ vstupnych hodnot
#define SEM_ERR_OTHER               7 // Ostattne semanticke chyby
#define SEM_ERR_ZERO_DIV            9 // Semanticka chyba - delenie nulou
#define ERROR_INTERNAL				99 // Interna chyba programu


#endif