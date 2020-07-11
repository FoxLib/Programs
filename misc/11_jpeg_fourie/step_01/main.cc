#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "sdl.class.h"

#define N 8.0
#define C(n) ((n == 0) ? 1.0 / sqrt(N) : sqrt(2.0 / N))

// Преобразование в волные коэффициенты
void DCT1(double v[], double t[]) {

    double dk = M_PI/(2.0*N);
    for (int i = 0; i < N; i++) {

        double sum = 0.0;
        for (int n = 0; n < N; n++) {

            double tm = cos((2.0*(double)n + 1.0)*(double)i*dk);
            sum += v[n] * tm; 
        }

        t[i] = C(i) * sum;
    }
}

// Преобразование из волновых коэффициентов в изображение
void IDCT1(double v[], double t[]) {

    double dk = M_PI/(2.0*N);
    for (int i = 0; i < N; i++) {

        double sum = 0.0;
        for (int n = 0; n < N; n++) {

            double tm = cos((2.0*(double)i + 1.0)*(double)n*dk);
            sum += C(n) * t[n] * tm;
        }

        v[i] = sum;
    }
}

int main(int argc, char* argv[]) {

    /*
    SDLClass main(2*640, 2*480, "Hello World");
    for (int i = 0; i < 480; i++) for (int j = 0; j < 640; j++)
        main.pset(j,i,i*j);

    main.flip();
    main.loop();
    */

    // Входящее изображение 8x1
    double V[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    double T[8];
    int i;

    DCT1 (V, T);  // V->T
    // for (i = 4; i < 8; i++) T[i] = 0.0;
    IDCT1(V, T);  // T->V

    // 0    1    2    3    4    5    6    7
    // 0.06 0.88 2.04 3.05 3.94 4.95 6.11 6.93

    for (i = 0; i < N; i++) { printf("%f ", T[i]); } printf("\n");
    for (i = 0; i < N; i++) { printf("%f ", V[i]); } printf("\n");

    return 0;
}
