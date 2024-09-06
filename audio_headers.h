#pragma once

#include <stdint.h>
#include <inttypes.h>

#define DATA_ID "data"
#define RIFF_ID "RIFF"
#define WAVE_ID "WAVE"
#define FMT_ID "fmt"
#define JUNK_ID "JUNK"

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

    char dataBlockID[4];
    uint dataSize;
} fmt_ck_t;

typedef struct {
    uint blockSize;
} junk_ck_t;

typedef struct {


} fact_ck_t;

typedef struct {


} bext_ck_t;

typedef struct {

    ;
} acid_ck_t;

typedef struct {

} data_ck_t;

typedef struct {
    riff_ck_t *riff;
    fmt_ck_t *fmt;
    junk_ck_t *junk;
    fact_ck_t *fact;
    acid_ck_t *acid;
    data_ck_t *data;
} ck_t;