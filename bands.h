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

extern float THIRD_OCTAVE_FREQUENCY[THIRD_OCTAVE_BANDS];
extern float OCTAVE_FREQUENCY[OCTAVE_BANDS];
extern float third_weight_A[THIRD_OCTAVE_BANDS];

/* continous frequency response to octave filter */
int freq_to_octave(double *__octave, double *__freq, double __step);

/* third octave filter */
int freq_to_third_octave(double *__octave, double *__freq, double __step);

/* octave weight adjustment using standard filters specified with __filter param */
int octave_weight_adjustment(double *__octave, char __filter);

/* third octave weight adjustment using standard filters specified with __filetr param */
int third_weight_adjustment(double *__octave, char __filter);