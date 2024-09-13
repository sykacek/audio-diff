#pragma once

#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#define FFT_BUFFER_SIZE (1 << 10)
#define FFT_OUT_FILE "fft_left.dat"
#define PI 3.14159265358979323846

/* basic print macros */
#define puti(i) printf("%ld\n", i)
#define putcf(i) printf("%f + %f i \n", crealf(i), cimagf(i))

#define fft_coefd(i) cexp(-2*PI*I*i)
#define fft_coeff(i) cexpf(-2*PI*I*i)


/* return non zero size of array*/
size_t size(char *__arr, size_t __size);

/* returns index of maximum value of array */
size_t max(int *__arr);

/* returns index of min value of array */
size_t min(int *__arr);

/* convert bytes to integer (little endian) */
unsigned long btoi(char *__src, long __size);

/* basic O(n2) DFT from unsigned int to complex */
void dft_uint_complex(uint32_t *__src, double complex *__dest, size_t __size, uint8_t __bitsPerSample);

/* DFT from unsigned int data to logarthmic double precision floating point magnitudes */
void dft_uint_double(uint32_t *__src, double *__dest, size_t __size, uint8_t __bitsPerSample);

/* radix 2 FFT from unsigned int to complex */
void fft_cooley(double complex *__src, size_t __size, size_t __step);
