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
#define putlx(a) printf("%lx\n", a)

#ifndef puti
#define puti(a) printf("%ld\n", a)
#endif

#define pftell(__stream) printf("%lx\n", ftell(__stream))

int main(int argc, char **argv){
    settings_t * settings = settings_init(argc, argv);
    if(settings == NULL){
        settings_help();
        return 0;
    }

    if(settings_apply(settings, argc, argv)){
        settings_einval();
        return -EINVAL;
    }

    if(!settings->param){
        settings_einval();
        return -EINVAL;
    }

    int size = 0;
    if(is_flag(settings->param, ST_OCTAVE))
        size = OCTAVE_BANDS;
    else
        size = THIRD_OCTAVE_BANDS;

    double octaveBuffer[2][size];
    memset(octaveBuffer[0], 0, 2 * size * sizeof(double));

    for(int i = 0; i < 2; ++i){
        ck_t * chunk = chunks_init(settings);
        if(chunk == NULL){
            printf("Error: ran out of memomy\n");
            return ENOMEM;
        }

        FILE *read;
        read = fopen(argv[argc - 2 + i], "rb");
        if(read == NULL){
            fprintf(stderr, "Error, failed to open file\n");
            return EBADFD;
        }

        char buf[4] = {0};
        while(!feof(read)){
            fread(buf, 1, 4, read);
            switch(btoi(buf, 4)){
            case(DATA_INT):{
                data_handler(read, chunk);
                break;
            }
            case(RIFF_INT):{
                riff_handler(read, chunk);
                break;
            }
            case(FMT_INT):{
                fmt_handler(read, chunk);
                break;
            }
            case(JUNK_INT):{
                junk_handler(read, chunk);
                break;
            }
            case(BEXT_INT):{
                bext_handler(read, chunk);
                break;
            }
            case(FACT_INT):{
                fact_handler(read, chunk);
                break;
            }
            case(ACID_INT):{
                acid_handler(read, chunk);
                break;
            }
            case(WAVE_INT):{
                wave_handler(read, chunk);
                puts("wave read");
                break;
            }
            default:{
                fprintf(stderr, "Error: unknown chunk %lx\n", btoi(buf, 4));

                /* ignore chunk */
                junk_handler(read, chunk);
                break;
            }
            }
        }

        /* get and copy the results */
        if(is_flag(settings->param, ST_OCTAVE))
            generate_octave(chunk);
        else 
            generate_third_octave(chunk);
        memcpy(octaveBuffer[i], chunk->data->octaveBuffer, size * sizeof(double));

        /* free data structures */
        fclose(read);
        chunks_free(chunk);
    }

    char out_file[24] = {0};
    if(is_flag(settings->param, ST_OUTPUT))
        memcpy(out_file, settings->output, 24);
    else
        memcpy(out_file, "out.txt", 8);

    FILE *w = fopen(out_file, "w");
    if(w == NULL){
        fprintf(stderr, "Error: failed to open %s\n", out_file);
        return EBADFD;
    }

    /* substarct octave values */
    for(int i = 0; i < size; ++i)
        octaveBuffer[0][i] -= octaveBuffer[1][i];

    write_octave(w, octaveBuffer[0], size);
    fclose(w);
    settings_free(settings);
    return 0;
}
