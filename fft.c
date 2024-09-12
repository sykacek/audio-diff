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

    for(int i = 0; i < len; ++i){
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

    for(int i = 0; i < len; ++i){
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

void fft(uint32_t *__src, double complex *__dest, size_t __size, uint8_t __bitsPerSample)
{
    memset(__dest, 0, __size * sizeof(double complex));
    double complex temp;

    for(int i = 0; i < __size; ++i){
        temp = 0;
        for(int j = 0; j < __size; ++j){
            temp += ((double)*(__src + j) / (1 << __bitsPerSample) + 0 * I) * fft_coefd(i * j / __size);
        }
        *(__dest + i) = temp;
    }
}
