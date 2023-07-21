#include "argument-parser.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct argument_parser
{
	char **args;
	int count;

	void *value;
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
	if (parser == NULL)
		return NULL;

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

int *_argument_parser_get_int(ArgumentParser parser, char *token)
{
	int index = find_token_index(parser->args, token);

	if (index++ < 0)
		return NULL;

	if (parser->value != NULL) free(parser->value);

	parser->value = malloc(sizeof(int));
	if (parser->value == NULL)
		return NULL;

	char *endptr;
	long value = strtol(parser->args[index], &endptr, 10);
	if (endptr == parser->args[index])
		return NULL;

	if (value >= INT_MAX || value < INT_MIN)
		return NULL;

	*(int *) parser->value = (int) value;

	return parser->value;
}

char (*_argument_parser_get_string(ArgumentParser parser, char *token))[]
{
	return NULL;
}

double *argument_parser_get_float(ArgumentParser parser, char *token)
{
	return NULL;
}

char *_argument_parser_get_char(ArgumentParser parser, char *token)
{
	return NULL;
}

void argument_parser_destroy(ArgumentParser parser)
{
	if (parser->value)
		free(parser->value);

	free(parser);
}
