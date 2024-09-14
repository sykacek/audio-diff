#pragma once

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <complex.h>

/* calibration */
#define OCTAVE_BASE         (31.25) //Hz
#define OCTAVE_BANDS        (10)

#define THIRD_OCTAVE_BASE   (25) //Hz
#define THIRD_OCTAVE_BANDS  (30)

#define SQRT2_1             (1.414213562)
#define SQRT2_3             (1.25992105)
#define SQRT2_6             (1.122462048)

/* band boundaries calculation */
#define octave_low(__freq) (__freq / SQRT2_1)
#define octave_high(__freq) (__freq * SQRT2_1)
#define octave_nominal_next(__freq) (__freq * 2)

#define third_low(__freq) (__freq / SQRT2_6)
#define third_high(__freq) (__freq * SQRT2_6)
#define third_nominal_next(__freq) (__freq * SQRT2_3)

extern const float THIRD_OCTAVE_FREQUENCY[THIRD_OCTAVE_BANDS];
extern const float OCTAVE_FREQUENCY[OCTAVE_BANDS];
extern const double third_weight_A[THIRD_OCTAVE_BANDS];

/* third octave nominal frequency table (rounded) */
/*
const float THIRD_OCTAVE_FREQUENCY[THIRD_OCTAVE_BANDS] = {
    25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, \
    500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, \
    6300, 8000, 10000, 12500, 16000, 20000
};

/* octave nominal frequency table (rounded)*//*
const float OCTAVE_FREQUENCY[OCTAVE_BANDS] = {
    31.5, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000
};

/* A weight filter *//*
const double third_weight_A[THIRD_OCTAVE_BANDS] = {
    -44.8,
    -39.5,
    -34.5,
    -30.3,
    -26.2,
    -22.4,
    -19.1,
    -16.2,
    -13.2,
    -10.8,
    -8.7,
    -6.6,
    -4.8,
    -3.2,
    -1.9,
    -0.8,
    0.0,
    0.6,
    1.0,
    1.2,
    1.3,
    1.2,
    1.0,
    0.6,
    -0.1,
    -1.1,
    -2.5,
    -4.3
    -6.7,
    -9.3
};

/* continous frequency response to octave filter */
int freq_to_octave(double *__octave, double complex *__freq, double __step);

/* third octave filter */
int freq_to_third_octave(double *__octave, double complex *__freq, double __step);

/* octave weight adjustment using standard filters specified with __filter param */
int octave_weight_adjustment(double *__octave, char __filter);

/* third octave weight adjustment using standard filters specified with __filetr param */
int third_weight_adjustment(double *__octave, char __filter);