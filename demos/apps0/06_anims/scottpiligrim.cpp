#include <SDL/SDL.h>
#include <math.h>

// Объявления
// ----------------------------
#include "main.h"
#include "font/font8x8_basic.h"
#include "font/font8x16.h"

// Библиотеки
// ----------------------------
#include "jpeg.cpp"
#include "ppm.cpp"
#include "image.cpp"
#include "audio.cpp"
#include "studio.cpp"

// Компоненты
// ----------------------------
#include "com/mandel.cpp"
#include "com/figures.cpp"

// Команда для сборки видео
// ffmpeg -r 30 -i build/%06d.jpeg -c:v libx264 -s 1920x1080 -pix_fmt yuv420p test.mp4

// ----------------------------
studio app(960, 540);
// ----------------------------

int main(int argc, char* argv[]) {

    image img(1920, 1080);

    image sp(new ppm("rsrc/scottpilgrim.ppm"));
    image bg(new ppm("rsrc/bgc.ppm"));

    sp.transparent(0xD200FF, 0);

    int pl[8] = { 4,  120, 226, 334, 432, 538, 650, 760 };

    int x = 0;
    int y = 1080 - 120;

    for (int i = 0; i < 1000; i++) {
                
        img.cls(0x808080);
        img.draw(&bg, -(float)x / 15.0, 0);
        img.draw(&sp, x, y, pl[(i>>2) % 8], 10, 102, 128);

        app.update(& img);
        app.events();

        x += 8;

        app.savejpeg(&img, i);

        if (x > 1920) break;
    }
    
    return 0;
}

