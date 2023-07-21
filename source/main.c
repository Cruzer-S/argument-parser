#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "argument-parser.h"

int main(int argc, char *argv[])
{
	ArgumentParser parser;

	parser = argument_parser_create(argv);
	if (parser == NULL)
		exit(EXIT_FAILURE);

	if ( !argument_parser_has(parser, "range") ) {
		fprintf(stderr, "usage: %s -range <number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int range = argument_parser_get(parser, "range", int);

	srand( (unsigned int) time(NULL) );
 	printf("random number: %d\n", rand() % range);

	argument_parser_destroy(parser);

	return 0;
}
