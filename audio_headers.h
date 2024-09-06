#pragma once

#include <stdint.h>
#include <inttypes.h>

/* define common chunk ID names */
#define DATA_ID "data"
#define RIFF_ID "RIFF"
#define WAVE_ID "WAVE"
#define FMT_ID "fmt"
#define JUNK_ID "JUNK"
#define BEXT_ID "bext"
#define FACT_ID "fact"
#define ACID_ID "acid"

/* ASCII to int constants (got from btoi func)*/
#define DATA_INT (0x61746164) // ASCII atad
#define RIFF_INT (0x46464952) //ASCII FFIR
#define WAVE_INT (0x45564157) //ASCII EVAW
#define FMT_INT  (0x746d66) //ASCII tmf
#define JUNK_INT (0x4b4e554a) //ASCII KNUJ
#define BEXT_INT (0x)
#define FACT_INT (0x)
#define ACID_INT (0x)

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
    uint64_t sampleCount;
} fact_ck_t;

typedef struct {
    uint32_t blockSize;
} bext_ck_t;

typedef struct {

    ;
} acid_ck_t;

typedef struct {
    uint blockSize;
} data_ck_t;

typedef struct {
    riff_ck_t *riff;
    fmt_ck_t *fmt;
    junk_ck_t *junk;
    fact_ck_t *fact;
    acid_ck_t *acid;
    data_ck_t *data;
    bext_ck_t *bext;
} ck_t;