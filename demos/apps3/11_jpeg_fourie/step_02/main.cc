#include <stdlib.h>
#include <stdio.h>

#include "sdl.class.h"
#include "jpeg.h"

int main(int argc, char* argv[]) {

    jpeg image;

    image.LoadPPM("kagaya.ppm");

    // ---
    SDLClass main(1024, 768, "Hello World");
    
    for (int i = 0; i < image.GetHeight(); i++)
    for (int j = 0; j < image.GetWidth(); j++) {

        YCR pix = image.GetYCR(j, i);    
        main.pset(j,i, image.GetRGB(pix.Y, pix.Cr, pix.Cb) );
    }

    main.flip();
    main.loop();
    // ---

    return 0;
}
