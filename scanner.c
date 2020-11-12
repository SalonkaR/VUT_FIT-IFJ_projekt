// lexikalni analyzator
#include <stdio.h>
#include <ctype.h>
#include "str.h"
#include "scanner.h"
#include "error.h"

#define STATE_START 100 //S
#define STATE_EOL 101  //F1
#define STATE_SEMICOLON 102 //F2
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
#define STATE_RIGHT_VINCULUM 120 //
#define STATE_STRING 121 //
#define STATE_STRING_BACKSLASH 122 //ocakavame bud x alebo ", n, t, "\"
#define STATE_STRING_HEXADECIMAL 123 //
#define STATE_STRING_HEXADECIMAL_SECOND 124
#define STATE_STRING_END 125 //F20 ... to be continue
#define STATE_NUMBER 126 //
#define STATE_NUMBER_E 127 //
#define STATE_NUMBER_E_SIGN 128 //
#define STATE_NUMBER_E_END 129 //
#define STATE_NUMBER_FLOAT 130 //
#define STATE_NUMBER_FLOAT_END 131 //
#define STATE_ID 132 //
#define STATE_DIVIDE_OR_COMMENTARY 133 //
#define STATE_COMMENTARY_LINE 134 //
#define STATE_COMMENTARY_BLOCK_START 135 //
#define STATE_COMMENTARY_BLOCK_END 136 //

// promenna pro ulozeni vstupniho souboru
FILE *source;
struct str_struct *str;

void setSourceFile(FILE *f)
{
  source = f;
}

int cleaner(int exit_code, struct str_struct *s)
{
	str_free(s);
	return exit_code;
}

int get_token(struct token *token)
// hlavni funkce lexikalniho analyzatoru
{
	if (source == NULL)
	{
		return ERR_INTER;
	}

	if (str == NULL)
	{
		return ERR_INTER;
	}

    int state = STATE_START;
    int c;
    // vymazeme obsah atributu a v pripade identifikatoru
    // budeme postupne do nej vkladat jeho nazev
    str_clear(token);
	
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
					token->type = T_TYPE_SEMICOLON;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == ',')
				{
					token->type = T_TYPE_COMMA;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == '*')
				{
					token->type = T_TYPE_MUL;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == '-')
				{
					token->type = T_TYPE_MINUS;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == '+')
				{
					token->type = T_TYPE_PLUS;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == '(')
				{
					token->type = T_TYPE_LEFT_BRACKET;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == ')')
				{
					token->type = T_TYPE_RIGHT_BRACKET;
					return cleaner(LEX_TOKEN_OK, str);
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
					token->type = T_TYPE_LEFT_VINCULUM;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == '}')
				{
					token->type = T_TYPE_RIGHT_BRACKET;
					return cleaner(LEX_TOKEN_OK, str);
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
				else if (c == EOF)
				{
					token->type = T_TYPE_EOF;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else
				{
					return cleaner(LEX_ERR, str);
				}
				break;

			case(STATE_EOL):
				if (isspace(c))
				{
					break;
				}
				ungetc(c, source);
				token->type = T_TYPE_EOL;
				return cleaner(LEX_TOKEN_OK, str);

			case(STATE_LESS_THAN):
				if (c == '=')
				{
					token->type = T_TYPE_LESS_EQUAL;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else
				{
					ungetc(c, source);
					token->type = T_TYPE_LESS;
					return cleaner(LEX_TOKEN_OK, str);
				}

			case(STATE_MORE_THAN):
				if (c == '=')
				{
					token->type = T_TYPE_MORE_EQUAL;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else
				{
					ungetc(c, source);
					token->type = T_TYPE_MORE;
					return cleaner(LEX_TOKEN_OK, str);
				}

			case(STATE_ASSIGNMENT):
				if (c == '=')
				{
					token->type = T_TYPE_COMPARISON;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else
				{
					ungetc(c, source);
					token->type = T_TYPE_ASSIGN;
					return cleaner(LEX_TOKEN_OK, str);
				}

			case(STATE_SCREAMER):
				if (c == '=')
				{
					token->type = T_TYPE_NOT_EQUAL;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else
				{
					return cleaner(LEX_ERR, str);
				}

			case(STATE_COLON):
				if (c == '=')
				{
					token->type = T_TYPE_VARIABLE_DEFINITION;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else
				{
					return cleaner(LEX_ERR, str);
				}

			case(STATE_STRING):
				if (c < 32 || c > 255)
				{
					return cleaner(LEX_ERR, str);
				}
				else if (c == '\'')
				{
					state = STATE_STRING_BACKSLASH;
				}
				else if (c == '"')
				{
					
				}

			case(STATE_STRING_BACKSLASH):
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
				if (c != '/' || c != '*')
				{
					token->type = T_TYPE_DIV;
					return cleaner(LEX_TOKEN_OK, str);
				}
				else if (c == '/')
				{
					state = STATE_COMMENTARY_LINE;
				}
				else if (c == '*')
				{
					state = STATE_COMMENTARY_BLOCK_START;
				}
				break;

			case(STATE_COMMENTARY_LINE):
				if (c == '\n' || c == EOF)	
				{
					state = STATE_START;
					ungetc(c, source);
				}
				break;

			case(STATE_COMMENTARY_BLOCK_START):				
				if (c == '*')
				{
					state = STATE_COMMENTARY_BLOCK_END;
				}
				break;
				
			case(STATE_COMMENTARY_BLOCK_END):
				if (c == '/')
				{
					state = STATE_START;
				}
				else if (c != '*')
				{
					state = STATE_COMMENTARY_BLOCK_START;
				}
				else if (c == '*')
				{
					state = STATE_COMMENTARY_BLOCK_END;
				}
				break;

		}
	}
}