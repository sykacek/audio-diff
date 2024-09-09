#pragma once

#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#define FFT_BUFFER_SIZE (1 << 12)
#define FFT_OUT_FILE "fft_left.dat"

/* return non zero size of array*/
size_t size(char *__arr, size_t __size);

/* returns index of maximum value of array */
size_t max(int *__arr);

/* returns index of min value of array */
size_t min(int *__arr);

/* convert bytes to integer (little endian) */
unsigned long btoi(char *__src, long __size);

/* impelemt radix 2 fft */
void fft(int *__src, int *__dest, size_t __size);