#include <stdlib.h>
#include <stdio.h>

#include "sdl.class.h"
#include "jpeg.h"

int main(int argc, char* argv[]) {

    jpeg image;

    double V[8][8] = {
        {0, 1, 2, 3, 4, 5, 6, 7},
        {1, 1, 2, 3, 4, 5, 6, 7},
        {2, 1, 2, 3, 4, 5, 6, 7},
        {3, 1, 2, 3, 4, 5, 6, 7},
        {4, 1, 2, 3, 4, 5, 6, 7},
        {5, 1, 2, 3, 4, 5, 6, 7},
        {6, 1, 2, 3, 4, 5, 6, 7},
        {7, 1, 2, 3, 4, 5, 6, 7},
    };

    double T[8][8];

    image.DCT8(V, T);
    image.IDCT8(V, T);

    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            printf("%.2f ", T[i][j]);
        }        
        printf("\n");
    }

    printf("\n");
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            printf("%.2f ", V[i][j]);
        }        
        printf("\n");
    }

    // image.LoadPPM("kagaya.ppm");
    // SDLClass main(1024, 768, "Hello World");    
    // main.flip();
    // main.loop();
    
    return 0;
}
