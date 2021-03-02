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
// ffmpeg -r 60 -i build/%06d.jpeg -c:v libx264 -s 1920x1080 -pix_fmt yuv420p test.mp4

// ----------------------------
studio app(960, 540);
// ----------------------------

int main(int argc, char* argv[]) {

    image img(960, 540);

    // Загрузка соника
    image sonic(new ppm("rsrc/sonic.ppm"));
    image bgsnc(new jpeg("rsrc/bgsonic.jpg"));
    
    sonic.transparent(0x008080, 16);

    int i, j;
    int hz = 0;

    int maxland = 16384;
    float land[ maxland ];

    int sonicpos[16][2] = {
        {0,   54},
        {54,  56},
        {110, 68},
        {178, 60},
        {238, 54},
        {292, 60},
        {352, 72},
        {424, 62},
        {486, 58},
        {544, 66},
        {610, 80},
        {690, 70},
        {760, 58},
        {818, 68},
        {886, 80},
        {966, 80},
    };

    int k = 0;
    int left = 0;
    int x = 10, y;
    
    float ysonic = 1000.0;
    float sonic_speed = 0.0;
    
    uint color;

    srand (3);

    // Генерация ландшафта
    for (i = 0; i < maxland; i++) {

        hz += (rand()%5) - 2;

        if (hz > 75) hz = 75;
        land[i] = 400.0 + (float)hz;
    }

    // Сглаживание
    for (j = 0; j < 16; j++)
    for (i = 0; i < maxland; i++) {
        land[i] = (land[(maxland+i-1)%maxland] + land[i] + land[(i+1)%maxland]) / 3.0;
    }

    while (1) {
    
        img.cls(0x000080);

        // Фон, который "ездит"    
        img.draw(&bgsnc, (int)(-(float)left / 32.0) % 960, -148,     0, 0, 1920, 315);
        img.draw(&bgsnc, (int)(-(float)left / 16.0) % 960, -148+315, 0, 315, 1920, 180);
        
        img.draw(&bgsnc, (int)(-(float)left / 12.0) % 960, -148+480, 0, 480, 1920, 105);
        img.draw(&bgsnc, (int)(-(float)left / 8.0) % 960,  -148+585, 0, 585, 1920, 200);

        // Отрисовать ландшафт
        for (int j = 0; j < 960; j++) {

            int ys = land[(left + j) % maxland];
            for (int i = ys; i < 540; i++) {

                // Сама трава
                if (i < ys + 32) {

                    int yi = (i-ys)>>3;
                    switch ((((j + left)>>2)^((i-ys)>>3)) & 1) {

                        case 0: color = yi & 2 ? 0x44aa00 : 0x88ee00; break;
                        case 1: color = yi & 2 ? 0x006600 : 0x44aa00; break;
                    }                
                }
                // Тень от травы
                else if (i < ys + 64) {

                    color = ((i>>4)^((j + left)>>4)) & 1 ? 0x662200 : 0x22000;
                
                }
                // Земля
                else {
                    color = ((i>>4)^((j + left)>>4)) & 1 ? 0xcc6600 : 0x662200;
                }    

                img.pset(j, i, color);
            }
        }

        // Положение соника
        y = land[ (x + left) % maxland ] - 60;

        ysonic += sonic_speed;

        // Не дать упасть ниже пола
        if (ysonic > y) { ysonic = y; sonic_speed = 0; } else sonic_speed += 0.1;

        int kp = (k>>3) % 8;
        img.draw(&sonic, x, ysonic,  sonicpos[kp][0],0, sonicpos[kp][1], 70 );

        if (x < 480) {            
            x += 4;
        } else {
            left += 4;
        }
        
        app.savejpeg(&img, k);

        char dbg[256]; sprintf(dbg, "%d", k); img.font(FONT8x16, 0, 0, dbg, 0xffffff);
        app.update(& img);
        app.events();
        
        k++;
    }
    
    return 0;
}

