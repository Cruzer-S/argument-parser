#ifndef ARGUMENTS_H__
#define ARGUMENTS_H__

#include <stdbool.h>

#define argument_parser_get(PARSER, TOKEN, TYPE) \
	argument_parser_get_##TYPE(PARSER, TOKEN)

struct argument_parser;

typedef struct argument_parser *ArgumentParser;

ArgumentParser argument_parser_create(char *args[]);

bool argument_parser_has(ArgumentParser parser, char *token);

const long *argument_parser_get_int(ArgumentParser parser, char *token);
const char *argument_parser_get_char(ArgumentParser parser, char *token);
const char (*argument_parser_get_string(ArgumentParser parser, char *token))[];
const double *argument_parser_get_float(ArgumentParser parser, char *token);

const char *argument_parser_error(ArgumentParser parser);

void argument_parser_destroy(ArgumentParser );

#endif
