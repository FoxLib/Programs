#include "SDL.h"
#include <sys/timeb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

class screen {
protected:

    int width, height;

    SDL_Event    event;
    SDL_Surface* sdl_screen;
    unsigned int ms_time_diff;
    struct timeb ms_clock;
    unsigned int ms_clock_old;
    unsigned int event_mask;

public:

    int mx;
    int my;
    int mk;
    int kb_code;
    int kb_mod;

    // Конструкторы с параметрами
    screen(int w, int h) { init(w, h, "Application"); }
    ~screen() { }

    // Запуск окна
    void init(int w, int h, const char* caption) {

        width  = w;
        height = h;
        mx = my = mk = 0;
        ms_time_diff = 20;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        SDL_EnableUNICODE(1);

        sdl_screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        SDL_WM_SetCaption(caption, 0);
        SDL_EnableKeyRepeat(500, 30);
    }

    // Установка точки
    void pset(int x, int y, uint color) {

        if (x >= 0 && y >= 0 && x < width && y < height) {
            ( (Uint32*)sdl_screen->pixels )[ x + width*y ] = color;
        }
    }

    // Нарисовать линию
    void line(int x1, int y1, int x2, int y2, int color) {

        int deltax = abs(x2 - x1);
        int deltay = abs(y2 - y1);
        int signx  = x1 < x2 ? 1 : -1;
        int signy  = y1 < y2 ? 1 : -1;

        int error = deltax - deltay;
        int error2;

        pset(x2, y2, color);
        while (x1 != x2 || y1 != y2)
        {
            pset(x1, y1, color);
            error2 = error * 2;

            if (error2 > -deltay) {
                error -= deltay;
                x1 += signx;
            }

            if (error2 < deltax) {
                error += deltax;
                y1 += signy;
            }
        }
    }

    // Очистить весь экран
    void cls(uint32_t color) {

        for (int i = 0; i < width * height; i++)
            ( (Uint32*)sdl_screen->pixels )[ i ] = color;
    }

    // Обмен видео-буфера
    void flip() { SDL_Flip(sdl_screen); }

    // Получение событий
    int poll() {

        int tmp;
        event_mask = 1; // 1=IDLE

        // Регистрация событий
        while (SDL_PollEvent(& event)) {
            switch (event.type) {

                case SDL_QUIT: return 0;

                // Перемещение мыши
                case SDL_MOUSEMOTION:

                    event_mask |= 4;
                    mx = event.motion.x;
                    my = event.motion.y;
                    break;

                // Нажата мышь
                case SDL_MOUSEBUTTONDOWN:

                    event_mask |= (4 | 8);
                    mx = event.motion.x;
                    my = event.motion.y;
                    tmp = event.motion.state;

                    if (tmp == SDL_BUTTON_LEFT)        mk |= 1;
                    else if (tmp == SDL_BUTTON_RIGHT)  mk |= 2;
                    else if (tmp == SDL_BUTTON_MIDDLE) mk |= 4;
                    break;

                // Мышь отпущена
                case SDL_MOUSEBUTTONUP:

                    event_mask |= (4 | 16);
                    mx = event.motion.x;
                    my = event.motion.y;
                    tmp = event.motion.state;

                    if (tmp == SDL_BUTTON_LEFT)        mk &= ~1;
                    else if (tmp == SDL_BUTTON_RIGHT)  mk &= ~2;
                    else if (tmp == SDL_BUTTON_MIDDLE) mk &= ~4;
                    break;

                // https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkeysym.html
                case SDL_KEYDOWN:

                    event_mask |= 32;
                    kb_code = event.key.keysym.unicode;
                    kb_mod  = event.key.keysym.mod;
                    if (kb_code == 0) kb_code = 0x100 + event.key.keysym.sym;
                    break;

                case SDL_KEYUP:

                    event_mask |= 64;
                    kb_code = event.key.keysym.unicode;
                    kb_mod  = event.key.keysym.mod;
                    if (kb_code == 0) kb_code = 0x100 + event.key.keysym.sym;
                    break;
            }
        }

        // Вычисление разности времени
        ftime(&ms_clock);
        int time_curr = ms_clock.millitm;
        int time_diff = time_curr - ms_clock_old;
        if (time_diff < 0) time_diff += 1000;

        // Если прошло 20 мс
        if (time_diff >= ms_time_diff) {

            SDL_Flip(sdl_screen);
            ms_clock_old = time_curr;
            event_mask |= 2; // 2=TIMER
        }

        SDL_Delay(1);
        return event_mask;
    }

    // Определение того, какое событие наступило
    int etimer() { return !!(event_mask & 2); }
    int emouse() { return !!(event_mask & 4); }

    // Какая клавиша мыши была нажата: id => 1 левая, 2 правая, 3 средняя
    int mdn(int id) { return ((event_mask &  8) && (mk & id)); }
    int mup(int id) { return ((event_mask & 16) && (mk & id)); }

    // Обнаружено нажатие клавиши
    int kdn() { return !!(event_mask & 32); }
    int kup() { return !!(event_mask & 64); }
};
