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

    // Конструкторы с параметрами
    screen(int w, int h) { init(w, h, "Application"); }

    // Запуск окна
    void init(int w, int h, const char* caption) {

        width  = w;
        height = h;
        ms_time_diff = 20;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        SDL_EnableUNICODE(1);

        sdl_screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
        SDL_WM_SetCaption(caption, 0);
    }

    // Установка точки
    void pset(int x, int y, uint color) {

        if (x >= 0 && y >= 0 && x < width && y < height) {
            ( (Uint32*)sdl_screen->pixels )[ x + width*y ] = color;
        }
    }

    // Обмен видео-буфера
    void flip() { SDL_Flip(sdl_screen); }

    // Получение событий
    int poll() {

        event_mask = 1; // 1=IDLE

        // Регистрация событий
        while (SDL_PollEvent(& event)) {
            switch (event.type) {
                case SDL_QUIT: return 0;
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
};
