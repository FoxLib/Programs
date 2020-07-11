#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct __attribute__((packed)) rgbi { unsigned char r, g, b; };
struct rgbf { float r, g, b; };

// Палитра для DOS 320x200 MODE 13h
static const int dospal[256] = {
  0x000000, 0x0000aa, 0x00aa00, 0x00aaaa, 0xaa0000, 0xaa00aa, 0xaa5500, 0xaaaaaa, // 0
  0x555555, 0x5555ff, 0x55ff55, 0x55ffff, 0xff5555, 0xff55ff, 0xffff55, 0xffffff, // 8
  0x000000, 0x141414, 0x202020, 0x2c2c2c, 0x383838, 0x454545, 0x515151, 0x616161, // 10
  0x717171, 0x828282, 0x929292, 0xa2a2a2, 0xb6b6b6, 0xcbcbcb, 0xe3e3e3, 0xffffff, // 18
  0x0000ff, 0x4100ff, 0x7d00ff, 0xbe00ff, 0xff00ff, 0xff00be, 0xff007d, 0xff0041, // 20
  0xff0000, 0xff4100, 0xff7d00, 0xffbe00, 0xffff00, 0xbeff00, 0x7dff00, 0x41ff00, // 28
  0x00ff00, 0x00ff41, 0x00ff7d, 0x00ffbe, 0x00ffff, 0x00beff, 0x007dff, 0x0041ff, // 30
  0x7d7dff, 0x9e7dff, 0xbe7dff, 0xdf7dff, 0xff7dff, 0xff7ddf, 0xff7dbe, 0xff7d9e, // 38
  0xff7d7d, 0xff9e7d, 0xffbe7d, 0xffdf7d, 0xffff7d, 0xdfff7d, 0xbeff7d, 0x9eff7d, // 40
  0x7dff7d, 0x7dff9e, 0x7dffbe, 0x7dffdf, 0x7dffff, 0x7ddfff, 0x7dbeff, 0x7d9eff, // 48
  0xb6b6ff, 0xc7b6ff, 0xdbb6ff, 0xebb6ff, 0xffb6ff, 0xffb6eb, 0xffb6db, 0xffb6c7, // 50
  0xffb6b6, 0xffc7b6, 0xffdbb6, 0xffebb6, 0xffffb6, 0xebffb6, 0xdbffb6, 0xc7ffb6, // 58
  0xb6ffb6, 0xb6ffc7, 0xb6ffdb, 0xb6ffeb, 0xb6ffff, 0xb6ebff, 0xb6dbff, 0xb6c7ff, // 60
  0x000071, 0x1c0071, 0x380071, 0x550071, 0x710071, 0x710055, 0x710038, 0x71001c, // 68
  0x710000, 0x711c00, 0x713800, 0x715500, 0x717100, 0x557100, 0x387100, 0x1c7100, // 70
  0x007100, 0x00711c, 0x007138, 0x007155, 0x007171, 0x005571, 0x003871, 0x001c71, // 78
  0x383871, 0x453871, 0x553871, 0x613871, 0x713871, 0x713861, 0x713855, 0x713845, // 80
  0x713838, 0x714538, 0x715538, 0x716138, 0x717138, 0x617138, 0x557138, 0x457138, // 88
  0x387138, 0x387145, 0x387155, 0x387161, 0x387171, 0x386171, 0x385571, 0x384571, // 90
  0x515171, 0x595171, 0x615171, 0x695171, 0x715171, 0x715169, 0x715161, 0x715159, // 98
  0x715151, 0x715951, 0x716151, 0x716951, 0x717151, 0x697151, 0x617151, 0x597151, // A0
  0x517151, 0x517159, 0x517161, 0x517169, 0x517171, 0x516971, 0x516171, 0x515971, // A8
  0x000041, 0x100041, 0x200041, 0x300041, 0x410041, 0x410030, 0x410020, 0x410010, // B0
  0x410000, 0x411000, 0x412000, 0x413000, 0x414100, 0x304100, 0x204100, 0x104100, // B8
  0x004100, 0x004110, 0x004120, 0x004130, 0x004141, 0x003041, 0x002041, 0x001041, // C0
  0x202041, 0x282041, 0x302041, 0x382041, 0x412041, 0x412038, 0x412030, 0x412028, // C8
  0x412020, 0x412820, 0x413020, 0x413820, 0x414120, 0x384120, 0x304120, 0x284120, // D0
  0x204120, 0x204128, 0x204130, 0x204138, 0x204141, 0x203841, 0x203041, 0x202841, // D8
  0x2c2c41, 0x302c41, 0x342c41, 0x3c2c41, 0x412c41, 0x412c3c, 0x412c34, 0x412c30, // E0
  0x412c2c, 0x41302c, 0x41342c, 0x413c2c, 0x41412c, 0x3c412c, 0x34412c, 0x30412c, // E8
  0x2c412c, 0x2c4130, 0x2c4134, 0x2c413c, 0x2c4141, 0x2c3c41, 0x2c3441, 0x2c3041, // F0
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000  // F8
};

class Dither {

protected:

    int width, height, depth;

    struct rgbi* canvas;
    struct rgbf* ws;
    unsigned char* indexed;

    int sdlw, sdlh;
    SDL_Event    event;
    SDL_Surface* sdl_screen;

public:

    // Загрузка файла в память
    void loadfile(const char* fn) {

        char s[256];

        FILE* fp = fopen(fn, "rb");
        if (fp == NULL) { printf("Error loading image\n"); exit(1); }

        fgets(s, 16, fp);
        fgets(s, 255, fp);
        fscanf(fp, "%d %d", &width, &height);
        fscanf(fp, "%d", &depth);
        fread(s, 1, 1, fp);

        // Выделить память и загрузить туда данные
        canvas  = (rgbi*) malloc(sizeof(rgbi) * width * height);
        ws      = (rgbf*) malloc(sizeof(rgbf) * width * height);
        indexed = (unsigned char*) malloc(width * height);

        // Загрузка в память
        fread(canvas, sizeof(rgbi), width * height, fp);
        fclose(fp);
    }

