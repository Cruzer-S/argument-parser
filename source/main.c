#include <stdio.h>
#include <stdlib.h>

#include "argument-parser.h"

int main(int argc, char *argv[])
{
	ArgumentParser parser;
	char *name;
	bool flag;
	int port;

	parser = argument_parser_create(argv);
	if (parser == NULL)
		exit(EXIT_FAILURE);

	name = "default name";
	flag = false;
	port = 1584;

	argument_parser_add(
		parser, "n", "name", "this is name", (ArgumentValue *) &name,
		ARGUMENT_PARSER_TYPE_STRING
	);

	argument_parser_add(
		parser, "f", "flag", "this is flag", (ArgumentValue *) &flag,
		ARGUMENT_PARSER_TYPE_BOOLEAN
	);

	argument_parser_add(
		parser, "p", "port", "this is port number", (ArgumentValue *) &port,
		ARGUMENT_PARSER_TYPE_INTEGER
	);

	argument_parser_parse(parser);

	printf("name: %s\n", name);
	printf("flag_on: %s\n", flag ? "on" : "off");
	printf("port: %d\n", port);

	argument_parser_destroy(parser);

	return 0;
}
