run: build
	./bin/main

build: 
	mkdir -p ./bin
	cc -o ./bin/main main.c lexer.c parser.c vm.c -lm -ggdb
