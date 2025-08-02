#ifndef ARGUMENTS_H__
#define ARGUMENTS_H__

#include <stdbool.h>
#include <stddef.h>

#define MAX_ARGUMENTS	128
#define MAX_NAME_LEN	64

struct argument_parser;

typedef struct argument_parser *ArgumentParser;
typedef struct argument_info *ArgumentInfo;

typedef enum argument_parser_type {
	ARGUMENT_PARSER_TYPE_INTEGER	= 0x0001,
	ARGUMENT_PARSER_TYPE_STRING	= 0x0002,
	ARGUMENT_PARSER_TYPE_FLAG	= 0x0004,
	ARGUMENT_PARSER_TYPE_MANDATORY	= 0x0008,
} ArgumentParserType;

typedef union argument_value {
	int i;
	char *s;
	bool b;
} ArgumentValue;

struct argument_info {
	char *name;
	char *shrt;
	char *description;

	ArgumentValue *output;
	ArgumentParserType type;
};

ArgumentParser argument_parser_create(int argc, char *argv[]);

void argument_parser_add(ArgumentParser , ArgumentInfo );

int argument_parser_parse(ArgumentParser );

void argument_parser_destroy(ArgumentParser );

char *argument_parser_get_error(ArgumentParser );

#endif
