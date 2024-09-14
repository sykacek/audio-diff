#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

#include "fft.h"
#include "audio_headers.h"
#include "bands.h"

/* basic formated print macros */
#define putx(a) printf("%lx\n", a)

#ifndef puti
#define puti(a) printf("%ld\n", a)
#endif

#define pftell(__stream) printf("%lx\n", ftell(__stream))

int main(int argc, char **argv){
    char buf[4] = {0};
    ck_t * chunk = chunks_init();
    FILE *read;

    read = fopen("audio/440hz16bit.wav", "rb");

    if(read == NULL){
        fprintf(stderr, "Error, failed to open file\n");
        return EBADFD;
    }

    while(!feof(read)){
        fread(buf, 1, 4, read);
        switch(btoi(buf, 4)){
        case(DATA_INT):{
            data_handler(read, chunk);
            puts("data read");
            break;
        }
        case(RIFF_INT):{
            riff_handler(read, chunk);
            puts("riff read");
            break;
        }
        case(FMT_INT):{
            fmt_handler(read, chunk);
            puts("fmt read");
            break;
        }
        case(JUNK_INT):{
            junk_handler(read, chunk);
            puts("junk handler");
            break;
        }
        case(BEXT_INT):{
            bext_handler(read, chunk);
            puts("bext read");
            break;
        }
        case(FACT_INT):{
            fact_handler(read, chunk);
            puts("fact read");            
            break;
        }
        case(ACID_INT):{
            acid_handler(read, chunk);
            puts("acid read");
            break;
        }
        case(WAVE_INT):{
            wave_handler(read, chunk);
            puts("wave read");
            break;
        }
        default:{
            fprintf(stderr, "Error: unknown chunk %s\n", buf);
            putx(btoi(buf, 4));

            /* ignore chunk */
            junk_handler(read, chunk);
            break;
        }
        }
    pftell(read);
    }

    double third[OCTAVE_BANDS] = {0};
    freq_to_octave(third, chunk->data->fftBuffer, chunk->fmt->frequency / FFT_BUFFER_SIZE);

    FILE *w = fopen("out.txt", "w");
    if(w == NULL){
        fprintf(stderr, "Error: failed to open output file\n");
        return EBADFD;
    }

    for(int i = 0; i < FFT_BUFFER_SIZE / 2 /*&& i * chunk->fmt->frequency / FFT_BUFFER_SIZE < 20000*/; ++i){
        //printf("%d %lf\n", i * chunk->fmt->frequency / FFT_BUFFER_SIZE, cabs(chunk->data->logBuffer[i]));
        fprintf(w, "%d %lf\n", i * chunk->fmt->frequency / FFT_BUFFER_SIZE, cabs(chunk->data->logBuffer[i]));
    }

    for(int i = 0; i  < OCTAVE_BANDS; ++i)
        printf("%lf\t%lf\n", OCTAVE_FREQUENCY[i], third[i]);

    fclose(read);
    fclose(w);
    chunks_free(chunk);
    return 0;
}
