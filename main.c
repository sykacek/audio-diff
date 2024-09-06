#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "fft.h"
#include "audio_headers.h"

#define puti(a) printf("%x\n", a)

int main(int argc, char **argv){
    
    char buf[4] = {0};
    riff_ck_t riff = {"", 0};
    FILE *read;
    read = fopen("audio/master.wav", "rb");

    fread(&riff, sizeof(riff), 1, read);

    if(btoi(riff.fileFormatID, 4) != btoi(RIFF_ID, 4))
        printf("true\n");

    fclose(read);

    return 0;
}