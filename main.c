#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define DATA_BLOCK_ID "data"
#define RIFF "RIFF"
#define WAVE "WAVE"
#define FMT "fmt"

struct wav_header {
    char riff [4];
    unsigned long len; //len of file in bytes
    char wave [4];
    char fmt[4];
    unsigned int flen; //format len in bytes
    uint32_t ftype; //format type
    uint32_t channels;
    uint32_t srate; //sample rate
    uint32_t bytePerSecond; //bytes per second = srate * channels * ftype / 8
    uint16_t bytePerBlock;
    uint16_t bitsPerSample;
    char data[4];
    uint32_t flen_readable; //in bytes
};

/* bytes to integer */
long btoi(char * __loc, uint8_t __size){
    long ret = 0;
    for(int i = 0; i < __size; ++i)
        ret += __loc[i] << i * 8;

    return ret;
}

int main(int argc, char **argv){
    char head[40] = {0};
    char buf[4] = {0};
    int i = 0;
    struct wav_header header;

    FILE *read, *write;
    read = fopen("audio/foobar.wav", "rb");
    write = fopen("copy.wav", "w");
/*
    fread(head, 1, 40, read);
    fwrite(head, 1, 40, write);

    fread(buf, 1, 4, read);
    header.flen_readable = btoi(buf, 4);

    for(int i = 0; i < header.flen_readable; i += 4){
        fread(buf, 1, 4, read);
     
        fwrite(buf, 1, 4, write);
    }
*/
    do {
        fread(buf, 1, 4, read);
        i++;
    } while(!strcmp(buf, "data"));

    printf("%d\n", i);

    fclose(read);
    fclose(write);

    return 0;
}