#include "argument-parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum argument_parser_error
{
	PARSER_ERROR_STANDARD_LIBRARY,
	PARSER_ERROR_UNDEFINED,
	PARSER_ERROR_WRONG_FORMAT,
};

typedef enum argument_parser_error ParserError;

struct argument_parser
{
	char **args;
	int count;

	void *value;

	ParserError reason;
};

static const char * const error_string[] = {
	[PARSER_ERROR_STANDARD_LIBRARY] = "Standard library error",
	[PARSER_ERROR_UNDEFINED] = "Undefined",
	[PARSER_ERROR_WRONG_FORMAT] = "Wrong Format"
};

static int find_token_index(char **args, char *token)
{
	for (int i = 0; args[i]; i++)
	{
		if (args[i][0] != '-')
			continue;

		if (strcmp(&args[i][1], token))
			continue;

		return i;
	}

	return -1;
}

ArgumentParser argument_parser_create(char *args[])
{
	ArgumentParser parser;

	parser = malloc(sizeof(struct argument_parser));
	if (parser == NULL) {
		parser->reason = PARSER_ERROR_STANDARD_LIBRARY;
		return NULL;
	}
	
	parser->value = NULL;

	parser->args = args;
	for (int i = 0; args[i]; i++)
		parser->count = i;

	return parser;
}

bool argument_parser_has(ArgumentParser parser, char *token)
{
	return find_token_index(parser->args, token) > 0;
}

const long *argument_parser_get_int(ArgumentParser parser, char *token)
{
	int index = find_token_index(parser->args, token);

	if (index++ < 0) {
		parser->reason = PARSER_ERROR_UNDEFINED;
		return NULL;
	}

	if (parser->value != NULL) {
		free(parser->value);
		parser->value = NULL;
	}

	parser->value = malloc(sizeof(long));
	if (parser->value == NULL) {
		parser->reason = PARSER_ERROR_STANDARD_LIBRARY;
		return NULL;
	}

	char *endptr;
	long value = strtol(parser->args[index], &endptr, 10);
	if (endptr == parser->args[index]) {
		parser->reason = PARSER_ERROR_WRONG_FORMAT;
		return NULL;
	}

	*(long *) parser->value = (long) value;

	return parser->value;
}

const char (*argument_parser_get_string(ArgumentParser parser, char *token))[]
{
	int index = find_token_index(parser->args, token);

	if (index++ < 0)
		return NULL;

	if (parser->value != NULL) {
		free(parser->value);
		parser->value = NULL;
	}

	size_t len = strlen(parser->args[index]);
	parser->value = malloc(len + 1);
	if (parser->value == NULL) {
		parser->reason = PARSER_ERROR_STANDARD_LIBRARY;
		return NULL;
	}

	memcpy(parser->value, parser->args[index], len + 1);

	return parser->value;
}

const double *argument_parser_get_float(ArgumentParser parser, char *token)
{
	int index = find_token_index(parser->args, token);

	if (index++ < 0) {
		parser->reason = PARSER_ERROR_UNDEFINED;
		return NULL;
	}

	if (parser->value != NULL) {
		free(parser->value);
		parser->value = NULL;
	}

	parser->value = malloc(sizeof(double));
	if (parser->value == NULL) {
		parser->reason = PARSER_ERROR_STANDARD_LIBRARY;
		return NULL;
	}

	int retval = sscanf(
		parser->args[index], "%lf", (double *) parser->value
	);
	if (retval != 1) {
		parser->reason = (retval == EOF)
			? PARSER_ERROR_STANDARD_LIBRARY 
			: PARSER_ERROR_WRONG_FORMAT
		;

		return NULL;
	}
	
	return parser->value;
}

const char *argument_parser_get_char(ArgumentParser parser, char *token)
{
	int index = find_token_index(parser->args, token);

	if (index++ < 0) {
		parser->reason = PARSER_ERROR_UNDEFINED;
		return NULL;
	}

	if (parser->value != NULL) {
		free(parser->value);
		parser->value = NULL;
	}

	parser->value = malloc(sizeof(char));
	if (parser->value == NULL) {
		parser->reason = PARSER_ERROR_STANDARD_LIBRARY;
		return NULL;
	}

	* (char *) parser->value = *parser->args[index];

	return parser->value;
}

const char * argument_parser_error(ArgumentParser parser)
{
	return error_string[parser->reason];
}

void argument_parser_destroy(ArgumentParser parser)
{
	if (parser->value)
		free(parser->value);

	free(parser);
}
