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
#include "image.cpp"
#include "audio.cpp"
#include "studio.cpp"

// Компоненты
// ----------------------------
#include "com/figures.cpp"

// Команда для сборки видео
// ffmpeg -r 30 -i build/%06d.jpeg -c:v libx264 -s 1920x1080 -pix_fmt yuv420p test.mp4

// ----------------------------
studio app(960, 540);
// ----------------------------

struct xy {

    float x, y;
    float sy;
    int   ac;
    int   cnt;
    int   size;     // Размер
    int   fig;      // Номер фигуры

};

int main(int argc, char* argv[]) {

    figures fig;                // рисование фигур
    image   img(1920, 1080);    // основной экран
    image   disk[32];           // дискеты (разной величины)

    // Нагенерировать дискет
    disk[0].create(420, 420); disk[0].cls(0xffffff);
    disk[1].create(256, 256); disk[1].cls(0xffffff);

    fig.diskette(&disk[0], 0, 0, 420.0);
    fig.diskette(&disk[1], 0, 0, 256.0);

    int top = 0;
    int maxdisk = 1366;
    int id = 0;

    struct xy D[ maxdisk ];

    while (1) {

        img.cls(0x80);

        for (int i = 0; i < 1080; i++) {

            float k = (float)i / 1080.0;
            
            int s = k*100.0;
            int v = 128.0;
            uint color; // = img.hsv2rgb(200.0 + k*40.0, s, v);

            if (i < 540)
                color = img.hsv2rgb(200.0 + k*40.0, s, v);
            else
                color = img.hsv2rgb(230.0 - k*40.0, s, v);
            
            img.line(0, i, 1920, i, color);
        }

        int actived = 0;
        for (int i = 0; i < top; i++) {

            // Пока дискета активна
            if (D[i].ac) {
                
                img.draw(&disk[ D[i].fig ], D[i].x, D[i].y);

                // 1 удар об пол
                if (D[i].cnt < 1) {
                
                    if (D[i].y + D[i].size > 1080.0) {                
                        D[i].sy = -D[i].sy * 0.5;
                        D[i].cnt++;

                        printf("%d %f\n", D[i].cnt, D[i].y);
                    }
                }

                D[i].y  += D[i].sy;
                D[i].sy += 0.25;

                // Выключение
                if (D[i].y > 1280) {
                    D[i].ac = 0;
                }

                actived++;
            }
        }

        // Добавить новые диски
        if (((rand() % 10) == 0) && top < maxdisk) {

            int fg = rand() % 2;
            int ds = fg == 0 ? 420 : 256;
            
            D[top].x      = rand() % 1920;
            D[top].y      = -ds;
            D[top].sy     = 0;
            D[top].ac     = 1;
            D[top].cnt    = 0;
            D[top].fig    = fg;
            D[top].size   = ds;

            top++;
        }

        char stat[256]; sprintf(stat, "%d disk left, %d on screen, %d mb of 1gb installed", maxdisk - top, actived, (760*top)>>10);
        img.font(FONT8x16, 8, 1050, stat, 0xffffff);
        
        app.update(&img);
        app.events();

        if (actived == 0 && top >= maxdisk)
            break;

        app.savejpeg(&img, id++);
    }
    
    return 0;
}

