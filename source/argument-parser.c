#include "argument-parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define ERR(PARSER, ...) do {						      \
	snprintf((PARSER)->error, sizeof((PARSER)->error), __VA_ARGS__);      \
	return -1;							      \
} while (false)

struct argument_pair {
	char *key;
	char *value;
};

struct argument_parser {
	int argc;
	char **argv;

	int ninfo;
	ArgumentInfo info[MAX_ARGUMENTS];

	int npair;
	struct argument_pair pair[MAX_ARGUMENTS];

	char error[BUFSIZ];
};

static struct argument_info *find_argument_info(
	ArgumentParser parser, const char *name, const char *shrt
) {
	for (int i = 0; i < parser->ninfo; i++) {
		if (name && !strcmp(parser->info[i]->name, name))
			return parser->info[i];

		if (shrt && !strcmp(parser->info[i]->shrt, shrt))
			return parser->info[i];
	}

	return NULL;
}

static struct argument_pair *find_argument_pair(
	ArgumentParser parser, const char *name, const char *shrt
) {
	for (int i = 0; i < parser->npair; i++) {
		if (name && !strcmp(parser->pair[i].key, name))
			return &parser->pair[i];

		if (shrt && !strcmp(parser->pair[i].key, shrt))
			return &parser->pair[i];
	}

	return NULL;
}

static int extract_pairs(ArgumentParser parser)
{
	struct argument_pair *pair;
	struct argument_info *info;
	char *argv, *has_value;
	bool parse_key;

	parse_key = true;
	for (int i = 1; i < parser->argc; i++) {
		pair = &parser->pair[parser->npair];
		argv = parser->argv[i];

		if ( !parse_key ) {
			info = find_argument_info(parser, NULL, pair->key);
			if (info == NULL)
				ERR(parser, "invalid option %s", pair->key);

			if (info->type == ARGUMENT_PARSER_TYPE_FLAG) {
				argv = NULL;
				i--; // re-parse argv[i] as a key
			} else if (*argv == '\0') {
				ERR(parser,
				    "empty value are not allowed for %s",
				    pair->key);
			}

			pair->value = argv;
			parser->npair++;
			parse_key = true;

			continue;
		}

		if (*argv++ != '-') // option does not start with '-'
			ERR(parser, "option must be started with -");

		if (*argv != '-') { // short-key
			if (*argv == '\0') // zero-length key
				ERR(parser, "option is missing after -");

			pair->key = argv;
			parse_key = false;

			continue;
		}

		// long-key
		has_value = strchr(++argv, '=');
		if (has_value)
			*has_value++ = '\0';

		pair->key = argv;
		if (*pair->key == '\0') // zero-length key
			ERR(parser, "empty option are not allowed");

		info = find_argument_info(parser, pair->key, NULL);
		if (info == NULL)
			ERR(parser, "invalid option %s", pair->key);

		if (info->type == ARGUMENT_PARSER_TYPE_FLAG) {
			if (has_value) // flag type does not take a value
				ERR(parser, "option %s doesn't take value",
					    pair->key);
		} else {
			if (!has_value) // non-flag type requires a value
				ERR(parser, "option %s requires a value",
					    pair->key);
		}

		pair->value = has_value;
		if (pair->value && *pair->value == '\0') // zero-length value
			ERR(parser, "empty value are not allowed for %s",
       				    pair->key);

		parser->npair++;
		parse_key = true;
	}

	if ( !parse_key ) { // check whether missing value or just flag type
		info = find_argument_info(parser, NULL, pair->key);
		if (info == NULL)
			ERR(parser, "invalid option %s", pair->key);

		if (info->type != ARGUMENT_PARSER_TYPE_FLAG)
			ERR(parser, "option %s requires a value", pair->key);

		pair->value = NULL;
		parser->npair++;
	}

	return 0;
}

ArgumentParser argument_parser_create(int argc, char *argv[])
{
	ArgumentParser parser = malloc(sizeof(struct argument_parser));

	if (parser == NULL)
		goto RETURN_NULL;

	parser->argc = argc;
	parser->argv = malloc(sizeof(char *) * parser->argc);
	if (parser->argv == NULL)
		goto FREE_PARSER;

	for (int i = 0; i < parser->argc; i++) {
		parser->argv[i] = malloc(strlen(argv[i]) + 1);
		if (parser->argv[i] == NULL) {
			for (int j = i - 1; j >= 0; j--)
				free(parser->argv[i]);

			goto FREE_PARSER;
		}

		strcpy(parser->argv[i], argv[i]);
	}

	parser->ninfo = 0;
	parser->npair = 0;

	return parser;

FREE_PARSER:	free(parser);
RETURN_NULL:	return NULL;
}

void argument_parser_add(ArgumentParser parser, ArgumentInfo info)
{
	parser->info[parser->ninfo++] = info;
}

int argument_parser_parse(ArgumentParser parser)
{
	struct argument_info *info;
	struct argument_pair *pair;

	if (extract_pairs(parser) == -1)
		return -1;

	for (int i = 0; i < parser->ninfo; i++) {
		info = parser->info[i];

		if (!(info->type & ARGUMENT_PARSER_TYPE_MANDATORY))
			continue;

		if (find_argument_pair(parser, info->name, info->shrt))
			continue;

		ERR(parser, "option %s is mandatory", info->name);
	}

	for (int i = 0; i < parser->npair; i++) {
		pair = &parser->pair[i];

		info = find_argument_info(parser, pair->key, pair->key);
		if (info == NULL)
			ERR(parser, "invalid option %s", pair->key);

		if (info->type & ARGUMENT_PARSER_TYPE_FLAG) {
			info->output->b = 1;
			continue;
		}

		if (info->type & ARGUMENT_PARSER_TYPE_INTEGER) {
			long value;
			char *endptr;

			value = strtol(pair->value, &endptr, 10);
			if (pair->value == endptr)
				ERR(parser, "invalid value %s for %s",
					    pair->value, pair->key);

			if (errno == ERANGE)
				ERR(parser, "value %s out of range",
					    pair->value);

			info->output->i = value;
		} else if (info->type & ARGUMENT_PARSER_TYPE_STRING) {
			info->output->s = pair->value;
		}
	}

	return 0;
}

void argument_parser_destroy(ArgumentParser parser)
{
	for (int i = 0; i < parser->argc; i++)
		free(parser->argv[i]);

	free(parser->argv);
	free(parser);
}

char *argument_parser_get_error(ArgumentParser parser)
{
	return parser->error;
}
