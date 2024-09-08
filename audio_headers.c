#include "audio_headers.h"

ck_t *chunks_init(void)
{
    return (ck_t *)malloc(sizeof(ck_t));
}

void chunks_free(ck_t *__chunks)
{
    free(__chunks->acid);
    free(__chunks->bext->byte);
    free(__chunks->bext);
    free(__chunks->data);
    free(__chunks->fact);
    free(__chunks->fmt);
    free(__chunks->junk);
    free(__chunks->riff);
    return free(__chunks);
}

int junk_handler(FILE *__file, ck_t *__chunks)
{
    junk_ck_t *__junk= (junk_ck_t *)malloc(sizeof(junk_ck_t));
    if(__junk == NULL)
        return -1;

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

int riff_handler(FILE *__file, ck_t *__chunks)
{
    riff_ck_t *__riff = (riff_ck_t *)malloc(sizeof(riff_ck_t));
    if(__riff == NULL)
        return -1;

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
        return -1;

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
        return -1;

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
        return -1;

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
        return -1;

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
        return -1;

    if(!fread(__data, sizeof(data_ck_t), 1, __file)){
        free(__data);
        return -1;
    }

    __chunks->data = __data;
    if(__chunks->fmt == NULL){
        fprintf(stderr, "Error: format failed to read");
        return -1;
    }

    char buf[4];

    for(uint i = 0; i < __chunks->data->blockSize; ++i)
        fread(buf, 1, 1, __file);

    return 0;
}
