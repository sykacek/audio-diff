#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

#include "fft.h"
#include "audio_headers.h"

#define putx(a) printf("%lx\n", a)
#define puti(a) printf("%ld\n", a)

int main(int argc, char **argv){
    char buf[4] = {0};
    riff_ck_t riff = {"", 0};
    ck_t chunk = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    FILE *read;

    read = fopen("audio/master.wav", "rb");
    fread(&riff, sizeof(riff), 1, read);

    if(btoi(riff.fileFormatID, 4) != btoi(RIFF_ID, 4)){
        fprintf(stderr, "Error: invalid format (RIFF)");
        return -1;
    }
    chunk.riff = &riff;

    fread(buf, 1, 4, read);
    if(btoi(buf, 4) != btoi(WAVE_ID, 4)){
        fprintf(stderr, "Error: invalid format\n");
        return -1;
    }

    fread(buf, 1, 4, read);

    if(junk_handler(read, &chunk))
        puts("failed");

    fread(buf, 1, 4, read);

    if(fmt_handler(read, &chunk))
        puts("failed 2");

    fread(buf, 1, 4, read);
    puts(buf);

    if(fact_handler(read, &chunk))
        puts("failed 3");

    fread(buf, 1, 4, read);
    puts(buf);

    if(bext_handler(read, &chunk))
        puts("failed 4");

    fread(buf, 1, 4, read);
    puts(buf);

    if(acid_handler(read, &chunk))
        puts("failed 5");

    fread(buf, 1, 4, read);
    puts(buf);

    if(data_handler(read, &chunk))
        puts("failed 6");

    fclose(read);

    return 0;
}