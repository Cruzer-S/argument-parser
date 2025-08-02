#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "argument-parser.h"

#define ARRAY_SIZE(ARR) sizeof(ARR) / sizeof(*ARR)

char *name = "default name";
bool flag = false;
int port = 1584;

struct argument_info argument_info[] = {
	{
		"name", "n", "this is name",
		(ArgumentValue *) &name,
		ARGUMENT_PARSER_TYPE_STRING
	},
	{
		"flag", "f", "this is flag",
		(ArgumentValue *) &flag,
		ARGUMENT_PARSER_TYPE_FLAG
	},
	{
		"port", "p", "this is port number",
		(ArgumentValue *) &port,
		ARGUMENT_PARSER_TYPE_INTEGER | ARGUMENT_PARSER_TYPE_MANDATORY
	}
};

int main(int argc, char *argv[])
{
	ArgumentParser parser;
	
	parser = argument_parser_create(argc, argv);
	if (parser == NULL) {
		fprintf(stderr, "failed to argument_parser_create(): %s",
	  			strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < ARRAY_SIZE(argument_info); i++)
		argument_parser_add(parser, &argument_info[i]);

	if (argument_parser_parse(parser) == -1) {
		fprintf(stderr, "failed to argument_parser_parse(): %s\n",
	  		argument_parser_get_error(parser));
		exit(EXIT_FAILURE);
	}

	printf("name: %s\n", name);
	printf("flag: %s\n", flag ? "on" : "off");
	printf("port: %d\n", port);
	
	argument_parser_destroy(parser);

	return 0;
}
