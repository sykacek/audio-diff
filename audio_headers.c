#include "audio_headers.h"

ck_t *chunks_init(settings_t *__settings)
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
    __ck->settings = __settings;

    return __ck;
}

void chunks_free(ck_t *__chunks)
{
    if(__chunks == NULL) return;

    /* free all heap allocated memory */
    if(__chunks->acid != NULL)
        free(__chunks->acid);

    if(__chunks->bext != NULL){
        free(__chunks->bext->byte);
        free(__chunks->bext);
    }
    if(__chunks->data != NULL){
        free(__chunks->data->buffer);
        free(__chunks->data->fftBuffer);
        free(__chunks->data->logBuffer);
        free(__chunks->data->octaveBuffer);
        free(__chunks->data);
    }
    if(__chunks->fact != NULL)        
        free(__chunks->fact);   
    
    if(__chunks->fmt != NULL)
        free(__chunks->fmt);

    if(__chunks->junk != NULL)
        free(__chunks->junk);

    if(__chunks->riff != NULL)
        free(__chunks->riff);
    
    if(__chunks->wave != NULL)
        free(__chunks->wave);

    free(__chunks);
}

size_t buffer_read(FILE *__file, ck_t *__chunks, size_t __elements)
{
    size_t i = 0;

    char cbuf[FMT_MAX_CHANNELS * sizeof(int)] = {0};

    for(; i < __elements; i++){
        memset(cbuf, 0, sizeof(int) * FMT_MAX_CHANNELS);
        if(!fread(cbuf, __chunks->fmt->bitsPerSample / 8, __chunks->fmt->nChannels, __file))
            break;

        *(__chunks->data->buffer + i) = btoi(cbuf, __chunks->fmt->bitsPerSample / 8);
    }

    return i;
}

size_t buffer_copy(ck_t *__chunks, size_t __size)
{
    size_t i = 0;

    for(; i < __size; ++i)
        __chunks->data->logBuffer[i] = ((cabs(__chunks->data->fftBuffer[i])) \
        + (__chunks->data->buffersRead - 1) * (__chunks->data->logBuffer[i]) ) / __chunks->data->buffersRead;

    return i;
}

size_t buffer_log(ck_t *__chunks, size_t __size)
{
    size_t i = 0;
    for(; i < __size; ++i)
        __chunks->data->logBuffer[i] = 20*log10(__chunks->data->logBuffer[i]);

    return i;
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
    /* allocate buffers */
    data_ck_t *__data = (data_ck_t *)malloc(sizeof(data_ck_t));
    if(__data == NULL)
        return ENOMEM;

    __chunks->data = __data;

    int *__buffer = (int *)malloc(sizeof(uint)*FFT_BUFFER_SIZE);
    if(__buffer == NULL)
        return ENOMEM;
    
    memset(__buffer, 0, sizeof(uint) * FFT_BUFFER_SIZE);
    __chunks->data->buffer = __buffer;

    double complex *__fftBuffer = (double complex*)malloc(sizeof(double complex)*FFT_BUFFER_SIZE);
    if(__fftBuffer == NULL)
        return ENOMEM;

    memset(__fftBuffer, 0, sizeof(double complex) * FFT_BUFFER_SIZE);
    __chunks->data->fftBuffer = __fftBuffer;

    double *__logBuffer = (double *)malloc(sizeof(double) * FFT_BUFFER_SIZE);
    if(__logBuffer == NULL)
        return ENOMEM;

    memset(__logBuffer, 0, FFT_BUFFER_SIZE * sizeof(double));
    __chunks->data->logBuffer = __logBuffer;

    double *__octaveBuffer = (double *)malloc(sizeof(double) * THIRD_OCTAVE_BANDS);
    __chunks->data->octaveBufferSize = 0;
    if(__octaveBuffer == NULL)
        return ENOMEM;

    memset(__octaveBuffer, 0, sizeof(double) * THIRD_OCTAVE_BANDS);
    __chunks->data->octaveBuffer = __octaveBuffer;
    __chunks->data->octaveBufferSize = THIRD_OCTAVE_BANDS;

    /* read only blockSize*/
    if(!fread(__data, sizeof(uint), 1, __file)){
        free(__data);
        free(__buffer);
        free(__fftBuffer);
        return -1;
    }

    __data->buffersRead = 0;

    if(__chunks->fmt == NULL){
        fprintf(stderr, "Error: failed to read format before\n");
        return -1;
    }

    /* read data and perform DFT */
    while(!feof(__file)){
        /* read int data (PCM) and convert to double complex*/
        memset(__chunks->data->buffer, 0, FFT_BUFFER_SIZE * sizeof(int));

        /* procced only with full buffers */
        if(buffer_read(__file, __chunks, FFT_BUFFER_SIZE) != FFT_BUFFER_SIZE)
            break;
        
        itodc(__chunks->data->fftBuffer, __chunks->data->buffer, FFT_BUFFER_SIZE, __chunks->fmt->bitsPerSample - 1);

        /* perform fft on complex data */
        fft_cooley(__chunks->data->fftBuffer, FFT_BUFFER_SIZE);
        __chunks->data->buffersRead++;

        if(!buffer_copy(__chunks, FFT_BUFFER_SIZE))
            return 1;
    }

    if(!buffer_log(__chunks, FFT_BUFFER_SIZE))
        return 1;

    return 0;
}

int generate_octave(ck_t *__chunks)
{
    if(__chunks == NULL)
        return 1;

    if(__chunks->data == NULL)
        return 1;

    freq_to_octave(__chunks->data->octaveBuffer, __chunks->data->logBuffer, __chunks->fmt->frequency / FFT_BUFFER_SIZE);

    double maxd = max(__chunks->data->octaveBuffer, __chunks->data->octaveBufferSize);
    if(!maxd)
        return 1;

    for(uint i = 0; i < __chunks->data->octaveBufferSize; ++i)
        __chunks->data->octaveBuffer[i] -= maxd;
    
    return 0;
}

int generate_third_octave(ck_t *__chunks)
{
    if(__chunks == NULL)
        return 1;

    if(__chunks->data == NULL)
        return 1;

    freq_to_third_octave(__chunks->data->octaveBuffer, __chunks->data->logBuffer, __chunks->fmt->frequency / FFT_BUFFER_SIZE);

    double maxd = max(__chunks->data->octaveBuffer, __chunks->data->octaveBufferSize);
    if(!maxd)
        return 1;

    for(uint i = 0; i < __chunks->data->octaveBufferSize; ++i)
        __chunks->data->octaveBuffer[i] -= maxd;

    return 0;
}

int write_octave(FILE *__file, ck_t *__chunks)
{
    if(__file == NULL || __chunks == NULL)
        return 1;

    float *p = NULL;
    if(__chunks->data->octaveBufferSize == OCTAVE_BANDS)
        p = OCTAVE_FREQUENCY;
    else if(__chunks->data->octaveBufferSize == THIRD_OCTAVE_BANDS)
        p = THIRD_OCTAVE_FREQUENCY;

    for(uint i = 0; i < __chunks->data->octaveBufferSize; ++i)
        fprintf(__file, "%f\t%lf\n", p[i], __chunks->data->octaveBuffer[i]);


    return 0;
}