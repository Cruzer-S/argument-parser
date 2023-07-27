#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "argument-parser.h"

int main(int argc, char *argv[])
{
	ArgumentParser parser;
	int retval = -1;

	parser = argument_parser_create(argv);
	if (parser == NULL)
		exit(EXIT_FAILURE);

	if ( !argument_parser_has(parser, "range") ) {
		fprintf(stderr, "usage: %s -range <number>\n", argv[0]);
		goto CLEANUP;
	}

	const long *range = argument_parser_get(parser, "range", int);
	if (range == NULL) {
		fprintf(stderr, "Failed to get <range>: %s\n", 
	  		argument_parser_error(parser));
		goto CLEANUP;
	}

	srand( (unsigned int) time(NULL) );
 	printf("random number: %d\n", rand() % ( (int) *range ) );

	retval = 0;
CLEANUP: argument_parser_destroy(parser);
	return retval;
}
