all: run
build:
	gcc -Wall -ggdb -O0 main.c fft.c -lm -o exe
run: build
	./exe