#include "fft.h"

size_t size(char *__arr, size_t __size)
{
    unsigned long i = 0;
    while(*__arr){
            i ++;
            __arr++;
        }

        return i/__size;
    }

    size_t max(int *__arr)
    {
        int cmp = INT32_MIN;
        unsigned long  j = 0, len = size((char *)__arr, sizeof(int));

        for(unsigned long i = 0; i < len; ++i){
            if(*__arr > cmp){
                j = i;
                cmp = *__arr;
            }

            __arr++;
        }

        return j;
    }

    size_t min(int *__arr)
    {
        int cmp = INT32_MAX;
        unsigned long j = 0, len = size((char *)__arr, sizeof(int));

        for(unsigned long i = 0; i < len; ++i){
            if(*__arr < cmp){
                j = i;
                cmp = *__arr;
            }

            __arr++;
        }

        return j;
    }

    unsigned long btoi(char *__src, long __size)
    {   
        unsigned long ret = 0;

        for(long i = 0; i < __size; ++i)
            ret += *(__src++) << 8 * i;
        return ret;
    }

    void *itodc(double complex *__dest, int *__src, size_t __size, uint16_t __bitsPerSample)
    {
        for(size_t i = 0; i < __size; ++i)
            *(__dest + i) = ((double)*(__src + i) / (1 << __bitsPerSample)) + 0 * I;

        return (void *)__dest;
    }

    void dft_uint_complex(int *__src, double complex *__dest, size_t __size, uint8_t __bitsPerSample)
    {
        memset(__dest, 0, __size * sizeof(double complex));
        double complex temp;

        for(size_t i = 0; i < __size; ++i){
            temp = 0 + 0*I;
            for(size_t j = 0; j < __size; ++j){
                temp += ((double)*(__src + j) / (1 << __bitsPerSample) + 0 * I) * fft_coefd(i * j / __size);
            }
            *(__dest + i) = temp;
        }
    }

    void dft_complex_complex(double complex *__src, double complex *__dest, size_t __size)
    {
        memset(__dest, 0, sizeof(double complex) * __size);
        double complex temp;

        for(size_t i = 0; i < __size; ++i){
            temp = 0 + 0 * I;
            for(size_t j = 0; j < __size; ++j)
                temp += *(__src + j) * fft_coefd(i * j / __size);
        
            *(__dest + i) = temp;
        }
    }

    void dft_uint_double(int *__src, double *__dest, size_t __size, uint8_t __bitsPerSample)
    {
        memset(__dest, 0, sizeof(double) * __size);
        double complex temp;

        for(size_t i = 0; i < __size; ++i){
            temp = 0 + 0*I;
            for(size_t j = 0; j < __size; ++j){
                temp += ((double)*(__src + j) / (1 << __bitsPerSample) + 0 * I) * fft_coefd(i * j / __size);
            }
            *(__dest + i) = 10 * log(cabs(temp));
        }
    }

    void fft_cooley(double complex *__src, size_t __size, size_t __step)
    {
        if (__size == 1) return;

       // even and odd
       fft_cooley(__src, __size / 2, __step * 2);
       fft_cooley(__src + __step, __size / 2, __step * 2);

       double complex temp = 0;

       for(size_t k = 0; k < __size / 2; ++k){
            temp = fft_coefd(k / __size) * *(__src + k * __step + __size / 2);

            *(__src + k) = *(__src + k * __step) + temp;
            *(__src + k + __size / 2) = *(__src + k * __step) - temp;
       }
    }
