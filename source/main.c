#include <stdio.h>
#include <stdlib.h>

#include "argument-parser.h"

#define ARRAY_SIZE(ARR) sizeof(ARR) / sizeof(*ARR)

char *name = "default name";
bool flag = false;
int port = 1584;

struct argument_info argument_info[] = {
	{
		"n", "name", "this is name",
		(ArgumentValue *) &name, ARGUMENT_PARSER_TYPE_STRING
	},
	{
		"f", "flag", "this is flag",
		(ArgumentValue *) &flag, ARGUMENT_PARSER_TYPE_FLAG
	},
	{
		"p", "port", "this is port number",
		(ArgumentValue *) &port, ARGUMENT_PARSER_TYPE_INTEGER
	}
};

int main(int argc, char *argv[])
{
	ArgumentParser parser;
	
	parser = argument_parser_create(argv);
	if (parser == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; i < ARRAY_SIZE(argument_info); i++)
		argument_parser_add(parser, &argument_info[i]);

	argument_parser_parse(parser);

	printf("name: %s\n", name);
	printf("flag_on: %s\n", flag ? "on" : "off");
	printf("port: %d\n", port);

	argument_parser_destroy(parser);

	return 0;
}
