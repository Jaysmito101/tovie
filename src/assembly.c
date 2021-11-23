#include "assembly.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef enum token_type {
	TOKEN_NONE = -1,
	TOKEN_END,
	TOKEN_INVALID,
	TOKEN_NUMBER,
	TOKEN_STRING,
	TOKEN_WORD
} token_type_t;

typedef struct token {
	token_type_t  type;
	const char*	  begin;
	size_t		  length;
	struct token* next;
} token_t;

void syntax_error(const char* begin, size_t length, const char* message) {
	char* text	 = strdup(begin);
	text[length] = '\0';
	fprintf(stderr, "error: [%s]: %s", text, message);
}

uint8_t* parse_asm(const char* content) {
	token_t* head_token	   = malloc(sizeof(token_t));
	token_t* current_token = head_token;

	const char*	 sub = content;
	const char*	 token_start;
	token_type_t type;

	while (1) {
		token_start = sub;
		type		= TOKEN_NONE;
		switch (*sub) {
			case '\0':
				goto done;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				sub++;
				break;
			case '0' ... '9':
				type = TOKEN_NUMBER;
				while (isdigit(*sub))
					sub++;
				break;
			case '"':
				type = TOKEN_STRING;
				sub++;
				while (*sub != '\0' && *sub != '"')
					sub++;

				if (*sub == '"')
					sub++;
				break;
			case 'A' ... 'Z':
			case 'a' ... 'z':
				type = TOKEN_WORD;
				while (isalnum(*sub))
					sub++;
				break;
			default:
				type = TOKEN_INVALID;
				sub++;
		}
		if (type != TOKEN_NONE) {
			current_token->type	  = type;
			current_token->begin  = token_start;
			current_token->length = (size_t) (sub - token_start);
			current_token->next	  = malloc(sizeof(token_t));

			current_token = current_token->next;
		}
	}
done:
	current_token->type = TOKEN_END;

	uint8_t* program = malloc(1024);

	for (token_t* tok = head_token; tok->type != TOKEN_END; tok = tok->next) {
		switch (tok->type) {
			case TOKEN_INVALID:
				syntax_error(tok->begin, tok->length, "illegal symbol found");
				break;
			case TOKEN_NUMBER:

			case TOKEN_WORD:
				if (!strncmp(tok->begin, "if", tok->length)) {
				}
		}
	}

	return program;
}

void run_asm(const uint8_t* program) {
}

int main() {
	parse_asm("help 423 \"abcdefg\"");
}