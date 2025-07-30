#include "argument-parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct parsed_argument {
	char name[MAX_NAME_LEN];
	char *value;
};

struct argument_parser {
	int argc;
	char **argv;

	int num_args;
	ArgumentInfo info[MAX_ARGUMENTS];

	int num_parsed;
	struct parsed_argument parsed[MAX_ARGUMENTS];
};

static struct argument_info *find_argument(
	ArgumentParser parser, const char *name
) {
	for (int i = 0; i < parser->num_args; i++) {
		if (strcmp(parser->info[i]->name, name) == 0 ||
		    strcmp(parser->info[i]->longname, name) == 0)
		{
			return parser->info[i];
		}
	}

	return NULL;
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

void argument_parser_add(ArgumentParser parser, ArgumentInfo info)
{
	parser->info[parser->num_args++] = info;
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
