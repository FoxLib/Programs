#include "sdl.class.h"

SDLClass::SDLClass(int w, int h, const char* caption) {

    width  = w;
    height = h;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);

    sdl_screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption(caption, 0);
}

// Бесконечный цикл
void SDLClass::loop() {

    while (1) {

        while (SDL_PollEvent(& event)) {

            switch (event.type) {

                // Если нажата на крестик, то приложение будет закрыто
                case SDL_QUIT: return;

                // Нажата мышь
                case SDL_MOUSEBUTTONDOWN: break;

                // Кнопка мыши отжата
                case SDL_MOUSEBUTTONUP: break;

                // Мышь перемещена
                case SDL_MOUSEMOTION: break;

                // Нажата какая-то клавиша
                case SDL_KEYDOWN: break;

                // Вызывается по таймеру
                case SDL_USEREVENT:  break;
            }
        }

        SDL_Delay(1);
    }
}

// Нарисовать точку
void SDLClass::pset(int x, int y, uint color) {

    if (x >= 0 && y >= 0 && x < width && y < height) {
        ( (Uint32*)sdl_screen->pixels )[ x + width*y ] = color;
    }
}

// Обменять буфер
void SDLClass::flip() {
    SDL_Flip(sdl_screen);
}