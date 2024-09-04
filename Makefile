all: run
build:
	gcc -Wall -ggdb -O0 main.c -o exe
run: build
	./exe