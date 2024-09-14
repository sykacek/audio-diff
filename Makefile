all: run
build:
	gcc -Wall -ggdb -O0 main.c fft.c audio_headers.c bands.c -lm -o exe
run: build
	./exe