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

    printf("%d\n", settings->frequency);

    putx(settings->param);

    ck_t * chunk = chunks_init(settings);
    if(chunk == NULL){
        printf("Error: ran out of memomy\n");
        return ENOMEM;
    }

    FILE *read;
    read = fopen("audio/440hz16bit.wav", "rb");
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

    FILE *w = fopen("out.txt", "w");
    if(w == NULL){
        fprintf(stderr, "Error: failed to open output file\n");
        return EBADFD;
    }

    /* write results */
    generate_third_octave(chunk);
    write_octave(w, chunk);

    /* close file streams */
    fclose(read);
    fclose(w);

    /* free data structures */
    chunks_free(chunk);
    settings_free(settings);
    return 0;
}
