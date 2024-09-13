#include "audio_headers.h"

ck_t *chunks_init(void)
{
    ck_t *__ck = (ck_t *)malloc(sizeof(ck_t));
    __ck->acid = NULL;
    __ck->bext = NULL;
    __ck->riff = NULL;
    __ck->fmt = NULL;
    __ck->junk = NULL;
    __ck->fact = NULL;
    __ck->data = NULL;
    __ck->wave = NULL;

    return __ck;
}

void chunks_free(ck_t *__chunks)
{
    /* free all heap (malloc) memory */
    free(__chunks->acid);
    free(__chunks->bext->byte);
    free(__chunks->bext);
    free(__chunks->data->buffer);
    free(__chunks->data->fftBuffer);
    free(__chunks->data);
    free(__chunks->fact);
    free(__chunks->fmt);
    free(__chunks->junk);
    free(__chunks->riff);
    free(__chunks->wave);
    free(__chunks);
}

int junk_handler(FILE *__file, ck_t *__chunks)
{
    junk_ck_t *__junk= (junk_ck_t *)malloc(sizeof(junk_ck_t));
    if(__junk == NULL)
        return ENOMEM;

    __chunks->junk = __junk;
    char buf[4] = {0};

    if(!fread(buf, 1, 4, __file)){
        free(__junk);
        return -1;
    }

    uint _len = btoi(buf, 4);
    for(uint i = 0; i < _len; ++i)
        fread(buf, 1, 1, __file);

    __junk->blockSize = _len;    

    return 0;
}

int wave_handler(FILE *__file, ck_t *__chunks)
{
    wave_ck_t *__wave = (wave_ck_t *)malloc(sizeof(wave_ck_t));
    if(__wave == NULL)
        return ENOMEM;

    __chunks ->wave = __wave;
    __wave->wasRead = 1;
    return 0;
}

int riff_handler(FILE *__file, ck_t *__chunks)
{
    riff_ck_t *__riff = (riff_ck_t *)malloc(sizeof(riff_ck_t));
    if(__riff == NULL)
        return ENOMEM;

    if(!fread(__riff, sizeof(riff_ck_t), 1, __file)){
        free(__riff);
        return -1;
    }

    __chunks->riff = __riff;
    return 0;
}

int acid_handler(FILE *__file, ck_t *__chunks)
{
    acid_ck_t *__acid = (acid_ck_t *)malloc(sizeof(acid_ck_t));
    if(__acid == NULL)
        return ENOMEM;

    if(!fread(__acid, sizeof(acid_ck_t), 1, __file)){
        free(__acid);
        return -1;
    }

    __chunks->acid = __acid;
    return 0;
}

int bext_handler(FILE *__file, ck_t *__chunks)
{
    bext_ck_t *__bext = (bext_ck_t *)malloc(sizeof(bext_ck_t));
    if(__bext == NULL)
        return ENOMEM;

    char buf[4];
    if(!fread(buf, 1, 4, __file)){
        free(__bext);
        return -1;
    }

    uint len = btoi(buf, 4);
    if(!len){
        free(__bext);
        return -1;
    }

    __bext->byte = (char *)malloc(len * sizeof(char));
    __bext->blockSize = len;
    __chunks->bext = __bext;

    fread(__bext->byte, len, 1, __file);

    return 0;
}

int fact_handler(FILE *__file, ck_t *__chunks)
{
    fact_ck_t *__fact = (fact_ck_t *)malloc(sizeof(fact_ck_t));
    if(__fact == NULL)
        return ENOMEM;

    if(!fread(__fact, sizeof(fact_ck_t), 1, __file)){
        free(__fact);
        return -1;
    }

    __chunks->fact = __fact;
    return 0;
}

int fmt_handler(FILE *__file, ck_t *__chunks)
{
    fmt_ck_t *__fmt = (fmt_ck_t *)malloc(sizeof(fmt_ck_t));
    if(__fmt == NULL)
        return ENOMEM;

    if(!fread(__fmt, sizeof(fmt_ck_t), 1, __file)){
        free(__fmt);
        return -1;
    }

    __chunks->fmt = __fmt;
    return 0;
}

int data_handler(FILE *__file, ck_t *__chunks)
{
    data_ck_t *__data = (data_ck_t *)malloc(sizeof(data_ck_t));
    if(__data == NULL)
        return ENOMEM;

    uint *__buffer = (uint *)malloc(sizeof(uint)*FFT_BUFFER_SIZE);
    if(__buffer == NULL)
        return ENOMEM;
    

    memset(__buffer, 0, sizeof(uint) * FFT_BUFFER_SIZE);

    double complex *__fftBuffer = (double complex*)malloc(sizeof(double complex)*FFT_BUFFER_SIZE);
    if(__fftBuffer == NULL)
        return ENOMEM;

    memset(__fftBuffer, 0, sizeof(double complex) * FFT_BUFFER_SIZE);

    /* read only blockSize*/
    if(!fread(__data, sizeof(uint), 1, __file)){
        free(__data);
        free(__buffer);
        free(__fftBuffer);
        return -1;
    }

    __chunks->data = __data;
    __data->buffer = __buffer;
    __data->fftBuffer = __fftBuffer;
    __data->buffersRead = 0;
    if(__chunks->fmt == NULL){
        fprintf(stderr, "Error: failed to read format before\n");
        return -1;
    }

    char buf[4];

    memset(__chunks->data->buffer, 0, FFT_BUFFER_SIZE * sizeof(uint));
    for(uint i = 0; i < FFT_BUFFER_SIZE; ++i){
        fread(buf, 1, 4, __file);
        *(__chunks->data->buffer + i) = btoi(buf, 4);
    }

    //dft_uint_complex(__chunks->data->buffer, __chunks->data->fftBuffer, FFT_BUFFER_SIZE, __chunks->fmt->bitsPerSample - 1);

    itodc(__chunks->data->fftBuffer, __chunks->data, FFT_BUFFER_SIZE, __chunks->fmt->bitsPerSample - 1);
    fft_cooley(__chunks->data->fftBuffer, FFT_BUFFER_SIZE, 1);
    __chunks->data->buffersRead++;

    /* read data and perform DFT */
    while(!feof(__file)){
        memset(__chunks->data->buffer, 0, FFT_BUFFER_SIZE * sizeof(uint));
        for(uint i = 0; i < FFT_BUFFER_SIZE; ++i){
            fread(buf, 1, 4, __file);
            *(__chunks->data->buffer + i) = btoi(buf, 4);
        }

        //fft_uint_complex(__chunks->data->buffer, __chunks->data->fftBuffer, FFT_BUFFER_SIZE, __chunks->fmt->bitsPerSample - 1);
        __chunks->data->buffersRead++;
    }

    return 0;
}
