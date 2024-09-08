#pragma once

#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "fft.h"

/* define common chunk ID names */
#define DATA_ID "data"
#define RIFF_ID "RIFF"
#define WAVE_ID "WAVE"
#define FMT_ID "fmt"
#define JUNK_ID "JUNK"
#define BEXT_ID "bext"
#define FACT_ID "fact"
#define ACID_ID "acid"

/* ASCII to int constants (btoi generated)*/
#define DATA_INT (0x61746164)   // inverted ASCII data
#define RIFF_INT (0x46464952)   // inverted ASCII RIFF
#define WAVE_INT (0x45564157)   // inverted ASCII WAVE
#define FMT_INT  (0x746d66)     // inverted ASCII fmt
#define JUNK_INT (0x4b4e554a)   // inverted ASCII JUNK
#define BEXT_INT (0x74786562)   // inverted ASCII bext
#define FACT_INT (0x74636166)   // inverted ASCII fact
#define ACID_INT (0x64696361)   // inverted ASCII acid

typedef uint32_t uint;

/* chunks data structures */
typedef struct {
    char fileFormatID[4];
    uint fileSize;
} riff_ck_t;

typedef struct {
    uint blockSize;
    uint16_t audioFormat;
    uint16_t nChannels;
    uint frequency;
    uint bytePerSec;
    uint16_t bytePerBlock;
    uint16_t bitsPerSample;
} fmt_ck_t;

typedef struct {
    uint blockSize;
} junk_ck_t;

typedef struct {
    uint32_t sampleCount;
    char byte[4];
} fact_ck_t;

typedef struct {
    uint32_t blockSize;
    char *byte; //mallocated
} bext_ck_t;

typedef struct {
    uint32_t blockSize;
    char byte[24];
} acid_ck_t;

typedef struct {
    uint blockSize;
} data_ck_t;

typedef struct {
    riff_ck_t *riff;    //mallocated
    fmt_ck_t *fmt;      //mallocated
    junk_ck_t *junk;    //mallocated
    fact_ck_t *fact;    //mallocated
    acid_ck_t *acid;    //mallocated
    data_ck_t *data;    //mallocated
    bext_ck_t *bext;    //mallocated
} ck_t;

ck_t *chunks_init(void);
void chunks_free(ck_t *__chunks);

int junk_handler(FILE *__file, ck_t *__chunks);
int riff_handler(FILE *__file, ck_t *__chunks);
int acid_handler(FILE *__file, ck_t *__chunks);
int bext_handler(FILE *__file, ck_t *__chunks);
int fact_handler(FILE *__file, ck_t *__chunks);
int fmt_handler(FILE *__file, ck_t *__chunks);
int data_handler(FILE *__file, ck_t *__chunks);