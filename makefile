argument-parser: main.o argument-parser.o

.PHONY: clean
clean:
	rm -f argument-parser.o main.o argument-parser
