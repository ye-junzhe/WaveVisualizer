#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <complex.h>
#include <assert.h>
#include "plug.h"

// NOTE: DFT
void dft(float in[], float complex out[], size_t n) {
    for (size_t f = 0; f < n; ++f) {
        out[f] = 0;
        for (size_t i = 0; i < n; ++i) {
            float t= (float)i/n;
            // If the frequence is part of the input wave,
            // then the output should be upper 0
            out[f] += in[i]*cexp(2*I*PI*f*t);
        }
    }
}

// NOTE: FFT
void fft(float in[], size_t stride, float complex out[], size_t n) {

    assert(n > 0);
    if (n == 1) {
        out[0] = in[0];
        return;
    }

    fft(in, stride*2, out, n/2);
    fft(in + stride, stride*2, out + n/2, n/2);

    for (size_t k = 0; k < n/2; ++k) {
        float t = (float)k/n;
        float complex v = cexp(-2*I*PI*t)*out[k + n/2];
        float complex e = out[k];
        out[k]       = e + v;
        out[k + n/2] = e - v;
    }
}


// int main()
// {
//     pi = acosf(-1);
//
//     size_t n = 8;
//     float in[n]; // n samples
//     float complex out[n]; // n frequences we measure
//
//     for (size_t i = 0; i < n; ++i) {
//         float t = (float)i/n;
//         // Where waves have acoustic interference
//         in[i] = cosf(2*pi*t*1) + sinf(2*pi*t*2); // 1 & 2 Hertz
//     }
//
//     // dft(in, out, n);
//     fft(in, 1, out, n);
//
// // WARNING:
// #if 0
//
//     /* NOTE: FFT
//         * * * * * * * * | * * * * * * * *
//         In FFT we only consider the first half of the n frequences
//         Due to the periodicity of the sin and cos waves:
//             All of the odd&even frequences have the same value at some point
//             For each pair of frequences, the absolute value are the same
//      */
//
//     for (size_t f = 0; f < n/2; ++f) {
//         out[f] = 0;
//         out[f + n/2] = 0;
//         // even
//         for (size_t i = 0; i < n; i += 2) {
//             float t = (float)i/n;
//             float complex v = in[i]*cexp(2*I*pi*t*f);
//             out[f] += v;
//             out[f + n/2] += v;
//         }
//         // odd
//         for (size_t i = 1; i < n; i += 2) {
//             float t = (float)i/n;
//             float complex v = in[i]*cexp(2*I*pi*t*f);
//             out[f] += v;
//             out[f + n/2] -= v;
//         }
//     }
//
//     /* NOTE: List the samples for each frequences
//
//     size_t f = 1;
//     // f += n/2;
//     printf("%02zu:", f);
//     for (size_t i = 1; i < n; i += 2) {
//         float t = (float)i/n;
//         printf(" %5.2f", sinf(2*pi*t*f));
//     }
//     printf("\n");
//     */
//
//     /* NOTE: List the input sample
//         for (size_t i = 0; i < n; i++) {
//             printf("%f\n", in[i]);
//         }
//         printf("\n");
//     */
//
// #endif
//
//     for (size_t f = 0; f < n; f++) {
//         printf("%02zu: %.2f, %.2f\n", f, creal(out[f]), cimag(out[f]));
//     }
//
//     return 0;
// }
