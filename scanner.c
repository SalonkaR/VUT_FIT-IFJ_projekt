// lexikalni analyzator
#include <stdio.h>
#include <ctype.h>
#include "str.h"
#include "scaner.h"

#define STATE_START 100 //S
#define STATE_EOL 101  //F1
#define STATE_FOR_SPLITER 102 //F2
#define STATE_COMMA 103 //F3
#define STATE_MULTIPLY 104 //F4
#define STATE_MINUS 105 //F5
#define STATE_PLUS 106 //F6
#define STATE_LEFT_BRACKET 107 //F7
#define STATE_RIGHT_BRACKET 108 //F8
#define STATE_LESS_THAN 109 //F9
#define STATE_LESS_OR_EQUAL_THAN 110 //F10 
#define STATE_MORE_THAN 111 //F11
#define STATE_MORE_OR_EQUAL_THAN 112 //F12
#define STATE_ASSIGNMENT 113 //F13
#define STATE_EQUAL 114 //F14
#define STATE_SCREAMER 115 //R1
#define STATE_NOT_EQUAL 116 //F15
#define STATE_COLON 117 //R2
#define STATE_DECLARATION 118 //F16 
#define STATE_LEFT_VINCULUM 119 //
#define STATE_RIGHT_VINCULUM 120 
#define STATE_STRING 121
#define STATE_STRING_BACKSLAH 122 //ocakavame bud x alebo ", n, t, "\"
#define STATE_STRING_HEXADECIMAL 123
#define STATE_STRING_HEXADECIMAL_SECOND 124
#define STATE_STRING_END 125 //F20 ... to be continue
#define STATE_NUMBER 126
#define STATE_NUMBER_E 127
#define STATE_NUMBER_E_SIGN 128
#define STATE_NUMBER_E_END 129
#define STATE_NUMBER_FLOAT 130
#define STATE_NUMBER_FLOAT_END 131
#define STATE_ID 132
#define STATE_DIVIDE_OR_COMMENTARY 133
#define STATE_COMMENTARY_LINE 134
#define STATE_COMMENTARY_BLOCK_START 135
#define STATE_COMMENTARY_BLOCK_END 136


// promenna pro ulozeni vstupniho souboru
FILE *source;

void setSourceFile(FILE *f)
{
  source = f;
}

