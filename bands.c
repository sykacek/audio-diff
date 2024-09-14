#include "bands.h"

/* third octave nominal frequency table (rounded) */
const float THIRD_OCTAVE_FREQUENCY[THIRD_OCTAVE_BANDS] = {
    25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, \
    500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, \
    6300, 8000, 10000, 12500, 16000, 20000
};

/* octave nominal frequency table (rounded)*/
const float OCTAVE_FREQUENCY[OCTAVE_BANDS] = {
    31.5, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000
};

/* A weight filter */
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


int freq_to_octave(double *__octave, double *__freq, double __step)
{
    uint32_t i = 0, j = 0;
    double freq = OCTAVE_BASE, step = __step;

    if(memset(__octave, 0, OCTAVE_BANDS * sizeof(double)) == NULL)
        return 1;

    /* ignore values outside the bands */
    while(i * step < octave_low(freq))
        i++;
    
    while(*(__freq + i) && j < OCTAVE_BANDS){
        while(i * step < octave_high(freq)){
            *(__octave + j) += *(__freq + i);
            i++;
        }

        freq = octave_nominal_next(freq);
        j++;
    } 
    return j ? 0 : 1;
}

int freq_to_third_octave(double *__octave, double *__freq, double __step)
{
    int i = 0, j = 0;
    double freq = THIRD_OCTAVE_BASE, step = __step;

    if(memset(__octave, 0, sizeof(double) * THIRD_OCTAVE_BANDS) == NULL)
        return 1;

    /* ignore values outside the bands */
    while(i * step < third_low(freq))
        i++;

    while(*(__freq + i) && j < THIRD_OCTAVE_BANDS){
        while(i * step < third_high(freq)){
            *(__octave + j) += *(__freq + i);
            i++;
        }

        freq = third_nominal_next(freq);
        j++;
    }

    return j ? 0 : 1;
}
