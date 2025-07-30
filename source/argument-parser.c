#include "argument-parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct argument_info {
	char *name;
	char *longname;
	char *description;

	ArgumentValue *output;

	ArgumentParserType type;
};

struct parsed_argument {
	char name[MAX_NAME_LEN];
	char *value;
};

struct argument_parser {
	int argc;
	char **argv;

	int num_args;
	struct argument_info args[MAX_ARGUMENTS];

	int num_parsed;
	struct parsed_argument parsed[MAX_ARGUMENTS];
};

static struct argument_info *find_argument(
	ArgumentParser parser, const char *name
) {
	for (int i = 0; i < parser->num_args; i++) {
		if (strcmp(parser->args[i].name, name) == 0 ||
		    strcmp(parser->args[i].longname, name) == 0)
		{
			return &parser->args[i];
		}
	}

	return NULL;
}

static bool check_duplicate_name(
	ArgumentParser parser, char *name, char *longname
) {
	for (int i = 0; i < parser->num_args; i++) {
		if (!strcmp(parser->args[i].name, name)
      		 || !strcmp(parser->args[i].name, longname))
			return true;

		if (!strcmp(parser->args[i].longname, name)
		 || !strcmp(parser->args[i].longname, longname))
			return true;
	}

	return false;
}

ArgumentParser argument_parser_create(char *args[])
{
	ArgumentParser parser = malloc(sizeof(struct argument_parser));

	if (parser == NULL)
		return NULL;

	parser->num_args = 0;
	parser->num_parsed = 0;

	parser->argv = args;
	for (int i = 0; args[i]; i++)
		parser->argc = i;

	return parser;
}

int argument_parser_add(
	ArgumentParser parser,
	char name[MAX_NAME_LEN], char longname[MAX_NAME_LEN], char *description,
	ArgumentValue *output, ArgumentParserType type
) {
	int n = parser->num_args;

	if (strlen(name) >= MAX_NAME_LEN || strlen(longname) >= MAX_NAME_LEN)
		return -1;

	if (check_duplicate_name(parser, name, longname))
		return -1;

	parser->args[n].name = name;
	parser->args[n].longname = longname;
	parser->args[n].description = description;
	parser->args[n].output = output;
	parser->args[n].type = type;

	parser->num_args++;

	return 0;
}

int argument_parser_parse(ArgumentParser parser)
{
	struct argument_info *info;
	struct parsed_argument *parsed;
	
	for (int i = 1; i <= parser->argc; i++) {
		char *arg, *end;

		parsed = &parser->parsed[parser->num_parsed];
		arg = parser->argv[i];

		if (*arg != '-')
			return -1;
		arg++;

		if (*arg == '-') {
			arg++;

			end = strchr(arg, '=');
			if (end == NULL) {
				strcpy(parsed->name, arg);
			} else {
				strncpy(parsed->name, arg, end - arg);
				parsed->name[end - arg] = '\0';
			}

			end++;
		} else {
			strcpy(parsed->name, arg);

			if (parser->argv[i + 1] == NULL
			||  *parser->argv[i + 1] == '-')
			{
				parser->num_parsed++;
				parsed->value = NULL;
				continue;
			}

			arg = parser->argv[++i];
			end = arg;
		}

		parsed->value = end;
		parser->num_parsed++;
	}

	for (int i = 0; i < parser->num_parsed; i++) {
		parsed = &parser->parsed[i];
		info = find_argument(parser, parsed->name);

		if (info == NULL)
			return -1;

		if (info->type & ARGUMENT_PARSER_TYPE_BOOLEAN) {
			info->output->b = true;
			continue;
		}

		if (parsed->value == NULL)
			return -1;

		if (info->type & ARGUMENT_PARSER_TYPE_INTEGER) {
			info->output->i = atoi(parsed->value);
		} else if (info->type & ARGUMENT_PARSER_TYPE_STRING) {
			info->output->s = parsed->value;
		} else {
			return -1;
		}
	}

	return 0;
}

void argument_parser_destroy(ArgumentParser parser)
{
	free(parser);
}