int getNextToken(string *attr)
// hlavni funkce lexikalniho analyzatoru
{
   int state = STATE_START;
   int c;
   // vymazeme obsah atributu a v pripade identifikatoru
   // budeme postupne do nej vkladat jeho nazev
   strClear(attr);
   while (1)
   {     
		// nacteni dalsiho znaku
		c = getc(source);
			
		switch (state)
		{
			case 0:
			// zakladni stav automatu 
				if (isspace(c))
				// bila mista - ignorovat
				state = 0;
				else
				if (c == '<')
				// komentar
					state = 1;
				else
				if (isalpha(c))
				// identifikator nebo klicove slovo
				{    
					strAddChar(attr, c);
					state = 2;
				}
				else
				if (c == '+')
				// operator ++
					state = 3;
				else
				if (c == '-')
				// operator --
					state = 4;
				else
				if (c == '{') return LEFT_VINCULUM;
				else
				if (c == '}') return RIGHT_VINCULUM;
				else
				if (c == ';') return SEMICOLON;
				else
				if (c == EOF) return END_OF_FILE;
				else
				return LEX_ERROR;
				break;
			

			case 1:
			// komentar
				if (c == '>') state = 0; 
				else 
				// komentar pokracuje libovolnymi znaky, zustan ve stavu 1,
				// ale kontroluj, zda neprijde EOF (neukonceny komentar)
				if (c == EOF) return LEX_ERROR;
			break;
			
			
			case 2:
			// identifikator nebo klicove slovo
				if (isalnum(c))
			// identifikator pokracuje
				strAddChar(attr, c);
			else
			// konec identifikatoru
			{
					ungetc(c, source); // POZOR! Je potreba vratit posledni nacteny znak
				
				// kontrola, zda se nejedna o klicove slovo
				if (strCmpConstStr(attr, "setzero") == 0) return SETZERO;
				else 
				if (strCmpConstStr(attr, "read") == 0) return READ;
				else 
				if (strCmpConstStr(attr, "write") == 0) return WRITE;
				else 
				if (strCmpConstStr(attr, "while") == 0) return WHILE;
				else 
				// jednalo se skutecne o identifikator
				return ID;
				}
			break;   
			

			case 3:
			// pokracovani operatoru ++
				if (c == '+') return INC;
				else
				return LEX_ERROR;
			break;
			
			
			case 4:
			// pokracovani operatoru --
				if (c == '-') return DEC;
				else
				return LEX_ERROR;
			break;
		}	
    }
	while (true)
	{
		c = getc(source);

		switch (state)
		{
			case (STATE_START):
				if (isspace(c))
				{
					state = STATE_START;
				}
				else if (c == '\n')
				{
					state = STATE_EOL;
				}
				else if (c == ';')
				{
					state = STATE_FOR_SPLITER;
				}
				else if (c == ',')
				{
					state = STATE_COMMA;
				}
				else if (c == '*')
				{
					state = STATE_MULTIPLY;
				}
				else if (c == '-')
				{
					state = STATE_MINUS;
				}
				else if (c == '+')
				{
					state = STATE_PLUS;
				}
				else if (c == '(')
				{
					state = STATE_LEFT_BRACKET;
				}
				else if (c == ')')
				{
					state = STATE_RIGHT_BRACKET;
				}
				else if (c == '<')
				{
					state = STATE_LESS_THAN;
				}
				else if (c == '>')
				{
					state = STATE_MORE_THAN;
				}
				else if (c == '=')
				{
					state = STATE_ASSIGNMENT;
				}
				else if (c == '!')
				{
					state = STATE_SCREAMER;
				}
				else if (c == ':')
				{
					state = STATE_COLON;
				}
				else if (c == '{')
				{
					state = STATE_LEFT_VINCULUM;
				}
				else if (c == '}')
				{
					state = STATE_RIGHT_VINCULUM;
				}
				else if (c == '"')
				{
					state = STATE_STRING;
				}
				else if (isdigit(c))
				{
					state = STATE_NUMBER;
				}
				else if (isalpha(c) || c == '_')
				{
					state = STATE_ID;
				}
				else if (c == '/')
				{
					state = STATE_DIVIDE_OR_COMMENTARY;
				}
				else
				{
					return lexikalna chyba
				}
				break;

			case(STATE_EOL):
				if (isspace(c))
				{
					break;
				}
				token->type = T_TYPE_EOL;
				return free_resources(SCANNER_TOKEN_OK, str);

			case(STATE_FOR_SPLITER):
				token->type = T_TYPE_SEMICOLON; 
				state = STATE_START;
				return free_resources(SCANNER_TOKEN_OK, str);

			case(STATE_COMMA):
				break;

			case(STATE_MULTIPLY):
				break;

			case(STATE_MINUS):
				break;

			case(STATE_PLUS):
				break;

			case(STATE_LEFT_BRACKET):
				break;

			case(STATE_RIGHT_BRACKET):
				break;

			case(STATE_LESS_THAN):
				break;

			case(STATE_LESS_OR_EQUAL_THAN):
				break;

			case(STATE_MORE_THAN):
				break;

			case(STATE_MORE_OR_EQUAL_THAN):
				break;

			case(STATE_ASSIGNMENT):
				break;

			case(STATE_EQUAL):
				break;

			case(STATE_SCREAMER):
				break;

			case(STATE_NOT_EQUAL):
				break;

			case(STATE_COLON):
				break;

			case(STATE_DECLARATION):
				break;

			case(STATE_LEFT_VINCULUM):
				break;

			case(STATE_RIGHT_VINCULUM):
				break;

			case(STATE_STRING):
				break;

			case(STATE_STRING_BACKSLAH):
				break;

			case(STATE_STRING_HEXADECIMAL):
				break;

			case(STATE_STRING_HEXADECIMAL_SECOND):
				break;

			case(STATE_STRING_END):
				break;

			case(STATE_NUMBER):
				break;

			case(STATE_NUMBER_E):
				break;

			case(STATE_NUMBER_E_SIGN):
				break;

			case(STATE_NUMBER_E_END):
				break;

			case(STATE_NUMBER_FLOAT):
				break;

			case(STATE_NUMBER_FLOAT_END):
				break;

			case(STATE_ID):
				break;

			case(STATE_DIVIDE_OR_COMMENTARY):
				break;

			case(STATE_COMMENTARY_LINE):
				break;

			case(STATE_COMMENTARY_BLOCK_START):
				break;
				
			case(STATE_COMMENTARY_BLOCK_END):
				break;
			

				else if (c == '/')
				{
					state = STATE_DIVIDE_OR_COMMENTARY;	
				}
				else if (isalpha(c) || c == '_')
				{
					if (!dynamic_string_add_char(str, (char) tolower(c)))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
					state = SCANNER_STATE_IDENTIFIER_OR_KEYWORD;
				}
				else if (isdigit(c))
				{
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
					state = SCANNER_STATE_NUMBER;
				}
				else if (c == '!')
				{
					state = SCANNER_STATE_STRING_START;
				}
				else if (c == '<')
				{
					state = SCANNER_STATE_LESS_THAN;
				}
				else if (c == '>')
				{
					state = SCANNER_STATE_MORE_THAN;
				}
				else if (c == '=')
				{
					token->type = TOKEN_TYPE_ASSIGN;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == '+')
				{
					token->type = TOKEN_TYPE_PLUS;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == '-')
				{
					token->type = TOKEN_TYPE_MINUS;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == '*')
				{
					token->type = TOKEN_TYPE_MUL;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == '\\')
				{
					token->type = TOKEN_TYPE_IDIV;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == '(')
				{
					token->type = TOKEN_TYPE_LEFT_BRACKET;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == ')')
				{
					token->type = TOKEN_TYPE_RIGHT_BRACKET;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == ',')
				{
					token->type = TOKEN_TYPE_COMMA;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == ';')
				{
					token->type = TOKEN_TYPE_SEMICOLON;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else if (c == EOF)
				{
					token->type = TOKEN_TYPE_EOF;
					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else
				{
					return free_resources
				}

				break;

			case (SCANNER_STATE_COMMENTARY):
				if (c == '\n' || c == EOF)
				{
					state = SCANNER_STATE_START;
					ungetc(c, source_file);
				}

				break;

			case (SCANNER_STATE_BACKSLASH):
				if (c == '\'')
				{
					state = SCANNER_STATE_BLOCK_COMMENTARY;
				}
				else
				{
					ungetc(c, source_file);
					token->type = TOKEN_TYPE_DIV;
					return free_resources(SCANNER_TOKEN_OK, str);
				}

				break;

			case (SCANNER_STATE_BLOCK_COMMENTARY):
				if (c == '\'')
				{
					state = SCANNER_STATE_BLOCK_COMMENTARY_LEAVE;
				}
				else if (c == EOF)
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_BLOCK_COMMENTARY_LEAVE):
				if (c == EOF)
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}
				else if (c == '/')
				{
					state = SCANNER_STATE_START;
				}
				else if (c == '\'')
				{
					state = SCANNER_STATE_BLOCK_COMMENTARY_LEAVE;
				}
				else
				{
					state = SCANNER_STATE_BLOCK_COMMENTARY;
				}

				break;

			case (SCANNER_STATE_IDENTIFIER_OR_KEYWORD):
				if (isalnum(c) || c == '_')
				{
					if (!dynamic_string_add_char(str, (char) tolower(c)))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					ungetc(c, source_file);
					return process_identifier(str, token);
				}

				break;

			case (SCANNER_STATE_NUMBER):
				if (isdigit(c))
				{
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else if (c == '.')
				{
					state = SCANNER_STATE_NUMBER_POINT;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else if (tolower(c) == 'e')
				{
					state = SCANNER_STATE_NUMBER_EXPONENT;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					ungetc(c, source_file);
					return process_integer(str, token);
				}

				break;

			case (SCANNER_STATE_NUMBER_POINT):
				if (isdigit(c))
				{
					state = SCANNER_STATE_NUMBER_DOUBLE;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_NUMBER_DOUBLE):
				if (isdigit(c))
				{
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else if (tolower(c) == 'e')
				{
					state = SCANNER_STATE_NUMBER_EXPONENT;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					ungetc(c, source_file);
					return process_decimal(str, token);
				}

				break;

			case (SCANNER_STATE_NUMBER_EXPONENT):
				if (isdigit(c))
				{
					state = SCANNER_STATE_NUMBER_EXPONENT_FINAL;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else if (c == '+' || c == '-')
				{
					state = SCANNER_STATE_NUMBER_EXPONENT_SIGN;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_NUMBER_EXPONENT_SIGN):
				if (isdigit(c))
				{
					state = SCANNER_STATE_NUMBER_EXPONENT_FINAL;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_NUMBER_EXPONENT_FINAL):
				if (isdigit(c))
				{
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					ungetc(c, source_file);
					return process_decimal(str, token);
				}

				break;

			case (SCANNER_STATE_STRING_START):
				if (c == '"')
				{
					state = SCANNER_STATE_STRING;
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_STRING):
				if (c < 32)
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}
				else if (c == '\\')
				{
					state = SCANNER_STATE_STRING_ESCAPE;
				}
				else if (c == '"')
				{
					if (!dynamic_string_copy(str, token->attribute.string))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
					token->type = TOKEN_TYPE_STRING;

					return free_resources(SCANNER_TOKEN_OK, str);
				}
				else
				{
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}

				break;

			case (SCANNER_STATE_STRING_ESCAPE):
				if (c < 32)
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}
				else if (c == 'n')
				{
					c = '\n';
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
					state = SCANNER_STATE_STRING;
				}
				else if (c == '"')
				{
					c = '"';
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
					state = SCANNER_STATE_STRING;
				}
				else if (c == 't')
				{
					c = '\t';
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
					state = SCANNER_STATE_STRING;
				}
				else if (c == '\\')
				{
					c = '\\';
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
					state = SCANNER_STATE_STRING;
				}
				else if (c == '0')
				{
					strnum[0] = c;
					state = SCANNER_STATE_STRING_ESCAPE_ZERO;
				}
				else if (c == '1')
				{
					strnum[0] = c;
					state = SCANNER_STATE_STRING_ESCAPE_ONE;
				}
				else if (c == '2')
				{
					strnum[0] = c;
					state = SCANNER_STATE_STRING_ESCAPE_TWO;
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_STRING_ESCAPE_ZERO):
				if (c == '0')
				{
					strnum[1] = c;
					state = SCANNER_STATE_STRING_ESCAPE_ZERO_ZERO;
				}
				else if (isdigit(c))
				{
					strnum[1] = c;
					state = SCANNER_STATE_STRING_ESCAPE_REST;
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_STRING_ESCAPE_ZERO_ZERO):
				if (isdigit(c) && c != '0')
				{
					strnum[2] = c;
					state = SCANNER_STATE_STRING;

					int val = (int) strtol(strnum, &endptr, 10);
					if (*endptr)
					{
						return free_resources(ERROR_INTERNAL, str);
					}

					c = (char) val;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_STRING_ESCAPE_ONE):
				if (isdigit(c))
				{
					strnum[1] = c;
					state = SCANNER_STATE_STRING_ESCAPE_REST;
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_STRING_ESCAPE_TWO):
				if (isdigit(c))
				{
					if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4')
					{
						strnum[1] = c;
						state = SCANNER_STATE_STRING_ESCAPE_REST;
					}
					else if (c == '5')
					{
						strnum[1] = c;
						state = SCANNER_STATE_STRING_ESCAPE_TWO_FIVE;
					}
					else
					{
						return free_resources(SCANNER_ERROR_LEX, str);
					}
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_STRING_ESCAPE_TWO_FIVE):
				if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5')
				{
					strnum[2] = c;
					state = SCANNER_STATE_STRING;

					int val = (int) strtol(strnum, &endptr, 10);
					if (*endptr)
					{
						return free_resources(ERROR_INTERNAL, str);
					}

					c = (char) val;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_STRING_ESCAPE_REST):
				if (isdigit(c))
				{
					strnum[2] = c;
					state = SCANNER_STATE_STRING;

					int val = (int) strtol(strnum, &endptr, 10);
					if (*endptr)
					{
						return free_resources(ERROR_INTERNAL, str);
					}

					c = (char) val;
					if (!dynamic_string_add_char(str, c))
					{
						return free_resources(ERROR_INTERNAL, str);
					}
				}
				else
				{
					return free_resources(SCANNER_ERROR_LEX, str);
				}

				break;

			case (SCANNER_STATE_LESS_THAN):
				if (c == '>')
				{
					token->type = TOKEN_TYPE_NEQ;
				}
				else if (c == '=')
				{
					token->type = TOKEN_TYPE_LEQ;
				}
				else
				{
					ungetc(c, source_file);
					token->type = TOKEN_TYPE_LTN;
				}

				return free_resources(SCANNER_TOKEN_OK, str);

			case (SCANNER_STATE_MORE_THAN):
				if (c == '=')
				{
					token->type = TOKEN_TYPE_MEQ;
				}
				else
				{
					ungetc(c, source_file);
					token->type = TOKEN_TYPE_MTN;
				}

				return free_resources(SCANNER_TOKEN_OK, str);

			case (SCANNER_STATE_EOL):
				if (isspace(c))
				{
					break;
				}

				ungetc(c, source_file);
				token->type = TOKEN_TYPE_EOL;
				return free_resources(SCANNER_TOKEN_OK, str);


}