    // Запустить окно SDL
    void init() {

        SDL_Init(SDL_INIT_VIDEO);
        SDL_EnableUNICODE(1);

        sdlw = 2 * width;
        sdlh = 2 * height;
        sdl_screen = SDL_SetVideoMode(sdlw, sdlh, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        SDL_WM_SetCaption("Floyd-Steinberg", 0);
    }

    // Включить отслеживание событий
    void start() {

        while (1) {
        while (SDL_PollEvent(& event)) {
            switch (event.type) {
                case SDL_QUIT: exit(0);
            }
        }
        SDL_Delay(1);
        }
    }

    // Нарисовать точку
    void pset(int x, int y, uint color) {

        if (x >= 0 && y >= 0 && x < sdlw && y < sdlh) {
            ( (Uint32*)sdl_screen->pixels )[ x + sdlw*y ] = color;
        }
    }

    // Обменять буфер
    void flip() { SDL_Flip(sdl_screen); }

    // Вывод изображения
    void repaint() {

        for (int i = 0; i < 2*height; i++)
        for (int j = 0; j < 2*width; j++) {

            int p = width*(i>>1) + (j>>1);
            pset(j, i, canvas[p].r*65536 + canvas[p].g*256 + canvas[p].b);
        }

        flip();
    }

    struct rgbf get_dos_col(int i) {

        // Сравниваемый цвет
        float rc = (float)((dospal[i] >> 16));
        float gc = (float)((dospal[i] >>  8) & 0xff);
        float bc = (float)((dospal[i])       & 0xff);

        return {rc, gc, bc};
    }

    // Процедура поиска ближайшего цвета
    int search_nearest(struct rgbf color) {

        float dist_min = 3*pow(256, 2);
        int   dist_col = 0;

        // dospal
        for (int i = 0; i < 256; i++) {

            // Сравниваемый цвет
            struct rgbf cl = get_dos_col(i);

            // Дистанция между точками (r'-r)^2 + (g'-g)^2 + (b'-b)^2
            float dist = pow(color.r - cl.r, 2) +
                         pow(color.g - cl.g, 2) +
                         pow(color.b - cl.b, 2);

            if (dist < dist_min) {

                dist_min = dist;
                dist_col = i;
            }
        }

        return dist_col;
    }

    // Процедура дизеринга
    void dither(int dith) {

        int i, j;

        // Копировать целочисленные точки в не целочисленные
        for (i = 0; i < height; i++)
        for (j = 0; j < width; j++) {

            int p = j + width*i;
            ws[p] = { (float) canvas[p].r, (float) canvas[p].g, (float) canvas[p].b };
        }

        // Выполнить дизеринг
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {

                int p = j + width*i;

                // Старые цвета
                struct rgbf old = ws[p];

                // Поиск ближайшего цвета из палитры
                int color_id = search_nearest(old);

                // Заменить на новый цвет (из палитры)
                struct rgbf ncl = get_dos_col(color_id);

                // Запись обратно нового цвета
                ws[p] = ncl;

                // Полученный индекс цвета
                indexed[p] = color_id;

                // Учесть ошибки квантования
                if (dith) {

                    // Вычисляем ошибку квантования
                    struct rgbf quant;

                    quant.r = old.r - ncl.r;
                    quant.g = old.g - ncl.g;
                    quant.b = old.b - ncl.b;
                    
                    //   x 7
                    // 3 5 1

                    // [+1, +0] 7/16
                    if (j + 1 < width) {

                        ws[p + 1].r += (quant.r * 7.0/16.0);
                        ws[p + 1].g += (quant.g * 7.0/16.0);
                        ws[p + 1].b += (quant.b * 7.0/16.0);
                    }

                    // [-1, +1] 3/16
                    if (j - 1 >= 0 && i + 1 < height) {

                        ws[p - 1 + width].r += (quant.r * 3.0/16.0);
                        ws[p - 1 + width].g += (quant.g * 3.0/16.0);
                        ws[p - 1 + width].b += (quant.b * 3.0/16.0);
                    }

                    // [+0, +1] 5/16
                    if (i + 1 < height) {

                        ws[p + 0 + width].r += (quant.r * 5.0/16.0);
                        ws[p + 0 + width].g += (quant.g * 5.0/16.0);
                        ws[p + 0 + width].b += (quant.b * 5.0/16.0);
                    }

                    // [+1, +1] 1/16
                    if (j + 1 < width && i + 1 < height) {

                        ws[p + 1 + width].r += (quant.r * 1.0/16.0);
                        ws[p + 1 + width].g += (quant.g * 1.0/16.0);
                        ws[p + 1 + width].b += (quant.b * 1.0/16.0);
                    }
                }
            }
        }

        // Копировать целочисленные точки в не целочисленные
        for (i = 0; i < height; i++)
        for (j = 0; j < width; j++) {

            int p = j + width*i;
            struct rgbf c = ws[p];
            canvas[width*i + j] = { (unsigned char)c.r, (unsigned char)c.g, (unsigned char)c.b };
        }
    }

    void write(const char* fn) {

        FILE* fp = fopen(fn, "wb");
        fwrite(indexed, 1, width*height, fp);
        fclose(fp);
    }
};

int main(int argc, char** argv) {

    Dither dc;

    dc.loadfile("space.ppm");
    dc.init();
    dc.repaint();
    dc.dither(1);
    dc.write("idx.bin");
    dc.repaint();
    dc.start();

    return 0;
}
