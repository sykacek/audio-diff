#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

#include "fft.h"
#include "audio_headers.h"

#define puti(a) printf("%lx\n", a)

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
    puts(buf);

    switch(btoi(buf, 4)){
    case(DATA_INT):{

        break;
    }
    case(FMT_INT):{

        break;
    }
    case(JUNK_INT):{
        junk_ck_t junk = {0};
        fread(buf, 1, 4, read);
        junk.blockSize = btoi(buf, 4);

        for(uint i = 0; i < junk.blockSize; ++i)
            fread(buf, 1, 1, read);

        puts("read junk");
        chunk.junk = &junk;

        break;
    }
    }

    fclose(read);

    return 0;
}