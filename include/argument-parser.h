#ifndef ARGUMENTS_H__
#define ARGUMENTS_H__

#include <stdbool.h>
#include <stddef.h>

#define MAX_ARGUMENTS	128

struct argument_parser;

typedef struct argument_parser *ArgumentParser;

typedef enum argument_parser_type {
	ARGUMENT_PARSER_TYPE_VOID	= 0x0001,
	ARGUMENT_PARSER_TYPE_INTEGER	= 0x0002,
	ARGUMENT_PARSER_TYPE_STRING	= 0x0004,
	ARGUMENT_PARSER_TYPE_BOOLEAN	= 0x0008,
	ARGUMENT_PARSER_TYPE_MANDATORY	= 0x0010,
} ArgumentParserType;

typedef union argument_value {
	int i;
	char *s;
	bool b;
} ArgumentValue;

ArgumentParser argument_parser_create(char *args[]);

void argument_parser_add(ArgumentParser,
			 char *name, char *longname, char *description,
			 ArgumentValue *output, ArgumentParserType );

int argument_parser_parse(ArgumentParser );

void argument_parser_destroy(ArgumentParser );

#endif
