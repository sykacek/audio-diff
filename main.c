#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "fft.h"

#define DATA_BLOCK_ID "data"
#define FILE_TYPE_BLOCK_ID "RIFF"
#define FILE_FORMAT_ID "WAVE"
#define FORMAT_BLOCK_ID "fmt"

#define puti(a) printf("%d\n", a)

typedef uint32_t uint;

struct wav_header {
    char fileTypeBlockID[4];
    uint fileSize;
    char fileFormatID[4];
    
    char formatBlockID[4];
    uint blockSize;
    uint16_t audioFormat;
    uint16_t nChannels;
    uint frequency;
    uint bytePerSec;
    uint16_t bytePerBlock;
    uint16_t bitsPerSample;

    char dataBlockID[4];
    uint dataSize;
};

void show_header(struct wav_header *h){
    puts(h->fileTypeBlockID);
    puti(h->fileSize);
    puts(h->fileFormatID);

    puts(h->formatBlockID);
    puti(h->blockSize);
    puti(h->audioFormat);
    puti(h->nChannels);
    puti(h->frequency);
    puti(h->bytePerSec);
    puti(h->bytePerBlock);
    puti(h->bitsPerSample);

    puts(h->dataBlockID);
    puti(h->dataSize);
}

int main(int argc, char **argv){
    struct wav_header header;

    FILE *read;
    read = fopen("audio/foobar.wav", "rb");

    fread(&header, sizeof(header), 1, read);

    show_header(&header);
    fclose(read);

    int arr[2][8] = {0};

    for(int i = 0; i < 8; ++i)
        arr[0][i] = (double)1000*sin(i) + 1;

    fft(arr[0], arr[1]);

    for(int i = 0; i < size(arr[1]); ++i){
        puti(arr[1][i]);
    }
    return 0;
}