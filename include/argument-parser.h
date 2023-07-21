#ifndef ARGUMENTS_H__
#define ARGUMENTS_H__

#include <stdbool.h>

#define argument_parser_get(PARSER, TOKEN, TYPE) \
	*_argument_parser_get_##TYPE(PARSER, TOKEN)

struct argument_parser;

enum argument_parser_error
{
	ARGUMENT_PARSER_ERROR_UNDEFINED,
};

typedef struct argument_parser *ArgumentParser;
typedef enum argument_parser_error ArgumentParserError;

ArgumentParser argument_parser_create(char *args[]);

bool argument_parser_has(ArgumentParser parser, char *token);

int *_argument_parser_get_int(ArgumentParser parser, char *token);
char *_argument_parser_get_char(ArgumentParser parser, char *token);
char (*_argument_parser_get_string(ArgumentParser parser, char *token))[];
double *_argument_parser_get_float(ArgumentParser parser, char *token);

void argument_parser_destroy(ArgumentParser );

#endif
