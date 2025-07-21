#include <stdio.h>
#include <stdlib.h>

#include "argument-parser.h"

int main(int argc, char *argv[])
{
	ArgumentParser parser;
	ArgumentValue name, flag, port;

	parser = argument_parser_create(argv);
	if (parser == NULL)
		exit(EXIT_FAILURE);

	name.s = "default name";
	flag.b = false;
	port.i = 1584;

	argument_parser_add(
		parser, "n", "name", "this is name", &name,
		ARGUMENT_PARSER_TYPE_STRING
	);

	argument_parser_add(
		parser, "f", "flag", "this is flag", &flag,
		ARGUMENT_PARSER_TYPE_BOOLEAN
	);

	argument_parser_add(
		parser, "p", "port", "this is port number", &port,
		ARGUMENT_PARSER_TYPE_INTEGER
	);

	argument_parser_parse(parser);

	printf("name: %s\n", name.s);
	printf("flag_on: %s\n", flag.b ? "on" : "off");
	printf("port: %d\n", port.i);

	argument_parser_destroy(parser);

	return 0;
}
