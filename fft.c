#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <complex.h>

float pi;

int main()
{
    pi = acosf(-1);

    size_t n = 8;
    float in[n]; // n samples
    float complex out[n]; // n frequences we measure

    for (size_t i = 0; i < n; ++i) {
        float t = (float)i/n;
        // Where waves have acoustic interference
        in[i] = cosf(2*pi*t*1) + sinf(2*pi*t*2); // 1 & 2 Hertz
    }

    // for (size_t i = 0; i < n; i++) {
    //     printf("%f\n", in[i]);
    // }
    // printf("\n");

    for (size_t f = 0; f < n; ++f) {
        out[f] = 0;
        for (size_t i = 0; i < n; ++i) {
            float t= (float)i/n;
            // If the frequence is part of the input wave,
            // then the output should be bigger than 0
            out[f] += in[i]*cexp(2*I*pi*f*t);
        }
    }

    for (size_t f = 0; f < n; f++) {
        printf("%02zu: %.2f, %.2f\n", f, creal(out[f]), cimag(out[f]));
    }

    return 0;
}
