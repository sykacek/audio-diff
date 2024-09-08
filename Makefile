all: run
build:
	gcc -Wall -ggdb -O0 main.c fft.c audio_headers.c -lm -o exe
run: build
	./exe